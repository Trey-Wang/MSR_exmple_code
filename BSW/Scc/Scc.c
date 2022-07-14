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
/**        \file  Scc.c
 *        \brief  Smart Charge Communication Source Code File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2,
 *                DIN SPEC 70121 and customer specific schemas.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/ 
#define SCC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc.h"
#include "Scc_Priv.h"
#include "Scc_Cbk.h"
#include "Scc_Cfg.h"
#include "Scc_Lcfg.h"
#include "Scc_Exi.h"
#include SCC_IF_CFG_H /* PRQA S 0832 */ /* MD_Scc_0832 */
#include SCC_CP_CFG_H /* PRQA S 0832 */ /* MD_Scc_0832 */

#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
#include "EthIf_Types.h"
#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
#include "EthTrcv_30_Ar7000.h"
#endif /* SCC_ENABLE_SLAC_HANDLING */
#include "IpBase.h"
#include "TcpIp_IpV6.h"
#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
#include "NvM.h"
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */
#include "TcpIp.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (SCC_SW_MAJOR_VERSION != 9u) || (SCC_SW_MINOR_VERSION != 6u) || (SCC_SW_PATCH_VERSION != 1u) )
  #error "Scc.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  MISRA & PClint
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */
/*lint -e572 Conversion to network byte order. */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Read 3 bytes out of a buffer into a uint32 variable. Implementation is similar to IPBASE_GET_UINT32() and TLS_GET_UINT24. */
#define SCC_GET_UINT24(Buffer, Offset, Variable) ( (Variable) = (uint32)(((uint32)((Buffer)[(Offset)+0]) << 16) | \
                                                                         ((uint32)((Buffer)[(Offset)+1]) <<  8) | \
                                                                         ((uint32)((Buffer)[(Offset)+2]))) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define SCC_V2GTP_HDR_PAYLOAD_LEN_OFFSET 4U
#define SCC_SDP_SERVER_PORT              15118U
#define SCC_SDP_TP_TCP                    0x00U
#define SCC_SDP_REQ_LEN                     10U
#define SCC_SDP_RES_LEN                     28U
#define SCC_SDP_MULTCAST_IP_ADDR         { 0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3458 MACROS_BRACES */ /* MD_MSR_19.4 */
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */

#define Scc_MsgStateFct(MsgStateNew) \
  { Scc_MsgState = (MsgStateNew); Scc_Set_Core_MsgState(MsgStateNew); }

/* PRQA L:MACROS_BRACES */
/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/

/* 8bit variables - no init */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARNOINIT(boolean) Scc_CyclicMsgTrig;
STATIC SCC_VARNOINIT(uint16)  Scc_SdpRetransRetriesCnt;
STATIC SCC_VARNOINIT(uint8)   Scc_ExiStreamRxPBufIdx;
STATIC SCC_VARNOINIT(boolean) Scc_V2GSocketInUse;

SCC_VARNOINIT(boolean) Scc_TxStreamingActive;

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 16bit variables */
#define SCC_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_VARNOINIT(uint16) Scc_TimeoutCnt;

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
STATIC SCC_VARNOINIT(uint16) Scc_QCAIdleTimer;
#endif /* SCC_ENABLE_SLAC_HANDLING */

#define SCC_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 32bit variables */
#define SCC_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARNOINIT(uint32) Scc_TxdDataConfirmed;

SCC_VARNOINIT(uint32) Scc_TxDataSent;

#define SCC_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARNOINIT(Scc_MsgTrigType)  Scc_MsgTrigNew;
STATIC SCC_VARNOINIT(Scc_MsgStateType) Scc_MsgState;

SCC_VARNOINIT(Scc_SAPSchemaIDType) Scc_SAPSchemaID;
SCC_VARNOINIT(Scc_MsgTrigType)     Scc_MsgTrig;
SCC_VARNOINIT(Scc_MsgStatusType)   Scc_MsgStatus;
SCC_VARNOINIT(Scc_StackErrorType)  Scc_StackError;
SCC_VARNOINIT(Scc_SDPSecurityType) Scc_SelectedConnectionType;

SCC_VARNOINIT(Scc_PbufType)   Scc_ExiStreamRxPBuf[SCC_EXI_RX_STREAM_PBUF_ELEMENTS];
SCC_P2VARNOINIT(Scc_PbufType) Scc_ExiStreamTxPBufPtr;

SCC_VARNOINIT(Scc_SockAddrIn6Type) Scc_ServerSockAddr;

SCC_VARNOINIT(TcpIp_SocketIdType) Scc_SDPSocket;
SCC_VARNOINIT(TcpIp_SocketIdType) Scc_V2GSocket;

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - zero init */
#define SCC_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARZEROINIT(uint8) Scc_IPAddressCnt = 0;

SCC_VARZEROINIT(Scc_StateType) Scc_State = Scc_State_Uninitialized;

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
STATIC SCC_VARZEROINIT(Scc_FirmwareDownloadStatusType) Scc_QCAFirmwareDownloadComplete = Scc_FirmwareDownloadStatus_Unknown;
STATIC SCC_VARZEROINIT(EthTrcv_LinkStateType)          Scc_DLinkReady = (EthTrcv_LinkStateType)0u;
#endif /* SCC_ENABLE_SLAC_HANDLING */

#define SCC_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - no init - nvm */
#define SCC_START_SEC_VAR_NOINIT_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_VARNOINIT(uint8)   Scc_SessionIDNvm[SCC_SESSION_ID_NVM_BLOCK_LEN];

#define SCC_STOP_SEC_VAR_NOINIT_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - const - nvm */
#define SCC_START_SEC_CONST_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
SCC_CONSTCONST(uint8) Scc_SessionIDNvmRomDefault[SCC_SESSION_ID_NVM_BLOCK_LEN] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

#define SCC_STOP_SEC_CONST_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_STATICFUNCCODE(void) Scc_Reset(void);
#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
SCC_STATICFUNCCODE(void) Scc_TriggerSLAC(void);
#endif /* SCC_ENABLE_SLAC_HANDLING */
SCC_STATICFUNCCODE(void) Scc_TriggerV2G(void);
SCC_STATICFUNCCODE(void) Scc_TimeoutHandling(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_TxSECCDiscoveryProtocolReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_RxSECCDiscoveryProtocolRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_CheckV2GHeader(Scc_SocketType Socket);
SCC_STATICFUNCCODE(void) Scc_EstablishTLConnection(void);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Scc_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
                The versions are BCD-coded.
 *  \param[out] VersionInfoPtr        pointer for version info
 *  \context    initialization or task level
 *********************************************************************************************************************/
#if ( defined SCC_VERSION_INFO_API ) && ( SCC_VERSION_INFO_API == STD_ON )
SCC_FUNCCODE(void) Scc_GetVersionInfo(SCC_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr)
{
  /* check the DETs */ /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnVoid(Scc_State_Uninitialized == Scc_State, SCC_API_ID_GET_VERSION_INFO, SCC_DET_NOT_INITIALIZED)
  Scc_CheckDetErrorReturnVoid((NULL_PTR == VersionInfoPtr), SCC_API_ID_GET_VERSION_INFO, SCC_DET_INV_POINTER)

  VersionInfoPtr->vendorID   = SCC_VENDOR_ID;
  VersionInfoPtr->moduleID   = SCC_MODULE_ID;
  VersionInfoPtr->sw_major_version = SCC_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = SCC_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = SCC_SW_PATCH_VERSION;
}
#endif /* SCC_VERSION_INFO_API */

/**********************************************************************************************************************
 *  Scc_InitMemory
 *********************************************************************************************************************/
/*! \brief      initializes global variables
 *  \pre        has to be called before any other calls to the module
 *  \context    initialization
 *  \note       AUTOSAR extension
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_InitMemory(void)
{
  Scc_State = Scc_State_Uninitialized;
  Scc_IPAddressCnt = 0;

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  Scc_QCAFirmwareDownloadComplete = Scc_FirmwareDownloadStatus_Unknown;
  Scc_DLinkReady                  = FALSE;
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_OEM_VECTOR )
#if ( defined SCC_ENABLE_STATE_MACHINE ) && ( SCC_ENABLE_STATE_MACHINE == STD_ON )
  Scc_StateM_InitMemory();
#endif /* SCC_ENABLE_STATE_MACHINE */
#endif
}

/**********************************************************************************************************************
 *  Scc_Init
 *********************************************************************************************************************/
/*! \brief      stores the start address of the post build time configuration of the module
 *              and may be used to initialize the data structures
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \pre        has to be called before useage of the module
 *  \context    initialization
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Init(SCC_P2CONSTCFGROOT(Scc_ConfigType) CfgPtr)
{
  /* check the DETs */ /* PRQA S 3109 3 */ /* MD_MSR_14.3 */

  SCC_DUMMY_STATEMENT(CfgPtr); /* PRQA S 3112,3199 */ /* MD_MSR_14.2,MD_Scc_3198_3199 */

  Scc_V2GSocketInUse = FALSE;
  Scc_SDPSocket      = (Scc_SocketType)0xFFu;
  Scc_V2GSocket      = (Scc_SocketType)0xFFu;

  /* initialize the core parameters */
  Scc_Reset();

  Scc_Priv_Init();
  Scc_Exi_Init();
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  Scc_ExiTx_ISO_Init();
  Scc_ExiRx_ISO_Init();
#endif /* SCC_SCHEMA_ISO */
#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
  Scc_ExiTx_ISO_Ed2_CD_Init();
  Scc_ExiRx_ISO_Ed2_CD_Init();
#endif /* SCC_SCHEMA_ISO_ED2_CD */
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  Scc_ExiTx_DIN_Init();
  Scc_ExiRx_DIN_Init();
#endif /* SCC_SCHEMA_DIN */
#if ( defined SCC_OEM_VECTOR )
#if ( defined SCC_ENABLE_STATE_MACHINE ) && ( SCC_ENABLE_STATE_MACHINE == STD_ON )
  Scc_StateM_Init();
#endif /* SCC_ENABLE_STATE_MACHINE */
#endif
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_MainFunction
 *********************************************************************************************************************/
/*! \brief      main function of this module, has to be called cyclically
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_MainFunction(void)
{
  if ( Scc_State_Uninitialized == Scc_State )
  {
    return;
  }

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  /* check if the QCA idle timer is active */
  if ( 0 != Scc_QCAIdleTimer )
  {
    /* decrement the idle timer */
    Scc_QCAIdleTimer--;
  }
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_OEM_VECTOR )
#if ( defined SCC_ENABLE_STATE_MACHINE ) && ( SCC_ENABLE_STATE_MACHINE == STD_ON )
  Scc_StateM_MainFunction();
#endif
#endif

  /* get the current trigger */
  Scc_Get_Core_MsgTrig(&Scc_MsgTrigNew);
  Scc_Get_Core_CyclicMsgTrigRx(&Scc_CyclicMsgTrig);

  switch ( Scc_MsgTrigNew )
  {
    /* check if the SCC should be reset */
  case Scc_MsgTrig_None:
    /* check if this trigger is new */
    if (   ( Scc_MsgTrig != Scc_MsgTrigNew )
        || ( TRUE == Scc_CyclicMsgTrig ))
    {
      /* accept the new trigger */
      Scc_MsgTrig = Scc_MsgTrigNew;
      /* reset the core module */
      Scc_Reset();
      /* reset the cyclic message trigger at the application */
      Scc_Set_Core_CyclicMsgTrigTx(FALSE);
    }
    break;

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  case Scc_MsgTrig_SLAC:
    Scc_TriggerSLAC();
    break;
#endif /* SCC_ENABLE_SLAC_HANDLING */

    /* other triggers */
  default:
    /* only check V2G message triggers if an IP address is assigned */
    if (   ( Scc_State_Initialized <= Scc_State )
        && ( 0 < Scc_IPAddressCnt ))
    {
      switch ( Scc_MsgState )
      {
        /* check if a response was received, which needs to be decoded */
      case Scc_MsgState_ResponseReceived:
        /* check if this should be a SECCDiscoveryProtocolRes */
        if ( Scc_State_Initialized == Scc_State )
        {
          /* decode and process the SECCDiscoveryProtocolRes */
          if ( E_OK == Scc_RxSECCDiscoveryProtocolRes() )
          {
            /* SDPRes was successfully received, UDP socket no longer necessary */
            (void) TcpIp_Close(Scc_SDPSocket, FALSE);
            Scc_SDPSocket = (Scc_SocketType)0xFFu;

            Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)
          }
          /* received SECCDiscoveryProtocolRes was not valid, wait for the next one */
          else
          {
            /* go back to the transmission confirmed state */
            Scc_MsgStateFct(Scc_MsgState_RequestSent)
          }
        }
        /* check if this should be a SupportedAppProtocolRes */
        else if ( Scc_State_TLConnected == Scc_State )
        {
          /* decode and process the SupportedAppProtocolRes */
          (void) Scc_Exi_DecodeSupportedAppProtocolRes();

          Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)

          /* make the received message available to the application */
          {
            /* create and set the rx buffer pointer */
            Scc_TxRxBufferPointerType V2GResponse;
            V2GResponse.PbufPtr = &Scc_ExiStreamRxPBuf[0];
            V2GResponse.FirstPart = TRUE;
            V2GResponse.StreamComplete = TRUE;
            /* provide the buffer to the application */
            Scc_Set_Core_V2GResponse(&V2GResponse);
          }

          /* free the buffer */
          (void) Tl_Received(Scc_V2GSocket, Scc_ExiStreamRxPBuf[0].totLen);
        }
        /* if this should be a V2G message */
        else if (   ( Scc_State_SAPComplete == Scc_State )
                 || ( Scc_State_Connected == Scc_State ))
        {
          /* decode and process the received message */
          (void) (*Scc_ExiRx_Xyz_DecodeMessageFctPtr)();

          Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)

          /* make the received message available to the application */
          {
            /* create and set the rx buffer pointer */
            Scc_TxRxBufferPointerType V2GResponse;
            V2GResponse.PbufPtr = &Scc_ExiStreamRxPBuf[0];
            V2GResponse.FirstPart = TRUE;
            V2GResponse.StreamComplete = TRUE;
            /* provide the buffer to the application */
            Scc_Set_Core_V2GResponse(&V2GResponse);
          }

          /* free the buffer */
          (void) Tl_Received(Scc_V2GSocket, Scc_ExiStreamRxPBuf[0].totLen);
        }
        /* received message can be ignored */
        else
        {
          /* check if data was received on the TCP connection */
          if ( 0 != Scc_ExiStreamRxPBuf[0].totLen )
          {
            /* free the buffer */
            (void) Tl_Received(Scc_V2GSocket, Scc_ExiStreamRxPBuf[0].totLen);
          }
        }
        /* the whole message was processed, thus reset the pbuf */
        Scc_ExiStreamRxPBuf[0].totLen = 0;
        Scc_ExiStreamRxPBuf[0].len = 0;
        Scc_ExiStreamRxPBufIdx = 0;
        break;

        /* check if a request was sent and the response is still pending */
      case Scc_MsgState_StreamingRequest:
        if ( Scc_ReturnType_OK == Scc_Exi_StreamRequest() )
        {
            Scc_MsgStateFct(Scc_MsgState_RequestSent)
        }
        break;

        /* if a request shall be sent */
      case Scc_MsgState_WaitForNextRequest:
        Scc_TriggerV2G();
        break;

        /* check if a request was sent and the response is still pending */
      default:
        Scc_TimeoutHandling();
        break;
      }
    }
    break;
  }
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_ResetSessionID
 *********************************************************************************************************************/
/*! \brief      resets the SessionID to '0x00'
 *  \pre        will only have effect outside of a V2G session
 *  \return     E_OK            SessionID has been reset
 *  \return     E_NOT_OK        a V2G session is currently active, SessionID was not reset
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_ResetSessionID(boolean ForceReset)
{
  Scc_MsgTrigType MsgTrig;
  boolean SccResetSessionID = FALSE;

  /* check if the session id shall not be overwritten during an active session */
  if ( FALSE == ForceReset )
  {
    /* get the current message trigger from the application */
    Scc_Get_Core_MsgTrig(&MsgTrig);
    /* check if a session is active */
    if (   ( Scc_MsgTrig_SessionSetup > MsgTrig )
        || ( Scc_MsgTrig_SessionStop < MsgTrig ))
    {
      SccResetSessionID = TRUE;
    }
  }

  /* check if there is no V2G session active */
  if (   ( TRUE == SccResetSessionID )
      || ( TRUE == ForceReset ))
  {
    /* reset the SessionID */
    Scc_SessionIDNvm[1] = 0x00;
    /* set the length of the SessionID */
    Scc_SessionIDNvm[0] = 0x01;
#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
    /* set the flag for the NvM to copy the new SessionID */
    (void) NvM_SetRamBlockStatus(SCC_SESSION_ID_NVM_BLOCK, TRUE);
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

    return (Std_ReturnType)E_OK;
  }
  /* if a V2G session is active */
  else
  {
    /* operation not allowed */
    return (Std_ReturnType)E_NOT_OK;
  }
}

/**********************************************************************************************************************
 *  GLOBAL CALLBACKS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_Cbk_TL_RxIndication
 *********************************************************************************************************************/
/*! \brief      receive indication
 *  \param[in]  SockHnd               socket handle
 *  \param[in]  PbufPtr/DataPtr       pointer to the received data
 *  \param[in]  SourcePtr             source network address and port
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_TL_RxIndication(
  Scc_SocketType SockHnd,
  SCC_P2VARAPPLDATA(Scc_SockAddrType) SourcePtr, /* PRQA S 3673 */ /* MD_SCC_16.7_3673 */
  SCC_P2VARAPPLDATA(uint8) DataPtr,
  uint16 DataLen
  )
{
  uint8_least Counter;
  /* create a pseudo pbuf and a pointer to it */
  Scc_PbufType Pbuf;
  SCC_P2VARAPPLDATA(Scc_PbufType) PbufPtr;

  /* check the DETs */ /* PRQA S 3109 8 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnVoid(Scc_State_Uninitialized == Scc_State, SCC_API_ID_RX_INDICATION, SCC_DET_NOT_INITIALIZED)
  Scc_CheckDetErrorReturnVoid(NULL_PTR == DataPtr, SCC_API_ID_RX_INDICATION, SCC_DET_INV_POINTER)
  Scc_CheckDetErrorReturnVoid(NULL_PTR == SourcePtr, SCC_API_ID_RX_INDICATION, SCC_DET_INV_POINTER)

  SCC_DUMMY_STATEMENT(SourcePtr); /* PRQA S 3112,3199 */ /* MD_MSR_14.2,MD_Scc_3198_3199 */

  /* set the pbuf */
  Pbuf.payload = DataPtr;
  Pbuf.len     = DataLen;
  Pbuf.totLen  = DataLen;
  /* set the pbuf pointer */
  PbufPtr = &Pbuf;

  /* check if a SDP or V2G message is expected */
  if (   ( Scc_MsgState_RequestSent == Scc_MsgState )
      && (   (   ( Scc_SDPSocket == SockHnd )
              && ( Scc_MsgTrig_SECCDiscoveryProtocol == Scc_MsgTrig ))
          || (   ( Scc_V2GSocket == SockHnd )
              && ( Scc_MsgTrig_SupportedAppProtocol <= Scc_MsgTrig )
              && ( Scc_MsgTrig_StopCommunicationSession > Scc_MsgTrig ))))
  {
    uint16 PBufPtrIdx = 0;
    uint32 PBufPtrRemLen = PbufPtr->totLen;

    /* add all remaining segments of the PBuf */
    do
    {
      /* check if this element is adjacent to the one before */ /* PRQA S 0488 3 */ /* MD_Scc_0488 */
      if (   ( 0 < Scc_ExiStreamRxPBufIdx )
          && ( ( Scc_ExiStreamRxPBuf[Scc_ExiStreamRxPBufIdx-1].payload +
                 Scc_ExiStreamRxPBuf[Scc_ExiStreamRxPBufIdx-1].len )
                 == PbufPtr[PBufPtrIdx].payload ))
      {
        /* only increase the length */
        Scc_ExiStreamRxPBuf[Scc_ExiStreamRxPBufIdx-1].len += PbufPtr[PBufPtrIdx].len;
        Scc_ExiStreamRxPBuf[0].totLen                     += PbufPtr[PBufPtrIdx].len;
      }
      /* this element is somewhere else in the memory */
      else
      {
        /* add the next segmnet */
        Scc_ExiStreamRxPBuf[Scc_ExiStreamRxPBufIdx].payload = PbufPtr[PBufPtrIdx].payload;
        Scc_ExiStreamRxPBuf[0].totLen                      += PbufPtr[PBufPtrIdx].len;
        Scc_ExiStreamRxPBuf[Scc_ExiStreamRxPBufIdx].len     = PbufPtr[PBufPtrIdx].len;
        /* increase the index */
        Scc_ExiStreamRxPBufIdx++;
      }
      /* reduce the remaining length */
      PBufPtrRemLen -= PbufPtr[PBufPtrIdx].len;
      /* increase the index */
      PBufPtrIdx++;
    }
    while ( 0 < PBufPtrRemLen );
  }
  /* if no message was excepected, ignore it */
  else
  {

    /* check if this RxIndication was issued for the TCP socket */
    if ( Scc_V2GSocket == SockHnd )
    {
      /* release buffer in transport layer */
      (void) Tl_Received(SockHnd, PbufPtr->totLen);
    }
    return;
  }

  /* check if the V2G header is not completely received yet */
  if ( SCC_V2GTP_HDR_LEN > Scc_ExiStreamRxPBuf[0].totLen )
  {
    /* wait for more data */
    return;
  }

  /* check if the v2g header is invalid */
  if ( (Std_ReturnType)E_OK != Scc_CheckV2GHeader(SockHnd) )
  {
    /* reset the PBuf */
    Scc_ExiStreamRxPBuf[0].totLen = 0;
    Scc_ExiStreamRxPBuf[0].len    = 0;
    Scc_ExiStreamRxPBufIdx        = 0;
    /* ignore this message */
    return;
  }

  {
    uint8  PayloadLengthArr[4];
    uint32 PayloadLengthBuf;
    /* copy the payload length from the pbuf */
    if ( (Std_ReturnType)E_OK != IpBase_CopyPbuf2String(&PayloadLengthArr[0], &Scc_ExiStreamRxPBuf[0], 4, 4) )
    {
      Scc_CallDetReportError(SCC_API_ID_RX_INDICATION, SCC_DET_INV_PARAM)
    }
    /* convert the uint8 array to uint32 */
    PayloadLengthBuf  =           PayloadLengthArr[3];
    PayloadLengthBuf += ( (uint32)PayloadLengthArr[2] <<  8 );
    PayloadLengthBuf += ( (uint32)PayloadLengthArr[1] << 16 );
    PayloadLengthBuf += ( (uint32)PayloadLengthArr[0] << 24 );
    /* check if the packet is not received completely */
    if ( ( SCC_V2GTP_HDR_LEN + PayloadLengthBuf ) > Scc_ExiStreamRxPBuf[0].totLen )
    {
      /* wait for more data */
      return;
    }
  }

  /* set the totLen element of all PBufs */
  for ( Counter = 1; Counter < Scc_ExiStreamRxPBufIdx; Counter++ )
  {
    Scc_ExiStreamRxPBuf[Counter].totLen = Scc_ExiStreamRxPBuf[0].totLen;
  }

  /* check if this message was received on an UDP socket */
  if ( Scc_SDPSocket == SockHnd )
  {
    /* copy pbuf to exi struct buf, since the data will be gone after return */
    if ( (Std_ReturnType)E_OK != IpBase_CopyPbuf2String(&Scc_Exi_StructBuf[0], &Scc_ExiStreamRxPBuf[0],
      (uint16)( Scc_ExiStreamRxPBuf[0].totLen - SCC_V2GTP_HDR_LEN ), SCC_V2GTP_HDR_LEN) )
    {
      Scc_CallDetReportError(SCC_API_ID_RX_INDICATION, SCC_DET_INV_PARAM)
    }
  }

  /* set the flag for the received response, the message will be processed in the main function */
  Scc_MsgStateFct(Scc_MsgState_ResponseReceived)

  return;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_Cbk_TL_TCPConnected
 *********************************************************************************************************************/
/*! \brief      TCP connected
 *  \param[in]  SockHnd            socket handle
 *  \context    interrupt or task level
 **********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_TL_TCPConnected(Scc_SocketType SockHnd)
{
  /* check the DETs */ /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnVoid(Scc_State_Uninitialized == Scc_State, SCC_API_ID_TCP_CONNECTED, SCC_DET_NOT_INITIALIZED)
  Scc_CheckDetErrorReturnVoid(Scc_V2GSocket != SockHnd, SCC_API_ID_TCP_CONNECTED, SCC_DET_INV_PARAM)

  if (   ( Scc_State_TLConnecting == Scc_State )
      && ( Scc_V2GSocket == SockHnd )) /* PRQA S 3355 */ /* MD_Scc_3109_3201_3355_3358 */ 
  {
    /* update the states */
    Scc_State = Scc_State_TLConnected;
    Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)
    /* report it to the application */
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Connected);
    Scc_ReportSuccessAndStatus(Scc_MsgStatus_TransportLayer_OK);
  }
  /* illegal state for this callback */
  else
  {
    /* ignore */
  }
}

/**********************************************************************************************************************
 *  Scc_Cbk_TL_TCPEvent
 *********************************************************************************************************************/
/*! \brief      TCP event handling
 *  \param[in]  SockHnd            socket handle
 *  \param[in]  Event              socket event
 *  \context    interrupt or task level
 **********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_TL_TCPEvent(Scc_SocketType SockHnd, IpBase_TcpIpEventType Event)
{
  Std_ReturnType RetVal = (Std_ReturnType)E_OK;

  /* check the DETs */ /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorContinue(Scc_State_Uninitialized == Scc_State, SCC_API_ID_TCP_IP_EVENT, SCC_DET_NOT_INITIALIZED)

  /* check if this callback does not concern the TL (TCP/TLS) socket */
  if ( Scc_V2GSocket != SockHnd )
  {
    /* ignore this event */
    return;
  }

  switch ( Event )
  {
    /* check if the connection was closed */
  case IPBASE_TCP_EVENT_CLOSED:
  case IPBASE_TCP_EVENT_RESET:
    /* update the flag */
    Scc_V2GSocketInUse = FALSE;
    Scc_V2GSocket = (Scc_SocketType)0xFFu;
    /* report it to the application */
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Closed);
    break;

  default:
    /* ignore other events */
    break;
  }

  if (( Scc_State_TLConnecting <= Scc_State ) && ( Scc_State_ShuttingDown >= Scc_State ))
  {
    switch ( Event )
    {
    case IPBASE_TCP_EVENT_FIN_RECEIVED:
      /* the SECC is not allowed to close the connection */
      RetVal = (Std_ReturnType)E_NOT_OK;
      break;

    case IPBASE_TCP_EVENT_CLOSED:
      /* check if this shutdown was correctly triggered */
      if ( Scc_State_ShuttingDown == Scc_State )
      {
        Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)
        Scc_ReportSuccessAndStatus(Scc_MsgStatus_StopCommunicationSession_OK);
      }
      /* the connection was closed although there was still an active V2G session */
      else
      {
        RetVal = (Std_ReturnType)E_NOT_OK;
      }
      /* TL connection was disconnected, module will be shut down */
      Scc_State = Scc_State_ShutDown;
      break;

    case IPBASE_TCP_EVENT_RESET:
      /* the connection must not be reset by the EVSE */
      RetVal = (Std_ReturnType)E_NOT_OK;
      break;

    default:
      Scc_CallDetReportError(SCC_API_ID_TCP_IP_EVENT, SCC_DET_INV_PARAM)
      break;
    }
  }

  if ( (Std_ReturnType)E_NOT_OK == RetVal )
  {
    /* write the error and report it to the application */ /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
    Scc_ReportError(Scc_StackError_TransportLayer);
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  Scc_Cbk_TLS_CertChain
 *********************************************************************************************************************/
/*! \brief          TLS validation result for a server certificate chain
 *  \param[in]      SockHnd                socket handle
 *  \param[in,out]  validationResultPtr    Validation result (VALIDATION_OK, VALIDATION_UNKNOWN_CA) 
 *  \param[in]      certChainPtr           pointer to start of the certificate chain 
 *  \param[in]      certChainLen           length of the certificate chain
 *  \context        interrupt or task level
 **********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_TLS_CertChain(
  Scc_SocketType              SockHnd, 
  SCC_P2VARAPPLDATA(uint8)    validationResultPtr,
  SCC_P2CONSTAPPLDATA(uint8)  certChainPtr,
  uint32                      certChainLen )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType       RetVal = (Std_ReturnType)E_OK;
  uint32               CertLen = 0;

  /* check the DETs */ /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorContinue(Scc_State_Uninitialized == Scc_State, SCC_API_ID_TCP_IP_CERT, SCC_DET_NOT_INITIALIZED)

  /* check if this callback does not concern the TL (TCP/TLS) socket */
  if ( Scc_V2GSocket != SockHnd )
  {
    /* ignore this event */
    return;
  }

  /* ----- Implementation ----------------------------------------------- */

  /* read certLen from first 3 bytes of certChainPtr */
  SCC_GET_UINT24(certChainPtr, 0, CertLen);

  switch (*validationResultPtr)
  {
  case TCPIP_TLS_VALIDATION_OK:
    if ( (Std_ReturnType)E_OK != Scc_CheckForCpoInDc(&certChainPtr[3], (uint16)CertLen) )
    {
      /* CPO was not set in Domain Component */
      *validationResultPtr = TCPIP_TLS_VALIDATION_REFUSED_BY_OWNER;
    }
    else
    {
      /* Report leaf certificate to application */
      Scc_Set_Core_TLS_CertChain(certChainPtr);
    }
    break;

  case TCPIP_TLS_VALIDATION_UNKNOWN_CA:
    Scc_Set_Core_TLS_UnknownCALeafCert(certChainPtr);
    break;

  case TCPIP_TLS_VALIDATION_LAST_SIGN_INVALID:
    break;

  case TCPIP_TLS_VALIDATION_INT_SIGN_INVALID:
    break;

  case TCPIP_TLS_VALIDATION_REFUSED_BY_OWNER:
    break;

  default:
    Scc_ReportError(Scc_StackError_TransportLayer);
    break;
  }

  if ( (Std_ReturnType)E_NOT_OK == RetVal )
  {
    /* write the error and report it to the application */ /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
    Scc_ReportError(Scc_StackError_TransportLayer);
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_FirmwareDownloadComplete
 *********************************************************************************************************************/
/*! \brief      callback that is called after the firmware download was processed and the firmware was started
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_SLAC_FirmwareDownloadComplete(boolean DownloadSuccessful)
{
  if ( TRUE == DownloadSuccessful )
  {
    /* firmware download is complete */
    Scc_QCAFirmwareDownloadComplete = Scc_FirmwareDownloadStatus_Complete;
    /* set the qca idle timer */
    Scc_Get_SLAC_QCAIdleTimer(&Scc_QCAIdleTimer);
  }
  else
  {
    /* firmware download did not complete successfully */
    Scc_QCAFirmwareDownloadComplete = Scc_FirmwareDownloadStatus_ErrorOccurred;
  }
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_AssociationStatus
 *********************************************************************************************************************/
/*! \brief      callback function for ongoing association errors and status information
 *  \param[in]  AssociationStatus      the association status reported to the application
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_SLAC_AssociationStatus(uint8 AssociationStatus)
{
  /* check if SLAC failed completely */
  if (( AssociationStatus & 0x0F ) == ETHTRCV_30_AR7000_SLAC_E_VALIDATION_NO_SUCCESS )
  {
    /* report the failure to the application */
    Scc_ReportErrorAndStatus(Scc_StackError_SLAC, Scc_MsgStatus_SLAC_Failed);
  }

  /* provide detailed information to the application */
  Scc_Set_SLAC_AssociationStatus(AssociationStatus);

  return;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_DLinkReady
 *********************************************************************************************************************/
/*! \brief      D-LINK READY indication informs higher layers about a change of PLC link status
 *  \param[in]  DLinkReady      the power line link state after SLAC was finished
 *  \param[in]  NMKPtr          the network membership key (NMK) that was established during the SLAC session
 *  \param[in]  NIDPtr          the network identifier (NID) that was established during the SLAC session
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_SLAC_DLinkReady(EthTrcv_LinkStateType DLinkReady,
  SCC_P2VARAPPLDATA(uint8) NMKPtr, SCC_P2VARAPPLDATA(uint8) NIDPtr)
{
  Scc_DLinkReady = DLinkReady;

  /* check if SLAC was successful */
  if ( ETHTRCV_LINK_STATE_ACTIVE == Scc_DLinkReady )
  {
    Scc_BufferPointerType BufPtr;
    /* provide the NMK to the application */
    BufPtr.BufferLen = ETHTRCV_30_AR7000_NMK_SIZE_BYTE;
    BufPtr.BufferPtr = NMKPtr;
    Scc_Set_SLAC_NMK(&BufPtr);
    /* provide the NID to the application */
    BufPtr.BufferLen = ETHTRCV_30_AR7000_NID_SIZE_BYTE;
    BufPtr.BufferPtr = NIDPtr;
    Scc_Set_SLAC_NID(&BufPtr);
    /* set the next state */
    Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)
    /* report the success to the application */
    Scc_ReportSuccessAndStatus(Scc_MsgStatus_SLAC_OK);
  }
  /* check if no link could be established during SLAC */
  else if ( Scc_MsgTrig_SLAC == Scc_MsgTrigNew )
  {
    /* report the failure to the application */
    Scc_ReportErrorAndStatus(Scc_StackError_SLAC, Scc_MsgStatus_SLAC_Failed);
  }
  /* if the link was lost, the module will be informed via the TrcvLinkStateChg callback */

  return;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_GetRandomizedDataBuffer
 *********************************************************************************************************************/
/*! \brief      the callback is used by the SLAC component to gain a random byte array
 *  \param[out] RandomDataPtr      the buffer that must be randomized
 *  \param[in]  RandomDataLen      the length of the buffer
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_SLAC_GetRandomizedDataBuffer(SCC_P2VARAPPLDATA(uint8) RandomDataPtr, uint16 RandomDataLen)
{
  /* provide the random data */
  Scc_Get_RandomData(RandomDataPtr, RandomDataLen);

  return;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_GetValidateToggles
 *********************************************************************************************************************/
/*! \brief      callback function to generate a random number of validation toggles
 *  \return     [1,2,3]      random number of validation toggles
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(uint8) Scc_Cbk_SLAC_GetValidateToggles(void)
{
  uint8 ToggleNum;

  /* get random data */
  Scc_Get_RandomData(&ToggleNum, sizeof(ToggleNum));

  /* value might only be between 1 and 3 */
  return (( ToggleNum % 3 ) + 1 );
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_ToggleRequest
 *********************************************************************************************************************/
/*! \brief      callback to request the toggle process during the validation phase
 *  \param[in]  ToggleNum      number of BCB-Toggles the application must generate on the Control Pilot
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_SLAC_ToggleRequest(uint8 ToggleNum)
{
  /* inform the application that toggling shall be started */
  Scc_Set_SLAC_ToggleRequest(ToggleNum);

  return;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

/**********************************************************************************************************************
 *  Scc_Cbk_Eth_TransceiverLinkStateChange
 **********************************************************************************************************************/
/*! \brief        Called by lower layer (e.g. EthIf) to indicate a change of the transceiver link state
 *  \param[in]    CtrlIdx         index of the ethernet interface controller
 *  \param[in]    TrcvLinkState   new state of the transceiver
 **********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_Eth_TransceiverLinkStateChange(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState
  )
{
  /* check if this callback is for a different controller */
  if ( SCC_CTRL_IDX != CtrlIdx )
  {
    return;
  }

  /* check if the link state went up */
  if ( ETHTRCV_LINK_STATE_ACTIVE == TrcvLinkState )
  {
    /* inform the application */
    Scc_Set_Core_TrcvLinkState(TRUE);
  }
  /* link went down */
  else
  {
    /* remove all assigned IP addresses */
    Scc_IPAddressCnt = 0;
    /* inform the application */
    Scc_Set_Core_TrcvLinkState(FALSE);
  }
}

/**********************************************************************************************************************
 *  Scc_Cbk_IP_AddressAssignmentChange
 *********************************************************************************************************************/
/*! \brief      IP address assignment change callback
 *  \param[in]  LocalAddrId           ID of the local address
 *  \param[in]  State                 state of the local address
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_IP_AddressAssignmentChange(TcpIp_LocalAddrIdType LocalAddrId, TcpIp_IpAddrStateType State)
{

  /* check the DETs */ /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnVoid(Scc_State_Uninitialized == Scc_State,
    SCC_API_ID_IP_ADDRESS_CHG_CBK, SCC_DET_NOT_INITIALIZED);

  /* check if this is the assigned address */
  if ( SCC_IPV6_ADDRESS != LocalAddrId )
  {
    return;
  }

  /* check if the SCC was already initialized */
  if ( Scc_State_Initialized <= Scc_State )
  {
    /* check if the IP address was assigned */
    if ( State == TCPIP_IPADDR_STATE_ASSIGNED )
    {
      if ( 0 == Scc_IPAddressCnt )
      {
        /* inform the application about the IP assignment */
        Scc_Set_Core_IPAssigned(TRUE);
      }

      /* more than 255 IPv6 addresses are not supported */
      if ( 0xFF > Scc_IPAddressCnt )
      {
        Scc_IPAddressCnt++;
      }
    }
    /* the IP address was lost */
    else
    {
      if ( 1 >= Scc_IPAddressCnt )
      {
        /* check if a V2G communication session was running */
        if (   ( Scc_State_SDPComplete <= Scc_State )
            && ( Scc_State_Disconnected >= Scc_State ))
        {
          /* reset the message state */ /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
          Scc_MsgStateFct(Scc_MsgState_WaitForNextRequest)
          /* stop the timeout counter */
          Scc_TimeoutCnt = 0;
          /* report the error to the application */ /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
          Scc_ReportError(Scc_StackError_TransportLayer);
        }

        /* stop connection with charge point, reset state to init */
        Scc_State = Scc_State_Initialized;
        Scc_IPAddressCnt = 0;
        /* inform the application about the loss of the IP addresses */
        Scc_Set_Core_IPAssigned(FALSE);
      }
      else
      {
        Scc_IPAddressCnt--;
      }
    }
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

#if ( defined SCC_NUM_OF_DYN_CONFIG_PARAMS ) && ( SCC_NUM_OF_DYN_CONFIG_PARAMS != 0 )
/**********************************************************************************************************************
 *  Scc_DynConfigDataReadAccess
 *********************************************************************************************************************/
/*! \brief       configuration data read access
 *  \param[in]   DataID         data identifier
 *  \param[out]  DataPtr        pointer for diagnostic data
 *  \return      E_OK           configuration data was successfully read
 *  \return      E_NOT_OK       invalid DataID
 *  \context     task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_DynConfigDataReadAccess(Scc_DynConfigParamsType DataID,
  SCC_P2VARAPPLDATA(uint16) DataPtr)
{
  Std_ReturnType RetVal = E_OK;

  /* check the DETs */ /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnValue(Scc_State_Uninitialized == Scc_State,
    SCC_API_ID_DIAG_DATA_READ_ACCESS, SCC_DET_NOT_INITIALIZED, E_NOT_OK);
  Scc_CheckDetErrorReturnValue(NULL_PTR == DataPtr,
    SCC_API_ID_DIAG_DATA_READ_ACCESS, SCC_DET_INV_POINTER, E_NOT_OK);

  switch ( DataID )
  {

    /* --- Timer General --- */

#if ( defined Scc_ConfigType_Timer_General_IPAddressWaitTimeout ) && ( Scc_ConfigType_Timer_General_IPAddressWaitTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_IPAddressWaitTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_General_IPAddressWaitTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_IPAddressWaitTimeout */

#if ( defined Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries ) && ( Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SECCDiscoveryProtocolRetries:
    *DataPtr = (uint16)Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolRetries;
    break;
#endif /* Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries */

#if ( defined Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout ) && ( Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SECCDiscoveryProtocolTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout */

#if ( defined Scc_ConfigType_Timer_General_TransportLayerTimeout ) && ( Scc_ConfigType_Timer_General_TransportLayerTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_TransportLayerTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_General_TransportLayerTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_TransportLayerTimeout */

#if ( defined Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout ) && ( Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SupportedAppProtocolMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_General_SupportedAppProtocolMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout */

    /* --- Timer ISO --- */

#if ( defined Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout ) && ( Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SequencePerformanceTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_SequencePerformanceTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout ) && ( Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CommunicationSetupTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_CommunicationSetupTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_AuthorizationMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_AuthorizationMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CableCheckMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_CableCheckMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CertificateInstallationMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_CertificateInstallationMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CertificateUpdateMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_CertificateUpdateMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ChargeParameterDiscoveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_ChargeParameterDiscoveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ChargingStatusMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_ChargingStatusMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CurrentDemandMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_CurrentDemandMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_MeteringReceiptMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_MeteringReceiptMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PaymentDetailsMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_PaymentDetailsMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PaymentServiceSelectionMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_PaymentServiceSelectionMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PowerDeliveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_PowerDeliveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PreChargeMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_PreChargeMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ServiceDetailMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_ServiceDetailMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ServiceDiscoveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_ServiceDiscoveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SessionSetupMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_SessionSetupMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SessionStopMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_SessionStopMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_WeldingDetectionMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_ISO_WeldingDetectionMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout */

    /* --- Timer DIN --- */

#if ( defined Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout ) && ( Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SequencePerformanceTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_SequencePerformanceTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout ) && ( Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CommunicationSetupTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_CommunicationSetupTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout ) && ( Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ReadyToChargeTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_ReadyToChargeTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ContractAuthenticationMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_ContractAuthenticationMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CableCheckMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_CableCheckMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ChargeParameterDiscoveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_ChargeParameterDiscoveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CurrentDemandMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_CurrentDemandMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ServicePaymentSelectionMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_ServicePaymentSelectionMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_PowerDeliveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_PowerDeliveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_PreChargeMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_PreChargeMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ServiceDiscoveryMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_ServiceDiscoveryMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SessionSetupMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_SessionSetupMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SessionStopMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_SessionStopMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_WeldingDetectionMessageTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_Timer_DIN_WeldingDetectionMessageTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout */

#if ( defined SCC_OEM_VECTOR )

    /* --- OEM Vector - State Machine --- */

#if ( defined Scc_ConfigType_StateM_AcceptUnsecureConnection ) && ( Scc_ConfigType_StateM_AcceptUnsecureConnection == SCC_CFG_TYPE_DYNAMIC )
    /* Accept Unsecure Connection */
  case Scc_DynConfigParam_StateM_AcceptUnsecureConnection:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_AcceptUnsecureConnection;
    break;
#endif /* Scc_ConfigType_StateM_AcceptUnsecureConnection */

#if ( defined Scc_ConfigType_StateM_ReconnectRetries ) && ( Scc_ConfigType_StateM_ReconnectRetries == SCC_CFG_TYPE_DYNAMIC )
    /* Reconnect Retries */
  case Scc_DynConfigParam_StateM_ReconnectRetries:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_ReconnectRetries;
    break;
#endif /* Scc_ConfigType_StateM_ReconnectRetries */

#if ( defined Scc_ConfigType_StateM_ReconnectDelay ) && ( Scc_ConfigType_StateM_ReconnectDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Reconnect Delay */
  case Scc_DynConfigParam_StateM_ReconnectDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_ReconnectDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ReconnectDelay */

#if ( defined Scc_ConfigType_StateM_RequestInternetDetails ) && ( Scc_ConfigType_StateM_RequestInternetDetails == SCC_CFG_TYPE_DYNAMIC )
    /* Request Internet Details */
  case Scc_DynConfigParam_StateM_RequestInternetDetails:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_RequestInternetDetails;
    break;
#endif /* Scc_ConfigType_StateM_RequestInternetDetails */

#if ( defined Scc_ConfigType_StateM_AuthorizationOngoingTimeout ) && ( Scc_ConfigType_StateM_AuthorizationOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
    /* Authorization Cycle Timeout */
  case Scc_DynConfigParam_StateM_AuthorizationOngoingTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_AuthorizationOngoingTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_AuthorizationOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout ) && ( Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
    /* Charge Parameter Discovery Cycle Timeout */
  case Scc_DynConfigParam_StateM_ChargeParameterDiscoveryOngoingTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_ChargeParameterDiscoveryOngoingTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_CableCheckOngoingTimeout ) && ( Scc_ConfigType_StateM_CableCheckOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
    /* Charge Parameter Discovery Cycle Timeout */
  case Scc_DynConfigParam_StateM_CableCheckOngoingTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_CableCheckOngoingTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CableCheckOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_PreChargeOngoingTimeout ) && ( Scc_ConfigType_StateM_PreChargeOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
    /* Charge Parameter Discovery Cycle Timeout */
  case Scc_DynConfigParam_StateM_PreChargeOngoingTimeout:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_PreChargeOngoingTimeout * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_PreChargeOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_AuthorizationNextReqDelay ) && ( Scc_ConfigType_StateM_AuthorizationNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Authorization Next Request Delay */
  case Scc_DynConfigParam_StateM_AuthorizationNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_AuthorizationNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_AuthorizationNextReqDelay */

#if ( defined Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay ) && ( Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Charge Parameter Discovery Next Request Delay */
  case Scc_DynConfigParam_StateM_ChargeParameterDiscoveryNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_ChargeParameterDiscoveryNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay */

#if ( defined Scc_ConfigType_StateM_ChargingStatusNextReqDelay ) && ( Scc_ConfigType_StateM_ChargingStatusNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Charging Status Next Request Delay */
  case Scc_DynConfigParam_StateM_ChargingStatusNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_ChargingStatusNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargingStatusNextReqDelay */

#if ( defined Scc_ConfigType_StateM_CableCheckNextReqDelay ) && ( Scc_ConfigType_StateM_CableCheckNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Cable Check Next Request Delay */
  case Scc_DynConfigParam_StateM_CableCheckNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_CableCheckNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CableCheckNextReqDelay */

#if ( defined Scc_ConfigType_StateM_PreChargeNextReqDelay ) && ( Scc_ConfigType_StateM_PreChargeNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Pre Charge Next Request Delay */
  case Scc_DynConfigParam_StateM_PreChargeNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_PreChargeNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_PreChargeNextReqDelay */

#if ( defined Scc_ConfigType_StateM_CurrentDemandNextReqDelay ) && ( Scc_ConfigType_StateM_CurrentDemandNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Current Demand Next Request Delay */
  case Scc_DynConfigParam_StateM_CurrentDemandNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_CurrentDemandNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CurrentDemandNextReqDelay */

#if ( defined Scc_ConfigType_StateM_WeldingDetectionNextReqDelay ) && ( Scc_ConfigType_StateM_WeldingDetectionNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
    /* Welding Detection Next Request Delay */
  case Scc_DynConfigParam_StateM_WeldingDetectionNextReqDelay:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_WeldingDetectionNextReqDelay * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_WeldingDetectionNextReqDelay */

#if ( defined Scc_ConfigType_StateM_PaymentPrioritization ) && ( Scc_ConfigType_StateM_PaymentPrioritization == SCC_CFG_TYPE_DYNAMIC )
    /* Payment Priorisation */
  case Scc_DynConfigParam_StateM_PaymentPrioritization:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_PaymentPrioritization;
    break;
#endif /* Scc_ConfigType_StateM_PaymentPrioritization */

#if ( defined Scc_ConfigType_StateM_ContractCertificateChainIndexInUse ) && ( Scc_ConfigType_StateM_ContractCertificateChainIndexInUse == SCC_CFG_TYPE_DYNAMIC )
    /* Selected Contract Certificate Slot */
  case Scc_DynConfigParam_StateM_ContractCertificateChainIndexInUse:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_ContractCertificateChainIndexInUse;
    break;
#endif /* Scc_ConfigType_StateM_ContractCertificateChainIndexInUse */

#if ( defined Scc_ConfigType_StateM_CertificateExpireThreshold ) && ( Scc_ConfigType_StateM_CertificateExpireThreshold == SCC_CFG_TYPE_DYNAMIC )
    /* Certificate Expire Threshold */
  case Scc_DynConfigParam_StateM_CertificateExpireThreshold:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_CertificateExpireThreshold;
    break;
#endif /* Scc_ConfigType_StateM_CertificateExpireThreshold */

#if ( defined Scc_ConfigType_StateM_RequestCertificateDetails ) && ( Scc_ConfigType_StateM_RequestCertificateDetails == SCC_CFG_TYPE_DYNAMIC )
    /* Request Certificate Details */
  case Scc_DynConfigParam_StateM_RequestCertificateDetails:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_RequestCertificateDetails;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateDetails */

#if ( defined Scc_ConfigType_StateM_RequestCertificateInstall ) && ( Scc_ConfigType_StateM_RequestCertificateInstall == SCC_CFG_TYPE_DYNAMIC )
    /* Request Certificate Install */
  case Scc_DynConfigParam_StateM_RequestCertificateInstallation:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_RequestCertificateInstall;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateInstall */

#if ( defined Scc_ConfigType_StateM_RequestCertificateUpdate ) && ( Scc_ConfigType_StateM_RequestCertificateUpdate == SCC_CFG_TYPE_DYNAMIC )
    /* Request Certificate Update */
  case Scc_DynConfigParam_StateM_RequestCertificateUpdate:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_RequestCertificateUpdate;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateUpdate */

#if ( defined Scc_ConfigType_StateM_SLACStartMode ) && ( Scc_ConfigType_StateM_SLACStartMode == SCC_CFG_TYPE_DYNAMIC )
    /* SLAC Start Mode */
  case Scc_DynConfigParam_StateM_SLACStartMode:
    *DataPtr = (uint16)Scc_ConfigValue_StateM_SLACStartMode;
    break;
#endif /* Scc_ConfigType_StateM_SLACStartMode */

#if ( defined Scc_ConfigType_StateM_QCAIdleTimer ) && ( Scc_ConfigType_StateM_QCAIdleTimer == SCC_CFG_TYPE_DYNAMIC )
    /* QCA Idle Timer */
  case Scc_DynConfigParam_StateM_QCAIdleTimer:
    *DataPtr = (uint16)( Scc_ConfigValue_StateM_QCAIdleTimer * SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_QCAIdleTimer */

#endif

    /* invalid DataID */
  default:
    RetVal = E_NOT_OK;
    break;
  }

  return RetVal;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif /* SCC_NUM_OF_DYN_CONFIG_PARAMS */

#if ( defined SCC_NUM_OF_DYN_CONFIG_PARAMS ) && ( SCC_NUM_OF_DYN_CONFIG_PARAMS != 0 )
/**********************************************************************************************************************
 *  Scc_DynConfigDataWriteAccess
 *********************************************************************************************************************/
/*! \brief       configuration data write access
 *  \param[in]   DataID         data identifier
 *  \param[in]   Data           configuration data that shall be written to NVRAM
 *  \return      E_OK           configuration data was successfully written
 *  \return      E_NOT_OK       invalid DataID
 *  \context     task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_DynConfigDataWriteAccess(Scc_DynConfigParamsType DataID, uint16 Data)
{
  /* check the DETs */ /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Scc_CheckDetErrorReturnValue(Scc_State_Uninitialized == Scc_State,
    SCC_API_ID_DIAG_DATA_READ_ACCESS, SCC_DET_NOT_INITIALIZED, E_NOT_OK);

  switch ( DataID )
  {

    /* --- Timer General --- */

#if ( defined Scc_ConfigType_Timer_General_IPAddressWaitTimeout ) && ( Scc_ConfigType_Timer_General_IPAddressWaitTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_IPAddressWaitTimeout:
    Scc_ConfigValue_Timer_General_IPAddressWaitTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_IPAddressWaitTimeout */

#if ( defined Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries ) && ( Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SECCDiscoveryProtocolRetries:
    Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolRetries = Data;
    break;
#endif /* Scc_ConfigType_Timer_General_SECCDiscoveryProtocolRetries */

#if ( defined Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout ) && ( Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SECCDiscoveryProtocolTimeout:
    Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_SECCDiscoveryProtocolTimeout */

#if ( defined Scc_ConfigType_Timer_General_TransportLayerTimeout ) && ( Scc_ConfigType_Timer_General_TransportLayerTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_TransportLayerTimeout:
    Scc_ConfigValue_Timer_General_TransportLayerTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_TransportLayerTimeout */

#if ( defined Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout ) && ( Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_General_SupportedAppProtocolMessageTimeout:
    Scc_ConfigValue_Timer_General_SupportedAppProtocolMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_General_SupportedAppProtocolMessageTimeout */

    /* --- Timer ISO --- */

#if ( defined Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout ) && ( Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SequencePerformanceTimeout:
    Scc_ConfigValue_Timer_ISO_SequencePerformanceTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SequencePerformanceTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout ) && ( Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CommunicationSetupTimeout:
    Scc_ConfigValue_Timer_ISO_CommunicationSetupTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CommunicationSetupTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_AuthorizationMessageTimeout:
    Scc_ConfigValue_Timer_ISO_AuthorizationMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_AuthorizationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CableCheckMessageTimeout:
    Scc_ConfigValue_Timer_ISO_CableCheckMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CableCheckMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CertificateInstallationMessageTimeout:
    Scc_ConfigValue_Timer_ISO_CertificateInstallationMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CertificateInstallationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CertificateUpdateMessageTimeout:
    Scc_ConfigValue_Timer_ISO_CertificateUpdateMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CertificateUpdateMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ChargeParameterDiscoveryMessageTimeout:
    Scc_ConfigValue_Timer_ISO_ChargeParameterDiscoveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ChargeParameterDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ChargingStatusMessageTimeout:
    Scc_ConfigValue_Timer_ISO_ChargingStatusMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ChargingStatusMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_CurrentDemandMessageTimeout:
    Scc_ConfigValue_Timer_ISO_CurrentDemandMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_CurrentDemandMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_MeteringReceiptMessageTimeout:
    Scc_ConfigValue_Timer_ISO_MeteringReceiptMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_MeteringReceiptMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PaymentDetailsMessageTimeout:
    Scc_ConfigValue_Timer_ISO_PaymentDetailsMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PaymentDetailsMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PaymentServiceSelectionMessageTimeout:
    Scc_ConfigValue_Timer_ISO_PaymentServiceSelectionMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PaymentServiceSelectionMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PowerDeliveryMessageTimeout:
    Scc_ConfigValue_Timer_ISO_PowerDeliveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PowerDeliveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_PreChargeMessageTimeout:
    Scc_ConfigValue_Timer_ISO_PreChargeMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_PreChargeMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ServiceDetailMessageTimeout:
    Scc_ConfigValue_Timer_ISO_ServiceDetailMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ServiceDetailMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_ServiceDiscoveryMessageTimeout:
    Scc_ConfigValue_Timer_ISO_ServiceDiscoveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_ServiceDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SessionSetupMessageTimeout:
    Scc_ConfigValue_Timer_ISO_SessionSetupMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SessionSetupMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_SessionStopMessageTimeout:
    Scc_ConfigValue_Timer_ISO_SessionStopMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_SessionStopMessageTimeout */

#if ( defined Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout ) && ( Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_ISO_WeldingDetectionMessageTimeout:
    Scc_ConfigValue_Timer_ISO_WeldingDetectionMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_ISO_WeldingDetectionMessageTimeout */

    /* --- Timer DIN --- */

#if ( defined Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout ) && ( Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SequencePerformanceTimeout:
    Scc_ConfigValue_Timer_DIN_SequencePerformanceTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SequencePerformanceTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout ) && ( Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CommunicationSetupTimeout:
    Scc_ConfigValue_Timer_DIN_CommunicationSetupTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CommunicationSetupTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout ) && ( Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ReadyToChargeTimeout:
    Scc_ConfigValue_Timer_DIN_ReadyToChargeTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ReadyToChargeTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ContractAuthenticationMessageTimeout:
    Scc_ConfigValue_Timer_DIN_ContractAuthenticationMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ContractAuthenticationMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CableCheckMessageTimeout:
    Scc_ConfigValue_Timer_DIN_CableCheckMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CableCheckMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ChargeParameterDiscoveryMessageTimeout:
    Scc_ConfigValue_Timer_DIN_ChargeParameterDiscoveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ChargeParameterDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_CurrentDemandMessageTimeout:
    Scc_ConfigValue_Timer_DIN_CurrentDemandMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_CurrentDemandMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ServicePaymentSelectionMessageTimeout:
    Scc_ConfigValue_Timer_DIN_ServicePaymentSelectionMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ServicePaymentSelectionMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_PowerDeliveryMessageTimeout:
    Scc_ConfigValue_Timer_DIN_PowerDeliveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_PowerDeliveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_PreChargeMessageTimeout:
    Scc_ConfigValue_Timer_DIN_PreChargeMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_PreChargeMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_ServiceDiscoveryMessageTimeout:
    Scc_ConfigValue_Timer_DIN_ServiceDiscoveryMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_ServiceDiscoveryMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SessionSetupMessageTimeout:
    Scc_ConfigValue_Timer_DIN_SessionSetupMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SessionSetupMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_SessionStopMessageTimeout:
    Scc_ConfigValue_Timer_DIN_SessionStopMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_SessionStopMessageTimeout */

#if ( defined Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout ) && ( Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_Timer_DIN_WeldingDetectionMessageTimeout:
    Scc_ConfigValue_Timer_DIN_WeldingDetectionMessageTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_Timer_DIN_WeldingDetectionMessageTimeout */

#if ( defined SCC_OEM_VECTOR )

    /* --- OEM Vector - State Machine --- */

#if ( defined Scc_ConfigType_StateM_AcceptUnsecureConnection ) && ( Scc_ConfigType_StateM_AcceptUnsecureConnection == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_AcceptUnsecureConnection:
    Scc_ConfigValue_StateM_AcceptUnsecureConnection = Data;
    break;
#endif /* Scc_ConfigType_StateM_AcceptUnsecureConnection */

#if ( defined Scc_ConfigType_StateM_ReconnectRetries ) && ( Scc_ConfigType_StateM_ReconnectRetries == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ReconnectRetries:
    Scc_ConfigValue_StateM_ReconnectRetries = Data;
    break;
#endif /* Scc_ConfigType_StateM_ReconnectRetries */

#if ( defined Scc_ConfigType_StateM_ReconnectDelay ) && ( Scc_ConfigType_StateM_ReconnectDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ReconnectDelay:
    Scc_ConfigValue_StateM_ReconnectDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ReconnectDelay */

#if ( defined Scc_ConfigType_StateM_RequestInternetDetails ) && ( Scc_ConfigType_StateM_RequestInternetDetails == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_RequestInternetDetails:
    Scc_ConfigValue_StateM_RequestInternetDetails = Data;
    break;
#endif /* Scc_ConfigType_StateM_RequestInternetDetails */

#if ( defined Scc_ConfigType_StateM_AuthorizationOngoingTimeout ) && ( Scc_ConfigType_StateM_AuthorizationOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_AuthorizationOngoingTimeout:
    Scc_ConfigValue_StateM_AuthorizationOngoingTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_AuthorizationOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout ) && ( Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ChargeParameterDiscoveryOngoingTimeout:
    Scc_ConfigValue_StateM_ChargeParameterDiscoveryOngoingTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargeParameterDiscoveryOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_CableCheckOngoingTimeout ) && ( Scc_ConfigType_StateM_CableCheckOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_CableCheckOngoingTimeout:
    Scc_ConfigValue_StateM_CableCheckOngoingTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CableCheckOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_PreChargeOngoingTimeout ) && ( Scc_ConfigType_StateM_PreChargeOngoingTimeout == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_PreChargeOngoingTimeout:
    Scc_ConfigValue_StateM_PreChargeOngoingTimeout = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_PreChargeOngoingTimeout */

#if ( defined Scc_ConfigType_StateM_AuthorizationNextReqDelay ) && ( Scc_ConfigType_StateM_AuthorizationNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_AuthorizationNextReqDelay:
    Scc_ConfigValue_StateM_AuthorizationNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_AuthorizationNextReqDelay */

#if ( defined Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay ) && ( Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ChargeParameterDiscoveryNextReqDelay:
    Scc_ConfigValue_StateM_ChargeParameterDiscoveryNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargeParameterDiscoveryNextReqDelay */

#if ( defined Scc_ConfigType_StateM_ChargingStatusNextReqDelay ) && ( Scc_ConfigType_StateM_ChargingStatusNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ChargingStatusNextReqDelay:
    Scc_ConfigValue_StateM_ChargingStatusNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_ChargingStatusNextReqDelay */

#if ( defined Scc_ConfigType_StateM_CableCheckNextReqDelay ) && ( Scc_ConfigType_StateM_CableCheckNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_CableCheckNextReqDelay:
    Scc_ConfigValue_StateM_CableCheckNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CableCheckNextReqDelay */

#if ( defined Scc_ConfigType_StateM_PreChargeNextReqDelay ) && ( Scc_ConfigType_StateM_PreChargeNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_PreChargeNextReqDelay:
    Scc_ConfigValue_StateM_PreChargeNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_PreChargeNextReqDelay */

#if ( defined Scc_ConfigType_StateM_CurrentDemandNextReqDelay ) && ( Scc_ConfigType_StateM_CurrentDemandNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_CurrentDemandNextReqDelay:
    Scc_ConfigValue_StateM_CurrentDemandNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_CurrentDemandNextReqDelay */

#if ( defined Scc_ConfigType_StateM_WeldingDetectionNextReqDelay ) && ( Scc_ConfigType_StateM_WeldingDetectionNextReqDelay == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_WeldingDetectionNextReqDelay:
    Scc_ConfigValue_StateM_WeldingDetectionNextReqDelay = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_WeldingDetectionNextReqDelay */

#if ( defined Scc_ConfigType_StateM_PaymentPrioritization ) && ( Scc_ConfigType_StateM_PaymentPrioritization == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_PaymentPrioritization:
    Scc_ConfigValue_StateM_PaymentPrioritization = Data;
    break;
#endif /* Scc_ConfigType_StateM_PaymentPrioritization */

#if ( defined Scc_ConfigType_StateM_ContractCertificateChainIndexInUse ) && ( Scc_ConfigType_StateM_ContractCertificateChainIndexInUse == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_ContractCertificateChainIndexInUse:
    Scc_ConfigValue_StateM_ContractCertificateChainIndexInUse = Data;
    break;
#endif /* Scc_ConfigType_StateM_ContractCertificateChainIndexInUse */

#if ( defined Scc_ConfigType_StateM_CertificateExpireThreshold ) && ( Scc_ConfigType_StateM_CertificateExpireThreshold == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_CertificateExpireThreshold:
    Scc_ConfigValue_StateM_CertificateExpireThreshold = Data;
    break;
#endif /* Scc_ConfigType_StateM_CertificateExpireThreshold */

#if ( defined Scc_ConfigType_StateM_RequestCertificateDetails ) && ( Scc_ConfigType_StateM_RequestCertificateDetails == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_RequestCertificateDetails:
    Scc_ConfigValue_StateM_RequestCertificateDetails = Data;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateDetails */

#if ( defined Scc_ConfigType_StateM_RequestCertificateInstall ) && ( Scc_ConfigType_StateM_RequestCertificateInstall == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_RequestCertificateInstallation:
    Scc_ConfigValue_StateM_RequestCertificateInstall = Data;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateInstall */

#if ( defined Scc_ConfigType_StateM_RequestCertificateUpdate ) && ( Scc_ConfigType_StateM_RequestCertificateUpdate == SCC_CFG_TYPE_DYNAMIC )
  case Scc_DynConfigParam_StateM_RequestCertificateUpdate:
    Scc_ConfigValue_StateM_RequestCertificateUpdate = Data;
    break;
#endif /* Scc_ConfigType_StateM_RequestCertificateUpdate */

#if ( defined Scc_ConfigType_StateM_SLACStartMode ) && ( Scc_ConfigType_StateM_SLACStartMode == SCC_CFG_TYPE_DYNAMIC ) /* PRQA S 3332 */ /* MD_Scc_3332 */
    /* SLAC Start Mode */
  case Scc_DynConfigParam_StateM_SLACStartMode:
    Scc_ConfigValue_StateM_SLACStartMode = Data;
    break;
#endif /* Scc_ConfigType_StateM_SLACStartMode */

#if ( defined Scc_ConfigType_StateM_QCAIdleTimer ) && ( Scc_ConfigType_StateM_QCAIdleTimer == SCC_CFG_TYPE_DYNAMIC ) /* PRQA S 3332 */ /* MD_Scc_3332 */
    /* QCA Idle Timer */
  case Scc_DynConfigParam_StateM_QCAIdleTimer:
    Scc_ConfigValue_StateM_QCAIdleTimer = ( Data / SCC_MAIN_FUNCTION_PERIOD_MS );
    break;
#endif /* Scc_ConfigType_StateM_QCAIdleTimer */

#endif

    /* invalid DataID */
  default:
    return E_NOT_OK;
  }

  /* inform the NvM to update the NVRAM */
  (void) NvM_SetRamBlockStatus(SCC_DIAG_PARAMETERS_NVM_BLOCK, TRUE);

  return E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif /* SCC_NUM_OF_DYN_CONFIG_PARAMS */

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ChangeContrCertPrivKeyPassword
 *********************************************************************************************************************/
/*! \brief      changes the key of the stored private key
 *  \pre        not available during an active V2G session
 *  \param[in]  NewPasswordPtr   pointer to the new password
 *  \param[in]  NewSaltPtr       pointer to the new salt (optional, if not used set to NULL)
 *  \return     OK               key was changed successfully
 *  \return     Busy             change of key currently not possible because of an active V2G session
 *  \return     Pending          change of key ongoing
 *  \return     NotOK            an error occurred while changing the key
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ChangeContrCertPrivKeyPassword(
  SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewPasswordPtr, SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewSaltPtr)
{
  /* check if currently no V2G session is active */
  if (   ( Scc_State_TLConnecting <= Scc_State )
      && ( Scc_State_Connected >= Scc_State ))
  {
    return Scc_ReturnType_Busy;
  }

  /* check if the key has not already been loaded */
  if ( Scc_NvMBlockReadState_Processed != Scc_CertsWs.ContrCertPrivKeyReadState )
  {
    /* check if an error occurred or there is no installed key */
    if (   ( Scc_NvMBlockReadState_Error == Scc_CertsWs.ContrCertPrivKeyReadState )
        || ( Scc_NvMBlockReadState_Invalidated == Scc_CertsWs.ContrCertPrivKeyReadState ))
    {
      return Scc_ReturnType_NotOK;
    }
    /* the key has to be loaded */
    else
    {
      /* continue loading */
      Scc_NvmReadContrCertChain(TRUE, FALSE);
      /* wait */
      return Scc_ReturnType_Pending;
    }
  }
  /* the key has been loaded */
  else
  {
    uint8                    NvmReadState;
    SCC_P2VARAPPLDATA(uint8) EncPrivKeyPtr;
    /* check if the NvM is busy */
    (void) NvM_GetErrorStatus(Scc_GetNvMBlockIDContrCertPrivKey(Scc_CertsWs.ChosenContrCertChainIdx), &NvmReadState);
    if ( NVM_REQ_PENDING == NvmReadState )
    {
      return Scc_ReturnType_Pending;
    }

    /* encrypt the private key */
    EncPrivKeyPtr = Scc_XcryptPrivateKey(Scc_XDOM_Encrypt,
      NewPasswordPtr, NewSaltPtr, &Scc_CertsWs.ContrCertPrivKey->Buffer[0]);
    if ( NULL_PTR == EncPrivKeyPtr )
    {
      return Scc_ReturnType_NotOK;
    }

    /* try to write the contract certificate private key to the NvM and check if an error occured */
    if ( (Std_ReturnType)E_OK != NvM_WriteBlock(Scc_GetNvMBlockIDContrCertPrivKey(Scc_CertsWs.ChosenContrCertChainIdx),
      EncPrivKeyPtr) )
    {
      return Scc_ReturnType_NotOK;
    }
  }

  return Scc_ReturnType_OK;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ChangeProvCertPrivKeyPassword
 *********************************************************************************************************************/
/*! \brief      changes the key of the stored private key
 *  \pre        not available during an active V2G session
 *  \param[in]  NewPasswordPtr   pointer to the new password
 *  \param[in]  NewSaltPtr       pointer to the new salt (optional, if not used set to NULL)
 *  \return     OK               key was changed successfully
 *  \return     Busy             change of key currently not possible because of an active V2G session
 *  \return     Pending          change of key ongoing
 *  \return     NotOK            an error occurred while changing the key
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ChangeProvCertPrivKeyPassword(
  SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewPasswordPtr, SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewSaltPtr)
{
  /* check if currently no V2G session is active */
  if (   ( Scc_State_TLConnecting <= Scc_State )
      && ( Scc_State_Connected >= Scc_State ))
  {
    return Scc_ReturnType_Busy;
  }

  /* check if the key has not already been loaded */
  if ( Scc_NvMBlockReadState_Processed != Scc_CertsWs.ProvCertPrivKeyReadState )
  {
    /* check if an error occurred or there is no installed key */
    if (   ( Scc_NvMBlockReadState_Error == Scc_CertsWs.ProvCertPrivKeyReadState )
        || ( Scc_NvMBlockReadState_Invalidated == Scc_CertsWs.ProvCertPrivKeyReadState ))
    {
      return Scc_ReturnType_NotOK;
    }
    /* the key has to be loaded */
    else
    {
      /* continue loading */
      Scc_NvmReadProvCert(TRUE);
      /* wait */
      return Scc_ReturnType_Pending;
    }
  }
  /* the key has been loaded */
  else
  {
    uint8                    NvmReadState;
    SCC_P2VARAPPLDATA(uint8) EncPrivKeyPtr;
    /* check if the NvM is busy */
    (void) NvM_GetErrorStatus(Scc_GetNvMBlockIDProvCertPrivKey(Scc_CertsWs.ChosenContrCertChainIdx), &NvmReadState);
    if ( NVM_REQ_PENDING == NvmReadState )
    {
      return Scc_ReturnType_Pending;
    }

    /* encrypt the private key */
    EncPrivKeyPtr = Scc_XcryptPrivateKey(Scc_XDOM_Encrypt,
      NewPasswordPtr, NewSaltPtr, &Scc_CertsWs.ProvCertPrivKey->Buffer[0]);
    if ( NULL_PTR == EncPrivKeyPtr )
    {
      return Scc_ReturnType_NotOK;
    }

    /* try to write the contract certificate private key to the NvM and check if an error occured */
    if ( (Std_ReturnType)E_OK != NvM_WriteBlock(Scc_GetNvMBlockIDProvCertPrivKey(Scc_CertsWs.ChosenContrCertChainIdx), EncPrivKeyPtr) )
    {
      return Scc_ReturnType_NotOK;
    }
  }

  return Scc_ReturnType_OK;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_Reset
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_Reset(void)
{
  /* check if an UDP socket is opened */
  if ( (Scc_SocketType)0xFFu != Scc_SDPSocket )
  {
    /* close the UDP socket */
    (void) TcpIp_Close(Scc_SDPSocket, FALSE);
  }

  /* check if a TL connection is established */
  if ( TRUE == Scc_V2GSocketInUse )
  {
    /* close the TL connection */
    (void) Tl_Close(Scc_V2GSocket);
    /* report it to the application */
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Disconnecting);
  }
  else
  {
    /* report to the application that the socket is closed*/
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Closed);
  }

  /* reset all state and message relevant parameters */
  Scc_State                     = Scc_State_Initialized;
  Scc_MsgState                  = Scc_MsgState_WaitForNextRequest;
  Scc_MsgTrig                   = Scc_MsgTrig_None;
  Scc_CyclicMsgTrig             = FALSE;
  Scc_MsgTrigNew                = Scc_MsgTrig_None;
  Scc_MsgStatus                 = Scc_MsgStatus_None;
  Scc_StackError                = Scc_StackError_NoError;
  Scc_TimeoutCnt                = 0;
  Scc_SdpRetransRetriesCnt      = (uint16)Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolRetries;
  Scc_ExiStreamRxPBuf[0].totLen = 0;
  Scc_ExiStreamRxPBuf[0].len    = 0;
  Scc_ExiStreamTxPBufPtr        = (SCC_P2VARNOINIT(IpBase_PbufType))NULL_PTR;
  Scc_ExiStreamRxPBufIdx        = 0;
  Scc_TxDataSent                = 0;
  Scc_TxdDataConfirmed          = 0;
  Scc_TxStreamingActive         = FALSE;
  Scc_SDPSocket                 = (Scc_SocketType)0xFFu;

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  Scc_QCAIdleTimer = 0;
#endif /* SCC_ENABLE_SLAC_HANDLING */

}

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_TriggerSLAC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_TriggerSLAC(void)
{
  EthTrcv_30_Ar7000_Slac_StartModeType SLACStartMode;

  /* check if this is the initial trigger */
  if ( TRUE == Scc_CyclicMsgTrig )
  {
    /* accept the new trigger */
    Scc_MsgTrig = Scc_MsgTrigNew;
    /* reset the StackError */
    Scc_StackError = Scc_StackError_NoError;
    /* reset the cyclic message trigger at the application */
    Scc_Set_Core_CyclicMsgTrigTx(FALSE);
  }

  if ( Scc_MsgState_WaitForNextRequest == Scc_MsgState )
  {
    /* check if the link is already established */
    if ( TRUE == Scc_DLinkReady )
    {
      /* check if it was not already reported to the application that SLAC is done */
      if ( Scc_MsgStatus_SLAC_OK != Scc_MsgStatus )
      {
        /* report the success to the application */
        Scc_ReportSuccessAndStatus(Scc_MsgStatus_SLAC_OK);
      }
      return;
    }
    /* check if the firmware download is not finished or the idle timer is not elapsed yet */
    if (   ( Scc_FirmwareDownloadStatus_Unknown == Scc_QCAFirmwareDownloadComplete )
        || (   ( 0 != Scc_QCAIdleTimer )
            && ( Scc_FirmwareDownloadStatus_ErrorOccurred != Scc_QCAFirmwareDownloadComplete )))
    {
      return;
    }
    /* check if an error occurred during the download of the firmware to the QCA */
    else if ( Scc_FirmwareDownloadStatus_ErrorOccurred == Scc_QCAFirmwareDownloadComplete )
    {
      /* check if it was not already reported to the application that SLAC has failed */
      if ( Scc_MsgStatus_SLAC_Failed != Scc_MsgStatus )
      {
        /* report the failure to the application */
        Scc_ReportErrorAndStatus(Scc_StackError_SLAC, Scc_MsgStatus_SLAC_Failed);
      }
      return;
    }
    /* firmware download was successful */
    else /* if ( Scc_FirmwareDownloadStatus_Complete == Scc_QCAFirmwareDownloadComplete ) */
    {
      /* get the start type */
      Scc_Get_SLAC_StartMode(&SLACStartMode);
      /* start SLAC handling */
      if ( E_OK == EthTrcv_30_Ar7000_Slac_Start(SCC_TRANSCEIVER_INDEX, SLACStartMode) )
      {
        /* go to the next state */
        Scc_MsgStateFct(Scc_MsgState_RequestSent)
      }
    }
  }
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

/**********************************************************************************************************************
 *  Scc_TriggerV2G
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_TriggerV2G(void)
{
  /* check if it is allowed to send a request */
  if (   ( Scc_MsgTrig == Scc_MsgTrigNew )
      && ( TRUE != Scc_CyclicMsgTrig ))
  {
    return;
  }

  /* check if there is still an open connection */
  if (   ( Scc_MsgTrig_TransportLayer == Scc_MsgTrigNew )
      && ( TRUE == Scc_V2GSocketInUse ))
  {
    /* close the connection before opening a new one */
    (void) Tl_Close(Scc_V2GSocket);
    /* report it to the application */
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Disconnecting);
    return;
  }

  /* accept the new trigger */
  Scc_MsgTrig = Scc_MsgTrigNew;
  /* reset the StackError */
  Scc_StackError = Scc_StackError_NoError;
  /* reset the cyclic message trigger at the application */
  Scc_Set_Core_CyclicMsgTrigTx(FALSE);

  /* check if a secc discovery protocol request shall be sent */
  if ( Scc_MsgTrig_SECCDiscoveryProtocol == Scc_MsgTrig )
  {
    uint8 ChangeParameterParam = 1;
    uint16 Port = TCPIP_PORT_ANY;

    /* check if the UDP socket is not already bound to SCC */
    if ( (Scc_SocketType)0xFFu == Scc_SDPSocket )
    {
      /* get the socket */
      if ( (Std_ReturnType)E_OK !=
        TcpIp_SccGetSocket(TCPIP_AF_INET6, TCPIP_IPPROTO_UDP, &Scc_SDPSocket) )
      {
        Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
        return;
      }
      /* set the tx confirmation list */
      if ( (Std_ReturnType)E_OK !=
        TcpIp_ChangeParameter(Scc_SDPSocket, TCPIP_PARAMID_V_UDP_TXREQLISTSIZE, &ChangeParameterParam) )
      {
        Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
        return;
      }
      /* bind the socket */
      if ( (Std_ReturnType)E_OK != TcpIp_Bind(Scc_SDPSocket, SCC_IPV6_ADDRESS, &Port) )
      {
        Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
        return;
      }
    }

    /* try to send a SDP request and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_TxSECCDiscoveryProtocolReq() )
    {
        Scc_MsgStateFct(Scc_MsgState_RequestSent)
      /* start the timer */
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolTimeout;
    }
  }

  /* check if the transport layer connection shall be established */
  else if (   ( Scc_State_SDPComplete == Scc_State )
           && ( Scc_MsgTrig_TransportLayer == Scc_MsgTrig ))
  {
    Scc_EstablishTLConnection();
  }

  /* check if a SupportedAppProtocolReq shall be sent */
  else if (   ( Scc_State_TLConnected == Scc_State )
           && ( Scc_MsgTrig_SupportedAppProtocol == Scc_MsgTrig ))
  {
    /* send the SAP request */
    Scc_ReturnType RetVal = Scc_Exi_EncodeSupportedAppProtocolReq();

    /* check if it was successful */
    if ( Scc_ReturnType_OK == RetVal )
    {
        Scc_MsgStateFct(Scc_MsgState_RequestSent)
    }
    /* check if this packet is streamed */
    else if ( Scc_ReturnType_Pending == RetVal ) /* PRQA S 2004 */ /* MD_Scc_2004 */
    {
      Scc_MsgStateFct(Scc_MsgState_StreamingRequest)
    }
  }

  /* if a transport layer connection is established */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
  else if (   (   ( Scc_State_SAPComplete == Scc_State )
               && ( Scc_MsgTrig_SessionSetup == Scc_MsgTrig ))
           || (   ( Scc_State_Connected == Scc_State )
               && (   ( Scc_MsgTrig_ServiceDiscovery <= Scc_MsgTrig )
                   && ( Scc_MsgTrig_SessionStop >= Scc_MsgTrig ))))
  {
    /* send a V2G request */
    Scc_ReturnType RetVal = (*Scc_ExiTx_Xyz_EncodeMessageFctPtr)();

    /* check if it was successful */
    if ( Scc_ReturnType_OK == RetVal )
    {
        Scc_MsgStateFct(Scc_MsgState_RequestSent)
    }
    /* check if this packet is streamed */
    else if ( Scc_ReturnType_Pending == RetVal ) /* PRQA S 2004 */ /* MD_Scc_2004 */
    {
      Scc_MsgStateFct(Scc_MsgState_StreamingRequest)
    }
  }

  /* check if the tcp connection should be closed */
  else if (   ( Scc_State_Disconnected == Scc_State )
           && ( Scc_MsgTrig_StopCommunicationSession == Scc_MsgTrig ))
  {
    if ( (Std_ReturnType)E_OK == Tl_Close(Scc_V2GSocket) )
    {
      Scc_State = Scc_State_ShuttingDown;
      Scc_MsgStateFct(Scc_MsgState_RequestSent)
      /* inform the application */
      Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Disconnecting);
      /* start the timeout (if value is 0, timeout will not be started) */
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_General_TransportLayerTimeout;
    }
    else
    {
      /* write the error */ /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
      Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_StopCommunicationSession_Failed);
    }
  }

  /* if the trigger was not valid */
  else
  {
    /* report a V2G error of the current trigger */ /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Scc_ReportError(Scc_StackError_InvalidTxParameter);
  }
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_TimeoutHandling
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_TimeoutHandling(void)
{
  /* check if the timer is active */
  if ( 0 < Scc_TimeoutCnt )
  {
    /* decrement the timer */
    Scc_TimeoutCnt--;
    /* check if a timeout occured */
    if ( 0 == Scc_TimeoutCnt )
    {
      /* check if the timeout happened during the SDP */
      if ( Scc_MsgTrig_SECCDiscoveryProtocol == Scc_MsgTrig )
      {
        /* check if at least one more retry is left */
        if ( 0 < Scc_SdpRetransRetriesCnt )
        {
          /* decrement the SDP retries */
          Scc_SdpRetransRetriesCnt--;
          /* try to send another SDP request and check if it was successful */
          if ( (Std_ReturnType)E_OK == Scc_TxSECCDiscoveryProtocolReq() )
          {
              Scc_MsgStateFct(Scc_MsgState_RequestSent)
            /* set the timer */
            Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolTimeout;
            return;
          }
        }
      }

      /* if the packet was not transmitted (completely) */
      /* report the timeout to the application */
      Scc_ReportError(Scc_StackError_Timeout);
    }
  }
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_TxSECCDiscoveryProtocolReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_TxSECCDiscoveryProtocolReq(void)
{
  /* return type for ProvideTxBuffer */
  BufReq_ReturnType BufReqRetVal;
  /* pointer to the buffer for the SDP request */
  IpBase_PbufType *PbufPtr;
  /* create the socket address struct */
  Scc_SockAddrIn6Type IpAddrPortDst;

  /* use the exi struct buf to create the v2gtp header */

  /* set the version info */
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_Version]        = (uint8)   SCC_V2G_PROT_VER;
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_InverseVersion] = (uint8)((~SCC_V2G_PROT_VER) & 0xFFu);
  /* set the payload type */
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadType] =
    (uint8)(( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SDP_REQ & 0xFF00u ) >> 8u );
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadType+1] =
    (uint8) ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SDP_REQ & 0x00FFu );
  /* set the payload length */
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadLength] =
    (uint8)(( (SCC_SDP_REQ_LEN-SCC_V2GTP_HDR_LEN) & 0xFF000000u ) >> 24u );
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadLength+1] =
    (uint8)(( (SCC_SDP_REQ_LEN-SCC_V2GTP_HDR_LEN) & 0x00FF0000u ) >> 16u );
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadLength+2] =
    (uint8)(( (SCC_SDP_REQ_LEN-SCC_V2GTP_HDR_LEN) & 0x0000FF00u ) >> 8u );
  Scc_Exi_StructBuf[Scc_V2GTPOffsets_PayloadLength+3] =
    (uint8) ( (SCC_SDP_REQ_LEN-SCC_V2GTP_HDR_LEN) & 0x000000FFu );

  /* add the SDP request */
  Scc_Get_Core_SDPSecurityRx(&Scc_SelectedConnectionType);
  Scc_Exi_StructBuf[SCC_V2GTP_HDR_LEN+Scc_SDPReqOffsets_Security] = (uint8)Scc_SelectedConnectionType;
  Scc_Exi_StructBuf[SCC_V2GTP_HDR_LEN+Scc_SDPReqOffsets_Protocol] = SCC_SDP_TP_TCP;

  /* create and initialize the socket address struct */
  IpAddrPortDst.domain = SCC_IPvX_IPV6;
  IpAddrPortDst.port   = IPBASE_HTON16(SCC_SDP_SERVER_PORT);
  /* copy the IP address */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType))&IpAddrPortDst.addr[0],
    (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType))&IpV6_AddrAllNodesLL, sizeof(IpV6_AddrAllNodesLL));

  /* get the tx buffer from Udp */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BufReqRetVal = TcpIp_ProvideTxBuffer(Scc_SDPSocket, (SCC_P2CONSTAPPLDATA(Scc_SockAddrType))&IpAddrPortDst,
    &PbufPtr, SCC_SDP_REQ_LEN);
  /* check if an error occured */
  if (   ( BUFREQ_OK != BufReqRetVal )
      && ( BUFREQ_E_BUSY != BufReqRetVal ))
  {
    /* report the error */
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_SECCDiscoveryProtocol_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }
  /* check if at the moment no buffer is available */
  else if ( BUFREQ_E_BUSY == BufReqRetVal ) /* PRQA S 2004 */ /* MD_Scc_2004 */
  {
    /* skip this SECCDiscoveryProtocolReq transmission */
    Scc_MsgStateFct(Scc_MsgState_RequestSent)
    return (Std_ReturnType)E_OK;
  }

  /* copy the sdp request from the linear buffer to the pbuf of Udp */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK == IpBase_CopyString2PbufAt((SCC_P2CONSTAPPLDATA(uint8))&Scc_Exi_StructBuf[0],
    SCC_SDP_REQ_LEN, PbufPtr, 0) )
  {
    Scc_TxDataSent = PbufPtr->totLen;
  }
  else
  {
    Scc_ReportErrorAndStatus(Scc_StackError_IpBase, Scc_MsgStatus_SECCDiscoveryProtocol_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }
  /* transmit SDP request and check if it failed */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != TcpIp_TransmitTo(Scc_SDPSocket, PbufPtr,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&IpAddrPortDst) )
  {
    Scc_TxDataSent = 0;
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_SECCDiscoveryProtocol_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_RxSECCDiscoveryProtocolRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_RxSECCDiscoveryProtocolRes(void)
{
  Std_ReturnType RetVal = E_OK;

  /* check if the option for the transport protocol and security are correct, else ignore this message */
  if (   ( (uint8)SCC_SDP_TP_TCP == Scc_Exi_StructBuf[Scc_SDPResOffsets_Protocol] )
      && (   ( (uint8)Scc_SDPSecurity_Tls == Scc_Exi_StructBuf[Scc_SDPResOffsets_Security] )
          || ( (uint8)Scc_SDPSecurity_None == Scc_Exi_StructBuf[Scc_SDPResOffsets_Security] )))
  {
    uint16 Port;

    Port  = (uint16)( (uint16)Scc_Exi_StructBuf[Scc_SDPResOffsets_Port   ] << 8u );
    Port |= (uint16)          Scc_Exi_StructBuf[Scc_SDPResOffsets_Port+1u];

    /* set the server socket address struct */
    Scc_ServerSockAddr.domain = SCC_IPvX_IPV6;
    Scc_ServerSockAddr.port   = IPBASE_HTON16(Port);
    /* copy the IP address of the server socket */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType))&Scc_ServerSockAddr.addr[0u],
      (SCC_P2VARAPPLDATA(IpBase_CopyDataType))&Scc_Exi_StructBuf[Scc_SDPResOffsets_IPAddress], 16u);

    /* provide the socket to the application */
    Scc_Set_Core_SECCIPAddress(&Scc_Exi_StructBuf[Scc_SDPResOffsets_IPAddress]);
    Scc_Set_Core_SECCPort(Port);

    /* extract the Security information */
    Scc_Set_Core_SDPSecurityTx((Scc_SDPSecurityType)Scc_Exi_StructBuf[Scc_SDPResOffsets_Security]);

    /* reset the retry counter */
    Scc_SdpRetransRetriesCnt = (uint16)Scc_ConfigValue_Timer_General_SECCDiscoveryProtocolRetries;

    /* report the status to the application */
    Scc_ReportSuccessAndStatus(Scc_MsgStatus_SECCDiscoveryProtocol_OK);

    /* set the new state */
    Scc_State = Scc_State_SDPComplete;
  }
  /* invalid message received */
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}

/**********************************************************************************************************************
 *  Scc_CheckV2GHeader
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_CheckV2GHeader(Scc_SocketType Socket)
{
  Std_ReturnType RetVal = (Std_ReturnType)E_OK;
  uint8          V2GTPHeaderBuf[4];

  /* copy the header from the pbuf */
  if ( (Std_ReturnType)E_OK != IpBase_CopyPbuf2String(&V2GTPHeaderBuf[0], &Scc_ExiStreamRxPBuf[0], 4, 0) )
  {
    Scc_CallDetReportError(SCC_API_ID_V_CHECK_V2G_HEADER, SCC_DET_INV_PARAM)
  }

  /* check the version */
  if (   ( (uint8)SCC_V2G_PROT_VER != V2GTPHeaderBuf[0] )
      || ( (uint8)SCC_V2G_PROT_VER != (uint8)(~V2GTPHeaderBuf[1]) ))
  {
    RetVal = (Std_ReturnType)E_NOT_OK;
  }

  /* if it is a SDP packet */
  else if ( Scc_SDPSocket == Socket )
  {
    /* check the payload type */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
    if (   ( ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SDP_RES & 0xFF00u ) >> 8 ) != V2GTPHeaderBuf[2] )
        ||   ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SDP_RES & 0x00FFu )        != V2GTPHeaderBuf[3] ))
    {
      RetVal = (Std_ReturnType)E_NOT_OK;
    }
  }
  /* if it is a V2G packet */
  else if ( Scc_V2GSocket == Socket )
  {
    /* check the payload type */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
    if (   (   ( ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SAP & 0xFF00u ) >> 8 ) != V2GTPHeaderBuf[2] )
            ||   ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_SAP & 0x00FFu )        != V2GTPHeaderBuf[3] ))
        && (   ( ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_EXI & 0xFF00u ) >> 8 ) != V2GTPHeaderBuf[2] )
            ||   ( ( (uint16)SCC_V2GTP_HDR_PAYLOAD_TYPE_EXI & 0x00FFu )        != V2GTPHeaderBuf[3] )))
    {
      RetVal = (Std_ReturnType)E_NOT_OK;
    }
  }
  else
  {
    /* invalid socket */
    RetVal = (Std_ReturnType)E_NOT_OK;
  }

  return RetVal;
}

/**********************************************************************************************************************
 *  Scc_EstablishTLConnection
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_EstablishTLConnection(void)
{
  uint32         u32ChangeParameterParam;
  uint16         Port = TCPIP_PORT_ANY;
  uint8          u8ChangeParameterParam;

  /* get the socket */
  if ( (Std_ReturnType)E_OK != TcpIp_SccGetSocket(TCPIP_AF_INET6, TCPIP_IPPROTO_TCP, &Scc_V2GSocket) )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
    return;
  }

  /* set the tcp tx buffer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  u32ChangeParameterParam = SCC_TCP_TX_BUF_LEN;
  if ( (Std_ReturnType)E_OK != TcpIp_ChangeParameter(Scc_V2GSocket, TCPIP_PARAMID_V_TCP_TXBUFSIZE,
    (SCC_P2VARAPPLDATA(uint8))&u32ChangeParameterParam) )
  {
    (void) TcpIp_Close(Scc_V2GSocket, FALSE);
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
    return;
  }
  /* set the tcp rx buffer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  u32ChangeParameterParam = SCC_TCP_RX_BUF_LEN;
  if ( (Std_ReturnType)E_OK != TcpIp_ChangeParameter(Scc_V2GSocket, TCPIP_PARAMID_TCP_RXWND_MAX,
    (SCC_P2VARAPPLDATA(uint8))&u32ChangeParameterParam) )
  {
    (void) TcpIp_Close(Scc_V2GSocket, FALSE);
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
    return;
  }
  
#if ( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON )
  /* get the SDPSecurity from the application */
  Scc_Get_Core_SDPSecurityRx(&Scc_SelectedConnectionType);

  /* check if TLS shall be used */
  if ( Scc_SDPSecurity_Tls == Scc_SelectedConnectionType )
  {
    /* set the tcp tx buffer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
    u8ChangeParameterParam = (uint8)TRUE;
    if ( (Std_ReturnType)E_OK != TcpIp_ChangeParameter(Scc_V2GSocket, TCPIP_PARAMID_V_USE_TLS,
      (SCC_P2VARAPPLDATA(uint8))&u8ChangeParameterParam) )
    {
      (void) TcpIp_Close(Scc_V2GSocket, FALSE);
      Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
      return;
    }
    /* set the tcp tx buffer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
    u32ChangeParameterParam = SCC_TLS_TX_BUF_LEN;
    if ( (Std_ReturnType)E_OK != TcpIp_ChangeParameter(Scc_V2GSocket, TCPIP_PARAMID_V_TLS_TXBUFSIZE,
      (SCC_P2VARAPPLDATA(uint8))&u32ChangeParameterParam) )
    {
      (void) TcpIp_Close(Scc_V2GSocket, FALSE);
      Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
      return;
    }
    /* set the tcp rx buffer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
    u32ChangeParameterParam = SCC_TLS_RX_BUF_LEN;
    if ( (Std_ReturnType)E_OK != TcpIp_ChangeParameter(Scc_V2GSocket, TCPIP_PARAMID_V_TLS_RXBUFSIZE,
      (SCC_P2VARAPPLDATA(uint8))&u32ChangeParameterParam) )
    {
      (void) TcpIp_Close(Scc_V2GSocket, FALSE);
      Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
      return;
    }
  }
#endif

  /* bind the socket */
  if ( (Std_ReturnType)E_OK != TcpIp_Bind(Scc_V2GSocket, SCC_IPV6_ADDRESS, &Port) )
  {
    (void) TcpIp_Close(Scc_V2GSocket, FALSE);
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
    return;
  }

  /* start the TL connection */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK == TcpIp_TcpConnect(Scc_V2GSocket, (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr) )
  {
    /* socket is now in use */
    Scc_V2GSocketInUse = TRUE;
    /* update the states */
    Scc_State = Scc_State_TLConnecting;
    Scc_MsgStateFct(Scc_MsgState_RequestSent)
    /* report it to the application */
    Scc_Set_Core_TCPSocketState(Scc_TCPSocketState_Connecting);
    /* start the timeout (if value is 0, timeout will not be started) */
    Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_General_TransportLayerTimeout;
  }
  else
  {
    /* report the error */
    Scc_ReportErrorAndStatus(Scc_StackError_TransportLayer, Scc_MsgStatus_TransportLayer_Failed);
    /* close the transport layer connection */
    (void) TcpIp_Close(Scc_V2GSocket, FALSE);
  }
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

/* Justification for module specific MISRA deviations:

  MD_Scc_0290:
    Reason:     Unsigned variable is cast to signed because of a bitwise operator. The variable is then again assigned
                  to an unsigned variable, which leads to the PRQA warning.
    Risk:       No risk, since the variable is used bitwise.
    Prevention: Covered by code review.

  MD_Scc_0310_3305:
    Reason:     Cast is necessary to better use the resources.
    Risk:       Alignment issues may occur.
    Prevention: Covered by code review.

  MD_Scc_0488:
    Reason:     Avoiding these calculations would prevent to use only two PBuf elements.
    Risk:       Incorrect caluclation could lead to memory access outside of the original area.
    Prevention: Covered by code review.

  MD_Scc_0750_0759:
    Reason:     A union type or parameter has been defined. This is necessary to avoid alignment issues.
    Risk:       Unknown.
    Prevention: Covered by code review.

  MD_Scc_0770:
    Reason:     While searching, the code after the continue must not be executed.
    Risk:       Statements that follow the break are not executed, code could behave unexpected.
    Prevention: Covered by code review.

  MD_Scc_0771:
    Reason:     The code after the break must not be executed.
    Risk:       Statements that follow the break are not executed, code could behave unexpected.
    Prevention: Covered by code review.

  MD_Scc_0777_0779:
    Reason:     Identifier does not differ from another within 31 characters.
    Risk:       Unknown.
    Prevention: Unknown.

  MD_Scc_0781:
    Reason:     Name of atomic parameter is also used as a name of a struct parameter.
    Risk:       None.
    Prevention: Covered by code review.

  MD_Scc_0832:
    Reason:     Better readability.
    Risk:       Unknown.
    Prevention: Covered by code review.

  MD_Scc_2002:
    Reason:     No default handling required.
    Risk:       Missing default handling.
    Prevention: Covered by test cases and code review.

  MD_Scc_2004:
    Reason:     No default handling required.
    Risk:       Missing default handling.
    Prevention: Covered by test cases and code review.

  MD_Scc_2018:
    Reason:     Switch parameter may contain a value that is not within its original value range.
    Risk:       None.
    Prevention: Covered by test cases and code review.

  MD_Scc_2462_2463_2469:
    Reason:     Control variable is not used in the usual way.
    Risk:       Infinite loop could be created.
    Prevention: Covered by test cases and code review.

  MD_Scc_3109_3201_3355_3358:
    Reason:     The statement will not be reached in this configuration. But there are other configuration variants
                  in which this statement will be reached. E.g. other post-build configuration or different use-case.
    Risk:       None.
    Prevention: Already correctly handled, but not detected by PRQA.

  MD_Scc_3197:
    Reason:     Assignment is only obsolete, if application correctly sets the value in the callback. Otherwise it
                  prevents an issue from occurring.
    Risk:       None.
    Prevention: Covered by code review.

  MD_Scc_3198_3199:
    Reason:     Assignment may be obsolete, but improves code readability.
    Risk:       None.
    Prevention: Covered by code review.

  MD_Scc_3218:
    Reason:     Readability is more important here than functional-local information hiding.
    Risk:       Small, parameters are already file-local.
    Prevention: Covered by code review.

  MD_Scc_3332:
    Reason:     It is checked if the macro is defined.
    Risk:       None.
    Prevention: Already correctly handled, but not detected by PRQA.

  MD_Scc_3333:
    Reason:     The loop/case is used to search for something. If it was found,
                  the loop/case can be quit (by using break).
    Risk:       Statements that follow the break are not executed, code could behave unexpected.
    Prevention: Covered by code review.

  MD_Scc_3415:
    Reason:     If the first function doesn't return successfully, the other functions mustn't be called.
    Risk:       The functions following may need to be called regardless.
    Prevention: Covered by code review.

  MD_Scc_3680_3685:
    Reason:     Since the actual length of the payload is variable, the length of the payload in the mask is set to 1.
    Risk:       Data outside of the actual payload may be accessed and changed.
    Prevention: Covered by code review.

  MD_Scc_3762:
    Reason:     The scaling is defined in the configuration. It is not known whether the scaling will be positive or
                  negative. Therefore "correct" casting is not possible.
    Risk:       None.
    Prevention: Covered by code review.

  MD_SCC_16.7_3673:
    Reason:     Pointer could be const, but API has to be AUTOSAR-conform
    Risk:       Function could try to change the element the pointer points to, but this should not be allowed.
    Prevention: Covered by code review.

  MD_Scc_External:
    Reason:     MISRA warning occured in or because of an external component.
    Risk:       Unknown.
    Prevention: Unknown.

  MD_Scc_QAC_Mistaken:
    Reason:     MISRA detected an issue where there is none. The code does not violate any rules.
    Risk:       None.
    Prevention: Covered by code review.

*/

/**********************************************************************************************************************
 *  END OF FILE: Scc.c
 *********************************************************************************************************************/
