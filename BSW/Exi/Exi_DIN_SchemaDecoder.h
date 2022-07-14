/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Exi_DIN_SchemaDecoder.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  implementation
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file Exi.h.
 *********************************************************************************************************************/
#if defined(C_COMMENT_VECTOR)
/*  TODOs:
 *  - *
 */
#endif


#if !defined (EXI_DIN_SCHEMA_DECODER_H)
# define EXI_DIN_SCHEMA_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaDecoder.h"

#if (!defined (EXI_ENABLE_DECODE_DIN_MESSAGE_SET))
# if (defined (EXI_ENABLE_DIN_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_DIN_MESSAGE_SET   EXI_ENABLE_DIN_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_DIN_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_DIN_MESSAGE_SET) && (EXI_ENABLE_DECODE_DIN_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_DECODE_DIN_AC_EVCHARGE_PARAMETER 0x30U
#define EXI_API_ID_DECODE_DIN_AC_EVSECHARGE_PARAMETER 0x31U
#define EXI_API_ID_DECODE_DIN_AC_EVSESTATUS 0x32U
#define EXI_API_ID_DECODE_DIN_ATTRIBUTE_ID 0x33U
#define EXI_API_ID_DECODE_DIN_ATTRIBUTE_NAME 0x34U
#define EXI_API_ID_DECODE_DIN_ATTRIBUTE_VALUE 0x35U
#define EXI_API_ID_DECODE_DIN_BODY 0x36U
#define EXI_API_ID_DECODE_DIN_CABLE_CHECK_REQ 0x37U
#define EXI_API_ID_DECODE_DIN_CABLE_CHECK_RES 0x38U
#define EXI_API_ID_DECODE_DIN_CERTIFICATE_CHAIN 0x39U
#define EXI_API_ID_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ 0x3AU
#define EXI_API_ID_DECODE_DIN_CERTIFICATE_INSTALLATION_RES 0x3BU
#define EXI_API_ID_DECODE_DIN_CERTIFICATE_UPDATE_REQ 0x3CU
#define EXI_API_ID_DECODE_DIN_CERTIFICATE_UPDATE_RES 0x3DU
#define EXI_API_ID_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ 0x3EU
#define EXI_API_ID_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES 0x3FU
#define EXI_API_ID_DECODE_DIN_CHARGING_PROFILE 0x40U
#define EXI_API_ID_DECODE_DIN_CHARGING_STATUS_RES 0x41U
#define EXI_API_ID_DECODE_DIN_CONSUMPTION_COST 0x42U
#define EXI_API_ID_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ 0x43U
#define EXI_API_ID_DECODE_DIN_CONTRACT_AUTHENTICATION_RES 0x44U
#define EXI_API_ID_DECODE_DIN_COST 0x45U
#define EXI_API_ID_DECODE_DIN_CURRENT_DEMAND_REQ 0x46U
#define EXI_API_ID_DECODE_DIN_CURRENT_DEMAND_RES 0x47U
#define EXI_API_ID_DECODE_DIN_DC_EVCHARGE_PARAMETER 0x48U
#define EXI_API_ID_DECODE_DIN_DC_EVERROR_CODE 0x49U
#define EXI_API_ID_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER 0x4AU
#define EXI_API_ID_DECODE_DIN_DC_EVSECHARGE_PARAMETER 0x4BU
#define EXI_API_ID_DECODE_DIN_DC_EVSESTATUS_CODE 0x4CU
#define EXI_API_ID_DECODE_DIN_DC_EVSESTATUS 0x4DU
#define EXI_API_ID_DECODE_DIN_DC_EVSTATUS 0x4EU
#define EXI_API_ID_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER 0x4FU
#define EXI_API_ID_DECODE_DIN_EVSENOTIFICATION 0x50U
#define EXI_API_ID_DECODE_DIN_EVSEPROCESSING 0x51U
#define EXI_API_ID_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER 0x52U
#define EXI_API_ID_DECODE_DIN_ENTRY 0x53U
#define EXI_API_ID_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS 0x54U
#define EXI_API_ID_DECODE_DIN_MESSAGE_HEADER 0x55U
#define EXI_API_ID_DECODE_DIN_METER_INFO 0x56U
#define EXI_API_ID_DECODE_DIN_METERING_RECEIPT_REQ 0x57U
#define EXI_API_ID_DECODE_DIN_METERING_RECEIPT_RES 0x58U
#define EXI_API_ID_DECODE_DIN_NOTIFICATION 0x59U
#define EXI_API_ID_DECODE_DIN_PMAX_SCHEDULE_ENTRY 0x5AU
#define EXI_API_ID_DECODE_DIN_PMAX_SCHEDULE 0x5BU
#define EXI_API_ID_DECODE_DIN_PARAMETER_SET 0x5CU
#define EXI_API_ID_DECODE_DIN_PARAMETER 0x5DU
#define EXI_API_ID_DECODE_DIN_PAYMENT_DETAILS_REQ 0x5EU
#define EXI_API_ID_DECODE_DIN_PAYMENT_DETAILS_RES 0x5FU
#define EXI_API_ID_DECODE_DIN_PAYMENT_OPTIONS 0x60U
#define EXI_API_ID_DECODE_DIN_PHYSICAL_VALUE 0x61U
#define EXI_API_ID_DECODE_DIN_POWER_DELIVERY_REQ 0x62U
#define EXI_API_ID_DECODE_DIN_POWER_DELIVERY_RES 0x63U
#define EXI_API_ID_DECODE_DIN_PRE_CHARGE_REQ 0x64U
#define EXI_API_ID_DECODE_DIN_PRE_CHARGE_RES 0x65U
#define EXI_API_ID_DECODE_DIN_PROFILE_ENTRY 0x66U
#define EXI_API_ID_DECODE_DIN_RELATIVE_TIME_INTERVAL 0x67U
#define EXI_API_ID_DECODE_DIN_SASCHEDULE_LIST 0x68U
#define EXI_API_ID_DECODE_DIN_SASCHEDULE_TUPLE 0x69U
#define EXI_API_ID_DECODE_DIN_SALES_TARIFF_ENTRY 0x6AU
#define EXI_API_ID_DECODE_DIN_SALES_TARIFF 0x6BU
#define EXI_API_ID_DECODE_DIN_SELECTED_SERVICE_LIST 0x6CU
#define EXI_API_ID_DECODE_DIN_SELECTED_SERVICE 0x6DU
#define EXI_API_ID_DECODE_DIN_SERVICE_CHARGE 0x6EU
#define EXI_API_ID_DECODE_DIN_SERVICE_DETAIL_REQ 0x6FU
#define EXI_API_ID_DECODE_DIN_SERVICE_DETAIL_RES 0x70U
#define EXI_API_ID_DECODE_DIN_SERVICE_DISCOVERY_REQ 0x71U
#define EXI_API_ID_DECODE_DIN_SERVICE_DISCOVERY_RES 0x72U
#define EXI_API_ID_DECODE_DIN_SERVICE_PARAMETER_LIST 0x73U
#define EXI_API_ID_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ 0x74U
#define EXI_API_ID_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES 0x75U
#define EXI_API_ID_DECODE_DIN_SERVICE_TAG_LIST 0x76U
#define EXI_API_ID_DECODE_DIN_SERVICE_TAG 0x77U
#define EXI_API_ID_DECODE_DIN_SERVICE 0x78U
#define EXI_API_ID_DECODE_DIN_SESSION_SETUP_REQ 0x79U
#define EXI_API_ID_DECODE_DIN_SESSION_SETUP_RES 0x7AU
#define EXI_API_ID_DECODE_DIN_SESSION_STOP_RES 0x7BU
#define EXI_API_ID_DECODE_DIN_SUB_CERTIFICATES 0x7CU
#define EXI_API_ID_DECODE_DIN_V2G_MESSAGE 0x7DU
#define EXI_API_ID_DECODE_DIN_WELDING_DETECTION_REQ 0x7EU
#define EXI_API_ID_DECODE_DIN_WELDING_DETECTION_RES 0x7FU
#define EXI_API_ID_DECODE_DIN_CERTIFICATE 0x80U
#define EXI_API_ID_DECODE_DIN_CONTRACT_ID 0x81U
#define EXI_API_ID_DECODE_DIN_COST_KIND 0x82U
#define EXI_API_ID_DECODE_DIN_D_HPARAMS 0x83U
#define EXI_API_ID_DECODE_DIN_EVCC_ID 0x84U
#define EXI_API_ID_DECODE_DIN_EVSE_ID 0x85U
#define EXI_API_ID_DECODE_DIN_FAULT_CODE 0x86U
#define EXI_API_ID_DECODE_DIN_FAULT_MSG 0x87U
#define EXI_API_ID_DECODE_DIN_GEN_CHALLENGE 0x88U
#define EXI_API_ID_DECODE_DIN_ISOLATION_LEVEL 0x89U
#define EXI_API_ID_DECODE_DIN_METER_ID 0x8AU
#define EXI_API_ID_DECODE_DIN_PAYMENT_OPTION 0x8BU
#define EXI_API_ID_DECODE_DIN_PRIVATE_KEY 0x8CU
#define EXI_API_ID_DECODE_DIN_RESPONSE_CODE 0x8DU
#define EXI_API_ID_DECODE_DIN_ROOT_CERTIFICATE_ID 0x8EU
#define EXI_API_ID_DECODE_DIN_SERVICE_CATEGORY 0x8FU
#define EXI_API_ID_DECODE_DIN_SERVICE_NAME 0x90U
#define EXI_API_ID_DECODE_DIN_SERVICE_SCOPE 0x91U
#define EXI_API_ID_DECODE_DIN_SESSION_ID 0x92U
#define EXI_API_ID_DECODE_DIN_SIG_METER_READING 0x93U
#define EXI_API_ID_DECODE_DIN_TARIFF_DESCRIPTION 0x94U
#define EXI_API_ID_DECODE_DIN_UNIT_SYMBOL 0x95U
#define EXI_API_ID_DECODE_SCHEMA_SET_DIN 0x176U
#endif

/* Decoding default switches */
#ifndef EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER
#define EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER
#define EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_AC_EVSESTATUS
#define EXI_DECODE_DIN_AC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ATTRIBUTE_ID
#define EXI_DECODE_DIN_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ATTRIBUTE_NAME
#define EXI_DECODE_DIN_ATTRIBUTE_NAME STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ATTRIBUTE_VALUE
#define EXI_DECODE_DIN_ATTRIBUTE_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_BODY_BASE
#define EXI_DECODE_DIN_BODY_BASE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_BODY
#define EXI_DECODE_DIN_BODY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CABLE_CHECK_REQ
#define EXI_DECODE_DIN_CABLE_CHECK_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CABLE_CHECK_RES
#define EXI_DECODE_DIN_CABLE_CHECK_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE_CHAIN
#define EXI_DECODE_DIN_CERTIFICATE_CHAIN STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ
#define EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES
#define EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ
#define EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES
#define EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ
#define EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES
#define EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CHARGING_PROFILE
#define EXI_DECODE_DIN_CHARGING_PROFILE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CHARGING_STATUS_REQ
#define EXI_DECODE_DIN_CHARGING_STATUS_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CHARGING_STATUS_RES
#define EXI_DECODE_DIN_CHARGING_STATUS_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CONSUMPTION_COST
#define EXI_DECODE_DIN_CONSUMPTION_COST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ
#define EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES
#define EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_COST
#define EXI_DECODE_DIN_COST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CURRENT_DEMAND_REQ
#define EXI_DECODE_DIN_CURRENT_DEMAND_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CURRENT_DEMAND_RES
#define EXI_DECODE_DIN_CURRENT_DEMAND_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER
#define EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVERROR_CODE
#define EXI_DECODE_DIN_DC_EVERROR_CODE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER
#define EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER
#define EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVSESTATUS_CODE
#define EXI_DECODE_DIN_DC_EVSESTATUS_CODE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVSESTATUS
#define EXI_DECODE_DIN_DC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_DC_EVSTATUS
#define EXI_DECODE_DIN_DC_EVSTATUS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVCHARGE_PARAMETER
#define EXI_DECODE_DIN_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVPOWER_DELIVERY_PARAMETER
#define EXI_DECODE_DIN_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER
#define EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSECHARGE_PARAMETER
#define EXI_DECODE_DIN_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSENOTIFICATION
#define EXI_DECODE_DIN_EVSENOTIFICATION STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSEPROCESSING
#define EXI_DECODE_DIN_EVSEPROCESSING STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSESTATUS
#define EXI_DECODE_DIN_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER
#define EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSTATUS
#define EXI_DECODE_DIN_EVSTATUS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ENTRY
#define EXI_DECODE_DIN_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_INTERVAL
#define EXI_DECODE_DIN_INTERVAL STD_OFF
#endif
#ifndef EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS
#define EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_MESSAGE_HEADER
#define EXI_DECODE_DIN_MESSAGE_HEADER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_METER_INFO
#define EXI_DECODE_DIN_METER_INFO STD_OFF
#endif
#ifndef EXI_DECODE_DIN_METERING_RECEIPT_REQ
#define EXI_DECODE_DIN_METERING_RECEIPT_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_METERING_RECEIPT_RES
#define EXI_DECODE_DIN_METERING_RECEIPT_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_NOTIFICATION
#define EXI_DECODE_DIN_NOTIFICATION STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY
#define EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PMAX_SCHEDULE
#define EXI_DECODE_DIN_PMAX_SCHEDULE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PARAMETER_SET
#define EXI_DECODE_DIN_PARAMETER_SET STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PARAMETER
#define EXI_DECODE_DIN_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PAYMENT_DETAILS_REQ
#define EXI_DECODE_DIN_PAYMENT_DETAILS_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PAYMENT_DETAILS_RES
#define EXI_DECODE_DIN_PAYMENT_DETAILS_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PAYMENT_OPTIONS
#define EXI_DECODE_DIN_PAYMENT_OPTIONS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PHYSICAL_VALUE
#define EXI_DECODE_DIN_PHYSICAL_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_POWER_DELIVERY_REQ
#define EXI_DECODE_DIN_POWER_DELIVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_POWER_DELIVERY_RES
#define EXI_DECODE_DIN_POWER_DELIVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PRE_CHARGE_REQ
#define EXI_DECODE_DIN_PRE_CHARGE_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PRE_CHARGE_RES
#define EXI_DECODE_DIN_PRE_CHARGE_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PROFILE_ENTRY
#define EXI_DECODE_DIN_PROFILE_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL
#define EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SASCHEDULE_LIST
#define EXI_DECODE_DIN_SASCHEDULE_LIST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SASCHEDULE_TUPLE
#define EXI_DECODE_DIN_SASCHEDULE_TUPLE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SASCHEDULES
#define EXI_DECODE_DIN_SASCHEDULES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SALES_TARIFF_ENTRY
#define EXI_DECODE_DIN_SALES_TARIFF_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SALES_TARIFF
#define EXI_DECODE_DIN_SALES_TARIFF STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SELECTED_SERVICE_LIST
#define EXI_DECODE_DIN_SELECTED_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SELECTED_SERVICE
#define EXI_DECODE_DIN_SELECTED_SERVICE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_CHARGE
#define EXI_DECODE_DIN_SERVICE_CHARGE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_DETAIL_REQ
#define EXI_DECODE_DIN_SERVICE_DETAIL_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_DETAIL_RES
#define EXI_DECODE_DIN_SERVICE_DETAIL_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ
#define EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_DISCOVERY_RES
#define EXI_DECODE_DIN_SERVICE_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_PARAMETER_LIST
#define EXI_DECODE_DIN_SERVICE_PARAMETER_LIST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ
#define EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES
#define EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_TAG_LIST
#define EXI_DECODE_DIN_SERVICE_TAG_LIST STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_TAG
#define EXI_DECODE_DIN_SERVICE_TAG STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE
#define EXI_DECODE_DIN_SERVICE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SESSION_SETUP_REQ
#define EXI_DECODE_DIN_SESSION_SETUP_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SESSION_SETUP_RES
#define EXI_DECODE_DIN_SESSION_SETUP_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SESSION_STOP_REQ
#define EXI_DECODE_DIN_SESSION_STOP_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SESSION_STOP_RES
#define EXI_DECODE_DIN_SESSION_STOP_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SUB_CERTIFICATES
#define EXI_DECODE_DIN_SUB_CERTIFICATES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_V2G_MESSAGE
#define EXI_DECODE_DIN_V2G_MESSAGE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_WELDING_DETECTION_REQ
#define EXI_DECODE_DIN_WELDING_DETECTION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_DIN_WELDING_DETECTION_RES
#define EXI_DECODE_DIN_WELDING_DETECTION_RES STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CERTIFICATE
#define EXI_DECODE_DIN_CERTIFICATE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_CONTRACT_ID
#define EXI_DECODE_DIN_CONTRACT_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_COST_KIND
#define EXI_DECODE_DIN_COST_KIND STD_OFF
#endif
#ifndef EXI_DECODE_DIN_D_HPARAMS
#define EXI_DECODE_DIN_D_HPARAMS STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVCC_ID
#define EXI_DECODE_DIN_EVCC_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_EVSE_ID
#define EXI_DECODE_DIN_EVSE_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_FAULT_CODE
#define EXI_DECODE_DIN_FAULT_CODE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_FAULT_MSG
#define EXI_DECODE_DIN_FAULT_MSG STD_OFF
#endif
#ifndef EXI_DECODE_DIN_GEN_CHALLENGE
#define EXI_DECODE_DIN_GEN_CHALLENGE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ISOLATION_LEVEL
#define EXI_DECODE_DIN_ISOLATION_LEVEL STD_OFF
#endif
#ifndef EXI_DECODE_DIN_METER_ID
#define EXI_DECODE_DIN_METER_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PAYMENT_OPTION
#define EXI_DECODE_DIN_PAYMENT_OPTION STD_OFF
#endif
#ifndef EXI_DECODE_DIN_PRIVATE_KEY
#define EXI_DECODE_DIN_PRIVATE_KEY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_RESPONSE_CODE
#define EXI_DECODE_DIN_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_ROOT_CERTIFICATE_ID
#define EXI_DECODE_DIN_ROOT_CERTIFICATE_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_CATEGORY
#define EXI_DECODE_DIN_SERVICE_CATEGORY STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_NAME
#define EXI_DECODE_DIN_SERVICE_NAME STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SERVICE_SCOPE
#define EXI_DECODE_DIN_SERVICE_SCOPE STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SESSION_ID
#define EXI_DECODE_DIN_SESSION_ID STD_OFF
#endif
#ifndef EXI_DECODE_DIN_SIG_METER_READING
#define EXI_DECODE_DIN_SIG_METER_READING STD_OFF
#endif
#ifndef EXI_DECODE_DIN_TARIFF_DESCRIPTION
#define EXI_DECODE_DIN_TARIFF_DESCRIPTION STD_OFF
#endif
#ifndef EXI_DECODE_DIN_UNIT_SYMBOL
#define EXI_DECODE_DIN_UNIT_SYMBOL STD_OFF
#endif
#ifndef EXI_DECODE_SCHEMA_SET_DIN
#define EXI_DECODE_SCHEMA_SET_DIN STD_OFF
#endif


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Decode_DIN_AC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AC_EVChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER) && (EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AC_EVChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER) && (EXI_DECODE_DIN_AC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_AC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AC_EVSEChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER) && (EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AC_EVSEChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER) && (EXI_DECODE_DIN_AC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_AC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AC_EVSEStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_AC_EVSESTATUS) && (EXI_DECODE_DIN_AC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AC_EVSEStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_AC_EVSESTATUS) && (EXI_DECODE_DIN_AC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_AttributeId
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AttributeIdType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ATTRIBUTE_ID) && (EXI_DECODE_DIN_ATTRIBUTE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AttributeId( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ATTRIBUTE_ID) && (EXI_DECODE_DIN_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_AttributeName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AttributeNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ATTRIBUTE_NAME) && (EXI_DECODE_DIN_ATTRIBUTE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AttributeName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ATTRIBUTE_NAME) && (EXI_DECODE_DIN_ATTRIBUTE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_AttributeValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_AttributeValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ATTRIBUTE_VALUE) && (EXI_DECODE_DIN_ATTRIBUTE_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_AttributeValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ATTRIBUTE_VALUE) && (EXI_DECODE_DIN_ATTRIBUTE_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Body
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_BodyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_BODY) && (EXI_DECODE_DIN_BODY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Body( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_BODY) && (EXI_DECODE_DIN_BODY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CableCheckReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CableCheckReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CABLE_CHECK_REQ) && (EXI_DECODE_DIN_CABLE_CHECK_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CableCheckReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CABLE_CHECK_REQ) && (EXI_DECODE_DIN_CABLE_CHECK_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CableCheckRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CableCheckResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CABLE_CHECK_RES) && (EXI_DECODE_DIN_CABLE_CHECK_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CableCheckRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CABLE_CHECK_RES) && (EXI_DECODE_DIN_CABLE_CHECK_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CertificateChain
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CertificateChainType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE_CHAIN) && (EXI_DECODE_DIN_CERTIFICATE_CHAIN == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CertificateChain( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE_CHAIN) && (EXI_DECODE_DIN_CERTIFICATE_CHAIN == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CertificateInstallationReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CertificateInstallationReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ) && (EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CertificateInstallationReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ) && (EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CertificateInstallationRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CertificateInstallationResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES) && (EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CertificateInstallationRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES) && (EXI_DECODE_DIN_CERTIFICATE_INSTALLATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CertificateUpdateReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CertificateUpdateReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ) && (EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CertificateUpdateReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ) && (EXI_DECODE_DIN_CERTIFICATE_UPDATE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CertificateUpdateRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CertificateUpdateResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES) && (EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CertificateUpdateRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES) && (EXI_DECODE_DIN_CERTIFICATE_UPDATE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ChargeParameterDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ChargeParameterDiscoveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ChargeParameterDiscoveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ChargeParameterDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ChargeParameterDiscoveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ChargeParameterDiscoveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_DECODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ChargingProfile
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ChargingProfileType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CHARGING_PROFILE) && (EXI_DECODE_DIN_CHARGING_PROFILE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ChargingProfile( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CHARGING_PROFILE) && (EXI_DECODE_DIN_CHARGING_PROFILE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ChargingStatusRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ChargingStatusResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CHARGING_STATUS_RES) && (EXI_DECODE_DIN_CHARGING_STATUS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ChargingStatusRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CHARGING_STATUS_RES) && (EXI_DECODE_DIN_CHARGING_STATUS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ConsumptionCost
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ConsumptionCostType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CONSUMPTION_COST) && (EXI_DECODE_DIN_CONSUMPTION_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ConsumptionCost( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CONSUMPTION_COST) && (EXI_DECODE_DIN_CONSUMPTION_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ContractAuthenticationReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ContractAuthenticationReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ) && (EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ContractAuthenticationReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ) && (EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ContractAuthenticationRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ContractAuthenticationResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES) && (EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ContractAuthenticationRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES) && (EXI_DECODE_DIN_CONTRACT_AUTHENTICATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Cost
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CostType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_COST) && (EXI_DECODE_DIN_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Cost( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_COST) && (EXI_DECODE_DIN_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CurrentDemandReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CurrentDemandReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CURRENT_DEMAND_REQ) && (EXI_DECODE_DIN_CURRENT_DEMAND_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CurrentDemandReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CURRENT_DEMAND_REQ) && (EXI_DECODE_DIN_CURRENT_DEMAND_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_CurrentDemandRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_CurrentDemandResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CURRENT_DEMAND_RES) && (EXI_DECODE_DIN_CURRENT_DEMAND_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_CurrentDemandRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CURRENT_DEMAND_RES) && (EXI_DECODE_DIN_CURRENT_DEMAND_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER) && (EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER) && (EXI_DECODE_DIN_DC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVErrorCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVErrorCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVERROR_CODE) && (EXI_DECODE_DIN_DC_EVERROR_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVErrorCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVERROR_CODE) && (EXI_DECODE_DIN_DC_EVERROR_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVPowerDeliveryParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVPowerDeliveryParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVPowerDeliveryParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_DECODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVSEChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER) && (EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVSEChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER) && (EXI_DECODE_DIN_DC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVSEStatusCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVSEStatusCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVSESTATUS_CODE) && (EXI_DECODE_DIN_DC_EVSESTATUS_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVSEStatusCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVSESTATUS_CODE) && (EXI_DECODE_DIN_DC_EVSESTATUS_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVSEStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVSESTATUS) && (EXI_DECODE_DIN_DC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVSEStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVSESTATUS) && (EXI_DECODE_DIN_DC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_DC_EVStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_DC_EVStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_DC_EVSTATUS) && (EXI_DECODE_DIN_DC_EVSTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_DC_EVStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_DC_EVSTATUS) && (EXI_DECODE_DIN_DC_EVSTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_EVRequestedEnergyTransfer
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_EVRequestedEnergyTransferType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER) && (EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_EVRequestedEnergyTransfer( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER) && (EXI_DECODE_DIN_EVREQUESTED_ENERGY_TRANSFER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_EVSENotification
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_EVSENotificationType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVSENOTIFICATION) && (EXI_DECODE_DIN_EVSENOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_EVSENotification( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVSENOTIFICATION) && (EXI_DECODE_DIN_EVSENOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_EVSEProcessing
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_EVSEProcessingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVSEPROCESSING) && (EXI_DECODE_DIN_EVSEPROCESSING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_EVSEProcessing( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVSEPROCESSING) && (EXI_DECODE_DIN_EVSEPROCESSING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_EVSESupportedEnergyTransfer
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_EVSESupportedEnergyTransferType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER) && (EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_EVSESupportedEnergyTransfer( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER) && (EXI_DECODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Entry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_EntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ENTRY) && (EXI_DECODE_DIN_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Entry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ENTRY) && (EXI_DECODE_DIN_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ListOfRootCertificateIDs
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ListOfRootCertificateIDsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ListOfRootCertificateIDs( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_DECODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_MessageHeader
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_MessageHeaderType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_MESSAGE_HEADER) && (EXI_DECODE_DIN_MESSAGE_HEADER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_MessageHeader( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_MESSAGE_HEADER) && (EXI_DECODE_DIN_MESSAGE_HEADER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_MeterInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_MeterInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_METER_INFO) && (EXI_DECODE_DIN_METER_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_MeterInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_METER_INFO) && (EXI_DECODE_DIN_METER_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_MeteringReceiptReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_MeteringReceiptReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_METERING_RECEIPT_REQ) && (EXI_DECODE_DIN_METERING_RECEIPT_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_MeteringReceiptReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_METERING_RECEIPT_REQ) && (EXI_DECODE_DIN_METERING_RECEIPT_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_MeteringReceiptRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_MeteringReceiptResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_METERING_RECEIPT_RES) && (EXI_DECODE_DIN_METERING_RECEIPT_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_MeteringReceiptRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_METERING_RECEIPT_RES) && (EXI_DECODE_DIN_METERING_RECEIPT_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Notification
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_NotificationType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_NOTIFICATION) && (EXI_DECODE_DIN_NOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Notification( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_NOTIFICATION) && (EXI_DECODE_DIN_NOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PMaxScheduleEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PMaxScheduleEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY) && (EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PMaxScheduleEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY) && (EXI_DECODE_DIN_PMAX_SCHEDULE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PMaxSchedule
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PMaxScheduleType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PMAX_SCHEDULE) && (EXI_DECODE_DIN_PMAX_SCHEDULE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PMaxSchedule( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PMAX_SCHEDULE) && (EXI_DECODE_DIN_PMAX_SCHEDULE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ParameterSet
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ParameterSetType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PARAMETER_SET) && (EXI_DECODE_DIN_PARAMETER_SET == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ParameterSet( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PARAMETER_SET) && (EXI_DECODE_DIN_PARAMETER_SET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Parameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PARAMETER) && (EXI_DECODE_DIN_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Parameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PARAMETER) && (EXI_DECODE_DIN_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PaymentDetailsReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PaymentDetailsReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PAYMENT_DETAILS_REQ) && (EXI_DECODE_DIN_PAYMENT_DETAILS_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PaymentDetailsReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PAYMENT_DETAILS_REQ) && (EXI_DECODE_DIN_PAYMENT_DETAILS_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PaymentDetailsRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PaymentDetailsResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PAYMENT_DETAILS_RES) && (EXI_DECODE_DIN_PAYMENT_DETAILS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PaymentDetailsRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PAYMENT_DETAILS_RES) && (EXI_DECODE_DIN_PAYMENT_DETAILS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PaymentOptions
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PaymentOptionsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PAYMENT_OPTIONS) && (EXI_DECODE_DIN_PAYMENT_OPTIONS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PaymentOptions( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PAYMENT_OPTIONS) && (EXI_DECODE_DIN_PAYMENT_OPTIONS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PhysicalValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PhysicalValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PHYSICAL_VALUE) && (EXI_DECODE_DIN_PHYSICAL_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PhysicalValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PHYSICAL_VALUE) && (EXI_DECODE_DIN_PHYSICAL_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PowerDeliveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PowerDeliveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_POWER_DELIVERY_REQ) && (EXI_DECODE_DIN_POWER_DELIVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PowerDeliveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_POWER_DELIVERY_REQ) && (EXI_DECODE_DIN_POWER_DELIVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PowerDeliveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PowerDeliveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_POWER_DELIVERY_RES) && (EXI_DECODE_DIN_POWER_DELIVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PowerDeliveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_POWER_DELIVERY_RES) && (EXI_DECODE_DIN_POWER_DELIVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PreChargeReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PreChargeReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PRE_CHARGE_REQ) && (EXI_DECODE_DIN_PRE_CHARGE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PreChargeReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PRE_CHARGE_REQ) && (EXI_DECODE_DIN_PRE_CHARGE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_PreChargeRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_PreChargeResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PRE_CHARGE_RES) && (EXI_DECODE_DIN_PRE_CHARGE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_PreChargeRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PRE_CHARGE_RES) && (EXI_DECODE_DIN_PRE_CHARGE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ProfileEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ProfileEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PROFILE_ENTRY) && (EXI_DECODE_DIN_PROFILE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ProfileEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PROFILE_ENTRY) && (EXI_DECODE_DIN_PROFILE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_RelativeTimeInterval
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_RelativeTimeIntervalType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL) && (EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_RelativeTimeInterval( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL) && (EXI_DECODE_DIN_RELATIVE_TIME_INTERVAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SAScheduleList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SAScheduleListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SASCHEDULE_LIST) && (EXI_DECODE_DIN_SASCHEDULE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SAScheduleList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SASCHEDULE_LIST) && (EXI_DECODE_DIN_SASCHEDULE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SAScheduleTuple
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SAScheduleTupleType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SASCHEDULE_TUPLE) && (EXI_DECODE_DIN_SASCHEDULE_TUPLE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SAScheduleTuple( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SASCHEDULE_TUPLE) && (EXI_DECODE_DIN_SASCHEDULE_TUPLE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SalesTariffEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SalesTariffEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SALES_TARIFF_ENTRY) && (EXI_DECODE_DIN_SALES_TARIFF_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SalesTariffEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SALES_TARIFF_ENTRY) && (EXI_DECODE_DIN_SALES_TARIFF_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SalesTariff
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SalesTariffType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SALES_TARIFF) && (EXI_DECODE_DIN_SALES_TARIFF == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SalesTariff( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SALES_TARIFF) && (EXI_DECODE_DIN_SALES_TARIFF == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SelectedServiceList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SelectedServiceListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SELECTED_SERVICE_LIST) && (EXI_DECODE_DIN_SELECTED_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SelectedServiceList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SELECTED_SERVICE_LIST) && (EXI_DECODE_DIN_SELECTED_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SelectedService
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SelectedServiceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SELECTED_SERVICE) && (EXI_DECODE_DIN_SELECTED_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SelectedService( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SELECTED_SERVICE) && (EXI_DECODE_DIN_SELECTED_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceCharge
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceChargeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_CHARGE) && (EXI_DECODE_DIN_SERVICE_CHARGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceCharge( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_CHARGE) && (EXI_DECODE_DIN_SERVICE_CHARGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceDetailReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceDetailReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_DETAIL_REQ) && (EXI_DECODE_DIN_SERVICE_DETAIL_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceDetailReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_DETAIL_REQ) && (EXI_DECODE_DIN_SERVICE_DETAIL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceDetailRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceDetailResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_DETAIL_RES) && (EXI_DECODE_DIN_SERVICE_DETAIL_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceDetailRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_DETAIL_RES) && (EXI_DECODE_DIN_SERVICE_DETAIL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceDiscoveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ) && (EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceDiscoveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ) && (EXI_DECODE_DIN_SERVICE_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceDiscoveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_DISCOVERY_RES) && (EXI_DECODE_DIN_SERVICE_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceDiscoveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_DISCOVERY_RES) && (EXI_DECODE_DIN_SERVICE_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceParameterList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceParameterListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_PARAMETER_LIST) && (EXI_DECODE_DIN_SERVICE_PARAMETER_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceParameterList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_PARAMETER_LIST) && (EXI_DECODE_DIN_SERVICE_PARAMETER_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServicePaymentSelectionReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServicePaymentSelectionReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ) && (EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServicePaymentSelectionReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ) && (EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServicePaymentSelectionRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServicePaymentSelectionResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES) && (EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServicePaymentSelectionRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES) && (EXI_DECODE_DIN_SERVICE_PAYMENT_SELECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceTagList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceTagListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_TAG_LIST) && (EXI_DECODE_DIN_SERVICE_TAG_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceTagList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_TAG_LIST) && (EXI_DECODE_DIN_SERVICE_TAG_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_ServiceTag
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceTagType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_TAG) && (EXI_DECODE_DIN_SERVICE_TAG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_ServiceTag( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_TAG) && (EXI_DECODE_DIN_SERVICE_TAG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_Service
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_ServiceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE) && (EXI_DECODE_DIN_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_Service( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE) && (EXI_DECODE_DIN_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SessionSetupReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SessionSetupReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SESSION_SETUP_REQ) && (EXI_DECODE_DIN_SESSION_SETUP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SessionSetupReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SESSION_SETUP_REQ) && (EXI_DECODE_DIN_SESSION_SETUP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SessionSetupRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SessionSetupResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SESSION_SETUP_RES) && (EXI_DECODE_DIN_SESSION_SETUP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SessionSetupRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SESSION_SETUP_RES) && (EXI_DECODE_DIN_SESSION_SETUP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SessionStopRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SessionStopResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SESSION_STOP_RES) && (EXI_DECODE_DIN_SESSION_STOP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SessionStopRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SESSION_STOP_RES) && (EXI_DECODE_DIN_SESSION_STOP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_SubCertificates
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_SubCertificatesType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SUB_CERTIFICATES) && (EXI_DECODE_DIN_SUB_CERTIFICATES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_SubCertificates( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SUB_CERTIFICATES) && (EXI_DECODE_DIN_SUB_CERTIFICATES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_V2G_Message
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_V2G_MessageType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_V2G_MESSAGE) && (EXI_DECODE_DIN_V2G_MESSAGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_V2G_Message( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_V2G_MESSAGE) && (EXI_DECODE_DIN_V2G_MESSAGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_WeldingDetectionReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_WeldingDetectionReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_WELDING_DETECTION_REQ) && (EXI_DECODE_DIN_WELDING_DETECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_WeldingDetectionReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_WELDING_DETECTION_REQ) && (EXI_DECODE_DIN_WELDING_DETECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_WeldingDetectionRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_WeldingDetectionResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_WELDING_DETECTION_RES) && (EXI_DECODE_DIN_WELDING_DETECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_WeldingDetectionRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_WELDING_DETECTION_RES) && (EXI_DECODE_DIN_WELDING_DETECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_certificate
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_certificateType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CERTIFICATE) && (EXI_DECODE_DIN_CERTIFICATE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_certificate( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CERTIFICATE) && (EXI_DECODE_DIN_CERTIFICATE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_contractID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_contractIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_CONTRACT_ID) && (EXI_DECODE_DIN_CONTRACT_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_contractID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_CONTRACT_ID) && (EXI_DECODE_DIN_CONTRACT_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_costKind
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_costKindType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_COST_KIND) && (EXI_DECODE_DIN_COST_KIND == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_costKind( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_COST_KIND) && (EXI_DECODE_DIN_COST_KIND == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_dHParams
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_dHParamsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_D_HPARAMS) && (EXI_DECODE_DIN_D_HPARAMS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_dHParams( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_D_HPARAMS) && (EXI_DECODE_DIN_D_HPARAMS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_evccID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_evccIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVCC_ID) && (EXI_DECODE_DIN_EVCC_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_evccID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVCC_ID) && (EXI_DECODE_DIN_EVCC_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_evseID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_evseIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_EVSE_ID) && (EXI_DECODE_DIN_EVSE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_evseID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_EVSE_ID) && (EXI_DECODE_DIN_EVSE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_faultCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_faultCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_FAULT_CODE) && (EXI_DECODE_DIN_FAULT_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_faultCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_FAULT_CODE) && (EXI_DECODE_DIN_FAULT_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_faultMsg
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_faultMsgType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_FAULT_MSG) && (EXI_DECODE_DIN_FAULT_MSG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_faultMsg( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_FAULT_MSG) && (EXI_DECODE_DIN_FAULT_MSG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_genChallenge
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_genChallengeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_GEN_CHALLENGE) && (EXI_DECODE_DIN_GEN_CHALLENGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_genChallenge( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_GEN_CHALLENGE) && (EXI_DECODE_DIN_GEN_CHALLENGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_isolationLevel
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_isolationLevelType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ISOLATION_LEVEL) && (EXI_DECODE_DIN_ISOLATION_LEVEL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_isolationLevel( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ISOLATION_LEVEL) && (EXI_DECODE_DIN_ISOLATION_LEVEL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_meterID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_meterIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_METER_ID) && (EXI_DECODE_DIN_METER_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_meterID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_METER_ID) && (EXI_DECODE_DIN_METER_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_paymentOption
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_paymentOptionType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PAYMENT_OPTION) && (EXI_DECODE_DIN_PAYMENT_OPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_paymentOption( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PAYMENT_OPTION) && (EXI_DECODE_DIN_PAYMENT_OPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_privateKey
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_privateKeyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_PRIVATE_KEY) && (EXI_DECODE_DIN_PRIVATE_KEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_privateKey( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_PRIVATE_KEY) && (EXI_DECODE_DIN_PRIVATE_KEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_responseCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_responseCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_RESPONSE_CODE) && (EXI_DECODE_DIN_RESPONSE_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_responseCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_RESPONSE_CODE) && (EXI_DECODE_DIN_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_rootCertificateID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_rootCertificateIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_ROOT_CERTIFICATE_ID) && (EXI_DECODE_DIN_ROOT_CERTIFICATE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_rootCertificateID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_ROOT_CERTIFICATE_ID) && (EXI_DECODE_DIN_ROOT_CERTIFICATE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_serviceCategory
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_serviceCategoryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_CATEGORY) && (EXI_DECODE_DIN_SERVICE_CATEGORY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_serviceCategory( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_CATEGORY) && (EXI_DECODE_DIN_SERVICE_CATEGORY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_serviceName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_serviceNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_NAME) && (EXI_DECODE_DIN_SERVICE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_serviceName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_NAME) && (EXI_DECODE_DIN_SERVICE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_serviceScope
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_serviceScopeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SERVICE_SCOPE) && (EXI_DECODE_DIN_SERVICE_SCOPE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_serviceScope( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SERVICE_SCOPE) && (EXI_DECODE_DIN_SERVICE_SCOPE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_sessionID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_sessionIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SESSION_ID) && (EXI_DECODE_DIN_SESSION_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_sessionID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SESSION_ID) && (EXI_DECODE_DIN_SESSION_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_sigMeterReading
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_sigMeterReadingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_SIG_METER_READING) && (EXI_DECODE_DIN_SIG_METER_READING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_sigMeterReading( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_SIG_METER_READING) && (EXI_DECODE_DIN_SIG_METER_READING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_tariffDescription
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_tariffDescriptionType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_TARIFF_DESCRIPTION) && (EXI_DECODE_DIN_TARIFF_DESCRIPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_tariffDescription( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_TARIFF_DESCRIPTION) && (EXI_DECODE_DIN_TARIFF_DESCRIPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_DIN_unitSymbol
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_DIN_unitSymbolType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_DIN_UNIT_SYMBOL) && (EXI_DECODE_DIN_UNIT_SYMBOL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_DIN_unitSymbol( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_DIN_UNIT_SYMBOL) && (EXI_DECODE_DIN_UNIT_SYMBOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_DIN
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_DIN object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_DIN) && (EXI_DECODE_SCHEMA_SET_DIN == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_DIN( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SCHEMA_SET_DIN) && (EXI_DECODE_SCHEMA_SET_DIN == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined (EXI_ENABLE_DECODE_DIN_MESSAGE_SET) && (EXI_ENABLE_DECODE_DIN_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_DIN_SCHEMA_DECODER_H */
