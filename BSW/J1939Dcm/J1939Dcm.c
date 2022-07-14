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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Dcm.c
 *    Component:  MICROSAR Diagnostic Communication Manager for SAE J1939
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  implementation file for J1939 DCM
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials       Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Thomas Dedler                 Dth            Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-02-07  Dth     ESCAN00071378: AR4-465: Component Diag_Asr4J1939Dcm created
 *  01.00.01  2014-04-11  Dth     ESCAN00074274: Compiler error, J1939Dcm_GetNodeHdlByAddr is undefined
 *  02.00.00  2014-05-12  Dth     ESCAN00073996: Missing InitMemory API
 *                        Dth     ESCAN00071929: AR4-619: StartOfReception changed according to AR4.1.2
 *                        Dth     ESCAN00073477: TxConfirmation implementation incomplete
 *                        Dth     ESCAN00073465: Interrupt lock times reduced
 *                        Dth     ESCAN00073464: Use improved data types
 *                        Dth     ESCAN00076282: Transmission may be blocked after changing to offline mode
 *  02.01.00  2014-09-01  Dth     ESCAN00076487: Received J1939Tp message are discarded by J1939Dcm
 *                        Dth     ESCAN00077928: Repeated requests of DM5 will fail
 *                        Dth     ESCAN00076604: AR4-669: Support for J1939 Diagnostics (release status)
 *                        Dth     ESCAN00078264: DM31 deviation from SAE in case of no DTC to be reported
 *                        Dth     ESCAN00076611: AR4-741: Support multiple J1939 nodes in DEM and J1939DCM
 *  03.00.00  2015-07-01  Dth     ESCAN00080031: DM35 buffer overrun may occur
 *                        Dth     ESCAN00083142: FEAT-1452: Add broadcast parameter to J1939Rm_SendAck
 *                        Dth     ESCAN00083700: Compiler error: incompatible pointer type for J1939Dcm_CopyRxData
 *  04.00.00  2016-02-23  Aey     ESCAN00087869: FEAT-1605: Support DM5/27/53/54/55 in J1939Dcm
 *                        Aey     ESCAN00084694: FEAT-1644: Adapt to changes of J1939Dcm_RequestIndication and J1939Rm_SendAck
 *  04.01.00  2016-05-17  Aey     ESCAN00089033: FEAT-1236: PB-S and PB-L for J1939Dcm
 *                        Aey     ESCAN00089284: Compiler warning: uninitialized local variable 'lFilterHandler'
 *                        Aey     ESCAN00089006: Node goes offline only when all the referenced channels are offline
 *  04.02.00  2016-06-20  Aey     ESCAN00089269: FEAT-1327: ISOBUS support in J1939Dcm
 **********************************************************************************************************************/

#define J1939DCM_SOURCE

/* ********************************************************************************************************************
 *  GLOBAL MISRA VIOLATIONS
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0779 EOF */ /* MD_J1939Dcm_5.1 */ /* Identifier does not differ in 32 significant characters -- caused by ComStackLib algorithm for constructing guaranteed unique names. */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* module definitions */
#include "J1939Dcm.h"

/* used modules */
#include "Det.h"
#include "SchM_J1939Dcm.h"
#include "BswM_J1939Dcm.h"
#include "J1939Rm.h"
#include "J1939Nm.h"
#include "Dem_J1939Dcm.h"
#include "PduR_J1939Dcm.h"
#include "ComM.h"

#if (J1939DCM_CONFIGURATION_VARIANT == J1939DCM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
/* post-build error reporting */
# include "EcuM_Error.h"
#endif

/* internal definitions */
#include "J1939Dcm_Priv.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (  (J1939DCM_SW_MAJOR_VERSION != (0x04)) \
    || (J1939DCM_SW_MINOR_VERSION != (0x02)) \
    || (J1939DCM_SW_PATCH_VERSION != (0x00)) )
# error "Vendor specific version numbers of J1939Dcm.c and J1939Dcm.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if (  (J1939DCM_AR_MAJOR_VERSION != (0x04)) \
    || (J1939DCM_AR_MINOR_VERSION != (0x01)) \
    || (J1939DCM_AR_PATCH_VERSION != (0x02)) )
# error "AUTOSAR Specification Version numbers of J1939Dcm.c and J1939Dcm.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
#if !defined (J1939DCM_LOCAL)
# define J1939DCM_LOCAL                 static
#endif

#if !defined (J1939DCM_LOCAL_INLINE)
# define J1939DCM_LOCAL_INLINE          LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* --- Assertions ----------------------------------------------------------------------------------------------------*/
#if(J1939DCM_DEV_ERROR_DETECT == STD_ON)
# define J1939Dcm_DetAssertAlways(ApiId, ErrorId)                  (void)Det_ReportError( J1939DCM_MODULE_ID, J1939DCM_INSTANCE_ID, (ApiId), (ErrorId) );    /* PRQA S 3412 */ /* MD_MSR_19.4 */
# define J1939Dcm_DetAssert(Check, ApiId, ErrorId)                 { if(!(Check)) {J1939Dcm_DetAssertAlways( (ApiId), (ErrorId) ) }}                         /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */
#else
# define J1939Dcm_DetAssertAlways(ApiId, ErrorId)
# define J1939Dcm_DetAssert(Check, ApiId, ErrorId)
#endif

#define J1939Dcm_DetAssertReturn(Check, ApiId, ErrorId, RetCode)    { if(!(Check)) {J1939Dcm_DetAssertAlways( (ApiId), (ErrorId) ) return(RetCode); }}         /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */
#define J1939Dcm_DetAssertVReturn(Check, ApiId, ErrorId)            { if(!(Check)) {J1939Dcm_DetAssertAlways( (ApiId), (ErrorId) ) return; }}                  /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */
#define J1939Dcm_DetAssertNodeHdlVReturn(Node, ApiId)               J1939Dcm_DetAssertVReturn (((Node)    < J1939Dcm_Cfg_GetNumberOfNodes()),    (ApiId), J1939DCM_E_INVALID_NODE)                /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DetAssertNodeHdlReturn(Node, ApiId, RetCode)       J1939Dcm_DetAssertReturn  (((Node)    < J1939Dcm_Cfg_GetNumberOfNodes()),    (ApiId), J1939DCM_E_INVALID_NODE, (RetCode))     /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DetAssertChannelHdlVReturn(Channel, ApiId)         J1939Dcm_DetAssertVReturn (((Channel) < J1939Dcm_Cfg_GetNumberOfChannels()), (ApiId), J1939DCM_E_INVALID_CHANNEL)             /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DetAssertChannelHdlReturn(Channel, ApiId, RetCode) J1939Dcm_DetAssertReturn  (((Channel) < J1939Dcm_Cfg_GetNumberOfChannels()), (ApiId), J1939DCM_E_INVALID_CHANNEL, (RetCode))  /* PRQA S 3412,3458 */ /* MD_MSR_19.4 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* --- EcuM --- */
#if !defined (J1939DCM_EcuMReportBswError)
# if (J1939DCM_CONFIGURATION_VARIANT == J1939DCM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
#  define J1939DCM_EcuMReportBswError(EcuMErrorId)                      (EcuM_BswErrorHook((uint16) J1939DCM_MODULE_ID, (EcuMErrorId)));         /* PRQA S 3412 */ /* MD_MSR_19.7 */
# else
#  define J1939DCM_EcuMReportBswError(EcuMErrorId)
# endif
#endif

/* --- critical sections --------------------------------------------------------------------------------------------*/
#define J1939Dcm_EnterCritical()                          SchM_Enter_J1939Dcm_J1939DCM_EXCLUSIVE_AREA_0()   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_ExitCritical()                           SchM_Exit_J1939Dcm_J1939DCM_EXCLUSIVE_AREA_0()    /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef uint8 J1939Dcm_InitStateType;
#define J1939DCM_STATE_UNINIT     (J1939Dcm_InitStateType)(0x00u)
#define J1939DCM_STATE_INIT       (J1939Dcm_InitStateType)(0x01u)

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define J1939DCM_START_SEC_VAR_INIT_8BIT
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */

J1939DCM_LOCAL VAR(J1939Dcm_InitStateType, J1939DCM_VAR_INIT) J1939Dcm_InitState = J1939DCM_STATE_UNINIT;

#define J1939DCM_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define J1939DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
J1939DCM_LOCAL VAR(J1939Dcm_ChannelSupportType, J1939DCM_VAR_NOINIT) J1939Dcm_ComState[J1939DCM_MAX_NUM_NODES];

#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_DM1StateType,  J1939DCM_VAR_NOINIT) J1939Dcm_DM1State[J1939DCM_MAX_NUM_NODES];
J1939DCM_LOCAL volatile VAR(J1939Dcm_DM1QueueType, J1939DCM_VAR_NOINIT) J1939Dcm_DM1Queue[J1939DCM_MAX_NUM_NODES];
#endif

#if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_DM13StateType, J1939DCM_VAR_NOINIT) J1939Dcm_DM13State;
J1939DCM_LOCAL volatile VAR(J1939Dcm_DM13QueueType, J1939DCM_VAR_NOINIT) J1939Dcm_DM13Queue;
#endif

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_DM35StateType, J1939DCM_VAR_NOINIT) J1939Dcm_DM35State[J1939DCM_MAX_NUM_NODES];
J1939DCM_LOCAL volatile VAR(J1939Dcm_DM35QueueType, J1939DCM_VAR_NOINIT) J1939Dcm_DM35Queue[J1939DCM_MAX_NUM_NODES];
#endif

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_DMxStateType, J1939DCM_VAR_NOINIT) J1939Dcm_DMxState[J1939DCM_MAX_NUM_NODES];
J1939DCM_LOCAL volatile VAR(J1939Dcm_DMxQueueType, J1939DCM_VAR_NOINIT) J1939Dcm_DMxQueue[J1939DCM_MAX_NUM_NODES];
#endif

#if (J1939DCM_NEED_DTC_HANDLER == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_DtcHandlerType, J1939DCM_VAR_NOINIT) J1939Dcm_DtcHandlerState;
#endif

#if (J1939DCM_NEED_CLEAR_HANDLER == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_ClearHandlerType, J1939DCM_VAR_NOINIT) J1939Dcm_ClearHandlerState;
#endif

#if (J1939DCM_NEED_MEM_ACCESS == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_MemHandlerType,     J1939DCM_VAR_NOINIT) J1939Dcm_MemHandlerState;
#endif

# if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
J1939DCM_LOCAL VAR(J1939Dcm_RxHandlerType,   J1939DCM_VAR_NOINIT) J1939Dcm_RxHandlerState;
# endif

J1939DCM_LOCAL volatile VAR(J1939Dcm_TxHandlerType, J1939DCM_VAR_NOINIT) J1939Dcm_TxHandlerState;
J1939DCM_LOCAL VAR(boolean, J1939DCM_VAR_NOINIT) J1939Dcm_DemAccessSemaphore;

#define J1939DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* --- Global Config Pointer --------------------------------------------------------------------------------------- */
#if (J1939DCM_USE_INIT_POINTER == STD_ON)
# define J1939DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"                                                                                                   /* PRQA S 5087 */ /* MD_MSR_19.1 */
J1939DCM_LOCAL P2CONST(J1939Dcm_ConfigType, J1939DCM_VAR_NOINIT, J1939DCM_INIT_DATA)  J1939Dcm_ConfigDataPtr;
# define J1939DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"                                                                                                   /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  CONSISTENCY CHECKS
 **********************************************************************************************************************/
#if (J1939DCM_MAX_NUM_CHANNELS > 16)
# error "Currently, the J1939Dcm does not support more than 16 channels"
#endif

#if (J1939DCM_METADATA_LEN != 2)
# error "Currently, only a MetaDataLength of 2 is supported by J1939Dcm"
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define J1939DCM_START_SEC_CODE
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * J1939Dcm_IsNodeChannelSupported()
 **********************************************************************************************************************/
/*! \brief      Verifies whether a node references a specific channel.
 *  \details    Returns true if a node references a specific channel. Returns false otherwise.
 *  \param[in]  NodeHdl              Internal Node Handle
 *  \param[in]  ChannelHdl           Internal Channel Handle
 *  \return     TRUE                 If a node references a specific channel.
 *  \return     FALSE                If a node does not reference a specific channel.
 *  \context    TASK|ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        -
 **********************************************************************************************************************/
J1939DCM_LOCAL_INLINE FUNC(boolean, J1939DCM_CODE) J1939Dcm_IsNodeChannelSupported(J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl);

/* --- common functions --------------------------------------------------------------------------------------------- */
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_Reset( void );
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DemSemaphoreLock( P2VAR(boolean, AUTOMATIC, J1939DCM_VAR_NOINIT) pUserSemaphore );
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DemSemaphoreRelease( P2VAR(boolean, AUTOMATIC, J1939DCM_VAR_NOINIT) pUserSemaphore );

#if (J1939DCM_SINGLE_NODE_OPT != STD_ON)
J1939DCM_LOCAL FUNC(J1939Dcm_NodeHdlType,    J1939DCM_CODE) J1939Dcm_GetNodeHdlById( uint8_least NodeId );
J1939DCM_LOCAL FUNC(J1939Dcm_NodeHdlType, J1939DCM_CODE) J1939Dcm_GetNodeHdlByAddr( uint8_least NodeAddress );
#endif

#if (J1939DCM_SINGLE_CHANNEL_OPT != STD_ON)
J1939DCM_LOCAL FUNC(J1939Dcm_ChannelHdlType, J1939DCM_CODE) J1939Dcm_GetChannelHdlById( uint8_least ChannelId );
#endif

/**********************************************************************************************************************
 * J1939Dcm_GetMsgHdlByPGN()
 **********************************************************************************************************************/
/*! \brief      Finds a local ID of a given DM in the message table.
 *  \details    Verifies a particular DM belongs to a particular pair of node and channel. Upon success, a valid local
 *              ID (index) in the message table for the requested DM is returned.
 *  \param[in]  PGN          Requested PGN
 *  \param[in]  NodeHdl      Node by which the request was received
 *  \param[in]  ChannelHdl   Handle of the current channel on which DM is received
 *  \return     lMessageHdl  A valid index in the message table if DM is found or invalid handle
 *  \context    ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Dcm_MessageHdlType, J1939DCM_CODE) J1939Dcm_GetMsgHdlByPGN( uint32 PGN, J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl );
J1939DCM_LOCAL FUNC(PduIdType, J1939DCM_CODE) J1939Dcm_GetPeerTxPduByDM( J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl, uint8_least DiagMsg );

#if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_RxHandlerReset( void );
#else
# define J1939Dcm_RxHandlerReset()
#endif

/* --- message processors ------------------------------------------------------------------------------------------- */
#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Init( void );

/**********************************************************************************************************************
 * J1939Dcm_DM1Task()
 **********************************************************************************************************************/
/*! \brief      Executes the task elements of DM1.
 *  \details    Prepares for DM1 processing and triggers DM1 processor.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if DM1 is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Task( void );
/**********************************************************************************************************************
 * J1939Dcm_DM1Processor()
 **********************************************************************************************************************/
/*! \brief      Processes DM1 message.
 *  \details    Prepares for a DM1 response and triggers transmit for a response or an acknowledgement.
 *  \param[in]  Node      Node by which the request was received
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if DM1 is supported
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Processor( J1939Dcm_NodeHdlType Node );
#else
# define J1939Dcm_DM1Init()
# define J1939Dcm_DM1Task()
#endif

#if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM13Init( void );
/**********************************************************************************************************************
 * J1939Dcm_DM13Task()
 **********************************************************************************************************************/
/*! \brief      Executes the task elements of DM13.
 *  \details    Prepares and processes DM13.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if DM13 is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM13Task( void );
#else
# define J1939Dcm_DM13Init()
# define J1939Dcm_DM13Task()
#endif

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Init( void );
/**********************************************************************************************************************
 * J1939Dcm_DM35Task()
 **********************************************************************************************************************/
/*! \brief      Executes the task elements of DM35.
 *  \details    Prepares for DM35 processing and triggers DM35 processor.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if DM35 is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Task( void );
/**********************************************************************************************************************
 * J1939Dcm_DM35Processor()
 **********************************************************************************************************************/
/*! \brief      Processes DM35 message.
 *  \details    Prepares for a DM35 response and triggers transmit for a response.
 *  \param[in]  Node      Node by which the request was received
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if DM35 is supported
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Processor( J1939Dcm_NodeHdlType Node );
#else
# define J1939Dcm_DM35Init()
# define J1939Dcm_DM35Task()
#endif

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxInit( void );

/**********************************************************************************************************************
 * J1939Dcm_DMxTask()
 **********************************************************************************************************************/
/*! \brief      Executes task elements for a generic DM.
 *  \details    Prepares for a generic DM processing and triggers DMx processor.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if at least one of the diagnostic messages DM2-DM5, DM11, DM24, DM25,
 *              DM27, DM31, and DM53-DM55, is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxTask( void );

/**********************************************************************************************************************
 * J1939Dcm_DMxProcessor()
 **********************************************************************************************************************/
/*! \brief      Processes a generic DM message.
 *  \details    Prepares for a response to a requested DM and triggers transmit for a response or an acknowledgement.
 *  \param[in]  Node      Node by which the request was received
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only if at least one of the diagnostic messages DM2-DM5, DM11, DM24, DM25,
 *              DM27, DM31, and DM53-DM55, is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxProcessor( J1939Dcm_NodeHdlType Node );
#else
# define J1939Dcm_DMxInit()
# define J1939Dcm_DMxTask()
#endif

/* --- Tx handler --------------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 * J1939Dcm_TxHandlerPrepare()
 **********************************************************************************************************************/
/*! \brief      Prepares for message transmission.
 *  \details    Decides whether to append the meta data according to the transmission path. Triggers transmission.
 *  \param[in]  pMsgBuffer       Pointer to message buffer
 *  \param[in]  DiagMsg          DM to be transmitted
 *  \param[in]  NodeHdl          Requested node
 *  \param[in]  ResponseAddress  Address to be added to meta data
 *  \return     E_OK             Transmission request accepted
 *  \return     E_NOT_OK         Transmission is in progress, request not accepted
 *  \context    TASK
 *  \reentrant  FALSE
 *  \pre        -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_TxHandlerPrepare( P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pMsgBuffer,
                                                                              uint8_least DiagMsg,
                                                                              J1939Dcm_NodeHdlType NodeHdl,
                                                                              uint8_least ResponseAddress );
/**********************************************************************************************************************
 * J1939Dcm_TxHandlerTransmit()
 **********************************************************************************************************************/
/*! \brief      Prepares for message transmission.
 *  \details    Triggers the PduR transmit function. Checks whether the transmission has been confirmed.
 *  \param[in]  TxPdu         PduId
 *  \param[in]  ChannelHdl    Channel on which transmission shall start
 *  \return     E_OK          Transmission request accepted
 *  \return     E_NOT_OK      Transmission failed
 *  \context    TASK
 *  \reentrant  FALSE
 *  \pre        -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_TxHandlerTransmit( PduIdType TxPdu, J1939Dcm_ChannelHdlType ChannelHdl );
/**********************************************************************************************************************
 * J1939Dcm_TxHandlerConfirm()
 **********************************************************************************************************************/
/*! \brief      Indicates transmission success or failure.
    \details    Triggers transmission finalization or negative acknowledgement.
 *  \param[in]  NodeHdl   Node which is confirmed
 *  \param[in]  DiagMsg   Message which is confirmed
 *  \param[in]  result    Transmission result
 *  \context    TASK|ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        -
 *  \note       Is used to finalize a transmission when TpTxConfirmation, TxConfirmation or timeout occurs
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void,           J1939DCM_CODE) J1939Dcm_TxHandlerConfirm(J1939Dcm_NodeHdlType NodeHdl, uint8_least DiagMsg, Std_ReturnType Result );
J1939DCM_LOCAL FUNC(void,           J1939DCM_CODE) J1939Dcm_TxHandlerReset( void );
J1939DCM_LOCAL FUNC(void,           J1939DCM_CODE) J1939Dcm_TxHandlerTask( void );

/* --- Memory handler ----------------------------------------------------------------------------------------------- */
#if (J1939DCM_NEED_MEM_ACCESS == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_MemHandlerReset( void );
/**********************************************************************************************************************
 * J1939Dcm_MemHandlerTask()
 **********************************************************************************************************************/
/*! \brief      Executes the task elements of a memory message transmission.
 *  \details    Transmits and retries transmission of memory message.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     This function is available only when any from DM14 to DM18 is supported.
 *  \ pre       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_MemHandlerTask( void );
#else
# define J1939Dcm_MemHandlerReset()
# define J1939Dcm_MemHandlerTask()
#endif

/* --- DTC handler -------------------------------------------------------------------------------------------------- */
#if (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetDtcFilter( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                          P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                          P2CONST(J1939Dcm_FilterHandlerType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pFilterHandler);
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetDTCs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState, uint16_least MaxNumDtcs );
#endif

#if (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetLampFilter( P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData, J1939Dcm_NodeHdlType NodeHdl );
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetLampDTCs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState );
#endif

#if (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetFFFilter( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                         P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                         Dem_J1939DcmSetFreezeFrameFilterType FFKind,
                                                                         J1939Dcm_NodeHdlType NodeHdl );
# if (J1939DCM_DM24_SUPPORT == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetSupportedSPNs( J1939Dcm_NodeHdlType NodeHdl );
# endif
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetFFs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState );
#endif

#if (J1939DCM_NEED_DTC_HANDLER_NUM == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetNumber( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState );
#endif

#if (J1939DCM_NEED_DTC_HANDLER_OBD == STD_ON)
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetReadiness( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                          P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                          uint8 DiagMsg,
                                                                          J1939Dcm_NodeHdlType NodeHdl);
#endif

/* --- clear handler ------------------------------------------------------------------------------------------------ */
#if (J1939DCM_NEED_CLEAR_HANDLER == STD_ON)

/**********************************************************************************************************************
 * J1939Dcm_ClearHandler()
 **********************************************************************************************************************/
/*! \brief       Forwards clear requests to DEM.
 *  \details     Updates processing state and sets acknowledgement code based on the result of clearing DTC.
 *  \param[out]  pDMProcessorState       pointer to state of DM processor
 *  \return      J1939RM_ACK_POSITIVE    DEM accepted the clear request or the clear is pending.
 *  \return      J1939RM_ACK_NEGATIVE    DEM did not accept the clear request.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \config      If supported any DM that needs the clear handler.
 *  \pre         Lock for DEM resources is obtained.
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Rm_AckCode, J1939DCM_CODE) J1939Dcm_ClearHandler( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * J1939Dcm_IsNodeChannelSupported()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL_INLINE FUNC(boolean, J1939DCM_CODE) J1939Dcm_IsNodeChannelSupported(J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl)
{
  J1939Dcm_IgnoreUnusedArgument(NodeHdl)         /* parameter not used in single node configurations. */     /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(ChannelHdl)      /* parameter not used in single channel configurations. */  /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

   /* #10 Return whether the requested node references the requested channel. */
  return J1939Dcm_Cfg_GetNodeChannelSupport(NodeHdl, ChannelHdl);
}

#if (J1939DCM_SINGLE_NODE_OPT != STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_GetNodeHdlById
 **********************************************************************************************************************/
/*! \brief      for a given node id, the index to access the node configuration is returned
 *  \param[in]  NodeId   Id defined by NM
 *  \return     lNodeHdl Node handle for a given NodeId or invalid handle
 *  \context    request indication functions
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Dcm_NodeHdlType, J1939DCM_CODE) J1939Dcm_GetNodeHdlById( uint8_least NodeId )
{
  J1939Dcm_NodeHdlType lNodeHdl = J1939Dcm_Cfg_GetNumberOfNodes();

  do
  {
    lNodeHdl--;

    if (J1939Dcm_Cfg_GetNodeId(lNodeHdl) == NodeId)
    {
      return lNodeHdl;
    }
  } while (lNodeHdl > 0);

  /* when coming here, no matching entry have been found */
  return J1939DCM_INVALID_NODE_HDL;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* J1939DCM_SINGLE_NODE_OPT != STD_ON */

#if (J1939DCM_SINGLE_NODE_OPT != STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_GetNodeHdlByAddr
 **********************************************************************************************************************/
/*! \brief      for a given node address, the index to access the node configuration is returned
 *  \param[in]  uint8_least             NodeAddress   address defined by NM
 *  \return     J1939Dcm_NodeHdlType    Node handle for a given NodeId
 *  \context    request indication functions
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Dcm_NodeHdlType, J1939DCM_CODE) J1939Dcm_GetNodeHdlByAddr( uint8_least NodeAddress )
{
  J1939Dcm_NodeHdlType lNodeHdl = J1939Dcm_Cfg_GetNumberOfNodes();

  do
  {
    lNodeHdl--;

    if (J1939Dcm_Cfg_GetNodeAddress(lNodeHdl) == NodeAddress)
    {
      return lNodeHdl;
    }
  } while (lNodeHdl > 0);

  /* when coming here, no matching entry have been found */
  return J1939DCM_INVALID_NODE_HDL;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* J1939DCM_SINGLE_NODE_OPT != STD_ON */

#if (J1939DCM_SINGLE_CHANNEL_OPT != STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_GetChannelHdlById
 **********************************************************************************************************************/
/*! \brief      for a given channel id, the index to access the channel configuration is returned
 *  \param[in]  uint8                   ChannelId   Id defined by ComM
 *  \return     J1939Dcm_ChannelHdlType    Channel handle for a given ChannelId
 *  \context    request indication functions
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Dcm_ChannelHdlType, J1939DCM_CODE) J1939Dcm_GetChannelHdlById( uint8_least ChannelId )
{
  J1939Dcm_ChannelHdlType lChannelHdl = J1939Dcm_Cfg_GetNumberOfChannels();
  do
  {
    lChannelHdl--;

    if (J1939Dcm_Cfg_GetChannelId(lChannelHdl) == ChannelId)
    {
      return lChannelHdl;
    }
  } while (lChannelHdl > 0);

  /* when coming here, no matching entry have been found */
  return J1939DCM_INVALID_CHANNEL_HDL;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* J1939DCM_SINGLE_CHANNEL_OPT != STD_ON */

/**********************************************************************************************************************
 * J1939Dcm_GetMsgHdlByPGN ()
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
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Dcm_MessageHdlType, J1939DCM_CODE) J1939Dcm_GetMsgHdlByPGN( uint32 PGN, J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl )
{
  uint8_least lUpLimit;
  uint8_least lLoLimit;
  J1939Dcm_MessageHdlType lTmpHdl;
  J1939Dcm_MessageHdlType lMessageHdl;

  J1939Dcm_IgnoreUnusedArgument(NodeHdl)         /* parameter not used in single node configurations. */     /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(ChannelHdl)      /* parameter not used in single channel configurations. */  /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* Start binary search routine based on the ascendingly sorted PGN number for each channel in the message table. */

  /* #10 Obtain the initial start and end search indices for messages that belong to a channel handle in the message table. */
  lLoLimit = J1939Dcm_GetCfg_MessageTableStartIdxOfCfg_ChannelTable(ChannelHdl);
  lUpLimit = J1939Dcm_GetCfg_MessageTableEndIdxOfCfg_ChannelTable(ChannelHdl);

  /* #11 Set an initial position for the message handle in the middle of a range specified by the initial start and end search indices. */
  lMessageHdl = (J1939Dcm_MessageHdlType)(lLoLimit + (uint8_least)((uint8_least)(lUpLimit - lLoLimit) >> 1));

  /* #12 Iterate over the DMs. */
  do
  {
    /* #13 Update the iterator with the previously calculated message handle. */
    lTmpHdl = lMessageHdl;
    /* #14 If the looked-up-for PGN in the message table is equal to the requested PGN: */
    if (J1939Dcm_Cfg_GetMessagePGN(lMessageHdl) == PGN)
    {
      /* #15 If multiple nodes are configured: */
# if (J1939DCM_SINGLE_NODE_OPT != STD_ON)
      /* #16 If the DM is not configured in the requested node: */
      if (J1939DCM_INVALID_PDU_ID == J1939Dcm_Cfg_GetMessageTxPdu(NodeHdl, lMessageHdl))
      {
        /* Message found, but it is not supported by current node */
        /* #17 Return invalid message handle. */
        return J1939DCM_INVALID_MESSAGE_HDL;
      }
      else
#endif
        /* #18 Otherwise (if the DM is configured in the requested node or a single node is configured): */
      {
        /* #19 Return a valid message handle. */
        return lMessageHdl;
      }
    }
    /* #20 If the looked-up-for PGN in the message table is greater than the requested PGN: */
    else if (J1939Dcm_Cfg_GetMessagePGN(lMessageHdl) > PGN)
    {
      /* Message exists in the lower half of the binary search domain. */
      /* #21 Update the end search index with the previously calculated message handle. */
      lUpLimit = lMessageHdl;
    }
    /* #22 If the looked-up-for PGN in the message table is smaller than the requested PGN: */
    else
    {
      /* Message exists in the upper half of the binary search domain. */
      /* #23 Update the start search index with the previously calculated message handle. */
      lLoLimit = lMessageHdl;
    }
    /* #24 Update the message handle to be in the middle of the newly calculated binary search domain. */
    lMessageHdl = (J1939Dcm_MessageHdlType)(lLoLimit + (uint8_least)((uint8_least)(lUpLimit - lLoLimit) >> 1));
  } while (lMessageHdl != lTmpHdl); /* #25 Terminate iteration when the start and end search indices coincide. */

  /* #26 Return invalid message handle because no match was found. */
  return J1939DCM_INVALID_MESSAGE_HDL;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 * J1939Dcm_GetPeerTxPduByDM
 **********************************************************************************************************************/
/*! \brief      for a given node, channel and DM, get the according peer TxPdu Id
 *  \param[in]  NodeHdl              internal Node Handle
 *  \param[in]  ChannelHdl           internal Channel Handle
 *  \param[in]  DiagMsg              DM for which the PduId shall be retrieved
 *  \return     PduIdType            Peer TxPduId
 *  \context    task and API context
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(PduIdType, J1939DCM_CODE) J1939Dcm_GetPeerTxPduByDM( J1939Dcm_NodeHdlType NodeHdl, J1939Dcm_ChannelHdlType ChannelHdl, uint8_least DiagMsg )
{
  PduIdType               lTxPdu;
  J1939Dcm_MessageHdlType lMessageHdl;
  boolean                 lIsSupported;

  J1939Dcm_IgnoreUnusedArgument(NodeHdl)         /* parameter not used in single node configurations */        /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(ChannelHdl)      /* parameter not used in single channel configurations */     /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  lIsSupported = J1939Dcm_IsNodeChannelSupported(NodeHdl, ChannelHdl);
  if (FALSE != lIsSupported )
  {
    /* channel is linked to current node; check if DM is supported */
    lMessageHdl = J1939Dcm_GetCfg_MessageTableStartIdxOfCfg_ChannelTable(ChannelHdl);
    do
    {
      if (DiagMsg == J1939Dcm_Cfg_GetMessageDM(lMessageHdl))
      {
        /* current channel supports DM1; remember TxPdu for later use */
        lTxPdu = J1939Dcm_Cfg_GetMessageTxPdu(NodeHdl, lMessageHdl);

        return lTxPdu;
      }
      lMessageHdl++;
    } while (lMessageHdl < J1939Dcm_GetCfg_MessageTableEndIdxOfCfg_ChannelTable(ChannelHdl) );
  } /* else : channel is not referenced by node at all */

  return J1939DCM_INVALID_PDU_ID;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DM1_Init
 **********************************************************************************************************************/
/*! \brief      reset DM1 structures
 *  \context    J1939Dcm_Init
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Init( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;

  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    J1939Dcm_DM1State[lNodeHdl].ProcessingState       = J1939DCM_MSG_STATE_IDLE;
    J1939Dcm_DM1State[lNodeHdl].Timer                 = J1939DCM_DM1_TIMEOUT;
    J1939Dcm_DM1State[lNodeHdl].ChannelHdl            = J1939DCM_INVALID_CHANNEL_HDL;
    J1939Dcm_DM1State[lNodeHdl].ResponseDA            = J1939DCM_ADDRESS_BROADCAST;
    J1939Dcm_DM1State[lNodeHdl].DTCSTatusChanged      = FALSE;
    J1939Dcm_DM1State[lNodeHdl].HasDemAccess          = FALSE;
    J1939Dcm_DM1State[lNodeHdl].BufferInfo.SduDataPtr = &J1939Dcm_DM1State[lNodeHdl].Buffer[0];
    J1939Dcm_DM1State[lNodeHdl].BufferInfo.SduLength  = 0;
    J1939Dcm_DM1State[lNodeHdl].IsTimerExpired        = FALSE;

    J1939Dcm_DM1Queue[lNodeHdl].RequestPending        = J1939DCM_QUEUE_EMPTY;
    J1939Dcm_DM1Queue[lNodeHdl].StateChangeRequest    = J1939DCM_STATE_OFFLINE;
    J1939Dcm_DM1Queue[lNodeHdl].ChannelHdl            = J1939DCM_INVALID_CHANNEL_HDL;
    J1939Dcm_DM1Queue[lNodeHdl].ResponseDA            = J1939DCM_ADDRESS_BROADCAST;

    /* get the DM1 TxPdus for each channel from config */
    J1939Dcm_ForEachChannel_Start
    {
      J1939Dcm_ForEachChannel_Next;

      J1939Dcm_DM1State[lNodeHdl].TxPdus[lChannelHdl] = J1939Dcm_GetPeerTxPduByDM(lNodeHdl, lChannelHdl, J1939DCM_DM1);

    } J1939Dcm_ForEachChannel_End
  } J1939Dcm_ForEachNode_End
}
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DM13Init
 **********************************************************************************************************************/
/*! \brief      reset of DM13 structures
 *  \context    J1939Dcm_Init
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM13Init( void )
{
  J1939Dcm_ChannelHdlType lChannelHdl;

  J1939Dcm_DM13State.NumActiveBroadcastChannels = J1939Dcm_Cfg_GetNumberOfChannels();
  J1939Dcm_DM13State.BroadcastStatusInformation = J1939Dcm_Cfg_GetInitBroadcastStatus();  /* SWS_J1939Dcm_00129 */
  J1939Dcm_DM13State.BroadcastDisableTimer = 0;
  J1939Dcm_DM13Queue.RequestPending = FALSE;

  /* clear queue for all configured channels */
  J1939Dcm_ForEachChannel_Start
  {
    J1939Dcm_ForEachChannel_Next;
    J1939Dcm_DM13Queue.BroadcastCommand[lChannelHdl] = J1939DCM_DM13_BROADCAST_DC;
  } J1939Dcm_ForEachChannel_End
}
#endif /* (J1939DCM_NEED_DM13_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DM35_Init
 **********************************************************************************************************************/
/*! \brief      reset of DM35 structures
 *  \context    J1939Dcm_Init
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Init( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;

  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    J1939Dcm_DM35State[lNodeHdl].ProcessingState       = J1939DCM_MSG_STATE_IDLE;
    J1939Dcm_DM35State[lNodeHdl].Timer                 = J1939DCM_DM35_TIMEOUT;
    J1939Dcm_DM35State[lNodeHdl].ActiveChannels        = 0;
    J1939Dcm_DM35State[lNodeHdl].HasDemAccess          = FALSE;
    J1939Dcm_DM35State[lNodeHdl].BufferInfo.SduDataPtr = &J1939Dcm_DM35State[lNodeHdl].Buffer[0];
    J1939Dcm_DM35State[lNodeHdl].BufferInfo.SduLength  = 0;

    J1939Dcm_DM35Queue[lNodeHdl].RequestPending        = J1939DCM_QUEUE_EMPTY;
    J1939Dcm_DM35Queue[lNodeHdl].StateChangeRequest    = J1939DCM_STATE_OFFLINE;
    J1939Dcm_DM35Queue[lNodeHdl].RequestedChannels     = 0;

    /* get the DM35 TxPdus for each channel from config */
    J1939Dcm_ForEachChannel_Start
    {
      J1939Dcm_ForEachChannel_Next;

      J1939Dcm_DM35State[lNodeHdl].TxPdus[lChannelHdl] = J1939Dcm_GetPeerTxPduByDM(lNodeHdl, lChannelHdl, J1939DCM_DM35);

    } J1939Dcm_ForEachChannel_End

  } J1939Dcm_ForEachNode_End
}
#endif /* (J1939DCM_NEED_DM35_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DMxInit
 **********************************************************************************************************************/
/*! \brief      reset DMx structures
 *  \context    J1939Dcm_Init
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxInit( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;

  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    J1939Dcm_DMxState[lNodeHdl].ProcessingState       = J1939DCM_MSG_STATE_IDLE;
    J1939Dcm_DMxState[lNodeHdl].ChannelHdl            = J1939DCM_INVALID_CHANNEL_HDL;
    J1939Dcm_DMxState[lNodeHdl].MessageHdl            = J1939DCM_INVALID_MESSAGE_HDL;
    J1939Dcm_DMxState[lNodeHdl].ResponseDA            = J1939DCM_ADDRESS_BROADCAST;
    J1939Dcm_DMxState[lNodeHdl].HasDemAccess          = FALSE;
    J1939Dcm_DMxState[lNodeHdl].BufferInfo.SduDataPtr = &J1939Dcm_DMxState[lNodeHdl].Buffer[0];
    J1939Dcm_DMxState[lNodeHdl].BufferInfo.SduLength  = 0;

    J1939Dcm_DMxQueue[lNodeHdl].RequestPending        = J1939DCM_QUEUE_EMPTY;
    J1939Dcm_DMxQueue[lNodeHdl].ChannelHdl            = J1939DCM_INVALID_CHANNEL_HDL;
    J1939Dcm_DMxQueue[lNodeHdl].MessageHdl            = J1939DCM_INVALID_MESSAGE_HDL;
    J1939Dcm_DMxQueue[lNodeHdl].ResponseDA            = J1939DCM_ADDRESS_BROADCAST;
  } J1939Dcm_ForEachNode_End
}
#endif /* (J1939DCM_NEED_DMX_PROCESSOR == STD_ON) */

/**********************************************************************************************************************
 * J1939Dcm_DemSemaphoreLock
 **********************************************************************************************************************/
/*! \brief      lock DEM semaphore
 *  \context    J1939Dcm_MainFunction
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DemSemaphoreLock( P2VAR(boolean, AUTOMATIC, J1939DCM_VAR_NOINIT) pUserSemaphore )
{
  /* semaphore is free */
  if (FALSE == J1939Dcm_DemSemaphoreGetLocked())
  {
    J1939Dcm_DemSemaphoreSetLocked(TRUE);
    *pUserSemaphore = TRUE;
  }
  else
  {
    *pUserSemaphore = FALSE;
  }
}

/**********************************************************************************************************************
 * J1939Dcm_DemSemaphoreRelease
 **********************************************************************************************************************/
/*! \brief      lock DEM semaphore
 *  \context    J1939Dcm_MainFunction
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DemSemaphoreRelease( P2VAR(boolean, AUTOMATIC, J1939DCM_VAR_NOINIT) pUserSemaphore )
{
  /* semaphore is in use by the user who requests the semaphore */
  if ( (FALSE != J1939Dcm_DemSemaphoreGetLocked()) && (FALSE != *pUserSemaphore) )
  {
    J1939Dcm_DemSemaphoreSetLocked(FALSE);
    *pUserSemaphore = FALSE;
  }
  /* else: the semaphore is in use by another user or has already been released */
}

#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DM1_Task
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
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Task( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;

  /* #10 Iterate over the defined nodes because the queue of events of a diagnostic message is node dependent. */
  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    /* --- check queued events --- */
    /* #11 If any of the interrupt service routine functions updated the queue of events of a diagnostic message: */
    if (J1939DCM_QUEUE_EMPTY != J1939Dcm_DM1Queue[lNodeHdl].RequestPending)
    {
      J1939Dcm_DM1QueueType lTmpQueueData;
      boolean               lSendNACK;

      lSendNACK = FALSE;

      /* #12 Enter critical section (Reason: protect data consistency of the queue of events against request manager interruptions). */
      J1939Dcm_EnterCritical();

      /* #13 Copy the queue of events into a local parameter and clear the queue of events. */
      lTmpQueueData.ResponseDA         = J1939Dcm_DM1Queue[lNodeHdl].ResponseDA;
      lTmpQueueData.ChannelHdl         = J1939Dcm_DM1Queue[lNodeHdl].ChannelHdl;
      lTmpQueueData.StateChangeRequest = J1939Dcm_DM1Queue[lNodeHdl].StateChangeRequest;
      lTmpQueueData.RequestPending     = J1939Dcm_DM1Queue[lNodeHdl].RequestPending;

      J1939Dcm_QueueClearAll(J1939Dcm_DM1Queue[lNodeHdl]);

      /* #14 Leave critical section. */
      J1939Dcm_ExitCritical();

      /* Handle queued TxConfirmation. */
      /* #15 If a transmission of a diagnostic message is confirmed in the current task: */
      if (FALSE != J1939Dcm_QueueGetTxConf(lTmpQueueData))
      {
        /* #16 If a transmission of a diagnostic message was triggered in the previous task: */
        if (J1939DCM_MSG_STATE_TXCONF == J1939Dcm_DM1State[lNodeHdl].ProcessingState)
        {
          /* #17 Set Processing state to not busy because TxConfirmation for all transmitting channels were received. */
          J1939Dcm_DM1State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_WAIT;
        } /* else: TxConfirmation no longer awaited (e.g. because of change to offline); ignore */

        /* #18 Reset transmission because the TxHandler is still in use even if the confirmation is no longer expected. */
        J1939Dcm_TxHandlerReset();
      }

      /* Handle queued failed TxConfirmation. */
      /* #19 If a transmission of a diagnostic message is failed in the current task: */
      if (FALSE != J1939Dcm_QueueGetTxFail(lTmpQueueData))
      {
        /* #20 If a transmission of a diagnostic message was triggered in the previous task: */
        if (J1939DCM_MSG_STATE_TXCONF == J1939Dcm_DM1State[lNodeHdl].ProcessingState)
        {
          /* #21 Set the processing state to send acknowledgement because TxConfiramtion was failed and thus NACK is required. */
          J1939Dcm_DM1State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
        } /* else: TxConfirmation no longer awaited (e.g. because of change to offline); ignore */

        /* #22 Reset transmission because the TxHandler is still in use even if the confirmation is no longer expected. */
        J1939Dcm_TxHandlerReset();
      }

      /* Handle queued communication state change. */
      /* #23 If the communication state was changed: */
      if (FALSE != J1939Dcm_QueueGetStateChange(lTmpQueueData))
      {
        /* #24 If the communication state went from offline to online: */
        if (J1939DCM_STATE_ONLINE == lTmpQueueData.StateChangeRequest)
        {
          /* #25 If processing state indicates idle: */
          if (J1939DCM_MSG_STATE_IDLE == J1939Dcm_DM1State[lNodeHdl].ProcessingState)
          {
            /* #26 Set processing state to not busy. */
            J1939Dcm_DM1State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_WAIT;
          } /* else: node is already online; do not change internal state */
        }
        /* #27 Otherwise (if the communication state went from online to offline): */
        else
        {
          /* #28 Release DEM resources. */
          J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM1State[lNodeHdl].HasDemAccess);
          /* #29 Set processing state to idle. */
          J1939Dcm_DM1State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_IDLE;
        }
      }

      /* Handle queued DM1 request. */
      /* #30 If a new diagnostic message is requested: */
      if (FALSE != J1939Dcm_QueueGetMsgRequest(lTmpQueueData))
      {
        /* #31 If the processing state indicates not busy (insure that DM1 processor is inactive in order to trigger a single transmission): */
        if (J1939DCM_MSG_STATE_WAIT == J1939Dcm_DM1State[lNodeHdl].ProcessingState)
        {
          /* #32 Prepare for message processing by updating the processing state to start message preparation and copying the queue of events into the state of the diagnostic message. */
          J1939Dcm_DM1State[lNodeHdl].ProcessingState   = J1939DCM_MSG_STATE_PREP_START;
          J1939Dcm_DM1State[lNodeHdl].ResponseDA        = lTmpQueueData.ResponseDA;
          J1939Dcm_DM1State[lNodeHdl].ChannelHdl        = lTmpQueueData.ChannelHdl;
        }
        /* #33 Otherwise (if the processing state indicates busy): */
        else
        {
          /* #34 If the response address of the requested diagnostic message is not global: */
          if (J1939DCM_ADDRESS_BROADCAST != lTmpQueueData.ResponseDA)
          {
            /* #35 Trigger sending a negative acknowledgement for non-broadcast requests and do not change processing state because transmission is already in progress. */
            lSendNACK = TRUE;
          }
        }
      }

      /* Handle queued DTC status change. */
      /* #36 If DTC status change occurred, if the change is the first in this one-second period, and if the processing state indicates not busy: */
      if ( (FALSE != J1939Dcm_QueueGetDTCChange(lTmpQueueData)) &&                        /* DTC status change occurred */
           (FALSE == J1939Dcm_DM1State[lNodeHdl].DTCSTatusChanged) &&                   /* first DTC status change in this 1sec period? */
           (J1939DCM_MSG_STATE_WAIT == J1939Dcm_DM1State[lNodeHdl].ProcessingState) )   /* is DM1 already processed? */
      {
        /* #37 Indicate the occurrence of the first DTC status change in this one-second period. */
        J1939Dcm_DM1State[lNodeHdl].DTCSTatusChanged = TRUE;

        /* DM1 processor is inactive; trigger broadcast transmission. */
        /* #38 Prepare for message processing by updating the processing state to start message preparation and the state of the diagnostic message to broadcast transmission via all online channels. */
        J1939Dcm_DM1State[lNodeHdl].ProcessingState   = J1939DCM_MSG_STATE_PREP_START;
        J1939Dcm_DM1State[lNodeHdl].ResponseDA        = J1939DCM_ADDRESS_BROADCAST;
        J1939Dcm_DM1State[lNodeHdl].ChannelHdl        = J1939DCM_INVALID_CHANNEL_HDL;
      } /* else: ignore eventually pending requests */

      /* #39 If Sending a negative acknowledgement was triggered: */
      if (FALSE != lSendNACK)
      {
        /* #40 Request sending negative acknowledgement from the request manager. */
        (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(lNodeHdl),
                              J1939Dcm_Cfg_GetChannelId(lTmpQueueData.ChannelHdl),                                      /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */
                              J1939DCM_DM1_PGN,
                              NULL_PTR,
                              J1939RM_ACK_CANNOT_RESPOND,
                              lTmpQueueData.ResponseDA,                                                                 /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */
                              J1939DCM_DM1_PRIORITY,
                              FALSE);
      }
    } /* J1939DCM_QUEUE_EMPTY */

    /* #41 If the processing state does not indicate idle: */
    if (J1939DCM_MSG_STATE_IDLE != J1939Dcm_DM1State[lNodeHdl].ProcessingState)
    {
      /* Node is currently online */

      /* --- update timers --- */
      /* #42 If the one-second period is not expired: */
      if (0 < J1939Dcm_DM1State[lNodeHdl].Timer)
      {
        /* #43 If DM13 is supported, then at least one channel has to be active. */
# if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
        if (0 != J1939Dcm_DM13State.NumActiveBroadcastChannels)   /* if broadcast is disabled for all channels, there's no need to count */
# endif
        {
          /* #44 Decrement the one-second timer. */
          J1939Dcm_DM1State[lNodeHdl].Timer--;
        }
      }

      /* --- check for timeout --- */
      /* If the one-second period timer expired: */
      if ( 0 == J1939Dcm_DM1State[lNodeHdl].Timer)
      {
        /* timeout; try to trigger broadcast response */
        /* #46 Indicate a none occurrence of any DTC status change in this one-second period. */
        J1939Dcm_DM1State[lNodeHdl].DTCSTatusChanged = FALSE;  /* 1sec period is over; allow DTC status change trigger */
        /* #47 If the processing state indicates not busy: */
        if (J1939DCM_MSG_STATE_WAIT == J1939Dcm_DM1State[lNodeHdl].ProcessingState)
        {
          /* #48 If DEM resources are free: */
          if (FALSE == J1939Dcm_DemSemaphoreGetLocked())
          {
            /* DM1 processor is inactive; trigger broadcast transmission */
            /* #49 Prepare for message processing by updating the processing state to start message preparation and the state of the diagnostic message to broadcast transmission via all online channels. */
            J1939Dcm_DM1State[lNodeHdl].ProcessingState  = J1939DCM_MSG_STATE_PREP_START;
            J1939Dcm_DM1State[lNodeHdl].ResponseDA       = J1939DCM_ADDRESS_BROADCAST;
            J1939Dcm_DM1State[lNodeHdl].ChannelHdl       = J1939DCM_INVALID_CHANNEL_HDL;
            /* #50 Indicate the one-second timer expiration if any channel conforms with ISOBUS and reset the one-second timer. */
            J1939Dcm_DM1State[lNodeHdl].IsTimerExpired   = TRUE;
            J1939Dcm_DM1State[lNodeHdl].Timer            = J1939DCM_DM1_TIMEOUT;
          }
          /* else: DEM is in use; postpone timer reload until DEM becomes available */
        }
        /* #51 Otherwise (if processing state indicates busy): */
        else
        {
          /* #52 If DM1 processor prepares for broadcast: */
          if (J1939DCM_INVALID_CHANNEL_HDL == J1939Dcm_DM1State[lNodeHdl].ChannelHdl)
          {
            /* #53 Reload the one-second period timer. */
            J1939Dcm_DM1State[lNodeHdl].Timer = J1939DCM_DM1_TIMEOUT;
          }
          /* else: DM1 processor is preparing a single response; postpone timer reload until transmission is finished */
        }
      }
      /* else: no timeout */

      /* --- handle message preparation and transmission --- */
      /* #54 If the processing state indicates busy:
                                           ( If a new DM1 is requested
                                         Or  If any DTC status change in the current one-second period occurred
                                         or  If the 1 second period expired in the current task
                                         Or  If the DM1 processing has not been completed in the previous task
                                         Or  If the DM1 transmission was failed in the current task) */
      if (J1939DCM_MSG_STATE_WAIT < J1939Dcm_DM1State[lNodeHdl].ProcessingState)
      {
        /* #55 Trigger the processing of the diagnostic message. */
        J1939Dcm_DM1Processor(lNodeHdl);
      } /* end: state > J1939DCM_MSG_STATE_WAIT */

    } /* end: not J1939DCM_MSG_STATE_IDLE */
  } J1939Dcm_ForEachNode_End
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
 /***********************************************************************************************************************
 * J1939Dcm_DM1Processor()
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
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM1Processor( J1939Dcm_NodeHdlType Node )
{
  J1939Rm_AckCode lAcknowledge;
  J1939Dcm_FilterHandlerType  lFilterHandler;

  lAcknowledge = J1939RM_ACK_NEGATIVE;

  /* --- start DM1 processing: filter DTCs --- */
  /* #10 If the processing state indicates message preparation start (a new request was received): */
  if (J1939DCM_MSG_STATE_PREP_START == J1939Dcm_DM1State[Node].ProcessingState)
  {
    /* #11 Get a lock on DEM resources in order to import the required data from DEM for the incoming DM request. */
    J1939Dcm_DemSemaphoreLock(&J1939Dcm_DM1State[Node].HasDemAccess);
    /* #12 If a lock on DEM resources is successfully obtained: */
    if (FALSE != J1939Dcm_DM1State[Node].HasDemAccess )
    {
      /* #13 Update the local DTC filter according to DM01 requirements. Trigger read Dtc Handler. */
      lFilterHandler.StatusFilter = DEM_J1939DTC_ACTIVE;
      lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
      lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      lFilterHandler.FiltredNode = Node;
      J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DM1State[Node].ProcessingState, &J1939Dcm_DM1State[Node].BufferInfo,
                                      &lFilterHandler);
    }
    /* #14 Otherwise: */
    else
    {
      /* #15 Skip processing because DEM is currently in use (set processing state to send acknowledgement with code: cannot respond). */
      J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
      lAcknowledge = J1939RM_ACK_CANNOT_RESPOND;
    }
  }

  /* --- finalize DM1 processing: get DTCs --- */
  /* #16 Get matching DTCs, if the processing state is set to get DTCs. */
  if (J1939DCM_MSG_STATE_PREP_DTCS == J1939Dcm_DM1State[Node].ProcessingState)
  {
    J1939Dcm_DtcHandlerGetDTCs(&J1939Dcm_DM1State[Node].ProcessingState, J1939DCM_DM1_MAX_DTCS);
  }

  /* --- transmit prepared DM1 message --- */
  /* #17 If the processing state indicates that the DM is ready for transmission: */
  if (J1939DCM_MSG_STATE_TX == J1939Dcm_DM1State[Node].ProcessingState)
  {
    Std_ReturnType TxResult;
    J1939Dcm_ChannelHdlType lChannelHdl;
    /* #18 If ISOBUS feature is configured, indicate whether an error is currently active. */
# if (J1939DCM_DM1_ISOBUS_CONFORM == STD_ON)
    boolean lIsErrorInactive = (boolean)(0 == J1939Dcm_DtcHandlerState.NumDTCs);
# endif

    /* #19 Release DEM semaphore. */
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM1State[Node].HasDemAccess);

    /* #20 Prepare for transmission. */
    TxResult = J1939Dcm_TxHandlerPrepare(&J1939Dcm_DM1State[Node].BufferInfo, J1939DCM_DM1,
                                         Node, J1939Dcm_DM1State[Node].ResponseDA);

    /* #21 Check for buffer overflow. */
    J1939Dcm_DetAssert((J1939DCM_BUFFERSIZE_DM1 > J1939Dcm_DM1State[Node].BufferInfo.SduLength), J1939DCM_API_MAINFUNCTION, J1939DCM_E_PARAM_CONFIG)

    /* #22 If preparation for transmission was succeeded: */
    if (E_OK == TxResult)
    {
      /* transmission prepared; trigger transmit */
      /* #23 If the transmission is global broadcast: */
      if (J1939DCM_INVALID_CHANNEL_HDL == J1939Dcm_DM1State[Node].ChannelHdl)
      {
        boolean lAnyTxSucceeded;

        /* #24 Set processing state to wait for TxConfirmation. */
        J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_TXCONF;
        lAnyTxSucceeded = FALSE;

        /* broadcast: transmit on all channels which support DM1 */
        /* #25 Iterate over the available channels. */
        J1939Dcm_ForEachChannel_Start
        {
          J1939Dcm_ForEachChannel_Next;
          /* #26 If the incoming TxPdu is valid and the communication state of the node-channel pair is online: */
          if ( (J1939DCM_INVALID_PDU_ID != J1939Dcm_DM1State[Node].TxPdus[lChannelHdl]) &&
               (J1939Dcm_ComStateIsOnline(Node, lChannelHdl)) )
          {
            /* #27 If DM13 is supported, then at least one channel has to be active. */
# if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
            if (FALSE != J1939Dcm_DM13GetBroadcastStatusActive(lChannelHdl))
#endif
            {
              /* channel is online, broadcast is active and it supports DM1 */
              /* #28 If ISOBUS feature is configured, and by the end of the one-second period an error was not active, suppress the transmission. */
# if (J1939DCM_DM1_ISOBUS_CONFORM == STD_ON)
              if ( (TRUE == J1939Dcm_DM1State[Node].IsTimerExpired) &&
                   (TRUE == lIsErrorInactive) &&
                   (TRUE == J1939Dcm_Cfg_GetIsobusChannel(lChannelHdl)))
              {
                /* Do not indicate success for transmission preparation. */
                /* If all other not-ISOBUS channels failed transmission, the processing state of that particular node will be set to not busy (reset processor). */
              }
              /* #29 Otherwise: */
              else
# endif
              {
                /* #30 Trigger transmission and register the transmission results. */
                TxResult = J1939Dcm_TxHandlerTransmit(J1939Dcm_DM1State[Node].TxPdus[lChannelHdl], lChannelHdl);
                lAnyTxSucceeded |= (boolean)(TxResult == E_OK);                                                                   /* PRQA S 4105 */ /* MD_J1939Dcm_12.6 */
              }
            }
          }
        } J1939Dcm_ForEachChannel_End

        /* #31 If ISOBUS feature is configured, reset the active error flag and the flag that indicates the expiration of the one-second period. */
        J1939Dcm_DM1State[Node].IsTimerExpired = FALSE;

        /* #32 If the transmission over all the referenced channels by the requested node was failed:  */
        if (FALSE == lAnyTxSucceeded)
        {
          /* #33 Reset state machine.  */
          J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_WAIT;
          J1939Dcm_TxHandlerReset();
        }
      }
      /* #34 Otherwise (if single transmission): */
      else
      {
        /* channel supports DM1 */

        /* #35 Set processing state to wait for TxConfirmation. */
        J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_TXCONF;
        /* #36 Transmit the request. */
        TxResult = J1939Dcm_TxHandlerTransmit(J1939Dcm_DM1State[Node].TxPdus[J1939Dcm_DM1State[Node].ChannelHdl],
                                              J1939Dcm_DM1State[Node].ChannelHdl);

        /* #37 If the triggering for transmission is failed: */
        if (E_OK != TxResult)
        {
          /* #38 Cancel transmission by resetting Txhandler. Update processing state to send acknowledgement. */
          J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
          J1939Dcm_TxHandlerReset();
        }
      }
    } /* else: transmit handler is busy; retry later */
  }

  /* --- transmit NACK --- */
  /* #39 If the processing state indicates sending acknowledgement: */
  if (J1939DCM_MSG_STATE_SENDACK == J1939Dcm_DM1State[Node].ProcessingState)
  {
    /* #40 Release DEM resources.*/
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM1State[Node].HasDemAccess);

    /* #41 If the source address of the requested message is specific: */
    if ( J1939DCM_ADDRESS_BROADCAST != J1939Dcm_DM1State[Node].ResponseDA )
    {
      /* #42 Request sending acknowledgement from the request manager. */
      (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(Node),
                            J1939Dcm_Cfg_GetChannelId(J1939Dcm_DM1State[Node].ChannelHdl),
                            J1939DCM_DM1_PGN,
                            NULL_PTR,
                            lAcknowledge,
                            J1939Dcm_DM1State[Node].ResponseDA,
                            J1939DCM_DM1_PRIORITY,
                            FALSE);
    }

    /* else: skip NACK; directly reset DM1 processor */
    /* #43 Set processing state to not busy (reset DM1 processor). */
    J1939Dcm_DM1State[Node].ProcessingState = J1939DCM_MSG_STATE_WAIT;

  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_DM13Task()
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
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM13Task( void )
{
  J1939Dcm_DM13QueueType  lTmpQueueData;
  J1939Dcm_ChannelHdlType lChannelHdl;
  boolean                 lBroadcastStateChanged;
  boolean                 lRestartTimeout;

  lBroadcastStateChanged = FALSE;
  lRestartTimeout        = FALSE;

  /* #10 If DM13 has been received but has not yet processed: */
  if (FALSE != J1939Dcm_DM13Queue.RequestPending)
  {
    /* #11 Enter critical section (Reason: protect data consistency of the queue of events against PduR interruptions). */
    J1939Dcm_EnterCritical();

    /* #12 Copy the queue of events into a local parameter and indicate that DM13 has been processed. */
    lTmpQueueData.HoldSignal     = J1939Dcm_DM13Queue.HoldSignal;

    J1939Dcm_ForEachChannel_Start
    {
      J1939Dcm_ForEachChannel_Next;
      lTmpQueueData.BroadcastCommand[lChannelHdl] = J1939Dcm_DM13Queue.BroadcastCommand[lChannelHdl];
    } J1939Dcm_ForEachChannel_End

    J1939Dcm_DM13Queue.RequestPending = FALSE;

    /* #13 Leave critical section. */
    J1939Dcm_ExitCritical();

    /* --- handle queued DM13 requests --- */

    /* #14 If the purpose of the incoming DM13 is to modify the broadcast status (setup Message): */
    if (lTmpQueueData.HoldSignal == J1939DCM_DM13_HOLD_NA)   /* SWS_J1939Dcm_00054 */
    {

      /* #15 Start iteration over the configured channels. */
      J1939Dcm_ForEachChannel_Start
      {
        J1939Dcm_ForEachChannel_Next;

        /* #16 If the broadcast command of a channel indicates start broadcast: */
        if (lTmpQueueData.BroadcastCommand[lChannelHdl] == J1939DCM_DM13_BROADCAST_START)
        {
          /* #17 If the channel is inactive: */
          if (FALSE == J1939Dcm_DM13GetBroadcastStatusActive(lChannelHdl))
          {
            /* #18 Re-activate the channel. */
            J1939Dcm_DM13State.NumActiveBroadcastChannels++;
            J1939Dcm_DM13SetBroadcastStatusActive(lChannelHdl);
            /* #19 Indicate a change in the broadcast state. */
            lBroadcastStateChanged = TRUE;
          } /* else: nothing to be done; broadcast is already active */
        }
        /* #20 Otherwise (if the broadcast command of a channel indicates stop broadcast or do not care): */
        else
        {
          /* #21 If the broadcast command of a channel indicates stop broadcast: */
          if (lTmpQueueData.BroadcastCommand[lChannelHdl] == J1939DCM_DM13_BROADCAST_STOP)
          {
            /* #22 If the channel is active: */
            if (FALSE != J1939Dcm_DM13GetBroadcastStatusActive(lChannelHdl))
            {
              /* #23 Deactivate the channel. */
              J1939Dcm_DM13State.NumActiveBroadcastChannels--;
              J1939Dcm_DM13ClearBroadcastStatusActive(lChannelHdl);
              /* #24 Indicate a change in the broadcast state. */
              lBroadcastStateChanged = TRUE;
            } /* else: nothing to be done; broadcast is already inactive */

            /* valid DM13 received and any channel is stopped: restart timeout */
            /* #25 Trigger reloading DM13 timer. */
            lRestartTimeout = TRUE;
          } /* else: neither start nor stop, ignore (do not care) */
        }
      } J1939Dcm_ForEachChannel_End
    }
    /* #26 If the purpose of the incoming DM13 is to hold the broadcast status (hold Message): */
    else
    {
      /* #27 If the hold signal applies to all available channels: */
      if (lTmpQueueData.HoldSignal == J1939DCM_DM13_HOLD_ALL)
      {
        /* #28 Trigger reloading DM13 timer. */
        lRestartTimeout = TRUE;
      }
      /* else: unknown hold signal; ignore (hold signal to a partial set of the available channels is not supported) */
    }

    /* #29 If all the available channels are active: */
    if (J1939Dcm_Cfg_GetInitBroadcastStatus() == J1939Dcm_DM13State.BroadcastStatusInformation)
    {
      /* #30 Stop timeout supervision. */
      J1939Dcm_DM13State.BroadcastDisableTimer = 0;
    }
    /* #31 Otherwise (if at least one channel is inactive): */
    else
    {
      /* #32 If the reloading of DM13 timer was triggered: */
      if ( FALSE != lRestartTimeout )
      {
        /* #33 Reload DM13 timer. */
        J1939Dcm_DM13State.BroadcastDisableTimer = J1939DCM_DM13_TIMEOUT;
      }
    }
  }

  /* --- update channel timer --- */
  /* #34 If DM13 timer has not been expired: */
  if (J1939Dcm_DM13State.BroadcastDisableTimer != 0)
  {
    /* #35 Decrement DM13 timer. */
    J1939Dcm_DM13State.BroadcastDisableTimer--;

    /* #36 If DM13 is expired: */
    if (J1939Dcm_DM13State.BroadcastDisableTimer == 0)
    {
      /* #37 Re-activate normal state (start broadcast of all available channels). */
      J1939Dcm_DM13State.NumActiveBroadcastChannels = J1939Dcm_Cfg_GetNumberOfChannels();
      J1939Dcm_DM13State.BroadcastStatusInformation = J1939Dcm_Cfg_GetInitBroadcastStatus();
      /*#38 Indicate a change in the broadcast state. */
      lBroadcastStateChanged = TRUE;
    }
  }

  /* -- report new broadcast status to BswM, if needed --- */
  /* #39 If a change in the broadcast occurred: */
  if (FALSE != lBroadcastStateChanged)
  {
    /* #40 Inform BswM with the new broadcast status. */
    BswM_J1939DcmBroadcastStatus(J1939Dcm_DM13State.BroadcastStatusInformation);
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DM13_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_DM35Task()
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
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Task( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_DM35QueueType  lTmpQueueData;

  /* #10 Iterate over the defined nodes because the queue of events of a diagnostic message is node dependent. */
  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    /* --- check queued events --- */
    /* #11 If any of the interrupt service routine functions updated the queue of events of a diagnostic message: */
    if (J1939DCM_QUEUE_EMPTY != J1939Dcm_DM35Queue[lNodeHdl].RequestPending)
    {

      /* #12 Enter critical section (Reason: protect data consistency of the queue of events against request manager interruptions). */
      J1939Dcm_EnterCritical();

      /* #13 Copy the queue of events into a local parameter, clear the queue of events, and the channel bitmask. */
      lTmpQueueData.RequestedChannels  = J1939Dcm_DM35Queue[lNodeHdl].RequestedChannels;
      lTmpQueueData.StateChangeRequest = J1939Dcm_DM35Queue[lNodeHdl].StateChangeRequest;
      lTmpQueueData.RequestPending     = J1939Dcm_DM35Queue[lNodeHdl].RequestPending;

      J1939Dcm_QueueClearAll(J1939Dcm_DM35Queue[lNodeHdl]);
      J1939Dcm_DM35Queue[lNodeHdl].RequestedChannels = 0;

      /* #14 Leave critical section. */
      J1939Dcm_ExitCritical();

      /* handle queued TxConfirmation */
      /* #15 If a transmission of a diagnostic message is confirmed in the current task: */
      if (FALSE != J1939Dcm_QueueGetTxConf(lTmpQueueData))
      {
        /* #16 If a transmission of a diagnostic message was triggered in the previous task: */
        if (J1939DCM_MSG_STATE_TXCONF == J1939Dcm_DM35State[lNodeHdl].ProcessingState)
        {
          /* #17 Set Processing state to not busy because TxConfirmation for all transmitting channels were received. */
          J1939Dcm_DM35State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_WAIT;
        } /* else: TxConfirmation no longer awaited (e.g. because of change to offline); ignore */

        /* #18 Reset transmission because the TxHandler is still in use even if the confirmation is no longer expected. */
        J1939Dcm_TxHandlerReset();
      }

      /* handle queued communication state change */
      /* #19 If the communication state was changed: */
      if (FALSE != J1939Dcm_QueueGetStateChange(lTmpQueueData))
      {
        /* #20 If the communication state went from online to offline: */
        if (J1939DCM_STATE_OFFLINE == lTmpQueueData.StateChangeRequest)
        {
          /* #21 Release DEM resources. */
          J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM35State[lNodeHdl].HasDemAccess);
          /* #22 Set processing state to idle. */
          J1939Dcm_DM35State[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_IDLE;
          /* #23 Stop the periodic transmission and reset the one-second timer. */
          J1939Dcm_DM35State[lNodeHdl].ActiveChannels = 0;
          J1939Dcm_DM35State[lNodeHdl].Timer = J1939DCM_DM35_TIMEOUT;
        } /* else: offline -> online: has no effect, DM35 must be explicitly requested */
      }

      /* handle queued DM35 request */
      /* #24 If a new diagnostic message is requested: */
      if (FALSE != J1939Dcm_QueueGetMsgRequest(lTmpQueueData))
      {
        /* #25 Update the active channel mask. */
        J1939Dcm_DM35State[lNodeHdl].ActiveChannels |= lTmpQueueData.RequestedChannels;

        /* #26 If the processing state indicates idle: */
        if (J1939DCM_MSG_STATE_IDLE == J1939Dcm_DM35State[lNodeHdl].ProcessingState)
        {
          /* first channel to be activated; start DM35 state machine */
          /* #27 Set processing state to not busy. */
          J1939Dcm_DM35State[lNodeHdl].ProcessingState   = J1939DCM_MSG_STATE_WAIT;
        }
      }
    } /* J1939DCM_QUEUE_EMPTY */

    /* #28 If the processing state does not indicate idle: */
    if (J1939DCM_MSG_STATE_IDLE != J1939Dcm_DM35State[lNodeHdl].ProcessingState)
    {
      /* DM35 has been activated */

      /* --- update timers --- */
      /* #29 If the one-second period is not expired: */
      if (0 < J1939Dcm_DM35State[lNodeHdl].Timer)
      {
        /* #30 If DM13 is supported, then at least one channel has to be active. */
# if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
        if (0 != J1939Dcm_DM13State.NumActiveBroadcastChannels)   /* else: broadcast is disabled for all channels; there's no need to count */
# endif
        {
          /* #31 Decrement the one-second timer. */
          J1939Dcm_DM35State[lNodeHdl].Timer--;
        }
      }

      /* --- check for timeout --- */
      /* #32 If the one-second period timer expired: */
      if ( 0 == J1939Dcm_DM35State[lNodeHdl].Timer)
      {
        /* DET check: if previous DM35 message is not yet completed, it took longer than 1sec and we certainly have a problem */
        /* #33 Make sure that the processing state indicates not busy. */
        J1939Dcm_DetAssert( (J1939DCM_MSG_STATE_WAIT == J1939Dcm_DM35State[lNodeHdl].ProcessingState), J1939DCM_API_MAINFUNCTION, J1939DCM_E_OVERRUN)

        /* #34 If DEM resources are free: */
        if (FALSE == J1939Dcm_DemSemaphoreGetLocked())
        {
          /* #35 Trigger broadcast transmission and reload the one-second timer. */
          J1939Dcm_DM35State[lNodeHdl].ProcessingState  = J1939DCM_MSG_STATE_PREP_START;                                            /* SWS_J1939Dcm_00082 */
          J1939Dcm_DM35State[lNodeHdl].Timer = J1939DCM_DM35_TIMEOUT;
        }
        /* else: DEM is in use; postpone timer reload until DEM becomes available */
      }
      /* else: no timeout */

      /* --- handle message preparation and transmission --- */
      /* #36 If the processing state indicates busy:
 *                                           ( If the 1 second period expired in the current task
 *                                         Or  If the DM1 processing has not been completed in the previous task) */
      if (J1939DCM_MSG_STATE_WAIT < J1939Dcm_DM35State[lNodeHdl].ProcessingState)
      {
        /* 37 Trigger the processing of the diagnostic message. */
        J1939Dcm_DM35Processor(lNodeHdl);
      } /* end: state > J1939DCM_MSG_STATE_WAIT */

    } /* end: not J1939DCM_MSG_STATE_IDLE */
  } J1939Dcm_ForEachNode_End
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DM35_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_DM35Processor()
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
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DM35Processor( J1939Dcm_NodeHdlType Node )
{

  J1939Dcm_FilterHandlerType  lFilterHandler;
  /* --- start DM35 processing: filter DTCs --- */
  /* #10 If the processing state indicates message preparation start (a new request was received): */
  if (J1939DCM_MSG_STATE_PREP_START == J1939Dcm_DM35State[Node].ProcessingState)
  {
    /* #11 Get a lock on DEM resources in order to import the required data from DEM for the incoming DM request. */
    J1939Dcm_DemSemaphoreLock(&J1939Dcm_DM35State[Node].HasDemAccess);
    /* #12 If a lock on DEM resources is successfully obtained: */
    if (FALSE != J1939Dcm_DM35State[Node].HasDemAccess )
    {
      /* #13 Update the local DTC filter according to DM01 requirements. Trigger read Dtc Handler. */
      lFilterHandler.StatusFilter = DEM_J1939DTC_CURRENTLY_ACTIVE;
      lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
      lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      lFilterHandler.FiltredNode = Node;
      J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DM35State[Node].ProcessingState, &J1939Dcm_DM35State[Node].BufferInfo,
                                       &lFilterHandler);
    }
    /* #14 Otherwise: */
    else
    {
      /* #15 Skip processing because DEM is currently in use (set processing state to send acknowledgement with code: cannot respond). */
      J1939Dcm_DM35State[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
    }
  }

  /* --- finalize DM35 processing: get DTCs --- */
  /* #16 Get matching DTCs, if the processing state is set to get DTCs. */
  if (J1939DCM_MSG_STATE_PREP_DTCS == J1939Dcm_DM35State[Node].ProcessingState)
  {
    J1939Dcm_DtcHandlerGetDTCs(&J1939Dcm_DM35State[Node].ProcessingState, J1939DCM_DM_MAX_DTCS);
  }

  /* --- transmit prepared DM35 message --- */
  /* #17 If the processing state indicates that the DM is ready for transmission: */
  if (J1939DCM_MSG_STATE_TX == J1939Dcm_DM35State[Node].ProcessingState)
  {
    Std_ReturnType TxResult;
    J1939Dcm_ChannelHdlType lChannelHdl;

    /* #18 Release DEM semaphore. */
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM35State[Node].HasDemAccess);

    /* #19 Prepare for transmission.*/
    TxResult = J1939Dcm_TxHandlerPrepare(&J1939Dcm_DM35State[Node].BufferInfo, J1939DCM_DM35,
                                         Node, J1939DCM_ADDRESS_BROADCAST);

    /* #20 Check for buffer overflow. */
    J1939Dcm_DetAssert((J1939DCM_BUFFERSIZE_COMMON > J1939Dcm_DM35State[Node].BufferInfo.SduLength), J1939DCM_API_MAINFUNCTION, J1939DCM_E_PARAM_CONFIG)

    /* #21 If preparation for transmission was succeeded: */
    if (E_OK == TxResult)
    {
      boolean lAnyTxSucceeded;

      lAnyTxSucceeded = FALSE;

      /* transmission prepared; trigger transmit */
      /* #22 Set processing state to wait for TxConfirmation. */
      J1939Dcm_DM35State[Node].ProcessingState = J1939DCM_MSG_STATE_TXCONF;

      /* #23 Iterate over the available channels. */
      J1939Dcm_ForEachChannel_Start
      {
        J1939Dcm_ForEachChannel_Next;
        /* #24 If DM35 was requested on the incoming channel and the node-channel pair is online: */
        if ( (J1939Dcm_DM35ChannelIsActive(J1939Dcm_DM35State[Node].ActiveChannels, lChannelHdl)) &&
             (J1939Dcm_ComStateIsOnline(Node, lChannelHdl)) )
        {
          /* #25 If DM13 is supported, then at least one channel has to be active. */
# if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
          if (FALSE != J1939Dcm_DM13GetBroadcastStatusActive(lChannelHdl))
#endif
          {
            /* channel is online, broadcast is active and DM35 has been requested */
            /* #26 Trigger transmission and register the transmission results. */
            TxResult = J1939Dcm_TxHandlerTransmit(J1939Dcm_DM35State[Node].TxPdus[lChannelHdl], lChannelHdl);
            lAnyTxSucceeded |= (boolean)(TxResult == E_OK);                                                                     /* PRQA S 4105 */ /* MD_J1939Dcm_12.6 */
          }
        }
      } J1939Dcm_ForEachChannel_End
      
      /* #27 If the transmission over all the referenced channels by the requested node was failed:*/
      if (FALSE == lAnyTxSucceeded)
      {
        /* #28 Reset state machine. */
        J1939Dcm_DM35State[Node].ProcessingState = J1939DCM_MSG_STATE_WAIT;
        J1939Dcm_TxHandlerReset();
      }
    } /* else: transmit handler is busy; retry later */
  }

  /* --- transmit NACK --- */
  /* #29 If the processing state indicates sending acknowledgement: */
  if (J1939DCM_MSG_STATE_SENDACK == J1939Dcm_DM35State[Node].ProcessingState)
  {
    /* #30 Release DEM resources. */
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DM35State[Node].HasDemAccess);

    /* No need to send a NACK for because DM35 once requested, it is issued periodically. */
    /* NACK would be needed for a single response DM which is not the case for DM35. */
    /* #31 Set processing state to not busy (reset DM1 processor). */
    J1939Dcm_DM35State[Node].ProcessingState = J1939DCM_MSG_STATE_WAIT;
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DM35_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DMxTask()
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
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxTask( void )
{
  J1939Dcm_DMxQueueType lTmpQueueData;
  J1939Dcm_NodeHdlType    lNodeHdl;

  /* #10 Iterate over the defined nodes because the queue of events of a diagnostic message is node dependent. */
  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    /* --- Check queued events --- */
    /* #11 If any of the interrupt service routine functions updated the queue of events of a diagnostic message: */
    if (J1939DCM_QUEUE_EMPTY != J1939Dcm_DMxQueue[lNodeHdl].RequestPending)
    {

      /* #12 Enter critical section (Reason: protect data consistency of the queue of events against request manager interruptions). */
      J1939Dcm_EnterCritical();

      /* #13 Copy the queue of events into a local parameter and clear the queue of events. */
      lTmpQueueData.ChannelHdl         = J1939Dcm_DMxQueue[lNodeHdl].ChannelHdl;
      lTmpQueueData.MessageHdl         = J1939Dcm_DMxQueue[lNodeHdl].MessageHdl;
      lTmpQueueData.ResponseDA         = J1939Dcm_DMxQueue[lNodeHdl].ResponseDA;
      lTmpQueueData.StateChangeRequest = J1939Dcm_DMxQueue[lNodeHdl].StateChangeRequest;
      lTmpQueueData.RequestPending     = J1939Dcm_DMxQueue[lNodeHdl].RequestPending;

      J1939Dcm_QueueClearAll(J1939Dcm_DMxQueue[lNodeHdl]);

      /* #14 Leave critical section. */
      J1939Dcm_ExitCritical();

      /* Handle queued DM request. */

      /* #15 If a new diagnostic message is requested: */
      if (FALSE != J1939Dcm_QueueGetMsgRequest(lTmpQueueData))
      {
        /* #16 Prepare for message processing by updating the processing state to start message preparation and copying the queue of events into the state of the diagnostic message. */
        J1939Dcm_DMxState[lNodeHdl].ProcessingState   = J1939DCM_MSG_STATE_PREP_START;
        J1939Dcm_DMxState[lNodeHdl].ChannelHdl        = lTmpQueueData.ChannelHdl;
        J1939Dcm_DMxState[lNodeHdl].MessageHdl        = lTmpQueueData.MessageHdl;
        J1939Dcm_DMxState[lNodeHdl].ResponseDA        = lTmpQueueData.ResponseDA;
      }

      /* #17 If a transmission of a diagnostic message is confirmed in the current task: */
      if (FALSE != J1939Dcm_QueueGetTxConf(lTmpQueueData))
      {
        /* #18 If a transmission of a diagnostic message was triggered in the previous task: */
        if (J1939DCM_MSG_STATE_TXCONF == J1939Dcm_DMxState[lNodeHdl].ProcessingState)
        {
         /* #19 Set Processing state to not busy because TxConfirmation for all transmitting channels were received. */
          J1939Dcm_DMxState[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_WAIT;
        } /* else: TxConfirmation no longer awaited (e.g. because of change to offline); ignore */

        /* #20 Reset transmission because the TxHandler is still in use even if the confirmation is no longer expected. */
        J1939Dcm_TxHandlerReset();
      }

      /* #21 If a transmission of a diagnostic message is failed in the current task: */
      if (FALSE != J1939Dcm_QueueGetTxFail(lTmpQueueData))
      {
        /* #22 If a transmission of a diagnostic message was triggered in the previous task: */
        if (J1939DCM_MSG_STATE_TXCONF == J1939Dcm_DMxState[lNodeHdl].ProcessingState)
        {
          /* #23 Set the processing state to send acknowledgement because TxConfiramtion was failed and thus NACK is required. */
          J1939Dcm_DMxState[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
        } /* else: TxConfirmation no longer awaited (e.g. because of change to offline); ignore */

        /* #24 Reset transmission because the TxHandler is still in use even if the confirmation is no longer expected. */
        J1939Dcm_TxHandlerReset();
      }

      /* #25 If the communication state was changed: */
      if (FALSE != J1939Dcm_QueueGetStateChange(lTmpQueueData))
      {
        /* #26 If the communication state went from offline to online: */
        if (J1939DCM_STATE_ONLINE == lTmpQueueData.StateChangeRequest)
        {
          /* #27 If processing state indicates idle: */
          if (J1939DCM_MSG_STATE_IDLE == J1939Dcm_DMxState[lNodeHdl].ProcessingState)
          {
            /* #28 Set processing state to not busy. */
            J1939Dcm_DMxState[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_WAIT;
          } /* else: node is already online; do not change internal state */
        }
        /* #29 Otherwise (If the communication state went from online to offline): */
        else
        {
          /* #30 Release DEM resources. */
          J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DMxState[lNodeHdl].HasDemAccess);
          /* #31 Set processing state to idle. */
          J1939Dcm_DMxState[lNodeHdl].ProcessingState = J1939DCM_MSG_STATE_IDLE;
        }
      }
    }

    /* #32 If processing state indicates busy: (If a new DM is requested in the current task
                                            Or If a DM processing has not been completed in the previous task
                                            Or If a DM transmission was failed in the current task) */
    if (J1939DCM_MSG_STATE_WAIT < J1939Dcm_DMxState[lNodeHdl].ProcessingState)
    {
      /* #33 Trigger the processing of the diagnostic message. */
      J1939Dcm_DMxProcessor(lNodeHdl);
    } /* end: state > J1939DCM_MSG_STATE_WAIT */
  } J1939Dcm_ForEachNode_End
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_NEED_DMX_PROCESSOR == STD_ON) */

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DMxProcessor()
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
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DMxProcessor( J1939Dcm_NodeHdlType Node )
{

  uint8_least     lDiagnosticMessage;
  J1939Rm_AckCode lAcknowledge;
#if ( (J1939DCM_DM2_SUPPORT == STD_ON) || (J1939DCM_DM27_SUPPORT == STD_ON) || (J1939DCM_DM53_SUPPORT == STD_ON) || (J1939DCM_DM54_SUPPORT == STD_ON))
  J1939Dcm_FilterHandlerType  lFilterHandler;
#endif

  lDiagnosticMessage = J1939Dcm_Cfg_GetMessageDM(J1939Dcm_DMxState[Node].MessageHdl);

  /* Default acknowledge code in case anything fails. */
  lAcknowledge = J1939RM_ACK_NEGATIVE;

  /* --- Start DM processing --- */
  /* #10 If the processing state indicates message preparation start (a new request was received): */
  if (J1939DCM_MSG_STATE_PREP_START == J1939Dcm_DMxState[Node].ProcessingState)
  {
    /* #11 Get a lock on DEM resources in order to import the required data from DEM for the incoming DM request. */
    J1939Dcm_DemSemaphoreLock(&J1939Dcm_DMxState[Node].HasDemAccess);
    /* #12 If a lock on DEM resources is successfully obtained: */
    if (FALSE != J1939Dcm_DMxState[Node].HasDemAccess )
    {
      /* #13 Process the requested DM:
              In case of DM02: Update the local DTC filter according to DM02 requirements. Trigger read Dtc Handler.
              In case of DM03: Update the state of the clear handler according to DM03 requirements. Set the processing state to clear DTCs.
              In case of DM04: Trigger freeze frame handler.
              In case of DM05: Trigger readiness 1 handler.
              In case of DM11: Update the state of the clear handler according to DM11 requirements. Set the processing state to clear DTCs.
              In case of DM24: Trigger SPN handler. Set the processing state to trigger transmission of message.
              In case of DM25: Trigger freeze frame handler.
              In case of DM27: Update local DTC filter according to DM27 requirements. Trigger read Dtc Handler.
              In case of DM31: Trigger status lamp handler. Set the processing state to get DTC with lamp status.
              In case of DM53: Update local DTC filter according to DM53 requirements. Trigger read Dtc Handler.
              In case of DM54: Update local DTC filter according to DM54 requirements. Trigger read Dtc Handler.
              In case of DM55: Update the state of the clear handler according to DM55 requirements. Set the processing state to clear DTCs.
              Otherwise: Set the processing state to send acknowledgement according to J1939-21. */

      switch (lDiagnosticMessage)
      {
# if (J1939DCM_DM2_SUPPORT == STD_ON)
        case J1939DCM_DM2:
          lFilterHandler.StatusFilter = DEM_J1939DTC_PREVIOUSLY_ACTIVE;
          lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
          lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
          lFilterHandler.FiltredNode = Node;
          J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                          &lFilterHandler);
          break;
# endif

# if (J1939DCM_DM3_SUPPORT == STD_ON)
        case J1939DCM_DM3:
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_CLEAR;
          J1939Dcm_ClearHandlerState.ClearFilter = DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE;
          J1939Dcm_ClearHandlerState.ClearNode = J1939Dcm_Cfg_GetNodeId(Node);
          J1939Dcm_ClearHandlerState.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
          break;
# endif

# if (J1939DCM_DM4_SUPPORT == STD_ON)
        case J1939DCM_DM4:
          J1939Dcm_DtcHandlerSetFFFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                        DEM_J1939DCM_FREEZEFRAME, Node);
          break;
# endif

# if (J1939DCM_DM5_SUPPORT == STD_ON)
        case J1939DCM_DM5:
          J1939Dcm_DtcHandlerGetReadiness(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo, J1939DCM_DM5, Node);
          break;
# endif

# if (J1939DCM_DM11_SUPPORT == STD_ON)
        case J1939DCM_DM11:
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_CLEAR;
          J1939Dcm_ClearHandlerState.ClearFilter = DEM_J1939DTC_CLEAR_ALL;
          J1939Dcm_ClearHandlerState.ClearNode = J1939Dcm_Cfg_GetNodeId(Node);
          J1939Dcm_ClearHandlerState.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
          break;
# endif

# if (J1939DCM_DM24_SUPPORT == STD_ON)
        case J1939DCM_DM24:
          J1939Dcm_DtcHandlerGetSupportedSPNs(Node);
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_TX;
          break;
# endif

# if (J1939DCM_DM25_SUPPORT == STD_ON)
        case J1939DCM_DM25:
          J1939Dcm_DtcHandlerSetFFFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                        DEM_J1939DCM_EXPANDED_FREEZEFRAME, Node);
          break;
# endif

# if (J1939DCM_DM27_SUPPORT == STD_ON)
        case J1939DCM_DM27:
          lFilterHandler.StatusFilter = DEM_J1939DTC_PENDING;
          lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
          lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
          lFilterHandler.FiltredNode = Node;
          J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                          &lFilterHandler);
          break;
# endif

# if (J1939DCM_DM31_SUPPORT == STD_ON)
        case J1939DCM_DM31:
          J1939Dcm_DtcHandlerSetLampFilter(&J1939Dcm_DMxState[Node].BufferInfo, Node);
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_PREP_LAMPDTC;
          break;
# endif

# if (J1939DCM_DM53_SUPPORT == STD_ON)
        case J1939DCM_DM53:
          lFilterHandler.StatusFilter = DEM_J1939DTC_ACTIVE;
          lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
          lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_SECONDARY_MEMORY;
          lFilterHandler.FiltredNode = Node;
          J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                          &lFilterHandler);
          break;
# endif

# if (J1939DCM_DM54_SUPPORT == STD_ON)
        case J1939DCM_DM54:
          lFilterHandler.StatusFilter = DEM_J1939DTC_PREVIOUSLY_ACTIVE;
          lFilterHandler.DTCKind = DEM_DTC_KIND_ALL_DTCS;
          lFilterHandler.DTCOrigin = DEM_DTC_ORIGIN_SECONDARY_MEMORY;
          lFilterHandler.FiltredNode = Node;
          J1939Dcm_DtcHandlerSetDtcFilter(&J1939Dcm_DMxState[Node].ProcessingState, &J1939Dcm_DMxState[Node].BufferInfo,
                                          &lFilterHandler);
          break;
# endif

# if (J1939DCM_DM55_SUPPORT == STD_ON)
        case J1939DCM_DM55:
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_CLEAR;
          J1939Dcm_ClearHandlerState.ClearFilter = DEM_J1939DTC_CLEAR_ALL_AND_PREVIOUSLY_ACTIVE;
          J1939Dcm_ClearHandlerState.ClearNode = J1939Dcm_Cfg_GetNodeId(Node);
          J1939Dcm_ClearHandlerState.DTCOrigin = DEM_DTC_ORIGIN_SECONDARY_MEMORY;
          break;
# endif

        default:
          /* [J1939-21] A response is always required from the specified destination(not global) even if it is a NACK indicating that the particular
           *            PGN value is not supported. */
          J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
          break;
      }
    }
    /* #14 Otherwise: */
    else
    {
      /* #15 Skip processing because DEM is currently in use (set processing state to send acknowledgement with code: cannot respond). */
      J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
      lAcknowledge = J1939RM_ACK_CANNOT_RESPOND;
    }
  }
  /* #16 If DM29 is supported: */
# if (J1939DCM_NEED_DTC_HANDLER_NUM == STD_ON)
  /* #17 Get number of matching DTCs, if the processing state is set to get number of DTCs. */
  if (J1939DCM_MSG_STATE_PREP_NUMBER == J1939Dcm_DMxState[Node].ProcessingState)
  {
    J1939Dcm_DtcHandlerGetNumber(&J1939Dcm_DMxState[Node].ProcessingState);
  }
# endif

  /* #18 If supported any DM that needs SPN: */
# if (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON)
  /* #19 Get matching DTCs, if the processing state is set to get DTCs. */
  if (J1939DCM_MSG_STATE_PREP_DTCS == J1939Dcm_DMxState[Node].ProcessingState)
  {
    /* Start adding the DTCs to the message buffer. */
    J1939Dcm_DtcHandlerGetDTCs(&J1939Dcm_DMxState[Node].ProcessingState, J1939DCM_DM_MAX_DTCS);
  }
# endif

  /* #20 If DM31 is supported: */
# if (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON)
  /* #21 Get DTCs with lamp status, if the processing state is set to get lamp status. */
  if (J1939DCM_MSG_STATE_PREP_LAMPDTC == J1939Dcm_DMxState[Node].ProcessingState)
  {
    /* Start adding lamp status in the message buffer. */
    J1939Dcm_DtcHandlerGetLampDTCs(&J1939Dcm_DMxState[Node].ProcessingState);
  }
# endif

  /* #22 If supported any DM that needs freeze frame: */
# if (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON)
  /* #23 Get freeze frames, if the processing state is set to get freeze frames. */
  if (J1939DCM_MSG_STATE_PREP_FF == J1939Dcm_DMxState[Node].ProcessingState)
  {
    /* Start adding the freeze frames in the message buffer. */
    J1939Dcm_DtcHandlerGetFFs(&J1939Dcm_DMxState[Node].ProcessingState);
  }
# endif

  /* #24 If supported any DM that needs clear handler: */
# if (J1939DCM_NEED_CLEAR_HANDLER == STD_ON)
  /* #25 Clear DTCs and store locally the clear acknowledgement code, if the processing state is set to clear DTCs. */
  if (J1939DCM_MSG_STATE_CLEAR == J1939Dcm_DMxState[Node].ProcessingState)
  {
    lAcknowledge = J1939Dcm_ClearHandler(&J1939Dcm_DMxState[Node].ProcessingState);
  }
# endif

  /* #26 If the processing state indicates that the DM is ready for transmission: */
  if (J1939DCM_MSG_STATE_TX == J1939Dcm_DMxState[Node].ProcessingState)
  {
    Std_ReturnType TxResult;

    /* #27 Release DEM semaphore. */
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DMxState[Node].HasDemAccess);

    /* #28 Prepare for transmission. */
    TxResult = J1939Dcm_TxHandlerPrepare(&J1939Dcm_DMxState[Node].BufferInfo, lDiagnosticMessage,
                                         Node, J1939Dcm_DMxState[Node].ResponseDA);

    /* #29 Check for buffer overflow. */
    J1939Dcm_DetAssert((J1939DCM_BUFFERSIZE_COMMON > J1939Dcm_DMxState[Node].BufferInfo.SduLength), J1939DCM_API_MAINFUNCTION, J1939DCM_E_PARAM_CONFIG)

    /* #30 If preparation for transmission was succeeded: */
    if (E_OK == TxResult)
    {
      /* #31 Set processing state to wait for TxConfirmation. */
      J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_TXCONF;
      /* #32 Trigger transmission. */
      TxResult = J1939Dcm_TxHandlerTransmit(J1939Dcm_Cfg_GetMessageTxPdu(Node, J1939Dcm_DMxState[Node].MessageHdl),
                                            J1939Dcm_DMxState[Node].ChannelHdl);
      /* #33 If triggering for transmission is failed: */
      if (E_OK != TxResult)
      {
        /* #34 Cancel transmission by resetting Txhandler. Update processing state to send acknowledgement. */
        J1939Dcm_TxHandlerReset();
        J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_SENDACK;
      }
    } /* else: Transmit handler is busy; retry later */
  }

  /* #35 If the processing state indicates sending acknowledgement: */
  if (J1939DCM_MSG_STATE_SENDACK == J1939Dcm_DMxState[Node].ProcessingState)
  {
    /* #36 Release DEM resources. */
    J1939Dcm_DemSemaphoreRelease(&J1939Dcm_DMxState[Node].HasDemAccess);

    /* #37 If the source address of the requested message is specific: */
    if (J1939DCM_ADDRESS_BROADCAST != J1939Dcm_DMxState[Node].ResponseDA)
    {
      /* #38 Request sending acknowledgement from the request manager. */
      (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(Node),
                            J1939Dcm_Cfg_GetChannelId(J1939Dcm_DMxState[Node].ChannelHdl),
                            J1939Dcm_Cfg_GetMessagePGN(J1939Dcm_DMxState[Node].MessageHdl),
                            NULL_PTR,
                            lAcknowledge,
                            J1939Dcm_DMxState[Node].ResponseDA,
                            J1939DCM_DM_PRIORITY_DEFAULT,
                            FALSE);
    }/* else: Skip NACK */

    /* #39 Reset DM processor. */
    J1939Dcm_DMxState[Node].ProcessingState = J1939DCM_MSG_STATE_WAIT;

  } /* End J1939DCM_MSG_STATE_SENDACK */
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (J1939DCM_NEED_DMX_PROCESSOR == STD_ON) */

/**********************************************************************************************************************
 * J1939Dcm_TxHandlerTask
 **********************************************************************************************************************/
/*! \brief      timeout supervision for Tx connections
 *  \pre        Module must be initialized
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_TxHandlerTask( void )
{
  J1939Dcm_NodeHdlType lNodeHdl;
  uint8_least          lDiagMsg;

  lDiagMsg = J1939DCM_DM_UNDEF;
  lNodeHdl = J1939DCM_INVALID_NODE_HDL;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  J1939Dcm_EnterCritical();

  if (0 != J1939Dcm_TxHandlerState.TxTimer)
  {
    J1939Dcm_TxHandlerState.TxTimer--;

    if (0 == J1939Dcm_TxHandlerState.TxTimer)
    {
      lNodeHdl = J1939Dcm_TxHandlerState.NodeHdl;
      lDiagMsg = J1939Dcm_TxHandlerState.DM;
    }
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  J1939Dcm_ExitCritical();

  /* confirm diagnostic message which started transmission; this will also reset the TxHandler */
  if (J1939DCM_DM_UNDEF != lDiagMsg)
  {
    J1939Dcm_TxHandlerConfirm(lNodeHdl, lDiagMsg, E_NOT_OK);
  }
}

#if (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerSetFilter
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: setup read sequence, set filter and get lamp status
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \param[in]    pMsgBuffer          pointer to DM message buffer
 *  \param[in]    FilterCriteria      filter criteria parameter for DEM
 *  \param[in]    DTCKind             DTCKind parameter for DEM
 *  \param[in]    NodeHdl             node for which the filter need to be set
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetDtcFilter( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                         P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                         P2CONST(J1939Dcm_FilterHandlerType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pFilterHandler )
{
  Dem_ReturnSetFilterType lFilterReturn;
  uint16                  lLampStatus;

  /* setup DEM DTC interface */
  J1939Dcm_DtcHandlerState.MsgData            = pBufferData;
  J1939Dcm_DtcHandlerState.NumDTCsInCycle     = 0;
  J1939Dcm_DtcHandlerState.NumDTCs            = 0;
  J1939Dcm_DtcHandlerState.MsgData->SduLength = 0;

  /* set filter */
  lFilterReturn = Dem_J1939DcmSetDTCFilter(pFilterHandler->StatusFilter, pFilterHandler->DTCKind, pFilterHandler->DTCOrigin,
                                           J1939Dcm_Cfg_GetNodeId(pFilterHandler->FiltredNode), &lLampStatus);

  if (DEM_FILTER_ACCEPTED == lFilterReturn)
  {
    J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti16BitGetHiByte(lLampStatus);
    J1939Dcm_DtcHandlerState.MsgData->SduLength++;
    J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti16BitGetLoByte(lLampStatus);
    J1939Dcm_DtcHandlerState.MsgData->SduLength++;
    *pDMProcessorState = J1939DCM_MSG_STATE_PREP_DTCS;
  }
  else
  {
    /* filtering failed; reset DM processor */
    *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
  }
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerSetLampFilter
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: setup read sequence for DM31
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \param[in]    pMsgBuffer          pointer to DM message buffer
 *  \param[in]    NodeHdl             node for which the filter need to be set
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetLampFilter( P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                           J1939Dcm_NodeHdlType NodeHdl )
{
  J1939Dcm_IgnoreUnusedArgument(NodeHdl)         /* parameter not used in single node configurations */     /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* setup DEM DTC interface */
  J1939Dcm_DtcHandlerState.MsgData            = pBufferData;
  J1939Dcm_DtcHandlerState.MsgData->SduLength = 0;
  J1939Dcm_DtcHandlerState.NumDTCsInCycle     = 0;
  J1939Dcm_DtcHandlerState.NumDTCs            = 0;

  /* start sequence in DEM */
  Dem_J1939DcmFirstDTCwithLampStatus(J1939Dcm_Cfg_GetNodeId(NodeHdl));
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerSetFilter
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: setup read sequence, set filter and get lamp status
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \param[in]    pMsgBuffer          pointer to DM message buffer
 *  \param[in]    FilterCriteria      filter criteria parameter for DEM
 *  \param[in]    DTCKind             DTCKind parameter for DEM
 *  \param[in]    NodeHdl             node for which the filter need to be set
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerSetFFFilter( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                         P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                         Dem_J1939DcmSetFreezeFrameFilterType FFKind,
                                                                         J1939Dcm_NodeHdlType NodeHdl )
{
  Dem_ReturnSetFilterType lFilterReturn;

  J1939Dcm_IgnoreUnusedArgument(NodeHdl)         /* parameter not used in single node configurations */    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* setup DEM DTC interface */
  J1939Dcm_DtcHandlerState.MsgData            = pBufferData;
  J1939Dcm_DtcHandlerState.NumDTCsInCycle     = 0;
  J1939Dcm_DtcHandlerState.NumDTCs            = 0;
  J1939Dcm_DtcHandlerState.MsgData->SduLength = 0;

  /* set filter */
  lFilterReturn = Dem_J1939DcmSetFreezeFrameFilter(FFKind, J1939Dcm_Cfg_GetNodeId(NodeHdl));

  if (DEM_FILTER_ACCEPTED == lFilterReturn)
  {
    *pDMProcessorState = J1939DCM_MSG_STATE_PREP_FF;
  }
  else
  {
    /* filtering failed; reset DM processor */
    *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
  }
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_NUM == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetNumber
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: get number of filtered DTCs
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetNumber( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState )
{
  Dem_ReturnGetNumberOfFilteredDTCType lGetNumberReturn;
  uint16                               lNumDTCs;
  lGetNumberReturn = Dem_J1939DcmGetNumberOfFilteredDTC(&lNumDTCs);

  if (DEM_NUMBER_OK == lGetNumberReturn)
  {
    /* not yet implemented */
    J1939Dcm_DetAssertAlways(J1939DCM_API_MAINFUNCTION, J1939DCM_E_INVALID_STATE)
  }
  else
  {
    if (DEM_NUMBER_PENDING != lGetNumberReturn)
    {
      *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
    }
    /* else: pending; do not change state so we'll retry in next task cycle */
  }
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_NUM == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetDTCs
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: get DTCs and add them to message buffer
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \param[in]    MaxNumDtcs          number of DTCs after which the message shall be transmitted
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetDTCs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState, uint16_least MaxNumDtcs )
{
  Dem_ReturnGetNextFilteredElementType lGetDtcResult;
  uint32                               lDTCNumber;
  uint8                                lOccurrenceCounter;

  do
  {
    lGetDtcResult = Dem_J1939DcmGetNextFilteredDTC(&lDTCNumber, &lOccurrenceCounter);

    switch (lGetDtcResult)
    {
      case DEM_FILTERED_OK:
        /* add data to buffer */
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetHiByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetMidByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetLoByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = lOccurrenceCounter;
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;

        J1939Dcm_DtcHandlerState.NumDTCsInCycle++;
        J1939Dcm_DtcHandlerState.NumDTCs++;

        if (J1939Dcm_DtcHandlerState.NumDTCs < MaxNumDtcs)
        {
          if (J1939Dcm_DtcHandlerState.NumDTCsInCycle >= J1939DCM_MAX_DTCS_PER_CYCLE)
          {
            /* max number in this cycle reached; do not change DM processor state and continue on next task cycle */
            lGetDtcResult = DEM_FILTERED_PENDING;
            J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
          }
        }
        else
        {
          /* max number of DTCs reached; transmit message */
          *pDMProcessorState = J1939DCM_MSG_STATE_TX;
          lGetDtcResult = DEM_FILTERED_NO_MATCHING_ELEMENT;
        }
        break;

      case DEM_FILTERED_PENDING:
        /* do not change state and retry on next task cycle */
        J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
        break;

      case DEM_FILTERED_NO_MATCHING_ELEMENT:
        if (0 == J1939Dcm_DtcHandlerState.NumDTCs)
        {
          /* no matching DTCs; send default message */
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00;  /* SPN byte 1 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00;  /* SPN byte 2 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00;  /* SPN byte 3 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00;  /* Occurrence Counter */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        }

        /* message finished; continue with transmission */
        *pDMProcessorState = J1939DCM_MSG_STATE_TX;
        break;

      default:
        /* something's wrong! */
        *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
        break;
    }
  } while (DEM_FILTERED_OK == lGetDtcResult);
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_SPN == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetLampDTCs
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: get DTCs with lamp status and add them to message buffer
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetLampDTCs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState )
{
  Dem_ReturnGetNextFilteredElementType lGetDtcResult;
  uint32                               lDTCNumber;
  uint8                                lOccurrenceCounter;
  uint16                               lLampStatus;

  do
  {
    lGetDtcResult = Dem_J1939DcmGetNextDTCwithLampStatus(&lLampStatus, &lDTCNumber, &lOccurrenceCounter);

    switch (lGetDtcResult)
    {
      case DEM_FILTERED_OK:
        /* add data to buffer */
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetHiByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetMidByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti24BitGetLoByte(lDTCNumber);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = lOccurrenceCounter;
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti16BitGetHiByte(lLampStatus);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = J1939Dcm_Uti16BitGetLoByte(lLampStatus);
        J1939Dcm_DtcHandlerState.MsgData->SduLength++;

        J1939Dcm_DtcHandlerState.NumDTCsInCycle++;
        J1939Dcm_DtcHandlerState.NumDTCs++;

        if (J1939Dcm_DtcHandlerState.NumDTCsInCycle >= J1939DCM_MAX_DTCS_PER_CYCLE)
        {
          /* max number in this cycle reached; do not change DM processor state and continue on next task cycle */
          lGetDtcResult = DEM_FILTERED_PENDING;
          J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
        }
        break;

      case DEM_FILTERED_PENDING:
        /* do not change state and retry on next task cycle */
        J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
        break;

      case DEM_FILTERED_NO_MATCHING_ELEMENT:
        /* all DTCs have been read */
        if (0 == J1939Dcm_DtcHandlerState.NumDTCs)
        {
          /* no DTCs reported; add default values */
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 1 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 2 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 3 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* Occurrence Counter */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* lamp status byte 1 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0xFFu; /* lamp status byte 2 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        }

        if (1 >= J1939Dcm_DtcHandlerState.NumDTCs)
        {
          /* one or no DTC, fill up last bytes and start transmission immediately */
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0xFFu;
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0xFFu;
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        }

        /* finished; continue with transmission */
        *pDMProcessorState = J1939DCM_MSG_STATE_TX;
        break;

      default:
        /* something's wrong! */
        *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
        break;
    }
  } while (DEM_FILTERED_OK == lGetDtcResult);
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON) */

#if (J1939DCM_DM24_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetSupportedSPNs
 **********************************************************************************************************************/
/*! \brief        J1939 Freeze Frame Handler: get list of supported SPNs from configuration
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetSupportedSPNs( J1939Dcm_NodeHdlType NodeHdl )
{
  PduLengthType lByteCounter;

  lByteCounter = J1939Dcm_DM24GetLength(NodeHdl);
  J1939Dcm_DMxState[NodeHdl].BufferInfo.SduLength = lByteCounter;

  do
  {
    lByteCounter--;
    J1939Dcm_DMxState[NodeHdl].BufferInfo.SduDataPtr[lByteCounter] = J1939Dcm_GetCfg_SPNArray(lByteCounter + J1939Dcm_GetCfg_SPNArrayStartIdxOfCfg_NodeTable(NodeHdl));
  } while (lByteCounter > 0);

}
#endif /* (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetFFs
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: get Freeze Frames of one DTC and add it to message buffer
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetFFs( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState )
{
  Dem_ReturnGetNextFilteredElementType lGetFFResult;
  uint32                               lDTCNumber;
  uint8                                *lFreezeFramePtr;
  PduLengthType                        lFreezeFrameBufferLength;
  uint8                                lFreezeFrameDataLength;

  lDTCNumber = 0;
  lFreezeFrameDataLength = 0;
  do
  {
    lFreezeFramePtr = &J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength];           /* start of freeze frame data set */
    lFreezeFrameBufferLength = (PduLengthType)J1939DCM_BUFFERSIZE_COMMON - J1939Dcm_DtcHandlerState.MsgData->SduLength;   /* remaining buffer length */

#if (J1939DCM_BUFFERSIZE_COMMON > J1939DCM_FF_MAX_DATA_LENGTH)
    /* length of one single freeze frame limited to uint8 by SAE / AR DEM Spec */
    /* furthermore,  4 bytes for freeze frame header have to be added, so the  */
    /* DEM is not allowed to write the full 255 byte of data                   */
    if (lFreezeFrameBufferLength > J1939DCM_FF_MAX_DATA_LENGTH)
    {
      /* limit buffer size to DEM to max possible data length */
      lFreezeFrameDataLength = J1939DCM_FF_MAX_DATA_LENGTH;
      lGetFFResult = DEM_FILTERED_OK;
    }
    else
#endif
    if (lFreezeFrameBufferLength > J1939DCM_FF_HEADER_LENGTH)
    {
      /* calculate allowed data length for DEM, taking into account that also length and SPN need to be added */
      lFreezeFrameDataLength = (uint8)(lFreezeFrameBufferLength - J1939DCM_FF_HEADER_LENGTH);
      lGetFFResult = DEM_FILTERED_OK;
    }
    else
    {
      /* insufficient buffer to store the header itself! */
      lGetFFResult = DEM_FILTERED_BUFFER_TOO_SMALL;
    }

    if (DEM_FILTERED_OK == lGetFFResult)
    {
      lGetFFResult = Dem_J1939DcmGetNextFreezeFrame(&lDTCNumber,
                                                    &lFreezeFramePtr[J1939DCM_FF_OCC_OFFSET],
                                                    &lFreezeFramePtr[J1939DCM_FF_DATA_OFFSET],
                                                    &lFreezeFrameDataLength);
    }

    switch (lGetFFResult)
    {
      case DEM_FILTERED_OK:
        /* data have been added to buffer; add DTC number and length */
        lFreezeFramePtr[J1939DCM_FF_DTC_OFFSET]     = J1939Dcm_Uti24BitGetHiByte(lDTCNumber);                                                        /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */
        lFreezeFramePtr[J1939DCM_FF_DTC_OFFSET + 1] = J1939Dcm_Uti24BitGetMidByte(lDTCNumber);                                                       /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */
        lFreezeFramePtr[J1939DCM_FF_DTC_OFFSET + 2] = J1939Dcm_Uti24BitGetLoByte(lDTCNumber);                                                        /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */
        lFreezeFramePtr[J1939DCM_FF_LENGTH_OFFSET]  = J1939DCM_FF_SPN_OCC_LENGTH + lFreezeFrameDataLength;   /* SPN + Occ + Data */                  /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */

        J1939Dcm_DtcHandlerState.MsgData->SduLength += (J1939DCM_FF_HEADER_LENGTH + lFreezeFrameDataLength); /* Length + SPN + Occ + Data */         /* PRQA S 3353 */ /* MD_J1939Dcm_9.1 */

        J1939Dcm_DtcHandlerState.NumDTCsInCycle++;
        J1939Dcm_DtcHandlerState.NumDTCs++;

        if (J1939Dcm_DtcHandlerState.NumDTCsInCycle >= J1939DCM_MAX_FF_PER_CYCLE)
        {
          /* max number in this cycle reached; do not change DM processor state and continue on next task cycle */
          lGetFFResult = DEM_FILTERED_PENDING;
          J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
        }
        break;

      case DEM_FILTERED_PENDING:
        /* do not change state and retry on next task cycle */
        J1939Dcm_DtcHandlerState.NumDTCsInCycle = 0;
        break;

      case DEM_FILTERED_NO_MATCHING_ELEMENT:
        /* finished; continue with transmission */

        if (0 == J1939Dcm_DtcHandlerState.NumDTCs)
        {
          /* no FF has been read; add default message */
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* length */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 1 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 2 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* SPN byte 3 */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
          J1939Dcm_DtcHandlerState.MsgData->SduDataPtr[J1939Dcm_DtcHandlerState.MsgData->SduLength] = 0x00u; /* Occurrence Counter */
          J1939Dcm_DtcHandlerState.MsgData->SduLength++;
        }

        /* message finished; continue with transmission */
        *pDMProcessorState = J1939DCM_MSG_STATE_TX;
        break;

      default:
        /* something's wrong! Maybe buffer is too small? */
        J1939Dcm_DetAssertAlways(J1939DCM_API_MAINFUNCTION, J1939DCM_E_INVALID_STATE)
        *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
        break;
    }
  } while (DEM_FILTERED_OK == lGetFFResult);
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_FF == STD_ON) */

#if (J1939DCM_NEED_DTC_HANDLER_OBD == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_DtcHandlerGetReadiness
 **********************************************************************************************************************/
/*! \brief        J1939 Dtc Handler: get OBD monitor readiness
 *  \param[out]   pDMProcessorState   pointer to state of DM processor
 *  \context      Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_DtcHandlerGetReadiness( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState,
                                                                          P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pBufferData,
                                                                          uint8 DiagMsg,
                                                                          J1939Dcm_NodeHdlType NodeHdl)
{
  Std_ReturnType                               lReadinessReturn;
  Dem_J1939DcmDiagnosticReadiness1Type         lReadinessData;
  /* currently only DM5 is supported; other readiness messages, which will then be read from DEM, may be added in the future */
  J1939Dcm_IgnoreUnusedArgument(DiagMsg)                            /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* Get the count of the active and peviously active DTCs*/
  lReadinessReturn = Dem_J1939DcmReadDiagnosticReadiness1(&lReadinessData, J1939Dcm_Cfg_GetNodeId(NodeHdl));

	if (E_OK == lReadinessReturn)
	{
    /* build default DM5 for non-OBD systems */
    pBufferData->SduLength = 0;
    pBufferData->SduDataPtr[pBufferData->SduLength] = lReadinessData.ActiveTroubleCodes;  /* active DTCs -> no OBD DTCs available */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = lReadinessData.PreviouslyActiveDiagnosticTroubleCodes;  /* previously active DTCs -> no OBD DTCs available */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 5;  /* OBD compliance; 5 = no OBD */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 0;  /* Continuously Monitored Systems Support/Status; 0 = not supported */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 0;  /* Non-continuously Monitored Systems Support; 0 = not supported */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 0;  /* Non-continuously Monitored Systems Support; 0 = not supported */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 0;  /* Non-continuously Monitored Systems Status; 0 = not supported  */
    pBufferData->SduLength++;
    pBufferData->SduDataPtr[pBufferData->SduLength] = 0;  /* Non-continuously Monitored Systems Status; 0 = not supported  */
    pBufferData->SduLength++;

    *pDMProcessorState = J1939DCM_MSG_STATE_TX;
	}

	else
  {
    /* retrieving readiness data failed; reset DM processor */
    *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
  }
}
#endif /* (J1939DCM_NEED_DTC_HANDLER_OBD == STD_ON) */

#if (J1939DCM_NEED_CLEAR_HANDLER == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_ClearHandler()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(J1939Rm_AckCode, J1939DCM_CODE) J1939Dcm_ClearHandler( P2VAR(uint8, AUTOMATIC, J1939DCM_INTERNAL_DATA) pDMProcessorState)
{
  Dem_ReturnClearDTCType ClearResult;
  J1939Rm_AckCode        AckCode;

  /* #10 Request clearing DTC from DEM. */
  ClearResult = Dem_J1939DcmClearDTC(J1939Dcm_ClearHandlerState.ClearFilter, J1939Dcm_ClearHandlerState.DTCOrigin, J1939Dcm_ClearHandlerState.ClearNode);
  /* #11 Update the processing state and set the acknowledgement code based on the return value of the clear handler.*/

  /*
         ClearResult          | ProcessorState                 | AckCode
        ======================+================================+======================
         DEM_CLEAR_OK         | J1939DCM_MSG_STATE_SENDACK     | J1939RM_ACK_POSITIVE
         DEM_CLEAR_PENDING    | J1939DCM_MSG_STATE_CLEAR       | J1939RM_ACK_POSITIVE
         Other                | J1939DCM_MSG_STATE_SENDACK     | J1939RM_ACK_NEGATIVE
  */
  switch (ClearResult)
  {
    case DEM_CLEAR_OK:
      *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
      AckCode = J1939RM_ACK_POSITIVE;
      break;

    case DEM_CLEAR_PENDING:
      *pDMProcessorState = J1939DCM_MSG_STATE_CLEAR;
      AckCode = J1939RM_ACK_POSITIVE;
      break;

    default:
      *pDMProcessorState = J1939DCM_MSG_STATE_SENDACK;
      AckCode = J1939RM_ACK_NEGATIVE;
      break;
  }

  /* #12 Return the acknowledgement code. */
  return AckCode;
}
#endif /* (J1939DCM_NEED_CLEAR_HANDLER == STD_ON) */

/***********************************************************************************************************************
 * J1939Dcm_TxHandlerPrepare()
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
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_TxHandlerPrepare( P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) pMsgBuffer, uint8_least DiagMsg, J1939Dcm_NodeHdlType NodeHdl, uint8_least ResponseAddress )
{
  J1939Dcm_ChannelHdlType lChannelHdl;
  Std_ReturnType          lResult;
  /* #10 Assume unsuccessful transmission preparation. */
  /* In case of something went wrong! */
  lResult = E_NOT_OK;

  /* #11 Enter critical section (Reason: protect data consistency of the transmission handler state during the transmission preparation of a DM). */
  J1939Dcm_EnterCritical();

  /* #12 If the transmission handler is not busy: */
  if (NULL_PTR == J1939Dcm_TxHandlerState.Buffer)
  {
    /* #13 Update the transmission handler state with the meta data, the payload, the DM, and the node handle. */
#if (2 == J1939DCM_METADATA_LEN)
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_SA_OFFSET]   = J1939Dcm_Cfg_GetNodeAddress(NodeHdl);
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_DA_OFFSET]   = (uint8)ResponseAddress;
#elif (4 == J1939DCM_METADATA_LEN)
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_SA_OFFSET]   = J1939Dcm_Cfg_GetNodeAddress(NodeHdl);
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_DA_OFFSET]   = (uint8)ResponseAddress;
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_PDUF_OFFSET] = 0; /* not used */
    J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_PRIO_OFFSET] = J1939DCM_DM_PRIORITY_DEFAULT;
#else
    /* meta data length not supported */
    J1939Dcm_DetAssertAlways(J1939DCM_API_MAINFUNCTION, J1939DCM_E_PARAM_CONFIG)
    return E_NOT_OK;
#endif

    /* setup transmission */
    J1939Dcm_TxHandlerState.Buffer  = pMsgBuffer;
    J1939Dcm_TxHandlerState.DM      = (uint8)DiagMsg;
    J1939Dcm_TxHandlerState.NodeHdl = NodeHdl;

    /* IF or TP? */
    /* #14 If the incoming DM can be sent directly via the CAN interface (message length is 8 bytes and only applicable for DM5, DM15, DM21, or DM26): */
    if ( (J1939DCM_DM5 == DiagMsg) || (J1939DCM_DM15 == DiagMsg) ) /* not yet supported DMs: (J1939DCM_DM21 == DiagMsg) || (J1939DCM_DM26 == DiagMsg) */
    {
      /* for IF connections: meta data is part of SduLength and SduPointer */
      /* when coming here, meta data is 2 or 4 */
      /* #15 Append the meta data to the payload. */
      pMsgBuffer->SduDataPtr[pMsgBuffer->SduLength + J1939DCM_METADATA_SA_OFFSET]   = J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_SA_OFFSET];
      pMsgBuffer->SduDataPtr[pMsgBuffer->SduLength + J1939DCM_METADATA_DA_OFFSET]   = J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_DA_OFFSET];
#if (4 == J1939DCM_METADATA_LEN)
      pMsgBuffer->SduDataPtr[pMsgBuffer->SduLength + J1939DCM_METADATA_PDUF_OFFSET] = J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_PDUF_OFFSET];
      pMsgBuffer->SduDataPtr[pMsgBuffer->SduLength + J1939DCM_METADATA_PRIO_OFFSET] = J1939Dcm_TxHandlerState.MetaData[J1939DCM_METADATA_PRIO_OFFSET];
#endif
      /* #16 Update the length of the service data unit by adding the length of the meta data. */
      pMsgBuffer->SduLength += J1939DCM_METADATA_LEN;
      /* #17 Set the transmission path to send via CAN interface and reload the transmission timer. */
      J1939Dcm_TxHandlerState.TransmitPath = J1939DCM_TX_PATH_IF;
      J1939Dcm_TxHandlerState.TxTimer      = J1939DCM_TXTIMEOUT;
    }
    /* #18 Otherwise (if the incoming DM needs a TP session because the message length is greater than 8 bytes): */
    else
    {
      /* for TP connections: SduLength contains payload only; no change required */
      /* #19 Set the transmission path to send via a TP session (transmission timeout is supervised by J1939TP). */
      J1939Dcm_TxHandlerState.TransmitPath = J1939DCM_TX_PATH_TP;
      J1939Dcm_TxHandlerState.TxTimer      = 0; /* no timeout supervision is used */
    }

    /* Set channels counters initially to message length --> nothing will be transmitted  */
    /* Transmission preparation is triggered on the node level.
     * All channels referenced by the current node are accounted for in case of a global channel broadcast is requested (DM1). */
    /* #20 Iterate over the configured channels for the requested node. */
    J1939Dcm_ForEachChannel_Start
    {
      J1939Dcm_ForEachChannel_Next;
      /* #21 Update the buffer indices of all channels as if all channels transmit the same DM (in case of global broadcast of DM1). */
      J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl] = pMsgBuffer->SduLength;
    } J1939Dcm_ForEachChannel_End

    /* #22 Indicate successful transmission preparation. */
    lResult = E_OK;
  }
  /* else: transmission is already in progress; reject request */

  /* #23 Leave critical section. */
  J1939Dcm_ExitCritical();

  /* #24 Return transmission results. */
  return lResult;
}

/***********************************************************************************************************************
 * J1939Dcm_TxHandlerTransmit()
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
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_TxHandlerTransmit( PduIdType TxPdu, J1939Dcm_ChannelHdlType ChannelHdl )
{
  Std_ReturnType lResult;
  uint8_least    lUnconfChannels;
  PduInfoType    lTpTransmitData;

  /* #10 Enter critical section (Reason: protect data consistency of the transmission handler state during the transmission preparation of a DM). */
  J1939Dcm_EnterCritical();

  /* #11 Prepare a transmission for one specific channel (reset the buffer index and increment the number of channels that are waiting for confirmation). */
  J1939Dcm_TxHandlerState.BufferIndex[ChannelHdl] = 0;
  J1939Dcm_TxHandlerState.UnconfirmedChannels++;
  lUnconfChannels = J1939Dcm_TxHandlerState.UnconfirmedChannels;

  /* #12 Leave critical section. */
  J1939Dcm_ExitCritical();

  /* #13 If the transmission path is to send via a TP session: */
  if (J1939DCM_TX_PATH_TP == J1939Dcm_TxHandlerState.TransmitPath)
  {
    /* for TP transmission: pass only meta data to PduR_Transmit. Payload is requested for from PduR via a copy Tx function. */
    /* #14 Reference only the meta data via the Sdu data pointer of the local transmitting object. */
    lTpTransmitData.SduDataPtr = (SduDataPtrType)J1939Dcm_TxHandlerState.MetaData;
  }
  /* #15 Otherwise (if the transmission path is to send via a CAN interface): */
  else
  {
    /* for IF transmissions: use buffer as it is; meta data have been added during Tx preparation */
    /* #16 Reference the whole message (payload and meta data) via the Sdu data pointer of the local transmitting object. */
    lTpTransmitData.SduDataPtr = J1939Dcm_TxHandlerState.Buffer->SduDataPtr;
  }
  /* #17 Update the Sdu length of the local transmitting object. */
  /* The length of the Sdu does not include the length of the meta data when sending via TP session while it is included when sending via CAN interface. */
  lTpTransmitData.SduLength = J1939Dcm_TxHandlerState.Buffer->SduLength;

  /* #18 Start transmission on channel. */
  lResult = PduR_J1939DcmTransmit(TxPdu, &lTpTransmitData);

  /* #19 If the transmission is unsuccessful and the transmission is not confirmed: */
  if ( (E_OK != lResult) && (J1939Dcm_TxHandlerState.UnconfirmedChannels >= lUnconfChannels) )
  {
     /* #20 Enter critical section (Reason: protect data consistency of the transmission handler state during the transmission preparation of a DM). */
    J1939Dcm_EnterCritical();

    /* nothing transmitted and apparently no confirmation occurred */
    /* In case of transmission was declined even before calling copy transmitted data function (Concerning DM1 and DM35). */
    /* Otherwise, channels waiting for confirmation are decremented in TP transmission confirmation function. */
    /* #21 Decrement the number of channels waiting for confirmation. */
    J1939Dcm_TxHandlerState.UnconfirmedChannels--;

    /* #22 Leave critical section. */
    J1939Dcm_ExitCritical();
  }
  /* #23 Return the transmission results. */
  return lResult;
}

/***********************************************************************************************************************
 * J1939Dcm_TxHandlerConfirm()
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
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_TxHandlerConfirm(J1939Dcm_NodeHdlType NodeHdl, uint8_least DiagMsg, Std_ReturnType Result)
{
  switch (DiagMsg)
  {
    /* #10 In case of DM1: */
#if (J1939DCM_DM1_SUPPORT == STD_ON)
    case J1939DCM_DM1:
      /* #11 Enter critical section (Reason: protect data consistency of the queue of events of DM1 against the request manager interruptions). */
      J1939Dcm_EnterCritical();
      /* #12 Update the queue of events of DM1 with a transmission success or a transmission failure. */
      if (E_OK == Result)
      {
        /* trigger transmission finalization */
        J1939Dcm_QueueSetTxConf(J1939Dcm_DM1Queue[NodeHdl]);
      }
      else
      {
        /* trigger NACK transmission (in case of broadcast) */
        J1939Dcm_QueueSetTxFail(J1939Dcm_DM1Queue[NodeHdl]);
      }
      /* #13 Leave critical section. */
      J1939Dcm_ExitCritical();

      break;
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */
      
#if ((J1939DCM_DM2_SUPPORT == STD_ON)  || \
     (J1939DCM_DM4_SUPPORT == STD_ON)  || \
     (J1939DCM_DM5_SUPPORT == STD_ON)  || \
     (J1939DCM_DM21_SUPPORT == STD_ON) || \
     (J1939DCM_DM24_SUPPORT == STD_ON) || \
     (J1939DCM_DM25_SUPPORT == STD_ON) || \
     (J1939DCM_DM26_SUPPORT == STD_ON) || \
     (J1939DCM_DM27_SUPPORT == STD_ON) || \
     (J1939DCM_DM31_SUPPORT == STD_ON) || \
     (J1939DCM_DM53_SUPPORT == STD_ON) || \
     (J1939DCM_DM54_SUPPORT == STD_ON) || \
     (J1939DCM_DM55_SUPPORT == STD_ON) )
      /* #14 in case of generic DMx: */
# if (J1939DCM_DM2_SUPPORT == STD_ON)
    case J1939DCM_DM2:
# endif
# if (J1939DCM_DM4_SUPPORT == STD_ON)
    case J1939DCM_DM4:
# endif
# if (J1939DCM_DM5_SUPPORT == STD_ON)
    case J1939DCM_DM5:
# endif
# if (J1939DCM_DM21_SUPPORT == STD_ON)
    case J1939DCM_DM21:
# endif
# if (J1939DCM_DM24_SUPPORT == STD_ON)
    case J1939DCM_DM24:
# endif
# if (J1939DCM_DM25_SUPPORT == STD_ON)
    case J1939DCM_DM25:
# endif
# if (J1939DCM_DM26_SUPPORT == STD_ON)
    case J1939DCM_DM26:
# endif
# if (J1939DCM_DM27_SUPPORT == STD_ON)
    case J1939DCM_DM27:
# endif
# if (J1939DCM_DM31_SUPPORT == STD_ON)
    case J1939DCM_DM31:
# endif
# if (J1939DCM_DM53_SUPPORT == STD_ON)
    case J1939DCM_DM53:
# endif
# if (J1939DCM_DM54_SUPPORT == STD_ON)
    case J1939DCM_DM54:
# endif
# if (J1939DCM_DM55_SUPPORT == STD_ON)
    case J1939DCM_DM55:
# endif
      
      /* #15 Enter critical section (Reason: protect data consistency of the queue of events of DMx against the request manager interruptions). */
      J1939Dcm_EnterCritical();
      /* #16 Update the queue of events of DMx with a transmission success or a transmission failure. */
      if (E_OK == Result)
      {
        /* trigger transmission finalization */
        J1939Dcm_QueueSetTxConf(J1939Dcm_DMxQueue[NodeHdl]);
      }
      else
      {
        /* trigger NACK transmission (in case of broadcast) */
        J1939Dcm_QueueSetTxFail(J1939Dcm_DMxQueue[NodeHdl]);
      }
      /* #17 Leave critical section.*/
      J1939Dcm_ExitCritical();

      break;
#endif

      /* #18 In case of DM35: */
#if (J1939DCM_DM35_SUPPORT == STD_ON)
    case J1939DCM_DM35:
      /* #19 Enter critical section (Reason: protect data consistency of the queue of events of DM35 against the request manager interruptions). */
      J1939Dcm_EnterCritical();
      /* trigger transmission finalization (DM35 is always broadcast, no NACK need to be sent) */
      /* #20 Update the queue of events of DMx with a transmission success. */
      J1939Dcm_QueueSetTxConf(J1939Dcm_DM35Queue[NodeHdl]);
      /* #21 Leave critical section. */
      J1939Dcm_ExitCritical();
      break;
#endif /* (J1939DCM_NEED_DM35_PROCESSOR == STD_ON) */

      /* #22 In case of DM15 to DM18, reset memory and transmission handlers and notify the application. */
#if (J1939DCM_DM15_SUPPORT == STD_ON)
    case J1939DCM_DM15:
      J1939Dcm_TxHandlerReset();
      J1939Dcm_MemHandlerReset();

      J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemResponseTxConfFunc), J1939DCM_API_TXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)            /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
      J1939Dcm_Cfg_MemResponseTxConfFunc(Result);
      break;
#endif

#if (J1939DCM_DM16_SUPPORT == STD_ON)
    case J1939DCM_DM16:
      J1939Dcm_TxHandlerReset();
      J1939Dcm_MemHandlerReset();
      J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemDataTxConfFunc), J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)            /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
      J1939Dcm_Cfg_MemDataTxConfFunc(J1939DCM_MEM_DATA_BINARY, Result);
      break;
#endif

#if (J1939DCM_DM18_SUPPORT == STD_ON)
    case J1939DCM_DM18:
      J1939Dcm_TxHandlerReset();
      J1939Dcm_MemHandlerReset();
      J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemDataTxConfFunc), J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)            /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
      J1939Dcm_Cfg_MemDataTxConfFunc(J1939DCM_MEM_DATA_SECURITY, Result);
      break;
#endif
    /* #23 If the incoming message does not match any of the previous cases, check whether the configuration is correct.*/
    default:
      /* no TxConfirmation implemented for given DM! */
      J1939Dcm_DetAssertAlways(J1939DCM_API_TXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)
      break;
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 * J1939Dcm_TxHandlerReset
 **********************************************************************************************************************/
/*! \brief      J1939 Transmit Handler: reset handler when transmission is complete / aborted
 *  \pre
 *  \context    Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_TxHandlerReset( void )
{

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  J1939Dcm_EnterCritical();

  J1939Dcm_TxHandlerState.Buffer              = NULL_PTR;
  J1939Dcm_TxHandlerState.UnconfirmedChannels = 0;
  J1939Dcm_TxHandlerState.DM                  = J1939DCM_DM_UNDEF;
  J1939Dcm_TxHandlerState.NodeHdl             = J1939DCM_INVALID_NODE_HDL;
  J1939Dcm_TxHandlerState.TxTimer             = 0;

  J1939Dcm_ExitCritical();
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
}

#if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_RxHandlerReset
 **********************************************************************************************************************/
/*! \brief      J1939 Receive Handler: reset handler when reception is complete / aborted
 *  \pre
 *  \context    Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_RxHandlerReset( void )
{
  J1939Dcm_RxHandlerState.RxPduId = J1939DCM_INVALID_PDU_ID;
  J1939Dcm_RxHandlerState.RxAddressInfo.ChannelId          = J1939DCM_INVALID_CHANNEL_HDL;
  J1939Dcm_RxHandlerState.RxAddressInfo.SourceAddress      = J1939DCM_INVALID_MEM_ADDRESS;
  J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress = J1939DCM_INVALID_MEM_ADDRESS;
}
#endif

#if (J1939DCM_NEED_MEM_ACCESS == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_MemHandlerReset
 **********************************************************************************************************************/
/*! \brief      J1939 Memory Handler: reset handler when transmission is complete / aborted
 *  \pre
 *  \context    Task
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_MemHandlerReset( void )
{

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  J1939Dcm_EnterCritical();

  J1939Dcm_MemHandlerState.DiagMsg            = J1939DCM_DM_UNDEF;
  J1939Dcm_MemHandlerState.ChannelHdl         = J1939DCM_INVALID_CHANNEL_HDL;
  J1939Dcm_MemHandlerState.NodeHdl            = J1939DCM_INVALID_NODE_HDL;
  J1939Dcm_MemHandlerState.DestinationAddress = J1939DCM_INVALID_MEM_ADDRESS;
  J1939Dcm_MemHandlerState.TxPduId            = J1939DCM_INVALID_PDU_ID;

  J1939Dcm_ExitCritical();
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
}
#endif /* (J1939DCM_NEED_MEM_ACCESS == STD_ON) */

#if (J1939DCM_NEED_MEM_ACCESS == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_MemHandlerTask()
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
 *
 *
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_MemHandlerTask( void )
{
  Std_ReturnType  lTxResult;
  uint8_least     lDiagMessage;
  PduIdType       lTxPdu;
  J1939Dcm_ChannelHdlType lChannelHdl;

  /* #10 Enter critical section (Reason: protect data consistency of the memory handler state during the transmission preparation of a DM). */
  J1939Dcm_EnterCritical();

  /* #11 If there is a memory access message (DM15, DM16, or DM18) to be transmitted: */
  if (J1939DCM_DM_UNDEF != J1939Dcm_MemHandlerState.DiagMsg)
  {
    /* #12 Prepare for a transmission according to the queued data. */
    lDiagMessage = J1939Dcm_MemHandlerState.DiagMsg;
    lTxResult = J1939Dcm_TxHandlerPrepare(&J1939Dcm_MemHandlerState.TxData,
                                          J1939Dcm_MemHandlerState.DiagMsg,
                                          J1939Dcm_MemHandlerState.NodeHdl,
                                          J1939Dcm_MemHandlerState.DestinationAddress);
    lTxPdu      = J1939Dcm_MemHandlerState.TxPduId;
    lChannelHdl = J1939Dcm_MemHandlerState.ChannelHdl;

    /* #13 Leave critical section. */
    J1939Dcm_ExitCritical();

    /* #14 If the preparation for the DM transmission is successful: */
    if (E_OK == lTxResult)
    {
      /* #15 Transmit the DM because the transmission handler is free. */
      lTxResult = J1939Dcm_TxHandlerTransmit(lTxPdu, lChannelHdl);
    }
    /* #16 Otherwise (if the preparation for the DM transmission is unsuccessful): */
    else
    {
      /* #17 Retry in next cycle because the transmission handler is busy. */
      lTxResult = E_OK;
    }

    /* #18 If the transmission was unsuccessful: */
    if (E_OK != lTxResult)
    {
      /* #19 Reset the transmission Handler and the memory Handler and notify the application.  */
      J1939Dcm_TxHandlerReset();
      J1939Dcm_MemHandlerReset();

      switch (lDiagMessage)
      {
# if (J1939DCM_DM15_SUPPORT == STD_ON)
        case J1939DCM_DM15:
          J1939Dcm_Cfg_MemResponseTxConfFunc(E_NOT_OK);
          break;
# endif
# if (J1939DCM_DM16_SUPPORT == STD_ON)
        case J1939DCM_DM16:
          J1939Dcm_Cfg_MemDataTxConfFunc(J1939DCM_MEM_DATA_BINARY, E_NOT_OK);
          break;
# endif
# if (J1939DCM_DM18_SUPPORT == STD_ON)
        case J1939DCM_DM18:
          J1939Dcm_Cfg_MemDataTxConfFunc(J1939DCM_MEM_DATA_SECURITY, E_NOT_OK);
          break;
# endif
        default:
          /* should already be detected in RxIndication APIs, but also done here for robustness */
          J1939Dcm_DetAssertAlways(J1939DCM_API_MAINFUNCTION, J1939DCM_E_INVALID_PGN)
          break;
      }
    }
  }
  /* #20 Otherwise (if there is no memory access message (DM15, DM16, or DM18) to be transmitted): */
  else
  {

    /* #21 Leave critical section. */
    J1939Dcm_ExitCritical();
  }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (J1939DCM_NEED_MEM_ACCESS == STD_ON) */

/**********************************************************************************************************************
 * J1939Dcm_Reset
 **********************************************************************************************************************/
/*! \brief      reset internal states of J1939 DCM
 *  \pre
 *  \context    J1939Dcm_Init, J1939Dcm_ReInit
 *  \note
 **********************************************************************************************************************/
J1939DCM_LOCAL FUNC(void, J1939DCM_CODE) J1939Dcm_Reset( void )
{
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;

  /* --- global module status ----------------------------------------------------------------- */
  J1939Dcm_ForEachNode_Start
  {
    J1939Dcm_ForEachNode_Next;

    J1939Dcm_ForEachChannel_Start
    {
      J1939Dcm_ForEachChannel_Next;

      J1939Dcm_ComStateSetOffline(lNodeHdl, lChannelHdl);    /* SWS_J1939Dcm_00002 */                                  /* PRQA S 0290 */ /* MD_J1939Dcm_21.1b */

    } J1939Dcm_ForEachChannel_End
  } J1939Dcm_ForEachNode_End

  /* --- DEM Semaphore ------------------------------------------------------------------------ */
  J1939Dcm_DemSemaphoreSetLocked(FALSE);

  /* --- Communication Handler ---------------------------------------------------------------- */
  J1939Dcm_TxHandlerReset();
  J1939Dcm_RxHandlerReset();

  /* --- DM specific data --------------------------------------------------------------------- */
  J1939Dcm_DM1Init();
  J1939Dcm_DM13Init();
  J1939Dcm_DM35Init();
  J1939Dcm_DMxInit();

  J1939Dcm_MemHandlerReset();
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* --- Service functions ---------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
 * J1939Dcm_Init
 **********************************************************************************************************************/
/*! \brief      J1939 DCM initialization function.
 *  \param[in]  J1939Dcm_ConfigType        ConfigPtr          Configuration structure for initializing the module
 *  \pre        Interrupts must be disabled
 *  \context    System Startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_Init( P2CONST(J1939Dcm_ConfigType, AUTOMATIC, J1939DCM_INIT_DATA) configPtr )
{
  /* ----- Local Variables --------------------------------------------------------------------- */
#if (J1939DCM_USE_INIT_POINTER == STD_ON)
  uint8 lErrorId = J1939DCM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* NULL pointer check is not always needed and is done later for ECUM error reporting */

  /* ----- Implementation ---------------------------------------------------------------------- */
  /* config pointer is set immediately, but will only be used if the validity check below is passed */
  J1939Dcm_Data_ConfigPtr() = configPtr;

  /* post-build loadable: report errors to EcuM     */
  /* post-build selectable: only check init pointer */
  if (configPtr == NULL_PTR)
  {
    lErrorId = J1939DCM_E_PARAM_POINTER;
    J1939DCM_EcuMReportBswError(ECUM_BSWERROR_NULLPTR)
  }
# if (J1939DCM_CONFIGURATION_VARIANT == J1939DCM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
  /* Check generator version compatibility: SPEC00035482 */
  else if (J1939DCM_GENERATOR_COMPATIBILITY_VERSION != configPtr->CompatibilityVersionOfPBConfig)
  {
    J1939DCM_EcuMReportBswError(ECUM_BSWERROR_COMPATIBILITYVERSION)
  }
  /* Check magic number: SPEC00035483 */
  else if (J1939DCM_FINAL_MAGIC_NUMBER != configPtr->FinalMagicNumberOfPBConfig)
  {
    J1939DCM_EcuMReportBswError(ECUM_BSWERROR_MAGICNUMBER)
  }
# endif
  else
#endif  /* J1939DCM_USE_INIT_POINTER == STD_ON */
  {
    if (J1939DCM_STATE_UNINIT == J1939Dcm_InitState)
    {
      J1939Dcm_InitState = J1939DCM_STATE_INIT;
      J1939Dcm_Reset();
    }
    else
    {
      J1939Dcm_DetAssertAlways(J1939DCM_API_INIT, J1939DCM_E_REINIT)   /* SWS_J1939Dcm_00005 */
    }
  }

  /* ----- Development Error Report ------------------------------------------------------------ */
#if (J1939DCM_USE_INIT_POINTER == STD_ON)
# if (J1939DCM_DEV_ERROR_DETECT == STD_ON)
  if (lErrorId != J1939DCM_E_NO_ERROR)
  {
    J1939Dcm_DetAssertAlways(J1939DCM_API_INIT, lErrorId)
  }
# else
  /* errors may occur, but aren't reported */
  J1939Dcm_IgnoreUnusedArgument(lErrorId)   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#else
  J1939Dcm_IgnoreUnusedArgument(configPtr)   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* J1939DCM_USE_INIT_POINTER == STD_ON */
}

/**********************************************************************************************************************
 * J1939Dcm_InitMemory
 **********************************************************************************************************************/
/*! \brief      J1939 DCM initialization function for VAR_INIT data
 *  \pre        Interrupts must be disabled
 *  \context    System Startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_InitMemory(void)
{
  J1939Dcm_InitState = J1939DCM_STATE_UNINIT;
}

/**********************************************************************************************************************
 * J1939Dcm_DeInit
 **********************************************************************************************************************/
/*! \brief      reset the J1939 DCM to uninitialized state
 *  \pre        -
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_DeInit( void )
{

  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_DEINIT, J1939DCM_E_UNINIT)  /* SWS_J1939Dcm_00004 */

  J1939Dcm_InitState = J1939DCM_STATE_UNINIT;    /* SWS_J1939Dcm_00003 */
  J1939Dcm_Reset();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (J1939DCM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * J1939Dcm_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      J1939 DCM version
 *  \param[in]  Std_VersionInfoType        versioninfo         version of the used implementation
 *  \pre        -
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, J1939DCM_APPL_DATA) versioninfo )
{
  J1939Dcm_DetAssertVReturn((versioninfo != NULL_PTR), J1939DCM_API_GETVERSIONINFO, J1939DCM_E_PARAM_POINTER)

  versioninfo->vendorID         = J1939DCM_VENDOR_ID;
  versioninfo->moduleID         = J1939DCM_MODULE_ID;

  versioninfo->sw_major_version = J1939DCM_SW_MAJOR_VERSION;
  versioninfo->sw_minor_version = J1939DCM_SW_MINOR_VERSION;
  versioninfo->sw_patch_version = J1939DCM_SW_PATCH_VERSION;
} /* J1939Dcm_GetVersionInfo */  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (J1939DCM_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 * J1939Dcm_SetState
 **********************************************************************************************************************/
/*! \brief      Set the communication state of the J1939 DCM
 *  \param[in]  NetworkHandleType   channel    channel for which the state shall be changed
 *              uint8               node       node for which the state shall be changed
 *              J1939Dcm_StateType  newState   new state the J1939Dcm shall enter
 *  \return     Std_ReturnType      E_OK:     new communication state was set
 *                                  E_NOT_OK: communication state was not set due to error
 *  \pre        Module must be initialized
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_SetState( NetworkHandleType channel,
                                                       uint8 node,
                                                       J1939Dcm_StateType newState )
{
  Std_ReturnType          lResult;
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;
  boolean                 lIsSupported;

  J1939Dcm_DetAssertReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_SETSTATE, J1939DCM_E_UNINIT, E_NOT_OK) /* SWS_J1939Dcm_00004 */

  lResult     = E_NOT_OK;
  lNodeHdl    = J1939Dcm_Cfg_GetNodeHdlById(node);
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlById(channel);

  J1939Dcm_DetAssertNodeHdlReturn(lNodeHdl, J1939DCM_API_SETSTATE, E_NOT_OK)
  J1939Dcm_DetAssertChannelHdlReturn(lChannelHdl, J1939DCM_API_SETSTATE, E_NOT_OK)

  lIsSupported = J1939Dcm_IsNodeChannelSupported(lNodeHdl, lChannelHdl);
  if (FALSE != lIsSupported)
  {
    /* change state according to newState parameter */
    if (J1939DCM_STATE_OFFLINE == newState)
    {
      J1939Dcm_ComStateSetOffline(lNodeHdl, lChannelHdl);                                                              /* PRQA S 0290 */ /* MD_J1939Dcm_21.1b */   /* PRQA S 3689 */ /* MD_J1939Dcm_21.1b */
      if (0 != J1939Dcm_ComState[(lNodeHdl)])                                                                          /* PRQA S 3689 */ /* MD_J1939Dcm_21.1b */
      {
        newState = J1939DCM_STATE_ONLINE;
      }
      /* else all the channels referenced by that particular node are offline and hence the node communication state shall be offline as well */
      lResult = E_OK;
    }
    else if (J1939DCM_STATE_ONLINE == newState)
    {
      J1939Dcm_ComStateSetOnline(lNodeHdl, lChannelHdl);                                                               /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
      lResult = E_OK;
    }
    else
    {
      /* state parameter is out of range */
      J1939Dcm_DetAssertAlways(J1939DCM_API_SETSTATE, J1939DCM_E_INVALID_STATE)  /* SWS_J1939Dcm_00130 */
    }

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    J1939Dcm_EnterCritical();

    /* queue state request to notify DM message handlers */
#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
    J1939Dcm_QueueSetStateChange(J1939Dcm_DM1Queue[lNodeHdl]);                                                         /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
    J1939Dcm_DM1Queue[lNodeHdl].StateChangeRequest = newState;                                                         /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
#endif

#if (J1939DCM_NEED_DM35_PROCESSOR == STD_ON)
    J1939Dcm_QueueSetStateChange(J1939Dcm_DM35Queue[lNodeHdl]);                                                        /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
    J1939Dcm_DM35Queue[lNodeHdl].StateChangeRequest = newState;                                                        /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
#endif

#if (J1939DCM_NEED_DMX_PROCESSOR == STD_ON)
    J1939Dcm_QueueSetStateChange(J1939Dcm_DMxQueue[lNodeHdl]);                                                         /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
    J1939Dcm_DMxQueue[lNodeHdl].StateChangeRequest = newState;                                                         /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
#endif

    J1939Dcm_ExitCritical();
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  }
  else
  {                                                                                                                    /* PRQA S 3201 */ /* MD_J1939Dcm_13.7 */
    /* node does not support the specified channel */
    J1939Dcm_DetAssertAlways(J1939DCM_API_SETSTATE, J1939DCM_E_PARAM_CONFIG)
  }

  return lResult;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* --- Scheduled functions -------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
 * J1939Dcm_MainFunction
 **********************************************************************************************************************/
/*! \brief      main function of the J1939 DCM for scheduling purposes and timing supervision
 *  \pre        -
 *  \context    task
 *  \note       -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_MainFunction( void )
{

  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_MAINFUNCTION, J1939DCM_E_UNINIT)  /* SWS_J1939Dcm_00004 */

  J1939Dcm_TxHandlerTask();

  J1939Dcm_MemHandlerTask();

  J1939Dcm_DM1Task();
  J1939Dcm_DM13Task();
  J1939Dcm_DM35Task();
  J1939Dcm_DMxTask();

} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* --- Callback functions --------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
 * J1939Dcm_RequestIndication()
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
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_RequestIndication( uint8 node,
                                                      NetworkHandleType channel,
                                                      uint32 requestedPgn,
                                                      P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939DCM_APPL_DATA) extIdInfo,
                                                      uint8 sourceAddress,
                                                      uint8 destAddress,
                                                      uint8 priority )
{
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;
  J1939Dcm_MessageHdlType lMsgHdl;
  uint8                   lDiagMsg;
  uint8                   lResponseAddress;

  J1939Dcm_IgnoreUnusedArgument(extIdInfo)     /* not needed; DMx messages don't use extended identifier bytes. */      /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(destAddress)   /* not needed; address is already known through node parameter. */       /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(priority)      /* not needed; default priority is used. */                              /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* #10 Check whether the module is initialized. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_REQUESTINDICATION, J1939DCM_E_UNINIT) /* SWS_J1939Dcm_00004 */

  /* #11 Calculate local IDs for NM node ComM channel if configured. */
  lNodeHdl    = J1939Dcm_Cfg_GetNodeHdlById(node);
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlById(channel);

  /* #12 Check if either the node or the channel or both is(are) not configured. */
  J1939Dcm_DetAssertNodeHdlVReturn(lNodeHdl, J1939DCM_API_REQUESTINDICATION)
  J1939Dcm_DetAssertChannelHdlVReturn(lChannelHdl, J1939DCM_API_REQUESTINDICATION)

  /* #13 If the requested node and the referenced channel are in online state: */
  if (J1939Dcm_ComStateIsOnline(lNodeHdl, lChannelHdl))                                                                 /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
  {
    /* Resolve address to which the response shall be transmitted. */
    /* [J1939-73] If the Request or applicable PGN is sent to a global address, then the response is sent to a global address.
     *            If the Request or applicable PGN is sent to a specific address, then the response is sent to a specific address.*/
    /* #14 If the destination address is global: */
    if (J1939DCM_ADDRESS_BROADCAST == destAddress)
    {
      /* #15 Set the response address to global. */
      lResponseAddress = J1939DCM_ADDRESS_BROADCAST;
    }
    /* #16 Otherwise (if the destination address is specific): */
    else
    {
      /* #17 Set the response address to the specific source address of the requested diagnostic message. */
      lResponseAddress = sourceAddress;
    }

    /* #18 Calculate the local ID of the requested diagnostic message. */
    lMsgHdl = J1939Dcm_GetMsgHdlByPGN(requestedPgn, lNodeHdl, lChannelHdl);

    /* #19 If a valid local ID for the requested DM was found: */
    if (J1939DCM_INVALID_MESSAGE_HDL != lMsgHdl)
    {
      /* #20 Calculate the corresponding message handle. */
      lDiagMsg = J1939Dcm_Cfg_GetMessageDM(lMsgHdl);
    }
    /* #21 Otherwise (if the local ID of the requested DM is invalid): */
    else
    {
      /* #22 Set the message handle to undefined. */
      lDiagMsg = J1939DCM_DM_UNDEF;
    }

    /* #23 Forward the DM request to respective message handler. */
    switch (lDiagMsg)
    {
#if (J1939DCM_DM1_SUPPORT == STD_ON)
      /* #24 In case of DM1: */
      case J1939DCM_DM1:
        /* #25 If currently there is no other DM1 request being processed: */
        if (FALSE == J1939Dcm_QueueGetMsgRequest(J1939Dcm_DM1Queue[lNodeHdl]))                                          /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
        {
            /* #26 Enter critical section (Reason: protect data consistency of the queue of events against another request manager interruption). */
          J1939Dcm_EnterCritical();
          /* #27 Indicate the incoming request in the queue of events and set the corresponding channel handle and response address. */
          J1939Dcm_QueueSetMsgRequest(J1939Dcm_DM1Queue[lNodeHdl]);                                                     /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          J1939Dcm_DM1Queue[lNodeHdl].ChannelHdl = lChannelHdl;                                                         /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          J1939Dcm_DM1Queue[lNodeHdl].ResponseDA = lResponseAddress;                                                    /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          /* #28 Leave critical section. */
          J1939Dcm_ExitCritical();
        }
        /* #29 Otherwise (if currently a DM1 request is being processed): */
        else
        {
          /* DM request has already been queued. */
          /* [J1939-73] If for some reason a device can not perform the requested action, then it is required to send a negative acknowledgement.
           *            Implementers be aware that no positive or negative acknowledgement is sent when the request was sent to a global address. */
          /* #30 If the response address of the requested DM is specific: */
          if (J1939DCM_ADDRESS_BROADCAST != lResponseAddress)
          {
            /* #31 Request sending negative acknowledgement from the request manager. */
            (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(lNodeHdl),
                                  J1939Dcm_Cfg_GetChannelId(lChannelHdl),
                                  requestedPgn,
                                  NULL_PTR,
                                  J1939RM_ACK_CANNOT_RESPOND,
                                  lResponseAddress,
                                  priority,
                                  FALSE);
          }
        }
        break;
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */

        /* #32 In case of DMx: */
#if ( (J1939DCM_DM2_SUPPORT == STD_ON) || (J1939DCM_DM3_SUPPORT == STD_ON) || (J1939DCM_DM4_SUPPORT == STD_ON) || (J1939DCM_DM5_SUPPORT == STD_ON) || (J1939DCM_DM11_SUPPORT == STD_ON) || (J1939DCM_DM24_SUPPORT == STD_ON) || (J1939DCM_DM25_SUPPORT == STD_ON) || (J1939DCM_DM27_SUPPORT == STD_ON) || (J1939DCM_DM31_SUPPORT == STD_ON) || (J1939DCM_DM53_SUPPORT == STD_ON) || (J1939DCM_DM54_SUPPORT == STD_ON) || (J1939DCM_DM55_SUPPORT == STD_ON) )
# if (J1939DCM_DM2_SUPPORT == STD_ON)
      case J1939DCM_DM2:
# endif
# if (J1939DCM_DM3_SUPPORT == STD_ON)
      case J1939DCM_DM3:
# endif
# if (J1939DCM_DM4_SUPPORT == STD_ON)
      case J1939DCM_DM4:
# endif
# if (J1939DCM_DM5_SUPPORT == STD_ON)
      case J1939DCM_DM5:
# endif
# if (J1939DCM_DM11_SUPPORT == STD_ON)
      case J1939DCM_DM11:
# endif
# if (J1939DCM_DM24_SUPPORT == STD_ON)
      case J1939DCM_DM24:
# endif
# if (J1939DCM_DM25_SUPPORT == STD_ON)
      case J1939DCM_DM25:
# endif
# if (J1939DCM_DM27_SUPPORT == STD_ON)
      case J1939DCM_DM27:
# endif
# if (J1939DCM_DM31_SUPPORT == STD_ON)
      case J1939DCM_DM31:
# endif
# if (J1939DCM_DM53_SUPPORT == STD_ON)
      case J1939DCM_DM53:
# endif
# if (J1939DCM_DM54_SUPPORT == STD_ON)
      case J1939DCM_DM54:
# endif
# if (J1939DCM_DM55_SUPPORT == STD_ON)
      case J1939DCM_DM55:
# endif
        /* #33 If currently there is no other DMx request being processed: */
        if ( FALSE == J1939Dcm_QueueGetMsgRequest(J1939Dcm_DMxQueue[lNodeHdl]) )                                              /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
        {
          /* #34 Enter critical section (Reason: protect data consistency of the queue of events against another request manager interruption). */
          J1939Dcm_EnterCritical();

          /* #35 Indicate the incoming request in the queue of events and set the corresponding channel handle, message, and response address. */
          J1939Dcm_QueueSetMsgRequest(J1939Dcm_DMxQueue[lNodeHdl]);                                                           /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          J1939Dcm_DMxQueue[lNodeHdl].ChannelHdl = lChannelHdl;                                                               /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          J1939Dcm_DMxQueue[lNodeHdl].MessageHdl = lMsgHdl;                                                                   /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
          J1939Dcm_DMxQueue[lNodeHdl].ResponseDA = lResponseAddress;                                                          /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */

          /* #36 Leave critical section. */
          J1939Dcm_ExitCritical();
        }
        /* #37 Otherwise (if currently a DMx request is being processed): */
        else
        {
          /* DM request have already been queued. */
          /* [J1939-73] If for some reason a device can not perform the requested action, then it is required to send a negative acknowledgement.
           *            Implementers be aware that no positive or negative acknowledgement is sent when the request was sent to a global address. */
          /* #38 If the response address of the requested DM is specific: */
          if (J1939DCM_ADDRESS_BROADCAST != lResponseAddress)
          {
            /* #39 Request sending negative acknowledgement from the request manager. */
            (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(lNodeHdl),
                                  J1939Dcm_Cfg_GetChannelId(lChannelHdl),
                                  requestedPgn,
                                  NULL_PTR,
                                  J1939RM_ACK_CANNOT_RESPOND,
                                  lResponseAddress,
                                  priority,
                                  FALSE);
          }
        }
        break;
#endif

        /* #40 In case of DM35: */
#if (J1939DCM_DM35_SUPPORT == STD_ON)
      case J1939DCM_DM35:
        /* #41 Enter critical section (Reason: protect data consistency of the queue of events against another request manager interruption). */
        J1939Dcm_EnterCritical();

        /* #42 Indicate the incoming request in the queue of events and activate the corresponding channel. */
        J1939Dcm_QueueSetMsgRequest(J1939Dcm_DM35Queue[lNodeHdl]);                                                                     /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
        J1939Dcm_DM35SetChannelActive(J1939Dcm_DM35Queue[lNodeHdl].RequestedChannels, lChannelHdl);                                    /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */

        /* #43 Leave critical section. */
        J1939Dcm_ExitCritical();
        break;
#endif /* (J1939DCM_NEED_DM35_PROCESSOR == STD_ON) */

      /* #44 In case of default: */
      default:
        /* #45 Check whether PGN is invalid. */
        J1939Dcm_DetAssertAlways(J1939DCM_API_REQUESTINDICATION, J1939DCM_E_INVALID_PGN)   /* SWS_J1939Dcm_00006 */

        /* [J1939-21] A response is always required from the specified destination(not global) even if it is a NACK indicating that the particular
         *            PGN value is not supported.
         * [J1939-73] Implementers be aware that no positive or negative acknowledgement is sent when the request was sent to a global address. */
         /* #46 If the response address to the requested DM is specific: */
        if (J1939DCM_ADDRESS_BROADCAST != lResponseAddress)
        {
          /* #47 Request sending negative acknowledgement from the request manager. */
          (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(lNodeHdl),
                                J1939Dcm_Cfg_GetChannelId(lChannelHdl),
                                requestedPgn,
                                NULL_PTR,
                                J1939RM_ACK_NEGATIVE,
                                lResponseAddress,
                                priority,
                                FALSE);
        }
        break;
    }
  }
  /* #48 Otherwise (if either the requested node or the referenced channel or both is(are) in offline state): */
  else
  {
    /* Channel is offline; request reception should be impossible */
    /* #49 Check if the communication state is invalid. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_REQUESTINDICATION, J1939DCM_E_INVALID_STATE)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6060 */ /* MD_MSR_STPAR */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 * J1939Dcm_RxIndication()
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
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_RxIndication( PduIdType RxPduId,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) PduInfoPtr )
{
#if (J1939DCM_NEED_IF_RECEPTION == STD_ON)
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lCurrentChannel;
  J1939Dcm_MessageHdlType lMessageHdl;
  PduLengthType           lPayloadLength;
  uint8                   lSourceAddress;
  uint8                   lDestinationAddress;
  boolean                 lAnyNodeOnline;

  /* parameter check */
  /* #10 Check whether the module is initialized. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_RXINDICATION, J1939DCM_E_UNINIT)  /* SWS_J1939Dcm_00004 */
    /* #11 Check whether the ID of the RxPdu is less than the total number of the configured RxPdus. */
  J1939Dcm_DetAssertVReturn(RxPduId < J1939Dcm_Cfg_GetNumberOfRxPdus(), J1939DCM_API_RXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #12 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #13 Check whether the incoming RxPdu defined in the RxPdu table has a valid index in the message table. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_RxPduTable(RxPduId), J1939DCM_API_RXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #14 Check whether at least one valid RxPdu exists in RxPdu table. */
  J1939Dcm_DetAssertVReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedRxPdus(), J1939DCM_API_RXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
# endif
  /* #15 Check whether the pointer to the buffer containing the I-PDU (the payload and the meta data) is NULL. */
  J1939Dcm_DetAssertVReturn(PduInfoPtr != NULL_PTR, J1939DCM_API_RXINDICATION, J1939DCM_E_PARAM_POINTER)

  /* get PDU data from the configuration */
  /* #16 Obtain the local channel and message handles for the incoming RxPdu. */
  lCurrentChannel = J1939Dcm_Cfg_GetChannelHdlByRxPdu(RxPduId);
  lMessageHdl     = J1939Dcm_Cfg_GetMessageHdlByRxPdu(RxPduId);

  /* #17 Analyse meta data (extract length of the payload, source address, and destination address). */
  lPayloadLength      = PduInfoPtr->SduLength - J1939DCM_METADATA_LEN;
  lSourceAddress      = PduInfoPtr->SduDataPtr[(lPayloadLength + J1939DCM_METADATA_SA_OFFSET)];
  lDestinationAddress = PduInfoPtr->SduDataPtr[(lPayloadLength + J1939DCM_METADATA_DA_OFFSET)];

  /* #18 Assume that all the configured node-channel pairs are offline. */
  lAnyNodeOnline = FALSE;
  /* #19 If the diagnostic message is requested from a specific address: */
  if (J1939DCM_ADDRESS_BROADCAST != lDestinationAddress)
  {
    /* #20 If the destination address of the requested DM references a configured node: */
# if (J1939DCM_SINGLE_NODE_OPT == STD_ON)
    /* only one node configured: check address */
    lNodeHdl = 0;
    if (J1939Dcm_Cfg_GetNodeAddress(lNodeHdl) == lDestinationAddress)
# else
    /* node specific request: check status of related node */
    lNodeHdl = J1939Dcm_Cfg_GetNodeHdlByAddr(lDestinationAddress);
    if (J1939DCM_INVALID_NODE_HDL != lNodeHdl)
# endif
    {
      /* #21 Obtain the communication status of the node-channel pair. */
      lAnyNodeOnline = (boolean)(J1939Dcm_ComStateIsOnline(lNodeHdl, lCurrentChannel));
    }
  }
  /* #22 Otherwise (if the diagnostic message is requested globally): */
  else
  {
    /* #23 Check the communication status of all nodes with the requested channel (broadcast request).  */
    J1939Dcm_ForEachNode_Start
    {
      J1939Dcm_ForEachNode_Next;
      lAnyNodeOnline |= (boolean)(J1939Dcm_ComStateIsOnline(lNodeHdl, lCurrentChannel));
    } J1939Dcm_ForEachNode_End
  }

  /* #24 If at least the communication status of a one node-channel pair is online: */
  if (FALSE != lAnyNodeOnline)
  {
    switch (J1939Dcm_Cfg_GetMessageDM(lMessageHdl))
    {
# if (J1939DCM_DM13_SUPPORT == STD_ON)
      /* #25 In case of DM13: */
      case J1939DCM_DM13:
      {
        J1939Dcm_ChannelHdlType lChannelHdl;
        uint32_least            lMessageData;
        uint8                   lBroadcastCommand;
        uint8                   lHoldSignal;

        /* #26 Extract the first 4 bytes of the DM from the payload (the broadcast status of the different bus types [3 bytes] + hold and suspend Signals [1 byte]). */
        lMessageData = J1939Dcm_UtiSduDataTo32Bit(PduInfoPtr->SduDataPtr);
        /* #27 Extract the hold signal (the high nibble of the fourth byte of the DM). */
        lHoldSignal = J1939Dcm_DM13DataGetHoldSignal(lMessageData);

        /* check for each channel if broadcast status need to be changed */
        /* #28 Iterate over the available channels. */
        J1939Dcm_ForEachChannel_Start
        {
          J1939Dcm_ForEachChannel_Next;
          /* #29 If a channel has same bus type as the network where DM13 has been received: */
          if (J1939Dcm_Cfg_GetChannelBusType (lChannelHdl) == J1939Dcm_Cfg_GetChannelBusType (lCurrentChannel))
          {
            /* #30 Update the broadcast command of that channel according to the bus type status of the current network. */
            lBroadcastCommand = J1939Dcm_DM13DataGetBroadcastStatus(lMessageData, J1939DCM_CURRENT_NETWORK);
          }
          /* #31 Otherwise (if a channel has a different bus type than the network where DM13 has been received): */
          else
          {
            /* #32 Update the broadcast command of that channel according to the bus type status of the corresponding network. */
            lBroadcastCommand = J1939Dcm_DM13DataGetBroadcastStatus(lMessageData, J1939Dcm_Cfg_GetChannelBusType (lChannelHdl));
          }

          /* #33 Enter critical section (Reason: protect data consistency of the queue of events against another PduR interruption). */
          J1939Dcm_EnterCritical();
          /* #34 Update the queue of events according to the incoming DM. Indicate that DM13 has not been processed yet.*/
          J1939Dcm_DM13Queue.HoldSignal = lHoldSignal;
          J1939Dcm_DM13Queue.BroadcastCommand[lChannelHdl] = lBroadcastCommand;
          J1939Dcm_DM13Queue.RequestPending = TRUE;
          /* #35 Leave critical section. */
          J1939Dcm_ExitCritical();

        } J1939Dcm_ForEachChannel_End
        break;
        /* Note: NodeId not relevant for DM13; broadcast status is only channel specific and handled for the complete J1939Dcm */
      }
# endif /* (J1939DCM_NEED_DM13_PROCESSOR == STD_ON) */

# if (J1939DCM_DM14_SUPPORT == STD_ON)
      /* #36 In case of DM14: */
      case J1939DCM_DM14:
        /* #37 If the DM is requested from a specific address and the length of the DM is equivalent to a length of a CAN frame (8 bytes): */
        if ( (J1939DCM_ADDRESS_BROADCAST     != lDestinationAddress) &&
             (J1939DCM_MEM_ACCESS_MSG_LENGTH == lPayloadLength) )
        {
          J1939Dcm_MemReqParamType    MemRequestInfo;
          J1939Dcm_AddressInfoType    MemAddressInfo;

          /* #38 Collect addressing information for the application (channel handler and destination and source addresses). */
          MemAddressInfo.ChannelId          = J1939Dcm_Cfg_GetChannelId(lCurrentChannel);
          MemAddressInfo.DestinationAddress = lDestinationAddress;
          MemAddressInfo.SourceAddress      = lSourceAddress;

          /* #39 Collect request contents for the application. */
          MemRequestInfo.Command            = J1939Dcm_DM14GetCommand(PduInfoPtr->SduDataPtr);
          MemRequestInfo.Key                = J1939Dcm_DM14GetKey(PduInfoPtr->SduDataPtr);
          MemRequestInfo.Length             = J1939Dcm_DM14GetLength(PduInfoPtr->SduDataPtr);
          MemRequestInfo.Pointer            = J1939Dcm_DM14GetPointer(PduInfoPtr->SduDataPtr);
          MemRequestInfo.PointerExtension   = J1939Dcm_DM14GetPointerExt(PduInfoPtr->SduDataPtr);
          MemRequestInfo.PointerType        = J1939Dcm_DM14GetPointerType(PduInfoPtr->SduDataPtr);

          /* #40 Check whether a call-out function for the memory access request is configured. */
          J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemRequestIndicationFunc), J1939DCM_API_RXINDICATION, J1939DCM_E_PARAM_CONFIG)         /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
          /* #41 Pass the addressing information and the request contents to the application. */
          J1939Dcm_Cfg_MemRequestIndicationFunc(&MemAddressInfo, &MemRequestInfo);
        }
        /* else: broadcast memory access makes no sense; ignore request as broadcast NACK is also not allowed */
        /*   or: data length must be 8 */
        break;
# endif

# if (J1939DCM_DM17_SUPPORT == STD_ON)
        /* #42 In case of DM17: */
      case J1939DCM_DM17:
        /* #43 If the diagnostic message is requested from a specific address: */
        if ( J1939DCM_ADDRESS_BROADCAST != lDestinationAddress )
        {
          PduInfoType                 BootLoadData;
          J1939Dcm_AddressInfoType    MemAddressInfo;

          /* #44 Collect request contents for the application (message content without meta data). */
          BootLoadData.SduDataPtr = PduInfoPtr->SduDataPtr;
          BootLoadData.SduLength  = lPayloadLength;

          /* #45 Collect addressing information for the application (channel handler and destination and source addresses). */
          MemAddressInfo.ChannelId          = J1939Dcm_Cfg_GetChannelId(lCurrentChannel);
          MemAddressInfo.DestinationAddress = lDestinationAddress;
          MemAddressInfo.SourceAddress      = lSourceAddress;

          /* #46 Check whether a call-out function for the boot load data request is configured. */
          J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemDataRxIndicationFunc), J1939DCM_API_RXINDICATION, J1939DCM_E_PARAM_CONFIG)          /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
          /* #47 Pass the addressing information and the request contents to the application. */
          J1939Dcm_Cfg_MemDataRxIndicationFunc(J1939DCM_MEM_DATA_BOOTLOAD, &MemAddressInfo, &BootLoadData, E_OK);
        }
        /* else: broadcast memory access makes no sense; ignore request as broadcast NACK is also not allowed */
        break;
# endif
      /* #48 Otherwise: */
      default:
        /* #49 If the diagnostic message is requested from a specific address: */
        if (J1939DCM_ADDRESS_BROADCAST != lDestinationAddress)
        {
          /* #50 Request sending negative acknowledgement from the request manager. */
          (void)J1939Rm_SendAck(J1939Dcm_Cfg_GetNodeUserId(lNodeHdl),                                                  /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
                                J1939Dcm_Cfg_GetChannelId(lCurrentChannel),
                                J1939Dcm_Cfg_GetMessagePGN(lMessageHdl),
                                NULL_PTR,
                                J1939RM_ACK_NEGATIVE,
                                lSourceAddress,
                                J1939DCM_PRIORITY_DEFAULT,
                                FALSE);
        }
        break;
    } /* end: switch DiagMessage */
  }
  /* #51 Otherwise (if the communication statuses of all node-channel pairs are offline): */
  else
  {
    /* #52 Notify DET: J1939Dcm is offline. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_RXINDICATION, J1939DCM_E_INVALID_STATE)
  }
  /* else: address not found or broadcast */
#else
  J1939Dcm_IgnoreUnusedArgument(RxPduId)                                                                               /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(PduInfoPtr)                                                                            /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 * J1939Dcm_StartOfReception()
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
 *
 *
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_StartOfReception( PduIdType id,
                                                                  P2VAR(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,                     /* PRQA S 3673 */ /* MD_J1939Dcm_14.2 */
                                                                  PduLengthType TpSduLength,
                                                                  P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) bufferSizePtr )         /* PRQA S 3673 */ /* MD_J1939Dcm_16.7 */
{
  /* #10 If any DM requires TP reception is configured: */
#if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
  J1939Dcm_NodeHdlType        lNodeHdl;
  J1939Dcm_MemDataType        lDataKind;
  BufReq_ReturnType           lBufferResult;

  /* #11 Check whether the module is initialized. */
  J1939Dcm_DetAssertReturn((J1939Dcm_InitState != J1939DCM_STATE_UNINIT), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_UNINIT, BUFREQ_E_NOT_OK)  /* SWS_J1939Dcm_00004 */
  /* #12 Check whether the ID of the RxPdu is less than the total number of the configured RxPdus. */
  J1939Dcm_DetAssertReturn((id < J1939Dcm_Cfg_GetNumberOfRxPdus()), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #13 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #14 Check whether the incoming RxPdu defined in the RxPdu table has a valid index in the message table.*/
  J1939Dcm_DetAssertReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_RxPduTable(id), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #15 Check whether at least one valid RxPdu exists in RxPdu table. */
  J1939Dcm_DetAssertReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedRxPdus(), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
# endif
  /* #16 Check whether the pointer to the buffer of the incoming data from the PduR is NULL. */
  J1939Dcm_DetAssertReturn((info != NULL_PTR), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)
  J1939Dcm_DetAssertReturn((info->SduDataPtr != NULL_PTR), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)
  /* #17 Check whether the length of the data in the PduR buffer is at least the legth of the meta data. */
  J1939Dcm_DetAssertReturn((info->SduLength >= J1939DCM_METADATA_LEN), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_CONFIG, BUFREQ_E_NOT_OK)
  /* #18 Check whether the pointer to the available buffer of the application is NULL. */
  J1939Dcm_DetAssertReturn((bufferSizePtr != NULL_PTR), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)

  /* #19 Obtain the node handle based on the destination address. */
  lNodeHdl = J1939Dcm_Cfg_GetNodeHdlByAddr(info->SduDataPtr[(info->SduLength - J1939DCM_METADATA_LEN) + J1939DCM_METADATA_DA_OFFSET]);

  /* #20 Check whether the obtained node handle is valid. */
  J1939Dcm_DetAssertNodeHdlReturn(lNodeHdl, J1939DCM_API_STARTOFRECEPTION, BUFREQ_E_NOT_OK)

  /* #21 Assume unsuccessful reception. */
  lBufferResult = BUFREQ_E_NOT_OK;
  /* #22 If the communication status of the node-channel pair is online: */
  if ( J1939Dcm_ComStateIsOnline(lNodeHdl, J1939Dcm_Cfg_GetChannelHdlByRxPdu(id)) )                                              /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
  {
    /* #23 Obtain the kind of the incoming message (the definition is equal to related DM). */
    lDataKind = J1939Dcm_Cfg_GetMessageDM( J1939Dcm_Cfg_GetMessageHdlByRxPdu(id) );

    /* #24 If the incoming message is binary data transfer or data security: */
    if ((lDataKind == J1939DCM_MEM_DATA_BINARY) || (lDataKind == J1939DCM_MEM_DATA_SECURITY))
    {

      /* #25 Enter critical section (Reason: protect data consistency of the reception handler state against another PduR interruption). */
      J1939Dcm_EnterCritical();

      /* #26 If there is no other TP reception in progress: */
      if (J1939DCM_INVALID_PDU_ID == J1939Dcm_RxHandlerState.RxPduId)
      {
        /* #27 Update reception handler state (memory access address info from meta data) to be used later in the Copy received data function. */
        J1939Dcm_RxHandlerState.RxAddressInfo.ChannelId          = J1939Dcm_Cfg_GetChannelId(J1939Dcm_Cfg_GetChannelHdlByRxPdu(id));
        J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress = info->SduDataPtr[(info->SduLength - J1939DCM_METADATA_LEN) + J1939DCM_METADATA_DA_OFFSET];
        J1939Dcm_RxHandlerState.RxAddressInfo.SourceAddress      = info->SduDataPtr[(info->SduLength - J1939DCM_METADATA_LEN) + J1939DCM_METADATA_SA_OFFSET];
        J1939Dcm_RxHandlerState.RxPduId = id;

        /* #28 Check whether a call-out function to start receiving the binary data transfer and data security requests is configured. */
        J1939Dcm_DetAssertReturn((NULL_PTR != J1939Dcm_Cfg_MemDataStartOfReceptionFunc), J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_CONFIG, BUFREQ_E_NOT_OK)     /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
        /* #29 Inform the application with the a start of a message reception (DM16 or DM18). */
        lBufferResult = J1939Dcm_Cfg_MemDataStartOfReceptionFunc(lDataKind, &J1939Dcm_RxHandlerState.RxAddressInfo, TpSduLength, bufferSizePtr);
      }
      /* #30 Otherwise (if there is other TP reception in progress): */
      else
      {
        /* #31 Notify DET: parallel reception is not yet supported by the J1939Dcm. */
        J1939Dcm_DetAssertAlways(J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_INVALID_STATE)
      }

      /* #32 Leave critical section. */
      J1939Dcm_ExitCritical();
    }
    /* #33 Otherwise (if the incoming message is not binary data transfer or data security requests): */
    else
    {
      /* #34 Notify DET: message is not supported. */
      J1939Dcm_DetAssertAlways(J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_CONFIG)
    }
  }
  /* #35 Otherwise (if the communication status of the node-channel pair is offline): */
  else
  {
    /* #36 Notify DET: communication state of the concerned channel-node is offline. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_INVALID_STATE)
  }
  /* #37 Return the result of triggering the start of reception. */
  return lBufferResult;
  /* #38 Otherwise (If none of the DMs that require TP reception is configured): */
#else
  /* no DM configured which needs reception via TP */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(info)                                                                                  /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(TpSduLength)                                                                           /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(bufferSizePtr)                                                                         /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #39 Notify DET: invalid configuration. */
  J1939Dcm_DetAssertAlways(J1939DCM_API_STARTOFRECEPTION, J1939DCM_E_PARAM_CONFIG)                                     /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #40 Return unsuccessful triggering start of reception process. */
  return BUFREQ_E_NOT_OK;
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 * J1939Dcm_CopyRxData()
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
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_CopyRxData( PduIdType id,
                                                            P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,
                                                            P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) bufferSizePtr )           /* PRQA S 3673 */ /* MD_J1939Dcm_16.7 */
{
  /* #10 If any DM requires TP reception is configured: */
#if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
  J1939Dcm_NodeHdlType        lNodeHdl;
  BufReq_ReturnType           lBufferResult;
  J1939Dcm_AddressInfoType    lMemAddressInfo;

  /* #11 Check whether the module is initialized. */
  J1939Dcm_DetAssertReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_COPYRXDATA, J1939DCM_E_UNINIT, BUFREQ_E_NOT_OK)  /* SWS_J1939Dcm_00004 */
    /* #12 Check whether the ID of the RxPdu is less than the total number of the configured RxPdus. */
  J1939Dcm_DetAssertReturn(id < J1939Dcm_Cfg_GetNumberOfRxPdus(), J1939DCM_API_COPYRXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #13 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #14 Check whether the incoming RxPdu defined in the RxPdu table has a valid index in the message table. */
  J1939Dcm_DetAssertReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_RxPduTable(id), J1939DCM_API_COPYRXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #15 Check whether at least one valid RxPdu exists in RxPdu table. */
  J1939Dcm_DetAssertReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedRxPdus(), J1939DCM_API_COPYRXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
# endif
  /* #16 Check whether the pointer to the buffer of the incoming data from the PduR is NULL. */
  J1939Dcm_DetAssertReturn(info != NULL_PTR, J1939DCM_API_COPYRXDATA, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)
  /* #17 Check whether the pointer to the available buffer of the application is NULL. */
  J1939Dcm_DetAssertReturn(bufferSizePtr != NULL_PTR, J1939DCM_API_COPYRXDATA, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)

  /* #18 Assume unsuccessful copying process from the PduR buffer to the application buffer. */
  lBufferResult = BUFREQ_E_NOT_OK;

  /* #19 Enter critical section (Reason: protect data consistency of the reception handler state against PduR interruptions). */
  J1939Dcm_EnterCritical();

  /* #20 If the requested message is consistent with the DM indicated by the reception handler state: */
  if (id == J1939Dcm_RxHandlerState.RxPduId)
  {
    /* If RxPduId for reception handler State is set correctly, the other parameters can be expected as correct. */
    /* Checks on data kind and node handle have been done already in the start of reception function          */
    /* #21 Obtain the local node handle. */
    lNodeHdl = J1939Dcm_Cfg_GetNodeHdlByAddr(J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress);

    /* #22 If the communication status of the node-channel pair is online: */
    if ( J1939Dcm_ComStateIsOnline(lNodeHdl, J1939Dcm_Cfg_GetChannelHdlByRxPdu(id)) )                   /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
    {
      /* #23 Prepare for forwarding of the incoming payload to the application. */
      lMemAddressInfo.ChannelId          = J1939Dcm_RxHandlerState.RxAddressInfo.ChannelId;
      lMemAddressInfo.DestinationAddress = J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress;
      lMemAddressInfo.SourceAddress      = J1939Dcm_RxHandlerState.RxAddressInfo.SourceAddress;
      /* #24 Indicate successful preparation for the forwarding process of the payload to the application. */
      lBufferResult = BUFREQ_OK;
    }
    /* #25 Otherwise (if the communication status of the node-channel pair is offline): */
    else
    {
      /* J1939Dcm has been set to offline after StartOfReception! */
      /* #26 Notify DET: communication state of the concerned channel-node is offline. */
      J1939Dcm_DetAssertAlways(J1939DCM_API_COPYRXDATA, J1939DCM_E_INVALID_STATE)
    }
  }
  /* #27 Otherwise (if the requested message is inconsistent with the DM indicated by the reception handler state): */
  else
  {
    /* StartOfreception has not yet been called; no CopyRxData expected */
    /* #28 Notify DET: start of reception has not yet been called; no Copy Rx data expected. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_COPYRXDATA, J1939DCM_E_INVALID_STATE)
  }

  /* #29 Leave critical section. */
  J1939Dcm_ExitCritical();

  /* #30 If the preparation for the copying process from PduR buffer to the application buffer is successful: */
  if (BUFREQ_OK == lBufferResult)
  {
    /* #31 Check whether a call-out function to copy received data for the binary data transfer and data security requests is configured. */
    J1939Dcm_DetAssertReturn((NULL_PTR != J1939Dcm_Cfg_MemDataCopyRxFunc), J1939DCM_API_COPYRXDATA, J1939DCM_E_PARAM_CONFIG, BUFREQ_E_NOT_OK)    /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
      /* #32 Trigger the application to copy the data from PduR. */
      /* The application is notified each time this function is executed. Therefore, this function is reentrant. */
    lBufferResult = J1939Dcm_Cfg_MemDataCopyRxFunc( J1939Dcm_Cfg_GetMessageDM(J1939Dcm_Cfg_GetMessageHdlByRxPdu(id)),
                                                    &lMemAddressInfo, info, bufferSizePtr);
  }

  /* #33 Return the results of the copying process form the PduR buffer to the application buffer. */
  return lBufferResult;
  /* #34 Otherwise (If none of the DMs that require TP reception is configured):*/
#else
  /* no DM configured which needs reception via TP */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(info)                                                                                  /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(bufferSizePtr)                                                                         /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #35 Notify DET: invalid configuration. */
  J1939Dcm_DetAssertAlways(J1939DCM_API_COPYRXDATA, J1939DCM_E_PARAM_CONFIG)
  /* #36 Return unsuccessful copying process. */
  return BUFREQ_E_NOT_OK;
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 * J1939Dcm_TpRxIndication
 **********************************************************************************************************************/
/*! \brief      Called after an I-PDU has been received via the TP API
 *  \param[in]  PduIdType       id        ID of the received I-PDU
 *              Std_ReturnType  result    Result of the reception.
 *  \pre        Reception must have been started
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_TpRxIndication( PduIdType id,
                                                   Std_ReturnType result )
{
#if (J1939DCM_NEED_TP_RECEPTION == STD_ON)
  J1939Dcm_NodeHdlType        lNodeHdl;
  J1939Dcm_AddressInfoType    lMemAddressInfo;
  boolean                     lCallApplication;

  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_TPRXINDICATION, J1939DCM_E_UNINIT)  /* SWS_J1939Dcm_00004 */
  J1939Dcm_DetAssertVReturn(id < J1939Dcm_Cfg_GetNumberOfRxPdus(), J1939DCM_API_TPRXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  J1939Dcm_DetAssertVReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_RxPduTable(id), J1939DCM_API_TPRXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  J1939Dcm_DetAssertVReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedRxPdus(), J1939DCM_API_TPRXINDICATION, J1939DCM_E_INVALID_PDU_SDU_ID)
# endif

  lCallApplication = FALSE;

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  J1939Dcm_EnterCritical();

  if (id == J1939Dcm_RxHandlerState.RxPduId)
  {
    /* if RxPduId for RxHandlerState is set correctly, the other parameters can be expected as correct  */
    /* checks of data kind and node handle have already been done on J1939Dcm_StartOfReception          */
    lNodeHdl = J1939Dcm_Cfg_GetNodeHdlByAddr(J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress);

    if ( J1939Dcm_ComStateIsOnline(lNodeHdl, J1939Dcm_Cfg_GetChannelHdlByRxPdu(id)) )                   /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
    {
      /* node / channel is still online */
      lMemAddressInfo.ChannelId          = J1939Dcm_RxHandlerState.RxAddressInfo.ChannelId;
      lMemAddressInfo.DestinationAddress = J1939Dcm_RxHandlerState.RxAddressInfo.DestinationAddress;
      lMemAddressInfo.SourceAddress      = J1939Dcm_RxHandlerState.RxAddressInfo.SourceAddress;
      lCallApplication = TRUE;
    }
    else
    {
      /* J1939Dcm has been set to offline after StartOfReception! */
      J1939Dcm_DetAssertAlways(J1939DCM_API_TPRXINDICATION, J1939DCM_E_INVALID_STATE)
    }

    /* successfully or not: TP reception is now finished */
    J1939Dcm_RxHandlerReset();
  }
  else
  {
    /* StartOfreception has not yet been called; no TpRxIndication expected */
    J1939Dcm_DetAssertAlways(J1939DCM_API_TPRXINDICATION, J1939DCM_E_INVALID_STATE)
  }

  J1939Dcm_ExitCritical();
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

  if (FALSE != lCallApplication)
  {
    J1939Dcm_DetAssertVReturn((NULL_PTR != J1939Dcm_Cfg_MemDataRxIndicationFunc), J1939DCM_API_TPRXINDICATION, J1939DCM_E_PARAM_CONFIG)          /* PRQA S 0428 */ /* MD_J1939Dcm_16.9 */
    J1939Dcm_Cfg_MemDataRxIndicationFunc( J1939Dcm_Cfg_GetMessageDM(J1939Dcm_Cfg_GetMessageHdlByRxPdu(id)),
                                          &lMemAddressInfo, NULL_PTR, result );
  }

#else
  /* no DM configured which needs reception via TP */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(result)                                                                                /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_DetAssertAlways(J1939DCM_API_TPRXINDICATION, J1939DCM_E_PARAM_CONFIG)
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 * J1939Dcm_CopyTxData()
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
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_CopyTxData( PduIdType id,
                                                            P2VAR(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,
                                                            P2VAR(RetryInfoType, AUTOMATIC, J1939DCM_APPL_DATA) retry,                 /* PRQA S 3673 */ /* MD_J1939Dcm_14.2 */
                                                            P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) availableDataPtr )
{
  /* This function is called several times by the PduR to transmit a response of a DM. */
  /* The confirmation only comes when the complete message is transmitted, then the application is notified. */
  /* Therefore, this function cannot be reentrant. */
  /* #10 If any DM requires TP transmission is configured: */
#if (J1939DCM_NEED_TP_TRANSMISSION == STD_ON)
  J1939Dcm_ChannelHdlType lChannelHdl;
  uint8_least             lDiagMsg;
  PduLengthType           lCopyIndex;
  BufReq_ReturnType       lResult;

  J1939Dcm_IgnoreUnusedArgument(retry)                                                                                 /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

  /* #11 Check whether the module is initialized. */
  J1939Dcm_DetAssertReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_COPYTXDATA, J1939DCM_E_UNINIT, BUFREQ_E_NOT_OK)  /* SWS_J1939Dcm_00004 */
  /* #12 Check whether the ID of the TxPdu is less than the total number of the configured TxPdus. */
  J1939Dcm_DetAssertReturn(id < J1939Dcm_Cfg_GetNumberOfTxPdus(), J1939DCM_API_COPYTXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #13 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #14 Check whether the incoming TxPdu defined in the TxPdu table has a valid index in the message table. */
  J1939Dcm_DetAssertReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_TxPduTable(id), J1939DCM_API_COPYTXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
  /* #15 Check whether at least one valid TxPdu exists in TxPdu table. */
  J1939Dcm_DetAssertReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedTxPdus(), J1939DCM_API_COPYTXDATA, J1939DCM_E_INVALID_PDU_SDU_ID, BUFREQ_E_NOT_OK)
# endif
  /* #16 Check whether the pointer to the buffer provided by the PduR is NULL. */
  J1939Dcm_DetAssertReturn(info != NULL_PTR, J1939DCM_API_COPYTXDATA, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)
  /* #17 Check whether the pointer to the available data in the buffer of the transmission handler state is NULL. */
  J1939Dcm_DetAssertReturn(availableDataPtr != NULL_PTR, J1939DCM_API_COPYTXDATA, J1939DCM_E_PARAM_POINTER, BUFREQ_E_NOT_OK)

  /* #18 Obtain the local channel and message handles for the requested TxPdu. */
  lChannelHdl    = J1939Dcm_Cfg_GetChannelHdlByTxPdu(id);
  lDiagMsg       = J1939Dcm_Cfg_GetMessageDM( J1939Dcm_Cfg_GetMessageHdlByTxPdu(id) );
  /* #19 Assume unsuccessful copying process from the buffer of the transmission handler state to the buffer provided by the PduR. */
  lResult        = BUFREQ_E_NOT_OK;

  /* #20 Check whether the requested channel is configured. */
  J1939Dcm_DetAssertChannelHdlReturn(lChannelHdl, J1939DCM_API_COPYTXDATA, BUFREQ_E_NOT_OK)                            /* PRQA S 3355,3356,3359,3201 */ /* MD_J1939Dcm_13.7 */

  /* #21 Enter critical section (Reason: protect data consistency of the transmission handler state against another PduR interruption). */
  J1939Dcm_EnterCritical();

  /* #22 If the requested message is consistent with the DM indicated by the transmission handler state: */
  if (lDiagMsg == J1939Dcm_TxHandlerState.DM)
  {
    /* Tx Handler prepared and Pdu-DM is consistent */
    /* #23 If the communication status of the node-channel pair is online: */
    if ( J1939Dcm_ComStateIsOnline(J1939Dcm_TxHandlerState.NodeHdl, lChannelHdl) )
    {
      /* #24 If there is still data in the buffer of the transmission handler state to be copied or the PduR requires to copy no data (request for buffer size): */
      if ( (J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl] < J1939Dcm_TxHandlerState.Buffer->SduLength) ||
           (0 == info->SduLength) )
      {
        /* message not yet completely transmitted or request for buffer size */
        /* #25 If there is enough buffer remaining in the transmission handler state to fill the buffer provided by the PduR: */
        if ( info->SduLength <= (J1939Dcm_TxHandlerState.Buffer->SduLength - J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl]) )
        {
          /* enough buffer remaining; copy data */
          /* #26 Copy the data. */
          lCopyIndex = info->SduLength;
          while (0 < lCopyIndex)
          {
            lCopyIndex--;
            info->SduDataPtr[lCopyIndex] = J1939Dcm_TxHandlerState.Buffer->SduDataPtr[(J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl] + lCopyIndex)];
          }

          /* #27 Update the buffer index and the pointer to the available data in the buffer of the transmission handler state. */
          J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl] += info->SduLength;

          *availableDataPtr = (PduLengthType)(J1939Dcm_TxHandlerState.Buffer->SduLength - J1939Dcm_TxHandlerState.BufferIndex[lChannelHdl]);
          /* #28 Return successful copying process. */
          lResult = BUFREQ_OK;
        }
        /* else: lower layer requests more data than overall message length; reject! */
      }
      /* else: message already completed; reject! */
    }
    /* #29 Otherwise (if the communication status of the node-channel pair is offline): */
    else
    {
      /* channel / node is offline */
      /* #30 Notify DET: communication state of the concerned channel-node is offline. */
      J1939Dcm_DetAssertAlways(J1939DCM_API_COPYTXDATA, J1939DCM_E_INVALID_STATE)
    }
  }
  /* else: TxHandler not prepared or not prepared for requested DM */

  /* #31 Leave critical section. */
  J1939Dcm_ExitCritical();

  /* #32 Return the copying results. */
  return lResult;
  /* #33 Otherwise (If none of the DMs that require TP transmission is configured): */
#else
  /* no DM configured which needs transmission via TP */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(info)                                                                                  /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(retry)                                                                                 /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(availableDataPtr)                                                                      /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #34 Notify DET: invalid configuration. */
  J1939Dcm_DetAssertAlways(J1939DCM_API_COPYTXDATA, J1939DCM_E_PARAM_CONFIG)

  /* #35 Return unsuccessful copying process. */
  return BUFREQ_E_NOT_OK;
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 * J1939Dcm_TpTxConfirmation()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_TpTxConfirmation( PduIdType id,
                                                     Std_ReturnType result )
{
  /* #10 If any DM requires TP transmission is configured: */
#if (J1939DCM_NEED_TP_TRANSMISSION == STD_ON)
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;
  uint8_least             lDiagMsg;
  uint8_least             lUnconfChannels;

  /* parameter check */
  /* #11 Check whether the module is initialized. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_UNINIT)  /* SWS_J1939Dcm_00004 */
  /* #12 Check whether the ID of the TxPdu is less than the total number of the configured TxPdus. */
  J1939Dcm_DetAssertVReturn(id < J1939Dcm_Cfg_GetNumberOfTxPdus(), J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #13 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #14 Check whether the incoming TxPdu defined in the TxPdu table has a valid index in the message table. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_TxPduTable(id), J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #15 Check whether at least one valid TxPdu exists in TxPdu table. */
  J1939Dcm_DetAssertVReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedTxPdus(), J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
# endif

  /* #16 Obtain the local channel handle and the confirmed message from the incoming TxPdu Id. */
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlByTxPdu(id);
  lDiagMsg    = J1939Dcm_Cfg_GetMessageDM(J1939Dcm_Cfg_GetMessageHdlByTxPdu(id));

  /* #17 Check whether the obtained local channel handle is valid.  */
  J1939Dcm_DetAssertChannelHdlVReturn((lChannelHdl), J1939DCM_API_TPTXCONFIRMATION)                                    /* PRQA S 3355,3356,3359,3201 */ /* MD_J1939Dcm_13.7 */

  /* #18 Enter critical section (Reason: protect data consistency of the transmission handler state). */
  J1939Dcm_EnterCritical();

  /* #19 If the requested message is consistent with the DM indicated by the transmission handler state and at least one channel is still waiting for transmission confirmation: */
  if ( (lDiagMsg == J1939Dcm_TxHandlerState.DM) &&
       (0 < J1939Dcm_TxHandlerState.UnconfirmedChannels) )
  {
    /* #20 Mark the channel as confirmed (a transmission is in progress for the received TxPdu).  */
    J1939Dcm_TxHandlerState.UnconfirmedChannels--;
    lUnconfChannels = J1939Dcm_TxHandlerState.UnconfirmedChannels;
    /* TxHandler ensures that only one node at a time can trigger a transmission (Via transmission handler prepare) */
    /* therefore this confirmation must belong to the node which currently uses the TxHandler */
    lNodeHdl        = J1939Dcm_TxHandlerState.NodeHdl;

    /* #21 Leave critical section. */
    J1939Dcm_ExitCritical();

    /* #22 If confirmations for all channels belong to the current node are received: */
    if (0 == lUnconfChannels)
    {
      /* #23 Finalize the transmission (on the node level). */
      J1939Dcm_TxHandlerConfirm(lNodeHdl, lDiagMsg, result);
    } /* else: there are still channels left waiting for a Tx Confirmation (may happen on DM1 broadcast transmission) */
  }
  /* #24 Otherwise (if the requested message is not consistent with the DM indicated by the transmission handler state or there are no channels still waiting for transmission confirmation):*/
  else
  {
    /* unexpected confirmation; ignore */
    /* #25 Leave critical section. */
    J1939Dcm_ExitCritical();
  }

  /* #26 Otherwise (If none of the DMs that require TP transmission is configured): */
#else
  /* no DM configured which needs transmission via TP */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(result)                                                                                /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #27 Notify DET: invalid configuration. */
  J1939Dcm_DetAssertAlways(J1939DCM_API_TPTXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 * J1939Dcm_TxConfirmation()
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
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_TxConfirmation( PduIdType id )
{
  /* #10 If any DM requires TP transmission is configured: */
#if (J1939DCM_NEED_IF_TRANSMISSION == STD_ON)
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;
  uint8_least             lDiagMsg;

  /* parameter check */
  /* #11 Check whether the module is initialized. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_TXCONFIRMATION, J1939DCM_E_UNINIT)
  /* #12 Check whether the ID of the TxPdu is less than the total number of the configured TxPdus. */
  J1939Dcm_DetAssertVReturn(id < J1939Dcm_Cfg_GetNumberOfTxPdus(), J1939DCM_API_TXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #13 If the configuration variant does not indicate precompile: */
# if (J1939DCM_CONFIGURATION_VARIANT != J1939DCM_CONFIGURATION_VARIANT_PRECOMPILE)
  /* #14 Check whether the incoming TxPdu defined in the TxPdu table has a valid index in the message table. */
  J1939Dcm_DetAssertVReturn(J1939Dcm_IsCfg_MessageTableUsedOfCfg_TxPduTable(id), J1939DCM_API_TXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
  /* #15 Check whether at least one valid TxPdu exists in TxPdu table. */
  J1939Dcm_DetAssertVReturn(0 < J1939Dcm_Cfg_GetNumberOfExistedTxPdus(), J1939DCM_API_TXCONFIRMATION, J1939DCM_E_INVALID_PDU_SDU_ID)
# endif

  /* #16 Obtain the local channel handle and the confirmed message from the incoming TxPdu Id. */
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlByTxPdu(id);
  lDiagMsg    = J1939Dcm_Cfg_GetMessageDM(J1939Dcm_Cfg_GetMessageHdlByTxPdu(id));

  /* #17 Check whether the obtained local channel handle is valid. */
  J1939Dcm_DetAssertChannelHdlVReturn((lChannelHdl), J1939DCM_API_TXCONFIRMATION)                                      /* PRQA S 3355,3356,3359,3201 */ /* MD_J1939Dcm_13.7 */

  /* #18 Enter critical section (Reason: protect data consistency of the transmission handler state). */
  J1939Dcm_EnterCritical();

  /* #19 If the requested message is consistent with the DM indicated by the transmission handler state: */
  if ( lDiagMsg == J1939Dcm_TxHandlerState.DM )
  {
    /* #20 Update the node handle of the transmission handler state. */
    lNodeHdl = J1939Dcm_TxHandlerState.NodeHdl;

    /* #21 Leave critical section. */
    J1939Dcm_ExitCritical();

    /* note: as all IF messages are always transmitted on only one channel, transmission confirmation counter is not required like in TP transmission confirmation function. */
    /* notify message handler (for IF transmission, result is always ok) */
    /* #22 Finalize the transmission (on the node level). */
    J1939Dcm_TxHandlerConfirm(lNodeHdl, lDiagMsg, E_OK);
  }
  /* #23 Otherwise (if the requested message is not consistent with the DM indicated by the transmission handler state): */
  else
  {
    /* unexpected confirmation; ignore */
    /* #24 Leave critical section. */
    J1939Dcm_ExitCritical();
  }
  /* #25 Otherwise (If none of the DMs that require TP transmission is configured): */
#else
  /* no DM configured which needs transmission via IF */
  J1939Dcm_IgnoreUnusedArgument(id)                                                                                    /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  /* #26 Notify DET: invalid configuration. */
  J1939Dcm_DetAssertAlways(J1939DCM_API_TXCONFIRMATION, J1939DCM_E_PARAM_CONFIG)
#endif

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 * J1939Dcm_DemTriggerOnDTCStatus
 **********************************************************************************************************************/
/*! \brief      Called when the status byte of an UDS DTC changes
 *  \param[in]  uint32  DTC           Diagnostic Trouble Code in UDS format
 *              uint8   DTCStatusOld  DTC status byte before change
 *              uint8   DTCStatusNew  DTC status byte after change
 *  \pre        Module must be initialized
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_DemTriggerOnDTCStatus( uint32 DTC,
                                                                    uint8 DTCStatusOld,
                                                                    uint8 DTCStatusNew )
{
  J1939Dcm_DetAssertReturn(J1939Dcm_InitState != J1939DCM_STATE_UNINIT, J1939DCM_API_DEMTRIGGERONDTCSTATUS, J1939DCM_E_UNINIT, E_NOT_OK)  /* SWS_J1939Dcm_00004 */

  J1939Dcm_IgnoreUnusedArgument(DTC)                                                                                   /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(DTCStatusOld)                                                                          /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */
  J1939Dcm_IgnoreUnusedArgument(DTCStatusNew)                                                                          /* PRQA S 3112 */ /* MD_J1939Dcm_14.2 */

#if (J1939DCM_NEED_DM1_PROCESSOR == STD_ON)
# if (J1939DCM_NEED_DM13_PROCESSOR == STD_ON)
  if (0 != J1939Dcm_DM13State.NumActiveBroadcastChannels)
# endif
  {
    J1939Dcm_NodeHdlType lNodeHdl;

    /* queue DTC status change for all nodes */
    J1939Dcm_ForEachNode_Start
    {
      J1939Dcm_ForEachNode_Next;

      /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
      J1939Dcm_EnterCritical();

      J1939Dcm_QueueSetDTCChange(J1939Dcm_DM1Queue[lNodeHdl]);

      J1939Dcm_ExitCritical();
      /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    } J1939Dcm_ForEachNode_End
  } /* else: broadcast is disabled on all channels; transmission will be rejected anyway by BswM */
#endif /* (J1939DCM_NEED_DM1_PROCESSOR == STD_ON) */

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (J1939DCM_DM15_SUPPORT == STD_ON)
/***********************************************************************************************************************
 * J1939Dcm_MemResponseTransmit()
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
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_MemResponseTransmit (P2CONST(J1939Dcm_AddressInfoType, AUTOMATIC, J1939DCM_APPL_DATA) AddressInfo,
                                                                  P2CONST(J1939Dcm_MemRespParamType, AUTOMATIC, J1939DCM_APPL_DATA) MemResponseParameter)
{
  /* This function is called by the application to transmit a response of a DM. */
  /* The application is notified when J1939Dcm receives a confirmation. */
  /* Therefore, this function cannot be reentrant. */
  Std_ReturnType          TxResult;
  PduIdType               lTxPduId;
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;

  /* #10 Assume unsuccessful transmission. */
  TxResult    = E_NOT_OK;
  /* #11 Obtain the local source node and the channel handles from the incoming addressing information. */
  lNodeHdl    = J1939Dcm_Cfg_GetNodeHdlByAddr(AddressInfo->SourceAddress);
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlById(AddressInfo->ChannelId);

  /* #12 Check whether the source node and the channel are configured. */
  J1939Dcm_DetAssertNodeHdlReturn(lNodeHdl, J1939DCM_API_MEMRESPONSETRANSMIT, E_NOT_OK)
  J1939Dcm_DetAssertChannelHdlReturn(lChannelHdl, J1939DCM_API_MEMRESPONSETRANSMIT, E_NOT_OK)

  /* #13 If the communication status of the source node-channel pair is online: */
  if ( J1939Dcm_ComStateIsOnline(lNodeHdl, lChannelHdl) )                                                                           /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
  {
    /* #14 Obtain the corresponding TxPdu Id for DM15 transmitted from the source node via the requested channel. */
    lTxPduId = J1939Dcm_GetPeerTxPduByDM(lNodeHdl, lChannelHdl, J1939DCM_DM15);

    /* #15 If the TxPdu Id is valid: */
    if (J1939DCM_INVALID_PDU_ID != lTxPduId)
    {
      /* #16 Enter critical section (Reason: protect data consistency of the memory handler state against another CDD interruption). */
      J1939Dcm_EnterCritical();

      /* #17 If there is no memory access message (DM15, DM16, or DM18) to be transmitted: */
      if (J1939DCM_DM_UNDEF == J1939Dcm_MemHandlerState.DiagMsg)
      {
        /* #18 Store required information to transmit DM15 on task level. */
        J1939Dcm_MemHandlerState.DiagMsg            = J1939DCM_DM15;
        J1939Dcm_MemHandlerState.TxPduId            = lTxPduId;
        J1939Dcm_MemHandlerState.NodeHdl            = lNodeHdl;
        J1939Dcm_MemHandlerState.ChannelHdl         = lChannelHdl;
        J1939Dcm_MemHandlerState.DestinationAddress = AddressInfo->DestinationAddress;

        /* #19 Build DM15 content without taking into account meta data. */
        J1939Dcm_MemHandlerState.DM15Buffer[0] = (uint8)(MemResponseParameter->Length & 0x00FFu);
        J1939Dcm_MemHandlerState.DM15Buffer[1] = (uint8)( (uint8)((MemResponseParameter->Length & 0x0700u) >> 3) |
                                                 (uint8)((MemResponseParameter->Status & 0x07u)   << 1) );
        J1939Dcm_MemHandlerState.DM15Buffer[2] = J1939Dcm_Uti24BitGetLoByte (MemResponseParameter->ErrorIndicator);
        J1939Dcm_MemHandlerState.DM15Buffer[3] = J1939Dcm_Uti24BitGetMidByte(MemResponseParameter->ErrorIndicator);
        J1939Dcm_MemHandlerState.DM15Buffer[4] = J1939Dcm_Uti24BitGetHiByte (MemResponseParameter->ErrorIndicator);
        J1939Dcm_MemHandlerState.DM15Buffer[5] = MemResponseParameter->EDCPExtension;
        J1939Dcm_MemHandlerState.DM15Buffer[6] = J1939Dcm_Uti16BitGetLoByte(MemResponseParameter->Seed);
        J1939Dcm_MemHandlerState.DM15Buffer[7] = J1939Dcm_Uti16BitGetHiByte(MemResponseParameter->Seed);

        J1939Dcm_MemHandlerState.TxData.SduDataPtr = J1939Dcm_MemHandlerState.DM15Buffer;
        J1939Dcm_MemHandlerState.TxData.SduLength  = J1939DCM_MEM_ACCESS_MSG_LENGTH;

        /* #20 Indicate successful transmission. */
        TxResult = E_OK;
      }
      /* #21 Otherwise (if there is memory access message (DM15, DM16, or DM18) to be transmitted): */
      else
      {
        /* #22 Notify DET: J1939Dcm memory handler is already in use; reentrancy is not allowed. */
        J1939Dcm_DetAssertAlways(J1939DCM_API_MEMRESPONSETRANSMIT, J1939DCM_E_INVALID_STATE)
      }

      /* #23 Leave critical section. */
      J1939Dcm_ExitCritical();
    }
    /* #24 Otherwise (if the TxPdu Id is invalid): */
    else
    {
      /* message not supported on node/channel */
      /* #25 Notify DET: invalid configuration. */
      J1939Dcm_DetAssertAlways(J1939DCM_API_MEMRESPONSETRANSMIT, J1939DCM_E_PARAM_CONFIG)
    }
  }
  /* #26 Otherwise (if the communication status of the source node-channel pair is offline): */
  else
  {
    /* no transmission allowed in offline state */
    /* #27 Notify DET: communication state of the concerned channel-node is offline. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_MEMRESPONSETRANSMIT, J1939DCM_E_INVALID_STATE)
  }

  /* #28 Return the transmission results. */
  return TxResult;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_DM15_SUPPORT == STD_ON) */

#if ( (J1939DCM_DM16_SUPPORT == STD_ON) || (J1939DCM_DM18_SUPPORT == STD_ON) )
/***********************************************************************************************************************
 * J1939Dcm_MemDataTransmit()
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
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_MemDataTransmit      (J1939Dcm_MemDataType DataKind,
                                                                   P2CONST(J1939Dcm_AddressInfoType, AUTOMATIC, J1939DCM_APPL_DATA) AddressInfo,
                                                                   P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) PduInfo)
{
  PduIdType               lTxPdu;
  Std_ReturnType          TxResult;
  J1939Dcm_NodeHdlType    lNodeHdl;
  J1939Dcm_ChannelHdlType lChannelHdl;

  /* API only allowed for binary and security data */
  J1939Dcm_DetAssertReturn( ((DataKind == J1939DCM_MEM_DATA_BINARY) || (DataKind == J1939DCM_MEM_DATA_SECURITY)), J1939DCM_API_MEMDATATRANSMIT, J1939DCM_E_INVALID_PGN, E_NOT_OK )
  
  /* #10 Assume unsuccessful transmission. */
  TxResult = E_NOT_OK;

  /* #11 Obtain the local source node and the channel handles from the incoming addressing information. */
  /* this node can be identified by the source address */
  lNodeHdl    = J1939Dcm_Cfg_GetNodeHdlByAddr(AddressInfo->SourceAddress);
  /* channel is identified via ComM ChannelId */
  lChannelHdl = J1939Dcm_Cfg_GetChannelHdlById(AddressInfo->ChannelId);

  /* #12 Check whether the source node and the channel are configured. */
  J1939Dcm_DetAssertNodeHdlReturn(lNodeHdl, J1939DCM_API_MEMRESPONSETRANSMIT, E_NOT_OK)
  J1939Dcm_DetAssertChannelHdlReturn(lChannelHdl, J1939DCM_API_MEMRESPONSETRANSMIT, E_NOT_OK)

  /* #13 If the communication status of the source node-channel pair is online: */
  if ( J1939Dcm_ComStateIsOnline(lNodeHdl, lChannelHdl) )                                                                       /* PRQA S 3689 */ /* MD_J1939Dcm_21.1a */
  {
    /*#14 Obtain the corresponding TxPdu Id for DM16 or DM18 transmitted from the source node via the requested channel. */
    lTxPdu = J1939Dcm_GetPeerTxPduByDM(lNodeHdl, lChannelHdl, DataKind);
    /* #15 If the TxPdu Id is valid: */
    if (J1939DCM_INVALID_PDU_ID != lTxPdu)
    {

      /* #16 Enter critical section (Reason: protect data consistency of the memory handler state against another CDD interruption). */
      J1939Dcm_EnterCritical();
      /* #17 If there is no memory access message (DM15, DM16, or DM18) to be transmitted: */
      if (J1939DCM_DM_UNDEF == J1939Dcm_MemHandlerState.DiagMsg)
      {
        /* #18 Store required information to transmit DM16 or DM18 on task level. */
        J1939Dcm_MemHandlerState.DiagMsg            = DataKind;
        J1939Dcm_MemHandlerState.TxPduId            = lTxPdu;
        J1939Dcm_MemHandlerState.NodeHdl            = lNodeHdl;
        J1939Dcm_MemHandlerState.ChannelHdl         = lChannelHdl;
        J1939Dcm_MemHandlerState.DestinationAddress = AddressInfo->DestinationAddress;

        /* #19 Reference the data to be transmitted. */
        J1939Dcm_MemHandlerState.TxData.SduDataPtr  = PduInfo->SduDataPtr;
        J1939Dcm_MemHandlerState.TxData.SduLength   = PduInfo->SduLength;

        /* #20 Indicate successful transmission. */
        TxResult = E_OK;
      }
      /* #21 Otherwise (if there is memory access message (DM15, DM16, or DM18) to be transmitted): */
      else
      {
        /* #22 Check whether J1939Dcm is not busy to transmit a memory access message. */
        J1939Dcm_DetAssertAlways(J1939DCM_API_MEMDATATRANSMIT, J1939DCM_E_INVALID_STATE)
      }

      /* #23 Leave critical section. */
      J1939Dcm_ExitCritical();
    }
    /* #24 Otherwise (if the TxPdu Id is invalid):*/
    else
    {
      /* message not supported by given node on given channel */
      /* #25 Check whether the TxPdu Id is valid. */
      J1939Dcm_DetAssertAlways(J1939DCM_API_MEMDATATRANSMIT, J1939DCM_E_PARAM_CONFIG)
    }
  }
  /* #26 Otherwise (if the communication status of the source node-channel pair is offline): */
  else
  { 
    /* #27 Notify DET: no transmission allowed in offline state. */
    J1939Dcm_DetAssertAlways(J1939DCM_API_MEMDATATRANSMIT, J1939DCM_E_INVALID_STATE)
  }

  /* #28 Return the transmission results. */
  return TxResult;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (J1939DCM_DM17_SUPPORT == STD_ON) || (J1939DCM_DM18_SUPPORT == STD_ON) */

#define J1939DCM_STOP_SEC_CODE
#include "MemMap.h"                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************
 module specific MISRA deviations:
************************************
  MD_J1939Dcm_12.6:
    Description: Rule 12.6: The operands of logical operators (&&, || and !) should be effectively Boolean. Expressions that are effectively Boolean should not be used as operands to operators other than (&&, || and !).
    Reason:      result of boolean operation is OR'ed with a local variable to avoid additional if-statement.
    Risk:        logical and bitwise operators may be confused
    Prevention:  code inspection

  MD_J1939Dcm_9.1:
    Description: Rule 9.1: All automatic variables shall have been assigned a value before being used.
    Reason:      the variable is only set under special conditions which are stored in another variable. This variable is checked before use, so the first variable is always set at this point.
    Risk:        variable may be not set if check before usage is not correct
    Prevention:  code inspection

  MD_J1939Dcm_16.9:
    Description: Rule 16.9: A function identifier shall only be used with either a preceding &, or with a parenthesized parameter list, which may be empty.
    Reason:      function name is generated. It is used without () to check before call if a NULL pointer have been configured
    Risk:        none
    Prevention:  n/a

  MD_J1939Dcm_14.2
    Description: Rule 14.2: All non-null statements shall either (i) have at least one side-effect however executed, or (ii) cause control flow to change.
    Reason:      state has no side effect because it is used to avoid compiler warning for unused parameters; parameter is unused because of API definition
    Risk:        none
    Prevention:  n/a

  MD_J1939Dcm_16.7
    Description: Rule 16.7: A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object.
    Reason:      A pointer parameter in a function prototye is declared as a pointer to variable becasue it is used to call another function that takes a pointer to variable as an argument.
    Risk:        none
    Prevention:  n/a

  MD_J1939Dcm_21.1a
    Description: Indexing array with value that will apparently be out of bounds.
    Reason:      the array index is derived from another variable, which is checked before. This implicit dependency between index and variable can't be detected by QAC.
    Risk:        index may become invalid after code change
    Prevention:  code inspection

  MD_J1939Dcm_21.1b
    Description: An integer expression with a value that is definitely negative is being converted to an unsigned type.
    Reason:      When using the '~' operator, the result is signed and the MSB is set, so numeric interpretation is negative.
    Risk:        none; operator is only used for bit masks which are not used as numerical values, so the signedness is irrelevant
    Prevention:  n/a

  MD_J1939Dcm_5.1
    Description: Rule 5.1: Identifiers (internal and external) shall not rely on the significance of more than 31 characters.
    Reason:      Code maintainability requires structured names conflicting with this limitation.
    Risk:        Compilation errors are possible.
    Prevention:  Compiler compatibility is tested during integration.

  MD_J1939Dcm_13.7
    Description: Rule 13.7: Boolean operations whose results are invariant shall not be permitted.
    Reason:      Due to optimization, some variables may be generated as defines, so the values are already known to the preprocessor, conditions are invariant an branches unreachable.
    Risk:        Unreachable code.
    Prevention:  code inspection.
*/

/**********************************************************************************************************************
 *  END OF FILE: J1939DCM.C
 *********************************************************************************************************************/
