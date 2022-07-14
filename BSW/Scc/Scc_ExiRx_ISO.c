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
/**        \file  Scc_ExiRx_ISO.c
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

#define SCC_EXIRX_ISO_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )

#include "Scc_Exi.h"
#include "Scc.h"
#include "Scc_Priv.h"
#include "Scc_Lcfg.h"
#include SCC_IF_CFG_H

#if ( defined SCC_PROD_ERROR_DETECT ) && ( SCC_PROD_ERROR_DETECT == STD_ON )
#include "Dem.h"
#endif /* SCC_PROD_ERROR_DETECT */
#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
#include "Exi.h"
#include "IpBase.h"
#include "NvM.h"

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */

/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/
/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
STATIC SCC_P2VARNOINIT(Exi_ISO_V2G_MessageType) Scc_ExiRx_ISO_MsgPtr;

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_SessionSetupRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ServiceDiscoveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ServiceDetailRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PaymentServiceSelectionRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_AuthorizationRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ChargeParameterDiscoveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PowerDeliveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_SessionStopRes(void);

/* AC */
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ChargingStatusRes(void);
#endif /* SCC_CHARGING_AC */

/* DC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_CableCheckRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PreChargeRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_CurrentDemandRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_WeldingDetectionRes(void);
#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
SCC_STATICFUNCCODE(void) Scc_ExiRx_ISO_ReadEVSEStatusAC(SCC_P2CONSTNOINIT(Exi_ISO_AC_EVSEStatusType) EVSEStatusPtr);
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(void) Scc_ExiRx_ISO_ReadEVSEStatusDC(SCC_P2CONSTNOINIT(Exi_ISO_DC_EVSEStatusType) EVSEStatusPtr);
#endif /* SCC_CHARGING_DC */

/* XmlSecurity */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_Init
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_ExiRx_ISO_Init(void)
{
  /* set the message pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiRx_ISO_MsgPtr = (SCC_P2VARNOINIT(Exi_ISO_V2G_MessageType))&Scc_Exi_StructBuf[0];

#if ( defined SCC_DISABLE_XML_SEC_RX ) && ( SCC_DISABLE_XML_SEC_RX == STD_ON )
#else
  /* initialize the XmlSecurity workspace */
  if ( XmlSec_RetVal_OK != XmlSecurity_InitSigValWorkspace(&Scc_Exi_XmlSecSigValWs,
    Scc_ExiRx_ISO_XmlSecDereference, Scc_XmlSecGetPublicKey,
#if ( defined XMLSECURITY_ENABLE_KEY_INFO_SUPPORT ) && ( XMLSECURITY_ENABLE_KEY_INFO_SUPPORT == STD_ON )
    (XmlSecurity_GetIssuerSerialCertFctType)NULL_PTR, (XmlSecurity_ValidateCertFctType)NULL_PTR,
#endif /* XMLSECURITY_ENABLE_KEY_INFO_SUPPORT */
    &Scc_CryptoWs, &Scc_SHA256Ws) )
  {
    /* since this should not happen, issue a DET */
    Scc_CallDetReportError(SCC_API_ID_V_CERTIFICATE_INSTALL_RES, SCC_DET_XML_SEC)
  }
#endif /* SCC_DISABLE_XML_SEC_RX */

  return;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_DecodeMessage
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_DecodeMessage(void)
{
  /* initialize the exi workspace and check if the workspace initialization failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitDecodingWorkspace() )
  {
    return (Std_ReturnType)E_NOT_OK;
  }

  /* set the decode information */
  Scc_Exi_DecWs.OutputData.RootElementId = EXI_ISO_V2G_MESSAGE_TYPE;
  Scc_Exi_DecWs.OutputData.SchemaSetId = EXI_SCHEMA_SET_ISO_TYPE;
  /* try to decode the message and check if it failed */
  if ( (Std_ReturnType)E_OK != Exi_Decode(&Scc_Exi_DecWs) )
  {
    /* report the error */
    Scc_ReportError(Scc_StackError_Exi);
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

  /* only check the SessionID starting from ServiceDiscovery */
  if ( Scc_MsgTrig_SessionSetup < Scc_MsgTrig )
  {
    /* compare the length of the stored and received SessionID */
    if ( Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Length == Scc_SessionIDNvm[0] )
    {
      /* if the length is the same, check if the content is the same, too */
      if ( IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Buffer[0],
        &Scc_SessionIDNvm[1], Scc_SessionIDNvm[0] ))
      {
        /* SessionID is not equal, report the error */
        Scc_ReportError(Scc_StackError_InvalidRxParameter);
        return (Std_ReturnType)E_NOT_OK;
      }
    }
    /* length is not the same */
    else
    {
      /* report the error */
      Scc_ReportError(Scc_StackError_InvalidRxParameter);
      return (Std_ReturnType)E_NOT_OK;
    }
    /* Notification */
    Scc_Set_ISO_Notification(Scc_ExiRx_ISO_MsgPtr->Header->Notification, Scc_ExiRx_ISO_MsgPtr->Header->NotificationFlag);
  }

  switch ( Scc_MsgTrig )
  {
    /* handle an incoming session setup response message */
  case Scc_MsgTrig_SessionSetup:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_SessionSetupRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_SessionSetup_OK);
      /* the session has been successfully connected */
      Scc_State = Scc_State_Connected;
    }
    break;

    /* handle an incoming service discovery response message */
  case Scc_MsgTrig_ServiceDiscovery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_ServiceDiscoveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ServiceDiscovery_OK);
    }
    break;

    /* handle an incoming service detail response message */
  case Scc_MsgTrig_ServiceDetail:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_ServiceDetailRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ServiceDetail_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming service payment selection response message */
  case Scc_MsgTrig_PaymentServiceSelection:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_PaymentServiceSelectionRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PaymentServiceSelection_OK);
    }
    break;

    /* handle an incoming contract authentication response message */
  case Scc_MsgTrig_Authorization:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_AuthorizationRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_Authorization_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming charge parameter discovery response message */
  case Scc_MsgTrig_ChargeParameterDiscovery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_ChargeParameterDiscoveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ChargeParameterDiscovery_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming power delivery response message */
  case Scc_MsgTrig_PowerDelivery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_PowerDeliveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PowerDelivery_OK);
    }
    break;

    /* handle an incoming session stop response message */
  case Scc_MsgTrig_SessionStop:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_SessionStopRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_SessionStop_OK);
    }
    /* the session is now disconnected */
    Scc_State = Scc_State_Disconnected;
    break;

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
    /* handle an incoming charging status response message */
  case Scc_MsgTrig_ChargingStatus:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_ChargingStatusRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ChargingStatus_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

    /* handle an incoming cable check response message */
  case Scc_MsgTrig_CableCheck:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_CableCheckRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_CableCheck_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming pre charge response message */
  case Scc_MsgTrig_PreCharge:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_PreChargeRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PreCharge_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming current demand response message */
  case Scc_MsgTrig_CurrentDemand:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_CurrentDemandRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_CurrentDemand_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming welding detection response message */
  case Scc_MsgTrig_WeldingDetection:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_ISO_WeldingDetectionRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_WeldingDetection_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

#endif /* SCC_CHARGING_DC */

  default:
    /* invalid state */
    return (Std_ReturnType)E_NOT_OK;
  }

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_SessionSetupRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_SessionSetupRes(void)
{
  Exi_ISO_responseCodeType responseCode;

  SCC_P2CONSTNOINIT(Exi_ISO_SessionSetupResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_SessionSetupResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_SESSION_SETUP_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_SessionSetup_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_SessionSetup_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ResponseCode */
  responseCode = BodyPtr->ResponseCode;

  /* SessionID */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_OK_CERTIFICATE_EXPIRES_SOON > responseCode )
  {
    /* check for a positive response code */
    if ( EXI_ISO_RESPONSE_CODE_TYPE_OK == responseCode )
    {
      /* check if the lengths do not match */
      if ( Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Length != Scc_SessionIDNvm[0] )
      {
        responseCode = EXI_ISO_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED;
      }
      /* if the lengths match */
      else
      {
        /* check if the SessionIDs match */
        if ( IPBASE_CMP_EQUAL == IpBase_StrCmpLen(&Scc_SessionIDNvm[1],
          &Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Buffer[0], Scc_SessionIDNvm[0]) )
        {
          responseCode = EXI_ISO_RESPONSE_CODE_TYPE_OK_OLD_SESSION_JOINED;
        }
        /* if they do not match */
        else
        {
          responseCode = EXI_ISO_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED;
        }
      }
    }
    /* check if the EVSE has opened a new session */
    if ( EXI_ISO_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED == responseCode )
    {
      /* get the new length of the SessionID */
      Scc_SessionIDNvm[0] = (uint8) Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Length;
      /* copy the SessionID */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
      IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType)) &Scc_SessionIDNvm[1],
        (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType)) &Scc_ExiRx_ISO_MsgPtr->Header->SessionID->Buffer[0], Scc_SessionIDNvm[0]);
      /* write the SessionID to the NvM */
      (void) NvM_SetRamBlockStatus(SCC_SESSION_ID_NVM_BLOCK, TRUE);
    }
  }

  /* SessionID */
  Scc_Set_ISO_SessionID(Scc_ExiRx_ISO_MsgPtr->Header->SessionID);

  /* EVSEID */
  Scc_Set_ISO_EVSEID(BodyPtr->EVSEID);

  /* EVSETimeStamp */
  Scc_Set_ISO_EVSETimeStamp(BodyPtr->EVSETimeStamp, BodyPtr->EVSETimeStampFlag);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(responseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ServiceDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ServiceDiscoveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_ServiceDiscoveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_ServiceDiscoveryResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_SERVICE_DISCOVERY_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_ServiceDiscovery_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ServiceDiscovery_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* PaymentOptionList */
  Scc_Set_ISO_PaymentOptionList(BodyPtr->PaymentOptionList);

  /* ChargeService */
  Scc_Set_ISO_ChargeService(BodyPtr->ChargeService);

  /* ServiceList */
  Scc_Set_ISO_ServiceList(BodyPtr->ServiceList, BodyPtr->ServiceListFlag);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ServiceDetailRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ServiceDetailRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_ServiceDetailResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_ServiceDetailResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_SERVICE_DETAIL_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_ServiceDetail_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ServiceDetail_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ServiceID */
  Scc_Set_ISO_ServiceIDTx(BodyPtr->ServiceID);

  /* ServiceParameterList */
  Scc_Set_ISO_ServiceParameterList(BodyPtr->ServiceParameterList, BodyPtr->ServiceParameterListFlag);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_PaymentServiceSelectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PaymentServiceSelectionRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_PaymentServiceSelectionResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_PaymentServiceSelectionResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_PAYMENT_SERVICE_SELECTION_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_PaymentServiceSelection_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PaymentServiceSelection_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_AuthorizationRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_AuthorizationRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_AuthorizationResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_AuthorizationResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_AUTHORIZATION_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_Authorization_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_Authorization_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* EVSEProcessing */
  Scc_Set_ISO_EVSEProcessing(BodyPtr->EVSEProcessing);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ChargeParameterDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ChargeParameterDiscoveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_ChargeParameterDiscoveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_ChargeParameterDiscoveryResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  SCC_P2CONSTNOINIT(Exi_ISO_AC_EVSEChargeParameterType) AC_EVSEChargeParameterPtr;
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  SCC_P2CONSTNOINIT(Exi_ISO_DC_EVSEChargeParameterType) DC_EVSEChargeParameterPtr;
#endif /* SCC_CHARGING_DC */

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_CHARGE_PARAMETER_DISCOVERY_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* EVSEProcessing */
  Scc_Set_ISO_EVSEProcessing(BodyPtr->EVSEProcessing);

  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_ISO_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
  {
    {
      /* check if at least one SASchedule exists */
      if ( 0 == BodyPtr->SASchedulesFlag )
      {
        /* report the invalid parameter as stack error */
        Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
        return (Std_ReturnType)E_NOT_OK;
      }
    }

    Scc_Set_ISO_SAScheduleList((SCC_P2CONSTAPPLDATA(Exi_ISO_SAScheduleListType))BodyPtr->SASchedules);
  }

#if ( SCC_ENABLE_ONGOING_CALLBACKS ) && ( SCC_ENABLE_ONGOING_CALLBACKS == STD_ON )
#else
  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_ISO_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
#endif
  {
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
    /* check if AC charging was selected */
    if (   ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode )
        || ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode ))
#endif /* SCC_CHARGING_DC */
    {
      /* check if the EVSEChargeParameter have the correct type */
      if ( EXI_ISO_AC_EVSECHARGE_PARAMETER_TYPE != BodyPtr->EVSEChargeParameterElementId )
      {
        Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
        return (Std_ReturnType)E_NOT_OK;
      }
      /* set the pointer to the AC_EVSEChargeParameterPtr */
      AC_EVSEChargeParameterPtr = (SCC_P2VARAPPLDATA(Exi_ISO_AC_EVSEChargeParameterType))BodyPtr->EVSEChargeParameter;
      /* AC_EVSEChargeParameter -> AC_EVSEStatus */
      Scc_ExiRx_ISO_ReadEVSEStatusAC(AC_EVSEChargeParameterPtr->AC_EVSEStatus);
      /* AC_EVSEChargeParameter -> EVSENominalVoltage */
      Scc_Set_ISO_AC_EVSENominalVoltage(AC_EVSEChargeParameterPtr->EVSENominalVoltage);
      /* AC_EVSEChargeParameter -> EVSEMaxCurrent */
      Scc_Set_ISO_AC_EVSEMaxCurrent(AC_EVSEChargeParameterPtr->EVSEMaxCurrent, 1);
    }
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
    /* else DC charging was selected */
    else
#endif /* SCC_CHARGING_AC */
    {
      /* check if the EVSEChargeParameter have the correct type */
      if ( EXI_ISO_DC_EVSECHARGE_PARAMETER_TYPE != BodyPtr->EVSEChargeParameterElementId )
      {
        Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
        return (Std_ReturnType)E_NOT_OK;
      }
      /* set the pointer to the DC_EVSEChargeParameterPtr */
      DC_EVSEChargeParameterPtr = (SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEChargeParameterType))BodyPtr->EVSEChargeParameter;
      /* DC_EVSEChargeParameter -> DC_EVSEStatus */
      Scc_ExiRx_ISO_ReadEVSEStatusDC(DC_EVSEChargeParameterPtr->DC_EVSEStatus);
      /* DC_EVSEChargeParameter -> EVSEMaximumCurrentLimit */
      Scc_Set_ISO_DC_EVSEMaximumCurrentLimit(DC_EVSEChargeParameterPtr->EVSEMaximumCurrentLimit, 1);
      /* DC_EVSEChargeParameter -> EVSEMaximumPowerLimit */
      Scc_Set_ISO_DC_EVSEMaximumPowerLimit(DC_EVSEChargeParameterPtr->EVSEMaximumPowerLimit, 1);
      /* DC_EVSEChargeParameter -> EVSEMaximumVoltageLimit */
      Scc_Set_ISO_DC_EVSEMaximumVoltageLimit(DC_EVSEChargeParameterPtr->EVSEMaximumVoltageLimit, 1);
      /* DC_EVSEChargeParameter -> EVSEMinimumCurrentLimit */
      Scc_Set_ISO_DC_EVSEMinimumCurrentLimit(DC_EVSEChargeParameterPtr->EVSEMinimumCurrentLimit);
      /* DC_EVSEChargeParameter -> EVSEMinimumVoltageLimit */
      Scc_Set_ISO_DC_EVSEMinimumVoltageLimit(DC_EVSEChargeParameterPtr->EVSEMinimumVoltageLimit);
      /* DC_EVSEChargeParameter -> EVSECurrentRegulationTolerance */
      Scc_Set_ISO_DC_EVSECurrentRegulationTolerance(DC_EVSEChargeParameterPtr->EVSECurrentRegulationTolerance,
        DC_EVSEChargeParameterPtr->EVSECurrentRegulationToleranceFlag);
      /* DC_EVSEChargeParameter -> EVSEPeakCurrentRipple */
      Scc_Set_ISO_DC_EVSEPeakCurrentRipple(DC_EVSEChargeParameterPtr->EVSEPeakCurrentRipple);
      /* DC_EVSEChargeParameter -> EVSEEnergyToBeDelivered */
      Scc_Set_ISO_DC_EVSEEnergyToBeDelivered(DC_EVSEChargeParameterPtr->EVSEEnergyToBeDelivered,
        DC_EVSEChargeParameterPtr->EVSEEnergyToBeDeliveredFlag);
    }
#endif /* SCC_CHARGING_DC */
  }

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_PowerDeliveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PowerDeliveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_PowerDeliveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_PowerDeliveryResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_POWER_DELIVERY_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_PowerDelivery_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PowerDelivery_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  /* check if AC charging was selected */
  if (   ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode )
      || ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode ))
#endif /* SCC_CHARGING_DC */
  {
    /* check if the EVSEStatus contains an AC_EVSEStatus */
    if ( EXI_ISO_AC_EVSESTATUS_TYPE != BodyPtr->EVSEStatusElementId )
    {
      Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_PowerDelivery_Failed);
      return (Std_ReturnType)E_NOT_OK;
    }
    /* AC_EVSEStatus */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    Scc_ExiRx_ISO_ReadEVSEStatusAC((SCC_P2VARAPPLDATA(Exi_ISO_AC_EVSEStatusType))BodyPtr->EVSEStatus);
  }
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  /* DC charging was selected */
  else
#endif /* SCC_CHARGING_AC */
  {
    /* check if the EVSEStatus contains an DC_EVSEStatus */
    if ( EXI_ISO_DC_EVSESTATUS_TYPE != BodyPtr->EVSEStatusElementId )
    {
      Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_PowerDelivery_Failed);
      return (Std_ReturnType)E_NOT_OK;
    }
    /* DC_EVSEStatus */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
    Scc_ExiRx_ISO_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEStatusType))BodyPtr->EVSEStatus);
  }
#endif /* SCC_CHARGING_DC */

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_SessionStopRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_SessionStopRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_SessionStopResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_SessionStopResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_SESSION_STOP_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_SessionStop_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* reset the SessionID if the session was terminated */
  if ( EXI_ISO_CHARGING_SESSION_TYPE_TERMINATE == Scc_Exi_ChargingSession )
  {
    /* reset the SessionID */
    Scc_SessionIDNvm[0] = 0x01;
    Scc_SessionIDNvm[1] = 0x00;

    /* set the flag for the NvM to copy the new SessionID */
    (void) NvM_SetRamBlockStatus(SCC_SESSION_ID_NVM_BLOCK, TRUE);
  }

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_SessionStop_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ChargingStatusRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_ChargingStatusRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_ChargingStatusResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_ChargingStatusResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_CHARGING_STATUS_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_ChargingStatus_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ChargingStatus_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* EVSEID */
  Scc_Set_ISO_EVSEID((BodyPtr)->EVSEID);

  /* SAScheduleTupleID */
  if ( BodyPtr->SAScheduleTupleID != Scc_Exi_SAScheduleTupleID )
  {
    /* the received SAScheduleTupleID is different from the one which was sent to the EVSE in the PowerDeliveryReq */
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargingStatus_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* EVSEMaxCurrent */
  if ( EXI_ISO_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT == Scc_Exi_PaymentOption )
  {
    Scc_Set_ISO_AC_EVSEMaxCurrent(BodyPtr->EVSEMaxCurrent, BodyPtr->EVSEMaxCurrentFlag);
  }

  /* MeterInfo */
  Scc_Set_ISO_MeterInfo(BodyPtr->MeterInfo, BodyPtr->MeterInfoFlag);

  /* AC_EVSEStatus */
  Scc_ExiRx_ISO_ReadEVSEStatusAC((SCC_P2VARAPPLDATA(Exi_ISO_AC_EVSEStatusType))BodyPtr->AC_EVSEStatus);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_CableCheckRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_CableCheckRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_CableCheckResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_CableCheckResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_CABLE_CHECK_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_CableCheck_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_CableCheck_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */
  
  /* EVSEProcessing */
  Scc_Set_ISO_EVSEProcessing(BodyPtr->EVSEProcessing);

#if ( SCC_ENABLE_ONGOING_CALLBACKS ) && ( SCC_ENABLE_ONGOING_CALLBACKS == STD_ON )
#else
  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_ISO_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
#endif
  {
    /* DC_EVSEStatus */
    Scc_ExiRx_ISO_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);
  }

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_PreChargeRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_PreChargeRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_PreChargeResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_PreChargeResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_PRE_CHARGE_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_PreCharge_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PreCharge_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_ISO_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_ISO_DC_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_CurrentDemandRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_CurrentDemandRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_CurrentDemandResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_CurrentDemandResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_CURRENT_DEMAND_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_CurrentDemand_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_CurrentDemand_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_ISO_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_ISO_DC_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* EVSEPresentCurrent */
  Scc_Set_ISO_DC_EVSEPresentCurrent(BodyPtr->EVSEPresentCurrent);

  /* EVSECurrentLimitAchieved */
  Scc_Set_ISO_DC_EVSECurrentLimitAchieved(BodyPtr->EVSECurrentLimitAchieved);

  /* EVSEVoltageLimitAchieved */
  Scc_Set_ISO_DC_EVSEVoltageLimitAchieved(BodyPtr->EVSEVoltageLimitAchieved);

  /* EVSEPowerLimitAchieved */
  Scc_Set_ISO_DC_EVSEPowerLimitAchieved(BodyPtr->EVSEPowerLimitAchieved);

  /* EVSEMaximumVoltageLimit */
  Scc_Set_ISO_DC_EVSEMaximumVoltageLimit(BodyPtr->EVSEMaximumVoltageLimit, BodyPtr->EVSEMaximumVoltageLimitFlag);

  /* EVSEMaximumCurrentLimit */
  Scc_Set_ISO_DC_EVSEMaximumCurrentLimit(BodyPtr->EVSEMaximumCurrentLimit, BodyPtr->EVSEMaximumCurrentLimitFlag);

  /* EVSEMaximumPowerLimit */
  Scc_Set_ISO_DC_EVSEMaximumPowerLimit(BodyPtr->EVSEMaximumPowerLimit, BodyPtr->EVSEMaximumPowerLimitFlag);

  /* EVSEID */
  Scc_Set_ISO_EVSEID(BodyPtr->EVSEID);

  /* SAScheduleTupleID */
  if ( BodyPtr->SAScheduleTupleID != Scc_Exi_SAScheduleTupleID )
  {
    /* the received SAScheduleTupleID is different from the one which was sent to the EVSE in the PowerDeliveryReq */
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_CurrentDemand_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* MeterInfo */
  Scc_Set_ISO_MeterInfo(BodyPtr->MeterInfo, BodyPtr->MeterInfoFlag);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_WeldingDetectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_WeldingDetectionRes(void)
{
  SCC_P2CONSTNOINIT(Exi_ISO_WeldingDetectionResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_ISO_WeldingDetectionResType))Scc_ExiRx_ISO_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_ISO_WELDING_DETECTION_RES_TYPE != Scc_ExiRx_ISO_MsgPtr->Body->BodyElementElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxMessage, Scc_MsgStatus_WeldingDetection_Failed);
#if ( defined SCC_DEM_UNEXPECTED_MSG )
    /* report status to DEM */
    Scc_DemReportErrorStatusFailed(SCC_DEM_UNEXPECTED_MSG);
#endif /* SCC_DEM_UNEXPECTED_MSG */
    return (Std_ReturnType)E_NOT_OK;
  }
#if ( defined SCC_DEM_UNEXPECTED_MSG )
  else
  {
    /* report status to DEM */
    Scc_DemReportErrorStatusPassed(SCC_DEM_UNEXPECTED_MSG);
  }
#endif /* SCC_DEM_UNEXPECTED_MSG */

  /* check if the response is negative */
  if ( EXI_ISO_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_WeldingDetection_Failed);
    /* provide the response code to the application */
    Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_ISO_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_ISO_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_ISO_DC_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* ResponseCode */
  Scc_Set_ISO_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ReadEVSEStatusAC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiRx_ISO_ReadEVSEStatusAC(SCC_P2CONSTNOINIT(Exi_ISO_AC_EVSEStatusType) EVSEStatusPtr)
{
  /* RCD */
  Scc_Set_ISO_AC_RCD(EVSEStatusPtr->RCD);
  /* EVSENotification */
  Scc_Set_ISO_EVSENotification(EVSEStatusPtr->EVSENotification);
  /* NotificationMaxDelay */
  Scc_Set_ISO_NotificationMaxDelay(EVSEStatusPtr->NotificationMaxDelay);

  return;
}
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_ExiRx_ISO_ReadEVSEStatusDC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiRx_ISO_ReadEVSEStatusDC(SCC_P2CONSTNOINIT(Exi_ISO_DC_EVSEStatusType) EVSEStatusPtr)
{
  /* EVSEIsolationStatus */
  Scc_Set_ISO_DC_EVSEIsolationStatus(EVSEStatusPtr->EVSEIsolationStatus, EVSEStatusPtr->EVSEIsolationStatusFlag);
  /* EVSEStatusCode */
  Scc_Set_ISO_DC_EVSEStatusCode(EVSEStatusPtr->EVSEStatusCode);
  /* EVSENotification */
  Scc_Set_ISO_EVSENotification(EVSEStatusPtr->EVSENotification);
  /* NotificationMaxDelay */
  Scc_Set_ISO_NotificationMaxDelay(EVSEStatusPtr->NotificationMaxDelay);

  return;
}
#endif /* SCC_CHARGING_DC */

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

#endif /* SCC_SCHEMA_ISO */

/**********************************************************************************************************************
 *  END OF FILE: Scc_ExiRx_ISO.c
 *********************************************************************************************************************/
