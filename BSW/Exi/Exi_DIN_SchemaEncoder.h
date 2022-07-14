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
 *         File:  Exi_DIN_SchemaEncoder.h
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


#if !defined (EXI_DIN_SCHEMA_ENCODER_H)
# define EXI_DIN_SCHEMA_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaEncoder.h"

#if (!defined (EXI_ENABLE_ENCODE_DIN_MESSAGE_SET))
# if (defined (EXI_ENABLE_DIN_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_DIN_MESSAGE_SET   EXI_ENABLE_DIN_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_DIN_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_ENCODE_DIN_MESSAGE_SET) && (EXI_ENABLE_ENCODE_DIN_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_ENCODE_DIN_AC_EVCHARGE_PARAMETER 0x32U
#define EXI_API_ID_ENCODE_DIN_AC_EVSECHARGE_PARAMETER 0x33U
#define EXI_API_ID_ENCODE_DIN_AC_EVSESTATUS 0x34U
#define EXI_API_ID_ENCODE_DIN_ATTRIBUTE_ID 0x35U
#define EXI_API_ID_ENCODE_DIN_ATTRIBUTE_NAME 0x36U
#define EXI_API_ID_ENCODE_DIN_ATTRIBUTE_VALUE 0x37U
#define EXI_API_ID_ENCODE_DIN_BODY 0x38U
#define EXI_API_ID_ENCODE_DIN_CABLE_CHECK_REQ 0x39U
#define EXI_API_ID_ENCODE_DIN_CABLE_CHECK_RES 0x3AU
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE_CHAIN 0x3BU
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ 0x3CU
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES 0x3DU
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE_UPDATE_REQ 0x3EU
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE_UPDATE_RES 0x3FU
#define EXI_API_ID_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ 0x40U
#define EXI_API_ID_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES 0x41U
#define EXI_API_ID_ENCODE_DIN_CHARGING_PROFILE 0x42U
#define EXI_API_ID_ENCODE_DIN_CHARGING_STATUS_RES 0x43U
#define EXI_API_ID_ENCODE_DIN_CONSUMPTION_COST 0x44U
#define EXI_API_ID_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ 0x45U
#define EXI_API_ID_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES 0x46U
#define EXI_API_ID_ENCODE_DIN_COST 0x47U
#define EXI_API_ID_ENCODE_DIN_CURRENT_DEMAND_REQ 0x48U
#define EXI_API_ID_ENCODE_DIN_CURRENT_DEMAND_RES 0x49U
#define EXI_API_ID_ENCODE_DIN_DC_EVCHARGE_PARAMETER 0x4AU
#define EXI_API_ID_ENCODE_DIN_DC_EVERROR_CODE 0x4BU
#define EXI_API_ID_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER 0x4CU
#define EXI_API_ID_ENCODE_DIN_DC_EVSECHARGE_PARAMETER 0x4DU
#define EXI_API_ID_ENCODE_DIN_DC_EVSESTATUS_CODE 0x4EU
#define EXI_API_ID_ENCODE_DIN_DC_EVSESTATUS 0x4FU
#define EXI_API_ID_ENCODE_DIN_DC_EVSTATUS 0x50U
#define EXI_API_ID_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER 0x51U
#define EXI_API_ID_ENCODE_DIN_EVSENOTIFICATION 0x52U
#define EXI_API_ID_ENCODE_DIN_EVSEPROCESSING 0x53U
#define EXI_API_ID_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER 0x54U
#define EXI_API_ID_ENCODE_DIN_ENTRY 0x55U
#define EXI_API_ID_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS 0x56U
#define EXI_API_ID_ENCODE_DIN_MESSAGE_HEADER 0x57U
#define EXI_API_ID_ENCODE_DIN_METER_INFO 0x58U
#define EXI_API_ID_ENCODE_DIN_METERING_RECEIPT_REQ 0x59U
#define EXI_API_ID_ENCODE_DIN_METERING_RECEIPT_RES 0x5AU
#define EXI_API_ID_ENCODE_DIN_NOTIFICATION 0x5BU
#define EXI_API_ID_ENCODE_DIN_PMAX_SCHEDULE_ENTRY 0x5CU
#define EXI_API_ID_ENCODE_DIN_PMAX_SCHEDULE 0x5DU
#define EXI_API_ID_ENCODE_DIN_PARAMETER_SET 0x5EU
#define EXI_API_ID_ENCODE_DIN_PARAMETER 0x5FU
#define EXI_API_ID_ENCODE_DIN_PAYMENT_DETAILS_REQ 0x60U
#define EXI_API_ID_ENCODE_DIN_PAYMENT_DETAILS_RES 0x61U
#define EXI_API_ID_ENCODE_DIN_PAYMENT_OPTIONS 0x62U
#define EXI_API_ID_ENCODE_DIN_PHYSICAL_VALUE 0x63U
#define EXI_API_ID_ENCODE_DIN_POWER_DELIVERY_REQ 0x64U
#define EXI_API_ID_ENCODE_DIN_POWER_DELIVERY_RES 0x65U
#define EXI_API_ID_ENCODE_DIN_PRE_CHARGE_REQ 0x66U
#define EXI_API_ID_ENCODE_DIN_PRE_CHARGE_RES 0x67U
#define EXI_API_ID_ENCODE_DIN_PROFILE_ENTRY 0x68U
#define EXI_API_ID_ENCODE_DIN_RELATIVE_TIME_INTERVAL 0x69U
#define EXI_API_ID_ENCODE_DIN_SASCHEDULE_LIST 0x6AU
#define EXI_API_ID_ENCODE_DIN_SASCHEDULE_TUPLE 0x6BU
#define EXI_API_ID_ENCODE_DIN_SALES_TARIFF_ENTRY 0x6CU
#define EXI_API_ID_ENCODE_DIN_SALES_TARIFF 0x6DU
#define EXI_API_ID_ENCODE_DIN_SELECTED_SERVICE_LIST 0x6EU
#define EXI_API_ID_ENCODE_DIN_SELECTED_SERVICE 0x6FU
#define EXI_API_ID_ENCODE_DIN_SERVICE_CHARGE 0x70U
#define EXI_API_ID_ENCODE_DIN_SERVICE_DETAIL_REQ 0x71U
#define EXI_API_ID_ENCODE_DIN_SERVICE_DETAIL_RES 0x72U
#define EXI_API_ID_ENCODE_DIN_SERVICE_DISCOVERY_REQ 0x73U
#define EXI_API_ID_ENCODE_DIN_SERVICE_DISCOVERY_RES 0x74U
#define EXI_API_ID_ENCODE_DIN_SERVICE_PARAMETER_LIST 0x75U
#define EXI_API_ID_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ 0x76U
#define EXI_API_ID_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES 0x77U
#define EXI_API_ID_ENCODE_DIN_SERVICE_TAG_LIST 0x78U
#define EXI_API_ID_ENCODE_DIN_SERVICE_TAG 0x79U
#define EXI_API_ID_ENCODE_DIN_SERVICE 0x7AU
#define EXI_API_ID_ENCODE_DIN_SESSION_SETUP_REQ 0x7BU
#define EXI_API_ID_ENCODE_DIN_SESSION_SETUP_RES 0x7CU
#define EXI_API_ID_ENCODE_DIN_SESSION_STOP_RES 0x7DU
#define EXI_API_ID_ENCODE_DIN_SUB_CERTIFICATES 0x7EU
#define EXI_API_ID_ENCODE_DIN_V2G_MESSAGE 0x7FU
#define EXI_API_ID_ENCODE_DIN_WELDING_DETECTION_REQ 0x80U
#define EXI_API_ID_ENCODE_DIN_WELDING_DETECTION_RES 0x81U
#define EXI_API_ID_ENCODE_DIN_CERTIFICATE 0x82U
#define EXI_API_ID_ENCODE_DIN_CONTRACT_ID 0x83U
#define EXI_API_ID_ENCODE_DIN_COST_KIND 0x84U
#define EXI_API_ID_ENCODE_DIN_D_HPARAMS 0x85U
#define EXI_API_ID_ENCODE_DIN_EVCC_ID 0x86U
#define EXI_API_ID_ENCODE_DIN_EVSE_ID 0x87U
#define EXI_API_ID_ENCODE_DIN_FAULT_CODE 0x88U
#define EXI_API_ID_ENCODE_DIN_FAULT_MSG 0x89U
#define EXI_API_ID_ENCODE_DIN_GEN_CHALLENGE 0x8AU
#define EXI_API_ID_ENCODE_DIN_ISOLATION_LEVEL 0x8BU
#define EXI_API_ID_ENCODE_DIN_METER_ID 0x8CU
#define EXI_API_ID_ENCODE_DIN_PAYMENT_OPTION 0x8DU
#define EXI_API_ID_ENCODE_DIN_PRIVATE_KEY 0x8EU
#define EXI_API_ID_ENCODE_DIN_RESPONSE_CODE 0x8FU
#define EXI_API_ID_ENCODE_DIN_ROOT_CERTIFICATE_ID 0x90U
#define EXI_API_ID_ENCODE_DIN_SERVICE_CATEGORY 0x91U
#define EXI_API_ID_ENCODE_DIN_SERVICE_NAME 0x92U
#define EXI_API_ID_ENCODE_DIN_SERVICE_SCOPE 0x93U
#define EXI_API_ID_ENCODE_DIN_SESSION_ID 0x94U
#define EXI_API_ID_ENCODE_DIN_SIG_METER_READING 0x95U
#define EXI_API_ID_ENCODE_DIN_TARIFF_DESCRIPTION 0x96U
#define EXI_API_ID_ENCODE_DIN_UNIT_SYMBOL 0x97U
#define EXI_API_ID_ENCODE_DIN_SCHEMA_FRAGMENT 0x17FU
#define EXI_API_ID_ENCODE_DIN_SCHEMA_ROOT 0x180U
#endif

/* Encoding default switches */
#ifndef EXI_ENCODE_DIN_SCHEMA_FRAGMENT
#define EXI_ENCODE_DIN_SCHEMA_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SCHEMA_ROOT
#define EXI_ENCODE_DIN_SCHEMA_ROOT STD_ON
#endif
#ifndef EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER
#define EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER
#define EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_AC_EVSESTATUS
#define EXI_ENCODE_DIN_AC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ATTRIBUTE_ID
#define EXI_ENCODE_DIN_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ATTRIBUTE_NAME
#define EXI_ENCODE_DIN_ATTRIBUTE_NAME STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ATTRIBUTE_VALUE
#define EXI_ENCODE_DIN_ATTRIBUTE_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_BODY_BASE
#define EXI_ENCODE_DIN_BODY_BASE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_BODY
#define EXI_ENCODE_DIN_BODY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CABLE_CHECK_REQ
#define EXI_ENCODE_DIN_CABLE_CHECK_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CABLE_CHECK_RES
#define EXI_ENCODE_DIN_CABLE_CHECK_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE_CHAIN
#define EXI_ENCODE_DIN_CERTIFICATE_CHAIN STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ
#define EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES
#define EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ
#define EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES
#define EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ
#define EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES
#define EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CHARGING_PROFILE
#define EXI_ENCODE_DIN_CHARGING_PROFILE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CHARGING_STATUS_REQ
#define EXI_ENCODE_DIN_CHARGING_STATUS_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CHARGING_STATUS_RES
#define EXI_ENCODE_DIN_CHARGING_STATUS_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CONSUMPTION_COST
#define EXI_ENCODE_DIN_CONSUMPTION_COST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ
#define EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES
#define EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_COST
#define EXI_ENCODE_DIN_COST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CURRENT_DEMAND_REQ
#define EXI_ENCODE_DIN_CURRENT_DEMAND_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CURRENT_DEMAND_RES
#define EXI_ENCODE_DIN_CURRENT_DEMAND_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER
#define EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVERROR_CODE
#define EXI_ENCODE_DIN_DC_EVERROR_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER
#define EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER
#define EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVSESTATUS_CODE
#define EXI_ENCODE_DIN_DC_EVSESTATUS_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVSESTATUS
#define EXI_ENCODE_DIN_DC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_DC_EVSTATUS
#define EXI_ENCODE_DIN_DC_EVSTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVCHARGE_PARAMETER
#define EXI_ENCODE_DIN_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVPOWER_DELIVERY_PARAMETER
#define EXI_ENCODE_DIN_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER
#define EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSECHARGE_PARAMETER
#define EXI_ENCODE_DIN_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSENOTIFICATION
#define EXI_ENCODE_DIN_EVSENOTIFICATION STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSEPROCESSING
#define EXI_ENCODE_DIN_EVSEPROCESSING STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSESTATUS
#define EXI_ENCODE_DIN_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER
#define EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSTATUS
#define EXI_ENCODE_DIN_EVSTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ENTRY
#define EXI_ENCODE_DIN_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_INTERVAL
#define EXI_ENCODE_DIN_INTERVAL STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS
#define EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_MESSAGE_HEADER
#define EXI_ENCODE_DIN_MESSAGE_HEADER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_METER_INFO
#define EXI_ENCODE_DIN_METER_INFO STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_METERING_RECEIPT_REQ
#define EXI_ENCODE_DIN_METERING_RECEIPT_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_METERING_RECEIPT_RES
#define EXI_ENCODE_DIN_METERING_RECEIPT_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_NOTIFICATION
#define EXI_ENCODE_DIN_NOTIFICATION STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY
#define EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PMAX_SCHEDULE
#define EXI_ENCODE_DIN_PMAX_SCHEDULE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PARAMETER_SET
#define EXI_ENCODE_DIN_PARAMETER_SET STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PARAMETER
#define EXI_ENCODE_DIN_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ
#define EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PAYMENT_DETAILS_RES
#define EXI_ENCODE_DIN_PAYMENT_DETAILS_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PAYMENT_OPTIONS
#define EXI_ENCODE_DIN_PAYMENT_OPTIONS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PHYSICAL_VALUE
#define EXI_ENCODE_DIN_PHYSICAL_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_POWER_DELIVERY_REQ
#define EXI_ENCODE_DIN_POWER_DELIVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_POWER_DELIVERY_RES
#define EXI_ENCODE_DIN_POWER_DELIVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PRE_CHARGE_REQ
#define EXI_ENCODE_DIN_PRE_CHARGE_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PRE_CHARGE_RES
#define EXI_ENCODE_DIN_PRE_CHARGE_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PROFILE_ENTRY
#define EXI_ENCODE_DIN_PROFILE_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL
#define EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SASCHEDULE_LIST
#define EXI_ENCODE_DIN_SASCHEDULE_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SASCHEDULE_TUPLE
#define EXI_ENCODE_DIN_SASCHEDULE_TUPLE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SASCHEDULES
#define EXI_ENCODE_DIN_SASCHEDULES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SALES_TARIFF_ENTRY
#define EXI_ENCODE_DIN_SALES_TARIFF_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SALES_TARIFF
#define EXI_ENCODE_DIN_SALES_TARIFF STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SELECTED_SERVICE_LIST
#define EXI_ENCODE_DIN_SELECTED_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SELECTED_SERVICE
#define EXI_ENCODE_DIN_SELECTED_SERVICE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_CHARGE
#define EXI_ENCODE_DIN_SERVICE_CHARGE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_DETAIL_REQ
#define EXI_ENCODE_DIN_SERVICE_DETAIL_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_DETAIL_RES
#define EXI_ENCODE_DIN_SERVICE_DETAIL_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ
#define EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES
#define EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST
#define EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ
#define EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES
#define EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_TAG_LIST
#define EXI_ENCODE_DIN_SERVICE_TAG_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_TAG
#define EXI_ENCODE_DIN_SERVICE_TAG STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE
#define EXI_ENCODE_DIN_SERVICE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SESSION_SETUP_REQ
#define EXI_ENCODE_DIN_SESSION_SETUP_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SESSION_SETUP_RES
#define EXI_ENCODE_DIN_SESSION_SETUP_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SESSION_STOP_REQ
#define EXI_ENCODE_DIN_SESSION_STOP_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SESSION_STOP_RES
#define EXI_ENCODE_DIN_SESSION_STOP_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SUB_CERTIFICATES
#define EXI_ENCODE_DIN_SUB_CERTIFICATES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_V2G_MESSAGE
#define EXI_ENCODE_DIN_V2G_MESSAGE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_WELDING_DETECTION_REQ
#define EXI_ENCODE_DIN_WELDING_DETECTION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_WELDING_DETECTION_RES
#define EXI_ENCODE_DIN_WELDING_DETECTION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CERTIFICATE
#define EXI_ENCODE_DIN_CERTIFICATE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_CONTRACT_ID
#define EXI_ENCODE_DIN_CONTRACT_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_COST_KIND
#define EXI_ENCODE_DIN_COST_KIND STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_D_HPARAMS
#define EXI_ENCODE_DIN_D_HPARAMS STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVCC_ID
#define EXI_ENCODE_DIN_EVCC_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_EVSE_ID
#define EXI_ENCODE_DIN_EVSE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_FAULT_CODE
#define EXI_ENCODE_DIN_FAULT_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_FAULT_MSG
#define EXI_ENCODE_DIN_FAULT_MSG STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_GEN_CHALLENGE
#define EXI_ENCODE_DIN_GEN_CHALLENGE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ISOLATION_LEVEL
#define EXI_ENCODE_DIN_ISOLATION_LEVEL STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_METER_ID
#define EXI_ENCODE_DIN_METER_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PAYMENT_OPTION
#define EXI_ENCODE_DIN_PAYMENT_OPTION STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_PRIVATE_KEY
#define EXI_ENCODE_DIN_PRIVATE_KEY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_RESPONSE_CODE
#define EXI_ENCODE_DIN_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID
#define EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_CATEGORY
#define EXI_ENCODE_DIN_SERVICE_CATEGORY STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_NAME
#define EXI_ENCODE_DIN_SERVICE_NAME STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SERVICE_SCOPE
#define EXI_ENCODE_DIN_SERVICE_SCOPE STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SESSION_ID
#define EXI_ENCODE_DIN_SESSION_ID STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_SIG_METER_READING
#define EXI_ENCODE_DIN_SIG_METER_READING STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_TARIFF_DESCRIPTION
#define EXI_ENCODE_DIN_TARIFF_DESCRIPTION STD_OFF
#endif
#ifndef EXI_ENCODE_DIN_UNIT_SYMBOL
#define EXI_ENCODE_DIN_UNIT_SYMBOL STD_OFF
#endif


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Encode_DIN_AC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AC_EVChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVChargeParameterPtr     pointer to Exi_DIN_AC_EVChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_DIN_AC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_AC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AC_EVSEChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVSEChargeParameterPtr   pointer to Exi_DIN_AC_EVSEChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_DIN_AC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_AC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AC_EVSEStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVSEStatusPtr            pointer to Exi_DIN_AC_EVSEStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_AC_EVSESTATUS) && (EXI_ENCODE_DIN_AC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEStatusPtr );
#endif /* (defined(EXI_ENCODE_DIN_AC_EVSESTATUS) && (EXI_ENCODE_DIN_AC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_AttributeId
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AttributeIdType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeIdPtr              pointer to Exi_DIN_AttributeIdType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ATTRIBUTE_ID) && (EXI_ENCODE_DIN_ATTRIBUTE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AttributeId( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AttributeIdType, AUTOMATIC, EXI_APPL_DATA) AttributeIdPtr );
#endif /* (defined(EXI_ENCODE_DIN_ATTRIBUTE_ID) && (EXI_ENCODE_DIN_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_AttributeName
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AttributeNameType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeNamePtr            pointer to Exi_DIN_AttributeNameType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ATTRIBUTE_NAME) && (EXI_ENCODE_DIN_ATTRIBUTE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AttributeName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AttributeNameType, AUTOMATIC, EXI_APPL_DATA) AttributeNamePtr );
#endif /* (defined(EXI_ENCODE_DIN_ATTRIBUTE_NAME) && (EXI_ENCODE_DIN_ATTRIBUTE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_AttributeValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_AttributeValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeValuePtr           pointer to Exi_DIN_AttributeValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ATTRIBUTE_VALUE) && (EXI_ENCODE_DIN_ATTRIBUTE_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_AttributeValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_AttributeValueType, AUTOMATIC, EXI_APPL_DATA) AttributeValuePtr );
#endif /* (defined(EXI_ENCODE_DIN_ATTRIBUTE_VALUE) && (EXI_ENCODE_DIN_ATTRIBUTE_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Body
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_BodyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     BodyPtr                     pointer to Exi_DIN_BodyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_BODY) && (EXI_ENCODE_DIN_BODY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Body( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_BodyType, AUTOMATIC, EXI_APPL_DATA) BodyPtr );
#endif /* (defined(EXI_ENCODE_DIN_BODY) && (EXI_ENCODE_DIN_BODY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CableCheckReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CableCheckReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CableCheckReqPtr            pointer to Exi_DIN_CableCheckReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CABLE_CHECK_REQ) && (EXI_ENCODE_DIN_CABLE_CHECK_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CableCheckReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CableCheckReqType, AUTOMATIC, EXI_APPL_DATA) CableCheckReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CABLE_CHECK_REQ) && (EXI_ENCODE_DIN_CABLE_CHECK_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CableCheckRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CableCheckResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CableCheckResPtr            pointer to Exi_DIN_CableCheckResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CABLE_CHECK_RES) && (EXI_ENCODE_DIN_CABLE_CHECK_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CableCheckRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CableCheckResType, AUTOMATIC, EXI_APPL_DATA) CableCheckResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CABLE_CHECK_RES) && (EXI_ENCODE_DIN_CABLE_CHECK_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CertificateChain
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CertificateChainType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateChainPtr         pointer to Exi_DIN_CertificateChainType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE_CHAIN) && (EXI_ENCODE_DIN_CERTIFICATE_CHAIN == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CertificateChain( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CertificateChainType, AUTOMATIC, EXI_APPL_DATA) CertificateChainPtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE_CHAIN) && (EXI_ENCODE_DIN_CERTIFICATE_CHAIN == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CertificateInstallationReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CertificateInstallationReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateInstallationReqPtrpointer to Exi_DIN_CertificateInstallationReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CertificateInstallationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CertificateInstallationReqType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CertificateInstallationRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CertificateInstallationResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateInstallationResPtrpointer to Exi_DIN_CertificateInstallationResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CertificateInstallationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CertificateInstallationResType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_DIN_CERTIFICATE_INSTALLATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CertificateUpdateReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CertificateUpdateReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateUpdateReqPtr     pointer to Exi_DIN_CertificateUpdateReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CertificateUpdateReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CertificateUpdateReqType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_DIN_CERTIFICATE_UPDATE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CertificateUpdateRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CertificateUpdateResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateUpdateResPtr     pointer to Exi_DIN_CertificateUpdateResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CertificateUpdateRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CertificateUpdateResType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_DIN_CERTIFICATE_UPDATE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ChargeParameterDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ChargeParameterDiscoveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargeParameterDiscoveryReqPtrpointer to Exi_DIN_ChargeParameterDiscoveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ChargeParameterDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ChargeParameterDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ChargeParameterDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ChargeParameterDiscoveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargeParameterDiscoveryResPtrpointer to Exi_DIN_ChargeParameterDiscoveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ChargeParameterDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ChargeParameterDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_DIN_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ChargingProfile
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ChargingProfileType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargingProfilePtr          pointer to Exi_DIN_ChargingProfileType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CHARGING_PROFILE) && (EXI_ENCODE_DIN_CHARGING_PROFILE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ChargingProfile( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ChargingProfileType, AUTOMATIC, EXI_APPL_DATA) ChargingProfilePtr );
#endif /* (defined(EXI_ENCODE_DIN_CHARGING_PROFILE) && (EXI_ENCODE_DIN_CHARGING_PROFILE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ChargingStatusRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ChargingStatusResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargingStatusResPtr        pointer to Exi_DIN_ChargingStatusResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CHARGING_STATUS_RES) && (EXI_ENCODE_DIN_CHARGING_STATUS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ChargingStatusRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ChargingStatusResType, AUTOMATIC, EXI_APPL_DATA) ChargingStatusResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CHARGING_STATUS_RES) && (EXI_ENCODE_DIN_CHARGING_STATUS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ConsumptionCost
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ConsumptionCostType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ConsumptionCostPtr          pointer to Exi_DIN_ConsumptionCostType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CONSUMPTION_COST) && (EXI_ENCODE_DIN_CONSUMPTION_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ConsumptionCost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ConsumptionCostType, AUTOMATIC, EXI_APPL_DATA) ConsumptionCostPtr );
#endif /* (defined(EXI_ENCODE_DIN_CONSUMPTION_COST) && (EXI_ENCODE_DIN_CONSUMPTION_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ContractAuthenticationReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ContractAuthenticationReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ContractAuthenticationReqPtrpointer to Exi_DIN_ContractAuthenticationReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ) && (EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ContractAuthenticationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ContractAuthenticationReqType, AUTOMATIC, EXI_APPL_DATA) ContractAuthenticationReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ) && (EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ContractAuthenticationRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ContractAuthenticationResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ContractAuthenticationResPtrpointer to Exi_DIN_ContractAuthenticationResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES) && (EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ContractAuthenticationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ContractAuthenticationResType, AUTOMATIC, EXI_APPL_DATA) ContractAuthenticationResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES) && (EXI_ENCODE_DIN_CONTRACT_AUTHENTICATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Cost
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CostType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CostPtr                     pointer to Exi_DIN_CostType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_COST) && (EXI_ENCODE_DIN_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Cost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CostType, AUTOMATIC, EXI_APPL_DATA) CostPtr );
#endif /* (defined(EXI_ENCODE_DIN_COST) && (EXI_ENCODE_DIN_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CurrentDemandReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CurrentDemandReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CurrentDemandReqPtr         pointer to Exi_DIN_CurrentDemandReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CURRENT_DEMAND_REQ) && (EXI_ENCODE_DIN_CURRENT_DEMAND_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CurrentDemandReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CurrentDemandReqType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_CURRENT_DEMAND_REQ) && (EXI_ENCODE_DIN_CURRENT_DEMAND_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_CurrentDemandRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_CurrentDemandResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CurrentDemandResPtr         pointer to Exi_DIN_CurrentDemandResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CURRENT_DEMAND_RES) && (EXI_ENCODE_DIN_CURRENT_DEMAND_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_CurrentDemandRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_CurrentDemandResType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandResPtr );
#endif /* (defined(EXI_ENCODE_DIN_CURRENT_DEMAND_RES) && (EXI_ENCODE_DIN_CURRENT_DEMAND_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVChargeParameterPtr     pointer to Exi_DIN_DC_EVChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_DIN_DC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVErrorCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVErrorCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVErrorCodePtr           pointer to Exi_DIN_DC_EVErrorCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVERROR_CODE) && (EXI_ENCODE_DIN_DC_EVERROR_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVErrorCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVErrorCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVErrorCodePtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVERROR_CODE) && (EXI_ENCODE_DIN_DC_EVERROR_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVPowerDeliveryParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVPowerDeliveryParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVPowerDeliveryParameterPtrpointer to Exi_DIN_DC_EVPowerDeliveryParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVPowerDeliveryParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVPowerDeliveryParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVPowerDeliveryParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_DIN_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVSEChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEChargeParameterPtr   pointer to Exi_DIN_DC_EVSEChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_DIN_DC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVSEStatusCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVSEStatusCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEStatusCodePtr        pointer to Exi_DIN_DC_EVSEStatusCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVSESTATUS_CODE) && (EXI_ENCODE_DIN_DC_EVSESTATUS_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVSEStatusCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVSEStatusCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusCodePtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVSESTATUS_CODE) && (EXI_ENCODE_DIN_DC_EVSESTATUS_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVSEStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEStatusPtr            pointer to Exi_DIN_DC_EVSEStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVSESTATUS) && (EXI_ENCODE_DIN_DC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusPtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVSESTATUS) && (EXI_ENCODE_DIN_DC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_DC_EVStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_DC_EVStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVStatusPtr              pointer to Exi_DIN_DC_EVStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_DC_EVSTATUS) && (EXI_ENCODE_DIN_DC_EVSTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_DC_EVStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_DC_EVStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVStatusPtr );
#endif /* (defined(EXI_ENCODE_DIN_DC_EVSTATUS) && (EXI_ENCODE_DIN_DC_EVSTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_EVRequestedEnergyTransfer
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_EVRequestedEnergyTransferType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVRequestedEnergyTransferPtrpointer to Exi_DIN_EVRequestedEnergyTransferType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER) && (EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_EVRequestedEnergyTransfer( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_EVRequestedEnergyTransferType, AUTOMATIC, EXI_APPL_DATA) EVRequestedEnergyTransferPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER) && (EXI_ENCODE_DIN_EVREQUESTED_ENERGY_TRANSFER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_EVSENotification
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_EVSENotificationType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSENotificationPtr         pointer to Exi_DIN_EVSENotificationType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVSENOTIFICATION) && (EXI_ENCODE_DIN_EVSENOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_EVSENotification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_EVSENotificationType, AUTOMATIC, EXI_APPL_DATA) EVSENotificationPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVSENOTIFICATION) && (EXI_ENCODE_DIN_EVSENOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_EVSEProcessing
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_EVSEProcessingType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSEProcessingPtr           pointer to Exi_DIN_EVSEProcessingType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVSEPROCESSING) && (EXI_ENCODE_DIN_EVSEPROCESSING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_EVSEProcessing( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_EVSEProcessingType, AUTOMATIC, EXI_APPL_DATA) EVSEProcessingPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVSEPROCESSING) && (EXI_ENCODE_DIN_EVSEPROCESSING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_EVSESupportedEnergyTransfer
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_EVSESupportedEnergyTransferType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSESupportedEnergyTransferPtrpointer to Exi_DIN_EVSESupportedEnergyTransferType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER) && (EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_EVSESupportedEnergyTransfer( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_EVSESupportedEnergyTransferType, AUTOMATIC, EXI_APPL_DATA) EVSESupportedEnergyTransferPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER) && (EXI_ENCODE_DIN_EVSESUPPORTED_ENERGY_TRANSFER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Entry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_EntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EntryPtr                    pointer to Exi_DIN_EntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ENTRY) && (EXI_ENCODE_DIN_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Entry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_EntryType, AUTOMATIC, EXI_APPL_DATA) EntryPtr );
#endif /* (defined(EXI_ENCODE_DIN_ENTRY) && (EXI_ENCODE_DIN_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ListOfRootCertificateIDs
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ListOfRootCertificateIDsType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ListOfRootCertificateIDsPtr pointer to Exi_DIN_ListOfRootCertificateIDsType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ListOfRootCertificateIDs( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ListOfRootCertificateIDsType, AUTOMATIC, EXI_APPL_DATA) ListOfRootCertificateIDsPtr );
#endif /* (defined(EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_DIN_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_MessageHeader
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_MessageHeaderType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MessageHeaderPtr            pointer to Exi_DIN_MessageHeaderType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_MESSAGE_HEADER) && (EXI_ENCODE_DIN_MESSAGE_HEADER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_MessageHeader( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_MessageHeaderType, AUTOMATIC, EXI_APPL_DATA) MessageHeaderPtr );
#endif /* (defined(EXI_ENCODE_DIN_MESSAGE_HEADER) && (EXI_ENCODE_DIN_MESSAGE_HEADER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_MeterInfo
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_MeterInfoType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeterInfoPtr                pointer to Exi_DIN_MeterInfoType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_METER_INFO) && (EXI_ENCODE_DIN_METER_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_MeterInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_MeterInfoType, AUTOMATIC, EXI_APPL_DATA) MeterInfoPtr );
#endif /* (defined(EXI_ENCODE_DIN_METER_INFO) && (EXI_ENCODE_DIN_METER_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_MeteringReceiptReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_MeteringReceiptReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeteringReceiptReqPtr       pointer to Exi_DIN_MeteringReceiptReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_METERING_RECEIPT_REQ) && (EXI_ENCODE_DIN_METERING_RECEIPT_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_MeteringReceiptReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_MeteringReceiptReqType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_METERING_RECEIPT_REQ) && (EXI_ENCODE_DIN_METERING_RECEIPT_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_MeteringReceiptRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_MeteringReceiptResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeteringReceiptResPtr       pointer to Exi_DIN_MeteringReceiptResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_METERING_RECEIPT_RES) && (EXI_ENCODE_DIN_METERING_RECEIPT_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_MeteringReceiptRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_MeteringReceiptResType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptResPtr );
#endif /* (defined(EXI_ENCODE_DIN_METERING_RECEIPT_RES) && (EXI_ENCODE_DIN_METERING_RECEIPT_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Notification
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_NotificationType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     NotificationPtr             pointer to Exi_DIN_NotificationType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_NOTIFICATION) && (EXI_ENCODE_DIN_NOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Notification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_NotificationType, AUTOMATIC, EXI_APPL_DATA) NotificationPtr );
#endif /* (defined(EXI_ENCODE_DIN_NOTIFICATION) && (EXI_ENCODE_DIN_NOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PMaxScheduleEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PMaxScheduleEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PMaxScheduleEntryPtr        pointer to Exi_DIN_PMaxScheduleEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PMaxScheduleEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PMaxScheduleEntryType, AUTOMATIC, EXI_APPL_DATA) PMaxScheduleEntryPtr );
#endif /* (defined(EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_DIN_PMAX_SCHEDULE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PMaxSchedule
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PMaxScheduleType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PMaxSchedulePtr             pointer to Exi_DIN_PMaxScheduleType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PMAX_SCHEDULE) && (EXI_ENCODE_DIN_PMAX_SCHEDULE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PMaxSchedule( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PMaxScheduleType, AUTOMATIC, EXI_APPL_DATA) PMaxSchedulePtr );
#endif /* (defined(EXI_ENCODE_DIN_PMAX_SCHEDULE) && (EXI_ENCODE_DIN_PMAX_SCHEDULE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ParameterSet
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ParameterSetType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ParameterSetPtr             pointer to Exi_DIN_ParameterSetType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PARAMETER_SET) && (EXI_ENCODE_DIN_PARAMETER_SET == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ParameterSet( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ParameterSetType, AUTOMATIC, EXI_APPL_DATA) ParameterSetPtr );
#endif /* (defined(EXI_ENCODE_DIN_PARAMETER_SET) && (EXI_ENCODE_DIN_PARAMETER_SET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Parameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ParameterPtr                pointer to Exi_DIN_ParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PARAMETER) && (EXI_ENCODE_DIN_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Parameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ParameterType, AUTOMATIC, EXI_APPL_DATA) ParameterPtr );
#endif /* (defined(EXI_ENCODE_DIN_PARAMETER) && (EXI_ENCODE_DIN_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PaymentDetailsReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PaymentDetailsReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentDetailsReqPtr        pointer to Exi_DIN_PaymentDetailsReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PaymentDetailsReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PaymentDetailsReqType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_DIN_PAYMENT_DETAILS_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PaymentDetailsRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PaymentDetailsResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentDetailsResPtr        pointer to Exi_DIN_PaymentDetailsResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PAYMENT_DETAILS_RES) && (EXI_ENCODE_DIN_PAYMENT_DETAILS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PaymentDetailsRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PaymentDetailsResType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsResPtr );
#endif /* (defined(EXI_ENCODE_DIN_PAYMENT_DETAILS_RES) && (EXI_ENCODE_DIN_PAYMENT_DETAILS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PaymentOptions
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PaymentOptionsType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentOptionsPtr           pointer to Exi_DIN_PaymentOptionsType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PAYMENT_OPTIONS) && (EXI_ENCODE_DIN_PAYMENT_OPTIONS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PaymentOptions( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PaymentOptionsType, AUTOMATIC, EXI_APPL_DATA) PaymentOptionsPtr );
#endif /* (defined(EXI_ENCODE_DIN_PAYMENT_OPTIONS) && (EXI_ENCODE_DIN_PAYMENT_OPTIONS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PhysicalValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PhysicalValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PhysicalValuePtr            pointer to Exi_DIN_PhysicalValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PHYSICAL_VALUE) && (EXI_ENCODE_DIN_PHYSICAL_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PhysicalValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PhysicalValueType, AUTOMATIC, EXI_APPL_DATA) PhysicalValuePtr );
#endif /* (defined(EXI_ENCODE_DIN_PHYSICAL_VALUE) && (EXI_ENCODE_DIN_PHYSICAL_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PowerDeliveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PowerDeliveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PowerDeliveryReqPtr         pointer to Exi_DIN_PowerDeliveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_POWER_DELIVERY_REQ) && (EXI_ENCODE_DIN_POWER_DELIVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PowerDeliveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PowerDeliveryReqType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_POWER_DELIVERY_REQ) && (EXI_ENCODE_DIN_POWER_DELIVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PowerDeliveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PowerDeliveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PowerDeliveryResPtr         pointer to Exi_DIN_PowerDeliveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_POWER_DELIVERY_RES) && (EXI_ENCODE_DIN_POWER_DELIVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PowerDeliveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PowerDeliveryResType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryResPtr );
#endif /* (defined(EXI_ENCODE_DIN_POWER_DELIVERY_RES) && (EXI_ENCODE_DIN_POWER_DELIVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PreChargeReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PreChargeReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PreChargeReqPtr             pointer to Exi_DIN_PreChargeReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PRE_CHARGE_REQ) && (EXI_ENCODE_DIN_PRE_CHARGE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PreChargeReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PreChargeReqType, AUTOMATIC, EXI_APPL_DATA) PreChargeReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_PRE_CHARGE_REQ) && (EXI_ENCODE_DIN_PRE_CHARGE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_PreChargeRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_PreChargeResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PreChargeResPtr             pointer to Exi_DIN_PreChargeResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PRE_CHARGE_RES) && (EXI_ENCODE_DIN_PRE_CHARGE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_PreChargeRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_PreChargeResType, AUTOMATIC, EXI_APPL_DATA) PreChargeResPtr );
#endif /* (defined(EXI_ENCODE_DIN_PRE_CHARGE_RES) && (EXI_ENCODE_DIN_PRE_CHARGE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ProfileEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ProfileEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ProfileEntryPtr             pointer to Exi_DIN_ProfileEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PROFILE_ENTRY) && (EXI_ENCODE_DIN_PROFILE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ProfileEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ProfileEntryType, AUTOMATIC, EXI_APPL_DATA) ProfileEntryPtr );
#endif /* (defined(EXI_ENCODE_DIN_PROFILE_ENTRY) && (EXI_ENCODE_DIN_PROFILE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_RelativeTimeInterval
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_RelativeTimeIntervalType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     RelativeTimeIntervalPtr     pointer to Exi_DIN_RelativeTimeIntervalType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_RelativeTimeInterval( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_RelativeTimeIntervalType, AUTOMATIC, EXI_APPL_DATA) RelativeTimeIntervalPtr );
#endif /* (defined(EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_DIN_RELATIVE_TIME_INTERVAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SAScheduleList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SAScheduleListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SAScheduleListPtr           pointer to Exi_DIN_SAScheduleListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SASCHEDULE_LIST) && (EXI_ENCODE_DIN_SASCHEDULE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SAScheduleList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SAScheduleListType, AUTOMATIC, EXI_APPL_DATA) SAScheduleListPtr );
#endif /* (defined(EXI_ENCODE_DIN_SASCHEDULE_LIST) && (EXI_ENCODE_DIN_SASCHEDULE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SAScheduleTuple
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SAScheduleTupleType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SAScheduleTuplePtr          pointer to Exi_DIN_SAScheduleTupleType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SASCHEDULE_TUPLE) && (EXI_ENCODE_DIN_SASCHEDULE_TUPLE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SAScheduleTuple( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SAScheduleTupleType, AUTOMATIC, EXI_APPL_DATA) SAScheduleTuplePtr );
#endif /* (defined(EXI_ENCODE_DIN_SASCHEDULE_TUPLE) && (EXI_ENCODE_DIN_SASCHEDULE_TUPLE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SalesTariffEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SalesTariffEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SalesTariffEntryPtr         pointer to Exi_DIN_SalesTariffEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SALES_TARIFF_ENTRY) && (EXI_ENCODE_DIN_SALES_TARIFF_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SalesTariffEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SalesTariffEntryType, AUTOMATIC, EXI_APPL_DATA) SalesTariffEntryPtr );
#endif /* (defined(EXI_ENCODE_DIN_SALES_TARIFF_ENTRY) && (EXI_ENCODE_DIN_SALES_TARIFF_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SalesTariff
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SalesTariffType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SalesTariffPtr              pointer to Exi_DIN_SalesTariffType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SALES_TARIFF) && (EXI_ENCODE_DIN_SALES_TARIFF == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SalesTariff( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SalesTariffType, AUTOMATIC, EXI_APPL_DATA) SalesTariffPtr );
#endif /* (defined(EXI_ENCODE_DIN_SALES_TARIFF) && (EXI_ENCODE_DIN_SALES_TARIFF == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SelectedServiceList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SelectedServiceListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SelectedServiceListPtr      pointer to Exi_DIN_SelectedServiceListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SELECTED_SERVICE_LIST) && (EXI_ENCODE_DIN_SELECTED_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SelectedServiceList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SelectedServiceListType, AUTOMATIC, EXI_APPL_DATA) SelectedServiceListPtr );
#endif /* (defined(EXI_ENCODE_DIN_SELECTED_SERVICE_LIST) && (EXI_ENCODE_DIN_SELECTED_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SelectedService
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SelectedServiceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SelectedServicePtr          pointer to Exi_DIN_SelectedServiceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SELECTED_SERVICE) && (EXI_ENCODE_DIN_SELECTED_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SelectedService( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SelectedServiceType, AUTOMATIC, EXI_APPL_DATA) SelectedServicePtr );
#endif /* (defined(EXI_ENCODE_DIN_SELECTED_SERVICE) && (EXI_ENCODE_DIN_SELECTED_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceCharge
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceChargeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceChargePtr            pointer to Exi_DIN_ServiceChargeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_CHARGE) && (EXI_ENCODE_DIN_SERVICE_CHARGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceCharge( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceChargeType, AUTOMATIC, EXI_APPL_DATA) ServiceChargePtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_CHARGE) && (EXI_ENCODE_DIN_SERVICE_CHARGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceDetailReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceDetailReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDetailReqPtr         pointer to Exi_DIN_ServiceDetailReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_DETAIL_REQ) && (EXI_ENCODE_DIN_SERVICE_DETAIL_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceDetailReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceDetailReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_DETAIL_REQ) && (EXI_ENCODE_DIN_SERVICE_DETAIL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceDetailRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceDetailResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDetailResPtr         pointer to Exi_DIN_ServiceDetailResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_DETAIL_RES) && (EXI_ENCODE_DIN_SERVICE_DETAIL_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceDetailRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceDetailResType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailResPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_DETAIL_RES) && (EXI_ENCODE_DIN_SERVICE_DETAIL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceDiscoveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDiscoveryReqPtr      pointer to Exi_DIN_ServiceDiscoveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_DIN_SERVICE_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceDiscoveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDiscoveryResPtr      pointer to Exi_DIN_ServiceDiscoveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryResPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_DIN_SERVICE_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceParameterList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceParameterListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceParameterListPtr     pointer to Exi_DIN_ServiceParameterListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceParameterList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceParameterListType, AUTOMATIC, EXI_APPL_DATA) ServiceParameterListPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_DIN_SERVICE_PARAMETER_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServicePaymentSelectionReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServicePaymentSelectionReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServicePaymentSelectionReqPtrpointer to Exi_DIN_ServicePaymentSelectionReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ) && (EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServicePaymentSelectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServicePaymentSelectionReqType, AUTOMATIC, EXI_APPL_DATA) ServicePaymentSelectionReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ) && (EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServicePaymentSelectionRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServicePaymentSelectionResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServicePaymentSelectionResPtrpointer to Exi_DIN_ServicePaymentSelectionResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES) && (EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServicePaymentSelectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServicePaymentSelectionResType, AUTOMATIC, EXI_APPL_DATA) ServicePaymentSelectionResPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES) && (EXI_ENCODE_DIN_SERVICE_PAYMENT_SELECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceTagList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceTagListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceTagListPtr           pointer to Exi_DIN_ServiceTagListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_TAG_LIST) && (EXI_ENCODE_DIN_SERVICE_TAG_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceTagList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceTagListType, AUTOMATIC, EXI_APPL_DATA) ServiceTagListPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_TAG_LIST) && (EXI_ENCODE_DIN_SERVICE_TAG_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_ServiceTag
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceTagType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceTagPtr               pointer to Exi_DIN_ServiceTagType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_TAG) && (EXI_ENCODE_DIN_SERVICE_TAG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_ServiceTag( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceTagType, AUTOMATIC, EXI_APPL_DATA) ServiceTagPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_TAG) && (EXI_ENCODE_DIN_SERVICE_TAG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_Service
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_ServiceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServicePtr                  pointer to Exi_DIN_ServiceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE) && (EXI_ENCODE_DIN_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_Service( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_ServiceType, AUTOMATIC, EXI_APPL_DATA) ServicePtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE) && (EXI_ENCODE_DIN_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SessionSetupReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SessionSetupReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionSetupReqPtr          pointer to Exi_DIN_SessionSetupReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SESSION_SETUP_REQ) && (EXI_ENCODE_DIN_SESSION_SETUP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SessionSetupReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SessionSetupReqType, AUTOMATIC, EXI_APPL_DATA) SessionSetupReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_SESSION_SETUP_REQ) && (EXI_ENCODE_DIN_SESSION_SETUP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SessionSetupRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SessionSetupResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionSetupResPtr          pointer to Exi_DIN_SessionSetupResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SESSION_SETUP_RES) && (EXI_ENCODE_DIN_SESSION_SETUP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SessionSetupRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SessionSetupResType, AUTOMATIC, EXI_APPL_DATA) SessionSetupResPtr );
#endif /* (defined(EXI_ENCODE_DIN_SESSION_SETUP_RES) && (EXI_ENCODE_DIN_SESSION_SETUP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SessionStopRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SessionStopResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionStopResPtr           pointer to Exi_DIN_SessionStopResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SESSION_STOP_RES) && (EXI_ENCODE_DIN_SESSION_STOP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SessionStopRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SessionStopResType, AUTOMATIC, EXI_APPL_DATA) SessionStopResPtr );
#endif /* (defined(EXI_ENCODE_DIN_SESSION_STOP_RES) && (EXI_ENCODE_DIN_SESSION_STOP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SubCertificates
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SubCertificatesType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SubCertificatesPtr          pointer to Exi_DIN_SubCertificatesType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SUB_CERTIFICATES) && (EXI_ENCODE_DIN_SUB_CERTIFICATES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SubCertificates( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_SubCertificatesType, AUTOMATIC, EXI_APPL_DATA) SubCertificatesPtr );
#endif /* (defined(EXI_ENCODE_DIN_SUB_CERTIFICATES) && (EXI_ENCODE_DIN_SUB_CERTIFICATES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_V2G_Message
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_V2G_MessageType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     V2G_MessagePtr              pointer to Exi_DIN_V2G_MessageType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_V2G_MESSAGE) && (EXI_ENCODE_DIN_V2G_MESSAGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_V2G_Message( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_V2G_MessageType, AUTOMATIC, EXI_APPL_DATA) V2G_MessagePtr );
#endif /* (defined(EXI_ENCODE_DIN_V2G_MESSAGE) && (EXI_ENCODE_DIN_V2G_MESSAGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_WeldingDetectionReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_WeldingDetectionReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     WeldingDetectionReqPtr      pointer to Exi_DIN_WeldingDetectionReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_WELDING_DETECTION_REQ) && (EXI_ENCODE_DIN_WELDING_DETECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_WeldingDetectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_WeldingDetectionReqType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionReqPtr );
#endif /* (defined(EXI_ENCODE_DIN_WELDING_DETECTION_REQ) && (EXI_ENCODE_DIN_WELDING_DETECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_WeldingDetectionRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_WeldingDetectionResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     WeldingDetectionResPtr      pointer to Exi_DIN_WeldingDetectionResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_WELDING_DETECTION_RES) && (EXI_ENCODE_DIN_WELDING_DETECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_WeldingDetectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_WeldingDetectionResType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionResPtr );
#endif /* (defined(EXI_ENCODE_DIN_WELDING_DETECTION_RES) && (EXI_ENCODE_DIN_WELDING_DETECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_certificate
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_certificateType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     certificatePtr              pointer to Exi_DIN_certificateType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CERTIFICATE) && (EXI_ENCODE_DIN_CERTIFICATE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_certificate( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_certificateType, AUTOMATIC, EXI_APPL_DATA) certificatePtr );
#endif /* (defined(EXI_ENCODE_DIN_CERTIFICATE) && (EXI_ENCODE_DIN_CERTIFICATE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_contractID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_contractIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     contractIDPtr               pointer to Exi_DIN_contractIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_CONTRACT_ID) && (EXI_ENCODE_DIN_CONTRACT_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_contractID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_contractIDType, AUTOMATIC, EXI_APPL_DATA) contractIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_CONTRACT_ID) && (EXI_ENCODE_DIN_CONTRACT_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_costKind
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_costKindType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     costKindPtr                 pointer to Exi_DIN_costKindType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_COST_KIND) && (EXI_ENCODE_DIN_COST_KIND == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_costKind( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_costKindType, AUTOMATIC, EXI_APPL_DATA) costKindPtr );
#endif /* (defined(EXI_ENCODE_DIN_COST_KIND) && (EXI_ENCODE_DIN_COST_KIND == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_dHParams
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_dHParamsType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     dHParamsPtr                 pointer to Exi_DIN_dHParamsType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_D_HPARAMS) && (EXI_ENCODE_DIN_D_HPARAMS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_dHParams( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_dHParamsType, AUTOMATIC, EXI_APPL_DATA) dHParamsPtr );
#endif /* (defined(EXI_ENCODE_DIN_D_HPARAMS) && (EXI_ENCODE_DIN_D_HPARAMS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_evccID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_evccIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     evccIDPtr                   pointer to Exi_DIN_evccIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVCC_ID) && (EXI_ENCODE_DIN_EVCC_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_evccID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_evccIDType, AUTOMATIC, EXI_APPL_DATA) evccIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVCC_ID) && (EXI_ENCODE_DIN_EVCC_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_evseID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_evseIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     evseIDPtr                   pointer to Exi_DIN_evseIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_EVSE_ID) && (EXI_ENCODE_DIN_EVSE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_evseID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_evseIDType, AUTOMATIC, EXI_APPL_DATA) evseIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_EVSE_ID) && (EXI_ENCODE_DIN_EVSE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_faultCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_faultCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     faultCodePtr                pointer to Exi_DIN_faultCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_FAULT_CODE) && (EXI_ENCODE_DIN_FAULT_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_faultCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_faultCodeType, AUTOMATIC, EXI_APPL_DATA) faultCodePtr );
#endif /* (defined(EXI_ENCODE_DIN_FAULT_CODE) && (EXI_ENCODE_DIN_FAULT_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_faultMsg
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_faultMsgType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     faultMsgPtr                 pointer to Exi_DIN_faultMsgType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_FAULT_MSG) && (EXI_ENCODE_DIN_FAULT_MSG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_faultMsg( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_faultMsgType, AUTOMATIC, EXI_APPL_DATA) faultMsgPtr );
#endif /* (defined(EXI_ENCODE_DIN_FAULT_MSG) && (EXI_ENCODE_DIN_FAULT_MSG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_genChallenge
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_genChallengeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     genChallengePtr             pointer to Exi_DIN_genChallengeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_GEN_CHALLENGE) && (EXI_ENCODE_DIN_GEN_CHALLENGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_genChallenge( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_genChallengeType, AUTOMATIC, EXI_APPL_DATA) genChallengePtr );
#endif /* (defined(EXI_ENCODE_DIN_GEN_CHALLENGE) && (EXI_ENCODE_DIN_GEN_CHALLENGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_isolationLevel
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_isolationLevelType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     isolationLevelPtr           pointer to Exi_DIN_isolationLevelType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ISOLATION_LEVEL) && (EXI_ENCODE_DIN_ISOLATION_LEVEL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_isolationLevel( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_isolationLevelType, AUTOMATIC, EXI_APPL_DATA) isolationLevelPtr );
#endif /* (defined(EXI_ENCODE_DIN_ISOLATION_LEVEL) && (EXI_ENCODE_DIN_ISOLATION_LEVEL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_meterID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_meterIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     meterIDPtr                  pointer to Exi_DIN_meterIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_METER_ID) && (EXI_ENCODE_DIN_METER_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_meterID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_meterIDType, AUTOMATIC, EXI_APPL_DATA) meterIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_METER_ID) && (EXI_ENCODE_DIN_METER_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_paymentOption
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_paymentOptionType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     paymentOptionPtr            pointer to Exi_DIN_paymentOptionType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PAYMENT_OPTION) && (EXI_ENCODE_DIN_PAYMENT_OPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_paymentOption( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_paymentOptionType, AUTOMATIC, EXI_APPL_DATA) paymentOptionPtr );
#endif /* (defined(EXI_ENCODE_DIN_PAYMENT_OPTION) && (EXI_ENCODE_DIN_PAYMENT_OPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_privateKey
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_privateKeyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     privateKeyPtr               pointer to Exi_DIN_privateKeyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_PRIVATE_KEY) && (EXI_ENCODE_DIN_PRIVATE_KEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_privateKey( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_privateKeyType, AUTOMATIC, EXI_APPL_DATA) privateKeyPtr );
#endif /* (defined(EXI_ENCODE_DIN_PRIVATE_KEY) && (EXI_ENCODE_DIN_PRIVATE_KEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_responseCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_responseCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     responseCodePtr             pointer to Exi_DIN_responseCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_RESPONSE_CODE) && (EXI_ENCODE_DIN_RESPONSE_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_responseCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_responseCodeType, AUTOMATIC, EXI_APPL_DATA) responseCodePtr );
#endif /* (defined(EXI_ENCODE_DIN_RESPONSE_CODE) && (EXI_ENCODE_DIN_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_rootCertificateID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_rootCertificateIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     rootCertificateIDPtr        pointer to Exi_DIN_rootCertificateIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID) && (EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_rootCertificateID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_rootCertificateIDType, AUTOMATIC, EXI_APPL_DATA) rootCertificateIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID) && (EXI_ENCODE_DIN_ROOT_CERTIFICATE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_serviceCategory
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_serviceCategoryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceCategoryPtr          pointer to Exi_DIN_serviceCategoryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_CATEGORY) && (EXI_ENCODE_DIN_SERVICE_CATEGORY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_serviceCategory( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_serviceCategoryType, AUTOMATIC, EXI_APPL_DATA) serviceCategoryPtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_CATEGORY) && (EXI_ENCODE_DIN_SERVICE_CATEGORY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_serviceName
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_serviceNameType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceNamePtr              pointer to Exi_DIN_serviceNameType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_NAME) && (EXI_ENCODE_DIN_SERVICE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_serviceName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_serviceNameType, AUTOMATIC, EXI_APPL_DATA) serviceNamePtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_NAME) && (EXI_ENCODE_DIN_SERVICE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_serviceScope
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_serviceScopeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceScopePtr             pointer to Exi_DIN_serviceScopeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SERVICE_SCOPE) && (EXI_ENCODE_DIN_SERVICE_SCOPE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_serviceScope( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_serviceScopeType, AUTOMATIC, EXI_APPL_DATA) serviceScopePtr );
#endif /* (defined(EXI_ENCODE_DIN_SERVICE_SCOPE) && (EXI_ENCODE_DIN_SERVICE_SCOPE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_sessionID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_sessionIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     sessionIDPtr                pointer to Exi_DIN_sessionIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SESSION_ID) && (EXI_ENCODE_DIN_SESSION_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_sessionID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_sessionIDType, AUTOMATIC, EXI_APPL_DATA) sessionIDPtr );
#endif /* (defined(EXI_ENCODE_DIN_SESSION_ID) && (EXI_ENCODE_DIN_SESSION_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_sigMeterReading
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_sigMeterReadingType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     sigMeterReadingPtr          pointer to Exi_DIN_sigMeterReadingType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SIG_METER_READING) && (EXI_ENCODE_DIN_SIG_METER_READING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_sigMeterReading( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_sigMeterReadingType, AUTOMATIC, EXI_APPL_DATA) sigMeterReadingPtr );
#endif /* (defined(EXI_ENCODE_DIN_SIG_METER_READING) && (EXI_ENCODE_DIN_SIG_METER_READING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_tariffDescription
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_tariffDescriptionType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     tariffDescriptionPtr        pointer to Exi_DIN_tariffDescriptionType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_TARIFF_DESCRIPTION) && (EXI_ENCODE_DIN_TARIFF_DESCRIPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_tariffDescription( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_tariffDescriptionType, AUTOMATIC, EXI_APPL_DATA) tariffDescriptionPtr );
#endif /* (defined(EXI_ENCODE_DIN_TARIFF_DESCRIPTION) && (EXI_ENCODE_DIN_TARIFF_DESCRIPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_unitSymbol
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_unitSymbolType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     unitSymbolPtr               pointer to Exi_DIN_unitSymbolType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_UNIT_SYMBOL) && (EXI_ENCODE_DIN_UNIT_SYMBOL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_unitSymbol( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_DIN_unitSymbolType, AUTOMATIC, EXI_APPL_DATA) unitSymbolPtr );
#endif /* (defined(EXI_ENCODE_DIN_UNIT_SYMBOL) && (EXI_ENCODE_DIN_UNIT_SYMBOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SCHEMA_FRAGMENT) && (EXI_ENCODE_DIN_SCHEMA_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_DIN_SCHEMA_FRAGMENT) && (EXI_ENCODE_DIN_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_DIN_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_DIN_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_DIN_SCHEMA_ROOT) && (EXI_ENCODE_DIN_SCHEMA_ROOT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_DIN_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_DIN_SCHEMA_ROOT) && (EXI_ENCODE_DIN_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined(EXI_ENABLE_ENCODE_DIN_MESSAGE_SET) && (EXI_ENABLE_ENCODE_DIN_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_DIN_SCHEMA_ENCODER_H */
