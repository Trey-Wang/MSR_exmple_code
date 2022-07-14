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
/**        \file  Scc_StateM_Vector.h
 *        \brief  Smart Charge Communication Header File
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
#if !defined (SCC_STATEM_H)
# define SCC_STATEM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Cfg.h"

#if ( defined SCC_ENABLE_STATE_MACHINE ) && ( SCC_ENABLE_STATE_MACHINE == STD_ON )

#include "Scc_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_FUNCCODE(void) Scc_StateM_InitMemory(void);
SCC_FUNCCODE(void) Scc_StateM_Init(void);
SCC_FUNCCODE(void) Scc_StateM_MainFunction(void);

/**********************************************************************************************************************
 *  Scc_StateM_Get***
 *********************************************************************************************************************/
/*! \brief      Scc <- Scc_StateMachine
 *  \context    task level
 *********************************************************************************************************************/
/* message independent */
SCC_FUNCCODE(void) Scc_StateM_Get_Core_MsgTrig(SCC_P2VARNOINIT(Scc_MsgTrigType) MsgTrig);
SCC_FUNCCODE(void) Scc_StateM_Get_Core_CyclicMsgTrigRx(SCC_P2VARNOINIT(boolean) CyclicMsgTrig);

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON )
/* SLAC */
SCC_FUNCCODE(void) Scc_StateM_Get_SLAC_QCAIdleTimer(SCC_P2VARNOINIT(uint16) QCAIdleTimer);
SCC_FUNCCODE(void) Scc_StateM_Get_SLAC_StartMode(SCC_P2VARNOINIT(EthTrcv_30_Ar7000_Slac_StartModeType) SLACStartMode);
#endif /* SCC_ENABLE_SLAC_HANDLING */

/* SECC Discovery Protocol */
SCC_FUNCCODE(void) Scc_StateM_Get_Core_SDPSecurityRx(SCC_P2VARNOINIT(Scc_SDPSecurityType) SDPSecurity);

/* ServiceDiscovery */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceScope(SCC_P2VARNOINIT(Exi_ISO_serviceScopeType) ServiceScope,
  SCC_P2VARNOINIT(boolean) Flag);
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceCategory(SCC_P2VARNOINIT(Exi_ISO_serviceCategoryType) ServiceCategory,
  SCC_P2VARNOINIT(boolean) Flag);
#endif /* SCC_SCHEMA_ISO */

/* ServiceDetail */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ServiceIDRx(SCC_P2VARNOINIT(uint16) ServiceID);
#endif /* SCC_SCHEMA_ISO */

/* Payment Service Selection */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_SelectedPaymentOption(SCC_P2VARNOINIT(Exi_ISO_paymentOptionType) SelectedPaymentOption);
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_SelectedServiceListPtr(SCC_P2VARNOINIT(Exi_ISO_SelectedServiceListType*) SelectedServiceListPtr);
#endif /* SCC_SCHEMA_ISO */

/* Charge Parameter Discovery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_RequestedEnergyTransferMode(SCC_P2VARNOINIT(Exi_ISO_EnergyTransferModeType) EnergyTransferMode);
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_DIN_RequestedEnergyTransferMode(SCC_P2VARNOINIT(Exi_DIN_EVRequestedEnergyTransferType) EnergyTransferMode);
#endif /* SCC_SCHEMA_DIN */

/* Power Delivery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_ISO_ChargeProgress(SCC_P2VARNOINIT(Exi_ISO_chargeProgressType) ChargeProgress);
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Get_DIN_ReadyToChargeState(SCC_P2VARNOINIT(boolean) ReadyToChargeState);
#endif /* SCC_SCHEMA_DIN */

/**********************************************************************************************************************
 *  Scc_StateM_Set***
 *********************************************************************************************************************/
/*! \brief      Scc -> Scc_StateMachine
 *  \context    task level
 *********************************************************************************************************************/
/* message independent */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_TrcvLinkState(boolean LinkStateActive);
SCC_FUNCCODE(void) Scc_StateM_Set_Core_IPAssigned(boolean IPAssigned);
SCC_FUNCCODE(void) Scc_StateM_Set_Core_CyclicMsgTrigTx(boolean CyclicMsgTrig);
SCC_FUNCCODE(void) Scc_StateM_Set_Core_CyclicMsgRcvd(boolean CyclicMsgRcvd);
SCC_FUNCCODE(void) Scc_StateM_Set_Core_MsgStatus(Scc_MsgStatusType MsgStatus);
SCC_FUNCCODE(void) Scc_StateM_Set_Core_StackError(Scc_StackErrorType StackError);

/* multiple messages */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_EVSEProcessing(Exi_ISO_EVSEProcessingType EVSEProcessing);
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_DIN_EVSEProcessing(Exi_DIN_EVSEProcessingType EVSEProcessing);
#endif /* SCC_SCHEMA_DIN */

/* SECC Discovery Protocol */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_SDPSecurityTx(Scc_SDPSecurityType SDPSecurity);

/* Supported App Protocol */
SCC_FUNCCODE(void) Scc_StateM_Set_Core_SAPSchemaID(Scc_SAPSchemaIDType SAPSchemaID);

/* Service Discovery */
#if ( ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ChargeService(SCC_P2CONSTNOINIT(Exi_ISO_ChargeServiceType) ChargeService);
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_PaymentOptionList(SCC_P2CONSTNOINIT(Exi_ISO_PaymentOptionListType) PaymentOptionList);
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceList(SCC_P2CONSTNOINIT(Exi_ISO_ServiceListType) ServiceList, Exi_BitType Flag);
#endif /* SCC_SCHEMA_ISO */
#if ( ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 ) )
SCC_FUNCCODE(void) Scc_StateM_Set_DIN_ChargeService(SCC_P2CONSTNOINIT(Exi_DIN_ServiceChargeType) ChargeService);
#endif /* SCC_SCHEMA_DIN */

/* ServiceDetail */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceIDTx(uint16 ServiceID);
SCC_FUNCCODE(void) Scc_StateM_Set_ISO_ServiceParameterList(
  SCC_P2CONSTNOINIT(Exi_ISO_ServiceParameterListType) ServiceParameterList, Exi_BitType ServiceParameterListFlag);
#endif /* SCC_SCHEMA_ISO */

/* ChargingStatus */

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* SCC_ENABLE_STATE_MACHINE */

#endif /* SCC_STATEM_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc_StateM_Vector.h
 *********************************************************************************************************************/
