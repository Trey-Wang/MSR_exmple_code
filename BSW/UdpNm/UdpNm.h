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
 *         File:  UdpNm.h
 *      Project:  TcpIp-Stack
 *       Module:  UDP Network Management
 *    Generator:  Nm_AsrNmUdp.jar
 *
 *  Description:  Implementation
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alex Lunkenheimer             visalr        Vector Informatik GmbH
 *  Marc Weber                    viswmc        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  Frederik Dornemann            visfdn        Vector Informatik GmbH
 *  Markus Schuster               vissrk        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  00.00.00  2009-02-13  visalr  -             Created
 *                                -             BRS Time Measurement support added
 *                                -             Config variant check for library build
 *                                -             Bugfix include files adapted
 *                                -             Single module header introduced (single source)
 *  00.00.01  2011-11-15  viswmc  -             Configuration parameters according to AUTOSAR 4.0.3 specification
 *  00.00.02  2011-12-07  viswmc  -             Functionality according to AUTOSAR 4.0.3 specification
 *                                -             Adapted timer handling
 *  00.00.03  2011-12-08  viswmc  -             MISRA adaptations
 *  01.00.00  2012-02-22  viswmc  -             Function signatures of RxIndication and TxConfirmation adapted
 *  01.01.00  2012-03-07  viswmc  -             Changes due to review
 *  01.02.00  2012-03-19  viswmc  -             Multi-channel support added (Cfg5 only)
 *            2012-03-19  viswmc  -             Compiler abstraction bugfixes
 *            2012-03-20  viswmc  -             MISRA and Vector Coding Rule adaption
 *            2012-04-02  viswmc  ESCAN00056362 VAR_INIT / VAR_ZERO_INIT Memory Mapping sections
 *            2012-04-12  viswmc  -             Module ID corrected
 *            2012-04-12  viswmc  -             Unused typedefs removed
 *            2012-04-12  viswmc  -             Formal changes
 *  01.02.01  2012-10-02  visalr  ESCAN00061923 ASR4 Enhancement
 *            2012-10-02  visalr  ESCAN00058888 AR4-125: Remove support for v_cfg.h in Cfg5 systems
 *  02.00.00  2013-09-04  vismha  ESCAN00068231 AR4-496: Implementation according to ASR 4.1.1 (BSWMD Schema 4.0.3)
 *            2013-09-05  vismha  ESCAN00069264 AR4-450: Usage of section PBCFG in PB files
 *            2013-10-08  vismha  ESCAN00070928 R8 AR4-89: Support Optional Confirmation
 *  02.01.00  2014-01-13  vismha  ESCAN00071762 AR4-517: Partial Networking for Ethernet
 *  02.01.01  2014-06-17  vismha  ESCAN00076294 Define STATIC in source files instead of UdpNm_Priv.h
 *                                ESCAN00076293 Compiler error: Redeclaration of UdpNm_Init
 *  02.01.02  2014-10-10  vismha  ESCAN00077708 Compiler warning: Several warnings when the component is compiled with
 *                                              Visual Studio
 *  02.01.03  2015-08-04  vismha  ESCAN00081457 State change issued to Nm during initialization although state hasn't
 *                                              changed
 *  02.01.04  2015-11-02  vismha  ESCAN00086204 AR3-2679: Description BCD-coded return-value of Xyz_GetVersionInfo()
 *                                              in TechRef
 *                                ESCAN00082595 Compiler error: Tx Buffers are used while passive mode is enabled
 *                                ESCAN00082617 UdpNm use the Nm_TxTimeoutException Api in passive mode
 *                                ESCAN00082618 UdpNm uses not available channel configuration parameters
 *  03.00.00  2015-11-26  vismha  ESCAN00084966 Remove PDU length restriction according to AUTOSAR RfC 70333
 *  03.00.01  2016-02-04  visfdn  ESCAN00088184 Nm_TxTimeoutException() called although not in state NM_STATE_NORMAL_OPERATION
 *                                ESCAN00088118 Fixed small review findings
 *  03.01.00  2016-02-12  visfdn  ESCAN00087843 FEAT-1631: Trigger Transmit API with SduLength In/Out according to ASR4.2.2
 *  03.02.00  2016-08-08  vissrk  FEATC-59      UdpNM Transmission of NM Coordinator Sleep Ready Bit
 *                                FEATC-63      Immediate Transmission Feature for UdpNM
 *            2016-09-12          FEATC-324     Support Selective Shutdown (Partial Networking)
 *            2016-10-19          ESCAN00092379 Wrong state change or no state change is notified to upper layer after NetworkRequest in state Ready Sleep or Normal Operation
 *  03.02.01  2016-11-25  vissrk  ESCAN00093017 Immediate Nm Transmissions are not executed in case 'Pn Handle Multiple Network Requests' is ON and an internal PN is requested or released
 *                                ESCAN00093027 Immediate Nm Transmissions are executed even though a value equal to 0 is configured
 *  03.02.02  2017-02-17 vissrk   ESCAN00092989 MISRA deviation: MISRA-C:2004 Rule 19.15, Rule 19.6, Rule 8.8
 *                                ESCAN00092916 For PN Handling only the latest NM message content is processed in between two UdpNm main function calls.
 *                                ESCAN00092452 UdpNm_Init compiler abstraction memclass changed to AUTOMATIC
 *                                ESCAN00092947 Com User Data is not updated in next NM message
 **********************************************************************************************************************/

#ifndef UDPNM_H /* PRQA S 0883 1 */ /* MD_MSR_19.15 */
#define UDPNM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* inclusion of SoAd config header to get access to SoAd PDU IDs */
#include "SoAd_Cfg.h"
/* inclusion of ComM config header to get access to ComM Channel Handles */
#include "ComM.h"
#include "Nm_Cfg.h"
#include "UdpNm_Types.h"
#include "UdpNm_Cfg.h"
#include "UdpNm_Lcfg.h"
#include "NmStack_Types.h"
#include "SchM_UdpNm.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* BCD coded version number */
#define NM_ASRNMUDP_VERSION          (0x0302u)
/* BCD coded release version number */
#define NM_ASRNMUDP_RELEASE_VERSION  (0x02u)

/* UDPNM software version */
#define UDPNM_SW_MAJOR_VERSION                      3u
#define UDPNM_SW_MINOR_VERSION                      2u
#define UDPNM_SW_PATCH_VERSION                      2u

/* UDPNM supported Autosar version */
#define UDPNM_AR_MAJOR_VERSION                      4u
#define UDPNM_AR_MINOR_VERSION                      1u
#define UDPNM_AR_PATCH_VERSION                      1u

/* UDP NM Module ID */
/* Vector ID */
#define UDPNM_VENDOR_ID                              (30u)
/* UDP Network Management ID */
#define UDPNM_MODULE_ID                              (33u)

/* UDP NM API IDs */
#define UDPNM_API_ID_INIT                            (0x01u)
#define UDPNM_API_ID_NETWORK_REQUEST                 (0x02u)
#define UDPNM_API_ID_NETWORK_RELEASE                 (0x03u)
#define UDPNM_API_ID_SET_USER_DATA                   (0x04u)
#define UDPNM_API_ID_GET_USER_DATA                   (0x05u)
#define UDPNM_API_ID_GET_NODE_IDENTIFIER             (0x06u)
#define UDPNM_API_ID_GET_LOCAL_NODE_IDENTIFIER       (0x07u)
#define UDPNM_API_ID_REPEAT_MESSAGE_REQUEST          (0x08u)
#define UDPNM_API_ID_GET_VERSION_INFO                (0x09u)
#define UDPNM_API_ID_GET_PDU_DATA                    (0x0Au)
#define UDPNM_API_ID_GET_STATE                       (0x0Bu)
#define UDPNM_API_ID_DISABLE_COMMUNICATION           (0x0Cu)
#define UDPNM_API_ID_ENABLE_COMMUNICATION            (0x0Du)
#define UDPNM_API_ID_PASSIVE_START_UP                (0x0Eu)
#define UDPNM_API_ID_SOADIF_TX_CONFIRMATION          (0x0Fu)
#define UDPNM_API_ID_SOADIF_RX_INDICATION            (0x10u)
#define UDPNM_API_ID_CHECK_REMOTE_SLEEP_INDICATION   (0x11u)
#define UDPNM_API_ID_SET_COORD_BITS                  (0x12u)
#define UDPNM_API_ID_MAIN_FUNCTION                   (0x13u)
#define UDPNM_API_ID_REQUEST_BUS_SYNCHRONIZATION     (0x14u)
#define UDPNM_API_ID_TRANSMIT                        (0x15u)
#define UDPNM_API_ID_SET_SLEEP_READY_BIT             (0x16u)
#define UDPNM_API_ID_V_TRANSMIT_PDU                  (0x20u)

/* UDPNM DET Errors */
#define UDPNM_E_NO_INIT                 (0x01u)
#define UDPNM_E_INVALID_CHANNEL         (0x02u)
#define UDPNM_E_INVALID_PDUID           (0x03u)
#define UDPNM_E_NULL_POINTER            (0x12u)
#define UDPNM_E_PDUR_TRIGGERTX_ERROR    (0x21u)
#define UDPNM_E_INVALID_USER_DATA_LEN   (0x22u)

/* UDPNM PDU Positions */
#define UDPNM_PDU_BYTE_0                     (0x00u)
#define UDPNM_PDU_BYTE_1                     (0x01u)
#define UDPNM_PDU_OFF                        (0xFFu)

/* UDP NM Network States */
#define UDPNM_NETWORK_STATE_RELEASED    ((UdpNm_NetworkStateType)0u)
#define UDPNM_NETWORK_STATE_REQUESTED   ((UdpNm_NetworkStateType)1u)

#if( UDPNM_PN_ENABLED == STD_ON )
#define UDPNM_PN_STATE_RELEASED         ((UdpNm_PnStateType)0U)
#define UDPNM_PN_STATE_REQUESTED        ((UdpNm_PnStateType)1U)
#endif /* UDPNM_PN_ENABLED */

#define UDPNM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  UdpNm_InitMemory
 *********************************************************************************************************************/
/*! \brief      Initialize global variables.
 *  \pre        This function has to be called before any other calls to the module.
 *  \context    Task level
 *  \note       AUTOSAR extension
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_InitMemory(void);

/**********************************************************************************************************************
 *  UdpNm_Init
 *********************************************************************************************************************/
/*! \brief      Initialize the complete UdpNm module, i.e. all channels which are activated at configuration time are
 *              initialized. A UDP socket shall be set up with the TCP/IP stack.
 *  \param[in]  UdpNmConfigPtr       Pointer to a selected configuration structure
 *  \pre        This function has to be called before usage of the module.
 *  \pre        This function has to be called after initialization of the TCP/IP stack.
 *  \context    Initialization
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_Init(P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_CONST) UdpNmConfigPtr);

/**********************************************************************************************************************
 *  UdpNm_PassiveStartUp
 *********************************************************************************************************************/
/*! \brief      Passive startup of the AUTOSAR UdpNm. It triggers the transition from Bus-Sleep Mode to the Network
 *              Mode in Repeat Message State.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Passive startup of network management has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_PassiveStartUp(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);

#if ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_NetworkRequest
 *********************************************************************************************************************/
/*! \brief      Request the network, since ECU needs to communicate on the bus. Network state shall be changed to
 *              'requested'.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Requesting of network has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRequest(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_NetworkRelease
 *********************************************************************************************************************/
/*! \brief      Release the network, since ECU doesn't have to communicate on the bus. Network state shall be changed
 *              to 'released'.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Releasing of network has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRelease(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( STD_ON == UDPNM_COM_CONTROL_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_DisableCommunication
 *********************************************************************************************************************/
/*! \brief      Disable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Disabling of NM PDU transmission ability has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_DisableCommunication(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_COM_CONTROL_ENABLED */

#if ( STD_ON == UDPNM_COM_CONTROL_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_EnableCommunication
 *********************************************************************************************************************/
/*! \brief      Enable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Enabling of NM PDU transmission ability has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_EnableCommunication(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_COM_CONTROL_ENABLED */

#if (( STD_ON == UDPNM_USER_DATA_ENABLED ) && ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED ) \
  &&  ( STD_OFF == UDPNM_COM_USER_DATA_SUPPORT ))
/**********************************************************************************************************************
 *  UdpNm_SetUserData
 *********************************************************************************************************************/
/*! \brief      Set user data for all NM messages transmitted on the bus after this function has returned without
 *              error.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmUserDataPtr        Pointer where the user data for the next transmitted NM message shall be copied
 *                                   from
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Setting of user data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetUserData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                   P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) NmUserDataPtr);
#endif /* UDPNM_USER_DATA_ENABLED, UDPNM_PASSIVE_MODE_ENABLED, UDPNM_COM_USER_DATA_SUPPORT */

#if ( STD_ON == UDPNM_USER_DATA_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_GetUserData
 *********************************************************************************************************************/
/*! \brief      Get user data from the most recently received NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmUserDataPtr        Pointer where user data out of the most recently received NM message shall be
 *                                   copied to.
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of user data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetUserData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                   CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmUserDataPtr);
#endif /* UDPNM_USER_DATA_ENABLED */

#if ( STD_ON == UDPNM_NODE_ID_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_GetNodeIdentifier
 *********************************************************************************************************************/
/*! \brief      Get node identifier from the most recently received NM PDU.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmNodeIdPtr          Pointer where the source node identifier from the most recently received NM PDU
 *                                   shall be copied to.
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of the source node identifier from the most recently received NM PDU has
 *                                failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                         CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmNodeIdPtr);
#endif /* UDPNM_NODE_ID_ENABLED */

#if ( STD_ON == UDPNM_NODE_ID_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_GetLocalNodeIdentifier
 *********************************************************************************************************************/
/*! \brief      Get node identifier configured for the local node.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmNodeIdPtr          Pointer where node identifier of the local node shall be copied to.
 *  \return     NM_E_OK              No error
 *  \return     NM_E_NOT_OK          Getting of the node identifier of the local node has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetLocalNodeIdentifier(
  CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmNodeIdPtr);
#endif /* UDPNM_NODE_ID_ENABLED */

#if ( STD_ON == UDPNM_NODE_DETECTION_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_RepeatMessageRequest --> UdpNm_VRepeatMessageRequest
 *********************************************************************************************************************/
/*! \brief      Trigger transition to Repeat Message State and set Repeat Message Request Bit for all NM messages
 *              transmitted on the bus after this function has returned without error (dependent on the parameter
 *              'isRepeatMessageBitIndication').
 *  \param[in]  NmChannelHandle                Identification of the NM-channel
 *  \param[in]  isRepeatMessageBitIndication   Flag indicating whether a Repeat Message Request Bit was received
 *  \return     E_OK                        No error
 *  \return     E_NOT_OK                    Setting of Repeat Message Request Bit has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RepeatMessageRequest(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_NODE_DETECTION_ENABLED */

#if (( STD_ON == UDPNM_NODE_ID_ENABLED ) ||  ( STD_ON == UDPNM_NODE_DETECTION_ENABLED ) \
        || ( STD_ON == UDPNM_USER_DATA_ENABLED ))
/**********************************************************************************************************************
 *  UdpNm_GetPduData
 *********************************************************************************************************************/
/*! \brief      Get the whole PDU data out of the most recently received NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmPduDataPtr         Pointer where NM PDU shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of NM PDU data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetPduData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmPduDataPtr);
#endif /* UDPNM_NODE_ID_ENABLED, UDPNM_NODE_DETECTION_ENABLED, UDPNM_USER_DATA_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_GetState
 *********************************************************************************************************************/
/*! \brief      Returns the state and the mode of the network management.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmStatePtr           Pointer where state of the network management shall be copied to
 *  \param[out] NmModePtr            Pointer where the mode of the network management shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of NM state has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetState(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                CONSTP2VAR(Nm_StateType, AUTOMATIC, UDPNM_APPL_VAR) NmStatePtr,
                                                CONSTP2VAR(Nm_ModeType, AUTOMATIC, UDPNM_APPL_VAR) NmModePtr);

#if ( STD_ON == UDPNM_VERSION_INFO_API )
/**********************************************************************************************************************
 *  UdpNm_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Returns the version information, vendor ID and AUTOSAR module ID of the component.
 *              The versions are BCD-coded.
 *  \param[in]  VersionInfoPtr        Pointer for version info
 *  \return     void
 *  \context    Initialization or task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, UDPNM_APPL_VAR) Versioninfo);
#endif /* UDPNM_VERSION_INFO_API */

#if (( STD_ON == UDPNM_BUS_SYNCHRONIZATION_ENABLED ) && ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED ))
/**********************************************************************************************************************
 *  UdpNm_RequestBusSynchronization
 *********************************************************************************************************************/
/*! \brief      Request bus synchronization.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Requesting of bus synchronization has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RequestBusSynchronization(
  CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);
#endif /* UDPNM_BUS_SYNCHRONIZATION_ENABLED, UDPNM_PASSIVE_MODE_ENABLED */

#if ( STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED )
/**********************************************************************************************************************
 *  UdpNm_CheckRemoteSleepIndication
 *********************************************************************************************************************/
/*! \brief      Check if remote sleep indication takes place or not.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmRemoteSleepIndPtr  Pointer where check result of remote sleep indication shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Checking of remote sleep indication bits has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_CheckRemoteSleepIndication(
  CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
  CONSTP2VAR(boolean, AUTOMATIC, UDPNM_APPL_VAR) NmRemoteSleepIndPtr);
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_SetCoordBits
 *********************************************************************************************************************/
/*! \brief      Sets the NM coordinator ID in the control bit vector of each NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmCoordBits          2 bit value to set the NM coordinator ID in the control bit vector of each NM
 *                                   message (coding as depicted in Figure "Control Bit Vector")
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Setting the coordinator ID bits has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetCoordBits(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                    CONST(uint8, AUTOMATIC) NmCoordBits);

#if ( STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT )
/**********************************************************************************************************************
 *  UdpNm_SetSleepReadyBit
 *********************************************************************************************************************/
/*! \brief      Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmSleepReadyBit      Value written to ReadySleep Bit in CBV
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Writing of remote sleep indication bit has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetSleepReadyBit(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                        CONST(boolean, AUTOMATIC) NmSleepReadyBit);
#endif /* UDPNM_COORDINATOR_SYNC_SUPPORT */

#if ( STD_ON == UDPNM_COM_USER_DATA_SUPPORT )
/**********************************************************************************************************************
 *  UdpNm_Transmit
 *********************************************************************************************************************/
/*! \brief      UdpNm_Transmit is implemented as an empty function and shall always return E_OK. The function
 *              UdpNm_Transmit is only available if the configuration switch UdpNmComUserDataSupport is enabled.
 *  \param[in]  UdpNmSrcPduId        This parameter contains a unique identifier referencing to the PDU Routing Table
 *                                   and thereby specifying the socket to be used for transmission of the data
 *  \param[in]  UdpNmSrcPduInfoPtr   A pointer to a structure with socket related data: data length and pointer to a
 *                                   data buffer
 *  \return     E_OK              The request has been accepted
 *  \return     E_NOT_OK          The request has not been accepted, e.g. due to a still ongoing transmission in the
 *                                corresponding socket or the to be transmitted message is too long
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_Transmit(PduIdType UdpNmTxPduId,
                                                P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) UdpNmSrcPduInfoPtr);
#endif /* UDPNM_COM_USER_DATA_SUPPORT */

#define UDPNM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#endif /* UDPNM_H */

/**********************************************************************************************************************
 *  END OF FILE: UdpNm.h
 *********************************************************************************************************************/
 
