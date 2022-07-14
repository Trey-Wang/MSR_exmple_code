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
 *         File:  Exi_ISO_SchemaDecoder.h
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


#if !defined (EXI_ISO_SCHEMA_DECODER_H)
# define EXI_ISO_SCHEMA_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaDecoder.h"

#if (!defined (EXI_ENABLE_DECODE_ISO_MESSAGE_SET))
# if (defined (EXI_ENABLE_ISO_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_ISO_MESSAGE_SET   EXI_ENABLE_ISO_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_ISO_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_ISO_MESSAGE_SET) && (EXI_ENABLE_DECODE_ISO_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_DECODE_ISO_AC_EVCHARGE_PARAMETER 0x96U
#define EXI_API_ID_DECODE_ISO_AC_EVSECHARGE_PARAMETER 0x97U
#define EXI_API_ID_DECODE_ISO_AC_EVSESTATUS 0x98U
#define EXI_API_ID_DECODE_ISO_ATTRIBUTE_ID 0x99U
#define EXI_API_ID_DECODE_ISO_ATTRIBUTE_NAME 0x9AU
#define EXI_API_ID_DECODE_ISO_AUTHORIZATION_REQ 0x9BU
#define EXI_API_ID_DECODE_ISO_AUTHORIZATION_RES 0x9CU
#define EXI_API_ID_DECODE_ISO_BODY 0x9DU
#define EXI_API_ID_DECODE_ISO_CABLE_CHECK_REQ 0x9EU
#define EXI_API_ID_DECODE_ISO_CABLE_CHECK_RES 0x9FU
#define EXI_API_ID_DECODE_ISO_CERTIFICATE_CHAIN 0xA0U
#define EXI_API_ID_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ 0xA1U
#define EXI_API_ID_DECODE_ISO_CERTIFICATE_INSTALLATION_RES 0xA2U
#define EXI_API_ID_DECODE_ISO_CERTIFICATE_UPDATE_REQ 0xA3U
#define EXI_API_ID_DECODE_ISO_CERTIFICATE_UPDATE_RES 0xA4U
#define EXI_API_ID_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ 0xA5U
#define EXI_API_ID_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES 0xA6U
#define EXI_API_ID_DECODE_ISO_CHARGE_SERVICE 0xA7U
#define EXI_API_ID_DECODE_ISO_CHARGING_PROFILE 0xA8U
#define EXI_API_ID_DECODE_ISO_CHARGING_STATUS_RES 0xA9U
#define EXI_API_ID_DECODE_ISO_CONSUMPTION_COST 0xAAU
#define EXI_API_ID_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY 0xABU
#define EXI_API_ID_DECODE_ISO_COST 0xACU
#define EXI_API_ID_DECODE_ISO_CURRENT_DEMAND_REQ 0xADU
#define EXI_API_ID_DECODE_ISO_CURRENT_DEMAND_RES 0xAEU
#define EXI_API_ID_DECODE_ISO_DC_EVCHARGE_PARAMETER 0xAFU
#define EXI_API_ID_DECODE_ISO_DC_EVERROR_CODE 0xB0U
#define EXI_API_ID_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER 0xB1U
#define EXI_API_ID_DECODE_ISO_DC_EVSECHARGE_PARAMETER 0xB2U
#define EXI_API_ID_DECODE_ISO_DC_EVSESTATUS_CODE 0xB3U
#define EXI_API_ID_DECODE_ISO_DC_EVSESTATUS 0xB4U
#define EXI_API_ID_DECODE_ISO_DC_EVSTATUS 0xB5U
#define EXI_API_ID_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY 0xB6U
#define EXI_API_ID_DECODE_ISO_EMAID 0xB7U
#define EXI_API_ID_DECODE_ISO_EVCHARGE_PARAMETER 0xB8U
#define EXI_API_ID_DECODE_ISO_EVSENOTIFICATION 0xB9U
#define EXI_API_ID_DECODE_ISO_EVSEPROCESSING 0xBAU
#define EXI_API_ID_DECODE_ISO_EVSESTATUS 0xBBU
#define EXI_API_ID_DECODE_ISO_ENERGY_TRANSFER_MODE 0xBCU
#define EXI_API_ID_DECODE_ISO_ENTRY 0xBDU
#define EXI_API_ID_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS 0xBEU
#define EXI_API_ID_DECODE_ISO_MESSAGE_HEADER 0xBFU
#define EXI_API_ID_DECODE_ISO_METER_INFO 0xC0U
#define EXI_API_ID_DECODE_ISO_METERING_RECEIPT_REQ 0xC1U
#define EXI_API_ID_DECODE_ISO_METERING_RECEIPT_RES 0xC2U
#define EXI_API_ID_DECODE_ISO_NOTIFICATION 0xC3U
#define EXI_API_ID_DECODE_ISO_PMAX_SCHEDULE_ENTRY 0xC4U
#define EXI_API_ID_DECODE_ISO_PMAX_SCHEDULE 0xC5U
#define EXI_API_ID_DECODE_ISO_PARAMETER_SET 0xC6U
#define EXI_API_ID_DECODE_ISO_PARAMETER 0xC7U
#define EXI_API_ID_DECODE_ISO_PAYMENT_DETAILS_REQ 0xC8U
#define EXI_API_ID_DECODE_ISO_PAYMENT_DETAILS_RES 0xC9U
#define EXI_API_ID_DECODE_ISO_PAYMENT_OPTION_LIST 0xCAU
#define EXI_API_ID_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ 0xCBU
#define EXI_API_ID_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES 0xCCU
#define EXI_API_ID_DECODE_ISO_PHYSICAL_VALUE 0xCDU
#define EXI_API_ID_DECODE_ISO_POWER_DELIVERY_REQ 0xCEU
#define EXI_API_ID_DECODE_ISO_POWER_DELIVERY_RES 0xCFU
#define EXI_API_ID_DECODE_ISO_PRE_CHARGE_REQ 0xD0U
#define EXI_API_ID_DECODE_ISO_PRE_CHARGE_RES 0xD1U
#define EXI_API_ID_DECODE_ISO_PROFILE_ENTRY 0xD2U
#define EXI_API_ID_DECODE_ISO_RELATIVE_TIME_INTERVAL 0xD3U
#define EXI_API_ID_DECODE_ISO_SASCHEDULE_LIST 0xD4U
#define EXI_API_ID_DECODE_ISO_SASCHEDULE_TUPLE 0xD5U
#define EXI_API_ID_DECODE_ISO_SALES_TARIFF_ENTRY 0xD6U
#define EXI_API_ID_DECODE_ISO_SALES_TARIFF 0xD7U
#define EXI_API_ID_DECODE_ISO_SELECTED_SERVICE_LIST 0xD8U
#define EXI_API_ID_DECODE_ISO_SELECTED_SERVICE 0xD9U
#define EXI_API_ID_DECODE_ISO_SERVICE_DETAIL_REQ 0xDAU
#define EXI_API_ID_DECODE_ISO_SERVICE_DETAIL_RES 0xDBU
#define EXI_API_ID_DECODE_ISO_SERVICE_DISCOVERY_REQ 0xDCU
#define EXI_API_ID_DECODE_ISO_SERVICE_DISCOVERY_RES 0xDDU
#define EXI_API_ID_DECODE_ISO_SERVICE_LIST 0xDEU
#define EXI_API_ID_DECODE_ISO_SERVICE_PARAMETER_LIST 0xDFU
#define EXI_API_ID_DECODE_ISO_SERVICE 0xE0U
#define EXI_API_ID_DECODE_ISO_SESSION_SETUP_REQ 0xE1U
#define EXI_API_ID_DECODE_ISO_SESSION_SETUP_RES 0xE2U
#define EXI_API_ID_DECODE_ISO_SESSION_STOP_REQ 0xE3U
#define EXI_API_ID_DECODE_ISO_SESSION_STOP_RES 0xE4U
#define EXI_API_ID_DECODE_ISO_SUB_CERTIFICATES 0xE5U
#define EXI_API_ID_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE 0xE6U
#define EXI_API_ID_DECODE_ISO_V2G_MESSAGE 0xE7U
#define EXI_API_ID_DECODE_ISO_WELDING_DETECTION_REQ 0xE8U
#define EXI_API_ID_DECODE_ISO_WELDING_DETECTION_RES 0xE9U
#define EXI_API_ID_DECODE_ISO_CERTIFICATE 0xEAU
#define EXI_API_ID_DECODE_ISO_CHARGE_PROGRESS 0xEBU
#define EXI_API_ID_DECODE_ISO_CHARGING_SESSION 0xECU
#define EXI_API_ID_DECODE_ISO_COST_KIND 0xEDU
#define EXI_API_ID_DECODE_ISO_E_MAID 0xEEU
#define EXI_API_ID_DECODE_ISO_EVCC_ID 0xEFU
#define EXI_API_ID_DECODE_ISO_EVSE_ID 0xF0U
#define EXI_API_ID_DECODE_ISO_FAULT_CODE 0xF1U
#define EXI_API_ID_DECODE_ISO_FAULT_MSG 0xF2U
#define EXI_API_ID_DECODE_ISO_GEN_CHALLENGE 0xF3U
#define EXI_API_ID_DECODE_ISO_ISOLATION_LEVEL 0xF4U
#define EXI_API_ID_DECODE_ISO_METER_ID 0xF5U
#define EXI_API_ID_DECODE_ISO_PAYMENT_OPTION 0xF6U
#define EXI_API_ID_DECODE_ISO_RESPONSE_CODE 0xF7U
#define EXI_API_ID_DECODE_ISO_SERVICE_CATEGORY 0xF8U
#define EXI_API_ID_DECODE_ISO_SERVICE_NAME 0xF9U
#define EXI_API_ID_DECODE_ISO_SERVICE_SCOPE 0xFAU
#define EXI_API_ID_DECODE_ISO_SESSION_ID 0xFBU
#define EXI_API_ID_DECODE_ISO_SIG_METER_READING 0xFCU
#define EXI_API_ID_DECODE_ISO_TARIFF_DESCRIPTION 0xFDU
#define EXI_API_ID_DECODE_ISO_UNIT_SYMBOL 0xFEU
#define EXI_API_ID_DECODE_SCHEMA_SET_ISO 0x175U
#endif

/* Decoding default switches */
#ifndef EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER
#define EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER
#define EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_AC_EVSESTATUS
#define EXI_DECODE_ISO_AC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_ATTRIBUTE_ID
#define EXI_DECODE_ISO_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_ATTRIBUTE_NAME
#define EXI_DECODE_ISO_ATTRIBUTE_NAME STD_OFF
#endif
#ifndef EXI_DECODE_ISO_AUTHORIZATION_REQ
#define EXI_DECODE_ISO_AUTHORIZATION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_AUTHORIZATION_RES
#define EXI_DECODE_ISO_AUTHORIZATION_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_BODY_BASE
#define EXI_DECODE_ISO_BODY_BASE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_BODY
#define EXI_DECODE_ISO_BODY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CABLE_CHECK_REQ
#define EXI_DECODE_ISO_CABLE_CHECK_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CABLE_CHECK_RES
#define EXI_DECODE_ISO_CABLE_CHECK_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE_CHAIN
#define EXI_DECODE_ISO_CERTIFICATE_CHAIN STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ
#define EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES
#define EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ
#define EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES
#define EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ
#define EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES
#define EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGE_SERVICE
#define EXI_DECODE_ISO_CHARGE_SERVICE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGING_PROFILE
#define EXI_DECODE_ISO_CHARGING_PROFILE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGING_STATUS_REQ
#define EXI_DECODE_ISO_CHARGING_STATUS_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGING_STATUS_RES
#define EXI_DECODE_ISO_CHARGING_STATUS_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CONSUMPTION_COST
#define EXI_DECODE_ISO_CONSUMPTION_COST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY
#define EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_COST
#define EXI_DECODE_ISO_COST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CURRENT_DEMAND_REQ
#define EXI_DECODE_ISO_CURRENT_DEMAND_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CURRENT_DEMAND_RES
#define EXI_DECODE_ISO_CURRENT_DEMAND_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER
#define EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVERROR_CODE
#define EXI_DECODE_ISO_DC_EVERROR_CODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER
#define EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER
#define EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVSESTATUS_CODE
#define EXI_DECODE_ISO_DC_EVSESTATUS_CODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVSESTATUS
#define EXI_DECODE_ISO_DC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DC_EVSTATUS
#define EXI_DECODE_ISO_DC_EVSTATUS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY
#define EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EMAID
#define EXI_DECODE_ISO_EMAID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVCHARGE_PARAMETER
#define EXI_DECODE_ISO_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVPOWER_DELIVERY_PARAMETER
#define EXI_DECODE_ISO_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSECHARGE_PARAMETER
#define EXI_DECODE_ISO_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSENOTIFICATION
#define EXI_DECODE_ISO_EVSENOTIFICATION STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSEPROCESSING
#define EXI_DECODE_ISO_EVSEPROCESSING STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSESTATUS
#define EXI_DECODE_ISO_EVSESTATUS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSTATUS
#define EXI_DECODE_ISO_EVSTATUS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_ENERGY_TRANSFER_MODE
#define EXI_DECODE_ISO_ENERGY_TRANSFER_MODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_ENTRY
#define EXI_DECODE_ISO_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_INTERVAL
#define EXI_DECODE_ISO_INTERVAL STD_OFF
#endif
#ifndef EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS
#define EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_MESSAGE_HEADER
#define EXI_DECODE_ISO_MESSAGE_HEADER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_METER_INFO
#define EXI_DECODE_ISO_METER_INFO STD_OFF
#endif
#ifndef EXI_DECODE_ISO_METERING_RECEIPT_REQ
#define EXI_DECODE_ISO_METERING_RECEIPT_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_METERING_RECEIPT_RES
#define EXI_DECODE_ISO_METERING_RECEIPT_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_NOTIFICATION
#define EXI_DECODE_ISO_NOTIFICATION STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY
#define EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PMAX_SCHEDULE
#define EXI_DECODE_ISO_PMAX_SCHEDULE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PARAMETER_SET
#define EXI_DECODE_ISO_PARAMETER_SET STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PARAMETER
#define EXI_DECODE_ISO_PARAMETER STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_DETAILS_REQ
#define EXI_DECODE_ISO_PAYMENT_DETAILS_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_DETAILS_RES
#define EXI_DECODE_ISO_PAYMENT_DETAILS_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_OPTION_LIST
#define EXI_DECODE_ISO_PAYMENT_OPTION_LIST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ
#define EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES
#define EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PHYSICAL_VALUE
#define EXI_DECODE_ISO_PHYSICAL_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_POWER_DELIVERY_REQ
#define EXI_DECODE_ISO_POWER_DELIVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_POWER_DELIVERY_RES
#define EXI_DECODE_ISO_POWER_DELIVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PRE_CHARGE_REQ
#define EXI_DECODE_ISO_PRE_CHARGE_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PRE_CHARGE_RES
#define EXI_DECODE_ISO_PRE_CHARGE_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PROFILE_ENTRY
#define EXI_DECODE_ISO_PROFILE_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL
#define EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SASCHEDULE_LIST
#define EXI_DECODE_ISO_SASCHEDULE_LIST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SASCHEDULE_TUPLE
#define EXI_DECODE_ISO_SASCHEDULE_TUPLE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SASCHEDULES
#define EXI_DECODE_ISO_SASCHEDULES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SALES_TARIFF_ENTRY
#define EXI_DECODE_ISO_SALES_TARIFF_ENTRY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SALES_TARIFF
#define EXI_DECODE_ISO_SALES_TARIFF STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SELECTED_SERVICE_LIST
#define EXI_DECODE_ISO_SELECTED_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SELECTED_SERVICE
#define EXI_DECODE_ISO_SELECTED_SERVICE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_DETAIL_REQ
#define EXI_DECODE_ISO_SERVICE_DETAIL_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_DETAIL_RES
#define EXI_DECODE_ISO_SERVICE_DETAIL_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ
#define EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_DISCOVERY_RES
#define EXI_DECODE_ISO_SERVICE_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_LIST
#define EXI_DECODE_ISO_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_PARAMETER_LIST
#define EXI_DECODE_ISO_SERVICE_PARAMETER_LIST STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE
#define EXI_DECODE_ISO_SERVICE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SESSION_SETUP_REQ
#define EXI_DECODE_ISO_SESSION_SETUP_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SESSION_SETUP_RES
#define EXI_DECODE_ISO_SESSION_SETUP_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SESSION_STOP_REQ
#define EXI_DECODE_ISO_SESSION_STOP_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SESSION_STOP_RES
#define EXI_DECODE_ISO_SESSION_STOP_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SUB_CERTIFICATES
#define EXI_DECODE_ISO_SUB_CERTIFICATES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE
#define EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_V2G_MESSAGE
#define EXI_DECODE_ISO_V2G_MESSAGE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_WELDING_DETECTION_REQ
#define EXI_DECODE_ISO_WELDING_DETECTION_REQ STD_OFF
#endif
#ifndef EXI_DECODE_ISO_WELDING_DETECTION_RES
#define EXI_DECODE_ISO_WELDING_DETECTION_RES STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CERTIFICATE
#define EXI_DECODE_ISO_CERTIFICATE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGE_PROGRESS
#define EXI_DECODE_ISO_CHARGE_PROGRESS STD_OFF
#endif
#ifndef EXI_DECODE_ISO_CHARGING_SESSION
#define EXI_DECODE_ISO_CHARGING_SESSION STD_OFF
#endif
#ifndef EXI_DECODE_ISO_COST_KIND
#define EXI_DECODE_ISO_COST_KIND STD_OFF
#endif
#ifndef EXI_DECODE_ISO_E_MAID
#define EXI_DECODE_ISO_E_MAID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVCC_ID
#define EXI_DECODE_ISO_EVCC_ID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_EVSE_ID
#define EXI_DECODE_ISO_EVSE_ID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_FAULT_CODE
#define EXI_DECODE_ISO_FAULT_CODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_FAULT_MSG
#define EXI_DECODE_ISO_FAULT_MSG STD_OFF
#endif
#ifndef EXI_DECODE_ISO_GEN_CHALLENGE
#define EXI_DECODE_ISO_GEN_CHALLENGE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_ISOLATION_LEVEL
#define EXI_DECODE_ISO_ISOLATION_LEVEL STD_OFF
#endif
#ifndef EXI_DECODE_ISO_METER_ID
#define EXI_DECODE_ISO_METER_ID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_PAYMENT_OPTION
#define EXI_DECODE_ISO_PAYMENT_OPTION STD_OFF
#endif
#ifndef EXI_DECODE_ISO_RESPONSE_CODE
#define EXI_DECODE_ISO_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_CATEGORY
#define EXI_DECODE_ISO_SERVICE_CATEGORY STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_NAME
#define EXI_DECODE_ISO_SERVICE_NAME STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SERVICE_SCOPE
#define EXI_DECODE_ISO_SERVICE_SCOPE STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SESSION_ID
#define EXI_DECODE_ISO_SESSION_ID STD_OFF
#endif
#ifndef EXI_DECODE_ISO_SIG_METER_READING
#define EXI_DECODE_ISO_SIG_METER_READING STD_OFF
#endif
#ifndef EXI_DECODE_ISO_TARIFF_DESCRIPTION
#define EXI_DECODE_ISO_TARIFF_DESCRIPTION STD_OFF
#endif
#ifndef EXI_DECODE_ISO_UNIT_SYMBOL
#define EXI_DECODE_ISO_UNIT_SYMBOL STD_OFF
#endif
#ifndef EXI_DECODE_SCHEMA_SET_ISO
#define EXI_DECODE_SCHEMA_SET_ISO STD_OFF
#endif


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Decode_ISO_AC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AC_EVChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AC_EVChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AC_EVSEChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AC_EVSEChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_DECODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AC_EVSEStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_AC_EVSESTATUS) && (EXI_DECODE_ISO_AC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AC_EVSEStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_AC_EVSESTATUS) && (EXI_DECODE_ISO_AC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AttributeId
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AttributeIdType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_ATTRIBUTE_ID) && (EXI_DECODE_ISO_ATTRIBUTE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AttributeId( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_ATTRIBUTE_ID) && (EXI_DECODE_ISO_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AttributeName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AttributeNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_ATTRIBUTE_NAME) && (EXI_DECODE_ISO_ATTRIBUTE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AttributeName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_ATTRIBUTE_NAME) && (EXI_DECODE_ISO_ATTRIBUTE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AuthorizationReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AuthorizationReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_AUTHORIZATION_REQ) && (EXI_DECODE_ISO_AUTHORIZATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AuthorizationReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_AUTHORIZATION_REQ) && (EXI_DECODE_ISO_AUTHORIZATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_AuthorizationRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_AuthorizationResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_AUTHORIZATION_RES) && (EXI_DECODE_ISO_AUTHORIZATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_AuthorizationRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_AUTHORIZATION_RES) && (EXI_DECODE_ISO_AUTHORIZATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Body
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_BodyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_BODY) && (EXI_DECODE_ISO_BODY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Body( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_BODY) && (EXI_DECODE_ISO_BODY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CableCheckReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CableCheckReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CABLE_CHECK_REQ) && (EXI_DECODE_ISO_CABLE_CHECK_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CableCheckReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CABLE_CHECK_REQ) && (EXI_DECODE_ISO_CABLE_CHECK_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CableCheckRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CableCheckResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CABLE_CHECK_RES) && (EXI_DECODE_ISO_CABLE_CHECK_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CableCheckRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CABLE_CHECK_RES) && (EXI_DECODE_ISO_CABLE_CHECK_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CertificateChain
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CertificateChainType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE_CHAIN) && (EXI_DECODE_ISO_CERTIFICATE_CHAIN == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CertificateChain( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE_CHAIN) && (EXI_DECODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CertificateInstallationReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CertificateInstallationReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CertificateInstallationReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CertificateInstallationRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CertificateInstallationResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CertificateInstallationRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_DECODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CertificateUpdateReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CertificateUpdateReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CertificateUpdateReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_DECODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CertificateUpdateRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CertificateUpdateResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CertificateUpdateRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_DECODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ChargeParameterDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ChargeParameterDiscoveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ChargeParameterDiscoveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ChargeParameterDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ChargeParameterDiscoveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ChargeParameterDiscoveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_DECODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ChargeService
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ChargeServiceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGE_SERVICE) && (EXI_DECODE_ISO_CHARGE_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ChargeService( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGE_SERVICE) && (EXI_DECODE_ISO_CHARGE_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ChargingProfile
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ChargingProfileType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGING_PROFILE) && (EXI_DECODE_ISO_CHARGING_PROFILE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ChargingProfile( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGING_PROFILE) && (EXI_DECODE_ISO_CHARGING_PROFILE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ChargingStatusRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ChargingStatusResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGING_STATUS_RES) && (EXI_DECODE_ISO_CHARGING_STATUS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ChargingStatusRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGING_STATUS_RES) && (EXI_DECODE_ISO_CHARGING_STATUS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ConsumptionCost
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ConsumptionCostType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CONSUMPTION_COST) && (EXI_DECODE_ISO_CONSUMPTION_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ConsumptionCost( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CONSUMPTION_COST) && (EXI_DECODE_ISO_CONSUMPTION_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ContractSignatureEncryptedPrivateKey
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ContractSignatureEncryptedPrivateKeyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ContractSignatureEncryptedPrivateKey( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_DECODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Cost
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CostType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_COST) && (EXI_DECODE_ISO_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Cost( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_COST) && (EXI_DECODE_ISO_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CurrentDemandReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CurrentDemandReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CURRENT_DEMAND_REQ) && (EXI_DECODE_ISO_CURRENT_DEMAND_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CurrentDemandReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CURRENT_DEMAND_REQ) && (EXI_DECODE_ISO_CURRENT_DEMAND_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_CurrentDemandRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_CurrentDemandResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CURRENT_DEMAND_RES) && (EXI_DECODE_ISO_CURRENT_DEMAND_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_CurrentDemandRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CURRENT_DEMAND_RES) && (EXI_DECODE_ISO_CURRENT_DEMAND_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVErrorCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVErrorCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVERROR_CODE) && (EXI_DECODE_ISO_DC_EVERROR_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVErrorCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVERROR_CODE) && (EXI_DECODE_ISO_DC_EVERROR_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVPowerDeliveryParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVPowerDeliveryParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVPowerDeliveryParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_DECODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVSEChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVSEChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_DECODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVSEStatusCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVSEStatusCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVSESTATUS_CODE) && (EXI_DECODE_ISO_DC_EVSESTATUS_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVSEStatusCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVSESTATUS_CODE) && (EXI_DECODE_ISO_DC_EVSESTATUS_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVSEStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVSESTATUS) && (EXI_DECODE_ISO_DC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVSEStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVSESTATUS) && (EXI_DECODE_ISO_DC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DC_EVStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DC_EVStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DC_EVSTATUS) && (EXI_DECODE_ISO_DC_EVSTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DC_EVStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DC_EVSTATUS) && (EXI_DECODE_ISO_DC_EVSTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_DiffieHellmanPublickey
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_DiffieHellmanPublickeyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_DiffieHellmanPublickey( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_DECODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EMAID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EMAIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EMAID) && (EXI_DECODE_ISO_EMAID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EMAID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EMAID) && (EXI_DECODE_ISO_EMAID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EVChargeParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EVChargeParameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVCHARGE_PARAMETER) && (EXI_DECODE_ISO_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EVSENotification
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EVSENotificationType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVSENOTIFICATION) && (EXI_DECODE_ISO_EVSENOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EVSENotification( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVSENOTIFICATION) && (EXI_DECODE_ISO_EVSENOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EVSEProcessing
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EVSEProcessingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVSEPROCESSING) && (EXI_DECODE_ISO_EVSEPROCESSING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EVSEProcessing( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVSEPROCESSING) && (EXI_DECODE_ISO_EVSEPROCESSING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EVSEStatusType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVSESTATUS) && (EXI_DECODE_ISO_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EVSEStatus( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVSESTATUS) && (EXI_DECODE_ISO_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_EnergyTransferMode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EnergyTransferModeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_DECODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_EnergyTransferMode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_DECODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Entry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_EntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_ENTRY) && (EXI_DECODE_ISO_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Entry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_ENTRY) && (EXI_DECODE_ISO_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ListOfRootCertificateIDs
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ListOfRootCertificateIDsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ListOfRootCertificateIDs( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_DECODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_MessageHeader
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_MessageHeaderType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_MESSAGE_HEADER) && (EXI_DECODE_ISO_MESSAGE_HEADER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_MessageHeader( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_MESSAGE_HEADER) && (EXI_DECODE_ISO_MESSAGE_HEADER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_MeterInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_MeterInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_METER_INFO) && (EXI_DECODE_ISO_METER_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_MeterInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_METER_INFO) && (EXI_DECODE_ISO_METER_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_MeteringReceiptReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_MeteringReceiptReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_METERING_RECEIPT_REQ) && (EXI_DECODE_ISO_METERING_RECEIPT_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_MeteringReceiptReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_METERING_RECEIPT_REQ) && (EXI_DECODE_ISO_METERING_RECEIPT_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_MeteringReceiptRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_MeteringReceiptResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_METERING_RECEIPT_RES) && (EXI_DECODE_ISO_METERING_RECEIPT_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_MeteringReceiptRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_METERING_RECEIPT_RES) && (EXI_DECODE_ISO_METERING_RECEIPT_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Notification
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_NotificationType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_NOTIFICATION) && (EXI_DECODE_ISO_NOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Notification( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_NOTIFICATION) && (EXI_DECODE_ISO_NOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PMaxScheduleEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PMaxScheduleEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PMaxScheduleEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_DECODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PMaxSchedule
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PMaxScheduleType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PMAX_SCHEDULE) && (EXI_DECODE_ISO_PMAX_SCHEDULE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PMaxSchedule( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PMAX_SCHEDULE) && (EXI_DECODE_ISO_PMAX_SCHEDULE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ParameterSet
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ParameterSetType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PARAMETER_SET) && (EXI_DECODE_ISO_PARAMETER_SET == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ParameterSet( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PARAMETER_SET) && (EXI_DECODE_ISO_PARAMETER_SET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Parameter
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ParameterType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PARAMETER) && (EXI_DECODE_ISO_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Parameter( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PARAMETER) && (EXI_DECODE_ISO_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PaymentDetailsReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PaymentDetailsReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_DECODE_ISO_PAYMENT_DETAILS_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PaymentDetailsReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_DECODE_ISO_PAYMENT_DETAILS_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PaymentDetailsRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PaymentDetailsResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_DETAILS_RES) && (EXI_DECODE_ISO_PAYMENT_DETAILS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PaymentDetailsRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_DETAILS_RES) && (EXI_DECODE_ISO_PAYMENT_DETAILS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PaymentOptionList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PaymentOptionListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_OPTION_LIST) && (EXI_DECODE_ISO_PAYMENT_OPTION_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PaymentOptionList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_OPTION_LIST) && (EXI_DECODE_ISO_PAYMENT_OPTION_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PaymentServiceSelectionReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PaymentServiceSelectionReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PaymentServiceSelectionReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PaymentServiceSelectionRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PaymentServiceSelectionResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PaymentServiceSelectionRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_DECODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PhysicalValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PhysicalValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PHYSICAL_VALUE) && (EXI_DECODE_ISO_PHYSICAL_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PhysicalValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PHYSICAL_VALUE) && (EXI_DECODE_ISO_PHYSICAL_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PowerDeliveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PowerDeliveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_POWER_DELIVERY_REQ) && (EXI_DECODE_ISO_POWER_DELIVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PowerDeliveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_POWER_DELIVERY_REQ) && (EXI_DECODE_ISO_POWER_DELIVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PowerDeliveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PowerDeliveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_POWER_DELIVERY_RES) && (EXI_DECODE_ISO_POWER_DELIVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PowerDeliveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_POWER_DELIVERY_RES) && (EXI_DECODE_ISO_POWER_DELIVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PreChargeReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PreChargeReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PRE_CHARGE_REQ) && (EXI_DECODE_ISO_PRE_CHARGE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PreChargeReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PRE_CHARGE_REQ) && (EXI_DECODE_ISO_PRE_CHARGE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_PreChargeRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_PreChargeResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PRE_CHARGE_RES) && (EXI_DECODE_ISO_PRE_CHARGE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_PreChargeRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PRE_CHARGE_RES) && (EXI_DECODE_ISO_PRE_CHARGE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ProfileEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ProfileEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PROFILE_ENTRY) && (EXI_DECODE_ISO_PROFILE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ProfileEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PROFILE_ENTRY) && (EXI_DECODE_ISO_PROFILE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_RelativeTimeInterval
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_RelativeTimeIntervalType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_RelativeTimeInterval( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_DECODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SAScheduleList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SAScheduleListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SASCHEDULE_LIST) && (EXI_DECODE_ISO_SASCHEDULE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SAScheduleList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SASCHEDULE_LIST) && (EXI_DECODE_ISO_SASCHEDULE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SAScheduleTuple
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SAScheduleTupleType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SASCHEDULE_TUPLE) && (EXI_DECODE_ISO_SASCHEDULE_TUPLE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SAScheduleTuple( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SASCHEDULE_TUPLE) && (EXI_DECODE_ISO_SASCHEDULE_TUPLE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SalesTariffEntry
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SalesTariffEntryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SALES_TARIFF_ENTRY) && (EXI_DECODE_ISO_SALES_TARIFF_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SalesTariffEntry( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SALES_TARIFF_ENTRY) && (EXI_DECODE_ISO_SALES_TARIFF_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SalesTariff
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SalesTariffType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SALES_TARIFF) && (EXI_DECODE_ISO_SALES_TARIFF == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SalesTariff( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SALES_TARIFF) && (EXI_DECODE_ISO_SALES_TARIFF == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SelectedServiceList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SelectedServiceListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SELECTED_SERVICE_LIST) && (EXI_DECODE_ISO_SELECTED_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SelectedServiceList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SELECTED_SERVICE_LIST) && (EXI_DECODE_ISO_SELECTED_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SelectedService
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SelectedServiceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SELECTED_SERVICE) && (EXI_DECODE_ISO_SELECTED_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SelectedService( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SELECTED_SERVICE) && (EXI_DECODE_ISO_SELECTED_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceDetailReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceDetailReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_DETAIL_REQ) && (EXI_DECODE_ISO_SERVICE_DETAIL_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceDetailReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_DETAIL_REQ) && (EXI_DECODE_ISO_SERVICE_DETAIL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceDetailRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceDetailResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_DETAIL_RES) && (EXI_DECODE_ISO_SERVICE_DETAIL_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceDetailRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_DETAIL_RES) && (EXI_DECODE_ISO_SERVICE_DETAIL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceDiscoveryReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceDiscoveryReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_DECODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceDiscoveryResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_DECODE_ISO_SERVICE_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceDiscoveryRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_DECODE_ISO_SERVICE_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_LIST) && (EXI_DECODE_ISO_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_LIST) && (EXI_DECODE_ISO_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_ServiceParameterList
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceParameterListType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_DECODE_ISO_SERVICE_PARAMETER_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_ServiceParameterList( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_DECODE_ISO_SERVICE_PARAMETER_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_Service
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_ServiceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE) && (EXI_DECODE_ISO_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_Service( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE) && (EXI_DECODE_ISO_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SessionSetupReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SessionSetupReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SESSION_SETUP_REQ) && (EXI_DECODE_ISO_SESSION_SETUP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SessionSetupReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SESSION_SETUP_REQ) && (EXI_DECODE_ISO_SESSION_SETUP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SessionSetupRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SessionSetupResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SESSION_SETUP_RES) && (EXI_DECODE_ISO_SESSION_SETUP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SessionSetupRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SESSION_SETUP_RES) && (EXI_DECODE_ISO_SESSION_SETUP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SessionStopReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SessionStopReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SESSION_STOP_REQ) && (EXI_DECODE_ISO_SESSION_STOP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SessionStopReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SESSION_STOP_REQ) && (EXI_DECODE_ISO_SESSION_STOP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SessionStopRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SessionStopResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SESSION_STOP_RES) && (EXI_DECODE_ISO_SESSION_STOP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SessionStopRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SESSION_STOP_RES) && (EXI_DECODE_ISO_SESSION_STOP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SubCertificates
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SubCertificatesType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SUB_CERTIFICATES) && (EXI_DECODE_ISO_SUB_CERTIFICATES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SubCertificates( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SUB_CERTIFICATES) && (EXI_DECODE_ISO_SUB_CERTIFICATES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_SupportedEnergyTransferMode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_SupportedEnergyTransferModeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_SupportedEnergyTransferMode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_DECODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_V2G_Message
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_V2G_MessageType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_V2G_MESSAGE) && (EXI_DECODE_ISO_V2G_MESSAGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_V2G_Message( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_V2G_MESSAGE) && (EXI_DECODE_ISO_V2G_MESSAGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_WeldingDetectionReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_WeldingDetectionReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_WELDING_DETECTION_REQ) && (EXI_DECODE_ISO_WELDING_DETECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_WeldingDetectionReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_WELDING_DETECTION_REQ) && (EXI_DECODE_ISO_WELDING_DETECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_WeldingDetectionRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_WeldingDetectionResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_WELDING_DETECTION_RES) && (EXI_DECODE_ISO_WELDING_DETECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_WeldingDetectionRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_WELDING_DETECTION_RES) && (EXI_DECODE_ISO_WELDING_DETECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_certificate
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_certificateType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CERTIFICATE) && (EXI_DECODE_ISO_CERTIFICATE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_certificate( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CERTIFICATE) && (EXI_DECODE_ISO_CERTIFICATE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_chargeProgress
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_chargeProgressType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGE_PROGRESS) && (EXI_DECODE_ISO_CHARGE_PROGRESS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_chargeProgress( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGE_PROGRESS) && (EXI_DECODE_ISO_CHARGE_PROGRESS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_chargingSession
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_chargingSessionType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_CHARGING_SESSION) && (EXI_DECODE_ISO_CHARGING_SESSION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_chargingSession( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_CHARGING_SESSION) && (EXI_DECODE_ISO_CHARGING_SESSION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_costKind
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_costKindType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_COST_KIND) && (EXI_DECODE_ISO_COST_KIND == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_costKind( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_COST_KIND) && (EXI_DECODE_ISO_COST_KIND == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_eMAID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_eMAIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_E_MAID) && (EXI_DECODE_ISO_E_MAID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_eMAID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_E_MAID) && (EXI_DECODE_ISO_E_MAID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_evccID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_evccIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVCC_ID) && (EXI_DECODE_ISO_EVCC_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_evccID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVCC_ID) && (EXI_DECODE_ISO_EVCC_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_evseID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_evseIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_EVSE_ID) && (EXI_DECODE_ISO_EVSE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_evseID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_EVSE_ID) && (EXI_DECODE_ISO_EVSE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_faultCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_faultCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_FAULT_CODE) && (EXI_DECODE_ISO_FAULT_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_faultCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_FAULT_CODE) && (EXI_DECODE_ISO_FAULT_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_faultMsg
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_faultMsgType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_FAULT_MSG) && (EXI_DECODE_ISO_FAULT_MSG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_faultMsg( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_FAULT_MSG) && (EXI_DECODE_ISO_FAULT_MSG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_genChallenge
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_genChallengeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_GEN_CHALLENGE) && (EXI_DECODE_ISO_GEN_CHALLENGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_genChallenge( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_GEN_CHALLENGE) && (EXI_DECODE_ISO_GEN_CHALLENGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_isolationLevel
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_isolationLevelType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_ISOLATION_LEVEL) && (EXI_DECODE_ISO_ISOLATION_LEVEL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_isolationLevel( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_ISOLATION_LEVEL) && (EXI_DECODE_ISO_ISOLATION_LEVEL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_meterID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_meterIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_METER_ID) && (EXI_DECODE_ISO_METER_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_meterID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_METER_ID) && (EXI_DECODE_ISO_METER_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_paymentOption
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_paymentOptionType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_PAYMENT_OPTION) && (EXI_DECODE_ISO_PAYMENT_OPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_paymentOption( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_PAYMENT_OPTION) && (EXI_DECODE_ISO_PAYMENT_OPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_responseCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_responseCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_RESPONSE_CODE) && (EXI_DECODE_ISO_RESPONSE_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_responseCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_RESPONSE_CODE) && (EXI_DECODE_ISO_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_serviceCategory
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_serviceCategoryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_CATEGORY) && (EXI_DECODE_ISO_SERVICE_CATEGORY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_serviceCategory( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_CATEGORY) && (EXI_DECODE_ISO_SERVICE_CATEGORY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_serviceName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_serviceNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_NAME) && (EXI_DECODE_ISO_SERVICE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_serviceName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_NAME) && (EXI_DECODE_ISO_SERVICE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_serviceScope
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_serviceScopeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SERVICE_SCOPE) && (EXI_DECODE_ISO_SERVICE_SCOPE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_serviceScope( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SERVICE_SCOPE) && (EXI_DECODE_ISO_SERVICE_SCOPE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_sessionID
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_sessionIDType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SESSION_ID) && (EXI_DECODE_ISO_SESSION_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_sessionID( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SESSION_ID) && (EXI_DECODE_ISO_SESSION_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_sigMeterReading
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_sigMeterReadingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_SIG_METER_READING) && (EXI_DECODE_ISO_SIG_METER_READING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_sigMeterReading( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_SIG_METER_READING) && (EXI_DECODE_ISO_SIG_METER_READING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_tariffDescription
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_tariffDescriptionType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_TARIFF_DESCRIPTION) && (EXI_DECODE_ISO_TARIFF_DESCRIPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_tariffDescription( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_TARIFF_DESCRIPTION) && (EXI_DECODE_ISO_TARIFF_DESCRIPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_ISO_unitSymbol
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_ISO_unitSymbolType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_ISO_UNIT_SYMBOL) && (EXI_DECODE_ISO_UNIT_SYMBOL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_ISO_unitSymbol( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_ISO_UNIT_SYMBOL) && (EXI_DECODE_ISO_UNIT_SYMBOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_ISO
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_ISO object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_ISO) && (EXI_DECODE_SCHEMA_SET_ISO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_ISO( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SCHEMA_SET_ISO) && (EXI_DECODE_SCHEMA_SET_ISO == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined (EXI_ENABLE_DECODE_ISO_MESSAGE_SET) && (EXI_ENABLE_DECODE_ISO_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_ISO_SCHEMA_DECODER_H */
