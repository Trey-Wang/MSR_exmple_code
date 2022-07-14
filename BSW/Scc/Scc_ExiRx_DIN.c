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
/**        \file  Scc_ExiRx_DIN.c
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

#define SCC_EXIRX_DIN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )

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

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */

/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/

/* 16bit variables */
#define SCC_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_VARNOINIT(uint16) Scc_ExiRx_DIN_ServiceID;

#define SCC_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
STATIC SCC_P2VARNOINIT(Exi_DIN_V2G_MessageType) Scc_ExiRx_DIN_MsgPtr;

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Common */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_SessionSetupRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ServiceDiscoveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ServicePaymentSelectionRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ContractAuthenticationRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ChargeParameterDiscoveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_PowerDeliveryRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_SessionStopRes(void);

/* DC */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_CableCheckRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_PreChargeRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_CurrentDemandRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_WeldingDetectionRes(void);

SCC_STATICFUNCCODE(void) Scc_ExiRx_DIN_ReadEVSEStatusDC(SCC_P2CONSTNOINIT(Exi_DIN_DC_EVSEStatusType) EVSEStatusPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_Init
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_ExiRx_DIN_Init(void)
{
  /* set the message pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiRx_DIN_MsgPtr = (SCC_P2VARNOINIT(Exi_DIN_V2G_MessageType))&Scc_Exi_StructBuf[0];

  return;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_DecodeMessage
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_DecodeMessage(void)
{
  /* initialize the exi workspace and check if the workspace initialization failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitDecodingWorkspace() )
  {
    return (Std_ReturnType)E_NOT_OK;
  }

  /* set the decode information */
  Scc_Exi_DecWs.OutputData.RootElementId = EXI_DIN_V2G_MESSAGE_TYPE;
  Scc_Exi_DecWs.OutputData.SchemaSetId = EXI_SCHEMA_SET_DIN_TYPE;
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
    if ( Scc_ExiRx_DIN_MsgPtr->Header->SessionID->Length == Scc_SessionIDNvm[0] )
    {
      /* if the length is the same, check if the content is the same, too */
      if ( IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Scc_ExiRx_DIN_MsgPtr->Header->SessionID->Buffer[0],
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
    /* the Notification is currently ignored */
  }

  switch ( Scc_MsgTrig )
  {
    /* handle an incoming session setup response message */
  case Scc_MsgTrig_SessionSetup:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_SessionSetupRes() )
    {
      Scc_State = Scc_State_Connected;
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_SessionSetup_OK);
    }
    break;

    /* handle an incoming service discovery response message */
  case Scc_MsgTrig_ServiceDiscovery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_ServiceDiscoveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ServiceDiscovery_OK);
    }
    break;

    /* handle an incoming service payment selection response message */
  case Scc_MsgTrig_PaymentServiceSelection:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_ServicePaymentSelectionRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PaymentServiceSelection_OK);
    }
    break;

    /* handle an incoming contract authentication response message */
  case Scc_MsgTrig_Authorization:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_ContractAuthenticationRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_Authorization_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming charge parameter discovery response message */
  case Scc_MsgTrig_ChargeParameterDiscovery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_ChargeParameterDiscoveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_ChargeParameterDiscovery_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming power delivery response message */
  case Scc_MsgTrig_PowerDelivery:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_PowerDeliveryRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PowerDelivery_OK);
    }
    break;

    /* handle an incoming session stop response message */
  case Scc_MsgTrig_SessionStop:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_SessionStopRes() )
    {
      Scc_State = Scc_State_Disconnected;
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_SessionStop_OK);
    }
    break;

    /* handle an incoming cable check response message */
  case Scc_MsgTrig_CableCheck:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_CableCheckRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_CableCheck_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming pre charge response message */
  case Scc_MsgTrig_PreCharge:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_PreChargeRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_PreCharge_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming current demand response message */
  case Scc_MsgTrig_CurrentDemand:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_CurrentDemandRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_CurrentDemand_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

    /* handle an incoming welding detection response message */
  case Scc_MsgTrig_WeldingDetection:
    /* try to process the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiRx_DIN_WeldingDetectionRes() )
    {
      Scc_ReportSuccessAndStatus(Scc_MsgStatus_WeldingDetection_OK);
      Scc_Set_Core_CyclicMsgRcvd(TRUE);
    }
    break;

  default:
    return (Std_ReturnType)E_NOT_OK; /* invalid state */
  }

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VSessionSetupRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_SessionSetupRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_SessionSetupResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_SessionSetupResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_SESSION_SETUP_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_SessionSetup_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* SessionID */
  /* get the new length of the SessionID */
  Scc_SessionIDNvm[0] = (uint8) Scc_ExiRx_DIN_MsgPtr->Header->SessionID->Length;
  /* copy the SessionID */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType)) &Scc_SessionIDNvm[1],
    (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType)) &Scc_ExiRx_DIN_MsgPtr->Header->SessionID->Buffer[0], Scc_SessionIDNvm[0]);

  /* SessionID */
  Scc_Set_DIN_SessionID(Scc_ExiRx_DIN_MsgPtr->Header->SessionID);

  /* EVSEID */
  Scc_Set_DIN_EVSEID(BodyPtr->EVSEID);

  /* DateTimeNow */
  Scc_Set_DIN_DateTimeNow(BodyPtr->DateTimeNow, BodyPtr->DateTimeNowFlag);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(EXI_DIN_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VServiceDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ServiceDiscoveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_ServiceDiscoveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_ServiceDiscoveryResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  uint8_least Counter;
  boolean     ExternalPaymentFound = FALSE;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_SERVICE_DISCOVERY_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ServiceDiscovery_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* PaymentOptionList */
  for ( Counter = 0; Counter < BodyPtr->PaymentOptions->PaymentOptionCount; Counter++ )
  {
    /* check if this PaymentOption element contains ExternalPayment */
    if ( EXI_DIN_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT == BodyPtr->PaymentOptions->PaymentOption[Counter] )
    {
      /* set the flag */
      ExternalPaymentFound = TRUE;
    }
  }
  /* check if ExternalPayment was not found */
  if ( TRUE != ExternalPaymentFound )
  {
    /* report the error */
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ServiceDiscovery_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* ChargeService */ /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Scc_Set_DIN_ChargeService(BodyPtr->ChargeService);
  /* save the ServiceID */
  Scc_ExiRx_DIN_ServiceID = BodyPtr->ChargeService->ServiceTag->ServiceID;

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VServicePaymentSelectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ServicePaymentSelectionRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_ServicePaymentSelectionResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_ServicePaymentSelectionResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_SERVICE_PAYMENT_SELECTION_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PaymentServiceSelection_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VContractAuthenticationRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ContractAuthenticationRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_ContractAuthenticationResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_ContractAuthenticationResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_CONTRACT_AUTHENTICATION_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_Authorization_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* EVSEProcessing */
  Scc_Set_DIN_EVSEProcessing(BodyPtr->EVSEProcessing);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VChargeParameterDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_ChargeParameterDiscoveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_ChargeParameterDiscoveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_ChargeParameterDiscoveryResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;
  SCC_P2CONSTNOINIT(Exi_DIN_DC_EVSEChargeParameterType) DC_EVSEChargeParameterPtr =
    (SCC_P2CONSTAPPLDATA(Exi_DIN_DC_EVSEChargeParameterType))BodyPtr->EVSEChargeParameter;
  
  /* --- verify data --- */

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_CHARGE_PARAMETER_DISCOVERY_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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

  /* check if the EVSEChargeParameter has the correct type */
  if ( EXI_DIN_DC_EVSECHARGE_PARAMETER_TYPE != BodyPtr->EVSEChargeParameterElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* check if the response is negative */
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* only further verify the message when EVSEProcessing is set to Finished */
  if ( EXI_DIN_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
  {
#if ( defined SCC_IGNORE_EVSE_MAXIMUM_POWER_LIMIT ) && ( SCC_IGNORE_EVSE_MAXIMUM_POWER_LIMIT == STD_ON )
#else
    /* check if the parameter was sent by the EVSE */
    if ( 0u == DC_EVSEChargeParameterPtr->EVSEMaximumPowerLimitFlag )
    {
      Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
      return (Std_ReturnType)E_NOT_OK;
    }
#endif /* SCC_IGNORE_EVSE_MAXIMUM_POWER_LIMIT */
  }

  /* --- evaluate data --- */

  /* EVSEProcessing */
  Scc_Set_DIN_EVSEProcessing(BodyPtr->EVSEProcessing);

  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_DIN_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
  {
    /* provide the SAScheduleList to the application */
    Scc_Set_DIN_SAScheduleList((SCC_P2CONSTAPPLDATA(Exi_DIN_SAScheduleListType))BodyPtr->SASchedules);
  }

#if ( SCC_ENABLE_ONGOING_CALLBACKS ) && ( SCC_ENABLE_ONGOING_CALLBACKS == STD_ON )
#else
  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_DIN_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
#endif
  {
    /* DC_EVSEChargeParameter -> DC_EVSEStatus */
    Scc_ExiRx_DIN_ReadEVSEStatusDC(DC_EVSEChargeParameterPtr->DC_EVSEStatus);
    /* DC_EVSEChargeParameter -> EVSEMaximumCurrentLimit */
    Scc_Set_DIN_EVSEMaximumCurrentLimit(DC_EVSEChargeParameterPtr->EVSEMaximumCurrentLimit, 1);
    /* DC_EVSEChargeParameter -> EVSEMaximumPowerLimit */
    Scc_Set_DIN_EVSEMaximumPowerLimit(DC_EVSEChargeParameterPtr->EVSEMaximumPowerLimit,
      DC_EVSEChargeParameterPtr->EVSEMaximumPowerLimitFlag);
    /* DC_EVSEChargeParameter -> EVSEMaximumVoltageLimit */
    Scc_Set_DIN_EVSEMaximumVoltageLimit(DC_EVSEChargeParameterPtr->EVSEMaximumVoltageLimit, 1);
    /* DC_EVSEChargeParameter -> EVSEMinimumCurrentLimit */
    Scc_Set_DIN_EVSEMinimumCurrentLimit(DC_EVSEChargeParameterPtr->EVSEMinimumCurrentLimit);
    /* DC_EVSEChargeParameter -> EVSEMinimumVoltageLimit */
    Scc_Set_DIN_EVSEMinimumVoltageLimit(DC_EVSEChargeParameterPtr->EVSEMinimumVoltageLimit);
    /* DC_EVSEChargeParameter -> EVSECurrentRegulationTolerance */
    Scc_Set_DIN_EVSECurrentRegulationTolerance(DC_EVSEChargeParameterPtr->EVSECurrentRegulationTolerance,
      DC_EVSEChargeParameterPtr->EVSECurrentRegulationToleranceFlag);
    /* DC_EVSEChargeParameter -> EVSEPeakCurrentRipple */
    Scc_Set_DIN_EVSEPeakCurrentRipple(DC_EVSEChargeParameterPtr->EVSEPeakCurrentRipple);
    /* DC_EVSEChargeParameter -> EVSEEnergyToBeDelivered */
    Scc_Set_DIN_EVSEEnergyToBeDelivered(DC_EVSEChargeParameterPtr->EVSEEnergyToBeDelivered,
      DC_EVSEChargeParameterPtr->EVSEEnergyToBeDeliveredFlag);
  }

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VPowerDeliveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_PowerDeliveryRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_PowerDeliveryResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_PowerDeliveryResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_POWER_DELIVERY_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PowerDelivery_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* check if the EVSEStatus contains an DC_EVSEStatus */
  if ( EXI_DIN_DC_EVSESTATUS_TYPE != BodyPtr->EVSEStatusElementId )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidRxParameter, Scc_MsgStatus_PowerDelivery_Failed);
    return (Std_ReturnType)E_NOT_OK;
  }
  /* DC_EVSEStatus */
  Scc_ExiRx_DIN_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_DIN_DC_EVSEStatusType))BodyPtr->EVSEStatus);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VSessionStopRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_SessionStopRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_SessionStopResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_SessionStopResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_SESSION_STOP_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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

  /* check if the response is negative */
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_SessionStop_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VCableCheckRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_CableCheckRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_CableCheckResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_CableCheckResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_CABLE_CHECK_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_CableCheck_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */
  
  /* EVSEProcessing */
  Scc_Set_DIN_EVSEProcessing(BodyPtr->EVSEProcessing);

#if ( SCC_ENABLE_ONGOING_CALLBACKS ) && ( SCC_ENABLE_ONGOING_CALLBACKS == STD_ON )
#else
  /* only update the values when EVSEProcessing is set to Finished */
  if ( EXI_DIN_EVSEPROCESSING_TYPE_FINISHED == BodyPtr->EVSEProcessing )
#endif
  {
    /* DC_EVSEStatus */
    Scc_ExiRx_DIN_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_DIN_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);
  }

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VPreChargeRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_PreChargeRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_PreChargeResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_PreChargeResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_PRE_CHARGE_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_PreCharge_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_DIN_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_DIN_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_DIN_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VCurrentDemandRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_CurrentDemandRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_CurrentDemandResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_CurrentDemandResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_CURRENT_DEMAND_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_CurrentDemand_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_DIN_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_DIN_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_DIN_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* EVSEPresentCurrent */
  Scc_Set_DIN_EVSEPresentCurrent(BodyPtr->EVSEPresentCurrent);

  /* EVSECurrentLimitAchieved */
  Scc_Set_DIN_EVSECurrentLimitAchieved(BodyPtr->EVSECurrentLimitAchieved);

  /* EVSEVoltageLimitAchieved */
  Scc_Set_DIN_EVSEVoltageLimitAchieved(BodyPtr->EVSEVoltageLimitAchieved);

  /* EVSEPowerLimitAchieved */
  Scc_Set_DIN_EVSEPowerLimitAchieved(BodyPtr->EVSEPowerLimitAchieved);

  /* EVSEMaximumVoltageLimit */
  Scc_Set_DIN_EVSEMaximumVoltageLimit(BodyPtr->EVSEMaximumVoltageLimit,
    BodyPtr->EVSEMaximumVoltageLimitFlag);

  /* EVSEMaximumCurrentLimit */
  Scc_Set_DIN_EVSEMaximumCurrentLimit(BodyPtr->EVSEMaximumCurrentLimit, BodyPtr->EVSEMaximumCurrentLimitFlag);

  /* EVSEMaximumPowerLimit */
  Scc_Set_DIN_EVSEMaximumPowerLimit(BodyPtr->EVSEMaximumPowerLimit, BodyPtr->EVSEMaximumPowerLimitFlag);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VWeldingDetectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_WeldingDetectionRes(void)
{
  SCC_P2CONSTNOINIT(Exi_DIN_WeldingDetectionResType) BodyPtr =
    (SCC_P2CONSTNOINIT(Exi_DIN_WeldingDetectionResType))Scc_ExiRx_DIN_MsgPtr->Body->BodyElement;

  /* check if the message is not the one that was expected */
  if ( EXI_DIN_WELDING_DETECTION_RES_TYPE != Scc_ExiRx_DIN_MsgPtr->Body->BodyElementElementId )
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
  if ( EXI_DIN_RESPONSE_CODE_TYPE_FAILED <= BodyPtr->ResponseCode )
  {
    /* report the negative response code as stack error */
    Scc_ReportErrorAndStatus(Scc_StackError_NegativeResponseCode, Scc_MsgStatus_WeldingDetection_Failed);
    /* provide the response code to the application */
    Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);
    return (Std_ReturnType)E_NOT_OK;
  }

  /* --- evaluate data --- */

  /* DC_EVSEStatus */
  Scc_ExiRx_DIN_ReadEVSEStatusDC((SCC_P2VARAPPLDATA(Exi_DIN_DC_EVSEStatusType))BodyPtr->DC_EVSEStatus);

  /* EVSEPresentVoltage */
  Scc_Set_DIN_EVSEPresentVoltage(BodyPtr->EVSEPresentVoltage);

  /* ResponseCode */
  Scc_Set_DIN_ResponseCode(BodyPtr->ResponseCode);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiRx_DIN_VReadEVSEStatusDC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiRx_DIN_ReadEVSEStatusDC(SCC_P2CONSTNOINIT(Exi_DIN_DC_EVSEStatusType) EVSEStatusPtr)
{
  /* EVSEIsolationStatus */
  Scc_Set_DIN_EVSEIsolationStatus(EVSEStatusPtr->EVSEIsolationStatus, EVSEStatusPtr->EVSEIsolationStatusFlag);
  /* EVSEStatusCode */
  Scc_Set_DIN_EVSEStatusCode(EVSEStatusPtr->EVSEStatusCode);
  /* NotificationMaxDelay */
  Scc_Set_DIN_NotificationMaxDelay(EVSEStatusPtr->NotificationMaxDelay);
  /* EVSENotification */
  Scc_Set_DIN_EVSENotification(EVSEStatusPtr->EVSENotification);

  return;
}

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

#endif /* SCC_SCHEMA_DIN */

/**********************************************************************************************************************
 *  END OF FILE: Scc_ExiRx_DIN.c
 *********************************************************************************************************************/
