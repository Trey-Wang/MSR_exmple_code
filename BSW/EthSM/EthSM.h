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
 *         File:  EthSM.h
 *      Project:  MICROSAR ETH
 *       Module:  Ethernet State Manager
 *    Generator:  Ccl_AsrSmEth.jar
 *
 *  Description:  module header
 *  
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Marc Weber                    viswmc        Vector Informatik GmbH
 *  Alex Lunkenheimer             visalr        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  Martin Nonnenmann             vismno        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2009-01-29  visalr  -             created
 *  01.00.01  2009-03-26  visalr  -             bugfix: DET error if ConfigVariant != 3
 *  01.00.02  2009-03-30  visalr  -             bugfix: dummy mode support
 *  01.00.03  2009-07-31  visalr  -             new DET macros
 *                                -             BRS Time Measurement support added
 *                                -             Config variant check for library build
 *  01.00.04  2009-09-15  visalr  -             Config version check
 *  01.00.05  2009-09-28  visalr  -             GENy release version
 *  01.00.06  2009-09-29  visalr  -             Memory Abstraction for stack local pointer
 *                                -             bugfix support all configuration variants
 *  01.00.07  2009-10-05  visalr  -             bugfix include files adapted
 *                                -             single module header introduced (single source)
 *  01.00.08  2009-10-05  visalr  -             no changes
 *  01.00.09  2009-10-05  visalr  -             no changes
 *  01.00.10  2009-10-05  visalr  -             no changes
 *  01.00.11  2009-10-05  visalr  -             no changes
 *  01.00.12  2009-10-05  visalr  -             no changes
 *  02.00.00  2009-10-08  visalr  -             no changes
 *  02.00.01  2009-10-09  visalr  -             encapsulate BswM by VGEN_ENABLE_SYSSERVICE_BSWECUM
 *  02.00.02  2009-10-13  visalr  -             comment format updated
 *  02.00.03  2009-11-05  visalr  -             ComM_BusSM callback added
 *  02.00.04  2009-11-05  visalr  -             EthSM_SchM.h introduced
 *  02.00.05  2009-11-05  visalr  -             no changes
 *  02.00.06  2009-11-19  visalr  -             bugfix DET checks without actual condition did not fire
 *                                -             EthSM_MainFunction encapsulated for optimization
 *                                -             validate post-build configuration
 *                                -             DET check bugfixing
 *                                -             support multiple calls of EthSM_Init
 *  02.00.07  2010-01-12  visalr  ESCAN00040066 EthSM: Compiler Error without DET
 *  02.00.08  2010-01-13  visalr  ESCAN00040066 EthSM: Compiler Error without DET
 *  02.00.09  2010-02-11  visalr  ESCAN00040759 Wrong module id returned
 *  02.00.10  2010-08-02  visalr  ESCAN00044387 EthSM does not use global network handle
 *  02.00.11  2010-09-16  visalr  ESCAN00045367 EthSM does not compile without EthTrcv
 *  02.00.12  2011-01-27  visalr  ESCAN00047348 Adapt MainFunction for usage with IdentityManagerConfig
 *  02.00.13  2011-01-27  visalr                ModHdr removed
 *  02.00.14  2011-02-07  visalr  -             Adapt AUTOSAR version check
 *  02.00.15  2011-02-11  visalr  ESCAN00048011 MemMap.h: Missing Start/Stop Sections (PBCFG_ROOT)
 *            2011-02-16  visalr  -             compiler abstraction adapted (use of AUTOMATIC)
 *  02.00.16  2011-03-04  visalr                missing comment within EthSM_Compiler_Cfg.inc
 *  02.00.17  2011-05-02  visalr  ESCAN00050515 Generate EthSM  EcuC Global Configuration Container Name
 *  02.00.18  2011-06-14  visalr                ALM
 *  02.00.19  2011-09-07  visalr  ESCAN00053424 Addtional preprocessor argument for macro without DEM has to be removed
 *  02.00.20  2011-09-23  visalr                GHS compiler warnings resolved (undefined preprocessing identifier)
 *  02.00.21  2011-12-13  visalr  ESCAN00051701 Remove the AUTOSAR Release Version Check
 *  02.00.22  2012-02-14  visalr  ESCAN00056348 Remove Macro support for Version Info API
 *            2012-02-14  visalr  ESCAN00056347 VAR_INIT / VAR_ZERO_INIT Memory Mapping sections
 *            2012-02-22  visalr  ESCAN00056601 Compiler Abstractions are partly wrong
 *  02.01.00  2012-05-04  visalr  ESCAN00058749 Enhance API to ignore communication request (Dcm AUITOSAR3 issue)
 *            2012-04-16  viswmc  ESCAN00055769 SoAd_BusSM_ModeIndication() API not called
 *  02.02.00  2012-08-31  viswmc  ESCAN00055769 Invocation of callback API SoAd_BusSM_ModeIndication() added
 *            2012-09-21  visalr  ESCAN00058885 AR4-125: Remove support for v_cfg.h in Cfg5 systems
 *            2012-10-02  visalr  ESCAN00061915 ASR4 Enhancement
 *  02.02.01  2012-11-15  visalr  ESCAN00063046 EthSM_State linkage error
 *  02.02.02  2012-12-21  visalr  ESCAN00062245 AR4-220: Remove STATIC
 *            2012-12-21  visalr  ESCAN00053455 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *  02.03.00  2013-04-08  vismha  ESCAN00066532 Support of multiple Ethernet Channels
 *            2013-04-08  vismha  ESCAN00066533 Add ability for Auto Mode Change Request Retry
 *  02.03.01  2013-06-12  vismha  ESCAN00067946 Compiler error "syntax error : identifier 'STATIC'" occurs
 *            2013-06-14  vismha  ESCAN00068057 Unsuccessful Mode Change handling not AUTOSAR conform
 *            2013-06-17  vismha  ESCAN00068115 State handling in multiple Ethernet channel case wrong
 *  03.00.00  2013-08-14  vismha  ESCAN00068229 AR4-502: Implementation according to ASR 4.1.1
 *            2013-08-14  vismha  ESCAN00069266 AR4-450: Usage of section PBCFG in PB files
 *            2013-10-17  vismha  ESCAN00071144 Provide Support for mixed TcpIp/AVB communication stack
 *            2013-10-18  vismha  ESCAN00071213 EthSM_VTcpIpModeIndication isn't called by TcpIp anymore but by the
 *                                              IpVx modules
 *  03.00.01  2014-07-10  vismha  ESCAN00074412 Compiler warning: function EthSM_MainFunction declared implicitly
 *                        vismha  ESCAN00076288 Vector Coding Rules error: #include "EthSM_priv.h" does not match
 *                                              spelling of file name EthSM_Priv.h
 *                        vismha  ESCAN00076302 Compiler warning: EthSM.c line 945 truncating assignment
 *                        vismha  ESCAN00076303 PC-lint warning: indentation not correct
 *                        vismha  ESCAN00076304 MISRA deviation: MISRA-C:2004 Rule 19.7 and Rule 16.4
 *                        vismha  ESCAN00076699 Unconditional initialization and mode setting of Eth Controller and
 *                                              Transceiver by EthSM
 *                        vismha  ESCAN00076309 Linker error: Undefined symbol BrsTimeStopEthSM_MainFunction
 *  03.01.00  2014-07-15  vismha  ESCAN00073606 AR4-500: AUTOSAR CONC_600_SwitchConfiguration
 *                        vismha  ESCAN00062245 AR4-220: Remove STATIC
 *  03.01.01  2015-04-28  vismha  ESCAN00082729 Ethernet network stays in FULL_COM although Link and TcpIp
 *                                              communication is lost
 *  03.01.02  2015-11-06  vismha  ESCAN00077697 Compiler warning: Several warnings when the component is compiled
 *                                              with Visual Studio
 *            2015-11-27  vismha  ESCAN00085012 Compiler error: Missing identifier ETHSM_AR_RELEASE_MAJOR_VERSION
 *  03.01.03  2016-11-14  vismno  ESCAN00092052 Invalid MemMap.h section for variables INIT vs NOINIT
 *                                ESCAN00089264 Adapted critical section macros
 *                                ESCAN00093042 Always include ComM header file and adapt encapsulation in
 *                                              EthSM_NetworkType
 *********************************************************************************************************************/

#if !defined( ETHSM_H )
#define ETHSM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* EthSM Module includes */
#include "EthSM_Types.h"
#include "EthSM_Cfg.h"
#include "EthSM_Lcfg.h"

/* adjacent module includes */
#if( STD_OFF == ETHSM_DUMMY_MODE)
#include "EthIf.h"
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#  if ( STD_ON == ETHSM_IS_VECTOR_TCPIP)
#include "TcpIp.h"
#  else
#include "TcpIp_EthSM.h"
#  endif /* ETHSM_IS_VECTOR_TCPIP */
#endif /* ETHSM_IS_TCPIP_ENABLED */

#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
/* IpBase */
#include "IpBase.h"
#endif /* ETHSM_IS_VECTOR_TCPIP */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   Ccl_AsrSmEth CQComponent : Implementation */
/* BCD coded version number */
#define CCL_ASRSMETH_VERSION                   (0x0301u)
/* BCD coded release version number */
#define CCL_ASRSMETH_RELEASE_VERSION           (0x03u)

/* ETHSM software version */
#define ETHSM_SW_MAJOR_VERSION                 3u
#define ETHSM_SW_MINOR_VERSION                 1u
#define ETHSM_SW_PATCH_VERSION                 3u
/* ETHSM supported Autosar version */          
#define ETHSM_AR_RELEASE_MAJOR_VERSION         4u
#define ETHSM_AR_RELEASE_MINOR_VERSION         1u
#define ETHSM_AR_RELEASE_REVISION_VERSION      1u

/* ETHSM ModuleId */                           
/* Vector ID */
#define ETHSM_VENDOR_ID                        30u
/* Ethernet State Manager ID */
#define ETHSM_MODULE_ID                        143u

/* ETHSM API IDs according to Service IDs given in AUTOSAR SWS */
#define ETHSM_API_ID_MAIN_FUNCTION             0x01u
#define ETHSM_API_ID_GET_VERSION_INFO          0x02u
#define ETHSM_API_ID_GET_CURRENT_INTERNAL_MODE 0x03u
#define ETHSM_API_ID_GET_CURRENT_COM_MODE      0x04u
#define ETHSM_API_ID_REQUEST_COM_MODE          0x05u
#define ETHSM_API_ID_TRCV_LINK_STATE_CHG       0x06u
#define ETHSM_API_ID_INIT                      0x07u
#define ETHSM_API_ID_TCPIP_MODE_INDICATION     0x08u
#define ETHSM_API_ID_CTRL_MODE_INDICATION      0x09u
#define ETHSM_API_ID_TRCV_MODE_INDICATION      0x10u

/* ETHSM DET errors */
#define ETHSM_E_INVALID_NETWORK_MODE           0x01u
#define ETHSM_E_UNINIT                         0x02u
#define ETHSM_E_PARAM_POINTER                  0x03u
#define ETHSM_E_INVALID_NETWORK_HANDLE         0x04u
#define ETHSM_E_INVALID_TcpIpMode              0x05u
#define ETHSM_E_INVALID_TRCV_LINK_STATE        0x06u
#define ETHSM_E_PARAM_CONTROLLER               0x07u
#define ETHSM_E_PARAM_TRANSCEIVER              0x08u
#define ETHSM_E_ETHIF_TIMEOUT                  0x09u
#define ETHSM_E_STACKTYPE_WRONG                0x0Au
                                               
/* EthSM states */                             
/* States in NO_COMMUNICATION mode */
#define ETHSM_STATE_OFFLINE                    0x00u
#define ETHSM_STATE_WAIT_TRCVLINK              0x01u
#define ETHSM_STATE_WAIT_ONLINE                0x02u
/* States in FULL_COMMUNICATION mode*/
#define ETHSM_STATE_ONLINE                     0x03u
#define ETHSM_STATE_ONHOLD                     0x04u
#define ETHSM_STATE_WAIT_OFFLINE               0x05u

/* States to pass to BswM */
#define ETHSM_BSWM_OFFLINE                     ETHSM_STATE_OFFLINE
#define ETHSM_BSWM_WAIT_TRCVLINK               ETHSM_STATE_WAIT_TRCVLINK
#define ETHSM_BSWM_WAIT_ONLINE                 ETHSM_STATE_WAIT_ONLINE
#define ETHSM_BSWM_ONLINE                      ETHSM_STATE_ONLINE
#define ETHSM_BSWM_ONHOLD                      ETHSM_STATE_ONHOLD
#define ETHSM_BSWM_WAIT_OFFLINE                ETHSM_STATE_WAIT_OFFLINE

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHSM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  EthSM_InitMemory
 *********************************************************************************************************************/
/*! \brief      Initializes global variables.
 *  \warning    Has to be called before any other calls to the module.
 *  \context    Initialization
 *  \note       AUTOSAR extension.
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_InitMemory(void);

/**********************************************************************************************************************
 *  EthSM_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the data needed by EthSM for proper operation.
 *  \pre        EthSM_InitMemory should be called before.
 *  \warning    Has to be called before usage of the module.
 *  \context    Initialization
 *  \note       EthSM supports only the PRECOMPILE configuration variant.\n
 *              Therefore the API is declared like mentioned in SWS_EthSM_00044 although SWS_EthSM_00043 declares it
 *              with configuration pointer.
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_Init(void);

/**********************************************************************************************************************
 *  EthSM_MainFunction
 *********************************************************************************************************************/
/*! \brief      Main function of the Ethernet State Manager.
 *  \context    Task level
 *  \note       The main function processes the state transitions.
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_MainFunction(void);

/**********************************************************************************************************************
 *  EthSM_RequestComMode
 *********************************************************************************************************************/
/*! \brief      Requests a communication mode change for an Ethernet network.
 *  \param[in]  NetworkHandle         Network Handle of the Ethernet network.
 *  \param[in]  Mode                  Requested Communication Mode.\n
 *              COMM_NO_COMMUNICATION   : Request no communication.\n
 *              COMM_FULL_COMMUNICATION : Request full communication.
 *  \return     E_OK                  : Request was called with correct parameters.\n
 *              E_NOT_OK              : Request was called with erroneous parameters.
 *  \context    Task level
 *  \note       Dependent on the current state of the Ethernet Network and the requested communication mode transitions
 *              to other states are triggered and processed in next main function cycle.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_RequestComMode(NetworkHandleType NetworkHandle, ComM_ModeType Mode);

/**********************************************************************************************************************
 *  EthSM_GetCurrentComMode
 *********************************************************************************************************************/
/*! \brief      Get the current communication mode for an Ethernet network.
 *  \param[in]  NetworkHandle         Network Handle of the Ethernet network.
 *  \param[out] ModePtr               Pointer to store the communication mode value.
 *  \return     E_OK                  : Communication mode of network could be retrieved.\n
 *              E_NOT_OK              : Communication mode of network couldn't be retrieved.
 *  \context    Task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentComMode(NetworkHandleType NetworkHandle,
                                                                ETHSM_P2VARAPPLDATA(ComM_ModeType) ModePtr);

/**********************************************************************************************************************
 *  EthSM_GetCurrentInternalMode
 *********************************************************************************************************************/
/*! \brief      Get the current internal EthSM state for an Ethernet network.
 *  \param[in]  NetworkHandle         Network Handle of the Ethernet network.
 *  \param[out] ModePtr               Pointer to store the internal state value.
 *  \return     E_OK                  : Internal state of network could be retrieved.\n
 *              E_NOT_OK              : Internal state of network couldn't be retrieved.
 *  \context    Task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentInternalMode(NetworkHandleType NetworkHandle,
                                                                     ETHSM_P2VARAPPLDATA(EthSM_NetworkModeStateType) ModePtr);

#if ( STD_OFF == ETHSM_DUMMY_MODE )
/**********************************************************************************************************************
 *  EthSM_TrcvLinkStateChg
 *********************************************************************************************************************/
/*! \brief      Called by EthIf to report an Link State change for an Ethernet network.
 *  \param[in]  NetworkHandle         Network Handle of the Ethernet network.
 *  \param[in]  TransceiverLinkState  Reported Link State.
 *  \return     E_OK                  : Link State change called with correct parameters.\n
 *              E_NOT_OK              : Link State change called with erroneous parameters.
 *  \context    Task level
 *  \note       Dependent on the current state of the Ethernet Network and the reported Link State transitions to
 *              other states are triggered and processed in next main function cycle.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_TrcvLinkStateChg(NetworkHandleType NetworkHandle,
                                                               EthTrcv_LinkStateType TransceiverLinkState);
#  if ( STD_ON == ETHSM_IS_VECTOR_ETHIF )
/**********************************************************************************************************************
 *  EthSM_VTrcvLinkStateChg
 *********************************************************************************************************************/
/*! \brief      Called by EthIf to report an Link State change for an EthIf Controller.
 *  \param[in]  CtrlIdx               Index of the EthIf Controller.
 *  \param[in]  TrcvLinkState         Reported Link State.
 *  \return     E_OK                  : Link State change called with correct parameters.\n
 *              E_NOT_OK              : Link State change called with erroneous parameters.
 *  \context    Task level
 *  \note       This API wraps the AUTOSAR API EthSM_TrcvLinkStateChg to fit the EthSM according to AUTOSAR 4.1.1 into
 *              the Vector IP-Stack.\n
 *              If the EthSM isn't used in a configuration containing the Vector EthIf the API will not be available.
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_VTrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState);
#  endif /* ETHSM_IS_VECTOR_ETHIF */
#endif /* ETHSM_DUMMY_MODE */

#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
/**********************************************************************************************************************
 *  EthSM_TcpIpModeIndication
 *********************************************************************************************************************/
/*! \brief      Called by TcpIp to report an TcpIp mode change for an Ethernet network.
 *  \param[in]  NetworkHandle         Network Handle of the Ethernet network.
 *  \param[in]  TransceiverLinkState  Reported Link State.
 *  \return     E_OK                  : TcpIp mode indication called with correct parameters.\n
 *              E_NOT_OK              : TcpIp mode indication called with erroneous parameters.
 *  \context    Task level
 *  \note       Dependent on the current state of the Ethernet Network and the reported TcpIp mode transitions to
 *              other states are triggered and processed in next main function cycle.\n
 *              If no TcpIp is contained in the configuration the API will not be available. Use Cases for this
 *              configuration variant are e.g. AVB use cases.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_TcpIpModeIndication(NetworkHandleType NetworkHandle,
                                                                  TcpIp_StateType TcpIpState);

#  if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
/**********************************************************************************************************************
 *  EthSM_VTcpIpModeIndication
 *********************************************************************************************************************/
/*! \brief      Called by IpVx to report a address availability for the Ethernet network.
 *  \param[in]  CtrlIdx               Index of the EthIf Controller.
 *  \param[in]  IsAnyAddrOnCtrlActive Reported availability of an IP-Address.
 *  \return     void
 *  \context    Task level
 *  \note       This API wraps the AUTOSAR API EthSM_TcpIpModeIndication to fit the EthSM according to ASR 4.1.1 into
 *              the Vector IP-Stack.\n
 *              If the EthSM isn't used in a configuration containing the Vector TcpIp the API will not be available.
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_VTcpIpModeIndication(uint8 CtrlIdx, boolean IsAnyAddrOnCtrlActive);
#  endif /* ETHSM_IS_VECTOR_TCPIP */
#endif /* ETHSM_IS_TCPIP_ENABLED */

#if defined( ETHSM_ENABLE_CTRL_TRCV_IND_API )
#  if ( STD_ON == ETHSM_ENABLE_CTRL_TRCV_IND_API )
/**********************************************************************************************************************
 *  EthSM_CtrlModeIndication
 *********************************************************************************************************************/
/*! \brief      Purpose of this API not specified by AUTOSAR SWS.
 *  \param[in]  CtrlIdx        Controller Index
 *  \param[in]  CtrlMode       Controller Mode
 *  \context    Task level
 *  \note       The API is an empty implementation and not available by default.\n
 *              To enable the API use a User Config File and add\n
 *              #define ETHSM_ENABLE_CTRL_TRCV_IND_API == STD_ON
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode);

/**********************************************************************************************************************
 *  EthSM_TrcvModeIndication
 *********************************************************************************************************************/
/*! \brief      Purpose of this API not specified by AUTOSAR SWS.
 *  \param[in]  TrcvIdx        Transceiver Index
 *  \param[in]  TrcvMode       Transceiver Mode
 *  \context    Task level
 *  \note       The API is an empty implementation and not available by default.\n
 *              To enable the API use a User Config File and add\n
 *              #define ETHSM_ENABLE_CTRL_TRCV_IND_API == STD_ON
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_TrcvModeIndication(uint8 TrcvIdx, EthTrcv_ModeType TrcvMode);
#  endif /* ETHSM_ENABLE_CTRL_TRCV_IND_API */
#endif /* ETHSM_ENABLE_CTRL_TRCV_IND_API */

#if ( defined ETHSM_VERSION_INFO_API ) && ( ETHSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  EthSM_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Get Ethernet State Manager version information.
 *  \param[in]  VersionInfoPtr        Pointer to store the version info.
 *  \context    Task level
 *********************************************************************************************************************/
extern FUNC(void, ETHSM_CODE) EthSM_GetVersionInfo(ETHSM_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr);
#endif /* ETHSM_VERSION_INFO_API */

#define ETHSM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#endif /* ETHSM_H */

/**********************************************************************************************************************
 *  END OF FILE: EthSM.h
 *********************************************************************************************************************/
