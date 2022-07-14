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
 /*!        \file  FrNm.h
 *         \brief  AUTOSAR FlexRay Network Management
 *
 *       \details  Header of AUTOSAR FlexRay Network Management
 *                 AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Drescher               vismdr        Vector Informatik GmbH
 *  Markus Schuster               vissrk        Vector Informatik GmbH
 *  Marco Pfalzgraf               vismpf        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2012-04-25  vismdr  ESCAN00058397 Creation of component for AUTOSAR Release 4.0
 *  1.01.00   2012-09-12  vismdr  ESCAN00060592 Using plausibility macro checks more consequently
 *                                ESCAN00060628 Introducing the usage of FRNM_E_PDU_ID_INVALID
 *                                ESCAN00061349 Removed instanceID from FrNm_GetVersionInfo
 *                                ESCAN00063840 Fixed conditions for accepting Communication Control Requests
 *                                ESCAN00063844 Tx of NM PDUs in dynamic segment can be disabled
 *                                ESCAN00063869 Added typecast for frNmPduScheduleVariantIs2or5or6
 *                                ESCAN00063870 Limited definition of frNmPduScheduleVariantIs2or5or6
 *                                ESCAN00063938 Fixed compile error caused by the FrNm_LConfigPtr identifier
 *  1.02.00   2013-03-12  vismdr  ESCAN00059536 No changes in this file
 *                                ESCAN00064310 Implemented Variant Post-Build Loadable
 *                                ESCAN00065857 Fixed initialization of FrNm_ConfigurationList for Api Optimization
 *                                ESCAN00066828 Using general define names for determining the configuration variant
 *  2.00.00   2013-09-18  vismdr  ESCAN00067702 Added Run-Time Measurement Support
 *                                ESCAN00070927 Support optional call of PduR_FrNmTxConfirmation
 *  2.01.00   2014-04-30  vismdr  ESCAN00075325 Adapted implementation due to new general defines
 *  3.00.00   2014-09-01  vismdr  ESCAN00076885 Adapted implementation for usage of ComStackLib
 *                                ESCAN00078214 Adapted availability of FrNm_Transmit
 *                                ESCAN00078357 Removed typecast of pointer returned by address operator
 *                                ESCAN00078610 Added FrNm_TxConfirmation / FrNm_TriggerTransmit
 *                                ESCAN00078812 Adapted condition to set the active wake-up bit
 *  3.00.01   2015-02-11  vismdr  ESCAN00081165 Added missing closing parenthesis
 *  4.00.00   2015-03-24  vissrk  ESCAN00082307 Added support for Nm Sync PDUs
 *  5.00.00   2015-11-17  vissrk  ESCAN00080027 Incorrect service IDs used for FrNm_RxIndication and FrNm_TriggerTransmit for Det_ReportError
 *                                ESCAN00086562 Incorrect service IDs used for FrNm_TxConfirmation and FrNm_StartupError for Det_ReportError
 *                                ESCAN00086626 Incorrect Service ID passed in case of Det_ReportError call in FrNm_Mainfunction_<1-16>
 *  6.00.00   2015-12-08  vissrk  ESCAN00080302 FEAT-427: SafeBSW Step I
 *                                ESCAN00085150 FEAT-1527: SafeBSW Step 3
 *                                ESCAN00087841 FEAT-1631: Trigger Transmit API with SduLength In/Out according to ASR4.2.2
 *                                ESCAN00078982 The Car Wake Up Bit is generated as Mask
 *                                ESCAN00089147 Support activation/deactivation of 'Active Wakeup' feature
 *  6.00.01   2016-03-31  vissrk  ESCAN00089145 Correction of Memory Abstraction  in Function Declaration
 *                                ESCAN00089157 Compiler error: 'FRNM_MULTIPLE_SCHEDULE_VARIANTS' is not defined
 *  6.00.02   2016-04-04  vissrk  ESCAN00087807 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00089196 Repeat Message state is entered by Repeat Message Indication even though not configured in the currently active variant [PB-S only]
 *                                ESCAN00089531 Compiler error: 'FrNm_SetTxControlState' undefined; assuming extern returning int
 *                                ESCAN00089533 Compiler error: 'FrNm_GetSizeOfPnClusterReqNew' undefined; assuming extern returning int
 *  6.00.03   2016-06-09  vissrk  ESCAN00090344 Silent Analysis: Justification for the call to FrIf_GetNmVector is not applicable
 *                                ESCAN00090603 Silent Analysis: Corrected justification for SBSW_FRNM_USER_DATA and SBSW_FRNM_PDU_DATA
 *  6.01.00   2017-02-22  vissrk  FEATC-1088    Support a component-specific SafeBSW configuration switch
 *                                ESCAN00094123 Missing Init Check in FrNm_PnEiraCalculation()
 *                                FEATC-534     AUTOSAR 4.2.2 SWS Traceability Goals 2&3 for FrNm
 *  6.01.01   2017-07-10  vismpf  ESCAN00095352 Routed Partial Networking request from the second and all following FrNm cluster to any other channel are never released or released unexpectedly
 *                                STORY-1446    Deactivate Tx Nm message timeout supervision in case 'Msg Timeout Time' is configured to zero
 *********************************************************************************************************************/

#if !defined (FRNM_H)
# define FRNM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "FrNm_Cfg.h"                                                           /* Include FlexRay NM Configuration */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Version - BCD coded version number - Main- and Sub-Version - Release-Version */
# define NM_ASR4NMFR_VERSION                           (0x0601U)
# define NM_ASR4NMFR_RELEASE_VERSION                   (0x01U)

/* Vendor and module identification */
# define FRNM_VENDOR_ID                                (30U)
# define FRNM_MODULE_ID                                (32U)

/* AUTOSAR Software specification version information */
# define FRNM_AR_RELEASE_MAJOR_VERSION                 (0x04U)
# define FRNM_AR_RELEASE_MINOR_VERSION                 (0x00U)
# define FRNM_AR_RELEASE_REVISION_VERSION              (0x03U)

/* ----- Component version information (BCD version of ALM implementation package) ----- */
# define FRNM_SW_MAJOR_VERSION                         (NM_ASR4NMFR_VERSION >> 8U)
# define FRNM_SW_MINOR_VERSION                         (NM_ASR4NMFR_VERSION & 0x00FFU)
# define FRNM_SW_PATCH_VERSION                         (NM_ASR4NMFR_RELEASE_VERSION)

/* ----- API service IDs ----- */
# define FRNM_SID_INIT                                 (0x00U) /*!< Service ID: FrNm_Init() */
# define FRNM_SID_PASSIVESTARTUP                       (0x01U) /*!< Service ID: FrNm_PassiveStartUp() */
# define FRNM_SID_NETWORKREQUEST                       (0x02U) /*!< Service ID: FrNm_NetworkRequest() */
# define FRNM_SID_NETWORKRELEASE                       (0x03U) /*!< Service ID: FrNm_NetworkRelease() */
# define FRNM_SID_ENABLECOMMUNICATION                  (0x05U) /*!< Service ID: FrNm_EnableCommunication() */
# define FRNM_SID_SETUSERDATA                          (0x06U) /*!< Service ID: FrNm_SetUserData() */
# define FRNM_SID_GETUSERDATA                          (0x07U) /*!< Service ID: FrNm_GetUserData() */
# define FRNM_SID_GETPDUDATA                           (0x08U) /*!< Service ID: FrNm_GetPduData() */
# define FRNM_SID_REPEATMESSAGEREQUEST                 (0x09U) /*!< Service ID: FrNm_RepeatMessageRequest() */
# define FRNM_SID_GETNODEIDENTIFIER                    (0x0AU) /*!< Service ID: FrNm_GetNodeIdentifier() */
# define FRNM_SID_GETLOCALNODEIDENTIFIER               (0x0BU) /*!< Service ID: FrNm_GetLocalNodeIdentifier() */
# define FRNM_SID_DISABLECOMMUNICATION                 (0x0CU) /*!< Service ID: FrNm_DisableCommunication() */
# define FRNM_SID_CHECKREMOTESLEEPIND                  (0x0DU) /*!< Service ID: FrNm_CheckRemoteSleepIndication() */
# define FRNM_SID_GETSTATE                             (0x0EU) /*!< Service ID: FrNm_GetState() */
# define FRNM_SID_GETVERSIONINFO                       (0x0FU) /*!< Service ID: FrNm_GetVersionInfo() */
# define FRNM_SID_TRANSMIT                             (0x11U) /*!< Service ID: FrNm_Transmit() */
# define FRNM_SID_SETSLEEPREADYBIT                     (0x12U) /*!< Service ID: FrNm_SetSleepReadyBit() */
# define FRNM_SID_REQUESTBUSSYNC                       (0xC0U) /*!< Service ID: FrNm_RequestBusSynchronization() */
# define FRNM_SID_MAINFUNCTION                         (0xF0U) /*!< Service ID: FrNm_MainFunction() */
# define FRNM_SID_STARTUPERROR                         (0x10U) /*!< Service ID: FrNm_StartUpError() */

/* ----- Error codes ----- */
/* \trace SPEC-16128, SPEC-50093, SPEC-50084, SPEC-15960 */
# define FRNM_E_NO_ERROR                               (0x00U)  /*!< used to check if no error occurred - use a value unequal to any error code */
# define FRNM_E_UNINIT                                 (0x01U)  /*!< Error code: API service used without module initialization */
# define FRNM_E_INVALID_CHANNEL                        (0x02U)  /*!< Error Code: API service used with an invalid channel identifier or channel was not configured for the functionality of the calling API */
# define FRNM_E_INVALID_POINTER                        (0x03U)  /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define FRNM_E_PDU_ID_INVALID                         (0x04U)  /*!< Error code: API service called with invalid PDU ID */
# define FRNM_E_ALREADY_INITIALIZED                    (0x05U)  /*!< Error code: The service FrNm_Init() is called while the module is already initialized  */
# define FRNM_E_INVALID_GENDATA                        (0x06U)  /*!< Error code: The value of a variable used for write accesses is out of bounds */
# define FRNM_E_INIT_FAILED                            (0x10U)  /*!< Error code: API service FrNm_Init() called with wrong parameter */
# define FRNM_E_RXINDICATION_DLC_ERROR                 (0x11U)  /*!< Error code: PDU Length mismatch in FrIf API with configured Length */
# define FRNM_E_PDUR_TRIGGERTX_ERROR                   (0x12U)  /*!< Error code: Call of PduR_TriggerTransmit failed */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define FRNM_START_SEC_CONST_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! FlexRay NM version */
extern CONST(uint8, FRNM_CONST) FrNm_MainVersion;
extern CONST(uint8, FRNM_CONST) FrNm_SubVersion;
extern CONST(uint8, FRNM_CONST) FrNm_ReleaseVersion;

# define FRNM_STOP_SEC_CONST_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define FRNM_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FrNm_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module FrNm. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   nmConfigPtr             Configuration structure for initializing the module
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         FrNm_InitMemory has been called unless FrNm_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-15947, SPEC-50121, SPEC-16166, SPEC-16152, SPEC-16113, SPEC-15883, SPEC-15939, SPEC-15936, 
 *  \trace       SPEC-15945, SPEC-16186, SPEC-16156
 *********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_Init( CONSTP2CONST( FrNm_ConfigType, AUTOMATIC, FRNM_INIT_DATA ) nmConfigPtr );

/***********************************************************************************************************************
 *  FrNm_PassiveStartUp()
 **********************************************************************************************************************/
/*! \brief       Requests the given network passively
 *  \details     Starts the FrNm from the Bus Sleep Mode and triggers
 *               a transition to the Network Mode (Repeat Message State)
 *               This service has no effect if the current state is not equal to Bus Sleep Mode.
 *               In that case E_NOT_OK is returned.
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Start of network management failed
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \note        Called from NM Interface
 *  \trace       SPEC-16044, SPEC-16111, SPEC-16157
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_PassiveStartUp( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );

# if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/***********************************************************************************************************************
 *  FrNm_NetworkRequest()
 **********************************************************************************************************************/
/*! \brief       Requests the given network actively
 *  \details     A call of FrNm_NetworkRequest overrides a previous call of FrNm_PassiveStartUp
 *               ECU has a need to communicate on the bus
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Requesting bus-communication was not accepted
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \note        Called from NM Interface
 *  \config      FRNM_PASSIVE_MODE_ENABLED == STD_OFF
 *  \trace       SPEC-16073, SPEC-16102, SPEC-15872, SPEC-15974
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_NetworkRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );

/***********************************************************************************************************************
 *  FrNm_NetworkRelease()
 **********************************************************************************************************************/
/*! \brief       Releases the given network
 *  \details     ECU has no need to communicate on the bus
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Releasing bus-communication was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_PASSIVE_MODE_ENABLED == STD_OFF
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-16042, SPEC-15874, SPEC-50116, SPEC-16118
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_NetworkRelease( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

/***********************************************************************************************************************
 *  FrNm_GetState()
 **********************************************************************************************************************/
/*! \brief       Returns current state and mode of the network management
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[out]  nmStatePtr                  Pointer where the state of the Network Management shall be copied to
 *  \param[out]  nmModePtr                   Pointer where the mode of the Network Management shall be copied to
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Getting the NM state was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-107582
 *  \trace       SPEC-15984, SPEC-16218
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetState( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                        CONSTP2VAR( Nm_StateType, AUTOMATIC, FRNM_APPL_VAR ) nmStatePtr,
                                                        CONSTP2VAR( Nm_ModeType, AUTOMATIC, FRNM_APPL_VAR ) nmModePtr );

# if ( FRNM_USER_DATA_ENABLED == STD_ON )
#  if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_USER_DATA_SUPPORT == STD_OFF )
/***********************************************************************************************************************
 *  FrNm_SetUserData()
 **********************************************************************************************************************/
/*! \brief       Sets user data for NM messages transmitted next on the bus
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[in]   nmUserDataPtr               Pointer to User data for the next transmitted NM message shall be copied
 *                                           from
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Setting of user data was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_USER_DATA_ENABLED = STD_ON && FRNM_PASSIVE_MODE_ENABLED == STD_OFF && FRNM_COM_USER_DATA_SUPPORT == STD_OFF
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-15996, SPEC-50117, SPEC-2622867, SPEC-16060
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_SetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                           CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmUserDataPtr );
#  endif

/***********************************************************************************************************************
 *  FrNm_GetUserData()
 **********************************************************************************************************************/
/*! \brief       Gets user data from last received NM message
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[in]   nmUserDataPtr               Pointer where user data out of the last received NM message shall be copied
 *                                           to
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Getting of user data was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_USER_DATA_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-16035, SPEC-2622866, SPEC-16222, SPEC-16024
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                           CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmUserDataPtr );
# endif

# if ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON )
/***********************************************************************************************************************
 *  FrNm_GetNodeIdentifier()
 **********************************************************************************************************************/
/*! \brief       Gets node identifier of the last received NM message
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[out]  nmNodeIdPtr                 Pointer where node identifier from the last received NM message shall be
 *                                           copied to
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Getting of node identifier was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_SOURCE_NODE_IDENTIFIER_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-15999, SPEC-15922
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                 CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmNodeIdPtr );

/***********************************************************************************************************************
 *  FrNm_GetLocalNodeIdentifier()
 **********************************************************************************************************************/
/*! \brief       Gets node identifier configured for the local node
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[out]  nmNodeIdPtr                 Pointer where node identifier of the local node shall be copied to
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Getting of local node identifier was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_SOURCE_NODE_IDENTIFIER_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-15926, SPEC-16163, SPEC-16193
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetLocalNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                      CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmNodeIdPtr );
# endif

# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  FrNm_RepeatMessageRequest()
 **********************************************************************************************************************/
/*! \brief       Requests state change to Repeat Message State
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Repeat Message Request was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_NODE_DETECTION_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-16133, SPEC-15949, SPEC-16114, SPEC-16195, SPEC-50107, SPEC-16201, SPEC-15978
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_RepeatMessageRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

# if ( ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) || \
      ( FRNM_USER_DATA_ENABLED == STD_ON ) )
/***********************************************************************************************************************
 *  FrNm_GetPduData()
 **********************************************************************************************************************/
/*! \brief       Gets the whole PDU data out of the last NM message received from the bus
 *  \details     This includes also the CBV and NID Byte, if configured
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[out]  nmPduDataPtr                Pointer where PDU Data out of the most recently received NM message shall be
 *                                           copied to
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Getting the PDU data was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_SOURCE_NODE_IDENTIFIER_ENABLED = STD_ON || FRNM_USER_DATA_ENABLED = STD_ON || FRNM_CONTROL_BIT_VECTOR_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-15993, SPEC-15927, SPEC-16086, SPEC-15894
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetPduData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                          CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmPduData );
# endif

# if ( FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  FrNm_RequestBusSynchronization()
 **********************************************************************************************************************/
/*! \brief       Requests bus synchronization
 *  \details     This API does not implement any functionality.
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_BUS_SYNCHRONIZATION_ENABLED = STD_ON
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-16028, SPEC-15928, SPEC-16173
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_RequestBusSynchronization( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  FrNm_CheckRemoteSleepIndication()
 **********************************************************************************************************************/
/*! \brief       Checks if remote sleep was indicated or not
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[out]  nmRemoteSleepIndPtr         Pointer where PDU Data out of the most recently received NM message
 *                                           shall be copied to. (Range: true, false)
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Checking remote sleep indication was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_REMOTE_SLEEP_INDICATION_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-16087, SPEC-16065, SPEC-50083, SPEC-16041, SPEC-15879
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_CheckRemoteSleepIndication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                          CONSTP2VAR( boolean, AUTOMATIC, FRNM_APPL_VAR ) nmRemoteSleepIndPtr );
# endif

# if ( FRNM_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  FrNm_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief       Returns version information of this module
 *  \details     -
 *  \param[out]  NmVerInfoPtr                Pointer to where to store the version information of this module
 *  \pre         -
 *  \note        Called from Application
 *  \config      FRNM_VERSION_INFO_API = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-103140
 *  \trace       SPEC-16223, SPEC-16174
 **********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, FRNM_APPL_VAR ) NmVerInfoPtr);
# endif

/***********************************************************************************************************************
 *  FrNm_StartupError()
 **********************************************************************************************************************/
/*! \brief       Notify the FlexRay NM that a long-term synchronization lost occurred.
 *  \details     Called from FlexRay State Manager during a long-term loss
 *  \param[in]   NetworkHandle               System channel handle
 *  \pre         -
 *  \note        Called from FlexRay State Manager
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-50145, SPEC-50112, SPEC-50102, SPEC-50129
 **********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_StartupError( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );

# if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) || defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/* ESCAN00078214 */
/***********************************************************************************************************************
 *  FrNm_Transmit()
 **********************************************************************************************************************/
/*! \brief       Triggers a spontaneous transmission of an NM message.
 *  \details     This API does not implement any functionality.
 *  \param[in]   FrNmTxPduId   L-PDU handle of CAN L-PDU to be transmitted. This handle specifies the corresponding CAN
 *                             LPDU ID and implicitly the CAN Driver instance as well as the corresponding CAN
 *                             controller device.
 *  \param[in]   PduInfoPtr    Parameter is ignored.
 *  \pre         -
 *  \return      E_OK                        Transmit request was accepted (FRNM_PASSIVE_MODE = STD_OFF)
 *               E_NOT_OK                    Transmit request was not accepted (FRNM_PASSIVE_MODE = STD_ON)
 *  \note        Called from PduR
 *  \config      FRNM_COM_USER_DATA_SUPPORT = STD_ON || FRNM_PN_CALC_FEATURE_ENABLED defined
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-50135, SPEC-15951
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_Transmit( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId,
                                                        P2CONST( PduInfoType, AUTOMATIC, FRNM_APPL_VAR ) PduInfoPtr );
# endif

# if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  FrNm_EnableCommunication()
 **********************************************************************************************************************/
/*! \brief       Enables NM message transmission control status
 *  \details     Transmission can only be enabled in case PDUs are transmitted in dynamic segment.
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Enabling of NM PDU transmission ability was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_COM_CONTROL_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-15910, SPEC-15886, SPEC-15964, SPEC-15910, SPEC-15886
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_EnableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );

/***********************************************************************************************************************
 *  FrNm_DisableCommunication()
 **********************************************************************************************************************/
/*! \brief       Disables NM message transmission control status
 *  \details     Transmission can only be disabled in case PDUs are transmitted in dynamic segment.
 *  \param[in]   NetworkHandle               System channel handle
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Disabling NM PDU transmission ability was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_COM_CONTROL_ENABLED = STD_ON
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       SPEC-16026, SPEC-15975, SPEC-16064
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_DisableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

/**********************************************************************************************************************
 *  FrNm_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \config      -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_InitMemory( void );

# if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  FrNm_SetSleepReadyBit()
 **********************************************************************************************************************/
/*! \brief       Sets the NM Coordinator Sleep Ready Bit in the Control Bit Vector
 *  \details     -
 *  \param[in]   NetworkHandle               System channel handle
 *  \param[in]   nmSleepReadyBit             Value written to ReadySleep Bit in CBV (Range: 0,1)
 *  \return      E_OK                        Function successful
 *               E_NOT_OK                    Writing of Sleep Ready Bit was not accepted
 *  \pre         -
 *  \note        Called from NM Interface
 *  \config      FRNM_COORDINATOR_SYNC_SUPPORT = STD_ON
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-16055, SPEC-15898
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_SetSleepReadyBit( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                CONST( boolean, AUTOMATIC ) nmSleepReadyBit );
# endif

# define FRNM_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* FRNM_H */

/*!
 * \exclusivearea FRNM_EXCLUSIVE_AREA_0
 * Protects the global Pn Cluster requests from being modified from FrNm_RxIndication or FrNm_StartupError while copying to temporary buffer.
 * \protects FrNm_PnClusterReqExtNew, FrNm_PnClusterReqNew (read/write)
 * \length SHORT At max 7 bytes are copied within this exclusive area.
 * \usedin FrNm_LocalMainFunction, FrNm_PnEiraCalculation, FrNm_RxIndication, FrNm_StartupError
 * \exclude FrNm_LocalMainFunction, FrNm_PnEiraCalculation, FrNm_RxIndication, FrNm_StartupError
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_1
 * Protects the global TX message data from being modified while copying to the SDU.
 * \protects FrNm_TxMessageData (read)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_LocalMainFunction
 * \exclude FrNm_SetUserData, FrNm_SetSleepReadyBit, FrNm_TriggerTransmit
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_2
 * Protects the global TX message buffer from being read while user data is written to it.
 * \protects FrNm_TxMessageData (write)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_TriggerTransmit
 * \exclude FrNm_LocalMainFunction, FrNm_SetSleepReadyBit, FrNm_SetUserData
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_3
 * Protects the global TX message buffer from being read while user data is written to it.
 * \protects FrNm_TxMessageData (write)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_SetUserData, FrNm_SetSleepReadyBit
 * \exclude FrNm_LocalMainFunction, FrNm_TriggerTransmit
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_4
 * Ensures that the global RX message buffer is not read while data is written to it.
 * \protects FrNm_RxMessageVote (write)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_RxIndication
 * \exclude FrNm_LocalMainFunction
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_5
 * Ensures that the global RX message buffer is not read while data is written to it.
 * \protects FrNm_RxMessageData (write)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_RxIndication
 * \exclude FrNm_GetUserData, FrNm_GetNodeIdentifier, FrNm_GetPduData
 * \endexclusivearea
 *
 * \exclusivearea FRNM_EXCLUSIVE_AREA_6
 * Ensures that the global RX message buffer is not written while user data is read from it.
 * \protects FrNm_RxMessageData (read)
 * \length SHORT At max 8 bytes are copied within this exclusive area.
 * \usedin FrNm_GetUserData, FrNm_GetPduData
 * \exclude FrNm_RxIndication
 * \endexclusivearea
 *
 */

/**********************************************************************************************************************
 *  END OF FILE: FrNm.h
 *********************************************************************************************************************/
