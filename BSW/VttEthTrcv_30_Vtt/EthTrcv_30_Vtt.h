/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  EthTrcv_30_Vtt.h
 *      Project:  MICROSAR Eth
 *       Module:  Ethernet Transceiver Driver for VTT
 *    Generator:  DrvTrans_VttEthAsr.jar
 *
 *  Description:  Module header file
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Harald Walter                 vishaw        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  David Feﬂler                  visfer        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2015-01-16  visfer  ESCAN00080252 Renamed to VTT
 *            2015-03-06  vismha                further adaptions to achieve proper VTT renaming
 *  04.00.01  2015-08-07  visfer  ESCAN00084421 Link is reported as active in error condition
 *  04.00.02  2015-09-09  visfer  ESCAN00085089 bcd-coded version number is wrong
 *  04.01.00  2016-03-29  visfer  ESCAN00089106 osek.h should not be included directly
 *  04.02.00  2016-06-28  visfer  ESCAN00090721 Remove distinction between single and multi transceiver usecases
 *  04.02.01  2016-11-22  visfer  ESCAN00091968 Compiler error: Undefined symbol CANoeAPI_GetValueInt in EthTrcv_30_Vtt.c
 *  04.02.02  2017-07-21  visfer  ESCAN00096002 Compiler warning: Several unreferenced formal parameter warnings
 *********************************************************************************************************************/

#if !defined (ETHTRCV_30_VTT_H)
# define ETHTRCV_30_VTT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTrcv_30_Vtt_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   DrvTrans_VttEthAsr CQComponent : Implementation */
#define DRVTRANS_VTTETHASR_VERSION          (0x0402u) /* BCD coded version number */
#define DRVTRANS_VTTETHASR_RELEASE_VERSION  (0x01u) /* BCD coded release version number */

/* ETHTRCV software version */
#define ETHTRCV_30_VTT_SW_MAJOR_VERSION                      4u
#define ETHTRCV_30_VTT_SW_MINOR_VERSION                      2u
#define ETHTRCV_30_VTT_SW_PATCH_VERSION                      2u


/* AUTOSAR 4.x Software Specification Version Information */
#define ETHTRCV_30_VTT_AR_RELEASE_MAJOR_VERSION               (0x04)
#define ETHTRCV_30_VTT_AR_RELEASE_MINOR_VERSION               (0x01)
#define ETHTRCV_30_VTT_AR_RELEASE_REVISION_VERSION            (0x01)

/* ETHTRCV ModuleId */
/* Vector ID */
#define ETHTRCV_30_VTT_VENDOR_ID                            30u
/* Ethernet Transceiver Driver ID */
#define ETHTRCV_30_VTT_MODULE_ID                            73u

/* ETHTRCV ApiIds */
#define ETHTRCV_30_VTT_API_ID_INIT                        0x01u
#define ETHTRCV_30_VTT_API_ID_TRANSCEIVER_INIT            0x02u
#define ETHTRCV_30_VTT_API_ID_SET_TRANSCEIVER_MODE        0x03u
#define ETHTRCV_30_VTT_API_ID_GET_TRANSCEIVER_MODE        0x04u
#define ETHTRCV_30_VTT_API_ID_START_AUTO_NEG              0x05u
#define ETHTRCV_30_VTT_API_ID_GET_LINK_STATE              0x06u
#define ETHTRCV_30_VTT_API_ID_GET_BAUD_RATE               0x07u
#define ETHTRCV_30_VTT_API_ID_GET_DUPLEX_MODE             0x08u
#define ETHTRCV_30_VTT_API_ID_GET_VERSION_INFO            0x09u

/* ETHTRCV DET errors */
#define ETHTRCV_30_VTT_E_INV_TRCV_IDX                     0x01u
#define ETHTRCV_30_VTT_E_NOT_INITIALIZED                  0x02u
#define ETHTRCV_30_VTT_E_INV_POINTER                      0x03u
#define ETHTRCV_30_VTT_E_INV_CONFIG                       0x04u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHTRCV_30_VTT_START_SEC_CODE
#include "MemMap.h"
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_InitMemory
 **********************************************************************************************************************/
/*! \brief      Initializes global variables
 *  \param      void
 *  \return     void
 *  \context    Initialization
 *  \warning    Has to be called before usage of the module
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
extern FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_InitMemory(void);
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_Init
 **********************************************************************************************************************/
/*! \brief      Stores the start address of the post build time configuration of the module and may be used to
 *              initialize the data  structures
 *  \param[in]  CfgPtr             Pointer to module configuration
 *  \return     void
 *  \warning    Has to be called before usage of the module
 *  \context    Initialization
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
extern FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_Init(
  P2CONST(EthTrcv_30_Vtt_ConfigType, AUTOMATIC, ETHTRCV_30_VTT_CONST) CfgPtr);
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_TransceiverInit
 **********************************************************************************************************************/
/*! \brief      Initializes an Ethernet transceiver (register configuration)
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[in]  CfgIdx                Configuration index
 *  \return     E_OK                  : Transceiver configured\n
 *              E_NOT_OK              : Transceiver configuration failed
 *  \warning    Has to be called before usage of the module
 *  \context    Initialization
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_TransceiverInit(
  uint8 TrcvIdx,
  uint8 CfgIdx);
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_SetTransceiverMode
 **********************************************************************************************************************/
/*! \brief      Set transceiver mode
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[in]  TrcvMode              Transceiver mode
 *  \return     E_OK                  : Transceiver mode changed\n
 *              E_NOT_OK              : Transceiver mode change failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_SetTransceiverMode(
  uint8 TrcvIdx,
  EthTrcv_ModeType TrcvMode);
#endif
  /* ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_GetTransceiverMode
 **********************************************************************************************************************/
/*! \brief      Get transceiver mode
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[in]  TrcvModePtr           Pointer for transceiver mode
 *  \return     E_OK                  : Transceiver mode evaluated\n
 *              E_NOT_OK              : Transceiver mode evaluation failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetTransceiverMode(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_ModeType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) TrcvModePtr);
#endif
  /* ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_StartAutoNegotiation
 **********************************************************************************************************************/
/*! \brief      Start automatic mode negotiation (10/100MBit, Full/Half-Duplex)
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \return     E_OK                  : Auto negotiation started\n
 *              E_NOT_OK              : Auto negotiation start failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_START_AUTO_NEG == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_StartAutoNegotiation(
  uint8 TrcvIdx);
#endif
  /* ETHTRCV_30_VTT_ENABLE_START_AUTO_NEG */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_GetLinkState
 **********************************************************************************************************************/
/*! \brief      Get transceiver link state
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[out] LinkStatePtr          Pointer for link state value
 *  \return     E_OK                  : Link state read\n
 *              E_NOT_OK              : Link state read failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_GET_LINK_STATE == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetLinkState(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) LinkStatePtr);
#endif
  /* ETHTRCV_30_VTT_ENABLE_GET_LINK_STATE */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_GetBaudRate
 **********************************************************************************************************************/
/*! \brief      Get transceiver baud rate
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[out] BaudRatePtr           pointer for baud rate value
 *  \return     E_OK                  : Baud rate read\n
 *              E_NOT_OK              : Baud rate read failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_GET_BAUD_RATE == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetBaudRate(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_BaudRateType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) BaudRatePtr);
#endif
  /* ETHTRCV_30_VTT_ENABLE_GET_BAUD_RATE */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_GetDuplexMode
 **********************************************************************************************************************/
/*! \brief      Get transceiver duplex mode
 *  \param[in]  TrcvIdx               Zero based index of the transceiver
 *  \param[out] DuplexModePtr         Pointer for duplex mode value
 *  \return     E_OK                  : Duplex mode read\n
 *              E_NOT_OK              : Duplex mode read failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 *  \pre        Init and TransceiverInit must be called before, otherwise a DET is thrown (if enabled)
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_ENABLE_GET_DUPLEX_MODE == STD_ON )
extern FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetDuplexMode(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_DuplexModeType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) DuplexModePtr);
#endif
  /* ETHTRCV_30_VTT_ENABLE_GET_DUPLEX_MODE */
/***********************************************************************************************************************
 *  EthTrcv_30_Vtt_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      Returns version information, vendor ID and AUTOSAR module ID of the component.
 *              The versions are BCD-coded.
 *  \param[out] VersionInfoPtr        Returns the following version information:\n
 *                                     - Vendor ID\n
 *                                     - Module ID\n
 *                                     - Software major version\n
 *                                     - Software minor version\n
 *                                     - Software patch version
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
#if ( ETHTRCV_30_VTT_VERSION_INFO_API == STD_ON )
extern FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) VersionInfoPtr);
#endif
  /* ETHTRCV_30_VTT_VERSION_INFO_API == STD_ON */

#define ETHTRCV_30_VTT_STOP_SEC_CODE
#include "MemMap.h"
#endif 
  /* ETHTRCV_30_VTT_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTrcv_30_Vtt.h
 *********************************************************************************************************************/
