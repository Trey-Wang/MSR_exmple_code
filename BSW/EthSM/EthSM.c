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
 *         File:  EthSM.c
 *      Project:  MICROSAR ETH
 *       Module:  Ethernet State Manager
 *    Generator:  Ccl_AsrSmEth.jar
 *
 *  Description:  implementation
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


#define ETHSM_SOURCE

/* Turn off PC-lint warnings issued -> issues known and coverd by code review */
/*lint -e438 */ /* Suppress ID438 because Config pointer only used in Post-Build Variant */
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* EthSM Module includes */
#include "EthSM.h"
#include "SchM_EthSM.h"
#include "EthSM_Priv.h"
#include "EthSM_Types.h"

/* adjacent module includes */
#if ( STD_ON == ETHSM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* ETHSM_DEV_ERROR_DETECT */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
#include "BswM_EthSM.h"
#endif /* ETHSM_ENABLE_SYSSERVICE_BSWECUM */
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
#include "ComM.h"
#include "ComM_BusSM.h"
#endif /* ETHSM_ENABLE_CCL_ASRCOMM */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
#include "EthIf.h"
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#  if ( STD_ON == ETHSM_IS_VECTOR_TCPIP)
#include "TcpIp.h"
#  else
#include "TcpIp_EthSM.h"
#  endif /* ETHSM_IS_VECTOR_TCPIP */
#endif /* ETHSM_IS_TCPIP_ENABLED */

/* global type definition includes */
#include "ComStack_Types.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (ETHSM_SW_MAJOR_VERSION != 3u) || (ETHSM_SW_MINOR_VERSION != 1u) || (ETHSM_SW_PATCH_VERSION != 3u) )
  #error "EthSM.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION VARIANT CHECK
 *********************************************************************************************************************/
#if ( ETHSM_CONFIGURATION_VARIANT_PRECOMPILE != ETHSM_CONFIGURATION_VARIANT )
  #error "EthSM.c: Only PreCompile configuration variant supported!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHSM_CTRL_CFG_IDX  0
#define ETHSM_TRCV_CFG_IDX  0

#ifndef STATIC
#define STATIC static
#endif /* STATIC */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define ETHSM_START_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/*
 * Because there is no UNINIT state in the EthSM_NetworkModeStateType defined this boolean value is used to
 * indicate if EthSM was initialized.
 */
ETHSM_VARZEROINIT(boolean) EthSM_IsInitialized = FALSE;

#define ETHSM_STOP_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define ETHSM_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

STATIC ETHSM_VARNOINIT(EthSM_NetworkStateMngmtType) EthSM_NetworkStateMngmt[EthSM_VCfgGetNetworkTotal()];

#define ETHSM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
#define EthSM_IsIpActiveOnNetwork(NetIdx) \
          (boolean)((0 == EthSM_NetworkStateMngmt[(NetIdx)].ActiveIpVxCnt) ? FALSE : TRUE)
/* PRQA L:FCT_MACROS */
#endif /* ETHSM_IS_VECTOR_TCPIP */

#define ETHSM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthSM_GetNetIdxByComMNetwHnd
 *********************************************************************************************************************/
/*! \brief      Derives the EthSM Network index from the ComM Channel Handle.
 *  \param[in]  ComM Channel Handle
 *  \return     EthSM Network index (ETHSM_INVALID_NETWORK_IDX if no Ethernet Network for passed ComM Channel Handle
 *              exists)
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(EthSM_NetworkIdxType, ETHSM_CODE) EthSM_GetNetIdxByComMNetwHnd(NetworkHandleType NetworkHandle);

#if ( ( STD_ON == ETHSM_IS_VECTOR_ETHIF ) || ( STD_ON == ETHSM_IS_VECTOR_TCPIP ) )
/**********************************************************************************************************************
 *  EthSM_GetNetIdxByVirtCtrlIdx
 *********************************************************************************************************************/
/*! \brief      Derives the EthSM Network index from the EthIf Controller Index.
 *  \param[in]  EthIf Controller Index
 *  \return     EthSM Network index (ETHSM_INVALID_NETWORK_IDX if no Ethernet Network for passed EthIf Controller
 *              Index exists)
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(EthSM_NetworkIdxType, ETHSM_CODE) EthSM_GetNetIdxByEthIfCtrlIdx(uint8 CtrlIdx);
#endif /* ETHSM_IS_VECTOR_ETHIF, ETHSM_IS_VECTOR_TCPIP */

/**********************************************************************************************************************
 *  EthSM_TransOfflineToWaitTrcvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state OFFLINE to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOfflineToWaitTrcvLink(EthSM_NetworkIdxType NetIdx);

/**********************************************************************************************************************
 *  EthSM_TransWaitTrcvLinkToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_TRCVLINK to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitTrcvLinkToOffline(EthSM_NetworkIdxType NetIdx);

/**********************************************************************************************************************
 *  EthSM_TransWaitTrcvLinkToWaitOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_TRCVLINK to WAIT_ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitTrcvLinkToWaitOnline(EthSM_NetworkIdxType NetIdx);

#if ( STD_OFF == ETHSM_DUMMY_MODE )
/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToWaitTrvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToWaitTrvLink(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_DUMMY_MODE */

/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToOffline(EthSM_NetworkIdxType NetIdx);

/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToOnline(EthSM_NetworkIdxType NetIdx);

#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
/**********************************************************************************************************************
 *  EthSM_TransOnlineToWaitOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to WAIT_ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToWaitOnline(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_IS_TCPIP_ENABLED */

#if ( STD_OFF == ETHSM_DUMMY_MODE )
/**********************************************************************************************************************
 *  EthSM_TransOnlineToOnhold
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to ONHOLD.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToOnhold(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_DUMMY_MODE */

/**********************************************************************************************************************
 *  EthSM_TransOnlineToWaitOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to WAIT_OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToWaitOffline(EthSM_NetworkIdxType NetIdx);

#if ( STD_OFF == ETHSM_DUMMY_MODE)
/**********************************************************************************************************************
 *  EthSM_TransOnholdToOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToOnline(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_DUMMY_MODE */

#if ( ( ( STD_OFF == ETHSM_DUMMY_MODE ) && ( STD_OFF == ETHSM_IS_TCPIP_ENABLED ) ) \
         || ( STD_ON == ETHSM_IS_TCPIP_ENABLED ) )
/**********************************************************************************************************************
 *  EthSM_TransOnholdToWaitTrcvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToWaitTrcvLink(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_DUMMY_MODE, ETHSM_IS_TCPIP_ENABLED */

/**********************************************************************************************************************
 *  EthSM_TransOnholdToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToOffline(EthSM_NetworkIdxType NetIdx);

/**********************************************************************************************************************
 *  EthSM_TransWaitOfflineToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_OFFLINE to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOfflineToOffline(EthSM_NetworkIdxType NetIdx);

#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
/**********************************************************************************************************************
 *  EthSM_ProcessEthIfCallRetries
 *********************************************************************************************************************/
/*! \brief      Process the EthIf call retry handling.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(void, ETHSM_CODE) EthSM_ProcessEthIfCallRetries(EthSM_NetworkIdxType NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthSM_InitMemory
 *********************************************************************************************************************/
/*! \brief      Initializes global variables.
 *  \warning    Has to be called before any other calls to the module.
 *  \context    Initialization
 *  \note       AUTOSAR extension.
 *********************************************************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_InitMemory(void)
{
  EthSM_IsInitialized = FALSE;
}

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
FUNC(void, ETHSM_CODE) EthSM_Init(void)
{
  EthSM_NetworkIdxType NetIdx = EthSM_VCfgGetNetworkTotal();

  /*
   * Initialize the state management for each network.
   */
  while( (EthSM_NetworkIdxType)0U < NetIdx )
  {
    --NetIdx;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
    EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime = EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout;
    EthSM_NetworkStateMngmt[NetIdx].RetriesLeft    = EthSM_VCfgGetNetworks()[NetIdx].MaxRetries;
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
    EthSM_NetworkStateMngmt[NetIdx].Mode           = COMM_NO_COMMUNICATION;
    EthSM_NetworkStateMngmt[NetIdx].State          = ETHSM_STATE_OFFLINE;
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans  = NULL_PTR;
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
    EthSM_NetworkStateMngmt[NetIdx].TcpIpState     = TCPIP_STATE_OFFLINE;
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    EthSM_NetworkStateMngmt[NetIdx].TrcvLinkState  = ETHTRCV_MODE_DOWN;
#endif /* ETHSM_DUMMY_MODE */
#if (( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT ) || ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS ))
    EthSM_IndCtrlBitfield_Init(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT, ETHSM_ENABLE_OOO_INDICATIONS */
#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
    EthSM_NetworkStateMngmt[NetIdx].ActiveIpVxCnt  = 0u;
#endif /* ETHSM_IS_VECTOR_TCPIP */
  }

  EthSM_IsInitialized = TRUE;

}

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
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_RequestComMode(NetworkHandleType NetworkHandle, ComM_ModeType Mode)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByComMNetwHnd(NetworkHandle);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnValue( \
      ETHSM_API_ID_REQUEST_COM_MODE,    \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(        \
      (ETHSM_INV_NETWORK_IDX != NetIdx), \
      ETHSM_API_ID_REQUEST_COM_MODE,     \
      ETHSM_E_INVALID_NETWORK_HANDLE,    \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(                                                                                    \
      ((COMM_NO_COMMUNICATION == Mode) || (COMM_FULL_COMMUNICATION == Mode) || (COMM_SILENT_COMMUNICATION == Mode)), \
      ETHSM_API_ID_REQUEST_COM_MODE,                                                                                 \
      ETHSM_E_INVALID_NETWORK_MODE,                                                                                  \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* SILENT_COMMUNICATION according to SWS_EthSM_00087 (SREQ00018408) of EthSM AUTOSAR SWS not supported
   * -> ignore mode request like mentioned in SWS_EthSM_00188 (SREQ00025109)
   */
  if( COMM_SILENT_COMMUNICATION != Mode )
  {
    /*
     * Check for current communication mode to limit possible states of state machine.
     */
    if( COMM_NO_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
    {
      if(( ETHSM_STATE_OFFLINE == EthSM_NetworkStateMngmt[NetIdx].State )
            && ( COMM_FULL_COMMUNICATION == Mode ))
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOfflineToWaitTrcvLink;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
        EthSM_ProcessEthIfCallRetries(NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
      }
      else if(( ETHSM_STATE_WAIT_TRCVLINK == EthSM_NetworkStateMngmt[NetIdx].State )
                 && ( COMM_NO_COMMUNICATION == Mode ))
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitTrcvLinkToOffline;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
        EthSM_ProcessEthIfCallRetries(NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
      }
      else if(( ETHSM_STATE_WAIT_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
                && ( COMM_NO_COMMUNICATION == Mode ))
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToOffline;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
        EthSM_ProcessEthIfCallRetries(NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
      }
      else
      {
        /* nothing to do, only for MISRA compliance */
      }
    }
    else if( COMM_FULL_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
    {
      if(( ETHSM_STATE_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
            && ( COMM_NO_COMMUNICATION == Mode ))
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnlineToWaitOffline;
      }
      else if(( ETHSM_STATE_ONHOLD == EthSM_NetworkStateMngmt[NetIdx].State )
                 && ( COMM_NO_COMMUNICATION == Mode ))
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToOffline;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
        EthSM_ProcessEthIfCallRetries(NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
      }
      else
      {
        /* nothing to do, only for MISRA compliance */
      }
    }
    else
    {
      /* nothing to do, only for MISRA compliance */
    }
  }
  
  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030,6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

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
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentComMode(NetworkHandleType NetworkHandle,
                                                         ETHSM_P2VARAPPLDATA(ComM_ModeType) ModePtr)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByComMNetwHnd(NetworkHandle);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnValue(  \
      ETHSM_API_ID_GET_CURRENT_COM_MODE, \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(        \
      (ETHSM_INV_NETWORK_IDX != NetIdx), \
      ETHSM_API_ID_GET_CURRENT_COM_MODE, \
      ETHSM_E_INVALID_NETWORK_HANDLE,    \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(        \
      (NULL_PTR != ModePtr),             \
      ETHSM_API_ID_GET_CURRENT_COM_MODE, \
      ETHSM_E_PARAM_POINTER,             \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  *ModePtr = EthSM_NetworkStateMngmt[NetIdx].Mode;

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

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
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentInternalMode(NetworkHandleType NetworkHandle,
                                                              ETHSM_P2VARAPPLDATA(EthSM_NetworkModeStateType) ModePtr)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByComMNetwHnd(NetworkHandle);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnValue(       \
      ETHSM_API_ID_GET_CURRENT_INTERNAL_MODE, \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(             \
      (ETHSM_INV_NETWORK_IDX != NetIdx),      \
      ETHSM_API_ID_GET_CURRENT_INTERNAL_MODE, \
      ETHSM_E_INVALID_NETWORK_HANDLE,         \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(             \
      (NULL_PTR != ModePtr),                  \
      ETHSM_API_ID_GET_CURRENT_INTERNAL_MODE, \
      ETHSM_E_PARAM_POINTER,                  \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  *ModePtr = EthSM_NetworkStateMngmt[NetIdx].State;

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

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
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_TrcvLinkStateChg(NetworkHandleType NetworkHandle,
                                                        EthTrcv_LinkStateType TransceiverLinkState)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByComMNetwHnd(NetworkHandle);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnValue( \
      ETHSM_API_ID_TRCV_LINK_STATE_CHG, \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(        \
      (ETHSM_INV_NETWORK_IDX != NetIdx), \
      ETHSM_API_ID_TRCV_LINK_STATE_CHG,  \
      ETHSM_E_INVALID_NETWORK_HANDLE,    \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(                                                                                 \
      ((ETHTRCV_LINK_STATE_DOWN == TransceiverLinkState) || (ETHTRCV_LINK_STATE_ACTIVE == TransceiverLinkState)), \
      ETHSM_API_ID_TRCV_LINK_STATE_CHG,                                                                           \
      ETHSM_E_INVALID_TRCV_LINK_STATE,                                                                            \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /*
   * Store Transceiver Link State according to SWS_EthSM_0114 (SREQ00025132).
   */
  EthSM_NetworkStateMngmt[NetIdx].TrcvLinkState = TransceiverLinkState;

  /*
   * Check for current communication mode to limit possible states of state machine.
   */
  if( COMM_NO_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
  {
    if(( ETHSM_STATE_WAIT_TRCVLINK == EthSM_NetworkStateMngmt[NetIdx].State )
          && ( ETHTRCV_LINK_STATE_ACTIVE == TransceiverLinkState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitTrcvLinkToWaitOnline;
    }
    else if(( ETHSM_STATE_WAIT_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
               && ( ETHTRCV_LINK_STATE_DOWN == TransceiverLinkState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToWaitTrvLink;
    }
    else
    {
#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
      /* Recognition of Out of Order Transceiver Link State Change */
      EthSM_IndCtrlBitfield_SetOooTrcvLinkStateChg(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_OOO_INDICATIONS */
    }
  }
  else if( COMM_FULL_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
  {
    if(( ETHSM_STATE_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
          && ( ETHTRCV_LINK_STATE_DOWN == TransceiverLinkState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnlineToOnhold;
#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
      /* There are two possible paths to leave state ONLINE. In case of out of order indications
       * the transition to WAIT_ONLINE would have been triggered by the TcpIp mode indication.
       * The occurence of this indication can be recognized by the OFFLINE state of the TcpIp
       * has indicated.
       */
      if( TCPIP_STATE_OFFLINE == EthSM_NetworkStateMngmt[NetIdx].TcpIpState )
      {
        EthSM_IndCtrlBitfield_SetOooTcpIpModeInd(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
      }
#endif /* ETHSM_ENABLE_OOO_INDICATIONS */
    }
    else if(( ETHSM_STATE_ONHOLD == EthSM_NetworkStateMngmt[NetIdx].State )
               && ( ETHTRCV_LINK_STATE_ACTIVE == TransceiverLinkState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToOnline;
    }
    else
    {
#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
      /* Recognition of Out of Order Transceiver Link State Change */
      EthSM_IndCtrlBitfield_SetOooTrcvLinkStateChg(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_OOO_INDICATIONS */
    }
  }
  else
  {
    /* nothing to do, only for MISRA compliance */
  }

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

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
FUNC(void, ETHSM_CODE) EthSM_VTrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByEthIfCtrlIdx(CtrlIdx);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorReturnVoid(         \
      (ETHSM_INV_NETWORK_IDX != NetIdx), \
      ETHSM_API_ID_TRCV_LINK_STATE_CHG,  \
      ETHSM_E_PARAM_CONTROLLER); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnVoid(                                                                    \
      ((ETHTRCV_LINK_STATE_DOWN == TrcvLinkState) || (ETHTRCV_LINK_STATE_ACTIVE == TrcvLinkState)), \
      ETHSM_API_ID_TRCV_LINK_STATE_CHG,                                                             \
      ETHSM_E_INVALID_TRCV_LINK_STATE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /*
   * Call AUTOSAR API.
   */
  (void) EthSM_TrcvLinkStateChg(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, TrcvLinkState);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_TcpIpModeIndication(NetworkHandleType NetworkHandle,
                                                           TcpIp_StateType TcpIpState)
{
  EthSM_NetworkIdxType NetIdx = EthSM_GetNetIdxByComMNetwHnd(NetworkHandle);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnValue(   \
      ETHSM_API_ID_TCPIP_MODE_INDICATION, \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(         \
      (ETHSM_INV_NETWORK_IDX != NetIdx),  \
      ETHSM_API_ID_TCPIP_MODE_INDICATION, \
      ETHSM_E_INVALID_NETWORK_HANDLE,     \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnValue(                                                                                        \
      ((TCPIP_STATE_ONLINE == TcpIpState) || (TCPIP_STATE_OFFLINE == TcpIpState) || (TCPIP_STATE_ONHOLD == TcpIpState)), \
      ETHSM_API_ID_TCPIP_MODE_INDICATION,                                                                                \
      ETHSM_E_INVALID_TcpIpMode,                                                                                         \
      E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /*
   * Store TcpIp state according to SWS_EthSM_0119 (SREQ00025081).
   */
  EthSM_NetworkStateMngmt[NetIdx].TcpIpState = TcpIpState;

   /*
   * Check for current communication mode to limit possible states of state machine.
   */
  if( COMM_NO_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
  {
    if(( ETHSM_STATE_WAIT_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
          && ( TCPIP_STATE_ONLINE == TcpIpState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToOnline;
    }
    else
    {
#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS)
      /* Recognition of Out of Order TcpIp Mode Indication */
      EthSM_IndCtrlBitfield_SetOooTcpIpModeInd(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif
    }
  }
  else if( COMM_FULL_COMMUNICATION == EthSM_NetworkStateMngmt[NetIdx].Mode )
  {
    if(( ETHSM_STATE_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
          && ( TCPIP_STATE_OFFLINE == TcpIpState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnlineToWaitOnline;
    }
    else if(( ETHSM_STATE_ONHOLD == EthSM_NetworkStateMngmt[NetIdx].State )
               && ( TCPIP_STATE_OFFLINE == TcpIpState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToWaitTrcvLink;
    }
    else if(( ETHSM_STATE_WAIT_OFFLINE == EthSM_NetworkStateMngmt[NetIdx].State )
               && ( TCPIP_STATE_OFFLINE == TcpIpState ))
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOfflineToOffline;
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_ProcessEthIfCallRetries(NetIdx);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
    }
    else
    {
#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
      /* Recognition of Out of Order TcpIp Mode Indication */
      EthSM_IndCtrlBitfield_SetOooTcpIpModeInd(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_OOO_INDICATIONS */
    }
  }
  else
  {
    /* nothing to do, only for MISRA compliance */
  }

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHSM_IS_TCPIP_ENABLED */

#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
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
FUNC(void, ETHSM_CODE) EthSM_VTcpIpModeIndication(uint8 CtrlIdx, boolean IsAnyAddrOnCtrlActive)
{
  EthSM_NetworkIdxType NetIdx          = EthSM_GetNetIdxByEthIfCtrlIdx(CtrlIdx);
  /* store previous activity state to recognize if a state machine transition must be processed */
  boolean IsIpActivityBeforeProcessing = EthSM_IsIpActiveOnNetwork(NetIdx);

  /*
   * Perform DET checks.
   */
  ETHSM_CheckDetErrorIsInitReturnVoid( \
      ETHSM_API_ID_TCPIP_MODE_INDICATION); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  ETHSM_CheckDetErrorReturnVoid(          \
      (ETHSM_INV_NETWORK_IDX != NetIdx),  \
      ETHSM_API_ID_TCPIP_MODE_INDICATION, \
      ETHSM_E_PARAM_CONTROLLER); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#if ( STD_ON == ETHSM_IS_MIXED_STACK )
  ETHSM_CheckDetErrorReturnVoid(                                             \
      (EthSM_VCfgGetNetworks()[NetIdx].StackType == ETHSM_STACK_TYPE_TCPIP), \
      ETHSM_API_ID_TCPIP_MODE_INDICATION,                                    \
      ETHSM_E_STACKTYPE_WRONG); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif /* ETHSM_IS_MIXED_STACK */
  
  /* adapt activity count depending on passed state */
  if( TRUE == IsAnyAddrOnCtrlActive )
  {
    EthSM_NetworkStateMngmt[NetIdx].ActiveIpVxCnt++;
  }
  else
  {
    EthSM_NetworkStateMngmt[NetIdx].ActiveIpVxCnt--;
  }

  /* only process a transition if a state change is needed */
  if( IsIpActivityBeforeProcessing != EthSM_IsIpActiveOnNetwork(NetIdx) )
  {
    TcpIp_StateType WrappedTcpIpState
        = (TcpIp_StateType)((TRUE == IsAnyAddrOnCtrlActive) ? TCPIP_STATE_ONLINE : TCPIP_STATE_OFFLINE);
    /*
     * Call AUTOSAR API.
     */
    (void) EthSM_TcpIpModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, WrappedTcpIpState);
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ETHSM_IS_VECTOR_TCPIP */

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
 *              #define ETHSM_ENABLE_CTRL_TRCV_IND_API STD_ON
 *********************************************************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
  /*
   * SWS_EthSM_00191 and SWS_EthSM_00192 are not implemented because purpose off API isn't clear and
   * configuration data doesn't fit to API. (Controllers and Transceivers are abstracted by the
   * EthIf Controller -> no knowledge of controllers beyond EthIf)
   */

  /* do nothing, AUTOSAR SWS doesn't even specify why this API exists and what its purpose is */
}

/**********************************************************************************************************************
 *  EthSM_TrcvModeIndication
 *********************************************************************************************************************/
/*! \brief      Purpose of this API not specified by AUTOSAR SWS.
 *  \param[in]  TrcvIdx        Transceiver Index
 *  \param[in]  TrcvMode       Transceiver Mode
 *  \context    Task level
 *  \note       The API is an empty implementation and not available by default.\n
 *              To enable the API use a User Config File and add\n
 *              #define ETHSM_ENABLE_CTRL_TRCV_IND_API STD_ON
 *********************************************************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_TrcvModeIndication(uint8 TrcvIdx, EthTrcv_ModeType TrcvMode)
{
  /*
   * SWS_EthSM_00194 and SWS_EthSM_00195 are not implemented because purpose off API isn't clear and
   * configuration data doesn't fit to API. (Controllers and Transceivers are abstracted by the
   * EthIf Controller -> no knowledge of transceivers beyond EthIf)
   */

  /* do nothing, AUTOSAR SWS doesn't even specify why this API exists and what its purpose is */
}
#  endif /* ETHSM_ENABLE_CTRL_TRCV_IND_API */
#endif /* ETHSM_ENABLE_CTRL_TRCV_IND_API */

#if ( ETHSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  EthSM_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Get Ethernet State Manager version information.
 *  \param[in]  VersionInfoPtr        Pointer to store the version info.
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_GetVersionInfo(ETHSM_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr)
{
  ETHSM_CheckDetErrorReturnVoid(     \
      (NULL_PTR != VersionInfoPtr),  \
      ETHSM_API_ID_GET_VERSION_INFO, \
      ETHSM_E_PARAM_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  VersionInfoPtr->vendorID         = ETHSM_VENDOR_ID;
  VersionInfoPtr->moduleID         = ETHSM_MODULE_ID;
  VersionInfoPtr->sw_major_version = ETHSM_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = ETHSM_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = ETHSM_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ETHSM_VERSION_INFO_API */

/**********************************************************************************************************************
 *  EthSM_MainFunction
 *********************************************************************************************************************/
/*! \brief      Main function of the Ethernet State Manager.
 *  \context    Task level
 *  \note       The main function processes the state transitions.
 *********************************************************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_MainFunction(void)
{
  if( EthSM_IsInitialized == TRUE )
  {
    EthSM_NetworkIdxType NetIdx = EthSM_VCfgGetNetworkTotal();

    while( (EthSM_NetworkIdxType)0U < NetIdx)
    {
      --NetIdx;

      /*
       * Handling of State transitions.
       */
      if( NULL_PTR != EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans )
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans(NetIdx);
      }

#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      /*
       * EthIf API call timeout handling.
       */
      if( TRUE == EthSM_IndCtrlBitfield_IsEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield) )
      {
        /*
         * Handle the timeout
         * not exceeded -> decrement and proceed further
         * exceeded     -> additional checks have to performed to investigate if error handling has to be processed
         */
        if( (EthSM_EthIfTimeoutType)0U < EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
        {
          --EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime;
        }
        else
        {
          /*
           * If timeout and retries exceeded error handling will be processed. The error handling depends on the configured
           * error handling capabilities (DET or/and DEM).
           * Unrelated to the error handling configuration the network will stay in the state the transition has been sent off.
           */
          if( (EthSM_EthIfRetriesType)0U < EthSM_NetworkStateMngmt[NetIdx].RetriesLeft )
          {
            /* Reset timeout and decrement retries to initiate next retry cycle. */
            EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime = EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout;
            --EthSM_NetworkStateMngmt[NetIdx].RetriesLeft;
          }
          else
          {
            /*
             * Stopping state transition, due to EthIf API call retry exceed.
             * EthSM network will stay in its state until the module is re-initialized.
             */
            EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#  if (STD_ON == ETHSM_DEV_ERROR_DETECT )
            Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, ETHSM_API_ID_MAIN_FUNCTION, ETHSM_E_ETHIF_TIMEOUT);
#  endif /* ETHSM_DEV_ERROR_DETECT */
#  if ( STD_ON == ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED )
            /* only report a DEM event if it is configured (DEM Event ID 0u is invalid) */
            if( DEM_EVENT_INVALID != EthSM_VCfgGetNetworks()[NetIdx].EthIfTimeoutDemEventId )
            {
              Dem_ReportErrorStatus(EthSM_VCfgGetNetworks()[NetIdx].EthIfTimeoutDemEventId, DEM_EVENT_STATUS_FAILED);
            }
#  endif /* ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED */
          }
        }
      }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */

#if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
      /* Handling of Out of Order Transceiver Link State Change and TcpIp Mode Indication.
       * (Workaround due to async behavior of Vector IP-Stack, IP assignment at the moment not triggered by EthSM but
       *  initiated by Transceiver Link State Change callback)
       */
      if( TRUE == EthSM_IndCtrlBitfield_IsOooTrcvLinkStateChgOccurred(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield) )
      {
        /*
         * Allow State change from WAIT_ONLINE to WAIT_TRCVLINK if a Transceiver Link State change with mode DOWN
         * occurred before the transition from ONLINE to WAIT_ONLINE was triggered by a TcpIp mode indication.
         */
        if(( ETHSM_STATE_WAIT_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
              && ( ETHTRCV_MODE_DOWN == EthSM_NetworkStateMngmt[NetIdx].TrcvLinkState ))
        {
          EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToWaitTrvLink;
          EthSM_IndCtrlBitfield_ClearOooTrcvLinkStateChg(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
        }
        else
        {
          /* nothing to do, only for MISRA compliance */
        }
      }
      else
      {
        /* nothing to do, only for MISRA compliance */
      }

      if( TRUE == EthSM_IndCtrlBitfield_IsOooTcpIpModeIndOccurred(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield) )
      {
        /*
         * Allow State change from WAIT_ONLINE to ONLINE if a TcpIp mode indication with mode ONLINE occurred before
         * the transition from WAIT_TRCVLINK to WAIT_ONLINE was triggered by a Transceiver Link State Change.
         */
        if(( ETHSM_STATE_WAIT_ONLINE == EthSM_NetworkStateMngmt[NetIdx].State )
              && ( TCPIP_STATE_ONLINE == EthSM_NetworkStateMngmt[NetIdx].TcpIpState ))
        {
          EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToOnline;
          EthSM_IndCtrlBitfield_ClearOooTcpIpModeInd(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
        }
        else if(( ETHSM_STATE_ONHOLD == EthSM_NetworkStateMngmt[NetIdx].State )
                   && ( TCPIP_STATE_OFFLINE == EthSM_NetworkStateMngmt[NetIdx].TcpIpState ))
        {
          EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToWaitTrcvLink;
          EthSM_IndCtrlBitfield_ClearOooTcpIpModeInd(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
        }
        else
        {
          /* nothing to do, only for MISRA compliance */
        }
      }
      else
      {
        /* nothing to do, only for MISRA compliance */
      }
#endif /* ETHSM_ENABLE_OOO_INDICATIONS */
    } /* for each network */

  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthSM_GetNetIdxByComMNetwHnd
 *********************************************************************************************************************/
/*! \brief      Derives the EthSM Network index from the ComM Channel Handle.
 *  \param[in]  ComM Channel Handle
 *  \return     EthSM Network index (ETHSM_INVALID_NETWORK_IDX if no Ethernet Network for passed ComM Channel Handle
 *              exists)
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(EthSM_NetworkIdxType, ETHSM_CODE) EthSM_GetNetIdxByComMNetwHnd(NetworkHandleType NetworkHandle)
{
  EthSM_NetworkIdxType RetVal = ETHSM_INV_NETWORK_IDX;
  EthSM_NetworkIdxType NetIdx = (EthSM_NetworkIdxType)EthSM_VCfgGetNetworkTotal();

  while( (EthSM_NetworkIdxType)0U < NetIdx )
  {
    --NetIdx;
    if( NetworkHandle == EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd )
    {
      RetVal = NetIdx;
      break;
    }
  }

  return RetVal;
}

#if (( STD_ON == ETHSM_IS_VECTOR_ETHIF ) || ( STD_ON == ETHSM_IS_VECTOR_TCPIP ) )
/**********************************************************************************************************************
 *  EthSM_GetNetIdxByVirtCtrlIdx
 *********************************************************************************************************************/
/*! \brief      Derives the EthSM Network index from the EthIf Controller Index.
 *  \param[in]  EthIf Controller Index
 *  \return     EthSM Network index (ETHSM_INVALID_NETWORK_IDX if no Ethernet Network for passed EthIf Controller
 *              Index exists)
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(EthSM_NetworkIdxType, ETHSM_CODE) EthSM_GetNetIdxByEthIfCtrlIdx(uint8 CtrlIdx)
{
  EthSM_NetworkIdxType RetVal = ETHSM_INV_NETWORK_IDX;
  EthSM_NetworkIdxType NetIdx = (EthSM_NetworkIdxType)EthSM_VCfgGetNetworkTotal();

  while( (EthSM_NetworkIdxType)0U < NetIdx )
  {
    --NetIdx;
    if( CtrlIdx == EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx )
    {
      RetVal = NetIdx;
      break;
    }
  }

  return RetVal;
}
#endif /* ETHSM_IS_VECTOR_ETHIF, ETHSM_IS_VECTOR_TCPIP */

/**********************************************************************************************************************
 *  EthSM_TransOfflineToWaitTrcvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state OFFLINE to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOfflineToWaitTrcvLink(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  if( EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
  {
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    Std_ReturnType IsModeSetSucceeded;
    uint8 CtrlIdx = EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx;

    IsModeSetSucceeded = EthIf_ControllerInit(CtrlIdx, ETHSM_CTRL_CFG_IDX);
    /* check if initialization was successful */
    if( E_OK == IsModeSetSucceeded )
    {
      IsModeSetSucceeded = EthIf_SetControllerMode(CtrlIdx, ETH_MODE_ACTIVE);
    }

    if( E_OK == IsModeSetSucceeded )
    {
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_DUMMY_MODE )
      /*
       * Handling of Transition to WAIT_ONLINE if Dummy mode is enabled.
       */
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitTrcvLinkToWaitOnline;
#else
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#endif /* ETHSM_DUMMY_MODE */
      EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_TRCVLINK;
#  if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
      BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_WAIT_TRCVLINK);
#  endif /* ETHSM_IS_BSWM_ENABLED */
#  if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_IndCtrlBitfield_ClearEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#  endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    }
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
}

/**********************************************************************************************************************
 *  EthSM_TransWaitTrcvLinkToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_TRCVLINK to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitTrcvLinkToOffline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  if( EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
  {
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    Std_ReturnType IsModeSetSucceeded;
    uint8 CtrlIdx = EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx;

    IsModeSetSucceeded = EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

    if( E_OK == IsModeSetSucceeded )
    {
#endif /* ETHSM_DUMMY_MODE */
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
      EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_OFFLINE;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
      BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_OFFLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_IndCtrlBitfield_ClearEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    }
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
}

/**********************************************************************************************************************
 *  EthSM_TransWaitTrcvLinkToWaitOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_TRCVLINK to WAIT_ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitTrcvLinkToWaitOnline(EthSM_NetworkIdxType NetIdx)
{
#if( STD_ON == ETHSM_IS_TCPIP_ENABLED)
  Std_ReturnType IsModeSetSucceeded;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  IsModeSetSucceeded = E_OK;

  if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
  {
#  endif /* ETHSM_IS_MIXED_STACK */
    IsModeSetSucceeded = TcpIp_RequestComMode(EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx, TCPIP_STATE_ONLINE);
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  }
#  endif /* ETHSM_IS_MIXED_STACK */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  if( E_OK == IsModeSetSucceeded )
  {
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
    /*
     * In a mixed stack containing channels on which either TcpIp or non TcpIp (e.g. AVB)
     * communication is performed the state transitions must be redirected to be able to
     * omit a TcpIp Mode Request on non TcpIp channels.
     */
    if( ETHSM_STACK_TYPE_AVB == EthSM_VCfgGetNetworks()[NetIdx].StackType )
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToOnline;
    }
#  endif /* ETHSM_IS_MIXED_STACK */
#else
    /*
     * Handling of Transition to ONLINE if no TcpIp is contained in the system.
     */
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToOnline;
#endif /* ETHSM_IS_TCPIP_ENABLED */
    EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_ONLINE;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
    BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_WAIT_ONLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  }
#endif /* ETHSM_IS_TCPIP_ENABLED */
}

#if ( STD_OFF == ETHSM_DUMMY_MODE )
/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToWaitTrvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToWaitTrvLink(EthSM_NetworkIdxType NetIdx)
{
#if( STD_ON == ETHSM_IS_TCPIP_ENABLED)
  Std_ReturnType IsModeSetSucceeded;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  IsModeSetSucceeded = E_OK;

  if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
  {
#  endif /* ETHSM_IS_MIXED_STACK */
    IsModeSetSucceeded = TcpIp_RequestComMode(EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx, TCPIP_STATE_OFFLINE);
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  }
#  endif /* ETHSM_IS_MIXED_STACK */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  if( E_OK == IsModeSetSucceeded )
  {
#endif /* ETHSM_IS_TCPIP_ENABLED */
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
    EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_TRCVLINK;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
    BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_WAIT_TRCVLINK);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  }
#endif /* ETHSM_IS_TCPIP_ENABLED */
}
#endif /* ETHSM_DUMMY_MODE */

/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToOffline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  if( EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
  {
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if(( STD_ON == ETHSM_IS_TCPIP_ENABLED ) || ( STD_OFF == ETHSM_DUMMY_MODE ))
    Std_ReturnType IsModeSetSucceeded = E_OK;
    uint8 CtrlIdx = EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx;

#  if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#    if ( STD_ON == ETHSM_IS_MIXED_STACK )
    if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
    {
#    endif /* ETHSM_IS_MIXED_STACK */
      IsModeSetSucceeded |= TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);
#    if ( STD_ON == ETHSM_IS_MIXED_STACK )
    }
#    endif /* ETHSM_IS_MIXED_STACK */
#  endif /* ETHSM_IS_TCPIP_ENABLED */
#  if ( STD_OFF == ETHSM_DUMMY_MODE )
    IsModeSetSucceeded |= EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);
#  endif /* ETHSM_DUMMY_MODE */

    if( E_OK == IsModeSetSucceeded )
    {
#endif /* ETHSM_IS_TCPIP_ENABLED, ETHSM_DUMMY_MODE */
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
      EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_OFFLINE;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
      BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_OFFLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_IndCtrlBitfield_ClearEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if(( STD_ON == ETHSM_IS_TCPIP_ENABLED ) || ( STD_OFF == ETHSM_DUMMY_MODE ))
    }
#endif /* ETHSM_IS_TCPIP_ENABLED, ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
}

/**********************************************************************************************************************
 *  EthSM_TransWaitOnlineToOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_ONLINE to ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOnlineToOnline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  ComM_ModeType ModeToPass = COMM_FULL_COMMUNICATION;
#endif /* ETHSM_IS_COMM_ENABLED */

  EthSM_NetworkStateMngmt[NetIdx].Mode          = COMM_FULL_COMMUNICATION;
  EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
  EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_ONLINE;

#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  ComM_BusSM_ModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, &ModeToPass);
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
  BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_ONLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
}

#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
/**********************************************************************************************************************
 *  EthSM_TransOnlineToWaitOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to WAIT_ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToWaitOnline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  /*
   * Local variable used due to encapsulation of EthSM internal states to ComM.
   * ComM shouldn't be able to change internal states.
   */
  ComM_ModeType ModeToPass = COMM_NO_COMMUNICATION;
#endif /* ETHSM_IS_COMM_ENABLED */

  EthSM_NetworkStateMngmt[NetIdx].Mode          = COMM_NO_COMMUNICATION;
#if ( STD_ON == ETHSM_DUMMY_MODE )
 /*
  * Handling of Transition to WAIT_TRCVLINK if Dummy Mode is enabled.
  */
  EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOnlineToWaitTrvLink;
#else
  EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#endif /* ETHSM_DUMMY_MODE */
  EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_ONLINE;
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  ComM_BusSM_ModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, &ModeToPass);
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
  BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_ONLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
}
#endif /* ETHSM_IS_TCPIP_ENABLED */

#if ( STD_OFF == ETHSM_DUMMY_MODE )
/**********************************************************************************************************************
 *  EthSM_TransOnlineToOnhold
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to ONHOLD.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToOnhold(EthSM_NetworkIdxType NetIdx)
{
#if( STD_ON == ETHSM_IS_TCPIP_ENABLED)
  Std_ReturnType IsModeSetSucceeded;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  IsModeSetSucceeded = E_OK;

  if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
  {
#  endif /* ETHSM_IS_MIXED_STACK */
    IsModeSetSucceeded = TcpIp_RequestComMode(EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx, TCPIP_STATE_ONHOLD);
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  }
#  endif /* ETHSM_IS_MIXED_STACK */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  if( E_OK == IsModeSetSucceeded )
  {
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
      /*
       * In a mixed stack containing channels on which either TcpIp or non TcpIp (e.g. AVB)
       * communication is performed the state transitions must be redirected to be able to
       * omit a TcpIp Mode Request on non TcpIp channels.
       */
      if( ETHSM_STACK_TYPE_AVB == EthSM_VCfgGetNetworks()[NetIdx].StackType )
      {
        EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToWaitTrcvLink;
      }
#  endif /* ETHSM_IS_MIXED_STACK */
#else
    /*
     * Handling of Transition to WAIT_TRCVLINK if no TcpIp is contained in the system.
     */
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransOnholdToWaitTrcvLink;
#endif /* ETHSM_IS_TCPIP_ENABLED */
    EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_ONHOLD;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
    BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_ONHOLD);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_IS_LINK_DOWN_DEM_ENABLED )
    /* only report a DEM event if it is configured (DEM Event ID 0u is invalid) */
    if( DEM_EVENT_INVALID != EthSM_VCfgGetNetworks()[NetIdx].LinkDownDemEventId )
    {
      Dem_ReportErrorStatus(EthSM_VCfgGetNetworks()[NetIdx].LinkDownDemEventId, DEM_EVENT_STATUS_FAILED);
    }
#endif /* ETHSM_IS_LINK_DOWN_DEM_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  }
#endif /* ETHSM_IS_TCPIP_ENABLED */
}
#endif /* ETHSM_DUMMY_MODE */

/**********************************************************************************************************************
 *  EthSM_TransOnlineToWaitOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONLINE to WAIT_OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnlineToWaitOffline(EthSM_NetworkIdxType NetIdx)
{
#if( STD_ON == ETHSM_IS_TCPIP_ENABLED)
  Std_ReturnType IsModeSetSucceeded;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  IsModeSetSucceeded = E_OK;

  if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
  {
#  endif /* ETHSM_IS_MIXED_STACK */
    IsModeSetSucceeded = TcpIp_RequestComMode(EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx, TCPIP_STATE_OFFLINE);
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  }
#  endif /* ETHSM_IS_MIXED_STACK */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  if( E_OK == IsModeSetSucceeded )
  {
#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOfflineToOffline;
#else
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
#endif /* ETHSM_IS_VECTOR_TCPIP */
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
    /*
     * In a mixed stack containing channels on which either TcpIp or non TcpIp (e.g. AVB)
     * communication is performed the state transitions must be redirected to be able to
     * omit a TcpIp Mode Request on non TcpIp channels.
     */
    if( ETHSM_STACK_TYPE_AVB == EthSM_VCfgGetNetworks()[NetIdx].StackType )
    {
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOfflineToOffline;
    }
#  endif /* ETHSM_IS_MIXED_STACK */
#else
    /*
     * Handling of Transition to OFFLINE if no TcpIp is contained in the system.
     */
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = EthSM_TransWaitOfflineToOffline;
#endif /* ETHSM_IS_TCPIP_ENABLED */
    EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_OFFLINE;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
    BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_WAIT_OFFLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  }
#endif /* ETHSM_IS_TCPIP_ENABLED */
}

#if ( STD_OFF == ETHSM_DUMMY_MODE)
/**********************************************************************************************************************
 *  EthSM_TransOnholdToOnline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to ONLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToOnline(EthSM_NetworkIdxType NetIdx)
{
#if( STD_ON == ETHSM_IS_TCPIP_ENABLED)
  Std_ReturnType IsModeSetSucceeded;
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  IsModeSetSucceeded = E_OK;

  if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
  {
#  endif /* ETHSM_IS_MIXED_STACK */
    IsModeSetSucceeded = TcpIp_RequestComMode(EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx, TCPIP_STATE_ONLINE);
#  if ( STD_ON == ETHSM_IS_MIXED_STACK )
  }
#  endif /* ETHSM_IS_MIXED_STACK */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  if( E_OK == IsModeSetSucceeded )
  {
#endif /* ETHSM_IS_TCPIP_ENABLED */
    EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
    EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_ONLINE;
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
    BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_ONLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_IS_LINK_DOWN_DEM_ENABLED )
    /* only report a DEM event if it is configured (DEM Event ID 0u is invalid) */
    if( DEM_EVENT_INVALID != EthSM_VCfgGetNetworks()[NetIdx].LinkDownDemEventId )
    {
      Dem_ReportErrorStatus(EthSM_VCfgGetNetworks()[NetIdx].LinkDownDemEventId, DEM_EVENT_STATUS_PASSED);
    }
#endif /* ETHSM_IS_LINK_DOWN_DEM_ENABLED */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  }
#endif /* ETHSM_IS_TCPIP_ENABLED */
}
#endif /* ETHSM_DUMMY_MODE */

#if (( ( STD_OFF == ETHSM_DUMMY_MODE ) && ( STD_OFF == ETHSM_IS_TCPIP_ENABLED ) ) \
         || ( STD_ON == ETHSM_IS_TCPIP_ENABLED ))
/**********************************************************************************************************************
 *  EthSM_TransOnholdToWaitTrcvLink
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to WAIT_TRCVLINK.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToWaitTrcvLink(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  /*
   * Local variable used due to encapsulation of EthSM internal states to ComM.
   * ComM shouldn't be able to change internal states.
   */
  ComM_ModeType ModeToPass = COMM_NO_COMMUNICATION;
#endif /* ETHSM_IS_COMM_ENABLED */

  EthSM_NetworkStateMngmt[NetIdx].Mode          = COMM_NO_COMMUNICATION;
  EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
  EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_WAIT_TRCVLINK;

#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
  ComM_BusSM_ModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, &ModeToPass);
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
  BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_WAIT_TRCVLINK);
#endif /* ETHSM_IS_BSWM_ENABLED */
}
#endif /* ETHSM_DUMMY_MODE, ETHSM_IS_TCPIP_ENABLED */

/**********************************************************************************************************************
 *  EthSM_TransOnholdToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state ONHOLD to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransOnholdToOffline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  if( EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
  {
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
    /*
     * Local variable used due to encapsulation of EthSM internal states to ComM.
     * ComM shouldn't be able to change internal states.
     */
    ComM_ModeType ModeToPass = COMM_NO_COMMUNICATION;
#endif /* ETHSM_IS_COMM_ENABLED */
#if (( STD_ON == ETHSM_IS_TCPIP_ENABLED ) || ( STD_OFF == ETHSM_DUMMY_MODE ))
    Std_ReturnType IsModeSetSucceeded = E_OK;
    uint8 CtrlIdx = EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx;

#  if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#    if ( STD_ON == ETHSM_IS_MIXED_STACK )
    if( ETHSM_STACK_TYPE_TCPIP == EthSM_VCfgGetNetworks()[NetIdx].StackType )
    {
#    endif /* ETHSM_IS_MIXED_STACK */
      IsModeSetSucceeded |= TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);
#    if ( STD_ON == ETHSM_IS_MIXED_STACK )
    }
#    endif /* ETHSM_IS_MIXED_STACK */
#  endif /* ETHSM_IS_TCPIP_ENABLED */
#  if ( STD_OFF == ETHSM_DUMMY_MODE )
    IsModeSetSucceeded |= EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);
#  endif /* ETHSM_DUMMY_MODE */

    if( E_OK == IsModeSetSucceeded )
    {
#endif /* ETHSM_IS_TCPIP_ENABLED, ETHSM_DUMMY_MODE */
      EthSM_NetworkStateMngmt[NetIdx].Mode          = COMM_NO_COMMUNICATION;
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
      EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_OFFLINE;

#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
      ComM_BusSM_ModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, &ModeToPass);
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
      BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_OFFLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_IndCtrlBitfield_ClearEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if (( STD_ON == ETHSM_IS_TCPIP_ENABLED ) || ( STD_OFF == ETHSM_DUMMY_MODE ))
    }
#endif /* ETHSM_IS_TCPIP_ENABLED, ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
}

/**********************************************************************************************************************
 *  EthSM_TransWaitOfflineToOffline
 *********************************************************************************************************************/
/*! \brief      Handle Transition from state WAIT_OFFLINE to OFFLINE.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC FUNC(void, ETHSM_CODE) EthSM_TransWaitOfflineToOffline(EthSM_NetworkIdxType NetIdx)
{
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  if( EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime )
  {
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
    /*
     * Local variable used due to encapsulation of EthSM internal states to ComM.
     * ComM shouldn't be able to change internal states.
     */
    ComM_ModeType ModeToPass = COMM_NO_COMMUNICATION;
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    Std_ReturnType IsModeSetSucceeded;
    uint8 CtrlIdx = EthSM_VCfgGetNetworks()[NetIdx].EthIfCtrlIdx;

    IsModeSetSucceeded = EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

    if( E_OK == IsModeSetSucceeded )
    {
#endif /* ETHSM_DUMMY_MODE */
      EthSM_NetworkStateMngmt[NetIdx].Mode          = COMM_NO_COMMUNICATION;
      EthSM_NetworkStateMngmt[NetIdx].ExeStateTrans = NULL_PTR;
      EthSM_NetworkStateMngmt[NetIdx].State         = ETHSM_STATE_OFFLINE;

#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
      ComM_BusSM_ModeIndication(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, &ModeToPass);
#endif /* ETHSM_IS_COMM_ENABLED */
#if ( STD_ON == ETHSM_IS_BSWM_ENABLED )
      BswM_EthSM_CurrentState(EthSM_VCfgGetNetworks()[NetIdx].ComMNetwHnd, ETHSM_BSWM_OFFLINE);
#endif /* ETHSM_IS_BSWM_ENABLED */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
      EthSM_IndCtrlBitfield_ClearEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
    }
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  }
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
}

#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
/**********************************************************************************************************************
 *  EthSM_ProcessEthIfCallRetries
 *********************************************************************************************************************/
/*! \brief      Process the EthIf call retry handling.
 *  \param[in]  EthSM Network index
 *  \return     void
 *  \context    task level
 *  \note       local function
 *********************************************************************************************************************/
STATIC INLINE FUNC(void, ETHSM_CODE) EthSM_ProcessEthIfCallRetries(EthSM_NetworkIdxType NetIdx)
{
  /*
   * Allow re-triggering of mode change if EthIf API call retries exceeded.
   */
  if(( TRUE == EthSM_IndCtrlBitfield_IsEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield) )
        && ( (EthSM_EthIfRetriesType)0U == EthSM_NetworkStateMngmt[NetIdx].RetriesLeft )
        && ( (EthSM_EthIfTimeoutType)0U == EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime ))
  {
    EthSM_NetworkStateMngmt[NetIdx].RetriesLeft    = EthSM_VCfgGetNetworks()[NetIdx].MaxRetries;
    EthSM_NetworkStateMngmt[NetIdx].RemModeChgTime = EthSM_VCfgGetNetworks()[NetIdx].ModeChgTimeout;
  }
  /*
   * Set control bit for the EthIf API call timeout handling.
   */
  EthSM_IndCtrlBitfield_SetEthIfCallPending(EthSM_NetworkStateMngmt[NetIdx].IndCtrlBitfield);
}
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */

#define ETHSM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  END OF FILE: EthSM.c
 *********************************************************************************************************************/
