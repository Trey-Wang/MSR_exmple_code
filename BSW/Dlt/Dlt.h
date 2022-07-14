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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dlt.h
 *      Project:  Diagnostic Log and Trace
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implements partly the functionality of the AUTOSAR 4.x module DLT based on XCP.
 *
 *
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Patrick Markl                  visml        Vector Informatik GmbH
 *  Alexander Zeeb                 viazb        Vector Informatik GmbH
 *  Oliver Reineke                 visore       Vector Informatik GmbH
 *  David Zentner                  viszda       Vector Informatik GmbH
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2011-05-12  visml   ESCAN00050129   Creation
 *  01.02.00  2013-03-26  visml   ESCAN00065961   Added DLT_APPL_VAR and DLT_PBCFG     
 *                        visml   ESCAN00065958   Added DLT_XCP_DET_EVENT to the template
 *                        visml   ESCAN00066198   Added default XCP event to A2L file
 *  01.02.01  2013-05-21  visazb  ESCAN00067468   Corrected vesion mismatch
 *  01.03.00  2013-07-02  visore  ESCAN00068516   Compiler warning: parameter 'ConfigPtr' unused in function 'Dlt_Init'
 *                        visore  ESCAN00068517   The DET service and error IDs are not compliant to the AUTOSAR4 specification
 *                        visore  ESCAN00068518   The MISRA justifications are not implemented according WI_MISRAC2004_PES.pdf
 *                        visore  ESCAN00068274   AR4-292: Reporting of DET and DEM errors via DLT
 *                        visore  ESCAN00068519   Signature of Dlt_DetForwardErrorTrace() is not compliant to AUTOSAR4
 *                        visore  ESCAN00070450   No changes here
 *                        visore  ESCAN00070451   No changes here
 *  01.03.01  2013-11-20  visore  ESCAN00071890   No changes here
 *  01.03.02  2014-01-20  visore  ESCAN00072716   Improve DLT include structure
 *  01.03.03  2014-05-21  visore  ESCAN00074824   No changes here
 *  01.05.00  2014-02-25  viszda  ESCAN00081343   FEAT-1311: DLT with AUTOSAR functionality and DLT Transport layer
 *            2015-03-17  viszda  ESCAN00081868   No changes here
 *  01.06.00  2015-04-22  viszda  ESCAN00082261   FEAT-1312: Continue DLT development with AUTOSAR functionality and DLT Transport layer (based on FEAT-1311)
 *                        viszda  ESCAN00082468   Compiler error: error C2065: 'xyz' : undeclared identifier
 *                        viszda  ESCAN00082469   Compiler warning: warning C4244, warning C4101, warning C4189, warning C4100
 *                        viszda  ESCAN00082472   Missing filters, compiler abstractions and exclusive areas
 *                        viszda  ESCAN00082473   Reduce code redundancies and code complexity due to FEAT-1311
 *                        viszda  ESCAN00082474   Change erroneous compiler abstractions and use the correct initialization for global variables
 *                        viszda  ESCAN00083946   No changes here
 *  01.06.01  2015-12-17  viszda  ESCAN00087203   No changes here
 *                        viszda  ESCAN00087212   Wrong DLT module ID
 *  01.07.00  2015-02-01  viszda  ESCAN00087978   Correction of version number in DltCom messages
 *            2016-02-26  viszda  ESCAN00088121   Product release of ASR DLT
 *  01.07.01  2016-06-21  viszda  ESCAN00090043   Get the socket connection state via API
 *            2016-07-25  viszda  ESCAN00091129   Valid DEM Event forwarded to DLT triggers DET Error
 *  01.07.02  2016-11-18  viszda  ESCAN00092787   Control messages from Dlt client are rejected and no response is send.
 *                        viszda  ESCAN00081639   Correction of formal issues
 *                        viszda  ESCAN00092560   Compiler error: The include file 'Det.h' cannot be opened
 *            2016-11-22  viszda  ESCAN00092936   Reduce Stack consumption of Dlt
 *********************************************************************************************************************/

#if !defined(DLT_H)
# define DLT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Dlt_Types.h"
# include "SchM_Dlt.h"

# include "Dem.h" /* needed for Dem_EventIdType */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# include "Dlt_Cbk.h"
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* vendor and module identification */
# define DLT_VENDOR_ID                       (30u)
# define DLT_MODULE_ID                       (55u) /* ESCAN00087212 */

/* AUTOSAR Software specification version information */
# define DLT_AR_RELEASE_MAJOR_VERSION        (4u)
# define DLT_AR_RELEASE_MINOR_VERSION        (0u)
# define DLT_AR_RELEASE_REVISION_VERSION     (3u)

/* Component version information (decimal version of ALM implementation package) */
#define DLT_SW_MAJOR_VERSION                 (1u)
#define DLT_SW_MINOR_VERSION                 (7u)
#define DLT_SW_PATCH_VERSION                 (2u)

# define DLT_INSTANCE_ID_DET                 (0x00u)

/* API service IDs */
# define DLT_API_ID_INIT                     ((uint8)0x01u) /*!< ASR4.0 Dlt_Init() service ID [Dlt239]. */
# define DLT_API_ID_GETVERSIONINFO           ((uint8)0x02u) /*!< ASR4.0 Dlt_GetVersionInfo() service ID [Dlt271]. */
# define DLT_API_ID_SENDLOGMESSAGE           ((uint8)0x03u) /*!< ASR4.0 Dlt_SendLogMessage() service ID [Dlt241]. */
# define DLT_API_ID_SENDTRACEMESSAGE         ((uint8)0x04u) /*!< ASR4.0 Dlt_SendTraceMessage() service ID [Dlt243]. */
# define DLT_API_ID_REGISTERCONTEXT          ((uint8)0x05u) /*!< ASR4.0 Dlt_RegisterContext() service ID [Dlt245]. */
# define DLT_API_ID_DETFORWARDERRORTRACE     ((uint8)0x07u) /*!< ASR4.0 Dlt_DetForwardErrorTrace() service ID [Dlt432]. */
# define DLT_API_ID_COMRXINDICATION          ((uint8)0x0Fu) /*!< ASR4.0 Dlt_ComRxIndication() service ID [Dlt272]. */
# define DLT_API_ID_COMTXCONFIRMATION        ((uint8)0x10u) /*!< ASR4.0 Dlt_ComTxConfirmation() service ID [Dlt273]. */
# define DLT_API_ID_SETLOGLEVEL              ((uint8)0x11u) /*!< ASR4.0 Dlt_SetLogLevel() service ID [Dlt252]. */
# define DLT_API_ID_SETTRACESTATUS           ((uint8)0x12u) /*!< ASR4.0 Dlt_SetTraceStatus() service ID [Dlt254]. */
# define DLT_API_ID_SETVERBOSEMODE           ((uint8)0x13u) /*!< ASR4.0 Dlt_SetverboseMode() service ID [Dlt256]. */
# define DLT_API_ID_DEMTRIGGERONEVENTSTATUS  ((uint8)0x15u) /*!< ASR4.0 Dlt_DemTriggerOnEventStatus() service ID [Dlt470]. */
# define DLT_API_ID_MAINFUNCTION             ((uint8)0x50u) /*!< MSR4.0 Dlt_MainFunction() service ID. */
# define DLT_API_ID_SETSTATE                 ((uint8)0x51u) /*!< MSR4.0 Dlt_SetState() service ID. */
# define DLT_API_ID_GETSTATE                 ((uint8)0x52u) /*!< MSR4.0 Dlt_GetState() service ID. */

/* Error codes */
# define DLT_E_NO_ERROR                      ((uint8)0x00u) /*!< Error code: No error. */
# define DLT_E_WRONG_PARAMETERS              ((uint8)0x01u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_ERROR_IN_PROV_SERVICE         ((uint8)0x02u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_COM_FAILURE                   ((uint8)0x03u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_ERROR_TO_MANY_CONTEXT         ((uint8)0x04u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_MSG_LOOSE                     ((uint8)0x05u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_PARAM_POINTER                 ((uint8)0x06u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_INIT_FAILED                   ((uint8)0x07u) /*!< Error code: [SWS_Dlt_00447]. */
# define DLT_E_UNINITIALIZED                 ((uint8)0x08u) /*!< Error code: DLT is not initialized. */
# define DLT_E_INVALID_STATE                 ((uint8)0x09u) /*!< Error code: DLT is in an invalid state. */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of Dlt_Types.h */
# if (  (DLT_TYPES_MAJOR_VERSION != (1u)) \
     || (DLT_TYPES_MINOR_VERSION != (7u)) \
     || (DLT_TYPES_PATCH_VERSION != (2u)) )
#  error "Version numbers of Dlt_Types.h are inconsistent"
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */

# if !defined(DLT_DUMMY_STATEMENT)     /* COV_DLT_MSR_COMPATIBILITY */
#  if defined (DLT_USE_DUMMY_STATEMENT)  
# define DLT_DUMMY_STATEMENT(statement) ((void)(statement))
#  else
# define DLT_DUMMY_STATEMENT(statement)
#  endif
# endif

# if !defined(DLT_DUMMY_STATEMENT_CONST)     /* COV_DLT_MSR_COMPATIBILITY */
#  if defined (DLT_USE_DUMMY_STATEMENT_CONST)  
# define DLT_DUMMY_STATEMENT_CONST(statement) ((void)(statement))
#  else
# define DLT_DUMMY_STATEMENT_CONST(statement)
#  endif
# endif

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#define DLT_IS_CONNECTION_ONLINE() ((boolean)(Dlt_SoConMode == DLT_SOCON_ONLINE))
# else
#define DLT_IS_CONNECTION_ONLINE() ((boolean)FALSE)
# endif

/* PRQA L:FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/* Context Types */
typedef struct
{
  Dlt_SessionIDType            session_id;                  /*!< the ECU wide unique Session ID of this context. */
  uint8                        verbose_mode;                /*!< the verbose mode active for this context. */
  Dlt_SetVerboseModeOpFuncType Dlt_SetVerboseMode_;         /*!< the callback function to notify the Dlt User about change of verbose mode. */
  Dlt_SetLogLevelOpFuncType    Dlt_SetLogLevel_;            /*!< the callback function to notify the Dlt User about change of log level. */
  Dlt_SetTraceStatusOpFuncType Dlt_SetTraceStatus_;         /*!< the callback function to notify the Dlt User about change of trace status. */
} Dlt_CbkInfoType;                                          /*!< data type: struct containing callback and other details of Dlt context.  */

typedef struct
{
  Dlt_ContextIDType context_id;                             /*!< the context ID of the this context. */
  sint8             log_level;                              /*!< the importance of this context's log messages. */
  sint8             trace_status;                           /*!< the status of this context's trace messages. */
  uint8             len_context_description;                /*!< length of the context ID description.  */ /* Optional, if option == 7 */
  P2CONST(uint8, AUTOMATIC, DLT_CONST) context_description; /*!< the context ID description. */ /* Optional, if option == 7 */
  Dlt_CbkInfoType   cbk_info;                               /*!< additional information about this context. */
} Dlt_ContextIDsInfoType;                                   /*!< data type: struct containing information about a specific Context ID. */

typedef struct
{
  Dlt_ApplicationIDType  app_id;                            /*!< the application ID */
  uint16                 count_context_ids;                 /*!< number of registered contexts under this application ID. */
  Dlt_ContextIDsInfoType context_id_info[DLT_MAX_COUNT_CONTEXT_IDS_PER_APP_ID]; /*!< list of all registered contexts under this application ID. */
  uint8                  len_app_description;               /*!< length of the application ID description. */ /* Optional, if option == 7 */
  P2CONST(uint8, AUTOMATIC, DLT_CONST) app_description;     /*!< application ID description. */ /* Optional, if option == 7 */
} Dlt_AppIDsType;                                           /*!< data type: struct containing information about a specific Application ID. */

typedef struct
{
  uint16         count_app_ids;                             /*!< the number of all registered application IDs. */
  Dlt_AppIDsType app_ids[DLT_MAX_COUNT_APP_IDS];            /*!< list of all registered application in this ECU. */
} Dlt_LogInfoType;                                          /*!< data type: struct containing information about the Dlt context table. */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define DLT_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
extern CONST(uint8, DLT_CONST)                     Dlt_StandardHeaderType_SoftwareVersion;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

extern CONST(uint8, DLT_CONST)                     Dlt_ModuleTable[256];

#define DLT_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# define DLT_START_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Variables for Dlt runtime configuration */
#  if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_FilterMessages;
#  endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
#  if defined (DLT_IMPLEMENT_TIMESTAMP)
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_HeaderUseTimestamp;
#  endif /* (DLT_IMPLEMENT_TIMESTAMP) */
#  if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_HeaderUseExtendedHeader;
#  endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
#  if defined (DLT_IMPLEMENT_VERBOSE_MODE)
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_HeaderUseVerboseMode;
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */

extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_HeaderUseEcuId;
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_HeaderUseSessionId;
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_DefaultTraceStatus;
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_DefaultMaxLogLevel;
extern VAR(PduInfoType, DLT_VAR_NOINIT)            Dlt_SendPdu;

#define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#define DLT_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Variable is loaded with the last DET error code */
extern VAR(uint32, DLT_VAR_NOINIT)                 Dlt_DetErrorCode;

#define DLT_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#define DLT_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
extern VAR(PduInfoType, DLT_VAR_NOINIT)            Dlt_SendPdu;
extern VAR(Dlt_SoConModeType, DLT_VAR_NOINIT)      Dlt_SoConMode;
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

# if defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/* Variable is loaded with DEM event status code. */
extern VAR(Dlt_DemEventStatusType, DLT_VAR_NOINIT) Dlt_DemEventStatus;

/* Variable is loaded with message ID of non-verbose message . */
extern VAR(Dlt_MessageIDType, DLT_VAR_NOINIT)      Dlt_NonVerboseMessageId;

#  if (DLT_USE_VERBOSE_MODE == STD_ON)
/* Buffer is loaded with payload of verbose message. */
extern VAR(uint8, DLT_VAR_NOINIT)                  Dlt_VerboseMessageData[DLT_MAX_MESSAGE_LENGTH];
#  endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */
# endif /* defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) */

#define DLT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define DLT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Dlt_InitMemory
**********************************************************************************************************************/
/*! \brief       Power-up memory initialization
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized.
 *  \return      -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106553
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_InitMemory(void);

/**********************************************************************************************************************
 *  Dlt_Init
**********************************************************************************************************************/
/*! \brief       Initializes component
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr        Component configuration structure
 *  \return      -
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         Dlt_InitMemory has been called unless Dlt_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106553
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_Init(P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_PBCFG) ConfigPtr);

# if(DLT_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 *  Dlt_GetVersionInfo()
**********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo      Pointer to where to store the version information. Parameter must not be NULL.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-106554
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_VAR) VersionInfo);

# endif

/**********************************************************************************************************************
 *  Dlt_DetForwardErrorTrace()
**********************************************************************************************************************/
/*! \brief       Service to forward error reports from Det to Dlt.
 *  \details     Builds and sends a log message containing all passed data.
 *  \param[in]   ModuleId         Module ID of calling module.
 *  \param[in]   InstanceId       The identifier of the index based instance of a module.
 *  \param[in]   ApiId            ID of API service in which error is detected.
 *  \param[in]   ErrorId          ID of detected development error.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_DetForwardErrorTrace(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

/**********************************************************************************************************************
 *  Dlt_DemTriggerOnEventStatus()
**********************************************************************************************************************/
/*! \brief       This service is provided by the Dem in order to call Dlt upon status changes.
 *  \details     Builds and sends a log message containing the passed data.
 *  \param[in]   EventId          Module ID of calling module.
 *  \param[in]   EventStatusOld   The identifier of the index based instance of a module.
 *  \param[in]   EventStatusNew   ID of API service in which error is detected.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_DemTriggerOnEventStatus(Dem_EventIdType EventId,
                                                 Dem_EventStatusExtendedType EventStatusOld,
                                                 Dem_EventStatusExtendedType EventStatusNew);

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 *  Dlt_ComRxIndication()
**********************************************************************************************************************/
/*! \brief       Notifies DLT that complete control message has been received.
 *  \details     Sets flag Dlt_RxIndicationOccured if control message has been successfully received.
 *  \param[in]   DltRxPduId       ID of DLT I-PDU that has been received. Identifies the data that has been received.
 *  \param[in]   Result           Result of the N-PDU reception:
 *                                 - E_OK      the complete N-PDU has been received.
 *                                 - E_NOT_OK  an error occurred during reception, used to enable unlocking of the
 *                                             receive buffer.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_ComRxIndication(PduIdType DltRxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
 *  Dlt_ComTxConfirmation()
**********************************************************************************************************************/
/*! \brief       Notifies DLT that message has been successfully transmitted.
 *  \details     Sets flag Dlt_MessageTransmitted if message has been successfully transmitted.
 *  \param[in]   DltTxPduId       ID of Dlt I-PDU that has been transmitted.
 *  \param[in]   Result           Result of the N-PDU transmission:
 *                                 - E_OK      the complete N-PDU has been transmitted.
 *                                 - E_NOT_OK  an error occurred during transmission, used to enable unlocking of the
 *                                             transmit buffer.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_ComTxConfirmation(PduIdType DltTxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
 *  Dlt_ComStartOfReception()
**********************************************************************************************************************/
/*! \brief       Notifies DLT about incoming message.
 *  \details     Gets the length of received buffer and checks if message fits in receive buffer.
 *  \param[in]   info             Pointer to a PduInfoType structure containing the payload data (without protocol
 *                                information) and payload length of the first frame or single frame of a transport
 *                                protocol I-PDU reception.
 *  \param[in]   TpSduLength      Total length of the N-SDU to be received.
 *  \param[out]  bufferSizePtr    Available receive buffer in the receiving module. This parameter will be used to
 *                                compute the Block Size (BS) in the transport protocol module.
 *  \return      BUFREQ_OK        Connection has been accepted. bufferSizePtr indicates the available receive buffer.
 *  \return      BUFREQ_E_NOT_OK  Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 *  \return      BUFREQ_E_OVFL    No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 *                                remains unchanged.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception(P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info,
                                                          PduLengthType TpSduLength,
                                                          P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  Dlt_ComCopyRxData()
**********************************************************************************************************************/
/*! \brief       Provides the received data of an I-PDU segment (N-PDU) to the upper layer.
 *  \details     Copies received data to receive buffer and requests next bytes of message until complete message is
 *               received.
 *  \param[in]   info             Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength).
 *  \param[out]  bufferSizePtr    Available receive buffer after data has been copied.
 *  \return      BUFREQ_OK        Data copied successfully.
 *  \return      BUFREQ_E_NOT_OK  Data was not copied because an error occurred.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData(P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info,
                                                    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  Dlt_ComCopyTxData()
**********************************************************************************************************************/
/*! \brief       Acquires the transmit data of an I-PDU segment (N-PDU).
 *  \details     Copies as much Tx data to lower layer as requested. Fails if more data is requested then available.
 *               received.
 *  \param[in]   info             Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength).
 *  \param[in]   retry            This parameter is used to acknowledge transmitted data or to retransmit data after
 *                                transmission problems.
 *  \param[out]  availableDataPtr Indicates the remaining number of bytes that are available in the upper layer
 *                                module's Tx buffer.
 *  \return      BUFREQ_OK        Data has been copied to the transmit buffer completely as requested.
 *  \return      BUFREQ_E_BUSY    Request could not be fulfilled, because the required amount of Tx data is not
 *                                available.
 *  \return      BUFREQ_E_NOT_OK  Data has not been copied. Request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyTxData(P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info,
                                                    P2CONST(RetryInfoType, AUTOMATIC, DLT_CONST) retry,
                                                    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) availableDataPtr);

/**********************************************************************************************************************
 *  Dlt_ComSoConModeChg()
**********************************************************************************************************************/
/*! \brief       Notifies the DLT that the mode of the socket connection has changed.
 *  \details     Notifies the DLT that the mode of the socket connection has changed.
 *               received.
 *  \param[in]   Mode             The new mode of the socket connection (DLT_SOCON_ONLINE, DLT_SOCON_RECONNECT,
 *                                DLT_SOCON_OFFLINE.
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_ComSoConModeChg(Dlt_SoConModeType Mode);

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

# if (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  Appl_DltDemEventFilterCbk()
**********************************************************************************************************************/
/*! \brief       Checks if DEM event status has changed.
 *  \details     Must be implemented by user and should be used to compare old event status and new event status. The
 *               result should be returned.
 *  \param[in]   EventId          Identification of an Event by assigned event number. The Event number is configured
 *                                in the Dem:
*                                   Min.: 1 (0: Indication of no Event or Failure).
*                                   Max.: Result of configuration of Event Numbers in Dem (Max is either 255 or 65535).
 *  \param[in]   EventStatusOld   Extended event status before change.
 *  \param[in]   EventStatusNew   Detected / reported of event status.
 *  \return      TRUE             The DEM event has passed the filter (log message will be generated by the DLT).
 *  \return      FALSE            The DEM event has not passed the filter (no log message will be generated).
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* UREQ00041311 */
FUNC(boolean, DLT_CODE) Appl_DltDemEventFilterCbk(Dem_EventIdType EventId,
                                                  Dem_EventStatusExtendedType EventStatusOld,
                                                  Dem_EventStatusExtendedType EventStatusNew);

# endif /* (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  Dlt_MainFunction()
**********************************************************************************************************************/
/*! \fn          void Dlt_MainFunction(void)
 *  \brief       Checks periodically the Dlt status.
 *  \details     Handles cyclically the state machine(s).
 *  \param[in]   -
 *  \return      -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-106560
 *********************************************************************************************************************/
/* FUNC(void, DLT_CODE) Dlt_MainFunction(void);
*/

#define DLT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/**********************************************************************************************************************
 *  END OF FILE: Dlt.h
 *********************************************************************************************************************/
