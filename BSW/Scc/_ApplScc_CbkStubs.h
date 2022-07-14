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
/**        \file  _ApplScc_CbkStubs.h
 *        \brief  Smart Charge Communication Stub Header File
 *
 *      \details  Provides callback stubs for the parameter interface.
 *
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 *********************************************************************************************************************/ 
#if !defined (SCC_CBK_STUBS_H)
#define SCC_CBK_STUBS_H

#error "This file is only intended to provide stubs for the callback interface. Do not use this file in your project!"

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Types.h"
#include "Scc_Cfg.h"

/* Callbacks */

/* Rx Core */
void Appl_SccCbk_Get_Core_CyclicMsgTrigRx(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) CyclicMsgTrig);
void Appl_SccCbk_Get_Core_MsgTrig(P2VAR(Scc_MsgTrigType, AUTOMATIC, SCC_APPL_DATA) MsgTrig);
void Appl_SccCbk_Get_Core_SDPSecurityRx(P2VAR(Scc_SDPSecurityType, AUTOMATIC, SCC_APPL_DATA) SDPSecurity);

/* Tx Core */
void Appl_SccCbk_Set_Core_CyclicMsgRcvd(boolean CyclicMsgRcvd);
void Appl_SccCbk_Set_Core_CyclicMsgTrigTx(boolean CyclicMsgTrig);
void Appl_SccCbk_Set_Core_IPAssigned(boolean IPAssigned);
void Appl_SccCbk_Set_Core_MsgState(Scc_MsgStateType MsgState);
void Appl_SccCbk_Set_Core_MsgStatus(Scc_MsgStatusType MsgStatus);
void Appl_SccCbk_Set_Core_SAPResponseCode(Exi_SAP_responseCodeType SAPResponseCode);
void Appl_SccCbk_Set_Core_SAPSchemaID(Scc_SAPSchemaIDType SAPSchemaID);
void Appl_SccCbk_Set_Core_SDPSecurityTx(Scc_SDPSecurityType SDPSecurityTx);
void Appl_SccCbk_Set_Core_SECCIPAddress(P2CONST(uint8, AUTOMATIC, SCC_APPL_DATA) SECCIPAddress);
void Appl_SccCbk_Set_Core_SECCPort(uint16 SECCPort);
void Appl_SccCbk_Set_Core_StackError(Scc_StackErrorType StackError);
void Appl_SccCbk_Set_Core_TrcvLinkState(boolean TrcvLinkState);
void Appl_SccCbk_Set_Core_TCPSocketState(Scc_TCPSocketStateType TCPSocketStateType);

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )

/* Rx SLAC */
void Appl_SccCbk_Get_SLAC_QCAIdleTimer(P2VAR(uint16, AUTOMATIC, SCC_APPL_DATA) QCAIdleTimer);
void Appl_SccCbk_Get_SLAC_StartMode(P2VAR(EthTrcv_30_Ar7000_Slac_StartModeType, AUTOMATIC, SCC_APPL_DATA) SLACStartMode);

/* Tx SLAC */
void Appl_SccCbk_Set_SLAC_AssociationStatus(uint8 AssociationStatus);
void Appl_SccCbk_Set_SLAC_ToggleRequest(uint8 ToogleNum);
void Appl_SccCbk_Set_SLAC_NMK(P2CONST(Scc_BufferPointerType, AUTOMATIC, SCC_APPL_DATA) NMKPtr);
void Appl_SccCbk_Set_SLAC_NID(P2CONST(Scc_BufferPointerType, AUTOMATIC, SCC_APPL_DATA) NIDPtr);

#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_OEM_VECTOR)

/* Rx StateM */
void Appl_SccCbk_Get_StateM_ChargingControl(P2VAR(Scc_StateM_ChargingControlType, AUTOMATIC, SCC_APPL_DATA) ChargingControl);
void Appl_SccCbk_Get_StateM_EnergyTransferMode(P2VAR(Scc_StateM_EnergyTransferModeType, AUTOMATIC, SCC_APPL_DATA) EnergyTransferMode);
void Appl_SccCbk_Get_StateM_EVTimeStamp(P2VAR(uint32, AUTOMATIC, SCC_APPL_DATA) EVTimeStamp);
void Appl_SccCbk_Get_StateM_FlowControl(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) FlowControl);
void Appl_SccCbk_Get_StateM_FunctionControl(P2VAR(Scc_StateM_FunctionControlType, AUTOMATIC, SCC_APPL_DATA) FunctionControl);
void Appl_SccCbk_Get_StateM_ControlPilotState(P2VAR(Scc_StateM_ControlPilotStateType, AUTOMATIC, SCC_APPL_DATA) ControlPilotState);
void Appl_SccCbk_Get_StateM_PWMState(P2VAR(Scc_StateM_PWMStateType, AUTOMATIC, SCC_APPL_DATA) PWMState);

/* Tx StateM */
void Appl_SccCbk_Set_StateM_EnergyTransferModeFlags(uint8 EnergyTransferModeFlags);
void Appl_SccCbk_Set_StateM_InternetAvailable(boolean InternetAvailable);
void Appl_SccCbk_Set_StateM_StateMachineError(Scc_StateM_StateMachineErrorType StateMachineError);
void Appl_SccCbk_Set_StateM_StateMachineMessageState(Scc_StateM_MsgStateType StateMachineMessageState);
void Appl_SccCbk_Set_StateM_StateMachineStatus(Scc_StateM_StateMachineStatusType StateMachineStatus);

#endif /* SCC_OEM_VECTOR */

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )

/* Rx ISO */
void Appl_SccCbk_Get_ISO_ChargingSession(P2VAR(Exi_ISO_chargingSessionType, AUTOMATIC, SCC_APPL_DATA) ChargingSession);
#if ( defined SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER ) && ( SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER == STD_ON )
void Appl_SccCbk_Get_ISO_ChargingProfilePtr(P2VAR(Scc_BufferPointerType, AUTOMATIC, SCC_APPL_DATA) ChargingProfilePtr, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
#else
void Appl_SccCbk_Get_ISO_ChargingProfilePtr(P2VAR(Exi_ISO_ChargingProfileType*, AUTOMATIC, SCC_APPL_DATA) ChargingProfilePtr, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
#endif /* SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER */
void Appl_SccCbk_Get_ISO_DepartureTime(P2VAR(uint32, AUTOMATIC, SCC_APPL_DATA) DepartureTime, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_MaxEntriesSAScheduleTuple(P2VAR(uint16, AUTOMATIC, SCC_APPL_DATA) MaxEntriesSAScheduleTuple, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_SAScheduleTupleID(P2VAR(uint8, AUTOMATIC, SCC_APPL_DATA) SAScheduleTupleID);
void Appl_SccCbk_Get_ISO_ServiceCategory(P2VAR(Exi_ISO_serviceCategoryType, AUTOMATIC, SCC_APPL_DATA) ServiceCategory, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_ServiceScope(P2VAR(Exi_ISO_serviceScopeType, AUTOMATIC, SCC_APPL_DATA) ServiceScope, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_PnC_CurrentTime(P2VAR(uint32, AUTOMATIC, SCC_APPL_DATA) CurrentTime);

void Appl_SccCbk_Get_ISO_AC_EAmount(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EAmount);
void Appl_SccCbk_Get_ISO_AC_EVMaxVoltage(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaxVoltage);
void Appl_SccCbk_Get_ISO_AC_EVMaxCurrent(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaxCurrent);
void Appl_SccCbk_Get_ISO_AC_EVMinCurrent(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMinCurrent);

void Appl_SccCbk_Get_ISO_DC_BulkChargingComplete(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) BulkChargingComplete, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_BulkSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) BulkSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_ChargingComplete(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) ChargingComplete);
void Appl_SccCbk_Get_ISO_DC_EVEnergyCapacity(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVEnergyCapacity, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_EVEnergyRequest(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVEnergyRequest, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_EVErrorCode(P2VAR(Exi_ISO_DC_EVErrorCodeType, AUTOMATIC, SCC_APPL_DATA) EVErrorCode);
void Appl_SccCbk_Get_ISO_DC_EVMaximumCurrentLimit(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumCurrentLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_EVMaximumPowerLimit(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumPowerLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_EVMaximumVoltageLimit(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumVoltageLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_EVPowerDeliveryParameterFlag(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVPowerDeliveryParameterFlag);
void Appl_SccCbk_Get_ISO_DC_EVReady(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVReady);
void Appl_SccCbk_Get_ISO_DC_EVRESSSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) EVRESSSOC);
void Appl_SccCbk_Get_ISO_DC_EVTargetCurrent(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVTargetCurrent);
void Appl_SccCbk_Get_ISO_DC_EVTargetVoltage(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVTargetVoltage);
void Appl_SccCbk_Get_ISO_DC_FullSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) FullSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_RemainingTimeToBulkSoC(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) RemainingTimeToBulkSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_ISO_DC_RemainingTimeToFullSoC(P2VAR(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) RemainingTimeToFullSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);

/* Tx ISO */
void Appl_SccCbk_Set_ISO_ChargeService(P2CONST(Exi_ISO_ChargeServiceType, AUTOMATIC, SCC_APPL_DATA) ChargeService);
void Appl_SccCbk_Set_ISO_EVSEID(P2CONST(Exi_ISO_evseIDType, AUTOMATIC, SCC_APPL_DATA) EVSEID);
void Appl_SccCbk_Set_ISO_EVSENotification(Exi_ISO_EVSENotificationType EVSENotification);
void Appl_SccCbk_Set_ISO_EVSEProcessing(Exi_ISO_EVSEProcessingType EVSEProcessing);
void Appl_SccCbk_Set_ISO_EVSETimeStamp(Exi_SInt64 EVSETimeStamp, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_MeterInfo(P2CONST(Exi_ISO_MeterInfoType, AUTOMATIC, SCC_APPL_DATA) MeterInfo, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_Notification(P2CONST(Exi_ISO_NotificationType,  AUTOMATIC, SCC_APPL_DATA) Notification, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_NotificationMaxDelay(uint16 NotificationMaxDelay);
void Appl_SccCbk_Set_ISO_PaymentOptionList(P2CONST(Exi_ISO_PaymentOptionListType, AUTOMATIC, SCC_APPL_DATA) PaymentOptionList);
void Appl_SccCbk_Set_ISO_ResponseCode(Exi_ISO_responseCodeType ResponseCode);
void Appl_SccCbk_Set_ISO_SAScheduleList(P2CONST(Exi_ISO_SAScheduleListType, AUTOMATIC, SCC_APPL_DATA) SAScheduleList);
void Appl_SccCbk_Set_ISO_ServiceIDTx(uint16 ServiceID);
void Appl_SccCbk_Set_ISO_ServiceList(P2CONST(Exi_ISO_ServiceListType, AUTOMATIC, SCC_APPL_DATA) ServiceList, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_ServiceParameterList(P2CONST(Exi_ISO_ServiceParameterListType, AUTOMATIC, SCC_APPL_DATA) ServiceParameterList, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_PnC_ReceiptRequired(boolean ReceiptRequired, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_PnC_RetryCounter(sint16 RetryCounter, Exi_BitType Flag);

void Appl_SccCbk_Set_ISO_AC_EVSEMaxCurrent(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaxCurrent, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_AC_EVSENominalVoltage(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSENominalVoltage);
void Appl_SccCbk_Set_ISO_AC_RCD(boolean RCD);

void Appl_SccCbk_Set_ISO_DC_EVSECurrentLimitAchieved(boolean EVSECurrentLimitAchieved);
void Appl_SccCbk_Set_ISO_DC_EVSECurrentRegulationTolerance(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSECurrentRegulationTolerance, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEEnergyToBeDelivered(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEEnergyToBeDelivered, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEIsolationStatus(Exi_ISO_isolationLevelType EVSEIsolationStatus, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEMaximumCurrentLimit(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumCurrentLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEMaximumPowerLimit(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumPowerLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEMaximumVoltageLimit(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumVoltageLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_ISO_DC_EVSEMinimumCurrentLimit(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMinimumCurrentLimit);
void Appl_SccCbk_Set_ISO_DC_EVSEMinimumVoltageLimit(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMinimumVoltageLimit);
void Appl_SccCbk_Set_ISO_DC_EVSEPeakCurrentRipple(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPeakCurrentRipple);
void Appl_SccCbk_Set_ISO_DC_EVSEPowerLimitAchieved(boolean EVSEPowerLimitAchieved);
void Appl_SccCbk_Set_ISO_DC_EVSEPresentCurrent(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPresentCurrent);
void Appl_SccCbk_Set_ISO_DC_EVSEPresentVoltage(P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPresentVoltage);
void Appl_SccCbk_Set_ISO_DC_EVSEStatusCode(Exi_ISO_DC_EVSEStatusCodeType EVSEStatusCode);
void Appl_SccCbk_Set_ISO_DC_EVSEVoltageLimitAchieved(boolean EVSEVoltageLimitAchieved);

#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )

/* Rx DIN */
void Appl_SccCbk_Get_DIN_ChargingProfilePtr(P2VAR(Exi_DIN_ChargingProfileType*, AUTOMATIC, SCC_APPL_DATA) ChargingProfilePtr, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVCabinConditioning(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVCabinConditioning, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVRESSConditioning(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVRESSConditioning, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVEnergyCapacity(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVEnergyCapacity, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVEnergyRequest(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVEnergyRequest, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVErrorCode(P2VAR(Exi_DIN_DC_EVErrorCodeType, AUTOMATIC, SCC_APPL_DATA) EVErrorCode);
void Appl_SccCbk_Get_DIN_EVMaximumCurrentLimit(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumCurrentLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVMaximumPowerLimit(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumPowerLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVMaximumVoltageLimit(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVMaximumVoltageLimit, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVTargetCurrent(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVTargetCurrent);
void Appl_SccCbk_Get_DIN_EVTargetVoltage(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVTargetVoltage);
void Appl_SccCbk_Get_DIN_RemainingTimeToBulkSoC(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) RemainingTimeToBulkSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_RemainingTimeToFullSoC(P2VAR(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) RemainingTimeToFullSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_BulkChargingComplete(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) BulkChargingComplete, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_BulkSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) BulkSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_ChargingComplete(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) ChargingComplete);
void Appl_SccCbk_Get_DIN_FullSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) FullSOC, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_DIN_EVPowerDeliveryParameterFlag(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVPowerDeliveryParameterFlag);
void Appl_SccCbk_Get_DIN_EVRESSSOC(P2VAR(sint8, AUTOMATIC, SCC_APPL_DATA) EVRESSSOC);
void Appl_SccCbk_Get_DIN_EVReady(P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) EVReady);
/* Tx DIN */
void Appl_SccCbk_Set_DIN_ChargeService(P2CONST(Exi_DIN_ServiceChargeType, AUTOMATIC, SCC_APPL_DATA) ChargeService);
void Appl_SccCbk_Set_DIN_DateTimeNow(Exi_SInt64 DateTimeNow, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSECurrentRegulationTolerance(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSECurrentRegulationTolerance, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEEnergyToBeDelivered(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEEnergyToBeDelivered, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEID(P2CONST(Exi_DIN_evseIDType, AUTOMATIC, SCC_APPL_DATA) EVSEID);
void Appl_SccCbk_Set_DIN_EVSEIsolationStatus(Exi_DIN_isolationLevelType EVSEIsolationStatus, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEMaximumCurrentLimit(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumCurrentLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEMaximumPowerLimit(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumPowerLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEMaximumVoltageLimit(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMaximumVoltageLimit, Exi_BitType Flag);
void Appl_SccCbk_Set_DIN_EVSEMinimumCurrentLimit(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMinimumCurrentLimit);
void Appl_SccCbk_Set_DIN_EVSEMinimumVoltageLimit(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEMinimumVoltageLimit);
void Appl_SccCbk_Set_DIN_EVSENotification(Exi_DIN_EVSENotificationType EVSENotification);
void Appl_SccCbk_Set_DIN_EVSEPeakCurrentRipple(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPeakCurrentRipple);
void Appl_SccCbk_Set_DIN_EVSEPresentCurrent(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPresentCurrent);
void Appl_SccCbk_Set_DIN_EVSEPresentVoltage(P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, SCC_APPL_DATA) EVSEPresentVoltage);
void Appl_SccCbk_Set_DIN_EVSEStatusCode(Exi_DIN_DC_EVSEStatusCodeType EVSEStatusCode);
void Appl_SccCbk_Set_DIN_NotificationMaxDelay(uint32 NotificationMaxDelay);
void Appl_SccCbk_Set_DIN_ResponseCode(Exi_DIN_responseCodeType ResponseCode);
void Appl_SccCbk_Set_DIN_SAScheduleList(P2CONST(Exi_DIN_SAScheduleListType, AUTOMATIC, SCC_APPL_DATA) SAScheduleList);
void Appl_SccCbk_Set_DIN_EVSECurrentLimitAchieved(boolean EVSECurrentLimitAchieved);
void Appl_SccCbk_Set_DIN_EVSEPowerLimitAchieved(boolean EVSEPowerLimitAchieved);
void Appl_SccCbk_Set_DIN_EVSEVoltageLimitAchieved(boolean EVSEVoltageLimitAchieved);

#endif

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )

void Appl_SccCbk_Get_Core_ContrCertPrivKeyPasswordPtr(P2VAR(Scc_BufferPointerType*, AUTOMATIC, SCC_APPL_DATA) PasswordPtr);
void Appl_SccCbk_Get_Core_ProvCertPrivKeyPasswordPtr(P2VAR(Scc_BufferPointerType*, AUTOMATIC, SCC_APPL_DATA) PasswordPtr);
void Appl_SccCbk_Get_Core_ContrCertPrivKeySaltPtr(P2VAR(Scc_BufferPointerType*, AUTOMATIC, SCC_APPL_DATA) SaltPtr, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);
void Appl_SccCbk_Get_Core_ProvCertPrivKeySaltPtr(P2VAR(Scc_BufferPointerType*, AUTOMATIC, SCC_APPL_DATA) SaltPtr, P2VAR(boolean, AUTOMATIC, SCC_APPL_DATA) Flag);

#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

#endif /* SCC_CBK_STUBS_H */
/**********************************************************************************************************************
 *  END OF FILE: _ApplScc_CbkStubs.h
 *********************************************************************************************************************/
