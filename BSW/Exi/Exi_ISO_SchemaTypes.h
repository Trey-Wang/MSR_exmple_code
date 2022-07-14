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
 *         File:  Exi_ISO_SchemaTypes.h
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


#if !defined (EXI_ISO_SCHEMA_TYPES_H)
# define EXI_ISO_SCHEMA_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"

/* PRQA S 0779 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
/* PRQA S 0780 NAMESPACE */ /* MD_Exi_5.6 */
/* PRQA S 0750 UNION */ /* MD_Exi_18.4 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Root elements */
#define EXI_ISO_AC_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)80U
#define EXI_ISO_AC_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)81U
#define EXI_ISO_AC_EVSESTATUS_TYPE (Exi_RootElementIdType)82U
#define EXI_ISO_AUTHORIZATION_REQ_TYPE (Exi_RootElementIdType)83U
#define EXI_ISO_AUTHORIZATION_RES_TYPE (Exi_RootElementIdType)84U
#define EXI_ISO_BODY_BASE_TYPE (Exi_RootElementIdType)85U
#define EXI_ISO_CABLE_CHECK_REQ_TYPE (Exi_RootElementIdType)86U
#define EXI_ISO_CABLE_CHECK_RES_TYPE (Exi_RootElementIdType)87U
#define EXI_ISO_CERTIFICATE_INSTALLATION_REQ_TYPE (Exi_RootElementIdType)88U
#define EXI_ISO_CERTIFICATE_INSTALLATION_RES_TYPE (Exi_RootElementIdType)89U
#define EXI_ISO_CERTIFICATE_UPDATE_REQ_TYPE (Exi_RootElementIdType)90U
#define EXI_ISO_CERTIFICATE_UPDATE_RES_TYPE (Exi_RootElementIdType)91U
#define EXI_ISO_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE (Exi_RootElementIdType)92U
#define EXI_ISO_CHARGE_PARAMETER_DISCOVERY_RES_TYPE (Exi_RootElementIdType)93U
#define EXI_ISO_CHARGING_STATUS_REQ_TYPE (Exi_RootElementIdType)94U
#define EXI_ISO_CHARGING_STATUS_RES_TYPE (Exi_RootElementIdType)95U
#define EXI_ISO_CURRENT_DEMAND_REQ_TYPE (Exi_RootElementIdType)96U
#define EXI_ISO_CURRENT_DEMAND_RES_TYPE (Exi_RootElementIdType)97U
#define EXI_ISO_DC_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)98U
#define EXI_ISO_DC_EVPOWER_DELIVERY_PARAMETER_TYPE (Exi_RootElementIdType)99U
#define EXI_ISO_DC_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)100U
#define EXI_ISO_DC_EVSESTATUS_TYPE (Exi_RootElementIdType)101U
#define EXI_ISO_DC_EVSTATUS_TYPE (Exi_RootElementIdType)102U
#define EXI_ISO_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)103U
#define EXI_ISO_EVPOWER_DELIVERY_PARAMETER_TYPE (Exi_RootElementIdType)104U
#define EXI_ISO_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)105U
#define EXI_ISO_EVSESTATUS_TYPE (Exi_RootElementIdType)106U
#define EXI_ISO_EVSTATUS_TYPE (Exi_RootElementIdType)107U
#define EXI_ISO_ENTRY_TYPE (Exi_RootElementIdType)108U
#define EXI_ISO_INTERVAL_TYPE (Exi_RootElementIdType)109U
#define EXI_ISO_METERING_RECEIPT_REQ_TYPE (Exi_RootElementIdType)110U
#define EXI_ISO_METERING_RECEIPT_RES_TYPE (Exi_RootElementIdType)111U
#define EXI_ISO_PMAX_SCHEDULE_ENTRY_TYPE (Exi_RootElementIdType)112U
#define EXI_ISO_PAYMENT_DETAILS_REQ_TYPE (Exi_RootElementIdType)113U
#define EXI_ISO_PAYMENT_DETAILS_RES_TYPE (Exi_RootElementIdType)114U
#define EXI_ISO_PAYMENT_SERVICE_SELECTION_REQ_TYPE (Exi_RootElementIdType)115U
#define EXI_ISO_PAYMENT_SERVICE_SELECTION_RES_TYPE (Exi_RootElementIdType)116U
#define EXI_ISO_POWER_DELIVERY_REQ_TYPE (Exi_RootElementIdType)117U
#define EXI_ISO_POWER_DELIVERY_RES_TYPE (Exi_RootElementIdType)118U
#define EXI_ISO_PRE_CHARGE_REQ_TYPE (Exi_RootElementIdType)119U
#define EXI_ISO_PRE_CHARGE_RES_TYPE (Exi_RootElementIdType)120U
#define EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE (Exi_RootElementIdType)121U
#define EXI_ISO_SASCHEDULE_LIST_TYPE (Exi_RootElementIdType)122U
#define EXI_ISO_SASCHEDULES_TYPE (Exi_RootElementIdType)123U
#define EXI_ISO_SALES_TARIFF_ENTRY_TYPE (Exi_RootElementIdType)124U
#define EXI_ISO_SERVICE_DETAIL_REQ_TYPE (Exi_RootElementIdType)125U
#define EXI_ISO_SERVICE_DETAIL_RES_TYPE (Exi_RootElementIdType)126U
#define EXI_ISO_SERVICE_DISCOVERY_REQ_TYPE (Exi_RootElementIdType)127U
#define EXI_ISO_SERVICE_DISCOVERY_RES_TYPE (Exi_RootElementIdType)128U
#define EXI_ISO_SESSION_SETUP_REQ_TYPE (Exi_RootElementIdType)129U
#define EXI_ISO_SESSION_SETUP_RES_TYPE (Exi_RootElementIdType)130U
#define EXI_ISO_SESSION_STOP_REQ_TYPE (Exi_RootElementIdType)131U
#define EXI_ISO_SESSION_STOP_RES_TYPE (Exi_RootElementIdType)132U
#define EXI_ISO_V2G_MESSAGE_TYPE (Exi_RootElementIdType)133U
#define EXI_ISO_WELDING_DETECTION_REQ_TYPE (Exi_RootElementIdType)134U
#define EXI_ISO_WELDING_DETECTION_RES_TYPE (Exi_RootElementIdType)135U
/* Elements */
#define EXI_ISO_BODY_TYPE (Exi_RootElementIdType)228U
#define EXI_ISO_BODY_ELEMENT_TYPE (Exi_RootElementIdType)229U
#define EXI_ISO_BULK_CHARGING_COMPLETE_TYPE (Exi_RootElementIdType)230U
#define EXI_ISO_BULK_SOC_TYPE (Exi_RootElementIdType)231U
#define EXI_ISO_CERTIFICATE_TYPE (Exi_RootElementIdType)232U
#define EXI_ISO_CHARGE_PROGRESS_TYPE (Exi_RootElementIdType)233U
#define EXI_ISO_CHARGE_SERVICE_TYPE (Exi_RootElementIdType)234U
#define EXI_ISO_CHARGING_COMPLETE_TYPE (Exi_RootElementIdType)235U
#define EXI_ISO_CHARGING_PROFILE_TYPE (Exi_RootElementIdType)236U
#define EXI_ISO_CHARGING_PROFILE_ENTRY_MAX_NUMBER_OF_PHASES_IN_USE_TYPE (Exi_RootElementIdType)237U
#define EXI_ISO_CHARGING_PROFILE_ENTRY_MAX_POWER_TYPE (Exi_RootElementIdType)238U
#define EXI_ISO_CHARGING_PROFILE_ENTRY_START_TYPE (Exi_RootElementIdType)239U
#define EXI_ISO_CHARGING_SESSION_TYPE (Exi_RootElementIdType)240U
#define EXI_ISO_CONSUMPTION_COST_TYPE (Exi_RootElementIdType)241U
#define EXI_ISO_CONTRACT_SIGNATURE_CERT_CHAIN_TYPE (Exi_RootElementIdType)242U
#define EXI_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY_TYPE (Exi_RootElementIdType)243U
#define EXI_ISO_COST_TYPE (Exi_RootElementIdType)244U
#define EXI_ISO_DHPUBLICKEY_TYPE (Exi_RootElementIdType)245U
#define EXI_ISO_DEPARTURE_TIME_TYPE (Exi_RootElementIdType)246U
#define EXI_ISO_EAMOUNT_TYPE (Exi_RootElementIdType)247U
#define EXI_ISO_EPRICE_LEVEL_TYPE (Exi_RootElementIdType)248U
#define EXI_ISO_EVCCID_TYPE (Exi_RootElementIdType)249U
#define EXI_ISO_EVENERGY_CAPACITY_TYPE (Exi_RootElementIdType)250U
#define EXI_ISO_EVENERGY_REQUEST_TYPE (Exi_RootElementIdType)251U
#define EXI_ISO_EVERROR_CODE_TYPE (Exi_RootElementIdType)252U
#define EXI_ISO_EVMAX_CURRENT_TYPE (Exi_RootElementIdType)253U
#define EXI_ISO_EVMAX_VOLTAGE_TYPE (Exi_RootElementIdType)254U
#define EXI_ISO_EVMAXIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)255U
#define EXI_ISO_EVMAXIMUM_POWER_LIMIT_TYPE (Exi_RootElementIdType)256U
#define EXI_ISO_EVMAXIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)257U
#define EXI_ISO_EVMIN_CURRENT_TYPE (Exi_RootElementIdType)258U
#define EXI_ISO_EVRESSSOC_TYPE (Exi_RootElementIdType)259U
#define EXI_ISO_EVREADY_TYPE (Exi_RootElementIdType)260U
#define EXI_ISO_EVSECURRENT_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)261U
#define EXI_ISO_EVSECURRENT_REGULATION_TOLERANCE_TYPE (Exi_RootElementIdType)262U
#define EXI_ISO_EVSEENERGY_TO_BE_DELIVERED_TYPE (Exi_RootElementIdType)263U
#define EXI_ISO_EVSEID_TYPE (Exi_RootElementIdType)264U
#define EXI_ISO_EVSEISOLATION_STATUS_TYPE (Exi_RootElementIdType)265U
#define EXI_ISO_EVSEMAX_CURRENT_TYPE (Exi_RootElementIdType)266U
#define EXI_ISO_EVSEMAXIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)267U
#define EXI_ISO_EVSEMAXIMUM_POWER_LIMIT_TYPE (Exi_RootElementIdType)268U
#define EXI_ISO_EVSEMAXIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)269U
#define EXI_ISO_EVSEMINIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)270U
#define EXI_ISO_EVSEMINIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)271U
#define EXI_ISO_EVSENOMINAL_VOLTAGE_TYPE (Exi_RootElementIdType)272U
#define EXI_ISO_EVSENOTIFICATION_TYPE (Exi_RootElementIdType)273U
#define EXI_ISO_EVSEPEAK_CURRENT_RIPPLE_TYPE (Exi_RootElementIdType)274U
#define EXI_ISO_EVSEPOWER_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)275U
#define EXI_ISO_EVSEPRESENT_CURRENT_TYPE (Exi_RootElementIdType)276U
#define EXI_ISO_EVSEPRESENT_VOLTAGE_TYPE (Exi_RootElementIdType)277U
#define EXI_ISO_EVSEPROCESSING_TYPE (Exi_RootElementIdType)278U
#define EXI_ISO_EVSESTATUS_CODE_TYPE (Exi_RootElementIdType)279U
#define EXI_ISO_EVSETIME_STAMP_TYPE (Exi_RootElementIdType)280U
#define EXI_ISO_EVSEVOLTAGE_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)281U
#define EXI_ISO_EVTARGET_CURRENT_TYPE (Exi_RootElementIdType)282U
#define EXI_ISO_EVTARGET_VOLTAGE_TYPE (Exi_RootElementIdType)283U
#define EXI_ISO_ENERGY_TRANSFER_MODE_TYPE (Exi_RootElementIdType)284U
#define EXI_ISO_FAULT_CODE_TYPE (Exi_RootElementIdType)285U
#define EXI_ISO_FAULT_MSG_TYPE (Exi_RootElementIdType)286U
#define EXI_ISO_FREE_SERVICE_TYPE (Exi_RootElementIdType)287U
#define EXI_ISO_FULL_SOC_TYPE (Exi_RootElementIdType)288U
#define EXI_ISO_GEN_CHALLENGE_TYPE (Exi_RootElementIdType)289U
#define EXI_ISO_HEADER_TYPE (Exi_RootElementIdType)290U
#define EXI_ISO_LIST_OF_ROOT_CERTIFICATE_IDS_TYPE (Exi_RootElementIdType)291U
#define EXI_ISO_MAX_ENTRIES_SASCHEDULE_TUPLE_TYPE (Exi_RootElementIdType)292U
#define EXI_ISO_METER_ID_TYPE (Exi_RootElementIdType)293U
#define EXI_ISO_METER_INFO_TYPE (Exi_RootElementIdType)294U
#define EXI_ISO_METER_READING_TYPE (Exi_RootElementIdType)295U
#define EXI_ISO_METER_STATUS_TYPE (Exi_RootElementIdType)296U
#define EXI_ISO_MULTIPLIER_TYPE (Exi_RootElementIdType)297U
#define EXI_ISO_NOTIFICATION_TYPE (Exi_RootElementIdType)298U
#define EXI_ISO_NOTIFICATION_MAX_DELAY_TYPE (Exi_RootElementIdType)299U
#define EXI_ISO_NUM_EPRICE_LEVELS_TYPE (Exi_RootElementIdType)300U
#define EXI_ISO_OEMPROVISIONING_CERT_TYPE (Exi_RootElementIdType)301U
#define EXI_ISO_PMAX_TYPE (Exi_RootElementIdType)302U
#define EXI_ISO_PMAX_SCHEDULE_TYPE (Exi_RootElementIdType)303U
#define EXI_ISO_PARAMETER_TYPE (Exi_RootElementIdType)304U
#define EXI_ISO_PARAMETER_SET_TYPE (Exi_RootElementIdType)305U
#define EXI_ISO_PARAMETER_SET_ID_TYPE (Exi_RootElementIdType)306U
#define EXI_ISO_PAYMENT_OPTION_TYPE (Exi_RootElementIdType)307U
#define EXI_ISO_PAYMENT_OPTION_LIST_TYPE (Exi_RootElementIdType)308U
#define EXI_ISO_PROFILE_ENTRY_TYPE (Exi_RootElementIdType)309U
#define EXI_ISO_RCD_TYPE (Exi_RootElementIdType)310U
#define EXI_ISO_RECEIPT_REQUIRED_TYPE (Exi_RootElementIdType)311U
#define EXI_ISO_REMAINING_TIME_TO_BULK_SOC_TYPE (Exi_RootElementIdType)312U
#define EXI_ISO_REMAINING_TIME_TO_FULL_SOC_TYPE (Exi_RootElementIdType)313U
#define EXI_ISO_REQUESTED_ENERGY_TRANSFER_MODE_TYPE (Exi_RootElementIdType)314U
#define EXI_ISO_RESPONSE_CODE_TYPE (Exi_RootElementIdType)315U
#define EXI_ISO_RETRY_COUNTER_TYPE (Exi_RootElementIdType)316U
#define EXI_ISO_ROOT_CERTIFICATE_ID_TYPE (Exi_RootElementIdType)317U
#define EXI_ISO_SAPROVISIONING_CERTIFICATE_CHAIN_TYPE (Exi_RootElementIdType)318U
#define EXI_ISO_SASCHEDULE_TUPLE_TYPE (Exi_RootElementIdType)319U
#define EXI_ISO_SASCHEDULE_TUPLE_ID_TYPE (Exi_RootElementIdType)320U
#define EXI_ISO_SALES_TARIFF_TYPE (Exi_RootElementIdType)321U
#define EXI_ISO_SALES_TARIFF_DESCRIPTION_TYPE (Exi_RootElementIdType)322U
#define EXI_ISO_SALES_TARIFF_ID_TYPE (Exi_RootElementIdType)323U
#define EXI_ISO_SELECTED_PAYMENT_OPTION_TYPE (Exi_RootElementIdType)324U
#define EXI_ISO_SELECTED_SERVICE_TYPE (Exi_RootElementIdType)325U
#define EXI_ISO_SELECTED_SERVICE_LIST_TYPE (Exi_RootElementIdType)326U
#define EXI_ISO_SERVICE_TYPE (Exi_RootElementIdType)327U
#define EXI_ISO_SERVICE_CATEGORY_TYPE (Exi_RootElementIdType)328U
#define EXI_ISO_SERVICE_ID_TYPE (Exi_RootElementIdType)329U
#define EXI_ISO_SERVICE_LIST_TYPE (Exi_RootElementIdType)330U
#define EXI_ISO_SERVICE_NAME_TYPE (Exi_RootElementIdType)331U
#define EXI_ISO_SERVICE_PARAMETER_LIST_TYPE (Exi_RootElementIdType)332U
#define EXI_ISO_SERVICE_SCOPE_TYPE (Exi_RootElementIdType)333U
#define EXI_ISO_SESSION_ID_TYPE (Exi_RootElementIdType)334U
#define EXI_ISO_SIG_METER_READING_TYPE (Exi_RootElementIdType)335U
#define EXI_ISO_SIGNATURE_TYPE (Exi_RootElementIdType)336U
#define EXI_ISO_SUB_CERTIFICATES_TYPE (Exi_RootElementIdType)337U
#define EXI_ISO_SUPPORTED_ENERGY_TRANSFER_MODE_TYPE (Exi_RootElementIdType)338U
#define EXI_ISO_TMETER_TYPE (Exi_RootElementIdType)339U
#define EXI_ISO_TIME_INTERVAL_TYPE (Exi_RootElementIdType)340U
#define EXI_ISO_UNIT_TYPE (Exi_RootElementIdType)341U
#define EXI_ISO_VALUE_TYPE (Exi_RootElementIdType)342U
#define EXI_ISO_AMOUNT_TYPE (Exi_RootElementIdType)343U
#define EXI_ISO_AMOUNT_MULTIPLIER_TYPE (Exi_RootElementIdType)344U
#define EXI_ISO_BOOL_VALUE_TYPE (Exi_RootElementIdType)345U
#define EXI_ISO_BYTE_VALUE_TYPE (Exi_RootElementIdType)346U
#define EXI_ISO_COST_KIND_TYPE (Exi_RootElementIdType)347U
#define EXI_ISO_DURATION_TYPE (Exi_RootElementIdType)348U
#define EXI_ISO_E_MAID_TYPE (Exi_RootElementIdType)349U
#define EXI_ISO_EMAID_TYPE (Exi_RootElementIdType)350U
#define EXI_ISO_INT_VALUE_TYPE (Exi_RootElementIdType)351U
#define EXI_ISO_PHYSICAL_VALUE_TYPE (Exi_RootElementIdType)352U
#define EXI_ISO_SHORT_VALUE_TYPE (Exi_RootElementIdType)353U
#define EXI_ISO_START_TYPE (Exi_RootElementIdType)354U
#define EXI_ISO_START_VALUE_TYPE (Exi_RootElementIdType)355U
#define EXI_ISO_STRING_VALUE_TYPE (Exi_RootElementIdType)356U
#define EXI_SCHEMA_SET_ISO_TYPE (Exi_RootElementIdType)2U
#ifndef EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID
  #define EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME
  #define EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_ISO_EMAID
  #define EXI_MAX_BUFFER_SIZE_ISO_EMAID EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct Exi_ISO_sessionIDType Exi_ISO_sessionIDType;

struct Exi_ISO_sessionIDType
{
  uint16 Length;
  uint8 Buffer[8];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_FAULT_CODE_TYPE_PARSING_ERROR,
  EXI_ISO_FAULT_CODE_TYPE_NO_TLSROOT_CERTIFICAT_AVAILABLE,
  EXI_ISO_FAULT_CODE_TYPE_UNKNOWN_ERROR
} Exi_ISO_faultCodeType;

typedef struct Exi_ISO_faultMsgType Exi_ISO_faultMsgType;

struct Exi_ISO_faultMsgType
{
  uint16 Length;
  uint8 Buffer[64];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_NotificationType Exi_ISO_NotificationType;

struct Exi_ISO_NotificationType
{
  Exi_ISO_faultMsgType* FaultMsg;
  Exi_ISO_faultCodeType FaultCode;
  Exi_BitType FaultMsgFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_MessageHeaderType Exi_ISO_MessageHeaderType;

struct Exi_ISO_MessageHeaderType
{
  Exi_ISO_NotificationType* Notification;
  Exi_ISO_sessionIDType* SessionID;
  Exi_XMLSIG_SignatureType* Signature;
  Exi_BitType NotificationFlag : 1;
  Exi_BitType SignatureFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef void Exi_ISO_BodyBaseType;

typedef struct Exi_ISO_BodyType Exi_ISO_BodyType;

struct Exi_ISO_BodyType
{
  Exi_ISO_BodyBaseType* BodyElement;
  Exi_RootElementIdType BodyElementElementId;
  Exi_BitType BodyElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_V2G_MessageType Exi_ISO_V2G_MessageType;

struct Exi_ISO_V2G_MessageType
{
  Exi_ISO_BodyType* Body;
  Exi_ISO_MessageHeaderType* Header;
};

typedef struct Exi_ISO_evccIDType Exi_ISO_evccIDType;

struct Exi_ISO_evccIDType
{
  uint16 Length;
  uint8 Buffer[6];
};

typedef struct Exi_ISO_SessionSetupReqType Exi_ISO_SessionSetupReqType;

struct Exi_ISO_SessionSetupReqType
{
  Exi_ISO_evccIDType* EVCCID;
};

typedef enum
{
  EXI_ISO_RESPONSE_CODE_TYPE_OK,
  EXI_ISO_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED,
  EXI_ISO_RESPONSE_CODE_TYPE_OK_OLD_SESSION_JOINED,
  EXI_ISO_RESPONSE_CODE_TYPE_OK_CERTIFICATE_EXPIRES_SOON,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SEQUENCE_ERROR,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SERVICE_IDINVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_UNKNOWN_SESSION,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SERVICE_SELECTION_INVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_PAYMENT_SELECTION_INVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_EXPIRED,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SIGNATURE_ERROR,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_NO_CERTIFICATE_AVAILABLE,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERT_CHAIN_ERROR,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CHALLENGE_INVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CONTRACT_CANCELED,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_WRONG_CHARGE_PARAMETER,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_POWER_DELIVERY_NOT_APPLIED,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_TARIFF_SELECTION_INVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CHARGING_PROFILE_INVALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_METERING_SIGNATURE_NOT_VALID,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_NO_CHARGE_SERVICE_SELECTED,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_WRONG_ENERGY_TRANSFER_MODE,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CONTACTOR_ERROR,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_NOT_ALLOWED_AT_THIS_EVSE,
  EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_REVOKED
} Exi_ISO_responseCodeType;

typedef struct Exi_ISO_evseIDType Exi_ISO_evseIDType;

struct Exi_ISO_evseIDType
{
  uint16 Length;
  uint8 Buffer[37];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SessionSetupResType Exi_ISO_SessionSetupResType;

struct Exi_ISO_SessionSetupResType
{
  Exi_ISO_evseIDType* EVSEID;
  Exi_SInt64 EVSETimeStamp;
  Exi_ISO_responseCodeType ResponseCode;
  Exi_BitType EVSETimeStampFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_serviceScopeType Exi_ISO_serviceScopeType;

struct Exi_ISO_serviceScopeType
{
  uint16 Length;
  uint8 Buffer[64];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_SERVICE_CATEGORY_TYPE_EVCHARGING,
  EXI_ISO_SERVICE_CATEGORY_TYPE_INTERNET,
  EXI_ISO_SERVICE_CATEGORY_TYPE_CONTRACT_CERTIFICATE,
  EXI_ISO_SERVICE_CATEGORY_TYPE_OTHER_CUSTOM
} Exi_ISO_serviceCategoryType;

typedef struct Exi_ISO_ServiceDiscoveryReqType Exi_ISO_ServiceDiscoveryReqType;

struct Exi_ISO_ServiceDiscoveryReqType
{
  Exi_ISO_serviceScopeType* ServiceScope;
  Exi_ISO_serviceCategoryType ServiceCategory;
  Exi_BitType ServiceCategoryFlag : 1;
  Exi_BitType ServiceScopeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_PAYMENT_OPTION_TYPE_CONTRACT,
  EXI_ISO_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT
} Exi_ISO_paymentOptionType;

typedef struct Exi_ISO_PaymentOptionListType Exi_ISO_PaymentOptionListType;

struct Exi_ISO_PaymentOptionListType
{
  Exi_ISO_paymentOptionType PaymentOption[2];
  uint8 PaymentOptionCount;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_serviceNameType Exi_ISO_serviceNameType;

struct Exi_ISO_serviceNameType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE,
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE,
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_CORE,
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_EXTENDED,
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_COMBO_CORE,
  EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_UNIQUE
} Exi_ISO_EnergyTransferModeType;

typedef struct Exi_ISO_SupportedEnergyTransferModeType Exi_ISO_SupportedEnergyTransferModeType;

struct Exi_ISO_SupportedEnergyTransferModeType
{
  Exi_ISO_EnergyTransferModeType EnergyTransferMode[6];
  uint8 EnergyTransferModeCount;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (6 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (6 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (6 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ChargeServiceType Exi_ISO_ChargeServiceType;

struct Exi_ISO_ChargeServiceType
{
  Exi_ISO_serviceNameType* ServiceName;
  Exi_ISO_serviceScopeType* ServiceScope;
  Exi_ISO_SupportedEnergyTransferModeType* SupportedEnergyTransferMode;
  uint16 ServiceID;
  Exi_ISO_serviceCategoryType ServiceCategory;
  boolean FreeService;
  Exi_BitType ServiceNameFlag : 1;
  Exi_BitType ServiceScopeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ServiceType Exi_ISO_ServiceType;

struct Exi_ISO_ServiceType
{
  struct Exi_ISO_ServiceType* NextServicePtr;
  Exi_ISO_serviceNameType* ServiceName;
  Exi_ISO_serviceScopeType* ServiceScope;
  uint16 ServiceID;
  Exi_ISO_serviceCategoryType ServiceCategory;
  boolean FreeService;
  Exi_BitType ServiceNameFlag : 1;
  Exi_BitType ServiceScopeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ServiceListType Exi_ISO_ServiceListType;

struct Exi_ISO_ServiceListType
{
  Exi_ISO_ServiceType* Service;
};

typedef struct Exi_ISO_ServiceDiscoveryResType Exi_ISO_ServiceDiscoveryResType;

struct Exi_ISO_ServiceDiscoveryResType
{
  Exi_ISO_ChargeServiceType* ChargeService;
  Exi_ISO_PaymentOptionListType* PaymentOptionList;
  Exi_ISO_ServiceListType* ServiceList;
  Exi_ISO_responseCodeType ResponseCode;
  Exi_BitType ServiceListFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ServiceDetailReqType Exi_ISO_ServiceDetailReqType;

struct Exi_ISO_ServiceDetailReqType
{
  uint16 ServiceID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_AttributeNameType Exi_ISO_AttributeNameType;

struct Exi_ISO_AttributeNameType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTENAME ) + 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_UNIT_SYMBOL_TYPE_H,
  EXI_ISO_UNIT_SYMBOL_TYPE_M,
  EXI_ISO_UNIT_SYMBOL_TYPE_S,
  EXI_ISO_UNIT_SYMBOL_TYPE_A,
  EXI_ISO_UNIT_SYMBOL_TYPE_V,
  EXI_ISO_UNIT_SYMBOL_TYPE_W,
  EXI_ISO_UNIT_SYMBOL_TYPE_WH
} Exi_ISO_unitSymbolType;

typedef struct Exi_ISO_PhysicalValueType Exi_ISO_PhysicalValueType;

struct Exi_ISO_PhysicalValueType
{
  sint16 Value;
  Exi_ISO_unitSymbolType Unit;
  sint8 Multiplier;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ParameterChoiceType Exi_ISO_ParameterChoiceType;

struct Exi_ISO_ParameterChoiceType
{
  union
  {
    Exi_ISO_PhysicalValueType* physicalValue;
    Exi_stringType* stringValue;
    sint32 intValue;
    sint16 shortValue;
    boolean boolValue;
    sint8 byteValue;
  } ChoiceValue;
  Exi_BitType physicalValueFlag : 1;
  Exi_BitType stringValueFlag : 1;
  Exi_BitType intValueFlag : 1;
  Exi_BitType shortValueFlag : 1;
  Exi_BitType boolValueFlag : 1;
  Exi_BitType byteValueFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ParameterType Exi_ISO_ParameterType;

struct Exi_ISO_ParameterType
{
  Exi_ISO_ParameterChoiceType* ChoiceElement;
  Exi_ISO_AttributeNameType* Name;
  struct Exi_ISO_ParameterType* NextParameterPtr;
};

typedef struct Exi_ISO_ParameterSetType Exi_ISO_ParameterSetType;

struct Exi_ISO_ParameterSetType
{
  struct Exi_ISO_ParameterSetType* NextParameterSetPtr;
  Exi_ISO_ParameterType* Parameter;
  sint16 ParameterSetID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ServiceParameterListType Exi_ISO_ServiceParameterListType;

struct Exi_ISO_ServiceParameterListType
{
  Exi_ISO_ParameterSetType* ParameterSet;
};

typedef struct Exi_ISO_ServiceDetailResType Exi_ISO_ServiceDetailResType;

struct Exi_ISO_ServiceDetailResType
{
  Exi_ISO_ServiceParameterListType* ServiceParameterList;
  uint16 ServiceID;
  Exi_ISO_responseCodeType ResponseCode;
  Exi_BitType ServiceParameterListFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SelectedServiceType Exi_ISO_SelectedServiceType;

struct Exi_ISO_SelectedServiceType
{
  struct Exi_ISO_SelectedServiceType* NextSelectedServicePtr;
  sint16 ParameterSetID;
  uint16 ServiceID;
  Exi_BitType ParameterSetIDFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SelectedServiceListType Exi_ISO_SelectedServiceListType;

struct Exi_ISO_SelectedServiceListType
{
  Exi_ISO_SelectedServiceType* SelectedService;
};

typedef struct Exi_ISO_PaymentServiceSelectionReqType Exi_ISO_PaymentServiceSelectionReqType;

struct Exi_ISO_PaymentServiceSelectionReqType
{
  Exi_ISO_SelectedServiceListType* SelectedServiceList;
  Exi_ISO_paymentOptionType SelectedPaymentOption;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PaymentServiceSelectionResType Exi_ISO_PaymentServiceSelectionResType;

struct Exi_ISO_PaymentServiceSelectionResType
{
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_eMAIDType Exi_ISO_eMAIDType;

struct Exi_ISO_eMAIDType
{
  uint16 Length;
  uint8 Buffer[15];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_AttributeIdType Exi_ISO_AttributeIdType;

struct Exi_ISO_AttributeIdType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_ISO_ATTRIBUTEID ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_certificateType Exi_ISO_certificateType;

struct Exi_ISO_certificateType
{
  struct Exi_ISO_certificateType* NextCertificatePtr;
  uint16 Length;
  uint8 Buffer[800];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SubCertificatesType Exi_ISO_SubCertificatesType;

struct Exi_ISO_SubCertificatesType
{
  Exi_ISO_certificateType* Certificate;
};

typedef struct Exi_ISO_CertificateChainType Exi_ISO_CertificateChainType;

struct Exi_ISO_CertificateChainType
{
  Exi_ISO_certificateType* Certificate;
  Exi_ISO_AttributeIdType* Id;
  Exi_ISO_SubCertificatesType* SubCertificates;
  Exi_BitType IdFlag : 1;
  Exi_BitType SubCertificatesFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PaymentDetailsReqType Exi_ISO_PaymentDetailsReqType;

struct Exi_ISO_PaymentDetailsReqType
{
  Exi_ISO_CertificateChainType* ContractSignatureCertChain;
  Exi_ISO_eMAIDType* eMAID;
};

typedef struct Exi_ISO_genChallengeType Exi_ISO_genChallengeType;

struct Exi_ISO_genChallengeType
{
  uint16 Length;
  uint8 Buffer[16];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PaymentDetailsResType Exi_ISO_PaymentDetailsResType;

struct Exi_ISO_PaymentDetailsResType
{
  Exi_ISO_genChallengeType* GenChallenge;
  Exi_SInt64 EVSETimeStamp;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_AuthorizationReqType Exi_ISO_AuthorizationReqType;

struct Exi_ISO_AuthorizationReqType
{
  Exi_ISO_genChallengeType* GenChallenge;
  Exi_ISO_AttributeIdType* Id;
  Exi_BitType GenChallengeFlag : 1;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_EVSEPROCESSING_TYPE_FINISHED,
  EXI_ISO_EVSEPROCESSING_TYPE_ONGOING,
  EXI_ISO_EVSEPROCESSING_TYPE_ONGOING_WAITING_FOR_CUSTOMER_INTERACTION
} Exi_ISO_EVSEProcessingType;

typedef struct Exi_ISO_AuthorizationResType Exi_ISO_AuthorizationResType;

struct Exi_ISO_AuthorizationResType
{
  Exi_ISO_EVSEProcessingType EVSEProcessing;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_EVChargeParameterType Exi_ISO_EVChargeParameterType;

struct Exi_ISO_EVChargeParameterType
{
  uint32 DepartureTime;
  Exi_BitType DepartureTimeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ChargeParameterDiscoveryReqType Exi_ISO_ChargeParameterDiscoveryReqType;

struct Exi_ISO_ChargeParameterDiscoveryReqType
{
  Exi_ISO_EVChargeParameterType* EVChargeParameter;
  Exi_RootElementIdType EVChargeParameterElementId;
  uint16 MaxEntriesSAScheduleTuple;
  Exi_ISO_EnergyTransferModeType RequestedEnergyTransferMode;
  Exi_BitType MaxEntriesSAScheduleTupleFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef void Exi_ISO_SASchedulesType;

typedef void Exi_ISO_EVSEChargeParameterType;

typedef struct Exi_ISO_ChargeParameterDiscoveryResType Exi_ISO_ChargeParameterDiscoveryResType;

struct Exi_ISO_ChargeParameterDiscoveryResType
{
  Exi_ISO_EVSEChargeParameterType* EVSEChargeParameter;
  Exi_ISO_SASchedulesType* SASchedules;
  Exi_RootElementIdType EVSEChargeParameterElementId;
  Exi_RootElementIdType SASchedulesElementId;
  Exi_ISO_EVSEProcessingType EVSEProcessing;
  Exi_ISO_responseCodeType ResponseCode;
  Exi_BitType SASchedulesFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_CHARGE_PROGRESS_TYPE_START,
  EXI_ISO_CHARGE_PROGRESS_TYPE_STOP,
  EXI_ISO_CHARGE_PROGRESS_TYPE_RENEGOTIATE
} Exi_ISO_chargeProgressType;

typedef struct Exi_ISO_ProfileEntryType Exi_ISO_ProfileEntryType;

struct Exi_ISO_ProfileEntryType
{
  Exi_ISO_PhysicalValueType* ChargingProfileEntryMaxPower;
  struct Exi_ISO_ProfileEntryType* NextProfileEntryPtr;
  uint32 ChargingProfileEntryStart;
  sint8 ChargingProfileEntryMaxNumberOfPhasesInUse;
  Exi_BitType ChargingProfileEntryMaxNumberOfPhasesInUseFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ChargingProfileType Exi_ISO_ChargingProfileType;

struct Exi_ISO_ChargingProfileType
{
  Exi_ISO_ProfileEntryType* ProfileEntry;
};

typedef void Exi_ISO_EVPowerDeliveryParameterType;

typedef struct Exi_ISO_PowerDeliveryReqType Exi_ISO_PowerDeliveryReqType;

struct Exi_ISO_PowerDeliveryReqType
{
  Exi_ISO_ChargingProfileType* ChargingProfile;
  Exi_ISO_EVPowerDeliveryParameterType* EVPowerDeliveryParameter;
  Exi_RootElementIdType EVPowerDeliveryParameterElementId;
  Exi_ISO_chargeProgressType ChargeProgress;
  uint8 SAScheduleTupleID;
  Exi_BitType ChargingProfileFlag : 1;
  Exi_BitType EVPowerDeliveryParameterFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_EVSENOTIFICATION_TYPE_NONE,
  EXI_ISO_EVSENOTIFICATION_TYPE_STOP_CHARGING,
  EXI_ISO_EVSENOTIFICATION_TYPE_RE_NEGOTIATION
} Exi_ISO_EVSENotificationType;

typedef struct Exi_ISO_EVSEStatusType Exi_ISO_EVSEStatusType;

struct Exi_ISO_EVSEStatusType
{
  uint16 NotificationMaxDelay;
  Exi_ISO_EVSENotificationType EVSENotification;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PowerDeliveryResType Exi_ISO_PowerDeliveryResType;

struct Exi_ISO_PowerDeliveryResType
{
  Exi_ISO_EVSEStatusType* EVSEStatus;
  Exi_RootElementIdType EVSEStatusElementId;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_meterIDType Exi_ISO_meterIDType;

struct Exi_ISO_meterIDType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_sigMeterReadingType Exi_ISO_sigMeterReadingType;

struct Exi_ISO_sigMeterReadingType
{
  uint16 Length;
  uint8 Buffer[64];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_MeterInfoType Exi_ISO_MeterInfoType;

struct Exi_ISO_MeterInfoType
{
  Exi_ISO_meterIDType* MeterID;
  Exi_ISO_sigMeterReadingType* SigMeterReading;
  Exi_SInt64 TMeter;
  Exi_UInt64 MeterReading;
  sint16 MeterStatus;
  Exi_BitType MeterReadingFlag : 1;
  Exi_BitType MeterStatusFlag : 1;
  Exi_BitType SigMeterReadingFlag : 1;
  Exi_BitType TMeterFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_MeteringReceiptReqType Exi_ISO_MeteringReceiptReqType;

struct Exi_ISO_MeteringReceiptReqType
{
  Exi_ISO_AttributeIdType* Id;
  Exi_ISO_MeterInfoType* MeterInfo;
  Exi_ISO_sessionIDType* SessionID;
  uint8 SAScheduleTupleID;
  Exi_BitType IdFlag : 1;
  Exi_BitType SAScheduleTupleIDFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_MeteringReceiptResType Exi_ISO_MeteringReceiptResType;

struct Exi_ISO_MeteringReceiptResType
{
  Exi_ISO_EVSEStatusType* EVSEStatus;
  Exi_RootElementIdType EVSEStatusElementId;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_CHARGING_SESSION_TYPE_TERMINATE,
  EXI_ISO_CHARGING_SESSION_TYPE_PAUSE
} Exi_ISO_chargingSessionType;

typedef struct Exi_ISO_SessionStopReqType Exi_ISO_SessionStopReqType;

struct Exi_ISO_SessionStopReqType
{
  Exi_ISO_chargingSessionType ChargingSession;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SessionStopResType Exi_ISO_SessionStopResType;

struct Exi_ISO_SessionStopResType
{
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ListOfRootCertificateIDsType Exi_ISO_ListOfRootCertificateIDsType;

struct Exi_ISO_ListOfRootCertificateIDsType
{
  Exi_XMLSIG_X509IssuerSerialType* RootCertificateID;
};

typedef struct Exi_ISO_CertificateUpdateReqType Exi_ISO_CertificateUpdateReqType;

struct Exi_ISO_CertificateUpdateReqType
{
  Exi_ISO_CertificateChainType* ContractSignatureCertChain;
  Exi_ISO_AttributeIdType* Id;
  Exi_ISO_ListOfRootCertificateIDsType* ListOfRootCertificateIDs;
  Exi_ISO_eMAIDType* eMAID;
};

typedef struct Exi_ISO_ContractSignatureEncryptedPrivateKeyType Exi_ISO_ContractSignatureEncryptedPrivateKeyType;

struct Exi_ISO_ContractSignatureEncryptedPrivateKeyType
{
  Exi_ISO_AttributeIdType* Id;
  uint16 Length;
  uint8 Buffer[48];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_DiffieHellmanPublickeyType Exi_ISO_DiffieHellmanPublickeyType;

struct Exi_ISO_DiffieHellmanPublickeyType
{
  Exi_ISO_AttributeIdType* Id;
  uint16 Length;
  uint8 Buffer[65];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_EMAIDType Exi_ISO_EMAIDType;

struct Exi_ISO_EMAIDType
{
  Exi_ISO_AttributeIdType* Id;
  uint16 Length;
  uint8 Buffer[15];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CertificateUpdateResType Exi_ISO_CertificateUpdateResType;

struct Exi_ISO_CertificateUpdateResType
{
  Exi_ISO_CertificateChainType* ContractSignatureCertChain;
  Exi_ISO_ContractSignatureEncryptedPrivateKeyType* ContractSignatureEncryptedPrivateKey;
  Exi_ISO_DiffieHellmanPublickeyType* DHpublickey;
  Exi_ISO_CertificateChainType* SAProvisioningCertificateChain;
  Exi_ISO_EMAIDType* eMAID;
  sint16 RetryCounter;
  Exi_ISO_responseCodeType ResponseCode;
  Exi_BitType RetryCounterFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CertificateInstallationReqType Exi_ISO_CertificateInstallationReqType;

struct Exi_ISO_CertificateInstallationReqType
{
  Exi_ISO_AttributeIdType* Id;
  Exi_ISO_ListOfRootCertificateIDsType* ListOfRootCertificateIDs;
  Exi_ISO_certificateType* OEMProvisioningCert;
};

typedef struct Exi_ISO_CertificateInstallationResType Exi_ISO_CertificateInstallationResType;

struct Exi_ISO_CertificateInstallationResType
{
  Exi_ISO_CertificateChainType* ContractSignatureCertChain;
  Exi_ISO_ContractSignatureEncryptedPrivateKeyType* ContractSignatureEncryptedPrivateKey;
  Exi_ISO_DiffieHellmanPublickeyType* DHpublickey;
  Exi_ISO_CertificateChainType* SAProvisioningCertificateChain;
  Exi_ISO_EMAIDType* eMAID;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef void Exi_ISO_ChargingStatusReqType;

typedef struct Exi_ISO_AC_EVSEStatusType Exi_ISO_AC_EVSEStatusType;

struct Exi_ISO_AC_EVSEStatusType
{
  uint16 NotificationMaxDelay;
  Exi_ISO_EVSENotificationType EVSENotification;
  boolean RCD;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ChargingStatusResType Exi_ISO_ChargingStatusResType;

struct Exi_ISO_ChargingStatusResType
{
  Exi_ISO_AC_EVSEStatusType* AC_EVSEStatus;
  Exi_ISO_evseIDType* EVSEID;
  Exi_ISO_PhysicalValueType* EVSEMaxCurrent;
  Exi_ISO_MeterInfoType* MeterInfo;
  Exi_ISO_responseCodeType ResponseCode;
  boolean ReceiptRequired;
  uint8 SAScheduleTupleID;
  Exi_BitType EVSEMaxCurrentFlag : 1;
  Exi_BitType MeterInfoFlag : 1;
  Exi_BitType ReceiptRequiredFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_DC_EVERROR_CODE_TYPE_NO_ERROR,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_RESSTEMPERATURE_INHIBIT,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_EVSHIFT_POSITION,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGER_CONNECTOR_LOCK_FAULT,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_EVRESSMALFUNCTION,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_CURRENTDIFFERENTIAL,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_VOLTAGE_OUT_OF_RANGE,
  EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_A,
  EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_B,
  EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_C,
  EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_SYSTEM_INCOMPATIBILITY,
  EXI_ISO_DC_EVERROR_CODE_TYPE_NO_DATA
} Exi_ISO_DC_EVErrorCodeType;

typedef struct Exi_ISO_DC_EVStatusType Exi_ISO_DC_EVStatusType;

struct Exi_ISO_DC_EVStatusType
{
  Exi_ISO_DC_EVErrorCodeType EVErrorCode;
  boolean EVReady;
  sint8 EVRESSSOC;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CableCheckReqType Exi_ISO_CableCheckReqType;

struct Exi_ISO_CableCheckReqType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
};

typedef enum
{
  EXI_ISO_ISOLATION_LEVEL_TYPE_INVALID,
  EXI_ISO_ISOLATION_LEVEL_TYPE_VALID,
  EXI_ISO_ISOLATION_LEVEL_TYPE_WARNING,
  EXI_ISO_ISOLATION_LEVEL_TYPE_FAULT,
  EXI_ISO_ISOLATION_LEVEL_TYPE_NO_IMD
} Exi_ISO_isolationLevelType;

typedef enum
{
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_NOT_READY,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_READY,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_SHUTDOWN,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_UTILITY_INTERRUPT_EVENT,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_ISOLATION_MONITORING_ACTIVE,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_EMERGENCY_SHUTDOWN,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_MALFUNCTION,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_8,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_9,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_A,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_B,
  EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_C
} Exi_ISO_DC_EVSEStatusCodeType;

typedef struct Exi_ISO_DC_EVSEStatusType Exi_ISO_DC_EVSEStatusType;

struct Exi_ISO_DC_EVSEStatusType
{
  uint16 NotificationMaxDelay;
  Exi_ISO_DC_EVSEStatusCodeType EVSEStatusCode;
  Exi_ISO_EVSENotificationType EVSENotification;
  Exi_ISO_isolationLevelType EVSEIsolationStatus;
  Exi_BitType EVSEIsolationStatusFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CableCheckResType Exi_ISO_CableCheckResType;

struct Exi_ISO_CableCheckResType
{
  Exi_ISO_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_ISO_EVSEProcessingType EVSEProcessing;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (2 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PreChargeReqType Exi_ISO_PreChargeReqType;

struct Exi_ISO_PreChargeReqType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
  Exi_ISO_PhysicalValueType* EVTargetCurrent;
  Exi_ISO_PhysicalValueType* EVTargetVoltage;
};

typedef struct Exi_ISO_PreChargeResType Exi_ISO_PreChargeResType;

struct Exi_ISO_PreChargeResType
{
  Exi_ISO_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_ISO_PhysicalValueType* EVSEPresentVoltage;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CurrentDemandReqType Exi_ISO_CurrentDemandReqType;

struct Exi_ISO_CurrentDemandReqType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
  Exi_ISO_PhysicalValueType* EVMaximumCurrentLimit;
  Exi_ISO_PhysicalValueType* EVMaximumPowerLimit;
  Exi_ISO_PhysicalValueType* EVMaximumVoltageLimit;
  Exi_ISO_PhysicalValueType* EVTargetCurrent;
  Exi_ISO_PhysicalValueType* EVTargetVoltage;
  Exi_ISO_PhysicalValueType* RemainingTimeToBulkSoC;
  Exi_ISO_PhysicalValueType* RemainingTimeToFullSoC;
  boolean BulkChargingComplete;
  boolean ChargingComplete;
  Exi_BitType BulkChargingCompleteFlag : 1;
  Exi_BitType EVMaximumCurrentLimitFlag : 1;
  Exi_BitType EVMaximumPowerLimitFlag : 1;
  Exi_BitType EVMaximumVoltageLimitFlag : 1;
  Exi_BitType RemainingTimeToBulkSoCFlag : 1;
  Exi_BitType RemainingTimeToFullSoCFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_CurrentDemandResType Exi_ISO_CurrentDemandResType;

struct Exi_ISO_CurrentDemandResType
{
  Exi_ISO_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_ISO_evseIDType* EVSEID;
  Exi_ISO_PhysicalValueType* EVSEMaximumCurrentLimit;
  Exi_ISO_PhysicalValueType* EVSEMaximumPowerLimit;
  Exi_ISO_PhysicalValueType* EVSEMaximumVoltageLimit;
  Exi_ISO_PhysicalValueType* EVSEPresentCurrent;
  Exi_ISO_PhysicalValueType* EVSEPresentVoltage;
  Exi_ISO_MeterInfoType* MeterInfo;
  Exi_ISO_responseCodeType ResponseCode;
  boolean EVSECurrentLimitAchieved;
  boolean EVSEPowerLimitAchieved;
  boolean EVSEVoltageLimitAchieved;
  boolean ReceiptRequired;
  uint8 SAScheduleTupleID;
  Exi_BitType EVSEMaximumCurrentLimitFlag : 1;
  Exi_BitType EVSEMaximumPowerLimitFlag : 1;
  Exi_BitType EVSEMaximumVoltageLimitFlag : 1;
  Exi_BitType MeterInfoFlag : 1;
  Exi_BitType ReceiptRequiredFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_WeldingDetectionReqType Exi_ISO_WeldingDetectionReqType;

struct Exi_ISO_WeldingDetectionReqType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
};

typedef struct Exi_ISO_WeldingDetectionResType Exi_ISO_WeldingDetectionResType;

struct Exi_ISO_WeldingDetectionResType
{
  Exi_ISO_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_ISO_PhysicalValueType* EVSEPresentVoltage;
  Exi_ISO_responseCodeType ResponseCode;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef void Exi_ISO_IntervalType;

typedef struct Exi_ISO_PMaxScheduleEntryType Exi_ISO_PMaxScheduleEntryType;

struct Exi_ISO_PMaxScheduleEntryType
{
  struct Exi_ISO_PMaxScheduleEntryType* NextPMaxScheduleEntryPtr;
  Exi_ISO_PhysicalValueType* PMax;
  Exi_ISO_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_PMaxScheduleType Exi_ISO_PMaxScheduleType;

struct Exi_ISO_PMaxScheduleType
{
  Exi_ISO_PMaxScheduleEntryType* PMaxScheduleEntry;
};

typedef struct Exi_ISO_tariffDescriptionType Exi_ISO_tariffDescriptionType;

struct Exi_ISO_tariffDescriptionType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_ISO_COST_KIND_TYPE_RELATIVE_PRICE_PERCENTAGE,
  EXI_ISO_COST_KIND_TYPE_RENEWABLE_GENERATION_PERCENTAGE,
  EXI_ISO_COST_KIND_TYPE_CARBON_DIOXIDE_EMISSION
} Exi_ISO_costKindType;

typedef struct Exi_ISO_CostType Exi_ISO_CostType;

struct Exi_ISO_CostType
{
  struct Exi_ISO_CostType* NextCostPtr;
  uint32 amount;
  Exi_ISO_costKindType costKind;
  sint8 amountMultiplier;
  Exi_BitType amountMultiplierFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_ConsumptionCostType Exi_ISO_ConsumptionCostType;

struct Exi_ISO_ConsumptionCostType
{
  Exi_ISO_CostType* Cost;
  struct Exi_ISO_ConsumptionCostType* NextConsumptionCostPtr;
  Exi_ISO_PhysicalValueType* startValue;
};

typedef struct Exi_ISO_SalesTariffEntryType Exi_ISO_SalesTariffEntryType;

struct Exi_ISO_SalesTariffEntryType
{
  Exi_ISO_ConsumptionCostType* ConsumptionCost;
  struct Exi_ISO_SalesTariffEntryType* NextSalesTariffEntryPtr;
  Exi_ISO_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
  uint8 EPriceLevel;
  Exi_BitType ConsumptionCostFlag : 1;
  Exi_BitType EPriceLevelFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 3 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 3 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SalesTariffType Exi_ISO_SalesTariffType;

struct Exi_ISO_SalesTariffType
{
  Exi_ISO_AttributeIdType* Id;
  Exi_ISO_tariffDescriptionType* SalesTariffDescription;
  Exi_ISO_SalesTariffEntryType* SalesTariffEntry;
  uint8 NumEPriceLevels;
  uint8 SalesTariffID;
  Exi_BitType IdFlag : 1;
  Exi_BitType NumEPriceLevelsFlag : 1;
  Exi_BitType SalesTariffDescriptionFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SAScheduleTupleType Exi_ISO_SAScheduleTupleType;

struct Exi_ISO_SAScheduleTupleType
{
  struct Exi_ISO_SAScheduleTupleType* NextSAScheduleTuplePtr;
  Exi_ISO_PMaxScheduleType* PMaxSchedule;
  Exi_ISO_SalesTariffType* SalesTariff;
  uint8 SAScheduleTupleID;
  Exi_BitType SalesTariffFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_SAScheduleListType Exi_ISO_SAScheduleListType;

struct Exi_ISO_SAScheduleListType
{
  Exi_ISO_SAScheduleTupleType* SAScheduleTuple;
};

typedef struct Exi_ISO_EntryType Exi_ISO_EntryType;

struct Exi_ISO_EntryType
{
  Exi_ISO_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_RelativeTimeIntervalType Exi_ISO_RelativeTimeIntervalType;

struct Exi_ISO_RelativeTimeIntervalType
{
  uint32 duration;
  uint32 start;
  Exi_BitType durationFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef void Exi_ISO_EVStatusType;

typedef struct Exi_ISO_AC_EVChargeParameterType Exi_ISO_AC_EVChargeParameterType;

struct Exi_ISO_AC_EVChargeParameterType
{
  Exi_ISO_PhysicalValueType* EAmount;
  Exi_ISO_PhysicalValueType* EVMaxCurrent;
  Exi_ISO_PhysicalValueType* EVMaxVoltage;
  Exi_ISO_PhysicalValueType* EVMinCurrent;
  uint32 DepartureTime;
  Exi_BitType DepartureTimeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_DC_EVChargeParameterType Exi_ISO_DC_EVChargeParameterType;

struct Exi_ISO_DC_EVChargeParameterType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
  Exi_ISO_PhysicalValueType* EVEnergyCapacity;
  Exi_ISO_PhysicalValueType* EVEnergyRequest;
  Exi_ISO_PhysicalValueType* EVMaximumCurrentLimit;
  Exi_ISO_PhysicalValueType* EVMaximumPowerLimit;
  Exi_ISO_PhysicalValueType* EVMaximumVoltageLimit;
  uint32 DepartureTime;
  sint8 BulkSOC;
  sint8 FullSOC;
  Exi_BitType BulkSOCFlag : 1;
  Exi_BitType DepartureTimeFlag : 1;
  Exi_BitType EVEnergyCapacityFlag : 1;
  Exi_BitType EVEnergyRequestFlag : 1;
  Exi_BitType EVMaximumPowerLimitFlag : 1;
  Exi_BitType FullSOCFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_AC_EVSEChargeParameterType Exi_ISO_AC_EVSEChargeParameterType;

struct Exi_ISO_AC_EVSEChargeParameterType
{
  Exi_ISO_AC_EVSEStatusType* AC_EVSEStatus;
  Exi_ISO_PhysicalValueType* EVSEMaxCurrent;
  Exi_ISO_PhysicalValueType* EVSENominalVoltage;
};

typedef struct Exi_ISO_DC_EVSEChargeParameterType Exi_ISO_DC_EVSEChargeParameterType;

struct Exi_ISO_DC_EVSEChargeParameterType
{
  Exi_ISO_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_ISO_PhysicalValueType* EVSECurrentRegulationTolerance;
  Exi_ISO_PhysicalValueType* EVSEEnergyToBeDelivered;
  Exi_ISO_PhysicalValueType* EVSEMaximumCurrentLimit;
  Exi_ISO_PhysicalValueType* EVSEMaximumPowerLimit;
  Exi_ISO_PhysicalValueType* EVSEMaximumVoltageLimit;
  Exi_ISO_PhysicalValueType* EVSEMinimumCurrentLimit;
  Exi_ISO_PhysicalValueType* EVSEMinimumVoltageLimit;
  Exi_ISO_PhysicalValueType* EVSEPeakCurrentRipple;
  Exi_BitType EVSECurrentRegulationToleranceFlag : 1;
  Exi_BitType EVSEEnergyToBeDeliveredFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_ISO_DC_EVPowerDeliveryParameterType Exi_ISO_DC_EVPowerDeliveryParameterType;

struct Exi_ISO_DC_EVPowerDeliveryParameterType
{
  Exi_ISO_DC_EVStatusType* DC_EVStatus;
  boolean BulkChargingComplete;
  boolean ChargingComplete;
  Exi_BitType BulkChargingCompleteFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + 2 ) % 4)];
# endif
#endif 
};

/* PRQA L:IDENTIFIER_NAMES */
/* PRQA L:NAMESPACE */
/* PRQA L:UNION */


#endif
  /* EXI_ISO_SCHEMA_TYPES_H */
