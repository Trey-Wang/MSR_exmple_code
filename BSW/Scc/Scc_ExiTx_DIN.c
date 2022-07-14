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
/**        \file  Scc_ExiTx_DIN.c
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

#define SCC_EXITX_DIN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )

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

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_P2VARNOINIT(Exi_DIN_V2G_MessageType) Scc_ExiTx_DIN_MsgPtr;

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

/* Common */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_SessionSetupReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ServiceDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ServicePaymentSelectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ContractAuthenticationReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ChargeParameterDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_PowerDeliveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_SessionStopReq(void);

/* DC */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_CableCheckReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_PreChargeReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_CurrentDemandReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_WeldingDetectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr);

SCC_STATICFUNCCODE(void) Scc_ExiTx_DIN_WriteHeader(SCC_P2VARNOINIT(uint16) BufIdxPtr);
SCC_STATICFUNCCODE(void) Scc_ExiTx_DIN_WriteEVStatusDC(SCC_P2VARNOINIT(Exi_DIN_DC_EVStatusType) *EVStatePtrPtr,
  SCC_P2VARNOINIT(uint16) BufIdxPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_Init
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_ExiTx_DIN_Init(void)
{
  /* set the message pointer */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr = (SCC_P2VARNOINIT(Exi_DIN_V2G_MessageType))&Scc_Exi_StructBuf[0];

  return;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_EncodeMessage
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ExiTx_DIN_EncodeMessage(void)
{
  Scc_ReturnType RetVal = Scc_ReturnType_OK;
  uint16         lBufIdx = 0;

  /* initialize the exi workspace and check if it failed */
  if ( (Std_ReturnType)E_OK != Scc_Exi_InitEncodingWorkspace() )
  {
    return Scc_ReturnType_NotOK;
  }

  /* set the V2G_Message as root element */
  Scc_Exi_EncWs.InputData.RootElementId = EXI_DIN_V2G_MESSAGE_TYPE;
  /* initialize the header of the V2G messages */
  Scc_ExiTx_DIN_WriteHeader(&lBufIdx);
  /* create the body */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body = (SCC_P2VARAPPLDATA(Exi_DIN_BodyType))&Scc_Exi_StructBuf[lBufIdx];
  lBufIdx += sizeof(Exi_DIN_BodyType);
  /* set the common body parameters */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementFlag = 1;
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElement = (SCC_P2VARAPPLDATA(Exi_DIN_BodyBaseType))&Scc_Exi_StructBuf[lBufIdx];

  switch ( Scc_MsgTrig )
  {
    /* check if a session setup request shall be sent */
  case Scc_MsgTrig_SessionSetup:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_SessionSetupReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_SessionSetupMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a service discovery request shall be sent */
  case Scc_MsgTrig_ServiceDiscovery:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_ServiceDiscoveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_ServiceDiscoveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a service payment selection request shall be sent */
  case Scc_MsgTrig_PaymentServiceSelection:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_ServicePaymentSelectionReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_ServicePaymentSelectionMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a contract authentication request shall be sent */
  case Scc_MsgTrig_Authorization:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_ContractAuthenticationReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_ContractAuthenticationMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a charge parameter discovery request shall be sent */
  case Scc_MsgTrig_ChargeParameterDiscovery:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_ChargeParameterDiscoveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_ChargeParameterDiscoveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a power delivery request shall be sent */
  case Scc_MsgTrig_PowerDelivery:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_PowerDeliveryReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_PowerDeliveryMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a session stop request shall be sent */
  case Scc_MsgTrig_SessionStop:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_SessionStopReq() )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_SessionStopMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a cable check request shall be sent */
  case Scc_MsgTrig_CableCheck:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_CableCheckReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_CableCheckMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a pre charge request shall be sent */
  case Scc_MsgTrig_PreCharge:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_PreChargeReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_PreChargeMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a current demand request shall be sent */
  case Scc_MsgTrig_CurrentDemand:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_CurrentDemandReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_CurrentDemandMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

    /* check if a welding detection request shall be sent */
  case Scc_MsgTrig_WeldingDetection:
    /* create the message and check if it was successful */
    if ( (Std_ReturnType)E_OK == Scc_ExiTx_DIN_WeldingDetectionReq(&lBufIdx) )
    {
      Scc_TimeoutCnt = (uint16)Scc_ConfigValue_Timer_DIN_WeldingDetectionMessageTimeout;
    }
    else
    {
      /* set the negative return value */
      RetVal = Scc_ReturnType_NotOK;
    }
    break;

  default:
    /* report a V2G error of the current trigger */
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
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VSessionSetupReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_SessionSetupReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_SessionSetupReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_SessionSetupReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_SESSION_SETUP_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_SessionSetupReqType);

  /* EVCCID */ /* PRQA S 0310,3305 6 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVCCID = (SCC_P2VARAPPLDATA(Exi_DIN_evccIDType))&Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_evccIDType);
  /* get the MAC address from EthIf */
  EthIf_GetPhysAddr(SCC_CTRL_IDX, &BodyPtr->EVCCID->Buffer[0]);
  BodyPtr->EVCCID->Length = 6;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VServiceDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ServiceDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_ServiceDiscoveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_ServiceDiscoveryReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_SERVICE_DISCOVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_ServiceDiscoveryReqType);

  /* ServiceScope */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->ServiceScopeFlag = 0;

  /* ServiceCategory */
  BodyPtr->ServiceCategoryFlag = 0;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VServicePaymentSelectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ServicePaymentSelectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_ServicePaymentSelectionReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_ServicePaymentSelectionReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_SERVICE_PAYMENT_SELECTION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_ServicePaymentSelectionReqType);

  /* SelectedPaymentOption */
  BodyPtr->SelectedPaymentOption = EXI_DIN_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT;

  /* create a SelectedServiceList */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->SelectedServiceList =
    (SCC_P2VARAPPLDATA(Exi_DIN_SelectedServiceListType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_SelectedServiceListType);
  /* create a SelectedService */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->SelectedServiceList->SelectedService =
    (SCC_P2VARAPPLDATA(Exi_DIN_SelectedServiceType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_SelectedServiceType);
  /* set the SelectedService */
  BodyPtr->SelectedServiceList->SelectedService->ServiceID = Scc_ExiRx_DIN_ServiceID;
  BodyPtr->SelectedServiceList->SelectedService->ParameterSetIDFlag = 0;
  BodyPtr->SelectedServiceList->SelectedService->NextSelectedServicePtr =
    (SCC_P2VARAPPLDATA(Exi_DIN_SelectedServiceType))NULL_PTR;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VContractAuthenticationReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ContractAuthenticationReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_ContractAuthenticationReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_ContractAuthenticationReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_CONTRACT_AUTHENTICATION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_ContractAuthenticationReqType);

  /* GenChallenge */
  BodyPtr->IdFlag = FALSE;
  BodyPtr->GenChallengeFlag = FALSE;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VChargeParameterDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_ChargeParameterDiscoveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_ChargeParameterDiscoveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_ChargeParameterDiscoveryReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;
  SCC_P2VARNOINIT(Exi_DIN_DC_EVChargeParameterType) DC_EVChargeParameterTypePtr;

  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_ChargeParameterDiscoveryReqType);
  /* set the EVChargeParameter pointer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVChargeParameter = (SCC_P2VARAPPLDATA(Exi_DIN_EVChargeParameterType))&Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_DC_EVChargeParameterType);
  BodyPtr->EVChargeParameterElementId = EXI_DIN_DC_EVCHARGE_PARAMETER_TYPE;

  /* EVRequestedEnergyTransferType */
  Scc_Get_DIN_RequestedEnergyTransferMode(&BodyPtr->EVRequestedEnergyTransferType);

  /* get the pointer to the Exi_DIN_DC_EVChargeParameterType */
  DC_EVChargeParameterTypePtr = (SCC_P2VARAPPLDATA(Exi_DIN_DC_EVChargeParameterType))(BodyPtr->EVChargeParameter);

  /* DC_EVChargeParameter -> DC_EVStatus*/
  Scc_ExiTx_DIN_WriteEVStatusDC(&DC_EVChargeParameterTypePtr->DC_EVStatus, BufIdxPtr);

  /* DC_EVChargeParameter -> EVMaximumCurrentLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  DC_EVChargeParameterTypePtr->EVMaximumCurrentLimit =
    (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumCurrentLimit(DC_EVChargeParameterTypePtr->EVMaximumCurrentLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* parameter is not optional for this message, check if it was set */
  if ( TRUE != Flag )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidTxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
  }

  /* DC_EVChargeParameter -> EVMaximumPowerLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  DC_EVChargeParameterTypePtr->EVMaximumPowerLimit =
    (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumPowerLimit(DC_EVChargeParameterTypePtr->EVMaximumPowerLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  DC_EVChargeParameterTypePtr->EVMaximumPowerLimitFlag = Flag;

  /* DC_EVChargeParameter -> EVMaximumVoltageLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  DC_EVChargeParameterTypePtr->EVMaximumVoltageLimit =
    (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumVoltageLimit(DC_EVChargeParameterTypePtr->EVMaximumVoltageLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* parameter is not optional for this message, check if it was set */
  if ( TRUE != Flag )
  {
    Scc_ReportErrorAndStatus(Scc_StackError_InvalidTxParameter, Scc_MsgStatus_ChargeParameterDiscovery_Failed);
  }

  /* EVEnergyCapacity */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  DC_EVChargeParameterTypePtr->EVEnergyCapacity =
    (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVEnergyCapacity(DC_EVChargeParameterTypePtr->EVEnergyCapacity, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  DC_EVChargeParameterTypePtr->EVEnergyCapacityFlag = Flag;

  /* EVEnergyRequest */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  DC_EVChargeParameterTypePtr->EVEnergyRequest =
    (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVEnergyRequest(DC_EVChargeParameterTypePtr->EVEnergyRequest, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  DC_EVChargeParameterTypePtr->EVEnergyRequestFlag = Flag;

  /* FullSOC */
  Scc_Get_DIN_FullSOC(&DC_EVChargeParameterTypePtr->FullSOC, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  DC_EVChargeParameterTypePtr->FullSOCFlag = Flag;

  /* BulkSOC */
  Scc_Get_DIN_BulkSOC(&DC_EVChargeParameterTypePtr->BulkSOC, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  DC_EVChargeParameterTypePtr->BulkSOCFlag = Flag;

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6050 */ /* MD_MSR_STPTH,MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VPowerDeliveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_PowerDeliveryReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_PowerDeliveryReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_PowerDeliveryReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;
  SCC_P2VARNOINIT(Exi_DIN_DC_EVPowerDeliveryParameterType) DC_EVPowerDeliveryParameter;

  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_POWER_DELIVERY_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_PowerDeliveryReqType);

  /* ReadyToChargeState */
  Scc_Get_DIN_ReadyToChargeState(&BodyPtr->ReadyToChargeState);

  /* ChargingProfile */
  Scc_Get_DIN_ChargingProfilePtr(&BodyPtr->ChargingProfile, &Flag);
  BodyPtr->ChargingProfileFlag = (Exi_BitType)Flag;

  /* check if the EVPowerDeliveryParameter shall be sent */
  Scc_Get_DIN_EVPowerDeliveryParameterFlag(&Flag);
  if ( TRUE == Flag )
  {
    /* get the pointer to the Exi_DIN_DC_EVPowerDeliveryParameterType */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    BodyPtr->EVPowerDeliveryParameter =
      (SCC_P2VARAPPLDATA(Exi_DIN_EVChargeParameterType)) &Scc_Exi_StructBuf[*BufIdxPtr];
    *BufIdxPtr += sizeof(Exi_DIN_DC_EVPowerDeliveryParameterType);
    DC_EVPowerDeliveryParameter =
      (SCC_P2VARAPPLDATA(Exi_DIN_DC_EVPowerDeliveryParameterType))(BodyPtr->EVPowerDeliveryParameter);

    /* DC_EVPowerDeliveryParameter */
    BodyPtr->EVPowerDeliveryParameterElementId = EXI_DIN_DC_EVPOWER_DELIVERY_PARAMETER_TYPE;
    BodyPtr->EVPowerDeliveryParameterFlag = 1;

    /* DC_EVPowerDeliveryParameter -> DC_EVStatus */
    Scc_ExiTx_DIN_WriteEVStatusDC(&DC_EVPowerDeliveryParameter->DC_EVStatus, BufIdxPtr);

    /* DC_EVPowerDeliveryParameter -> BulkChargingComplete */
    Scc_Get_DIN_BulkChargingComplete(&DC_EVPowerDeliveryParameter->BulkChargingComplete, &Flag);
    DC_EVPowerDeliveryParameter->BulkChargingCompleteFlag = Flag;

    /* DC_EVPowerDeliveryParameter -> ChargingComplete */
    Scc_Get_DIN_ChargingComplete(&DC_EVPowerDeliveryParameter->ChargingComplete);
  }
  else
  {
    BodyPtr->EVPowerDeliveryParameterFlag = 0;
  }

  return (Std_ReturnType)E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VSessionStopReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_SessionStopReq(void)
{
  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_SESSION_STOP_REQ_TYPE;
  /* *BufIdxPtr += sizeof(Exi_DIN_SessionStopReqType); */ /* change if SessionStopReq has elements */

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VCableCheckReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_CableCheckReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_CableCheckReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_CableCheckReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_CABLE_CHECK_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_CableCheckReqType);

  /* DC_EVStatus */
  Scc_ExiTx_DIN_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VPreChargeReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_PreChargeReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_PreChargeReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_PreChargeReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_PRE_CHARGE_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_PreChargeReqType);

  /* DC_EVStatus */
  Scc_ExiTx_DIN_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  /* EVTargetVoltage */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetVoltage = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVTargetVoltage(BodyPtr->EVTargetVoltage);

  /* EVTargetCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetCurrent = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVTargetCurrent(BodyPtr->EVTargetCurrent);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VCurrentDemandReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_CurrentDemandReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_CurrentDemandReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_CurrentDemandReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_CURRENT_DEMAND_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_CurrentDemandReqType);

  /* DC_EVStatus */
  Scc_ExiTx_DIN_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  /* EVTargetCurrent */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetCurrent = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVTargetCurrent(BodyPtr->EVTargetCurrent);

  /* EVMaximumVoltageLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumVoltageLimit = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumVoltageLimit(BodyPtr->EVMaximumVoltageLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumVoltageLimitFlag = Flag;

  /* EVMaximumCurrentLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumCurrentLimit = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumCurrentLimit(BodyPtr->EVMaximumCurrentLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumCurrentLimitFlag = Flag;

  /* EVMaximumPowerLimit */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVMaximumPowerLimit = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVMaximumPowerLimit(BodyPtr->EVMaximumPowerLimit, &Flag); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  BodyPtr->EVMaximumPowerLimitFlag = Flag;

  /* BulkChargingComplete */
  Scc_Get_DIN_BulkChargingComplete(&BodyPtr->BulkChargingComplete, &Flag);
  BodyPtr->BulkChargingCompleteFlag = Flag;

  /* ChargingComplete */
  Scc_Get_DIN_ChargingComplete(&BodyPtr->ChargingComplete);

  /* RemainingTimeToFullSoC */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->RemainingTimeToFullSoC = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_RemainingTimeToFullSoC(BodyPtr->RemainingTimeToFullSoC, &Flag);
  BodyPtr->RemainingTimeToFullSoCFlag = Flag;

  /* RemainingTimeToBulkSoC */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->RemainingTimeToBulkSoC = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_RemainingTimeToBulkSoC(BodyPtr->RemainingTimeToBulkSoC, &Flag);
  BodyPtr->RemainingTimeToBulkSoCFlag = Flag;

  /* EVTargetVoltage */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  BodyPtr->EVTargetVoltage = (SCC_P2VARAPPLDATA(Exi_DIN_PhysicalValueType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_PhysicalValueType);
  Scc_Get_DIN_EVTargetVoltage(BodyPtr->EVTargetVoltage);

  return (Std_ReturnType)E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VWeldingDetectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_ExiTx_DIN_WeldingDetectionReq(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  SCC_P2VARNOINIT(Exi_DIN_WeldingDetectionReqType) BodyPtr =
    (SCC_P2VARNOINIT(Exi_DIN_WeldingDetectionReqType))Scc_ExiTx_DIN_MsgPtr->Body->BodyElement;

  /* start to fill the exi struct */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Body->BodyElementElementId = EXI_DIN_WELDING_DETECTION_REQ_TYPE;
  *BufIdxPtr += sizeof(Exi_DIN_WeldingDetectionReqType);

  /* DC_EVStatus */
  Scc_ExiTx_DIN_WriteEVStatusDC(&BodyPtr->DC_EVStatus, BufIdxPtr);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  LOCAL HELPER FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VWriteHeader
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiTx_DIN_WriteHeader(SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  /* set the buffer for this new V2G message */
  *BufIdxPtr += sizeof(Exi_DIN_V2G_MessageType);
  /* set the buffer of the V2G message header */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Header = (SCC_P2VARAPPLDATA(Exi_DIN_MessageHeaderType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_MessageHeaderType);
  Scc_ExiTx_DIN_MsgPtr->Header->NotificationFlag = 0;
  Scc_ExiTx_DIN_MsgPtr->Header->SignatureFlag = 0;
  /* set the buffer of the SessionID */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_ExiTx_DIN_MsgPtr->Header->SessionID = (SCC_P2VARAPPLDATA(Exi_DIN_sessionIDType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_sessionIDType);
  Scc_ExiTx_DIN_MsgPtr->Header->SessionID->Length = Scc_SessionIDNvm[0];
  /* copy the SessionID to the buffer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
  IpBase_Copy((SCC_P2VARAPPLDATA(IpBase_CopyDataType)) &Scc_ExiTx_DIN_MsgPtr->Header->SessionID->Buffer[0],
    (SCC_P2CONSTAPPLDATA(IpBase_CopyDataType)) &Scc_SessionIDNvm[1], (uint32)Scc_SessionIDNvm[0]);

  return;
}

/**********************************************************************************************************************
 *  Scc_ExiTx_DIN_VWriteEVStatusDC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_ExiTx_DIN_WriteEVStatusDC(SCC_P2VARNOINIT(Exi_DIN_DC_EVStatusType) *EVStatePtrPtr,
  SCC_P2VARNOINIT(uint16) BufIdxPtr)
{
  boolean Flag = 0; /* PRQA S 3197 */ /* MD_Scc_3197 */

  /* create the root element */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  *EVStatePtrPtr = (SCC_P2VARNOINIT(Exi_DIN_DC_EVStatusType)) &Scc_Exi_StructBuf[*BufIdxPtr];
  *BufIdxPtr += sizeof(Exi_DIN_DC_EVStatusType);

  /* EVReady */
  Scc_Get_DIN_EVReady(&(*EVStatePtrPtr)->EVReady);

  /* EVCabinConditioning */
  Scc_Get_DIN_EVCabinConditioning(&(*EVStatePtrPtr)->EVCabinConditioning, &Flag);
  (*EVStatePtrPtr)->EVCabinConditioningFlag = Flag;

  /* EVRESSConditioning */
  Scc_Get_DIN_EVRESSConditioning(&(*EVStatePtrPtr)->EVRESSConditioning, &Flag);
  (*EVStatePtrPtr)->EVRESSConditioningFlag = Flag;

  /* EVErrorCode */ /* PRQA S 0310,3305 1 */ /* MD_Scc_0310_3305 */
  Scc_Get_DIN_EVErrorCode(&(*EVStatePtrPtr)->EVErrorCode);

  /* EVRESSSOC */
  Scc_Get_DIN_EVRESSSOC(&(*EVStatePtrPtr)->EVRESSSOC);

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
 *  END OF FILE: Scc_ExiTx_DIN.c
 *********************************************************************************************************************/
