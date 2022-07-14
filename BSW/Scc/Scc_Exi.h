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
/**        \file  Scc_Exi.h
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
#if !defined (SCC_EXI_H)
# define SCC_EXI_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Types.h"
#include "Scc_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL MACROS
 *********************************************************************************************************************/
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
#define SCC_EXI_SAP_ISO_FDIS_PROTOCOL_NAMESPACE     "urn:iso:15118:2:2013:MsgDef"
#define SCC_EXI_SAP_ISO_FDIS_PROTOCOL_NAMESPACE_LEN 27u
#define SCC_EXI_SAP_ISO_FDIS_VERSION_NUMBER_MAJOR   2u
#define SCC_EXI_SAP_ISO_FDIS_VERSION_NUMBER_MINOR   0u
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
#define SCC_EXI_SAP_ISO_ED2_CD_PROTOCOL_NAMESPACE     "urn:iso:15118:2:2016:MsgDef"
#define SCC_EXI_SAP_ISO_ED2_CD_PROTOCOL_NAMESPACE_LEN 27u
#define SCC_EXI_SAP_ISO_ED2_CD_VERSION_NUMBER_MAJOR   3u
#define SCC_EXI_SAP_ISO_ED2_CD_VERSION_NUMBER_MINOR   0u
#endif /* SCC_SCHEMA_ISO_ED2_CD */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
#define SCC_EXI_SAP_DIN_PROTOCOL_NAMESPACE     "urn:din:70121:2012:MsgDef"
#define SCC_EXI_SAP_DIN_PROTOCOL_NAMESPACE_LEN 25u
#define SCC_EXI_SAP_DIN_VERSION_NUMBER_MAJOR   2u
#define SCC_EXI_SAP_DIN_VERSION_NUMBER_MINOR   0u
#endif /* SCC_SCHEMA_DIN */

#define Scc_Exi_StructBuf Scc_Exi_StructBufUnion.Buf8

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/
/* 8bit variables */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
extern SCC_VARNOINIT(uint8) Scc_Exi_SAScheduleTupleID;
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 16bit variables */
#define SCC_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
extern SCC_VARNOINIT(uint16) Scc_ExiRx_DIN_ServiceID;
#endif /* SCC_SCHEMA_DIN */

#define SCC_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern SCC_VARNOINIT(Exi_EncodeWorkspaceType) Scc_Exi_EncWs;
extern SCC_VARNOINIT(Exi_DecodeWorkspaceType) Scc_Exi_DecWs;

extern SCC_VARNOINIT(Scc_Exi_StructBufType) Scc_Exi_StructBufUnion; /* PRQA S 0759 */ /* MD_Scc_0750_0759 */

#if (( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON ) && ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON ))
extern SCC_VARNOINIT(Exi_ISO_EnergyTransferModeType) Scc_ExiTx_ISO_EnergyTransferMode;
#endif /* SCC_CHARGING_AC, SCC_CHARGING_DC */

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
extern SCC_VARNOINIT(Exi_ISO_paymentOptionType)      Scc_Exi_PaymentOption;
extern SCC_VARNOINIT(Exi_ISO_chargingSessionType)    Scc_Exi_ChargingSession;
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_FUNCCODE(void) Scc_Exi_Init(void);
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_StreamRequest(void);

#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
SCC_FUNCCODE(void)           Scc_ExiTx_ISO_Init(void);
SCC_FUNCCODE(Scc_ReturnType) Scc_ExiTx_ISO_EncodeMessage(void);
SCC_FUNCCODE(void)           Scc_ExiRx_ISO_Init(void);
SCC_FUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_DecodeMessage(void);
#endif /* SCC_SCHEMA_ISO */

#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
SCC_FUNCCODE(void)           Scc_ExiTx_ISO_Ed2_CD_Init(void);
SCC_FUNCCODE(Scc_ReturnType) Scc_ExiTx_ISO_Ed2_CD_EncodeMessage(void);
SCC_FUNCCODE(void)           Scc_ExiRx_ISO_Ed2_CD_Init(void);
SCC_FUNCCODE(Std_ReturnType) Scc_ExiRx_ISO_Ed2_CD_DecodeMessage(void);
#endif /* SCC_SCHEMA_ISO_ED2_CD */

#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
SCC_FUNCCODE(void)           Scc_ExiTx_DIN_Init(void);
SCC_FUNCCODE(Scc_ReturnType) Scc_ExiTx_DIN_EncodeMessage(void);
SCC_FUNCCODE(void)           Scc_ExiRx_DIN_Init(void);
SCC_FUNCCODE(Std_ReturnType) Scc_ExiRx_DIN_DecodeMessage(void);
#endif /* SCC_SCHEMA_DIN */

/* Common */
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_InitEncodingWorkspace(void);
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_InitDecodingWorkspace(void);
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_EncodeAndTransmitExiStream(uint16 PayloadType);

/* SupportedAppProtocol */
SCC_FUNCCODE(Scc_ReturnType) Scc_Exi_EncodeSupportedAppProtocolReq(void);
SCC_FUNCCODE(Std_ReturnType) Scc_Exi_DecodeSupportedAppProtocolRes(void);

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* SCC_EXI_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc_Exi.h
 *********************************************************************************************************************/
