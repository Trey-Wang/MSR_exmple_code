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
 *         File:  Exi_ISO_SchemaEncoder.h
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


#if !defined (EXI_ISO_SCHEMA_ENCODER_H)
# define EXI_ISO_SCHEMA_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaEncoder.h"

#if (!defined (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET))
# if (defined (EXI_ENABLE_ISO_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_ISO_MESSAGE_SET   EXI_ENABLE_ISO_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_ISO_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_ENCODE_ISO_MESSAGE_SET) && (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER 0x98U
#define EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER 0x99U
#define EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS 0x9AU
#define EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID 0x9BU
#define EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME 0x9CU
#define EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ 0x9DU
#define EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES 0x9EU
#define EXI_API_ID_ENCODE_ISO_BODY 0x9FU
#define EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ 0xA0U
#define EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES 0xA1U
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN 0xA2U
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ 0xA3U
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES 0xA4U
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ 0xA5U
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES 0xA6U
#define EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ 0xA7U
#define EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES 0xA8U
#define EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE 0xA9U
#define EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE 0xAAU
#define EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES 0xABU
#define EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST 0xACU
#define EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY 0xADU
#define EXI_API_ID_ENCODE_ISO_COST 0xAEU
#define EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ 0xAFU
#define EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES 0xB0U
#define EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER 0xB1U
#define EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE 0xB2U
#define EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER 0xB3U
#define EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER 0xB4U
#define EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE 0xB5U
#define EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS 0xB6U
#define EXI_API_ID_ENCODE_ISO_DC_EVSTATUS 0xB7U
#define EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY 0xB8U
#define EXI_API_ID_ENCODE_ISO_EMAID 0xB9U
#define EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT 0xBAU
#define EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER 0xBBU
#define EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION 0xBCU
#define EXI_API_ID_ENCODE_ISO_EVSEPROCESSING 0xBDU
#define EXI_API_ID_ENCODE_ISO_EVSESTATUS 0xBEU
#define EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE 0xBFU
#define EXI_API_ID_ENCODE_ISO_ENTRY 0xC0U
#define EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS 0xC1U
#define EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER 0xC2U
#define EXI_API_ID_ENCODE_ISO_METER_INFO 0xC3U
#define EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ 0xC4U
#define EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES 0xC5U
#define EXI_API_ID_ENCODE_ISO_NOTIFICATION 0xC6U
#define EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY 0xC7U
#define EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE 0xC8U
#define EXI_API_ID_ENCODE_ISO_PARAMETER_SET 0xC9U
#define EXI_API_ID_ENCODE_ISO_PARAMETER 0xCAU
#define EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ 0xCBU
#define EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES 0xCCU
#define EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST 0xCDU
#define EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ 0xCEU
#define EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES 0xCFU
#define EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE 0xD0U
#define EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ 0xD1U
#define EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES 0xD2U
#define EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ 0xD3U
#define EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES 0xD4U
#define EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY 0xD5U
#define EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL 0xD6U
#define EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST 0xD7U
#define EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE 0xD8U
#define EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY 0xD9U
#define EXI_API_ID_ENCODE_ISO_SALES_TARIFF 0xDAU
#define EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST 0xDBU
#define EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE 0xDCU
#define EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ 0xDDU
#define EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES 0xDEU
#define EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ 0xDFU
#define EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES 0xE0U
#define EXI_API_ID_ENCODE_ISO_SERVICE_LIST 0xE1U
#define EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST 0xE2U
#define EXI_API_ID_ENCODE_ISO_SERVICE 0xE3U
#define EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ 0xE4U
#define EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES 0xE5U
#define EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ 0xE6U
#define EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES 0xE7U
#define EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES 0xE8U
#define EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE 0xE9U
#define EXI_API_ID_ENCODE_ISO_V2G_MESSAGE 0xEAU
#define EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ 0xEBU
#define EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES 0xECU
#define EXI_API_ID_ENCODE_ISO_CERTIFICATE 0xEDU
#define EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS 0xEEU
#define EXI_API_ID_ENCODE_ISO_CHARGING_SESSION 0xEFU
#define EXI_API_ID_ENCODE_ISO_COST_KIND 0xF0U
#define EXI_API_ID_ENCODE_ISO_E_MAID 0xF1U
#define EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT 0xF2U
#define EXI_API_ID_ENCODE_ISO_EVCC_ID 0xF3U
#define EXI_API_ID_ENCODE_ISO_EVSE_ID 0xF4U
#define EXI_API_ID_ENCODE_ISO_FAULT_CODE 0xF5U
#define EXI_API_ID_ENCODE_ISO_FAULT_MSG 0xF6U
#define EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE 0xF7U
#define EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL 0xF8U
#define EXI_API_ID_ENCODE_ISO_METER_ID 0xF9U
#define EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION 0xFAU
#define EXI_API_ID_ENCODE_ISO_RESPONSE_CODE 0xFBU
#define EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY 0xFCU
#define EXI_API_ID_ENCODE_ISO_SERVICE_NAME 0xFDU
#define EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE 0xFEU
#define EXI_API_ID_ENCODE_ISO_SESSION_ID 0xFFU
#define EXI_API_ID_ENCODE_ISO_SIG_METER_READING 0x100U
#define EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION 0x101U
#define EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL 0x102U
#define EXI_API_ID_ENCODE_ISO_SCHEMA_FRAGMENT 0x17DU
#define EXI_API_ID_ENCODE_ISO_SCHEMA_ROOT 0x17EU
#endif

/* Encoding default switches */
#ifndef EXI_ENCODE_ISO_SCHEMA_FRAGMENT
#define EXI_ENCODE_ISO_SCHEMA_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SCHEMA_ROOT
#define EXI_ENCODE_ISO_SCHEMA_ROOT STD_ON
#endif
#ifndef EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER
#define EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER
#define EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_AC_EVSESTATUS
#define EXI_ENCODE_ISO_AC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_ATTRIBUTE_ID
#define EXI_ENCODE_ISO_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_ATTRIBUTE_NAME
#define EXI_ENCODE_ISO_ATTRIBUTE_NAME STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_AUTHORIZATION_REQ
#define EXI_ENCODE_ISO_AUTHORIZATION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_AUTHORIZATION_RES
#define EXI_ENCODE_ISO_AUTHORIZATION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_BODY_BASE
#define EXI_ENCODE_ISO_BODY_BASE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_BODY
#define EXI_ENCODE_ISO_BODY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CABLE_CHECK_REQ
#define EXI_ENCODE_ISO_CABLE_CHECK_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CABLE_CHECK_RES
#define EXI_ENCODE_ISO_CABLE_CHECK_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE_CHAIN
#define EXI_ENCODE_ISO_CERTIFICATE_CHAIN STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ
#define EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES
#define EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ
#define EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES
#define EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ
#define EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES
#define EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGE_SERVICE
#define EXI_ENCODE_ISO_CHARGE_SERVICE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGING_PROFILE
#define EXI_ENCODE_ISO_CHARGING_PROFILE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGING_STATUS_REQ
#define EXI_ENCODE_ISO_CHARGING_STATUS_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGING_STATUS_RES
#define EXI_ENCODE_ISO_CHARGING_STATUS_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CONSUMPTION_COST
#define EXI_ENCODE_ISO_CONSUMPTION_COST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY
#define EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_COST
#define EXI_ENCODE_ISO_COST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CURRENT_DEMAND_REQ
#define EXI_ENCODE_ISO_CURRENT_DEMAND_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CURRENT_DEMAND_RES
#define EXI_ENCODE_ISO_CURRENT_DEMAND_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER
#define EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVERROR_CODE
#define EXI_ENCODE_ISO_DC_EVERROR_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER
#define EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER
#define EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVSESTATUS_CODE
#define EXI_ENCODE_ISO_DC_EVSESTATUS_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVSESTATUS
#define EXI_ENCODE_ISO_DC_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DC_EVSTATUS
#define EXI_ENCODE_ISO_DC_EVSTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY
#define EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EMAID
#define EXI_ENCODE_ISO_EMAID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT
#define EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVCHARGE_PARAMETER
#define EXI_ENCODE_ISO_EVCHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER
#define EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSECHARGE_PARAMETER
#define EXI_ENCODE_ISO_EVSECHARGE_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSENOTIFICATION
#define EXI_ENCODE_ISO_EVSENOTIFICATION STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSEPROCESSING
#define EXI_ENCODE_ISO_EVSEPROCESSING STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSESTATUS
#define EXI_ENCODE_ISO_EVSESTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSTATUS
#define EXI_ENCODE_ISO_EVSTATUS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE
#define EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_ENTRY
#define EXI_ENCODE_ISO_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_INTERVAL
#define EXI_ENCODE_ISO_INTERVAL STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS
#define EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_MESSAGE_HEADER
#define EXI_ENCODE_ISO_MESSAGE_HEADER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_METER_INFO
#define EXI_ENCODE_ISO_METER_INFO STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_METERING_RECEIPT_REQ
#define EXI_ENCODE_ISO_METERING_RECEIPT_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_METERING_RECEIPT_RES
#define EXI_ENCODE_ISO_METERING_RECEIPT_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_NOTIFICATION
#define EXI_ENCODE_ISO_NOTIFICATION STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY
#define EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PMAX_SCHEDULE
#define EXI_ENCODE_ISO_PMAX_SCHEDULE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PARAMETER_SET
#define EXI_ENCODE_ISO_PARAMETER_SET STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PARAMETER
#define EXI_ENCODE_ISO_PARAMETER STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ
#define EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_DETAILS_RES
#define EXI_ENCODE_ISO_PAYMENT_DETAILS_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_OPTION_LIST
#define EXI_ENCODE_ISO_PAYMENT_OPTION_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ
#define EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES
#define EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PHYSICAL_VALUE
#define EXI_ENCODE_ISO_PHYSICAL_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_POWER_DELIVERY_REQ
#define EXI_ENCODE_ISO_POWER_DELIVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_POWER_DELIVERY_RES
#define EXI_ENCODE_ISO_POWER_DELIVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PRE_CHARGE_REQ
#define EXI_ENCODE_ISO_PRE_CHARGE_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PRE_CHARGE_RES
#define EXI_ENCODE_ISO_PRE_CHARGE_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PROFILE_ENTRY
#define EXI_ENCODE_ISO_PROFILE_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL
#define EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SASCHEDULE_LIST
#define EXI_ENCODE_ISO_SASCHEDULE_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SASCHEDULE_TUPLE
#define EXI_ENCODE_ISO_SASCHEDULE_TUPLE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SASCHEDULES
#define EXI_ENCODE_ISO_SASCHEDULES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SALES_TARIFF_ENTRY
#define EXI_ENCODE_ISO_SALES_TARIFF_ENTRY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SALES_TARIFF
#define EXI_ENCODE_ISO_SALES_TARIFF STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SELECTED_SERVICE_LIST
#define EXI_ENCODE_ISO_SELECTED_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SELECTED_SERVICE
#define EXI_ENCODE_ISO_SELECTED_SERVICE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_DETAIL_REQ
#define EXI_ENCODE_ISO_SERVICE_DETAIL_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_DETAIL_RES
#define EXI_ENCODE_ISO_SERVICE_DETAIL_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ
#define EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES
#define EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_LIST
#define EXI_ENCODE_ISO_SERVICE_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST
#define EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE
#define EXI_ENCODE_ISO_SERVICE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SESSION_SETUP_REQ
#define EXI_ENCODE_ISO_SESSION_SETUP_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SESSION_SETUP_RES
#define EXI_ENCODE_ISO_SESSION_SETUP_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SESSION_STOP_REQ
#define EXI_ENCODE_ISO_SESSION_STOP_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SESSION_STOP_RES
#define EXI_ENCODE_ISO_SESSION_STOP_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SUB_CERTIFICATES
#define EXI_ENCODE_ISO_SUB_CERTIFICATES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE
#define EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_V2G_MESSAGE
#define EXI_ENCODE_ISO_V2G_MESSAGE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_WELDING_DETECTION_REQ
#define EXI_ENCODE_ISO_WELDING_DETECTION_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_WELDING_DETECTION_RES
#define EXI_ENCODE_ISO_WELDING_DETECTION_RES STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CERTIFICATE
#define EXI_ENCODE_ISO_CERTIFICATE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGE_PROGRESS
#define EXI_ENCODE_ISO_CHARGE_PROGRESS STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_CHARGING_SESSION
#define EXI_ENCODE_ISO_CHARGING_SESSION STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_COST_KIND
#define EXI_ENCODE_ISO_COST_KIND STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_E_MAID
#define EXI_ENCODE_ISO_E_MAID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT
#define EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVCC_ID
#define EXI_ENCODE_ISO_EVCC_ID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_EVSE_ID
#define EXI_ENCODE_ISO_EVSE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_FAULT_CODE
#define EXI_ENCODE_ISO_FAULT_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_FAULT_MSG
#define EXI_ENCODE_ISO_FAULT_MSG STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_GEN_CHALLENGE
#define EXI_ENCODE_ISO_GEN_CHALLENGE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_ISOLATION_LEVEL
#define EXI_ENCODE_ISO_ISOLATION_LEVEL STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_METER_ID
#define EXI_ENCODE_ISO_METER_ID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_PAYMENT_OPTION
#define EXI_ENCODE_ISO_PAYMENT_OPTION STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_RESPONSE_CODE
#define EXI_ENCODE_ISO_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_CATEGORY
#define EXI_ENCODE_ISO_SERVICE_CATEGORY STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_NAME
#define EXI_ENCODE_ISO_SERVICE_NAME STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SERVICE_SCOPE
#define EXI_ENCODE_ISO_SERVICE_SCOPE STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SESSION_ID
#define EXI_ENCODE_ISO_SESSION_ID STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_SIG_METER_READING
#define EXI_ENCODE_ISO_SIG_METER_READING STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_TARIFF_DESCRIPTION
#define EXI_ENCODE_ISO_TARIFF_DESCRIPTION STD_OFF
#endif
#ifndef EXI_ENCODE_ISO_UNIT_SYMBOL
#define EXI_ENCODE_ISO_UNIT_SYMBOL STD_OFF
#endif


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Encode_ISO_AC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AC_EVChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVChargeParameterPtr     pointer to Exi_ISO_AC_EVChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AC_EVSEChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVSEChargeParameterPtr   pointer to Exi_ISO_AC_EVSEChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AC_EVSEStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AC_EVSEStatusPtr            pointer to Exi_ISO_AC_EVSEStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEStatusPtr );
#endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AttributeId
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AttributeIdType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeIdPtr              pointer to Exi_ISO_AttributeIdType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AttributeId( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AttributeIdType, AUTOMATIC, EXI_APPL_DATA) AttributeIdPtr );
#endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AttributeName
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AttributeNameType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeNamePtr            pointer to Exi_ISO_AttributeNameType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_ATTRIBUTE_NAME) && (EXI_ENCODE_ISO_ATTRIBUTE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AttributeName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AttributeNameType, AUTOMATIC, EXI_APPL_DATA) AttributeNamePtr );
#endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_NAME) && (EXI_ENCODE_ISO_ATTRIBUTE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AuthorizationReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AuthorizationReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AuthorizationReqPtr         pointer to Exi_ISO_AuthorizationReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AuthorizationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AuthorizationReqType, AUTOMATIC, EXI_APPL_DATA) AuthorizationReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_AuthorizationRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_AuthorizationResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AuthorizationResPtr         pointer to Exi_ISO_AuthorizationResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_AuthorizationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AuthorizationResType, AUTOMATIC, EXI_APPL_DATA) AuthorizationResPtr );
#endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Body
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_BodyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     BodyPtr                     pointer to Exi_ISO_BodyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Body( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_BodyType, AUTOMATIC, EXI_APPL_DATA) BodyPtr );
#endif /* (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CableCheckReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CableCheckReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CableCheckReqPtr            pointer to Exi_ISO_CableCheckReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CableCheckReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CableCheckReqType, AUTOMATIC, EXI_APPL_DATA) CableCheckReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CableCheckRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CableCheckResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CableCheckResPtr            pointer to Exi_ISO_CableCheckResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CableCheckRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CableCheckResType, AUTOMATIC, EXI_APPL_DATA) CableCheckResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CertificateChain
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CertificateChainType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateChainPtr         pointer to Exi_ISO_CertificateChainType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateChain( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateChainType, AUTOMATIC, EXI_APPL_DATA) CertificateChainPtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CertificateInstallationReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CertificateInstallationReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateInstallationReqPtrpointer to Exi_ISO_CertificateInstallationReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateInstallationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateInstallationReqType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CertificateInstallationRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CertificateInstallationResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateInstallationResPtrpointer to Exi_ISO_CertificateInstallationResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateInstallationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateInstallationResType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CertificateUpdateReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CertificateUpdateReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateUpdateReqPtr     pointer to Exi_ISO_CertificateUpdateReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateUpdateReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateUpdateReqType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CertificateUpdateRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CertificateUpdateResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CertificateUpdateResPtr     pointer to Exi_ISO_CertificateUpdateResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateUpdateRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateUpdateResType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ChargeParameterDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ChargeParameterDiscoveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargeParameterDiscoveryReqPtrpointer to Exi_ISO_ChargeParameterDiscoveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeParameterDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeParameterDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ChargeParameterDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ChargeParameterDiscoveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargeParameterDiscoveryResPtrpointer to Exi_ISO_ChargeParameterDiscoveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeParameterDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeParameterDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ChargeService
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ChargeServiceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargeServicePtr            pointer to Exi_ISO_ChargeServiceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeService( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeServiceType, AUTOMATIC, EXI_APPL_DATA) ChargeServicePtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ChargingProfile
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ChargingProfileType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargingProfilePtr          pointer to Exi_ISO_ChargingProfileType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargingProfile( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargingProfileType, AUTOMATIC, EXI_APPL_DATA) ChargingProfilePtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ChargingStatusRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ChargingStatusResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ChargingStatusResPtr        pointer to Exi_ISO_ChargingStatusResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargingStatusRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargingStatusResType, AUTOMATIC, EXI_APPL_DATA) ChargingStatusResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ConsumptionCost
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ConsumptionCostType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ConsumptionCostPtr          pointer to Exi_ISO_ConsumptionCostType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ConsumptionCost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ConsumptionCostType, AUTOMATIC, EXI_APPL_DATA) ConsumptionCostPtr );
#endif /* (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ContractSignatureEncryptedPrivateKeyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ContractSignatureEncryptedPrivateKeyPtrpointer to Exi_ISO_ContractSignatureEncryptedPrivateKeyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ContractSignatureEncryptedPrivateKeyType, AUTOMATIC, EXI_APPL_DATA) ContractSignatureEncryptedPrivateKeyPtr );
#endif /* (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Cost
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CostType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CostPtr                     pointer to Exi_ISO_CostType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Cost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CostType, AUTOMATIC, EXI_APPL_DATA) CostPtr );
#endif /* (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CurrentDemandReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CurrentDemandReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CurrentDemandReqPtr         pointer to Exi_ISO_CurrentDemandReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CurrentDemandReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CurrentDemandReqType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_CurrentDemandRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_CurrentDemandResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CurrentDemandResPtr         pointer to Exi_ISO_CurrentDemandResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_CurrentDemandRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CurrentDemandResType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandResPtr );
#endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVChargeParameterPtr     pointer to Exi_ISO_DC_EVChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVErrorCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVErrorCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVErrorCodePtr           pointer to Exi_ISO_DC_EVErrorCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVErrorCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVErrorCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVErrorCodePtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVPowerDeliveryParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVPowerDeliveryParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVPowerDeliveryParameterPtrpointer to Exi_ISO_DC_EVPowerDeliveryParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVPowerDeliveryParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVPowerDeliveryParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVPowerDeliveryParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVSEChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVSEChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEChargeParameterPtr   pointer to Exi_ISO_DC_EVSEChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVSEStatusCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVSEStatusCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEStatusCodePtr        pointer to Exi_ISO_DC_EVSEStatusCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEStatusCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEStatusCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusCodePtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVSEStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVSEStatusPtr            pointer to Exi_ISO_DC_EVSEStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusPtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DC_EVStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DC_EVStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DC_EVStatusPtr              pointer to Exi_ISO_DC_EVStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVStatusPtr );
#endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_DiffieHellmanPublickey
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_DiffieHellmanPublickeyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DiffieHellmanPublickeyPtr   pointer to Exi_ISO_DiffieHellmanPublickeyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_DiffieHellmanPublickey( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DiffieHellmanPublickeyType, AUTOMATIC, EXI_APPL_DATA) DiffieHellmanPublickeyPtr );
#endif /* (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EMAID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EMAIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EMAIDPtr                    pointer to Exi_ISO_EMAIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EMAID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EMAIDType, AUTOMATIC, EXI_APPL_DATA) EMAIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EMAID_ElementFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EMAIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EMAIDPtr                    pointer to Exi_ISO_EMAIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EMAID_ElementFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EMAIDType, AUTOMATIC, EXI_APPL_DATA) EMAIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EVChargeParameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EVChargeParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVChargeParameterPtr        pointer to Exi_ISO_EVChargeParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) EVChargeParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EVSENotification
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EVSENotificationType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSENotificationPtr         pointer to Exi_ISO_EVSENotificationType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSENotification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSENotificationType, AUTOMATIC, EXI_APPL_DATA) EVSENotificationPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EVSEProcessing
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EVSEProcessingType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSEProcessingPtr           pointer to Exi_ISO_EVSEProcessingType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSEProcessing( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSEProcessingType, AUTOMATIC, EXI_APPL_DATA) EVSEProcessingPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EVSEStatus
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EVSEStatusType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EVSEStatusPtr               pointer to Exi_ISO_EVSEStatusType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) EVSEStatusPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_EnergyTransferMode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EnergyTransferModeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EnergyTransferModePtr       pointer to Exi_ISO_EnergyTransferModeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_EnergyTransferMode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EnergyTransferModeType, AUTOMATIC, EXI_APPL_DATA) EnergyTransferModePtr );
#endif /* (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Entry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_EntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     EntryPtr                    pointer to Exi_ISO_EntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_ENTRY) && (EXI_ENCODE_ISO_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Entry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EntryType, AUTOMATIC, EXI_APPL_DATA) EntryPtr );
#endif /* (defined(EXI_ENCODE_ISO_ENTRY) && (EXI_ENCODE_ISO_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ListOfRootCertificateIDs
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ListOfRootCertificateIDsType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ListOfRootCertificateIDsPtr pointer to Exi_ISO_ListOfRootCertificateIDsType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ListOfRootCertificateIDs( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ListOfRootCertificateIDsType, AUTOMATIC, EXI_APPL_DATA) ListOfRootCertificateIDsPtr );
#endif /* (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_MessageHeader
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_MessageHeaderType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MessageHeaderPtr            pointer to Exi_ISO_MessageHeaderType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_MessageHeader( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MessageHeaderType, AUTOMATIC, EXI_APPL_DATA) MessageHeaderPtr );
#endif /* (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_MeterInfo
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_MeterInfoType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeterInfoPtr                pointer to Exi_ISO_MeterInfoType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_MeterInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeterInfoType, AUTOMATIC, EXI_APPL_DATA) MeterInfoPtr );
#endif /* (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_MeteringReceiptReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_MeteringReceiptReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeteringReceiptReqPtr       pointer to Exi_ISO_MeteringReceiptReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_MeteringReceiptReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeteringReceiptReqType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_MeteringReceiptRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_MeteringReceiptResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MeteringReceiptResPtr       pointer to Exi_ISO_MeteringReceiptResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_MeteringReceiptRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeteringReceiptResType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptResPtr );
#endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Notification
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_NotificationType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     NotificationPtr             pointer to Exi_ISO_NotificationType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Notification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_NotificationType, AUTOMATIC, EXI_APPL_DATA) NotificationPtr );
#endif /* (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PMaxScheduleEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PMaxScheduleEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PMaxScheduleEntryPtr        pointer to Exi_ISO_PMaxScheduleEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PMaxScheduleEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PMaxScheduleEntryType, AUTOMATIC, EXI_APPL_DATA) PMaxScheduleEntryPtr );
#endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PMaxSchedule
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PMaxScheduleType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PMaxSchedulePtr             pointer to Exi_ISO_PMaxScheduleType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PMaxSchedule( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PMaxScheduleType, AUTOMATIC, EXI_APPL_DATA) PMaxSchedulePtr );
#endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ParameterSet
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ParameterSetType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ParameterSetPtr             pointer to Exi_ISO_ParameterSetType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ParameterSet( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ParameterSetType, AUTOMATIC, EXI_APPL_DATA) ParameterSetPtr );
#endif /* (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Parameter
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ParameterType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ParameterPtr                pointer to Exi_ISO_ParameterType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Parameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ParameterType, AUTOMATIC, EXI_APPL_DATA) ParameterPtr );
#endif /* (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PaymentDetailsReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PaymentDetailsReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentDetailsReqPtr        pointer to Exi_ISO_PaymentDetailsReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentDetailsReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentDetailsReqType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PaymentDetailsRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PaymentDetailsResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentDetailsResPtr        pointer to Exi_ISO_PaymentDetailsResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentDetailsRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentDetailsResType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsResPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PaymentOptionList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PaymentOptionListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentOptionListPtr        pointer to Exi_ISO_PaymentOptionListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentOptionList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentOptionListType, AUTOMATIC, EXI_APPL_DATA) PaymentOptionListPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PaymentServiceSelectionReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PaymentServiceSelectionReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentServiceSelectionReqPtrpointer to Exi_ISO_PaymentServiceSelectionReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentServiceSelectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentServiceSelectionReqType, AUTOMATIC, EXI_APPL_DATA) PaymentServiceSelectionReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PaymentServiceSelectionRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PaymentServiceSelectionResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PaymentServiceSelectionResPtrpointer to Exi_ISO_PaymentServiceSelectionResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentServiceSelectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentServiceSelectionResType, AUTOMATIC, EXI_APPL_DATA) PaymentServiceSelectionResPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PhysicalValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PhysicalValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PhysicalValuePtr            pointer to Exi_ISO_PhysicalValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PhysicalValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, EXI_APPL_DATA) PhysicalValuePtr );
#endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PowerDeliveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PowerDeliveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PowerDeliveryReqPtr         pointer to Exi_ISO_PowerDeliveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PowerDeliveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PowerDeliveryReqType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PowerDeliveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PowerDeliveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PowerDeliveryResPtr         pointer to Exi_ISO_PowerDeliveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PowerDeliveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PowerDeliveryResType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryResPtr );
#endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PreChargeReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PreChargeReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PreChargeReqPtr             pointer to Exi_ISO_PreChargeReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PreChargeReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PreChargeReqType, AUTOMATIC, EXI_APPL_DATA) PreChargeReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_PreChargeRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_PreChargeResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PreChargeResPtr             pointer to Exi_ISO_PreChargeResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_PreChargeRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PreChargeResType, AUTOMATIC, EXI_APPL_DATA) PreChargeResPtr );
#endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ProfileEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ProfileEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ProfileEntryPtr             pointer to Exi_ISO_ProfileEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ProfileEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ProfileEntryType, AUTOMATIC, EXI_APPL_DATA) ProfileEntryPtr );
#endif /* (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_RelativeTimeInterval
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_RelativeTimeIntervalType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     RelativeTimeIntervalPtr     pointer to Exi_ISO_RelativeTimeIntervalType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_RelativeTimeInterval( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_RelativeTimeIntervalType, AUTOMATIC, EXI_APPL_DATA) RelativeTimeIntervalPtr );
#endif /* (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SAScheduleList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SAScheduleListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SAScheduleListPtr           pointer to Exi_ISO_SAScheduleListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SAScheduleList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SAScheduleListType, AUTOMATIC, EXI_APPL_DATA) SAScheduleListPtr );
#endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SAScheduleTuple
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SAScheduleTupleType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SAScheduleTuplePtr          pointer to Exi_ISO_SAScheduleTupleType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SAScheduleTuple( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SAScheduleTupleType, AUTOMATIC, EXI_APPL_DATA) SAScheduleTuplePtr );
#endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SalesTariffEntry
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SalesTariffEntryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SalesTariffEntryPtr         pointer to Exi_ISO_SalesTariffEntryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SalesTariffEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SalesTariffEntryType, AUTOMATIC, EXI_APPL_DATA) SalesTariffEntryPtr );
#endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SalesTariff
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SalesTariffType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SalesTariffPtr              pointer to Exi_ISO_SalesTariffType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SalesTariff( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SalesTariffType, AUTOMATIC, EXI_APPL_DATA) SalesTariffPtr );
#endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SelectedServiceList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SelectedServiceListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SelectedServiceListPtr      pointer to Exi_ISO_SelectedServiceListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SelectedServiceList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SelectedServiceListType, AUTOMATIC, EXI_APPL_DATA) SelectedServiceListPtr );
#endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SelectedService
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SelectedServiceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SelectedServicePtr          pointer to Exi_ISO_SelectedServiceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SelectedService( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SelectedServiceType, AUTOMATIC, EXI_APPL_DATA) SelectedServicePtr );
#endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceDetailReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceDetailReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDetailReqPtr         pointer to Exi_ISO_ServiceDetailReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDetailReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDetailReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceDetailRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceDetailResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDetailResPtr         pointer to Exi_ISO_ServiceDetailResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDetailRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDetailResType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailResPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceDiscoveryReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceDiscoveryReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDiscoveryReqPtr      pointer to Exi_ISO_ServiceDiscoveryReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceDiscoveryRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceDiscoveryResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceDiscoveryResPtr      pointer to Exi_ISO_ServiceDiscoveryResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryResPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceListPtr              pointer to Exi_ISO_ServiceListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceListType, AUTOMATIC, EXI_APPL_DATA) ServiceListPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_ServiceParameterList
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceParameterListType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServiceParameterListPtr     pointer to Exi_ISO_ServiceParameterListType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceParameterList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceParameterListType, AUTOMATIC, EXI_APPL_DATA) ServiceParameterListPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_Service
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_ServiceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ServicePtr                  pointer to Exi_ISO_ServiceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_Service( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceType, AUTOMATIC, EXI_APPL_DATA) ServicePtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SessionSetupReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SessionSetupReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionSetupReqPtr          pointer to Exi_ISO_SessionSetupReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionSetupReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionSetupReqType, AUTOMATIC, EXI_APPL_DATA) SessionSetupReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SessionSetupRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SessionSetupResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionSetupResPtr          pointer to Exi_ISO_SessionSetupResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionSetupRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionSetupResType, AUTOMATIC, EXI_APPL_DATA) SessionSetupResPtr );
#endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SessionStopReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SessionStopReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionStopReqPtr           pointer to Exi_ISO_SessionStopReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionStopReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionStopReqType, AUTOMATIC, EXI_APPL_DATA) SessionStopReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SessionStopRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SessionStopResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SessionStopResPtr           pointer to Exi_ISO_SessionStopResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionStopRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionStopResType, AUTOMATIC, EXI_APPL_DATA) SessionStopResPtr );
#endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SubCertificates
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SubCertificatesType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SubCertificatesPtr          pointer to Exi_ISO_SubCertificatesType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SubCertificates( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SubCertificatesType, AUTOMATIC, EXI_APPL_DATA) SubCertificatesPtr );
#endif /* (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SupportedEnergyTransferMode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SupportedEnergyTransferModeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SupportedEnergyTransferModePtrpointer to Exi_ISO_SupportedEnergyTransferModeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SupportedEnergyTransferMode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SupportedEnergyTransferModeType, AUTOMATIC, EXI_APPL_DATA) SupportedEnergyTransferModePtr );
#endif /* (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_V2G_Message
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_V2G_MessageType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     V2G_MessagePtr              pointer to Exi_ISO_V2G_MessageType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_V2G_MESSAGE) && (EXI_ENCODE_ISO_V2G_MESSAGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_V2G_Message( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_V2G_MessageType, AUTOMATIC, EXI_APPL_DATA) V2G_MessagePtr );
#endif /* (defined(EXI_ENCODE_ISO_V2G_MESSAGE) && (EXI_ENCODE_ISO_V2G_MESSAGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_WeldingDetectionReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_WeldingDetectionReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     WeldingDetectionReqPtr      pointer to Exi_ISO_WeldingDetectionReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_WeldingDetectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_WeldingDetectionReqType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionReqPtr );
#endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_WeldingDetectionRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_WeldingDetectionResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     WeldingDetectionResPtr      pointer to Exi_ISO_WeldingDetectionResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_WeldingDetectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_WeldingDetectionResType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionResPtr );
#endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_certificate
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_certificateType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     certificatePtr              pointer to Exi_ISO_certificateType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_certificate( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_certificateType, AUTOMATIC, EXI_APPL_DATA) certificatePtr );
#endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_chargeProgress
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_chargeProgressType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     chargeProgressPtr           pointer to Exi_ISO_chargeProgressType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_chargeProgress( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_chargeProgressType, AUTOMATIC, EXI_APPL_DATA) chargeProgressPtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_chargingSession
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_chargingSessionType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     chargingSessionPtr          pointer to Exi_ISO_chargingSessionType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_chargingSession( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_chargingSessionType, AUTOMATIC, EXI_APPL_DATA) chargingSessionPtr );
#endif /* (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_costKind
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_costKindType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     costKindPtr                 pointer to Exi_ISO_costKindType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_costKind( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_costKindType, AUTOMATIC, EXI_APPL_DATA) costKindPtr );
#endif /* (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_eMAID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_eMAIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     eMAIDPtr                    pointer to Exi_ISO_eMAIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_eMAID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_eMAIDType, AUTOMATIC, EXI_APPL_DATA) eMAIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_eMAID_ElementFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_eMAIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     eMAIDPtr                    pointer to Exi_ISO_eMAIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_eMAID_ElementFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_eMAIDType, AUTOMATIC, EXI_APPL_DATA) eMAIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_evccID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_evccIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     evccIDPtr                   pointer to Exi_ISO_evccIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_evccID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_evccIDType, AUTOMATIC, EXI_APPL_DATA) evccIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_evseID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_evseIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     evseIDPtr                   pointer to Exi_ISO_evseIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_evseID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_evseIDType, AUTOMATIC, EXI_APPL_DATA) evseIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_faultCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_faultCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     faultCodePtr                pointer to Exi_ISO_faultCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_faultCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_faultCodeType, AUTOMATIC, EXI_APPL_DATA) faultCodePtr );
#endif /* (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_faultMsg
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_faultMsgType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     faultMsgPtr                 pointer to Exi_ISO_faultMsgType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_faultMsg( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_faultMsgType, AUTOMATIC, EXI_APPL_DATA) faultMsgPtr );
#endif /* (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_genChallenge
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_genChallengeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     genChallengePtr             pointer to Exi_ISO_genChallengeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_genChallenge( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_genChallengeType, AUTOMATIC, EXI_APPL_DATA) genChallengePtr );
#endif /* (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_isolationLevel
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_isolationLevelType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     isolationLevelPtr           pointer to Exi_ISO_isolationLevelType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_isolationLevel( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_isolationLevelType, AUTOMATIC, EXI_APPL_DATA) isolationLevelPtr );
#endif /* (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_meterID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_meterIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     meterIDPtr                  pointer to Exi_ISO_meterIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_meterID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_meterIDType, AUTOMATIC, EXI_APPL_DATA) meterIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_paymentOption
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_paymentOptionType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     paymentOptionPtr            pointer to Exi_ISO_paymentOptionType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_paymentOption( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_paymentOptionType, AUTOMATIC, EXI_APPL_DATA) paymentOptionPtr );
#endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_responseCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_responseCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     responseCodePtr             pointer to Exi_ISO_responseCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_responseCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_responseCodeType, AUTOMATIC, EXI_APPL_DATA) responseCodePtr );
#endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_serviceCategory
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_serviceCategoryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceCategoryPtr          pointer to Exi_ISO_serviceCategoryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceCategory( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceCategoryType, AUTOMATIC, EXI_APPL_DATA) serviceCategoryPtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_serviceName
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_serviceNameType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceNamePtr              pointer to Exi_ISO_serviceNameType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceNameType, AUTOMATIC, EXI_APPL_DATA) serviceNamePtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_serviceScope
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_serviceScopeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     serviceScopePtr             pointer to Exi_ISO_serviceScopeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceScope( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceScopeType, AUTOMATIC, EXI_APPL_DATA) serviceScopePtr );
#endif /* (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_sessionID
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_sessionIDType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     sessionIDPtr                pointer to Exi_ISO_sessionIDType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_sessionID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_sessionIDType, AUTOMATIC, EXI_APPL_DATA) sessionIDPtr );
#endif /* (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_sigMeterReading
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_sigMeterReadingType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     sigMeterReadingPtr          pointer to Exi_ISO_sigMeterReadingType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_sigMeterReading( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_sigMeterReadingType, AUTOMATIC, EXI_APPL_DATA) sigMeterReadingPtr );
#endif /* (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_tariffDescription
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_tariffDescriptionType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     tariffDescriptionPtr        pointer to Exi_ISO_tariffDescriptionType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_tariffDescription( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_tariffDescriptionType, AUTOMATIC, EXI_APPL_DATA) tariffDescriptionPtr );
#endif /* (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_unitSymbol
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_unitSymbolType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     unitSymbolPtr               pointer to Exi_ISO_unitSymbolType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_unitSymbol( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_unitSymbolType, AUTOMATIC, EXI_APPL_DATA) unitSymbolPtr );
#endif /* (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SCHEMA_ROOT) && (EXI_ENCODE_ISO_SCHEMA_ROOT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_ISO_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_ISO_SCHEMA_ROOT) && (EXI_ENCODE_ISO_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined(EXI_ENABLE_ENCODE_ISO_MESSAGE_SET) && (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_ISO_SCHEMA_ENCODER_H */
