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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file     Mirror.h
 *        \brief    MIRROR header file
 *
 *        \details  Implementation of Mirror component.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Matthias Mueller              vismmu        Vector Informatik GmbH
 *  Simon Gutjahr                 visgsi        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-09-30  vismmu  -             Creation
 *  01.00.01  2017-01-10  vismmu  ESCAN00093497 No reset of timeout MirrorDestPduMaxDelay after common transmission
 *            2017-01-10  vismmu  ESCAN00093507 CAN frames shorter than 8 Bytes
 *            2017-01-10  vismmu  ESCAN00093527 Header Base Timestamp too old
 *            2017-01-10  vismmu  ESCAN00093528 Data Timestamp not relative to the Base Timestamp
 *            2017-01-10  vismmu  ESCAN00093529 Config variants Link-time and Post-build do not work
 *            2017-01-10  vismmu  ESCAN00093531 More CAN channels are mirrored than active
 *            2017-01-10  vismmu  ESCAN00093533 CAN channel is not mirrored
 *  01.01.00  2017-03-15  visgsi  FEATC-616     Completion CAN-CAN Mirroring
 *                                ESCAN00094178 Support user defined network Id
 *                                ESCAN00094174 Support CAN range filter
 *                                ESCAN00094351 Support post-build loadable
 *                                ESCAN00094377 Support new API functions
 *                                ESCAN00094378 Improve maintainability of Can-Ip Mirroring
 *  02.00.00  2017-03-22  visgsi  STORYC-49     Release FEAT-1656 Mirroring (CDD)
 *********************************************************************************************************************/
#if !defined (MIRROR_H)
# define MIRROR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Mirror_Cfg.h"
# include "Mirror_Lcfg.h"
# include "Mirror_PBcfg.h"

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857  */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define MIRROR_VENDOR_ID                           (30u)
# define MIRROR_MODULE_ID                           (0xFFu)

/* AUTOSAR Software specification version information */
# define MIRROR_AR_RELEASE_MAJOR_VERSION            (4u)
# define MIRROR_AR_RELEASE_MINOR_VERSION            (0u)
# define MIRROR_AR_RELEASE_REVISION_VERSION         (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define MIRROR_SW_MAJOR_VERSION                    (2u)
# define MIRROR_SW_MINOR_VERSION                    (0u)
# define MIRROR_SW_PATCH_VERSION                    (0u)

# define MIRROR_INSTANCE_ID_DET                     (0x40u)

/* ----- API service IDs ----- */
# define MIRROR_SID_INIT                            (0x00u) /*!< Service ID: Mirror_Init() */
# define MIRROR_SID_START                           (0x01u) /*!< Service ID: Mirror_Start() */
# define MIRROR_SID_STOP                            (0x02u) /*!< Service ID: Mirror_Stop() */
# define MIRROR_SID_SWITCH_DEST_CHANNEL             (0x03u) /*!< Service ID: Mirror_SwitchDestChannel() */
# define MIRROR_SID_CAN_FRAME                       (0x04u) /*!< Service ID: Mirror_CanFrame() */
# define MIRROR_SID_GET_VERSION_INFO                (0x06u) /*!< Service ID: Mirror_GetVersionInfo() */
# define MIRROR_SID_SETFILTER                       (0x07u) /*!< Service ID: Mirror_SetFilter() */
# define MIRROR_SID_MAINFUNCTION                    (0x08u) /*!< Service ID: Mirror_MainFunction() */
# define MIRROR_SID_TXCONFIRMATION                  (0x0Fu) /*!< Service ID: Mirror_TxConfirmation() */
# define MIRROR_SID_DESTINATIONQUEUECANWRITE        (0x10u) /*!< Service ID: Mirror_DestChannelCanQueueWrite() */
# define MIRROR_SID_GET_DEST_CHANNEL                (0x11u) /*!< Service ID: Mirror_GetDestChannel() */
# define MIRROR_SID_IS_MIRROR_ACTIVE                (0x12u) /*!< Service ID: Mirror_IsMirrorActive() */
# define MIRROR_SID_IS_SOURCE_CHANNEL_STARTED       (0x13u) /*!< Service ID: Mirror_IsSourceChannelStarted() */
# define MIRROR_SID_SET_CAN_FILTER_STATE            (0x14u) /*!< Service ID: Mirror_SetCanFilterState() */
# define MIRROR_SID_GET_CAN_FILTER_STATE            (0x15u) /*!< Service ID: Mirror_GetCanFilterState() */
# define MIRROR_SID_GET_CAN_FILTER                  (0x16u) /*!< Service ID: Mirror_GetCanFilter() */
# define MIRROR_SID_DEINIT                          (0x17u) /*!< Service ID: Mirror_DeInit() */

/* ----- Error codes ----- */
# define MIRROR_E_NO_ERROR                          (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code */
# define MIRROR_E_UNINIT                            (0x01u) /*!< Error code: API service used without module initialization */
# define MIRROR_E_MODE_INV                          (0x02u) /*!< Runtime error: API service is used although mirror mode is inactive */
# define MIRROR_E_RETURN                            (0x03u) /*!< Runtime error: API service returned with E_NOT_OK */
# define MIRROR_E_PARAM_POINTER                     (0x04u) /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define MIRROR_E_PARAM_CTRL                        (0x05u) /*!< Error code: API service used with invalid controller index */
# define MIRROR_E_PARAM_CHANNEL                     (0x06u) /*!< Error Code: API service used with an invalid channel identifier or channel was not configured for the functionality of the calling API */
# define MIRROR_E_INIT_FAILED                       (0x07u) /*!< Error code: The service Mirror_Init() is called with invalid config pointer */
# define MIRROR_E_QUEUE_OVERRUN                     (0x17u) /*!< Error code: The destination channel queue is full */
# define MIRROR_E_QUEUE_INVALID_LENGTH              (0x18u) /*!< Error code: The incoming Frame can not be stored in the destination channel queue */
# define MIRROR_E_TIMEOUT_TXCONF                    (0x19u) /*!< Error code: Missing Tx Confirmation */
# define MIRROR_E_CANFILTER_ID                      (0x1Au) /*!< Error code: API service used with invalid can filter id */

/* ----- Modes ----- */
# define MIRROR_UNINIT                              (0x00u)
# define MIRROR_INIT                                (0x01u)

# define MIRROR_INVALID_NETW_HANDLE                 (0xFFu)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define MIRROR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern P2CONST(Mirror_ConfigType, AUTOMATIC, MIRROR_PBCFG) Mirror_ConfigDataPtr;

#define MIRROR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define MIRROR_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Mirror_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, MIRROR_CODE) Mirror_InitMemory(void);

/**********************************************************************************************************************
 * Mirror_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module Mirror. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   configPtr    Configuration structure for initializing the module, must not be NULL_PTR.
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         Mirror_InitMemory has been called unless Mirror_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, MIRROR_CODE) Mirror_Init(P2CONST(Mirror_ConfigType, AUTOMATIC, MIRROR_PBCFG) configPtr);

/**********************************************************************************************************************
 *  Mirror_DeInit()
 *********************************************************************************************************************/
/*! \brief       Resets the Mirror module to the uninitialized state.
 *  \details     The module is not truly shut down before all services and callback functions have terminated.
 *  \pre         The module must be in the initialized state.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, MIRROR_CODE) Mirror_DeInit(void);

# if (MIRROR_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Mirror_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Mirror_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if MIRROR_VERSION_INFO_API == STD_ON.
 *  \trace       CREQ-107318
 *********************************************************************************************************************/
FUNC(void, MIRROR_CODE) Mirror_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, MIRROR_APPL_VAR) versioninfo);
# endif

/**********************************************************************************************************************
 *  Mirror_SetFilter()
 *********************************************************************************************************************/
/*! \brief       Function sets a mirror mode filter. 
 *  \details     Function sets one mirror mode filter for a specific source bus.
 *  \param[in]   channel    ComMChannel index of the source bus the given filter is attached to
 *  \param[in]   filterId   Id of the given filter
 *  \param[in]   filter     Input parameter which defines a range or Id filter, must not be NULL_PTR.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SetCanFilter(CONST(NetworkHandleType, AUTOMATIC) channel, uint8 filterId, P2CONST(Mirror_CanFilterType, AUTOMATIC, MIRROR_APPL_DATA) filter);

/**********************************************************************************************************************
 * Mirror_SetCanFilterState()
 *********************************************************************************************************************/
/*! \brief       Function sets a mirror mode filter state. 
 *  \details     Function sets the mirror mode filter state for one filter for a specific source bus.
 *  \param[in]   channel    ComMChannel index of the source bus the given filter is attached to
 *  \param[in]   filterId   Id of the given filter
 *  \param[in]   isActive   True - Activate filter, False - Deactivate filter
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SetCanFilterState(NetworkHandleType channel, uint8 filterId, boolean isActive);

/**********************************************************************************************************************
 * Mirror_GetCanFilter()
 *********************************************************************************************************************/
/*! \brief        Function returns a can filter 
 *  \details      Function returns the CanId and CanIdMask for the given filterId of the given source channel.
 *  \param[in]    channel    ComMChannel index of the source bus the filter to return is attached to
 *  \param[in]    filterId   Id of the filter to return
 *  \param[out]   filter     Buffer for the filter values
 *  \return       E_NOT_OK - function has been called with invalid parameters.
 *  \return       E_OK - success
 *  \pre          Module is initialized.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE)  Mirror_GetCanFilter(NetworkHandleType channel, uint8 filterId, P2VAR(Mirror_CanFilterType, AUTOMATIC, MIRROR_APPL_DATA) filter);

/**********************************************************************************************************************
 * Mirror_GetCanFilterState()
 *********************************************************************************************************************/
/*! \brief       Function returns the state of a filter.
 *  \details     Function returns the state for the given filterId of the given source channel
 *  \param[in]   channel    ComMChannel index of the source bus the filter state to return is attached to
 *  \param[in]   filterId   Id of the filter state to return
 *  \param[out]  isActive   Buffer for the filter state
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_GetCanFilterState(NetworkHandleType channel, uint8 filterId, P2VAR(boolean, AUTOMATIC, MIRROR_APPL_DATA) isActive);

/**********************************************************************************************************************
 * Mirror_GetDestChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the current destination channel.
 *  \details     Function returns the ComMChannel index of the current used destination channel.
 *  \return      ComMChannel Id of the current used destination channel.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(NetworkHandleType, MIRROR_CODE) Mirror_GetDestChannel(void);

/**********************************************************************************************************************
 * Mirror_GetChannelType()
 *********************************************************************************************************************/
/*! \brief       Function returns the type of the given channel.
 *  \details     Function can be used for source and destination channel.
 *  \param[in]   channel   ComMChannel index of the channel to return the channel type.
 *  \return      Returns the type of the channel for the given ComMChannel.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Mirror_ChannelType, MIRROR_CODE)  Mirror_GetChannelType(NetworkHandleType channel);

/**********************************************************************************************************************
 * Mirror_SwitchDestChannel()
 *********************************************************************************************************************/
/*! \brief       Switch the destination channel.
 *  \details     Function change the destination channel. If the Mirror module is active, all active source channels 
                 will be stopped. To Restart the Mirror module the function Mirror_StartSourceChannel() must be used.
 *  \param[in]   channel    ComMChannel index of the new destination channel.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SwitchDestChannel(NetworkHandleType channel);

/**********************************************************************************************************************
 * Mirror_IsMirrorActive()
 *********************************************************************************************************************/
/*! \brief       Function returns if the Mirror module is active
 *  \details     Returns the global Mirror module state
 *  \return      TRUE  - Mirror module is active
 *  \return      FALSE - Mirror module is inactive
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, MIRROR_CODE) Mirror_IsMirrorActive(void);

/**********************************************************************************************************************
 * Mirror_IsSourceChannelStarted()
 *********************************************************************************************************************/
/*! \brief       Function returns the state of a source channel
 *  \details     Function returns the current state of the given source channel.
 *  \param[in]   channel  ComMChannel Index of the source channel
 *  \return      TRUE  - Source channel is started
 *  \return      FALSE - Source channel is not started
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, MIRROR_CODE) Mirror_IsSourceChannelStarted(NetworkHandleType channel);

/**********************************************************************************************************************
 *  Mirror_StartSourceChannel()
 *********************************************************************************************************************/
/*! \brief       Function activates the mirror mode. 
 *  \details     Function activates the mirror mode for a specific srcBus.
 *  \param[in]   channel    ComMChannel index of the source bus to start
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StartSourceChannel(CONST(NetworkHandleType, AUTOMATIC) channel);

/**********************************************************************************************************************
 *  Mirror_StopSourceChannel()
 *********************************************************************************************************************/
/*! \brief       Function deactivates the mirror mode. 
 *  \details     Function deactivates the mirror mode for all source buses.
 *  \param[in]   channel    ComMChannel index of the source bus to stop
 *  \return      E_NOT_OK - module is not initialized.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StopSourceChannel(CONST(NetworkHandleType, AUTOMATIC) channel);

/**********************************************************************************************************************
 * Mirror_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main function for timeout handling and FIFO processing.
 *  \details     This function takes care of the timeout handling and destination channel queue processing.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, MIRROR_APPL_CODE) Mirror_MainFunction(void);

# define MIRROR_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/*!
 * \exclusivearea  MIRROR_EXCLUSIVE_AREA_QUEUE
 *                 Ensure the consistency of the queue state.
 * \protects       Mirror_CurrentDestChannelIndex, Mirror_DestChannelState.
 * \usedin         Mirror_SwitchDestChannel, Mirror_StopSourceChannel, 
 *                 Mirror_MainFunction, Mirror_ReportCanFrame, Mirror_TxConfirmation.
 * \exclude        -
 * \length         LONG Several internal queue functions and a memory copy function is called. 
                        Depending on the data length of the queue element it can take a while.
 * \endexclusivearea
 */

/*!
 * \exclusivearea  MIRROR_EXCLUSIVE_AREA_TXPDU
 *                 Ensure the consistency of the Tx PDU state.
 * \protects       Mirror_CurrentDestChannelIndex, Mirror_DestChannel_TxPduLocked, Mirror_DestChannelIp_IsTransmit, 
 *                 Mirror_TxConfTimeout, Mirror_DestChannelIp_TxPduTimeout.
 * \usedin         Mirror_MainFunction, Mirror_ReportCanFrame, Mirror_TxConfirmation.
 * \exclude        -
 * \length         SHORT Only a few flags are checked. No calls to functions.
 * \endexclusivearea
 */

/*!
 * \exclusivearea  MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL
 *                 Ensure the consistency of global mirror state and the source channel filter.
 * \protects       Mirror_GlobalMode.
 * \usedin         Mirror_SetCanFilter, Mirror_SetCanFilterState, Mirror_GetCanFilter, Mirror_GetCanFilterState,
 *                 Mirror_IsSourceChannelStarted, Mirror_StartSourceChannel, Mirror_StopSourceChannel.
 * \exclude        -
 * \length         LONG Several Mirror functions are called. However, the whole processing shouldn't 
 *                      take long under normal conditions.
 * \endexclusivearea
 */

#endif /* MIRROR_H */

/**********************************************************************************************************************
 *  END OF FILE: Mirror.h
 *********************************************************************************************************************/
