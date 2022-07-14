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
/**        \file  Scc_ExiTx_ISO.c
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

#define SCC_EXITX_ISO_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )

#include "Scc_Exi.h"
#include "Scc.h"
#include "Scc_Lcfg.h"
#include "Scc_Priv.h"
#include SCC_IF_CFG_H /* PRQA S 0832 */ /* MD_Scc_0832 */
#include SCC_CP_CFG_H /* PRQA S 0832 */ /* MD_Scc_0832 */

#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
#include "EthIf.h"
#include "Exi.h"
#include "IpBase.h"
#if ( defined SCC_ENABLE_CSM_SUPPORT ) && ( SCC_ENABLE_CSM_SUPPORT == STD_ON )
# include "Csm.h"
#endif

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/
/* 8bit variables */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_P2VARNOINIT(Exi_ISO_V2G_MessageType) Scc_ExiTx_ISO_MsgPtr;

#if (( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON ) && ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON ))
SCC_VARNOINIT(Exi_ISO_EnergyTransferModeType) Scc_ExiTx_ISO_EnergyTransferMode;
#endif /* SCC_CHARGING_AC, SCC_CHARGING_DC */

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

SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_SessionSetupReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ServiceDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ServiceDetailReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PaymentServiceSelectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_AuthorizationReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ChargeParameterDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PowerDeliveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_SessionStopReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);

/* AC */
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ChargingStatusReq(void);
#endif /* SCC_CHARGING_AC */

/* DC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_CableCheckReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PreChargeReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_CurrentDemandReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_WeldingDetectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
#endif /* SCC_CHARGING_DC */

SCC_STATICFUNCCODE(void) Scc_ExiTx_ISO_WriteHeader(SCC_P2VARNOINIT(uint16) BufIdxPtr);
#if (   ( ( defined SCC_ENABLE_CERTIFICATE_INSTALLATION ) && ( SCC_ENABLE_CERTIFICATE_INSTALLATION == STD_ON )) \
     || ( ( defined SCC_ENABLE_CERTIFICATE_UPDATE ) && ( SCC_ENABLE_CERTIFICATE_UPDATE == STD_ON )))
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ExtractRootCertificateIDs(void);
#endif /* SCC_ENABLE_CERTIFICATE_INSTALLATION, SCC_ENABLE_CERTIFICATE_UPDATE */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(void) Scc_ExiTx_ISO_WriteEVStatusDC(SCC_P2VARNOINIT(Exi_ISO_DC_EVStatusType) *EVStatePtrPtr,
  SCC_P2VARNOINIT(uint16) BufIdxPtr);
#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_Init
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_ExiTx_ISO_Init(void)
{
  /* initialize the message pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr = (SCC_P2VARNOINIT(Exi_ISO_V2G_MessageType))&Scc_Exi_StructBuf[0];

  return;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_EncodeMessage
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ExiTx_ISO_EncodeMessage(void)
{
  Scc_ReturnType RetVal = Scc_ReturnType_OK;
  uint16         lBufIdx = 0;

  /* initialize the exi workspace and check if it failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitEncodingWorkspace() )
  {
    return Scc_ReturnType_NotOK;
  }

  /* set the V2G_Message as root element */
  Scc_Exi_EncWs.InputData.RootElementId = EXI_ISO_V2G_MESSAGE_TYPE;
  /* initialize the header of the V2G messages */
  Scc_ExiTx_ISO_WriteHeader(&lBufIdx);
  /* create the body */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body = (SCC_P2VARAPPLDATA(Exi_ISO_BodyType))&Scc_Exi_StructBuf[lBufIdx];
  lBufIdx += sizeof(Exi_ISO_BodyType);
  /* set the common body parameters */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementFlag = 1;
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElement = (SCC_P2VARAPPLDATA(Exi_ISO_BodyBaseType))&Scc_Exi_StructBuf[lBufIdx];

  switch ( Scc_MsgTrig )
  {
    /* check if a session setup request shall be sent */
  case Scc_MsgTrig_SessionSetup:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_SessionSetupReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_SessionSetupMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a service discovery request shall be sent */
  case Scc_MsgTrig_ServiceDiscovery:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_ServiceDiscoveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_ServiceDiscoveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a service detail request shall be sent */
  case Scc_MsgTrig_ServiceDetail:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_ServiceDetailReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_ServiceDetailMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a service payment selection request shall be sent */
  case Scc_MsgTrig_PaymentServiceSelection:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_PaymentServiceSelectionReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_PaymentServiceSelectionMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a contract authentication request shall be sent */
  case Scc_MsgTrig_Authorization:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_AuthorizationReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_AuthorizationMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a charge parameter discovery request shall be sent */
  case Scc_MsgTrig_ChargeParameterDiscovery:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_ChargeParameterDiscoveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_ChargeParameterDiscoveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a power delivery request shall be sent */
  case Scc_MsgTrig_PowerDelivery:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_PowerDeliveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_PowerDeliveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a session stop request shall be sent */
  case Scc_MsgTrig_SessionStop:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_SessionStopReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_SessionStopMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
    /* check if a charging status request shall be sent */
  case Scc_MsgTrig_ChargingStatus:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_ChargingStatusReq() )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_ChargingStatusMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

    /* check if a cable check request shall be sent */
  case Scc_MsgTrig_CableCheck:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_CableCheckReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_CableCheckMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a pre charge request shall be sent */
  case Scc_MsgTrig_PreCharge:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_PreChargeReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_PreChargeMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a current demand request shall be sent */
  case Scc_MsgTrig_CurrentDemand:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_CurrentDemandReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_CurrentDemandMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a welding detection request shall be sent */
  case Scc_MsgTrig_WeldingDetection:
    /* try to transmit the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_ISO_WeldingDetectionReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_ISO_WeldingDetectionMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

#endif /* SCC_CHARGING_DC */

  default:
    /* report the error */
    Scc_ReportError(Scc_StackError_InvalidTxParameter);
    RetVal = Scc_ReturnType_NotOK;
    break;
  }

  /* try to encode and send the message */
  if ( Scc_ReturnType_OK == RetVal )
  {
    return Scc_Exi_EncodeAndTransmitExiStream(SCC_V2GTP_HDR_PAYLOAD_TYPE_EXI);
  }
  else
  {
    return Scc_ReturnType_NotOK;
  }
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_SessionSetupReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_SessionSetupReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_SessionSetupReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_SessionSetupReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_SESSION_SETUP_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_SessionSetupReqType);

  /* EVCCID */ /* PRQA S 0310,3305 6 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVCCID = (SCC_P2VARAPPLDATA(Exi_ISO_evccIDType))&Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_evccIDType);
  /* get the MAC address from EthIf */
  EthIf_GetPhysAddr(SCC_CTRL_IDX, &BodyPtr->EVCCID->Buffer[0]);
  BodyPtr->EVCCID->Length = 6;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_ServiceDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ServiceDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_ServiceDiscoveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_ServiceDiscoveryReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_SERVICE_DISCOVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_ServiceDiscoveryReqType);

  /* ServiceScope */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->ServiceScope = (SCC_P2VARAPPLDATA(Exi_ISO_serviceScopeType))&Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_serviceScopeType);
  Scc_Get_ISO_ServiceScope(BodyPtr->ServiceScope, &Flag);
  BodyPtr->ServiceScopeFlag = Flag;

  /* ServiceCategory */
  Scc_Get_ISO_ServiceCategory(&BodyPtr->ServiceCategory, &Flag);
  BodyPtr->ServiceCategoryFlag = Flag;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_ServiceDetailReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ServiceDetailReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_ServiceDetailReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_ServiceDetailReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_SERVICE_DETAIL_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_ServiceDetailReqType);

  /* ServiceID */
  Scc_Get_ISO_ServiceIDRx(&BodyPtr->ServiceID);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_PaymentServiceSelectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PaymentServiceSelectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_PaymentServiceSelectionReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_PaymentServiceSelectionReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_PAYMENT_SERVICE_SELECTION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_PaymentServiceSelectionReqType);

  /* SelectedPaymentOption */
  Scc_Get_ISO_SelectedPaymentOption(&BodyPtr->SelectedPaymentOption);
  /* save the PaymentOption */
  Scc_Exi_PaymentOption = BodyPtr->SelectedPaymentOption;

  /* SelectedServiceList */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  Scc_Get_ISO_SelectedServiceListPtr(&BodyPtr->SelectedServiceList);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_AuthorizationReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_AuthorizationReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_AuthorizationReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_AuthorizationReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_AUTHORIZATION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_AuthorizationReqType);

  /* GenChallenge */
  {
    BodyPtr->IdFlag = FALSE;
    BodyPtr->GenChallengeFlag = FALSE;
  }

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_ChargeParameterDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ChargeParameterDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_ChargeParameterDiscoveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_ChargeParameterDiscoveryReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  SCC_P2VARNOINIT(Exi_ISO_AC_EVChargeParameterType) AC_EVChargeParameterTypePtr;
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  SCC_P2VARNOINIT(Exi_ISO_DC_EVChargeParameterType) DC_EVChargeParameterTypePtr;
#endif /* SCC_CHARGING_DC */

  Std_ReturnType RetVal = (Std_ReturnType)E_OK;
  boolean        Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_ChargeParameterDiscoveryReqType);

  /* MaxEntriesSAScheduleTuple */
  Scc_Get_ISO_MaxEntriesSAScheduleTuple(&BodyPtr->MaxEntriesSAScheduleTuple, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->MaxEntriesSAScheduleTupleFlag = Flag;

  /* RequestedEnergyTransferMode */
  Scc_Get_ISO_RequestedEnergyTransferMode(&BodyPtr->RequestedEnergyTransferMode);
#if ( ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON ) && ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON ))
  /* save the EnergyTransferMode */
  Scc_ExiTx_ISO_EnergyTransferMode = BodyPtr->RequestedEnergyTransferMode;
#endif /* SCC_CHARGING_AC, SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  /* check if AC charging was selected */
  if (   ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode )
      || ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE == Scc_ExiTx_ISO_EnergyTransferMode ))
#endif /* SCC_CHARGING_DC */
  {
    /* set the EVChargeParameter pointer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    BodyPtr->EVChargeParameter = (SCC_P2VARAPPLDATA(Exi_ISO_EVChargeParameterType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_AC_EVChargeParameterType);
    BodyPtr->EVChargeParameterElementId = EXI_ISO_AC_EVCHARGE_PARAMETER_TYPE;

    /* get the pointer to the Exi_ISO_AC_EVChargeParameterType */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AC_EVChargeParameterTypePtr = (SCC_P2VARAPPLDATA(Exi_ISO_AC_EVChargeParameterType))(BodyPtr->EVChargeParameter);

    /* AC_EVChargeParameter -> DepartureTime */
    Scc_Get_ISO_DepartureTime(&AC_EVChargeParameterTypePtr->DepartureTime, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    AC_EVChargeParameterTypePtr->DepartureTimeFlag = Flag;

    /* AC_EVChargeParameter -> EAmount */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AC_EVChargeParameterTypePtr->EAmount =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    AC_EVChargeParameterTypePtr->EAmount->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_WH;
    Scc_Get_ISO_AC_EAmount(AC_EVChargeParameterTypePtr->EAmount);

    /* AC_EVChargeParameter -> EVMaxVoltage */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AC_EVChargeParameterTypePtr->EVMaxVoltage =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    AC_EVChargeParameterTypePtr->EVMaxVoltage->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_V;
    Scc_Get_ISO_AC_EVMaxVoltage(AC_EVChargeParameterTypePtr->EVMaxVoltage);

    /* AC_EVChargeParameter -> EVMaxCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AC_EVChargeParameterTypePtr->EVMaxCurrent =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    AC_EVChargeParameterTypePtr->EVMaxCurrent->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
    Scc_Get_ISO_AC_EVMaxCurrent(AC_EVChargeParameterTypePtr->EVMaxCurrent);

    /* AC_EVChargeParameter -> EVMinCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    AC_EVChargeParameterTypePtr->EVMinCurrent =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    AC_EVChargeParameterTypePtr->EVMinCurrent->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
    Scc_Get_ISO_AC_EVMinCurrent(AC_EVChargeParameterTypePtr->EVMinCurrent);
  }
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  /* DC charging was selected */
  else
#endif /* SCC_CHARGING_AC */
  {
    /* set the EVChargeParameter pointer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    BodyPtr->EVChargeParameter =
      (SCC_P2VARAPPLDATA(Exi_ISO_EVChargeParameterType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_DC_EVChargeParameterType);
    BodyPtr->EVChargeParameterElementId =
      EXI_ISO_DC_EVCHARGE_PARAMETER_TYPE;

    /* get the pointer to the Exi_ISO_AC_EVChargeParameterType */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr = (SCC_P2VARAPPLDATA(Exi_ISO_DC_EVChargeParameterType))(BodyPtr->EVChargeParameter);
    /* DC_EVChargeParameter -> DepartureTime */
    Scc_Get_ISO_DepartureTime(&DC_EVChargeParameterTypePtr->DepartureTime, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->DepartureTimeFlag = Flag;

    /* DC_EVChargeParameter -> DC_EVStatus*/
    Scc_ExiTx_ISO_WriteEVStatusDC(&DC_EVChargeParameterTypePtr->DC_EVStatus, BufIdxPtr);

    /* DC_EVChargeParameter -> EVMaximumCurrentLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr->EVMaximumCurrentLimit =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    DC_EVChargeParameterTypePtr->EVMaximumCurrentLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
    Scc_Get_ISO_DC_EVMaximumCurrentLimit(DC_EVChargeParameterTypePtr->EVMaximumCurrentLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* parameter is not optional for this message, check if it was set */
    if ( TRUE != Flag )
    {
      Scc_ReportErrorAndStatus(Scc_StackError_InvalidTxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
      RetVal = (Std_ReturnType)E_NOT_OK;
    }

    /* DC_EVChargeParameter -> EVMaximumPowerLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr->EVMaximumPowerLimit =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    DC_EVChargeParameterTypePtr->EVMaximumPowerLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_W;
    Scc_Get_ISO_DC_EVMaximumPowerLimit(DC_EVChargeParameterTypePtr->EVMaximumPowerLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->EVMaximumPowerLimitFlag = Flag;

    /* DC_EVChargeParameter -> EVMaximumVoltageLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr->EVMaximumVoltageLimit =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    DC_EVChargeParameterTypePtr->EVMaximumVoltageLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_V;
    Scc_Get_ISO_DC_EVMaximumVoltageLimit(DC_EVChargeParameterTypePtr->EVMaximumVoltageLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* parameter is not optional for this message, check if it was set */
    if ( TRUE != Flag )
    {
      Scc_ReportErrorAndStatus(Scc_StackError_InvalidTxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
      RetVal = (Std_ReturnType)E_NOT_OK;
    }

    /* EVEnergyCapacity */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr->EVEnergyCapacity =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    DC_EVChargeParameterTypePtr->EVEnergyCapacity->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_WH;
    Scc_Get_ISO_DC_EVEnergyCapacity(DC_EVChargeParameterTypePtr->EVEnergyCapacity, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->EVEnergyCapacityFlag = Flag;

    /* EVEnergyRequest */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    DC_EVChargeParameterTypePtr->EVEnergyRequest =
      (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
    DC_EVChargeParameterTypePtr->EVEnergyRequest->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_WH;
    Scc_Get_ISO_DC_EVEnergyRequest(DC_EVChargeParameterTypePtr->EVEnergyRequest, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->EVEnergyRequestFlag = Flag;

    /* FullSOC */
    Scc_Get_ISO_DC_FullSOC(&DC_EVChargeParameterTypePtr->FullSOC, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->FullSOCFlag = Flag;

    /* BulkSOC */
    Scc_Get_ISO_DC_BulkSOC(&DC_EVChargeParameterTypePtr->BulkSOC, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    DC_EVChargeParameterTypePtr->BulkSOCFlag = Flag;
  }
#endif /* SCC_CHARGING_DC */

  return (Std_ReturnType)RetVal;
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_PowerDeliveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PowerDeliveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_PowerDeliveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_PowerDeliveryReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  SCC_P2VARNOINIT(Exi_ISO_DC_EVPowerDeliveryParameterType) DC_EVPowerDeliveryParameter;
#endif /* SCC_CHARGING_DC */

  boolean Flag = 0;
#if ( defined SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER ) && ( SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER == STD_ON )
  Scc_BufferPointerType ChargingProfileBufPtr;
#endif /* SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_POWER_DELIVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_PowerDeliveryReqType);

  /* ChargeProgress */
  Scc_Get_ISO_ChargeProgress(&BodyPtr->ChargeProgress);

  /* SAScheduleTupleID */
  Scc_Get_ISO_SAScheduleTupleID(&BodyPtr->SAScheduleTupleID);
  Scc_Exi_SAScheduleTupleID = BodyPtr->SAScheduleTupleID;

  /* ChargingProfile */
#if ( defined SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER ) && ( SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER == STD_ON )
    /* set the buffer pointer */
  ChargingProfileBufPtr.BufferPtr = &Scc_Exi_StructBuf[*BufIdxPtr];
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  ChargingProfileBufPtr.BufferLen =
    (uint16)( SCC_EXI_STRUCT_BUF_LEN - *BufIdxPtr - sizeof(Exi_ISO_DC_EVPowerDeliveryParameterType) );
#else
  ChargingProfileBufPtr.BufferLen = (uint16)( SCC_EXI_STRUCT_BUF_LEN - *BufIdxPtr );
#endif /* SCC_CHARGING_DC */
    /* get the charging profile */
  Scc_Get_ISO_ChargingProfilePtr(&ChargingProfileBufPtr, &Flag);
    /* check if the charging profile shall be used */
  if ( TRUE == Flag )
  {
    /* set the ChargingProfile of the application */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
    BodyPtr->ChargingProfile = (SCC_P2VARAPPLDATA(Exi_ISO_ChargingProfileType))&Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += ChargingProfileBufPtr.BufferLen;
  }
#else
    /* get the charging profile */
  Scc_Get_ISO_ChargingProfilePtr(&BodyPtr->ChargingProfile, &Flag);
#endif /* SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER */
  /* set the flag of the exi struct */
  BodyPtr->ChargingProfileFlag = (Exi_BitType)Flag;

  /* set the flag for the EVPowerDeliveryParameter element to 0 */
  BodyPtr->EVPowerDeliveryParameterFlag = 0;

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  /* check if DC charging is used */
  if (   ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE != Scc_ExiTx_ISO_EnergyTransferMode )
      && ( EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE != Scc_ExiTx_ISO_EnergyTransferMode ))
#endif /* SCC_CHARGING_AC */
  {
    /* check if the EVPowerDeliveryParameter shall be sent */
    Scc_Get_ISO_DC_EVPowerDeliveryParameterFlag(&Flag);
    if ( TRUE == Flag )
    {
      /* get the pointer to the Exi_ISO_DC_EVPowerDeliveryParameterType */ /* PRQA S 0310,3305 5 */ /* MD_Scc_0310_3305 */
      BodyPtr->EVPowerDeliveryParameter =
        (SCC_P2VARAPPLDATA(Exi_ISO_EVChargeParameterType))&Scc_Exi_StructBuf[*BufIdxPtr];
      *BufIdxPtr += sizeof(Exi_ISO_DC_EVPowerDeliveryParameterType);
      DC_EVPowerDeliveryParameter =
        (SCC_P2VARAPPLDATA(Exi_ISO_DC_EVPowerDeliveryParameterType))(BodyPtr->EVPowerDeliveryParameter);

      /* DC_EVPowerDeliveryParameter */
      BodyPtr->EVPowerDeliveryParameterElementId = EXI_ISO_DC_EVPOWER_DELIVERY_PARAMETER_TYPE;
      BodyPtr->EVPowerDeliveryParameterFlag = 1;

      /* DC_EVPowerDeliveryParameter -> DC_EVStatus */
      Scc_ExiTx_ISO_WriteEVStatusDC(&DC_EVPowerDeliveryParameter->DC_EVStatus, BufIdxPtr);

      /* DC_EVPowerDeliveryParameter -> BulkChargingComplete */
      Scc_Get_ISO_DC_BulkChargingComplete(&DC_EVPowerDeliveryParameter->BulkChargingComplete, &Flag);
      DC_EVPowerDeliveryParameter->BulkChargingCompleteFlag = Flag;

      /* DC_EVPowerDeliveryParameter -> ChargingComplete */
      Scc_Get_ISO_DC_ChargingComplete(&DC_EVPowerDeliveryParameter->ChargingComplete);
    }
  }
#endif /* SCC_CHARGING_DC */

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_SessionStopReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_SessionStopReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_SessionStopReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_SessionStopReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_SESSION_STOP_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_SessionStopReqType);

  /* ChargingSession */
  Scc_Get_ISO_ChargingSession(&Scc_Exi_ChargingSession);
  BodyPtr->ChargingSession = Scc_Exi_ChargingSession;

  return (Std_ReturnType)E_OK;
}

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_ChargingStatusReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_ChargingStatusReq(void)
{
  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_CHARGING_STATUS_REQ_TYPE;
  /* *BufIdxPtr += sizeof(Exi_ISO_ChargingStatusReqType); */ /* change if ChargingStatusReq has elements */

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_CableCheckReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_CableCheckReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_CableCheckReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_CableCheckReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_CABLE_CHECK_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_CableCheckReqType);

  /* DC_EVStatus */
  Scc_ExiTx_ISO_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_PreChargeReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_PreChargeReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_PreChargeReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_PreChargeReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_PRE_CHARGE_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_PreChargeReqType);

  /* DC_EVStatus */
  Scc_ExiTx_ISO_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  /* EVTargetVoltage */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetVoltage = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVTargetVoltage->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_V;
  Scc_Get_ISO_DC_EVTargetVoltage(BodyPtr->EVTargetVoltage);

  /* EVTargetCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetCurrent = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVTargetCurrent->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
  Scc_Get_ISO_DC_EVTargetCurrent(BodyPtr->EVTargetCurrent);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_CurrentDemandReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_CurrentDemandReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_CurrentDemandReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_CurrentDemandReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_CURRENT_DEMAND_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_CurrentDemandReqType);

  /* DC_EVStatus */
  Scc_ExiTx_ISO_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  /* EVTargetCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetCurrent = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVTargetCurrent->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
  Scc_Get_ISO_DC_EVTargetCurrent(BodyPtr->EVTargetCurrent);

  /* EVMaximumVoltageLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumVoltageLimit = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVMaximumVoltageLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_V;
  Scc_Get_ISO_DC_EVMaximumVoltageLimit(BodyPtr->EVMaximumVoltageLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumVoltageLimitFlag = Flag;

  /* EVMaximumCurrentLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumCurrentLimit = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVMaximumCurrentLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_A;
  Scc_Get_ISO_DC_EVMaximumCurrentLimit(BodyPtr->EVMaximumCurrentLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumCurrentLimitFlag = Flag;

  /* EVMaximumPowerLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumPowerLimit = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVMaximumPowerLimit->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_W;
  Scc_Get_ISO_DC_EVMaximumPowerLimit(BodyPtr->EVMaximumPowerLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumPowerLimitFlag = Flag;

  /* BulkChargingComplete */
  Scc_Get_ISO_DC_BulkChargingComplete(&BodyPtr->BulkChargingComplete, &Flag);
  BodyPtr->BulkChargingCompleteFlag = Flag;

  /* ChargingComplete */
  Scc_Get_ISO_DC_ChargingComplete(&BodyPtr->ChargingComplete);

  /* RemainingTimeToFullSoC */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->RemainingTimeToFullSoC = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->RemainingTimeToFullSoC->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_S;
  Scc_Get_ISO_DC_RemainingTimeToFullSoC(BodyPtr->RemainingTimeToFullSoC, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->RemainingTimeToFullSoCFlag = Flag;

  /* RemainingTimeToBulkSoC */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->RemainingTimeToBulkSoC = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->RemainingTimeToBulkSoC->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_S;
  Scc_Get_ISO_DC_RemainingTimeToBulkSoC(BodyPtr->RemainingTimeToBulkSoC, &Flag);
  BodyPtr->RemainingTimeToBulkSoCFlag = Flag;

  /* EVTargetVoltage */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetVoltage = (SCC_P2VARAPPLDATA(Exi_ISO_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_PhysicalValueType);
  BodyPtr->EVTargetVoltage->Unit = EXI_ISO_UNIT_SYMBOL_TYPE_V;
  Scc_Get_ISO_DC_EVTargetVoltage(BodyPtr->EVTargetVoltage);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_WeldingDetectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_ISO_WeldingDetectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_ISO_WeldingDetectionReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_ISO_WeldingDetectionReqType))Scc_ExiTx_ISO_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Body->BodyElementElementId = EXI_ISO_WELDING_DETECTION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_ISO_WeldingDetectionReqType);

  /* DC_EVStatus */
  Scc_ExiTx_ISO_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  return (Std_ReturnType)E_OK;
}

#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  LOCAL HELPER FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_WriteHeader
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiTx_ISO_WriteHeader(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  /* set the buffer for this new V2G message */
  *BufIdxPtr += sizeof(Exi_ISO_V2G_MessageType);
  /* set the buffer of the V2G message header */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Header = (SCC_P2VARAPPLDATA(Exi_ISO_MessageHeaderType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_MessageHeaderType);
  Scc_ExiTx_ISO_MsgPtr->Header->NotificationFlag = 0;
  Scc_ExiTx_ISO_MsgPtr->Header->SignatureFlag = 0;
  /* set the buffer of the SessionID */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_ISO_MsgPtr->Header->SessionID = (SCC_P2VARAPPLDATA(Exi_ISO_sessionIDType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_sessionIDType);
  Scc_ExiTx_ISO_MsgPtr->Header->SessionID->Length = Scc_SessionIDNvm[0];
  /* copy the SessionID to the buffer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType)) &Scc_ExiTx_ISO_MsgPtr->Header->SessionID->Buffer[0],
    (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType)) &Scc_SessionIDNvm[1], (uint32)Scc_SessionIDNvm[0]);

  return;
}

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_ExiTx_ISO_WriteEVStatusDC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiTx_ISO_WriteEVStatusDC(SCC_P2VARNOINIT(Exi_ISO_DC_EVStatusType) *EVStatePtrPtr,
  SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  /* create the root element */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  *EVStatePtrPtr = (SCC_P2VARNOINIT(Exi_ISO_DC_EVStatusType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_ISO_DC_EVStatusType);

  /* EVReady */
  Scc_Get_ISO_DC_EVReady(&(*EVStatePtrPtr)->EVReady);

  /* EVErrorCode */
  Scc_Get_ISO_DC_EVErrorCode(&(*EVStatePtrPtr)->EVErrorCode);

  /* EVRESSSOC */
  Scc_Get_ISO_DC_EVRESSSOC(&(*EVStatePtrPtr)->EVRESSSOC);

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
 *  END OF FILE: Scc_ExiTx_ISO.c
 *********************************************************************************************************************/
