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
/**        \file  Scc_Exi.c
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
 *********************************************************************************************************************/ 

#define SCC_EXI_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Exi.h"
#include "Scc.h"
#include "Scc_Cfg.h"
#include "Scc_Lcfg.h"
#include "Scc_Priv.h"
#include SCC_IF_CFG_H
#include SCC_CP_CFG_H

#if ( defined SCC_PROD_ERROR_DETECT ) && ( SCC_PROD_ERROR_DETECT == STD_ON )
#include "Dem.h"
#endif /* SCC_PROD_ERROR_DETECT */
#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
#include "EthIf.h"
#include "Exi.h"
#include "IpBase.h"
#include "TcpIp.h"
#if ( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON )
#include "Tls.h"
#endif /* SCC_ENABLE_TLS */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */
/*lint -e438 */
/*lint -e571 */
/*lint -e830 */

/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/

/* 8bit variables */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_VARNOINIT(Scc_Exi_StructBufType) Scc_Exi_StructBufUnion; /* PRQA S 0759 */ /* MD_Scc_0750_0759 */

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
SCC_VARNOINIT(uint8) Scc_Exi_SAScheduleTupleID;
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 16bit variables */
#define SCC_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SCC_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_VARNOINIT(Exi_EncodeWorkspaceType) Scc_Exi_EncWs;
SCC_VARNOINIT(Exi_DecodeWorkspaceType) Scc_Exi_DecWs;

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
SCC_VARNOINIT(Exi_ISO_paymentOptionType)   Scc_Exi_PaymentOption;
SCC_VARNOINIT(Exi_ISO_chargingSessionType) Scc_Exi_ChargingSession;
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

Scc_ReturnType (*Scc_ExiTx_Xyz_EncodeMessageFctPtr)(void);
Std_ReturnType (*Scc_ExiRx_Xyz_DecodeMessageFctPtr)(void);

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */
/* PRQA L:MACROS_FUNCTION_LIKE */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_Exi_Init
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Exi_Init(void)
{

  return;
}

/**********************************************************************************************************************
 *  Scc_Exi_StreamRequest
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_StreamRequest(void)
{
  Std_ReturnType RetVal;
  uint8_least    Counter = 0;
  uint16         RemLen;
  uint16         PacketLen;

  /* get the transmit buffer from the transport layer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  if ( BUFREQ_OK != Tl_ProvideTxBuffer((Scc_SocketType)Scc_V2GSocket,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr,
    (SCC_P2VARAPPLDATA(Scc_PbufType*))&Scc_ExiStreamTxPBufPtr, SCC_V2GTP_MAX_TX_LEN) )
  {
    return Scc_ReturnType_Pending;
  }

  /* initialize the exi encode workspace */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != Exi_InitEncodeWorkspace((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs,
    (SCC_P2VARAPPLDATA(uint8))&Scc_Exi_StructBuf[0],
    (SCC_P2VARAPPLDATA(IpBase_PbufType))Scc_ExiStreamTxPBufPtr, 0,
    Scc_Exi_EncWs.EncWs.StartWriteAtBytePos + Scc_Exi_EncWs.EncWs.CurrentStreamSegmentLen, FALSE) )
  {
    /* report the error to the application */
    Scc_ReportError(Scc_StackError_Exi);
    return Scc_ReturnType_NotOK;
  }

  /* try to encode the message and check if the encoding failed */
  RetVal = Exi_Encode((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs);
  if ( ( EXI_E_OK != RetVal ) && ( EXI_E_EOS != RetVal ))
  {
    /* report the error */
    Scc_ReportError(Scc_StackError_Exi);
    return Scc_ReturnType_NotOK;
  }

  /* finalize the exi stream and check if an error occured */
  if ( EXI_E_OK != Exi_FinalizeExiStream((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs) )
  {
    /* report the error to the application */
    Scc_ReportError(Scc_StackError_Exi);
    return Scc_ReturnType_NotOK;
  }

  /* copy the streaming status from the EXI workspace */
  if ( FALSE != Scc_Exi_EncWs.EncWs.StreamComplete )
  {
    Scc_TxStreamingActive = FALSE;
  }

  /* set the length of the tx data */
  Scc_TxDataSent += Scc_Exi_EncWs.EncWs.CurrentStreamSegmentLen;
  PacketLen = (uint16)Scc_Exi_EncWs.EncWs.CurrentStreamSegmentLen;
  RemLen = PacketLen;

  while ( 0 < RemLen )
  {
    /* set totLen to the packet length */
    Scc_ExiStreamTxPBufPtr[Counter].totLen = PacketLen;

    /* check if the packet is spread across more PBufs */
    if ( RemLen > Scc_ExiStreamTxPBufPtr[Counter].len )
    {
      RemLen -= Scc_ExiStreamTxPBufPtr[Counter].len;
      Counter++;
    }
    /* the remaining bytes are contained in this pbuf element */
    else
    {
      Scc_ExiStreamTxPBufPtr[Counter].len = RemLen;
      RemLen = 0;
    }
  }

  /* send the message and check if it failed */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != Tl_TransmitTo(Scc_V2GSocket, (SCC_P2VARAPPLDATA(Scc_PbufType))Scc_ExiStreamTxPBufPtr,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr) )
  {
    Scc_ReportError(Scc_StackError_TransportLayer);
    /* reset the length of the transmitted data */
    Scc_TxDataSent = 0;
    return Scc_ReturnType_NotOK;
  }

  /* provide sent data to application */
  {
    /* create and set the tx buffer pointer */
    Scc_TxRxBufferPointerType V2GRequest;
    V2GRequest.PbufPtr = Scc_ExiStreamTxPBufPtr;
    V2GRequest.FirstPart = FALSE;
    V2GRequest.StreamComplete = Scc_Exi_EncWs.EncWs.StreamComplete;
    /* provide the buffer to the application */
    Scc_Set_Core_V2GRequest(&V2GRequest);
  }

  /* reset the pbuf pointer */
  Scc_ExiStreamTxPBufPtr = (SCC_P2VARAPPLDATA(Scc_PbufType))NULL_PTR;

  /* check if streaming is finished */
  if ( FALSE == Scc_TxStreamingActive )
  {
    return Scc_ReturnType_OK;
  }
  /* streaming is still active */
  else
  {
    return Scc_ReturnType_Pending;
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  Scc_Exi_EncodeSupportedAppProtocolReq
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_EncodeSupportedAppProtocolReq(void)
{
  boolean                                              InitialAppProtocol = TRUE;
  uint8_least                                          Counter;
  uint16                                               lBufIdx = 0;
  Scc_SupportedSAPSchemasType                          SupportedSAPSchemas;
  SCC_P2VARNOINIT(Exi_SAP_supportedAppProtocolReqType) ExiSapMsgPtr;
  SCC_P2VARNOINIT(Exi_SAP_AppProtocolType)             AppProtocolPtr;

  /* initialize the exi workspace and check if it failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitEncodingWorkspace() )
  {
    return Scc_ReturnType_NotOK;
  }

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  SupportedSAPSchemas.Schema[SCC_SCHEMA_DIN-1]   = Scc_SAPSchemaIDs_DIN;
  SupportedSAPSchemas.Priority[SCC_SCHEMA_DIN-1] = SCC_SCHEMA_DIN;
#endif /* SCC_SCHEMA_DIN */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  SupportedSAPSchemas.Schema[SCC_SCHEMA_ISO-1]   = Scc_SAPSchemaIDs_ISO;
  SupportedSAPSchemas.Priority[SCC_SCHEMA_ISO-1] = SCC_SCHEMA_ISO;
#endif /* SCC_SCHEMA_ISO */
#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
  SupportedSAPSchemas.Schema[SCC_SCHEMA_ISO_ED2_CD-1]   = Scc_SAPSchemaIDs_ISO_Ed2_CD;
  SupportedSAPSchemas.Priority[SCC_SCHEMA_ISO_ED2_CD-1] = SCC_SCHEMA_ISO_ED2_CD;
#endif /* SCC_SCHEMA_ISO_ED2_CD */

#if ( defined Scc_Config_SupportedSAPSchemas ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  /* check if the application wants to reconfigure the transmitted schemas */
  Scc_Config_SupportedSAPSchemas(&SupportedSAPSchemas);
#endif /* Scc_Config_SupportedSAPSchemas */

  /* set the supportedAppProtocolReq as root element */
  Scc_Exi_EncWs.InputData.RootElementId = EXI_SAP_SUPPORTED_APP_PROTOCOL_REQ_TYPE;

  /* fill data */

  /* set the buffer for the SAP message */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  ExiSapMsgPtr = (SCC_P2VARNOINIT(Exi_SAP_supportedAppProtocolReqType))&Scc_Exi_StructBuf[lBufIdx];
  lBufIdx += sizeof(Exi_SAP_supportedAppProtocolReqType);

  /* set the first app protocol buffer pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  ExiSapMsgPtr->AppProtocol = (SCC_P2VARAPPLDATA(Exi_SAP_AppProtocolType))&Scc_Exi_StructBuf[lBufIdx];

  /* get the starting pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  AppProtocolPtr = (SCC_P2VARAPPLDATA(Exi_SAP_AppProtocolType))&Scc_Exi_StructBuf[lBufIdx];
  lBufIdx += sizeof(Exi_SAP_AppProtocolType);

  for ( Counter = 0; Counter < Scc_SAPSchemaIDs_NumOfSupportedSchemas; Counter++ )
  {
    /* check if no more schemas shall be sent */
    if ( Scc_SAPSchemaIDs_None == SupportedSAPSchemas.Schema[Counter] )
    {
      /* check if no schema was chosen (illegal) */
      if ( 0 == Counter )
      {
        /* report the error to the application */
        Scc_ReportError(Scc_StackError_InvalidTxParameter);
        return Scc_ReturnType_NotOK;
      }
      break;
    }
    /* check if the priority has an illegal value */
    if ( 0 == SupportedSAPSchemas.Priority[Counter] )
    {
      /* report the error to the application */
      Scc_ReportError(Scc_StackError_InvalidTxParameter);
      return Scc_ReturnType_NotOK;
    }

    /* check if this is not the first app protocol */
    if ( FALSE == InitialAppProtocol )
    {
      /* reference the buffer for the next AppProtocol element */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
      AppProtocolPtr->NextAppProtocolPtr = (SCC_P2VARAPPLDATA(Exi_SAP_AppProtocolType))&Scc_Exi_StructBuf[lBufIdx];
      lBufIdx += sizeof(Exi_SAP_AppProtocolType);
      /* set the AppProtocolPtr to the second element */
      AppProtocolPtr = AppProtocolPtr->NextAppProtocolPtr;
    }
    /* use the existing buffer */
    else
    {
      InitialAppProtocol = FALSE;
    }
    /* set the buffer for the protocol namespace */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AppProtocolPtr->ProtocolNamespace = (SCC_P2VARAPPLDATA(Exi_SAP_protocolNamespaceType))&Scc_Exi_StructBuf[lBufIdx];
    lBufIdx += sizeof(Exi_SAP_protocolNamespaceType);

    switch ( SupportedSAPSchemas.Schema[Counter] )
    {
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
    case Scc_SAPSchemaIDs_ISO:
      /* copy the namespace */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
      IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType))&AppProtocolPtr->ProtocolNamespace->Buffer[0],
        (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType))SCC_EXI_SAP_ISO_FDIS_PROTOCOL_NAMESPACE,
        SCC_EXI_SAP_ISO_FDIS_PROTOCOL_NAMESPACE_LEN);
      /* copy the length of the namespace */
      AppProtocolPtr->ProtocolNamespace->Length = SCC_EXI_SAP_ISO_FDIS_PROTOCOL_NAMESPACE_LEN;

      /* VersionNumberMajor */
      AppProtocolPtr->VersionNumberMajor = (uint32)SCC_EXI_SAP_ISO_FDIS_VERSION_NUMBER_MAJOR;
      /* VersionNumberMinor */
      AppProtocolPtr->VersionNumberMinor = (uint32)SCC_EXI_SAP_ISO_FDIS_VERSION_NUMBER_MINOR;
      /* SchemaID */
      AppProtocolPtr->SchemaID = (uint8)Scc_SAPSchemaIDs_ISO;
      /* Priority */
      AppProtocolPtr->Priority = SupportedSAPSchemas.Priority[Counter];
      break;
#endif /* SCC_SCHEMA_ISO */
      
#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
    case Scc_SAPSchemaIDs_ISO_Ed2_CD:
      /* copy the namespace */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
      IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType))&AppProtocolPtr->ProtocolNamespace->Buffer[0],
        (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType))SCC_EXI_SAP_ISO_ED2_CD_PROTOCOL_NAMESPACE,
        SCC_EXI_SAP_ISO_ED2_CD_PROTOCOL_NAMESPACE_LEN);
      /* copy the length of the namespace */
      AppProtocolPtr->ProtocolNamespace->Length = SCC_EXI_SAP_ISO_ED2_CD_PROTOCOL_NAMESPACE_LEN;

      /* VersionNumberMajor */
      AppProtocolPtr->VersionNumberMajor = (uint32)SCC_EXI_SAP_ISO_ED2_CD_VERSION_NUMBER_MAJOR;
      /* VersionNumberMinor */
      AppProtocolPtr->VersionNumberMinor = (uint32)SCC_EXI_SAP_ISO_ED2_CD_VERSION_NUMBER_MINOR;
      /* SchemaID */
      AppProtocolPtr->SchemaID = (uint8)Scc_SAPSchemaIDs_ISO_Ed2_CD;
      /* Priority */
      AppProtocolPtr->Priority = SupportedSAPSchemas.Priority[Counter];
      break;
#endif /* SCC_SCHEMA_ISO_ED2_CD */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
    case Scc_SAPSchemaIDs_DIN:
      /* copy the namespace */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
      IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType))&AppProtocolPtr->ProtocolNamespace->Buffer[0],
        (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType))SCC_EXI_SAP_DIN_PROTOCOL_NAMESPACE,
        SCC_EXI_SAP_DIN_PROTOCOL_NAMESPACE_LEN);
      /* copy the length of the namespace */
      AppProtocolPtr->ProtocolNamespace->Length = SCC_EXI_SAP_DIN_PROTOCOL_NAMESPACE_LEN;

      /* VersionNumberMajor */
      AppProtocolPtr->VersionNumberMajor = (uint32)SCC_EXI_SAP_DIN_VERSION_NUMBER_MAJOR;
      /* VersionNumberMinor */
      AppProtocolPtr->VersionNumberMinor = (uint32)SCC_EXI_SAP_DIN_VERSION_NUMBER_MINOR;
      /* SchemaID */
      AppProtocolPtr->SchemaID = (uint8)Scc_SAPSchemaIDs_DIN;
      /* Priority */
      AppProtocolPtr->Priority = SupportedSAPSchemas.Priority[Counter];
      break;
#endif /* SCC_SCHEMA_DIN */

    default:
      /* report the error to the application */
      Scc_ReportError(Scc_StackError_InvalidTxParameter);
      return Scc_ReturnType_NotOK;
    }
  }

  /* set the last nextappprotocolptr to NULL */
  AppProtocolPtr->NextAppProtocolPtr = (SCC_P2VARAPPLDATA(Exi_SAP_AppProtocolType))NULL_PTR;

  /* start the timeout counter */
  Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_General_SupportedAppProtocolMessageTimeout;

  /* try to finalize and send the message */
  return Scc_Exi_EncodeAndTransmitExiStream(SCC_V2GTP_HDR_PAYLOAD_TYPE_SAP);
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_Exi_DecodeSupportedAppProtocolRes
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_DecodeSupportedAppProtocolRes(void)
{
  /* set the exi message pointer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  SCC_P2VARNOINIT(Exi_SAP_supportedAppProtocolResType) ExiSapMsgPtr =
    (SCC_P2VARNOINIT(Exi_SAP_supportedAppProtocolResType)) &Scc_Exi_StructBuf[0];

  /* initialize the exi workspace and check if the workspace initialization failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitDecodingWorkspace() )
  {
    return (Std_ReturnType)E_NOT_OK;
  }
  /* set the decode information */
  Scc_Exi_DecWs.OutputData.RootElementId = EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE;
  Scc_Exi_DecWs.OutputData.SchemaSetId = EXI_SCHEMA_SET_SAP_TYPE;
  /* try to decode the message and check if it failed */
  if ( (Std_ReturnType)E_OK != Exi_Decode(&Scc_Exi_DecWs) )
  {
    /* report the error */
    Scc_ReportErrorAndStatus(Scc_StackError_Exi, Scc_MsgStatus_SupportedAppProtocol_Failed);
#if ( defined SCC_DEM_EXI )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_EXI);
#endif /* SCC_DEM_EXI */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_EXI )
  /* if it was successful */
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_EXI);
  }
#endif /* SCC_DEM_EXI */

  /* check if the response is negative */
  if ( EXI_SAP_RESPONSE_CODE_TYPE_FAILED_NO_NEGOTIATION <= ExiSapMsgPtr->ResponseCode )
  {
    /* report the negative response code */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_SupportedAppProtocol_Failed);
    /* provide the response code to the application */
    Scc_Set_Core_SAPResponseCode(ExiSapMsgPtr->ResponseCode); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
#if ( defined SCC_DEM_SAP_NO_NEGOTIATION )
    /* set DEM event */
    Scc_DemReportErrorStatusFailed(SCC_DEM_SAP_NO_NEGOTIATION);
#endif /* SCC_DEM_SAP_NO_NEGOTIATION */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_SAP_NO_NEGOTIATION )
  else
  {
    /* set DEM event */
    Scc_DemReportErrorStatusPassed(SCC_DEM_SAP_NO_NEGOTIATION);
  }
#endif /* SCC_DEM_SAP_NO_NEGOTIATION */

  /* --- evaluate data --- */

  /* SchemaID */
  if ( 0 == ExiSapMsgPtr->SchemaIDFlag )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_SupportedAppProtocol_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }
  Scc_SAPSchemaID = (Scc_SAPSchemaIDType)ExiSapMsgPtr->SchemaID;
  Scc_Set_Core_SAPSchemaID(Scc_SAPSchemaID); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* set the function pointers depending on the selected Schema */
  switch ( Scc_SAPSchemaID )
  {
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  case Scc_SAPSchemaIDs_ISO:
    Scc_ExiTx_Xyz_EncodeMessageFctPtr = Scc_ExiTx_ISO_EncodeMessage;
    Scc_ExiRx_Xyz_DecodeMessageFctPtr = Scc_ExiRx_ISO_DecodeMessage;
    break;
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
  case Scc_SAPSchemaIDs_ISO_Ed2_CD:
    Scc_ExiTx_Xyz_EncodeMessageFctPtr = Scc_ExiTx_ISO_Ed2_CD_EncodeMessage;
    Scc_ExiRx_Xyz_DecodeMessageFctPtr = Scc_ExiRx_ISO_Ed2_CD_DecodeMessage;
    break;
#endif /* SCC_SCHEMA_ISO_ED2_CD */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  case Scc_SAPSchemaIDs_DIN:
    Scc_ExiTx_Xyz_EncodeMessageFctPtr = Scc_ExiTx_DIN_EncodeMessage;
    Scc_ExiRx_Xyz_DecodeMessageFctPtr = Scc_ExiRx_DIN_DecodeMessage;
    /* reset the SessionID */
    Scc_SessionIDNvm[0] = 0x01;
    Scc_SessionIDNvm[1] = 0x00;
    break;
#endif /* SCC_SCHEMA_DIN */

  default:
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_SupportedAppProtocol_Failed);
    break;
  }

  /* ResponseCode */
  Scc_Set_Core_SAPResponseCode(ExiSapMsgPtr->ResponseCode); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */

  /* set the new state and report the success */
  Scc_State = Scc_State_SAPComplete;
  Scc_ReportSuccessAndStatus(Scc_MsgStatus_SupportedAppProtocol_OK);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_Exi_InitEncodingWorkspace
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_InitEncodingWorkspace(void)
{
  /* get the transmit buffer from the transport layer */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  if ( BUFREQ_OK != Tl_ProvideTxBuffer((TcpIp_SockHndType)Scc_V2GSocket,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr,
    (SCC_P2VARAPPLDATA(Scc_PbufType*))&Scc_ExiStreamTxPBufPtr, SCC_V2GTP_MAX_TX_LEN) )
  {
    /* report the error */
    Scc_ReportError(Scc_StackError_TransportLayer);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* initialize the exi encode workspace */ /* PRQA S 0310,3305 3 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != Exi_InitEncodeWorkspace((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs,
    (SCC_P2VARAPPLDATA(uint8))&Scc_Exi_StructBuf[0],
    (SCC_P2VARAPPLDATA(IpBase_PbufType))Scc_ExiStreamTxPBufPtr, SCC_V2GTP_HDR_LEN, 0, TRUE) )
  {
    /* report the error to the application */
    Scc_ReportError(Scc_StackError_Exi);
    return (Std_ReturnType)E_NOT_OK;
  }

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_Exi_InitDecodingWorkspace
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_InitDecodingWorkspace(void)
{
  /* initialize the decoding workspace */
  if ( (Std_ReturnType)E_OK != Exi_InitDecodeWorkspace((SCC_P2VARAPPLDATA(Exi_DecodeWorkspaceType))&Scc_Exi_DecWs,
    (SCC_P2VARAPPLDATA(IpBase_PbufType))&Scc_ExiStreamRxPBuf[0], (SCC_P2VARAPPLDATA(uint8))&Scc_Exi_StructBuf[0],
    (uint16)SCC_EXI_STRUCT_BUF_LEN, SCC_V2GTP_HDR_LEN) )
  {
    /* report the error to the application */
    Scc_ReportError(Scc_StackError_Exi);
    return (Std_ReturnType)E_NOT_OK;
  }

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_Exi_EncodeAndTransmitExiStream
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_EncodeAndTransmitExiStream(uint16 PayloadType)
{
  uint8_least Counter = 0;
  uint16      RemLen;
  uint8       Buf[8];

  /* try to encode the message and check if the encoding failed */
  if ( EXI_E_OK != Exi_Encode((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs) )
  {
    /* report the error */
    Scc_ReportError(Scc_StackError_Exi);
    return Scc_ReturnType_NotOK;
  }

  /* finalize the exi stream and check if an error occured */
  if ( EXI_E_OK != Exi_FinalizeExiStream((SCC_P2VARAPPLDATA(Exi_EncodeWorkspaceType))&Scc_Exi_EncWs) )
  {
    /* report the error to the application */
    Scc_ReportError(Scc_StackError_Exi);
    return Scc_ReturnType_NotOK;
  }

  /* copy the streaming status from the EXI workspace */
  if ( FALSE == Scc_Exi_EncWs.EncWs.StreamComplete )
  {
    Scc_TxStreamingActive = TRUE;
  }

  /* set the V2GTP header */
  /* set the version info */
  Buf[Scc_V2GTPOffsets_Version]         = (uint8)SCC_V2G_PROT_VER;
  Buf[Scc_V2GTPOffsets_InverseVersion]  = (uint8)((~SCC_V2G_PROT_VER) & 0xFF);
  /* set the payload type */
  Buf[Scc_V2GTPOffsets_PayloadType]     = (uint8)(( PayloadType & 0xFF00u ) >> 8u );
  Buf[Scc_V2GTPOffsets_PayloadType+1]   = (uint8) ( PayloadType & 0x00FFu );
  /* set the payload length */
  Buf[Scc_V2GTPOffsets_PayloadLength]   =
    (uint8)(( Scc_Exi_EncWs.EncWs.TotalStreamLength & 0xFF000000u ) >> 24u );
  Buf[Scc_V2GTPOffsets_PayloadLength+1] =
    (uint8)(( Scc_Exi_EncWs.EncWs.TotalStreamLength & 0x00FF0000u ) >> 16u );
  Buf[Scc_V2GTPOffsets_PayloadLength+2] =
    (uint8)(( Scc_Exi_EncWs.EncWs.TotalStreamLength & 0x0000FF00u ) >> 8u );
  Buf[Scc_V2GTPOffsets_PayloadLength+3] =
    (uint8) ( Scc_Exi_EncWs.EncWs.TotalStreamLength & 0x000000FFu );

  /* copy the V2GTP header to the PBuf */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != IpBase_CopyString2PbufAt((SCC_P2CONSTAPPLDATA(uint8))&Buf[0], sizeof(Buf),
    (SCC_P2VARAPPLDATA(IpBase_PbufType))Scc_ExiStreamTxPBufPtr, 0) )
  {
    Scc_CallDetReportError(SCC_API_ID_V_EXI_FIN_EXI_STREAM, SCC_DET_EXI)
  }

  /* set the length of the tx data */
  Scc_TxDataSent = Scc_Exi_EncWs.EncWs.CurrentStreamSegmentLen + SCC_V2GTP_HDR_LEN;
  RemLen = (uint16)Scc_TxDataSent;

  while ( 0 < RemLen )
  {
    /* set totLen to the packet length */
    Scc_ExiStreamTxPBufPtr[Counter].totLen = Scc_TxDataSent;

    /* check if the packet is spread across more PBufs */
    if ( RemLen > Scc_ExiStreamTxPBufPtr[Counter].len )
    {
      RemLen -= Scc_ExiStreamTxPBufPtr[Counter].len;
      Counter++;
    }
    /* the remaining bytes are contained in this pbuf element */
    else
    {
      Scc_ExiStreamTxPBufPtr[Counter].len = RemLen;
      RemLen = 0;
    }
  }

  /* send the message and check if it failed */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  if ( (Std_ReturnType)E_OK != Tl_TransmitTo(Scc_V2GSocket, (SCC_P2VARAPPLDATA(Scc_PbufType))Scc_ExiStreamTxPBufPtr,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr) )
  {
    Scc_ReportError(Scc_StackError_TransportLayer);
    /* reset the length of the transmitted data */
    Scc_TxDataSent = 0;
    return Scc_ReturnType_NotOK;
  }

  /* provide sent data to application */
  {
    /* create and set the tx buffer pointer */
    Scc_TxRxBufferPointerType V2GRequest;
    V2GRequest.PbufPtr = Scc_ExiStreamTxPBufPtr;
    V2GRequest.FirstPart = TRUE;
    V2GRequest.StreamComplete = Scc_Exi_EncWs.EncWs.StreamComplete;
    /* provide the buffer to the application */
    Scc_Set_Core_V2GRequest(&V2GRequest);
  }

  /* reset the pbuf pointer */
  Scc_ExiStreamTxPBufPtr = (SCC_P2VARAPPLDATA(Scc_PbufType))NULL_PTR;

  /* check if the whole message did not fit into the first packet */
  if ( TRUE == Scc_TxStreamingActive )
  {
    return Scc_ReturnType_Pending;
  }

  return Scc_ReturnType_OK;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

/**********************************************************************************************************************
 *  END OF FILE: Scc_ExiTx_ISO.c
 *********************************************************************************************************************/
