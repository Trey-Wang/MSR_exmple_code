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
/**        \file  Sd.c
 *        \brief  Service Discovery source file
 *
 *      \details  Vector static code implementation for AUTOSAR Service Discovery (SD) module.
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

#define SD_SOURCE

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use an include guard. */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Sd_Priv.h"
#if(SD_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "BswM_Sd.h"
#include "IpBase.h"
#if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Sd header file. */
#if(   (SD_SW_MAJOR_VERSION != (0x05u)) \
   ||  (SD_SW_MINOR_VERSION != (0x04u)) \
   ||  (SD_SW_PATCH_VERSION != (0x00u)) )
# error "Vendor specific version numbers of Sd.c and Sd.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if !defined (SD_LOCAL)                                                                                                 /* COV_SD_COMPATIBILITY */
# define SD_LOCAL static
#endif

#if !defined (SD_LOCAL_INLINE)                                                                                          /* COV_SD_COMPATIBILITY */
# define SD_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define SD_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

SD_LOCAL VAR(Sd_StateType, SD_VAR_ZERO_INIT) Sd_State = SD_UNINIT;

#define SD_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SD_START_SEC_VAR_ZERO_INIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SD_STOP_SEC_VAR_ZERO_INIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

SD_LOCAL VAR(Sd_ExtendedTimeType, SD_VAR_ZERO_INIT) Sd_TimeMsSinceStart = {0u, 0u};

#define SD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define SD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

P2CONST(Sd_ConfigType, SD_VAR_ZERO_INIT, SD_PBCFG) Sd_ConfigDataPtr = NULL_PTR;

#define SD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SD_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ClientServiceSetStateRequested()
 *********************************************************************************************************************/
/*! \brief          Configures the client as requested.
 *  \details        -
 *  \param[in]      ClientIdx   Index of the client.
 *                              [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ClientServiceSetStateRequested(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ClientServiceSetStateReleased()
 *********************************************************************************************************************/
/*! \brief          Configures the client as released.
 *  \details        -
 *  \param[in]      ClientIdx   Index of the client.
 *                              [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ClientServiceSetStateReleased(
  Sd_SizeOfClientType ClientIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

/**********************************************************************************************************************
 *  Sd_Init_General()
 *********************************************************************************************************************/
/*! \brief          Initializes all general variable data.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_General(void);

/**********************************************************************************************************************
 *  Sd_Init_Instance()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the instances.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Instance(void);

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_Server()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the servers.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Server(void);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_Client()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the clients.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Client(void);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ClientCeg()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the consumed eventgroups.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ClientCeg(void);
#endif

/**********************************************************************************************************************
 *  Sd_Init_ReInitialize()
 *********************************************************************************************************************/
/*! \brief          Initializes all communication state dependent data.
 *  \details        Re-initializes data if the module state was changed by a call of Sd_Init() or
 *                  Sd_LocalIpAddrAssignmentChg().
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitialize(
  Sd_SizeOfInstanceType InstanceIdx);

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ReInitializeServers()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all servers of the given instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeServers(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_Process()
 *********************************************************************************************************************/
/*! \brief          Executes the server state machine.
 *  \details        Is called by the MainFunction and processes the state of the server.
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Process(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessNotReady()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the down phases.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessNotReady(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessInitialWait()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the initial wait phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessInitialWait(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessRepetition()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the repetition phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessRepetition(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessMain()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the main phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessMain(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessEventHandlerStateMachine()
 *********************************************************************************************************************/
/*! \brief          Handles the event handler state machines of the given server.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessEventHandlerStateMachine(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerClients()
 *********************************************************************************************************************/
/*! \brief          Updates the state of all subscribed clients of an event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerClients(
  Sd_SizeOfEventHandlerType EventHandlerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchMulticast()
 *********************************************************************************************************************/
/*! \brief          Switches the event handler from unicast to multicast transmission mode.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchMulticast(
  Sd_SizeOfEventHandlerType EventHandlerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchUnicast()
 *********************************************************************************************************************/
/*! \brief          Switches the event handler from multicast to unicast transmission mode.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchUnicast(
  Sd_SizeOfEventHandlerType EventHandlerIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_DisableRouting()
 *********************************************************************************************************************/
/*! \brief          Disables routing for server methods and events.
 *  \details        Disables all routing groups of the event handler and resets the activation count for all server
 *                  socket connections.
 *  \param[in]      EventHandlerIdx         Index of the event handler.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_DisableRouting(
  Sd_SizeOfEventHandlerType EventHandlerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Processes the attached server Entry Option.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Index of the entry which shall be processed.
 *  \param[in]      InstanceIdx             Index of instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntry(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntryFind()
 *********************************************************************************************************************/
/*! \brief          Reads and processes the attached FindService entry.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntryFind(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribe()
 *********************************************************************************************************************/
/*! \brief          Reads and processes the attached SubscribeEventgroup entry.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntrySubscribe(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_SearchEventHandler()
 *********************************************************************************************************************/
/*! \brief          Searches the event handler with the matching ID.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \return         ID of the matching event handler.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfEventHandlerType, SD_CODE) Sd_RxServer_SearchEventHandler(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*! \brief          Triggers a user callout in order to validate the received configuration options.
 *  \details        -
 *  \param[in]      ServerIdx                   Corresponding server index.
 *                                              [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      RxPduId                     Receiving PDU-ID.
 *  \param[in]      RxEntryType                 Type of received entry.
 *  \param[in]      RxConfigOptionPtrArrayPtr   Configured configuration option pointer list.
 *  \return         E_OK                        Message was accepted and shall be processed.
 *  \return         E_NOT_OK                    Message was not accepted and shall be rejected/ignored.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_CheckConfigOptionByCbk(
  Sd_SizeOfServerType ServerIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr);
# endif
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionArray()
 *********************************************************************************************************************/
/*! \brief          Processes the attached option array.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option array is valid.
 *  \return         E_NOT_OK                Invalid option array, this SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionArray(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionRun()
 *********************************************************************************************************************/
/*! \brief          Processes the attached server options run.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in]      NrOfOptions             Number of successive attached Options in the run.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx <= Sd_GetSizeOfEventHandler()]
 *                                          [invalid: Sd_GetSizeOfEventHandler()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid Option, this SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionRun(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribe()
 *********************************************************************************************************************/
/*! \brief          Processes the attached (Stop)SubscribeEventgroup option.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid Option, this SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribe(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribeCheckSubnet()
 *********************************************************************************************************************/
/*! \brief          Checks if the attached endpoint options are within the same subnet.
 *  \details        Endpoints which are not within the configured subnet are ignored during processing of the
 *                  SubscribeEventgroup message.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Local address could be read.
 *  \return         E_NOT_OK                Failed to read the local address.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribeCheckSubnet(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeSaveParameters()
 *********************************************************************************************************************/
/*! \brief          Save all mandatory parameters of the received message for further processing.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr  Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      ClientListIdx           Client index which triggered the event.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[out]     AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid Option, this SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeSaveParameters(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all pending server events.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Process(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_Update()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the server and sets timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      ServerIdx               Server index to set the value and timer event.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      NewState                New state of the server service state machine.
 *  \param[in]      ResetTimeToTriggerSend  Indicates if the state machine has to be executed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Update(
  Sd_SizeOfServerType ServerIdx,
  Sd_ServerStateMachineType NewState,
  boolean ResetTimeToTriggerSend);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerStateMachine()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the event handler and sets a timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Event handler to set the value and timer event.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      NewState                New state of the event handler state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerStateMachine(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_EventHandlerStateMachineType NewState);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerRequestState()
 *********************************************************************************************************************/
/*! \brief          Updates the request state of the event handler.
 *  \details        Updates the request state of the event handler, triggers a state machine execution and informs the
 *                  BswM about the state change.
 *  \param[in]      EventHandlerIdx         Event handler to set the value and timer event.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      NewState                New state of the event handler state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerRequestState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_EventHandlerCurrentStateType NewState); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_AddEventToRingBuffer()
 *********************************************************************************************************************/
/*! \brief          Adds an received event to the ring buffer.
 *  \details        -
 *  \param[in]      InstanceIdx             Relevant instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      ClientListIdx           Client index which triggered the event.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[in]      Event                   Event to process.
 *  \return         E_OK                    Event successfully saved.
 *  \return         E_NOT_OK                Not able to save event for further processing.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_AddEventToRingBuffer(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  Sd_ServerEventgroupEventType Event);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_ReadFirst()
 *********************************************************************************************************************/
/*! \brief          Reads the first event from the ring buffer.
 *  \details        The event will NOT be removed from the list.
 *  \param[in]      InstanceIdx             Corresponding instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[out]     EventIdxPtr             Received event index.
 *                                          [range: RxEventIdx <= Sd_GetSizeOfRxEvent()]
 *                                          [invalid: Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Event read and ready to process.
 *  \return         E_NOT_OK                No pending event to process.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_ReadFirst(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2VAR(Sd_SizeOfRxEventType) EventIdxPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_RemoveFirst()
 *********************************************************************************************************************/
/*! \brief          Removes the first element of the ring buffer.
 *  \details        Handles also wrap around of the buffer and removes the saved endpoint information if not used by
 *                  other elements.
 *  \param[in]      InstanceIdx             Corresponding instance index where the first event should be removed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_RemoveFirst(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_SaveEndpoint()
 *********************************************************************************************************************/
/*! \brief          Searches for an unused slot and saves the address pointed by InAddrPtr.
 *  \details        -
 *  \param[in]      InstanceIdx                 Index of instance which handles this address.
 *                                              [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      InAddrPtr                   Pointer to address which shall be saved.
 *  \param[in]      Type                        Type of the endpoint address.
 *  \param[out]     OutEndpointStorageIdxPtr    Reference to the saved endpoint index.
 *                                              [range: OutEndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()()]
 *                                              [invalid: Sd_GetSizeOfEndpointStorage()()]
 *  \return         E_OK                        Endpoint address saved.
 *  \return         E_NOT_OK                    Unable to save the endpoint address.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_SaveEndpoint(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr,
  Sd_EndpointType Type,
  SD_P2VAR(Sd_SizeOfEndpointStorageType) OutEndpointStorageIdxPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_RemoveEndpoint()
 *********************************************************************************************************************/
/*! \brief          Removes the saved endpoint address.
 *  \details        -
 *  \param[in,out]  EndpointStorageIdx      Index of the saved endpoint to be removed.
 *                                          [range: EndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()()]
 *                                          [invalid: Sd_GetSizeOfEndpointStorage()()]
 *  \return         TRUE                    Endpoint is not referenced by an additional client.
 *  \return         FALSE                   Endpoint is referenced by another client and still required.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_RemoveEndpoint(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_Subscription()
 *********************************************************************************************************************/
/*! \brief          Processes the subscription of a remote client.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Subscription(
  Sd_SizeOfRxEventType RxEventIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitial()
 *********************************************************************************************************************/
/*! \brief          Processes the initial subscription of a remote client.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Remote client successfully subscribed.
 *  \return         E_NOT_OK                Not able to subscribe remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitial(
  Sd_SizeOfRxEventType RxEventIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialTcp()
 *********************************************************************************************************************/
/*! \brief          Configures the remote client for TCP.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[out]     UseTriggerTransmitPtr   Indicates if a TCP event has to be triggered.
 *  \return         E_OK                    Remote client successfully configured.
 *  \return         E_NOT_OK                Not able to configure remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialTcp(
  Sd_SizeOfRxEventType RxEventIdx,
  SD_P2VAR(boolean) UseTriggerTransmitPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialUdp()
 *********************************************************************************************************************/
/*! \brief          Configures the remote client for UDP.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[out]     UseTriggerTransmitPtr   Indicates if a UDP event has to be triggered.
 *  \return         E_OK                    Remote client successfully configured.
 *  \return         E_NOT_OK                Not able to configure remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialUdp(
  Sd_SizeOfRxEventType RxEventIdx,
  SD_P2VAR(boolean) UseTriggerTransmitPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionRenew()
 *********************************************************************************************************************/
/*! \brief          Processes the renewal of a remote client subscription.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Remote client successfully subscribed.
 *  \return         E_NOT_OK                Not able to subscribe remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionRenew(
  Sd_SizeOfRxEventType RxEventIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_AddClientToList()
 *********************************************************************************************************************/
/*! \brief          Adds a client to the client list of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      AddressStorageIdx       Remote IP address index of the client.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \return         Index of the saved client.
 *                  [invalid: Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_AddClientToList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientFromList()
 *********************************************************************************************************************/
/*! \brief          Removes a client from the client list of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      ClientListIdx           Client index which should be removed.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientFromList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_ReleaseEventHandlers()
 *********************************************************************************************************************/
/*! \brief          Releases all eventhandlers of the server.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ReleaseEventHandlers(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_CloseAllTcpSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all TCP socket connections of the server.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseAllTcpSoCons(
  Sd_SizeOfServerType ServerIdx,
  boolean Abort);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_CloseAllUdpSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all UDP socket connections of the server.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseAllUdpSoCons(
  Sd_SizeOfServerType ServerIdx,
  boolean Abort);
#endif

#if(SD_METHODIDXOFSERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_DisableMethodRouting()
 *********************************************************************************************************************/
/*! \brief          Disables the routing of provided methods.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_DisableMethodRouting(
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_LocalIpAddrAssignmentChgReset()
 *********************************************************************************************************************/
/*! \brief          Resets all dynamic server data in the case of a local IP address assignment change.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which has to be reset.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_GetActivationMapIdx()
 *********************************************************************************************************************/
/*! \brief          Returns the activation map index for the requested SoConId.
 *  \details        The activation map allows it to handle multiple instances of a remote client.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \return         Found SoConActivationMap index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfSoConActivationMapType, SD_CODE) Sd_UtilServer_GetActivationMapIdx(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SoAd_SoConIdType SoConId);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_IncreaseActivationCount()
 *********************************************************************************************************************/
/*! \brief          Increases the activation count for the requested SoConId.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_IncreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SoAd_SoConIdType SoConId);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_FindClientInList()
 *********************************************************************************************************************/
/*! \brief          Searches for a client in the client-list of the event handler.
 *  \details
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      AddressStorageIdx       Remote address index of the client.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      AttachedOptionsPtr      Structure containing all attached options.
 *                                          [invalid: NULL_PTR if a reboot of the remote service was detected.]
 *  \return         Found client index.
 *                  [invalid: Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_FindClientInList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Checks the attached endpoint options.
 *  \details        Compares the received endpoint address with the configured one.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      ClientListIdx           Corresponding client index.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[in]      AttachedOptionsPtr      Structure containing all attached options.
 *  \return         TRUE                    Attached endpoint options are valid.
 *  \return         FALSE                   Attached endpoint options are invalid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOptions(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Checks the attached UDP or TCP endpoint option.
 *  \details        Checks if the configured remote address is equivalent to the received endpoint address.
 *  \param[in]      SoConId                 Configured socket connection ID.
 *  \param[in]      EndpointStorageIdx      Index of the saved endpoint.
 *                                          [range: EndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()()]
 *                                          [invalid: Sd_GetSizeOfEndpointStorage()()]
 *  \param[in]      EndpointPtr             UDP or TCP endpoint address of the entry.
 *  \return         TRUE                    Attached endpoint option is valid.
 *  \return         FALSE                   Attached endpoint option is invalid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOption(
  SoAd_SoConIdType SoConId,
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  SD_P2CONST(Sd_SockAddrContainerType) EndpointPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all pending trigger transmit requests.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Process(void);
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Add()
 *********************************************************************************************************************/
/*! \brief          Adds an trigger transmit request to the ring buffer.
 *  \details        The trigger transmit request will be processed during the next main function.
 *  \param[in]      RxEventIdx              Event index which caused the trigger transmit.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Information if the event could be saved.
 *  \return         E_NOT_OK                Not able to save trigger transmit request.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_TriggerTransmit_Add(
  Sd_SizeOfRxEventType RxEventIdx);
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_ReadFirst()
 *********************************************************************************************************************/
/*! \brief          Reads the first event from the trigger transmit ring buffer.
 *  \details        The event will NOT be removed from the buffer.
 *  \param[out]     TriggerTransmitIdxPtr   Trigger transmit index.
 *  \return         E_OK                    Event successfully read and ready for processing.
 *  \return         E_NOT_OK                No more pending trigger transmit events.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_TriggerTransmit_ReadFirst(
  SD_P2VAR(Sd_SizeOfTriggerTransmitType) TriggerTransmitIdxPtr);
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_RemoveFirst()
 *********************************************************************************************************************/
/*! \brief          Removes the first element from the ring buffer.
 *  \details        -
 *  \pre            Ring buffer has to contain at least one element.
 *  \context        TASK
 *  \reentrant      FALSE
  *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_RemoveFirst(void);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ReInitializeClients()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all clients of the given instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeClients(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Process()
 *********************************************************************************************************************/
/*! \brief          Processes the state of the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Process(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStateMachine()
 *********************************************************************************************************************/
/*! \brief          Processes the state machine of the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStateMachine(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_HandleReboot()
 *********************************************************************************************************************/
/*! \brief          Check if a reboot of the remote server was detected and handle state accordingly.
 *  \details        -
 *  \param[in]      ClientIdx               Client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleReboot(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_HandleRequestState()
 *********************************************************************************************************************/
/*! \brief          Check if the client is still requested and handle the request state accordingly.
 *  \details        -
 *  \param[in]      ClientIdx               Client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleRequestState(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the initial state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessInitial(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequested()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the not requested state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequested(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveNotRequested()
 *********************************************************************************************************************/
/*! \brief          Checks if the not requested state can be left and handles the state change.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveNotRequested(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessRequestedNotReady()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the not ready state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessRequestedNotReady(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the searching initial wait state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingInitial(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingRepetition()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the searching repetition state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingRepetition(
  Sd_SizeOfClientType ClientIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearching()
 *********************************************************************************************************************/
/*! \brief          Checks if the searching state can be left and triggers the state change.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearching(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearchingRcvOffer()
 *********************************************************************************************************************/
/*! \brief          Handles the reception of an OfferService message in the searching phase.
 *  \details        -
 *  \param[in]      ClientIdx               Client index which received the OfferService message.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearchingRcvOffer(
  Sd_SizeOfClientType ClientIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReady()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the ready state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReady(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReadyEventReceived()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService or StopOfferService message.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReadyEventReceived(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStopped()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the stopped state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStopped(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCeg()
 *********************************************************************************************************************/
/*! \brief          Handles all consumed eventgroup state maches of the client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCeg(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegReset()
 *********************************************************************************************************************/
/*! \brief          Resets the consumed eventgroup if the client service is not ready anymore.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegReset(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegUnsubscribed()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while being unsubscribed.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegUnsubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSent()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while waiting for a response.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSent(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxAck()
 *********************************************************************************************************************/
/*! \brief          Processes the received SubscribeEventgroupAck message.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxAck(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxOffer()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService message while waiting for the SubscribeEventgroupAck message.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxOffer(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegSubscribed()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while being Subscribed.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegSubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_METHODIDXOFCLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Configure()
 *********************************************************************************************************************/
/*! \brief          Configures the client on service level.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_Configure(
  Sd_SizeOfClientType ClientIdx);
# endif
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_METHODIDXOFCLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ConfigureMethod()
 *********************************************************************************************************************/
/*! \brief          Configures the socket connection and routing group for the client service method.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in,out]  MethodSoConIdPtr        Socket connection IF of the corresponding method.
 *  \param[in]      ServiceStartSoConId     Socket connection ID used by the service.
 *  \param[in]      ServerEndpointAddrPtr   Server endpoint address.
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ConfigureMethod(
  Sd_SizeOfClientType ClientIdx,
  SD_P2VAR(SoAd_SoConIdType) MethodSoConIdPtr,
  SoAd_SoConIdType ServiceStartSoConId,
  SD_P2CONST(Sd_SockAddrContainerType) ServerEndpointAddrPtr);
# endif
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Unconfigure()
 *********************************************************************************************************************/
/*! \brief          Unconfigures all socket connections and routing groups for the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Unconfigure(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroup()
 *********************************************************************************************************************/
/*! \brief          Updates all socket connections and routing groups of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroup(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupTcp()
 *********************************************************************************************************************/
/*! \brief          Updates the TCP configuration of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update the socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupTcp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupUdp()
 *********************************************************************************************************************/
/*! \brief          Updates the UDP configuration of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update the socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupUdp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceived()
 *********************************************************************************************************************/
/*! \brief          Updates the configuration based on the received SubscribeEventgroupAck.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceived(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the initial multicast configuration of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    Configured the consumed eventgroup for multicast.
 *  \return         E_NOT_OK                Unable to configure the consumed eventgroup for multicast.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedInitial(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedRenew()
 *********************************************************************************************************************/
/*! \brief          Renews the multicast configuration of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \return         E_OK                    Renewed the multicast configuration of the consumed eventgroup.
 *  \return         E_NOT_OK                Unable to renew the multicast configuration of the consumed eventgroup.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedRenew(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoCon()
 *********************************************************************************************************************/
/*! \brief          Searches for the socket connection with the best local and remote address match.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[out]     MulticastSoConIdPtr     Best matching socket connection ID.
 *  \param[out]     MatchTypePtr            Match types of the local and remote address.
 *  \return         E_OK                    Matching/suitable socket connection found.
 *  \return         E_NOT_OK                No suitable socket connection found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoCon(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_SizeOfMulticastSoConIdType) MulticastSoConIdPtr,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) MatchTypePtr);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState()
 *********************************************************************************************************************/
/*! \brief          Calculates the state of the given address.
 *  \details        The state describes if port or address is set to a unique value or defines ANY value.
 *  \param[in]      AddrPtr
 *  \return         addrState               State of the given address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(uint8, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(
  SD_P2CONST(Sd_SockAddrContainerType) AddrPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType()
 *********************************************************************************************************************/
/*! \brief          Calculates the local match type of the given multicast socket connection.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      MulticastSoConIdIdx     Multicast socket connection ID of.
 *                                          [range: MulticastSoConIdx < Sd_GetSizeOfMulticastSoCon()]
 *  \param[out]     LocalMatchTypePtr       Match types of the local address.
 *  \return         E_OK                    Match type calculated.
 *  \return         E_NOT_OK                Not able to calculate match type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) LocalMatchTypePtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType()
 *********************************************************************************************************************/
/*! \brief          Calculates the remote match type of the given multicast socket connection.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      MulticastSoConIdIdx     Multicast socket connection ID of.
 *                                          [range: MulticastSoConIdx < Sd_GetSizeOfMulticastSoCon()]
 *  \param[out]     RemoteMatchTypePtr      Match types of the remote IP address.
 *  \return         E_OK                    Match type calculated.
 *  \return         E_NOT_OK                Not able to calculate match type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) RemoteMatchTypePtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio()
 *********************************************************************************************************************/
/*! \brief          Calculates the local match priority of the given multicast socket connection.
 *  \details        -
 *  \param[in]      MatchTypePtr            Match types of the local address.
 *  \param[in,out]  LocalMatchPrioPtr       Match priority of the local address.
 *  \param[in,out]  RemoteMatchPrioPtr      Match priority of the remote address.
 *  \return         TRUE                    Found local match.
 *  \return         FALSE                   Local address does not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) LocalMatchPrioPtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio()
 *********************************************************************************************************************/
/*! \brief          Calculates the remote match priority of the given multicast socket connection.
 *  \details        -
 *  \param[in]      MatchTypePtr            Match types of the remote address.
 *  \param[in,out]  RemoteMatchPrioPtr      Match priority of the remote address.
 *  \return         TRUE                    Found local match.
 *  \return         FALSE                   Local address does not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseCeg()
 *********************************************************************************************************************/
/*! \brief          Unconfigures the consumed eventgroup and informs the BswM if required.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ReleaseCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Update()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the client and sets a timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to set the value and timer event.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      NewState                New state of the client service state machine.
 *  \param[in]      ResetTimeToTriggerSend  Set TimeToTriggerSend to the invalid value.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Update(
  Sd_SizeOfClientType ClientIdx,
  Sd_ClientStateMachineType NewState,
  boolean ResetTimeToTriggerSend);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateCeg()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the consumed eventgroup.
 *  \details        Sets a timer event for the next main function cycle.
 *  \param[in]      ConsumedEventgroupIdx   Consumed eventgroup index to set the value and timer event.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      NewState                New state of the consumed eventgroup state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_UpdateCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_ConsumedEventGroupStateMachineType NewState);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UnconfigureCeg()
 *********************************************************************************************************************/
/*! \brief          Unconfigures all socket connections and routing groups of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Consumed eventgroup index to unconfigure.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_UnconfigureCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean AbortFlag);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_LocalIpAlreadyConfigured()
 *********************************************************************************************************************/
/*! \brief          Check if the requested multicast IP address is already configured at another socket.
 *  \details        -
 *  \param[in]      MulticastAddressPtr     Requested multicast IP address.
 *  \return         TRUE                    Matching/suitable socket connection found.
 *  \return         FALSE                   No suitable socket connection found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_LocalIpAlreadyConfigured(
  SD_P2CONST(Sd_SockAddrContainerType) MulticastAddressPtr);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief          Requests the given address as local IP address.
 *  \details        -
 *  \param[in]      SoConId                 Socket connection ID which shall be configured.
 *  \param[in]      MulticastAddrPtr        Multicast address which shall be configured as local address.
 *  \param[in]      PerformRequest          Indicating if the address shall be configured or the request counter incremented.
 *  \return         E_OK                    Request successfully processed.
 *  \return         E_NOT_OK                Not able to configure the local address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_RequestIpAddrAssignment(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) MulticastAddrPtr,
  boolean PerformRequest);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief          Releases the local IP address
 *  \details        The local address is not released, if it is still required by another client.
 *  \param[in]      SoConId                 Socket connection ID which is configured with the local address.
 *  \return         E_OK                    Local address was successfully released.
 *  \return         E_NOT_OK                Not able to release local address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ReleaseIpAddrAssignment(
  SoAd_SoConIdType SoConId);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_EntryListContainsSubscribe()
 *********************************************************************************************************************/
/*! \brief          Checks if a SubscribeEventgroup message is still pending.
 *  \details        -
 *  \param[in,out]  ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_EntryListContainsSubscribe(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Processes the attached client entry option.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Index of the entry which shall be processed.
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntry(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOffer()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService entry.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryOffer(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledge()
 *********************************************************************************************************************/
/*! \brief          Processes the attached SubscribeEventgroupAck entry.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledge(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*! \brief          Triggers a user callout in order to validate the received configuration options.
 *  \details        If no user callout is configured, the options are accepted.
 *  \param[in]      ClientIdx                   Corresponding client index.
 *                                              [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      RxPduId                     Receiving PDU-ID.
 *  \param[in]      RxEntryType                 Type of received entry.
 *  \param[in]      RxConfigOptionPtrArrayPtr   Configured configuration option pointer list.
 *  \return         E_OK                        Message was accepted and shall be processed.
 *  \return         E_NOT_OK                    Message was not accepted and shall be rejected/ignored.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxClient_CheckConfigOptionByCbk(
  Sd_SizeOfClientType ClientIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr);
# endif
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessOptionRun()
 *********************************************************************************************************************/
/*! \brief          Processes the attached client option run.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in]      NrOfOptions             Number of succeeding Options which are attached to the Entry-Option.
 *  \param[in]      EMalformedMsgId         DEM error id for malformed messages.
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx <= Sd_GetSizeOfConsumedEventgroup()]
 *                                          [invalid: Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid option, this SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxClient_ProcessOptionRun(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_Dem_EventIdType EMalformedMsgId,
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

/**********************************************************************************************************************
 *  Sd_Rx_GetReceivingInstance()
 *********************************************************************************************************************/
/*! \brief          Searches for the instance the message was directed to.
 *  \details        -
 *  \param[in]      RxPduId                 ID of the received I-PDU.
 *  \param[out]     InstanceIdxPtr          Index of instance which matches the I-PDU.
 *                                          [range: InstanceIdx <= Sd_GetSizeOfInstance()]
 *                                          [invalid: Sd_GetSizeOfInstance()]
 *  \param[out]     RxSoConIdPtr            Socket connection ID which was used for reception.
 *  \param[out]     MulticastMessagePtr     Indicates if the message was received via unicast or multicast.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_GetReceivingInstance(
  PduIdType RxPduId,
  SD_P2VAR(Sd_SizeOfInstanceType) InstanceIdxPtr,
  SD_P2VAR(SoAd_SoConIdType) RxSoConIdPtr,
  SD_P2VAR(boolean) MulticastMessagePtr);

/**********************************************************************************************************************
 *  Sd_Rx_SaveAddr()
 *********************************************************************************************************************/
/*! \brief          Saves the address pointed by InAddrPtr.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which handles this address.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      InAddrPtr               Address which shall be saved.
 *  \return         Index of the saved address.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Rx_SaveAddr(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr);

/**********************************************************************************************************************
 *  Sd_Rx_IndexOptionArray()
 *********************************************************************************************************************/
/*! \brief          Reads all attached options and generates an index in order to ease the access of individual options.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_IndexOptionArray(
  SD_P2CONST(PduInfoType) PduInfoPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckOption()
 *********************************************************************************************************************/
/*! \brief          Checks if the option is feasible.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      OptionIndex             Start index of the option.
 *  \param[in]      OptionArrayLength       Length of the options array.
 *  \param[out]     CoveredOptionLengthPtr  Covered length of the option.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Option is invalid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 OptionIndex,
  uint32 OptionArrayLength,
  SD_P2VAR(uint16) CoveredOptionLengthPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckRemoteReboot()
 *********************************************************************************************************************/
/*! \brief          Checks if the remote service has rebooted and handles the reboot.
 *  \details        -
 *  \param[in,out]  RxMessageInfoPtr        Information about the received message.
 *  \param[in]      SdHeaderPtr             Header information.
 *  \param[in]      InstanceIdx             Index of instance which handles this address.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_CheckRemoteReboot(
  SD_P2VAR(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_HeaderType) SdHeaderPtr,
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Rx_GetSdEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Checks if an SD endpoint option was attached to the message and extracts it.
 *  \details        -
 *  \param[in]      RxSoConId               Socket connection ID to obtain the subnet.
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[out]     SdEndpointAddressPtr    Pointer to a structure where the SD endpoint address shall be stored.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_GetSdEndpointOption(
  SoAd_SoConIdType RxSoConId,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SdEndpointAddressPtr);

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Rx_GetEndpointOptionParameters()
 *********************************************************************************************************************/
/*! \brief          Reads the parameters of the received endpoint options.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Index of the Option.
 *  \param[in]      ReadMulticastEndpoints  Specifies if unicast or multicast endpoints are allowed and shall be read.
 *  \param[in]      FamilyTypePtr           Address family.
 *  \param[in]      L4ProtocolTypePtr       L4 protocol type of the option.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_GetEndpointOptionParameters(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  boolean ReadMulticastEndpoints,
  SD_P2VAR(Sd_FamilyType) FamilyTypePtr,
  SD_P2VAR(Sd_L4ProtocolType) L4ProtocolTypePtr);
#endif

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Reads the received endpoint options.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Index of the Option.
 *  \param[in]      FamilyType              Address family.
 *  \param[in]      L4ProtocolType          L4 protocol type of the option.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Endpoint is valid.
 *  \return         E_NOT_OK                Invalid Endpoint.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ReadEndpointOptions(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_FamilyType FamilyType,
  Sd_L4ProtocolType L4ProtocolType,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);
#endif

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Reads the received endpoint option.
 *  \details        -
 *  \param[in]      FamilyType              Address family.
 *  \param[in]      DataPtr                 Data pointer to the received payload.
 *  \param[in]      DataIndex               Index of the option.
 *  \param[out]     EndpointAddrPtr         Reference where the endpoint address shall be stored.
 *  \return         E_OK                    Endpoint is valid.
 *  \return         E_NOT_OK                Invalid Endpoint.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ReadEndpointOption(
  Sd_FamilyType FamilyType,
  SD_P2CONST(uint8) DataPtr,
  uint16 DataIndex,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckConfigOption()
 **********************************************************************************************************************/
/*! \brief          Checks the received configuration option.
 *  \details        -
 *  \param[in]      PduInfoPtr                Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                            buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption             Index of the Option.
 *  \param[in,out]  ConfigOptionPtrArrayPtr   Pointer to the Configuration Option Pointer Array.
 *  \return         E_OK                      Configuration option is valid.
 *  \return         E_NOT_OK                  Invalid configuration option.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 **********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckConfigOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr);

#if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
# if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Rx_FillConfigOptionPtrArray()
 **********************************************************************************************************************/
/*! \brief          Adds information to the configuration option pointer array.
 *  \details        Adds the hostname and capability record configuration options to the configuration
 *                  option pointer array.
 *  \param[in,out]  ConfigOptionPtrArrayPtr   Pointer to the Configuration Option Pointer Array.
 *  \param[in]      InstanceCapabilityIdx     Capability record index of the instance.
 *  \param[in]      ServiceCapabilityIdx      Capability record index of the service.
 *  \param[in]      ServiceCapabilityInvIdx   Invalid capability record index of the service.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 **********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_FillConfigOptionPtrArray(
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr,
  Sd_SizeOfCapabilityRecordType InstanceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityInvIdx);
# endif
#endif

/**********************************************************************************************************************
 *  Sd_State_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all state machines of clients and servers.
 *  \details        State machines are only executed if the corresponding timer is exceeded.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_Process(
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Rx_ProcessHeader()
 *********************************************************************************************************************/
/*! \brief          Validates the message header and reads the header fields.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[out]     SdHeaderPtr             Pointer where to store the header.
 *  \return         E_OK                    Header is valid.
 *  \return         E_NOT_OK                Invalid Header, SD message is not valid.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ProcessHeader(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_HeaderType) SdHeaderPtr);

/**********************************************************************************************************************
 *  Sd_Rx_ProcessEntryArray()
 *********************************************************************************************************************/
/*! \brief          Processes all attached entries of the message.
 *  \details        -
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_ProcessEntryArray(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Rx_HandleRemoteReboot()
 *********************************************************************************************************************/
/*! \brief          Handles the remote reboot.
 *  \details        Searches all client and server services which are affected by the reboot of the
 *                  remote service and handles the reboot.
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      AddressStorageIdx       Index of the saved address.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_HandleRemoteReboot(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx);

/**********************************************************************************************************************
 *  Sd_Tx_ProcessPendingMessages()
 *********************************************************************************************************************/
/*! \brief          Processes all pending messages which are ready to transmit (TimeToTriggerSend).
 *  \details        If there is a ready message, also all pending messages with the same destination address
                    will be transmitted.
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      ForceTransmission       If set to TRUE, the TimeToSend is ignored in order to transmit all pending
 *                                          messages directly.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_ProcessPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx,
  boolean ForceTransmission);

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfMessageReadyForTransmission()
 *********************************************************************************************************************/
/*! \brief          Checks if there is a message which is ready for transmission.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      ForceTransmission       If set to TRUE, the TimeToSend is ignored in order to transmit all pending
 *                                          messages directly.
 *  \return         Destination index of the address storage array.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Tx_CheckIfMessageReadyForTransmission(
  Sd_SizeOfInstanceType InstanceIdx,
  boolean ForceTransmission);

/**********************************************************************************************************************
 *  Sd_Tx_CopyMessagesToTmpSendEntryList()
 *********************************************************************************************************************/
/*! \brief          Copies all messages for the same destination to the temporary send entries list.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      AddressStorageIdx       Address storage index defining the destination to transmit messages.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CopyMessagesToTmpSendEntryList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx);

/**********************************************************************************************************************
 *  Sd_Tx_TransmitPendingMessages()
 *********************************************************************************************************************/
/*! \brief          Transmits all pending messages of the temporary send entries list.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_TransmitPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfEntryShallBeSkipped()
 *********************************************************************************************************************/
/*! \brief          Checks if the referenced entry is ready for transmission.
 *  \details        Otherwise the transmission of the entry has to be postponed.
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      SendEntryIdx            Entry index to transmit.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \return         TRUE                    Entry is not ready for transmission, skip it.
 *  \return         FALSE                   Entry is ready for transmission.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_CheckIfEntryShallBeSkipped(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfSendEntryType SendEntryIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializePendingMessages()
 *********************************************************************************************************************/
/*! \brief          Serializes all pending messages to a single SD message and triggers the transmission.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which triggers transmission.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \return         E_OK                    All pending entries fit into the payload.
 *  \return         E_NOT_OK                Not able to serialize all entries to the payload.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Tx_SerializePendingMessages(
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeHeader()
 *********************************************************************************************************************/
/*! \brief          Serializes the header information of the Sd message.
 *  \details        -
 *  \param[in]      AddressStorageIdx       Address storage index defining the destination to transmit messages.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_SerializeHeader(
  Sd_SizeOfAddressStorageType AddressStorageIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeRelatedOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all options of the referenced entry to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[out]     EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \return         TRUE                    Buffer was too small to serialize related options.
 *  \return         FALSE                   All related options were serialized to the payload.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeRelatedOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEntryAndOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes the referenced entry and corresponding options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PayloadEntryIdx         Payload start index to serialize entry to.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \return         TRUE                    Buffer was too small to serialize related options.
 *  \return         FALSE                   All related options were serialized to the payload.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEntryAndOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfPayloadType PayloadEntryIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Analyzes which endpoint options have to be attached to the message.
 *  \details        -
 *  \param[in]      SendEntryIdx              Index of entry to send.
 *                                            [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[out]     SockAddrContainerUdpPtr   Reference to the relevant UDP address.
 *  \param[out]     SockAddrContainerTcpPtr   Reference to the relevant TCP address.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_GetRelevantEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerUdpPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerTcpPtr);

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantConfigOptions()
 *********************************************************************************************************************/
/*! \brief          Analyzes if a configuration option has to be attached at the message.
 *  \details        -
 *  \param[in]      SendEntryIdx                  Index of entry to send.
 *                                                [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[out]     HostnameStartIdxPtr           Start index of hostname.
 *  \param[out]     HostnameEndIdxPtr             End index of hostname.
 *  \param[out]     CapabilityRecordStartIdxPtr   Start index of capability record.
 *  \param[out]     CapabilityRecordEndIdxPtr     End index of capability record.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_GetRelevantConfigOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameEndIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordEndIdxPtr);

/**********************************************************************************************************************
 *  Sd_Tx_CheckForDuplicateOption()
 *********************************************************************************************************************/
/*! \brief          Checks the payload for duplicated options.
 *  \details        Reads the serialized payload and checks if the option which was serializes after the end of the
 *                  options array can be deduplicated.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CheckForDuplicateOption(
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_DeduplicateOption()
 *********************************************************************************************************************/
/*! \brief          Tries to deduplicate the option.
 *  \details        Not all possible combinations of options can be deduplicated, because all relevant options of the
 *                  message have to be referenced by the two option runs.
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \param[in]      RemainingOption         Indicates if the current option is the last one which has to be processed.
 *  \return         TRUE                    Option was deduplicated.
 *  \return         FALSE                   Algorithm was not able to deduplicate option.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_DeduplicateOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  boolean RemainingOption);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all required endpoint options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Serializes an endpoint option to the payload.
 *  \details        -
 *  \param[in]      SendEntryType           Entry type which is serialized.
 *  \param[in]      LocalIpAddrPtr          Address which shall be copied to the endpoint option.
 *  \param[in]      L4Proto                 Which L4-Protocol (UDP/TCP) has the option.
 *  \param[in]      PayloadOffset           Start offset for the option.
 *  \return         The length of the serialized option.
 *                  [invalid: SD_INV_OPTION_LENGTH if buffer is too small]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeEndpointOption(
  Sd_EntryType SendEntryType,
  SD_P2CONST(Sd_SockAddrContainerType) LocalIpAddrPtr,
  Sd_L4ProtocolType L4Proto,
  uint16 PayloadOffset);

#if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all required configuration options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeConfigurationOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);
#endif

#if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOption()
 *********************************************************************************************************************/
/*! \brief          Serializes a configuration option to the payload.
 *  \details        -
 *  \param[in]      ConfigurationOptionStartIdx   Start index of configuration option.
 *  \param[in]      ConfigurationOptionEndIdx     End index of configuration option.
 *  \param[in]      PayloadOffset                 Start offset for the option.
 *  \return         The length of the serialized option.
 *                  [invalid: SD_INV_OPTION_LENGTH if buffer is too small]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeConfigurationOption(
  Sd_SizeOfCapabilityRecordType ConfigurationOptionStartIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionEndIdx,
  uint16 PayloadOffset); /* PRQA S 0779 */  /* MD_MSR_5.1_779 */
#endif

/**********************************************************************************************************************
 *  Sd_Tx_UseNewSerializedOption()
 *********************************************************************************************************************/
/*! \brief          Updates the entry to option mapping in order to reference the new serialized option.
 *  \details        -
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \param[in]      NewOptionSize           Size of new serialized option.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_UseNewSerializedOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  uint16 NewOptionSize);

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllTcpSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all TCP socket connections of the client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllTcpSoCons(
  Sd_SizeOfClientType ClientIdx,
  boolean Abort);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllUdpSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all UDP socket connections of the client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllUdpSoCons(
  Sd_SizeOfClientType ClientIdx,
  boolean Abort);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllMulticastSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all multicast socket connections of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllMulticastSoCons(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean Abort);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ResetDynamicVariables()
 *********************************************************************************************************************/
/*! \brief          Reset all dynamic variables of the given client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ResetDynamicVariables(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_LocalIpAddrAssignmentChgReset()
 *********************************************************************************************************************/
/*! \brief          Resets all dynamic client data in the case of a local IP address assignment change.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which has to be reset.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_SetCurrentStateInformBswm()
 *********************************************************************************************************************/
/*! \brief          Updates the current state of the client and informs the BswM.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      State                   New state of the client.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_SetCurrentStateInformBswm(
  Sd_SizeOfClientType ClientIdx,
  Sd_ClientServiceCurrentStateType State);
#endif

/**********************************************************************************************************************
 *  Sd_Util_SetSoConMode()
 *********************************************************************************************************************/
/*! \brief          Saves the current mode of the socket connection.
 *  \details        -
 *  \param[in]      SoConId                 Socket connection ID specifying the socket connection with the mode change.
 *  \param[in]      Mode                    New socket connection mode.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetSoConMode(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode);

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetSoConMode()
 *********************************************************************************************************************/
/*! \brief          Returns the current mode of the socket connection.
 *  \details        -
 *  \param[in]      SoConId                 Socket connection ID specifying the socket connection.
 *  \return         Mode of the socket connection.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SoConModeType, SD_CODE) Sd_Util_GetSoConMode(
  SoAd_SoConIdType SoConId);
#endif

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUP == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetLocalAddr()
 *********************************************************************************************************************/
/*! \brief          Read the local address of the socket connection.
 *  \details        If the local address can not be read, the value is set to an invalid value.
 *  \param[in]      SoConId                 Socket connection ID.
 *  \param[out]     SockAddrContainerPtr    Address container for local address.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerPtr);
#endif

/**********************************************************************************************************************
 *  Sd_Util_GetEmptySendEntry()
 *********************************************************************************************************************/
/*! \brief          Searches for an unused slot in the SendEntriesList and return the entry.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of the instance handling the event.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \return         Index of the empty Slot.
 *                  [range: Sd_GetSendEntryStartIdxOfInstance(InstanceIdx) <= return < Sd_GetSendEntryEndIdxOfInstance(InstanceIdx)]
 *                  [invalid: Sd_GetSendEntryEndIdxOfInstance(InstanceIdx)]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Util_GetEmptySendEntry(
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Util_RemoveSendEntryFromList()
 *********************************************************************************************************************/
/*! \brief          Removes the referenced entry from the linked list of send entries.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of the instance handling the event.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      RemoveEntryIdx          Entry index to remove.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_RemoveSendEntryFromList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfSendEntryType RemoveEntryIdx);

/**********************************************************************************************************************
 *  Sd_Util_CheckSendEntryListFullAndTransmit()
 *********************************************************************************************************************/
/*! \brief          Check if the list of pending send entries is full and trigger a transmission.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of the instance with pending send entries.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CheckSendEntryListFullAndTransmit(
  Sd_SizeOfInstanceType InstanceIdx);

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddFindEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a FindService message to the list of send entries.
 *  \details        -
 *  \param[in,out]  ClientIdx               Client index searching for service.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddFindEntryToList(
  Sd_SizeOfClientType ClientIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddMulticastOfferEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds an OfferService message via multicast to the list of send entries.
 *  \details        -
 *  \param[in,out]  ServerIdx               Server index offering its availability.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddMulticastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  boolean StopOffer);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddUnicastOfferEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds an OfferService message via unicast to the list of send entries.
 *  \details        -
 *  \param[in,out]  ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      StopOffer               TRUE  = StopOfferService message.
                                            FALSE = OfferService message.
 *  \param[in]      SavedAddressStorageIdx  Destination address index.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      TransmissionOffset      Timing offset when the message shall be transmitted.
                                            (Random delay for responses to multicast messages.)
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddUnicastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_SizeOfAddressStorageType SavedAddressStorageIdx,
  uint32 TransmissionOffset);
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddSubscribeEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroup message to the list of send entries.
 *  \details        -
 *  \param[in,out]  ConsumedEventgroupIdx   Consumed eventgroup index which triggers the subscription.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      Subscribe               TRUE  = SubscribeEventgroup message.
                                            FALSE = StopSubscribeEventgroup message.
 *  \param[in]      TransmissionOffset      Timing offset when the message shall be transmitted.
                                            (Random delay for responses to multicast messages.)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddSubscribeEntryToList(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean Subscribe,
  uint32 TransmissionOffset);
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupAckEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroupAck message to the list of send entries.
 *  \details        -
 *  \param[in,out]  InstanceIdx             Index of instance handling the event.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      RxEventIdx              Received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupAckEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfRxEventType RxEventIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupNackEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroupNack message to the list of send entries.
 *  \details        -
 *  \param[in,out]  InstanceIdx             Index of instance where the NACK-Message should be handled.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      ServerIdx               Server service.
 *                                          [range: ServerIdx <= Sd_GetSizeOfServer()]
 *                                          [invalid: Sd_GetSizeOfServer()]
 *  \param[in]      ServiceId               ServiceId of the rejected SubscribeEventgroup Message.
 *  \param[in]      InstanceId              InstanceId of the rejected SubscribeEventgroup Message.
 *  \param[in]      MajorVersion            MajorVersion of the rejected SubscribeEventgroup Message.
 *  \param[in]      ReservedField           ReservedField of the rejected SubscribeEventgroup Message.
 *  \param[in]      EventHandlerId          EventHandlerId of the rejected SubscribeEventgroup Message.
 *  \param[in]      AddressStorageIdx       Destination address index.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      ServerTimerIdx          Index of the server timer used for calculation of the additional delay.
 *                                          [range: ServerTimerIdx <= Sd_GetSizeOfServerTimer()]
 *                                          [invalid: Sd_GetSizeOfServerTimer()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupNackEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfServerType ServerIdx,
  uint16 ServiceId,
  uint16 InstanceId,
  uint8 MajorVersion,
  uint16 ReservedField,
  uint16 EventHandlerId,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfServerTimerType ServerTimerIdx);
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_ConfigureEntry()
 *********************************************************************************************************************/
/*! \brief          Initialize all server based parameters of the send entry.
 *  \details        Unused parameters will be initialized with invalid values.
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      ServerIdx               Server service.
 *                                          [range: ServerIdx <= Sd_GetSizeOfServer()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfServerType ServerIdx);
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ConfigureEntry()
 *********************************************************************************************************************/
/*! \brief          Initialize all client based parameters of the send entry.
 *  \details        Unused parameters will be initialized with invalid values.
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      ClientIdx               Client service.
 *                                          [range: ClientIdx <= Sd_GetSizeOfClient()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfClientType ClientIdx);
#endif

/**********************************************************************************************************************
 *  Sd_Util_CopySockAddr()
 *********************************************************************************************************************/
/*! \brief          Copies socket addr (incl. family, port, IP-addr) from source to target.
 *  \details        -
 *  \param[in,out]  TgtSockPtr              Target socket address.
 *  \param[in]      SrcSockPtr              Source socket address.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CopySockAddr(
  SD_P2VAR(Sd_SockAddrContainerType) TgtSockPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SrcSockPtr);

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrIp()
 *********************************************************************************************************************/
/*! \brief          Checks if IP addresses of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \param[in]      AddrState               State of the source socket address.
 *  \return         SD_MATCH_FALSE          IP addresses are not identical.
 *  \return         SD_MATCH_TRUE           IP addresses are identical.
 *  \return         SD_MATCH_ANY            IP addresses match because of ANY state.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrIp(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState);
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrPort()
 *********************************************************************************************************************/
/*! \brief          Checks if the ports of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \param[in]      AddrState               State of the source socket address.
 *  \return         SD_MATCH_FALSE          Ports are not identical.
 *  \return         SD_MATCH_TRUE           Ports are identical.
 *  \return         SD_MATCH_ANY            IP addresses match because of ANY state.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrPort(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState);
#endif

/**********************************************************************************************************************
 *  Sd_Util_SockAddrIsEqual()
 *********************************************************************************************************************/
/*! \brief          Checks if IP addresses and ports of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \return         TRUE                    IP addresses and ports are equal.
 *  \return         FALSE                   IP addresses and/or ports are not equal.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_SockAddrIsEqual(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr);

/**********************************************************************************************************************
 *  Sd_Rx_SockAddrWithinSubnet()
 *********************************************************************************************************************/
/*! \brief          Checks if both IP addresses are within the same subnet.
 *  \details        -
 *  \param[in]      SockAPtr                Socket address A.
 *  \param[in]      SockBPtr                Socket address B.
 *  \param[in]      NetmaskCidr             Net mask in CIDR notation.
 *  \return         TRUE                    IP addresses are within the same subnet.
 *  \return         FALSE                   IP addresses are not within the same subnet.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Rx_SockAddrWithinSubnet(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 NetmaskCidr);

/**********************************************************************************************************************
 *  Sd_Util_ReportDemEvent()
 *********************************************************************************************************************/
/*! \brief          Reports the configured DEM event.
 *  \details        -
 *  \param[in]      EventId                 Configured event ID.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ReportDemEvent(
  Sd_Dem_EventIdType EventId);

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRange()
 *********************************************************************************************************************/
/*! \brief          Returns a random value.
 *  \details        The value is within the range: MinValue <= Random <= MaxValue.
 *  \param[in]      MinValue                Minimal allowed value.
 *  \param[in]      MaxValue                Maximal allowed value.
 *  \return         Random number.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRange(
  uint32 MinValue,
  uint32 MaxValue);

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUP == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRangeForResponse()
 *********************************************************************************************************************/
/*! \brief          Returns a random value which can be used to delay a response message.
 *  \details        The function uses the information if it is called from InterruptContext or not in order to adjust
 *                  the MinValue and/or the MaxValue of the range, because the system time is only updated in context
 *                  of the main function.
 *  \param[in]      MinValue                Minimal allowed value.
 *                                          [MinValue has to be multiple of SD_MAIN_FCT_CYCLE_TIME_MS]
 *  \param[in]      MaxValue                Maximal allowed value.
 *                                          [MaxValue has to be multiple of SD_MAIN_FCT_CYCLE_TIME_MS]
 *  \param[in]      InterruptContext        Specifies if the function is called from context of message reception or context
 *                                          of MainFunction.
 *  \return         Random number.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRangeForResponse(
  uint32 MinValue,
  uint32 MaxValue,
  boolean InterruptContext);
#endif

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceeded()
 *********************************************************************************************************************/
/*! \brief          Checks if the timer is already exceeded.
 *  \details        -
 *  \param[in]      Timer                   Timer to be checked.
 *  \param[in]      EqualIsExceeded         If set to TRUE, the timer is also indicated as exceeded if it is equal to the
 *                                          current time.
 *  \return         TRUE                    Timer is exceeded.
 *  \return         FALSE                   Timer is not exceeded.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceeded(
  Sd_ExtendedTimeType Timer,
  boolean EqualIsExceeded);

/**********************************************************************************************************************
 *  Sd_Util_TriggerStateMachineRun()
 *********************************************************************************************************************/
/*! \brief          Triggers a state machine execution at the next MainFunction.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_TriggerStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Util_SetNextStateMachineRun()
 *********************************************************************************************************************/
/*! \brief          Triggers a state machine execution at the point in time given by the timer parameter.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      Timer                   Time to trigger next state machine execution.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetNextStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_ExtendedTimeType Timer);

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetWithTrigger()
 *********************************************************************************************************************/
/*! \brief          Updates the timer to the actual time plus the given offset.
 *  \details        Function also triggers an execution of all state machines of the given instance if the
 *                  timer expires.
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      OffsetMs                Offset when the timer shall expire.
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs,
  Sd_SizeOfInstanceType InstanceIdx);

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffset()
 *********************************************************************************************************************/
/*! \brief          Updates the timer to the actual time plus the given offset.
 *  \details        -
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      OffsetMs                Offset when the timer shall expire.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffset(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs);

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZero()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to zero.
 *  \details        -
 *  \param[in,out]  TimerPtr                Timer to reset.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToZero(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr);

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZeroTrigger()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to zero.
 *  \details        Function also triggers an execution of all state machines of the given instance.
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToZeroTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  Sd_SizeOfInstanceType InstanceIdx);
#endif

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToInvalid()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to the invalid value.
 *  \details        -
 *  \param[in,out]  TimerPtr                Timer to reset.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToInvalid(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr);

/**********************************************************************************************************************
 *  Sd_Util_Write32Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 32 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least four additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write32Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write24Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 24 bit value to the given buffer. (Lower three bytes of the 32 bit value.)
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least three additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write24Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write16Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 16 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least two additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write16Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint16 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write8Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 8 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least one additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write8Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint8 Value);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize local data. */
  Sd_Util_ResetTimerToZero(&Sd_TimeMsSinceStart);                                                                       /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Set module state to uninitialized. */
  Sd_State = SD_UNINIT;

  Sd_ConfigDataPtr = NULL_PTR;
}

/**********************************************************************************************************************
 *  Sd_Init()
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
FUNC(void, SD_CODE) Sd_Init(
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_INIT_DATA) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  Sd_ConfigDataPtr = ConfigPtr;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(ConfigPtr == NULL_PTR)
  {
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
    EcuM_BswErrorHook(SD_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = SD_E_PARAM_POINTER;
  }
# if(SD_FINALMAGICNUMBER == STD_ON)
  else if(Sd_GetFinalMagicNumber() != SD_FINAL_MAGIC_NUMBER)
  {
#  if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)                                          /* COV_SD_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SD_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = SD_E_PARAM_CONFIG;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize general data. */
    Sd_Init_General();

    /* #30 Initialize instance data. */
    Sd_Init_Instance();

#if(SD_SERVER == STD_ON)
    /* #40 Initialize server data. */
    Sd_Init_Server();
#endif

#if(SD_CLIENT == STD_ON)
    /* #50 Initialize client data. */
    Sd_Init_Client();
#endif

    /* #60 Set the state to initialized. */
    Sd_State = SD_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_INIT, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_MainFunction()
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
FUNC(void, SD_CODE) Sd_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Sd_SizeOfInstanceType instanceIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of runtime parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Update the time since start of the Sd_Init() function. */
    Sd_Util_SetTimerOffset(&Sd_TimeMsSinceStart, SD_MAIN_FCT_CYCLE_TIME_MS);                                            /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #30 Iterate over all instances. */
    for(instanceIdx = 0; instanceIdx < Sd_GetSizeOfInstance(); instanceIdx++)
    {
      /* #31 Check if module was re-initialized by a call of Sd_Init() or Sd_LocalIpAddrAssignmentChg(). */
      if(Sd_GetRunModeOfInstanceDyn(instanceIdx) != SD_RUN_MODE_RUNNING)
      {
        /* #310 Re-initialize module during runtime. */
        Sd_Init_ReInitialize(instanceIdx);
        Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_RUNNING);                                                   /* SBSW_SD_CSL02 */
      }

#if(SD_EVENTHANDLER == STD_ON)
      /* #32 Process pending server events. */
      Sd_EventServer_Process(instanceIdx);
#endif

      /* #33 Process pending state machine executions. */
      Sd_State_Process(instanceIdx);

      /* #34 Process pending messages. Serialize message and trigger transmission. */
      Sd_Tx_ProcessPendingMessages(instanceIdx, FALSE);
    }

#if(SD_TRIGGERTRANSMIT == STD_ON)
    /* #40 Process pending trigger transmit requests. */
    Sd_TriggerTransmit_Process();
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_MAIN_FUNCTION, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

#if(SD_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Sd_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SD_APPL_VAR) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = SD_E_PARAM_POINTER;
  }
  else
# endif /* SD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set VersionInfo structure with corresponding macros. */
    VersionInfoPtr->vendorID = (SD_VENDOR_ID);                                                                          /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->moduleID = (SD_MODULE_ID);                                                                          /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_major_version = (SD_SW_MAJOR_VERSION);                                                           /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_minor_version = (SD_SW_MINOR_VERSION);                                                           /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_patch_version = (SD_SW_PATCH_VERSION);                                                           /* SBSW_SD_GETVERSION_PARAM */
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_GET_VERSION_INFO, errorId);
  }
# else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* SD_DEV_ERROR_REPORT == STD_ON */
}
#endif

/**********************************************************************************************************************
 *  Sd_RxIndication()
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
FUNC(void, SD_CODE) Sd_RxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SD_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfInstanceType instanceIdx;
  Sd_HeaderType sdHeader;
  Sd_RxMessageInfoType messageInfo;
  SoAd_SoConIdType rxSoConId = 0;
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    errorId = SD_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    messageInfo.PduInfoPtr = PduInfoPtr;
    messageInfo.RebootDetected = FALSE;
    messageInfo.MulticastMessage = FALSE;

    /* #20 Find the corresponding instance. */
    Sd_Rx_GetReceivingInstance(RxPduId, &instanceIdx, &rxSoConId, &messageInfo.MulticastMessage);                       /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(instanceIdx < Sd_GetSizeOfInstance())
    {
      /* #30 The correct instance was found, read and validate message header. */
      if(Sd_Rx_ProcessHeader(instanceIdx, PduInfoPtr, &sdHeader) == E_OK)                                               /* SBSW_SD_PUBLIC_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      {
        /* #31 Generate an index of all attached options to be able to address them directly. */
        Sd_Rx_IndexOptionArray(PduInfoPtr);                                                                             /* SBSW_SD_PUBLIC_API_POINTER */

        /* #32 Check if an SD endpoint option was attached and save it. */
        Sd_Rx_GetSdEndpointOption(Sd_GetUnicastRxSoConIdOfInstance(instanceIdx), PduInfoPtr, &remoteAddr);              /* SBSW_SD_PUBLIC_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

        if(remoteAddr.family == SOAD_AF_INVALID)
        {
          /* #320 No valid SD endpoint was attached, use the remote address of the sender. */
          retVal |= SoAd_GetRcvRemoteAddr(rxSoConId, (SD_P2VAR(SoAd_SockAddrType))(&remoteAddr));                       /* PRQA S 0310 */ /* MD_Sd_11.4_0310 */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
        }

        if(retVal == E_OK)
        {
          /* #330 Save the remote address. */
          messageInfo.AddressStorageIdx = Sd_Rx_SaveAddr(instanceIdx, &remoteAddr);                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */

          if(messageInfo.AddressStorageIdx < Sd_GetSizeOfAddressStorage())
          {
            /* #3300 Check if the remote service has rebooted. */
            Sd_Rx_CheckRemoteReboot(&messageInfo, &sdHeader, instanceIdx);                                              /* SBSW_SD_REFERENCE_OF_VARIABLE */

            /* #3301 Process attached entries. */
            Sd_Rx_ProcessEntryArray(&messageInfo, instanceIdx);                                                         /* SBSW_SD_REFERENCE_OF_VARIABLE */
          }
          else
          {
            /* #3302 Remote address could not be saved (and a DET error was reported).
             *       Discard the received message. */
          }
        }
      }
    }
#if(SD_DEV_ERROR_REPORT == STD_ON)
    else
    {
      /* #40 No instance is configured for the given RxPduId. Raise an error. */
      errorId = SD_E_INV_ID;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_RX_INDICATION, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6050, 6080 */ /* MD_Sd_STCAL_6050, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_LocalIpAddrAssignmentChg()
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
FUNC(void, SD_CODE) Sd_LocalIpAddrAssignmentChg(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Sd_SizeOfInstanceType instanceIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((State != SOAD_IPADDR_STATE_ASSIGNED) &&
    (State != SOAD_IPADDR_STATE_ONHOLD) &&
    (State != SOAD_IPADDR_STATE_UNASSIGNED))
  {
    errorId = SD_E_INV_MODE;
  }
  else if(SoConId == SOAD_INV_SO_CON_ID)
  {
    errorId = SD_E_INV_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate over all instances in order to find the corresponding instance. */
    for(instanceIdx = 0; instanceIdx < Sd_GetSizeOfInstance(); instanceIdx++)
    {
      /* #200 Check if the change occurred at the TxSoCon. */
      if(Sd_GetTxSoConIdOfInstance(instanceIdx) == SoConId)
      {
        /* #2000 Save the state of the IP address. */
        Sd_SetStateOfInstanceDyn(instanceIdx, (Sd_ServiceInstanceStateType)State);                                      /* SBSW_SD_CSL01 */
        Sd_Util_TriggerStateMachineRun(instanceIdx);

        if(State == SOAD_IPADDR_STATE_ASSIGNED)
        {
          /* #20000 Save the event that a new IP address was assigned.
           *        The event will be processed in the MainFunction. */
          Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_NEW_IP_ASSIGNED);                                         /* SBSW_SD_CSL02 */
        }
        else  /* State != SOAD_IPADDR_STATE_ASSIGNED */
        {
#if(SD_CLIENT == STD_ON)
          /* #20001 Reset state of all client services. */
          Sd_UtilClient_LocalIpAddrAssignmentChgReset(instanceIdx);
#endif

#if(SD_SERVER == STD_ON)
          /* #20002 Reset state of all server services. */
          Sd_UtilServer_LocalIpAddrAssignmentChgReset(instanceIdx);
#endif
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_LOCAL_IP_ADDR_ASSIGNMENT_CHG, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_SoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((Mode != SOAD_SOCON_ONLINE) && (Mode != SOAD_SOCON_RECONNECT) && (Mode != SOAD_SOCON_OFFLINE))
  {
    errorId = SD_E_INV_MODE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Save the current mode of the socket connection. */
    Sd_Util_SetSoConMode(SoConId, Mode);
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SOCONMODE_CHG, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_ServerServiceSetState()
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
FUNC(Std_ReturnType, SD_CODE) Sd_ServerServiceSetState(
  uint16 SdServerServiceHandleId,
  Sd_ServerServiceSetStateType ServerServiceState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  /* PRQA S 3355,3356,3359 1 */ /* MD_Sd_13.7 */
  else if((ServerServiceState != SD_SERVER_SERVICE_DOWN) && (ServerServiceState != SD_SERVER_SERVICE_AVAILABLE))
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    errorId = SD_E_INV_MODE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if(SD_SERVER == STD_ON)
    /* #20 Check if the SdServerServiceHandleId is valid. */
    if(SdServerServiceHandleId < Sd_GetSizeOfServerMap())
    {
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
      if(!Sd_IsInvalidHndOfServerMap(SdServerServiceHandleId))
# endif
      {
        Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfServerMap(SdServerServiceHandleId);

        /* #200 Found the corresponding server service. */
        retVal = E_OK;

        /* #201 Check if the server shall be shut down. */
        if(ServerServiceState != SD_SERVER_SERVICE_AVAILABLE)
        {
# if(SD_EVENTHANDLER == STD_ON)
          /* #2010 Set the state of all associated event handlers. */
          Sd_UtilServer_ReleaseEventHandlers(serverIdx);
# endif

          /* #2011 Close all TCP socket connections of the server. */
          Sd_UtilServer_CloseAllTcpSoCons(serverIdx, FALSE);

          /* #2012 Close all UDP socket connections. */
          Sd_UtilServer_CloseAllUdpSoCons(serverIdx, FALSE);

# if(SD_METHODIDXOFSERVER == STD_ON)
          /* #2013 Disable the routing for provided methods. */
          Sd_UtilServer_DisableMethodRouting(serverIdx);
# endif
        }

        /* #202 Update the state. */
        Sd_SetStateSwcOfServerDyn(serverIdx, ServerServiceState);                                                       /* SBSW_SD_CSL03 */
        Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(serverIdx));
      }
    }
#else
    SD_DUMMY_STATEMENT(SdServerServiceHandleId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    SD_DUMMY_STATEMENT(ServerServiceState); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
# if(SD_SERVER == STD_ON)
  /* #30 Check if the handle ID has an invalid/unknown value. */
  if((errorId == SD_E_NO_ERROR) && (retVal == E_NOT_OK))
# else
  if(errorId == SD_E_NO_ERROR)
# endif
  {
    errorId = SD_E_INV_ID;
  }

  /* #40 Report default errors if any occurred. */
# if(SD_SERVER == STD_ON)
  if(errorId != SD_E_NO_ERROR)
# endif
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERVER_SERVICE_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_ClientServiceSetState()
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
FUNC(Std_ReturnType, SD_CODE) Sd_ClientServiceSetState(
  uint16 ClientServiceHandleId,
  Sd_ClientServiceSetStateType ClientServiceState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  /* PRQA S 3355,3356,3359 1 */ /* MD_Sd_13.7 */
  else if((ClientServiceState != SD_CLIENT_SERVICE_RELEASED) && (ClientServiceState != SD_CLIENT_SERVICE_REQUESTED))
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    errorId = SD_E_INV_MODE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if(SD_CLIENT == STD_ON)
    /* #20 Check if the ClientServiceHandleId is valid. */
    if(ClientServiceHandleId < Sd_GetSizeOfClientMap())
    {
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
      if(!Sd_IsInvalidHndOfClientMap(ClientServiceHandleId))
# endif
      {
        Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfClientMap(ClientServiceHandleId);

        /* #200 Found the corresponding client service. */
        retVal = E_OK;

        if(ClientServiceState == SD_CLIENT_SERVICE_REQUESTED)
        {
          /* #2000 Request the client service. */
          Sd_UtilClient_ClientServiceSetStateRequested(clientIdx);
        }
        else
        {
          /* #2001 Release the client service. */
          Sd_UtilClient_ClientServiceSetStateReleased(clientIdx);
        }

        /* #201 Update the state. */
        Sd_SetStateSwcOfClientDyn(clientIdx, ClientServiceState);                                                       /* SBSW_SD_CSL03 */
        Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(clientIdx));

        Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(clientIdx));                                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_HANDLE_ID_INDIRECTION */
      }
    }
#else
    SD_DUMMY_STATEMENT(ClientServiceHandleId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    SD_DUMMY_STATEMENT(ClientServiceState); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
# if(SD_CLIENT == STD_ON)
  /* #30 Check if the handle ID has an invalid/unknown value. */
  if((errorId == SD_E_NO_ERROR) && (retVal == E_NOT_OK))
# else
  if(errorId == SD_E_NO_ERROR)
# endif
  {
    errorId = SD_E_INV_ID;
  }

  /* #40 Report default errors if any occurred. */
# if(SD_CLIENT == STD_ON)
  if(errorId != SD_E_NO_ERROR)
# endif
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_CLIENT_SERVICE_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_ConsumedEventGroupSetState()
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
FUNC(Std_ReturnType, SD_CODE) Sd_ConsumedEventGroupSetState(
  uint16 SdConsumedEventGroupHandleId,
  Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  boolean invalidId = TRUE;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(Sd_State == SD_UNINIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  /* PRQA S 3355,3356,3359 1 */ /* MD_Sd_13.7 */
  else if((ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_RELEASED) && (ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_REQUESTED))
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    errorId = SD_E_INV_MODE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if(SD_CONSUMEDEVENTGROUP == STD_ON)
    /* #20 Check if the SdConsumedEventGroupHandleId is valid. */
    if(SdConsumedEventGroupHandleId < Sd_GetSizeOfConsumedEventgroupMap())
    {
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
      if(!Sd_IsInvalidHndOfConsumedEventgroupMap(SdConsumedEventGroupHandleId))
# endif
      {
        Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx =
          Sd_GetConsumedEventgroupIdxOfConsumedEventgroupMap(SdConsumedEventGroupHandleId);

# if(SD_DEV_ERROR_REPORT == STD_ON)
        invalidId = FALSE;
# endif

        /* #30 Check if the ConsumedEventGroup is requested while the corresponding client service instance is not requested. */
        if((ConsumedEventGroupState == SD_CONSUMED_EVENTGROUP_REQUESTED) &&
          (Sd_GetStateSwcOfClientDyn(Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx)) != SD_CLIENT_SERVICE_REQUESTED))
        {
          /* #300 Do not update the state because of the actual state of the client. */
        }
        else
        {
          /* #301 Update the state. */
          Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, ConsumedEventGroupState);              /* SBSW_SD_CSL03 */
          Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx)));
          retVal = E_OK;
        }
      }
    }
#else
    SD_DUMMY_STATEMENT(SdConsumedEventGroupHandleId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    SD_DUMMY_STATEMENT(ConsumedEventGroupState); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    SD_DUMMY_STATEMENT(invalidId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #30 Check if the handle ID has an invalid/unknown value. */
  if((errorId == SD_E_NO_ERROR) && (invalidId == TRUE))
# else
  if(errorId == SD_E_NO_ERROR)
# endif
  {
    errorId = SD_E_INV_ID;
  }

  /* #40 Report default errors if any occurred. */
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  if(errorId != SD_E_NO_ERROR)
# endif
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_CONSUMED_EVENTGROUP_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  SD_DUMMY_STATEMENT(invalidId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ClientServiceSetStateRequested()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ClientServiceSetStateRequested(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the current request state of the client. */
  if(Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_RELEASED)
  {
    /* #100 The client is released, check if the remote server was already seen. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimerTTLOfClientDyn(ClientIdx), FALSE))
    {
      /* #1000 Update the state and reset the TimeToTriggerSend. */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
    }
    else
    {
# if(SD_METHODIDXOFCLIENT == STD_ON)
      Std_ReturnType retVal = E_OK;
      Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx);

      /* #1001 Check if the client is configured for consumed methods. */
      if((methodIdx != SD_NO_METHODIDXOFCLIENT) &&
        (Sd_IsTcpConfiguredOfMethod(methodIdx) || Sd_IsUdpConfiguredOfMethod(methodIdx)))
      {
        retVal |= Sd_StateClient_Configure(ClientIdx);
      }

      if(retVal == E_OK)
# endif
      {
        /* #10010 Enter the main phase and inform the BswM. */
        Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
        Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_AVAILABLE);
      }
    }
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ClientServiceSetStateReleased()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ClientServiceSetStateReleased(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if(SD_METHODIDXOFCLIENT == STD_ON)
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is configured for consumed methods. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) &&
    (Sd_IsTcpConfiguredOfMethod(methodIdx) || Sd_IsUdpConfiguredOfMethod(methodIdx)))
  {
    (void)SoAd_DisableRouting(Sd_GetActivationRefOfMethod(methodIdx));

    if(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #100 Unconfigure TCP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx), FALSE);
      Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                                 /* SBSW_SD_API_ARGUMENT_IDX */
    }

    if(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #101 Unconfigure UDP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx), FALSE);
      Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                                 /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }
# endif

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #20 Pass the RELEASED state to all eventgroups. */
  {
    Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx);
      consumedEventgroupIdx++)
    {
      (void)Sd_ConsumedEventGroupSetState((uint16)Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx),
        SD_CONSUMED_EVENTGROUP_RELEASED);
    }
  }
# endif
}
#endif

/**********************************************************************************************************************
 *  Sd_Init_General()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_General(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SoConModeMapIterType soConModeMapIdx;
  Sd_SendEntryIterType sendEntryIdx;
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  Sd_AddressStorageIterType addressStorageIdx;
  Sd_InstanceIterType instanceIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the socket connection mode map. */
  for(soConModeMapIdx = 0; soConModeMapIdx < Sd_GetSizeOfSoConModeMap(); soConModeMapIdx++)
  {
    Sd_SetSoConIdOfSoConModeMap(soConModeMapIdx, SOAD_INV_SO_CON_ID);                                                   /* SBSW_SD_CSL01 */
    Sd_SetSoConModeOfSoConModeMap(soConModeMapIdx, SD_SOCON_MODE_OFFLINE);                                              /* SBSW_SD_CSL01 */
  }

#if(SD_SERVER == STD_ON)
  /* #20 Initialize the RxEvent list. */
  {
    Sd_RxEventIterType rxEventIdx;

    for(rxEventIdx = 0; rxEventIdx < Sd_GetSizeOfRxEvent(); rxEventIdx++)
    {
      Sd_SetClientListIdxOfRxEvent(rxEventIdx, Sd_GetSizeOfClientList());                                               /* SBSW_SD_CSL01 */
      Sd_SetEventHandlerIdxOfRxEvent(rxEventIdx, Sd_GetSizeOfEventHandler());                                           /* SBSW_SD_CSL01 */
      Sd_SetEventOfRxEvent(rxEventIdx, SD_SERVER_EVENT_EVENTGROUP_NONE);                                                /* SBSW_SD_CSL01 */
    }
  }
#endif

  /* #30 Initialize the option index. */
  Sd_SetNrOfOptions(0);                                                                                                 /* SBSW_SD_CSL_VAR_ELEM */

  /* #40 Initialize the sendentry list. */
  for(sendEntryIdx = 0; sendEntryIdx < Sd_GetSizeOfSendEntry(); sendEntryIdx++)
  {
    Sd_SetValidOfSendEntry(sendEntryIdx, FALSE);                                                                        /* SBSW_SD_CSL01 */
    Sd_SetSendEntryNextIdxOfSendEntry(sendEntryIdx, SD_NO_SENDENTRYNEXTIDXOFSENDENTRY);                                 /* SBSW_SD_CSL01 */
  }
  Sd_SetNrOfTmpSendEntries(0);                                                                                          /* SBSW_SD_CSL_VAR_ELEM */

  /* #50 Add all TCP socket connections to the socket connection map in order to monitor their current mode. */
  for(tcpSoConIdIdx = 0; tcpSoConIdIdx < Sd_GetSizeOfTcpSoConId(); tcpSoConIdIdx++)
  {
    Sd_Util_SetSoConMode(Sd_GetTcpSoConId(tcpSoConIdIdx), SOAD_SOCON_OFFLINE);
  }

  /* #60 Initialize the address storage array. */
  for(addressStorageIdx = 0; addressStorageIdx < Sd_GetSizeOfAddressStorage(); addressStorageIdx++)
  {
    /* Session id of 0 is not allowed -> unused/invalid address. */
    Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 0);                                                       /* SBSW_SD_CSL01 */
    Sd_SetFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK | SD_FLAGS_UNICAST_MASK);                       /* SBSW_SD_CSL01 */
    Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0);                                                     /* SBSW_SD_CSL01 */
    Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0);                                                   /* SBSW_SD_CSL01 */
    Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK);                                      /* SBSW_SD_CSL01 */
    Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK);                                    /* SBSW_SD_CSL01 */
  }

  /* #70 Set the session ID of the SD multicast address. The address will be set during Sd_Init_ReInitialize(). */
  for(instanceIdx = 0; instanceIdx < Sd_GetSizeOfInstance(); instanceIdx++)
  {
    Sd_SetSessionIdCounterOfAddressStorage(Sd_GetAddressStorageStartIdxOfInstance(instanceIdx), 1);                     /* SBSW_SD_CSL01 */
  }
}

/**********************************************************************************************************************
 *  Sd_Init_Instance()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Instance(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all instances. */
  for(instanceIdx = 0; instanceIdx < Sd_GetSizeOfInstance(); instanceIdx++)
  {
    /* #20 Trigger an initial run of the state machines. */
    Sd_Util_TriggerStateMachineRun(instanceIdx);

    /* #30 Initialize the dynamic data structures. */
    Sd_Util_ResetTimerToZero(Sd_GetAddrNextStateMachineRunOfInstanceDyn(instanceIdx));                                  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */

    if(Sd_State == SD_UNINIT)
    {
      /* #40 The link state is only changed at the initial run after a reboot. */
      Sd_SetStateOfInstanceDyn(instanceIdx, SD_INSTANCE_DOWN);                                                          /* SBSW_SD_CSL02 */
      Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_INITIAL_RUN);                                                 /* SBSW_SD_CSL02 */
    }

    /* #50 Initialize the RX event list. */
    Sd_SetRxEventListFullOfInstanceDyn(instanceIdx, FALSE);                                                             /* SBSW_SD_CSL02 */
#if(SD_EVENTHANDLER == STD_ON)
    Sd_SetRxEventStartIdxOfInstanceDyn(instanceIdx, Sd_GetRxEventStartIdxOfInstance(instanceIdx));                      /* SBSW_SD_CSL02 */
    Sd_SetRxEventEndIdxOfInstanceDyn(instanceIdx, Sd_GetRxEventStartIdxOfInstance(instanceIdx));                        /* SBSW_SD_CSL02 */
#endif

    /* #60 Initialize the send entries list. */
    Sd_SetSendEntryNrPendingIdxOfInstanceDyn(instanceIdx, 0);                                                           /* SBSW_SD_CSL02 */

    Sd_SetSendEntryStartIdxOfInstanceDyn(instanceIdx, SD_NO_SENDENTRYSTARTIDXOFINSTANCEDYN);                            /* SBSW_SD_CSL02 */
    Sd_SetSendEntryEndIdxOfInstanceDyn(instanceIdx, SD_NO_SENDENTRYENDIDXOFINSTANCEDYN);                                /* SBSW_SD_CSL02 */
  }
}

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_Server()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Server(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ServerIterType serverIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the servers. */
  for(serverIdx = 0; serverIdx < Sd_GetSizeOfServer(); serverIdx++)
  {
    /* #100 Set the initial state of the server. */
    if(Sd_IsAutoAvailableOfServer(serverIdx))
    {
      Sd_SetStateSwcOfServerDyn(serverIdx, SD_SERVER_SERVICE_AVAILABLE);                                                /* SBSW_SD_CSL02 */
    }
    else
    {
      Sd_SetStateSwcOfServerDyn(serverIdx, SD_SERVER_SERVICE_DOWN);                                                     /* SBSW_SD_CSL02 */
    }

    /* #101 Initialize all dynamic variables. */
    Sd_SetStateMachineOfServerDyn(serverIdx, SD_SERVER_STATE_NONE);                                                     /* SBSW_SD_CSL02 */
    Sd_SetInitialOfferRepetitionRunOfServerDyn(serverIdx, 0);                                                           /* SBSW_SD_CSL02 */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(serverIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */
  }

# if(SD_EVENTHANDLER == STD_ON)
  /* #20 Initialize the event handlers. */
  {
    Sd_SizeOfEventHandlerType eventHandlerIdx;

    for(eventHandlerIdx = 0; eventHandlerIdx < Sd_GetSizeOfEventHandler(); eventHandlerIdx++)
    {
      /* #200 Initialize all dynamic variables. */
      Sd_SetStateMachineOfEventHandlerDyn(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NONE);                                /* SBSW_SD_CSL02 */
      Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0);                                                           /* SBSW_SD_CSL02 */
      Sd_SetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx, 0);                                                        /* SBSW_SD_CSL02 */
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, SD_EVENT_HANDLER_RELEASED);
    }
  }

  /* #30 Initialize the client list. */
  {
    Sd_ClientListIterType clientListIdx;

    for(clientListIdx = 0; clientListIdx < Sd_GetSizeOfClientList(); clientListIdx++)
    {
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID);                              /* SBSW_SD_CSL01 */
      Sd_SetNrOfReferencesOfClientList(clientListIdx, 0);                                                               /* SBSW_SD_CSL01 */
    }
  }

  /* #40 Initialize the socket connection activation map. */
  {
    Sd_SoConActivationMapIterType soConActivationMapIdx;

    for(soConActivationMapIdx = 0; soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap(); soConActivationMapIdx++)
    {
      Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0);                                              /* SBSW_SD_CSL01 */
      Sd_SetSoConIdOfSoConActivationMap(soConActivationMapIdx, SOAD_INV_SO_CON_ID);                                     /* SBSW_SD_CSL01 */
    }
  }
# endif

# if(SD_TRIGGERTRANSMIT == STD_ON)
  {
#  if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
    if(Sd_HasTriggerTransmit())
#  endif
    {
      Sd_TriggerTransmitIterType triggerTransmitIdx;

      /* #50 Initialize the trigger transmit control structures. */
      Sd_SetTriggerTransmitFull(FALSE);                                                                                 /* SBSW_SD_CSL_VAR_ELEM */
      Sd_SetTriggerTransmitStartIdx(0);                                                                                 /* SBSW_SD_CSL_VAR_ELEM */
      Sd_SetTriggerTransmitEndIdx(0);                                                                                   /* SBSW_SD_CSL_VAR_ELEM */

      for(triggerTransmitIdx = 0; triggerTransmitIdx < Sd_GetSizeOfTriggerTransmit(); triggerTransmitIdx++)
      {
        /* #500 Initialize the trigger transmit list. */
        Sd_SetClientListIdxOfTriggerTransmit(triggerTransmitIdx, Sd_GetSizeOfClientList()); /* Invalid value. */        /* SBSW_SD_CSL01 */
        Sd_SetEventHandlerIdxOfTriggerTransmit(triggerTransmitIdx, Sd_GetSizeOfEventHandler()); /* Invalid value. */    /* SBSW_SD_CSL01 */
        Sd_SetStateOfTriggerTransmit(triggerTransmitIdx, SD_TRIGGER_TRANSMIT_STATE_INVALID);                            /* SBSW_SD_CSL01 */
      }
    }
  }
# endif
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_Client()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Client(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the clients. */
  for(clientIdx = 0; clientIdx < Sd_GetSizeOfClient(); clientIdx++)
  {
    /* #100 Inform the BswM if the Sd_Init() is called while being in the initial wait phase. */
    if((Sd_State == SD_INIT) && (Sd_GetStateMachineOfClientDyn(clientIdx) == SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT))
    {
      /* #1000 Inform the BswM about the state of the client service and all consumed eventgroups. */
      Sd_UtilClient_SetCurrentStateInformBswm(clientIdx, SD_CLIENT_SERVICE_DOWN);

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
      {
        Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

        for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(clientIdx);
          consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(clientIdx);
          consumedEventgroupIdx++)
        {
          BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx),
            SD_CONSUMED_EVENTGROUP_DOWN);
        }
      }
# endif
    }

    /* #101 Set the initial state of the clients. */
    if(Sd_IsAutoRequireOfClient(clientIdx))
    {
      Sd_SetStateSwcOfClientDyn(clientIdx, SD_CLIENT_SERVICE_REQUESTED);                                                /* SBSW_SD_CSL02 */
    }
    else
    {
      Sd_SetStateSwcOfClientDyn(clientIdx, SD_CLIENT_SERVICE_RELEASED);                                                 /* SBSW_SD_CSL02 */
    }

    /* #102 Initialize all dynamic variables. */
    Sd_UtilClient_ResetDynamicVariables(clientIdx);
  }

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #20 Initialize all consumed eventgroups. */
  Sd_Init_ClientCeg();

#  if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
  {
    Sd_RequestCntIterType requestCntIdx;
    Sd_PortSetIterType portSetIdx;

    /* #30 Initialize the Request Count and Port Set flag for dynamic multicast addresses. */
    for(requestCntIdx = 0; requestCntIdx < Sd_GetSizeOfRequestCnt(); requestCntIdx++)
    {
      Sd_SetRequestCnt(requestCntIdx, 0);                                                                               /* SBSW_SD_CSL01 */
    }

    for(portSetIdx = 0; portSetIdx < Sd_GetSizeOfPortSet(); portSetIdx++)
    {
      Sd_SetPortSet(portSetIdx, FALSE);                                                                                 /* SBSW_SD_CSL01 */
    }
  }
#  endif
# endif
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ClientCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ClientCeg(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all consumed eventgroups. */
  for(consumedEventgroupIdx = 0; consumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup(); consumedEventgroupIdx++)
  {
    /* #100 Set the initial state of the consumed eventgroups. */
    if(Sd_IsAutoRequireOfConsumedEventgroup(consumedEventgroupIdx))
    {
      Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_REQUESTED);         /* SBSW_SD_CSL02 */
    }
    else
    {
      Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_RELEASED);          /* SBSW_SD_CSL02 */
    }

    /* #101 Initialize all dynamic variables. */
    Sd_SetCurrentStateOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                      /* SBSW_SD_CSL02 */
    Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                         /* SBSW_SD_CSL02 */
    Sd_SetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);        /* SBSW_SD_CSL02 */
    Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx));                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */
    Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, 0);                                             /* SBSW_SD_CSL02 */
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID);                           /* SBSW_SD_CSL02 */
    Sd_SetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID);                                 /* SBSW_SD_CSL02 */
    Sd_SetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID);                                 /* SBSW_SD_CSL02 */
  }
}
#endif

/**********************************************************************************************************************
 *  Sd_Init_ReInitialize()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitialize(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_EndpointStorageIterType endpointStorageIdx;
  Sd_SockAddrContainerType localAddr;
  Sd_SockAddrContainerType defaultRouter;
  uint8 netmaskCidr = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the list of endpoint entries. */
  for(endpointStorageIdx = 0; endpointStorageIdx < Sd_GetSizeOfEndpointStorage(); endpointStorageIdx++)
  {
    Sd_SetNrOfReferencesOfEndpointStorage(endpointStorageIdx, 0);                                                       /* SBSW_SD_CSL01 */
    Sd_SetTypeOfEndpointStorage(endpointStorageIdx, SD_ENDPOINT_TYPE_INVALID);                                          /* SBSW_SD_CSL01 */
    (Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx))->family = SOAD_AF_INVALID;                                    /* SBSW_SD_CSL01 */
  }

  /* #20 Get the Sd multicast address. */
  localAddr.family = SOAD_AF_INVALID;
  /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetLocalAddr(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx),
    (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #200 Save the multicast address and initialize the SessionID counter. */
    Sd_Util_CopySockAddr(
      Sd_GetAddrAddrOfAddressStorage(Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx)), &localAddr);                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    Sd_SetSessionIdCounterOfAddressStorage(Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx), 1);                     /* SBSW_SD_CSL03 */

    /* #201 Set the SoAd remote address of the Rx and Tx socket connections to multicast in order to initialize (open) the socket. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    if(E_OK == SoAd_SetRemoteAddr(Sd_GetUnicastRxSoConIdOfInstance(InstanceIdx),
      (SD_P2VAR(SoAd_SockAddrType))&localAddr))                                                                         /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      (void)SoAd_OpenSoCon(Sd_GetUnicastRxSoConIdOfInstance(InstanceIdx));
    }

    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    if(E_OK == SoAd_SetRemoteAddr(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx),
      (SD_P2VAR(SoAd_SockAddrType))&localAddr))                                                                         /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      (void)SoAd_OpenSoCon(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx));
    }

    /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
    if(E_OK == SoAd_SetRemoteAddr(Sd_GetTxSoConIdOfInstance(InstanceIdx), (SD_P2VAR(SoAd_SockAddrType))&localAddr))     /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      (void)SoAd_OpenSoCon(Sd_GetTxSoConIdOfInstance(InstanceIdx));
    }

#if(SD_SERVER == STD_ON)
    /* #202 Initialize all server data. */
    Sd_Init_ReInitializeServers(InstanceIdx);
#endif

#if(SD_CLIENT == STD_ON)
    /* #203 Initialize all client data. */
    Sd_Init_ReInitializeClients(InstanceIdx);
#endif
  }
}

/**********************************************************************************************************************
 *  SERVER FUNCTIONS
 *********************************************************************************************************************/

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ReInitializeServers()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeServers(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all server to update and process the state machine. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx);
    serverIdx++)
  {
# if(SD_EVENTHANDLER == STD_ON)
    Sd_SizeOfEventHandlerType eventHandlerIdx;

    /* #101 Iterate over all event handlers, disable the RoutingGroups and close the socket connections. */
    for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx);
      eventHandlerIdx++)
    {
      Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx;
      Sd_ClientListIterType clientListIdx;

      /* #1010 Reset multicast event handler configuration. */
      eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx);
      if(eventHandlerMulticastIdx != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)
      {
        (void)SoAd_DisableRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx));
        (void)SoAd_CloseSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx), TRUE);
      }

      /* #1011 Disable routing of UDP and TCP event handler and reset activation count. */
      Sd_UtilServer_DisableRouting(eventHandlerIdx);

      /* #1012 Clear the list of subscribed clients. */
      Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0);                                                           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx, 0);                                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(eventHandlerIdx);
        clientListIdx < Sd_GetClientListEndIdxOfEventHandler(eventHandlerIdx);
        clientListIdx++)
      {
        Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID);                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }
    }
# endif

# if(SD_METHODIDXOFSERVER == STD_ON)
    /* #100 Disable routing for provided methods. */
    Sd_UtilServer_DisableMethodRouting(serverIdx);
# endif

    /* #102 Close all TCP socket connections. */
    Sd_UtilServer_CloseAllTcpSoCons(serverIdx, TRUE);

    /* #103 Close all UDP socket connections. */
    Sd_UtilServer_CloseAllUdpSoCons(serverIdx, TRUE);
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Process(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle the state machine within the ready block. */
  switch(Sd_GetStateMachineOfServerDyn(ServerIdx))
  {
    case SD_SERVER_STATE_NONE:
    case SD_SERVER_STATE_NOT_READY:
      Sd_StateServer_ProcessNotReady(ServerIdx);
      break;

    case SD_SERVER_STATE_READY_INITIAL_WAIT:
      Sd_StateServer_ProcessInitialWait(ServerIdx);
      break;

    case SD_SERVER_STATE_READY_REPETITION:
      Sd_StateServer_ProcessRepetition(ServerIdx);
      break;

    case SD_SERVER_STATE_READY_MAIN_PHASE:
      Sd_StateServer_ProcessMain(ServerIdx);
      break;

    default:
      /* Undefined state. */
      break;
  }

  /* #20 Check if the state machine is within the ready block and a condition to leave the block is fulfilled. */
  if(((Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED) ||
    (Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_DOWN)) &&
    ((Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_READY_INITIAL_WAIT) ||
    (Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_READY_REPETITION) ||
    (Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_READY_MAIN_PHASE)))
  {
    /* #200 Send a StopOfferService message only if the SD is still running. */
    if(Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED)
    {
      /* Send a StopOfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, TRUE);
    }

    /* #201 Leave the ready-state and reset the TimeToTriggerSend. */
    Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_NOT_READY, TRUE);
  }

  /* #30 Run the state machine, handling all event handlers of the server instance. */
# if(SD_EVENTHANDLER == STD_ON)
  Sd_StateServer_ProcessEventHandlerStateMachine(ServerIdx);
# endif

  /* #40 Update the time for the next state machine run. */
  Sd_Util_SetNextStateMachineRun(instanceIdx, Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx));
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessNotReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessNotReady(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if it is the initial run of the server state machine. */
  if(Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_NONE)
  {
    /* #100 First run of the state machine. Update state. */
    if((Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED) &&
      (Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_AVAILABLE))
    {
      Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_READY_INITIAL_WAIT, FALSE);
    }
    else
    {
      Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_NOT_READY, FALSE);
    }
  }
  else /* Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_NOT_READY */
  {
    /* #101 Check if the server is requested and the instance is up and configured. */
    if((Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED) &&
      (Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_AVAILABLE))
    {
      /* #1010 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_READY_INITIAL_WAIT, TRUE);
    }
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessInitialWait()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessInitialWait(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx);
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  uint32 additionalOffset;

  /* ----- Implementation ----------------------------------------------- */
  if(Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
  {
    /* #10 Instance is not up and configured. Update state. */
    Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_NOT_READY, FALSE);
  }
  else if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx)))
  {
    /* #20 First run of the wait state. */
# if(SD_EVENTHANDLER == STD_ON)
    Sd_EventHandlerIterType eventHandlerIdx;

    /* #21 Open the multicast socket connections. */
    for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx);
      eventHandlerIdx++)
    {
      Sd_EventHandlerMulticastIterType eventHandlerMulticastIdx;
      eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx);

      if(eventHandlerMulticastIdx != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)
      {
        (void)SoAd_OpenSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx));
      }
    }
# endif

    /* #22 Open the TCP socket connections in order to allow incoming connections.
     *     (UDP socket connections will be configured and opened if a client subscribes.) */
    for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfServer(ServerIdx);
      tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfServer(ServerIdx);
      tcpSoConIdIdx++)
    {
      (void)SoAd_OpenSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx));
    }

# if(SD_METHODIDXOFSERVER == STD_ON)
    if(Sd_GetMethodIdxOfServer(ServerIdx) != SD_NO_METHODIDXOFSERVER)
    {
      /* #23 Enable the routing group for the provided method. */
      (void)SoAd_EnableRouting(Sd_GetActivationRefOfMethod(Sd_GetMethodIdxOfServer(ServerIdx)));
    }
# endif

    /* Timer is unused and can be set. */
    additionalOffset = Sd_Util_GetRandomNrInRange(
      Sd_GetInitialOfferDelayMinOfServerTimer(serverTimerIdx),
      Sd_GetInitialOfferDelayMaxOfServerTimer(serverTimerIdx));
    Sd_Util_SetTimerOffsetWithTrigger(
      Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx), additionalOffset, instanceIdx);                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else
  {
    /* #30 Timer is running, check if exceeded already. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx), TRUE))
    {
      /* #300 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_READY_REPETITION, TRUE);

      /* #301 Send an OfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, FALSE);
    }
  }
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessRepetition()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessRepetition(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the repetition phase is entered the first time.*/
  if(Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx) == 0)
  {
    /* #100 Go to the main state if there is no repetition phase configured.
     *      Update the state and reset the TimeToTriggerSend.*/
    Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_READY_MAIN_PHASE, TRUE);
  }
  else if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx)))
  {
    /* #101 Initial run of the repetition phase. */
    uint32 initialRepetitionDelay = Sd_GetInitialOfferRepetitionBaseDelayOfServerTimer(serverTimerIdx);
    if(initialRepetitionDelay >= SD_MAIN_FCT_CYCLE_TIME_MS)
    {
      initialRepetitionDelay -= SD_MAIN_FCT_CYCLE_TIME_MS;
    }

    /* #102 Reset the repetition counter and initialize the timer. */
    Sd_SetInitialOfferRepetitionRunOfServerDyn(ServerIdx, 0);                                                           /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_Util_SetTimerOffsetWithTrigger(
      Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx), initialRepetitionDelay, instanceIdx);                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else
  {
    /* No further state transitions available. */
  }

  /* #20 Succeeding runs of the repetition phase. */
  if((TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx), TRUE)) &&
    (Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx) > Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx)) &&
    (Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_READY_REPETITION))
  {
    /* #200 Send an OfferService message and wait for the next repetition. */
    Sd_Util_AddMulticastOfferEntryToList(ServerIdx, FALSE);
    Sd_IncInitialOfferRepetitionRunOfServerDyn(ServerIdx);                                                              /* SBSW_SD_API_ARGUMENT_IDX */

    if(Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx) == Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx))
    {
      /* #201 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, SD_SERVER_STATE_READY_MAIN_PHASE, TRUE);
    }
    else
    {
      /* #202 Wind up the timer. */
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx),
        (uint32)((uint32)(1 << Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx)) * Sd_GetInitialOfferRepetitionBaseDelayOfServerTimer(serverTimerIdx)),
        instanceIdx);                                                                                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessMain()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessMain(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the server shall offer the current service instance. */
  if(Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx) != 0)
  {
    /* #100 Check if initial run of the main phase. */
    if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx)))
    {
      uint32 initialMainPhaseDelay = Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx);
      if(initialMainPhaseDelay >= SD_MAIN_FCT_CYCLE_TIME_MS)
      {
        initialMainPhaseDelay -= SD_MAIN_FCT_CYCLE_TIME_MS;
      }

      /* #1000 Set timer to send the first offer message after the configured OfferCyclicDelay. */
      Sd_Util_SetTimerOffsetWithTrigger(
        Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx), initialMainPhaseDelay, instanceIdx);                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }

    /* #101 Check if timer is expired. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx), TRUE))
    {
      /* #1010 Send an OfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, FALSE);

      /* #1011 Wind up the timer. */
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx),
        Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx), instanceIdx);                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
  }
  else
  {
    /* No offer messages are transmitted for this service. */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_ProcessEventHandlerStateMachine()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessEventHandlerStateMachine(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #1 Handle all event handlers of the server. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx);
    eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx);
    eventHandlerIdx++)
  {
    eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx);

    /* #10 Update state of the event handler. */
    switch(Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx))
    {
      case SD_EVENT_HANDLER_STATE_NONE:
        /* #100 First run of the state machine. */
        if(Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_DOWN)
        {
          Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_SERVICE_DOWN);
        }
        else
        {
          Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
        }
        break;

      case SD_EVENT_HANDLER_STATE_SERVICE_DOWN:
        if(Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_AVAILABLE)
        {
          Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
        }
        break;

      default:
        /* All other states are handled within the context of a client. */
        break;
    }

    /* #20 Update the state of all subscribed clients. */
    Sd_StateServer_UpdateEventHandlerClients(eventHandlerIdx);

    /* #30 Switch to multicast if this functionality is configured and the corresponding amount of subscribed clients is reached. */
    if((Sd_EventHandlerUsesMulticast(eventHandlerIdx)) &&
      (Sd_GetMulticastThresholdOfEventHandler(eventHandlerIdx) <= Sd_GetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx)) &&
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION))
    {
      Sd_StateServer_EventHandlerSwitchMulticast(eventHandlerIdx);
    }

    /* #40 Switch back to unicast if this functionality is configured. */
    if((Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION) &&
      (Sd_GetMulticastThresholdOfEventHandler(eventHandlerIdx) > Sd_GetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx)))
    {
      Sd_StateServer_EventHandlerSwitchUnicast(eventHandlerIdx);
    }

    /* #50 Inform the BswM if no client is subscribed anymore. */
    if((Sd_GetNrOfClientsOfEventHandlerDyn(eventHandlerIdx) == 0) &&
      ((Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION) ||
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION)))
    {
      /* #500 Set the state of the event handler to released and inform the BswM. */
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, SD_EVENT_HANDLER_RELEASED);

      Sd_SetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx, 0);                                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

      /* #501 Disable the multicast socket connection routing. */
      if(Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION)
      {
        (void)SoAd_DisableRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx));
      }

      /* #502 Disable routing of UDP and TCP routing groups and reset activation count. */
      Sd_UtilServer_DisableRouting(eventHandlerIdx);

      Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
    }
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerClients()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerClients(
  Sd_SizeOfEventHandlerType EventHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx);
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #1 Iterate over all registered clients of an eventgroup. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx);
    clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      /* #10 Check if the client is still subscribed. */
      if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetSubscribedUntilOfClientList(clientListIdx), FALSE))
      {
        /* #100 The client has unsubscribed or a timeout occurred. Unconfigure and remove the client. */
        Sd_UtilServer_RemoveClientFromList(EventHandlerIdx, clientListIdx);

        /* #101 Check if the client was subscribed for a pure multicast eventgroup and was the last subscribed client. */
        if((!Sd_EventHandlerUsesUdp(EventHandlerIdx)) && (!Sd_EventHandlerUsesTcp(EventHandlerIdx)) &&
          (Sd_EventHandlerUsesMulticast(EventHandlerIdx)) && (Sd_GetNrOfClientsOfEventHandlerDyn(EventHandlerIdx) == 0))
        {
          (void)SoAd_DisableSpecificRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx),
            Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx));
        }
      }

      /* #20 Update the timer for the next state machine run. */
      Sd_Util_SetNextStateMachineRun(instanceIdx, Sd_GetSubscribedUntilOfClientList(clientListIdx));
    }
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchMulticast()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchMulticast(
  Sd_SizeOfEventHandlerType EventHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx);
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;
  SoAd_SoConIdType assignedSoConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enable the routing for the multicast socket connection. */
  (void)SoAd_EnableSpecificRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx),
    Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx));

  /* #20 Iterate over all registered clients of an eventgroup in order to disable the unicast routing groups. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx);
    clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      assignedSoConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx);

      if(assignedSoConId != SOAD_INV_SO_CON_ID)
      {
        soConActivationMapIdx = Sd_UtilServer_GetActivationMapIdx(EventHandlerIdx, assignedSoConId);

        if(soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap())
        {
          Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0);                                          /* SBSW_SD_CSL01 */
        }

        /* #200 Disable the routing for the unicast socket connection. */
        (void)SoAd_DisableSpecificRouting(
          Sd_GetActivationRefOfEventHandlerUdp(Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx)),
          assignedSoConId);
      }
    }
  }

  /* #30 Update the state. */
  Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION);
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchUnicast()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchUnicast(
  Sd_SizeOfEventHandlerType EventHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx);
  SoAd_SoConIdType assignedSoConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all registered clients of an eventgroup in order to enable the unicast routing groups. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx);
    clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      assignedSoConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx);
      if(assignedSoConId != SOAD_INV_SO_CON_ID)
      {
        /* #100 Enable the unicast routing group and increase the activation counter. */
        (void)SoAd_EnableSpecificRouting(
          Sd_GetActivationRefOfEventHandlerUdp(Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx)),
          assignedSoConId);
        Sd_UtilServer_IncreaseActivationCount(EventHandlerIdx, assignedSoConId);
      }
    }
  }

  /* #20 Disable the routing for the multicast socket connection. */
  (void)SoAd_DisableSpecificRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx),
    Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx));

  /* #30 Update the state. */
  Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION);
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_DisableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_DisableRouting(
  Sd_SizeOfEventHandlerType EventHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx;
  Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx;
  Sd_SoConActivationMapIterType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable routing of TCP event handler. */
  eventHandlerTcpIdx = Sd_GetEventHandlerTcpIdxOfEventHandler(EventHandlerIdx);
  if(eventHandlerTcpIdx != SD_NO_EVENTHANDLERTCPIDXOFEVENTHANDLER)
  {
    (void)SoAd_DisableRouting(Sd_GetActivationRefOfEventHandlerTcp(eventHandlerTcpIdx));
  }

  /* #20 Disable routing of UDP event handler. */
  eventHandlerUdpIdx = Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx);
  if(eventHandlerUdpIdx != SD_NO_EVENTHANDLERUDPIDXOFEVENTHANDLER)
  {
    (void)SoAd_DisableRouting(Sd_GetActivationRefOfEventHandlerUdp(eventHandlerUdpIdx));
  }

  /* #30 Reset the number of activations. */
  for(soConActivationMapIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx);
    soConActivationMapIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx);
    soConActivationMapIdx++)
  {
    Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0);                                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntry()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntry(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;
  Sd_EntryType rxEntryType;
  uint16 rxServiceId;
  uint16 rxInstanceId;
  uint8 rxMajorVersion;
  uint16 rxReservedField;
  uint32 rxMinorVersion;
  uint16 rxEventGroupId;
  boolean foundService = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the header fields of the entry. */
  SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, rxEntryType);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);

  /* #20 Iterate over all server to find the one corresponding to the entry. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx);
    serverIdx++)
  {
    /* #200 Check if the ServiceId and InstanceId match. The wildcards are only allowed in the case of a FindService entry (format type 1). */
    if(((rxServiceId == Sd_GetServiceIdOfServer(serverIdx)) ||
      ((rxServiceId == SD_ENTRY_WILDCARD_SERVICE_ID) && (rxEntryType == SD_ENTRY_FIND_SERVICE))) &&
      ((rxInstanceId == Sd_GetInstanceIdOfServer(serverIdx)) ||
      ((rxInstanceId == SD_ENTRY_WILDCARD_INSTANCE_ID) && (rxEntryType == SD_ENTRY_FIND_SERVICE))))
    {
      /* #2000 Found the corresponding service instance. */
      foundService = TRUE;

      /* #2001 Differentiate between entry types. */
      if(rxEntryType == SD_ENTRY_FIND_SERVICE)
      {
        /* #20010 Entry format type 1 - Service.
         *        Process the entry even if the service is not available. */
        SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY1_MINORVERSION_OFFSET, rxMinorVersion);

        /* #20011 Check the remaining entry array fields. */
        if(((rxMajorVersion == Sd_GetMajorVersionOfServer(serverIdx)) || (rxMajorVersion == SD_ENTRY_WILDCARD_MAJOR_VERSION)) &&
          ((rxMinorVersion == Sd_GetMinorVersionOfServer(serverIdx)) || (rxMinorVersion == SD_ENTRY_WILDCARD_MINOR_VERSION)))
        {
          /* #200110 Process the received FindService message. */
          Sd_RxServer_ProcessEntryFind(RxMessageInfoPtr, IndexOfEntry, serverIdx);                                      /* SBSW_SD_PASSING_API_POINTER */
        }
        else
        {
          /* #200111 The predefined fields of the entry array do not match, do not process the entry. */
        }
      }
      else /* rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP */
      {
        /* #20012 Entry format type 2 - Eventgroup.
         *        Check the remaining entry array fields, the reserved field is ignored. */
# if(SD_EVENTHANDLER == STD_ON)
        if((rxMajorVersion == Sd_GetMajorVersionOfServer(serverIdx)) && (RxMessageInfoPtr->RebootDetected == FALSE))
        {
          /* #200120 Process the received SubscribeEventgroup message. */
          Sd_RxServer_ProcessEntrySubscribe(RxMessageInfoPtr, IndexOfEntry, serverIdx);                                 /* SBSW_SD_PASSING_API_POINTER */
        }
        else
# endif
        {
          /* #200121 Trigger transmission of a NACK if the MajorVersion does not match or the remote client has rebooted. */
          SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, rxReservedField);
          SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

          Sd_Util_AddEventGroupNackEntryToList(InstanceIdx, serverIdx, rxServiceId, rxInstanceId, rxMajorVersion,
            rxReservedField, rxEventGroupId, RxMessageInfoPtr->AddressStorageIdx,
            (uint8)((RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetServerTimerIdxOfServer(serverIdx) : Sd_GetSizeOfServerTimer()));
        }
      }

      if(((rxEntryType == SD_ENTRY_FIND_SERVICE) &&
        ((rxServiceId == SD_ENTRY_WILDCARD_SERVICE_ID) || (rxInstanceId == SD_ENTRY_WILDCARD_INSTANCE_ID))) ||
        (rxServiceId == SD_SERVICE_ID_OTHERSERV))
      {
        /* #20013 The FindService entry contains wildcards or the message was for the OTHERSERV service.
         *        Search for additional matching services. */
      }
      else
      {
        /* #20014 Found the corresponding server and processed the EntryOption. */
        break;
      }
    }
  }

  /* #30 Send a negative acknowledgement if the ServiceId or the InstanceId are unknown. */
  if((foundService == FALSE) && (rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP) &&
    (Sd_GetServerStartIdxOfInstance(InstanceIdx) != Sd_GetServerEndIdxOfInstance(InstanceIdx)))
  {
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, rxReservedField);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

    Sd_Util_AddEventGroupNackEntryToList(InstanceIdx, Sd_GetSizeOfServer(), rxServiceId, rxInstanceId, rxMajorVersion,
      rxReservedField, rxEventGroupId, RxMessageInfoPtr->AddressStorageIdx, Sd_GetSizeOfServerTimer());
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntryFind()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntryFind(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx);
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry() + 1];
  Sd_AttachedOptionsType attachedOptions;
  uint32 rxTTL;
  uint32 randDelay = 0;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Read header of FindService entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);

  /* #20 Ignore FindService messages if the server service is not available, during the initial wait phase, or with a TTL == 0 */
  if((Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_DOWN) ||
    (Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_READY_INITIAL_WAIT)||
    (rxTTL == 0))
  {
    /* The FindService entry is ignored. */
  }
  else
  {
    /* #200 Process the corresponding option array(s).
     *      Configuration options are allowed, endpoint options shall be ignored. */
    if(nr1stOptions != 0)
    {
      /* #2000 Process first option run. */
      retVal = Sd_RxServer_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index1stOption, nr1stOptions,
        Sd_GetSizeOfEventHandler(), &attachedOptions);                                                                  /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }
    if((nr2ndOptions != 0) && ((nr1stOptions == 0) || (retVal == E_OK)))
    {
      /* #2001 Process second option run. */
      retVal = Sd_RxServer_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index2ndOption, nr2ndOptions,
        Sd_GetSizeOfEventHandler(), &attachedOptions);                                                                  /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }
    else if((nr1stOptions == 0) && (nr2ndOptions == 0))
    {
      /* Find messages without any attached option are also allowed. */
      retVal = E_OK;
    }
    else
    {
      /* Error: Leave retVal at value E_NOT_OK. */
    }

    /* #201 Process the find entry. */
    if((retVal == E_OK) && (Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED))
    {
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
      /* #2010 Check the attached configuration options by a user callout. */
      retVal = Sd_RxServer_CheckConfigOptionByCbk(
        ServerIdx,
        (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx) : Sd_GetUnicastRxPduIdOfInstance(instanceIdx),
        SD_ENTRY_FIND_SERVICE,
        rxConfigOptionPtrArray);                                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

      if(retVal == E_OK)
# endif
      {
        /* #2011 Check if the FindService message was transmitted by multicast or unicast. */
        if(RxMessageInfoPtr->MulticastMessage == TRUE)
        {
          /* Add a random delay to the response message. */
          randDelay = Sd_Util_GetRandomNrInRangeForResponse(Sd_GetRequestResponseMinDelayOfServerTimer(serverTimerIdx),
            Sd_GetRequestResponseMaxDelayOfServerTimer(serverTimerIdx), TRUE);
        }

        /* #2012 Add a unicast OfferService message to the list of sendentries. */
        Sd_Util_AddUnicastOfferEntryToList(ServerIdx, RxMessageInfoPtr->AddressStorageIdx, randDelay);
      }
    }
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribe()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntrySubscribe(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry() + 1];
  Sd_AttachedOptionsType attachedOptions;
  Sd_SizeOfClientListType clientListIdx;
  uint16 rxServiceId, rxInstanceId;
  uint16 rxReservedField;
  uint16 rxEventGroupId;
  uint8 rxMajorVersion;

  /* ----- Implementation ----------------------------------------------- */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Search for the matching event handler to process the SubscribeEventgroup message. */
  eventHandlerIdx = Sd_RxServer_SearchEventHandler(RxMessageInfoPtr, IndexOfEntry, ServerIdx);                          /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Check if the state of the corresponding server is available or down. */
  if((eventHandlerIdx < Sd_GetSizeOfEventHandler()) &&
    (Sd_GetStateSwcOfServerDyn(ServerIdx) == SD_SERVER_SERVICE_AVAILABLE))
  {
    /* #200 Process the attached option array. */
    retVal = Sd_RxServer_ProcessOptionArray(
      RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, eventHandlerIdx, &attachedOptions);                                   /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #201 Check if the options array is valid and all required endpoint options are present. */
    if(retVal == E_OK)
    {
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
      /* #2010 Check the attached configuration options by a user callout. */
      retVal = Sd_RxServer_CheckConfigOptionByCbk(
        ServerIdx,
        (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx) : Sd_GetUnicastRxPduIdOfInstance(instanceIdx),
        SD_ENTRY_SUBSCRIBE_EVENTGROUP,
        rxConfigOptionPtrArray);                                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

      if(retVal == E_OK)
# endif
      {
        /* #20100 Search the client in the list of subscribed clients. */
        clientListIdx = Sd_RxServer_FindClientInList(
          eventHandlerIdx, RxMessageInfoPtr->AddressStorageIdx, &attachedOptions);                                      /* SBSW_SD_REFERENCE_OF_VARIABLE */

        if(clientListIdx < Sd_GetSizeOfClientList())
        {
          /* #201000 Save all parameters and endpoints, the processing will be done in the next main function cycle. */
          retVal = Sd_RxServer_ProcessEntrySubscribeSaveParameters(RxMessageInfoPtr, IndexOfEntry,
            eventHandlerIdx, clientListIdx, &attachedOptions);                                                          /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
        }
        else
        {
          /* #201001 Maximum amount of clients reached, not able to process this request. */
          retVal = E_NOT_OK;
        }
      }
    }
    else
    {
      /* #2011 Not all required endpoints are present. */
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #202 Service instance is not ready or wrong EventGroup. Leave retVal at value E_NOT_OK.
     *      This will trigger a transmission of a SubscribeEventgroupNack message. */
  }

  /* #30 Send a NACK if we were not able to process the SUBSCRIBE, if no endpoint option was attached or if the eventgroup was not configured. */
  if((retVal != E_OK) || (eventHandlerIdx == Sd_GetSizeOfEventHandler()))
  {
    Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetSizeOfServerTimer();

    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
    SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, rxReservedField);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

    if(RxMessageInfoPtr->MulticastMessage == TRUE)
    {
      /* #300 Add additional delay to NACK message. */
      if(eventHandlerIdx < Sd_GetSizeOfEventHandler())
      {
        serverTimerIdx = Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx);
      }
      else
      {
        serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx);
      }
    }

    Sd_Util_AddEventGroupNackEntryToList(instanceIdx, ServerIdx, rxServiceId, rxInstanceId, rxMajorVersion,
      rxReservedField, rxEventGroupId, RxMessageInfoPtr->AddressStorageIdx, serverTimerIdx);
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_SearchEventHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfEventHandlerType, SD_CODE) Sd_RxServer_SearchEventHandler(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  uint16 rxEventGroupId;
  boolean foundEventHandler = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the eventgroup ID from the SD header. */
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

  /* #20 Iterate trough each event handler to find the corresponding one. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx);
    eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx);
    eventHandlerIdx++)
  {
    if(Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx) == rxEventGroupId)
    {
      /* #200 Found the corresponding event handler. Stop searching. */
      foundEventHandler = TRUE;
      break;
    }
  }

  /* #30 Set the eventHandlerIdx to an invalid value if no matching event handler was found. */
  if(foundEventHandler == FALSE)
  {
    eventHandlerIdx = Sd_GetSizeOfEventHandler();
  }

  return eventHandlerIdx;
}
#endif

#if(SD_SERVER == STD_ON)
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_CheckConfigOptionByCbk(
  Sd_SizeOfServerType ServerIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the attached configuration options by a user callout. */
  if(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfServer(ServerIdx) != SD_NO_CAPABILITYRECORDFUNCTIONCALLOUTPTRIDXOFSERVER)
  {
    Sd_ConfigOptionStringType configOptionPtrArray[3] = { NULL_PTR, NULL_PTR, NULL_PTR };

#  if(SD_CAPABILITYRECORD == STD_ON)
    /* #100 Add configuration options of hostname and capability records to the configuration option pointer array. */
    Sd_Rx_FillConfigOptionPtrArray(configOptionPtrArray,
      Sd_GetCapabilityRecordStartIdxOfInstance(Sd_GetInstanceIdxOfServer(ServerIdx)),
      Sd_GetCapabilityRecordStartIdxOfServer(ServerIdx),
      SD_NO_CAPABILITYRECORDSTARTIDXOFSERVER);                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */
#  endif

    /* #101 Call application to verify configuration options of received message. */
    if(TRUE != Sd_GetCapabilityRecordFunctionCalloutPtr(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfServer(ServerIdx))(
      RxPduId,
      (uint8)RxEntryType,
      Sd_GetServiceIdOfServer(ServerIdx),
      Sd_GetInstanceIdOfServer(ServerIdx),
      Sd_GetMajorVersionOfServer(ServerIdx),
      Sd_GetMinorVersionOfServer(ServerIdx),
      RxConfigOptionPtrArrayPtr,
      configOptionPtrArray))                                                                                            /* SBSW_SD_FUNCTION_PTR_CALL */
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
# endif
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionArray()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionArray(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the referenced options from the entry. */
  SD_GET_UINT8(PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Process the corresponding option array(s) and read the data of the attached endpoints. */
  if(nr1stOptions != 0)
  {
    /* #200 Process first option run. */
    retVal = Sd_RxServer_ProcessOptionRun(PduInfoPtr, index1stOption, nr1stOptions,
      EventHandlerIdx, AttachedOptionsPtr);                                                                             /* SBSW_SD_PASSING_API_POINTER */
  }
  if((nr2ndOptions != 0) && ((nr1stOptions == 0) || (retVal == E_OK)))
  {
    /* #201 Process second option run. */
    retVal = Sd_RxServer_ProcessOptionRun(PduInfoPtr, index2ndOption, nr2ndOptions,
      EventHandlerIdx, AttachedOptionsPtr);                                                                             /* SBSW_SD_PASSING_API_POINTER */
  }
  else if((nr1stOptions == 0) && (nr2ndOptions == 0))
  {
    if(Sd_EventHandlerUsesMulticast(EventHandlerIdx))
    {
      /* #202 The EventGroup is configured for multicast, allow the subscription without any attached endpoint option. */
      retVal = E_OK;
    }
  }
  else
  {
    /* #203 Error: Leave retVal at value E_NOT_OK. */
  }

  /* #30 Check if all required endpoint options are present. */
  if((Sd_EventHandlerUsesUdp(EventHandlerIdx) && (AttachedOptionsPtr->EndpointAddrUdp.family == SOAD_AF_INVALID)) ||
    (Sd_EventHandlerUsesTcp(EventHandlerIdx) && (AttachedOptionsPtr->EndpointAddrTcp.family == SOAD_AF_INVALID)))
  {
    /* #300 Not all required endpoint options are present. Reject message. */
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionRun()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionRun(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr) /* PRQA S 3673 */ /* MD_Sd_16.7_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 currentOptionIndex = IndexOfOption;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop processing of the message if the header indexes more options than attached. */
  if(Sd_GetNrOfOptions() < (IndexOfOption + NrOfOptions))
  {
    retVal = E_NOT_OK;
  }

  /* #20 Process consecutive options. */
  while((retVal == E_OK) && ((IndexOfOption + NrOfOptions) > currentOptionIndex))
  {
    /* #200 Read the option information. */
    SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(currentOptionIndex), optionType);

    /* #201 Differentiate between service- and eventgroup entry. */
    if(EventHandlerIdx == Sd_GetSizeOfEventHandler())
    {
      /* #2010 Received FindService message. */

      if(optionType == SD_OPTION_CONFIGURATION)
      {
        /* #20100 Check if configuration option is valid. */
        retVal = Sd_Rx_CheckConfigOption(PduInfoPtr, currentOptionIndex, AttachedOptionsPtr->ConfigOptionPtrArrayPtr);  /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
      }
      else
      {
        /* #20101 Endpoint or unknown options are not allowed for FindService entries and shall be ignored. */
      }
    }
# if(SD_EVENTHANDLER == STD_ON)
    else
    {
      /* #2011 Received (Stop)SubscribeEventgroup message. */
      retVal = Sd_RxServer_ProcessOptionSubscribe(PduInfoPtr, currentOptionIndex, EventHandlerIdx, AttachedOptionsPtr); /* SBSW_SD_PASSING_API_POINTER */
    }
# endif

    currentOptionIndex++;
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribe()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribe(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 currentOption = IndexOfOption;
  Sd_OptionType optionType;
  Sd_L4ProtocolType optionL4Proto = SD_L4PROTOCOL_NONE;
  uint16 addrType = SOAD_AF_INVALID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the option information. */
  SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption), optionType);

  /* #20 Differentiate between configuration option or endpoint option. */
  if(optionType == SD_OPTION_CONFIGURATION)
  {
    /* #200 Check if configuration option is valid. */
    retVal = Sd_Rx_CheckConfigOption(PduInfoPtr, currentOption, AttachedOptionsPtr->ConfigOptionPtrArrayPtr);           /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
  }
  else
  {
    /* #201 Read the type values of the endpoint options. */
    Sd_Rx_GetEndpointOptionParameters(PduInfoPtr, IndexOfOption, FALSE, &addrType, &optionL4Proto);                     /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  /* #30 Check if a valid endpoint option was attached. */
  if(addrType != SOAD_AF_INVALID)
  {
    /* #300 Read the attached endpoint option. */
    retVal = Sd_Rx_ReadEndpointOptions(PduInfoPtr, currentOption, addrType, optionL4Proto, AttachedOptionsPtr);         /* SBSW_SD_PASSING_API_POINTER */

    /* #301 Check if the attached endpoint addresses are part of the subnet. */
    retVal |= Sd_RxServer_ProcessOptionSubscribeCheckSubnet(EventHandlerIdx, AttachedOptionsPtr);                       /* SBSW_SD_PASSING_API_POINTER */
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribeCheckSubnet()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribeCheckSubnet(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx);
  Sd_SockAddrContainerType localAddr;
  Sd_SockAddrContainerType defaultRouter;
  uint8 netmaskCidr = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the attached UDP endpoint address is part of the subnet. */
  if((AttachedOptionsPtr->EndpointAddrUdp.family != SOAD_AF_INVALID) && Sd_EventHandlerUsesUdp(EventHandlerIdx))
  {
    /* #100 Get the local IP address and the subnet mask. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retVal |= SoAd_GetLocalAddr(Sd_GetUdpSoConId(Sd_GetUdpSoConIdStartIdxOfServer(serverIdx)),
      (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);              /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Check if the endpoint address is part of the subnet. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (TRUE != Sd_Rx_SockAddrWithinSubnet(&localAddr, &AttachedOptionsPtr->EndpointAddrUdp, netmaskCidr)))              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    {
      /* #1010 The endpoint references an IP address outside the configured subnet, do not consider this endpoint. */
      AttachedOptionsPtr->EndpointAddrUdp.family = SOAD_AF_INVALID;                                                     /* SBSW_SD_VALID_API_POINTER */
    }
  }

  /* #20 Check if the attached TCP endpoint address is part of the subnet. */
  if((AttachedOptionsPtr->EndpointAddrTcp.family != SOAD_AF_INVALID) && Sd_EventHandlerUsesTcp(EventHandlerIdx))
  {
    /* #200 Get the local IP address and the subnet mask. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retVal |= SoAd_GetLocalAddr(Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx)),
      (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);              /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */

    /* #201 Check if the endpoint address is part of the subnet. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (TRUE != Sd_Rx_SockAddrWithinSubnet(&localAddr, &AttachedOptionsPtr->EndpointAddrTcp, netmaskCidr)))              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    {
      /* #2010 The endpoint references an IP address outside the configured subnet, do not consider this endpoint. */
      AttachedOptionsPtr->EndpointAddrTcp.family = SOAD_AF_INVALID;                                                     /* SBSW_SD_VALID_API_POINTER */
    }
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeSaveParameters()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeSaveParameters(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx);
  uint32 rxTTL;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the TTL reserved field and counter of the received SubscribeEventgroup message. */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);
  Sd_SetRequestedTTLOfClientList(ClientListIdx, rxTTL);                                                                 /* SBSW_SD_API_ARGUMENT_IDX */
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr,                                                                           /* SBSW_SD_API_ARGUMENT_IDX */
    IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, *Sd_GetAddrRxReservedOfClientList(ClientListIdx));

  /* #20 Save all attached endpoints, the processing will be done in the next main function cycle. */
  if(Sd_EventHandlerUsesUdp(EventHandlerIdx))
  {
    /* #200 Save the received UDP endpoint. */
    Sd_EndpointStorageUdpIdxOfClientListType endpointStorageIdx = Sd_GetEndpointStorageUdpIdxOfClientList(ClientListIdx);

    retVal |= Sd_RxServer_SaveEndpoint(instanceIdx, &AttachedOptionsPtr->EndpointAddrUdp,
      SD_ENDPOINT_TYPE_UDP, &endpointStorageIdx);                                                                       /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX */

    Sd_SetEndpointStorageUdpIdxOfClientList(ClientListIdx, endpointStorageIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX */
  }

  if(Sd_EventHandlerUsesTcp(EventHandlerIdx))
  {
    /* #201 Save the received TCP endpoint. */
    Sd_EndpointStorageTcpIdxOfClientListType endpointStorageIdx = Sd_GetEndpointStorageTcpIdxOfClientList(ClientListIdx);

    retVal |= Sd_RxServer_SaveEndpoint(instanceIdx, &AttachedOptionsPtr->EndpointAddrTcp,
      SD_ENDPOINT_TYPE_TCP, &endpointStorageIdx);                                                                       /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX */

    Sd_SetEndpointStorageTcpIdxOfClientList(ClientListIdx, endpointStorageIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX */
  }

  /* #30 Save the received (Stop)SubscribeEventgroup message as event. */
  if(rxTTL == 0)
  {
    /* SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP */
    retVal |= Sd_RxServer_AddEventToRingBuffer(instanceIdx,
      EventHandlerIdx, ClientListIdx,
      (Sd_ServerEventgroupEventType)(SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE | RxMessageInfoPtr->MulticastMessage));
  }
  else
  {
    /* SD_ENTRY_SUBSCRIBE_EVENTGROUP */
    retVal |= Sd_RxServer_AddEventToRingBuffer(instanceIdx,
      EventHandlerIdx, ClientListIdx,
      (Sd_ServerEventgroupEventType)(SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE | RxMessageInfoPtr->MulticastMessage));
  }

  Sd_Util_TriggerStateMachineRun(instanceIdx);

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Process(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfRxEventType rxEventIdx;
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  boolean pendingStopSubscribe = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process pending events for event handler. */
  while(TRUE == Sd_EventServer_ReadFirst(InstanceIdx, &rxEventIdx))                                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(rxEventIdx);

    /* #100 Check that the event handler as well as the remote client are in valid states. */
    if((Sd_GetClientStateOfClientList(Sd_GetClientListIdxOfRxEvent(rxEventIdx)) != SD_EVENT_HANDLER_CLIENT_STATE_INVALID) &&
      ((Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED) ||
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION) ||
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION)))
    {
      if((Sd_GetEventOfRxEvent(rxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE) ||
        (Sd_GetEventOfRxEvent(rxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI))
      {
        /* #1000 Check if there was a pending StopSubscribeEventgroup event which has to be processed first. */
        if(pendingStopSubscribe == FALSE)
        {
          /* #10000 Process subscription of remote client. */
          Sd_EventServer_Subscription(rxEventIdx);
        }
        else
        {
          /* #10001 There is a pending StopSubscribeEventgroup which has to be handled during the next run of the SoAd_MainFunction().
           *        Resume processing of the subscription in the next MainFunction cycle. */
          break;
        }
      }
      else if((Sd_GetEventOfRxEvent(rxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE) ||
        (Sd_GetEventOfRxEvent(rxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE_MULTI))
      {
        /* #1001 Subscription was stopped, remove the client. */
        Sd_UtilServer_RemoveClientFromList(eventHandlerIdx, Sd_GetClientListIdxOfRxEvent(rxEventIdx));
        pendingStopSubscribe = TRUE;
      }
      else
      {
        /* #1002 Nothing to do. */
      }
    }

    /* #101 Remove the processed event from the buffer. */
    Sd_EventServer_RemoveFirst(InstanceIdx);
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateServer_Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Update(
  Sd_SizeOfServerType ServerIdx,
  Sd_ServerStateMachineType NewState,
  boolean ResetTimeToTriggerSend)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfServerDyn(ServerIdx, NewState);                                                                   /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Set a timer event. */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(ServerIdx));

  if(ResetTimeToTriggerSend == TRUE)
  {
    /* #200 Reset the TimeToTriggerSend. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerStateMachine()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerStateMachine(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_EventHandlerStateMachineType NewState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfEventHandlerDyn(EventHandlerIdx, NewState);                                                       /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Set a timer event. */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(Sd_GetServerIdxOfEventHandler(EventHandlerIdx)));
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerRequestState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerRequestState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_EventHandlerCurrentStateType NewState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the request state to the new value. */
  Sd_SetRequestStateOfEventHandlerDyn(EventHandlerIdx, NewState);                                                       /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Trigger state machine execution for the next MainFunction cycle. */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(Sd_GetServerIdxOfEventHandler(EventHandlerIdx)));

  /* #30 Inform the BswM about the new request state. */
  BswM_Sd_EventHandlerCurrentState(Sd_GetHandleIdOfEventHandler(EventHandlerIdx), NewState);
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_AddEventToRingBuffer()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_AddEventToRingBuffer(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  Sd_ServerEventgroupEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType eventSaved = E_NOT_OK;
  Sd_SizeOfRxEventType rxEventIdx;
  Sd_SizeOfEndpointStorageType endpointStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer is full. */
  if(!Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx))
  {
    SD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    eventSaved = E_OK;

    /* #100 Save the received event. */
    rxEventIdx = Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx);
    Sd_SetEventHandlerIdxOfRxEvent(rxEventIdx, EventHandlerIdx);                                                        /* SBSW_SD_RXEVENT_IDX */
    Sd_SetClientListIdxOfRxEvent(rxEventIdx, ClientListIdx);                                                            /* SBSW_SD_RXEVENT_IDX */
    Sd_SetEventOfRxEvent(rxEventIdx, Event);                                                                            /* SBSW_SD_RXEVENT_IDX */

    /* #101 Update the entry of the client list. */
    Sd_IncNrOfReferencesOfClientList(ClientListIdx);                                                                    /* SBSW_SD_API_ARGUMENT_IDX */

    endpointStorageIdx = Sd_GetEndpointStorageTcpIdxOfClientList(ClientListIdx);
    if(endpointStorageIdx < Sd_GetSizeOfEndpointStorage())
    {
      Sd_IncNrOfReferencesOfEndpointStorage(endpointStorageIdx);                                                        /* SBSW_SD_CSL01 */
    }

    endpointStorageIdx = Sd_GetEndpointStorageUdpIdxOfClientList(ClientListIdx);
    if(endpointStorageIdx < Sd_GetSizeOfEndpointStorage())
    {
      Sd_IncNrOfReferencesOfEndpointStorage(endpointStorageIdx);                                                        /* SBSW_SD_CSL01 */
    }

    /* #102 Increment the ring buffer end index. */
    Sd_IncRxEventEndIdxOfInstanceDyn(InstanceIdx);                                                                      /* SBSW_SD_API_ARGUMENT_IDX */

    /* #103 Handle wrap around. */
    if(Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx) >= Sd_GetRxEventEndIdxOfInstance(InstanceIdx))
    {
      Sd_SetRxEventEndIdxOfInstanceDyn(InstanceIdx, Sd_GetRxEventStartIdxOfInstance(InstanceIdx));                      /* SBSW_SD_API_ARGUMENT_IDX */
    }

    /* #104 Check if the ring buffer is full. */
    if(Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx) == Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx))
    {
      Sd_SetRxEventListFullOfInstanceDyn(InstanceIdx, TRUE);                                                            /* SBSW_SD_API_ARGUMENT_IDX */
    }

    SD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* #105 The ring buffer is full. Not able to save the event. */
  }

  return eventSaved;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_ReadFirst()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_ReadFirst(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2VAR(Sd_SizeOfRxEventType) EventIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean eventRead = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer contains an element. */
  if((Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx) != Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx)) ||
    Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx))
  {
    /* #100 Read the first element from of the buffer. */
    *EventIdxPtr = Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx);                                                     /* SBSW_SD_VALID_API_POINTER */
    eventRead = TRUE;
  }
  else
  {
    /* The buffer contains no elements. */
    *EventIdxPtr = Sd_GetSizeOfRxEvent();                                                                               /* SBSW_SD_VALID_API_POINTER */
  }

  return eventRead;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_RemoveFirst()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_RemoveFirst(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfRxEventType startIdx = Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx);
  Sd_SizeOfClientListType clientListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer contains an element. */
  if((startIdx != Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx)) || Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx))
  {
    clientListIdx = Sd_GetClientListIdxOfRxEvent(startIdx);

    /* #100 Remove the references of the saved client and the endpoints. */

    if(Sd_GetNrOfReferencesOfClientList(clientListIdx) != 0)
    {
      Sd_DecNrOfReferencesOfClientList(clientListIdx);                                                                  /* SBSW_SD_RXEVENT_MEMBER */
    }

    if(Sd_GetNrOfReferencesOfClientList(clientListIdx) == 0)
    {
      /* #1000 The client is not referenced by another event. Remove it from the list. */
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID);                              /* SBSW_SD_RXEVENT_MEMBER */
    }

    /* #101 Remove the TCP endpoint. */
    if(Sd_EventServer_RemoveEndpoint(Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx)) == FALSE)
    {
      /* #1010 Clear reference because the endpoint is not used by another instance. */
      Sd_SetEndpointStorageTcpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage());                            /* SBSW_SD_RXEVENT_MEMBER */
    }

    /* #102 Remove UDP endpoint. */
    if(Sd_EventServer_RemoveEndpoint(Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx)) == FALSE)
    {
      /* #1020 Clear reference because the endpoint is not used by another instance. */
      Sd_SetEndpointStorageUdpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage());                            /* SBSW_SD_RXEVENT_MEMBER */
    }

    SD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #103 Remove the first element of the buffer. */
    Sd_IncRxEventStartIdxOfInstanceDyn(InstanceIdx);                                                                    /* SBSW_SD_API_ARGUMENT_IDX */

    /* #104 Handle wrap around. */
    if(Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx) >= Sd_GetRxEventEndIdxOfInstance(InstanceIdx))
    {
      Sd_SetRxEventStartIdxOfInstanceDyn(InstanceIdx, Sd_GetRxEventStartIdxOfInstance(InstanceIdx));                    /* SBSW_SD_API_ARGUMENT_IDX */
    }

    /* #105 Check if the full flag has to be removed. */
    if(Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx))
    {
      Sd_SetRxEventListFullOfInstanceDyn(InstanceIdx, FALSE);                                                           /* SBSW_SD_API_ARGUMENT_IDX */
    }

    SD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* The buffer contains no elements. */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_SaveEndpoint()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_SaveEndpoint(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr,
  Sd_EndpointType Type,
  SD_P2VAR(Sd_SizeOfEndpointStorageType) OutEndpointStorageIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfEndpointStorageType endpointStorageIdx;
  boolean saveAddr = TRUE;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Set invalid endpoint index. Will be overwritten if endpoint could be saved. */
  *OutEndpointStorageIdxPtr = Sd_GetSizeOfEndpointStorage();                                                            /* SBSW_SD_VALID_API_POINTER */

  /* #20 Check if the endpoint is valid. */
  if(InAddrPtr->family != SOAD_AF_INVALID)
  {
    /* #201 Check if the address is already saved. */
    for(endpointStorageIdx = Sd_GetEndpointStorageStartIdxOfInstance(InstanceIdx);
      endpointStorageIdx < Sd_GetEndpointStorageEndIdxOfInstance(InstanceIdx);
      endpointStorageIdx++)
    {
      /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
      if((Sd_GetNrOfReferencesOfEndpointStorage(endpointStorageIdx) != 0) &&
        (Sd_GetTypeOfEndpointStorage(endpointStorageIdx) == Type) &&
        (Sd_Util_SockAddrIsEqual(Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx), InAddrPtr)))                      /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      {
        /* #2010 The address is already saved. Save index of address. */
        saveAddr = FALSE;
        retVal = E_OK;
        *OutEndpointStorageIdxPtr = endpointStorageIdx;                                                                 /* SBSW_SD_VALID_API_POINTER */
        break;
      }
    }

    if(saveAddr == TRUE)
    {
      /* #202 Search for the next free slot and save the address. */
      for(endpointStorageIdx = Sd_GetEndpointStorageStartIdxOfInstance(InstanceIdx);
        endpointStorageIdx < Sd_GetEndpointStorageEndIdxOfInstance(InstanceIdx);
        endpointStorageIdx++)
      {
        if(Sd_GetTypeOfEndpointStorage(endpointStorageIdx) == SD_ENDPOINT_TYPE_INVALID)
        {
          /* #2020 Save the address and type of the endpoint. */
          Sd_Util_CopySockAddr(Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx), InAddrPtr);                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
          Sd_SetTypeOfEndpointStorage(endpointStorageIdx, Type);                                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
          retVal = E_OK;
          *OutEndpointStorageIdxPtr = endpointStorageIdx;                                                               /* SBSW_SD_VALID_API_POINTER */
          break;
        }
      }
    }
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_RemoveEndpoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_RemoveEndpoint(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean stillReferenced = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the endpoint is valid. */
  if(EndpointStorageIdx < Sd_GetSizeOfEndpointStorage())
  {
    /* #100 Decrement number of references. */
    if(Sd_GetNrOfReferencesOfEndpointStorage(EndpointStorageIdx) != 0)
    {
      Sd_DecNrOfReferencesOfEndpointStorage(EndpointStorageIdx);                                                        /* SBSW_SD_CSL01 */
    }

    /* #101 Remove endpoint if not used anymore. */
    if(Sd_GetNrOfReferencesOfEndpointStorage(EndpointStorageIdx) == 0)
    {
      (Sd_GetAddrAddrOfEndpointStorage(EndpointStorageIdx))->family = SOAD_AF_INVALID;                                  /* SBSW_SD_CSL01 */
      Sd_SetTypeOfEndpointStorage(EndpointStorageIdx, SD_ENDPOINT_TYPE_INVALID);                                        /* SBSW_SD_CSL01 */
      stillReferenced = FALSE;
    }
  }

  return stillReferenced;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_Subscription()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Subscription(
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already subscribed. */
  if(Sd_GetClientStateOfClientList(clientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED)
  {
    /* #100 The client subscribed for the first time. */
    retVal = Sd_EventServer_SubscriptionInitial(RxEventIdx);
  }
  else  /* Sd_GetClientStateOfClientList(clientListIdx) != SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED */
  {
    /* #101 The client is already subscribed, renew subscription. */
    retVal = Sd_EventServer_SubscriptionRenew(RxEventIdx);
  }

  /* #20 Check if the configuration was successful. */
  if(retVal == E_OK)
  {
    /* #200 Subscription was successful.
     *      Update the client configuration. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx),
      Sd_GetRequestedTTLOfClientList(clientListIdx) * 1000, Sd_GetInstanceIdxOfServer(serverIdx));                      /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */

    /* #201 Send a SubscribeEventgroupAck message and update the state. */
    Sd_Util_AddEventGroupAckEntryToList(instanceIdx, RxEventIdx);

    if(Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED)
    {
      if(Sd_GetRequestStateOfEventHandlerDyn(eventHandlerIdx) != SD_EVENT_HANDLER_REQUESTED)
      {
        /* #2010 Set the state of the event handler to requested and inform the BswM. */
        Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, SD_EVENT_HANDLER_REQUESTED);
      }

      Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION);
    }
  }
  else
  {
    Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetSizeOfServerTimer();

    /* #202 Subscription was NOT successful. (Not able to configure all mandatory endpoints.) */

    if(Sd_GetEventOfRxEvent(RxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI)
    {
      /* #2020 Add a random delay to the response message. */
      serverTimerIdx = Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx);
    }

    /* #203 Send a SubscribeEventgroupNegativeAcknowledge message. */
    Sd_Util_AddEventGroupNackEntryToList(instanceIdx, serverIdx,
      Sd_GetServiceIdOfServer(serverIdx),
      Sd_GetInstanceIdOfServer(serverIdx),
      Sd_GetMajorVersionOfServer(serverIdx),
      Sd_GetRxReservedOfClientList(clientListIdx),
      Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx),
      Sd_GetAddressStorageIdxOfClientList(clientListIdx),
      serverTimerIdx);

    /* #204 Undo the configuration of the client and remove the client from the list. */
    Sd_UtilServer_RemoveClientFromList(eventHandlerIdx, clientListIdx);

  }
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitial()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitial(
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx);
  boolean useTriggerTransmit = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if all required endpoints are present. */
  if((Sd_EventHandlerUsesTcp(eventHandlerIdx) &&
      (Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx) == Sd_GetSizeOfEndpointStorage())) ||
    (Sd_EventHandlerUsesUdp(eventHandlerIdx) &&
      (Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx) == Sd_GetSizeOfEndpointStorage())))
  {
    /* #100 The client attached not all required endpoints, reject the subscription. */
    retVal = E_NOT_OK;
  }
  else
  {
    /* #101 All mandatory endpoints are present, check if the client is a pure multicast event handler. */
    if((!Sd_EventHandlerUsesTcp(eventHandlerIdx)) && (!Sd_EventHandlerUsesUdp(eventHandlerIdx)))
    {
      Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx);

      /* #1010 The event handler is a pure multicast event handler. Enable the routing of multicast messages. */
      retVal |= SoAd_EnableSpecificRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx),
        Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx));

      Sd_IncNrOfClientsOfEventHandlerDyn(eventHandlerIdx);                                                              /* SBSW_SD_RXEVENT_MEMBER */
      Sd_IncNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx);                                                           /* SBSW_SD_RXEVENT_MEMBER */
    }
    else
    {
      /* #1011 The event handler uses TCP. Update the configuration. */
      if(Sd_EventHandlerUsesTcp(eventHandlerIdx))
      {
        retVal |= Sd_EventServer_SubscriptionInitialTcp(RxEventIdx, &useTriggerTransmit);                               /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      /* #1012 The event handler uses UDP. Update the configuration. */
      if(Sd_EventHandlerUsesUdp(eventHandlerIdx) && (retVal == E_OK))
      {
        retVal |= Sd_EventServer_SubscriptionInitialUdp(RxEventIdx, &useTriggerTransmit);                               /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

# if(SD_TRIGGERTRANSMIT == STD_ON)
      /* #1013 Check if the event handler is configured for trigger transmit. */
      if((retVal == E_OK) && (useTriggerTransmit == TRUE))
      {
        /* #10130 The client subscribed the first time, save the request in the trigger transmit list. */
        (void)Sd_TriggerTransmit_Add(RxEventIdx);
      }
# endif
    }

    if(retVal == E_OK)
    {
      /* #2000 The client subscribed successfully. */
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED);                           /* SBSW_SD_RXEVENT_MEMBER */
      Sd_IncNrOfReferencesOfClientList(clientListIdx);                                                                  /* SBSW_SD_RXEVENT_MEMBER */
    }
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialTcp()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialTcp(
  Sd_SizeOfRxEventType RxEventIdx,
  SD_P2VAR(boolean) UseTriggerTransmitPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx);
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client has already established a TCP connection. */
  /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_SetUniqueRemoteAddr(Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx)),
    (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrAddrOfEndpointStorage(Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx)),
    &assignedSoConId);                                                                                                  /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Check if the socket connection is online. */
    if(SD_SOCON_MODE_ONLINE == Sd_Util_GetSoConMode(assignedSoConId))
    {
      Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx = Sd_GetEventHandlerTcpIdxOfEventHandler(eventHandlerIdx);

      /* #1000 The client TCP IP address is already configured, enable the TCP routing group. */
      retVal |= SoAd_EnableSpecificRouting(Sd_GetActivationRefOfEventHandlerTcp(eventHandlerTcpIdx),
        assignedSoConId);
      Sd_UtilServer_IncreaseActivationCount(eventHandlerIdx, assignedSoConId);

      /* #1001 Check if the new client requires a triggered transmission. */
      if(Sd_GetTriggerRefOfEventHandlerTcp(eventHandlerTcpIdx) != SOAD_INV_ROUT_GRP_ID)
      {
        *UseTriggerTransmitPtr = TRUE;                                                                                  /* SBSW_SD_VALID_API_POINTER */
      }

      /* #1002 Save the state of the client. */
      Sd_SetAssignedTcpSoConIdOfClientList(clientListIdx, assignedSoConId);                                             /* SBSW_SD_RXEVENT_MEMBER */
      Sd_IncNrOfClientsOfEventHandlerDyn(eventHandlerIdx);                                                              /* SBSW_SD_RXEVENT_MEMBER */
    }
  }

  /* #20 Check if the subscription was successful. */
  if(Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx) == SOAD_INV_SO_CON_ID)
  {
    /* #200 Reject the subscription. */
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialUdp()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialUdp(
  Sd_SizeOfRxEventType RxEventIdx,
  SD_P2VAR(boolean) UseTriggerTransmitPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx);
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Configure a socket connection with the client endpoint address if it is not already configured. */
  if(Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx) == SOAD_INV_SO_CON_ID)
  {
    /* #100 Configure the address of the new client. */
    /* PRQA S 0310 4 */ /* MD_Sd_11.4_0310 */
    retVal |= SoAd_SetUniqueRemoteAddr(
      Sd_GetUdpSoConId(Sd_GetUdpSoConIdStartIdxOfServer(serverIdx)),
      (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrAddrOfEndpointStorage(Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx)),
      &assignedSoConId);                                                                                                /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(retVal == E_OK)
    {
      Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx = Sd_GetEventHandlerUdpIdxOfEventHandler(eventHandlerIdx);

      /* #1000 Each event handler which requires the socket connection opens it in order to indicate the dependency. */
      retVal |= SoAd_OpenSoCon(assignedSoConId);

      /* #1001 Enable the unicast routing if the event handler is not in the multicast subscription state. */
      if(Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx) != SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION)
      {
        retVal |= SoAd_EnableSpecificRouting(Sd_GetActivationRefOfEventHandlerUdp(eventHandlerUdpIdx),
          assignedSoConId);
        Sd_UtilServer_IncreaseActivationCount(eventHandlerIdx, assignedSoConId);
      }

      /* #1002 Check if the new client requires a triggered transmission. */
      if(Sd_GetTriggerRefOfEventHandlerUdp(eventHandlerUdpIdx) != SOAD_INV_ROUT_GRP_ID)
      {
        *UseTriggerTransmitPtr = TRUE;                                                                                  /* SBSW_SD_VALID_API_POINTER */
      }

      Sd_SetAssignedUdpSoConIdOfClientList(clientListIdx, assignedSoConId);                                             /* SBSW_SD_RXEVENT_MEMBER */
      Sd_IncNrOfClientsOfEventHandlerDyn(eventHandlerIdx);                                                              /* SBSW_SD_RXEVENT_MEMBER */
      Sd_IncNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx);                                                           /* SBSW_SD_RXEVENT_MEMBER */
    }
  }

  /* #20 Check if we were able to add the client to the system. */
  if(Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx) == SOAD_INV_SO_CON_ID)
  {
    /* #200 The client address could not be configured, reject the subscription. */
    Sd_Util_ReportDemEvent(Sd_GetEOutOfResIdOfInstance(Sd_GetInstanceIdxOfServer(serverIdx)));
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionRenew()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionRenew(
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The client was already subscribed, check if the TCP configuration and the endpoints are consistent. */
  if(Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx) != SOAD_INV_SO_CON_ID)
  {
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retVal |= SoAd_GetRemoteAddr(Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx),
      (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) && (TRUE == Sd_Util_SockAddrIsEqual(
      Sd_GetAddrAddrOfEndpointStorage(Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx)), &remoteAddr)))           /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #100 The client attached the correct endpoint option. */
    }
    else
    {
      /* #101 The attached endpoint does not match to the configuration. */
      retVal = E_NOT_OK;
    }
  }

  /* #20 The client was already subscribed, check if the UDP configuration and the endpoints are consistent. */
  if(Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx) != SOAD_INV_SO_CON_ID)
  {
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retVal |= SoAd_GetRemoteAddr(Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx),
      (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) && (TRUE == Sd_Util_SockAddrIsEqual(
      Sd_GetAddrAddrOfEndpointStorage(Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx)), &remoteAddr)))           /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #200 The client attached the correct endpoint option. */
    }
    else
    {
      /* #201 The attached endpoint does not match to the configuration. */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_AddClientToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_AddClientToList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  boolean foundFreeSlot = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for the next free slot in client list. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx);
    clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_INVALID)
    {
      /* #20 Save client information. */
      foundFreeSlot = TRUE;
      Sd_SetAddressStorageIdxOfClientList(clientListIdx, AddressStorageIdx);                                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetAssignedTcpSoConIdOfClientList(clientListIdx, SOAD_INV_SO_CON_ID);                                          /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetAssignedUdpSoConIdOfClientList(clientListIdx, SOAD_INV_SO_CON_ID);                                          /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED);                         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetNrOfReferencesOfClientList(clientListIdx, 0);                                                               /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetRequestedTTLOfClientList(clientListIdx, 0);                                                                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetRxReservedOfClientList(clientListIdx, 0);                                                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetEndpointStorageTcpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage());                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetEndpointStorageUdpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage());                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_Util_ResetTimerToZero(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx));                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      break;
    }
  }

  if((foundFreeSlot == FALSE))
  {
    /* #30 No free slot found. Return invalid ClientList index. */
    /* The amount of manageable clients corresponds to the amount of configured socket connections for this event handler. */
    clientListIdx = Sd_GetSizeOfClientList();
# if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_ADD_CLIENT_TO_LIST, SD_E_CLIENT_LIST_FULL);
# endif /* SD_DEV_ERROR_REPORT == STD_ON */
  }

  return clientListIdx;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientFromList()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientFromList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;
  SoAd_SoConIdType assignedUdpSoConId, assignedTcpSoConId;

  /* ----- Implementation ----------------------------------------------- */
  assignedTcpSoConId = Sd_GetAssignedTcpSoConIdOfClientList(ClientListIdx);
  assignedUdpSoConId = Sd_GetAssignedUdpSoConIdOfClientList(ClientListIdx);

  /* #10 Undo/reset the configuration. */
  if((assignedTcpSoConId == SOAD_INV_SO_CON_ID) && (assignedUdpSoConId == SOAD_INV_SO_CON_ID) &&
    (Sd_GetClientStateOfClientList(ClientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED))
  {
    /* #100 The client was subscribed for a pure multicast eventgroup. */
    Sd_DecNrOfClientsOfEventHandlerDyn(EventHandlerIdx);                                                                /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_DecNrOfClientsUdpOfEventHandlerDyn(EventHandlerIdx);                                                             /* SBSW_SD_API_ARGUMENT_IDX */
  }
  else
  {
    /* #101 The client was subscribed using unicast TCP and/or UDP connections. */

    if(assignedTcpSoConId != SOAD_INV_SO_CON_ID)
    {
      /* #1010 Get the activation count of the corresponding socket connection ID. */
      soConActivationMapIdx = Sd_UtilServer_GetActivationMapIdx(EventHandlerIdx, assignedTcpSoConId);

      if((soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap()) &&
        (Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx) != 0))
      {
        /* #10100 Decrement the activation counter. */
        Sd_DecNrOfActivationsOfSoConActivationMap(soConActivationMapIdx);                                               /* SBSW_SD_CSL01 */

        /* #10101 Deactivate the routing group if no other instance of the client is active. */
        if(Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx) == 0)
        {
          (void)SoAd_DisableSpecificRouting(
            Sd_GetActivationRefOfEventHandlerTcp(Sd_GetEventHandlerTcpIdxOfEventHandler(EventHandlerIdx)),
            assignedTcpSoConId);
        }
      }

      Sd_DecNrOfClientsOfEventHandlerDyn(EventHandlerIdx);                                                              /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_SetAssignedTcpSoConIdOfClientList(ClientListIdx, SOAD_INV_SO_CON_ID);                                          /* SBSW_SD_API_ARGUMENT_IDX */
    }

    if(assignedUdpSoConId != SOAD_INV_SO_CON_ID)
    {
      /* #1011 Get the activation count of the corresponding socket connection ID. */
      soConActivationMapIdx = Sd_UtilServer_GetActivationMapIdx(EventHandlerIdx, assignedUdpSoConId);

      if((soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap()) &&
        (Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx) != 0))
      {
        /* #10110 Decrement the activation counter. */
        Sd_DecNrOfActivationsOfSoConActivationMap(soConActivationMapIdx);                                               /* SBSW_SD_CSL01 */

        /* #10111 Deactivate the routing group if no other instance of the client is active. */
        if(Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx) == 0)
        {
          (void)SoAd_DisableSpecificRouting(
            Sd_GetActivationRefOfEventHandlerUdp(Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx)),
            assignedUdpSoConId);
        }
      }

      /* #1012 Close the socket connection. (The OpenCount is handled by SoAd). */
      (void)SoAd_CloseSoCon(assignedUdpSoConId, FALSE);
      Sd_DecNrOfClientsOfEventHandlerDyn(EventHandlerIdx);                                                              /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_DecNrOfClientsUdpOfEventHandlerDyn(EventHandlerIdx);                                                           /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_SetAssignedUdpSoConIdOfClientList(ClientListIdx, SOAD_INV_SO_CON_ID);                                          /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #20 Check if the client has pending events. */
  if((Sd_GetNrOfReferencesOfClientList(ClientListIdx) != 0) &&
    (Sd_GetClientStateOfClientList(ClientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED))
  {
    Sd_DecNrOfReferencesOfClientList(ClientListIdx);                                                                    /* SBSW_SD_API_ARGUMENT_IDX */
  }

  if(Sd_GetNrOfReferencesOfClientList(ClientListIdx) == 0)
  {
    /* #200 The client is not referenced by another event. Remove it from the list. */
    Sd_SetClientStateOfClientList(ClientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID);                                /* SBSW_SD_API_ARGUMENT_IDX */
  }
  else
  {
    /* #201 The client is referenced by another event. Keep the information in the list. */
    Sd_SetClientStateOfClientList(ClientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED);                           /* SBSW_SD_API_ARGUMENT_IDX */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_ReleaseEventHandlers()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ReleaseEventHandlers(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx;
  Sd_SizeOfClientListType clientListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the state of all associated event handlers. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx);
    eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx);
    eventHandlerIdx++)
  {
    /* #100 Inform BswM if event handler was requested. */
    if(Sd_GetRequestStateOfEventHandlerDyn(eventHandlerIdx) == SD_EVENT_HANDLER_REQUESTED)
    {
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, SD_EVENT_HANDLER_RELEASED);
    }

    /* #101 Clear the list of clients. */
    for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(eventHandlerIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(eventHandlerIdx);
      clientListIdx++)
    {
      if(Sd_GetClientStateOfClientList(clientListIdx) != SD_EVENT_HANDLER_CLIENT_STATE_INVALID)
      {
        Sd_UtilServer_RemoveClientFromList(eventHandlerIdx, clientListIdx);
      }
    }

    /* #102 Disable all referenced routing groups and reset the socket connections. */
    eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx);
    if(eventHandlerMulticastIdx != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)
    {
      (void)SoAd_DisableRouting(Sd_GetActivationRefOfEventHandlerMulticast(eventHandlerMulticastIdx));
      (void)SoAd_CloseSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx), FALSE);
    }

    /* #103 Disable routing of UDP and TCP routing groups and reset activation count. */
    Sd_UtilServer_DisableRouting(eventHandlerIdx);
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_CloseAllTcpSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseAllTcpSoCons(
  Sd_SizeOfServerType ServerIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_TcpSoConIdIterType tcpSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all TCP socket connections of the server. */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfServer(ServerIdx);
    tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfServer(ServerIdx);
    tcpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx), Abort);
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_CloseAllUdpSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseAllUdpSoCons(
  Sd_SizeOfServerType ServerIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_UdpSoConIdIterType udpSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all UDP socket connections of the server. */
  for(udpSoConIdIdx = Sd_GetUdpSoConIdStartIdxOfServer(ServerIdx);
    udpSoConIdIdx < Sd_GetUdpSoConIdEndIdxOfServer(ServerIdx);
    udpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetUdpSoConId(udpSoConIdIdx), Abort);
  }
}
#endif

#if(SD_METHODIDXOFSERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_DisableMethodRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_DisableMethodRouting(
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the server provides methods. */
  if(Sd_GetMethodIdxOfServer(ServerIdx) != SD_NO_METHODIDXOFSERVER)
  {
    /* #100 Disable the routing group for the server methods. */
    (void)SoAd_DisableRouting(Sd_GetActivationRefOfMethod(Sd_GetMethodIdxOfServer(ServerIdx)));
  }
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_LocalIpAddrAssignmentChgReset()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset state of all server services. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx);
    serverIdx++)
  {
# if(SD_EVENTHANDLER == STD_ON)
    Sd_SizeOfEventHandlerType eventHandlerIdx;

    /* #100 Reset state of all dependent event handler. */
    for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx);
      eventHandlerIdx++)
    {
      Sd_SizeOfClientListType clientListIdx;

      /* #1010 Reset dynamic variables. */
      Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NONE);
      Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0);                                                           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetNrOfClientsUdpOfEventHandlerDyn(eventHandlerIdx, 0);                                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

      /* #1011 Reset all clients. */
      for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(eventHandlerIdx);
        clientListIdx < Sd_GetClientListEndIdxOfEventHandler(eventHandlerIdx);
        clientListIdx++)
      {
        Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID);                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }

      /* #1012 Inform the BswM about the state of all event handlers. */
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, SD_EVENT_HANDLER_RELEASED);
    }
# endif

    /* #101 Reset the dynamic variables. */
    Sd_SetInitialOfferRepetitionRunOfServerDyn(serverIdx, 0);                                                           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(serverIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_GetActivationMapIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_SizeOfSoConActivationMapType, SD_CODE) Sd_UtilServer_GetActivationMapIdx(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType iterIdx;
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx = Sd_GetSizeOfSoConActivationMap();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the socket connection is already saved in the map. */
  for(iterIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx);
    iterIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx);
    iterIdx++)
  {
    if(Sd_GetSoConIdOfSoConActivationMap(iterIdx) == SoConId)
    {
      soConActivationMapIdx = iterIdx;
      break;
    }
  }

  /* #20 Requested socket connection was not saved. */
  if(soConActivationMapIdx == Sd_GetSizeOfSoConActivationMap())
  {
    /* #200 Search for an unused slot to save the socket connection. */
    for(iterIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx);
      iterIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx);
      iterIdx++)
    {
      /* Check if the entry is unused. */
      if(Sd_GetSoConIdOfSoConActivationMap(iterIdx) == SOAD_INV_SO_CON_ID)
      {
        /* #2000 Save the new socket connection. */
        Sd_SetSoConIdOfSoConActivationMap(iterIdx, SoConId);                                                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetNrOfActivationsOfSoConActivationMap(iterIdx, 0);                                                          /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        soConActivationMapIdx = iterIdx;
        break;
      }
    }
  }

  return soConActivationMapIdx;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_IncreaseActivationCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_IncreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  soConActivationMapIdx = Sd_UtilServer_GetActivationMapIdx(EventHandlerIdx, SoConId);

  /* #10 Increase the activation count. */
  if(soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap())
  {
    Sd_IncNrOfActivationsOfSoConActivationMap(soConActivationMapIdx);                                                   /* SBSW_SD_CSL01 */
  }
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_FindClientInList()
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
SD_LOCAL FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_FindClientInList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  boolean matchingClient = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is part of the list. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx);
    clientListIdx++)
  {
    if((Sd_GetClientStateOfClientList(clientListIdx) != SD_EVENT_HANDLER_CLIENT_STATE_INVALID) &&
      (Sd_GetAddressStorageIdxOfClientList(clientListIdx) == AddressStorageIdx))
    {
      /* #100 Found the corresponding remote address. Check if a remote reboot was detected. */
      if(AttachedOptionsPtr == NULL_PTR)
      {
        /* #1000 Client has rebooted.
         *       Set the SubscribedUntil time to 0 in order to remove the client in the next main function cycle. */
        Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx),
          Sd_GetInstanceIdxOfServer(Sd_GetServerIdxOfEventHandler(EventHandlerIdx)));                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      }
      else
      {
        /* #1001 Check if the attached endpoints have changed. */
        matchingClient = Sd_RxServer_CheckAttachedEndpointOptions(EventHandlerIdx, clientListIdx, AttachedOptionsPtr);  /* SBSW_SD_PASSED_API_POINTER_NOT_NULL */

        if(matchingClient == TRUE)
        {
          /* Client matches the attached endpoint options. */
          break;
        }
      }
    }
  }

  if(matchingClient == TRUE)
  {
    /* #200 Found matching client.
     *      Return the index of the client. */
  }
  else
  {
    /* #201 The client was not found and is not subscribed. Add the client to the list. */
    clientListIdx = Sd_RxServer_AddClientToList(EventHandlerIdx, AddressStorageIdx);
  }

  return clientListIdx;
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOptions(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tcpMatch, udpMatch;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the attached TCP endpoint addresses. Ignore if the endpoint is redundant. */

  if((AttachedOptionsPtr->EndpointAddrTcp.family != SOAD_AF_INVALID) && Sd_EventHandlerUsesTcp(EventHandlerIdx))
  {
    /* #100 Check the attached TCP endpoint. */
    tcpMatch = Sd_RxServer_CheckAttachedEndpointOption(Sd_GetAssignedTcpSoConIdOfClientList(ClientListIdx),
      Sd_GetEndpointStorageTcpIdxOfClientList(ClientListIdx), &AttachedOptionsPtr->EndpointAddrTcp);                    /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }
  else if((!Sd_EventHandlerUsesTcp(EventHandlerIdx)) ||
    (Sd_GetClientStateOfClientList(ClientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED))
  {
    /* #101 Ignore TCP endpoint if event handler is not configured for TCP. */
    tcpMatch = TRUE;
  }
  else
  {
    tcpMatch = FALSE;
  }

  /* #20 Check the attached UDP endpoint addresses. Ignore if the endpoint is redundant. */
  if((AttachedOptionsPtr->EndpointAddrUdp.family != SOAD_AF_INVALID) && Sd_EventHandlerUsesUdp(EventHandlerIdx))
  {
    /* #200 Check the attached UDP endpoint. */
    udpMatch = Sd_RxServer_CheckAttachedEndpointOption(Sd_GetAssignedUdpSoConIdOfClientList(ClientListIdx),
      Sd_GetEndpointStorageUdpIdxOfClientList(ClientListIdx), &AttachedOptionsPtr->EndpointAddrUdp);                    /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }
  else if((!Sd_EventHandlerUsesUdp(EventHandlerIdx)) ||
    (Sd_GetClientStateOfClientList(ClientListIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED))
  {
    /* #201 Ignore UDP endpoint if event handler is not configured for UDP. */
    udpMatch = TRUE;
  }
  else
  {
    udpMatch = FALSE;
  }

  return (boolean)(tcpMatch && udpMatch);
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOption(
  SoAd_SoConIdType SoConId,
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  SD_P2CONST(Sd_SockAddrContainerType) EndpointPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean endpointMatch = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already configured with a valid socket connection. */
  if(SoConId != SOAD_INV_SO_CON_ID)
  {
    Sd_SockAddrContainerType remoteAddr;

    /* #100 Check the configured remote address against the endpoint information. */
    endpointMatch = FALSE;

    /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_GetRemoteAddr(SoConId, (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(retVal == E_OK)
    {
      if(TRUE == Sd_Util_SockAddrIsEqual(&remoteAddr, EndpointPtr))                                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */
      {
        endpointMatch = TRUE;
      }
    }
  }
  else if(EndpointStorageIdx < Sd_GetSizeOfEndpointStorage())
  {
    /* #101 The client is not (yet) configured, but has a temporary endpoint. Check it. */
    endpointMatch = FALSE;

    if(TRUE == Sd_Util_SockAddrIsEqual(
      Sd_GetAddrAddrOfEndpointStorage(EndpointStorageIdx), EndpointPtr))                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT */
    {
      endpointMatch = TRUE;
    }
  }
  else
  {
    /* #102 The client contains no information (and does not match). */
  }

  return endpointMatch;
}
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Process(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfTriggerTransmitType triggerTransmitIdx = 0;
  Sd_SizeOfEventHandlerType eventHandlerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read all pending trigger transmit requests. */
  while(TRUE == Sd_TriggerTransmit_ReadFirst(&triggerTransmitIdx))                                                      /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    /* #100 Check if the transmit shall be triggered now or during the next main function execution. */
    if(Sd_GetStateOfTriggerTransmit(triggerTransmitIdx) == SD_TRIGGER_TRANSMIT_STATE_NOW)
    {
      Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfTriggerTransmit(triggerTransmitIdx);
      SoAd_SoConIdType soConId;
      SoAd_RoutingGroupIdType routingGroupId;

      eventHandlerIdx = Sd_GetEventHandlerIdxOfTriggerTransmit(triggerTransmitIdx);

      /* #1000 Trigger transmission of initial events. */

      soConId = Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx);

      if(soConId != SOAD_INV_SO_CON_ID)
      {
        routingGroupId = Sd_GetTriggerRefOfEventHandlerTcp(Sd_GetEventHandlerTcpIdxOfEventHandler(eventHandlerIdx));

        if(routingGroupId != SOAD_INV_ROUT_GRP_ID)
        {
          /* #10000 Trigger the initial transmission for TCP. */
          (void)SoAd_IfSpecificRoutingGroupTransmit(routingGroupId, soConId);
        }
      }

      soConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx);

      if(soConId != SOAD_INV_SO_CON_ID)
      {
        routingGroupId = Sd_GetTriggerRefOfEventHandlerUdp(Sd_GetEventHandlerUdpIdxOfEventHandler(eventHandlerIdx));

        if(routingGroupId != SOAD_INV_ROUT_GRP_ID)
        {
          /* #10001 Trigger the initial transmission for UDP. */
          (void)SoAd_IfSpecificRoutingGroupTransmit(routingGroupId, soConId);
        }
      }

      /* #1001 Remove the trigger transmit request. */
      Sd_TriggerTransmit_RemoveFirst();
    }
    else  /* SD_TRIGGER_TRANSMIT_STATE_NOW != Sd_GetStateOfTriggerTransmit(triggerTransmitIdx) */
    {
      /* #1002 Postpone the transmission to the next main function execution. */
      Sd_SetStateOfTriggerTransmit(triggerTransmitIdx, SD_TRIGGER_TRANSMIT_STATE_NOW);                                  /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      break;
    }
  }
}
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Add()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_TriggerTransmit_Add(
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType requestSaved = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer is full. */
  if(!Sd_IsTriggerTransmitFull())
  {
    /* #100 Save the requested trigger transmit. */
    Sd_SetClientListIdxOfTriggerTransmit(                                                                               /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      Sd_GetTriggerTransmitEndIdx(), Sd_GetClientListIdxOfRxEvent(RxEventIdx));
    Sd_SetEventHandlerIdxOfTriggerTransmit(                                                                             /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      Sd_GetTriggerTransmitEndIdx(), Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx));
    Sd_SetStateOfTriggerTransmit(                                                                                       /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      Sd_GetTriggerTransmitEndIdx(), SD_TRIGGER_TRANSMIT_STATE_NEXT_CYCLE);

    Sd_IncTriggerTransmitEndIdx();                                                                                      /* SBSW_SD_CSL_VAR_ELEM */
    requestSaved = E_OK;

    /* #101 Handle wrap around. */
    if(Sd_GetTriggerTransmitEndIdx() >= Sd_GetSizeOfTriggerTransmit())
    {
      Sd_SetTriggerTransmitEndIdx(0);                                                                                   /* SBSW_SD_CSL_VAR_ELEM */
    }

    /* #102 Check if the ring buffer is full. */
    if(Sd_GetTriggerTransmitEndIdx() == Sd_GetTriggerTransmitStartIdx())
    {
      Sd_SetTriggerTransmitFull(TRUE);                                                                                  /* SBSW_SD_CSL_VAR_ELEM */
    }
  }
  else
  {
    /* #103 The ring buffer is full. Not able to save the event. */
  }

  return requestSaved;
}
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_ReadFirst()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_TriggerTransmit_ReadFirst(
  SD_P2VAR(Sd_SizeOfTriggerTransmitType) TriggerTransmitIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean eventRead = FALSE;

  /* ----- Implementation ----------------------------------------------- */
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
  if(Sd_HasTriggerTransmit())
# endif
  {
    /* #10 Check if the ring buffer contains an element. */
    if((Sd_GetTriggerTransmitStartIdx() != Sd_GetTriggerTransmitEndIdx()) ||
      Sd_IsTriggerTransmitFull())
    {
      /* #100 Read the first element from of the buffer. */
      *TriggerTransmitIdxPtr =  Sd_GetTriggerTransmitStartIdx();                                                        /* SBSW_SD_VALID_API_POINTER */
      eventRead = TRUE;
    }
    else
    {
      /* #101 The buffer contains no elements. */
    }
  }

  return eventRead;
}
#endif

#if(SD_TRIGGERTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  Sd_TriggerTransmit_RemoveFirst()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_RemoveFirst(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 The ring buffer will always contain at least one element. */

  Sd_SetStateOfTriggerTransmit(                                                                                         /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
    Sd_GetTriggerTransmitStartIdx(), SD_TRIGGER_TRANSMIT_STATE_INVALID);

  /* #20 Remove the first element of the buffer. */
  Sd_IncTriggerTransmitStartIdx();                                                                                      /* SBSW_SD_CSL_VAR_ELEM */

  /* #30 Handle wrap around. */
  if(Sd_GetTriggerTransmitStartIdx() >= Sd_GetSizeOfTriggerTransmit())
  {
    Sd_SetTriggerTransmitStartIdx(0);                                                                                   /* SBSW_SD_CSL_VAR_ELEM */
  }

  /* #40 Check if the full flag has to be removed. */
  if(Sd_IsTriggerTransmitFull())
  {
    Sd_SetTriggerTransmitFull(FALSE);                                                                                   /* SBSW_SD_CSL_VAR_ELEM */
  }
}
#endif

/**********************************************************************************************************************
 *  CLIENT FUNCTIONS
 *********************************************************************************************************************/

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Init_ReInitializeClients()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeClients(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all clients to update and process the state machine. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx);
    clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx);
    clientIdx++)
  {
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

    /* #100 Iterate over all eventgroups, disable the routing and close multicast socket connections. */
    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(clientIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(clientIdx);
      consumedEventgroupIdx++)
    {
      SoAd_RoutingGroupIdType routingGroupId;

#  if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
      /* #1000 Initialize multicast configuration. */
      routingGroupId = Sd_GetMulticastActRefOfConsumedEventgroup(consumedEventgroupIdx);
      if(routingGroupId != SOAD_INV_ROUT_GRP_ID)
      {
        (void)SoAd_DisableRouting(routingGroupId);

        /* #10000 Close all multicast socket connections of the client. */
        Sd_UtilClient_CloseAllMulticastSoCons(consumedEventgroupIdx, TRUE);
      }
#  endif

      /* #1001 Initialize TCP configuration. */
      routingGroupId = Sd_GetTcpActRefOfConsumedEventgroup(consumedEventgroupIdx);
      if(routingGroupId != SOAD_INV_ROUT_GRP_ID)
      {
        /* #10010 Disable TCP routing. */
        (void)SoAd_DisableRouting(routingGroupId);
      }

      /* #1002 Initialize UDP configuration. */
      routingGroupId = Sd_GetUdpActRefOfConsumedEventgroup(consumedEventgroupIdx);
      if(routingGroupId != SOAD_INV_ROUT_GRP_ID)
      {
        /* #10020 Disable UDP routing. */
        (void)SoAd_DisableRouting(routingGroupId);
      }
    }
# endif

# if(SD_METHODIDXOFCLIENT == STD_ON)
    /* #101 Disable routing for method socket connection. */
    if(Sd_GetMethodIdxOfClient(clientIdx) != SD_NO_METHODIDXOFCLIENT)
    {
      (void)SoAd_DisableRouting(Sd_GetActivationRefOfMethod(Sd_GetMethodIdxOfClient(clientIdx)));
    }
# endif

    /* #102 Close all TCP socket connections. */
    Sd_UtilClient_CloseAllTcpSoCons(clientIdx, TRUE);

    /* #103 Close all UDP socket connections. */
    Sd_UtilClient_CloseAllUdpSoCons(clientIdx, TRUE);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Process(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute client state machine. */
  Sd_StateClient_ProcessStateMachine(ClientIdx);

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #20 Run the state machine for consumed eventgroups of the client instance. */
  Sd_StateClient_ProcessCeg(ClientIdx);
# endif

  /* #30 Check if a reboot was detected. */
  Sd_StateClient_HandleReboot(ClientIdx);

  /* #40 Check if the client is still requested. */
  Sd_StateClient_HandleRequestState(ClientIdx);

  /* #50 Update the time for the next state machine run. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfClientDyn(ClientIdx)))
  {
    Sd_Util_SetNextStateMachineRun(instanceIdx, Sd_GetTimerTTLOfClientDyn(ClientIdx));
  }

  Sd_Util_SetNextStateMachineRun(instanceIdx, Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx));
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStateMachine()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStateMachine(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute client state machine. */
  switch(Sd_GetStateMachineOfClientDyn(ClientIdx))
  {
    case SD_CLIENT_STATE_NONE:
      /* #100 Initial run of the state machine. */
      Sd_StateClient_ProcessInitial(ClientIdx);
      break;

    case SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN:
    case SD_CLIENT_STATE_NOT_REQUESTED_SEEN:
      /* 101 Not requested state machine. */
      Sd_StateClient_ProcessNotRequested(ClientIdx);
      break;

    case SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY:
      /* #102 Not ready state machine. */
      Sd_StateClient_ProcessRequestedNotReady(ClientIdx);
      break;

    case SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT:
      /* #103 Searching initial wait state machine. */
      Sd_StateClient_ProcessSearchingInitial(ClientIdx);
      break;

    case SD_CLIENT_STATE_SEARCHING_REPETITION:
      /* #104 Searching repetition state machine. */
      Sd_StateClient_ProcessSearchingRepetition(ClientIdx);
      break;

    case SD_CLIENT_STATE_SERVICE_READY:
      /* #105 Ready state machine. */
      Sd_StateClient_ProcessReady(ClientIdx);
      break;

    case SD_CLIENT_STATE_STOPPED:
      /* #106 Stopped state machine. */
      Sd_StateClient_ProcessStopped(ClientIdx);
      break;

    default:
      /* #107 Undefined state. */
      break;
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_HandleReboot()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleReboot(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a reboot was detected. */
  if(Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_MULTI)
  {
    /* #100 Reboot detected - set event to multicast OfferService. */
    Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_OFFER_MULTI);                                             /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_Util_TriggerStateMachineRun(instanceIdx);
  }
  else if(Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_UNI)
  {
    /* #101 Reboot detected - set event to unicast OfferService. */
    Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_OFFER_UNI);                                               /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_Util_TriggerStateMachineRun(instanceIdx);
  }
  else
  {
    /* #102 No reboot detected - reset the event. */
    Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                    /* SBSW_SD_API_ARGUMENT_IDX */
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_HandleRequestState()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleRequestState(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is not requested anymore. */
  if((Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_RELEASED) &&
    ((Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY) ||
    (Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_SERVICE_READY) ||
    (Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_STOPPED)))
  {
    Sd_SizeOfSendEntryType sendEntryIdx;

    /* #100 Check if there are pending SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP messages.
     *      Transmit the messages before the connections are reset (the endpoints become invalid). */
    for(sendEntryIdx = Sd_GetSendEntryStartIdxOfInstanceDyn(instanceIdx);
      sendEntryIdx != SD_NO_SENDENTRYENDIDXOFINSTANCEDYN;
      sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx))
    {
      if(Sd_GetEntryTypeOfSendEntry(sendEntryIdx) == SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP)
      {
        Sd_Tx_ProcessPendingMessages(instanceIdx, TRUE);
        break;
      }
    }

    /* #101 Enter the down phase and indicate the state. */
    Sd_SetCurrentStateOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_DOWN);                                                   /* SBSW_SD_API_ARGUMENT_IDX */
    if(Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_SERVICE_READY)
    {
      BswM_Sd_ClientServiceCurrentState(Sd_GetHandleIdOfClient(ClientIdx), SD_CLIENT_SERVICE_DOWN);
    }

    /* #102 The client is not requested, change the state. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    Sd_StateClient_Unconfigure(ClientIdx);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessInitial()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessInitial(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initial run of the state machine. */
  if(Sd_GetStateSwcOfClientDyn(ClientIdx) != SD_CLIENT_SERVICE_REQUESTED)
  {
    /* #100 Update the state of the released client. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
  }
  else if((Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED) /* &&
    (Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED) */ )
  {
    /* #101 Update the state if the instance is not ready. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, FALSE);
  }
  else  /* (Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED) && */
        /* (Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED) */
  {
    /* #102 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequested()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequested(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client has already seen the remote server. */
  if(Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN)
  {
    /* #100 OfferService message received for the first time. Update the configuration. */
    if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
    {
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
        Sd_GetOfferedTTLOfClientDyn(ClientIdx) * 1000, instanceIdx);                                                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_NOT_REQUESTED_SEEN, FALSE);

      /* #1000 Reset the event. */
      Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                  /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }
  else /* Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_NOT_REQUESTED_SEEN */
  {
    /* #101 Remote server was already seen. Update the state. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_DOWN) ||
      (TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimerTTLOfClientDyn(ClientIdx), FALSE)) ||
      ((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER)) /*lint !e655 */
    {
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                           /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetCurrentStateOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_DOWN);                                                 /* SBSW_SD_API_ARGUMENT_IDX */

      /* #1010 Check if a StopOffer was received or a reboot detected. */
      if(Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER)
      {
        /* #10100 StopOffer - Reset the event. */
        Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                /* SBSW_SD_API_ARGUMENT_IDX */
      }
    }
    else if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
    {
      /* #1011 Reset the timer and the event. */
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
        Sd_GetOfferedTTLOfClientDyn(ClientIdx) * 1000, instanceIdx);                                                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                  /* SBSW_SD_API_ARGUMENT_IDX */
    }
    else if((Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED) &&
      (Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED))
    {
      /* #1012 Enter the main phase and inform the BswM. */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_AVAILABLE);
    }
    else
    {
      /* #102 No further state transitions available. */
    }
  }

  /* #20 Check if the "Not Requested" block shall be left. */
  Sd_StateClient_ProcessLeaveNotRequested(ClientIdx);
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveNotRequested()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveNotRequested(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the condition to leave the "Not Requested" block is fulfilled. */
  if(((Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN) ||
    (Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_NOT_REQUESTED_SEEN)) &&
    (Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED))
  {
    if(Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
    {
      /* #100 Update the state. */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, FALSE);
    }
    else
    {
      /* #101 Update the state and reset the TimeToTriggerSend. */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
    }
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessRequestedNotReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessRequestedNotReady(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  if(Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED)
  {
    /* #10 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingInitial()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingInitial(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initial run of the wait phase. */
  if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx)))
  {
    /* #100 Wind up the timer. */
    uint32 AdditionalDelay = Sd_Util_GetRandomNrInRange(
      Sd_GetInitialFindDelayMinOfClientTimer(clientTimerIdx),
      Sd_GetInitialFindDelayMaxOfClientTimer(clientTimerIdx));
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx), AdditionalDelay, instanceIdx); /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx), TRUE))
  {
    /* #101 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_REPETITION, TRUE);
    Sd_SetRepetitionRunOfClientDyn(ClientIdx, 0);                                                                       /* SBSW_SD_API_ARGUMENT_IDX */

    /* #102 Send a FindService message. */
    Sd_Util_AddFindEntryToList(ClientIdx);
  }
  else
  {
    /* #103 Timer is not yet expired. */
  }

  /* #20 Check conditions to leave the "Search for Service" block. */
  Sd_StateClient_ProcessLeaveSearching(ClientIdx);
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingRepetition()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingRepetition(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the repetition phase was entered the first time. */
  if((Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx))) &&
    (Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx) > 0) && (Sd_GetRepetitionRunOfClientDyn(ClientIdx) == 0))
  {
    uint32 initialRepetitionDelay = Sd_GetInitialFindRepetitionBaseDelayOfClientTimer(clientTimerIdx);
    if(initialRepetitionDelay >= SD_MAIN_FCT_CYCLE_TIME_MS)
    {
      initialRepetitionDelay -= SD_MAIN_FCT_CYCLE_TIME_MS;
    }

    /* #100 Wind up the timer. */
    Sd_Util_SetTimerOffsetWithTrigger(
      Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx), initialRepetitionDelay, instanceIdx);                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #20 Succeeding runs of the repetition phase. */
  if((TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx), TRUE)) &&
    (Sd_GetRepetitionRunOfClientDyn(ClientIdx) < Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx)))
  {
    /* #200 Send a FindService message. */
    Sd_Util_AddFindEntryToList(ClientIdx);
    Sd_IncRepetitionRunOfClientDyn(ClientIdx);                                                                          /* SBSW_SD_API_ARGUMENT_IDX */

    /* #201 Wind up the timer. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx),
      (uint32)((uint32)(1 << Sd_GetRepetitionRunOfClientDyn(ClientIdx)) * Sd_GetInitialFindRepetitionBaseDelayOfClientTimer(clientTimerIdx)),
      instanceIdx);                                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else if(Sd_GetRepetitionRunOfClientDyn(ClientIdx) == Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx))
  {
    /* #202 End of the searching repetition phase. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_IncRepetitionRunOfClientDyn(ClientIdx);                                                                          /* SBSW_SD_API_ARGUMENT_IDX */
  }
  else
  {
    /* #203 Timer is not yet expired. */
  }

  /* #30 Check conditions to leave the "Search for Service" block. */
  Sd_StateClient_ProcessLeaveSearching(ClientIdx);
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearching()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearching(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the remote server was seen. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfClientDyn(ClientIdx)))
  {
    /* #100 Reset the TTL. */
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #20 Check if the instance is no more active. */
  if(Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
  {
    /* #200 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, TRUE);

    /* #201 Inform the BswM about the change of the client service instance and all associated eventgroups. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_DOWN);

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
    {
      Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

      for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx);
        consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx);
        consumedEventgroupIdx++)
      {
        BswM_Sd_ConsumedEventGroupCurrentState(
          Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx), SD_CONSUMED_EVENTGROUP_DOWN);
      }
    }
# endif
  }

  /* #30 Check if the client was released. */
  if(Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_RELEASED)
  {
    /* #300 The client service is not requested, change the state. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #40 Check if an OfferService message was received. */
  if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #400 Process the received OfferService message. */
    Sd_StateClient_ProcessLeaveSearchingRcvOffer(ClientIdx);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearchingRcvOffer()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearchingRcvOffer(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the received OfferService message offers a bigger TTL as configured. Use the smaller value. */
  if(Sd_GetOfferedTTLOfClientDyn(ClientIdx) < Sd_GetTTLOfClientTimer(clientTimerIdx))
  {
    /* #100 Update the timer with the offered TTL. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
      (uint32)(Sd_GetOfferedTTLOfClientDyn(ClientIdx) * 1000), instanceIdx);                                            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else
  {
    /* #101 Update the timer with the configured TTL. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
      (uint32)(Sd_GetTTLOfClientTimer(clientTimerIdx) * 1000), instanceIdx);                                            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

# if(SD_METHODIDXOFCLIENT == STD_ON)
  /* #20 Configure the client according to the received OfferService message. */
  if(Sd_StateClient_Configure(ClientIdx) == E_OK)
# endif
  {
    /* #200 Enter the main phase and inform the BswM. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SERVICE_READY, TRUE);
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_AVAILABLE);

    /* #201 The event is not reset because it is needed during processing of the eventgroups. */
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReady()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReady(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OfferService or StopOfferService message was received. */
  if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) != SD_CLIENT_EVENT_SERVICE_NONE) /*lint !e655 */
  {
    /* #100 Process the received message. */
    Sd_StateClient_ProcessReadyEventReceived(ClientIdx);
  }
# if(SD_METHODIDXOFCLIENT == STD_ON)
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  else if((Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx) != SOAD_INV_SO_CON_ID) &&
    (SD_SOCON_MODE_LOST_CONNECTION == Sd_Util_GetSoConMode(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx))))
  {
    /* #101 No valid connection, update the configuration. */
    (void)Sd_StateClient_Configure(ClientIdx);
  }
# endif
  else if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimerTTLOfClientDyn(ClientIdx), FALSE))
  {
    /* #102 TTL of server expired, unconfigure all connections. */
    Sd_StateClient_Unconfigure(ClientIdx);

    /* #103 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    /* #104 Report the status of the client service. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_DOWN);
  }
  else
  {
    /* #105 Timer is not yet expired. */
  }
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReadyEventReceived()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReadyEventReceived(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OfferService or StopOfferService message was received. */
  if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
# if(SD_METHODIDXOFCLIENT == STD_ON)
    /* #100 Received an OfferService message. Configure the client. */
    if(Sd_StateClient_Configure(ClientIdx) == E_OK)
# endif
    {
      /* #1000 Update the timer. */
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
        Sd_GetOfferedTTLOfClientDyn(ClientIdx) * 1000, instanceIdx);                                                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

      /* #1001 The event is not reset because it is needed during processing of the eventgroups. */
    }
# if(SD_METHODIDXOFCLIENT == STD_ON)
    else
    {
      /* #1002 The server has changed the attached endpoint addresses. */

      /* #1003 Send a FindService message. */
      Sd_Util_AddFindEntryToList(ClientIdx);

      /* #1004 Cancel the timer and reset the event. */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                           /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_STOPPED, FALSE);
      Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                  /* SBSW_SD_API_ARGUMENT_IDX */

      /* #1005 Report the status of the client service. */
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_DOWN);
    }
# endif
  }
  else
  {
    /* #101 Received SD_CLIENT_EVENT_SERVICE_STOP_OFFER message. Cancel the timer and update state machine. */
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), instanceIdx);                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_STOPPED, FALSE);

    /* #102 Report the status of the client service. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_DOWN);

    Sd_StateClient_Unconfigure(ClientIdx);

    /* #103 Check if a StopOffer was received or a reboot detected. */
    if(Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER)
    {
      /* #1030 StopOffer - Reset the event. */
      Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                  /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStopped()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStopped(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OfferService message was received. */
  if((Sd_GetEventOfClientDyn(ClientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #100 Update the timer and the event. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx),
      Sd_GetOfferedTTLOfClientDyn(ClientIdx) * 1000, instanceIdx);                                                      /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                    /* SBSW_SD_API_ARGUMENT_IDX */

# if(SD_METHODIDXOFCLIENT == STD_ON)
    if(Sd_StateClient_Configure(ClientIdx) == E_OK)
# endif
    {
      /* #1000 Enter the main phase and inform the BswM. */
      Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, SD_CLIENT_SERVICE_AVAILABLE);
    }
  }

  /* #20 Change the state if the connection is lost. */
  if(Sd_GetStateOfInstanceDyn(instanceIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
  {
    Sd_StateClient_Update(ClientIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, FALSE);
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCeg()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCeg(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle all eventgroups of the client service instance. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx);
    consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx);
    consumedEventgroupIdx++)
  {
    /* #100 Check if the service is ready. */
    if(Sd_GetStateMachineOfClientDyn(ClientIdx) == SD_CLIENT_STATE_SERVICE_READY)
    {
      /* #1000 Execute the state machine of the consumed eventgroup. */
      switch(Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx))
      {
        case SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED:
          /* #10000 Unsubscribed state machine. */
          Sd_StateClient_ProcessCegUnsubscribed(consumedEventgroupIdx);
          break;

        case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT:
        case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER:
        case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RETRY:
          /* #10001 Registration sent state machine. */
          Sd_StateClient_ProcessCegRegistrationSent(consumedEventgroupIdx);
          break;

        case SD_CONSUMED_EVENTGROUP_STATE_SUBSCRIBED:
          /* #10002 Subscribed state machine. */
          Sd_StateClient_ProcessCegSubscribed(consumedEventgroupIdx);
          break;

        default:
            /* #10003 Undefined state. */
          break;
      }

      /* #1001 Check if the TTL expired or the eventgroup was set from requested to released. */
      if(((TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx), FALSE)) &&
        (!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx))) &&
        (Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx) != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED)) ||
        ((Sd_GetCurrentStateOfConsumedEventgroupDyn(consumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_AVAILABLE) &&
        (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_RELEASED)) ||
        (Sd_GetEventOfConsumedEventgroupDyn(consumedEventgroupIdx) == SD_CLIENT_EVENT_EVENTGROUP_NACK))
      {
        /* #10010 Unconfigure consumed eventgroup. */
        Sd_StateClient_ReleaseCeg(consumedEventgroupIdx);
      }
    }
    else
    {
      /* #1002 The service instance is not ready. Reset the state of the consumed eventgroup. */
      Sd_StateClient_ProcessCegReset(consumedEventgroupIdx);
    }

    if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx)))
    {
      /* #1003 Update the time for the next state machine run. */
      Sd_Util_SetNextStateMachineRun(instanceIdx, Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx));
    }
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegReset(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The service instance is not ready. Reset the state of the eventgroup. Do not trigger the state machine execution. */
  Sd_SetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);          /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                           /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Report the state of all consumed eventgroups to BswM if the service was changed to down. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx)))
  {
    if(Sd_GetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SD_CONSUMED_EVENTGROUP_DOWN)
    {
      BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
        SD_CONSUMED_EVENTGROUP_DOWN);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                    /* SBSW_SD_API_ARGUMENT_IDX */
    }

    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx), instanceIdx);     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegUnsubscribed()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegUnsubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check it the eventgroup is requested */
  if(Sd_GetServiceModeRequestOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_REQUESTED)
  {
    retVal = Sd_StateClient_UpdateEventGroup(ConsumedEventgroupIdx);

    if(retVal == E_OK)
    {
      /* #100 Check if the OfferService message was transmitted by multicast or unicast.
       *      Messages received via multicast will be answered with an additional delay. */
      if(Sd_GetEventOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
      {
        /* Calculate a random delay after which the response message will be transmitted. */
        randDelay = Sd_Util_GetRandomNrInRangeForResponse(
          Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx),
          Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx),
          FALSE);
        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
      }
      else
      {
        randDelay = 0;
        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
      }

      /* #101 Send a subscribe eventgroup message. */
      Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, TRUE, randDelay);

      /* #102 Enable the unicast PDU routings in order to prevent the race condition discussed in RFC 68309. */
      if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
      {
        (void)SoAd_EnableSpecificRouting(Sd_GetTcpActRefOfConsumedEventgroup(ConsumedEventgroupIdx),
          Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx));
      }
      if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
      {
        (void)SoAd_EnableSpecificRouting(Sd_GetUdpActRefOfConsumedEventgroup(ConsumedEventgroupIdx),
          Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx));
      }

      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx), instanceIdx);   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSent(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a new event was received. */
  if(Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CLIENT_EVENT_EVENTGROUP_ACK)
  {
    /* #100 Received SubscribeEventgroupAck message. Process the message. */
    Sd_StateClient_ProcessCegRegistrationSentRxAck(ConsumedEventgroupIdx);
  }
  else if((Sd_GetEventOfClientDyn(clientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #101 Received OfferService message. Process the message. */
    Sd_StateClient_ProcessCegRegistrationSentRxOffer(ConsumedEventgroupIdx);
  }
  else
  {
    /* #102 No relevant message received and server is still available. Wait for response from server or if its TTL expires. */
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxAck()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxAck(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The server acknowledged the subscription, update the socket connections. */
# if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
  Std_ReturnType retVal = Sd_StateClient_UpdateEventGroupAckReceived(ConsumedEventgroupIdx);

  if(retVal == E_OK)
# endif
  {
    /* #100 Inform the BswM if the eventgroup was subscribed for the first time. */
    if(Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx)))
    {
      /* Inform the BswM. */
      BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
        SD_CONSUMED_EVENTGROUP_AVAILABLE);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_AVAILABLE);               /* SBSW_SD_API_ARGUMENT_IDX */
    }

    /* #101 Start the timers, update the state and reset the event. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx),
      Sd_GetAcknowledgedTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx) * 1000,
      instanceIdx);                                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_SUBSCRIBED);
    Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                         /* SBSW_SD_API_ARGUMENT_IDX */
  }
# if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
  else
  {
    /* #102 The multicast socket connection could not be configured. */
    BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
      SD_CONSUMED_EVENTGROUP_DOWN);
    if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RETRY)
    {
      /* #1020 If the multicast endpoint was changed, the Sd will try to configure it in the next cycle.
       *       The SD_CLIENT_EVENT_EVENTGROUP_ACK event is not removed. */
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RETRY);
    }
    else
    {
      /* #1021 Inform the BswM and update the state. */
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);
      Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                       /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }
# endif
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxOffer()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxOffer(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the previous subscribe message is already transmitted, or still pending because of a missing TCP connection. */
  if(TRUE == Sd_StateClient_EntryListContainsSubscribe(ConsumedEventgroupIdx))
  {
    /* #100 There is already a SubscribeEventgroup message pending.
     *      Do not trigger the StopSubscribeEventgroup - SubscribeEventgroup message. */
  }
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  else if((Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID) &&
    (Sd_Util_GetSoConMode(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx)) != SD_SOCON_MODE_ONLINE))
  {
    /* #101 The TCP connection is not established. Do not respond to the OfferService message. */
  }
  else
  {
    /* #102 Check if the OfferService message was transmitted by multicast or unicast. */
    if(Sd_GetEventOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
    {
      /* #1020 Add a random delay to the response message. */
      randDelay = Sd_Util_GetRandomNrInRangeForResponse(Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx),
        Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx), FALSE);

      if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER)
      {
        /* #10200 Send out a StopSubscribeEventgroup followed by a SubscribeEventgroup message. */
        Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, FALSE, randDelay);
      }

      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
    }
    else  /* Sd_GetEventOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_UNI */
    {
      /* #1021 The response can be transmitted without additional delay. */
      randDelay = 0;
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
    }

    /* #103 Send out a SubscribeEventgroup message. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, TRUE, randDelay);
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegSubscribed()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegSubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Send a SubscribeEventgroup message if an OfferService message was received. */
  if((Sd_GetEventOfClientDyn(clientIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #100 Check if the OfferService message was transmitted by multicast or unicast. */
    if(Sd_GetEventOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
    {
      /* #1000 Calculate a random delay after which the response message will be transmitted. */
      randDelay = Sd_Util_GetRandomNrInRangeForResponse(Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx),
        Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx), FALSE);
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
    }
    else  /* Sd_GetEventOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_UNI */
    {
      /* #1001 Subscribe messages based on a unicast offer are not delayed. */
      randDelay = 0;
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
    }

    /* #101 Send a subscribe eventgroup message and init the mechanism waiting for the ACK. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, TRUE, randDelay);
  }
  else if(Sd_GetStateSwcOfClientDyn(clientIdx) == SD_CLIENT_SERVICE_RELEASED)
  {
    /* #102 Send a StopSubscribe message and inform the BswM. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, FALSE, 0);
    BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
      SD_CONSUMED_EVENTGROUP_DOWN);
    Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                      /* SBSW_SD_API_ARGUMENT_IDX */
  }
  /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
  else if((Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID) &&
    (Sd_Util_GetSoConMode(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx)) == SD_SOCON_MODE_LOST_CONNECTION))
  {
    /* #103 No valid connection, update the configuration and inform the BswM. */
    (void)Sd_StateClient_UpdateEventGroup(ConsumedEventgroupIdx);

    BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
      SD_CONSUMED_EVENTGROUP_DOWN);
    Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                      /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);
  }
  else
  {
    /* #104 Timer is not yet expired. */
  }
}
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_METHODIDXOFCLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Configure()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_Configure(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx);
  SoAd_SoConIdType soConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client configures a consumed method using TCP. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) && Sd_IsTcpConfiguredOfMethod(methodIdx))
  {
    soConId = Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx);

    /* #100 Check if the TCP connection is still established. */
    /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
    if((soConId != SOAD_INV_SO_CON_ID) && (Sd_Util_GetSoConMode(soConId) == SD_SOCON_MODE_LOST_CONNECTION))
    {
      /* #1000 No valid connection established, remove the configuration. */
      (void)SoAd_CloseSoCon(soConId, TRUE);
      Sd_Util_SetSoConMode(soConId, SOAD_SOCON_OFFLINE);
      Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                                 /* SBSW_SD_API_ARGUMENT_IDX */
    }

    /* #101 Check if the remote server has offered the required endpoint. */
    if(Sd_GetServerTcpEndpointAddrOfClientDyn(ClientIdx).family != SOAD_AF_INVALID)
    {
      /* #1010 Configure the TCP socket connection. */
      retVal = Sd_StateClient_ConfigureMethod(ClientIdx, Sd_GetAddrMethodTcpSoConIdOfClientDyn(ClientIdx),
        Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfClient(ClientIdx)),
        Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx));                                                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
    else
    {
      /* #1011 The server has NOT offered all required endpoints. */
      retVal = E_NOT_OK;
    }
  }

  /* #20 Check if the client configures a consumed method using UDP. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) && Sd_IsUdpConfiguredOfMethod(methodIdx))
  {
    /* #200 Check if the remote server has offered the required endpoint. */
    if(Sd_GetServerUdpEndpointAddrOfClientDyn(ClientIdx).family != SOAD_AF_INVALID)
    {
      /* #2000 Configure the UDP socket connection. */
      retVal = Sd_StateClient_ConfigureMethod(ClientIdx, Sd_GetAddrMethodUdpSoConIdOfClientDyn(ClientIdx),
        Sd_GetUdpSoConId(Sd_GetUdpSoConIdStartIdxOfClient(ClientIdx)),
        Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx));                                                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
    else
    {
      /* #2001 The server has NOT offered all required endpoints. */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
# endif
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_METHODIDXOFCLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ConfigureMethod()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ConfigureMethod(
  Sd_SizeOfClientType ClientIdx,
  SD_P2VAR(SoAd_SoConIdType) MethodSoConIdPtr,
  SoAd_SoConIdType ServiceStartSoConId,
  SD_P2CONST(Sd_SockAddrContainerType) ServerEndpointAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx);
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already configured for consumed methods. */
  if(*MethodSoConIdPtr == SOAD_INV_SO_CON_ID)
  {
    /* #100 Configure a socket connection with the corresponding remote address. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
    retVal = SoAd_SetUniqueRemoteAddr(ServiceStartSoConId,
      (SD_P2VAR(SoAd_SockAddrType))ServerEndpointAddrPtr, MethodSoConIdPtr);                                            /* SBSW_SD_PASSING_API_POINTER */

    if((retVal == E_OK) && (*MethodSoConIdPtr != SOAD_INV_SO_CON_ID))
    {
      /* #1000 Each entity which requires the socket connection opens it in order to indicate the dependency. */
      retVal |= SoAd_OpenSoCon(*MethodSoConIdPtr);
      retVal |= SoAd_EnableSpecificRouting(Sd_GetActivationRefOfMethod(methodIdx), *MethodSoConIdPtr);
    }
    else
    {
      *MethodSoConIdPtr = SOAD_INV_SO_CON_ID;                                                                           /* SBSW_SD_VALID_API_POINTER */
    }
  }
  else
  {
    /* #101 The socket connection is already configured. Check if the server endpoint address has changed. */

    /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_GetRemoteAddr(*MethodSoConIdPtr, (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                           /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_NOT_OK) || (FALSE == Sd_Util_SockAddrIsEqual(ServerEndpointAddrPtr, &remoteAddr)))                  /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1010 The attached endpoint has changed. Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(*MethodSoConIdPtr, FALSE);
      (void)SoAd_DisableSpecificRouting(Sd_GetActivationRefOfMethod(methodIdx), *MethodSoConIdPtr);
      *MethodSoConIdPtr = SOAD_INV_SO_CON_ID;                                                                           /* SBSW_SD_VALID_API_POINTER */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx), Sd_GetInstanceIdxOfClient(ClientIdx));  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
# endif
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Unconfigure()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Unconfigure(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean abortFlag = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the renewal was triggered by reboot detection. */
  if((Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_MULTI) ||
    (Sd_GetEventOfClientDyn(ClientIdx) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_UNI))
  {
    /* #100 Do not remove the server endpoint addresses. These will be used during the next cycle to reestablish connections.
     *      Abort existing TCP connections because remote server has rebooted. */
    abortFlag = TRUE;
  }
  else
  {
    /* #101 Remove the saved endpoint addresses. */
    (Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                                  /* SBSW_SD_API_ARGUMENT_IDX */
    (Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                                  /* SBSW_SD_API_ARGUMENT_IDX */
    (Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                            /* SBSW_SD_API_ARGUMENT_IDX */
  }

# if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #20 Iterate over all eventgroups, disable the routing groups and close the socket connections. */
  {
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx);
      consumedEventgroupIdx++)
    {
      Sd_StateClient_UnconfigureCeg(consumedEventgroupIdx, abortFlag);
    }
  }
# endif

# if(SD_METHODIDXOFCLIENT == STD_ON)
  {
    Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx);

    /* #30 Disable the SoAd routing groups for methods and close the socket connections. */
    if((methodIdx != SD_NO_METHODIDXOFCLIENT) &&
      ((Sd_IsTcpConfiguredOfMethod(methodIdx)) || (Sd_IsUdpConfiguredOfMethod(methodIdx))))
    {
      (void)SoAd_DisableRouting(Sd_GetActivationRefOfMethod(methodIdx));

      if(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx) != SOAD_INV_SO_CON_ID)
      {
        /* #300 Unconfigure TCP for consumed methods. */
        (void)SoAd_CloseSoCon(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx), abortFlag);
        Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                               /* SBSW_SD_API_ARGUMENT_IDX */
      }

      if(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx) != SOAD_INV_SO_CON_ID)
      {
        /* #301 Unconfigure UDP for consumed methods. */
        (void)SoAd_CloseSoCon(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx), abortFlag);
        Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                               /* SBSW_SD_API_ARGUMENT_IDX */
      }
    }
  }
# endif
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroup(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the consumed eventgroup is configured for TCP. */
  if(Sd_ConsumedEventGroupUsesTcp(ConsumedEventgroupIdx))
  {
    /* #100 Update the TCP configuration. */
    retVal |= Sd_StateClient_UpdateEventGroupTcp(ConsumedEventgroupIdx);
  }

  /* #20 Check if the consumed eventgroup is configured for UDP. */
  if(Sd_ConsumedEventGroupUsesUdp(ConsumedEventgroupIdx) && (retVal == E_OK))
  {
    /* #200 Update the UDP configuration. */
    retVal |= Sd_StateClient_UpdateEventGroupUdp(ConsumedEventgroupIdx);
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupTcp()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupTcp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  SoAd_SoConIdType soConId;
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the TCP connection is still established. */
  soConId = Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx);
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    if(Sd_Util_GetSoConMode(soConId) == SD_SOCON_MODE_LOST_CONNECTION)
    {
      /* #1000 No valid connection, remove the configuration. */
      (void)SoAd_CloseSoCon(soConId, TRUE);
      Sd_Util_SetSoConMode(soConId, SOAD_SOCON_OFFLINE);
      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                               /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #20 Check if the endpoint address has changed. */
  soConId = Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx);
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_GetRemoteAddr(soConId, (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (FALSE == Sd_Util_SockAddrIsEqual(Sd_GetAddrServerTcpEndpointAddrOfClientDyn(clientIdx), &remoteAddr)))           /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #2000 The attached endpoint has changed. Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(soConId, FALSE);
      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                               /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(clientIdx), Sd_GetInstanceIdxOfClient(clientIdx));  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #201 Try to establish a connection to the server. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_SetUniqueRemoteAddr(Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfClient(clientIdx)),
      (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerTcpEndpointAddrOfClientDyn(clientIdx),
      &assignedSoConId);                                                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */

    if(retVal == E_OK)
    {
      /* #2010 The requested IP address could be set or is already set. */

      /* #2011 Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      retVal |= SoAd_OpenSoCon(assignedSoConId);

      /* #2012 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, assignedSoConId);                                  /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #30 Check if the server endpoint address could be saved. */
  if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* PRQA S 6050 */ /* MD_Sd_STCAL_6050 */
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupUdp()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupUdp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  SoAd_SoConIdType soConId;
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the endpoint address has changed. */
  soConId = Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx);
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_GetRemoteAddr(soConId, (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (FALSE == Sd_Util_SockAddrIsEqual(Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx), &remoteAddr)))           /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1000 The attached endpoint has changed.
       *       Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(soConId, FALSE);
      Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                               /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(clientIdx), Sd_GetInstanceIdxOfClient(clientIdx));  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #101 Check if a socket connection is already configured with the correct server IP address or configure a new one. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_SetUniqueRemoteAddr(Sd_GetUdpSoConId(Sd_GetUdpSoConIdStartIdxOfClient(clientIdx)),
      (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx),
      &assignedSoConId);                                                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    if(retVal == E_OK)
    {
      /* #1010 The requested IP address could be set or is already set. */

      /* #1011 Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      retVal |= SoAd_OpenSoCon(assignedSoConId);

      /* #1012 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, assignedSoConId);                                  /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #20 Check if the server endpoint address could be saved. */
  if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceived()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceived(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the multicast configuration. */
  if(Sd_ConsumedEventGroupUsesMulticast(ConsumedEventgroupIdx))
  {
    /* #100 Check if the client is already configured for multicast reception. */
    if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #1000 Check the the new endpoint option matches the current configuration. */
      retVal |= Sd_StateClient_UpdateEventGroupAckReceivedRenew(ConsumedEventgroupIdx);
    }
    else
    {
      /* #1001 Configure consumed eventgroup for multicast. */
      retVal |= Sd_StateClient_UpdateEventGroupAckReceivedInitial(ConsumedEventgroupIdx);
    }

    if(retVal == E_OK)
    {
      /* #1002 Enable the multicast PDU routing. */
      if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
      {
        retVal |= SoAd_EnableSpecificRouting(Sd_GetMulticastActRefOfConsumedEventgroup(ConsumedEventgroupIdx),
          Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx));
      }
    }
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedInitial()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedInitial(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfMulticastSoConIdType multicastSoConIdIdx;
  Sd_LocalRemoteMatchTypeType matchType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all available socket connections and find the one which matches best to the server. */
  retVal = Sd_StateClient_FindBestMatchLocalRemoteSoCon(ConsumedEventgroupIdx, &multicastSoConIdIdx, &matchType);       /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Configure the local address according to the server multicast endpoint. */
    if((matchType.LocalIpMatchType != SD_MATCH_TRUE) || (matchType.LocalPortMatchType != SD_MATCH_TRUE))
    {
      /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
      if((matchType.LocalIpMatchType != SD_MATCH_TRUE) &&
        (TRUE == Sd_StateClient_LocalIpAlreadyConfigured(Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx)))) /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      {
        /* #10000 The requested multicast IP address is already configured at another socket. */
        retVal = E_NOT_OK;
      }
      else
      {
        /* #10001 Request the local address according to the endpoint. */
        retVal |= Sd_StateClient_RequestIpAddrAssignment(Sd_GetMulticastSoConId(multicastSoConIdIdx),
          Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx), TRUE);                                           /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      }
    }
    else
    {
      /* #1001 The local address is already configured. Increment the request counter. */
      retVal |= Sd_StateClient_RequestIpAddrAssignment(Sd_GetMulticastSoConId(multicastSoConIdIdx),
        Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx), FALSE);                                            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
    }

    if(retVal == E_OK)
    {
      /* #1002 Check if the remote address has to be configured. */
      if((matchType.RemoteIpMatchType != SD_MATCH_TRUE) || (matchType.RemotePortMatchType != SD_MATCH_TRUE))
      {
        /* #10020 Configure the remote address according to the server unicast endpoint. */
        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        retVal |= SoAd_SetRemoteAddr(Sd_GetMulticastSoConId(multicastSoConIdIdx),
          (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx));                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      }

      /* #1003 Open the socket connection. Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      retVal |= SoAd_OpenSoCon(Sd_GetMulticastSoConId(multicastSoConIdIdx));

      /* #1004 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, Sd_GetMulticastSoConId(multicastSoConIdIdx));/* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #20 Check if the server endpoint address could be saved. */
  if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;

    /* #200 Check if no valid multicast socket connection was found because of a missing multicast endpoint. */
    if(Sd_GetServerMulticastEndpointAddrOfClientDyn(clientIdx).family == SOAD_AF_INVALID)
    {
      /* #2000 Accept the acknowledge if the unicast configuration was successful. */
      if((Sd_ConsumedEventGroupUsesUdp(ConsumedEventgroupIdx) &&
        (Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)) ||
        (Sd_ConsumedEventGroupUsesTcp(ConsumedEventgroupIdx) &&
        (Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)))
      {
        /* #20000 Accept the acknowledge. */
        retVal = E_OK;
      }
    }
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedRenew()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedRenew(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  SoAd_SoConIdType soConId = Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx);

  /* ----- Implementation ----------------------------------------------- */
  Sd_SockAddrContainerType localAddr;
  Sd_SockAddrContainerType defaultRouter;
  uint8 netmaskCidr = 0;

  /* #10 Get the local address. */
  /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetLocalAddr(soConId,
    (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if the local endpoint address has changed. */
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  if((retVal == E_OK) &&
    (FALSE == Sd_Util_SockAddrIsEqual(Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx), &localAddr)))        /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
  {
    /* #200 The attached endpoint has changed. Unconfigure the local address. */
    (void)Sd_StateClient_ReleaseIpAddrAssignment(soConId);

    (void)SoAd_DisableSpecificRouting(Sd_GetMulticastActRefOfConsumedEventgroup(ConsumedEventgroupIdx), soConId);
    (void)SoAd_CloseSoCon(soConId, FALSE);
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                           /* SBSW_SD_API_ARGUMENT_IDX */

    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoCon()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoCon(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_SizeOfMulticastSoConIdType) MulticastSoConIdPtr,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) MatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfMulticastSoConIdType tmpMulticastSoConIdIdx, bestMulticastSoConIdIdx = Sd_GetSizeOfMulticastSoConId();
  uint8 localMatchPrio = 0, remoteMatchPrio = 0;
  Sd_LocalRemoteMatchTypeType bestMatchType;

  /* ----- Implementation ----------------------------------------------- */
  MatchTypePtr->LocalIpMatchType = SD_MATCH_FALSE;                                                                      /* SBSW_SD_VALID_API_POINTER */
  MatchTypePtr->LocalPortMatchType = SD_MATCH_FALSE;                                                                    /* SBSW_SD_VALID_API_POINTER */
  MatchTypePtr->RemoteIpMatchType = SD_MATCH_FALSE;                                                                     /* SBSW_SD_VALID_API_POINTER */
  MatchTypePtr->RemotePortMatchType = SD_MATCH_FALSE;                                                                   /* SBSW_SD_VALID_API_POINTER */
  bestMatchType = *MatchTypePtr;

  /* #10 Calculate the match priority for all configured multicast socket connections. */
  for(tmpMulticastSoConIdIdx = Sd_GetMulticastSoConIdStartIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
    tmpMulticastSoConIdIdx < Sd_GetMulticastSoConIdEndIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
    tmpMulticastSoConIdIdx++)
  {
    /* #100 Calculate the match type of the local address. */
    retVal = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
      ConsumedEventgroupIdx, tmpMulticastSoConIdIdx, MatchTypePtr);                                                     /* SBSW_SD_PASSING_API_POINTER */

    /* #101 Calculate the match type of the remote address. */
    retVal |= Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
      ConsumedEventgroupIdx, tmpMulticastSoConIdIdx, MatchTypePtr);                                                     /* SBSW_SD_PASSING_API_POINTER */

    /* #102 Check if the socket connection can be (re)configured to the expected values. */
    if((retVal == E_OK) &&
      (MatchTypePtr->LocalIpMatchType != SD_MATCH_FALSE) && (MatchTypePtr->LocalPortMatchType != SD_MATCH_FALSE) &&
      (MatchTypePtr->RemoteIpMatchType != SD_MATCH_FALSE) && (MatchTypePtr->RemotePortMatchType != SD_MATCH_FALSE))
    {
      /* #1020 The socket connection can be (re)configured. Check the priority of the local and remote match.  */
      /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
      if((Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(MatchTypePtr, &localMatchPrio, &remoteMatchPrio) == TRUE) &&
        (Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(MatchTypePtr, &remoteMatchPrio) == TRUE))      /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      {
        /* #10200 Found better matching socket connection. */
        bestMulticastSoConIdIdx = tmpMulticastSoConIdIdx;
        bestMatchType = *MatchTypePtr;
      }
    }
  }

  /* #20 Check if a feasible socket connection was found. */
  if((localMatchPrio != 0) && (remoteMatchPrio != 0))
  {
    /* #200 Found a feasible socket connection, update values. */
    *MulticastSoConIdPtr = bestMulticastSoConIdIdx;                                                                     /* SBSW_SD_VALID_API_POINTER */
    *MatchTypePtr = bestMatchType;                                                                                      /* SBSW_SD_VALID_API_POINTER */
    retVal = E_OK;
  }
  else
  {
    /* #201 No socket connection found which matches the given IP address. */
    *MulticastSoConIdPtr = Sd_GetSizeOfMulticastSoConId();                                                              /* SBSW_SD_VALID_API_POINTER */
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(uint8, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(
  SD_P2CONST(Sd_SockAddrContainerType) AddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 addrState;

  /* #10 Calculate the state of the port. */
  if(AddrPtr->port == SOAD_PORT_ANY)
  {
    addrState = SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_PORT;
  }
  else
  {
    addrState = SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_PORT;
  }

  /* #20 Calculate the state of the address. */
  if(AddrPtr->family == SOAD_AF_INET)
  {
    if(AddrPtr->data[0] == SOAD_IPADDR_ANY)
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR));
    }
    else
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_IPADDR));
    }
  }
  else if(AddrPtr->family == SOAD_AF_INET6)
  {
    if((AddrPtr->data[0] == SOAD_IPADDR_ANY) && (AddrPtr->data[1] == SOAD_IPADDR_ANY) &&
      (AddrPtr->data[2] == SOAD_IPADDR_ANY) && (AddrPtr->data[3] == SOAD_IPADDR_ANY))
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR));
    }
    else
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_IPADDR));
    }
  }
  else
  {
    /* Undefined domain type. */
  }

  return addrState;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) LocalMatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SockAddrContainerType localAddr;
  Sd_SockAddrContainerType defaultRouter;
  uint8 netmaskCidr = 0;
  uint8 localAddrState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured local IP address. */
  /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetLocalAddr(Sd_GetMulticastSoConId(MulticastSoConIdIdx),
    (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Calculate the state of the local address. */
    localAddrState = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(&localAddr);                             /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Compare the local address with the configured value and calculate a match type. */
    LocalMatchTypePtr->LocalIpMatchType = Sd_Util_CompareSockAddrIp(
      Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx),
      &localAddr, localAddrState);                                                                                      /* SBSW_SD_VALID_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    LocalMatchTypePtr->LocalPortMatchType = Sd_Util_CompareSockAddrPort(
      Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(clientIdx),
      &localAddr, localAddrState);                                                                                      /* SBSW_SD_VALID_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) RemoteMatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SockAddrContainerType remoteAddr;
  uint8 remoteAddrState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured remote IP address. */
  /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetRemoteAddr(Sd_GetMulticastSoConId(MulticastSoConIdIdx), (SD_P2VAR(SoAd_SockAddrType))&remoteAddr);   /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Calculate the state of the remote address. */
    remoteAddrState = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(&remoteAddr);                           /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Compare the remote address with the configured value and calculate a match type. */
    RemoteMatchTypePtr->RemoteIpMatchType = Sd_Util_CompareSockAddrIp(
      Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx), &remoteAddr, remoteAddrState);                             /* SBSW_SD_VALID_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    RemoteMatchTypePtr->RemotePortMatchType = Sd_Util_CompareSockAddrPort(
      Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx), &remoteAddr, remoteAddrState);                             /* SBSW_SD_VALID_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) LocalMatchPrioPtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean localMatch = FALSE;
  uint8 initialLocalMatchPrio = *LocalMatchPrioPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the match priority for the local IP address. */
  if((MatchTypePtr->LocalIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_TRUE))
  {
    *LocalMatchPrioPtr = 4;                                                                                             /* SBSW_SD_VALID_API_POINTER */
    localMatch = TRUE;
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_ANY))
  {
    if(*LocalMatchPrioPtr <= 3)
    {
      *LocalMatchPrioPtr = 3;                                                                                           /* SBSW_SD_VALID_API_POINTER */
      localMatch = TRUE;
    }
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_TRUE))
  {
    if(*LocalMatchPrioPtr <= 2)
    {
      *LocalMatchPrioPtr = 2;                                                                                           /* SBSW_SD_VALID_API_POINTER */
      localMatch = TRUE;
    }
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_ANY))
  {
    if(*LocalMatchPrioPtr <= 1)
    {
      *LocalMatchPrioPtr = 1;                                                                                           /* SBSW_SD_VALID_API_POINTER */
      localMatch = TRUE;
    }
  }
  else
  {
    /* #100 The socket connections do not match. */
  }

  /* #20 Check if the priority of the local match could be improved. */
  if(*LocalMatchPrioPtr > initialLocalMatchPrio)
  {
    /* #200 Reset the remote priority in order to calculate it again for the new socket connection. */
    *RemoteMatchPrioPtr = 0;                                                                                            /* SBSW_SD_VALID_API_POINTER */
  }

  return localMatch;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean betterMatchFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the match priority for the remote IP address. */
  if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_TRUE))
  {
    *RemoteMatchPrioPtr = 4;                                                                                            /* SBSW_SD_VALID_API_POINTER */
    betterMatchFound = TRUE;
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_ANY))
  {
    if(*RemoteMatchPrioPtr < 3)
    {
      *RemoteMatchPrioPtr = 3;                                                                                          /* SBSW_SD_VALID_API_POINTER */
      betterMatchFound = TRUE;
    }
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_TRUE))
  {
    if(*RemoteMatchPrioPtr < 2)
    {
      *RemoteMatchPrioPtr = 2;                                                                                          /* SBSW_SD_VALID_API_POINTER */
      betterMatchFound = TRUE;
    }
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_ANY))
  {
    if(*RemoteMatchPrioPtr < 1)
    {
      *RemoteMatchPrioPtr = 1;                                                                                          /* SBSW_SD_VALID_API_POINTER */
      betterMatchFound = TRUE;
    }
  }
  else
  {
    /* #100 The socket connections do not match. */
  }

  return betterMatchFound;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseCeg()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ReleaseCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Unconfigure consumed eventgroup. */
  Sd_StateClient_UnconfigureCeg(ConsumedEventgroupIdx, FALSE);

  /* #20 Check if the consumed eventgroup was released by BswM. */
  if((Sd_GetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_AVAILABLE) &&
  (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_RELEASED))
  {
    /* #200 The eventgroup was set from requested to released, send a StopSubscribe message. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, FALSE, 0);
  }
  else
  {
    /* #201 Inform the BswM about the state change. */
    BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx),
      SD_CONSUMED_EVENTGROUP_DOWN);

    if(Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx) == SD_CLIENT_EVENT_EVENTGROUP_NACK)
    {
      /* #2010 Report DEM if a NACK was received. */
      Sd_Util_ReportDemEvent(Sd_GetENackRecvIdOfInstance(instanceIdx));
      Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                       /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #30 Reset the state of the eventgroup. */
  Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);
  Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                        /* SBSW_SD_API_ARGUMENT_IDX */
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Update(
  Sd_SizeOfClientType ClientIdx,
  Sd_ClientStateMachineType NewState,
  boolean ResetTimeToTriggerSend)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfClientDyn(ClientIdx, NewState);                                                                   /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Set a timer event. */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(ClientIdx));

  if(ResetTimeToTriggerSend == TRUE)
  {
    /* #200 Reset the TimeToTriggerSend. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UpdateCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_UpdateCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_ConsumedEventGroupStateMachineType NewState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, NewState);                                           /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Set a timer event. */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx)));
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_UnconfigureCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_UnconfigureCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean AbortFlag)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Unconfigure TCP configuration. */
  if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)SoAd_DisableRouting(Sd_GetTcpActRefOfConsumedEventgroup(ConsumedEventgroupIdx));
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx), AbortFlag);
    Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                                 /* SBSW_SD_API_ARGUMENT_IDX */
  }

  /* #20 Unconfigure UDP configuration. */
  if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)SoAd_DisableRouting(Sd_GetUdpActRefOfConsumedEventgroup(ConsumedEventgroupIdx));
    (void)SoAd_CloseSoCon(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx), AbortFlag);
    Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                                 /* SBSW_SD_API_ARGUMENT_IDX */
  }

# if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
  /* #30 Unconfigure multicast configuration. */
  if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)SoAd_DisableRouting(Sd_GetMulticastActRefOfConsumedEventgroup(ConsumedEventgroupIdx));
    (void)Sd_StateClient_ReleaseIpAddrAssignment(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx));
    (void)SoAd_CloseSoCon(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx), AbortFlag);
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID);                           /* SBSW_SD_API_ARGUMENT_IDX */
  }
# endif
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_LocalIpAlreadyConfigured()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_LocalIpAlreadyConfigured(
  SD_P2CONST(Sd_SockAddrContainerType) MulticastAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_IpAddrRequestIterType ipAddressRequestIdx;
  Std_ReturnType retVal;
  Sd_SockAddrContainerType localAddr;
  Sd_SockAddrContainerType defaultRouter;
  uint8 netmaskCidr = 0;
  boolean localIpConfigured = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for(ipAddressRequestIdx = 0; ipAddressRequestIdx < Sd_GetSizeOfIpAddrRequest(); ipAddressRequestIdx++)
  {
    /* #10 Get the configured local IP address. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_GetLocalAddr(Sd_GetSoConIdOfIpAddrRequest(ipAddressRequestIdx),
      (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);              /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #20 Check if the local address is already configured. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (Sd_Util_CompareSockAddrIp(&localAddr, MulticastAddressPtr, SOAD_SOCON_IP_SET_PORT_SET) == SD_MATCH_TRUE))        /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */
    {
      /* #200 The multicast IP address is already configured. */
      localIpConfigured = TRUE;
      break;
    }
  }

  return localIpConfigured;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_RequestIpAddrAssignment(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) MulticastAddrPtr,
  boolean PerformRequest)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfIpAddrRequestType ipAddrRequestIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the corresponding local IP address request count. */
  for(ipAddrRequestIdx = 0; ipAddrRequestIdx < Sd_GetSizeOfIpAddrRequest(); ipAddrRequestIdx++)
  {
    if(Sd_GetSoConIdOfIpAddrRequest(ipAddrRequestIdx) == SoConId)
    {
      retVal = E_OK;

      /* #100 Check if the IP address is already requested. */
      if(((!Sd_IsPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx))) ||
        (Sd_GetRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx)) == 0)) &&
        (PerformRequest == TRUE))
      {
        /* #1000 The multicast address and port have to be requested. */
        Sd_SockAddrContainerType defaultRouter;

        /* Create an unspecified address for the default router and net mask. */
        uint8 netmaskCidr = 0;
        defaultRouter.family = MulticastAddrPtr->family;
        defaultRouter.data[0] = 0;                                                                                      /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[1] = 0;                                                                                      /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[2] = 0;                                                                                      /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[3] = 0;                                                                                      /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.port = 0;

        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        retVal = SoAd_RequestIpAddrAssignment(SoConId, SOAD_IPADDR_ASSIGNMENT_STATIC,
          (SD_P2VAR(SoAd_SockAddrType))MulticastAddrPtr, netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);     /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      if(retVal == E_OK)
      {
        /* #1001 Increment the request counter. */
        Sd_IncRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx));                                         /* SBSW_SD_CSL03 */
        Sd_SetPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx), TRUE);                                         /* SBSW_SD_CSL03 */
      }

      break;
    }
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseIpAddrAssignment()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ReleaseIpAddrAssignment(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfIpAddrRequestType ipAddrRequestIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for the request count of the given socket connection index. */
  for(ipAddrRequestIdx = 0; ipAddrRequestIdx < Sd_GetSizeOfIpAddrRequest(); ipAddrRequestIdx++)
  {
    if(Sd_GetSoConIdOfIpAddrRequest(ipAddrRequestIdx) == SoConId)
    {
      retVal = E_OK;

      /* #100 Check if the IP address is also assigned by another client. */
      if(Sd_GetRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx)) == 1)
      {
        if(!Sd_IsStaticIpConfigurationOfIpAddrRequest(ipAddrRequestIdx))
        {
          /* #1000 The multicast address is only requested by a single client. Release the request. */
          retVal = SoAd_ReleaseIpAddrAssignment(SoConId);
        }
      }

      /* #101 Reset the port set configuration. */
      if(!Sd_IsStaticPortConfigurationOfIpAddrRequest(ipAddrRequestIdx))
      {
        Sd_SetPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx), FALSE);                                        /* SBSW_SD_CSL03 */
      }

      /* #102 Decrement the request counter. (Even if the SoAd_ReleaseIpAddrAssignment fails). */
      Sd_DecRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx));                                           /* SBSW_SD_CSL03 */

      break;
    }
  }

  return retVal;
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_EntryListContainsSubscribe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_EntryListContainsSubscribe(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SendEntryIterType sendEntryIdx;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx));
  boolean ContainsSubscribe = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all possible entries. */
  for(sendEntryIdx = Sd_GetSendEntryStartIdxOfInstanceDyn(instanceIdx);
    sendEntryIdx != SD_NO_SENDENTRYENDIDXOFINSTANCEDYN;
    sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx))
  {
    /* #100 Check if the entry is a Subscribe-Entry for the given ConsumedEventGroup. */
    if((Sd_GetEntryTypeOfSendEntry(sendEntryIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP) &&
      (ConsumedEventgroupIdx == Sd_GetConsumedEventgroupIdxOfSendEntry(sendEntryIdx)))
    {
      ContainsSubscribe = TRUE;
      break;
    }
  }

  return ContainsSubscribe;
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntry()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntry(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;
  Sd_EntryType rxEntryType;
  uint16 rxServiceId;
  uint16 rxInstanceId;
  uint8 rxMajorVersion;
  uint32 rxMinorVersion;

  /* ----- Implementation ----------------------------------------------- */
  SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TYPE_OFFSET, rxEntryType);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);

  /* #10 Iterate over all clients to find the one corresponding to the entry. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx);
    clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx);
    clientIdx++)
  {
    /* #100 Check if the ServiceId, InstanceId and MajorVersion match. */
    if((rxServiceId == Sd_GetServiceIdOfClient(clientIdx)) &&
      (rxInstanceId == Sd_GetInstanceIdOfClient(clientIdx)) &&
      (rxMajorVersion == Sd_GetMajorVersionOfClient(clientIdx)))
    {
      /* #1000 Differentiate between entry format type 1 or 2 */
      if(rxEntryType == SD_ENTRY_OFFER_SERVICE)
      {
        /* Entry format type 1 - OfferService. */
        /* #10000 Process the entry even if the service is not available. */

        SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY1_MINORVERSION_OFFSET, rxMinorVersion);

        /* #10001 Check the minor version fits to the configuration. */
        if((rxMinorVersion == Sd_GetMinorVersionOfClient(clientIdx)) || Sd_IsMinorVersionWildcardOfClient(clientIdx))
        {
          /* #100010 Process the received OfferService entry. */
          Sd_RxClient_ProcessEntryOffer(RxMessageInfoPtr, IndexOfEntry, clientIdx);                                     /* SBSW_SD_PASSING_API_POINTER */
        }
      }
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
      else
      {
        /* Entry format type 2 - SubscribeEventgroup(N)Ack. */

        /* #10002 Process the acknowledge only if no reboot of the remote server was detected. */
        if(RxMessageInfoPtr->RebootDetected != TRUE)
        {
          Sd_RxClient_ProcessEntryAcknowledge(RxMessageInfoPtr, IndexOfEntry, clientIdx);                               /* SBSW_SD_PASSING_API_POINTER */
        }
      }
# endif
      /* #1001 If the entry was addressed to an OTHERSERV client, proceed searching for matching clients. */
      if(rxServiceId == SD_SERVICE_ID_OTHERSERV)
      {
        /* #10010 Do not exit the loop and search for additional matching OTHERSERV services. */
      }
      else
      {
        /* #10011 Found the corresponding client and processed the entry. Exit the loop. */
        break;
      }
    }
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOffer()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryOffer(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_AttachedOptionsType attachedOptions;
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry() + 1];
  uint32 rxTTL;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Parse OfferService entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);

  /* #20 Save the offered TTL of the server. */
  Sd_SetOfferedTTLOfClientDyn(ClientIdx, rxTTL);                                                                        /* SBSW_SD_API_ARGUMENT_IDX */

  /* #30 Process the corresponding option array.
   *     Check if all attached endpoint options are valid and there exist no conflicts. */
  if(nr1stOptions != 0)
  {
    /* #300 Process first option run. */
    retVal = Sd_RxClient_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index1stOption, nr1stOptions,
      Sd_GetEMalformedMsgIdOfInstance(instanceIdx), Sd_GetSizeOfConsumedEventgroup(), &attachedOptions);                /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  if((nr2ndOptions != 0) && ((nr1stOptions == 0) || (retVal == E_OK)))
  {
    /* #301 Process second option run. */
    retVal = Sd_RxClient_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index2ndOption, nr2ndOptions,
      Sd_GetEMalformedMsgIdOfInstance(instanceIdx), Sd_GetSizeOfConsumedEventgroup(), &attachedOptions);                /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }
  else if((nr1stOptions == 0) && (nr2ndOptions == 0) && (rxTTL == 0))
  {
    /* #302 In case no first or second option run exists, only SD_ENTRY_STOP_OFFER_SERVICE messages are allowed. */
    retVal = E_OK;
  }
  else
  {
    /* #303 Entry does not contain the required options. Discard entry. (retVal = E_NOT_OK) */
  }

# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
  if(retVal == E_OK)
  {
    /* #400 Check the attached configuration options by a user callout. */
    retVal = Sd_RxClient_CheckConfigOptionByCbk(
      ClientIdx,
      (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx) : Sd_GetUnicastRxPduIdOfInstance(instanceIdx),
      SD_ENTRY_OFFER_SERVICE,
      rxConfigOptionPtrArray);                                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }
# endif

  if(retVal == E_OK)
  {
    /* #501 Save the address of the server. */
    Sd_SetAddressStorageServerIdxOfClientDyn(ClientIdx, RxMessageInfoPtr->AddressStorageIdx);                           /* SBSW_SD_API_ARGUMENT_IDX */

    /* #502 Check the TTL of the received entry. */
    if(rxTTL == 0)
    {
      /* #5020 Received SD_ENTRY_STOP_OFFER_SERVICE message. Save the event. */
      Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_STOP_OFFER);                                            /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_Util_TriggerStateMachineRun(instanceIdx);
    }
    else
    {
      /* #5021 Received SD_ENTRY_OFFER_SERVICE message. */

      /* #5022 Save the attached endpoint options. */
      Sd_Util_CopySockAddr(Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx), &attachedOptions.EndpointAddrUdp);    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      Sd_Util_CopySockAddr(Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx), &attachedOptions.EndpointAddrTcp);    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

      /* #5023 Check if a valid endpoint option was attached. */
      if((attachedOptions.EndpointAddrUdp.family != SOAD_AF_INVALID) || (attachedOptions.EndpointAddrTcp.family != SOAD_AF_INVALID))
      {
        Sd_SetCurrentStateOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_AVAILABLE);                                          /* SBSW_SD_API_ARGUMENT_IDX */

        /* #50230 Check if a reboot occurred. */
        if(RxMessageInfoPtr->RebootDetected == TRUE)
        {
          /* #502300 Handle reboot as StopOffer -> Offer sequence. */
          Sd_SetEventOfClientDyn(ClientIdx, (RxMessageInfoPtr->MulticastMessage == TRUE) ?                              /* SBSW_SD_API_ARGUMENT_IDX */
            SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_MULTI : SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_UNI);
        }
        else
        {
          /* #502301 Save the received OfferService message as event. */
          Sd_SetEventOfClientDyn(ClientIdx, (RxMessageInfoPtr->MulticastMessage == TRUE) ?                              /* SBSW_SD_API_ARGUMENT_IDX */
            SD_CLIENT_EVENT_SERVICE_OFFER_MULTI : SD_CLIENT_EVENT_SERVICE_OFFER_UNI);
        }

        Sd_Util_TriggerStateMachineRun(instanceIdx);
      }
    }
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledge()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledge(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  uint16 IndexOfEntry,
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry() + 1];
  Sd_AttachedOptionsType attachedOptions;
  uint32 rxTTL;
  uint16 rxEventGroupId;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Parse SubscribeEventgroupAck entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

  /* #20 Iterate trough each eventgroup to find the corresponding one. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx);
    consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx);
    consumedEventgroupIdx++)
  {
    if((rxEventGroupId == Sd_GetEventGroupIdOfConsumedEventgroup(consumedEventgroupIdx)) &&
      (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_REQUESTED))
    {
      /* #200 Found the corresponding eventgroup, update the acknowledged TTL. */
      Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, rxTTL);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

      /* #201 Process the corresponding option array.
       *      Check if all attached endpoint options are valid and there exist no conflicts. */
      if(nr1stOptions != 0)
      {
        /* #2010 Process first option run. */
        retVal = Sd_RxClient_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index1stOption, nr1stOptions,
          Sd_GetEMalformedMsgIdOfInstance(instanceIdx), consumedEventgroupIdx, &attachedOptions);                       /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      if((nr2ndOptions != 0) && ((nr1stOptions == 0) || (retVal == E_OK)))
      {
        /* #2011 Process second option run. */
        retVal = Sd_RxClient_ProcessOptionRun(RxMessageInfoPtr->PduInfoPtr, index2ndOption, nr2ndOptions,
          Sd_GetEMalformedMsgIdOfInstance(instanceIdx), consumedEventgroupIdx, &attachedOptions);                       /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }
      else if((nr1stOptions == 0) && (nr2ndOptions == 0))
      {
        /* #2012 In case no first or second option run exists, only SD_ENTRY_SUBSCRIBE_EVENTGROUP_(N)ACK messages are allowed. */
        retVal = E_OK;
      }
      else
      {
        /* #2013 No second option run present. */
      }

# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
      if(retVal == E_OK)
      {
        /* #3000 Check the attached configuration options by a user callout. */
        retVal = Sd_RxClient_CheckConfigOptionByCbk(
          ClientIdx,
          (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx) : Sd_GetUnicastRxPduIdOfInstance(instanceIdx),
          SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK,
          rxConfigOptionPtrArray);                                                                                      /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }
# endif

      if(retVal == E_OK)
      {
        /* #4000 Check the TTL of the received subscription in order to differentiate ACK and NACK entries. */
        if(rxTTL == 0)
        {
          /* #40000 Received SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK. Save the event. */
          Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NACK);                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        }
        else
        {
          /* #40001 Received SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK message. */

          /* #40002 Save the attached endpoint option. */
          Sd_Util_CopySockAddr(Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(ClientIdx),
            &attachedOptions.EndpointAddrUdp);                                                                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

          /* #40003 Save the event. */
          Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_ACK);                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        }

        /* #4001 Trigger execution of state machines. */
        Sd_Util_TriggerStateMachineRun(instanceIdx);
      }

      /* #500 Found the corresponding eventgroup, stop searching. */
      break;
    }
  }
}
#endif

#if(SD_CLIENT == STD_ON)
# if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxClient_CheckConfigOptionByCbk(
  Sd_SizeOfClientType ClientIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client contains a configured user callout for checking capability records. */
  if(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfClient(ClientIdx) != SD_NO_CAPABILITYRECORDFUNCTIONCALLOUTPTRIDXOFCLIENT)
  {
    Sd_ConfigOptionStringType configOptionPtrArray[3] = { NULL_PTR, NULL_PTR, NULL_PTR };

#  if(SD_CAPABILITYRECORD == STD_ON)
    /* #100 Add configuration options of hostname and capability records to the configuration option pointer array. */
    Sd_Rx_FillConfigOptionPtrArray(configOptionPtrArray,
      Sd_GetCapabilityRecordStartIdxOfInstance(Sd_GetInstanceIdxOfClient(ClientIdx)),
      Sd_GetCapabilityRecordStartIdxOfClient(ClientIdx),
      SD_NO_CAPABILITYRECORDSTARTIDXOFCLIENT);                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */
#  endif

    /* #101 Call application to verify configuration options of received message. */
    if(TRUE != Sd_GetCapabilityRecordFunctionCalloutPtr(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfClient(ClientIdx))(
      RxPduId,
      (uint8)RxEntryType,
      Sd_GetServiceIdOfClient(ClientIdx),
      Sd_GetInstanceIdOfClient(ClientIdx),
      Sd_GetMajorVersionOfClient(ClientIdx),
      Sd_GetMinorVersionOfClient(ClientIdx),
      RxConfigOptionPtrArrayPtr,
      configOptionPtrArray))                                                                                            /* SBSW_SD_FUNCTION_PTR_CALL */
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
# endif
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_RxClient_ProcessOptionRun()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxClient_ProcessOptionRun(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_Dem_EventIdType EMalformedMsgId,
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 currentOption = IndexOfOption;
  Sd_OptionType optionType;
  Sd_L4ProtocolType optionL4Proto = SD_L4PROTOCOL_NONE;
  uint16 addrType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop processing of the message if the header indexes more options than attached. */
  if(Sd_GetNrOfOptions() < (IndexOfOption + NrOfOptions))
  {
    retVal = E_NOT_OK;
  }

  /* #20 Process consecutive options. */
  while((retVal == E_OK) && ((IndexOfOption + NrOfOptions) > currentOption))
  {
    addrType = SOAD_AF_INVALID;

    /* #200 Read the option information. */
    SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(currentOption), optionType);

    /* #201 Differentiate between configuration and endpoint option. */
    if(optionType == SD_OPTION_CONFIGURATION)
    {
      /* #2010 Configuration option: Check if the option is valid. */
      retVal |= Sd_Rx_CheckConfigOption(PduInfoPtr, currentOption, AttachedOptionsPtr->ConfigOptionPtrArrayPtr);        /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
    }
    else
    {
      /* #2011 Endpoint option: Read the type values of the options.
       *       Multicast endpoint options are only allowed for SubscribeEventgroup(N)Ack messages. */
      Sd_Rx_GetEndpointOptionParameters(PduInfoPtr, currentOption,
        (boolean)((ConsumedEventgroupIdx == Sd_GetSizeOfConsumedEventgroup()) ? FALSE : TRUE),
        &addrType, &optionL4Proto);                                                                                     /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }

    /* #202 Read the endpoint options. */
    retVal |= Sd_Rx_ReadEndpointOptions(PduInfoPtr, currentOption, addrType, optionL4Proto, AttachedOptionsPtr);        /* SBSW_SD_PASSING_API_POINTER */

    currentOption++;
  }

# if(SD_DEM_EVENTS_CONFIGURED == STD_ON)
  if(retVal == E_NOT_OK)
  {
    /* #203 Report an error if the endpoint configuration was not correct. */
    Sd_Util_ReportDemEvent(EMalformedMsgId);
  }
# else
  SD_DUMMY_STATEMENT(EMalformedMsgId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6060 */ /* MD_Sd_STPAR_6060 */
#endif

/**********************************************************************************************************************
 *  SHARED FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_Rx_GetReceivingInstance()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_GetReceivingInstance(
  PduIdType RxPduId,
  SD_P2VAR(Sd_SizeOfInstanceType) InstanceIdxPtr,
  SD_P2VAR(SoAd_SoConIdType) RxSoConIdPtr,
  SD_P2VAR(boolean) MulticastMessagePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType tmpInstanceIdx;

  /* ----- Implementation ----------------------------------------------- */
  *InstanceIdxPtr = Sd_GetSizeOfInstance();                                                                             /* SBSW_SD_VALID_API_POINTER */

  /* #10 Search for the corresponding instance. */
  for(tmpInstanceIdx = 0; tmpInstanceIdx < Sd_GetSizeOfInstance(); tmpInstanceIdx++)
  {
    /* #100 Check if the message was received by the multicast or the unicast RxPdu. */
    if(Sd_GetUnicastRxPduIdOfInstance(tmpInstanceIdx) == RxPduId)
    {
      /* #1000 Message was received by the unicast PDU. */
      *MulticastMessagePtr = FALSE;                                                                                     /* SBSW_SD_VALID_API_POINTER */
      *RxSoConIdPtr = Sd_GetUnicastRxSoConIdOfInstance(tmpInstanceIdx);                                                 /* SBSW_SD_VALID_API_POINTER */
      *InstanceIdxPtr = tmpInstanceIdx;                                                                                 /* SBSW_SD_VALID_API_POINTER */
    }
    else if(Sd_GetMulticastRxPduIdOfInstance(tmpInstanceIdx) == RxPduId)
    {
      /* #1001 Message was received by the multicast PDU. */
      *MulticastMessagePtr = TRUE;                                                                                      /* SBSW_SD_VALID_API_POINTER */
      *RxSoConIdPtr = Sd_GetMulticastRxSoConIdOfInstance(tmpInstanceIdx);                                               /* SBSW_SD_VALID_API_POINTER */
      *InstanceIdxPtr = tmpInstanceIdx;                                                                                 /* SBSW_SD_VALID_API_POINTER */
    }
    else
    {
      /* #1002 The message was received by another instance. Keep searching. */
    }

    /* #101 Stop searching as soon as the receiving instance has been found.  */
    if(*InstanceIdxPtr != Sd_GetSizeOfInstance())
    {
      break;
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_SaveAddr()
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
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Rx_SaveAddr(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx, savedAddressStorageIdx = Sd_GetSizeOfAddressStorage();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the address is already saved. */
  for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx);
    addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx);
    addressStorageIdx++)
  {
    if((Sd_Util_SockAddrIsEqual(Sd_GetAddrAddrOfAddressStorage(addressStorageIdx), InAddrPtr)) &&
      (Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx) != 0))                                                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_PASSING_API_POINTER */
    {
      /* #100 The address is already saved. Save index of the address. */
      savedAddressStorageIdx = addressStorageIdx;
      break;
    }
  }

  /* #20 Check if the address has to be saved. */
  if(savedAddressStorageIdx == Sd_GetSizeOfAddressStorage())
  {
    /* #200 Search for the next free slot and save the address. */
    for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx);
      addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx);
      addressStorageIdx++)
    {
      if(Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx) == 0)
      {
        /* #2000 Save the address and set the session counter to 1. */
        Sd_Util_CopySockAddr(Sd_GetAddrAddrOfAddressStorage(addressStorageIdx), InAddrPtr);                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_PASSING_API_POINTER */
        Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 1);                                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0);                                                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0);                                               /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK);                                  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        savedAddressStorageIdx = addressStorageIdx;
        break;
      }
    }
  }

  if(savedAddressStorageIdx == Sd_GetSizeOfAddressStorage())
  {
    /* #201 Not able to save the additional address.
     *      The amount of manageable destination addresses is configured in Sd/SdConfig/SdInstance/SdMaxNrDestAddr. */
#if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_RX_INDICATION, SD_E_ADDR_LIST_FULL);
#endif
  }

  return savedAddressStorageIdx;
}

/**********************************************************************************************************************
 *  Sd_Rx_IndexOptionArray()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_IndexOptionArray(
  SD_P2CONST(PduInfoType) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint32 lengthEntriesArray, lengthOfOptionsArray;
  uint16 lengthOfOption, tmpIndex, startIndexOptionsArray;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the option index. */
  Sd_SetNrOfOptions(0);                                                                                                 /* SBSW_SD_CSL_VAR_ELEM */

  /* #20 read length of the entries and options array. */
  SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);
  tmpIndex = (uint16)(SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray);

  SD_GET_UINT32(PduInfoPtr, tmpIndex, lengthOfOptionsArray);
  tmpIndex += SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE;

  startIndexOptionsArray = tmpIndex;

  /* #30 Iterate through all options and generate an index. */
  while ((tmpIndex < (startIndexOptionsArray + lengthOfOptionsArray)) && (retVal == E_OK))
  {
    /* #300 Read the option and check if it is valid. */
    retVal = Sd_Rx_CheckOption(PduInfoPtr, tmpIndex, lengthOfOptionsArray, &lengthOfOption);                            /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(retVal == E_OK)
    {
      /* #3000 The option is valid. Check if it could be saved in the OptionIndex. */
      if(Sd_GetNrOfOptions() < Sd_GetSizeOfOptionIndex())
      {
        /* #30000 Add the option to the index. */
        Sd_SetOptionIndex(Sd_GetNrOfOptions(), tmpIndex);                                                               /* SBSW_SD_CSL01 */
        Sd_IncNrOfOptions();                                                                                            /* SBSW_SD_CSL_VAR_ELEM */
        tmpIndex  +=  SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE + lengthOfOption;
      }
      else
      {
        /* #30001 The Sd_OptionIndex is full. Stop reading further options. */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* #3001 The option is not valid. Stop processing. */
    }
  }
}

/**********************************************************************************************************************
  *  Sd_Rx_CheckOption()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 OptionIndex,
  uint32 OptionArrayLength,
  SD_P2VAR(uint16) CoveredOptionLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the length and the type field are part of the payload. */
  if(PduInfoPtr->SduLength >= (OptionIndex + SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE))
  {
    /* #100 Read the length and the type of the option. */
    SD_GET_UINT16(PduInfoPtr, OptionIndex, *CoveredOptionLengthPtr);                                                    /* SBSW_SD_VALID_API_POINTER */
    SD_GET_OPTION_TYPE(PduInfoPtr, OptionIndex, optionType);

    /* #101 Check if the length of the option is feasible. */
    if((uint32)(SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE + *CoveredOptionLengthPtr) <= OptionArrayLength)
    {
      /* #1010 Check the length of IPv4 endpoint or multicast endpoint options. */
      if(((optionType == SD_OPTION_IPV4_ENDPOINT) ||
        (optionType == SD_OPTION_IPV4_MULTICAST) ||
        (optionType == SD_OPTION_IPV4_SD_ENDPOINT)) &&
        (*CoveredOptionLengthPtr != SD_OPTION_IPV4_COVERED_LENGTH))
      {
        retVal = E_NOT_OK;
      }

      /* #1011 Check the length of IPv6 endpoint or multicast endpoint options. */
      if(((optionType == SD_OPTION_IPV6_ENDPOINT) ||
        (optionType == SD_OPTION_IPV6_MULTICAST) ||
        (optionType == SD_OPTION_IPV6_SD_ENDPOINT)) &&
        (*CoveredOptionLengthPtr != SD_OPTION_IPV6_COVERED_LENGTH))
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    *CoveredOptionLengthPtr = 0;                                                                                        /* SBSW_SD_VALID_API_POINTER */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckRemoteReboot()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_CheckRemoteReboot(
  SD_P2VAR(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_HeaderType) SdHeaderPtr,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx = RxMessageInfoPtr->AddressStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  RxMessageInfoPtr->RebootDetected = FALSE;                                                                             /* SBSW_SD_VALID_API_POINTER */

  /* #10 Check if the message was received via unicast or multicast. */
  if(RxMessageInfoPtr->MulticastMessage == TRUE)
  {
    /* #100 Check if the device has rebooted. */
    if(((Sd_GetRxMulticastSessionIdOfAddressStorage(addressStorageIdx) > SdHeaderPtr->SessionID) ||
      ((Sd_GetRxMulticastFlagsOfAddressStorage(addressStorageIdx) & SD_FLAGS_REBOOT_MASK) == 0)) &&
      ((SdHeaderPtr->Flags & SD_FLAGS_REBOOT_MASK) != 0))
    {
      /* #1000 The remote device has rebooted. */
      RxMessageInfoPtr->RebootDetected = TRUE;                                                                          /* SBSW_SD_VALID_API_POINTER */
      Sd_Rx_HandleRemoteReboot(InstanceIdx, addressStorageIdx);

      /* #1001 Reset the unicast Session ID in order to detect the reboot only once. */
      Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0);                                                   /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    }

    /* #101 Update the Session ID and Flags field for multicast messages. */
    Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, SdHeaderPtr->SessionID);                              /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SdHeaderPtr->Flags);                                      /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
  }
  else
  {
    /* #102 Check if the device has rebooted. */
    if(((Sd_GetRxUnicastSessionIdOfAddressStorage(addressStorageIdx) > SdHeaderPtr->SessionID) ||
      ((Sd_GetRxUnicastFlagsOfAddressStorage(addressStorageIdx) & SD_FLAGS_REBOOT_MASK) == 0)) &&
      ((SdHeaderPtr->Flags & SD_FLAGS_REBOOT_MASK) != 0))
    {
      /* #1020 The remote device has rebooted. */
      RxMessageInfoPtr->RebootDetected = TRUE;                                                                          /* SBSW_SD_VALID_API_POINTER */
      Sd_Rx_HandleRemoteReboot(InstanceIdx, addressStorageIdx);

      /* #1021 Reset the multicast Session ID in order to detect the reboot only once. */
      Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0);                                                 /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    }

    /* #103 Update the Session ID and Flags field for unicast messages. */
    Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, SdHeaderPtr->SessionID);                                /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SdHeaderPtr->Flags);                                        /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_GetSdEndpointOption()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_GetSdEndpointOption(
  SoAd_SoConIdType RxSoConId,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SdEndpointAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint16 optionIndex = 0;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  SdEndpointAddressPtr->family = SOAD_AF_INVALID;                                                                       /* SBSW_SD_VALID_API_POINTER */

  /* #10 Check if an SD endpoint option is attached and save it. */
  while(optionIndex < Sd_GetNrOfOptions())
  {
    /* #100 Get the option type. */
    SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(optionIndex), optionType);

    if(optionType == SD_OPTION_IPV4_SD_ENDPOINT)
    {
      /* #1000 Read the address transported in the IPv4 SD endpoint option. */
      retVal = Sd_Rx_ReadEndpointOption(SOAD_AF_INET, PduInfoPtr->SduDataPtr,
        Sd_GetOptionIndex(optionIndex), SdEndpointAddressPtr);                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */

      /* #1001 Only the first SD endpoint option shall be processed. (All further SD endpoint options shall be ignored.)
       *       Set optionIndex to an invalid value. */
      optionIndex = Sd_GetNrOfOptions();
    }
    else if(optionType == SD_OPTION_IPV6_SD_ENDPOINT)
    {
      /* #1002 Read the address transported in the IPv6 SD endpoint option. */
      retVal = Sd_Rx_ReadEndpointOption(SOAD_AF_INET6, PduInfoPtr->SduDataPtr,
        Sd_GetOptionIndex(optionIndex), SdEndpointAddressPtr);                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */

      /* #1003 Only the first SD endpoint option shall be processed. (All further SD endpoint options shall be ignored.)
       *       Set optionIndex to an invalid value. */
      optionIndex = Sd_GetNrOfOptions();
    }
    else
    {
      /* #101 No IPv4 or IPv6 SD endpoint option. Keep searching. */
    }

    /* #102 Check if a valid SD endpoint option was found. */
    if(retVal == E_OK)
    {
      Sd_SockAddrContainerType localAddr;
      Sd_SockAddrContainerType defaultRouter;
      uint8 netmaskCidr = 0;

      /* #103 Get the local IP address and the subnet mask. */
      /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      retVal |= SoAd_GetLocalAddr(RxSoConId, (SD_P2VAR(SoAd_SockAddrType))&localAddr, &netmaskCidr,
        (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);                                                                   /* SBSW_SD_REFERENCE_OF_VARIABLE */

      /* #104 Check if the SD endpoint address is part of the subnet. */
      /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
      if((retVal == E_OK) && (TRUE == Sd_Rx_SockAddrWithinSubnet(&localAddr, SdEndpointAddressPtr, netmaskCidr)))       /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #1040 First valid SD endpoint option successfully read. */
      }
      else
      {
        /* #1041 The endpoint references an IP address outside the configured subnet, do not consider this endpoint option. */
        SdEndpointAddressPtr->family = SOAD_AF_INVALID;                                                                 /* SBSW_SD_VALID_API_POINTER */
      }
    }

    optionIndex++;
  }
}

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Rx_GetEndpointOptionParameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_GetEndpointOptionParameters(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  boolean ReadMulticastEndpoints,
  SD_P2VAR(Sd_FamilyType) FamilyTypePtr,
  SD_P2VAR(Sd_L4ProtocolType) L4ProtocolTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the option type information. */
  SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption), optionType);

  /* #20 Read endpoint option information. */
  if(((optionType == SD_OPTION_IPV4_MULTICAST) && (ReadMulticastEndpoints == TRUE)) ||
    ((optionType == SD_OPTION_IPV4_ENDPOINT) && (ReadMulticastEndpoints == FALSE)))
  {
    SD_GET_L4PROTOCOL_IPV4_TYPE(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption), *L4ProtocolTypePtr);                      /* SBSW_SD_VALID_API_POINTER */
    *FamilyTypePtr = SOAD_AF_INET;                                                                                      /* SBSW_SD_VALID_API_POINTER */
  }
  else if(((optionType == SD_OPTION_IPV6_MULTICAST) && (ReadMulticastEndpoints == TRUE)) ||
    ((optionType == SD_OPTION_IPV6_ENDPOINT) && (ReadMulticastEndpoints == FALSE)))
  {
    SD_GET_L4PROTOCOL_IPV6_TYPE(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption), *L4ProtocolTypePtr);                      /* SBSW_SD_VALID_API_POINTER */
    *FamilyTypePtr = SOAD_AF_INET6;                                                                                     /* SBSW_SD_VALID_API_POINTER */
  }
  else
  {
    /* #200 Ignore unknown and unexpected (ReadMulticastEndpoints) options. */
  }
}
#endif

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOptions()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ReadEndpointOptions(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_FamilyType FamilyType,
  Sd_L4ProtocolType L4ProtocolType,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an endpoint option was attached */
  if(FamilyType != SOAD_AF_INVALID)
  {
    /* #100 Differentiate between UDP and TCP endpoints. */
    if((L4ProtocolType == SD_L4PROTOCOL_UDP) && (AttachedOptionsPtr->EndpointAddrUdp.family == SOAD_AF_INVALID))
    {
      /* #1000 Read the UDP address transported in the endpoint. */
      retVal = Sd_Rx_ReadEndpointOption(FamilyType, PduInfoPtr->SduDataPtr, Sd_GetOptionIndex(IndexOfOption),
        &AttachedOptionsPtr->EndpointAddrUdp);                                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    }
    else if((L4ProtocolType == SD_L4PROTOCOL_TCP) && (AttachedOptionsPtr->EndpointAddrTcp.family == SOAD_AF_INVALID))
    {
      /* #1001 Read the TCP address transported in the endpoint. */
      retVal = Sd_Rx_ReadEndpointOption(FamilyType, PduInfoPtr->SduDataPtr, Sd_GetOptionIndex(IndexOfOption),
        &AttachedOptionsPtr->EndpointAddrTcp);                                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    }
    else
    {
      /* #1002 Reject subscription because of an undefined protocol type or because the entry references multiple endpoint options of the same type. */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOption()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ReadEndpointOption(
  Sd_FamilyType FamilyType,
  SD_P2CONST(uint8) DataPtr,
  uint16 DataIndex,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  EndpointAddrPtr->family = FamilyType;                                                                                 /* SBSW_SD_VALID_API_POINTER */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Data is saved in Network Byte Order. */

  if(FamilyType == SOAD_AF_INET)
  {
    /* #100 Save IPv4 endpoint option. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 1 */ /* MD_Sd_11.5_0311 */
    Sd_Copy(EndpointAddrPtr->data, &DataPtr[DataIndex + SD_OPTION_IPV4_ADDRESS_OFFSET], SD_OPTION_IPV4_ADDRESS_SIZE);   /* SBSW_SD_RX_MESSAGE_SIZE */

    if(EndpointAddrPtr->data[0] == 0)
    {
      /* #1000 Mark the endpoint address as invalid if no definite address is defined. */
      EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                        /* SBSW_SD_VALID_API_POINTER */
      retVal = E_NOT_OK;
    }

    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
    Sd_Copy(&EndpointAddrPtr->port,                                                                                     /* SBSW_SD_RX_MESSAGE_SIZE */
      &DataPtr[DataIndex + SD_OPTION_IPV4_PORTNUMBER_OFFSET], SD_OPTION_IPV4_PORTNUMBER_SIZE);
  }
  else /* SOAD_AF_INET6 == FamilyType. */
  {
    /* #101 Save IPv6 endpoint option. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 1 */ /* MD_Sd_11.5_0311 */
    Sd_Copy(EndpointAddrPtr->data, &DataPtr[DataIndex + SD_OPTION_IPV6_ADDRESS_OFFSET], SD_OPTION_IPV6_ADDRESS_SIZE);   /* SBSW_SD_RX_MESSAGE_SIZE */

    if((EndpointAddrPtr->data[0] | EndpointAddrPtr->data[1] | EndpointAddrPtr->data[2] | EndpointAddrPtr->data[3]) == 0)
    {
      /* #1010 Mark the endpoint address as invalid if no definite address is defined. */
      EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                        /* SBSW_SD_VALID_API_POINTER */
      retVal = E_NOT_OK;
    }

    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
    Sd_Copy(&EndpointAddrPtr->port,                                                                                     /* SBSW_SD_RX_MESSAGE_SIZE */
      &DataPtr[DataIndex + SD_OPTION_IPV6_PORTNUMBER_OFFSET], SD_OPTION_IPV6_PORTNUMBER_SIZE);
  }

  if(EndpointAddrPtr->port == SD_TCPIP_PORT_ANY)
  {
    /* #102 Mark the endpoint address as invalid if no definite port is defined. */
    EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                          /* SBSW_SD_VALID_API_POINTER */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckConfigOption()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckConfigOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  boolean correctTermination = FALSE;
  uint8 itemLength;
  uint16 lengthOfOption, reservedIndex, tmpIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get configuration option header. */
  SD_GET_UINT16(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption), lengthOfOption);

  /* #20 Skip Reserved Field. */
  reservedIndex = Sd_GetOptionIndex(IndexOfOption) + SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE;
  tmpIndex = SD_OPTION_RESERVED_SIZE;

  /* #30 Check configuration string. */
  while((lengthOfOption > tmpIndex) && (retVal == E_OK))
  {
    /* #300 Get length of capability string. */
    SD_GET_UINT8(PduInfoPtr, reservedIndex + tmpIndex, itemLength);
    tmpIndex++;

    /* #301 Check if the current byte defines the termination of the capability string. */
    if(itemLength == 0)
    {
      /* #3010 Termination of capability string. Exit loop. */
      correctTermination = TRUE;
      break;
    }

    /* #302 Check if the length of the item exceeds the length of the entire option. */
    if(lengthOfOption >= (itemLength + tmpIndex))
    {
      tmpIndex += itemLength;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  /* #40 Check if the option was terminated correctly. */
  if(correctTermination == TRUE)
  {
    Sd_SizeOfOptionIndexType optionIdx;

    /* #400 Save reference to the configuration string in the list of configuration options. */
    for(optionIdx = 0; optionIdx < Sd_GetMaxNrOptionsPerEntry(); optionIdx++)
    {
      if(ConfigOptionPtrArrayPtr[optionIdx] == NULL_PTR)
      {
        ConfigOptionPtrArrayPtr[optionIdx] = &PduInfoPtr->SduDataPtr[reservedIndex + SD_OPTION_RESERVED_SIZE];          /* SBSW_SD_CONFIG_OPTION_PTR_ARRAY_SIZE */
        break;
      }
    }

    /* #401 Check if the number of attached options exceeds number defined in BSWMD configuration parameter SdMaxNrOptionsPerMsg. */
    if(optionIdx == Sd_GetMaxNrOptionsPerEntry())
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

#if(SD_CAPABILITYRECORDFUNCTIONCALLOUTPTR == STD_ON)
# if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Rx_FillConfigOptionPtrArray()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_FillConfigOptionPtrArray(
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr,
  Sd_SizeOfCapabilityRecordType InstanceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityInvIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 configOptionIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add hostname configuration option if configured. */
  if(InstanceCapabilityIdx != SD_NO_CAPABILITYRECORDSTARTIDXOFINSTANCE)
  {
    ConfigOptionPtrArrayPtr[configOptionIdx] = Sd_GetAddrCapabilityRecord(InstanceCapabilityIdx);                       /* SBSW_SD_ATTACHED_OPTIONS_PTR */
    configOptionIdx++;
  }

  /* #20 Add configuration option of service level if configured. */
  if(ServiceCapabilityIdx != ServiceCapabilityInvIdx)
  {
    ConfigOptionPtrArrayPtr[configOptionIdx] = Sd_GetAddrCapabilityRecord(ServiceCapabilityIdx);                        /* SBSW_SD_ATTACHED_OPTIONS_PTR */
  }
}
# endif
#endif

/**********************************************************************************************************************
 *  Sd_State_Process()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_Process(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the state machines have to be triggered because of a pending timer event. */
  if(TRUE == Sd_Util_CheckIfTimerExceeded(Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx), TRUE))
  {
    /* #100 Reset the timer. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx));                               /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

#if(SD_SERVER == STD_ON)
    /* #101 Run all server state machines of the instance. */
    {
      Sd_SizeOfServerType serverIdx;

      for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx);
        serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx);
        serverIdx++)
      {
        Sd_StateServer_Process(serverIdx);
      }
    }
#endif

#if(SD_CLIENT == STD_ON)
    /* #102 Run all client state machines of the instance. */
    {
      Sd_SizeOfClientType clientIdx;

      for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx);
        clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx);
        clientIdx++)
      {
        Sd_StateClient_Process(clientIdx);
      }
    }
#endif
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_ProcessHeader()
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
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ProcessHeader(
  Sd_SizeOfInstanceType InstanceIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_HeaderType) SdHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 lengthEntriesArray, lengthOptionsArray = 0;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Drop message if it does not include a complete SD header as well as the length field for entries and options. */
  if(PduInfoPtr->SduLength >= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE))
  {
    retVal = E_OK;

    /* #100 Read the header, message ID and Length field of the SD header will be handled by the SoAd. */
    SD_GET_UINT16(PduInfoPtr, SD_HEADER_CLIENTID_OFFSET, SdHeaderPtr->ClientID);                                        /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT16(PduInfoPtr, SD_HEADER_SESSIONID_OFFSET, SdHeaderPtr->SessionID);                                      /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_PROTOCOLVERSION_OFFSET, SdHeaderPtr->ProtocolVersion);                           /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_INTERFACEVERSION_OFFSET, SdHeaderPtr->InterfaceVersion);                         /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_MESSAGETYPE_OFFSET, SdHeaderPtr->MessageType);                                   /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_RETURNCODE_OFFSET, SdHeaderPtr->ReturnCode);                                     /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_FLAGS_OFFSET, SdHeaderPtr->Flags);                                               /* SBSW_SD_VALID_API_POINTER */
    SD_GET_UINT24(PduInfoPtr, SD_HEADER_RESERVED_OFFSET, SdHeaderPtr->Reserved);                                        /* SBSW_SD_VALID_API_POINTER */

    /* #101 Check the predefined fields of the header. */
    if((SdHeaderPtr->ProtocolVersion != SD_HEADER_PROTOCOL_VERSION) ||
      (SdHeaderPtr->InterfaceVersion != SD_HEADER_INTERFACE_VERSION) || (SdHeaderPtr->MessageType != SD_HEADER_MESSAGE_TYPE) ||
      (SdHeaderPtr->ReturnCode != SD_HEADER_RETURN_CODE))
    {
      /* #1010 The header is not compliant with the statically defined values. */
      retVal = E_NOT_OK;
    }
    else if(0u == (SdHeaderPtr->Flags & SD_FLAGS_UNICAST_MASK))
    {
      /* #1011 Unicast flag not set. Ignore message. */
      retVal = E_NOT_OK;
    }
    else
    {
      /* #1012 Process the received SD payload data. */
      SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);

      /* #1013 Check if the entries array fits into the payload. */
      if(PduInfoPtr->SduLength >=
        (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE))
      {
        SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE +
          lengthEntriesArray, lengthOptionsArray);
      }
      else
      {
        /* #10130 The entries array exceeds the payload. */
        retVal = E_NOT_OK;
      }

      /* #1014 Check if the payload can contain all mentioned entry and option arrays. */
      if((retVal == E_OK) &&
        (PduInfoPtr->SduLength >= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray +
        SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE + lengthOptionsArray)))
      {
        /* #10140 Header fields of message are valid. */
        retVal = E_OK;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }

  if(retVal != E_OK)
  {
    /* #102 The message is malformed. */
    Sd_Util_ReportDemEvent(Sd_GetEMalformedMsgIdOfInstance(InstanceIdx));
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_ProcessEntryArray()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_ProcessEntryArray(
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 indexEntry;
  uint32 lengthEntriesArray;

  /* ----- Implementation ----------------------------------------------- */
  SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);
  indexEntry = SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE;

  /* #10 Iterate through all entries of the array. */
  while((indexEntry + SD_ENTRY_SIZE) <= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray))
  {
    /* #100 Process the current entry and all associated options. */
    Sd_EntryType rxEntryType;
    SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, indexEntry, rxEntryType);

#if(SD_SERVER == STD_ON)
    /* #101 Check if the entry has to be processed by a server or client service. */
    if((rxEntryType == SD_ENTRY_FIND_SERVICE) || (rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP))
    {
      /* #1010 Entry has to be processed by a server. */
      Sd_RxServer_ProcessEntry(RxMessageInfoPtr, indexEntry, InstanceIdx);                                              /* SBSW_SD_PASSING_API_POINTER */
    }
    else
#endif
#if(SD_CLIENT == STD_ON)
    if((rxEntryType == SD_ENTRY_OFFER_SERVICE) || (rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK))
    {
      /* #1011 Entry has to be processed by a client. */
      Sd_RxClient_ProcessEntry(RxMessageInfoPtr, indexEntry, InstanceIdx);                                              /* SBSW_SD_PASSING_API_POINTER */
    }
    else
#endif
    {
      /* #1012 Ignore entry with unknown type. */
    }

    /* #102 The length of all entry arrays (format type 1 and format type 2) is 16 bytes. */
    indexEntry += SD_ENTRY_SIZE;
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_HandleRemoteReboot()
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
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_HandleRemoteReboot(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if(SD_CLIENT == STD_ON)
  {
    Sd_ClientIterType clientIdx;

    /* #10 Search for all clients which are affected by the remote reboot. */
    for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx);
      clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx);
      clientIdx++)
    {
      /* #100 Check if the client is affected by the reboot of the remote service.  */
      if(AddressStorageIdx == Sd_GetAddressStorageServerIdxOfClientDyn(clientIdx))
      {
        /* #1000 Save reboot of remote server as if a StopOfferService message was received. */
        Sd_SetEventOfClientDyn(clientIdx, SD_CLIENT_EVENT_SERVICE_STOP_OFFER);                                          /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }
    }
  }
#endif

#if(SD_SERVER == STD_ON)
  {
    Std_ReturnType retVal;
    Sd_ServerIterType serverIdx;
    Sd_UdpSoConIdIterType udpSoConIdIdx;
    Sd_SizeOfTcpSoConIdType tcpSoConIdIdx;
    Sd_SockAddrContainerType tmpAddr;

    /* #20 Search for all server which are affected by the remote reboot. */
    for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx);
      serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx);
      serverIdx++)
    {
      /* #200 Close all UDP socket connections used on service level. */
      for(udpSoConIdIdx = Sd_GetUdpSoConIdStartIdxOfServer(serverIdx);
        udpSoConIdIdx < Sd_GetUdpSoConIdEndIdxOfServer(serverIdx);
        udpSoConIdIdx++)
      {
        tmpAddr.family = SOAD_AF_INVALID;

        /* #2000 Get Remote Address of UDP socket connection. */
        /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
        retVal = SoAd_GetRemoteAddr(Sd_GetUdpSoConId(udpSoConIdIdx), (SD_P2VAR(SoAd_SockAddrType))&tmpAddr);            /* SBSW_SD_REFERENCE_OF_VARIABLE */

        /* #2001 Check if the saved remote address is equivalent to the remote address of the rebooted service. */
        /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
        if((retVal == E_OK) &&
          (SD_MATCH_TRUE == Sd_Util_CompareSockAddrIp(Sd_GetAddrAddrOfAddressStorage(AddressStorageIdx), &tmpAddr, 0))) /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
        {
          /* #20010 Socket connection was used for the rebooted remote client, close and reopen it. */
          (void)SoAd_CloseSoCon(Sd_GetUdpSoConId(udpSoConIdIdx), TRUE);
          (void)SoAd_OpenSoCon(Sd_GetUdpSoConId(udpSoConIdIdx));
        }
      }

      /* #201 Close all TCP socket connections used on service level. */
      for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfServer(serverIdx);
        tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfServer(serverIdx);
        tcpSoConIdIdx++)
      {
        tmpAddr.family = SOAD_AF_INVALID;

        /* #2010 Get Remote Address of TCP socket connection. */
        /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
        retVal = SoAd_GetRemoteAddr(Sd_GetTcpSoConId(tcpSoConIdIdx), (SD_P2VAR(SoAd_SockAddrType))&tmpAddr);            /* SBSW_SD_REFERENCE_OF_VARIABLE */

        /* #2011 Check if the saved remote address is equivalent to the remote address of the rebooted service. */
        /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
        if((retVal == E_OK) &&
          (SD_MATCH_TRUE == Sd_Util_CompareSockAddrIp(Sd_GetAddrAddrOfAddressStorage(AddressStorageIdx), &tmpAddr, 0))) /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
        {
          /* #20110 Socket connection was used for the rebooted remote client, close and reopen it. */
          (void)SoAd_CloseSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx), TRUE);
          (void)SoAd_OpenSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx));
        }
      }

# if(SD_EVENTHANDLER == STD_ON)
      /* #202 Check if the remote service was subscribed before. */
      {
        Sd_SizeOfEventHandlerType eventHandlerIdx;

        for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx);
          eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx);
          eventHandlerIdx++)
        {
          /* #2020 Set the TTL of all matching clients to 0 in order to remove the clients during the next main function. */
          (void)Sd_RxServer_FindClientInList(eventHandlerIdx, AddressStorageIdx, NULL_PTR);                             /* SBSW_SD_FUNCTION_CALL_NULL_PTR */
        }
      }
# endif
    }
  }
#endif

  /* #30 Trigger execution of the state machines in order to process the detected remote reboot. */
  Sd_Util_TriggerStateMachineRun(InstanceIdx);
}

/**********************************************************************************************************************
 *  Sd_Tx_ProcessPendingMessages()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_ProcessPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx,
  boolean ForceTransmission)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType iteration;
  Sd_SizeOfAddressStorageType addressStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate till all messages are transmitted, but maximally SendEntryNrPendingIdx-times. */
  iteration = Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx);

  /* #20 Iterate over all entries. */
  while((iteration != 0) && (Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx) != 0))
  {
    iteration--;

    /* #200 Clear the temporary send entries list. */
    Sd_SetNrOfTmpSendEntries(0);                                                                                        /* SBSW_SD_CSL_VAR_ELEM */

    /* #201 Get the address storage index of the next entry which is ready for transmission. */
    addressStorageIdx = Sd_Tx_CheckIfMessageReadyForTransmission(InstanceIdx, ForceTransmission);

    /* #202 Check if a message is ready for transmission. */
    if(addressStorageIdx < Sd_GetSizeOfAddressStorage())
    {
      /* #2020 Copy all messages for the given destination to the temporary send entries list. */
      Sd_Tx_CopyMessagesToTmpSendEntryList(InstanceIdx, addressStorageIdx);
    }
    else
    {
      /* #2021 No message ready for transmission -> stop. */
      iteration = 0;
    }

    /* #203 Transmit all pending messages from the temporary send entries list. */
    Sd_Tx_TransmitPendingMessages(InstanceIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfMessageReadyForTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Tx_CheckIfMessageReadyForTransmission(
  Sd_SizeOfInstanceType InstanceIdx,
  boolean ForceTransmission)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType sendEntryIdx;
  Sd_SizeOfAddressStorageType validAddressStorageIdx = Sd_GetSizeOfAddressStorage();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over the list of send entries and search for a message which is ready to transmit. */
  for(sendEntryIdx = Sd_GetSendEntryStartIdxOfInstanceDyn(InstanceIdx);
    sendEntryIdx != SD_NO_SENDENTRYENDIDXOFINSTANCEDYN;
    sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx))
  {
    /* #100 Check if message is ready for transmission. */
    if((Sd_Util_CheckIfTimerExceeded(Sd_GetTimeToSendOfSendEntry(sendEntryIdx), TRUE) == TRUE) ||
      (ForceTransmission == TRUE))
    {
      /* #1000 Check if the transmission of the message shall be skipped (and processed in a subsequent call). */
      if(Sd_Tx_CheckIfEntryShallBeSkipped(InstanceIdx, sendEntryIdx) == FALSE)
      {
        /* #10000 Found message which is ready for transmission. */
        validAddressStorageIdx = Sd_GetAddressStorageIdxOfSendEntry(sendEntryIdx);
        break;
      }
    }
  }

  return validAddressStorageIdx;
}

/**********************************************************************************************************************
 *  Sd_Tx_CopyMessagesToTmpSendEntryList()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CopyMessagesToTmpSendEntryList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType sendEntryIdx, foundMsgIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for all send entries which shall be transmitted to the identical destination. */
  for(sendEntryIdx = Sd_GetSendEntryStartIdxOfInstanceDyn(InstanceIdx);
    sendEntryIdx != SD_NO_SENDENTRYENDIDXOFINSTANCEDYN;
    sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx))
  {
    /* #100 Check if the pending entry shall be transmitted to the identical destination. */
    if(Sd_GetAddressStorageIdxOfSendEntry(sendEntryIdx) == AddressStorageIdx)
    {
      /* #1000 Check if the transmission of the entry shall be skipped. */
      if(Sd_Tx_CheckIfEntryShallBeSkipped(InstanceIdx, sendEntryIdx) == FALSE)
      {
        /* #10000 Save the index of the send entry. */
        Sd_SetSendEntryIdxOfTmpSendEntry(foundMsgIdx, sendEntryIdx);                                                    /* SBSW_SD_TMP_SEND_ENTRY_IDX */
        Sd_IncNrOfTmpSendEntries();                                                                                     /* SBSW_SD_CSL_VAR_ELEM */
        foundMsgIdx++;
      }
    }
    else
    {
      /* #1001 The entry has to be sent to another address, or the TCP connection is not established. */
    }

    /* #101 Stop the processing of the list if all entries are processed. */
    if(foundMsgIdx == Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx))
    {
      break;
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_TransmitPendingMessages()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_TransmitPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfSendEntryType sendEntryIdx, sendEntryIndIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate an SD message and transmit it if there are pending send entries. */
  if(Sd_GetNrOfTmpSendEntries() > 0)
  {
    /* #100 Serialize all entries of the temporary list to the payload and transmit the message. */
    retVal = Sd_Tx_SerializePendingMessages(InstanceIdx);

    /* #101 Check if the message could be transmitted and fits into the payload buffer. */
    if(retVal == E_OK)
    {
      /* #1010 Transmitted successfully. Clear the send entries list. */
      for(sendEntryIndIdx = 0; sendEntryIndIdx < Sd_GetNrOfTmpSendEntries(); sendEntryIndIdx++)
      {
        sendEntryIdx = Sd_GetSendEntryIdxOfTmpSendEntry(sendEntryIndIdx);
        Sd_Util_RemoveSendEntryFromList(InstanceIdx, sendEntryIdx);
      }

      Sd_SetNrOfTmpSendEntries(0);                                                                                      /* SBSW_SD_CSL_VAR_ELEM */
    }
    else
    {
      /* #1011 Transmit buffer too small. Try to serialize and send all entries separately. */
      Sd_SizeOfSendEntryType tmpSendEntryIndIdx = 0;
      Sd_NrOfTmpSendEntriesType initialNrOfTmpSendEntries = Sd_GetNrOfTmpSendEntries();
      Sd_SetNrOfTmpSendEntries(1);                                                                                      /* SBSW_SD_CSL_VAR_ELEM */

      while(tmpSendEntryIndIdx < initialNrOfTmpSendEntries)
      {
        Sd_SetSendEntryIdxOfTmpSendEntry(0, Sd_GetSendEntryIdxOfTmpSendEntry(tmpSendEntryIndIdx));                      /* SBSW_SD_SERIALIZE_SEPARATELY */
        (void)Sd_Tx_SerializePendingMessages(InstanceIdx);

        /* #10110 Clear the send entry. */
        Sd_Util_RemoveSendEntryFromList(InstanceIdx, Sd_GetSendEntryIdxOfTmpSendEntry(0));

        tmpSendEntryIndIdx++;
      }

      Sd_SetNrOfTmpSendEntries(0);                                                                                      /* SBSW_SD_CSL_VAR_ELEM */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfEntryShallBeSkipped()
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
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_CheckIfEntryShallBeSkipped(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfSendEntryType SendEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean skipEntry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#if(SD_CONSUMEDEVENTGROUP == STD_ON)
  /* #10 Check if the selected entry is a (Stop)Subscribe entry. */
  if(((Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP)))
  {
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx = Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx);
    Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx);

    /* #100 Check if the TCP connection is established. */
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if(Sd_ConsumedEventGroupUsesTcp(consumedEventgroupIdx) &&
      (Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx) != SOAD_INV_SO_CON_ID) &&
      (Sd_Util_GetSoConMode(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx)) != SD_SOCON_MODE_ONLINE))
    {
      /* #1000 The TCP connection is not established. Skip the transmission of this message. */
      skipEntry = TRUE;
    }

    /* #101 Check if the client service and/or eventgroup is still requested. */
    if(((Sd_GetStateSwcOfClientDyn(clientIdx) == SD_CLIENT_SERVICE_RELEASED) ||
      (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx) == SD_CONSUMED_EVENTGROUP_RELEASED) ||
      (Sd_GetCurrentStateOfClientDyn(clientIdx) == SD_CLIENT_SERVICE_DOWN)) &&
      (Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP))
    {
      /* #1010 The eventgroup is not requested or not available any more. Remove the message. */
      Sd_Util_RemoveSendEntryFromList(InstanceIdx, SendEntryIdx);
      skipEntry = TRUE;
    }
  }
  else
#endif
#if(SD_SERVER == STD_ON)
  /* #20 Check if the entry corresponds to a server which is not available.  */
  if(((Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_OFFER_SERVICE) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK)) &&
    (Sd_GetStateSwcOfServerDyn(Sd_GetServerIdxOfSendEntry(SendEntryIdx)) == SD_SERVER_SERVICE_DOWN))
  {
    /* #200 The server is not available any more. Remove the message. */
    Sd_Util_RemoveSendEntryFromList(InstanceIdx, SendEntryIdx);
    skipEntry = TRUE;
  }
  else
#endif
  {
    /* #300 The entry shall not be skipped. */
  }

#if((SD_CONSUMEDEVENTGROUP == STD_OFF) && (SD_SERVER == STD_OFF))
  SD_DUMMY_STATEMENT(InstanceIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  SD_DUMMY_STATEMENT(SendEntryIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return skipEntry;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializePendingMessages()
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
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Tx_SerializePendingMessages(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetAddressStorageIdxOfSendEntry(Sd_GetSendEntryIdxOfTmpSendEntry(0));
  boolean bufferTooSmall = FALSE;
  uint32 sizeEntriesArray;
  PduInfoType pduInfo;
  Sd_SizeOfSendEntryType sendEntryInd;
  uint32 sizeOfOptionsArray;
  Sd_SizeOfPayloadType payloadEntryIdx;
  Sd_OptionArrayInfoType optionArrayInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the length of the Entries Array. */
  sizeEntriesArray = Sd_GetNrOfTmpSendEntries() * SD_ENTRY_SIZE;

  /* #20 Check if the TxBuffer can take at least the header and the entries array. */
  if(Sd_GetSizeOfPayload() >=
    (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + sizeEntriesArray + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE))
  {
    /* #200 Write the header of the SD message. */
    Sd_Tx_SerializeHeader(addressStorageIdx);

    /* #201 Write the length of the entries array field. */
    Sd_Util_Write32Bit(Sd_GetAddrPayload(SD_HEADER_LENGTH), sizeEntriesArray);                                          /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    payloadEntryIdx = SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE;
    sizeEntriesArray = Sd_GetNrOfTmpSendEntries() * SD_ENTRY_SIZE;

    optionArrayInfo.ArrayStartIdx = (uint16)(SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + sizeEntriesArray + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE);
    optionArrayInfo.ArrayEndIdx = optionArrayInfo.ArrayStartIdx;
    optionArrayInfo.FreeOptionIndex = 0;

    /* #202 Iterate over all entries and serialize entries as well as options. */
    for(sendEntryInd = 0; sendEntryInd < Sd_GetNrOfTmpSendEntries(); sendEntryInd++)
    {
      /* #2020 Serialize the entry with all corresponding options. */
      bufferTooSmall = Sd_Tx_SerializeEntryAndOptions(
        Sd_GetSendEntryIdxOfTmpSendEntry(sendEntryInd), payloadEntryIdx, &optionArrayInfo);                             /* SBSW_SD_REFERENCE_OF_VARIABLE */

      /* #2021 Stop processing if TxBuffer is too small. */
      if(bufferTooSmall == TRUE)
      {
        break;
      }

      payloadEntryIdx += SD_ENTRY_SIZE;
    }

    /* #203 Check if all entries fit into the payload and an IP address is assigned. */
    if((bufferTooSmall == FALSE) &&
      (Sd_GetStateOfInstanceDyn(InstanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED))
    {
      /* #2030 Set the length of the options array. */
      sizeOfOptionsArray =  optionArrayInfo.ArrayEndIdx - optionArrayInfo.ArrayStartIdx;
      Sd_Util_Write32Bit(
        Sd_GetAddrPayload(optionArrayInfo.ArrayStartIdx - SD_HEADER_LENGTHOFENTRIESARRAY_SIZE), sizeOfOptionsArray);    /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

      /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      retVal = SoAd_SetRemoteAddr(Sd_GetTxSoConIdOfInstance(InstanceIdx),
        (SD_P2VAR(SoAd_SockAddrType))(Sd_GetAddrAddrOfAddressStorage(addressStorageIdx)));                              /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */

      /* #2031 Send the message. (Return values are only used for debug purposes. Sd does not implement retry mechanism). */
      if(retVal == E_OK)
      {
        pduInfo.SduDataPtr = Sd_GetAddrPayload(0);
        pduInfo.SduLength =  optionArrayInfo.ArrayEndIdx;

        retVal = SoAd_IfTransmit(Sd_GetTxPduIdOfInstance(InstanceIdx), &pduInfo);                                       /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      if(retVal == E_NOT_OK)
      {
        /* #20310 Transmit failed. Return value is ignored. */
      }

      /* #2032 Increment SessionId for each transmitted message. */
      if(Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx) == 0xFFFF)
      {
        /* #20320 The SessionId should start with the value 1. */
        Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 1);                                                   /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
        /* #20321 Reset the reboot flag if the SessionId wraps around. */
        Sd_SetFlagsOfAddressStorage(addressStorageIdx,                                                                  /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
          (uint8)(Sd_GetFlagsOfAddressStorage(addressStorageIdx) & (~SD_FLAGS_REBOOT_MASK)));
      }
      else
      {
        Sd_IncSessionIdCounterOfAddressStorage(addressStorageIdx);                                                      /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
      }
    }
  }
  else
  {
    bufferTooSmall = TRUE;
  }

  /* #30 Check if the buffer is too small. */
  if(bufferTooSmall == TRUE)
  {
    /* #300 The configured Sd_Payload buffer is too small to contain all entries. */
    /* Buffer size is configured in Sd/SdConfig/SdInstance/SdInstanceTxPdu/SdTxPduRef or Sd/SdGeneral/SdTxBufferSize. */
#if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERIALIZE_PENDING_MESSAGES, SD_E_TX_BUFFER_FULL);
#endif
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_SerializeHeader(
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Fill the header of the SD message. */
  /* #20 The Session ID of the header is used as a counter.
   *     All unicast as well as multicast messages have their own counter. */
  Sd_Util_Write16Bit(Sd_GetAddrPayload(SD_HEADER_CLIENTID_OFFSET), SD_HEADER_CLIENTID);                                 /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write16Bit(
    Sd_GetAddrPayload(SD_HEADER_SESSIONID_OFFSET), Sd_GetSessionIdCounterOfAddressStorage(AddressStorageIdx));          /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_PROTOCOLVERSION_OFFSET), SD_HEADER_PROTOCOLVERSION);                    /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_INTERFACEVERSION_OFFSET), SD_HEADER_INTERFACEVERSION);                  /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_MESSAGETYPE_OFFSET), SD_HEADER_MESSAGETYPE);                            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_RETURNCODE_OFFSET), SD_HEADER_RETURNCODE);                              /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_FLAGS_OFFSET), Sd_GetFlagsOfAddressStorage(AddressStorageIdx));         /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write24Bit(Sd_GetAddrPayload(SD_HEADER_RESERVED_OFFSET), SD_HEADER_RESERVED);                                 /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeRelatedOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeRelatedOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean bufferTooSmall;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the control structures which map the options. */
  EntryToOptionMapPtr->Index1st = SD_INV_OPTION_INDEX;                                                                  /* SBSW_SD_VALID_API_POINTER */
  EntryToOptionMapPtr->Index2nd = SD_INV_OPTION_INDEX;                                                                  /* SBSW_SD_VALID_API_POINTER */
  EntryToOptionMapPtr->Nr1stOptions = 0;                                                                                /* SBSW_SD_VALID_API_POINTER */
  EntryToOptionMapPtr->Nr2ndOptions = 0;                                                                                /* SBSW_SD_VALID_API_POINTER */

  /* #20 Serialize all endpoint options into the message. */
  bufferTooSmall = Sd_Tx_SerializeEndpointOptions(SendEntryIdx, EntryToOptionMapPtr, OptionArrayInfoPtr);               /* SBSW_SD_PASSING_API_POINTER */

#if(SD_CAPABILITYRECORD == STD_ON)
  if(bufferTooSmall == FALSE)
  {
    /* #200 Serialize all configuration options into the message. */
    bufferTooSmall = Sd_Tx_SerializeConfigurationOptions(SendEntryIdx, EntryToOptionMapPtr, OptionArrayInfoPtr);        /* SBSW_SD_PASSING_API_POINTER */
  }
#endif

  if(EntryToOptionMapPtr->Index1st == SD_INV_OPTION_INDEX)
  {
    /* #201 1st option run not used, clear all values. */
    EntryToOptionMapPtr->Index1st = 0;                                                                                  /* SBSW_SD_VALID_API_POINTER */
    EntryToOptionMapPtr->Nr1stOptions = 0;                                                                              /* SBSW_SD_VALID_API_POINTER */
  }

  if(EntryToOptionMapPtr->Index2nd == SD_INV_OPTION_INDEX)
  {
    /* #202 2nd option run not used, clear all values. */
    EntryToOptionMapPtr->Index2nd = 0;                                                                                  /* SBSW_SD_VALID_API_POINTER */
    EntryToOptionMapPtr->Nr2ndOptions = 0;                                                                              /* SBSW_SD_VALID_API_POINTER */
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEntryAndOptions()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEntryAndOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfPayloadType PayloadEntryIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_EntryType entryType;
  boolean bufferTooSmall;
  uint32 usedTtl;
  Sd_EntryToOptionMapType entryToOptionMap;

  /* ----- Implementation ----------------------------------------------- */
  entryType = Sd_GetEntryTypeOfSendEntry(SendEntryIdx);

  /* #10 Serialize first all related options of the entry. */
  bufferTooSmall = Sd_Tx_SerializeRelatedOptions(SendEntryIdx, &entryToOptionMap, OptionArrayInfoPtr);                  /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Check if all options fit to the payload. */
  if(bufferTooSmall == FALSE)
  {
    /* #200 Serialize the entry itself. */

    /* Fill the type field. The first bit of the Sd_EntryType is only used internally to differentiate STOP/NACK entries. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_TYPE_OFFSET),
      (uint8)(entryType & SD_ENTRY_TYPE_MASK)); /*lint !e655 */                                                         /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* Write the option index and number of options. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INDEX1_OFFSET), entryToOptionMap.Index1st);          /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INDEX2_OFFSET), entryToOptionMap.Index2nd);          /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_NROFOPTS_OFFSET),
      (uint8)(entryToOptionMap.Nr1stOptions << 4) | entryToOptionMap.Nr2ndOptions);                                     /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* Write Service and Instance IDs. */
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_SERVICEID_OFFSET),
      Sd_GetServiceIdOfSendEntry(SendEntryIdx));                                                                        /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INSTANCEID_OFFSET),
      Sd_GetInstanceIdOfSendEntry(SendEntryIdx));                                                                       /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* Determine the correct TTL. */
    switch(entryType)
    {
#if(SD_CLIENT == STD_ON)
      case SD_ENTRY_FIND_SERVICE:
        usedTtl = Sd_GetTTLOfClientTimer(Sd_GetClientTimerIdxOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx)));
        break;
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
        usedTtl = Sd_GetTTLOfClientTimer(Sd_GetClientTimerIdxOfConsumedEventgroup(Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx)));
        break;
# endif
#endif
#if(SD_SERVER == STD_ON)
      case SD_ENTRY_OFFER_SERVICE:
        usedTtl = Sd_GetTTLOfServerTimer(Sd_GetServerTimerIdxOfServer(Sd_GetServerIdxOfSendEntry(SendEntryIdx)));
        break;
# if(SD_EVENTHANDLER == STD_ON)
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
        usedTtl = Sd_GetRequestedTTLOfClientList(Sd_GetClientListIdxOfSendEntry(SendEntryIdx));
        break;
# endif
#endif
      default:
        /* STOP/NACK entry with TTL = 0 */
        usedTtl = 0;
        break;
    }

    /* Write Major Version. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_MAJORVERSION_OFFSET),
      Sd_GetMajorVersionOfSendEntry(SendEntryIdx));                                                                     /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* Write TTL . */
    Sd_Util_Write24Bit(Sd_GetAddrPayload((PayloadEntryIdx +  SD_ENTRY_TTL_OFFSET)), usedTtl);                           /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* Write the Minor Version for send entries of format type 1 or the eventgroup ID for a format type 2 entry. */
    switch(entryType)
    {
#if(SD_CLIENT == STD_ON)
      case SD_ENTRY_FIND_SERVICE:
        Sd_Util_Write32Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY1_MINORVERSION_OFFSET),
          Sd_GetMinorVersionOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx)));                                        /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        break;
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
      case SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP:
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_RESERVEDCOUNTER_OFFSET), 0);                   /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_EVENTGROUPID_OFFSET),
          Sd_GetEventGroupIdOfConsumedEventgroup(Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx)));                /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        break;
# endif
#endif
#if(SD_SERVER == STD_ON)
      case SD_ENTRY_OFFER_SERVICE:
      case SD_ENTRY_STOP_OFFER_SERVICE:
        Sd_Util_Write32Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY1_MINORVERSION_OFFSET),
          Sd_GetMinorVersionOfServer(Sd_GetServerIdxOfSendEntry(SendEntryIdx)));                                        /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        break;
# if(SD_EVENTHANDLER == STD_ON)
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK:
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_RESERVEDCOUNTER_OFFSET),
          Sd_GetReservedFieldOfSendEntry(SendEntryIdx));                                                                /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_EVENTGROUPID_OFFSET),
          Sd_GetEventHandlerIdOfSendEntry(SendEntryIdx));                                                               /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        break;
# endif
#endif
      default:
        /* Nothing to do. */
        break;
    }
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantEndpointOptions()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_GetRelevantEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerUdpPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerTcpPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SockAddrContainerUdpPtr->family = SOAD_AF_INVALID;                                                                    /* SBSW_SD_VALID_API_POINTER */
  SockAddrContainerTcpPtr->family = SOAD_AF_INVALID;                                                                    /* SBSW_SD_VALID_API_POINTER */

  /* #10 Check which type of entry shall be transmitted. */
  switch(Sd_GetEntryTypeOfSendEntry(SendEntryIdx))
  {
    case SD_ENTRY_FIND_SERVICE:
    case SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK:
      /* #100 FindService and SubscribeEventgroupNack entries do not allow any endpoint options. */
      break;

#if(SD_SERVER == STD_ON)
    case SD_ENTRY_OFFER_SERVICE:
    case SD_ENTRY_STOP_OFFER_SERVICE:
      {
        Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfSendEntry(SendEntryIdx);

        /* #101 (Stop)OfferService entries require unicast endpoint options.
         *      Check if the service is configured for UDP and save the address. */
        if(Sd_GetUdpSoConIdStartIdxOfServer(serverIdx) != SD_NO_UDPSOCONIDSTARTIDXOFSERVER)
        {
          Sd_Util_GetLocalAddr(Sd_GetUdpSoConId(Sd_GetUdpSoConIdStartIdxOfServer(serverIdx)), SockAddrContainerUdpPtr); /* SBSW_SD_PASSING_API_POINTER */
        }

# if(SD_EVENTHANDLER == STD_ON)
        if(SockAddrContainerUdpPtr->family == SOAD_AF_INVALID)
        {
          Sd_SizeOfEventHandlerType eventHandlerIdx;

          /* #1010 Check if the service contains an event handler which is configured for multicast. */
          for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx);
            eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx);
            eventHandlerIdx++)
          {
            if(Sd_EventHandlerUsesMulticast(eventHandlerIdx))
            {
              Sd_Util_GetLocalAddr(
                Sd_GetSoConIdOfEventHandlerMulticast(Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx)),
                SockAddrContainerUdpPtr);                                                                               /* SBSW_SD_PASSING_API_POINTER */

              break;
            }
          }
        }
# endif

        /* #102 Check if the service is configured for TCP and save the address. */
        if(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx) != SD_NO_TCPSOCONIDSTARTIDXOFSERVER)
        {
          Sd_Util_GetLocalAddr(Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx)), SockAddrContainerTcpPtr); /* SBSW_SD_PASSING_API_POINTER */
        }
      }
      break;
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
    case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
    case SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP:
      {
        Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx = Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx);

        /* #103 (Stop)SubscribeEventgroup entries allow unicast endpoint options.
         *      Check if the service is configured for UDP and save the address. */
        if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
        {
          Sd_Util_GetLocalAddr(
            Sd_GetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx), SockAddrContainerUdpPtr);                   /* SBSW_SD_PASSING_API_POINTER */
        }

        /* #104 Check if the service is configured for TCP and save the address. */
        if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx) != SOAD_INV_SO_CON_ID)
        {
          Sd_Util_GetLocalAddr(
            Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx), SockAddrContainerTcpPtr);                   /* SBSW_SD_PASSING_API_POINTER */
        }
      }
      break;
#endif

#if(SD_EVENTHANDLER == STD_ON)
    case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
      {
        Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfSendEntry(SendEntryIdx);

        /* #105 SubscribeEventgroupAck entries allow multicast endpoint options.
         *      Check if the event handler is configured for multicast. */
        if(Sd_EventHandlerUsesMulticast(eventHandlerIdx))
        {
          Std_ReturnType retVal;

          /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
          retVal = SoAd_GetRemoteAddr(
            Sd_GetSoConIdOfEventHandlerMulticast(Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx)),
            (SD_P2VAR(SoAd_SockAddrType))SockAddrContainerUdpPtr);                                                      /* SBSW_SD_PASSING_API_POINTER */

          if(retVal != E_OK)
          {
            /* Not able to get local address. */
            SockAddrContainerUdpPtr->family = SOAD_AF_INVALID;                                                          /* SBSW_SD_VALID_API_POINTER */
          }
        }
      }
      break;
#endif

    default:
      /* #106 Undefined EntryType. */
      break;
  }
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantConfigOptions()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_GetRelevantConfigOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameEndIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordEndIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a configuration option is needed. */
  if((Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_FIND_SERVICE) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_OFFER_SERVICE) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx) == SD_ENTRY_STOP_OFFER_SERVICE) ||
    (Sd_GetServiceIdOfSendEntry(SendEntryIdx) == SD_SERVICE_ID_OTHERSERV))
  {
    /* #100 Update the hostname start and end indexes. */
    *HostnameStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfInstance(Sd_GetInstanceIdxOfSendEntry(SendEntryIdx));        /* SBSW_SD_VALID_API_POINTER */
    *HostnameEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfInstance(Sd_GetInstanceIdxOfSendEntry(SendEntryIdx));            /* SBSW_SD_VALID_API_POINTER */

    /* #101 Check if capability records are configured. */
#if(SD_SERVER == STD_ON)
    if(Sd_GetServerIdxOfSendEntry(SendEntryIdx) != Sd_GetSizeOfServer())
    {
      *CapabilityRecordStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfServer(Sd_GetServerIdxOfSendEntry(SendEntryIdx));  /* SBSW_SD_VALID_API_POINTER */
      *CapabilityRecordEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfServer(Sd_GetServerIdxOfSendEntry(SendEntryIdx));      /* SBSW_SD_VALID_API_POINTER */
    }
    else
#endif
#if(SD_CLIENT == STD_ON)
    if(Sd_GetClientIdxOfSendEntry(SendEntryIdx) != Sd_GetSizeOfClient())
    {
      *CapabilityRecordStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx));  /* SBSW_SD_VALID_API_POINTER */
      *CapabilityRecordEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx));      /* SBSW_SD_VALID_API_POINTER */
    }
    else
#endif
    {
      /* #1010 No capability records present (NACK for unknown service). */
#if(SD_SERVER == STD_ON)
      *CapabilityRecordStartIdxPtr = SD_NO_CAPABILITYRECORDSTARTIDXOFSERVER;                                            /* SBSW_SD_VALID_API_POINTER */
      *CapabilityRecordEndIdxPtr = SD_NO_CAPABILITYRECORDENDIDXOFSERVER;                                                /* SBSW_SD_VALID_API_POINTER */
#else
      *CapabilityRecordStartIdxPtr = SD_NO_CAPABILITYRECORDSTARTIDXOFCLIENT;                                            /* SBSW_SD_VALID_API_POINTER */
      *CapabilityRecordEndIdxPtr = SD_NO_CAPABILITYRECORDENDIDXOFCLIENT;                                                /* SBSW_SD_VALID_API_POINTER */
#endif
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckForDuplicateOption()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CheckForDuplicateOption(
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isDuplicateOption = FALSE;
  uint16 tmpPayloadIdx = OptionArrayInfoPtr->ArrayStartIdx, optionOffset, optionLength;
  uint8 lastDeduplicateAbleOptionIndex = SD_INV_OPTION_INDEX;

  /* ----- Implementation ----------------------------------------------- */
  OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                    /* SBSW_SD_VALID_API_POINTER */

  /* #10 Check if the payload contains the new option which was serialized after the end of the options array. */
  while(tmpPayloadIdx < OptionArrayInfoPtr->ArrayEndIdx)
  {
    /* #100 Read length of next option. */
    IPBASE_GET_UINT16(Sd_GetAddrPayload(0), tmpPayloadIdx, optionLength);

    /* Consider entire option. */
    optionLength += SD_OPTION_NOT_COVERED_FIELDS;

    optionOffset = 0;
    OptionArrayInfoPtr->DeduplicatedOptionIndex++;                                                                      /* SBSW_SD_VALID_API_POINTER */
    isDuplicateOption = TRUE;

    /* #101 Compare all bytes of the already serialized option. */
    while(optionOffset < optionLength)
    {
      /* #1010 Check if the current option is equivalent to the option that has been serialized already. */
      if(Sd_GetPayload(tmpPayloadIdx + optionOffset) != Sd_GetPayload(OptionArrayInfoPtr->ArrayEndIdx + optionOffset))
      {
        /* #10100 Option does not match and cannot be used for deduplication. Skip option. */
        isDuplicateOption = FALSE;
        break;
      }

      optionOffset++;
    }

    if(isDuplicateOption == TRUE)
    {
      /* #1011 Save option for possible deduplication. Repeat searching in order to find the last matching option. */
      lastDeduplicateAbleOptionIndex = OptionArrayInfoPtr->DeduplicatedOptionIndex;
    }

    tmpPayloadIdx += optionLength;
  }

  if(isDuplicateOption == TRUE)
  {
    /* #102 Return the last matching option.
     *      The struct member DeduplicatedOptionIndex is already set to the correct value. */
  }
  else if(lastDeduplicateAbleOptionIndex != SD_INV_OPTION_INDEX)
  {
    /* #103 Last checked options does not match, return the last matching option. */
    OptionArrayInfoPtr->DeduplicatedOptionIndex = lastDeduplicateAbleOptionIndex;                                       /* SBSW_SD_VALID_API_POINTER */
  }
  else
  {
    /* #104 No matching option found. */
    OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                  /* SBSW_SD_VALID_API_POINTER */
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_DeduplicateOption()
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
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_DeduplicateOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  boolean RemainingOption)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ableToDeduplicateOption = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the new option was already serialized to the payload. */
  Sd_Tx_CheckForDuplicateOption(OptionArrayInfoPtr);                                                                    /* SBSW_SD_PASSING_API_POINTER */

  if(OptionArrayInfoPtr->DeduplicatedOptionIndex != SD_INV_OPTION_INDEX)
  {
    ableToDeduplicateOption = TRUE;

    /* #100 Option was already serialized to the payload. Check if it can be deduplicated. */

    if(EntryToOptionMapPtr->Nr1stOptions == 0)
    {
      /* #1000 Use first option run to index the option. */
      EntryToOptionMapPtr->Index1st = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if((EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions) == OptionArrayInfoPtr->DeduplicatedOptionIndex)
    {
      /* #1001 Extend first option run at the end to index new option. */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if((EntryToOptionMapPtr->Index1st - 1) == OptionArrayInfoPtr->DeduplicatedOptionIndex)
    {
      /* #1002 Extend first option run at the beginning to index new option. */
      EntryToOptionMapPtr->Index1st--;                                                                                  /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if((RemainingOption == FALSE) && (EntryToOptionMapPtr->Nr2ndOptions == 0))
    {
      /* #1003 Use second option run to index the option because there are no more pending options. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if(OptionArrayInfoPtr->FreeOptionIndex == (OptionArrayInfoPtr->DeduplicatedOptionIndex + 1))
    {
      /* #1004 Use second option run to index the option because the found option is the last one in the array. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if(OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions))
    {
      /* #1005 Use second option run to index the option because the last option of the array is indexed by the current entry. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if((EntryToOptionMapPtr->Nr2ndOptions != 0) && ((EntryToOptionMapPtr->Index2nd + EntryToOptionMapPtr->Nr2ndOptions) == OptionArrayInfoPtr->DeduplicatedOptionIndex))
    {
      /* #1006 Extend second option run at the end to index new option. */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else if((EntryToOptionMapPtr->Nr2ndOptions != 0) && ((EntryToOptionMapPtr->Index2nd - 1) == OptionArrayInfoPtr->DeduplicatedOptionIndex))
    {
      /* #1007 Extend second option run at the beginning to index new option. */
      EntryToOptionMapPtr->Index2nd--;                                                                                  /* SBSW_SD_VALID_API_POINTER */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_VALID_API_POINTER */
    }
    else
    {
      /* #1008 Option can not be deduplicated. */
      ableToDeduplicateOption = FALSE;
      OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                /* SBSW_SD_VALID_API_POINTER */
    }
  }

  return ableToDeduplicateOption;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SockAddrContainerType sockAddrContainerUdp, sockAddrContainerTcp;
  Sd_SizeOfCapabilityRecordType hostnameStartIdx, hostnameEndIdx;
  Sd_SizeOfCapabilityRecordType capabilityRecordStartIdx, capabilityRecordEndIdx;
  boolean bufferTooSmall = FALSE, configOptionPresent = FALSE;
  uint16 additionalOptionSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get relevant configuration options. */
  Sd_Tx_GetRelevantConfigOptions(SendEntryIdx, &hostnameStartIdx, &hostnameEndIdx,
    &capabilityRecordStartIdx, &capabilityRecordEndIdx);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if a configuration option has to be added. */
  if((hostnameStartIdx != hostnameEndIdx) || (capabilityRecordStartIdx != capabilityRecordEndIdx))
  {
    configOptionPresent = TRUE;
  }

  /* #30 Get relevant endpoint options. */
  Sd_Tx_GetRelevantEndpointOptions(SendEntryIdx, &sockAddrContainerUdp, &sockAddrContainerTcp);                         /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #40 Serialize UDP endpoint option. */
  if(sockAddrContainerUdp.family != SOAD_AF_INVALID)
  {
    /* #400 Serialize option to the end of the options array. */
    additionalOptionSize = Sd_Tx_SerializeEndpointOption(Sd_GetEntryTypeOfSendEntry(SendEntryIdx),
      &sockAddrContainerUdp, SD_L4PROTOCOL_UDP, OptionArrayInfoPtr->ArrayEndIdx);                                       /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #401 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #4010 Check if the option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(EntryToOptionMapPtr, OptionArrayInfoPtr, TRUE))                               /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #40100 Use the option, which was already serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  /* #50 Serialize TCP endpoint option. */
  if((sockAddrContainerTcp.family != SOAD_AF_INVALID) && (bufferTooSmall == FALSE))
  {
    /* #500 Serialize option to the end of the payload. */
    additionalOptionSize = Sd_Tx_SerializeEndpointOption(Sd_GetEntryTypeOfSendEntry(SendEntryIdx),
      &sockAddrContainerTcp, SD_L4PROTOCOL_TCP, OptionArrayInfoPtr->ArrayEndIdx);                                       /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #501 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #5010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(EntryToOptionMapPtr, OptionArrayInfoPtr, configOptionPresent))                /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #50100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOption()
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
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeEndpointOption(
  Sd_EntryType SendEntryType,
  SD_P2CONST(Sd_SockAddrContainerType) LocalIpAddrPtr,
  Sd_L4ProtocolType L4Proto,
  uint16 PayloadOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionType optionType = SD_OPTION_IPV4_ENDPOINT;
  uint16 optionLength = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of option. */
  if(LocalIpAddrPtr->family == SOAD_AF_INET6)
  {
    optionType = SD_OPTION_IPV6_ENDPOINT;
  }

  /* #20 Check if a multicast endpoint has to be added. */
  if(SendEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK)
  {
    optionType = (Sd_OptionType)(optionType | SD_OPTION_MULTICAST_MASK); /*lint !e655 */
  }

  switch(optionType)
  {
    case SD_OPTION_IPV4_ENDPOINT:
    case SD_OPTION_IPV4_MULTICAST:
      /* #200 Check if the IPv4 endpoint can be written into the buffer. */
      if((PayloadOffset + SD_OPTION_IPV4_SIZE) >= Sd_GetSizeOfPayload())
      {
        /* #2000 Stop processing of the endpoint option. */
        optionLength = SD_INV_OPTION_LENGTH;
      }
      else
      {
        /* #2001 Serialize all fields of the IPv4 endpoint option. */

        /* Set the length field. */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_LENGTH_OFFSET), SD_OPTION_IPV4_COVERED_LENGTH);  /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */
        optionLength = SD_OPTION_IPV4_SIZE;

        /* Set the type field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET), (uint8)optionType);                 /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the first reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_RESERVED_OFFSET), 0u);                            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the IP address. */
        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        Sd_Copy(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_ADDRESS_OFFSET),                                       /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          LocalIpAddrPtr->data, SD_OPTION_IPV4_ADDRESS_SIZE);

        /* Set the second reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_RESERVED_OFFSET), 0);                        /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the L4-Proto field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_L4PROTO_OFFSET), (uint8)L4Proto);            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the port number. */
        /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
        /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
        Sd_Copy(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_PORTNUMBER_OFFSET),                                    /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          &LocalIpAddrPtr->port, SD_OPTION_IPV4_PORTNUMBER_SIZE);
      }
      break;

    case SD_OPTION_IPV6_ENDPOINT:
    case SD_OPTION_IPV6_MULTICAST:
      /* #201 Check if the IPv6 endpoint can be written into the buffer. */
      if((PayloadOffset + SD_OPTION_IPV6_SIZE) >= Sd_GetSizeOfPayload())
      {
        /* #2010 Stop processing of the endpoint option. */
        optionLength = SD_INV_OPTION_LENGTH;
      }
      else
      {
        /* #2011 Serialize all fields of the IPv6 endpoint option. */

        /* Set the length field. */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_LENGTH_OFFSET), SD_OPTION_IPV6_COVERED_LENGTH);  /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */
        optionLength = SD_OPTION_IPV6_SIZE;

        /* Set the type field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET), (uint8)optionType);                 /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the first reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_RESERVED_OFFSET), 0);                             /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the IP address. */
        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        Sd_Copy(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_ADDRESS_OFFSET),                                       /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          LocalIpAddrPtr->data, SD_OPTION_IPV6_ADDRESS_SIZE);

        /* Set the second reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_RESERVED_OFFSET), 0);                        /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the L4-Proto field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_L4PROTO_OFFSET), (uint8)L4Proto);            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

        /* Set the port number. */
        /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
        /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
        Sd_Copy(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_PORTNUMBER_OFFSET),                                    /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          &LocalIpAddrPtr->port, SD_OPTION_IPV6_PORTNUMBER_SIZE);
      }
      break;

    default: /* PRQA S 2018 */ /* MD_Sd_14.1_2018 */
      /* #202 Undefined OptionType. */
      break;
  }

  return optionLength;
}

#if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOptions()
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
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeConfigurationOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfCapabilityRecordType hostnameStartIdx, hostnameEndIdx;
  Sd_SizeOfCapabilityRecordType capabilityRecordStartIdx, capabilityRecordEndIdx;
  boolean bufferTooSmall = FALSE;
  uint16 additionalOptionSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get relevant configuration options. */
  Sd_Tx_GetRelevantConfigOptions(SendEntryIdx, &hostnameStartIdx, &hostnameEndIdx,
    &capabilityRecordStartIdx, &capabilityRecordEndIdx);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if hostname has to be added as configuration option. */
  if(hostnameStartIdx != hostnameEndIdx)
  {
    /* #200 Build and attach the configuration option. */
    additionalOptionSize = Sd_Tx_SerializeConfigurationOption(hostnameStartIdx, hostnameEndIdx,
      OptionArrayInfoPtr->ArrayEndIdx);

    /* #201 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #2010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(EntryToOptionMapPtr, OptionArrayInfoPtr,
        (boolean)(capabilityRecordStartIdx != capabilityRecordEndIdx)))                                                 /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #20100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  /* #30 Check if capability records have to be added as configuration option. */
  if(capabilityRecordStartIdx != capabilityRecordEndIdx)
  {
    /* #300 Build and attach the configuration option. */
    additionalOptionSize = Sd_Tx_SerializeConfigurationOption(capabilityRecordStartIdx, capabilityRecordEndIdx,
      OptionArrayInfoPtr->ArrayEndIdx);

    /* #301 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #3010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(EntryToOptionMapPtr, OptionArrayInfoPtr, FALSE))                              /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #30100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  return bufferTooSmall;
}
#endif

#if(SD_CAPABILITYRECORD == STD_ON)
/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOption()
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
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeConfigurationOption(
  Sd_SizeOfCapabilityRecordType ConfigurationOptionStartIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionEndIdx,
  uint16 PayloadOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfCapabilityRecordType configurationOptionLength = ConfigurationOptionEndIdx - ConfigurationOptionStartIdx;
  uint16 writtenBytes;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the configuration option fits into the buffer. */
  if((PayloadOffset + SD_OPTION_CONFIG_FIXED_HEADER_SIZE + configurationOptionLength) <= Sd_GetSizeOfPayload())
  {
    /* #100 Set the type field. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET), SD_OPTION_CONFIGURATION);               /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

    /* #101 Set the reserved field. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_RESERVED_OFFSET), 0);                                 /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */

    /* #102 Add the configuration option. */ /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
    Sd_Copy(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_CONFIG_STRING_OFFSET),                                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      Sd_GetAddrCapabilityRecord(ConfigurationOptionStartIdx), configurationOptionLength);

    /* #103 Set the length of the capability record. */
    writtenBytes = (uint16)(SD_OPTION_CONFIG_STRING_OFFSET + configurationOptionLength);
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_LENGTH_OFFSET),
      (uint16)(writtenBytes - SD_OPTION_NOT_COVERED_FIELDS));                                                           /* SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION */
  }
  else
  {
    /* #104 Buffer too small. */
    writtenBytes = SD_INV_OPTION_LENGTH;
  }

  return writtenBytes;
}
#endif

/**********************************************************************************************************************
 *  Sd_Tx_UseNewSerializedOption()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_UseNewSerializedOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  uint16 NewOptionSize)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Use option which was serialized to the end of the payload.
   *     Check if the option can be referenced by the first or the second option run. */

  if(EntryToOptionMapPtr->Nr1stOptions == 0)
  {
    /* #100 Use first option run to index the option. */
    EntryToOptionMapPtr->Index1st = OptionArrayInfoPtr->FreeOptionIndex;                                                /* SBSW_SD_VALID_API_POINTER */
    EntryToOptionMapPtr->Nr1stOptions++;                                                                                /* SBSW_SD_VALID_API_POINTER */
  }
  else if(OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions))
  {
    /* #101 Extend first option run at the end to index new option. */
    EntryToOptionMapPtr->Nr1stOptions++;                                                                                /* SBSW_SD_VALID_API_POINTER */
  }
  else if(EntryToOptionMapPtr->Nr2ndOptions == 0)
  {
    /* #102 Use second option run to index the option. */
    EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->FreeOptionIndex;                                                /* SBSW_SD_VALID_API_POINTER */
    EntryToOptionMapPtr->Nr2ndOptions++;                                                                                /* SBSW_SD_VALID_API_POINTER */
  }
  else if(OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index2nd + EntryToOptionMapPtr->Nr2ndOptions))
  {
    /* #103 Extend second option run at the end to index new option. */
    EntryToOptionMapPtr->Nr2ndOptions++;                                                                                /* SBSW_SD_VALID_API_POINTER */
  }
  else
  {
    /* #104 Not able to index new option. */
  }

  OptionArrayInfoPtr->FreeOptionIndex++;                                                                                /* SBSW_SD_VALID_API_POINTER */
  OptionArrayInfoPtr->ArrayEndIdx += NewOptionSize;                                                                     /* SBSW_SD_VALID_API_POINTER */
}

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllTcpSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllTcpSoCons(
  Sd_SizeOfClientType ClientIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_TcpSoConIdIterType tcpSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all TCP socket connections of the client. */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfClient(ClientIdx);
    tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfClient(ClientIdx);
    tcpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx), Abort);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllUdpSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllUdpSoCons(
  Sd_SizeOfClientType ClientIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_UdpSoConIdIterType udpSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all UDP socket connections of the client. */
  for(udpSoConIdIdx = Sd_GetUdpSoConIdStartIdxOfClient(ClientIdx);
    udpSoConIdIdx < Sd_GetUdpSoConIdEndIdxOfClient(ClientIdx);
    udpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetUdpSoConId(udpSoConIdIdx), Abort);
  }
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllMulticastSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllMulticastSoCons(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MulticastSoConIdIterType multicastSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all multicast socket connections of the client. */
  for(multicastSoConIdIdx = Sd_GetMulticastSoConIdStartIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
    multicastSoConIdIdx < Sd_GetMulticastSoConIdEndIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
    multicastSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetMulticastSoConId(multicastSoConIdIdx), Abort);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ResetDynamicVariables()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ResetDynamicVariables(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset dynamic variables. */
  Sd_SetStateMachineOfClientDyn(ClientIdx, SD_CLIENT_STATE_NONE);                                                       /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetEventOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE);                                                      /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetAddressStorageServerIdxOfClientDyn(ClientIdx, Sd_GetSizeOfAddressStorage());                                    /* SBSW_SD_API_ARGUMENT_IDX */
  (Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                                    /* SBSW_SD_API_ARGUMENT_IDX */
  (Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                                    /* SBSW_SD_API_ARGUMENT_IDX */
  (Sd_GetAddrServerMulticastEndpointAddrOfClientDyn(ClientIdx))->family = SOAD_AF_INVALID;                              /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                                     /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID);                                                     /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetOfferedTTLOfClientDyn(ClientIdx, 0);                                                                            /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx));                                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx));                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_SetRepetitionRunOfClientDyn(ClientIdx, 0);                                                                         /* SBSW_SD_API_ARGUMENT_IDX */
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_LocalIpAddrAssignmentChgReset()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset dynamic variables of all clients with and all consumed event groups. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx);
    clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx);
    clientIdx++)
  {
# if(SD_CONSUMEDEVENTGROUP == STD_ON)
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(clientIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(clientIdx);
      consumedEventgroupIdx++)
    {
      /* #1000 Reset dynamic variables of the consumed eventgroup. */
      Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE);                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, 0);                                           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */

      /* #1001 Inform the BswM about the state of the consumed eventgroups. */
      BswM_Sd_ConsumedEventGroupCurrentState(
        Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx), SD_CONSUMED_EVENTGROUP_DOWN);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN);                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
# endif

    /* #200 Reset dynamic variables of the client. */
    Sd_UtilClient_ResetDynamicVariables(clientIdx);

    /* #201 Inform the BswM about the state of the client service instance. */
    Sd_UtilClient_SetCurrentStateInformBswm(clientIdx, SD_CLIENT_SERVICE_DOWN);
  }
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_SetCurrentStateInformBswm()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_SetCurrentStateInformBswm(
  Sd_SizeOfClientType ClientIdx,
  Sd_ClientServiceCurrentStateType State)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the current state of the client. */
  Sd_SetCurrentStateOfClientDyn(ClientIdx, State);                                                                      /* SBSW_SD_API_ARGUMENT_IDX */

  /* #20 Inform the BswM about the state of the client service instance. */
  BswM_Sd_ClientServiceCurrentState(Sd_GetHandleIdOfClient(ClientIdx), State);
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_SetSoConMode()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetSoConMode(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SoConModeMapIterType soConModeMapIdx;
  boolean soConModeSaved = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the SoConId is already part of the map. */
  for(soConModeMapIdx = 0; soConModeMapIdx < Sd_GetSizeOfSoConModeMap(); soConModeMapIdx++)
  {
    if(SoConId == Sd_GetSoConIdOfSoConModeMap(soConModeMapIdx))
    {
      /* #100 Found the correct SoConId in the map. */
      soConModeSaved = TRUE;
      break;
    }
  }

  /* #20 Check if the SoConId was not found in the map. */
  if(soConModeSaved == FALSE)
  {
    /* #200 Search for an unused slot to save the mode. */
    for(soConModeMapIdx = 0; soConModeMapIdx < Sd_GetSizeOfSoConModeMap(); soConModeMapIdx++)
    {
      if(Sd_GetSoConIdOfSoConModeMap(soConModeMapIdx) == SOAD_INV_SO_CON_ID)
      {
        /* #2000 Found unused slot, save the mode of the SoConId. */
        Sd_SetSoConIdOfSoConModeMap(soConModeMapIdx, SoConId);                                                          /* SBSW_SD_CSL01 */
        soConModeSaved = TRUE;
        break;
      }
    }
  }

  /* #30 Check if the SoConId could be saved / was saved within the map. */
  if((soConModeSaved == TRUE) && (soConModeMapIdx < Sd_GetSizeOfSoConModeMap()))
  {
    /* #300 Update the mode. */
    if(((Sd_GetSoConModeOfSoConModeMap(soConModeMapIdx) == SD_SOCON_MODE_ONLINE) ||
      (Sd_GetSoConModeOfSoConModeMap(soConModeMapIdx) == SD_SOCON_MODE_RECONNECT)) &&
      (Mode == SOAD_SOCON_OFFLINE))
    {
      Sd_SetSoConModeOfSoConModeMap(soConModeMapIdx, SD_SOCON_MODE_LOST_CONNECTION);                                    /* SBSW_SD_CSL01 */
    }
    else
    {
       Sd_SetSoConModeOfSoConModeMap(soConModeMapIdx, (Sd_SoConModeType)Mode);                                          /* SBSW_SD_CSL01 */
    }
  }
}

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetSoConMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_SoConModeType, SD_CODE) Sd_Util_GetSoConMode(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SoConModeType tmpSoConMode = SD_SOCON_MODE_INVALID;
  Sd_SoConModeMapIterType soConModeMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for the SoConId. */
  for(soConModeMapIdx = 0; soConModeMapIdx < Sd_GetSizeOfSoConModeMap(); soConModeMapIdx++)
  {
    if(SoConId == Sd_GetSoConIdOfSoConModeMap(soConModeMapIdx))
    {
      /* #100 Found the socket connection. Return the actual mode. */
      tmpSoConMode = Sd_GetSoConModeOfSoConModeMap(soConModeMapIdx);
      break;
    }
  }

  return tmpSoConMode;
}
#endif

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUP == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetLocalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) SockAddrContainerPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SockAddrContainerType defaultRouterAddr;
  uint8 netmask;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Receive the locally configured address. */
  /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetLocalAddr(SoConId, (SD_P2VAR(SoAd_SockAddrType))SockAddrContainerPtr,
    &netmask, (SD_P2VAR(SoAd_SockAddrType))(&defaultRouterAddr));                                                       /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal != E_OK)
  {
    /* #100 Not able to get local address. Set the value to invalid. */
    SockAddrContainerPtr->family = SOAD_AF_INVALID;                                                                     /* SBSW_SD_VALID_API_POINTER */
  }
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_GetEmptySendEntry()
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
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Util_GetEmptySendEntry(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType tmpEntryIdx, emptyIdx = Sd_GetSendEntryEndIdxOfInstance(InstanceIdx);

  /* ----- Implementation ----------------------------------------------- */
  tmpEntryIdx = Sd_GetSendEntryStartIdxOfInstanceDyn(InstanceIdx);

  /* #10 Check if the list of send entries is empty. */
  if(tmpEntryIdx == SD_NO_SENDENTRYSTARTIDXOFINSTANCEDYN)
  {
    /* #100 Send entry list is empty. Choose the first element. */
    emptyIdx = Sd_GetSendEntryStartIdxOfInstance(InstanceIdx);
    Sd_SetValidOfSendEntry(emptyIdx, TRUE);                                                                             /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    Sd_SetSendEntryNextIdxOfSendEntry(emptyIdx, SD_NO_SENDENTRYNEXTIDXOFSENDENTRY);                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    Sd_IncSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx);                                                              /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_SetSendEntryStartIdxOfInstanceDyn(InstanceIdx, emptyIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_SetSendEntryEndIdxOfInstanceDyn(InstanceIdx, emptyIdx);                                                          /* SBSW_SD_API_ARGUMENT_IDX */
  }
  else
  {
    /* #101 Search for a free slot. */
    for(tmpEntryIdx = Sd_GetSendEntryStartIdxOfInstance(InstanceIdx);
      tmpEntryIdx < Sd_GetSendEntryEndIdxOfInstance(InstanceIdx);
      tmpEntryIdx++)
    {
      if(!Sd_IsValidOfSendEntry(tmpEntryIdx))
      {
        /* #10100 Found an empty slot. */
        emptyIdx = tmpEntryIdx;
        break;
      }
    }

    if(emptyIdx != Sd_GetSendEntryEndIdxOfInstance(InstanceIdx))
    {
      /* #1011 Found an empty slot, save the new send entry. */
      Sd_SetValidOfSendEntry(emptyIdx, TRUE);                                                                           /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      Sd_SetSendEntryNextIdxOfSendEntry(emptyIdx, SD_NO_SENDENTRYNEXTIDXOFSENDENTRY);                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      Sd_IncSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx);                                                            /* SBSW_SD_API_ARGUMENT_IDX */

      /* #1012 Update the linked list. */
      Sd_SetSendEntryNextIdxOfSendEntry(Sd_GetSendEntryEndIdxOfInstanceDyn(InstanceIdx), emptyIdx);                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetSendEntryEndIdxOfInstanceDyn(InstanceIdx, emptyIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX */
    }
    else
    {
      /* #1013 Not able to save additional send entry. Trigger DET error. */
      /* This should never happen, because all pending messages will be transmitted as soon as the send entry list is full. */
#if(SD_DEV_ERROR_REPORT == STD_ON)
      (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SAVE_SENDENTRY_IN_LIST, SD_E_CLIENT_LIST_FULL);
#endif
    }
  }

  return emptyIdx;
}

/**********************************************************************************************************************
 *  Sd_Util_RemoveSendEntryFromList()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_RemoveSendEntryFromList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfSendEntryType RemoveEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType tmpEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the entry to remove is the first entry of the linked list. */
  if(Sd_GetSendEntryStartIdxOfInstanceDyn(InstanceIdx) == RemoveEntryIdx)
  {
    /* #100 Invalidate the entry. */
    Sd_SetSendEntryStartIdxOfInstanceDyn(InstanceIdx, Sd_GetSendEntryNextIdxOfSendEntry(RemoveEntryIdx));               /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_SetValidOfSendEntry(RemoveEntryIdx, FALSE);                                                                      /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_SetSendEntryNextIdxOfSendEntry(RemoveEntryIdx, SD_NO_SENDENTRYNEXTIDXOFSENDENTRY);                               /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_DecSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx);                                                              /* SBSW_SD_API_ARGUMENT_IDX */

    /* #101 Check if the entry to remove is also the last entry of the linked list. */
    if(Sd_GetSendEntryEndIdxOfInstanceDyn(InstanceIdx) == RemoveEntryIdx)
    {
      /* #1010 The list is empty, update the end index. */
      Sd_SetSendEntryEndIdxOfInstanceDyn(InstanceIdx, SD_NO_SENDENTRYENDIDXOFINSTANCEDYN);                              /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }
  else
  {
    /* #102 Search the predecessor of the RemoveEntryIdx. */
    for(tmpEntryIdx = Sd_GetSendEntryStartIdxOfInstance(InstanceIdx);
      tmpEntryIdx < Sd_GetSendEntryEndIdxOfInstance(InstanceIdx);
      tmpEntryIdx++)
    {
      if(Sd_GetSendEntryNextIdxOfSendEntry(tmpEntryIdx) == RemoveEntryIdx)
      {
        /* #1020 Update the next entry index of the predecessor. */
        Sd_SetSendEntryNextIdxOfSendEntry(tmpEntryIdx, Sd_GetSendEntryNextIdxOfSendEntry(RemoveEntryIdx));              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */

        /* #1021 Invalidate the entry. */
        Sd_SetValidOfSendEntry(RemoveEntryIdx, FALSE);                                                                  /* SBSW_SD_API_ARGUMENT_IDX */
        Sd_SetSendEntryNextIdxOfSendEntry(RemoveEntryIdx, SD_NO_SENDENTRYNEXTIDXOFSENDENTRY);                           /* SBSW_SD_API_ARGUMENT_IDX */
        Sd_DecSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx);                                                          /* SBSW_SD_API_ARGUMENT_IDX */

        /* #1022 Check if the entry to remove is the last entry of the linked list. */
        if(Sd_GetSendEntryEndIdxOfInstanceDyn(InstanceIdx) == RemoveEntryIdx)
        {
          /* #10220 Update the end index of the list. */
          Sd_SetSendEntryEndIdxOfInstanceDyn(InstanceIdx, tmpEntryIdx);                                                 /* SBSW_SD_API_ARGUMENT_IDX */
        }

        break;
      }
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Util_CheckSendEntryListFullAndTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CheckSendEntryListFullAndTransmit(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the list with pending send entries is full.  */
  if(Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx) >=
    (Sd_GetSendEntryEndIdxOfInstance(InstanceIdx) - Sd_GetSendEntryStartIdxOfInstance(InstanceIdx)))
  {
    /* #100 The send entry list is full, trigger a transmission. */
    Sd_Tx_ProcessPendingMessages(InstanceIdx, TRUE);
  }
}

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddFindEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddFindEntryToList(
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Currently the specification does not support sending FindService entries using unicast. */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(instanceIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_FIND_SERVICE entry. */

    Sd_UtilClient_ConfigureEntry(sendEntryIdx, ClientIdx);

    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_FIND_SERVICE);                                                    /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, Sd_GetAddressStorageStartIdxOfInstance(instanceIdx));              /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_ResetTimerToZero(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx));                                            /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx);
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddMulticastOfferEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddMulticastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  boolean StopOffer)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(instanceIdx))
  {
    /* #100 Save all information relevant for multicast SD_ENTRY_(STOP)_OFFER_SERVICE entry. */

    Sd_UtilServer_ConfigureEntry(sendEntryIdx, ServerIdx);

    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, Sd_GetAddressStorageStartIdxOfInstance(instanceIdx));              /* SBSW_SD_SEND_ENTRY_IDX */

    if(StopOffer == FALSE)
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_OFFER_SERVICE);                                                 /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_STOP_OFFER_SERVICE);                                            /* SBSW_SD_SEND_ENTRY_IDX */
    }

    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, 0);                                       /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx, 0);                                        /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_ResetTimerToZero(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx));                                            /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx);
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddUnicastOfferEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddUnicastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_SizeOfAddressStorageType SavedAddressStorageIdx,
  uint32 TransmissionOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(instanceIdx))
  {
    /* #100 Save all information relevant for unicast SD_ENTRY_OFFER_SERVICE entry. */

    Sd_UtilServer_ConfigureEntry(sendEntryIdx, ServerIdx);

    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_OFFER_SERVICE);                                                   /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, SavedAddressStorageIdx);                                           /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, 0);                                       /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx, 0);                                        /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_SetTimerOffset(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx), TransmissionOffset);                          /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx);
}
#endif

#if(SD_CONSUMEDEVENTGROUP == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddSubscribeEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddSubscribeEntryToList(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  boolean Subscribe,
  uint32 TransmissionOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx);

  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(instanceIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_(STOP)_SUBSCRIBE_EVENTGROUP entry. */

    Sd_UtilClient_ConfigureEntry(sendEntryIdx, clientIdx);

    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, Sd_GetAddressStorageServerIdxOfClientDyn(clientIdx));              /* SBSW_SD_SEND_ENTRY_IDX */

    if(Subscribe == TRUE)
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP);                                          /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP);                                     /* SBSW_SD_SEND_ENTRY_IDX */
    }

    Sd_SetConsumedEventgroupIdxOfSendEntry(sendEntryIdx, ConsumedEventgroupIdx);                                        /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, Sd_GetEventGroupIdOfConsumedEventgroup(ConsumedEventgroupIdx));       /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_SetTimerOffset(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx), TransmissionOffset);                          /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx);
}
#endif

#if(SD_EVENTHANDLER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupAckEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupAckEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(InstanceIdx);
  uint32 AdditionalDelay = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(InstanceIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK entry. */
    Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx);
    Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx);
    Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx);

    Sd_UtilServer_ConfigureEntry(sendEntryIdx, serverIdx);

    /* Update the entry list. */
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK);                                        /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, Sd_GetAddressStorageIdxOfClientList(clientListIdx));               /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetEventHandlerIdxOfSendEntry(sendEntryIdx, eventHandlerIdx);                                                    /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientListIdxOfSendEntry(sendEntryIdx, clientListIdx);                                                        /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx));                   /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx, Sd_GetRxReservedOfClientList(clientListIdx));                          /* SBSW_SD_SEND_ENTRY_IDX */

    if(Sd_GetEventOfRxEvent(RxEventIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI)
    {
      AdditionalDelay = Sd_Util_GetRandomNrInRangeForResponse(
        Sd_GetRequestResponseMinDelayOfServerTimer(Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx)),
        Sd_GetRequestResponseMaxDelayOfServerTimer(Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx)),
        FALSE);
    }

    Sd_Util_SetTimerOffset(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx), AdditionalDelay);                             /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(InstanceIdx);
}
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupNackEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupNackEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfServerType ServerIdx,
  uint16 ServiceId,
  uint16 InstanceId,
  uint8 MajorVersion,
  uint16 ReservedField,
  uint16 EventHandlerId,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfServerTimerType ServerTimerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(InstanceIdx);
  uint32 additionalDelay = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx != Sd_GetSendEntryEndIdxOfInstance(InstanceIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK entry. */
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK);                                       /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetAddressStorageIdxOfSendEntry(sendEntryIdx, AddressStorageIdx);                                                /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetInstanceIdxOfSendEntry(sendEntryIdx, InstanceIdx);                                                            /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfClient());                         /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetConsumedEventgroupIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfConsumedEventgroup()); /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetServerIdxOfSendEntry(sendEntryIdx, ServerIdx);                                                                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfEventHandler());             /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientListIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfClientList());                 /* UNUSED */                /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetServiceIdOfSendEntry(sendEntryIdx, ServiceId);                                                                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetInstanceIdOfSendEntry(sendEntryIdx, InstanceId);                                                              /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetMajorVersionOfSendEntry(sendEntryIdx, MajorVersion);                                                          /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, EventHandlerId);                                                      /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx, ReservedField);                                                        /* SBSW_SD_SEND_ENTRY_IDX */

    if(ServerTimerIdx != Sd_GetSizeOfServerTimer())
    {
      additionalDelay = Sd_Util_GetRandomNrInRangeForResponse(
        Sd_GetRequestResponseMinDelayOfServerTimer(ServerTimerIdx),
        Sd_GetRequestResponseMaxDelayOfServerTimer(ServerTimerIdx), TRUE);
    }

    Sd_Util_SetTimerOffset(Sd_GetAddrTimeToSendOfSendEntry(sendEntryIdx), additionalDelay);                             /* SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX */
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(InstanceIdx);
} /* PRQA S 6060 */ /* MD_Sd_STPAR_6060 */
#endif

#if(SD_SERVER == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilServer_ConfigureEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfServerType ServerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_InstanceIdxOfServerType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all used parameters of the send entry element. */
  Sd_SetInstanceIdxOfSendEntry(SendEntryIdx, instanceIdx);                                                              /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetServerIdxOfSendEntry(SendEntryIdx, ServerIdx);                                                                  /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetServiceIdOfSendEntry(SendEntryIdx, Sd_GetServiceIdOfServer(ServerIdx));                                         /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetInstanceIdOfSendEntry(SendEntryIdx, Sd_GetInstanceIdOfServer(ServerIdx));                                       /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetMajorVersionOfSendEntry(SendEntryIdx, Sd_GetMajorVersionOfServer(ServerIdx));                                   /* SBSW_SD_SEND_ENTRY_IDX */

  /* #20 Initialize all unused parameters of the send entry element with invalid values.  */
  Sd_SetClientIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClient());                                   /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfConsumedEventgroup());           /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetEventHandlerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfEventHandler());                       /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetClientListIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClientList());                           /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetEventHandlerIdOfSendEntry(SendEntryIdx, 0);                                                 /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetReservedFieldOfSendEntry(SendEntryIdx, 0);                                                  /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
}
#endif

#if(SD_CLIENT == STD_ON)
/**********************************************************************************************************************
 *  Sd_UtilClient_ConfigureEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfClientType ClientIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_InstanceIdxOfClientType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all used parameters of the send entry element. */
  Sd_SetInstanceIdxOfSendEntry(SendEntryIdx, instanceIdx);                                                              /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetClientIdxOfSendEntry(SendEntryIdx, ClientIdx);                                                                  /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetServiceIdOfSendEntry(SendEntryIdx, Sd_GetServiceIdOfClient(ClientIdx));                                         /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetInstanceIdOfSendEntry(SendEntryIdx, Sd_GetInstanceIdOfClient(ClientIdx));                                       /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetMajorVersionOfSendEntry(SendEntryIdx, Sd_GetMajorVersionOfClient(ClientIdx));                                   /* SBSW_SD_SEND_ENTRY_IDX */

  /* #20 Initialize all unused parameters of the send entry element with invalid values.  */
  Sd_SetServerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfServer());                                   /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetEventHandlerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfEventHandler());                       /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetClientListIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClientList());                           /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfConsumedEventgroup());           /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetEventHandlerIdOfSendEntry(SendEntryIdx, 0);                                                 /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetReservedFieldOfSendEntry(SendEntryIdx, 0);                                                  /* UNUSED */        /* SBSW_SD_SEND_ENTRY_IDX */
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_CopySockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CopySockAddr(
  SD_P2VAR(Sd_SockAddrContainerType) TgtSockPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SrcSockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy the socket address. */
  /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
  retVal = IpBase_CopySockAddr((SD_P2VAR(IpBase_SockAddrType))TgtSockPtr, (SD_P2CONST(IpBase_SockAddrType))SrcSockPtr); /* SBSW_SD_PASSING_API_POINTER */

  if(retVal == E_NOT_OK)
  {
    /* #100 Source address is invalid, set the target address as invalid. */
    TgtSockPtr->family = SOAD_AF_INVALID;                                                                               /* SBSW_SD_VALID_API_POINTER */
  }
}

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrIp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrIp(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MatchType matchType = SD_MATCH_FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of both socket addresses. */
  if((SockAPtr->family == SockBPtr->family) &&
    ((SockAPtr->family == SOAD_AF_INET) || (SockAPtr->family == SOAD_AF_INET6)))
  {
    /* #100 Check if the address state defines ANY possible IP address. */
    if((AddrState & SOAD_SOCON_MASK_IPADDR & SOAD_SOCON_MASK_ANY) != 0)
    {
      matchType = SD_MATCH_ANY;
    }
    else
    {
      /* #1000 Check if the IP addresses are equal. */ /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      if(TRUE == IpBase_SockIpAddrIsEqual(
        (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                          /* SBSW_SD_PASSING_API_POINTER */
      {
        matchType = SD_MATCH_TRUE;
      }
    }
  }

  return matchType;
}
#endif

#if(SD_CONSUMEDEVENTGROUPMULTICAST == STD_ON)
/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrPort()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrPort(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MatchType matchType = SD_MATCH_FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of both socket addresses. */
  if((SockAPtr->family == SockBPtr->family) &&
    ((SockAPtr->family == SOAD_AF_INET) || (SockAPtr->family == SOAD_AF_INET6)))
  {
    /* #100 Check if the address state defines ANY possible port. */
    if(((AddrState & SOAD_SOCON_MASK_PORT) & SOAD_SOCON_MASK_ANY) > 0)
    {
      matchType = SD_MATCH_ANY;
    }
    else
    {
      /* #1000 Check if the ports are equal. */ /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      if(TRUE == IpBase_SockPortIsEqual(
        (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                          /* SBSW_SD_PASSING_API_POINTER */
      {
        matchType = SD_MATCH_TRUE;
      }
    }
  }

  return matchType;
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_SockAddrIsEqual()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_SockAddrIsEqual(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean equal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family and the IP addresses. */ /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  if(TRUE == IpBase_SockIpAddrIsEqual(
    (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                              /* SBSW_SD_PASSING_API_POINTER */
  {
    /* #100 Check the Ports. */ /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    if(TRUE == IpBase_SockPortIsEqual(
      (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                            /* SBSW_SD_PASSING_API_POINTER */
    {
      equal = TRUE;
    }
  }

  return equal;
}

/**********************************************************************************************************************
 *  Sd_Rx_SockAddrWithinSubnet()
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
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Rx_SockAddrWithinSubnet(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 NetmaskCidr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean sameSubnet = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the family type of both addresses are identical. */
  if(SockAPtr->family == SockBPtr->family)
  {
    if(NetmaskCidr == 0)
    {
      /* #1000 This check considers the special case in which a 32 bit value would be shifted for 32 bits.
       *       This special case would lead to an undefined result. */
      sameSubnet = TRUE;
    }
    else
    {
      /* #1001 Check IPv4 addresses. The net mask can be in the range [1 .. 32]. */
      if((SockAPtr->family == SOAD_AF_INET) && (NetmaskCidr <= 32))
      {
        uint32 AddrA, AddrB;

        /* #10010 Generate a copy of the IP addresses in network byte order. */
        AddrA = IPBASE_SWAP32(SockAPtr->data[0]);
        AddrB = IPBASE_SWAP32(SockBPtr->data[0]);

        /* #10011 Shift to remove the variable parts of the IP address. */
        AddrA >>= (32u - NetmaskCidr);
        AddrB >>= (32u - NetmaskCidr);

        /* #10012 Compare the remaining par of the addresses. */
        if(AddrA == AddrB)
        {
          sameSubnet = TRUE;
        }
      }
      /* #1002 Check IPv6 addresses. The net mask can be in the range [1 .. 128]. */
      else if((SockAPtr->family == SOAD_AF_INET6) && (NetmaskCidr <= 128))
      {
        uint32 AddrA[4], AddrB[4];

        /* #10020 Generate a copy of the IP addresses in network byte order. */
        AddrA[0] = IPBASE_SWAP32(SockAPtr->data[0]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrA[1] = IPBASE_SWAP32(SockAPtr->data[1]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrA[2] = IPBASE_SWAP32(SockAPtr->data[2]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrA[3] = IPBASE_SWAP32(SockAPtr->data[3]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrB[0] = IPBASE_SWAP32(SockBPtr->data[0]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrB[1] = IPBASE_SWAP32(SockBPtr->data[1]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrB[2] = IPBASE_SWAP32(SockBPtr->data[2]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        AddrB[3] = IPBASE_SWAP32(SockBPtr->data[3]);                                                                    /* SBSW_SD_IP_ADDR_SIZE */

        /* #10021 Shift to remove the variable parts of the IP address. */
        if(NetmaskCidr < 32u)
        {
          AddrA[3] >>= (32u - NetmaskCidr);                                                                             /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[3] >>= (32u - NetmaskCidr);                                                                             /* SBSW_SD_IP_ADDR_SIZE */
        }
        else if(NetmaskCidr < 64u)
        {
          if(NetmaskCidr != 32u)
          {
            AddrA[2] >>= (64u - NetmaskCidr);                                                                           /* SBSW_SD_IP_ADDR_SIZE */
            AddrB[2] >>= (64u - NetmaskCidr);                                                                           /* SBSW_SD_IP_ADDR_SIZE */
          }
          AddrA[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
        }
        else if(NetmaskCidr < 96u)
        {
          if(NetmaskCidr != 64u)
          {
            AddrA[1] >>= (96u - NetmaskCidr);                                                                           /* SBSW_SD_IP_ADDR_SIZE */
            AddrB[1] >>= (96u - NetmaskCidr);                                                                           /* SBSW_SD_IP_ADDR_SIZE */
          }
          AddrA[2] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrA[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[2] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
        }
        else if(NetmaskCidr < 128u)
        {
          if(NetmaskCidr != 96u)
          {
            AddrA[0] >>= (128u - NetmaskCidr);                                                                          /* SBSW_SD_IP_ADDR_SIZE */
            AddrB[0] >>= (128u - NetmaskCidr);                                                                          /* SBSW_SD_IP_ADDR_SIZE */
          }
          AddrA[1] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrA[2] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrA[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[1] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[2] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
          AddrB[3] = 0;                                                                                                 /* SBSW_SD_IP_ADDR_SIZE */
        }
        else
        {
          /* #100210 Net mask is too big, check if both addresses are identical. */
        }

        /* #10022 Compare both IP addresses. */
        if((AddrA[0] == AddrB[0]) &&
          (AddrA[1] == AddrB[1]) &&
          (AddrA[2] == AddrB[2]) &&
          (AddrA[3] == AddrB[3]))
        {
          sameSubnet = TRUE;
        }
      }
      else
      {
        /* #10023 The family is undefined or the net mask is out of range. */
      }
    }
  }

  return sameSubnet;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Sd_Util_ReportDemEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ReportDemEvent(
  Sd_Dem_EventIdType EventId)
{
  /* ----- Implementation ----------------------------------------------- */
#if(SD_DEM_EVENTS_CONFIGURED == STD_ON)
  /* #10 Check if the DEM event is configured. */
  if(EventId != 0)
  {
    /* #100 Report DEM event. */
    Dem_ReportErrorStatus(EventId, DEM_EVENT_STATUS_FAILED);
  }
#else
  SD_DUMMY_STATEMENT(EventId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRange(
  uint32 MinValue,
  uint32 MaxValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 randNo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a random number has to be calculated */
  if(MinValue == MaxValue)
  {
    /* #100 No need to calculate a random number. */
    randNo = MinValue;
  }
  else
  {
    /* #101 Calculate a random number in the range:  MinValue <= RandInRange <= MaxValue. */
    randNo = (Sd_ExternalRandomNumberFct() % ((MaxValue + 1) - MinValue)) + MinValue;
  }

  return randNo;
}

#if((SD_SERVER == STD_ON) || (SD_CONSUMEDEVENTGROUP == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRangeForResponse()
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
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRangeForResponse(
  uint32 MinValue,
  uint32 MaxValue,
  boolean InterruptContext)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the context the function is called from. */
  if(InterruptContext == TRUE)
  {
    /* #100 Response is generated in context of message transmission. */

    if((MinValue != 0) && (MinValue != MaxValue))
    {
      /* #1000 Increase the lower bound in order to consider the elapsed time between the last MainFunction and the RxIndication. */
      MinValue += SD_MAIN_FCT_CYCLE_TIME_MS;
    }
  }
  else
  {
    /* #101 Response is generated in context of MainFunction. */

    if(MaxValue != 0)
    {
      /* #1010 Decrease the upper bound in order to consider the elapsed time between the last MainFunction and the RxIndication. */
      if(MinValue == MaxValue)
      {
        MinValue -= SD_MAIN_FCT_CYCLE_TIME_MS;
      }

      MaxValue -= SD_MAIN_FCT_CYCLE_TIME_MS;
    }
  }

  return Sd_Util_GetRandomNrInRange(MinValue, MaxValue);
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceeded()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceeded(
  Sd_ExtendedTimeType Timer,
  boolean EqualIsExceeded)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean exceeded = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check if the seconds are exceeded. */
  if(Sd_TimeMsSinceStart.Seconds > Timer.Seconds)
  {
    exceeded = TRUE;
  }
  else if(Timer.Seconds == Sd_TimeMsSinceStart.Seconds)
  {
    /* #100 Seconds are identical. Check milliseconds. */
    if(Sd_TimeMsSinceStart.Milliseconds > Timer.Milliseconds)
    {
      exceeded = TRUE;
    }
    else if((EqualIsExceeded) && (Timer.Milliseconds == Sd_TimeMsSinceStart.Milliseconds))
    {
      exceeded = TRUE;
    }
    else
    {
      /* #1000 Milliseconds are not exceeded. */
    }
  }
  else
  {
    /* #101 Seconds are not exceeded. */
  }

  SD_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return exceeded;
}

/**********************************************************************************************************************
 *  Sd_Util_TriggerStateMachineRun()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_TriggerStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set next state machine execution time to 0 in order to trigger execution in the next MainFunction. */
  (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx))->Seconds = 0u;                                              /* SBSW_SD_API_ARGUMENT_IDX */
  (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx))->Milliseconds = 0u;                                         /* SBSW_SD_API_ARGUMENT_IDX */
}

/**********************************************************************************************************************
 *  Sd_Util_SetNextStateMachineRun()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetNextStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_ExtendedTimeType Timer)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the next state machine execution is scheduled later than the given time.
   *     If so, it has to be re-scheduled to an earlier point in time. */
  if((Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx).Seconds > Timer.Seconds) ||
    ((Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx).Seconds == Timer.Seconds) &&
    (Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx).Milliseconds > Timer.Milliseconds)))
  {
    /* #100 Set next state machine execution time to the given time. */
    (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx))->Seconds      = Timer.Seconds;                            /* SBSW_SD_API_ARGUMENT_IDX */
    (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx))->Milliseconds = Timer.Milliseconds;                       /* SBSW_SD_API_ARGUMENT_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetWithTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the timer. */
  Sd_Util_SetTimerOffset(TimerPtr, OffsetMs);                                                                           /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Trigger state machine execution based on the given offset. */
  Sd_Util_SetNextStateMachineRun(InstanceIdx, *TimerPtr);
}

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffset(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs)
{
  /* ----- Implementation ----------------------------------------------- */

  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Update the timer with the given offset.
   *     The timer value is saved in separated variables for seconds and milliseconds. */
  TimerPtr->Seconds      = Sd_TimeMsSinceStart.Seconds      + (OffsetMs / 1000);                                        /* SBSW_SD_VALID_API_POINTER */
  TimerPtr->Milliseconds = Sd_TimeMsSinceStart.Milliseconds + (uint16)(OffsetMs % 1000);                                /* SBSW_SD_VALID_API_POINTER */

  /* #20 Handle the case if the amount of milliseconds exceeds one second. */
  TimerPtr->Seconds      = TimerPtr->Seconds                + (TimerPtr->Milliseconds / 1000);                          /* SBSW_SD_VALID_API_POINTER */
  TimerPtr->Milliseconds = TimerPtr->Milliseconds           % 1000;                                                     /* SBSW_SD_VALID_API_POINTER */

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZero()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToZero(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the timer. */
  TimerPtr->Seconds = 0u;                                                                                               /* SBSW_SD_VALID_API_POINTER */
  TimerPtr->Milliseconds = 0u;                                                                                          /* SBSW_SD_VALID_API_POINTER */
}

#if((SD_CLIENT == STD_ON) || (SD_EVENTHANDLER == STD_ON))
/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZeroTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToZeroTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  Sd_SizeOfInstanceType InstanceIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the timer. */
  Sd_Util_ResetTimerToZero(TimerPtr);                                                                                   /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Trigger state machine execution. */
  Sd_Util_TriggerStateMachineRun(InstanceIdx);
}
#endif

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ResetTimerToInvalid(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the timer values to invalid. */
  TimerPtr->Seconds = SD_INV_TIMER_VALUE_32BIT;                                                                         /* SBSW_SD_VALID_API_POINTER */
  TimerPtr->Milliseconds = SD_INV_TIMER_VALUE_16BIT;                                                                    /* SBSW_SD_VALID_API_POINTER */
}

/**********************************************************************************************************************
 *  Sd_Util_Write32Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write32Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  BufferPtr[0] = (uint8)((uint32_least)(Value) >> 24);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1] = (uint8)((uint32_least)(Value) >> 16);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[2] = (uint8)((uint32_least)(Value) >>  8);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[3] = (uint8)((uint32_least)(Value));                                                                        /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write24Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write24Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  BufferPtr[0] = (uint8)((uint32_least)(Value) >> 16);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1] = (uint8)((uint32_least)(Value) >>  8);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[2] = (uint8)((uint32_least)(Value));                                                                        /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write16Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write16Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint16 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  BufferPtr[0] = (uint8)((uint16_least)(Value) >>  8);                                                                  /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1] = (uint8)((uint16_least)(Value));                                                                        /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write8Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_Write8Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint8 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  BufferPtr[0] = (uint8)(Value);                                                                                        /* SBSW_SD_WRITE_PAYLOAD */
}

#define SD_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

  MD_Sd_11.4_0310:
    Reason:     Pointer casts are standard proceeding in TcpIp address data handling. (Rule 0310, 3305)
    Risk:       None
    Prevention: Not applicable.
  MD_Sd_11.5_0311:
    Reason:     Function requires variable input parameter instead of const input parameter. (Rule 0311)
    Risk:       Changes made inside the function are not realized
    Prevention: Not applicable.
  MD_Sd_12.4_3415:
    Reason:     Function does not change any value and has therefore no side effects. (Rule 3415)
    Risk:       Changes made inside the function are not realized
    Prevention: Covered by code review.
  MD_Sd_13.7:
    Reason:     Check is required by AUTOSAR. (Rule 3355, 3356 3359)
    Risk:       None
    Prevention: Not applicable.
  MD_Sd_14.1_2018:
    Reason:     Default label is provided as a defensive measure against possible future code changes and to comply
                with rule 15.3.
    Risk:       There is no risk.
    Prevention: Not applicable.
  MD_Sd_16.7_3673:
    Reason:     Pointer could be const in special configuration variant.
    Risk:       Function could try to change the element the pointer points to.
    Prevention: Covered by code review.

  MD_Sd_STCAL_6050:
    Reason:     Deeper nesting of functions increases call stack usage and runtime.
    Risk:       Understandability and testability might become too complex due to fan-out to many functions.
    Prevention: Covered by code review.
  MD_Sd_STPAR_6060:
    Reason:     Function handles specific task in the overall component behavior. This requires multiple input
                parameters.
    Risk:       Stack usage and runtime too high for target uC.
    Prevention: Not applicable.
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_SD_CSL01
    \DESCRIPTION      Access of array element with index generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_SD_CSL02
    \DESCRIPTION      Access of array element with index checked against size of different array.
                      (Arrays Xyz[] and XyzDyn[] depend on ComStackLib size relation.)
    \COUNTERMEASURE   \N  Qualified use-case CSL02 of ComStackLib

  \ID SBSW_SD_CSL03
    \DESCRIPTION      Access of array element with index modelled by indirection.
    \COUNTERMEASURE   \N  Qualified use-case CSL03 of ComStackLib

  \ID SBSW_SD_CSL_VAR_ELEM
    \DESCRIPTION      Pointer write access of variable element modelled by ComStackLib.
    \COUNTERMEASURE   \N  Access macro is generated by ComStackLib and therefore qualified.

  \ID SBSW_SD_GETVERSION_PARAM
    \DESCRIPTION      The Sd_GetVersionInfo writes to the 'versioninfo' pointer parameter. It is checked against NULL,
                      but this does not guarantee that the referenced memory is valid.
    \COUNTERMEASURE   \N  Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_PUBLIC_API_POINTER
    \DESCRIPTION      The function forwards the input pointer. It is checked against NULL_PTR,
                      but this does not guarantee that the referenced memory is valid.
    \COUNTERMEASURE   \N  Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_VALID_API_POINTER
    \DESCRIPTION      Write access to element which is passed as function argument.
    \COUNTERMEASURE   \N  It is assumed, that the input pointer is valid and the function only writes members of the
                          pointed struct.

  \ID SBSW_SD_PASSING_API_POINTER
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer was passed as function argument and is used to call sub-function.
                          The validity of the pointer is ensured by the caller.

  \ID SBSW_SD_PASSED_API_POINTER_NOT_NULL
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \R  The pointer is checked against NULL_PTR.

  \ID SBSW_SD_FUNCTION_CALL_NULL_PTR
    \DESCRIPTION      NULL_PTR is passed as function argument.
    \COUNTERMEASURE   \N  NULL_PTR is an allowed value to identify special use-case.
                          The pointer value is checked against NULL_PTR within the called function.

  \ID SBSW_SD_REFERENCE_OF_VARIABLE
    \DESCRIPTION      Pointer to variable is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a variable by the address operator.

  \ID SBSW_SD_REFERENCE_OF_POINTER_ELEMENT
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a member of a struct passed as reference function
                          argument. The validity of the function argument is ensured by the caller.

  \ID SBSW_SD_PDU_INFO_PTR
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  The PduInfoPtr is a pointer which is passed via the public API Sd_RxIndication().
                          It is checked against NULL_PTR and saved in the Sd_RxMessageInfoType structure.
                          The pointed structure contains a member SduDataPtr which is also assumed to be valid.
                          Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_RX_ADDRESS_STORAGE_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The AddressStorageIdx index is determined during reception of a message within
                          Sd_Rx_SaveAddr(). The index is obtained by qualified use-case CSL01 of ComStackLib within
                          Sd_RxIndication().

  \ID SBSW_SD_SEND_ENTRY_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The index is determined by the Sd_Util_GetEmptySendEntry() function.
                          The valid range of the index is ensured by this function and defined at the function
                          declaration.

  \ID SBSW_SD_REFERENCE_USING_SEND_ENTRY_IDX
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element with an index qualified by
                          SBSW_SD_SEND_ENTRY_IDX.

  \ID SBSW_SD_CONFIG_OPTION_PTR_ARRAY_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The ConfigOptionPtrArray array has a size of (Sd_GetMaxNrOptionsPerEntry() + 1).
                          The write access of element with index is checked with qualified use-case CSL01 of
                          ComStackLib.

  \ID SBSW_SD_ATTACHED_OPTIONS_PTR
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  The ConfigOptionPtrArrayPtr element of the Sd_AttachedOptionsType structure points to a local
                          array of size (Sd_GetMaxNrOptionsPerEntry() + 1).
                          The assignment is done directly after creation of the structure.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element which is obtained by qualified
                          use-case CSL01 of ComStackLib.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element which is obtained by same size Relation.
                          (Qualified use-case CSL02 of ComStackLib.)

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element with index given as function argument.
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element with index.
                          The index is obtained by same size relation of function argument.
                          (Qualified use-case CSL02 of ComStackLib).
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is obtained by referencing an array element with index.
                          The index is obtained by indirection of function argument.
                          (Qualified use-case CSL03 of ComStackLib).
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element by index.
                          The index is obtained by indirection and same size relation of function argument.
                          (Qualified use-cases CSL03 and CSL02 of ComStackLib). The valid range of the function argument
                          is defined at the function declaration and has to be ensured by the calling function.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_HANDLE_ID_INDIRECTION
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  Pointer is obtained by referencing an array element with index.
                          The index is obtained by indirection of HandleID given as function argument.
                          (Qualified use-case CSL03 of ComStackLib).
                          A previous runtime check ensures that the HandleID is smaller than the size of the array.

  \ID SBSW_SD_API_ARGUMENT_IDX
    \DESCRIPTION      Access of array element with index given as function argument is not checked for validity.
    \COUNTERMEASURE   \N  The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_SD_API_ARGUMENT_IDX_INDIRECTION
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Index is obtained by indirection (qualified use-case CSL03 of ComStackLib) of function
                          argument. The valid range of the function argument is defined at the function declaration and
                          has to be ensured by the calling function.

  \ID SBSW_SD_FUNCTION_PTR_CALL
    \DESCRIPTION      Calling a function pointer without checking the reference.
    \COUNTERMEASURE   \R  It is checked, that the corresponding service has a function pointer configured.
                          The index is obtained by indirection of function argument.
                          (Qualified use-case CSL03 of ComStackLib). The valid range of the function argument is defined
                          at the function declaration and has to be ensured by the calling function.

  \ID SBSW_SD_IP_ADDR_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  IPv6 addresses are saved in an array with fixed size of 4.
                          The used indices are constant and smaller as the array size.

  \ID SBSW_SD_RXEVENT_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  All RxEvents are saved in a ring buffer in the Sd_RxEvent[] array.
                          Each instance refers to successive elements which are connected by an indirection modelled by
                          ComStackLib. The administrative structure of the ring buffer (per instance) consists of
                          dynamic start and end indices and a full-flag. All values are updated consecutively in order
                          to prevent invalid states and to save only valid indices.

  \ID SBSW_SD_RXEVENT_MEMBER
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  If a RxEvent is saved in the ring buffer, all members of the structure (Event, ClientListIdx,
                          EventHandlerIdx) are updated with valid values and can be used to access the element in the
                          corresponding array.

  \ID SBSW_SD_REFERENCE_RXEVENT_MEMBER
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element by index.
                          The index is obtained by using a member of the RxEvent structure which is qualified by
                          SBSW_SD_RXEVENT_MEMBER.

  \ID SBSW_SD_TMP_SEND_ENTRY_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The size of the Sd_TmpSendEntry[] array is based on a size relation to the maximal
                          configured no. of send entries per instance. The access of the array is checked against this
                          boarder.

  \ID SBSW_SD_SERIALIZE_SEPARATELY
    \DESCRIPTION      Access of array element with fixed index "0" is not checked for validity.
    \COUNTERMEASURE   \N  The size of the Sd_TmpSendEntry[] array is based on a size relation to the maximal
                          configured no. of send entries per instance, which is guaranteed to be at least "1".

  \ID SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The size of the Sd_Payload[] array is checked against the minimal size of an SD message
                          containing the entire header (SD_HEADER_LENGTH), all entries
                          (Sd_GetNrOfTmpSendEntries() * SD_ENTRY_SIZE) and the corresponding length fields
                          (SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE) at the beginning
                          of Sd_Tx_SerializePendingMessages() function.
                          This check ensures, that the corresponding pointer does not exceed the size of the
                          Sd_Payload[] array.

  \ID SBSW_SD_REFERENCE_MESSAGE_SIZE_OPTION
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The size of the Sd_Payload[] array is checked for each option which is serialized.
                          This check ensures, that the corresponding pointer does not exceed the size of the
                          Sd_Payload[] array.

  \ID SBSW_SD_RX_MESSAGE_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The size of each received message is checked to be valid by Sd_Rx_ProcessHeader() function.
                          These checks ensure that the given entries and options array lengths are valid.
                          Hence, the payload pointer point to valid bytes of the received message.

  \ID SBSW_SD_WRITE_PAYLOAD
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The Sd_Util_Write<8|16|24|32>Bit() functions access the element given by the pointer as well
                          as the succeeding element(s). The validity of the pointer and the minimal amount of valid
                          remaining elements is defined at the function description and has to be satisfied by the
                          caller.

  \ID SBSW_SD_TX_ADDRESS_STORAGE_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Each valid send entry references its destination address (AddressStorageIdx).
                          This index is always set to an existing value if a send entry is saved
                          (Sd_Util_Add<Xyz>EntryToList()).

  \ID SBSW_SD_TRIGGER_TRANSMIT_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The list of trigger transmit requests is managed in a ring-buffer with start and end indices
                          and a full-flag. The list is modified by Sd_TriggerTransmit_<Xyz>() functions. There it is
                          ensured that the corresponding indices are always valid.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_SD_COMPATIBILITY
    \ACCEPT   TX
    \ACCEPT   XF
    \REASON   [COV_MSR_COMPATIBILITY]

  \ID COV_SD_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Sd.c
 *********************************************************************************************************************/
