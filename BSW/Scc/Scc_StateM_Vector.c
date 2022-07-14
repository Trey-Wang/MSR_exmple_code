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
/**        \file  Scc_StateM_Vector.c
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

#define SCC_STATEM_VECTOR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_ENABLE_STATE_MACHINE ) && ( SCC_ENABLE_STATE_MACHINE == STD_ON )

#include "Scc_StateM_Vector.h"

#include "Scc.h"
#include "Scc_Lcfg.h"
#include "Scc_Priv.h"
#include SCC_IF_CFG_H
#include SCC_CP_CFG_H

#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
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
/* PRQA S 3458 MACROS_BRACES */ /* MD_MSR_19.4 */
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */

#define Scc_StateM_MsgStateFct(MsgStateNew) \
  { Scc_StateM_MsgState = (MsgStateNew); Scc_Set_StateM_StateMachineMessageState(MsgStateNew); }

/* PRQA L:MACROS_BRACES */
/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct Scc_RxEVSEType
{
  uint16                                              ChargeServiceID;
  boolean                                             ChargeServiceFree;
  Scc_StateM_PaymentPrioritizationType                EVSEOfferedPaymentOptions;
  Scc_StateM_EVSEProcessingType                       EVSEProcessing;
  uint16                                              ProvServiceID;
  boolean                                             ReceiptRequired;
  Scc_SAPSchemaIDType                                 SAPSchemaID;
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
  SCC_P2CONSTNOINIT(Exi_ISO_ServiceParameterListType) ServiceParameterListPtr;
#endif /* SCC_SCHEMA_ISO */
} Scc_StateM_RxEVSEType;

typedef struct Scc_TxEVSEType
{
  Scc_StateM_ChargeStatusType       ChargeStatus;
  Scc_StateM_EnergyTransferModeType EnergyTransferMode;
  Scc_StateM_PaymentOptionType      SelectedPaymentOption;
  uint16                            ReqServiceID;
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
  Exi_ISO_SelectedServiceListType   SelectedServiceList;
  Exi_ISO_SelectedServiceType       SelectedServices[3];
#endif /* SCC_SCHEMA_ISO */
} Scc_StateM_TxEVSEType;

typedef struct Scc_CounterType
{
  uint16 ReconnectRetryCnt;
  uint16 GeneralDelayCnt;
  uint16 CommunicationSetupTimerCnt;
  uint16 ReadyToChargeTimerCnt;
  uint16 OngoingTimerCnt;
  uint16 IPAddressWaitTimerCnt;
  uint16 NvmReadTimerCnt;
  uint16 SequencePerformanceTimerCnt;
} Scc_StateM_CounterType;

typedef struct Scc_TxCoreType
{
  Scc_MsgTrigType     MsgTrig;
  boolean             CyclicMsgTrig;
  Scc_SDPSecurityType ReqSDPSecurity;
} Scc_StateM_TxCoreType;

typedef struct Scc_RxCoreType
{
  Scc_MsgStatusType   MsgStatus;
  boolean             CyclicMsgRcvd;
  Scc_StackErrorType  StackError;
  Scc_SDPSecurityType SupSDPSecurity;
} Scc_StateM_RxCoreType;

typedef struct Scc_LocalType
{
  Scc_StateM_ChargingControlType   ChargingControl;
  uint32                           EVTimeStamp;
  Scc_StateM_FunctionControlType   FunctionControl;
#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
  boolean                          FlowControl;
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
  Scc_StateM_ControlPilotStateType ControlPilotState;
  Scc_StateM_PWMStateType          PWMState;
  boolean                          SendChargingProfile;
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
  boolean                          InternetAvailable;
  boolean                          ReqInetDetails;
  boolean                          InetDetailsRequested;
#endif /* SCC_SCHEMA_ISO */
} Scc_StateM_LocalType;

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
/* 8bit variables */
#define SCC_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARZEROINIT(boolean) Scc_StateM_IPAssigned = FALSE;
STATIC SCC_VARZEROINIT(boolean) Scc_StateM_TrcvLinkState = FALSE;

#define SCC_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARZEROINIT(Scc_StateM_StateType)   Scc_StateM_State = Scc_SMS_Uninitialized;
STATIC SCC_VARZEROINIT(Scc_StateM_CounterType) Scc_StateM_Counter =
  { 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u };

#define SCC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARNOINIT(Scc_StateM_MsgStateType) Scc_StateM_MsgState;

STATIC SCC_VARNOINIT(Scc_StateM_TxEVSEType) Scc_StateM_TxEVSE;
STATIC SCC_VARNOINIT(Scc_StateM_RxEVSEType) Scc_StateM_RxEVSE;
STATIC SCC_VARNOINIT(Scc_StateM_TxCoreType) Scc_StateM_TxCore;
STATIC SCC_VARNOINIT(Scc_StateM_RxCoreType) Scc_StateM_RxCore;
STATIC SCC_VARNOINIT(Scc_StateM_LocalType)  Scc_StateM_Local;

STATIC SCC_VARNOINIT(Scc_StateM_ControlPilotStateType) Scc_StateM_LastControlPilotState;

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareSLAC(void);
#endif /* SCC_ENABLE_SLAC_HANDLING */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSECCDiscoveryProtocolReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareTlConnection(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSupportedAppProtocolReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSessionSetupReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareServiceDiscoveryReq(void);
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareServiceDetailReq(void);
#endif /* SCC_SCHEMA_ISO */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PreparePaymentServiceSelectionReq(void);
#if ( defined SCC_ENABLE_CERTIFICATE_INSTALLATION ) && ( SCC_ENABLE_CERTIFICATE_INSTALLATION == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCertificateInstallationReq(void);
#endif /* SCC_ENABLE_CERTIFICATE_INSTALLATION */
#if ( defined SCC_ENABLE_CERTIFICATE_UPDATE ) && ( SCC_ENABLE_CERTIFICATE_UPDATE == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCertificateUpdateReq(void);
#endif /* SCC_ENABLE_CERTIFICATE_UPDATE */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareAuthorizationReq(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareChargeParameterDiscoveryReq(void);
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareCableCheckReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PreparePreChargeReq(void);
#endif /* SCC_CHARGING_DC */
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PreparePowerDeliveryReq(void);
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareChargingStatusReq(void);
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCurrentDemandReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareWeldingDetectionReq(void);
#endif /* SCC_CHARGING_DC */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSessionStopReq(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareStopCommunicationSession(void);

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSLAC(void);
#endif /* SCC_ENABLE_SLAC_HANDLING */
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSECCDiscoveryProtocolRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessTLConnected(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSupportedAppProtocolRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSessionSetupRes(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessServiceDiscoveryRes(void);
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessServiceDetailRes(void);
#endif /* SCC_SCHEMA_ISO */
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPaymentServiceSelectionRes(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessAuthorizationRes(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessChargeParameterDiscoveryRes(void);
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessCableCheckRes(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPreChargeRes(void);
#endif /* SCC_CHARGING_DC */
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPowerDeliveryRes(void);
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessChargingStatusRes(void);
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessCurrentDemandRes(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessWeldingDetectionRes(void);
#endif /* SCC_CHARGING_DC */
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessSessionStopRes(void);
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessStopCommunicationSession(void);

SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_GlobalTimerChecks(void);
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_GlobalParamsChecks(void);
SCC_STATICFUNCCODE(void) Scc_StateM_InitParams(void);
SCC_STATICFUNCCODE(void) Scc_StateM_ErrorHandling(Scc_StateM_StateMachineErrorType ErrorReason);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Scc_StateM_InitMemory
 *********************************************************************************************************************/
/*! \brief      initializes global variables
 *  \pre        has to be called before any other calls to the module
 *  \context    initialization
 *  \note       AUTOSAR extension
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_StateM_InitMemory(void)
{
  Scc_StateM_State = Scc_SMS_Uninitialized;
  Scc_StateM_IPAssigned = FALSE;

  Scc_StateM_Counter.CommunicationSetupTimerCnt = 0;
  Scc_StateM_Counter.GeneralDelayCnt = 0;
  Scc_StateM_Counter.IPAddressWaitTimerCnt = 0;
  Scc_StateM_Counter.NvmReadTimerCnt = 0;
  Scc_StateM_Counter.OngoingTimerCnt = 0;
  Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
  Scc_StateM_Counter.ReconnectRetryCnt = 0;
  Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;

  return;
}

/**********************************************************************************************************************
 *  Scc_StateM_Init
 *********************************************************************************************************************/
/*! \brief      init the values of the module
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_StateM_Init(void)
{
  Scc_StateM_State = Scc_SMS_Initialized;

  Scc_StateM_Counter.ReconnectRetryCnt = (uint16)Scc_ConfigValue_StateM_ReconnectRetries;

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  Scc_StateM_Local.ReqInetDetails = (boolean)Scc_ConfigValue_StateM_RequestInternetDetails;
#endif /* SCC_SCHEMA_ISO */

  Scc_StateM_LastControlPilotState = Scc_ControlPilotState_State_A_E_F;

  /* init the parameters */
  Scc_StateM_InitParams();

  return;
}

/**********************************************************************************************************************
 *  Scc_StateM_MainFunction
 *********************************************************************************************************************/
/*! \brief      main function of the state machine
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_StateM_MainFunction(void)
{
  /* check if module was already initialized */
  if ( Scc_SMS_Uninitialized < Scc_StateM_State )
  {

    /* check if global timers elapsed */
    if ( E_OK != Scc_StateM_GlobalTimerChecks() )
    {
      /* error occurred, return (core needs to reset itself before receiving new commands) */
      return;
    }
    /* check if global parameters are in invalid states */
    if ( Scc_ReturnType_OK != Scc_StateM_GlobalParamsChecks() )
    {
      /* error occurred, return (core needs to reset itself before receiving new commands) */
      return;
    }

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
    /* check if the state machine is waiting for the application */
    if ( Scc_SMMS_WaitingForApplication == Scc_StateM_MsgState )
    {
      /* get the flow control from the application */
      Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
      /* check if the state machine has to wait */
      if ( TRUE == Scc_StateM_Local.FlowControl )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)
      }
    }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
  }

  /* SCC is initialized */
  if ( Scc_SMS_Initialized == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_Initialized);
    /* update the FunctionControl value */
    Scc_Get_StateM_FunctionControl(&Scc_StateM_Local.FunctionControl);
    /* check if the module was signalized to get ready */
    if ( Scc_FunctionControl_ChargingMode == Scc_StateM_Local.FunctionControl )
    {

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
      /* set the new internal state */
      Scc_StateM_State = Scc_SMS_SLAC;
#else
      /* set the new internal state */
      Scc_StateM_State = Scc_SMS_WaitForIPAddress;
#endif /* SCC_ENABLE_SLAC_HANDLING */
    }
  }

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
  if ( Scc_SMS_SLAC == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_SLAC);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_PrepareSLAC() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      /* if the response was received successfully */
      if ( Scc_MsgStatus_SLAC_OK == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_SLAC_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the communication session */
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessSLAC() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }
#endif /* SCC_ENABLE_SLAC_HANDLING */

  /* Wait until the IP address is assigned */
  if ( Scc_SMS_WaitForIPAddress == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_WaitForIP);
    /* check if an IPv6 address has been assigned */
    if ( TRUE == Scc_StateM_IPAssigned )
    {
      /* stop the IP wait timer */
      Scc_StateM_Counter.IPAddressWaitTimerCnt = 0;
      /* continue with the SDP */
      Scc_StateM_State = Scc_SMS_SECCDiscoveryProtocol;
    }
    /* IPv6 address has not been assigned yet */
    else
    {
      /* check if the IP address timer is active */
      if ( 0u < Scc_StateM_Counter.IPAddressWaitTimerCnt )
      {
        /* decrement the timer */
        Scc_StateM_Counter.IPAddressWaitTimerCnt--;
        /* check if the timer elapsed */
        if ( 0u == Scc_StateM_Counter.IPAddressWaitTimerCnt )
        {
          /* start error handling */
          Scc_StateM_ErrorHandling(Scc_SMER_Timer_IPAddressWaitTimer);
        }
      }
      /* if the timer was not started yet */ /* PRQA S 3201,3325 2 */ /* MD_Scc_3109_3201_3355_3358 */ 
      else if ( 0u < (uint16)Scc_ConfigValue_Timer_General_IPAddressWaitTimeout ) /* PRQA S 2004 */ /* MD_Scc_2004 */
      {
        /* set the timer to the configured timeout and start it */
        Scc_StateM_Counter.IPAddressWaitTimerCnt = (uint16)Scc_ConfigValue_Timer_General_IPAddressWaitTimeout;
      }
    }
  }

  /* SECC Discovery Protocol */
  if ( Scc_SMS_SECCDiscoveryProtocol == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_SECCDiscoveryProtocol);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareSECCDiscoveryProtocolReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      /* if the response was received successfully */
      if ( Scc_MsgStatus_SECCDiscoveryProtocol_OK == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_SECCDiscoveryProtocol_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the communication session */
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessSECCDiscoveryProtocolRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Transport Layer */
  if ( Scc_SMS_TLConnection == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_TLConnection);
    /* prepare the establishment of the transport layer connection */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareTlConnection() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
      }
    }
    /* wait for the transport layer to be connected */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      /* if the connection establishment of either TLS or TCP was successful */
      if ( Scc_MsgStatus_TransportLayer_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_TransportLayer_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the result of the connection establishment */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessTLConnected() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Supported App Protocol */
  if ( Scc_SMS_SupportedAppProtocol == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_SupportedAppProtocol);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareSupportedAppProtocolReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_SupportedAppProtocol_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_SupportedAppProtocol_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessSupportedAppProtocolRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Session Setup */
  if ( Scc_SMS_SessionSetup == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_SessionSetup);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareSessionSetupReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_SessionSetup_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the CommunicationSetup timer */
        Scc_StateM_Counter.CommunicationSetupTimerCnt = 0;

        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_SessionSetup_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the CommunicationSetup timer */
        Scc_StateM_Counter.CommunicationSetupTimerCnt = 0;
        /* start error handling */
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessSessionSetupRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Service Discovery */
  if ( Scc_SMS_ServiceDiscovery == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_ServiceDiscovery);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareServiceDiscoveryReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_ServiceDiscovery_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_ServiceDiscovery_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      /* process the response */
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessServiceDiscoveryRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  /* Service Detail */
  if ( Scc_SMS_ServiceDetail == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_ServiceDetail);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareServiceDetailReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_ServiceDetail_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_ServiceDetail_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessServiceDetailRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }
#endif /* SCC_SCHEMA_ISO */

  /* Payment Service Selection */
  if ( Scc_SMS_PaymentServiceSelection == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_PaymentServiceSelection);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PreparePaymentServiceSelectionReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_PaymentServiceSelection_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_PaymentServiceSelection_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessPaymentServiceSelectionRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Authorization */
  if ( Scc_SMS_Authorization == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_Authorization);
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
    /* check if the cycle timeout is active */
    if ( Scc_SAPSchemaIDs_ISO == Scc_StateM_RxEVSE.SAPSchemaID )
    {
      /* check the cycle timeout */
      if ( 0u < Scc_StateM_Counter.OngoingTimerCnt )
      {
        Scc_StateM_Counter.OngoingTimerCnt--;

        if ( 0u == Scc_StateM_Counter.OngoingTimerCnt )
        {
          Scc_StateM_ErrorHandling(Scc_SMER_Timer_AuthorizationOngoingTimer);
          return;
        }
      }
      /* if the cycle timer is inactive, check if it shall be enabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_EVSEProcessing_Finished != Scc_StateM_RxEVSE.EVSEProcessing )
      {
        Scc_StateM_Counter.OngoingTimerCnt = (uint16)Scc_ConfigValue_StateM_AuthorizationOngoingTimeout;
      }
    }
#endif /* SCC_SCHEMA_ISO */
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareAuthorizationReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_Authorization_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_Authorization_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessAuthorizationRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Charge Parameter Discovery */
  if ( Scc_SMS_ChargeParameterDiscovery == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_ChargeParameterDiscovery);
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
    /* check if the cycle timeout is active */
    if ( Scc_SAPSchemaIDs_ISO == Scc_StateM_RxEVSE.SAPSchemaID )
    {
      /* check the cycle timeout */
      if ( 0u < Scc_StateM_Counter.OngoingTimerCnt )
      {
        Scc_StateM_Counter.OngoingTimerCnt--;

        if ( 0u == Scc_StateM_Counter.OngoingTimerCnt )
        {
          Scc_StateM_ErrorHandling(Scc_SMER_Timer_ChargeParameterDiscoveryOngoingTimer);
          return;
        }
      }
      /* if the cycle timer is inactive, check if it shall be enabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_EVSEProcessing_Finished != Scc_StateM_RxEVSE.EVSEProcessing )
      {
        Scc_StateM_Counter.OngoingTimerCnt =
          (uint16)Scc_ConfigValue_StateM_ChargeParameterDiscoveryOngoingTimeout;
      }
    }
#endif /* SCC_SCHEMA_ISO */
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_PrepareChargeParameterDiscoveryReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_ChargeParameterDiscovery_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_ChargeParameterDiscovery_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessChargeParameterDiscoveryRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

  /* Cable Check */
  if ( Scc_SMS_CableCheck == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_CableCheck);
    /* check the cycle timeout */
    if ( 0u < Scc_StateM_Counter.OngoingTimerCnt )
    {
      Scc_StateM_Counter.OngoingTimerCnt--;

      if ( 0u == Scc_StateM_Counter.OngoingTimerCnt )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_Timer_CableCheckOngoingTimer);
        return;
      }
    }
    /* if the cycle timer is inactive, check if it shall be enabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( Scc_EVSEProcessing_Finished != Scc_StateM_RxEVSE.EVSEProcessing )
    {
      Scc_StateM_Counter.OngoingTimerCnt = (uint16)Scc_ConfigValue_StateM_CableCheckOngoingTimeout;
    }
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_PrepareCableCheckReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_CableCheck_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_CableCheck_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessCableCheckRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* PreCharge */
  if ( Scc_SMS_PreCharge == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_PreCharge);
    /* check the cycle timeout */
    if ( 0u < Scc_StateM_Counter.OngoingTimerCnt )
    {
      Scc_StateM_Counter.OngoingTimerCnt--;

      if ( 0u == Scc_StateM_Counter.OngoingTimerCnt )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_Timer_PreChargeOngoingTimer);
        return;
      }
    }
    /* if the cycle timer is inactive, check if it shall be enabled */
    else
    {
      Scc_StateM_Counter.OngoingTimerCnt = (uint16)Scc_ConfigValue_StateM_PreChargeOngoingTimeout;
    }
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PreparePreChargeReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_PreCharge_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_PreCharge_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessPreChargeRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

#endif /* SCC_CHARGING_DC */

  /* Power Delivery */
  if ( Scc_SMS_PowerDelivery == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_PowerDelivery);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_PreparePowerDeliveryReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_PowerDelivery_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the ReadyToCharge timer */
        Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_PowerDelivery_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        /* stop the ReadyToCharge timer */
        Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
        /* start error handling */
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessPowerDeliveryRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  /* Charging Status */
  if ( Scc_SMS_ChargingStatus == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_ChargingStatus);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareChargingStatusReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_ChargingStatus_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_ChargingStatus_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessChargingStatusRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

  /* Current Demand */
  if ( Scc_SMS_CurrentDemand == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_CurrentDemand);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareCurrentDemandReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_CurrentDemand_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_CurrentDemand_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessCurrentDemandRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Welding Detection */
  if ( Scc_SMS_WeldingDetection == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_WeldingDetection);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareWeldingDetectionReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if (   ( Scc_MsgStatus_WeldingDetection_OK == Scc_StateM_RxCore.MsgStatus )
          && ( TRUE == Scc_StateM_RxCore.CyclicMsgRcvd ))
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
        /* reset the cyclic msg status */
        Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_WeldingDetection_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessWeldingDetectionRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

#endif /* SCC_CHARGING_DC */

  /* Session Stop */
  if ( Scc_SMS_SessionStop == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_SessionStop);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareSessionStopReq() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
        /* stop the sequence performance timeout */
        Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_SessionStop_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_SessionStop_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( Scc_ReturnType_OK == Scc_StateM_ProcessSessionStopRes() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)

#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
        /* get the flow control from the application */
        Scc_Get_StateM_FlowControl(&Scc_StateM_Local.FlowControl);
        /* check if the state machine has to wait */
        if ( TRUE != Scc_StateM_Local.FlowControl )
        {
          /* set the msg state */
          Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
        }
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
      }
    }
  }

  /* Stop Communication Session */
  if ( Scc_SMS_StopCommunicationSession == Scc_StateM_State )
  {
    /* report the current status to the application */
    Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_StopCommunicationSession);
    /* prepare the request */
    if ( Scc_SMMS_PreparingRequest == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_PrepareStopCommunicationSession() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForResponse)
      }
    }
    /* wait for the response */
    if ( Scc_SMMS_WaitingForResponse == Scc_StateM_MsgState )
    {
      if ( Scc_MsgStatus_StopCommunicationSession_OK == Scc_StateM_RxCore.MsgStatus )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_ProcessingResponse)
      }
      /* if an error occured */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( Scc_MsgStatus_StopCommunicationSession_Failed == Scc_StateM_RxCore.MsgStatus )
      {
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
      }
    }
    /* process the response */
    if ( Scc_SMMS_ProcessingResponse == Scc_StateM_MsgState )
    {
      if ( (Std_ReturnType)E_OK == Scc_StateM_ProcessStopCommunicationSession() )
      {
        /* set the msg state */
        Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)
      }
    }
  }

  /* Scc Stopped */
  if ( Scc_SMS_Stopped == Scc_StateM_State )
  {
    /* read the FunctionControl value */
    Scc_Get_StateM_FunctionControl(&Scc_StateM_Local.FunctionControl);

    /* check if the application requests an immediate restart */
    if ( Scc_FunctionControl_Reset == Scc_StateM_Local.FunctionControl )
    {
      /* reinitialize the volatile parameters */
      Scc_StateM_InitParams();
      /* go back to the init state */
      Scc_StateM_State = Scc_SMS_Initialized;
    }
    else
    {
      /* check if a reconnect was started with a delay */
      if ( 0 < Scc_StateM_Counter.GeneralDelayCnt )
      {
        Scc_StateM_Counter.GeneralDelayCnt--;
        /* if the timer has elapsed, start the reconnect */
        if ( 0 == Scc_StateM_Counter.GeneralDelayCnt )
        {
          Scc_StateM_InitParams();
          if ( FALSE == Scc_StateM_TrcvLinkState )
          {
            Scc_StateM_State = Scc_SMS_SLAC;
          }
          else
          {
            Scc_StateM_State = Scc_SMS_WaitForIPAddress;
          }
        }
      }
    }
  }

  return;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_StateM_Set***
 *********************************************************************************************************************/
/*! \brief      Scc -> Scc_StateMachine
 *  \context    task level
 *********************************************************************************************************************/
/* message independent parameters */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_TrcvLinkState(boolean LinkStateActive)
{
  if ( TRUE == LinkStateActive )
  {
    Scc_StateM_TrcvLinkState = TRUE;
    /* start the communication setup timer */
    Scc_StateM_Counter.CommunicationSetupTimerCnt = 1;
    /* start the ready to charge timer */
    Scc_StateM_Counter.ReadyToChargeTimerCnt = 1;
  }
  else
  {
    Scc_StateM_TrcvLinkState = FALSE;
    Scc_StateM_IPAssigned = FALSE;
    /* stop the communication setup timer */
    Scc_StateM_Counter.CommunicationSetupTimerCnt = 0;
    /* stop the ready to charge timer */
    Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
  }

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_Core_IPAssigned(boolean IPAssigned)
{
  if ( TRUE == IPAssigned )
  {
    Scc_StateM_IPAssigned = TRUE;
  }
  else
  {
    Scc_StateM_IPAssigned = FALSE;
  }

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_Core_CyclicMsgTrigTx(boolean CyclicMsgTrig)
{
  Scc_StateM_TxCore.CyclicMsgTrig = CyclicMsgTrig;

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_Core_CyclicMsgRcvd(boolean CyclicMsgRcvd)
{
  Scc_StateM_RxCore.CyclicMsgRcvd = CyclicMsgRcvd;

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_Core_MsgStatus(Scc_MsgStatusType MsgStatus)
{
  /* check if the msg status is positive */
  if (   ( 1u == ( MsgStatus % 2u ) )
      && ( Scc_MsgStatus_SupportedAppProtocol_OK <= MsgStatus )
      && ( Scc_MsgStatus_SessionStop_OK > MsgStatus ))
  {
    /* check which schema is used */
    switch ( Scc_StateM_RxEVSE.SAPSchemaID )
    {
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
    case Scc_SAPSchemaIDs_ISO:
      /* start the sequence performance timeout */
      Scc_StateM_Counter.SequencePerformanceTimerCnt =
        (uint16)Scc_ConfigValue_Timer_ISO_SequencePerformanceTimeout;
      break;
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
    case Scc_SAPSchemaIDs_DIN:
      /* start the sequence performance timeout */
      Scc_StateM_Counter.SequencePerformanceTimerCnt =
        (uint16)Scc_ConfigValue_Timer_DIN_SequencePerformanceTimeout;
      break;
#endif /* SCC_SCHEMA_DIN */

    default:
      /* this is an invalid state */
      Scc_CallDetReportError(SCC_API_ID_V_STATEM_SET_MSG_STATUS, SCC_DET_INV_PARAM);
      break;
    }
  }
  else
  {
    /* stop the sequence performance timeout */
    Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;
  }

  Scc_StateM_RxCore.MsgStatus = MsgStatus;

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_Core_StackError(Scc_StackErrorType StackError)
{
  Scc_StateM_RxCore.StackError = StackError;

  return;
}

/* multiple messages */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_EVSEProcessing(Exi_ISO_EVSEProcessingType EVSEProcessing)
{
  switch ( EVSEProcessing )
  {
  case EXI_ISO_EVSEPROCESSING_TYPE_ONGOING:
  case EXI_ISO_EVSEPROCESSING_TYPE_ONGOING_WAITING_FOR_CUSTOMER_INTERACTION:
    Scc_StateM_RxEVSE.EVSEProcessing = Scc_EVSEProcessing_Ongoing;
    break;

  default: /* case EXI_ISO_EVSEPROCESSING_TYPE_FINISHED: */
    Scc_StateM_RxEVSE.EVSEProcessing = Scc_EVSEProcessing_Finished;
    break;
  }

  return;
}
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_DIN_EVSEProcessing(Exi_DIN_EVSEProcessingType EVSEProcessing)
{
  switch ( EVSEProcessing )
  {
  case EXI_DIN_EVSEPROCESSING_TYPE_ONGOING:
    Scc_StateM_RxEVSE.EVSEProcessing = Scc_EVSEProcessing_Ongoing;
    break;

  default: /* case EXI_DIN_EVSEPROCESSING_TYPE_FINISHED: */
    Scc_StateM_RxEVSE.EVSEProcessing = Scc_EVSEProcessing_Finished;
    break;
  }

  return;
}
#endif /* SCC_SCHEMA_DIN */

/* SECC Discovery Protocol */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_SDPSecurityTx(Scc_SDPSecurityType SDPSecurity)
{
  Scc_StateM_RxCore.SupSDPSecurity = SDPSecurity;

  return;
}

/* Supported App Protocol */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_SAPSchemaID(Scc_SAPSchemaIDType SAPSchemaID)
{
  Scc_StateM_RxEVSE.SAPSchemaID = SAPSchemaID;

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  /* check if ISO was selected */
  if ( Scc_SAPSchemaIDs_ISO == Scc_StateM_RxEVSE.SAPSchemaID )
  {
    /* disable the ReadyToChargeTimer */
    Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
  }
#endif /* SCC_SCHEMA_ISO */

  return;
}

/* Service Discovery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_PaymentOptionList(SCC_P2CONSTNOINIT(Exi_ISO_PaymentOptionListType) PaymentOptionList)
{
  uint8_least counter;
  boolean ContractFound = FALSE;
  boolean ExternalPaymentFound = FALSE;

  for (counter = 0; counter < PaymentOptionList->PaymentOptionCount; counter++)
  {
    /* check if contract based payment was offered */
    if ( EXI_ISO_PAYMENT_OPTION_TYPE_CONTRACT == PaymentOptionList->PaymentOption[counter] )
    {
      ContractFound = TRUE;
    }
    /* check if external payment was offered */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( EXI_ISO_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT == PaymentOptionList->PaymentOption[counter] )
    {
      ExternalPaymentFound = TRUE;
    }
  }

  /* check if only contract based payment was found */
  if (   (TRUE == ContractFound)
      && (FALSE == ExternalPaymentFound) )
  {
    Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions = Scc_PaymentPrioritization_OnlyPnC;
  }
  /* check if only external payment was found */
  else if (   (FALSE == ContractFound)
           && (TRUE == ExternalPaymentFound) )
  {
    Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions = Scc_PaymentPrioritization_OnlyEIM;
  }
  /* check if both contract based and external payment was found */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
  else if (   (TRUE == ContractFound)
           && (TRUE == ExternalPaymentFound) )
  {
    Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions = Scc_PaymentPrioritization_PrioritizePnC;
  }

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ChargeService(SCC_P2CONSTNOINIT(Exi_ISO_ChargeServiceType) ChargeService)
{
  uint8_least Counter;
  uint8       EnergyTransferModeFlags = 0;

  Scc_StateM_RxEVSE.ChargeServiceID = ChargeService->ServiceID;
  Scc_StateM_RxEVSE.ChargeServiceFree = ChargeService->FreeService;

  /* step through all supported energy transfer modes */
  for ( Counter = 0; Counter < ChargeService->SupportedEnergyTransferMode->EnergyTransferModeCount; Counter++ )
  {
    switch ( ChargeService->SupportedEnergyTransferMode->EnergyTransferMode[Counter] )
    {
    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_AC_1P;
      break;

    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_AC_3P;
      break;

    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_CORE:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Core;
      break;

    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_EXTENDED:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Extended;
      break;

    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_COMBO_CORE:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Combo_Core;
      break;

    case EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_UNIQUE:
      EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Unique;
      break;

      /* invalid EnergyTransferMode */
    default:
      Scc_StateM_ErrorHandling(Scc_SMER_ServiceDiscovery_InvalidEnergyTransferMode);
      break;
    }
  }

  /* provide the supported energy transfer modes to the application */
  Scc_Set_StateM_EnergyTransferModeFlags(EnergyTransferModeFlags);

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceList(SCC_P2CONSTNOINIT(Exi_ISO_ServiceListType) ServiceList, Exi_BitType Flag)
{
  if ( TRUE == Flag )
  {
    boolean OffersInternetService = FALSE;
    Exi_ISO_ServiceType *ServicePtr = ServiceList->Service;

    /* step through all available services */
    while ( ServicePtr != NULL_PTR )
    {
      /* only selected free services */
      if ( TRUE == ServicePtr->FreeService )
      {
        /* check if a internet service is available */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
        if ( Scc_ServiceID_Internet == ServicePtr->ServiceID )
        {
          OffersInternetService = TRUE;
        }
      }
      /* select the next service */
      ServicePtr = ServicePtr->NextServicePtr;
    }
    /* check if internet service was not found */
    if ( OffersInternetService == FALSE )
    {
      Scc_StateM_Local.ReqInetDetails = FALSE;
    }
  }
  else
  {
    Scc_StateM_Local.ReqInetDetails = FALSE;
  }

  return;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_DIN_ChargeService(SCC_P2CONSTNOINIT(Exi_DIN_ServiceChargeType) ChargeService)
{
  uint8 EnergyTransferModeFlags = 0;

  Scc_StateM_RxEVSE.ChargeServiceID = ChargeService->ServiceTag->ServiceID;
  Scc_StateM_RxEVSE.ChargeServiceFree = ChargeService->FreeService;

  switch ( ChargeService->EnergyTransferType )
  {
  case EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_CORE:
    EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Core;
    break;

  case EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_EXTENDED:
    EnergyTransferModeFlags |= Scc_EnergyTransferMode_DC_Extended;
    break;

    /* invalid EnergyTransferType */
  default:
    Scc_StateM_ErrorHandling(Scc_SMER_ServiceDiscovery_InvalidEnergyTransferMode);
    break;
  }

  /* provide the supported energy transfer modes to the application */
  Scc_Set_StateM_EnergyTransferModeFlags(EnergyTransferModeFlags);

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  /* disable VAS */
  Scc_StateM_Local.ReqInetDetails = FALSE;
#endif /* SCC_SCHEMA_ISO */

  return;
}
#endif /* SCC_SCHEMA_DIN */

#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
/* ServiceDetail */
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceIDTx(uint16 ServiceID)
{
  Scc_StateM_RxEVSE.ProvServiceID = ServiceID;

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceParameterList(
  SCC_P2CONSTNOINIT(Exi_ISO_ServiceParameterListType) ServiceParameterList, Exi_BitType ServiceParameterListFlag)
{
  if ( 1 == ServiceParameterListFlag )
  {
    Scc_StateM_RxEVSE.ServiceParameterListPtr = ServiceParameterList;
  }
  else
  {
    Scc_StateM_RxEVSE.ServiceParameterListPtr = (Exi_ISO_ServiceParameterListType*)NULL_PTR;
  }

  return;
}
#endif /* SCC_SCHEMA_ISO */

/**********************************************************************************************************************
 *  Scc_StateM_Get***
 *********************************************************************************************************************/
/*! \brief      Scc <- Scc_StateMachine
 *  \context    task level
 *********************************************************************************************************************/
/* message independent parameters */
SCC_FUNCCODE(void) Scc_StateM_Get_Core_MsgTrig(SCC_P2VARNOINIT(Scc_MsgTrigType) MsgTrig)
{
  *MsgTrig = Scc_StateM_TxCore.MsgTrig;

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Get_Core_CyclicMsgTrigRx(SCC_P2VARNOINIT(boolean) CyclicMsgTrig)
{
  *CyclicMsgTrig = Scc_StateM_TxCore.CyclicMsgTrig;

  return;
}

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
/* SLAC */
SCC_FUNCCODE(void) Scc_StateM_Get_SLAC_QCAIdleTimer(SCC_P2VARNOINIT(uint16) QCAIdleTimer)
{
  *QCAIdleTimer = (uint16)Scc_ConfigValue_StateM_QCAIdleTimer;
}
SCC_FUNCCODE(void) Scc_StateM_Get_SLAC_StartMode(SCC_P2VARNOINIT(EthTrcv_30_Ar7000_Slac_StartModeType) SLACStartMode)
{
  *SLACStartMode = (EthTrcv_30_Ar7000_Slac_StartModeType)Scc_ConfigValue_StateM_SLACStartMode;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

/* SECC Discovery Protocol */
SCC_FUNCCODE(void) Scc_StateM_Get_Core_SDPSecurityRx(SCC_P2VARNOINIT(Scc_SDPSecurityType) SDPSecurity)
{
  *SDPSecurity = Scc_StateM_TxCore.ReqSDPSecurity;

  return;
}

/* Service Discovery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceScope(SCC_P2VARNOINIT(Exi_ISO_serviceScopeType) ServiceScope,
  SCC_P2VARNOINIT(boolean) Flag)
{
  *Flag = FALSE;

  SCC_DUMMY_STATEMENT(ServiceScope); /* PRQA S 3112,3199 */ /* MD_MSR_14.2,MD_Scc_3198_3199 */

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceCategory(SCC_P2VARNOINIT(Exi_ISO_serviceCategoryType) ServiceCategory,
  SCC_P2VARNOINIT(boolean) Flag)
{
  *Flag = FALSE;

  SCC_DUMMY_STATEMENT(ServiceCategory); /* PRQA S 3112,3199 */ /* MD_MSR_14.2,MD_Scc_3198_3199 */

  return;
}
#endif /* SCC_SCHEMA_ISO */

#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
/* Service Detail */
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceIDRx(SCC_P2VARNOINIT(uint16) ServiceID)
{
  *ServiceID = Scc_StateM_TxEVSE.ReqServiceID;

  return;
}
#endif /* SCC_SCHEMA_ISO */

/* Payment Service Selection */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_SelectedPaymentOption(SCC_P2VARNOINIT(Exi_ISO_paymentOptionType) SelectedPaymentOption)
{
  switch ( Scc_StateM_TxEVSE.SelectedPaymentOption )
  {
  case Scc_PaymentOption_PnC:
    *SelectedPaymentOption = EXI_ISO_PAYMENT_OPTION_TYPE_CONTRACT;
    break;

  default: /* case Scc_PaymentOption_EIM: */
    *SelectedPaymentOption = EXI_ISO_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT;
    break;
  }

  return;
}
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_SelectedServiceListPtr(SCC_P2VARNOINIT(Exi_ISO_SelectedServiceListType*) SelectedServiceListPtr)
{
  Scc_StateM_TxEVSE.SelectedServiceList.SelectedService = &Scc_StateM_TxEVSE.SelectedServices[0];
  (*SelectedServiceListPtr) = &Scc_StateM_TxEVSE.SelectedServiceList;

  return;
}
#endif /* SCC_SCHEMA_ISO */

/* Charge Parameter Discovery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_RequestedEnergyTransferMode(SCC_P2VARNOINIT(Exi_ISO_EnergyTransferModeType) EnergyTransferMode)
{
  switch ( Scc_StateM_TxEVSE.EnergyTransferMode )
  {
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )

    case Scc_EnergyTransferMode_AC_1P:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE;
      break;

    case Scc_EnergyTransferMode_AC_3P:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE;
      break;

#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

    case Scc_EnergyTransferMode_DC_Core:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_CORE;
      break;

    case Scc_EnergyTransferMode_DC_Extended:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_EXTENDED;
      break;

    case Scc_EnergyTransferMode_DC_Combo_Core:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_COMBO_CORE;
      break;

    case Scc_EnergyTransferMode_DC_Unique:
      *EnergyTransferMode = EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_UNIQUE;
      break;

#endif /* SCC_CHARGING_DC */

    default:
      Scc_CallDetReportError(SCC_API_ID_V_STATEM_CORE_GET_CBK, SCC_DET_INV_PARAM)
      break;
  }

  return;
}
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_DIN_RequestedEnergyTransferMode(SCC_P2VARNOINIT(Exi_DIN_EVRequestedEnergyTransferType) EnergyTransferMode)
{
  switch ( Scc_StateM_TxEVSE.EnergyTransferMode )
  {
    case Scc_EnergyTransferMode_DC_Core:
      *EnergyTransferMode = EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_CORE;
      break;

    case Scc_EnergyTransferMode_DC_Extended:
      *EnergyTransferMode = EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_EXTENDED;
      break;

    default:
      Scc_CallDetReportError(SCC_API_ID_V_STATEM_CORE_GET_CBK, SCC_DET_INV_PARAM)
      break;
  }

  return;
}
#endif /* SCC_SCHEMA_DIN */

/* Power Delivery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ChargeProgress(SCC_P2VARNOINIT(Exi_ISO_chargeProgressType) ChargeProgress)
{
  switch ( Scc_StateM_TxEVSE.ChargeStatus )
  {
  case Scc_ChargeStatus_Starting:
    *ChargeProgress = EXI_ISO_CHARGE_PROGRESS_TYPE_START;
    break;

  case Scc_ChargeStatus_Stopping:
    *ChargeProgress = EXI_ISO_CHARGE_PROGRESS_TYPE_STOP;
    break;

  case Scc_ChargeStatus_Renegotiating:
    *ChargeProgress = EXI_ISO_CHARGE_PROGRESS_TYPE_RENEGOTIATE;
    break;

  default:
    Scc_CallDetReportError(SCC_API_ID_V_STATEM_CORE_GET_CBK, SCC_DET_INV_PARAM)
    break;
  }

  return;
}
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_DIN_ReadyToChargeState(SCC_P2VARNOINIT(boolean) ReadyToChargeState)
{
  switch ( Scc_StateM_TxEVSE.ChargeStatus )
  {
  case Scc_ChargeStatus_Starting:
    *ReadyToChargeState = TRUE;
    break;

  case Scc_ChargeStatus_Stopping:
    *ReadyToChargeState = FALSE;
    break;

  default:
    Scc_CallDetReportError(SCC_API_ID_V_STATEM_CORE_GET_CBK, SCC_DET_INV_PARAM)
    break;
  }

  return;
}
#endif /* SCC_SCHEMA_DIN */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareSLAC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareSLAC(void)
{
  /* get the current status of the cable */
  Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
  /* wait for State B, C or D */
  if ( Scc_ControlPilotState_State_B > Scc_StateM_Local.ControlPilotState )
  {
    return Scc_ReturnType_Pending;
  }

  /* start SLAC */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_SLAC;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return Scc_ReturnType_OK;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareSECCDiscoveryProtocolReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSECCDiscoveryProtocolReq(void)
{
#if (( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON ))
  Scc_StateM_TxCore.ReqSDPSecurity = Scc_SDPSecurity_Tls;
#else
  Scc_StateM_TxCore.ReqSDPSecurity = Scc_SDPSecurity_None;
#endif /* SCC_ENABLE_TLS */

  /* trigger the SDP request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_SECCDiscoveryProtocol;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareTlConnection
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareTlConnection(void)
{
  /* trigger the TL connection establishment */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_TransportLayer;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareSupportedAppProtocolReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSupportedAppProtocolReq(void)
{
  /* trigger the SAP request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_SupportedAppProtocol;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareSessionSetupReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSessionSetupReq(void)
{
  /* trigger the SessionSetup request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_SessionSetup;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareServiceDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareServiceDiscoveryReq(void)
{
  /* trigger the ServiceDiscovery request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_ServiceDiscovery;

  return (Std_ReturnType)E_OK;
}

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareServiceDetailReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareServiceDetailReq(void)
{
  /* check if internet details are required */
  if (   ( TRUE == Scc_StateM_Local.ReqInetDetails )
      && ( FALSE == Scc_StateM_Local.InetDetailsRequested ) )
  {
    /* send a ServiceDetailReq requesting more details about the internet services */
    Scc_StateM_TxEVSE.ReqServiceID = Scc_ServiceID_Internet;
    Scc_StateM_Local.InetDetailsRequested = TRUE;
  }
  /* illegal state */
  else
  {
    Scc_CallDetReportError(SCC_API_ID_V_STATEM_PREP_SERV_DETAIL, SCC_DET_INV_STATE)
  }

  /* trigger the ServiceDetail request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_ServiceDetail;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_SCHEMA_ISO */

/**********************************************************************************************************************
 *  Scc_StateM_VPreparePaymentServiceSelectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PreparePaymentServiceSelectionReq(void)
{
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  Exi_ISO_SelectedServiceType *SelectedServiceTypePtr = &Scc_StateM_TxEVSE.SelectedServices[0];
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  if ( Scc_SAPSchemaIDs_DIN == Scc_StateM_RxEVSE.SAPSchemaID )
  {
    /* trigger the PaymentServiceSelection request */
    Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_PaymentServiceSelection;

    return (Std_ReturnType)E_OK;
  }
#endif /* SCC_SCHEMA_DIN */

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  if ( Scc_SAPSchemaIDs_ISO == Scc_StateM_RxEVSE.SAPSchemaID )
  {

    /* --- set the selected service list --- */

    /* set the charge service */
    SelectedServiceTypePtr->ParameterSetIDFlag = 0;
    SelectedServiceTypePtr->ServiceID = Scc_StateM_RxEVSE.ChargeServiceID;

    /* if true set the internet service */
    if ( TRUE == Scc_StateM_Local.InternetAvailable )
    {
      SelectedServiceTypePtr->NextSelectedServicePtr = &Scc_StateM_TxEVSE.SelectedServices[2];
      SelectedServiceTypePtr = SelectedServiceTypePtr->NextSelectedServicePtr;
      SelectedServiceTypePtr->ServiceID = Scc_ServiceID_Internet;
      SelectedServiceTypePtr->ParameterSetIDFlag = 1;
      SelectedServiceTypePtr->ParameterSetID = (sint16)Scc_ParameterSetID_Internet_Https;
    }

    /* mark the end of the SelectedServiceList */
    SelectedServiceTypePtr->NextSelectedServicePtr = (Exi_ISO_SelectedServiceType*)NULL_PTR;

    /* trigger the PaymentServiceSelection request */
    Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_PaymentServiceSelection;

    return (Std_ReturnType)E_OK;
  }

#endif /* SCC_SCHEMA_ISO */

  return (Std_ReturnType)E_NOT_OK;
}

#if ( defined SCC_ENABLE_CERTIFICATE_INSTALLATION ) && ( SCC_ENABLE_CERTIFICATE_INSTALLATION == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareCertificateInstallationReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCertificateInstallationReq(void)
{
  /* trigger the CertificateInstallation request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_CertificateInstallation;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_ENABLE_CERTIFICATE_INSTALLATION */

#if ( defined SCC_ENABLE_CERTIFICATE_UPDATE ) && ( SCC_ENABLE_CERTIFICATE_UPDATE == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareCertificateUpdateReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCertificateUpdateReq(void)
{
  /* trigger the CertificateUpdate request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_CertificateUpdate;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_ENABLE_CERTIFICATE_UPDATE */

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareAuthorizationReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareAuthorizationReq(void)
{
  /* trigger the Authorization request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_Authorization;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareChargeParameterDiscoveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareChargeParameterDiscoveryReq(void)
{
  /* get the current charging control command */
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);
  /* check if the application is not ready to continue */
  if ( Scc_ChargingControl_NegotiateChargingParameters != Scc_StateM_Local.ChargingControl )
  {
    return Scc_ReturnType_Pending;
  }

  {
    /* get the selected EnergyTransferMode */
    Scc_Get_StateM_EnergyTransferMode(&Scc_StateM_TxEVSE.EnergyTransferMode);
  }

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  /* check for AC Charging */
  if (   ( Scc_EnergyTransferMode_AC_1P == Scc_StateM_TxEVSE.EnergyTransferMode )
      || ( Scc_EnergyTransferMode_AC_3P == Scc_StateM_TxEVSE.EnergyTransferMode ) )
  {
    /* get the current status of the cable */
    Scc_Get_StateM_PWMState(&Scc_StateM_Local.PWMState);
    /* check if the duty cycle is not 5% or nominal */ /* V2G2-930 */
    if ( Scc_PWMState_ChargingNotAllowed == Scc_StateM_Local.PWMState )
    {
      /* wait */
      return Scc_ReturnType_Pending;
    }
  }
#endif

  /* trigger the ChargeParameterDiscovery request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_ChargeParameterDiscovery;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return Scc_ReturnType_OK;
}

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareCableCheckReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PrepareCableCheckReq(void)
{
  /* get the current status of the cable */
  Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
  /* check if the power switch was not closed */
  if ( Scc_ControlPilotState_State_C_D > Scc_StateM_Local.ControlPilotState )
  {
    return Scc_ReturnType_Pending;
  }

  /* trigger the Cable Check request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_CableCheck;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return Scc_ReturnType_OK;
}
#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPreparePreChargeReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PreparePreChargeReq(void)
{
  /* trigger the Pre Charge request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_PreCharge;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  Scc_StateM_VPreparePowerDeliveryReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_PreparePowerDeliveryReq(void)
{
  /* get the current charging control command */
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);

  /* check if the charging shall be started */
  if ( Scc_ChargingControl_StartPowerDelivery == Scc_StateM_Local.ChargingControl )
  {
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
    /* check for AC Charging (State C is already active in case of DC) */
    if (   ( Scc_EnergyTransferMode_AC_1P == Scc_StateM_TxEVSE.EnergyTransferMode )
        || ( Scc_EnergyTransferMode_AC_3P == Scc_StateM_TxEVSE.EnergyTransferMode ) )
#endif /* SCC_CHARGING_DC */
    {
      /* check if there was no previous renegotiation (since then State C is OK) */
      if ( Scc_ChargeStatus_Renegotiating != Scc_StateM_TxEVSE.ChargeStatus )
      {
        /* get the current status of the cable */
        Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
        /* get the current PWM state */
        Scc_Get_StateM_PWMState(&Scc_StateM_Local.PWMState);
        /* check if CP is not set to State B */ /* V2G2-846 */
        if (   ( Scc_ControlPilotState_State_B != Scc_StateM_Local.ControlPilotState )
            && ( Scc_PWMState_HLCOptional_BCActive != Scc_StateM_Local.PWMState ) )
        {
          return Scc_ReturnType_Pending;
        }
      }
    }
#endif /* SCC_CHARGING_AC */

    Scc_StateM_TxEVSE.ChargeStatus = Scc_ChargeStatus_Starting;
  }
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  /* check if a renegotation shall be started */
  else if (   ( Scc_ChargingControl_Renegotiation == Scc_StateM_Local.ChargingControl )
           && ( Scc_ChargeStatus_Starting == Scc_StateM_TxEVSE.ChargeStatus ))
  {
    Scc_StateM_TxEVSE.ChargeStatus = Scc_ChargeStatus_Renegotiating;
  }
#endif /* SCC_SCHEMA_ISO */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  /* check if a welding detection shall be started */
  else if (   ( Scc_ChargingControl_StartWeldingDetection == Scc_StateM_Local.ChargingControl )
           && ( Scc_EnergyTransferMode_AC_1P != Scc_StateM_TxEVSE.EnergyTransferMode )
           && ( Scc_EnergyTransferMode_AC_3P != Scc_StateM_TxEVSE.EnergyTransferMode ))
  {
    Scc_StateM_TxEVSE.ChargeStatus = Scc_ChargeStatus_Stopping;
  }
#endif /* SCC_CHARGING_DC */
  /* check if the charging shall be stopped or paused */
  else if ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl )
  {
    Scc_StateM_TxEVSE.ChargeStatus = Scc_ChargeStatus_Stopping;
  }
  /* if the application is not ready to continue yet */
  else
  {
    return Scc_ReturnType_Pending;
  }

  /* trigger the PowerDelivery request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_PowerDelivery;

  return Scc_ReturnType_OK;
}

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VPrepareChargingStatusReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareChargingStatusReq(void)
{
  /* trigger the ChargingStatus request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_ChargingStatus;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareCurrentDemandReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareCurrentDemandReq(void)
{
  /* trigger the CurrentDemand request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_CurrentDemand;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareWeldingDetectionReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareWeldingDetectionReq(void)
{
  /* trigger the WeldingDetection request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_WeldingDetection;
  /* set the cyclic trigger */
  Scc_StateM_TxCore.CyclicMsgTrig = TRUE;

  return (Std_ReturnType)E_OK;
}

#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareSessionStopReq
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareSessionStopReq(void)
{
  /* trigger the SessionStop request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_SessionStop;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VPrepareStopCommunicationSession
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_PrepareStopCommunicationSession(void)
{
  /* trigger the StopCommunicationSession request */
  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_StopCommunicationSession;

  return (Std_ReturnType)E_OK;
}

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VProcessSLAC
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSLAC(void)
{
  /* set the new state */
  Scc_StateM_State = Scc_SMS_WaitForIPAddress;

  return (Std_ReturnType)E_OK;
}
#endif /* SCC_ENABLE_SLAC_HANDLING */

/**********************************************************************************************************************
 *  Scc_StateM_VProcessSECCDiscoveryProtocolRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSECCDiscoveryProtocolRes(void)
{
  Std_ReturnType RetVal = (Std_ReturnType)E_OK;

  /* check if the requested security option was not chosen by the EVSE */
  if ( Scc_StateM_TxCore.ReqSDPSecurity != Scc_StateM_RxCore.SupSDPSecurity )
  {
    /* --- determine if the EVSE security option can be accepted --- */

    /* check if the EVSE only supports a connection with TLS */
    if ( Scc_SDPSecurity_Tls == Scc_StateM_RxCore.SupSDPSecurity )
    {
#if ( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON )
      /* use the supported SDPSecurity */
      Scc_StateM_TxCore.ReqSDPSecurity = Scc_StateM_RxCore.SupSDPSecurity;
#else
      /* transport layer connection cannot be established */
      RetVal = (Std_ReturnType)E_NOT_OK;
#endif /* SCC_ENABLE_TLS */
    }
    /* check if the EVSE only supports a connection without TLS */
    else /* if ( Scc_SDPSecurity_None == Scc_StateM_RxCore.SupSDPSecurity ) */
    {
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
      /* check if unsecure connections are not accepted */
      if ( FALSE == (boolean)Scc_ConfigValue_StateM_AcceptUnsecureConnection )
      {
        /* transport layer connection cannot be established */
        RetVal = (Std_ReturnType)E_NOT_OK;
      }
      else
#endif
      {
        /* use the supported SDPSecurity */
        Scc_StateM_TxCore.ReqSDPSecurity = Scc_StateM_RxCore.SupSDPSecurity;
      }
    }
  }

  if ( (Std_ReturnType)E_OK == RetVal )
  {
    /* set the new state */
    Scc_StateM_State = Scc_SMS_TLConnection;
  }
  else
  {
    /* start error handling */
    Scc_StateM_ErrorHandling(Scc_SMER_SECCDiscoveryProtocol_NoSharedSecurityOption);
  }

  return RetVal;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessTLConnected
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessTLConnected(void)
{
  /* set the new state */
  Scc_StateM_State = Scc_SMS_SupportedAppProtocol;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessSupportedAppProtocolRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSupportedAppProtocolRes(void)
{
  /* set the new state */
  Scc_StateM_State = Scc_SMS_SessionSetup;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessSessionSetupRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessSessionSetupRes(void)
{
  /* set the new state */
  Scc_StateM_State = Scc_SMS_ServiceDiscovery;

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessServiceDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessServiceDiscoveryRes(void)
{
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  if ( Scc_SAPSchemaIDs_DIN == Scc_StateM_RxEVSE.SAPSchemaID )
  {
    /* set the PaymentOption */
    Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_EIM;
    /* set the new state */
    Scc_StateM_State = Scc_SMS_PaymentServiceSelection;

    return Scc_ReturnType_OK;
  }
#endif /* SCC_SCHEMA_DIN */

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  if ( Scc_SAPSchemaIDs_ISO == Scc_StateM_RxEVSE.SAPSchemaID )
  {
    /* if no message set was selected yet */
    if ( Scc_PaymentOption_None == Scc_StateM_TxEVSE.SelectedPaymentOption )
    {
      /* check if the EVSE offers free service */
      if (   ( TRUE == Scc_StateM_RxEVSE.ChargeServiceFree )
          && (   ( Scc_PaymentPrioritization_OnlyEIM == Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions )
              || ( Scc_PaymentPrioritization_PrioritizePnC == Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions ) ) )
      {
        Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_EIM;
      }
#if (( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON ))
      /* if a TLS encrypted connection was established */
      else if ( Scc_SDPSecurity_Tls == Scc_StateM_RxCore.SupSDPSecurity )
      {
        /* check if the EVSE offers EIM */
        if (   ( Scc_PaymentPrioritization_OnlyEIM == Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions )
            || ( Scc_PaymentPrioritization_PrioritizePnC == Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions ) )
        {
          Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_EIM;
        }
        /* no matching message set could be determined */
        else
        {
          Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_None;
        }
      }
#endif /* SCC_ENABLE_TLS */
      /* if an unsecured connection was established */
      else /* if ( Scc_SDPSecurity_None == Scc_StateM_RxCore.SupSDPSecurity ) */
      {
        /* if the EVSE only offers contract, or the EV only supports contract */
        if (   ( Scc_PaymentPrioritization_OnlyPnC == Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions )
           )
        {
          Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_None;
        }
        /* if the EVSE and the EV support external payment */
        else
        {
          Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_EIM;
        }
      }

      /* if no message set could be determined */
      if ( Scc_PaymentOption_None == Scc_StateM_TxEVSE.SelectedPaymentOption )
      {
        /* stop the communication session */
        Scc_StateM_ErrorHandling(Scc_SMER_ServiceDiscovery_NoSharedPaymentOption);
        return Scc_ReturnType_NotOK;
      }
    }

    /* check if EIM charging was chosen */
    if ( Scc_PaymentOption_EIM == Scc_StateM_TxEVSE.SelectedPaymentOption )
    {

      /* set the new state */
      Scc_StateM_State = Scc_SMS_PaymentServiceSelection;

      /* if a connection with TLS was established and internet service is required */
      if (   ( Scc_SDPSecurity_Tls == Scc_StateM_TxCore.ReqSDPSecurity )
          && ( TRUE == Scc_StateM_Local.ReqInetDetails ) )
      {
        Scc_StateM_Local.InetDetailsRequested = FALSE;

        /* set the new state */
        Scc_StateM_State = Scc_SMS_ServiceDetail;
      }
    }

    return Scc_ReturnType_OK;
  }

#endif /* SCC_SCHEMA_ISO */

  return Scc_ReturnType_NotOK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
/**********************************************************************************************************************
 *  Scc_StateM_VProcessServiceDetailRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessServiceDetailRes(void)
{
  /* check if the provided ServiceID is the same as the requested */
  if ( Scc_StateM_RxEVSE.ProvServiceID == Scc_StateM_TxEVSE.ReqServiceID )
  {
    /* process the received ServiceParameterList */
    if ( NULL_PTR != Scc_StateM_RxEVSE.ServiceParameterListPtr )
    {
      Exi_ISO_ParameterSetType *ParameterSetPtr = Scc_StateM_RxEVSE.ServiceParameterListPtr->ParameterSet;

#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
      /* if the EVSE offers internet services */
      if ( Scc_ServiceID_Internet == Scc_StateM_RxEVSE.ProvServiceID ) /* PRQA S 2004 */ /* MD_Scc_2004 */
      {
        /* loop as long as the ParameterSet contains another Parameter */
        while ( NULL_PTR != ParameterSetPtr )
        {
          /* check if the ParameterSetID equals the relevant HTTPS service */
          if ( (sint16)Scc_ParameterSetID_Internet_Https == ParameterSetPtr->ParameterSetID )
          {
            /* enable the internet service */
            Scc_StateM_Local.InternetAvailable = TRUE;
            break;
          }
          else
          {
            /* disable the internet service */
            Scc_StateM_Local.InternetAvailable = FALSE;
          }
          /* step to the next Parameter */
          ParameterSetPtr = ParameterSetPtr->NextParameterSetPtr;
        }
        /* report the status of the internet service */
        Scc_Set_StateM_InternetAvailable(Scc_StateM_Local.InternetAvailable);
      }
#endif /* SCC_SCHEMA_ISO */
    }
  }

  {

    /* set the new state */
    Scc_StateM_State = Scc_SMS_PaymentServiceSelection;
  }

  return (Std_ReturnType)E_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif /* SCC_SCHEMA_ISO */

/**********************************************************************************************************************
 *  Scc_StateM_VProcessPaymentServiceSelectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPaymentServiceSelectionRes(void)
{
  {
    /* set the new state */
    Scc_StateM_State = Scc_SMS_Authorization;
  }

  return Scc_ReturnType_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessAuthorizationRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessAuthorizationRes(void)
{
  if ( Scc_EVSEProcessing_Finished == Scc_StateM_RxEVSE.EVSEProcessing )
  {
    /* reset the cycle timer */
    Scc_StateM_Counter.OngoingTimerCnt = 0;
    /* reset the delay timer */
    Scc_StateM_Counter.GeneralDelayCnt = 0;

    /* set the new state */
    Scc_StateM_State = Scc_SMS_ChargeParameterDiscovery;
  }
  else /* Scc_EVSEProcessing_Ongoing */
  {
    /* check if the delay counter is active */
    if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
    {
      Scc_StateM_Counter.GeneralDelayCnt--;
      /* if the next request has to be further delayed */
      if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* return OK */
        return Scc_ReturnType_Pending;
      }
    }
    /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( 0u < (uint16)Scc_ConfigValue_StateM_AuthorizationNextReqDelay )
    {
      /* enable the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_AuthorizationNextReqDelay;
      /* return pending */
      return Scc_ReturnType_Pending;
    }
  }

  return Scc_ReturnType_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessChargeParameterDiscoveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessChargeParameterDiscoveryRes(void)
{
  /* check if the EVSE signals that ChargeParameterDiscovery has finished */
  if ( Scc_EVSEProcessing_Finished == Scc_StateM_RxEVSE.EVSEProcessing )
  {
    /* reset the cycle timer */
    Scc_StateM_Counter.OngoingTimerCnt = 0;
    /* reset the delay timer */
    Scc_StateM_Counter.GeneralDelayCnt = 0;

    /* check which energy transfer mode is active */
    switch ( Scc_StateM_TxEVSE.EnergyTransferMode )
    {
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
      /* check for AC Charging */
    case Scc_EnergyTransferMode_AC_1P:
    case Scc_EnergyTransferMode_AC_3P:
      /* set the new state */
      Scc_StateM_State = Scc_SMS_PowerDelivery;
      break;
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
      /* check for DC Charging */
    case Scc_EnergyTransferMode_DC_Combo_Core:
    case Scc_EnergyTransferMode_DC_Core:
    case Scc_EnergyTransferMode_DC_Extended:
    case Scc_EnergyTransferMode_DC_Unique:
      /* set the new state */
      Scc_StateM_State = Scc_SMS_CableCheck;
      break;
#endif /* SCC_CHARGING_DC */

    default:
      /* this is an invalid state */
      Scc_CallDetReportError(SCC_API_ID_V_STATEM_PROC_CHARGE_PARAM, SCC_DET_INV_PARAM);
      break;
    }
  }
  /* continue with ChargeParameterDiscovery */
  else
  {
    /* check if the delay counter is active */
    if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
    {
      Scc_StateM_Counter.GeneralDelayCnt--;
      /* check if the delay counter has not elapsed yet */
      if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* wait */
        return Scc_ReturnType_Pending;
      }
    }
    /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( 0u < (uint16)Scc_ConfigValue_StateM_ChargeParameterDiscoveryNextReqDelay )
    {
      /* enable the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_ChargeParameterDiscoveryNextReqDelay;
      /* return pending */
      return Scc_ReturnType_Pending;
    }
  }

  return Scc_ReturnType_OK;
}

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VProcessCableCheckRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessCableCheckRes(void)
{
  /* check if the EVSE signals that CableCheck has finished */
  if ( Scc_EVSEProcessing_Finished == Scc_StateM_RxEVSE.EVSEProcessing )
  {
    /* reset the cycle timer */
    Scc_StateM_Counter.OngoingTimerCnt = 0;
    /* reset the delay timer */
    Scc_StateM_Counter.GeneralDelayCnt = 0;

    /* set the new state */
    Scc_StateM_State = Scc_SMS_PreCharge;
  }
  /* continue with CableCheck */
  else
  {
    /* check if the delay counter is active */
    if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
    {
      Scc_StateM_Counter.GeneralDelayCnt--;
      /* check if the delay counter has not elapsed yet */
      if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* wait */
        return Scc_ReturnType_Pending;
      }
    }
    /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( 0u < (uint16)Scc_ConfigValue_StateM_CableCheckNextReqDelay )
    {
      /* enable the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_CableCheckNextReqDelay;
      /* return pending */
      return Scc_ReturnType_Pending;
    }
  }

  return Scc_ReturnType_OK;
}
#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VProcessPreChargeRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPreChargeRes(void)
{
  /* get the current ChargingControl */
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);

  /* check if the application wants to stop the PreCharge sequence */
  if ( Scc_ChargingControl_PreChargeCompleted == Scc_StateM_Local.ChargingControl )
  {
    /* reset the cycle timer */
    Scc_StateM_Counter.OngoingTimerCnt = 0;
    /* reset the delay timer */
    Scc_StateM_Counter.GeneralDelayCnt = 0;
    /* set the new state */
    Scc_StateM_State = Scc_SMS_PowerDelivery;
  }
  /* continue with PreCharge */
  else
  {
    /* check if the delay counter is active */
    if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
    {
      Scc_StateM_Counter.GeneralDelayCnt--;
      /* check if the delay counter has not elapsed yet */
      if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* wait */
        return Scc_ReturnType_Pending;
      }
    }
    /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( 0u < (uint16)Scc_ConfigValue_StateM_PreChargeNextReqDelay )
    {
      /* enable the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_PreChargeNextReqDelay;
      /* return pending */
      return Scc_ReturnType_Pending;
    }
  }

  return Scc_ReturnType_OK;
}
#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  Scc_StateM_VProcessPowerDeliveryRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessPowerDeliveryRes(void)
{
  /* if the charging shall be started */
  if ( Scc_ChargingControl_StartPowerDelivery == Scc_StateM_Local.ChargingControl )
  {
    /* check which energy transfer mode is active */
    switch ( Scc_StateM_TxEVSE.EnergyTransferMode )
    {
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
      /* check for AC Charging */
    case Scc_EnergyTransferMode_AC_1P:
    case Scc_EnergyTransferMode_AC_3P:
      /* get the current cable state */
      Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
      /* check if the power switch is not closed yet */
      if ( Scc_ControlPilotState_State_C_D > Scc_StateM_Local.ControlPilotState )
      {
        /* wait for S2 to close (should happen at least 250ms after PowerDeliveryReq) */
        return Scc_ReturnType_Pending;
      }
      /* set the next state */
      Scc_StateM_State = Scc_SMS_ChargingStatus;
      break;
#endif /* SCC_CHARGING_DC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
      /* check for DC Charging */
    case Scc_EnergyTransferMode_DC_Combo_Core:
    case Scc_EnergyTransferMode_DC_Core:
    case Scc_EnergyTransferMode_DC_Extended:
    case Scc_EnergyTransferMode_DC_Unique:
      /* set the next state */
      Scc_StateM_State = Scc_SMS_CurrentDemand;
      break;
#endif /* SCC_CHARGING_DC */

    default:
      /* this is an invalid state */
      Scc_CallDetReportError(SCC_API_ID_V_STATEM_PROC_POW_DEL, SCC_DET_INV_PARAM);
      break;
    }
  }
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  /* if a renegotiation shall be triggered */
  else if ( Scc_ChargingControl_Renegotiation == Scc_StateM_Local.ChargingControl )
  {
    /* set the next state */
    Scc_StateM_State = Scc_SMS_ChargeParameterDiscovery;
  }
#endif /* SCC_SCHEMA_ISO */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  /* if a welding detection shall be started */
  else if ( Scc_ChargingControl_StartWeldingDetection == Scc_StateM_Local.ChargingControl )
  {
    Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);

    /* check if the power switch is still closed */
    if ( Scc_ControlPilotState_State_C_D == Scc_StateM_Local.ControlPilotState )
    {
      return Scc_ReturnType_Pending;
    }

    /* set the next state */
    Scc_StateM_State = Scc_SMS_WeldingDetection;
  }
#endif /* SCC_CHARGING_DC */
  /* if the session shall be paused or stopped */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
  else if ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl )
  {
    Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);

    /* check if the power switch is still closed */
    if ( Scc_ControlPilotState_State_C_D == Scc_StateM_Local.ControlPilotState )
    {
      return Scc_ReturnType_Pending;
    }

    /* set the next state */
    Scc_StateM_State = Scc_SMS_SessionStop;
  }

  return Scc_ReturnType_OK;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
/**********************************************************************************************************************
 *  Scc_StateM_VProcessChargingStatusRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessChargingStatusRes(void)
{
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);

  {
    /* check if the charging shall be stopped or a new charging profile shall be selected */
    if (   ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl )
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
        || ( Scc_ChargingControl_Renegotiation == Scc_StateM_Local.ChargingControl )
#endif /* SCC_SCHEMA_ISO */
       )
    {
      /* reset the delay */
      Scc_StateM_Counter.GeneralDelayCnt = 0;
      /* set the next state */
      Scc_StateM_State = Scc_SMS_PowerDelivery;
    }
    /* otherwise send another ChargingStatusReq */
    else
    {
      /* check if the delay counter is active */
      if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
      {
        Scc_StateM_Counter.GeneralDelayCnt--;
        /* if the next request has to be further delayed */
        if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
        {
          /* wait */
          return Scc_ReturnType_Pending;
        }
      }
      /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( 0u < (uint16)Scc_ConfigValue_StateM_ChargingStatusNextReqDelay )
      {
        /* enable the delay counter */
        Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_ChargingStatusNextReqDelay;
        /* wait */
        return Scc_ReturnType_Pending;
      }
    }
  }

  return Scc_ReturnType_OK;
}
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

/**********************************************************************************************************************
 *  Scc_StateM_VProcessCurrentDemandRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessCurrentDemandRes(void)
{
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);

  {
    /* check if the charging shall be stopped */
    if (   ( Scc_ChargingControl_StartWeldingDetection == Scc_StateM_Local.ChargingControl )
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
        || ( Scc_ChargingControl_Renegotiation == Scc_StateM_Local.ChargingControl )
#endif /* SCC_SCHEMA_ISO */
        || ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl ) )
    {
      /* reset the delay */
      Scc_StateM_Counter.GeneralDelayCnt = 0;
      /* set the next state */
      Scc_StateM_State = Scc_SMS_PowerDelivery;
    }
    /* otherwise send another CurrentDemandReq */
    else
    {
      /* check if the delay counter is active */
      if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* decrement the delay counter */
        Scc_StateM_Counter.GeneralDelayCnt--;
        /* check if the delay counter has not elapsed yet */
        if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
        {
          /* wait */
          return Scc_ReturnType_Pending;
        }
      }
      /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
      else if ( 0u < (uint16)Scc_ConfigValue_StateM_CurrentDemandNextReqDelay )
      {
        /* enable the delay counter */
        Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_CurrentDemandNextReqDelay;
        /* wait */
        return Scc_ReturnType_Pending;
      }
    }
  }

  return Scc_ReturnType_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessWeldingDetectionRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessWeldingDetectionRes(void)
{
  /* get the current ChargingControl */
  Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);

  /* check if the charging shall be stopped */
  if ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl )
  {
    /* reset the cycle timer */
    Scc_StateM_Counter.OngoingTimerCnt = 0;
    /* reset the delay timer */
    Scc_StateM_Counter.GeneralDelayCnt = 0;
    /* set the new state */
    Scc_StateM_State = Scc_SMS_SessionStop;
  }
  /* continue the welding detection */
  else
  {
    /* check if the delay counter is active */
    if ( 0u < Scc_StateM_Counter.GeneralDelayCnt )
    {
      Scc_StateM_Counter.GeneralDelayCnt--;
      /* if the next request has to be further delayed */
      if ( 0u != Scc_StateM_Counter.GeneralDelayCnt )
      {
        /* return OK */
        return Scc_ReturnType_Pending;
      }
    }
    /* if the delay counter is still disabled */ /* PRQA S 2004 1 */ /* MD_Scc_2004 */
    else if ( 0u < (uint16)Scc_ConfigValue_StateM_WeldingDetectionNextReqDelay )
    {
      /* enable the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_WeldingDetectionNextReqDelay;
      /* return pending */
      return Scc_ReturnType_Pending;
    }
  }

  return Scc_ReturnType_OK;
}

#endif /* SCC_CHARGING_DC */

/**********************************************************************************************************************
 *  Scc_StateM_VProcessSessionStopRes
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_ProcessSessionStopRes(void)
{
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  /* check if DIN schema is active */
  if ( Scc_SAPSchemaIDs_DIN == Scc_StateM_RxEVSE.SAPSchemaID )
  {
    Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
    /* check if the power switch is closed */ /* V2G-DC-750 */
    if ( Scc_ControlPilotState_State_C_D == Scc_StateM_Local.ControlPilotState )
    {
      return Scc_ReturnType_Pending;
    }
  }
#endif /* SCC_SCHEMA_DIN */

  /* switch to the next state */
  Scc_StateM_State = Scc_SMS_StopCommunicationSession;

  return Scc_ReturnType_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VProcessStopCommunicationSession
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_ProcessStopCommunicationSession(void)
{
  /* switch to the next state */
  Scc_StateM_State = Scc_SMS_Stopped;

  /* update the status */
  Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_Finished);

  return (Std_ReturnType)E_OK;
}

/**********************************************************************************************************************
 *  Scc_StateM_VGlobalTimerChecks
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Std_ReturnType) Scc_StateM_GlobalTimerChecks(void)
{
  Std_ReturnType RetVal = E_OK;

  Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);

  /* check if a cable is plugged in */
  if ( Scc_ControlPilotState_State_A_E_F < Scc_StateM_Local.ControlPilotState )
  {
    /* check the sequence performance timer */
    if ( 0 < Scc_StateM_Counter.SequencePerformanceTimerCnt )
    {
      /* decrement the timer */
      Scc_StateM_Counter.SequencePerformanceTimerCnt--;
      /* check if the timer elapsed */
      if ( 0 == Scc_StateM_Counter.SequencePerformanceTimerCnt )
      {
        /* start error handling */
        Scc_StateM_ErrorHandling(Scc_SMER_Timer_SequencePerformanceTimer);
        RetVal = E_NOT_OK;
      }
    }

    /* check the communication setup timer */
    if ( 0 < Scc_StateM_Counter.CommunicationSetupTimerCnt )
    {
      uint16 Timeout;

      switch ( Scc_StateM_RxEVSE.SAPSchemaID )
      {
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
      case Scc_SAPSchemaIDs_ISO:
        Timeout = (uint16)Scc_ConfigValue_Timer_ISO_CommunicationSetupTimeout;
        break;
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
      case Scc_SAPSchemaIDs_DIN:
        Timeout = (uint16)Scc_ConfigValue_Timer_DIN_CommunicationSetupTimeout;
        break;
#endif /* SCC_SCHEMA_DIN */

      default:
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO == 1 )
        Timeout = (uint16)Scc_ConfigValue_Timer_ISO_CommunicationSetupTimeout;
#else
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN == 1 )
        Timeout = (uint16)Scc_ConfigValue_Timer_DIN_CommunicationSetupTimeout;
#else
#endif /* SCC_SCHEMA_DIN */
#endif /* SCC_SCHEMA_ISO */
      }

      /* check if the timer elapsed */
      if ( Timeout == Scc_StateM_Counter.CommunicationSetupTimerCnt )
      {
        /* stop the communication setup timer */
        Scc_StateM_Counter.CommunicationSetupTimerCnt = 0;
        /* start error handling */
        Scc_StateM_ErrorHandling(Scc_SMER_Timer_CommunicationSetupTimer);
        RetVal = E_NOT_OK;
      }
      /* the timer has not yet elapsed */
      else
      {
        /* continue to count the timer */
        Scc_StateM_Counter.CommunicationSetupTimerCnt++;
      }
    }

    /* check the ready to charge timer */
    if ( 0 < Scc_StateM_Counter.ReadyToChargeTimerCnt )
    {
      uint16 Timeout;

      switch ( Scc_StateM_RxEVSE.SAPSchemaID )
      {
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
      case Scc_SAPSchemaIDs_DIN:
        Timeout = (uint16)Scc_ConfigValue_Timer_DIN_ReadyToChargeTimeout;
        break;
#endif /* SCC_SCHEMA_DIN */

      default:
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
        Timeout = (uint16)Scc_ConfigValue_Timer_DIN_ReadyToChargeTimeout;
#else
        /* this code can only be active when the only schema is ISO */
        /* ISO does not use the ReadyToChargeTimer, therefore this code will never be reached */
        Timeout = 0xFFFF;
#endif /* SCC_SCHEMA_DIN */
      }

      /* check if the timer elapsed */
      if ( Timeout == Scc_StateM_Counter.ReadyToChargeTimerCnt )
      {
        /* stop the ready to charge timer */
        Scc_StateM_Counter.ReadyToChargeTimerCnt = 0;
        /* start error handling */
        Scc_StateM_ErrorHandling(Scc_SMER_Timer_ReadyToChargeTimer);
        RetVal = E_NOT_OK;
      }
      /* the timer has not yet elapsed */
      else
      {
        /* continue to count the timer */
        Scc_StateM_Counter.ReadyToChargeTimerCnt++;
      }
    }
  }

  return RetVal;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_StateM_VGlobalParamsChecks
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(Scc_ReturnType) Scc_StateM_GlobalParamsChecks(void)
{
  Scc_ReturnType                          RetVal = Scc_ReturnType_OK;

  /* get the current state of the cable */
  Scc_Get_StateM_ControlPilotState(&Scc_StateM_Local.ControlPilotState);
  /* get the current function command of the application */
  Scc_Get_StateM_FunctionControl(&Scc_StateM_Local.FunctionControl);

  /* check if the module shall be reset */
  if ( Scc_FunctionControl_Reset == Scc_StateM_Local.FunctionControl )
  {
    /* only reset if state is higher than INIT */
    if (   ( Scc_SMS_Initialized < Scc_StateM_State )
        && ( Scc_SMS_Stopped > Scc_StateM_State ))
    {
      /* init the params */
      Scc_StateM_InitParams();
      /* set the state to INIT */
      Scc_StateM_State = Scc_SMS_Initialized;
      RetVal = Scc_ReturnType_Pending;
    }
  }

  /* check if the control pilot changed */
  if ( Scc_StateM_LastControlPilotState != Scc_StateM_Local.ControlPilotState )
  {
    /* update the function-local control pilot status */
    Scc_StateM_LastControlPilotState = Scc_StateM_Local.ControlPilotState;

    /* check if the cable was unplugged */
    if ( Scc_ControlPilotState_State_A_E_F == Scc_StateM_Local.ControlPilotState )
    {
      /* reset the retries */
      Scc_StateM_Counter.ReconnectRetryCnt = (uint16)Scc_ConfigValue_StateM_ReconnectRetries;

      /* reset the VAS settings */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
      Scc_StateM_Local.ReqInetDetails = (boolean)Scc_ConfigValue_StateM_RequestInternetDetails;
#endif /* SCC_SCHEMA_ISO */

      /* reset the SessionID */
      (void) Scc_ResetSessionID(TRUE);
    }
  }

  /* check if the cable is not plugged out */
  if ( Scc_ControlPilotState_State_A_E_F != Scc_StateM_Local.ControlPilotState )
  {
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
    /* check if DIN charging is active */
    if ( Scc_SAPSchemaIDs_DIN == Scc_StateM_RxEVSE.SAPSchemaID )
    {
      /* check if PowerDelivery was not sent */
      if (   ( Scc_SMS_PowerDelivery > Scc_StateM_State )
          && ( Scc_SMS_SessionSetup <= Scc_StateM_State ))
      {
        /* check if the application wants to stop */
        Scc_Get_StateM_ChargingControl(&Scc_StateM_Local.ChargingControl);
        if ( Scc_ChargingControl_StopCharging == Scc_StateM_Local.ChargingControl )
        {
          /* check if we are not waiting for a response */
          if (   ( Scc_SMMS_WaitingForResponse != Scc_StateM_MsgState )
              || (    (( (uint8)Scc_StateM_TxCore.MsgTrig * 2 )       == (uint8)Scc_StateM_RxCore.MsgStatus )
                  || ((( (uint8)Scc_StateM_TxCore.MsgTrig * 2 ) - 1 ) == (uint8)Scc_StateM_RxCore.MsgStatus )))
          {
            /* stop the delay timer */
            Scc_StateM_Counter.GeneralDelayCnt = 0;
            /* reset the cycle timer */
            Scc_StateM_Counter.OngoingTimerCnt = 0;
            /* go directly to SessionStop */
            Scc_StateM_State = Scc_SMS_SessionStop;
            /* go directly to SessionStop */
#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
            Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
#else /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
            Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
            
          }
        }
      }
    }
#endif /* SCC_SCHEMA_DIN */

    /* check if there was an error with the transport layer */
    if ( Scc_StackError_TransportLayer == Scc_StateM_RxCore.StackError )
    {
      /* check if there is an active V2G communication session */
      if (   ( Scc_SMS_SupportedAppProtocol <= Scc_StateM_State )
          && ( Scc_SMS_SessionStop >= Scc_StateM_State ) )
      {
        /* this is not allowed to happen */
        Scc_StateM_ErrorHandling(Scc_SMER_StackError);
        RetVal = Scc_ReturnType_NotOK;
      }
    }
  }

  return RetVal;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Scc_StateM_InitParams
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_StateM_InitParams(void)
{
  Scc_StateM_MsgState = Scc_SMMS_PreparingRequest;

  Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_None;
  Scc_StateM_TxCore.CyclicMsgTrig = FALSE;
  Scc_StateM_TxCore.ReqSDPSecurity = Scc_SDPSecurity_Tls;

  Scc_StateM_RxCore.CyclicMsgRcvd = FALSE;
  Scc_StateM_RxCore.MsgStatus = Scc_MsgStatus_None;
  Scc_StateM_RxCore.StackError = Scc_StackError_NoError;
  Scc_StateM_RxCore.SupSDPSecurity = Scc_SDPSecurity_None;

#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
  Scc_StateM_Local.InternetAvailable = FALSE;
#endif /* SCC_SCHEMA_ISO */

  Scc_StateM_Local.ChargingControl = Scc_ChargingControl_None;
  Scc_StateM_Local.EVTimeStamp = 0xFFFFFFFFU;
  Scc_StateM_Local.FunctionControl = Scc_FunctionControl_None;
  Scc_StateM_Local.ControlPilotState = Scc_ControlPilotState_None;
  Scc_StateM_Local.PWMState = Scc_PWMState_ChargingNotAllowed;

  Scc_StateM_TxEVSE.ChargeStatus = Scc_ChargeStatus_Preparing;
  Scc_StateM_TxEVSE.EnergyTransferMode = Scc_EnergyTransferMode_None;
  Scc_StateM_TxEVSE.SelectedPaymentOption = Scc_PaymentOption_None;

  Scc_StateM_RxEVSE.EVSEOfferedPaymentOptions = Scc_PaymentPrioritization_OnlyEIM;
  Scc_StateM_RxEVSE.EVSEProcessing = Scc_EVSEProcessing_Finished;
  Scc_StateM_RxEVSE.ProvServiceID = 0;
  Scc_StateM_RxEVSE.ReceiptRequired = FALSE;
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  Scc_StateM_RxEVSE.SAPSchemaID = Scc_SAPSchemaIDs_ISO;
#else
# if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  Scc_StateM_RxEVSE.SAPSchemaID = Scc_SAPSchemaIDs_DIN;
# else
# endif /* SCC_SCHEMA_DIN */
#endif /* SCC_SCHEMA_ISO */

#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
  Scc_StateM_Local.InetDetailsRequested = FALSE;
#endif /* SCC_SCHEMA_ISO */
  Scc_StateM_Local.SendChargingProfile = TRUE;

  Scc_StateM_Counter.OngoingTimerCnt = 0;
  Scc_StateM_Counter.GeneralDelayCnt = 0;
  Scc_StateM_Counter.IPAddressWaitTimerCnt = 0;
  Scc_StateM_Counter.NvmReadTimerCnt = 0;
  Scc_StateM_Counter.SequencePerformanceTimerCnt = 0;

  return;
}

/**********************************************************************************************************************
 *  Scc_StateM_VErrorHandling
 *********************************************************************************************************************/
SCC_STATICFUNCCODE(void) Scc_StateM_ErrorHandling(Scc_StateM_StateMachineErrorType ErrorReason)
{
  /* check if the module was not already stopped (new error will be ignored otherwise) */
  if ( Scc_SMS_Stopped > Scc_StateM_State )
  {
    /* report the error to the application */
    Scc_Set_StateM_StateMachineError(ErrorReason);

    /* reset the core state machine */
    Scc_StateM_TxCore.MsgTrig = Scc_MsgTrig_None;
    /* wait before starting with the next retry */
    Scc_StateM_State = Scc_SMS_Stopped;
    /* reset the msg state */
#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
    Scc_StateM_MsgStateFct(Scc_SMMS_WaitingForApplication)
#else /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
    Scc_StateM_MsgStateFct(Scc_SMMS_PreparingRequest)
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */

    /* if there are still retries left and the error was not caused by the NvM */
    if (   ( 0 < Scc_StateM_Counter.ReconnectRetryCnt )
        && ( Scc_StackError_NvM != Scc_StateM_RxCore.StackError ))
    {
      /* decrement the retries */
      Scc_StateM_Counter.ReconnectRetryCnt--;
      /* start the delay timer for the reconnect */
      Scc_StateM_Counter.GeneralDelayCnt = (uint16)Scc_ConfigValue_StateM_ReconnectDelay;

      /* update StateMachineStatus */
      Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_Error_WaitingForRetry);
    }
    /* no retries are left */
    else
    {
      /* reset the delay counter */
      Scc_StateM_Counter.GeneralDelayCnt = 0;

      /* update StateMachineStatus */
      Scc_Set_StateM_StateMachineStatus(Scc_StateMachineStatus_Error_Stopped);
    }
  }

  return;
}

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

#endif /* SCC_ENABLE_STATE_MACHINE */

/**********************************************************************************************************************
 *  END OF FILE: Scc_StateM_Vector.c
 *********************************************************************************************************************/
