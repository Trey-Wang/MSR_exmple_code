/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CddDrm.h
 *        \brief  CddDrm header file
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Mattias Koepke                viskem        Vector Informatik GmbH
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-01-25  viskem  ESCAN00089380 FEAT-1864: Release DRM (Diagnostic Request Manager)
 *                        visade  ESCAN00089380 FEAT-1864: Release DRM (Diagnostic Request Manager)
 *********************************************************************************************************************/

#if !defined (CDDDRM_H)
# define CDDDRM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "CddDrm_Cfg.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CDDDRM_VENDOR_ID                           (30U)
# define CDDDRM_MODULE_ID                           (0xffU)

/* AUTOSAR Software specification version information */
# define CDDDRM_AR_RELEASE_MAJOR_VERSION            (4U)
# define CDDDRM_AR_RELEASE_MINOR_VERSION            (0U)
# define CDDDRM_AR_RELEASE_REVISION_VERSION         (0U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CDDDRM_SW_MAJOR_VERSION                    (1U)
# define CDDDRM_SW_MINOR_VERSION                    (0U)
# define CDDDRM_SW_PATCH_VERSION                    (0U)

# define CDDDRM_INSTANCE_ID_DET                     (0x00U)

/* ----- API service IDs ----- */
# define CDDDRM_SID_INIT                            (0x00U) /*!< Service ID: CddDrm_Init()                     */
# define CDDDRM_SID_GET_VERSION_INFO                (0x02U) /*!< Service ID: CddDrm_GetVersionInfo()           */
# define CDDDRM_SID_MEMORY_INIT                     (0x03U) /*!< Service ID: CddDrm_InitMemory()               */
# define CDDDRM_SID_MAINFUNCTION                    (0x04U) /*!< Service ID: CddDrm_MainFunction()             */
/* ----- General API services ----- */
# define CDDDRM_SID_TRANSMIT                        (0x10U) /*!< Service ID: CddDrm_Transmit()                 */
# define CDDDRM_SID_CANCEL_REQUEST                  (0x11U) /*!< Service ID: CddDrm_CancelRequest()            */
# define CDDDRM_SID_EXTERNAL_TESTER_CONNECTED       (0x12U) /*!< Service ID: CddDrm_ExternalTesterConnected()  */
# define CDDDRM_SID_ECU_DETECTION_START             (0x13U) /*!< Service ID: CddDrm_StartEcuDetection()        */
# define CDDDRM_SID_ECU_DETECTION_STOP              (0x14U) /*!< Service ID: CddDrm_StopEcuDetection()         */
# define CDDDRM_SID_ECU_DETECTION_RESULT            (0x15U) /*!< Service ID: CddDrm_GetEcuDetectionResult()    */
# define CDDDRM_SID_INIT_ECU_DETECTION_DATA         (0x16U) /*!< Service ID: CddDrm_NvM_InitEcuDetectionData() */
/* ----- Service request APIs ----- */
# define CDDDRM_SID_SVC_SEND                        (0x20U) /*!< Service ID: CddDrm_SvcSend()                  */
# define CDDDRM_SID_SVC_SEND_10                     (0x21U) /*!< Service ID: CddDrm_SvcSend_10()               */
# define CDDDRM_SID_SVC_SEND_11                     (0x22U) /*!< Service ID: CddDrm_SvcSend_11()               */
# define CDDDRM_SID_SVC_SEND_1902                   (0x23U) /*!< Service ID: CddDrm_SvcSend_1902()             */
# define CDDDRM_SID_SVC_SEND_1904                   (0x24U) /*!< Service ID: CddDrm_SvcSend_1904()             */
# define CDDDRM_SID_SVC_SEND_22                     (0x25U) /*!< Service ID: CddDrm_SvcSend_22()               */
# define CDDDRM_SID_SVC_SEND_27                     (0x26U) /*!< Service ID: CddDrm_SvcSend_27()               */
# define CDDDRM_SID_SVC_SEND_28                     (0x27U) /*!< Service ID: CddDrm_SvcSend_28()               */
# define CDDDRM_SID_SVC_SEND_31                     (0x28U) /*!< Service ID: CddDrm_SvcSend_31()               */
# define CDDDRM_SID_SVC_SEND_34                     (0x29U) /*!< Service ID: CddDrm_SvcSend_34()               */
# define CDDDRM_SID_SVC_SEND_36                     (0x2aU) /*!< Service ID: CddDrm_SvcSend_36()               */
# define CDDDRM_SID_SVC_SEND_37                     (0x2bU) /*!< Service ID: CddDrm_SvcSend_37()               */
# define CDDDRM_SID_SVC_SEND_3E                     (0x2cU) /*!< Service ID: CddDrm_SvcSend_3E()               */
# define CDDDRM_SID_SVC_SEND_85                     (0x2dU) /*!< Service ID: CddDrm_SvcSend_85()               */
/* ----- API services for PduR ----- */
# define CDDDRM_SID_CANCELTRANSMIT                  (0x40U) /*!< Service ID: CddDrm_CancelTransmit()           */
# define CDDDRM_SID_CHANGEPARAMETER                 (0x41U) /*!< Service ID: CddDrm_ChangeParameter()          */
# define CDDDRM_SID_CANCELRECEIVE                   (0x42U) /*!< Service ID: CddDrm_CancelReceive()            */

/* ----- Error codes ----------------------------------------------------------------------------------------------- */
# define CDDDRM_E_NO_ERROR                          (0x00U) /*!< used to check if no error occurred - use a value unequal to any error code */
# define CDDDRM_E_PARAM_POINTER                     (0x03U) /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define CDDDRM_E_PARAM_VALUE                       (0x04U) /*!< Error code: API service used with invalid parameter value */
# define CDDDRM_E_UNINIT                            (0x05U) /*!< Error code: API service used without module initialization */
# define CDDDRM_E_ALREADY_INITIALIZED               (0x06U) /*!< Error code: The service CddDrm_Init() is called while the module is already initialized  */
# define CDDDRM_E_INVALID_CONNECTION                (0x07U) /*!< Error code: API service used with an invalid connection id */
# define CDDDRM_E_INVALID_BUFFER_LENGTH             (0x0aU) /*!< Error code: API service used with invalid buffer length parameter */
# define CDDDRM_E_PDU_ID_TX_OUT_OF_RANGE            (0x0cU) /*!< Error code: API service used with Tx-PDU-Id that is not configured */
# define CDDDRM_E_API_ERROR                         (0x0dU) /*!< Error code: API service called, but is not expected */

/* ----- Modes ----------------------------------------------------------------------------------------------------- */
# define CDDDRM_UNINIT                              (0x00U)
# define CDDDRM_INIT                                (0x01U)


#ifndef CDDDRM_CHANNEL_PDUR_RETRY_VALUE
# define CDDDRM_CHANNEL_PDUR_RETRY_VALUE                               (15U)     /*<! Tries before abort transmission between PduR and CDDDRM after BUFREQ_E_BUSY returned */
#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* ----- Interrupt control ----------------------------------------------------------------------------------------- */
# define CddDrm_EnterCritical_ChannelManager()      SchM_Enter_CddDrm_CDDDRM_EXCLUSIVE_AREA_0() /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define CddDrm_LeaveCritical_ChannelManager()      SchM_Exit_CddDrm_CDDDRM_EXCLUSIVE_AREA_0()  /* PRQA S 3453 */ /* MD_MSR_19.7 */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CDDDRM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CddDrm_InitMemory()
 *********************************************************************************************************************/
/*! \brief         Function for *_INIT_*-variable initialization
 *  \details       Service to initialize module global variables at power up. This function initializes the
 *                 variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre           Module is uninitialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_InitMemory(void);

/**********************************************************************************************************************
 *  CddDrm_Init()
 *********************************************************************************************************************/
/*! \brief         Initialization function
 *  \details       This function initializes the module DRM. It initializes all variables and sets the module state to
 *                 initialized.
 *  \param[in]     ConfigPtr               Configuration structure for initializing the module
 *  \pre           Interrupts are disabled.
 *  \pre           Module is uninitialized.
 *  \pre           CddDrm_InitMemory has been called unless CddDrm_ModuleInitialized is initialized by start-up code.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_Init(P2CONST(CddDrm_ConfigPtrType, AUTOMATIC, CDDDRM_PBCFG) ConfigPtr);

/**********************************************************************************************************************
 *  CddDrm_MainFunction()
 *********************************************************************************************************************/
/*! \brief         Handles all internal used timers and state machines
 *  \details       The main function provides a time base to the DRM module. Additionally all tasks which have been
 *                 delayed/shifted from DRM external context, e.g. send a diagnostic request or process ECU detection, 
 *                 to DRM context will be processed in context of the main function.
 *
 *                 If the DRM is not initialized, calling this function has no effect.
 *  \pre           Module is initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_MainFunction(void);

/**********************************************************************************************************************
 CddDrm_Transmit
 **********************************************************************************************************************/
/*! \brief         Initiate a request for transmission of a TX I-PDU
 *  \details       With this function, the transmission (from PduR point of view) of a message is requested. From the 
 *                 Drm/Application point of view this function is called when a diagnostic response is received.
 *  \param[in]     TxPduId      id of the TP CddPduRLowerLayerTxPdu.
 *  \param[in]     PduInfoPtr   a PduInfoType pointing to the transmit buffer.
 *  \return        E_OK:        the transmission request has been accepted.
 *  \return        E_NOT_OK:    the transmission request has NOT been accepted.
 *  \pre           The Drm is initialized and channel is active.
 *  \context       TASK|ISR
 *  \note          The function is called by the PduR.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_Transmit(PduIdType TxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA)PduInfoPtr);

#if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
/**********************************************************************************************************************
 CddDrm_CancelTransmit
 **********************************************************************************************************************/
/*! \brief         Cancel a transmission of a TX I-PDU
 *  \details       This function can be used to cancel an active Tx Connection (that is: the reception of a response)
 *  \param[in]     TxPduId   id of the TP CddPduRLowerLayerTxPdu.
 *  \return        E_OK:        the transmission has successfull been cancelled.
 *  \return        E_NOT_OK:    the transmission has not successful been cancelled.
 *  \pre           -
 *  \context       TASK|ISR
 *  \note          The function is called by the PduR.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelTransmit(PduIdType TxPduId);
#endif

#if (CDDDRM_CFG_SUPPORT_CHANGEPARAM == STD_ON)
/**********************************************************************************************************************
 CddDrm_ChangeParameter
 **********************************************************************************************************************/
/*! \brief         Only dummy function for PduR
 *  \details       The API is only available to satisfy the linker. A call to this API will always return E_NOT_OK.
 *  \param[in]     id         not used
 *  \param[in]     parameter  not used
 *  \param[in]     value      not used
 *  \return        E_NOT_OK:    Always
 *  \pre           -
 *  \context       TASK|ISR
 *  \note          The function is called by the PduR.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
/**********************************************************************************************************************
 CddDrm_CancelReceive
 **********************************************************************************************************************/
/*! \brief         Cancel request transmission to PduR.
 *  \details       With this function an active Rx connection (from DRM point of view the transmission of a diagnostic 
 *                 message) can be canceled.
 *  \param[in]     CddDrmRxSduId id of the TP CddPduRLowerLayerRxPdu.
 *  \return        E_OK:         cancellation sucessful.
 *  \return        E_NOT_OK:     cancellation failed.
 *  \pre           -
 *  \context       TASK|ISR
 *  \note          The function is called by the PduR.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelReceive(PduIdType CddDrmRxSduId);
#endif

/**********************************************************************************************************************
 *  CddDrm_SvcSend()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests a message that is build by application.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SPRMIB                         Idicates to DRM if the SPRMIB was set from application and thus a 
 *                                                positive response is expected or not.
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \param[in]     RequestLength                  Request data length
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend(CddDrm_ConnectionIdType ConnectionId,
                                                           boolean SPRMIB,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo,
                                                           CddDrm_LengthType RequestLength);

#if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_10()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending a Diagnostic Session Control request to configured connection.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SessionType                    Diagnostic Session Type
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_10(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SessionType,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_11()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending a ECU Reset.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     ResetType                      ECU Reset Type
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_11(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 ResetType,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_1902()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests DTCs by status mask
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     DTCStatusMask                  DTC Status Bit Mask
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1902(CddDrm_ConnectionIdType ConnectionId,
                                                                uint8 DTCStatusMask,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_1904()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests DTC Snapshot data.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     DTC                            DTC
 *  \param[in]     RecordNumber                   DTC Record number
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1904(CddDrm_ConnectionIdType ConnectionId,
                                                                uint32 DTC,
                                                                uint8 RecordNumber,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_22()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests data by identifier.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     DID                            the data identifier that is requested
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_22(CddDrm_ConnectionIdType ConnectionId,
                                                              uint16 DID,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_27()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests security access service to request seed or send key.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SubFunction                    select request seed / send key
 *  \param[in]     DataLength                     seed/key length
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           External Tester is not present.
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_27(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              CddDrm_LengthType DataLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_28()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests communication control service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SubFunction                    Control type
 *  \param[in]     CommunicationType              is bit coded to control multiple communication types
 *  \param[in]     NodeIdNumber                   node identification number (only required if sub-function 0x04 or 0x05)
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_28(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              uint8 CommunicationType,
                                                              uint16 NodeIdNumber,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_31()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending routine control service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SubFunction                    routine control sub-function
 *  \param[in]     RoutineId                      routine identifier
 *  \param[in]     RoutineOptionLength            length of routine options
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_31(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              uint16 RoutineId,
                                                              CddDrm_LengthType RoutineOptionLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_34()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending request download service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     DataFormatId                   Data format identifier
 *  \param[in]     AddressAndLength               Memory address and size bytes
 *  \param[in]     DataLength                     Request Data length of memory address and size bytes
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_34(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 DataFormatId,
                                                              uint8 AddressAndLength,
                                                              CddDrm_LengthType DataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_36()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending transfer data service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     BlockSeqCounter                Block sequence
 *  \param[in]     TransferDataLength             Length of the data that will be transfered
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_36(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 BlockSeqCounter,
                                                              CddDrm_LengthType TransferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_37()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending request transfer exit service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     TransferDataLength             Length of the data that will be transfered
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_37(CddDrm_ConnectionIdType ConnectionId,
                                                              CddDrm_LengthType TransferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_3E()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests sending tester present service.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[out]    BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_3E(CddDrm_ConnectionIdType ConnectionId,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_85()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests UDS service: Control DTC settings.
 *  \param[in]     ConnectionId                   connection related to the ECU to which the request shall be send
 *  \param[in]     SubFunction                    Control DTC setting sub-function
 *  \param[in]     RecordLength                   Length of optional record data
 *  \param[in]     SPRMIB                         suppresses positive response
 *  \param[in,out] BufferInfo                     contains request and response buffer, response buffer size
 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_85(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              CddDrm_LengthType RecordLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo);
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_CancelRequest()
 *********************************************************************************************************************/
/*! \brief         Cancels request on a connection.
 *  \details       This function cancel requests that will be sent through the connection.
 *  \param[in]     ConnectionId       Connection that want to be closed.
 *  \return        E_OK:              Cancel request accepted
 *  \return        E_NOT_OK:          Cancel request not accepted
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           Module is initialized.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelRequest(CddDrm_ConnectionIdType ConnectionId);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetEcuDetectionResult()
 *********************************************************************************************************************/
/*! \brief         Returns the result of ECU Detection.
 *  \details       This API provides the application the result of the ECU detection.
 *  \param[in]     ConnectionId       that is related to a ECU, which state will be returned
 *  \return        CDDDRM_ECUD_CONNECTION_AVAILABLE:      ECU is available
 *  \return        CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE:  ECU is not available
 *  \return        CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED: ECU Detection is not performed to given ConnectionId yet
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(CddDrm_EcudStateType, CDDDRM_CODE) CddDrm_GetEcuDetectionResult(CddDrm_ConnectionIdType ConnectionId);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_StartEcuDetection()
 *********************************************************************************************************************/
/*! \brief         Starts the background ECU Detection.
 *  \details       Triggers the ECU detection which is processed in context of the mainfunction.
 *  \return        E_OK:      ECU Detection has been started
 *  \return        E_NOT_OK:  ECU Detection already running or external tester active
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StartEcuDetection(void);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_StopEcuDetection()
 *********************************************************************************************************************/
/*! \brief         Stops the background ECU Detection.
 *  \details       Forces the end of the ECU detection. All connections which are not processed yet will be in
 *                 state not discovered.
 *  \return        E_OK:      always returned
 *  \return        E_NOT_OK:  never returned
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StopEcuDetection(void);
#endif

# if (CDDDRM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief         Returns the version information
 *  \details       CddDrm_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]    VersionInfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDRM_APPL_DATA) VersionInfo);
# endif

# define CDDDRM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* CDDDRM_H */

/**********************************************************************************************************************
 *  END OF FILE: CddDrm.h
 *********************************************************************************************************************/