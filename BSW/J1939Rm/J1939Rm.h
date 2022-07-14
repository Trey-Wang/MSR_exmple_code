/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*! \file  J1939Rm.h
 *  \brief SAE J1939 Request Manager (J1939Rm)
 *
 *  \details Main header of the SAE J1939 Request Manager.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Martin Schlodder              vismsr        Vector Informatik GmbH
 *  Thomas Albrecht               vistat        Vector Informatik GmbH
 *  Simon Gutjahr                 visgsi        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version  Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  0.01.00  2013-11-02  vismsr   -             First Version.
 *  0.01.01  2013-11-14  vismsr  ESCAN00071701  Fixed memory abstraction section names.
 *                               ESCAN00071942  Fixed Tx PDU and Queue handling.
 *                               ESCAN00071943  Added support for NULL address AC request from unknown user.
 *  0.01.02  2013-11-14  vistat  ESCAN00071701  Fixed memory abstraction section names.
 *  0.01.03  2013-11-18  vismsr  ESCAN00072028  Re-route transmitted requests to internal nodes.
 *  0.01.04  2013-11-27  vismsr  ESCAN00072230  Removed immediate initialization of local structs.
 *                               ESCAN00072265  Fixed DA of transmitted ACKM.
 *  0.01.05  2013-11-29  vismsr  ESCAN00072376  Fixed type of state parameter to J1939Rm_SetState.
 *  0.02.00  2014-02-28  vismsr  ESCAN00072898  Fixed compiler warning for conversion to PduInfoType.
 *                               ESCAN00073146  Fixed feedback of transmitted ACKM.
 *                               ESCAN00073521  Fixed DA of transmitted ACKM.
 *                               ESCAN00073970  Fixed channel indirection in SendAckInternal
 *                                -             Removed unused channel reference from queues.
 *                                -             Implemented module state handling and DET reporting.
 *  0.03.00  2014-06-16  vismsr  ESCAN00071603  Added callout to COM.
 *                               ESCAN00074032  Fixed assignment to uint8 variables.
 *                               ESCAN00074133  Support for post-build configuration.
 *                               ESCAN00075575  Added source identification.
 *  0.04.00  2015-01-29  vismsr  ESCAN00077426  Fixed inclusion of Det.h
 *                               ESCAN00079738  Changed ACKM to use DA in addition to AckAddr.
 *                                -             Introduced J1939Rm_Int.h.
 *  0.05.00  2015-03-26  vismsr  ESCAN00075913  J1939Rm_AckCode moved to Rte_J1939Rm_Type.h.
 *                               ESCAN00076734  Fixed check for non-J1939 channels.
 *                               ESCAN00081574  Fixed locking of Tx PDUs.
 *                               ESCAN00081648  Added DET check for valid AckCode.
 *                               ESCAN00081664  Added check for valid PGN received in RQST and ACKM.
 *                               ESCAN00082047  Fixed DET check for valid PGN.
 *                               ESCAN00082077  Fixed DA of ISOBUS ACKM transmitted from the queue.
 *  0.05.01  2015-06-08  vismsr  ESCAN00081742  Avoid access to variables in advance of DET checks.
 *                               ESCAN00082128  Fixed exclusive area access.
 *                               ESCAN00082285  Fixed priority of COM PDUs with MDL 4.
 *  1.00.00  2015-07-07  vismsr  ESCAN00081644  Added queue overrun notification.
 *                               ESCAN00081841  Accept ACKM with broadcast DA on ISOBUS.
 *                               ESCAN00083552  Changed return type of J1939Rm_CancelRequestTimeout to Std_ReturnType.
 *                               ESCAN00083573  Added include of Com.h.
 *                               ESCAN00083710  Send ACKM with broadcast DA on ISOBUS.
 *                               ESCAN00083797  Avoid length check if DLC check in CanIf is active.
 *  1.00.01  2015-09-02  vismsr  ESCAN00081802  Detection of illegitimate access to SendAck/SendRequest.
 *                               ESCAN00082180  Added check for correct channel and DA of COM-I-PDUs.
 *  1.00.02  2015-11-16  vismsr  ESCAN00084317  Changed AckCode DET check such that it does not stop execution.
 *                               ESCAN00085061  Added TxPduLock to TxConfirmation.
 *                               ESCAN00085127  Excluded unused code for J1939Nm only use case.
 *                               ESCAN00085212  Avoid NACK with NULL DA or AckAddress.
 *                               ESCAN00085700  Fixed compiler warnings caused by unused parameters.
 *                               ESCAN00086485  Added TxPduLock to main function.
 *                               ESCAN00086504  Protected against concurrent access to ACKM/RQST queues.
 *                               ESCAN00086514  Fixed broadcast DA of queued ISOBUS ACKM.
 *  1.01.00  2015-12-16  vismsr  ESCAN00084690  Support request of multiplexed COM-I-PDUs with RQST2.
 *                               ESCAN00086183  Separation of RTE and other callbacks to avoid MISRA warnings.
 *                               ESCAN00086498  Replaced intermediate TxPduLocks by ReqQueueLock and AckQueueLock.
 *  1.01.01  2016-02-04  vismsr  ESCAN00087494  Fixed handling of ackCode.
 *                               ESCAN00087757  Fixed initialization of local variables.
 *                               ESCAN00087785  Fixed handling of Request2 for other users than COM.
 *  2.00.00  2016-03-05  vismsr  ESCAN00087724  Extended API for ext id bytes, implemented Request2 transmission.
 *  2.00.01  2016-03-24  visgsi   -             Refactored source code to reduce code metrics.
 *  2.01.00  2016-05-03  visgsi  ESCAN00089548  Added check for invalid user IDs and invalid PDU IDs.
 *  2.02.00  2016-07-27  visgsi  ESCAN00090222  Separation of Request and Request2 message queue.
 *                               ESCAN00090301  Added check for invalid SA to request reception handling.
 *                               ESCAN00090342  Fixed queued request and acknowledgment messages in offline state.
 *                               ESCAN00090607  Added runtime error reporting.
 *                               ESCAN00091264  Added check for invalid SA to acknowledgement reception handling.
 *  2.02.01  2016-11-21  visgsi  ESCAN00091459  Evaluate DA of received Request or Request2 only if a node is online.
 *  2.02.02  2017-02-09  visgsi  ESCAN00093952  Fixed DET in J1939Rm_Init with wrong Service Id.
 *  2.02.03  2017-06-26  visgsi  STORYC-1413    Component Detailed Design.
 *                               ESCAN00095633  Compile error when J1939RmDevErrorDetect is disabled.
 *  2.02.04  2017-07-27  visgsi  STORYC-1413    Finished Component Detailed Design.
 *********************************************************************************************************************/
#if !defined (J1939RM_H)
# define J1939RM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "J1939Rm_Cfg.h"
# include "J1939Rm_Lcfg.h"
# include "J1939Rm_PBcfg.h"

# include "J1939Rm_Types.h"
# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define J1939RM_VENDOR_ID                      (30u)
# define J1939RM_MODULE_ID                      (59u)

/* AUTOSAR Specification version information */
# define J1939RM_AR_RELEASE_MAJOR_VERSION       (4u)
# define J1939RM_AR_RELEASE_MINOR_VERSION       (2u)
# define J1939RM_AR_RELEASE_REVISION_VERSION    (2u)

/* Vendor specific BSW module version information */
# define J1939RM_SW_MAJOR_VERSION               (2u)
# define J1939RM_SW_MINOR_VERSION               (2u)
# define J1939RM_SW_PATCH_VERSION               (4u)

/* Special user ID for sending Request with NULL source address */
# define J1939RM_NULLADDRESS_USER_ID            (0xFFu)

/*
 * Development Error Detection
 */
/* ----- API service IDs ----- */
/*! Service ID: J1939Rm_Init() */
# define J1939RM_SID_INIT                       (0x01u)
/*! Service ID: J1939Rm_DeInit() */
# define J1939RM_SID_DEINIT                     (0x02u)
/*! Service ID: J1939Rm_GetVersionInfo() */
# define J1939RM_SID_GETVERSIONINFO             (0x03u)
/*! Service ID: J1939Rm_MainFunction() */
# define J1939RM_SID_MAINFUNCTION               (0x04u)
/*! Service ID: J1939Rm_SetState() */
# define J1939RM_SID_SETSTATE                   (0x05u)
/*! Service ID: J1939Rm_SendRequest() */
# define J1939RM_SID_SENDREQUEST                (0x07u)
/*! Service ID: J1939Rm_CancelRequestTimeout() */
# define J1939RM_SID_CANCELREQUESTTIMEOUT       (0x08u)
/*! Service ID: J1939Rm_SendAck() */
# define J1939RM_SID_SENDACK                    (0x09u)
/*! Service ID: J1939Rm_RxIndication() */
# define J1939RM_SID_RXINDICATION               (0x42u)
/*! Service ID: J1939Rm_TxConfirmation() */
# define J1939RM_SID_TXCONFIRMATION             (0x40u)
/*! Service ID: ComRxIpduCallout() */
# define J1939RM_SID_COMRXIPDUCALLOUT           (0x28u)
/*! Proprietary service ID: J1939Rm_InitMemory() */
# define J1939RM_SID_INITMEMORY                 (0x80u)

/* -----Development Error codes ----- */
/*! Proprietary development error code: No error occurred */
# define J1939RM_E_NO_ERROR                     (0x00u)
/*! Development error code: An API was called while the module was uninitialized */
# define J1939RM_E_UNINIT                       (0x01u)
/*! Development error code: The Init API was called twice */
# define J1939RM_E_REINIT                       (0x02u)
/*! Development error code: An API service was called with a NULL pointer */
# define J1939RM_E_PARAM_POINTER                (0x03u)
/*! Development error code: An API service was called with a wrong ID */
# define J1939RM_E_INVALID_PDU_SDU_ID           (0x04u)
/*! Development error code: An API service was called with wrong network handle */
# define J1939RM_E_INVALID_NETWORK_ID           (0x05u)
/*! Development error code: The API J1939Rm_SetState was called with a wrong state */
# define J1939RM_E_INVALID_STATE                (0x06u)
/*! Development error code: An API was called with an illegal user ID */
# define J1939RM_E_INVALID_USER                 (0x07u)
/*! Development error code: An API was called with an unknown or illegal PGN */
# define J1939RM_E_INVALID_PGN                  (0x08u)
/*! Development error code: An API was called with an illegal priority */
# define J1939RM_E_INVALID_PRIO                 (0x09u)
/*! Development error code: An API was called with an illegal node address */
# define J1939RM_E_INVALID_ADDRESS              (0x0au)
/*! Development error code: An API was called with an illegal Boolean option */
# define J1939RM_E_INVALID_OPTION               (0x0bu)
/*! Development error code: An API was called with an illegal AckCode */
# define J1939RM_E_INVALID_ACK_CODE             (0x0cu)
/*! Development error code: An API was called with an illegal node ID */
# define J1939RM_E_INVALID_NODE                 (0x0du)
/*! Development error code: J1939Rm_Init called with invalid init structure */
# define J1939RM_E_INIT_FAILED                  (0x0eu)
/*! Proprietary development error code: An API was called that was not configured for the calling user */
# define J1939RM_E_INVALID_API                  (0x85u)
/*! Proprietary development error code: An API was called with invalid extended identifier bytes */
# define J1939RM_E_INVALID_EXTID_INFO           (0x88u)

/* -----Runtime Error codes ----- */
/*! Proprietary runtime error code: An illegal PDU size was reported by CanIf */
# define J1939RM_E_INVALID_PDU_SIZE             (0x80u)
/*! Proprietary runtime error code: Timeout of transmission confirmation callback */
# define J1939RM_E_TIMEOUT_TXCONF               (0x81u)
/*! Proprietary runtime error code: DA of ISOBUS ACKM differs from AddressAcknowledged */
# define J1939RM_E_ISOBUS_ACKM_ADDR             (0x82u)
/*! Proprietary runtime error code: Acknowledgement message could not be stored in the queue */
# define J1939RM_E_ACK_QUEUE_OVERRUN            (0x83u)
/*! Proprietary runtime error code: Request message could not be stored in the queue */
# define J1939RM_E_REQ_QUEUE_OVERRUN            (0x84u)
/*! Proprietary runtime error code: Received Request2 message contains an invalid Special Instructions code */
# define J1939RM_E_INVALID_SPEC_INSTR           (0x86u)
/*! Proprietary runtime error code: Received Acknowledgement message contains an invalid control byte */
# define J1939RM_E_INVALID_ACK_CB               (0x87u)
/*! Proprietary runtime error code: A PGN received within RQST, RQST2, or ACKM has an invalid format */
# define J1939RM_E_BAD_PGN                      (0x89u)
/*! Proprietary runtime error code: A RQST, RQST2, or ACKM message was received with an invalid source address */
# define J1939RM_E_BAD_SA                       (0x8au)
/*! Proprietary runtime error code: Request2 message could not be stored in the queue */
# define J1939RM_E_REQ2_QUEUE_OVERRUN           (0x8bu)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*================================================ J1939RM_START_SEC_CODE ===========================================*/
# define J1939RM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  J1939Rm_InitMemory
 *********************************************************************************************************************/
/*! \brief       Sets the global J1939Rm state to uninitialized.
 *  \details     -
 *  \pre         Module is not initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function should be used if the J1939Rm is not initialized by startup code.
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_InitMemory(void);

/**********************************************************************************************************************
 *  J1939Rm_Init
 *********************************************************************************************************************/
/*! \brief       Initializes the J1939 Request Manager.
 *  \details     Function only performs the parameter check and calls the internal initialization function.
 *  \param[in]   config          Pointer to configuration data structure.
 *  \pre         The module must be in the uninitialized state.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The config parameter is only required if the configuration is variant or changed at post-build time.
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_Init(P2CONST(J1939Rm_ConfigType, AUTOMATIC, J1939RM_INIT_DATA) config);

/**********************************************************************************************************************
 *  J1939Rm_DeInit
 *********************************************************************************************************************/
/*! \brief       Resets the J1939 Request Manager to the uninitialized state.
 *  \details     Function only performs the parameter check and calls the internal deinitialization function.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The module is not truly shut down before all services and callback functions have terminated.
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_DeInit(void);

# if (J1939RM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief       Returns the version information of the J1939 Request Manager.
 *  \details     -
 *  \param[out]  versionInfo     Pointer to the location where the version information shall be stored.
 *  \pre         The VersionInfo parameter must not be NULL.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-110304
 *  \config      This function is only available if J1939RM_VERSION_INFO_API == STD_ON.
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, J1939RM_APPL_DATA) versionInfo);
# endif

/**********************************************************************************************************************
 *  J1939Rm_SendRequest
 *********************************************************************************************************************/
/*! \brief       Requests transmission of a Request PG.
 *  \details     Function only performs the parameter check and calls the internal send Request function.
 *  \param[in]   userId          Identification of the calling module.
 *  \param[in]   channel         Channel on which the request shall be sent.
 *  \param[in]   requestedPgn    PGN of the requested PG.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   destAddress     Address of the destination node or 0xFF for broadcast.
 *  \param[in]   priority        Priority of the Request PG.
 *  \param[in]   checkTimeout    TRUE: Timeout supervision will be performed,
 *                               FALSE: No timeout supervision will be started
 *  \return      E_OK:           Transmission request is accepted,
 *  \return      E_NOT_OK:       Transmission request is not accepted.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SendRequest(uint8 userId, NetworkHandleType channel, uint32 requestedPgn,
                                                       P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939RM_APPL_DATA)
                                                       extIdInfo, uint8 destAddress, uint8 priority,
                                                       boolean checkTimeout);

/**********************************************************************************************************************
 *  J1939Rm_CancelRequestTimeout
 *********************************************************************************************************************/
/*! \brief       Cancels timeout monitoring of a Request. If the request is not active, or timeout monitoring
 *               was not requested, this call has no effect.
 *  \details     -
 *  \param[in]   userId          Identification of the calling module.
 *  \param[in]   channel         Channel on which the request shall be sent.
 *  \param[in]   requestedPgn    PGN of the requested PG.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   destAddress     Address of the destination node or 0xFF for broadcast.
 *  \return      E_OK:           Cancellation of request timeout was successful.
 *  \return      E_NOT_OK:       Cancellation of request timeout was not successful
 *  \pre         The request identified by requested PGN and DA has been sent less than 1.25s in advance.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_CancelRequestTimeout(uint8 userId, NetworkHandleType channel,
                                                                uint32 requestedPgn,
                                                                P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC,
                                                                        J1939RM_APPL_DATA) extIdInfo,
                                                                uint8 destAddress);

/**********************************************************************************************************************
 *  J1939Rm_SendAck
 *********************************************************************************************************************/
/*! \brief       Requests transmission of an Acknowledgement PG.
 *  \details     Function only performs the parameter check and calls the internal send Acknowledgment function.
 *  \param[in]   userId          Identification of the calling module.
 *  \param[in]   channel         Channel on which the acknowledgement shall be sent.
 *  \param[in]   ackPgn          Acknowledged PGN.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   ackCode         Type of acknowledgement, see definition of J1939Rm_AckCode for available codes.
 *  \param[in]   ackAddress      Address of the node that sent the request.
 *  \param[in]   priority        Priority of the transmitted Acknowledgement PG.
 *  \param[in]   broadcast       Indicates whether the Acknowledgement PG is a response to a broadcast request.
 *  \return      E_OK:           Transmission request is accepted.
 *  \return      E_NOT_OK:       Transmission request is not accepted.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SendAck(uint8 userId, NetworkHandleType channel, uint32 ackPgn,
                                                   P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939RM_APPL_DATA)
                                                   extIdInfo, J1939Rm_AckCode ackCode, uint8 ackAddress,
                                                   uint8 priority, boolean broadcast);

/**********************************************************************************************************************
 *  J1939Rm_SetState
 *********************************************************************************************************************/
/*! \brief       Changes the communication state of J1939Rm to offline (only Request for AC supported) or online.
 *  \details     Function only performs the parameter check and calls the internal set State function.
 *  \param[in]   channel         Channel for which the state shall be changed.
 *  \param[in]   node            Node for which the state shall be changed.
 *  \param[in]   newState        New state the J1939Rm shall enter, see definition of J1939Rm_StateType
 *                               for available states.
 *  \return      E_OK:           New communication state was set.
 *  \return      E_NOT_OK:       Communication state was not changed due to wrong value in newState, channel or node.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SetState(NetworkHandleType channel, uint8 node, J1939Rm_StateType newState);

# define J1939RM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939RM_STOP_SEC_CODE ============================================*/
/*!
 * \exclusivearea  J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK
 *                 Ensures the consistency of the Acknowledgment queue.
 * \protects       J1939Rm_AckQueue
 * \usedin         J1939Rm_InternalProcessAckQueue, J1939Rm_InternalTransmitAck
 * \exclude        -
 * \length         SHORT Only a few flags are checked. No calls to functions.
 * \endexclusivearea
 */
/*!
 * \exclusivearea  J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK
 *                 Ensures the consistency of the Request2 queue.
 * \protects       J1939Rm_Req2Queue
 * \usedin         J1939Rm_InternalProcessRqst2Queue, J1939Rm_InternalTransmitRequest2
 * \exclude        -
 * \length         SHORT Only a few flags are checked. No calls to functions.
 * \endexclusivearea
 */
/*!
 * \exclusivearea  J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK
 *                 Ensures the consistency of the Request queue.
 * \protects       J1939Rm_ReqQueue
 * \usedin         J1939Rm_InternalProcessRqstQueue, J1939Rm_InternalTransmitRequest
 * \exclude        -
 * \length         SHORT Only a few flags are checked. No calls to functions.
 * \endexclusivearea
 */
/*!
 * \exclusivearea  J1939RM_EXCLUSIVE_AREA_TXPDULOCK
 *                 Ensures the consistency of the Tx PDU state.
 * \protects       J1939Rm_TxPduState
 * \usedin         J1939Rm_InternalMainFunction, J1939Rm_InternalTransmitAck, J1939Rm_InternalTransmitRequest,
 *                 J1939Rm_InternalTransmitRequest2
 * \exclude        -
 * \length         SHORT Only a few flags are checked. No calls to functions.
 * \endexclusivearea
 */
#endif /* J1939RM_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Rm.h
 *********************************************************************************************************************/
