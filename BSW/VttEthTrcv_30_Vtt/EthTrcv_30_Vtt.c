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
 *         File:  EthTrcv_30_Vtt.c
 *      Project:  MICROSAR Eth
 *       Module:  Ethernet Transceiver Driver for VTT
 *    Generator:  DrvTrans_VttEthAsr.jar
 *
 *  Description:  Source file containing the modules implementation
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ETHTRCV_30_VTT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTrcv_30_Vtt.h"
#include "EthTrcv_30_Vtt_Priv.h"
#include "CANoeApi.h"
#include "VttCntrl_Eth.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if (                                                 \
      (ETHTRCV_30_VTT_SW_MAJOR_VERSION != 4u) || \
      (ETHTRCV_30_VTT_SW_MINOR_VERSION != 2u) || \
      (ETHTRCV_30_VTT_SW_PATCH_VERSION != 2u)    \
    )
  #error "EthTrcv_30_Vtt.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if defined (STATIC)
/* nothing todo */
#else
#define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
/* PRQA S 5087,3614 ASR_MEMMAP *//* MISRA 19.1: AUTOSAR Memory Mapping */

#define ETHTRCV_30_VTT_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"
#if ( (ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE == STD_ON) || (ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE == STD_ON) )
STATIC VAR(EthTrcv_ModeType, ETHTRCV_30_VTT_VAR_NOINIT) EthTrcv_30_Vtt_Mode[ETHTRCV_30_VTT_MAX_TRCVS_TOTAL];
#endif
  /* ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE, ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE */
#define ETHTRCV_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"

#define ETHTRCV_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"
#if ( ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON )
STATIC VAR(EthTrcv_StateType, ETHTRCV_30_VTT_VAR_ZERO_INIT) EthTrcv_30_Vtt_State[ETHTRCV_30_VTT_MAX_TRCVS_TOTAL];
#endif
  /* ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON */
#define ETHTRCV_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"

#define ETHTRCV_30_VTT_START_SEC_CODE
#include "MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* PRQA S 3201,3109,2006 ASR_DET *//* MISRA 14.1,14.3,14.7: AUTOSAR Det Checks */
/* PRQA S 0277,4130 UNSIGNED_BIT_OP *//* MISRA 3.1: Unsigned bit operation due to hardware oriented programming */
/**********************************************************************************************************************
 *  EthTrcv_30_Vtt_InitMemory
 **********************************************************************************************************************/
/*! \brief      Initializes global variables
 *  \param      void
 *  \return     void
 *  \context    Initialization
 *  \warning    Has to be called before usage of the module
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_InitMemory(void)
{
#if ( ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON )
  {
    uint8 Idx = ETHTRCV_30_VTT_MAX_TRCVS_TOTAL;
    while (0 < Idx)
    {
      Idx--;
      EthTrcv_30_Vtt_State[Idx] = ETHTRCV_STATE_UNINIT;
    }
  }
#endif
  /* ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON */
}
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
FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_Init(
  P2CONST(EthTrcv_30_Vtt_ConfigType, AUTOMATIC, ETHTRCV_30_VTT_CONST) CfgPtr)
{
#if (ETHTRCV_30_VTT_USE_DUMMY_STATEMENT == STD_ON)
  CfgPtr = CfgPtr; /* PRQA S 3199 */ /* MD_MSR_14.2  */
#endif

#if ( ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON )
  {
    uint8 Idx = ETHTRCV_30_VTT_MAX_TRCVS_TOTAL;
    while (0 < Idx)
    {
      Idx--;
      EthTrcv_30_Vtt_State[Idx] = ETHTRCV_STATE_INIT;
    }
  }
#endif
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_TransceiverInit(
  uint8 TrcvIdx,
  uint8 CfgIdx)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_TRANSCEIVER_INIT,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_INIT <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_TRANSCEIVER_INIT,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_CFGS_TOTAL > CfgIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_TRANSCEIVER_INIT,
    ETHTRCV_30_VTT_E_INV_CONFIG,
    E_NOT_OK);
    
  /* trigger start of runtime measurement */
  EthTrcv_30_Vtt_Rtm_Start(TransceiverInit);

  ETHTRCV_30_VTT_DUMMY_STATEMENT(CfgIdx);  /* PRQA S 3199 */ /* MD_MSR_14.2  */
  ETHTRCV_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_MSR_14.2  */

#if ( ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON )
  EthTrcv_30_Vtt_State[TrcvIdx] = ETHTRCV_STATE_ACTIVE;
#endif
  /* ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON */
#if ( (ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE == STD_ON) || (ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE == STD_ON) )
  EthTrcv_30_Vtt_Mode[TrcvIdx] = ETHTRCV_MODE_DOWN;
#endif
  /* ETHTRCV_30_VTT_ENABLE_SET_TRCV_MODE || ETHTRCV_30_VTT_ENABLE_GET_TRCV_MODE */

  /* trigger start of runtime measurement */
  EthTrcv_30_Vtt_Rtm_Stop(TransceiverInit);
  
  return E_OK;
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_SetTransceiverMode(
  uint8 TrcvIdx,
  EthTrcv_ModeType TrcvMode)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_SET_TRANSCEIVER_MODE,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_SET_TRANSCEIVER_MODE,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);

  EthTrcv_30_Vtt_Mode[TrcvIdx] = TrcvMode;

  return E_OK;
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetTransceiverMode(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_ModeType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) TrcvModePtr)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_TRANSCEIVER_MODE,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_TRANSCEIVER_MODE,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != TrcvModePtr),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_TRANSCEIVER_MODE,
    ETHTRCV_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  *TrcvModePtr = EthTrcv_30_Vtt_Mode[TrcvIdx];

  return E_OK;
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_StartAutoNegotiation(
  uint8 TrcvIdx)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_START_AUTO_NEG,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_START_AUTO_NEG,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);

  ETHTRCV_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_MSR_14.2  */
  return E_OK;
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetLinkState(
  uint8 TrcvIdx, 
  P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) LinkStatePtr)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_LINK_STATE,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_LINK_STATE,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != LinkStatePtr),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_LINK_STATE,
    ETHTRCV_30_VTT_E_INV_POINTER,    
    E_NOT_OK);

  if(ETHTRCV_MODE_ACTIVE == EthTrcv_30_Vtt_Mode[TrcvIdx])
  {
    /* Index for channel is controller index plus one! Ensure that CANoe channel
     * configuration is consistent with controller indexes */

    if ((VTTETH_STATE_CONNECTED == VttEth_GetStatus(EthTrcv_30_Vtt_VCfgGetCtrlIdx(TrcvIdx))) ||
        (VTTETH_STATE_UNKNOWN == VttEth_GetStatus(EthTrcv_30_Vtt_VCfgGetCtrlIdx(TrcvIdx))))
    {
      *LinkStatePtr = ETHTRCV_LINK_STATE_ACTIVE;
    }
    else if(VTTETH_STATE_NOT_CONNECTED == VttEth_GetStatus(EthTrcv_30_Vtt_VCfgGetCtrlIdx(TrcvIdx)))
    {
      *LinkStatePtr = ETHTRCV_LINK_STATE_DOWN;
    }
    else if(VTTETH_STATE_CHANNEL_UNKNOWN == VttEth_GetStatus(EthTrcv_30_Vtt_VCfgGetCtrlIdx(TrcvIdx)))
    {
      /* internal error - link state for non existing channel requested */
      *LinkStatePtr = ETHTRCV_LINK_STATE_DOWN;
    }
  }
  else
  {
    *LinkStatePtr = ETHTRCV_LINK_STATE_DOWN;
  }

  return E_OK;
}
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
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetBaudRate(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_BaudRateType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) BaudRatePtr)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_BAUD_RATE,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_BAUD_RATE,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != BaudRatePtr),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_BAUD_RATE,
    ETHTRCV_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  ETHTRCV_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_MSR_14.2  */
  *BaudRatePtr = ETHTRCV_BAUD_RATE_100MBIT;

  return E_OK;
}
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
#if ( ETHTRCV_30_VTT_ENABLE_GET_BAUD_RATE == STD_ON )
FUNC(Std_ReturnType, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetDuplexMode(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_DuplexModeType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) DuplexModePtr)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_30_VTT_MAX_TRCVS_TOTAL > TrcvIdx),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_DUPLEX_MODE,
    ETHTRCV_30_VTT_E_INV_TRCV_IDX,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (ETHTRCV_STATE_ACTIVE <= EthTrcv_30_Vtt_State[TrcvIdx]),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_DUPLEX_MODE,
    ETHTRCV_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  EthTrcv_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != DuplexModePtr),
    TrcvIdx,
    ETHTRCV_30_VTT_API_ID_GET_DUPLEX_MODE,
    ETHTRCV_30_VTT_E_INV_POINTER,
    E_NOT_OK);
  
  ETHTRCV_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_MSR_14.2  */
  *DuplexModePtr = ETHTRCV_DUPLEX_MODE_FULL;
  
  return E_OK;
}
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
#if (ETHTRCV_30_VTT_VERSION_INFO_API == STD_ON)
FUNC(void, ETHTRCV_30_VTT_CODE) EthTrcv_30_Vtt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTRCV_30_VTT_APPL_DATA) VersionInfoPtr)
{
  EthTrcv_30_Vtt_CheckDetErrorReturnVoid(
    (NULL_PTR != VersionInfoPtr),
    0,
    ETHTRCV_30_VTT_API_ID_GET_VERSION_INFO,
    ETHTRCV_30_VTT_E_INV_POINTER);

  VersionInfoPtr->vendorID = ETHTRCV_30_VTT_VENDOR_ID;
  VersionInfoPtr->moduleID = ETHTRCV_30_VTT_MODULE_ID;
  VersionInfoPtr->sw_major_version = ETHTRCV_30_VTT_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = ETHTRCV_30_VTT_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = ETHTRCV_30_VTT_SW_PATCH_VERSION;
}
#endif
  /* ETHTRCV_30_VTT_VERSION_INFO_API */

#define ETHTRCV_30_VTT_STOP_SEC_CODE
#include "MemMap.h"
/* PRQA L:UNSIGNED_BIT_OP */
/* PRQA L:ASR_DET */
/* PRQA L:ASR_MEMMAP */
/**********************************************************************************************************************
 *  END OF FILE: EthTrcv_30_Vtt.c
 *********************************************************************************************************************/
