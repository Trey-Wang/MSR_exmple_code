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
 *         File:  Exi_DIN_SchemaTypes.h
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


#if !defined (EXI_DIN_SCHEMA_TYPES_H)
# define EXI_DIN_SCHEMA_TYPES_H

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
#define EXI_DIN_AC_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)23U
#define EXI_DIN_AC_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)24U
#define EXI_DIN_AC_EVSESTATUS_TYPE (Exi_RootElementIdType)25U
#define EXI_DIN_BODY_BASE_TYPE (Exi_RootElementIdType)26U
#define EXI_DIN_CABLE_CHECK_REQ_TYPE (Exi_RootElementIdType)27U
#define EXI_DIN_CABLE_CHECK_RES_TYPE (Exi_RootElementIdType)28U
#define EXI_DIN_CERTIFICATE_INSTALLATION_REQ_TYPE (Exi_RootElementIdType)29U
#define EXI_DIN_CERTIFICATE_INSTALLATION_RES_TYPE (Exi_RootElementIdType)30U
#define EXI_DIN_CERTIFICATE_UPDATE_REQ_TYPE (Exi_RootElementIdType)31U
#define EXI_DIN_CERTIFICATE_UPDATE_RES_TYPE (Exi_RootElementIdType)32U
#define EXI_DIN_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE (Exi_RootElementIdType)33U
#define EXI_DIN_CHARGE_PARAMETER_DISCOVERY_RES_TYPE (Exi_RootElementIdType)34U
#define EXI_DIN_CHARGING_STATUS_REQ_TYPE (Exi_RootElementIdType)35U
#define EXI_DIN_CHARGING_STATUS_RES_TYPE (Exi_RootElementIdType)36U
#define EXI_DIN_CONTRACT_AUTHENTICATION_REQ_TYPE (Exi_RootElementIdType)37U
#define EXI_DIN_CONTRACT_AUTHENTICATION_RES_TYPE (Exi_RootElementIdType)38U
#define EXI_DIN_CURRENT_DEMAND_REQ_TYPE (Exi_RootElementIdType)39U
#define EXI_DIN_CURRENT_DEMAND_RES_TYPE (Exi_RootElementIdType)40U
#define EXI_DIN_DC_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)41U
#define EXI_DIN_DC_EVPOWER_DELIVERY_PARAMETER_TYPE (Exi_RootElementIdType)42U
#define EXI_DIN_DC_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)43U
#define EXI_DIN_DC_EVSESTATUS_TYPE (Exi_RootElementIdType)44U
#define EXI_DIN_DC_EVSTATUS_TYPE (Exi_RootElementIdType)45U
#define EXI_DIN_EVCHARGE_PARAMETER_TYPE (Exi_RootElementIdType)46U
#define EXI_DIN_EVPOWER_DELIVERY_PARAMETER_TYPE (Exi_RootElementIdType)47U
#define EXI_DIN_EVSECHARGE_PARAMETER_TYPE (Exi_RootElementIdType)48U
#define EXI_DIN_EVSESTATUS_TYPE (Exi_RootElementIdType)49U
#define EXI_DIN_EVSTATUS_TYPE (Exi_RootElementIdType)50U
#define EXI_DIN_ENTRY_TYPE (Exi_RootElementIdType)51U
#define EXI_DIN_INTERVAL_TYPE (Exi_RootElementIdType)52U
#define EXI_DIN_METERING_RECEIPT_REQ_TYPE (Exi_RootElementIdType)53U
#define EXI_DIN_METERING_RECEIPT_RES_TYPE (Exi_RootElementIdType)54U
#define EXI_DIN_PMAX_SCHEDULE_ENTRY_TYPE (Exi_RootElementIdType)55U
#define EXI_DIN_PAYMENT_DETAILS_REQ_TYPE (Exi_RootElementIdType)56U
#define EXI_DIN_PAYMENT_DETAILS_RES_TYPE (Exi_RootElementIdType)57U
#define EXI_DIN_POWER_DELIVERY_REQ_TYPE (Exi_RootElementIdType)58U
#define EXI_DIN_POWER_DELIVERY_RES_TYPE (Exi_RootElementIdType)59U
#define EXI_DIN_PRE_CHARGE_REQ_TYPE (Exi_RootElementIdType)60U
#define EXI_DIN_PRE_CHARGE_RES_TYPE (Exi_RootElementIdType)61U
#define EXI_DIN_RELATIVE_TIME_INTERVAL_TYPE (Exi_RootElementIdType)62U
#define EXI_DIN_SASCHEDULE_LIST_TYPE (Exi_RootElementIdType)63U
#define EXI_DIN_SASCHEDULES_TYPE (Exi_RootElementIdType)64U
#define EXI_DIN_SALES_TARIFF_ENTRY_TYPE (Exi_RootElementIdType)65U
#define EXI_DIN_SERVICE_CHARGE_TYPE (Exi_RootElementIdType)66U
#define EXI_DIN_SERVICE_DETAIL_REQ_TYPE (Exi_RootElementIdType)67U
#define EXI_DIN_SERVICE_DETAIL_RES_TYPE (Exi_RootElementIdType)68U
#define EXI_DIN_SERVICE_DISCOVERY_REQ_TYPE (Exi_RootElementIdType)69U
#define EXI_DIN_SERVICE_DISCOVERY_RES_TYPE (Exi_RootElementIdType)70U
#define EXI_DIN_SERVICE_PAYMENT_SELECTION_REQ_TYPE (Exi_RootElementIdType)71U
#define EXI_DIN_SERVICE_PAYMENT_SELECTION_RES_TYPE (Exi_RootElementIdType)72U
#define EXI_DIN_SESSION_SETUP_REQ_TYPE (Exi_RootElementIdType)73U
#define EXI_DIN_SESSION_SETUP_RES_TYPE (Exi_RootElementIdType)74U
#define EXI_DIN_SESSION_STOP_REQ_TYPE (Exi_RootElementIdType)75U
#define EXI_DIN_SESSION_STOP_RES_TYPE (Exi_RootElementIdType)76U
#define EXI_DIN_V2G_MESSAGE_TYPE (Exi_RootElementIdType)77U
#define EXI_DIN_WELDING_DETECTION_REQ_TYPE (Exi_RootElementIdType)78U
#define EXI_DIN_WELDING_DETECTION_RES_TYPE (Exi_RootElementIdType)79U
/* Elements */
#define EXI_DIN_BODY_TYPE (Exi_RootElementIdType)357U
#define EXI_DIN_BODY_ELEMENT_TYPE (Exi_RootElementIdType)358U
#define EXI_DIN_BULK_CHARGING_COMPLETE_TYPE (Exi_RootElementIdType)359U
#define EXI_DIN_BULK_SOC_TYPE (Exi_RootElementIdType)360U
#define EXI_DIN_CERTIFICATE_TYPE (Exi_RootElementIdType)361U
#define EXI_DIN_CHARGE_SERVICE_TYPE (Exi_RootElementIdType)362U
#define EXI_DIN_CHARGING_COMPLETE_TYPE (Exi_RootElementIdType)363U
#define EXI_DIN_CHARGING_PROFILE_TYPE (Exi_RootElementIdType)364U
#define EXI_DIN_CHARGING_PROFILE_ENTRY_MAX_POWER_TYPE (Exi_RootElementIdType)365U
#define EXI_DIN_CHARGING_PROFILE_ENTRY_START_TYPE (Exi_RootElementIdType)366U
#define EXI_DIN_CONSUMPTION_COST_TYPE (Exi_RootElementIdType)367U
#define EXI_DIN_CONTRACT_ID_TYPE (Exi_RootElementIdType)368U
#define EXI_DIN_CONTRACT_SIGNATURE_CERT_CHAIN_TYPE (Exi_RootElementIdType)369U
#define EXI_DIN_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY_TYPE (Exi_RootElementIdType)370U
#define EXI_DIN_COST_TYPE (Exi_RootElementIdType)371U
#define EXI_DIN_DHPARAMS_TYPE (Exi_RootElementIdType)372U
#define EXI_DIN_DATE_TIME_NOW_TYPE (Exi_RootElementIdType)373U
#define EXI_DIN_DEPARTURE_TIME_TYPE (Exi_RootElementIdType)374U
#define EXI_DIN_EAMOUNT_TYPE (Exi_RootElementIdType)375U
#define EXI_DIN_EPRICE_LEVEL_TYPE (Exi_RootElementIdType)376U
#define EXI_DIN_EVCCID_TYPE (Exi_RootElementIdType)377U
#define EXI_DIN_EVCABIN_CONDITIONING_TYPE (Exi_RootElementIdType)378U
#define EXI_DIN_EVENERGY_CAPACITY_TYPE (Exi_RootElementIdType)379U
#define EXI_DIN_EVENERGY_REQUEST_TYPE (Exi_RootElementIdType)380U
#define EXI_DIN_EVERROR_CODE_TYPE (Exi_RootElementIdType)381U
#define EXI_DIN_EVMAX_CURRENT_TYPE (Exi_RootElementIdType)382U
#define EXI_DIN_EVMAX_VOLTAGE_TYPE (Exi_RootElementIdType)383U
#define EXI_DIN_EVMAXIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)384U
#define EXI_DIN_EVMAXIMUM_POWER_LIMIT_TYPE (Exi_RootElementIdType)385U
#define EXI_DIN_EVMAXIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)386U
#define EXI_DIN_EVMIN_CURRENT_TYPE (Exi_RootElementIdType)387U
#define EXI_DIN_EVRESSCONDITIONING_TYPE (Exi_RootElementIdType)388U
#define EXI_DIN_EVRESSSOC_TYPE (Exi_RootElementIdType)389U
#define EXI_DIN_EVREADY_TYPE (Exi_RootElementIdType)390U
#define EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_TYPE (Exi_RootElementIdType)391U
#define EXI_DIN_EVSECURRENT_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)392U
#define EXI_DIN_EVSECURRENT_REGULATION_TOLERANCE_TYPE (Exi_RootElementIdType)393U
#define EXI_DIN_EVSEENERGY_TO_BE_DELIVERED_TYPE (Exi_RootElementIdType)394U
#define EXI_DIN_EVSEID_TYPE (Exi_RootElementIdType)395U
#define EXI_DIN_EVSEISOLATION_STATUS_TYPE (Exi_RootElementIdType)396U
#define EXI_DIN_EVSEMAX_CURRENT_TYPE (Exi_RootElementIdType)397U
#define EXI_DIN_EVSEMAX_VOLTAGE_TYPE (Exi_RootElementIdType)398U
#define EXI_DIN_EVSEMAXIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)399U
#define EXI_DIN_EVSEMAXIMUM_POWER_LIMIT_TYPE (Exi_RootElementIdType)400U
#define EXI_DIN_EVSEMAXIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)401U
#define EXI_DIN_EVSEMIN_CURRENT_TYPE (Exi_RootElementIdType)402U
#define EXI_DIN_EVSEMINIMUM_CURRENT_LIMIT_TYPE (Exi_RootElementIdType)403U
#define EXI_DIN_EVSEMINIMUM_VOLTAGE_LIMIT_TYPE (Exi_RootElementIdType)404U
#define EXI_DIN_EVSENOTIFICATION_TYPE (Exi_RootElementIdType)405U
#define EXI_DIN_EVSEPEAK_CURRENT_RIPPLE_TYPE (Exi_RootElementIdType)406U
#define EXI_DIN_EVSEPOWER_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)407U
#define EXI_DIN_EVSEPRESENT_CURRENT_TYPE (Exi_RootElementIdType)408U
#define EXI_DIN_EVSEPRESENT_VOLTAGE_TYPE (Exi_RootElementIdType)409U
#define EXI_DIN_EVSEPROCESSING_TYPE (Exi_RootElementIdType)410U
#define EXI_DIN_EVSESTATUS_CODE_TYPE (Exi_RootElementIdType)411U
#define EXI_DIN_EVSEVOLTAGE_LIMIT_ACHIEVED_TYPE (Exi_RootElementIdType)412U
#define EXI_DIN_EVTARGET_CURRENT_TYPE (Exi_RootElementIdType)413U
#define EXI_DIN_EVTARGET_VOLTAGE_TYPE (Exi_RootElementIdType)414U
#define EXI_DIN_ENERGY_TRANSFER_TYPE_TYPE (Exi_RootElementIdType)415U
#define EXI_DIN_FAULT_CODE_TYPE (Exi_RootElementIdType)416U
#define EXI_DIN_FAULT_MSG_TYPE (Exi_RootElementIdType)417U
#define EXI_DIN_FREE_SERVICE_TYPE (Exi_RootElementIdType)418U
#define EXI_DIN_FULL_SOC_TYPE (Exi_RootElementIdType)419U
#define EXI_DIN_GEN_CHALLENGE_TYPE (Exi_RootElementIdType)420U
#define EXI_DIN_HEADER_TYPE (Exi_RootElementIdType)421U
#define EXI_DIN_LIST_OF_ROOT_CERTIFICATE_IDS_TYPE (Exi_RootElementIdType)422U
#define EXI_DIN_METER_ID_TYPE (Exi_RootElementIdType)423U
#define EXI_DIN_METER_INFO_TYPE (Exi_RootElementIdType)424U
#define EXI_DIN_METER_READING_TYPE (Exi_RootElementIdType)425U
#define EXI_DIN_METER_STATUS_TYPE (Exi_RootElementIdType)426U
#define EXI_DIN_MULTIPLIER_TYPE (Exi_RootElementIdType)427U
#define EXI_DIN_NOTIFICATION_TYPE (Exi_RootElementIdType)428U
#define EXI_DIN_NOTIFICATION_MAX_DELAY_TYPE (Exi_RootElementIdType)429U
#define EXI_DIN_NUM_EPRICE_LEVELS_TYPE (Exi_RootElementIdType)430U
#define EXI_DIN_OEMPROVISIONING_CERT_TYPE (Exi_RootElementIdType)431U
#define EXI_DIN_PMAX_TYPE (Exi_RootElementIdType)432U
#define EXI_DIN_PMAX_SCHEDULE_TYPE (Exi_RootElementIdType)433U
#define EXI_DIN_PMAX_SCHEDULE_ID_TYPE (Exi_RootElementIdType)434U
#define EXI_DIN_PARAMETER_TYPE (Exi_RootElementIdType)435U
#define EXI_DIN_PARAMETER_SET_TYPE (Exi_RootElementIdType)436U
#define EXI_DIN_PARAMETER_SET_ID_TYPE (Exi_RootElementIdType)437U
#define EXI_DIN_PAYMENT_OPTION_TYPE (Exi_RootElementIdType)438U
#define EXI_DIN_PAYMENT_OPTIONS_TYPE (Exi_RootElementIdType)439U
#define EXI_DIN_POWER_SWITCH_CLOSED_TYPE (Exi_RootElementIdType)440U
#define EXI_DIN_PROFILE_ENTRY_TYPE (Exi_RootElementIdType)441U
#define EXI_DIN_RCD_TYPE (Exi_RootElementIdType)442U
#define EXI_DIN_READY_TO_CHARGE_STATE_TYPE (Exi_RootElementIdType)443U
#define EXI_DIN_RECEIPT_REQUIRED_TYPE (Exi_RootElementIdType)444U
#define EXI_DIN_REMAINING_TIME_TO_BULK_SOC_TYPE (Exi_RootElementIdType)445U
#define EXI_DIN_REMAINING_TIME_TO_FULL_SOC_TYPE (Exi_RootElementIdType)446U
#define EXI_DIN_RESPONSE_CODE_TYPE (Exi_RootElementIdType)447U
#define EXI_DIN_RETRY_COUNTER_TYPE (Exi_RootElementIdType)448U
#define EXI_DIN_ROOT_CERTIFICATE_ID_TYPE (Exi_RootElementIdType)449U
#define EXI_DIN_SASCHEDULE_TUPLE_TYPE (Exi_RootElementIdType)450U
#define EXI_DIN_SASCHEDULE_TUPLE_ID_TYPE (Exi_RootElementIdType)451U
#define EXI_DIN_SALES_TARIFF_TYPE (Exi_RootElementIdType)452U
#define EXI_DIN_SALES_TARIFF_DESCRIPTION_TYPE (Exi_RootElementIdType)453U
#define EXI_DIN_SALES_TARIFF_ID_TYPE (Exi_RootElementIdType)454U
#define EXI_DIN_SELECTED_PAYMENT_OPTION_TYPE (Exi_RootElementIdType)455U
#define EXI_DIN_SELECTED_SERVICE_TYPE (Exi_RootElementIdType)456U
#define EXI_DIN_SELECTED_SERVICE_LIST_TYPE (Exi_RootElementIdType)457U
#define EXI_DIN_SERVICE_TYPE (Exi_RootElementIdType)458U
#define EXI_DIN_SERVICE_CATEGORY_TYPE (Exi_RootElementIdType)459U
#define EXI_DIN_SERVICE_ID_TYPE (Exi_RootElementIdType)460U
#define EXI_DIN_SERVICE_LIST_TYPE (Exi_RootElementIdType)461U
#define EXI_DIN_SERVICE_NAME_TYPE (Exi_RootElementIdType)462U
#define EXI_DIN_SERVICE_PARAMETER_LIST_TYPE (Exi_RootElementIdType)463U
#define EXI_DIN_SERVICE_SCOPE_TYPE (Exi_RootElementIdType)464U
#define EXI_DIN_SERVICE_TAG_TYPE (Exi_RootElementIdType)465U
#define EXI_DIN_SESSION_ID_TYPE (Exi_RootElementIdType)466U
#define EXI_DIN_SIG_METER_READING_TYPE (Exi_RootElementIdType)467U
#define EXI_DIN_SIGNATURE_TYPE (Exi_RootElementIdType)468U
#define EXI_DIN_SUB_CERTIFICATES_TYPE (Exi_RootElementIdType)469U
#define EXI_DIN_TMETER_TYPE (Exi_RootElementIdType)470U
#define EXI_DIN_TIME_INTERVAL_TYPE (Exi_RootElementIdType)471U
#define EXI_DIN_UNIT_TYPE (Exi_RootElementIdType)472U
#define EXI_DIN_VALUE_TYPE (Exi_RootElementIdType)473U
#define EXI_DIN_AMOUNT_TYPE (Exi_RootElementIdType)474U
#define EXI_DIN_AMOUNT_MULTIPLIER_TYPE (Exi_RootElementIdType)475U
#define EXI_DIN_BOOL_VALUE_TYPE (Exi_RootElementIdType)476U
#define EXI_DIN_BYTE_VALUE_TYPE (Exi_RootElementIdType)477U
#define EXI_DIN_COST_KIND_TYPE (Exi_RootElementIdType)478U
#define EXI_DIN_DURATION_TYPE (Exi_RootElementIdType)479U
#define EXI_DIN_INT_VALUE_TYPE (Exi_RootElementIdType)480U
#define EXI_DIN_PHYSICAL_VALUE_TYPE (Exi_RootElementIdType)481U
#define EXI_DIN_SHORT_VALUE_TYPE (Exi_RootElementIdType)482U
#define EXI_DIN_START_TYPE (Exi_RootElementIdType)483U
#define EXI_DIN_START_VALUE_TYPE (Exi_RootElementIdType)484U
#define EXI_DIN_STRING_VALUE_TYPE (Exi_RootElementIdType)485U
#define EXI_SCHEMA_SET_DIN_TYPE (Exi_RootElementIdType)3U
#ifndef EXI_MAXOCCURS_DIN_CERTIFICATE
  #define EXI_MAXOCCURS_DIN_CERTIFICATE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_CONSUMPTIONCOST
  #define EXI_MAXOCCURS_DIN_CONSUMPTIONCOST EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_COST
  #define EXI_MAXOCCURS_DIN_COST EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_PARAMETER
  #define EXI_MAXOCCURS_DIN_PARAMETER EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_PARAMETERSET
  #define EXI_MAXOCCURS_DIN_PARAMETERSET EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_PAYMENTOPTION
  #define EXI_MAXOCCURS_DIN_PAYMENTOPTION EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_PMAXSCHEDULEENTRY
  #define EXI_MAXOCCURS_DIN_PMAXSCHEDULEENTRY EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_PROFILEENTRY
  #define EXI_MAXOCCURS_DIN_PROFILEENTRY EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_ROOTCERTIFICATEID
  #define EXI_MAXOCCURS_DIN_ROOTCERTIFICATEID EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_SALESTARIFFENTRY
  #define EXI_MAXOCCURS_DIN_SALESTARIFFENTRY EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_SASCHEDULETUPLE
  #define EXI_MAXOCCURS_DIN_SASCHEDULETUPLE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_SELECTEDSERVICE
  #define EXI_MAXOCCURS_DIN_SELECTEDSERVICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_DIN_SERVICE
  #define EXI_MAXOCCURS_DIN_SERVICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID
  #define EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME
  #define EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE
  #define EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct Exi_DIN_sessionIDType Exi_DIN_sessionIDType;

struct Exi_DIN_sessionIDType
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
  EXI_DIN_FAULT_CODE_TYPE_PARSING_ERROR,
  EXI_DIN_FAULT_CODE_TYPE_NO_TLSROOT_CERTIFICAT_AVAILABLE,
  EXI_DIN_FAULT_CODE_TYPE_UNKNOWN_ERROR
} Exi_DIN_faultCodeType;

typedef struct Exi_DIN_faultMsgType Exi_DIN_faultMsgType;

struct Exi_DIN_faultMsgType
{
  uint16 Length;
  uint8 Buffer[64];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_NotificationType Exi_DIN_NotificationType;

struct Exi_DIN_NotificationType
{
  Exi_DIN_faultMsgType* FaultMsg;
  Exi_DIN_faultCodeType FaultCode;
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

typedef struct Exi_DIN_MessageHeaderType Exi_DIN_MessageHeaderType;

struct Exi_DIN_MessageHeaderType
{
  Exi_DIN_NotificationType* Notification;
  Exi_DIN_sessionIDType* SessionID;
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

typedef void Exi_DIN_BodyBaseType;

typedef struct Exi_DIN_BodyType Exi_DIN_BodyType;

struct Exi_DIN_BodyType
{
  Exi_DIN_BodyBaseType* BodyElement;
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

typedef struct Exi_DIN_V2G_MessageType Exi_DIN_V2G_MessageType;

struct Exi_DIN_V2G_MessageType
{
  Exi_DIN_BodyType* Body;
  Exi_DIN_MessageHeaderType* Header;
};

typedef struct Exi_DIN_evccIDType Exi_DIN_evccIDType;

struct Exi_DIN_evccIDType
{
  uint16 Length;
  uint8 Buffer[8];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_SessionSetupReqType Exi_DIN_SessionSetupReqType;

struct Exi_DIN_SessionSetupReqType
{
  Exi_DIN_evccIDType* EVCCID;
};

typedef enum
{
  EXI_DIN_RESPONSE_CODE_TYPE_OK,
  EXI_DIN_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED,
  EXI_DIN_RESPONSE_CODE_TYPE_OK_OLD_SESSION_JOINED,
  EXI_DIN_RESPONSE_CODE_TYPE_OK_CERTIFICATE_EXPIRES_SOON,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_SEQUENCE_ERROR,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_SERVICE_IDINVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_UNKNOWN_SESSION,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_SERVICE_SELECTION_INVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_PAYMENT_SELECTION_INVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_EXPIRED,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_SIGNATURE_ERROR,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_NO_CERTIFICATE_AVAILABLE,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_CERT_CHAIN_ERROR,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_CHALLENGE_INVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_CONTRACT_CANCELED,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_WRONG_CHARGE_PARAMETER,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_POWER_DELIVERY_NOT_APPLIED,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_TARIFF_SELECTION_INVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_CHARGING_PROFILE_INVALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_EVSEPRESENT_VOLTAGE_TO_LOW,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_METERING_SIGNATURE_NOT_VALID,
  EXI_DIN_RESPONSE_CODE_TYPE_FAILED_WRONG_ENERGY_TRANSFER_TYPE
} Exi_DIN_responseCodeType;

typedef struct Exi_DIN_evseIDType Exi_DIN_evseIDType;

struct Exi_DIN_evseIDType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_SessionSetupResType Exi_DIN_SessionSetupResType;

struct Exi_DIN_SessionSetupResType
{
  Exi_DIN_evseIDType* EVSEID;
  Exi_SInt64 DateTimeNow;
  Exi_DIN_responseCodeType ResponseCode;
  Exi_BitType DateTimeNowFlag : 1;
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

typedef struct Exi_DIN_serviceScopeType Exi_DIN_serviceScopeType;

struct Exi_DIN_serviceScopeType
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
  EXI_DIN_SERVICE_CATEGORY_TYPE_EVCHARGING,
  EXI_DIN_SERVICE_CATEGORY_TYPE_INTERNET,
  EXI_DIN_SERVICE_CATEGORY_TYPE_CONTRACT_CERTIFICATE,
  EXI_DIN_SERVICE_CATEGORY_TYPE_OTHER_CUSTOM
} Exi_DIN_serviceCategoryType;

typedef struct Exi_DIN_ServiceDiscoveryReqType Exi_DIN_ServiceDiscoveryReqType;

struct Exi_DIN_ServiceDiscoveryReqType
{
  Exi_DIN_serviceScopeType* ServiceScope;
  Exi_DIN_serviceCategoryType ServiceCategory;
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
  EXI_DIN_PAYMENT_OPTION_TYPE_CONTRACT,
  EXI_DIN_PAYMENT_OPTION_TYPE_EXTERNAL_PAYMENT
} Exi_DIN_paymentOptionType;

typedef struct Exi_DIN_PaymentOptionsType Exi_DIN_PaymentOptionsType;

struct Exi_DIN_PaymentOptionsType
{
  uint16 PaymentOptionCount;
  Exi_DIN_paymentOptionType PaymentOption[EXI_MAXOCCURS_DIN_PAYMENTOPTION];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE * ( EXI_MAXOCCURS_DIN_PAYMENTOPTION )) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE * ( EXI_MAXOCCURS_DIN_PAYMENTOPTION )) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE * ( EXI_MAXOCCURS_DIN_PAYMENTOPTION )) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_serviceNameType Exi_DIN_serviceNameType;

struct Exi_DIN_serviceNameType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ServiceTagType Exi_DIN_ServiceTagType;

struct Exi_DIN_ServiceTagType
{
  Exi_DIN_serviceNameType* ServiceName;
  Exi_DIN_serviceScopeType* ServiceScope;
  uint16 ServiceID;
  Exi_DIN_serviceCategoryType ServiceCategory;
  Exi_BitType ServiceNameFlag : 1;
  Exi_BitType ServiceScopeFlag : 1;
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
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_SINGLE_PHASE_CORE,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_THREE_PHASE_CORE,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_CORE,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_EXTENDED,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_COMBO_CORE,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_DC_DUAL,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_CORE1P_DC_EXTENDED,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_SINGLE_DC_CORE,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_SINGLE_PHASE_THREE_PHASE_CORE_DC_EXTENDED,
  EXI_DIN_EVSESUPPORTED_ENERGY_TRANSFER_TYPE_AC_CORE3P_DC_EXTENDED
} Exi_DIN_EVSESupportedEnergyTransferType;

typedef struct Exi_DIN_ServiceChargeType Exi_DIN_ServiceChargeType;

struct Exi_DIN_ServiceChargeType
{
  Exi_DIN_ServiceTagType* ServiceTag;
  Exi_DIN_EVSESupportedEnergyTransferType EnergyTransferType;
  boolean FreeService;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ServiceType Exi_DIN_ServiceType;

struct Exi_DIN_ServiceType
{
  struct Exi_DIN_ServiceType* NextServicePtr;
  Exi_DIN_ServiceTagType* ServiceTag;
  boolean FreeService;
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

typedef struct Exi_DIN_ServiceTagListType Exi_DIN_ServiceTagListType;

struct Exi_DIN_ServiceTagListType
{
  Exi_DIN_ServiceType* Service;
};

typedef struct Exi_DIN_ServiceDiscoveryResType Exi_DIN_ServiceDiscoveryResType;

struct Exi_DIN_ServiceDiscoveryResType
{
  Exi_DIN_ServiceChargeType* ChargeService;
  Exi_DIN_PaymentOptionsType* PaymentOptions;
  Exi_DIN_ServiceTagListType* ServiceList;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_ServiceDetailReqType Exi_DIN_ServiceDetailReqType;

struct Exi_DIN_ServiceDetailReqType
{
  uint16 ServiceID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_AttributeNameType Exi_DIN_AttributeNameType;

struct Exi_DIN_AttributeNameType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTENAME ) + 2 ) % 4)];
# endif
#endif 
};

typedef enum
{
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_BOOL,
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_BYTE,
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_SHORT,
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_INT,
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_PHYSICAL_VALUE,
  EXI_DIN_ATTRIBUTE_VALUE_TYPE_STRING
} Exi_DIN_AttributeValueType;

typedef enum
{
  EXI_DIN_UNIT_SYMBOL_TYPE_H,
  EXI_DIN_UNIT_SYMBOL_TYPE_M,
  EXI_DIN_UNIT_SYMBOL_TYPE_S,
  EXI_DIN_UNIT_SYMBOL_TYPE_A,
  EXI_DIN_UNIT_SYMBOL_TYPE_AH,
  EXI_DIN_UNIT_SYMBOL_TYPE_V,
  EXI_DIN_UNIT_SYMBOL_TYPE_VA,
  EXI_DIN_UNIT_SYMBOL_TYPE_W,
  EXI_DIN_UNIT_SYMBOL_TYPE_W_S,
  EXI_DIN_UNIT_SYMBOL_TYPE_WH
} Exi_DIN_unitSymbolType;

typedef struct Exi_DIN_PhysicalValueType Exi_DIN_PhysicalValueType;

struct Exi_DIN_PhysicalValueType
{
  sint16 Value;
  Exi_DIN_unitSymbolType Unit;
  sint8 Multiplier;
  Exi_BitType UnitFlag : 1;
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

typedef struct Exi_DIN_ParameterChoiceType Exi_DIN_ParameterChoiceType;

struct Exi_DIN_ParameterChoiceType
{
  union
  {
    Exi_DIN_PhysicalValueType* physicalValue;
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

typedef struct Exi_DIN_ParameterType Exi_DIN_ParameterType;

struct Exi_DIN_ParameterType
{
  Exi_DIN_ParameterChoiceType* ChoiceElement;
  Exi_DIN_AttributeNameType* Name;
  struct Exi_DIN_ParameterType* NextParameterPtr;
  Exi_DIN_AttributeValueType ValueType;
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

typedef struct Exi_DIN_ParameterSetType Exi_DIN_ParameterSetType;

struct Exi_DIN_ParameterSetType
{
  struct Exi_DIN_ParameterSetType* NextParameterSetPtr;
  Exi_DIN_ParameterType* Parameter;
  sint16 ParameterSetID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ServiceParameterListType Exi_DIN_ServiceParameterListType;

struct Exi_DIN_ServiceParameterListType
{
  Exi_DIN_ParameterSetType* ParameterSet;
};

typedef struct Exi_DIN_ServiceDetailResType Exi_DIN_ServiceDetailResType;

struct Exi_DIN_ServiceDetailResType
{
  Exi_DIN_ServiceParameterListType* ServiceParameterList;
  uint16 ServiceID;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_SelectedServiceType Exi_DIN_SelectedServiceType;

struct Exi_DIN_SelectedServiceType
{
  struct Exi_DIN_SelectedServiceType* NextSelectedServicePtr;
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

typedef struct Exi_DIN_SelectedServiceListType Exi_DIN_SelectedServiceListType;

struct Exi_DIN_SelectedServiceListType
{
  Exi_DIN_SelectedServiceType* SelectedService;
};

typedef struct Exi_DIN_ServicePaymentSelectionReqType Exi_DIN_ServicePaymentSelectionReqType;

struct Exi_DIN_ServicePaymentSelectionReqType
{
  Exi_DIN_SelectedServiceListType* SelectedServiceList;
  Exi_DIN_paymentOptionType SelectedPaymentOption;
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

typedef struct Exi_DIN_ServicePaymentSelectionResType Exi_DIN_ServicePaymentSelectionResType;

struct Exi_DIN_ServicePaymentSelectionResType
{
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_contractIDType Exi_DIN_contractIDType;

struct Exi_DIN_contractIDType
{
  uint16 Length;
  uint8 Buffer[24];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_certificateType Exi_DIN_certificateType;

struct Exi_DIN_certificateType
{
  struct Exi_DIN_certificateType* NextCertificatePtr;
  uint16 Length;
  uint8 Buffer[1200];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_SubCertificatesType Exi_DIN_SubCertificatesType;

struct Exi_DIN_SubCertificatesType
{
  Exi_DIN_certificateType* Certificate;
};

typedef struct Exi_DIN_CertificateChainType Exi_DIN_CertificateChainType;

struct Exi_DIN_CertificateChainType
{
  Exi_DIN_certificateType* Certificate;
  Exi_DIN_SubCertificatesType* SubCertificates;
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

typedef struct Exi_DIN_PaymentDetailsReqType Exi_DIN_PaymentDetailsReqType;

struct Exi_DIN_PaymentDetailsReqType
{
  Exi_DIN_contractIDType* ContractID;
  Exi_DIN_CertificateChainType* ContractSignatureCertChain;
};

typedef struct Exi_DIN_genChallengeType Exi_DIN_genChallengeType;

struct Exi_DIN_genChallengeType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_DIN_GENCHALLENGE ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_PaymentDetailsResType Exi_DIN_PaymentDetailsResType;

struct Exi_DIN_PaymentDetailsResType
{
  Exi_DIN_genChallengeType* GenChallenge;
  Exi_SInt64 DateTimeNow;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_AttributeIdType Exi_DIN_AttributeIdType;

struct Exi_DIN_AttributeIdType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_DIN_ATTRIBUTEID ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ContractAuthenticationReqType Exi_DIN_ContractAuthenticationReqType;

struct Exi_DIN_ContractAuthenticationReqType
{
  Exi_DIN_genChallengeType* GenChallenge;
  Exi_DIN_AttributeIdType* Id;
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
  EXI_DIN_EVSEPROCESSING_TYPE_FINISHED,
  EXI_DIN_EVSEPROCESSING_TYPE_ONGOING
} Exi_DIN_EVSEProcessingType;

typedef struct Exi_DIN_ContractAuthenticationResType Exi_DIN_ContractAuthenticationResType;

struct Exi_DIN_ContractAuthenticationResType
{
  Exi_DIN_EVSEProcessingType EVSEProcessing;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef enum
{
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_AC_SINGLE_PHASE_CORE,
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_AC_THREE_PHASE_CORE,
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_CORE,
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_EXTENDED,
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_COMBO_CORE,
  EXI_DIN_EVREQUESTED_ENERGY_TRANSFER_TYPE_DC_UNIQUE
} Exi_DIN_EVRequestedEnergyTransferType;

typedef void Exi_DIN_EVChargeParameterType;

typedef struct Exi_DIN_ChargeParameterDiscoveryReqType Exi_DIN_ChargeParameterDiscoveryReqType;

struct Exi_DIN_ChargeParameterDiscoveryReqType
{
  Exi_DIN_EVChargeParameterType* EVChargeParameter;
  Exi_RootElementIdType EVChargeParameterElementId;
  Exi_DIN_EVRequestedEnergyTransferType EVRequestedEnergyTransferType;
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

typedef void Exi_DIN_SASchedulesType;

typedef void Exi_DIN_EVSEChargeParameterType;

typedef struct Exi_DIN_ChargeParameterDiscoveryResType Exi_DIN_ChargeParameterDiscoveryResType;

struct Exi_DIN_ChargeParameterDiscoveryResType
{
  Exi_DIN_EVSEChargeParameterType* EVSEChargeParameter;
  Exi_DIN_SASchedulesType* SASchedules;
  Exi_RootElementIdType EVSEChargeParameterElementId;
  Exi_RootElementIdType SASchedulesElementId;
  Exi_DIN_EVSEProcessingType EVSEProcessing;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_ProfileEntryType Exi_DIN_ProfileEntryType;

struct Exi_DIN_ProfileEntryType
{
  struct Exi_DIN_ProfileEntryType* NextProfileEntryPtr;
  uint32 ChargingProfileEntryStart;
  sint16 ChargingProfileEntryMaxPower;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ChargingProfileType Exi_DIN_ChargingProfileType;

struct Exi_DIN_ChargingProfileType
{
  Exi_DIN_ProfileEntryType* ProfileEntry;
  sint16 SAScheduleTupleID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef void Exi_DIN_EVPowerDeliveryParameterType;

typedef struct Exi_DIN_PowerDeliveryReqType Exi_DIN_PowerDeliveryReqType;

struct Exi_DIN_PowerDeliveryReqType
{
  Exi_DIN_ChargingProfileType* ChargingProfile;
  Exi_DIN_EVPowerDeliveryParameterType* EVPowerDeliveryParameter;
  Exi_RootElementIdType EVPowerDeliveryParameterElementId;
  boolean ReadyToChargeState;
  Exi_BitType ChargingProfileFlag : 1;
  Exi_BitType EVPowerDeliveryParameterFlag : 1;
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

typedef void Exi_DIN_EVSEStatusType;

typedef struct Exi_DIN_PowerDeliveryResType Exi_DIN_PowerDeliveryResType;

struct Exi_DIN_PowerDeliveryResType
{
  Exi_DIN_EVSEStatusType* EVSEStatus;
  Exi_RootElementIdType EVSEStatusElementId;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef void Exi_DIN_ChargingStatusReqType;

typedef struct Exi_DIN_meterIDType Exi_DIN_meterIDType;

struct Exi_DIN_meterIDType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_sigMeterReadingType Exi_DIN_sigMeterReadingType;

struct Exi_DIN_sigMeterReadingType
{
  uint16 Length;
  uint8 Buffer[32];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_MeterInfoType Exi_DIN_MeterInfoType;

struct Exi_DIN_MeterInfoType
{
  Exi_DIN_meterIDType* MeterID;
  Exi_DIN_PhysicalValueType* MeterReading;
  Exi_DIN_sigMeterReadingType* SigMeterReading;
  Exi_SInt64 TMeter;
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

typedef enum
{
  EXI_DIN_EVSENOTIFICATION_TYPE_NONE,
  EXI_DIN_EVSENOTIFICATION_TYPE_STOP_CHARGING,
  EXI_DIN_EVSENOTIFICATION_TYPE_RE_NEGOTIATION
} Exi_DIN_EVSENotificationType;

typedef struct Exi_DIN_AC_EVSEStatusType Exi_DIN_AC_EVSEStatusType;

struct Exi_DIN_AC_EVSEStatusType
{
  uint32 NotificationMaxDelay;
  Exi_DIN_EVSENotificationType EVSENotification;
  boolean PowerSwitchClosed;
  boolean RCD;
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

typedef struct Exi_DIN_ChargingStatusResType Exi_DIN_ChargingStatusResType;

struct Exi_DIN_ChargingStatusResType
{
  Exi_DIN_AC_EVSEStatusType* AC_EVSEStatus;
  Exi_DIN_evseIDType* EVSEID;
  Exi_DIN_PhysicalValueType* EVSEMaxCurrent;
  Exi_DIN_MeterInfoType* MeterInfo;
  sint16 SAScheduleTupleID;
  Exi_DIN_responseCodeType ResponseCode;
  boolean ReceiptRequired;
  Exi_BitType EVSEMaxCurrentFlag : 1;
  Exi_BitType MeterInfoFlag : 1;
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

typedef struct Exi_DIN_MeteringReceiptReqType Exi_DIN_MeteringReceiptReqType;

struct Exi_DIN_MeteringReceiptReqType
{
  Exi_DIN_AttributeIdType* Id;
  Exi_DIN_MeterInfoType* MeterInfo;
  Exi_DIN_sessionIDType* SessionID;
  sint16 SAScheduleTupleID;
  Exi_BitType IdFlag : 1;
  Exi_BitType SAScheduleTupleIDFlag : 1;
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

typedef struct Exi_DIN_MeteringReceiptResType Exi_DIN_MeteringReceiptResType;

struct Exi_DIN_MeteringReceiptResType
{
  Exi_DIN_AC_EVSEStatusType* AC_EVSEStatus;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef void Exi_DIN_SessionStopReqType;

typedef struct Exi_DIN_SessionStopResType Exi_DIN_SessionStopResType;

struct Exi_DIN_SessionStopResType
{
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_rootCertificateIDType Exi_DIN_rootCertificateIDType;

struct Exi_DIN_rootCertificateIDType
{
  struct Exi_DIN_rootCertificateIDType* NextRootCertificateIDPtr;
  uint16 Length;
  uint8 Buffer[40];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_ListOfRootCertificateIDsType Exi_DIN_ListOfRootCertificateIDsType;

struct Exi_DIN_ListOfRootCertificateIDsType
{
  Exi_DIN_rootCertificateIDType* RootCertificateID;
};

typedef struct Exi_DIN_dHParamsType Exi_DIN_dHParamsType;

struct Exi_DIN_dHParamsType
{
  uint16 Length;
  uint8 Buffer[256];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_CertificateUpdateReqType Exi_DIN_CertificateUpdateReqType;

struct Exi_DIN_CertificateUpdateReqType
{
  Exi_DIN_contractIDType* ContractID;
  Exi_DIN_CertificateChainType* ContractSignatureCertChain;
  Exi_DIN_dHParamsType* DHParams;
  Exi_DIN_AttributeIdType* Id;
  Exi_DIN_ListOfRootCertificateIDsType* ListOfRootCertificateIDs;
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

typedef struct Exi_DIN_privateKeyType Exi_DIN_privateKeyType;

struct Exi_DIN_privateKeyType
{
  uint16 Length;
  uint8 Buffer[128];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_CertificateUpdateResType Exi_DIN_CertificateUpdateResType;

struct Exi_DIN_CertificateUpdateResType
{
  Exi_DIN_contractIDType* ContractID;
  Exi_DIN_CertificateChainType* ContractSignatureCertChain;
  Exi_DIN_privateKeyType* ContractSignatureEncryptedPrivateKey;
  Exi_DIN_dHParamsType* DHParams;
  Exi_DIN_AttributeIdType* Id;
  sint16 RetryCounter;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_CertificateInstallationReqType Exi_DIN_CertificateInstallationReqType;

struct Exi_DIN_CertificateInstallationReqType
{
  Exi_DIN_dHParamsType* DHParams;
  Exi_DIN_AttributeIdType* Id;
  Exi_DIN_ListOfRootCertificateIDsType* ListOfRootCertificateIDs;
  Exi_DIN_certificateType* OEMProvisioningCert;
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

typedef struct Exi_DIN_CertificateInstallationResType Exi_DIN_CertificateInstallationResType;

struct Exi_DIN_CertificateInstallationResType
{
  Exi_DIN_contractIDType* ContractID;
  Exi_DIN_CertificateChainType* ContractSignatureCertChain;
  Exi_DIN_privateKeyType* ContractSignatureEncryptedPrivateKey;
  Exi_DIN_dHParamsType* DHParams;
  Exi_DIN_AttributeIdType* Id;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef enum
{
  EXI_DIN_DC_EVERROR_CODE_TYPE_NO_ERROR,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_RESSTEMPERATURE_INHIBIT,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_EVSHIFT_POSITION,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_CHARGER_CONNECTOR_LOCK_FAULT,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_EVRESSMALFUNCTION,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_CURRENTDIFFERENTIAL,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_VOLTAGE_OUT_OF_RANGE,
  EXI_DIN_DC_EVERROR_CODE_TYPE_RESERVED_A,
  EXI_DIN_DC_EVERROR_CODE_TYPE_RESERVED_B,
  EXI_DIN_DC_EVERROR_CODE_TYPE_RESERVED_C,
  EXI_DIN_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_SYSTEM_INCOMPATIBILITY,
  EXI_DIN_DC_EVERROR_CODE_TYPE_NO_DATA
} Exi_DIN_DC_EVErrorCodeType;

typedef struct Exi_DIN_DC_EVStatusType Exi_DIN_DC_EVStatusType;

struct Exi_DIN_DC_EVStatusType
{
  Exi_DIN_DC_EVErrorCodeType EVErrorCode;
  boolean EVCabinConditioning;
  boolean EVRESSConditioning;
  boolean EVReady;
  sint8 EVRESSSOC;
  Exi_BitType EVCabinConditioningFlag : 1;
  Exi_BitType EVRESSConditioningFlag : 1;
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

typedef struct Exi_DIN_CableCheckReqType Exi_DIN_CableCheckReqType;

struct Exi_DIN_CableCheckReqType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
};

typedef enum
{
  EXI_DIN_ISOLATION_LEVEL_TYPE_INVALID,
  EXI_DIN_ISOLATION_LEVEL_TYPE_VALID,
  EXI_DIN_ISOLATION_LEVEL_TYPE_WARNING,
  EXI_DIN_ISOLATION_LEVEL_TYPE_FAULT
} Exi_DIN_isolationLevelType;

typedef enum
{
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_NOT_READY,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_READY,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_SHUTDOWN,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_UTILITY_INTERRUPT_EVENT,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_ISOLATION_MONITORING_ACTIVE,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_EMERGENCY_SHUTDOWN,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_EVSE_MALFUNCTION,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_RESERVED_8,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_RESERVED_9,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_RESERVED_A,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_RESERVED_B,
  EXI_DIN_DC_EVSESTATUS_CODE_TYPE_RESERVED_C
} Exi_DIN_DC_EVSEStatusCodeType;

typedef struct Exi_DIN_DC_EVSEStatusType Exi_DIN_DC_EVSEStatusType;

struct Exi_DIN_DC_EVSEStatusType
{
  uint32 NotificationMaxDelay;
  Exi_DIN_DC_EVSEStatusCodeType EVSEStatusCode;
  Exi_DIN_EVSENotificationType EVSENotification;
  Exi_DIN_isolationLevelType EVSEIsolationStatus;
  Exi_BitType EVSEIsolationStatusFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (3 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_CableCheckResType Exi_DIN_CableCheckResType;

struct Exi_DIN_CableCheckResType
{
  Exi_DIN_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_DIN_EVSEProcessingType EVSEProcessing;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_PreChargeReqType Exi_DIN_PreChargeReqType;

struct Exi_DIN_PreChargeReqType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
  Exi_DIN_PhysicalValueType* EVTargetCurrent;
  Exi_DIN_PhysicalValueType* EVTargetVoltage;
};

typedef struct Exi_DIN_PreChargeResType Exi_DIN_PreChargeResType;

struct Exi_DIN_PreChargeResType
{
  Exi_DIN_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_DIN_PhysicalValueType* EVSEPresentVoltage;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef struct Exi_DIN_CurrentDemandReqType Exi_DIN_CurrentDemandReqType;

struct Exi_DIN_CurrentDemandReqType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
  Exi_DIN_PhysicalValueType* EVMaximumCurrentLimit;
  Exi_DIN_PhysicalValueType* EVMaximumPowerLimit;
  Exi_DIN_PhysicalValueType* EVMaximumVoltageLimit;
  Exi_DIN_PhysicalValueType* EVTargetCurrent;
  Exi_DIN_PhysicalValueType* EVTargetVoltage;
  Exi_DIN_PhysicalValueType* RemainingTimeToBulkSoC;
  Exi_DIN_PhysicalValueType* RemainingTimeToFullSoC;
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

typedef struct Exi_DIN_CurrentDemandResType Exi_DIN_CurrentDemandResType;

struct Exi_DIN_CurrentDemandResType
{
  Exi_DIN_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_DIN_PhysicalValueType* EVSEMaximumCurrentLimit;
  Exi_DIN_PhysicalValueType* EVSEMaximumPowerLimit;
  Exi_DIN_PhysicalValueType* EVSEMaximumVoltageLimit;
  Exi_DIN_PhysicalValueType* EVSEPresentCurrent;
  Exi_DIN_PhysicalValueType* EVSEPresentVoltage;
  Exi_DIN_responseCodeType ResponseCode;
  boolean EVSECurrentLimitAchieved;
  boolean EVSEPowerLimitAchieved;
  boolean EVSEVoltageLimitAchieved;
  Exi_BitType EVSEMaximumCurrentLimitFlag : 1;
  Exi_BitType EVSEMaximumPowerLimitFlag : 1;
  Exi_BitType EVSEMaximumVoltageLimitFlag : 1;
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

typedef struct Exi_DIN_WeldingDetectionReqType Exi_DIN_WeldingDetectionReqType;

struct Exi_DIN_WeldingDetectionReqType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
};

typedef struct Exi_DIN_WeldingDetectionResType Exi_DIN_WeldingDetectionResType;

struct Exi_DIN_WeldingDetectionResType
{
  Exi_DIN_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_DIN_PhysicalValueType* EVSEPresentVoltage;
  Exi_DIN_responseCodeType ResponseCode;
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

typedef void Exi_DIN_IntervalType;

typedef struct Exi_DIN_PMaxScheduleEntryType Exi_DIN_PMaxScheduleEntryType;

struct Exi_DIN_PMaxScheduleEntryType
{
  struct Exi_DIN_PMaxScheduleEntryType* NextPMaxScheduleEntryPtr;
  Exi_DIN_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
  sint16 PMax;
};

typedef struct Exi_DIN_PMaxScheduleType Exi_DIN_PMaxScheduleType;

struct Exi_DIN_PMaxScheduleType
{
  Exi_DIN_PMaxScheduleEntryType* PMaxScheduleEntry;
  sint16 PMaxScheduleID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_tariffDescriptionType Exi_DIN_tariffDescriptionType;

struct Exi_DIN_tariffDescriptionType
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
  EXI_DIN_COST_KIND_TYPE_RELATIVE_PRICE_PERCENTAGE,
  EXI_DIN_COST_KIND_TYPE_RENEWABLE_GENERATION_PERCENTAGE,
  EXI_DIN_COST_KIND_TYPE_CARBON_DIOXIDE_EMISSION
} Exi_DIN_costKindType;

typedef struct Exi_DIN_CostType Exi_DIN_CostType;

struct Exi_DIN_CostType
{
  struct Exi_DIN_CostType* NextCostPtr;
  uint32 amount;
  Exi_DIN_costKindType costKind;
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

typedef struct Exi_DIN_ConsumptionCostType Exi_DIN_ConsumptionCostType;

struct Exi_DIN_ConsumptionCostType
{
  Exi_DIN_CostType* Cost;
  struct Exi_DIN_ConsumptionCostType* NextConsumptionCostPtr;
  uint32 startValue;
  Exi_BitType CostFlag : 1;
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

typedef struct Exi_DIN_SalesTariffEntryType Exi_DIN_SalesTariffEntryType;

struct Exi_DIN_SalesTariffEntryType
{
  Exi_DIN_ConsumptionCostType* ConsumptionCost;
  struct Exi_DIN_SalesTariffEntryType* NextSalesTariffEntryPtr;
  Exi_DIN_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
  uint8 EPriceLevel;
  Exi_BitType ConsumptionCostFlag : 1;
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

typedef struct Exi_DIN_SalesTariffType Exi_DIN_SalesTariffType;

struct Exi_DIN_SalesTariffType
{
  Exi_DIN_AttributeIdType* Id;
  Exi_DIN_tariffDescriptionType* SalesTariffDescription;
  Exi_DIN_SalesTariffEntryType* SalesTariffEntry;
  sint16 SalesTariffID;
  uint8 NumEPriceLevels;
  Exi_BitType SalesTariffDescriptionFlag : 1;
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

typedef struct Exi_DIN_SAScheduleTupleType Exi_DIN_SAScheduleTupleType;

struct Exi_DIN_SAScheduleTupleType
{
  struct Exi_DIN_SAScheduleTupleType* NextSAScheduleTuplePtr;
  Exi_DIN_PMaxScheduleType* PMaxSchedule;
  Exi_DIN_SalesTariffType* SalesTariff;
  sint16 SAScheduleTupleID;
  Exi_BitType SalesTariffFlag : 1;
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

typedef struct Exi_DIN_SAScheduleListType Exi_DIN_SAScheduleListType;

struct Exi_DIN_SAScheduleListType
{
  Exi_DIN_SAScheduleTupleType* SAScheduleTuple;
};

typedef struct Exi_DIN_EntryType Exi_DIN_EntryType;

struct Exi_DIN_EntryType
{
  Exi_DIN_IntervalType* TimeInterval;
  Exi_RootElementIdType TimeIntervalElementId;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_DIN_RelativeTimeIntervalType Exi_DIN_RelativeTimeIntervalType;

struct Exi_DIN_RelativeTimeIntervalType
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

typedef void Exi_DIN_EVStatusType;

typedef struct Exi_DIN_AC_EVChargeParameterType Exi_DIN_AC_EVChargeParameterType;

struct Exi_DIN_AC_EVChargeParameterType
{
  Exi_DIN_PhysicalValueType* EAmount;
  Exi_DIN_PhysicalValueType* EVMaxCurrent;
  Exi_DIN_PhysicalValueType* EVMaxVoltage;
  Exi_DIN_PhysicalValueType* EVMinCurrent;
  uint32 DepartureTime;
};

typedef struct Exi_DIN_DC_EVChargeParameterType Exi_DIN_DC_EVChargeParameterType;

struct Exi_DIN_DC_EVChargeParameterType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
  Exi_DIN_PhysicalValueType* EVEnergyCapacity;
  Exi_DIN_PhysicalValueType* EVEnergyRequest;
  Exi_DIN_PhysicalValueType* EVMaximumCurrentLimit;
  Exi_DIN_PhysicalValueType* EVMaximumPowerLimit;
  Exi_DIN_PhysicalValueType* EVMaximumVoltageLimit;
  sint8 BulkSOC;
  sint8 FullSOC;
  Exi_BitType BulkSOCFlag : 1;
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

typedef struct Exi_DIN_AC_EVSEChargeParameterType Exi_DIN_AC_EVSEChargeParameterType;

struct Exi_DIN_AC_EVSEChargeParameterType
{
  Exi_DIN_AC_EVSEStatusType* AC_EVSEStatus;
  Exi_DIN_PhysicalValueType* EVSEMaxCurrent;
  Exi_DIN_PhysicalValueType* EVSEMaxVoltage;
  Exi_DIN_PhysicalValueType* EVSEMinCurrent;
};

typedef struct Exi_DIN_DC_EVSEChargeParameterType Exi_DIN_DC_EVSEChargeParameterType;

struct Exi_DIN_DC_EVSEChargeParameterType
{
  Exi_DIN_DC_EVSEStatusType* DC_EVSEStatus;
  Exi_DIN_PhysicalValueType* EVSECurrentRegulationTolerance;
  Exi_DIN_PhysicalValueType* EVSEEnergyToBeDelivered;
  Exi_DIN_PhysicalValueType* EVSEMaximumCurrentLimit;
  Exi_DIN_PhysicalValueType* EVSEMaximumPowerLimit;
  Exi_DIN_PhysicalValueType* EVSEMaximumVoltageLimit;
  Exi_DIN_PhysicalValueType* EVSEMinimumCurrentLimit;
  Exi_DIN_PhysicalValueType* EVSEMinimumVoltageLimit;
  Exi_DIN_PhysicalValueType* EVSEPeakCurrentRipple;
  Exi_BitType EVSECurrentRegulationToleranceFlag : 1;
  Exi_BitType EVSEEnergyToBeDeliveredFlag : 1;
  Exi_BitType EVSEMaximumPowerLimitFlag : 1;
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

typedef struct Exi_DIN_DC_EVPowerDeliveryParameterType Exi_DIN_DC_EVPowerDeliveryParameterType;

struct Exi_DIN_DC_EVPowerDeliveryParameterType
{
  Exi_DIN_DC_EVStatusType* DC_EVStatus;
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
  /* EXI_DIN_SCHEMA_TYPES_H */
