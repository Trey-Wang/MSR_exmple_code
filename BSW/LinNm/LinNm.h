
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
 *         File:  LinNm.h
 *    Component:  AUTOSAR LIN Network Management
 *       Module:  AUTOSAR Network Management
 *    Generator:  -
 *
 *  Description:  Header of AUTOSAR LIN Network Management
 *                AUTOSAR Release 4
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Pick                  visap         Vector Informatik GmbH
 *  Bastian Molkenthin            visbmo        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-07-16  visap   ESCAN00060143 Creation of component based on FrNm
 *  02.00.00  2013-09-26  visbmo  -             No changes, solely major version increased for R8 due to consistency
 *  02.01.00  2014-12-08  visbmo  ESCAN00080297 FEAT-427: SafeBSW Step I
 *  02.02.00  2015-06-16  visbmo  ESCAN00081785 FEAT-1275: SafeBSW Step 2
 *                                ESCAN00083441 Add support for multiple call contexts
 *  02.02.01  2015-11-04  visbmo  -             Adapted coverage justifications
 *  02.02.02  2016-02-15  visbmo  ESCAN00087803 FEAT-1688: SafeBSW Step 4
 *********************************************************************************************************************/

#if !defined (LINNM_H)
# define LINNM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "NmStack_Types.h"                                                                /* Include NM stack types */
# include "LinNm_Cfg.h"                                                              /* Include LIN NM Configuration */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define LINNM_SW_MAJOR_VERSION                     (2u)
# define LINNM_SW_MINOR_VERSION                     (2u)
# define LINNM_SW_PATCH_VERSION                     (2u)

/* vendor and module identification */
# define LINNM_VENDOR_ID                            (30u)
# define LINNM_MODULE_ID                            (63u)

/* AUTOSAR Software Specification Version Information */
/* Document Version   2.0.0
   Part of Release    4.0
   Revision           0003 */
# define LINNM_AR_RELEASE_MAJOR_VERSION             (4u)
# define LINNM_AR_RELEASE_MINOR_VERSION             (0u)
# define LINNM_AR_RELEASE_REVISION_VERSION          (3u)

/* ----- API service IDs ----- */
# define LINNM_SID_INIT_ID                          (0x00u) /*!< Service ID: LinNm_Init() */
# define LINNM_SID_PASSIVESTARTUP                   (0x01u) /*!< Service ID: LinNm_PassiveStartUp() */
# define LINNM_SID_NETWORKREQUEST                   (0x02u) /*!< Service ID: LinNm_NetworkRequest() */
# define LINNM_SID_NETWORKRELEASE                   (0x03u) /*!< Service ID: LinNm_NetworkRelease() */
# define LINNM_SID_DISABLECOMMUNICATION             (0x04u) /*!< Service ID: LinNm_DisableCommunication */
# define LINNM_SID_ENABLECOMMUNICATION              (0x05u) /*!< Service ID: LinNm_EnableCommunication */
# define LINNM_SID_SETUSERDATA                      (0x06u) /*!< Service ID: LinNm_SetUserData */
# define LINNM_SID_GETUSERDATA                      (0x07u) /*!< Service ID: LinNm_GetUserData */
# define LINNM_SID_GETPDUDATA                       (0x08u) /*!< Service ID: LinNm_GetPduData */
# define LINNM_SID_REPEATMESSAGEREQUEST             (0x09u) /*!< Service ID: LinNm_RepeatMessageRequest */
# define LINNM_SID_GETNODEIDENTIFIER                (0x0Au) /*!< Service ID: LinNm_GetNodeIdentifier */
# define LINNM_SID_GETLOCALNODEIDENTIFIER           (0x0Bu) /*!< Service ID: LinNm_GetLocalNodeIdentifier */
# define LINNM_SID_GETSTATE                         (0x0Eu) /*!< Service ID: LinNm_GetState */
# define LINNM_SID_TRANSMIT                         (0x0Fu) /*!< Service ID: LinNm_Transmit */
# define LINNM_SID_SETSLEEPREADYBIT                 (0x10u) /*!< Service ID: LinNm_SetSleepReadyBit */
# define LINNM_SID_TXCONFIRMATION                   (0x40u) /*!< Service ID: LinNm_TxConfirmation */
# define LINNM_SID_REQUESTBUSSYNCHRONIZATION        (0xC0u) /*!< Service ID: LinNm_RequestBusSynchronization */
# define LINNM_SID_CHECKREMOTESLEEPIND              (0xD0u) /*!< Service ID: LinNm_CheckRemoteSleepIndication */
# define LINNM_SID_GETVERSIONINFO                   (0xF1u) /*!< Service ID: LinNm_GetVersionInfo */

/* ----- Error Codes ----- */
# define LINNM_E_NO_ERROR                           (0x00u) /* No error occured */
# define LINNM_E_NO_INIT                            (0x01u) /*!< Error code: API service used without module initialization */
# define LINNM_E_INVALID_CHANNEL                    (0x02u) /*!< Error code: API service used with wrong channel handle */
# define LINNM_E_PARAM_POINTER                      (0x12u) /*!< Error code: API service used with null pointer parameter */

/* Invalid channel handle definition */
# define LINNM_INVALID_CHANNEL                      ((NetworkHandleType)0xFFu)

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

# define LINNM_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LinNm_InitMemory()
 **********************************************************************************************************************/
/*! \brief       Power-up memory initialization
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized. Interrupts are disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *  \trace       CREQ-312
 */
FUNC( void, LINNM_CODE ) LinNm_InitMemory( void );

/***********************************************************************************************************************
 *  LinNm_Init()
 **********************************************************************************************************************/
/*! \brief       Initializes component LinNm.
 *  \details     Initializes all component variables and sets the component state to initialized.
 *               By default the NM starts in the Bus-Sleep Mode.
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         LinNm_InitMemory has been called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by application (EcuM) and before of any NM service (except LinNm_InitMemory() ).
 *  \trace       CREQ-312
 */
FUNC( void, LINNM_CODE ) LinNm_Init( void );

/***********************************************************************************************************************
 *  LinNm_PassiveStartUp()
 **********************************************************************************************************************/
/*! \brief       Passive startup of LinNm.
 *  \details     Triggers a transition to the Network Mode (Repeat Message State) This service has no effect if the current
 *               state is not equal to Bus Sleep Mode.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \return      E_OK                     Request of start of network management was accepted
 *  \return      E_NOT_OK                 Request of start of network management was not accepted
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \note        Called from NM Interface
 *  \trace       CREQ-318
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_PassiveStartUp( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle );

# if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
/***********************************************************************************************************************
 *  LinNm_NetworkRequest()
 **********************************************************************************************************************/
/*! \brief       Requests bus communication.
 *  \details     Request the network and change to Network mode. This service has no effect if the current state is not
 *               equal to Bus Sleep Mode.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \return      E_OK                     Request for bus communication was accepted
 *  \return      E_NOT_OK                 Request for bus communication was not accepted
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \config      LINNM_PASSIVE_MODE_ENABLED
 *  \note        Called from NM Interface
 *  \trace       CREQ-315
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_NetworkRequest( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle );

/***********************************************************************************************************************
 *  LinNm_NetworkRelease()
 **********************************************************************************************************************/
/*! \brief       Releases bus communication.
 *  \details     Release the network and change to Bus Sleep mode. This service has no effect if the current state is not
 *               equal to Network Mode.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \return      E_OK                     Request for bus communication relase was accepted
 *  \return      E_NOT_OK                 Request for bus communication relase was not accepted
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \config      LINNM_PASSIVE_MODE_ENABLED
 *  \note        Called from NM Interface
 *  \trace       CREQ-316
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_NetworkRelease( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle );
# endif

# if ( LINNM_USER_DATA_ENABLED == STD_ON )
#  if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
#   if ( LINNM_COM_USER_DATA_SUPPORT == STD_OFF )
/***********************************************************************************************************************
 *  LinNm_SetUserData()
 **********************************************************************************************************************/
/*! \brief       Sets user data for next transmitted NM message.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \param[in]   nmUserDataPtr            Pointer to user data for the next transmitted NM message
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINNM_USER_DATA_ENABLED, LINNM_PASSIVE_MODE_ENABLED, LINNM_COM_USER_DATA_SUPPORT
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_SetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                      CONSTP2CONST( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmUserDataPtr );
#   endif
#  endif

/***********************************************************************************************************************
 *  LinNm_GetUserData()
 **********************************************************************************************************************/
/*! \brief       Gets user data from last received NM message.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \param[in]   nmUserDataPtr            Pointer where user data out of the last received NM message shall be copied to
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_USER_DATA_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                      CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmUserDataPtr );
# endif

# if ( ( LINNM_NODE_ID_ENABLED  == STD_ON ) || ( LINNM_USER_DATA_ENABLED == STD_ON ) )
/***********************************************************************************************************************
 *  LinNm_GetPduData()
 **********************************************************************************************************************/
/*! \brief       Gets whole PDU data from last received NM message.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \param[in]   nmPduDataPtr             Pointer where NM PDU of last received NM message shall be copied to.
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_NODE_ID_ENABLED, LINNM_USER_DATA_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetPduData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                     CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmPduDataPtr );
# endif

# if ( LINNM_NODE_DETECTION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_RepeatMessageRequest()
 **********************************************************************************************************************/
/*! \brief       Requests the state change to Repeat Message State.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_NODE_DETECTION_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_RepeatMessageRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

# if ( LINNM_NODE_ID_ENABLED  == STD_ON )
/***********************************************************************************************************************
 *  LinNm_GetNodeIdentifier()
 **********************************************************************************************************************/
/*! \brief       Gets the node identifier of the last received NM message.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \param[in]   nmNodeIdPtr              Pointer where node identifier from the last received NM message shall be copied to
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_NODE_ID_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                            CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmNodeIdPtr );

/***********************************************************************************************************************
 *  LinNm_GetLocalNodeIdentifier()
 **********************************************************************************************************************/
/*! \brief       Gets the node identifier configured for the local node.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \param[in]   nmNodeIdPtr              Pointer where node identifier of the local node shall be copied to
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_NODE_ID_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetLocalNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                 CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmNodeIdPtr );
# endif

# if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_RequestBusSynchronization()
 **********************************************************************************************************************/
/*! \brief       Requests bus synchronization (has no effect for LIN)
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_PASSIVE_MODE_ENABLED, LINNM_BUS_SYNCHRONIZATION_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_RequestBusSynchronization( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle );
# endif

# if ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_CheckRemoteSleepIndication()
 **********************************************************************************************************************/
/*! \brief       Checks if remote sleep was indicated or not.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \param[in]   nmRemoteSleepIndPtr      Pointer where check result of remote sleep indication shall be copied to
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_REMOTE_SLEEP_INDICATION_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_CheckRemoteSleepIndication( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                                     CONSTP2VAR( boolean, AUTOMATIC, LINNM_APPL_VAR ) nmRemoteSleepIndPtr );
# endif

/***********************************************************************************************************************
 *  LinNm_GetState()
 **********************************************************************************************************************/
/*! \brief       Returns the state of the NM.
 *  \details     Returns current state and mode of the network management.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \param[in]   nmStatePtr               Pointer where the state of the network management shall be copied to
 *  \param[in]   nmModePtr                Pointer where the mode of the network management shall be copied to
 *  \return      E_OK                     Request was accepted
 *  \return      E_NOT_OK                 Request was not accepted. Getting the NM state has failed
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        Called from NM Interface.
 *  \trace       CREQ-317
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetState( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                   CONSTP2VAR( Nm_StateType, AUTOMATIC, LINNM_APPL_VAR ) nmStatePtr,
                                                   CONSTP2VAR( Nm_ModeType, AUTOMATIC, LINNM_APPL_VAR ) nmModePtr );

# if ( LINNM_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinNm_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[in]   versioninfo              Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_VERSION_INFO_API
 *  \trace       CREQ-313
 */
FUNC( void, LINNM_CODE ) LinNm_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, LINNM_APPL_VAR ) versioninfo);
# endif

# if ( LINNM_COM_USER_DATA_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinNm_Transmit()
 **********************************************************************************************************************/
/*! \brief       Requests the transmission of an NM message.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   LinTxPduId               PDU ID of the LIN frame to be transmitted
 *  \param[in]   PduInfoPtr               Pointer to LIN PDU releated data containing DLC and pointer to LIN SDU buffer
 *  \return      E_NOT_OK                 Transmit request was not accepted. Function always return E_NOT_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_COM_USER_DATA_SUPPORT
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_Transmit( VAR( PduIdType, AUTOMATIC ) LinTxPduId,
                                                   P2CONST( PduInfoType, AUTOMATIC, LINNM_APPL_VAR ) PduInfoPtr);
# endif

# if ( LINNM_COM_CONTROL_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_EnableCommunication()
 **********************************************************************************************************************/
/*! \brief       Enables the NM PDU transmission ability.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_COM_CONTROL_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_EnableCommunication ( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );

/***********************************************************************************************************************
 *  LinNm_DisableCommunication()
 **********************************************************************************************************************/
/*! \brief       Disables the NM PDU transmission ability.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   NetworkHandle            NM channel handle
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_COM_CONTROL_ENABLED
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_DisableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle );
# endif

# if ( LINNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinNm_SetSleepReadyBit()
 **********************************************************************************************************************/
/*! \brief       Sets the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 *  \details     This is an empty function to be complaint with NM specifications.
 *  \param[in]   nmChannelHandle          NM channel handle
 *  \param[in]   nmSleepReadyBit          Value written to ReadySleep Bit in Control Bit Vector
 *  \return      E_OK                     Request was accepted. Function always returns E_OK.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINNM_COORDINATOR_SYNC_SUPPORT
 *  \note        Called from NM Interface.
 *  \trace       CREQ-314
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_SetSleepReadyBit( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                           CONST( boolean, AUTOMATIC ) nmSleepReadyBit);
# endif

# define LINNM_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \exclusivearea LINNM_EXCLUSIVE_AREA_0
 * Ensures consistency in LinNm service API functions if main function can interrupt it's own service functions.
 * \protects LinNm_NmState and LinNm_NmMode variables
 * \usedin LinNm_PassiveStartUp, LinNm_NetworkRequest, LinNm_NetworkRelease, LinNm_GetState
 * \exclude LinNm_MainFunction
 * \length SHORT At most two global variables are modified, no functions called.
 * \endexclusivearea
 */

#endif  /* LINNM_H */

/**********************************************************************************************************************
 *  END OF FILE: LinNm.h
 *********************************************************************************************************************/
