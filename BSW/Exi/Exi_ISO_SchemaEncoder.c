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
 *         File:  Exi_ISO_SchemaEncoder.c
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


#define EXI_ISO_SCHEMA_ENCODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_ISO_SchemaEncoder.h"
#include "Exi_BSEncoder.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_ISO_SchemaEncoder.c are inconsistent"
#endif

#if (!defined (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET))
# if (defined (EXI_ENABLE_ISO_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_ISO_MESSAGE_SET   EXI_ENABLE_ISO_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_ISO_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET) && (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET == STD_ON))

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */


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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVChargeParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AC_EVChargeParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == AC_EVChargeParameterPtr->DepartureTimeFlag)
  {
    /* SE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(AC_EVChargeParameterPtr->DepartureTime));
    /* EE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EAmount) */
  if(0 == AC_EVChargeParameterPtr->DepartureTimeFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVChargeParameterPtr->EAmount));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EAmount) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVMaxVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVChargeParameterPtr->EVMaxVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVMaxVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVMaxCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVChargeParameterPtr->EVMaxCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVMaxCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVMinCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVChargeParameterPtr->EVMinCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVMinCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEChargeParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AC_EVSEChargeParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(AC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, (AC_EVSEChargeParameterPtr->AC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */
  /* EE(AC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSENominalVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVSEChargeParameterPtr->EVSENominalVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSENominalVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMaxCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (AC_EVSEChargeParameterPtr->EVSEMaxCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMaxCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) AC_EVSEStatusPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AC_EVSEStatusPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(AC_EVSEStatusPtr->NotificationMaxDelay));
  /* EE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON))
  Exi_Encode_ISO_EVSENotification(EncWsPtr, &(AC_EVSEStatusPtr->EVSENotification));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AC_EVSESTATUS, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON)) */
  /* EE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(RCD) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (AC_EVSEStatusPtr->RCD));
  /* EE(RCD) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AttributeId( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AttributeIdType, AUTOMATIC, EXI_APPL_DATA) AttributeIdPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeIdPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeIdPtr->Length <= sizeof(AttributeIdPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeIdPtr->Buffer[0], AttributeIdPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AttributeName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AttributeNameType, AUTOMATIC, EXI_APPL_DATA) AttributeNamePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeNamePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeNamePtr->Length <= sizeof(AttributeNamePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ATTRIBUTE_NAME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeNamePtr->Buffer[0], AttributeNamePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AuthorizationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AuthorizationReqType, AUTOMATIC, EXI_APPL_DATA) AuthorizationReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AuthorizationReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == AuthorizationReqPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_ISO_AttributeId(EncWsPtr, (AuthorizationReqPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  }
  if(1 == AuthorizationReqPtr->GenChallengeFlag)
  {
    /* SE(GenChallenge) */
    if(0 == AuthorizationReqPtr->IdFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON))
    Exi_Encode_ISO_genChallenge(EncWsPtr, (AuthorizationReqPtr->GenChallenge));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON)) */
    /* EE(GenChallenge) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(AuthorizationReq) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == AuthorizationReqPtr->IdFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_AuthorizationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_AuthorizationResType, AUTOMATIC, EXI_APPL_DATA) AuthorizationResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AuthorizationResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(AuthorizationResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON))
  Exi_Encode_ISO_EVSEProcessing(EncWsPtr, &(AuthorizationResPtr->EVSEProcessing));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_AUTHORIZATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON)) */
  /* EE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Body( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_BodyType, AUTOMATIC, EXI_APPL_DATA) BodyPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != BodyPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_BODY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == BodyPtr->BodyElementFlag)
  {
    /* Start of Substitution Group */
    if(EXI_ISO_AUTHORIZATION_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON))
      /* SE(AuthorizationReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_AuthorizationReq(EncWsPtr, (Exi_ISO_AuthorizationReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(AuthorizationReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(AuthorizationReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON)) */
    }
    else if(EXI_ISO_AUTHORIZATION_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON))
      /* SE(AuthorizationRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 6);
      Exi_Encode_ISO_AuthorizationRes(EncWsPtr, (Exi_ISO_AuthorizationResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(AuthorizationRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON)) */
    }
    else if(EXI_ISO_BODY_ELEMENT_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_BODY_ELEMENT) && (EXI_ENCODE_ISO_BODY_ELEMENT == STD_ON))
      /* SE(BodyElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 6);
      /* EE(BodyElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_BODY_ELEMENT) && (EXI_ENCODE_ISO_BODY_ELEMENT == STD_ON)) */
    }
    else if(EXI_ISO_CABLE_CHECK_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON))
      /* SE(CableCheckReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 6);
      Exi_Encode_ISO_CableCheckReq(EncWsPtr, (Exi_ISO_CableCheckReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CableCheckReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CABLE_CHECK_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON))
      /* SE(CableCheckRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 6);
      Exi_Encode_ISO_CableCheckRes(EncWsPtr, (Exi_ISO_CableCheckResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CableCheckRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON)) */
    }
    else if(EXI_ISO_CERTIFICATE_INSTALLATION_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON))
      /* SE(CertificateInstallationReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 6);
      Exi_Encode_ISO_CertificateInstallationReq(EncWsPtr, (Exi_ISO_CertificateInstallationReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CertificateInstallationReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CERTIFICATE_INSTALLATION_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON))
      /* SE(CertificateInstallationRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 6);
      Exi_Encode_ISO_CertificateInstallationRes(EncWsPtr, (Exi_ISO_CertificateInstallationResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CertificateInstallationRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON)) */
    }
    else if(EXI_ISO_CERTIFICATE_UPDATE_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON))
      /* SE(CertificateUpdateReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 6);
      Exi_Encode_ISO_CertificateUpdateReq(EncWsPtr, (Exi_ISO_CertificateUpdateReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CertificateUpdateReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CERTIFICATE_UPDATE_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON))
      /* SE(CertificateUpdateRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_CertificateUpdateRes(EncWsPtr, (Exi_ISO_CertificateUpdateResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CertificateUpdateRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(CertificateUpdateRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON)) */
    }
    else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
      /* SE(ChargeParameterDiscoveryReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ChargeParameterDiscoveryReq(EncWsPtr, (Exi_ISO_ChargeParameterDiscoveryReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ChargeParameterDiscoveryReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ChargeParameterDiscoveryReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
      /* SE(ChargeParameterDiscoveryRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ChargeParameterDiscoveryRes(EncWsPtr, (Exi_ISO_ChargeParameterDiscoveryResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ChargeParameterDiscoveryRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ChargeParameterDiscoveryRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */
    }
    else if(EXI_ISO_CHARGING_STATUS_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CHARGING_STATUS_REQ) && (EXI_ENCODE_ISO_CHARGING_STATUS_REQ == STD_ON))
      /* SE(ChargingStatusReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 6);
      /* EE(ChargingStatusReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CHARGING_STATUS_REQ) && (EXI_ENCODE_ISO_CHARGING_STATUS_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CHARGING_STATUS_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON))
      /* SE(ChargingStatusRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ChargingStatusRes(EncWsPtr, (Exi_ISO_ChargingStatusResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ChargingStatusRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ChargingStatusRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON)) */
    }
    else if(EXI_ISO_CURRENT_DEMAND_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON))
      /* SE(CurrentDemandReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_CurrentDemandReq(EncWsPtr, (Exi_ISO_CurrentDemandReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CurrentDemandReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(CurrentDemandReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON)) */
    }
    else if(EXI_ISO_CURRENT_DEMAND_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON))
      /* SE(CurrentDemandRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_CurrentDemandRes(EncWsPtr, (Exi_ISO_CurrentDemandResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(CurrentDemandRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(CurrentDemandRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON)) */
    }
    else if(EXI_ISO_METERING_RECEIPT_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON))
      /* SE(MeteringReceiptReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 15, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_MeteringReceiptReq(EncWsPtr, (Exi_ISO_MeteringReceiptReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(MeteringReceiptReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(MeteringReceiptReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON)) */
    }
    else if(EXI_ISO_METERING_RECEIPT_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON))
      /* SE(MeteringReceiptRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 6);
      Exi_Encode_ISO_MeteringReceiptRes(EncWsPtr, (Exi_ISO_MeteringReceiptResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(MeteringReceiptRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON)) */
    }
    else if(EXI_ISO_PAYMENT_DETAILS_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON))
      /* SE(PaymentDetailsReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 6);
      Exi_Encode_ISO_PaymentDetailsReq(EncWsPtr, (Exi_ISO_PaymentDetailsReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PaymentDetailsReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON)) */
    }
    else if(EXI_ISO_PAYMENT_DETAILS_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON))
      /* SE(PaymentDetailsRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 6);
      Exi_Encode_ISO_PaymentDetailsRes(EncWsPtr, (Exi_ISO_PaymentDetailsResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PaymentDetailsRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON)) */
    }
    else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON))
      /* SE(PaymentServiceSelectionReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 6);
      Exi_Encode_ISO_PaymentServiceSelectionReq(EncWsPtr, (Exi_ISO_PaymentServiceSelectionReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PaymentServiceSelectionReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON)) */
    }
    else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON))
      /* SE(PaymentServiceSelectionRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 6);
      Exi_Encode_ISO_PaymentServiceSelectionRes(EncWsPtr, (Exi_ISO_PaymentServiceSelectionResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PaymentServiceSelectionRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON)) */
    }
    else if(EXI_ISO_POWER_DELIVERY_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON))
      /* SE(PowerDeliveryReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_PowerDeliveryReq(EncWsPtr, (Exi_ISO_PowerDeliveryReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PowerDeliveryReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(PowerDeliveryReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON)) */
    }
    else if(EXI_ISO_POWER_DELIVERY_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON))
      /* SE(PowerDeliveryRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 6);
      Exi_Encode_ISO_PowerDeliveryRes(EncWsPtr, (Exi_ISO_PowerDeliveryResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PowerDeliveryRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON)) */
    }
    else if(EXI_ISO_PRE_CHARGE_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON))
      /* SE(PreChargeReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 23, 6);
      Exi_Encode_ISO_PreChargeReq(EncWsPtr, (Exi_ISO_PreChargeReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PreChargeReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON)) */
    }
    else if(EXI_ISO_PRE_CHARGE_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON))
      /* SE(PreChargeRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 24, 6);
      Exi_Encode_ISO_PreChargeRes(EncWsPtr, (Exi_ISO_PreChargeResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(PreChargeRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON)) */
    }
    else if(EXI_ISO_SERVICE_DETAIL_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON))
      /* SE(ServiceDetailReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 25, 6);
      Exi_Encode_ISO_ServiceDetailReq(EncWsPtr, (Exi_ISO_ServiceDetailReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ServiceDetailReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON)) */
    }
    else if(EXI_ISO_SERVICE_DETAIL_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON))
      /* SE(ServiceDetailRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 26, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ServiceDetailRes(EncWsPtr, (Exi_ISO_ServiceDetailResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ServiceDetailRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ServiceDetailRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON)) */
    }
    else if(EXI_ISO_SERVICE_DISCOVERY_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON))
      /* SE(ServiceDiscoveryReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 27, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ServiceDiscoveryReq(EncWsPtr, (Exi_ISO_ServiceDiscoveryReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ServiceDiscoveryReq) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ServiceDiscoveryReq) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON)) */
    }
    else if(EXI_ISO_SERVICE_DISCOVERY_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON))
      /* SE(ServiceDiscoveryRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 28, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_ServiceDiscoveryRes(EncWsPtr, (Exi_ISO_ServiceDiscoveryResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(ServiceDiscoveryRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ServiceDiscoveryRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON)) */
    }
    else if(EXI_ISO_SESSION_SETUP_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON))
      /* SE(SessionSetupReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 29, 6);
      Exi_Encode_ISO_SessionSetupReq(EncWsPtr, (Exi_ISO_SessionSetupReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(SessionSetupReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON)) */
    }
    else if(EXI_ISO_SESSION_SETUP_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON))
      /* SE(SessionSetupRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 30, 6);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_SessionSetupRes(EncWsPtr, (Exi_ISO_SessionSetupResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(SessionSetupRes) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(SessionSetupRes) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON)) */
    }
    else if(EXI_ISO_SESSION_STOP_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON))
      /* SE(SessionStopReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 31, 6);
      Exi_Encode_ISO_SessionStopReq(EncWsPtr, (Exi_ISO_SessionStopReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(SessionStopReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON)) */
    }
    else if(EXI_ISO_SESSION_STOP_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON))
      /* SE(SessionStopRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 32, 6);
      Exi_Encode_ISO_SessionStopRes(EncWsPtr, (Exi_ISO_SessionStopResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(SessionStopRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON)) */
    }
    else if(EXI_ISO_WELDING_DETECTION_REQ_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON))
      /* SE(WeldingDetectionReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 33, 6);
      Exi_Encode_ISO_WeldingDetectionReq(EncWsPtr, (Exi_ISO_WeldingDetectionReqType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(WeldingDetectionReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON)) */
    }
    else if(EXI_ISO_WELDING_DETECTION_RES_TYPE == BodyPtr->BodyElementElementId)
    {
    #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON))
      /* SE(WeldingDetectionRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 34, 6);
      Exi_Encode_ISO_WeldingDetectionRes(EncWsPtr, (Exi_ISO_WeldingDetectionResType*)BodyPtr->BodyElement); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(WeldingDetectionRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    #endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON)) */
    }
    else
    {
      /* Substitution Element not supported */
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    /* End of Substitution Group */
  }
  else
  {
    /* EE(Body) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CableCheckReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CableCheckReqType, AUTOMATIC, EXI_APPL_DATA) CableCheckReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CableCheckReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (CableCheckReqPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CableCheckRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CableCheckResType, AUTOMATIC, EXI_APPL_DATA) CableCheckResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CableCheckResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(CableCheckResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (CableCheckResPtr->DC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  /* EE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON))
  Exi_Encode_ISO_EVSEProcessing(EncWsPtr, &(CableCheckResPtr->EVSEProcessing));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CABLE_CHECK_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON)) */
  /* EE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateChain( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateChainType, AUTOMATIC, EXI_APPL_DATA) CertificateChainPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CertificateChainPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == CertificateChainPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_ISO_AttributeId(EncWsPtr, (CertificateChainPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* SE(Certificate) */
  if(0 == CertificateChainPtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
  Exi_Encode_ISO_certificate(EncWsPtr, (CertificateChainPtr->Certificate));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */
  /* EE(Certificate) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CertificateChainPtr->SubCertificatesFlag)
  {
    /* SE(SubCertificates) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON))
    Exi_Encode_ISO_SubCertificates(EncWsPtr, (CertificateChainPtr->SubCertificates));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_CHAIN, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON)) */
    /* EE(SubCertificates) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SubCertificates) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(CertificateChain) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateInstallationReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateInstallationReqType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CertificateInstallationReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (CertificateInstallationReqPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* SE(OEMProvisioningCert) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
  Exi_Encode_ISO_certificate(EncWsPtr, (CertificateInstallationReqPtr->OEMProvisioningCert));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */
  /* EE(OEMProvisioningCert) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(ListOfRootCertificateIDs) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
  Exi_Encode_ISO_ListOfRootCertificateIDs(EncWsPtr, (CertificateInstallationReqPtr->ListOfRootCertificateIDs));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */
  /* EE(ListOfRootCertificateIDs) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ListOfRootCertificateIDs) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateInstallationRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateInstallationResType, AUTOMATIC, EXI_APPL_DATA) CertificateInstallationResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CertificateInstallationResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(CertificateInstallationResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SAProvisioningCertificateChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (CertificateInstallationResPtr->SAProvisioningCertificateChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(SAProvisioningCertificateChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SAProvisioningCertificateChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(ContractSignatureCertChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (CertificateInstallationResPtr->ContractSignatureCertChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(ContractSignatureCertChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ContractSignatureCertChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(ContractSignatureEncryptedPrivateKey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON))
  Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey(EncWsPtr, (CertificateInstallationResPtr->ContractSignatureEncryptedPrivateKey));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON)) */
  /* EE(ContractSignatureEncryptedPrivateKey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DHpublickey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON))
  Exi_Encode_ISO_DiffieHellmanPublickey(EncWsPtr, (CertificateInstallationResPtr->DHpublickey));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON)) */
  /* EE(DHpublickey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON))
  Exi_Encode_ISO_EMAID(EncWsPtr, (CertificateInstallationResPtr->eMAID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON)) */
  /* EE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateUpdateReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateUpdateReqType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CertificateUpdateReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (CertificateUpdateReqPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* SE(ContractSignatureCertChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (CertificateUpdateReqPtr->ContractSignatureCertChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(ContractSignatureCertChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ContractSignatureCertChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON))
  Exi_Encode_ISO_eMAID(EncWsPtr, (CertificateUpdateReqPtr->eMAID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON)) */
  /* EE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(ListOfRootCertificateIDs) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
  Exi_Encode_ISO_ListOfRootCertificateIDs(EncWsPtr, (CertificateUpdateReqPtr->ListOfRootCertificateIDs));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */
  /* EE(ListOfRootCertificateIDs) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ListOfRootCertificateIDs) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CertificateUpdateRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CertificateUpdateResType, AUTOMATIC, EXI_APPL_DATA) CertificateUpdateResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CertificateUpdateResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(CertificateUpdateResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SAProvisioningCertificateChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (CertificateUpdateResPtr->SAProvisioningCertificateChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(SAProvisioningCertificateChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SAProvisioningCertificateChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(ContractSignatureCertChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (CertificateUpdateResPtr->ContractSignatureCertChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(ContractSignatureCertChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ContractSignatureCertChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(ContractSignatureEncryptedPrivateKey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON))
  Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey(EncWsPtr, (CertificateUpdateResPtr->ContractSignatureEncryptedPrivateKey));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON)) */
  /* EE(ContractSignatureEncryptedPrivateKey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DHpublickey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON))
  Exi_Encode_ISO_DiffieHellmanPublickey(EncWsPtr, (CertificateUpdateResPtr->DHpublickey));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON)) */
  /* EE(DHpublickey) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON))
  Exi_Encode_ISO_EMAID(EncWsPtr, (CertificateUpdateResPtr->eMAID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE_UPDATE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON)) */
  /* EE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CertificateUpdateResPtr->RetryCounterFlag)
  {
    /* SE(RetryCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(CertificateUpdateResPtr->RetryCounter));
    /* EE(RetryCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(CertificateUpdateRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeParameterDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeParameterDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ChargeParameterDiscoveryReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == ChargeParameterDiscoveryReqPtr->MaxEntriesSAScheduleTupleFlag)
  {
    /* SE(MaxEntriesSAScheduleTuple) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ChargeParameterDiscoveryReqPtr->MaxEntriesSAScheduleTuple));
    /* EE(MaxEntriesSAScheduleTuple) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(RequestedEnergyTransferMode) */
  if(0 == ChargeParameterDiscoveryReqPtr->MaxEntriesSAScheduleTupleFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
  Exi_Encode_ISO_EnergyTransferMode(EncWsPtr, &(ChargeParameterDiscoveryReqPtr->RequestedEnergyTransferMode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */
  /* EE(RequestedEnergyTransferMode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* Start of Substitution Group */
  if(EXI_ISO_AC_EVCHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryReqPtr->EVChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON))
    /* SE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    Exi_Encode_ISO_AC_EVChargeParameter(EncWsPtr, (Exi_ISO_AC_EVChargeParameterType*)ChargeParameterDiscoveryReqPtr->EVChargeParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON)) */
  }
  else if(EXI_ISO_DC_EVCHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryReqPtr->EVChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON))
    /* SE(DC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVChargeParameter(EncWsPtr, (Exi_ISO_DC_EVChargeParameterType*)ChargeParameterDiscoveryReqPtr->EVChargeParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(DC_EVChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON)) */
  }
  else if(EXI_ISO_EVCHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryReqPtr->EVChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON))
    /* SE(EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EVChargeParameter(EncWsPtr, (Exi_ISO_EVChargeParameterType*)ChargeParameterDiscoveryReqPtr->EVChargeParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(EVChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(EVChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeParameterDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeParameterDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ChargeParameterDiscoveryResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ChargeParameterDiscoveryResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(ChargeParameterDiscoveryResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON))
  Exi_Encode_ISO_EVSEProcessing(EncWsPtr, &(ChargeParameterDiscoveryResPtr->EVSEProcessing));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON)) */
  /* EE(EVSEProcessing) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ChargeParameterDiscoveryResPtr->SASchedulesFlag)
  {
    /* Start of Substitution Group */
    if(EXI_ISO_SASCHEDULE_LIST_TYPE == ChargeParameterDiscoveryResPtr->SASchedulesElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON))
      /* SE(SAScheduleList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_SAScheduleList(EncWsPtr, (Exi_ISO_SAScheduleListType*)ChargeParameterDiscoveryResPtr->SASchedules); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(SAScheduleList) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(SAScheduleList) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    #endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON)) */
    }
    else if(EXI_ISO_SASCHEDULES_TYPE == ChargeParameterDiscoveryResPtr->SASchedulesElementId)
    {
    #if (defined(EXI_ENCODE_ISO_SASCHEDULES) && (EXI_ENCODE_ISO_SASCHEDULES == STD_ON))
      /* SE(SASchedules) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      /* EE(SASchedules) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    #endif /* (defined(EXI_ENCODE_ISO_SASCHEDULES) && (EXI_ENCODE_ISO_SASCHEDULES == STD_ON)) */
    }
    else
    {
      /* Substitution Element not supported */
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    /* End of Substitution Group */
  }
  /* Start of Substitution Group */
  if(EXI_ISO_AC_EVSECHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryResPtr->EVSEChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON))
    /* SE(AC_EVSEChargeParameter) */
    if(0 == ChargeParameterDiscoveryResPtr->SASchedulesFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    Exi_Encode_ISO_AC_EVSEChargeParameter(EncWsPtr, (Exi_ISO_AC_EVSEChargeParameterType*)ChargeParameterDiscoveryResPtr->EVSEChargeParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(AC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON)) */
  }
  else if(EXI_ISO_DC_EVSECHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryResPtr->EVSEChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON))
    /* SE(DC_EVSEChargeParameter) */
    if(0 == ChargeParameterDiscoveryResPtr->SASchedulesFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEChargeParameter(EncWsPtr, (Exi_ISO_DC_EVSEChargeParameterType*)ChargeParameterDiscoveryResPtr->EVSEChargeParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(DC_EVSEChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVSEChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON)) */
  }
  else if(EXI_ISO_EVSECHARGE_PARAMETER_TYPE == ChargeParameterDiscoveryResPtr->EVSEChargeParameterElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVSECHARGE_PARAMETER == STD_ON))
    /* SE(EVSEChargeParameter) */
    if(0 == ChargeParameterDiscoveryResPtr->SASchedulesFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    /* EE(EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVSECHARGE_PARAMETER == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargeService( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargeServiceType, AUTOMATIC, EXI_APPL_DATA) ChargeServicePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ChargeServicePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ChargeServicePtr->ServiceID));
  /* EE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ChargeServicePtr->ServiceNameFlag)
  {
    /* SE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON))
    Exi_Encode_ISO_serviceName(EncWsPtr, (ChargeServicePtr->ServiceName));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON)) */
    /* EE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(ServiceCategory) */
  if(0 == ChargeServicePtr->ServiceNameFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON))
  Exi_Encode_ISO_serviceCategory(EncWsPtr, &(ChargeServicePtr->ServiceCategory));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON)) */
  /* EE(ServiceCategory) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ChargeServicePtr->ServiceScopeFlag)
  {
    /* SE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON))
    Exi_Encode_ISO_serviceScope(EncWsPtr, (ChargeServicePtr->ServiceScope));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON)) */
    /* EE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(FreeService) */
  if(0 == ChargeServicePtr->ServiceScopeFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (ChargeServicePtr->FreeService));
  /* EE(FreeService) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SupportedEnergyTransferMode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON))
  Exi_Encode_ISO_SupportedEnergyTransferMode(EncWsPtr, (ChargeServicePtr->SupportedEnergyTransferMode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_SERVICE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON)) */
  /* EE(SupportedEnergyTransferMode) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SupportedEnergyTransferMode) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargingProfile( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargingProfileType, AUTOMATIC, EXI_APPL_DATA) ChargingProfilePtr )
{
  uint8 i = 0;
  Exi_ISO_ProfileEntryType* nextPtr = (Exi_ISO_ProfileEntryType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ChargingProfilePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_ProfileEntryType*)ChargingProfilePtr->ProfileEntry;
  for(i=0; i<24; i++)
  {
    /* SE(ProfileEntry) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON))
    Exi_Encode_ISO_ProfileEntry(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_PROFILE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON)) */
    /* EE(ProfileEntry) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ProfileEntry) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_ProfileEntryType*)(nextPtr->NextProfileEntryPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_ProfileEntryType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ChargingProfile) */
  if( i < 24)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ChargingStatusRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ChargingStatusResType, AUTOMATIC, EXI_APPL_DATA) ChargingStatusResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ChargingStatusResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= ChargingStatusResPtr->SAScheduleTupleID) && (ChargingStatusResPtr->SAScheduleTupleID <= 255)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(ChargingStatusResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON))
  Exi_Encode_ISO_evseID(EncWsPtr, (ChargingStatusResPtr->EVSEID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON)) */
  /* EE(EVSEID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(ChargingStatusResPtr->SAScheduleTupleID - 1), 8);
  /* EE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ChargingStatusResPtr->EVSEMaxCurrentFlag)
  {
    /* SE(EVSEMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (ChargingStatusResPtr->EVSEMaxCurrent));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSEMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == ChargingStatusResPtr->MeterInfoFlag)
  {
    /* SE(MeterInfo) */
    if(0 == ChargingStatusResPtr->EVSEMaxCurrentFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON))
    Exi_Encode_ISO_MeterInfo(EncWsPtr, (ChargingStatusResPtr->MeterInfo));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON)) */
    /* EE(MeterInfo) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(MeterInfo) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  if(1 == ChargingStatusResPtr->ReceiptRequiredFlag)
  {
    /* SE(ReceiptRequired) */
    if(0 == ChargingStatusResPtr->MeterInfoFlag)
    {
      if(0 == ChargingStatusResPtr->EVSEMaxCurrentFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBool(&EncWsPtr->EncWs, (ChargingStatusResPtr->ReceiptRequired));
    /* EE(ReceiptRequired) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(AC_EVSEStatus) */
  if(0 == ChargingStatusResPtr->ReceiptRequiredFlag)
  {
    if(0 == ChargingStatusResPtr->MeterInfoFlag)
    {
      if(0 == ChargingStatusResPtr->EVSEMaxCurrentFlag)
      {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, (ChargingStatusResPtr->AC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_STATUS_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */
  /* EE(AC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ConsumptionCost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ConsumptionCostType, AUTOMATIC, EXI_APPL_DATA) ConsumptionCostPtr )
{
  uint8 i = 0;
  Exi_ISO_CostType* nextPtr = (Exi_ISO_CostType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ConsumptionCostPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(startValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (ConsumptionCostPtr->startValue));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(startValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  nextPtr = (Exi_ISO_CostType*)ConsumptionCostPtr->Cost;
  for(i=0; i<3; i++)
  {
    /* SE(Cost) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON))
    Exi_Encode_ISO_Cost(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONSUMPTION_COST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON)) */
    /* EE(Cost) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Cost) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_CostType*)(nextPtr->NextCostPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_CostType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ConsumptionCost) */
  if( i < 3)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ContractSignatureEncryptedPrivateKeyType, AUTOMATIC, EXI_APPL_DATA) ContractSignatureEncryptedPrivateKeyPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ContractSignatureEncryptedPrivateKeyPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((ContractSignatureEncryptedPrivateKeyPtr->Length <= sizeof(ContractSignatureEncryptedPrivateKeyPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (ContractSignatureEncryptedPrivateKeyPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &ContractSignatureEncryptedPrivateKeyPtr->Buffer[0], ContractSignatureEncryptedPrivateKeyPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Cost( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CostType, AUTOMATIC, EXI_APPL_DATA) CostPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CostPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((0 == CostPtr->amountMultiplierFlag) || ((-3 <= CostPtr->amountMultiplier) && (CostPtr->amountMultiplier <= 3))), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(costKind) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON))
  Exi_Encode_ISO_costKind(EncWsPtr, &(CostPtr->costKind));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON)) */
  /* EE(costKind) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(amount) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(CostPtr->amount));
  /* EE(amount) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CostPtr->amountMultiplierFlag)
  {
    /* SE(amountMultiplier) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(CostPtr->amountMultiplier + 3), 3);
    /* EE(amountMultiplier) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(Cost) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CurrentDemandReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CurrentDemandReqType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CurrentDemandReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (CurrentDemandReqPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVTargetCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->EVTargetCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVTargetCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CurrentDemandReqPtr->EVMaximumVoltageLimitFlag)
  {
    /* SE(EVMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->EVMaximumVoltageLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandReqPtr->EVMaximumCurrentLimitFlag)
  {
    /* SE(EVMaximumCurrentLimit) */
    if(0 == CurrentDemandReqPtr->EVMaximumVoltageLimitFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->EVMaximumCurrentLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandReqPtr->EVMaximumPowerLimitFlag)
  {
    /* SE(EVMaximumPowerLimit) */
    if(0 == CurrentDemandReqPtr->EVMaximumCurrentLimitFlag)
    {
      if(0 == CurrentDemandReqPtr->EVMaximumVoltageLimitFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->EVMaximumPowerLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandReqPtr->BulkChargingCompleteFlag)
  {
    /* SE(BulkChargingComplete) */
    if(0 == CurrentDemandReqPtr->EVMaximumPowerLimitFlag)
    {
      if(0 == CurrentDemandReqPtr->EVMaximumCurrentLimitFlag)
      {
        if(0 == CurrentDemandReqPtr->EVMaximumVoltageLimitFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandReqPtr->BulkChargingComplete));
    /* EE(BulkChargingComplete) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(ChargingComplete) */
  if(0 == CurrentDemandReqPtr->BulkChargingCompleteFlag)
  {
    if(0 == CurrentDemandReqPtr->EVMaximumPowerLimitFlag)
    {
      if(0 == CurrentDemandReqPtr->EVMaximumCurrentLimitFlag)
      {
        if(0 == CurrentDemandReqPtr->EVMaximumVoltageLimitFlag)
        {
            Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandReqPtr->ChargingComplete));
  /* EE(ChargingComplete) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CurrentDemandReqPtr->RemainingTimeToFullSoCFlag)
  {
    /* SE(RemainingTimeToFullSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->RemainingTimeToFullSoC));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(RemainingTimeToFullSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandReqPtr->RemainingTimeToBulkSoCFlag)
  {
    /* SE(RemainingTimeToBulkSoC) */
    if(0 == CurrentDemandReqPtr->RemainingTimeToFullSoCFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->RemainingTimeToBulkSoC));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(RemainingTimeToBulkSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EVTargetVoltage) */
  if(0 == CurrentDemandReqPtr->RemainingTimeToBulkSoCFlag)
  {
    if(0 == CurrentDemandReqPtr->RemainingTimeToFullSoCFlag)
    {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandReqPtr->EVTargetVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVTargetVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_CurrentDemandRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_CurrentDemandResType, AUTOMATIC, EXI_APPL_DATA) CurrentDemandResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CurrentDemandResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= CurrentDemandResPtr->SAScheduleTupleID) && (CurrentDemandResPtr->SAScheduleTupleID <= 255)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(CurrentDemandResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (CurrentDemandResPtr->DC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  /* EE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandResPtr->EVSEPresentVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEPresentCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandResPtr->EVSEPresentCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEPresentCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSECurrentLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandResPtr->EVSECurrentLimitAchieved));
  /* EE(EVSECurrentLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEVoltageLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandResPtr->EVSEVoltageLimitAchieved));
  /* EE(EVSEVoltageLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEPowerLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandResPtr->EVSEPowerLimitAchieved));
  /* EE(EVSEPowerLimitAchieved) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CurrentDemandResPtr->EVSEMaximumVoltageLimitFlag)
  {
    /* SE(EVSEMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandResPtr->EVSEMaximumVoltageLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSEMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandResPtr->EVSEMaximumCurrentLimitFlag)
  {
    /* SE(EVSEMaximumCurrentLimit) */
    if(0 == CurrentDemandResPtr->EVSEMaximumVoltageLimitFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandResPtr->EVSEMaximumCurrentLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSEMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == CurrentDemandResPtr->EVSEMaximumPowerLimitFlag)
  {
    /* SE(EVSEMaximumPowerLimit) */
    if(0 == CurrentDemandResPtr->EVSEMaximumCurrentLimitFlag)
    {
      if(0 == CurrentDemandResPtr->EVSEMaximumVoltageLimitFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (CurrentDemandResPtr->EVSEMaximumPowerLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSEMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EVSEID) */
  if(0 == CurrentDemandResPtr->EVSEMaximumPowerLimitFlag)
  {
    if(0 == CurrentDemandResPtr->EVSEMaximumCurrentLimitFlag)
    {
      if(0 == CurrentDemandResPtr->EVSEMaximumVoltageLimitFlag)
      {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON))
  Exi_Encode_ISO_evseID(EncWsPtr, (CurrentDemandResPtr->EVSEID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON)) */
  /* EE(EVSEID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(CurrentDemandResPtr->SAScheduleTupleID - 1), 8);
  /* EE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == CurrentDemandResPtr->MeterInfoFlag)
  {
    /* SE(MeterInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON))
    Exi_Encode_ISO_MeterInfo(EncWsPtr, (CurrentDemandResPtr->MeterInfo));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CURRENT_DEMAND_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON)) */
    /* EE(MeterInfo) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(MeterInfo) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  if(1 == CurrentDemandResPtr->ReceiptRequiredFlag)
  {
    /* SE(ReceiptRequired) */
    if(0 == CurrentDemandResPtr->MeterInfoFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBool(&EncWsPtr->EncWs, (CurrentDemandResPtr->ReceiptRequired));
    /* EE(ReceiptRequired) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(CurrentDemandRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == CurrentDemandResPtr->MeterInfoFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVChargeParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVChargeParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((0 == DC_EVChargeParameterPtr->FullSOCFlag) || ((0 <= DC_EVChargeParameterPtr->FullSOC) && (DC_EVChargeParameterPtr->FullSOC <= 100))), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid(((0 == DC_EVChargeParameterPtr->BulkSOCFlag) || ((0 <= DC_EVChargeParameterPtr->BulkSOC) && (DC_EVChargeParameterPtr->BulkSOC <= 100))), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == DC_EVChargeParameterPtr->DepartureTimeFlag)
  {
    /* SE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(DC_EVChargeParameterPtr->DepartureTime));
    /* EE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(DC_EVStatus) */
  if(0 == DC_EVChargeParameterPtr->DepartureTimeFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (DC_EVChargeParameterPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVMaximumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVChargeParameterPtr->EVMaximumCurrentLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVMaximumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVChargeParameterPtr->EVMaximumPowerLimitFlag)
  {
    /* SE(EVMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVChargeParameterPtr->EVMaximumPowerLimit));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EVMaximumVoltageLimit) */
  if(0 == DC_EVChargeParameterPtr->EVMaximumPowerLimitFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVChargeParameterPtr->EVMaximumVoltageLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVMaximumVoltageLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVChargeParameterPtr->EVEnergyCapacityFlag)
  {
    /* SE(EVEnergyCapacity) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVChargeParameterPtr->EVEnergyCapacity));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVEnergyCapacity) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == DC_EVChargeParameterPtr->EVEnergyRequestFlag)
  {
    /* SE(EVEnergyRequest) */
    if(0 == DC_EVChargeParameterPtr->EVEnergyCapacityFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    }
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVChargeParameterPtr->EVEnergyRequest));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVEnergyRequest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == DC_EVChargeParameterPtr->FullSOCFlag)
  {
    /* SE(FullSOC) */
    if(0 == DC_EVChargeParameterPtr->EVEnergyRequestFlag)
    {
      if(0 == DC_EVChargeParameterPtr->EVEnergyCapacityFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(DC_EVChargeParameterPtr->FullSOC), 7);
    /* EE(FullSOC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == DC_EVChargeParameterPtr->BulkSOCFlag)
  {
    /* SE(BulkSOC) */
    if(0 == DC_EVChargeParameterPtr->FullSOCFlag)
    {
      if(0 == DC_EVChargeParameterPtr->EVEnergyRequestFlag)
      {
        if(0 == DC_EVChargeParameterPtr->EVEnergyCapacityFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(DC_EVChargeParameterPtr->BulkSOC), 7);
    /* EE(BulkSOC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(DC_EVChargeParameter) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == DC_EVChargeParameterPtr->FullSOCFlag)
    {
      if(0 == DC_EVChargeParameterPtr->EVEnergyRequestFlag)
      {
        if(0 == DC_EVChargeParameterPtr->EVEnergyCapacityFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVErrorCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVErrorCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVErrorCodePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVErrorCodePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVERROR_CODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_DC_EVERROR_CODE_TYPE_NO_ERROR == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_RESSTEMPERATURE_INHIBIT == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_EVSHIFT_POSITION == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGER_CONNECTOR_LOCK_FAULT == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_EVRESSMALFUNCTION == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_CURRENTDIFFERENTIAL == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_VOLTAGE_OUT_OF_RANGE == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_A == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_B == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_RESERVED_C == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 4);
  }
  else if(EXI_ISO_DC_EVERROR_CODE_TYPE_FAILED_CHARGING_SYSTEM_INCOMPATIBILITY == *DC_EVErrorCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 4);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVPowerDeliveryParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVPowerDeliveryParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVPowerDeliveryParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVPowerDeliveryParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (DC_EVPowerDeliveryParameterPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVPowerDeliveryParameterPtr->BulkChargingCompleteFlag)
  {
    /* SE(BulkChargingComplete) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBool(&EncWsPtr->EncWs, (DC_EVPowerDeliveryParameterPtr->BulkChargingComplete));
    /* EE(BulkChargingComplete) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(ChargingComplete) */
  if(0 == DC_EVPowerDeliveryParameterPtr->BulkChargingCompleteFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (DC_EVPowerDeliveryParameterPtr->ChargingComplete));
  /* EE(ChargingComplete) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEChargeParameterType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEChargeParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVSEChargeParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (DC_EVSEChargeParameterPtr->DC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  /* EE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMaximumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEMaximumCurrentLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMaximumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMaximumPowerLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEMaximumPowerLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMaximumPowerLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMaximumVoltageLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEMaximumVoltageLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMaximumVoltageLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMinimumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEMinimumCurrentLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMinimumCurrentLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEMinimumVoltageLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEMinimumVoltageLimit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEMinimumVoltageLimit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVSEChargeParameterPtr->EVSECurrentRegulationToleranceFlag)
  {
    /* SE(EVSECurrentRegulationTolerance) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSECurrentRegulationTolerance));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSECurrentRegulationTolerance) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EVSEPeakCurrentRipple) */
  if(0 == DC_EVSEChargeParameterPtr->EVSECurrentRegulationToleranceFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEPeakCurrentRipple));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEPeakCurrentRipple) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVSEChargeParameterPtr->EVSEEnergyToBeDeliveredFlag)
  {
    /* SE(EVSEEnergyToBeDelivered) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (DC_EVSEChargeParameterPtr->EVSEEnergyToBeDelivered));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSECHARGE_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(EVSEEnergyToBeDelivered) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(DC_EVSEChargeParameter) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEStatusCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEStatusCodeType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusCodePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVSEStatusCodePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS_CODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_NOT_READY == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_READY == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_SHUTDOWN == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_UTILITY_INTERRUPT_EVENT == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_ISOLATION_MONITORING_ACTIVE == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_EMERGENCY_SHUTDOWN == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_EVSE_MALFUNCTION == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_8 == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_9 == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_A == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 4);
  }
  else if(EXI_ISO_DC_EVSESTATUS_CODE_TYPE_RESERVED_B == *DC_EVSEStatusCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 4);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVSEStatusPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVSEStatusPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(DC_EVSEStatusPtr->NotificationMaxDelay));
  /* EE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON))
  Exi_Encode_ISO_EVSENotification(EncWsPtr, &(DC_EVSEStatusPtr->EVSENotification));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON)) */
  /* EE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DC_EVSEStatusPtr->EVSEIsolationStatusFlag)
  {
    /* SE(EVSEIsolationStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON))
    Exi_Encode_ISO_isolationLevel(EncWsPtr, &(DC_EVSEStatusPtr->EVSEIsolationStatus));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON)) */
    /* EE(EVSEIsolationStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(EVSEStatusCode) */
  if(0 == DC_EVSEStatusPtr->EVSEIsolationStatusFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatusCode(EncWsPtr, &(DC_EVSEStatusPtr->EVSEStatusCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSESTATUS, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON)) */
  /* EE(EVSEStatusCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DC_EVStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DC_EVStatusType, AUTOMATIC, EXI_APPL_DATA) DC_EVStatusPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DC_EVStatusPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((0 <= DC_EVStatusPtr->EVRESSSOC) && (DC_EVStatusPtr->EVRESSSOC <= 100)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(EVReady) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (DC_EVStatusPtr->EVReady));
  /* EE(EVReady) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVErrorCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON))
  Exi_Encode_ISO_DC_EVErrorCode(EncWsPtr, &(DC_EVStatusPtr->EVErrorCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DC_EVSTATUS, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON)) */
  /* EE(EVErrorCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVRESSSOC) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(DC_EVStatusPtr->EVRESSSOC), 7);
  /* EE(EVRESSSOC) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_DiffieHellmanPublickey( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_DiffieHellmanPublickeyType, AUTOMATIC, EXI_APPL_DATA) DiffieHellmanPublickeyPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DiffieHellmanPublickeyPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((DiffieHellmanPublickeyPtr->Length <= sizeof(DiffieHellmanPublickeyPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (DiffieHellmanPublickeyPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &DiffieHellmanPublickeyPtr->Buffer[0], DiffieHellmanPublickeyPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EMAID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EMAIDType, AUTOMATIC, EXI_APPL_DATA) EMAIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EMAIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EMAIDPtr->Length <= sizeof(EMAIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (EMAIDPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &EMAIDPtr->Buffer[0], EMAIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EMAID_ElementFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EMAIDType, AUTOMATIC, EXI_APPL_DATA) EMAIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EMAIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EMAIDPtr->Length <= sizeof(EMAIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Id) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 9);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
  Exi_Encode_ISO_AttributeId(EncWsPtr, (EMAIDPtr->Id));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EMAID, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  /* CH[untyped value] */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 256, 9);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &EMAIDPtr->Buffer[0], EMAIDPtr->Length);
  /* EE */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EVChargeParameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVChargeParameterType, AUTOMATIC, EXI_APPL_DATA) EVChargeParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EVChargeParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCHARGE_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == EVChargeParameterPtr->DepartureTimeFlag)
  {
    /* SE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(EVChargeParameterPtr->DepartureTime));
    /* EE(DepartureTime) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(EVChargeParameter) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSENotification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSENotificationType, AUTOMATIC, EXI_APPL_DATA) EVSENotificationPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EVSENotificationPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSENOTIFICATION, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_EVSENOTIFICATION_TYPE_NONE == *EVSENotificationPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_EVSENOTIFICATION_TYPE_STOP_CHARGING == *EVSENotificationPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSEProcessing( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSEProcessingType, AUTOMATIC, EXI_APPL_DATA) EVSEProcessingPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EVSEProcessingPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSEPROCESSING, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_EVSEPROCESSING_TYPE_FINISHED == *EVSEProcessingPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_EVSEPROCESSING_TYPE_ONGOING == *EVSEProcessingPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EVSEStatus( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EVSEStatusType, AUTOMATIC, EXI_APPL_DATA) EVSEStatusPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EVSEStatusPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(EVSEStatusPtr->NotificationMaxDelay));
  /* EE(NotificationMaxDelay) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON))
  Exi_Encode_ISO_EVSENotification(EncWsPtr, &(EVSEStatusPtr->EVSENotification));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSESTATUS, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON)) */
  /* EE(EVSENotification) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_EnergyTransferMode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EnergyTransferModeType, AUTOMATIC, EXI_APPL_DATA) EnergyTransferModePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EnergyTransferModePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_SINGLE_PHASE_CORE == *EnergyTransferModePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
  }
  else if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_AC_THREE_PHASE_CORE == *EnergyTransferModePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
  }
  else if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_CORE == *EnergyTransferModePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
  }
  else if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_EXTENDED == *EnergyTransferModePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
  }
  else if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE_DC_COMBO_CORE == *EnergyTransferModePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 3);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Entry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_EntryType, AUTOMATIC, EXI_APPL_DATA) EntryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EntryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Start of Substitution Group */
  if(EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE == EntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
    /* SE(RelativeTimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_RelativeTimeInterval(EncWsPtr, (Exi_ISO_RelativeTimeIntervalType*)EntryPtr->TimeInterval); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(RelativeTimeInterval) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(RelativeTimeInterval) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */
  }
  else if(EXI_ISO_TIME_INTERVAL_TYPE == EntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON))
    /* SE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    /* EE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ListOfRootCertificateIDs( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ListOfRootCertificateIDsType, AUTOMATIC, EXI_APPL_DATA) ListOfRootCertificateIDsPtr )
{
  uint8 i = 0;
  Exi_XMLSIG_X509IssuerSerialType* nextPtr = (Exi_XMLSIG_X509IssuerSerialType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ListOfRootCertificateIDsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_XMLSIG_X509IssuerSerialType*)ListOfRootCertificateIDsPtr->RootCertificateID;
  for(i=0; i<20; i++)
  {
    /* SE(RootCertificateID) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
    Exi_Encode_XMLSIG_X509IssuerSerial(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */
    /* EE(RootCertificateID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_XMLSIG_X509IssuerSerialType*)(nextPtr->NextRootCertificateIDPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_X509IssuerSerialType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ListOfRootCertificateIDs) */
  if( i < 20)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_MessageHeader( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MessageHeaderType, AUTOMATIC, EXI_APPL_DATA) MessageHeaderPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != MessageHeaderPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(SessionID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON))
  Exi_Encode_ISO_sessionID(EncWsPtr, (MessageHeaderPtr->SessionID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON)) */
  /* EE(SessionID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == MessageHeaderPtr->NotificationFlag)
  {
    /* SE(Notification) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON))
    Exi_Encode_ISO_Notification(EncWsPtr, (MessageHeaderPtr->Notification));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON)) */
    /* EE(Notification) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Notification) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  if(1 == MessageHeaderPtr->SignatureFlag)
  {
    /* SE(Signature) */
    if(0 == MessageHeaderPtr->NotificationFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON))
    Exi_Encode_XMLSIG_Signature(EncWsPtr, (MessageHeaderPtr->Signature));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_MESSAGE_HEADER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON)) */
    /* EE(Signature) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Signature) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(MessageHeader) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == MessageHeaderPtr->NotificationFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_MeterInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeterInfoType, AUTOMATIC, EXI_APPL_DATA) MeterInfoPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != MeterInfoPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(MeterID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON))
  Exi_Encode_ISO_meterID(EncWsPtr, (MeterInfoPtr->MeterID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON)) */
  /* EE(MeterID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == MeterInfoPtr->MeterReadingFlag)
  {
    /* SE(MeterReading) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt64(&EncWsPtr->EncWs, (MeterInfoPtr->MeterReading));
    /* EE(MeterReading) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == MeterInfoPtr->SigMeterReadingFlag)
  {
    /* SE(SigMeterReading) */
    if(0 == MeterInfoPtr->MeterReadingFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    }
    #if (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON))
    Exi_Encode_ISO_sigMeterReading(EncWsPtr, (MeterInfoPtr->SigMeterReading));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_INFO, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON)) */
    /* EE(SigMeterReading) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == MeterInfoPtr->MeterStatusFlag)
  {
    /* SE(MeterStatus) */
    if(0 == MeterInfoPtr->SigMeterReadingFlag)
    {
      if(0 == MeterInfoPtr->MeterReadingFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(MeterInfoPtr->MeterStatus));
    /* EE(MeterStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == MeterInfoPtr->TMeterFlag)
  {
    /* SE(TMeter) */
    if(0 == MeterInfoPtr->MeterStatusFlag)
    {
      if(0 == MeterInfoPtr->SigMeterReadingFlag)
      {
        if(0 == MeterInfoPtr->MeterReadingFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt64(&EncWsPtr->EncWs, (MeterInfoPtr->TMeter));
    /* EE(TMeter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(MeterInfo) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == MeterInfoPtr->MeterStatusFlag)
    {
      if(0 == MeterInfoPtr->SigMeterReadingFlag)
      {
        if(0 == MeterInfoPtr->MeterReadingFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_MeteringReceiptReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeteringReceiptReqType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != MeteringReceiptReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((0 == MeteringReceiptReqPtr->SAScheduleTupleIDFlag) || ((1 <= MeteringReceiptReqPtr->SAScheduleTupleID) && (MeteringReceiptReqPtr->SAScheduleTupleID <= 255))), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == MeteringReceiptReqPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_ISO_AttributeId(EncWsPtr, (MeteringReceiptReqPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* SE(SessionID) */
  if(0 == MeteringReceiptReqPtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON))
  Exi_Encode_ISO_sessionID(EncWsPtr, (MeteringReceiptReqPtr->SessionID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON)) */
  /* EE(SessionID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == MeteringReceiptReqPtr->SAScheduleTupleIDFlag)
  {
    /* SE(SAScheduleTupleID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(MeteringReceiptReqPtr->SAScheduleTupleID - 1), 8);
    /* EE(SAScheduleTupleID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(MeterInfo) */
  if(0 == MeteringReceiptReqPtr->SAScheduleTupleIDFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON))
  Exi_Encode_ISO_MeterInfo(EncWsPtr, (MeteringReceiptReqPtr->MeterInfo));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON)) */
  /* EE(MeterInfo) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(MeterInfo) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_MeteringReceiptRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_MeteringReceiptResType, AUTOMATIC, EXI_APPL_DATA) MeteringReceiptResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != MeteringReceiptResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(MeteringReceiptResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* Start of Substitution Group */
  if(EXI_ISO_AC_EVSESTATUS_TYPE == MeteringReceiptResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
    /* SE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, (Exi_ISO_AC_EVSEStatusType*)MeteringReceiptResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */
  }
  else if(EXI_ISO_DC_EVSESTATUS_TYPE == MeteringReceiptResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
    /* SE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (Exi_ISO_DC_EVSEStatusType*)MeteringReceiptResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(DC_EVSEStatus) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVSEStatus) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  }
  else if(EXI_ISO_EVSESTATUS_TYPE == MeteringReceiptResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON))
    /* SE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    Exi_Encode_ISO_EVSEStatus(EncWsPtr, (Exi_ISO_EVSEStatusType*)MeteringReceiptResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METERING_RECEIPT_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Notification( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_NotificationType, AUTOMATIC, EXI_APPL_DATA) NotificationPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != NotificationPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(FaultCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON))
  Exi_Encode_ISO_faultCode(EncWsPtr, &(NotificationPtr->FaultCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON)) */
  /* EE(FaultCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == NotificationPtr->FaultMsgFlag)
  {
    /* SE(FaultMsg) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON))
    Exi_Encode_ISO_faultMsg(EncWsPtr, (NotificationPtr->FaultMsg));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_NOTIFICATION, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON)) */
    /* EE(FaultMsg) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(Notification) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PMaxScheduleEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PMaxScheduleEntryType, AUTOMATIC, EXI_APPL_DATA) PMaxScheduleEntryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PMaxScheduleEntryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Start of Substitution Group */
  if(EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE == PMaxScheduleEntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
    /* SE(RelativeTimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_RelativeTimeInterval(EncWsPtr, (Exi_ISO_RelativeTimeIntervalType*)PMaxScheduleEntryPtr->TimeInterval); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(RelativeTimeInterval) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(RelativeTimeInterval) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */
  }
  else if(EXI_ISO_TIME_INTERVAL_TYPE == PMaxScheduleEntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON))
    /* SE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    /* EE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  /* SE(PMax) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (PMaxScheduleEntryPtr->PMax));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE_ENTRY, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(PMax) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PMaxSchedule( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PMaxScheduleType, AUTOMATIC, EXI_APPL_DATA) PMaxSchedulePtr )
{
  uint16 i = 0;
  Exi_ISO_PMaxScheduleEntryType* nextPtr = (Exi_ISO_PMaxScheduleEntryType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PMaxSchedulePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_PMaxScheduleEntryType*)PMaxSchedulePtr->PMaxScheduleEntry;
  for(i=0; i<1024; i++)
  {
    /* SE(PMaxScheduleEntry) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON))
    Exi_Encode_ISO_PMaxScheduleEntry(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PMAX_SCHEDULE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON)) */
    /* EE(PMaxScheduleEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_ISO_PMaxScheduleEntryType*)(nextPtr->NextPMaxScheduleEntryPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_PMaxScheduleEntryType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(PMaxSchedule) */
  if( i < 1024)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ParameterSet( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ParameterSetType, AUTOMATIC, EXI_APPL_DATA) ParameterSetPtr )
{
  uint8 i = 0;
  Exi_ISO_ParameterType* nextPtr = (Exi_ISO_ParameterType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ParameterSetPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ParameterSetID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(ParameterSetPtr->ParameterSetID));
  /* EE(ParameterSetID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  nextPtr = (Exi_ISO_ParameterType*)ParameterSetPtr->Parameter;
  for(i=0; i<16; i++)
  {
    /* SE(Parameter) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON))
    Exi_Encode_ISO_Parameter(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER_SET, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON)) */
    /* EE(Parameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_ISO_ParameterType*)(nextPtr->NextParameterPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_ParameterType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ParameterSet) */
  if( i < 16)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Parameter( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ParameterType, AUTOMATIC, EXI_APPL_DATA) ParameterPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ParameterPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ParameterPtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Name) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_NAME) && (EXI_ENCODE_ISO_ATTRIBUTE_NAME == STD_ON))
  Exi_Encode_ISO_AttributeName(EncWsPtr, (ParameterPtr->Name));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_NAME) && (EXI_ENCODE_ISO_ATTRIBUTE_NAME == STD_ON)) */
  
  /* Start of Choice Element */
  if(1 == ParameterPtr->ChoiceElement->boolValueFlag)
  {
    /* SE(boolValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBool(&EncWsPtr->EncWs, (ParameterPtr->ChoiceElement->ChoiceValue.boolValue));
    /* EE(boolValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == ParameterPtr->ChoiceElement->byteValueFlag)
  {
    /* SE(byteValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(ParameterPtr->ChoiceElement->ChoiceValue.byteValue + 128), 8);
    /* EE(byteValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == ParameterPtr->ChoiceElement->shortValueFlag)
  {
    /* SE(shortValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(ParameterPtr->ChoiceElement->ChoiceValue.shortValue));
    /* EE(shortValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == ParameterPtr->ChoiceElement->intValueFlag)
  {
    /* SE(intValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(ParameterPtr->ChoiceElement->ChoiceValue.intValue));
    /* EE(intValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == ParameterPtr->ChoiceElement->physicalValueFlag)
  {
    /* SE(physicalValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, (ParameterPtr->ChoiceElement->ChoiceValue.physicalValue));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
    /* EE(physicalValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == ParameterPtr->ChoiceElement->stringValueFlag)
  {
    /* SE(stringValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 3);
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
    Exi_Encode_string(EncWsPtr, (ParameterPtr->ChoiceElement->ChoiceValue.stringValue));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PARAMETER, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
    /* EE(stringValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* Choice Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Choice Element */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentDetailsReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentDetailsReqType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PaymentDetailsReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON))
  Exi_Encode_ISO_eMAID(EncWsPtr, (PaymentDetailsReqPtr->eMAID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON)) */
  /* EE(eMAID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(ContractSignatureCertChain) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
  Exi_Encode_ISO_CertificateChain(EncWsPtr, (PaymentDetailsReqPtr->ContractSignatureCertChain));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  /* EE(ContractSignatureCertChain) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ContractSignatureCertChain) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentDetailsRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentDetailsResType, AUTOMATIC, EXI_APPL_DATA) PaymentDetailsResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PaymentDetailsResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(PaymentDetailsResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(GenChallenge) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON))
  Exi_Encode_ISO_genChallenge(EncWsPtr, (PaymentDetailsResPtr->GenChallenge));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_DETAILS_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON)) */
  /* EE(GenChallenge) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSETimeStamp) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeInt64(&EncWsPtr->EncWs, (PaymentDetailsResPtr->EVSETimeStamp));
  /* EE(EVSETimeStamp) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentOptionList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentOptionListType, AUTOMATIC, EXI_APPL_DATA) PaymentOptionListPtr )
{
  uint8 i = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PaymentOptionListPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  for(i=0; i<PaymentOptionListPtr->PaymentOptionCount; i++)
  {
    /* SE(PaymentOption) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON))
    Exi_Encode_ISO_paymentOption(EncWsPtr, &(PaymentOptionListPtr->PaymentOption[i]));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION_LIST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON)) */
    /* EE(PaymentOption) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    if(i == (PaymentOptionListPtr->PaymentOptionCount - 1))
    {
      /* i holds the number of encoded Exi_ISO_paymentOptionType elements */
      i = PaymentOptionListPtr->PaymentOptionCount; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(PaymentOptionList) */
  if( i < 2)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentServiceSelectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentServiceSelectionReqType, AUTOMATIC, EXI_APPL_DATA) PaymentServiceSelectionReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PaymentServiceSelectionReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(SelectedPaymentOption) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON))
  Exi_Encode_ISO_paymentOption(EncWsPtr, &(PaymentServiceSelectionReqPtr->SelectedPaymentOption));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON)) */
  /* EE(SelectedPaymentOption) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SelectedServiceList) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON))
  Exi_Encode_ISO_SelectedServiceList(EncWsPtr, (PaymentServiceSelectionReqPtr->SelectedServiceList));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON)) */
  /* EE(SelectedServiceList) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SelectedServiceList) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PaymentServiceSelectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PaymentServiceSelectionResType, AUTOMATIC, EXI_APPL_DATA) PaymentServiceSelectionResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PaymentServiceSelectionResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(PaymentServiceSelectionResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PhysicalValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PhysicalValueType, AUTOMATIC, EXI_APPL_DATA) PhysicalValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PhysicalValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((-3 <= PhysicalValuePtr->Multiplier) && (PhysicalValuePtr->Multiplier <= 3)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(Multiplier) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(PhysicalValuePtr->Multiplier + 3), 3);
  /* EE(Multiplier) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(Unit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON))
  Exi_Encode_ISO_unitSymbol(EncWsPtr, &(PhysicalValuePtr->Unit));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PHYSICAL_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON)) */
  /* EE(Unit) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(Value) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(PhysicalValuePtr->Value));
  /* EE(Value) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PowerDeliveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PowerDeliveryReqType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PowerDeliveryReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= PowerDeliveryReqPtr->SAScheduleTupleID) && (PowerDeliveryReqPtr->SAScheduleTupleID <= 255)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ChargeProgress) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON))
  Exi_Encode_ISO_chargeProgress(EncWsPtr, &(PowerDeliveryReqPtr->ChargeProgress));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON)) */
  /* EE(ChargeProgress) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(PowerDeliveryReqPtr->SAScheduleTupleID - 1), 8);
  /* EE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == PowerDeliveryReqPtr->ChargingProfileFlag)
  {
    /* SE(ChargingProfile) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON))
    Exi_Encode_ISO_ChargingProfile(EncWsPtr, (PowerDeliveryReqPtr->ChargingProfile));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON)) */
    /* EE(ChargingProfile) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ChargingProfile) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  if(1 == PowerDeliveryReqPtr->EVPowerDeliveryParameterFlag)
  {
    /* Start of Substitution Group */
    if(EXI_ISO_DC_EVPOWER_DELIVERY_PARAMETER_TYPE == PowerDeliveryReqPtr->EVPowerDeliveryParameterElementId)
    {
    #if (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
      /* SE(DC_EVPowerDeliveryParameter) */
      if(0 == PowerDeliveryReqPtr->ChargingProfileFlag)
      {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      }
      EncWsPtr->EncWs.EERequired = TRUE;
      Exi_Encode_ISO_DC_EVPowerDeliveryParameter(EncWsPtr, (Exi_ISO_DC_EVPowerDeliveryParameterType*)PowerDeliveryReqPtr->EVPowerDeliveryParameter); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      /* EE(DC_EVPowerDeliveryParameter) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(DC_EVPowerDeliveryParameter) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    #endif /* (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */
    }
    else if(EXI_ISO_EVPOWER_DELIVERY_PARAMETER_TYPE == PowerDeliveryReqPtr->EVPowerDeliveryParameterElementId)
    {
    #if (defined(EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER == STD_ON))
      /* SE(EVPowerDeliveryParameter) */
      if(0 == PowerDeliveryReqPtr->ChargingProfileFlag)
      {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
      /* EE(EVPowerDeliveryParameter) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    #else
      /* encode as missing optional element */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    #endif /* (defined(EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */
    }
    else
    {
      /* Substitution Element not supported */
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    /* End of Substitution Group */
  }
  else
  {
    /* EE(PowerDeliveryReq) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == PowerDeliveryReqPtr->ChargingProfileFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PowerDeliveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PowerDeliveryResType, AUTOMATIC, EXI_APPL_DATA) PowerDeliveryResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PowerDeliveryResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(PowerDeliveryResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* Start of Substitution Group */
  if(EXI_ISO_AC_EVSESTATUS_TYPE == PowerDeliveryResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
    /* SE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, (Exi_ISO_AC_EVSEStatusType*)PowerDeliveryResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */
  }
  else if(EXI_ISO_DC_EVSESTATUS_TYPE == PowerDeliveryResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
    /* SE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (Exi_ISO_DC_EVSEStatusType*)PowerDeliveryResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(DC_EVSEStatus) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVSEStatus) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  }
  else if(EXI_ISO_EVSESTATUS_TYPE == PowerDeliveryResPtr->EVSEStatusElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON))
    /* SE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    Exi_Encode_ISO_EVSEStatus(EncWsPtr, (Exi_ISO_EVSEStatusType*)PowerDeliveryResPtr->EVSEStatus); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_POWER_DELIVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PreChargeReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PreChargeReqType, AUTOMATIC, EXI_APPL_DATA) PreChargeReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PreChargeReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (PreChargeReqPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVTargetVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (PreChargeReqPtr->EVTargetVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVTargetVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVTargetCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (PreChargeReqPtr->EVTargetCurrent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVTargetCurrent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_PreChargeRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_PreChargeResType, AUTOMATIC, EXI_APPL_DATA) PreChargeResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PreChargeResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(PreChargeResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (PreChargeResPtr->DC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  /* EE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (PreChargeResPtr->EVSEPresentVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PRE_CHARGE_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ProfileEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ProfileEntryType, AUTOMATIC, EXI_APPL_DATA) ProfileEntryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ProfileEntryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((0 == ProfileEntryPtr->ChargingProfileEntryMaxNumberOfPhasesInUseFlag) || ((1 <= ProfileEntryPtr->ChargingProfileEntryMaxNumberOfPhasesInUse) && (ProfileEntryPtr->ChargingProfileEntryMaxNumberOfPhasesInUse <= 3))), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ChargingProfileEntryStart) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ProfileEntryPtr->ChargingProfileEntryStart));
  /* EE(ChargingProfileEntryStart) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(ChargingProfileEntryMaxPower) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (ProfileEntryPtr->ChargingProfileEntryMaxPower));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PROFILE_ENTRY, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(ChargingProfileEntryMaxPower) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ProfileEntryPtr->ChargingProfileEntryMaxNumberOfPhasesInUseFlag)
  {
    /* SE(ChargingProfileEntryMaxNumberOfPhasesInUse) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(ProfileEntryPtr->ChargingProfileEntryMaxNumberOfPhasesInUse - 1), 2);
    /* EE(ChargingProfileEntryMaxNumberOfPhasesInUse) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ProfileEntry) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_RelativeTimeInterval( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_RelativeTimeIntervalType, AUTOMATIC, EXI_APPL_DATA) RelativeTimeIntervalPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != RelativeTimeIntervalPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RELATIVE_TIME_INTERVAL, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(start) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(RelativeTimeIntervalPtr->start));
  /* EE(start) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == RelativeTimeIntervalPtr->durationFlag)
  {
    /* SE(duration) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(RelativeTimeIntervalPtr->duration));
    /* EE(duration) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(RelativeTimeInterval) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SAScheduleList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SAScheduleListType, AUTOMATIC, EXI_APPL_DATA) SAScheduleListPtr )
{
  uint8 i = 0;
  Exi_ISO_SAScheduleTupleType* nextPtr = (Exi_ISO_SAScheduleTupleType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SAScheduleListPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_SAScheduleTupleType*)SAScheduleListPtr->SAScheduleTuple;
  for(i=0; i<3; i++)
  {
    /* SE(SAScheduleTuple) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON))
    Exi_Encode_ISO_SAScheduleTuple(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_LIST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON)) */
    /* EE(SAScheduleTuple) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SAScheduleTuple) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_SAScheduleTupleType*)(nextPtr->NextSAScheduleTuplePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_SAScheduleTupleType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SAScheduleList) */
  if( i < 3)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SAScheduleTuple( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SAScheduleTupleType, AUTOMATIC, EXI_APPL_DATA) SAScheduleTuplePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SAScheduleTuplePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= SAScheduleTuplePtr->SAScheduleTupleID) && (SAScheduleTuplePtr->SAScheduleTupleID <= 255)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(SAScheduleTuplePtr->SAScheduleTupleID - 1), 8);
  /* EE(SAScheduleTupleID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(PMaxSchedule) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON))
  Exi_Encode_ISO_PMaxSchedule(EncWsPtr, (SAScheduleTuplePtr->PMaxSchedule));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON)) */
  /* EE(PMaxSchedule) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(PMaxSchedule) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  if(1 == SAScheduleTuplePtr->SalesTariffFlag)
  {
    /* SE(SalesTariff) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON))
    Exi_Encode_ISO_SalesTariff(EncWsPtr, (SAScheduleTuplePtr->SalesTariff));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SASCHEDULE_TUPLE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON)) */
    /* EE(SalesTariff) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SalesTariff) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(SAScheduleTuple) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SalesTariffEntry( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SalesTariffEntryType, AUTOMATIC, EXI_APPL_DATA) SalesTariffEntryPtr )
{
  uint8 i = 0;
  Exi_ISO_ConsumptionCostType* nextPtr = (Exi_ISO_ConsumptionCostType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SalesTariffEntryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Start of Substitution Group */
  if(EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE == SalesTariffEntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
    /* SE(RelativeTimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_RelativeTimeInterval(EncWsPtr, (Exi_ISO_RelativeTimeIntervalType*)SalesTariffEntryPtr->TimeInterval); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    /* EE(RelativeTimeInterval) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(RelativeTimeInterval) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */
  }
  else if(EXI_ISO_TIME_INTERVAL_TYPE == SalesTariffEntryPtr->TimeIntervalElementId)
  {
  #if (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON))
    /* SE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    /* EE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_TIME_INTERVAL) && (EXI_ENCODE_ISO_TIME_INTERVAL == STD_ON)) */
  }
  else
  {
    /* Substitution Element not supported */
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* End of Substitution Group */
  if(1 == SalesTariffEntryPtr->EPriceLevelFlag)
  {
    /* SE(EPriceLevel) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(SalesTariffEntryPtr->EPriceLevel), 8);
    /* EE(EPriceLevel) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == SalesTariffEntryPtr->ConsumptionCostFlag)
  {
    boolean IsFirstLoopElement = TRUE;
    /* SE(ConsumptionCost) */
    if(0 == SalesTariffEntryPtr->EPriceLevelFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    nextPtr = (Exi_ISO_ConsumptionCostType*)SalesTariffEntryPtr->ConsumptionCost;
    for(i=0; i<3; i++)
    {
      if (TRUE == IsFirstLoopElement)
      {
        /* First element SE already encoded */
        IsFirstLoopElement = FALSE;
      }
      else
      {
        /* Next SE(ConsumptionCost) */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      }
      EncWsPtr->EncWs.EERequired = TRUE;
      #if (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON))
      Exi_Encode_ISO_ConsumptionCost(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF_ENTRY, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON)) */
      /* EE(ConsumptionCost) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE(ConsumptionCost) already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
      nextPtr = (Exi_ISO_ConsumptionCostType*)(nextPtr->NextConsumptionCostPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_ISO_ConsumptionCostType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(SalesTariffEntry) */
    if(i < 3)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      EncWsPtr->EncWs.EERequired = FALSE;
    }
  }
  else
  {
    /* EE(SalesTariffEntry) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == SalesTariffEntryPtr->EPriceLevelFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SalesTariff( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SalesTariffType, AUTOMATIC, EXI_APPL_DATA) SalesTariffPtr )
{
  uint16 i = 0;
  Exi_ISO_SalesTariffEntryType* nextPtr = (Exi_ISO_SalesTariffEntryType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SalesTariffPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= SalesTariffPtr->SalesTariffID) && (SalesTariffPtr->SalesTariffID <= 255)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SalesTariffPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_ISO_AttributeId(EncWsPtr, (SalesTariffPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ATTRIBUTE_ID) && (EXI_ENCODE_ISO_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* SE(SalesTariffID) */
  if(0 == SalesTariffPtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(SalesTariffPtr->SalesTariffID - 1), 8);
  /* EE(SalesTariffID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == SalesTariffPtr->SalesTariffDescriptionFlag)
  {
    /* SE(SalesTariffDescription) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON))
    Exi_Encode_ISO_tariffDescription(EncWsPtr, (SalesTariffPtr->SalesTariffDescription));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON)) */
    /* EE(SalesTariffDescription) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == SalesTariffPtr->NumEPriceLevelsFlag)
  {
    /* SE(NumEPriceLevels) */
    if(0 == SalesTariffPtr->SalesTariffDescriptionFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(SalesTariffPtr->NumEPriceLevels), 8);
    /* EE(NumEPriceLevels) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  nextPtr = (Exi_ISO_SalesTariffEntryType*)SalesTariffPtr->SalesTariffEntry;
  for(i=0; i<1024; i++)
  {
    /* SE(SalesTariffEntry) */
    if(0 == i)
    {
      if(0 == SalesTariffPtr->NumEPriceLevelsFlag)
      {
        if(0 == SalesTariffPtr->SalesTariffDescriptionFlag)
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
        }
        else
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
        }
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON))
    Exi_Encode_ISO_SalesTariffEntry(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SALES_TARIFF, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON)) */
    /* EE(SalesTariffEntry) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SalesTariffEntry) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_SalesTariffEntryType*)(nextPtr->NextSalesTariffEntryPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_SalesTariffEntryType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SalesTariff) */
  if( i < 1024)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SelectedServiceList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SelectedServiceListType, AUTOMATIC, EXI_APPL_DATA) SelectedServiceListPtr )
{
  uint8 i = 0;
  Exi_ISO_SelectedServiceType* nextPtr = (Exi_ISO_SelectedServiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SelectedServiceListPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_SelectedServiceType*)SelectedServiceListPtr->SelectedService;
  for(i=0; i<16; i++)
  {
    /* SE(SelectedService) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON))
    Exi_Encode_ISO_SelectedService(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE_LIST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON)) */
    /* EE(SelectedService) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SelectedService) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_SelectedServiceType*)(nextPtr->NextSelectedServicePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_SelectedServiceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SelectedServiceList) */
  if( i < 16)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SelectedService( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SelectedServiceType, AUTOMATIC, EXI_APPL_DATA) SelectedServicePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SelectedServicePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SELECTED_SERVICE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(SelectedServicePtr->ServiceID));
  /* EE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == SelectedServicePtr->ParameterSetIDFlag)
  {
    /* SE(ParameterSetID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt(&EncWsPtr->EncWs, (sint32)(SelectedServicePtr->ParameterSetID));
    /* EE(ParameterSetID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SelectedService) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDetailReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDetailReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceDetailReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ServiceDetailReqPtr->ServiceID));
  /* EE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDetailRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDetailResType, AUTOMATIC, EXI_APPL_DATA) ServiceDetailResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceDetailResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(ServiceDetailResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ServiceDetailResPtr->ServiceID));
  /* EE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ServiceDetailResPtr->ServiceParameterListFlag)
  {
    /* SE(ServiceParameterList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON))
    Exi_Encode_ISO_ServiceParameterList(EncWsPtr, (ServiceDetailResPtr->ServiceParameterList));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DETAIL_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON)) */
    /* EE(ServiceParameterList) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ServiceParameterList) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(ServiceDetailRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDiscoveryReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDiscoveryReqType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceDiscoveryReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == ServiceDiscoveryReqPtr->ServiceScopeFlag)
  {
    /* SE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON))
    Exi_Encode_ISO_serviceScope(EncWsPtr, (ServiceDiscoveryReqPtr->ServiceScope));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON)) */
    /* EE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == ServiceDiscoveryReqPtr->ServiceCategoryFlag)
  {
    /* SE(ServiceCategory) */
    if(0 == ServiceDiscoveryReqPtr->ServiceScopeFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON))
    Exi_Encode_ISO_serviceCategory(EncWsPtr, &(ServiceDiscoveryReqPtr->ServiceCategory));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON)) */
    /* EE(ServiceCategory) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(ServiceDiscoveryReq) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == ServiceDiscoveryReqPtr->ServiceScopeFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceDiscoveryRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceDiscoveryResType, AUTOMATIC, EXI_APPL_DATA) ServiceDiscoveryResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceDiscoveryResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(ServiceDiscoveryResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(PaymentOptionList) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON))
  Exi_Encode_ISO_PaymentOptionList(EncWsPtr, (ServiceDiscoveryResPtr->PaymentOptionList));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON)) */
  /* EE(PaymentOptionList) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(PaymentOptionList) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(ChargeService) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON))
  Exi_Encode_ISO_ChargeService(EncWsPtr, (ServiceDiscoveryResPtr->ChargeService));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON)) */
  /* EE(ChargeService) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ServiceDiscoveryResPtr->ServiceListFlag)
  {
    /* SE(ServiceList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON))
    Exi_Encode_ISO_ServiceList(EncWsPtr, (ServiceDiscoveryResPtr->ServiceList));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_DISCOVERY_RES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON)) */
    /* EE(ServiceList) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ServiceList) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(ServiceDiscoveryRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceListType, AUTOMATIC, EXI_APPL_DATA) ServiceListPtr )
{
  uint8 i = 0;
  Exi_ISO_ServiceType* nextPtr = (Exi_ISO_ServiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceListPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_ServiceType*)ServiceListPtr->Service;
  for(i=0; i<8; i++)
  {
    /* SE(Service) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON))
    Exi_Encode_ISO_Service(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_LIST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON)) */
    /* EE(Service) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_ISO_ServiceType*)(nextPtr->NextServicePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_ServiceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ServiceList) */
  if( i < 8)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_ServiceParameterList( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceParameterListType, AUTOMATIC, EXI_APPL_DATA) ServiceParameterListPtr )
{
  uint8 i = 0;
  Exi_ISO_ParameterSetType* nextPtr = (Exi_ISO_ParameterSetType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServiceParameterListPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_ParameterSetType*)ServiceParameterListPtr->ParameterSet;
  for(i=0; i<255; i++)
  {
    /* SE(ParameterSet) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON))
    Exi_Encode_ISO_ParameterSet(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_PARAMETER_LIST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON)) */
    /* EE(ParameterSet) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ParameterSet) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_ISO_ParameterSetType*)(nextPtr->NextParameterSetPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_ParameterSetType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(ServiceParameterList) */
  if( i < 255)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_Service( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_ServiceType, AUTOMATIC, EXI_APPL_DATA) ServicePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ServicePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(ServicePtr->ServiceID));
  /* EE(ServiceID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ServicePtr->ServiceNameFlag)
  {
    /* SE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON))
    Exi_Encode_ISO_serviceName(EncWsPtr, (ServicePtr->ServiceName));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON)) */
    /* EE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(ServiceCategory) */
  if(0 == ServicePtr->ServiceNameFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON))
  Exi_Encode_ISO_serviceCategory(EncWsPtr, &(ServicePtr->ServiceCategory));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON)) */
  /* EE(ServiceCategory) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == ServicePtr->ServiceScopeFlag)
  {
    /* SE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON))
    Exi_Encode_ISO_serviceScope(EncWsPtr, (ServicePtr->ServiceScope));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON)) */
    /* EE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(FreeService) */
  if(0 == ServicePtr->ServiceScopeFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBool(&EncWsPtr->EncWs, (ServicePtr->FreeService));
  /* EE(FreeService) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionSetupReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionSetupReqType, AUTOMATIC, EXI_APPL_DATA) SessionSetupReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SessionSetupReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(EVCCID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON))
  Exi_Encode_ISO_evccID(EncWsPtr, (SessionSetupReqPtr->EVCCID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON)) */
  /* EE(EVCCID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionSetupRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionSetupResType, AUTOMATIC, EXI_APPL_DATA) SessionSetupResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SessionSetupResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(SessionSetupResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON))
  Exi_Encode_ISO_evseID(EncWsPtr, (SessionSetupResPtr->EVSEID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_SETUP_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON)) */
  /* EE(EVSEID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == SessionSetupResPtr->EVSETimeStampFlag)
  {
    /* SE(EVSETimeStamp) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeInt64(&EncWsPtr->EncWs, (SessionSetupResPtr->EVSETimeStamp));
    /* EE(EVSETimeStamp) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SessionSetupRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionStopReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionStopReqType, AUTOMATIC, EXI_APPL_DATA) SessionStopReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SessionStopReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ChargingSession) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON))
  Exi_Encode_ISO_chargingSession(EncWsPtr, &(SessionStopReqPtr->ChargingSession));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON)) */
  /* EE(ChargingSession) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SessionStopRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SessionStopResType, AUTOMATIC, EXI_APPL_DATA) SessionStopResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SessionStopResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(SessionStopResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_STOP_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SubCertificates( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SubCertificatesType, AUTOMATIC, EXI_APPL_DATA) SubCertificatesPtr )
{
  uint8 i = 0;
  Exi_ISO_certificateType* nextPtr = (Exi_ISO_certificateType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SubCertificatesPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_ISO_certificateType*)SubCertificatesPtr->Certificate;
  for(i=0; i<4; i++)
  {
    /* SE(Certificate) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
    Exi_Encode_ISO_certificate(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUB_CERTIFICATES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */
    /* EE(Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_ISO_certificateType*)(nextPtr->NextCertificatePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_ISO_certificateType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SubCertificates) */
  if( i < 4)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_SupportedEnergyTransferMode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_SupportedEnergyTransferModeType, AUTOMATIC, EXI_APPL_DATA) SupportedEnergyTransferModePtr )
{
  uint8 i = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SupportedEnergyTransferModePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  for(i=0; i<SupportedEnergyTransferModePtr->EnergyTransferModeCount; i++)
  {
    /* SE(EnergyTransferMode) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
    Exi_Encode_ISO_EnergyTransferMode(EncWsPtr, &(SupportedEnergyTransferModePtr->EnergyTransferMode[i]));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */
    /* EE(EnergyTransferMode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    if(i == (SupportedEnergyTransferModePtr->EnergyTransferModeCount - 1))
    {
      /* i holds the number of encoded Exi_ISO_EnergyTransferModeType elements */
      i = SupportedEnergyTransferModePtr->EnergyTransferModeCount; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SupportedEnergyTransferMode) */
  if( i < 6)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_V2G_Message( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_V2G_MessageType, AUTOMATIC, EXI_APPL_DATA) V2G_MessagePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != V2G_MessagePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(Header) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON))
  Exi_Encode_ISO_MessageHeader(EncWsPtr, (V2G_MessagePtr->Header));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON)) */
  /* EE(Header) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(Header) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(Body) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON))
  Exi_Encode_ISO_Body(EncWsPtr, (V2G_MessagePtr->Body));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_V2G_MESSAGE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON)) */
  /* EE(Body) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(Body) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_WeldingDetectionReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_WeldingDetectionReqType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionReqPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != WeldingDetectionReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVStatus(EncWsPtr, (WeldingDetectionReqPtr->DC_EVStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_REQ, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  /* EE(DC_EVStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_WeldingDetectionRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_WeldingDetectionResType, AUTOMATIC, EXI_APPL_DATA) WeldingDetectionResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != WeldingDetectionResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
  Exi_Encode_ISO_responseCode(EncWsPtr, &(WeldingDetectionResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
  Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, (WeldingDetectionResPtr->DC_EVSEStatus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  /* EE(DC_EVSEStatus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
  Exi_Encode_ISO_PhysicalValue(EncWsPtr, (WeldingDetectionResPtr->EVSEPresentVoltage));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_WELDING_DETECTION_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  /* EE(EVSEPresentVoltage) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_certificate( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_certificateType, AUTOMATIC, EXI_APPL_DATA) certificatePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != certificatePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((certificatePtr->Length <= sizeof(certificatePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CERTIFICATE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &certificatePtr->Buffer[0], certificatePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_chargeProgress( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_chargeProgressType, AUTOMATIC, EXI_APPL_DATA) chargeProgressPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != chargeProgressPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGE_PROGRESS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_CHARGE_PROGRESS_TYPE_START == *chargeProgressPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_CHARGE_PROGRESS_TYPE_STOP == *chargeProgressPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_chargingSession( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_chargingSessionType, AUTOMATIC, EXI_APPL_DATA) chargingSessionPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != chargingSessionPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_CHARGING_SESSION, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_CHARGING_SESSION_TYPE_TERMINATE == *chargingSessionPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 1);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_costKind( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_costKindType, AUTOMATIC, EXI_APPL_DATA) costKindPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != costKindPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_COST_KIND, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_COST_KIND_TYPE_RELATIVE_PRICE_PERCENTAGE == *costKindPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_COST_KIND_TYPE_RENEWABLE_GENERATION_PERCENTAGE == *costKindPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_eMAID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_eMAIDType, AUTOMATIC, EXI_APPL_DATA) eMAIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != eMAIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((eMAIDPtr->Length <= sizeof(eMAIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &eMAIDPtr->Buffer[0], eMAIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_eMAID_ElementFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_eMAIDType, AUTOMATIC, EXI_APPL_DATA) eMAIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != eMAIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((eMAIDPtr->Length <= sizeof(eMAIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* CH[untyped value] */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 256, 9);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &eMAIDPtr->Buffer[0], eMAIDPtr->Length);
  /* EE */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_evccID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_evccIDType, AUTOMATIC, EXI_APPL_DATA) evccIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != evccIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((evccIDPtr->Length <= sizeof(evccIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVCC_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &evccIDPtr->Buffer[0], evccIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_evseID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_evseIDType, AUTOMATIC, EXI_APPL_DATA) evseIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != evseIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((evseIDPtr->Length <= sizeof(evseIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_EVSE_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &evseIDPtr->Buffer[0], evseIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_faultCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_faultCodeType, AUTOMATIC, EXI_APPL_DATA) faultCodePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != faultCodePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_CODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_FAULT_CODE_TYPE_PARSING_ERROR == *faultCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_FAULT_CODE_TYPE_NO_TLSROOT_CERTIFICAT_AVAILABLE == *faultCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_faultMsg( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_faultMsgType, AUTOMATIC, EXI_APPL_DATA) faultMsgPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != faultMsgPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((faultMsgPtr->Length <= sizeof(faultMsgPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_FAULT_MSG, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &faultMsgPtr->Buffer[0], faultMsgPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_genChallenge( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_genChallengeType, AUTOMATIC, EXI_APPL_DATA) genChallengePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != genChallengePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((genChallengePtr->Length <= sizeof(genChallengePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_GEN_CHALLENGE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &genChallengePtr->Buffer[0], genChallengePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_isolationLevel( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_isolationLevelType, AUTOMATIC, EXI_APPL_DATA) isolationLevelPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != isolationLevelPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_ISOLATION_LEVEL, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_ISOLATION_LEVEL_TYPE_INVALID == *isolationLevelPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
  }
  else if(EXI_ISO_ISOLATION_LEVEL_TYPE_VALID == *isolationLevelPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
  }
  else if(EXI_ISO_ISOLATION_LEVEL_TYPE_WARNING == *isolationLevelPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
  }
  else if(EXI_ISO_ISOLATION_LEVEL_TYPE_FAULT == *isolationLevelPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_meterID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_meterIDType, AUTOMATIC, EXI_APPL_DATA) meterIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != meterIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((meterIDPtr->Length <= sizeof(meterIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_METER_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &meterIDPtr->Buffer[0], meterIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_paymentOption( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_paymentOptionType, AUTOMATIC, EXI_APPL_DATA) paymentOptionPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != paymentOptionPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_PAYMENT_OPTION, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_PAYMENT_OPTION_TYPE_CONTRACT == *paymentOptionPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 1);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_responseCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_responseCodeType, AUTOMATIC, EXI_APPL_DATA) responseCodePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != responseCodePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_RESPONSE_CODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_RESPONSE_CODE_TYPE_OK == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_OK_NEW_SESSION_ESTABLISHED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_OK_OLD_SESSION_JOINED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_OK_CERTIFICATE_EXPIRES_SOON == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SEQUENCE_ERROR == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SERVICE_IDINVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_UNKNOWN_SESSION == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SERVICE_SELECTION_INVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_PAYMENT_SELECTION_INVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_EXPIRED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_SIGNATURE_ERROR == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_NO_CERTIFICATE_AVAILABLE == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERT_CHAIN_ERROR == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CHALLENGE_INVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CONTRACT_CANCELED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 15, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_WRONG_CHARGE_PARAMETER == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_POWER_DELIVERY_NOT_APPLIED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_TARIFF_SELECTION_INVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CHARGING_PROFILE_INVALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_METERING_SIGNATURE_NOT_VALID == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_NO_CHARGE_SERVICE_SELECTED == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_WRONG_ENERGY_TRANSFER_MODE == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CONTACTOR_ERROR == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 23, 5);
  }
  else if(EXI_ISO_RESPONSE_CODE_TYPE_FAILED_CERTIFICATE_NOT_ALLOWED_AT_THIS_EVSE == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 24, 5);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 25, 5);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceCategory( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceCategoryType, AUTOMATIC, EXI_APPL_DATA) serviceCategoryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != serviceCategoryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_CATEGORY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_SERVICE_CATEGORY_TYPE_EVCHARGING == *serviceCategoryPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_ISO_SERVICE_CATEGORY_TYPE_INTERNET == *serviceCategoryPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else if(EXI_ISO_SERVICE_CATEGORY_TYPE_CONTRACT_CERTIFICATE == *serviceCategoryPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceNameType, AUTOMATIC, EXI_APPL_DATA) serviceNamePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != serviceNamePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((serviceNamePtr->Length <= sizeof(serviceNamePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_NAME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &serviceNamePtr->Buffer[0], serviceNamePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_serviceScope( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_serviceScopeType, AUTOMATIC, EXI_APPL_DATA) serviceScopePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != serviceScopePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((serviceScopePtr->Length <= sizeof(serviceScopePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SERVICE_SCOPE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &serviceScopePtr->Buffer[0], serviceScopePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_sessionID( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_sessionIDType, AUTOMATIC, EXI_APPL_DATA) sessionIDPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != sessionIDPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((sessionIDPtr->Length <= sizeof(sessionIDPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SESSION_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &sessionIDPtr->Buffer[0], sessionIDPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_sigMeterReading( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_sigMeterReadingType, AUTOMATIC, EXI_APPL_DATA) sigMeterReadingPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != sigMeterReadingPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((sigMeterReadingPtr->Length <= sizeof(sigMeterReadingPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SIG_METER_READING, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &sigMeterReadingPtr->Buffer[0], sigMeterReadingPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_tariffDescription( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_tariffDescriptionType, AUTOMATIC, EXI_APPL_DATA) tariffDescriptionPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != tariffDescriptionPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((tariffDescriptionPtr->Length <= sizeof(tariffDescriptionPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_TARIFF_DESCRIPTION, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &tariffDescriptionPtr->Buffer[0], tariffDescriptionPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
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
FUNC(void, EXI_CODE) Exi_Encode_ISO_unitSymbol( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_ISO_unitSymbolType, AUTOMATIC, EXI_APPL_DATA) unitSymbolPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != unitSymbolPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_UNIT_SYMBOL, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_ISO_UNIT_SYMBOL_TYPE_H == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
  }
  else if(EXI_ISO_UNIT_SYMBOL_TYPE_M == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
  }
  else if(EXI_ISO_UNIT_SYMBOL_TYPE_S == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
  }
  else if(EXI_ISO_UNIT_SYMBOL_TYPE_A == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
  }
  else if(EXI_ISO_UNIT_SYMBOL_TYPE_V == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
  }
  else if(EXI_ISO_UNIT_SYMBOL_TYPE_W == *unitSymbolPtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 3);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 3);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_ISO_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Fragment AC_EVChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  if(EXI_ISO_AC_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_AC_EVChargeParameterType* AC_EVChargeParameterPtr = (Exi_ISO_AC_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 8);
    Exi_Encode_ISO_AC_EVChargeParameter(EncWsPtr, AC_EVChargeParameterPtr);
    /* EE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON)) */
  }
  /* Fragment AC_EVSEChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_AC_EVSECHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON))
    Exi_ISO_AC_EVSEChargeParameterType* AC_EVSEChargeParameterPtr = (Exi_ISO_AC_EVSEChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 8);
    Exi_Encode_ISO_AC_EVSEChargeParameter(EncWsPtr, AC_EVSEChargeParameterPtr);
    /* EE(AC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON)) */
  }
  /* Fragment AC_EVSEStatus urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_AC_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
    Exi_ISO_AC_EVSEStatusType* AC_EVSEStatusPtr = (Exi_ISO_AC_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 8);
    Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, AC_EVSEStatusPtr);
    /* EE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON)) */
  }
  /* Fragment AC_EVSEStatus urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment AuthorizationReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_AUTHORIZATION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON))
    Exi_ISO_AuthorizationReqType* AuthorizationReqPtr = (Exi_ISO_AuthorizationReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AuthorizationReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_AuthorizationReq(EncWsPtr, AuthorizationReqPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(AuthorizationReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON)) */
  }
  /* Fragment AuthorizationRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_AUTHORIZATION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON))
    Exi_ISO_AuthorizationResType* AuthorizationResPtr = (Exi_ISO_AuthorizationResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AuthorizationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 8);
    Exi_Encode_ISO_AuthorizationRes(EncWsPtr, AuthorizationResPtr);
    /* EE(AuthorizationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON)) */
  }
  /* Fragment Body urn:iso:15118:2:2013:MsgDef */
  else if(EXI_ISO_BODY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON))
    Exi_ISO_BodyType* BodyPtr = (Exi_ISO_BodyType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Body) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_Body(EncWsPtr, BodyPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Body) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_BODY) && (EXI_ENCODE_ISO_BODY == STD_ON)) */
  }
  /* Fragment BodyElement urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_BODY_ELEMENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  else if(EXI_ISO_BODY_BASE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(BodyElement) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 8);
    /* this type is empty */
    /* EE(BodyElement) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 255, 9);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment BulkChargingComplete urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_BULK_CHARGING_COMPLETE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment BulkChargingComplete urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment BulkSOC urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_BULK_SOC_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment CableCheckReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CABLE_CHECK_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON))
    Exi_ISO_CableCheckReqType* CableCheckReqPtr = (Exi_ISO_CableCheckReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CableCheckReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 8);
    Exi_Encode_ISO_CableCheckReq(EncWsPtr, CableCheckReqPtr);
    /* EE(CableCheckReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON)) */
  }
  /* Fragment CableCheckRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CABLE_CHECK_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON))
    Exi_ISO_CableCheckResType* CableCheckResPtr = (Exi_ISO_CableCheckResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CableCheckRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 8);
    Exi_Encode_ISO_CableCheckRes(EncWsPtr, CableCheckResPtr);
    /* EE(CableCheckRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON)) */
  }
  /* Fragment CanonicalizationMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_CANONICALIZATION_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
    Exi_XMLSIG_CanonicalizationMethodType* CanonicalizationMethodPtr = (Exi_XMLSIG_CanonicalizationMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CanonicalizationMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_CanonicalizationMethod(EncWsPtr, CanonicalizationMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(CanonicalizationMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
  }
  /* Fragment Certificate urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_CERTIFICATE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
    Exi_ISO_certificateType* CertificatePtr = (Exi_ISO_certificateType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 8);
    Exi_Encode_ISO_certificate(EncWsPtr, CertificatePtr);
    /* EE(Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */
  }
  /* Fragment CertificateInstallationReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CERTIFICATE_INSTALLATION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON))
    Exi_ISO_CertificateInstallationReqType* CertificateInstallationReqPtr = (Exi_ISO_CertificateInstallationReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateInstallationReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 15, 8);
    Exi_Encode_ISO_CertificateInstallationReq(EncWsPtr, CertificateInstallationReqPtr);
    /* EE(CertificateInstallationReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON)) */
  }
  /* Fragment CertificateInstallationRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CERTIFICATE_INSTALLATION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON))
    Exi_ISO_CertificateInstallationResType* CertificateInstallationResPtr = (Exi_ISO_CertificateInstallationResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateInstallationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 8);
    Exi_Encode_ISO_CertificateInstallationRes(EncWsPtr, CertificateInstallationResPtr);
    /* EE(CertificateInstallationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON)) */
  }
  /* Fragment CertificateUpdateReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CERTIFICATE_UPDATE_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON))
    Exi_ISO_CertificateUpdateReqType* CertificateUpdateReqPtr = (Exi_ISO_CertificateUpdateReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateUpdateReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 8);
    Exi_Encode_ISO_CertificateUpdateReq(EncWsPtr, CertificateUpdateReqPtr);
    /* EE(CertificateUpdateReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON)) */
  }
  /* Fragment CertificateUpdateRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CERTIFICATE_UPDATE_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON))
    Exi_ISO_CertificateUpdateResType* CertificateUpdateResPtr = (Exi_ISO_CertificateUpdateResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateUpdateRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateUpdateRes(EncWsPtr, CertificateUpdateResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(CertificateUpdateRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON)) */
  }
  /* Fragment ChargeParameterDiscoveryReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
    Exi_ISO_ChargeParameterDiscoveryReqType* ChargeParameterDiscoveryReqPtr = (Exi_ISO_ChargeParameterDiscoveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeParameterDiscoveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 8);
    Exi_Encode_ISO_ChargeParameterDiscoveryReq(EncWsPtr, ChargeParameterDiscoveryReqPtr);
    /* EE(ChargeParameterDiscoveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON)) */
  }
  /* Fragment ChargeParameterDiscoveryRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
    Exi_ISO_ChargeParameterDiscoveryResType* ChargeParameterDiscoveryResPtr = (Exi_ISO_ChargeParameterDiscoveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeParameterDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 8);
    Exi_Encode_ISO_ChargeParameterDiscoveryRes(EncWsPtr, ChargeParameterDiscoveryResPtr);
    /* EE(ChargeParameterDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON)) */
  }
  /* Fragment ChargeProgress urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGE_PROGRESS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON))
    Exi_ISO_chargeProgressType* ChargeProgressPtr = (Exi_ISO_chargeProgressType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeProgress) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_chargeProgress(EncWsPtr, ChargeProgressPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ChargeProgress) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_PROGRESS) && (EXI_ENCODE_ISO_CHARGE_PROGRESS == STD_ON)) */
  }
  /* Fragment ChargeService urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGE_SERVICE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON))
    Exi_ISO_ChargeServiceType* ChargeServicePtr = (Exi_ISO_ChargeServiceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeService) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 8);
    Exi_Encode_ISO_ChargeService(EncWsPtr, ChargeServicePtr);
    /* EE(ChargeService) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGE_SERVICE) && (EXI_ENCODE_ISO_CHARGE_SERVICE == STD_ON)) */
  }
  /* Fragment ChargingComplete urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGING_COMPLETE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment ChargingComplete urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment ChargingProfile urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGING_PROFILE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON))
    Exi_ISO_ChargingProfileType* ChargingProfilePtr = (Exi_ISO_ChargingProfileType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargingProfile) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 25, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ChargingProfile(EncWsPtr, ChargingProfilePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ChargingProfile) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGING_PROFILE) && (EXI_ENCODE_ISO_CHARGING_PROFILE == STD_ON)) */
  }
  /* Fragment ChargingProfileEntryMaxNumberOfPhasesInUse urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_CHARGING_PROFILE_ENTRY_MAX_NUMBER_OF_PHASES_IN_USE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment ChargingProfileEntryMaxPower urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_CHARGING_PROFILE_ENTRY_MAX_POWER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* ChargingProfileEntryMaxPowerPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargingProfileEntryMaxPower) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 27, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, ChargingProfileEntryMaxPowerPtr);
    /* EE(ChargingProfileEntryMaxPower) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment ChargingProfileEntryStart urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_CHARGING_PROFILE_ENTRY_START_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment ChargingSession urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGING_SESSION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON))
    Exi_ISO_chargingSessionType* ChargingSessionPtr = (Exi_ISO_chargingSessionType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargingSession) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 29, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_chargingSession(EncWsPtr, ChargingSessionPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ChargingSession) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGING_SESSION) && (EXI_ENCODE_ISO_CHARGING_SESSION == STD_ON)) */
  }
  /* Fragment ChargingStatusReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGING_STATUS_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(ChargingStatusReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 30, 8);
    /* this type is empty */
    /* EE(ChargingStatusReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment ChargingStatusRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CHARGING_STATUS_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON))
    Exi_ISO_ChargingStatusResType* ChargingStatusResPtr = (Exi_ISO_ChargingStatusResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargingStatusRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 31, 8);
    Exi_Encode_ISO_ChargingStatusRes(EncWsPtr, ChargingStatusResPtr);
    /* EE(ChargingStatusRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON)) */
  }
  /* Fragment ConsumptionCost urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_CONSUMPTION_COST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON))
    Exi_ISO_ConsumptionCostType* ConsumptionCostPtr = (Exi_ISO_ConsumptionCostType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ConsumptionCost) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 32, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ConsumptionCost(EncWsPtr, ConsumptionCostPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ConsumptionCost) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CONSUMPTION_COST) && (EXI_ENCODE_ISO_CONSUMPTION_COST == STD_ON)) */
  }
  /* Fragment ContractSignatureCertChain urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CONTRACT_SIGNATURE_CERT_CHAIN_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
    Exi_ISO_CertificateChainType* ContractSignatureCertChainPtr = (Exi_ISO_CertificateChainType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ContractSignatureCertChain) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 33, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateChain(EncWsPtr, ContractSignatureCertChainPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ContractSignatureCertChain) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  }
  /* Fragment ContractSignatureEncryptedPrivateKey urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON))
    Exi_ISO_ContractSignatureEncryptedPrivateKeyType* ContractSignatureEncryptedPrivateKeyPtr = (Exi_ISO_ContractSignatureEncryptedPrivateKeyType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ContractSignatureEncryptedPrivateKey) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 34, 8);
    Exi_Encode_ISO_ContractSignatureEncryptedPrivateKey(EncWsPtr, ContractSignatureEncryptedPrivateKeyPtr);
    /* EE(ContractSignatureEncryptedPrivateKey) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY) && (EXI_ENCODE_ISO_CONTRACT_SIGNATURE_ENCRYPTED_PRIVATE_KEY == STD_ON)) */
  }
  /* Fragment Cost urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_COST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON))
    Exi_ISO_CostType* CostPtr = (Exi_ISO_CostType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Cost) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_Cost(EncWsPtr, CostPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Cost) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_COST) && (EXI_ENCODE_ISO_COST == STD_ON)) */
  }
  /* Fragment CurrentDemandReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CURRENT_DEMAND_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON))
    Exi_ISO_CurrentDemandReqType* CurrentDemandReqPtr = (Exi_ISO_CurrentDemandReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CurrentDemandReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 36, 8);
    Exi_Encode_ISO_CurrentDemandReq(EncWsPtr, CurrentDemandReqPtr);
    /* EE(CurrentDemandReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON)) */
  }
  /* Fragment CurrentDemandRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_CURRENT_DEMAND_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON))
    Exi_ISO_CurrentDemandResType* CurrentDemandResPtr = (Exi_ISO_CurrentDemandResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CurrentDemandRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 37, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CurrentDemandRes(EncWsPtr, CurrentDemandResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(CurrentDemandRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON)) */
  }
  /* Fragment DC_EVChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_DC_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_DC_EVChargeParameterType* DC_EVChargeParameterPtr = (Exi_ISO_DC_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 38, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVChargeParameter(EncWsPtr, DC_EVChargeParameterPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(DC_EVChargeParameter) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON)) */
  }
  /* Fragment DC_EVPowerDeliveryParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_DC_EVPOWER_DELIVERY_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
    Exi_ISO_DC_EVPowerDeliveryParameterType* DC_EVPowerDeliveryParameterPtr = (Exi_ISO_DC_EVPowerDeliveryParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 39, 8);
    Exi_Encode_ISO_DC_EVPowerDeliveryParameter(EncWsPtr, DC_EVPowerDeliveryParameterPtr);
    /* EE(DC_EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON)) */
  }
  /* Fragment DC_EVSEChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_DC_EVSECHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON))
    Exi_ISO_DC_EVSEChargeParameterType* DC_EVSEChargeParameterPtr = (Exi_ISO_DC_EVSEChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 40, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEChargeParameter(EncWsPtr, DC_EVSEChargeParameterPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(DC_EVSEChargeParameter) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON)) */
  }
  /* Fragment DC_EVSEStatus urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_DC_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
    Exi_ISO_DC_EVSEStatusType* DC_EVSEStatusPtr = (Exi_ISO_DC_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 41, 8);
    Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, DC_EVSEStatusPtr);
    /* EE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON)) */
  }
  /* Fragment DC_EVSEStatus urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment DC_EVStatus urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_DC_EVSTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
    Exi_ISO_DC_EVStatusType* DC_EVStatusPtr = (Exi_ISO_DC_EVStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 43, 8);
    Exi_Encode_ISO_DC_EVStatus(EncWsPtr, DC_EVStatusPtr);
    /* EE(DC_EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON)) */
  }
  /* Fragment DC_EVStatus urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment DHpublickey urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_DHPUBLICKEY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON))
    Exi_ISO_DiffieHellmanPublickeyType* DHpublickeyPtr = (Exi_ISO_DiffieHellmanPublickeyType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DHpublickey) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 45, 8);
    Exi_Encode_ISO_DiffieHellmanPublickey(EncWsPtr, DHpublickeyPtr);
    /* EE(DHpublickey) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY) && (EXI_ENCODE_ISO_DIFFIE_HELLMAN_PUBLICKEY == STD_ON)) */
  }
  /* Fragment DSAKeyValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_DSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_DSAKeyValueType* DSAKeyValuePtr = (Exi_XMLSIG_DSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 8);
    Exi_Encode_XMLSIG_DSAKeyValue(EncWsPtr, DSAKeyValuePtr);
    /* EE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */
  }
  /* Fragment DepartureTime urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_DEPARTURE_TIME_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment DigestMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_DIGEST_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON))
    Exi_XMLSIG_DigestMethodType* DigestMethodPtr = (Exi_XMLSIG_DigestMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DigestMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 48, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_DigestMethod(EncWsPtr, DigestMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(DigestMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON)) */
  }
  /* Fragment DigestValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_DIGEST_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON))
    Exi_XMLSIG_DigestValueType* DigestValuePtr = (Exi_XMLSIG_DigestValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DigestValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 49, 8);
    Exi_Encode_XMLSIG_DigestValue(EncWsPtr, DigestValuePtr);
    /* EE(DigestValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON)) */
  }
  /* Fragment EAmount urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EAMOUNT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EAmountPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EAmount) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 50, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EAmountPtr);
    /* EE(EAmount) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EPriceLevel urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EPRICE_LEVEL_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVCCID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVCCID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON))
    Exi_ISO_evccIDType* EVCCIDPtr = (Exi_ISO_evccIDType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVCCID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 52, 8);
    Exi_Encode_ISO_evccID(EncWsPtr, EVCCIDPtr);
    /* EE(EVCCID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVCC_ID) && (EXI_ENCODE_ISO_EVCC_ID == STD_ON)) */
  }
  /* Fragment EVChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_EVChargeParameterType* EVChargeParameterPtr = (Exi_ISO_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 53, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EVChargeParameter(EncWsPtr, EVChargeParameterPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVChargeParameter) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON)) */
  }
  /* Fragment EVEnergyCapacity urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVENERGY_CAPACITY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVEnergyCapacityPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVEnergyCapacity) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 54, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVEnergyCapacityPtr);
    /* EE(EVEnergyCapacity) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVEnergyRequest urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVENERGY_REQUEST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVEnergyRequestPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVEnergyRequest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 55, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVEnergyRequestPtr);
    /* EE(EVEnergyRequest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVErrorCode urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVERROR_CODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON))
    Exi_ISO_DC_EVErrorCodeType* EVErrorCodePtr = (Exi_ISO_DC_EVErrorCodeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVErrorCode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 56, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVErrorCode(EncWsPtr, EVErrorCodePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVErrorCode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVERROR_CODE) && (EXI_ENCODE_ISO_DC_EVERROR_CODE == STD_ON)) */
  }
  /* Fragment EVMaxCurrent urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVMAX_CURRENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMaxCurrentPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 57, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMaxCurrentPtr);
    /* EE(EVMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVMaxVoltage urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVMAX_VOLTAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMaxVoltagePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMaxVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 58, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMaxVoltagePtr);
    /* EE(EVMaxVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVMaximumCurrentLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVMAXIMUM_CURRENT_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMaximumCurrentLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 59, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMaximumCurrentLimitPtr);
    /* EE(EVMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVMaximumCurrentLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVMaximumPowerLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVMAXIMUM_POWER_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMaximumPowerLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 61, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMaximumPowerLimitPtr);
    /* EE(EVMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVMaximumPowerLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVMaximumVoltageLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVMAXIMUM_VOLTAGE_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMaximumVoltageLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 63, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMaximumVoltageLimitPtr);
    /* EE(EVMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVMaximumVoltageLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVMinCurrent urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVMIN_CURRENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVMinCurrentPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVMinCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 65, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVMinCurrentPtr);
    /* EE(EVMinCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVPowerDeliveryParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVPOWER_DELIVERY_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 66, 8);
    /* this type is empty */
    /* EE(EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment EVRESSSOC urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVRESSSOC_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVReady urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVREADY_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVSEChargeParameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSECHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 69, 8);
    /* this type is empty */
    /* EE(EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment EVSECurrentLimitAchieved urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSECURRENT_LIMIT_ACHIEVED_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVSECurrentRegulationTolerance urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSECURRENT_REGULATION_TOLERANCE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSECurrentRegulationTolerancePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSECurrentRegulationTolerance) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 71, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSECurrentRegulationTolerancePtr);
    /* EE(EVSECurrentRegulationTolerance) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEEnergyToBeDelivered urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSEENERGY_TO_BE_DELIVERED_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEEnergyToBeDeliveredPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEEnergyToBeDelivered) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 72, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEEnergyToBeDeliveredPtr);
    /* EE(EVSEEnergyToBeDelivered) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON))
    Exi_ISO_evseIDType* EVSEIDPtr = (Exi_ISO_evseIDType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 73, 8);
    Exi_Encode_ISO_evseID(EncWsPtr, EVSEIDPtr);
    /* EE(EVSEID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSE_ID) && (EXI_ENCODE_ISO_EVSE_ID == STD_ON)) */
  }
  /* Fragment EVSEIsolationStatus urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSEISOLATION_STATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON))
    Exi_ISO_isolationLevelType* EVSEIsolationStatusPtr = (Exi_ISO_isolationLevelType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEIsolationStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 74, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_isolationLevel(EncWsPtr, EVSEIsolationStatusPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVSEIsolationStatus) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_ISOLATION_LEVEL) && (EXI_ENCODE_ISO_ISOLATION_LEVEL == STD_ON)) */
  }
  /* Fragment EVSEMaxCurrent urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEMAX_CURRENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMaxCurrentPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 75, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMaxCurrentPtr);
    /* EE(EVSEMaxCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEMaxCurrent urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVSEMaximumCurrentLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEMAXIMUM_CURRENT_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMaximumCurrentLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 77, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMaximumCurrentLimitPtr);
    /* EE(EVSEMaximumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEMaximumCurrentLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVSEMaximumPowerLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEMAXIMUM_POWER_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMaximumPowerLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 79, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMaximumPowerLimitPtr);
    /* EE(EVSEMaximumPowerLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEMaximumPowerLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVSEMaximumVoltageLimit urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEMAXIMUM_VOLTAGE_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMaximumVoltageLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 81, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMaximumVoltageLimitPtr);
    /* EE(EVSEMaximumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEMaximumVoltageLimit urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment EVSEMinimumCurrentLimit urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSEMINIMUM_CURRENT_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMinimumCurrentLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMinimumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 83, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMinimumCurrentLimitPtr);
    /* EE(EVSEMinimumCurrentLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEMinimumVoltageLimit urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSEMINIMUM_VOLTAGE_LIMIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEMinimumVoltageLimitPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEMinimumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 84, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEMinimumVoltageLimitPtr);
    /* EE(EVSEMinimumVoltageLimit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSENominalVoltage urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSENOMINAL_VOLTAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSENominalVoltagePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSENominalVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 85, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSENominalVoltagePtr);
    /* EE(EVSENominalVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSENotification urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSENOTIFICATION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON))
    Exi_ISO_EVSENotificationType* EVSENotificationPtr = (Exi_ISO_EVSENotificationType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSENotification) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 86, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EVSENotification(EncWsPtr, EVSENotificationPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVSENotification) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSENOTIFICATION) && (EXI_ENCODE_ISO_EVSENOTIFICATION == STD_ON)) */
  }
  /* Fragment EVSEPeakCurrentRipple urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSEPEAK_CURRENT_RIPPLE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEPeakCurrentRipplePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEPeakCurrentRipple) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 87, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEPeakCurrentRipplePtr);
    /* EE(EVSEPeakCurrentRipple) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEPowerLimitAchieved urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEPOWER_LIMIT_ACHIEVED_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVSEPresentCurrent urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEPRESENT_CURRENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEPresentCurrentPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEPresentCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 89, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEPresentCurrentPtr);
    /* EE(EVSEPresentCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEPresentVoltage urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEPRESENT_VOLTAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVSEPresentVoltagePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEPresentVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 90, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVSEPresentVoltagePtr);
    /* EE(EVSEPresentVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVSEProcessing urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEPROCESSING_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON))
    Exi_ISO_EVSEProcessingType* EVSEProcessingPtr = (Exi_ISO_EVSEProcessingType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEProcessing) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 91, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EVSEProcessing(EncWsPtr, EVSEProcessingPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVSEProcessing) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSEPROCESSING) && (EXI_ENCODE_ISO_EVSEPROCESSING == STD_ON)) */
  }
  /* Fragment EVSEStatus urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON))
    Exi_ISO_EVSEStatusType* EVSEStatusPtr = (Exi_ISO_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 92, 8);
    Exi_Encode_ISO_EVSEStatus(EncWsPtr, EVSEStatusPtr);
    /* EE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON)) */
  }
  /* Fragment EVSEStatusCode urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSESTATUS_CODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON))
    Exi_ISO_DC_EVSEStatusCodeType* EVSEStatusCodePtr = (Exi_ISO_DC_EVSEStatusCodeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEStatusCode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 93, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEStatusCode(EncWsPtr, EVSEStatusCodePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EVSEStatusCode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_DC_EVSESTATUS_CODE) && (EXI_ENCODE_ISO_DC_EVSESTATUS_CODE == STD_ON)) */
  }
  /* Fragment EVSETimeStamp urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSETIME_STAMP_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVSEVoltageLimitAchieved urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVSEVOLTAGE_LIMIT_ACHIEVED_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment EVStatus urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_EVSTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 96, 8);
    /* this type is empty */
    /* EE(EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment EVTargetCurrent urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVTARGET_CURRENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVTargetCurrentPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVTargetCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 97, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVTargetCurrentPtr);
    /* EE(EVTargetCurrent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EVTargetVoltage urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_EVTARGET_VOLTAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* EVTargetVoltagePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVTargetVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 98, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, EVTargetVoltagePtr);
    /* EE(EVTargetVoltage) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment EnergyTransferMode urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_ENERGY_TRANSFER_MODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
    Exi_ISO_EnergyTransferModeType* EnergyTransferModePtr = (Exi_ISO_EnergyTransferModeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EnergyTransferMode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 99, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EnergyTransferMode(EncWsPtr, EnergyTransferModePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(EnergyTransferMode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */
  }
  /* Fragment Entry urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_ENTRY) && (EXI_ENCODE_ISO_ENTRY == STD_ON))
    Exi_ISO_EntryType* EntryPtr = (Exi_ISO_EntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Entry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 100, 8);
    Exi_Encode_ISO_Entry(EncWsPtr, EntryPtr);
    /* EE(Entry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_ENTRY) && (EXI_ENCODE_ISO_ENTRY == STD_ON)) */
  }
  /* Fragment Exponent http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_EXPONENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* ExponentPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Exponent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 101, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, ExponentPtr);
    /* EE(Exponent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment FaultCode urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_FAULT_CODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON))
    Exi_ISO_faultCodeType* FaultCodePtr = (Exi_ISO_faultCodeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(FaultCode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 102, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_faultCode(EncWsPtr, FaultCodePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(FaultCode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_FAULT_CODE) && (EXI_ENCODE_ISO_FAULT_CODE == STD_ON)) */
  }
  /* Fragment FaultMsg urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_FAULT_MSG_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON))
    Exi_ISO_faultMsgType* FaultMsgPtr = (Exi_ISO_faultMsgType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(FaultMsg) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 103, 8);
    Exi_Encode_ISO_faultMsg(EncWsPtr, FaultMsgPtr);
    /* EE(FaultMsg) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_FAULT_MSG) && (EXI_ENCODE_ISO_FAULT_MSG == STD_ON)) */
  }
  /* Fragment FreeService urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_FREE_SERVICE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment FullSOC urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_FULL_SOC_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment G http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_G_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* GPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 106, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, GPtr);
    /* EE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment GenChallenge urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_GEN_CHALLENGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON))
    Exi_ISO_genChallengeType* GenChallengePtr = (Exi_ISO_genChallengeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(GenChallenge) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 107, 8);
    Exi_Encode_ISO_genChallenge(EncWsPtr, GenChallengePtr);
    /* EE(GenChallenge) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_GEN_CHALLENGE) && (EXI_ENCODE_ISO_GEN_CHALLENGE == STD_ON)) */
  }
  /* Fragment HMACOutputLength http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_HMACOUTPUT_LENGTH_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment Header urn:iso:15118:2:2013:MsgDef */
  else if(EXI_ISO_HEADER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON))
    Exi_ISO_MessageHeaderType* HeaderPtr = (Exi_ISO_MessageHeaderType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Header) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 109, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_MessageHeader(EncWsPtr, HeaderPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Header) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_MESSAGE_HEADER) && (EXI_ENCODE_ISO_MESSAGE_HEADER == STD_ON)) */
  }
  /* Fragment J http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_J_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* JPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 110, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, JPtr);
    /* EE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment KeyInfo http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_KEY_INFO_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON))
    Exi_XMLSIG_KeyInfoType* KeyInfoPtr = (Exi_XMLSIG_KeyInfoType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 111, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_KeyInfo(EncWsPtr, KeyInfoPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(KeyInfo) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON)) */
  }
  /* Fragment KeyName http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_KEY_NAME_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON))
    Exi_XMLSIG_KeyNameType* KeyNamePtr = (Exi_XMLSIG_KeyNameType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 112, 8);
    Exi_Encode_XMLSIG_KeyName(EncWsPtr, KeyNamePtr);
    /* EE(KeyName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON)) */
  }
  /* Fragment KeyValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_KEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON))
    Exi_XMLSIG_KeyValueType* KeyValuePtr = (Exi_XMLSIG_KeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 113, 8);
    Exi_Encode_XMLSIG_KeyValue(EncWsPtr, KeyValuePtr);
    /* EE(KeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON)) */
  }
  /* Fragment ListOfRootCertificateIDs urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_LIST_OF_ROOT_CERTIFICATE_IDS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON))
    Exi_ISO_ListOfRootCertificateIDsType* ListOfRootCertificateIDsPtr = (Exi_ISO_ListOfRootCertificateIDsType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ListOfRootCertificateIDs) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 114, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ListOfRootCertificateIDs(EncWsPtr, ListOfRootCertificateIDsPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ListOfRootCertificateIDs) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS) && (EXI_ENCODE_ISO_LIST_OF_ROOT_CERTIFICATE_IDS == STD_ON)) */
  }
  /* Fragment Manifest http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_MANIFEST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON))
    Exi_XMLSIG_ManifestType* ManifestPtr = (Exi_XMLSIG_ManifestType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Manifest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 115, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Manifest(EncWsPtr, ManifestPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Manifest) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON)) */
  }
  /* Fragment MaxEntriesSAScheduleTuple urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_MAX_ENTRIES_SASCHEDULE_TUPLE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment MeterID urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_METER_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON))
    Exi_ISO_meterIDType* MeterIDPtr = (Exi_ISO_meterIDType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeterID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 117, 8);
    Exi_Encode_ISO_meterID(EncWsPtr, MeterIDPtr);
    /* EE(MeterID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_METER_ID) && (EXI_ENCODE_ISO_METER_ID == STD_ON)) */
  }
  /* Fragment MeterInfo urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_METER_INFO_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON))
    Exi_ISO_MeterInfoType* MeterInfoPtr = (Exi_ISO_MeterInfoType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeterInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 118, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_MeterInfo(EncWsPtr, MeterInfoPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(MeterInfo) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_METER_INFO) && (EXI_ENCODE_ISO_METER_INFO == STD_ON)) */
  }
  /* Fragment MeterReading urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_METER_READING_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment MeterStatus urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_METER_STATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment MeteringReceiptReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_METERING_RECEIPT_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON))
    Exi_ISO_MeteringReceiptReqType* MeteringReceiptReqPtr = (Exi_ISO_MeteringReceiptReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeteringReceiptReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 121, 8);
    Exi_Encode_ISO_MeteringReceiptReq(EncWsPtr, MeteringReceiptReqPtr);
    /* EE(MeteringReceiptReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON)) */
  }
  /* Fragment MeteringReceiptRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_METERING_RECEIPT_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON))
    Exi_ISO_MeteringReceiptResType* MeteringReceiptResPtr = (Exi_ISO_MeteringReceiptResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeteringReceiptRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 122, 8);
    Exi_Encode_ISO_MeteringReceiptRes(EncWsPtr, MeteringReceiptResPtr);
    /* EE(MeteringReceiptRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON)) */
  }
  /* Fragment MgmtData http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_MGMT_DATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON))
    Exi_XMLSIG_MgmtDataType* MgmtDataPtr = (Exi_XMLSIG_MgmtDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MgmtData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 123, 8);
    Exi_Encode_XMLSIG_MgmtData(EncWsPtr, MgmtDataPtr);
    /* EE(MgmtData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON)) */
  }
  /* Fragment Modulus http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_MODULUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* ModulusPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Modulus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 124, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, ModulusPtr);
    /* EE(Modulus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment Multiplier urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_MULTIPLIER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment Notification urn:iso:15118:2:2013:MsgHeader */
  else if(EXI_ISO_NOTIFICATION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON))
    Exi_ISO_NotificationType* NotificationPtr = (Exi_ISO_NotificationType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Notification) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 126, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_Notification(EncWsPtr, NotificationPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Notification) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_NOTIFICATION) && (EXI_ENCODE_ISO_NOTIFICATION == STD_ON)) */
  }
  /* Fragment NotificationMaxDelay urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_NOTIFICATION_MAX_DELAY_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment NumEPriceLevels urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_NUM_EPRICE_LEVELS_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment OEMProvisioningCert urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_OEMPROVISIONING_CERT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON))
    Exi_ISO_certificateType* OEMProvisioningCertPtr = (Exi_ISO_certificateType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(OEMProvisioningCert) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 129, 8);
    Exi_Encode_ISO_certificate(EncWsPtr, OEMProvisioningCertPtr);
    /* EE(OEMProvisioningCert) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE) && (EXI_ENCODE_ISO_CERTIFICATE == STD_ON)) */
  }
  /* Fragment Object http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_OBJECT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON))
    Exi_XMLSIG_ObjectType* ObjectPtr = (Exi_XMLSIG_ObjectType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Object) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 130, 8);
    Exi_Encode_XMLSIG_Object(EncWsPtr, ObjectPtr);
    /* EE(Object) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON)) */
  }
  /* Fragment P http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_P_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* PPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(P) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 131, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, PPtr);
    /* EE(P) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment PGPData http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_PGPDATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON))
    Exi_XMLSIG_PGPDataType* PGPDataPtr = (Exi_XMLSIG_PGPDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PGPData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 132, 8);
    Exi_Encode_XMLSIG_PGPData(EncWsPtr, PGPDataPtr);
    /* EE(PGPData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON)) */
  }
  /* Fragment PGPKeyID http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_PGPKEY_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* PGPKeyIDPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PGPKeyID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 133, 8);
    Exi_Encode_base64Binary(EncWsPtr, PGPKeyIDPtr);
    /* EE(PGPKeyID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment PGPKeyPacket http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_PGPKEY_PACKET_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* PGPKeyPacketPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PGPKeyPacket) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 134, 8);
    Exi_Encode_base64Binary(EncWsPtr, PGPKeyPacketPtr);
    /* EE(PGPKeyPacket) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment PMax urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PMAX_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* PMaxPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PMax) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 135, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, PMaxPtr);
    /* EE(PMax) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment PMaxSchedule urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PMAX_SCHEDULE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON))
    Exi_ISO_PMaxScheduleType* PMaxSchedulePtr = (Exi_ISO_PMaxScheduleType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PMaxSchedule) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 136, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PMaxSchedule(EncWsPtr, PMaxSchedulePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(PMaxSchedule) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE) && (EXI_ENCODE_ISO_PMAX_SCHEDULE == STD_ON)) */
  }
  /* Fragment PMaxScheduleEntry urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PMAX_SCHEDULE_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON))
    Exi_ISO_PMaxScheduleEntryType* PMaxScheduleEntryPtr = (Exi_ISO_PMaxScheduleEntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PMaxScheduleEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 137, 8);
    Exi_Encode_ISO_PMaxScheduleEntry(EncWsPtr, PMaxScheduleEntryPtr);
    /* EE(PMaxScheduleEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON)) */
  }
  /* Fragment Parameter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON))
    Exi_ISO_ParameterType* ParameterPtr = (Exi_ISO_ParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Parameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 138, 8);
    Exi_Encode_ISO_Parameter(EncWsPtr, ParameterPtr);
    /* EE(Parameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PARAMETER) && (EXI_ENCODE_ISO_PARAMETER == STD_ON)) */
  }
  /* Fragment ParameterSet urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PARAMETER_SET_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON))
    Exi_ISO_ParameterSetType* ParameterSetPtr = (Exi_ISO_ParameterSetType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ParameterSet) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 139, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ParameterSet(EncWsPtr, ParameterSetPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ParameterSet) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PARAMETER_SET) && (EXI_ENCODE_ISO_PARAMETER_SET == STD_ON)) */
  }
  /* Fragment ParameterSetID urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PARAMETER_SET_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment PaymentDetailsReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PAYMENT_DETAILS_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON))
    Exi_ISO_PaymentDetailsReqType* PaymentDetailsReqPtr = (Exi_ISO_PaymentDetailsReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentDetailsReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 141, 8);
    Exi_Encode_ISO_PaymentDetailsReq(EncWsPtr, PaymentDetailsReqPtr);
    /* EE(PaymentDetailsReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON)) */
  }
  /* Fragment PaymentDetailsRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PAYMENT_DETAILS_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON))
    Exi_ISO_PaymentDetailsResType* PaymentDetailsResPtr = (Exi_ISO_PaymentDetailsResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentDetailsRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 142, 8);
    Exi_Encode_ISO_PaymentDetailsRes(EncWsPtr, PaymentDetailsResPtr);
    /* EE(PaymentDetailsRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON)) */
  }
  /* Fragment PaymentOption urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PAYMENT_OPTION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON))
    Exi_ISO_paymentOptionType* PaymentOptionPtr = (Exi_ISO_paymentOptionType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentOption) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 143, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_paymentOption(EncWsPtr, PaymentOptionPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(PaymentOption) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON)) */
  }
  /* Fragment PaymentOptionList urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PAYMENT_OPTION_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON))
    Exi_ISO_PaymentOptionListType* PaymentOptionListPtr = (Exi_ISO_PaymentOptionListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentOptionList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 144, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PaymentOptionList(EncWsPtr, PaymentOptionListPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(PaymentOptionList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION_LIST) && (EXI_ENCODE_ISO_PAYMENT_OPTION_LIST == STD_ON)) */
  }
  /* Fragment PaymentServiceSelectionReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON))
    Exi_ISO_PaymentServiceSelectionReqType* PaymentServiceSelectionReqPtr = (Exi_ISO_PaymentServiceSelectionReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentServiceSelectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 145, 8);
    Exi_Encode_ISO_PaymentServiceSelectionReq(EncWsPtr, PaymentServiceSelectionReqPtr);
    /* EE(PaymentServiceSelectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON)) */
  }
  /* Fragment PaymentServiceSelectionRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON))
    Exi_ISO_PaymentServiceSelectionResType* PaymentServiceSelectionResPtr = (Exi_ISO_PaymentServiceSelectionResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentServiceSelectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 146, 8);
    Exi_Encode_ISO_PaymentServiceSelectionRes(EncWsPtr, PaymentServiceSelectionResPtr);
    /* EE(PaymentServiceSelectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON)) */
  }
  /* Fragment PgenCounter http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_PGEN_COUNTER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* PgenCounterPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PgenCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 147, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, PgenCounterPtr);
    /* EE(PgenCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment PowerDeliveryReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_POWER_DELIVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON))
    Exi_ISO_PowerDeliveryReqType* PowerDeliveryReqPtr = (Exi_ISO_PowerDeliveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PowerDeliveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 148, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PowerDeliveryReq(EncWsPtr, PowerDeliveryReqPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(PowerDeliveryReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON)) */
  }
  /* Fragment PowerDeliveryRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_POWER_DELIVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON))
    Exi_ISO_PowerDeliveryResType* PowerDeliveryResPtr = (Exi_ISO_PowerDeliveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PowerDeliveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 149, 8);
    Exi_Encode_ISO_PowerDeliveryRes(EncWsPtr, PowerDeliveryResPtr);
    /* EE(PowerDeliveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON)) */
  }
  /* Fragment PreChargeReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PRE_CHARGE_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON))
    Exi_ISO_PreChargeReqType* PreChargeReqPtr = (Exi_ISO_PreChargeReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PreChargeReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 150, 8);
    Exi_Encode_ISO_PreChargeReq(EncWsPtr, PreChargeReqPtr);
    /* EE(PreChargeReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON)) */
  }
  /* Fragment PreChargeRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_PRE_CHARGE_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON))
    Exi_ISO_PreChargeResType* PreChargeResPtr = (Exi_ISO_PreChargeResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PreChargeRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 151, 8);
    Exi_Encode_ISO_PreChargeRes(EncWsPtr, PreChargeResPtr);
    /* EE(PreChargeRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON)) */
  }
  /* Fragment ProfileEntry urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PROFILE_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON))
    Exi_ISO_ProfileEntryType* ProfileEntryPtr = (Exi_ISO_ProfileEntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ProfileEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 152, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ProfileEntry(EncWsPtr, ProfileEntryPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ProfileEntry) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PROFILE_ENTRY) && (EXI_ENCODE_ISO_PROFILE_ENTRY == STD_ON)) */
  }
  /* Fragment Q http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_Q_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* QPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Q) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 153, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, QPtr);
    /* EE(Q) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment RCD urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_RCD_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment RSAKeyValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_RSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_RSAKeyValueType* RSAKeyValuePtr = (Exi_XMLSIG_RSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 155, 8);
    Exi_Encode_XMLSIG_RSAKeyValue(EncWsPtr, RSAKeyValuePtr);
    /* EE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */
  }
  /* Fragment ReceiptRequired urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_RECEIPT_REQUIRED_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment Reference http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_REFERENCE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
    Exi_XMLSIG_ReferenceType* ReferencePtr = (Exi_XMLSIG_ReferenceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 157, 8);
    Exi_Encode_XMLSIG_Reference(EncWsPtr, ReferencePtr);
    /* EE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON)) */
  }
  /* Fragment RelativeTimeInterval urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
    Exi_ISO_RelativeTimeIntervalType* RelativeTimeIntervalPtr = (Exi_ISO_RelativeTimeIntervalType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RelativeTimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 158, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_RelativeTimeInterval(EncWsPtr, RelativeTimeIntervalPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(RelativeTimeInterval) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON)) */
  }
  /* Fragment RemainingTimeToBulkSoC urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_REMAINING_TIME_TO_BULK_SOC_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* RemainingTimeToBulkSoCPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RemainingTimeToBulkSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 159, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, RemainingTimeToBulkSoCPtr);
    /* EE(RemainingTimeToBulkSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment RemainingTimeToFullSoC urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_REMAINING_TIME_TO_FULL_SOC_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* RemainingTimeToFullSoCPtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RemainingTimeToFullSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 160, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, RemainingTimeToFullSoCPtr);
    /* EE(RemainingTimeToFullSoC) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment RequestedEnergyTransferMode urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_REQUESTED_ENERGY_TRANSFER_MODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON))
    Exi_ISO_EnergyTransferModeType* RequestedEnergyTransferModePtr = (Exi_ISO_EnergyTransferModeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RequestedEnergyTransferMode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 161, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EnergyTransferMode(EncWsPtr, RequestedEnergyTransferModePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(RequestedEnergyTransferMode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_ENERGY_TRANSFER_MODE == STD_ON)) */
  }
  /* Fragment ResponseCode urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_RESPONSE_CODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON))
    Exi_ISO_responseCodeType* ResponseCodePtr = (Exi_ISO_responseCodeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ResponseCode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 162, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_responseCode(EncWsPtr, ResponseCodePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ResponseCode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_RESPONSE_CODE) && (EXI_ENCODE_ISO_RESPONSE_CODE == STD_ON)) */
  }
  /* Fragment RetrievalMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_RETRIEVAL_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
    Exi_XMLSIG_RetrievalMethodType* RetrievalMethodPtr = (Exi_XMLSIG_RetrievalMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RetrievalMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 163, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_RetrievalMethod(EncWsPtr, RetrievalMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(RetrievalMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
  }
  /* Fragment RetryCounter urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_RETRY_COUNTER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment RootCertificateID urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_ROOT_CERTIFICATE_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
    Exi_XMLSIG_X509IssuerSerialType* RootCertificateIDPtr = (Exi_XMLSIG_X509IssuerSerialType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RootCertificateID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 165, 8);
    Exi_Encode_XMLSIG_X509IssuerSerial(EncWsPtr, RootCertificateIDPtr);
    /* EE(RootCertificateID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */
  }
  /* Fragment SAProvisioningCertificateChain urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SAPROVISIONING_CERTIFICATE_CHAIN_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON))
    Exi_ISO_CertificateChainType* SAProvisioningCertificateChainPtr = (Exi_ISO_CertificateChainType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SAProvisioningCertificateChain) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 166, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateChain(EncWsPtr, SAProvisioningCertificateChainPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SAProvisioningCertificateChain) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_CERTIFICATE_CHAIN) && (EXI_ENCODE_ISO_CERTIFICATE_CHAIN == STD_ON)) */
  }
  /* Fragment SAScheduleList urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SASCHEDULE_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON))
    Exi_ISO_SAScheduleListType* SAScheduleListPtr = (Exi_ISO_SAScheduleListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SAScheduleList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 167, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SAScheduleList(EncWsPtr, SAScheduleListPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SAScheduleList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON)) */
  }
  /* Fragment SAScheduleTuple urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SASCHEDULE_TUPLE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON))
    Exi_ISO_SAScheduleTupleType* SAScheduleTuplePtr = (Exi_ISO_SAScheduleTupleType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SAScheduleTuple) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 168, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SAScheduleTuple(EncWsPtr, SAScheduleTuplePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SAScheduleTuple) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SASCHEDULE_TUPLE) && (EXI_ENCODE_ISO_SASCHEDULE_TUPLE == STD_ON)) */
  }
  /* Fragment SAScheduleTupleID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SASCHEDULE_TUPLE_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment SAScheduleTupleID urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment SASchedules urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SASCHEDULES_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(SASchedules) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 171, 8);
    /* this type is empty */
    /* EE(SASchedules) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment SPKIData http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SPKIDATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON))
    Exi_XMLSIG_SPKIDataType* SPKIDataPtr = (Exi_XMLSIG_SPKIDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SPKIData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 172, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SPKIData(EncWsPtr, SPKIDataPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SPKIData) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON)) */
  }
  /* Fragment SPKISexp http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SPKISEXP_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* SPKISexpPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SPKISexp) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 173, 8);
    Exi_Encode_base64Binary(EncWsPtr, SPKISexpPtr);
    /* EE(SPKISexp) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment SalesTariff urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SALES_TARIFF_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON))
    Exi_ISO_SalesTariffType* SalesTariffPtr = (Exi_ISO_SalesTariffType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SalesTariff) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 174, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SalesTariff(EncWsPtr, SalesTariffPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SalesTariff) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF) && (EXI_ENCODE_ISO_SALES_TARIFF == STD_ON)) */
  }
  /* Fragment SalesTariffDescription urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SALES_TARIFF_DESCRIPTION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON))
    Exi_ISO_tariffDescriptionType* SalesTariffDescriptionPtr = (Exi_ISO_tariffDescriptionType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SalesTariffDescription) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 175, 8);
    Exi_Encode_ISO_tariffDescription(EncWsPtr, SalesTariffDescriptionPtr);
    /* EE(SalesTariffDescription) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_TARIFF_DESCRIPTION) && (EXI_ENCODE_ISO_TARIFF_DESCRIPTION == STD_ON)) */
  }
  /* Fragment SalesTariffEntry urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SALES_TARIFF_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON))
    Exi_ISO_SalesTariffEntryType* SalesTariffEntryPtr = (Exi_ISO_SalesTariffEntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SalesTariffEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 176, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SalesTariffEntry(EncWsPtr, SalesTariffEntryPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SalesTariffEntry) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON)) */
  }
  /* Fragment SalesTariffID urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SALES_TARIFF_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment Seed http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SEED_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* SeedPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Seed) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 178, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, SeedPtr);
    /* EE(Seed) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment SelectedPaymentOption urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SELECTED_PAYMENT_OPTION_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON))
    Exi_ISO_paymentOptionType* SelectedPaymentOptionPtr = (Exi_ISO_paymentOptionType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SelectedPaymentOption) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 179, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_paymentOption(EncWsPtr, SelectedPaymentOptionPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SelectedPaymentOption) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PAYMENT_OPTION) && (EXI_ENCODE_ISO_PAYMENT_OPTION == STD_ON)) */
  }
  /* Fragment SelectedService urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SELECTED_SERVICE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON))
    Exi_ISO_SelectedServiceType* SelectedServicePtr = (Exi_ISO_SelectedServiceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SelectedService) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 180, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SelectedService(EncWsPtr, SelectedServicePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SelectedService) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE) && (EXI_ENCODE_ISO_SELECTED_SERVICE == STD_ON)) */
  }
  /* Fragment SelectedServiceList urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SELECTED_SERVICE_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON))
    Exi_ISO_SelectedServiceListType* SelectedServiceListPtr = (Exi_ISO_SelectedServiceListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SelectedServiceList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 181, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SelectedServiceList(EncWsPtr, SelectedServiceListPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SelectedServiceList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SELECTED_SERVICE_LIST) && (EXI_ENCODE_ISO_SELECTED_SERVICE_LIST == STD_ON)) */
  }
  /* Fragment Service urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SERVICE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON))
    Exi_ISO_ServiceType* ServicePtr = (Exi_ISO_ServiceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Service) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 182, 8);
    Exi_Encode_ISO_Service(EncWsPtr, ServicePtr);
    /* EE(Service) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE) && (EXI_ENCODE_ISO_SERVICE == STD_ON)) */
  }
  /* Fragment ServiceCategory urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_CATEGORY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON))
    Exi_ISO_serviceCategoryType* ServiceCategoryPtr = (Exi_ISO_serviceCategoryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceCategory) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 183, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_serviceCategory(EncWsPtr, ServiceCategoryPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceCategory) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_CATEGORY) && (EXI_ENCODE_ISO_SERVICE_CATEGORY == STD_ON)) */
  }
  /* Fragment ServiceCategory urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment ServiceDetailReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_DETAIL_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON))
    Exi_ISO_ServiceDetailReqType* ServiceDetailReqPtr = (Exi_ISO_ServiceDetailReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDetailReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 185, 8);
    Exi_Encode_ISO_ServiceDetailReq(EncWsPtr, ServiceDetailReqPtr);
    /* EE(ServiceDetailReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON)) */
  }
  /* Fragment ServiceDetailRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_DETAIL_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON))
    Exi_ISO_ServiceDetailResType* ServiceDetailResPtr = (Exi_ISO_ServiceDetailResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDetailRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 186, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDetailRes(EncWsPtr, ServiceDetailResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceDetailRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON)) */
  }
  /* Fragment ServiceDiscoveryReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_DISCOVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON))
    Exi_ISO_ServiceDiscoveryReqType* ServiceDiscoveryReqPtr = (Exi_ISO_ServiceDiscoveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDiscoveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 187, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDiscoveryReq(EncWsPtr, ServiceDiscoveryReqPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceDiscoveryReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON)) */
  }
  /* Fragment ServiceDiscoveryRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_DISCOVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON))
    Exi_ISO_ServiceDiscoveryResType* ServiceDiscoveryResPtr = (Exi_ISO_ServiceDiscoveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 188, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDiscoveryRes(EncWsPtr, ServiceDiscoveryResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceDiscoveryRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON)) */
  }
  /* Fragment ServiceID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment ServiceID urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment ServiceList urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON))
    Exi_ISO_ServiceListType* ServiceListPtr = (Exi_ISO_ServiceListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 191, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceList(EncWsPtr, ServiceListPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_LIST) && (EXI_ENCODE_ISO_SERVICE_LIST == STD_ON)) */
  }
  /* Fragment ServiceName urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SERVICE_NAME_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON))
    Exi_ISO_serviceNameType* ServiceNamePtr = (Exi_ISO_serviceNameType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 192, 8);
    Exi_Encode_ISO_serviceName(EncWsPtr, ServiceNamePtr);
    /* EE(ServiceName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_NAME) && (EXI_ENCODE_ISO_SERVICE_NAME == STD_ON)) */
  }
  /* Fragment ServiceParameterList urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_PARAMETER_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON))
    Exi_ISO_ServiceParameterListType* ServiceParameterListPtr = (Exi_ISO_ServiceParameterListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceParameterList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 193, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceParameterList(EncWsPtr, ServiceParameterListPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ServiceParameterList) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST) && (EXI_ENCODE_ISO_SERVICE_PARAMETER_LIST == STD_ON)) */
  }
  /* Fragment ServiceScope urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SERVICE_SCOPE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON))
    Exi_ISO_serviceScopeType* ServiceScopePtr = (Exi_ISO_serviceScopeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 194, 8);
    Exi_Encode_ISO_serviceScope(EncWsPtr, ServiceScopePtr);
    /* EE(ServiceScope) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SERVICE_SCOPE) && (EXI_ENCODE_ISO_SERVICE_SCOPE == STD_ON)) */
  }
  /* Fragment ServiceScope urn:iso:15118:2:2013:MsgDataTypes already implemented */
  /* Fragment SessionID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SESSION_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON))
    Exi_ISO_sessionIDType* SessionIDPtr = (Exi_ISO_sessionIDType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 196, 8);
    Exi_Encode_ISO_sessionID(EncWsPtr, SessionIDPtr);
    /* EE(SessionID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_ID) && (EXI_ENCODE_ISO_SESSION_ID == STD_ON)) */
  }
  /* Fragment SessionID urn:iso:15118:2:2013:MsgHeader already implemented */
  /* Fragment SessionSetupReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SESSION_SETUP_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON))
    Exi_ISO_SessionSetupReqType* SessionSetupReqPtr = (Exi_ISO_SessionSetupReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionSetupReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 198, 8);
    Exi_Encode_ISO_SessionSetupReq(EncWsPtr, SessionSetupReqPtr);
    /* EE(SessionSetupReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON)) */
  }
  /* Fragment SessionSetupRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SESSION_SETUP_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON))
    Exi_ISO_SessionSetupResType* SessionSetupResPtr = (Exi_ISO_SessionSetupResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionSetupRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 199, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SessionSetupRes(EncWsPtr, SessionSetupResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SessionSetupRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON)) */
  }
  /* Fragment SessionStopReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SESSION_STOP_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON))
    Exi_ISO_SessionStopReqType* SessionStopReqPtr = (Exi_ISO_SessionStopReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionStopReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 200, 8);
    Exi_Encode_ISO_SessionStopReq(EncWsPtr, SessionStopReqPtr);
    /* EE(SessionStopReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON)) */
  }
  /* Fragment SessionStopRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_SESSION_STOP_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON))
    Exi_ISO_SessionStopResType* SessionStopResPtr = (Exi_ISO_SessionStopResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionStopRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 201, 8);
    Exi_Encode_ISO_SessionStopRes(EncWsPtr, SessionStopResPtr);
    /* EE(SessionStopRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON)) */
  }
  /* Fragment SigMeterReading urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SIG_METER_READING_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON))
    Exi_ISO_sigMeterReadingType* SigMeterReadingPtr = (Exi_ISO_sigMeterReadingType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SigMeterReading) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 202, 8);
    Exi_Encode_ISO_sigMeterReading(EncWsPtr, SigMeterReadingPtr);
    /* EE(SigMeterReading) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SIG_METER_READING) && (EXI_ENCODE_ISO_SIG_METER_READING == STD_ON)) */
  }
  /* Fragment Signature http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON))
    Exi_XMLSIG_SignatureType* SignaturePtr = (Exi_XMLSIG_SignatureType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Signature) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 203, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Signature(EncWsPtr, SignaturePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Signature) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON)) */
  }
  /* Fragment SignatureMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
    Exi_XMLSIG_SignatureMethodType* SignatureMethodPtr = (Exi_XMLSIG_SignatureMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 204, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureMethod(EncWsPtr, SignatureMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */
  }
  /* Fragment SignatureProperties http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_PROPERTIES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
    Exi_XMLSIG_SignaturePropertiesType* SignaturePropertiesPtr = (Exi_XMLSIG_SignaturePropertiesType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureProperties) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 205, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperties(EncWsPtr, SignaturePropertiesPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureProperties) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON)) */
  }
  /* Fragment SignatureProperty http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_PROPERTY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
    Exi_XMLSIG_SignaturePropertyType* SignaturePropertyPtr = (Exi_XMLSIG_SignaturePropertyType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureProperty) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 206, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperty(EncWsPtr, SignaturePropertyPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureProperty) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
  }
  /* Fragment SignatureValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
    Exi_XMLSIG_SignatureValueType* SignatureValuePtr = (Exi_XMLSIG_SignatureValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 207, 8);
    Exi_Encode_XMLSIG_SignatureValue(EncWsPtr, SignatureValuePtr);
    /* EE(SignatureValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */
  }
  /* Fragment SignedInfo http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNED_INFO_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON))
    Exi_XMLSIG_SignedInfoType* SignedInfoPtr = (Exi_XMLSIG_SignedInfoType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignedInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 208, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignedInfo(EncWsPtr, SignedInfoPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignedInfo) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON)) */
  }
  /* Fragment SubCertificates urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SUB_CERTIFICATES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON))
    Exi_ISO_SubCertificatesType* SubCertificatesPtr = (Exi_ISO_SubCertificatesType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SubCertificates) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 209, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SubCertificates(EncWsPtr, SubCertificatesPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SubCertificates) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SUB_CERTIFICATES) && (EXI_ENCODE_ISO_SUB_CERTIFICATES == STD_ON)) */
  }
  /* Fragment SupportedEnergyTransferMode urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SUPPORTED_ENERGY_TRANSFER_MODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON))
    Exi_ISO_SupportedEnergyTransferModeType* SupportedEnergyTransferModePtr = (Exi_ISO_SupportedEnergyTransferModeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SupportedEnergyTransferMode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 210, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SupportedEnergyTransferMode(EncWsPtr, SupportedEnergyTransferModePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SupportedEnergyTransferMode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE) && (EXI_ENCODE_ISO_SUPPORTED_ENERGY_TRANSFER_MODE == STD_ON)) */
  }
  /* Fragment TMeter urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_TMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment TimeInterval urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_TIME_INTERVAL_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  else if(EXI_ISO_INTERVAL_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* SE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 212, 8);
    /* this type is empty */
    /* EE(TimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 255, 9);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  }
  /* Fragment Transform http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_TRANSFORM_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON))
    Exi_XMLSIG_TransformType* TransformPtr = (Exi_XMLSIG_TransformType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Transform) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 213, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transform(EncWsPtr, TransformPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Transform) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON)) */
  }
  /* Fragment Transforms http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_TRANSFORMS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON))
    Exi_XMLSIG_TransformsType* TransformsPtr = (Exi_XMLSIG_TransformsType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Transforms) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 214, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transforms(EncWsPtr, TransformsPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Transforms) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON)) */
  }
  /* Fragment Unit urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_UNIT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON))
    Exi_ISO_unitSymbolType* UnitPtr = (Exi_ISO_unitSymbolType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Unit) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 215, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_unitSymbol(EncWsPtr, UnitPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Unit) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_UNIT_SYMBOL) && (EXI_ENCODE_ISO_UNIT_SYMBOL == STD_ON)) */
  }
  /* Fragment V2G_Message urn:iso:15118:2:2013:MsgDef */
  else if(EXI_ISO_V2G_MESSAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_V2G_MESSAGE) && (EXI_ENCODE_ISO_V2G_MESSAGE == STD_ON))
    Exi_ISO_V2G_MessageType* V2G_MessagePtr = (Exi_ISO_V2G_MessageType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(V2G_Message) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 216, 8);
    Exi_Encode_ISO_V2G_Message(EncWsPtr, V2G_MessagePtr);
    /* EE(V2G_Message) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_V2G_MESSAGE) && (EXI_ENCODE_ISO_V2G_MESSAGE == STD_ON)) */
  }
  /* Fragment Value urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment WeldingDetectionReq urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_WELDING_DETECTION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON))
    Exi_ISO_WeldingDetectionReqType* WeldingDetectionReqPtr = (Exi_ISO_WeldingDetectionReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(WeldingDetectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 218, 8);
    Exi_Encode_ISO_WeldingDetectionReq(EncWsPtr, WeldingDetectionReqPtr);
    /* EE(WeldingDetectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON)) */
  }
  /* Fragment WeldingDetectionRes urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_WELDING_DETECTION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON))
    Exi_ISO_WeldingDetectionResType* WeldingDetectionResPtr = (Exi_ISO_WeldingDetectionResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(WeldingDetectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 219, 8);
    Exi_Encode_ISO_WeldingDetectionRes(EncWsPtr, WeldingDetectionResPtr);
    /* EE(WeldingDetectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON)) */
  }
  /* Fragment X509CRL http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509CRL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* X509CRLPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509CRL) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 220, 8);
    Exi_Encode_base64Binary(EncWsPtr, X509CRLPtr);
    /* EE(X509CRL) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment X509Certificate http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509CERTIFICATE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* X509CertificatePtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 221, 8);
    Exi_Encode_base64Binary(EncWsPtr, X509CertificatePtr);
    /* EE(X509Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment X509Data http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509DATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON))
    Exi_XMLSIG_X509DataType* X509DataPtr = (Exi_XMLSIG_X509DataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509Data) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 222, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_X509Data(EncWsPtr, X509DataPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(X509Data) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON)) */
  }
  /* Fragment X509IssuerName http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509ISSUER_NAME_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
    Exi_stringType* X509IssuerNamePtr = (Exi_stringType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509IssuerName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 223, 8);
    Exi_Encode_string(EncWsPtr, X509IssuerNamePtr);
    /* EE(X509IssuerName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
  }
  /* Fragment X509IssuerSerial http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509ISSUER_SERIAL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
    Exi_XMLSIG_X509IssuerSerialType* X509IssuerSerialPtr = (Exi_XMLSIG_X509IssuerSerialType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509IssuerSerial) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 224, 8);
    Exi_Encode_XMLSIG_X509IssuerSerial(EncWsPtr, X509IssuerSerialPtr);
    /* EE(X509IssuerSerial) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */
  }
  /* Fragment X509SKI http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509SKI_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* X509SKIPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509SKI) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 225, 8);
    Exi_Encode_base64Binary(EncWsPtr, X509SKIPtr);
    /* EE(X509SKI) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment X509SerialNumber http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509SERIAL_NUMBER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment X509SubjectName http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509SUBJECT_NAME_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
    Exi_stringType* X509SubjectNamePtr = (Exi_stringType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509SubjectName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 227, 8);
    Exi_Encode_string(EncWsPtr, X509SubjectNamePtr);
    /* EE(X509SubjectName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
  }
  /* Fragment XPath http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_XPATH_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
    Exi_stringType* XPathPtr = (Exi_stringType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(XPath) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 228, 8);
    Exi_Encode_string(EncWsPtr, XPathPtr);
    /* EE(XPath) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
  }
  /* Fragment Y http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_Y_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* YPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Y) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 229, 8);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, YPtr);
    /* EE(Y) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment amount urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_AMOUNT_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment amountMultiplier urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_AMOUNT_MULTIPLIER_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment boolValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_BOOL_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment byteValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_BYTE_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment costKind urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_COST_KIND_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON))
    Exi_ISO_costKindType* costKindPtr = (Exi_ISO_costKindType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(costKind) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 234, 8);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_costKind(EncWsPtr, costKindPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(costKind) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_COST_KIND) && (EXI_ENCODE_ISO_COST_KIND == STD_ON)) */
  }
  /* Fragment duration urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_DURATION_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment eMAID urn:iso:15118:2:2013:MsgBody */
  else if(EXI_ISO_E_MAID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON))
    /* SE(eMAID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 236, 8);
    /* Schema-informed Element Fragment Grammar required */
  #if (defined(EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT == STD_ON))
    Exi_Encode_ISO_eMAID_ElementFragment(EncWsPtr, (Exi_ISO_eMAIDType*)(EncWsPtr->InputData.StoragePtr)); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    /* Encode as EE */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 255, 9);
  #endif /* (defined(EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_E_MAID_ELEMENT_FRAGMENT == STD_ON)) */
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_E_MAID) && (EXI_ENCODE_ISO_E_MAID == STD_ON)) */
  }
  else if(EXI_ISO_EMAID_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON))
    /* SE(eMAID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 236, 8);
    /* Schema-informed Element Fragment Grammar required */
  #if (defined(EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT == STD_ON))
    Exi_Encode_ISO_EMAID_ElementFragment(EncWsPtr, (Exi_ISO_EMAIDType*)(EncWsPtr->InputData.StoragePtr)); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    /* Encode as EE */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 255, 9);
  #endif /* (defined(EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT) && (EXI_ENCODE_ISO_EMAID_ELEMENT_FRAGMENT == STD_ON)) */
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_EMAID) && (EXI_ENCODE_ISO_EMAID == STD_ON)) */
  }
  /* Fragment intValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_INT_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment physicalValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_PHYSICAL_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* physicalValuePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(physicalValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 238, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, physicalValuePtr);
    /* EE(physicalValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment shortValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_SHORT_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment start urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_START_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment startValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_START_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON))
    Exi_ISO_PhysicalValueType* startValuePtr = (Exi_ISO_PhysicalValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(startValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 241, 8);
    Exi_Encode_ISO_PhysicalValue(EncWsPtr, startValuePtr);
    /* EE(startValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_ISO_PHYSICAL_VALUE) && (EXI_ENCODE_ISO_PHYSICAL_VALUE == STD_ON)) */
  }
  /* Fragment stringValue urn:iso:15118:2:2013:MsgDataTypes */
  else if(EXI_ISO_STRING_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
    Exi_stringType* stringValuePtr = (Exi_stringType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(stringValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 242, 8);
    Exi_Encode_string(EncWsPtr, stringValuePtr);
    /* EE(stringValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 244, 8);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
  }
  else
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_ISO_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_ISO_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_ISO_SCHEMA_ROOT) && (EXI_ENCODE_ISO_SCHEMA_ROOT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_ISO_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_ROOT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_ISO_WELDING_DETECTION_RES_TYPE >= EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_ISO_SCHEMA_ROOT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(EXI_ISO_AC_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_AC_EVChargeParameterType* AC_EVChargeParameterPtr = (Exi_ISO_AC_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 7);
    Exi_Encode_ISO_AC_EVChargeParameter(EncWsPtr, AC_EVChargeParameterPtr);
    /* EE(AC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_AC_EVSECHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_AC_EVSECHARGE_PARAMETER == STD_ON))
    Exi_ISO_AC_EVSEChargeParameterType* AC_EVSEChargeParameterPtr = (Exi_ISO_AC_EVSEChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 7);
    Exi_Encode_ISO_AC_EVSEChargeParameter(EncWsPtr, AC_EVSEChargeParameterPtr);
    /* EE(AC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_AC_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AC_EVSESTATUS) && (EXI_ENCODE_ISO_AC_EVSESTATUS == STD_ON))
    Exi_ISO_AC_EVSEStatusType* AC_EVSEStatusPtr = (Exi_ISO_AC_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 7);
    Exi_Encode_ISO_AC_EVSEStatus(EncWsPtr, AC_EVSEStatusPtr);
    /* EE(AC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_AUTHORIZATION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_REQ) && (EXI_ENCODE_ISO_AUTHORIZATION_REQ == STD_ON))
    Exi_ISO_AuthorizationReqType* AuthorizationReqPtr = (Exi_ISO_AuthorizationReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AuthorizationReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_AuthorizationReq(EncWsPtr, AuthorizationReqPtr);
    /* EE(AuthorizationReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(AuthorizationReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_AUTHORIZATION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_AUTHORIZATION_RES) && (EXI_ENCODE_ISO_AUTHORIZATION_RES == STD_ON))
    Exi_ISO_AuthorizationResType* AuthorizationResPtr = (Exi_ISO_AuthorizationResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AuthorizationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 7);
    Exi_Encode_ISO_AuthorizationRes(EncWsPtr, AuthorizationResPtr);
    /* EE(AuthorizationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CABLE_CHECK_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_REQ) && (EXI_ENCODE_ISO_CABLE_CHECK_REQ == STD_ON))
    Exi_ISO_CableCheckReqType* CableCheckReqPtr = (Exi_ISO_CableCheckReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CableCheckReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 7);
    Exi_Encode_ISO_CableCheckReq(EncWsPtr, CableCheckReqPtr);
    /* EE(CableCheckReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CABLE_CHECK_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CABLE_CHECK_RES) && (EXI_ENCODE_ISO_CABLE_CHECK_RES == STD_ON))
    Exi_ISO_CableCheckResType* CableCheckResPtr = (Exi_ISO_CableCheckResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CableCheckRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 7);
    Exi_Encode_ISO_CableCheckRes(EncWsPtr, CableCheckResPtr);
    /* EE(CableCheckRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CERTIFICATE_INSTALLATION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_REQ == STD_ON))
    Exi_ISO_CertificateInstallationReqType* CertificateInstallationReqPtr = (Exi_ISO_CertificateInstallationReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateInstallationReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateInstallationReq(EncWsPtr, CertificateInstallationReqPtr);
    /* EE(CertificateInstallationReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(CertificateInstallationReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CERTIFICATE_INSTALLATION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES) && (EXI_ENCODE_ISO_CERTIFICATE_INSTALLATION_RES == STD_ON))
    Exi_ISO_CertificateInstallationResType* CertificateInstallationResPtr = (Exi_ISO_CertificateInstallationResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateInstallationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 7);
    Exi_Encode_ISO_CertificateInstallationRes(EncWsPtr, CertificateInstallationResPtr);
    /* EE(CertificateInstallationRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CERTIFICATE_UPDATE_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_REQ == STD_ON))
    Exi_ISO_CertificateUpdateReqType* CertificateUpdateReqPtr = (Exi_ISO_CertificateUpdateReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateUpdateReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateUpdateReq(EncWsPtr, CertificateUpdateReqPtr);
    /* EE(CertificateUpdateReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(CertificateUpdateReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CERTIFICATE_UPDATE_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES) && (EXI_ENCODE_ISO_CERTIFICATE_UPDATE_RES == STD_ON))
    Exi_ISO_CertificateUpdateResType* CertificateUpdateResPtr = (Exi_ISO_CertificateUpdateResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CertificateUpdateRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CertificateUpdateRes(EncWsPtr, CertificateUpdateResPtr);
    /* EE(CertificateUpdateRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(CertificateUpdateRes) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_REQ == STD_ON))
    Exi_ISO_ChargeParameterDiscoveryReqType* ChargeParameterDiscoveryReqPtr = (Exi_ISO_ChargeParameterDiscoveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeParameterDiscoveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ChargeParameterDiscoveryReq(EncWsPtr, ChargeParameterDiscoveryReqPtr);
    /* EE(ChargeParameterDiscoveryReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ChargeParameterDiscoveryReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CHARGE_PARAMETER_DISCOVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES) && (EXI_ENCODE_ISO_CHARGE_PARAMETER_DISCOVERY_RES == STD_ON))
    Exi_ISO_ChargeParameterDiscoveryResType* ChargeParameterDiscoveryResPtr = (Exi_ISO_ChargeParameterDiscoveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargeParameterDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 7);
    Exi_Encode_ISO_ChargeParameterDiscoveryRes(EncWsPtr, ChargeParameterDiscoveryResPtr);
    /* EE(ChargeParameterDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CHARGING_STATUS_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CHARGING_STATUS_RES) && (EXI_ENCODE_ISO_CHARGING_STATUS_RES == STD_ON))
    Exi_ISO_ChargingStatusResType* ChargingStatusResPtr = (Exi_ISO_ChargingStatusResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ChargingStatusRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 7);
    Exi_Encode_ISO_ChargingStatusRes(EncWsPtr, ChargingStatusResPtr);
    /* EE(ChargingStatusRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CURRENT_DEMAND_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_REQ) && (EXI_ENCODE_ISO_CURRENT_DEMAND_REQ == STD_ON))
    Exi_ISO_CurrentDemandReqType* CurrentDemandReqPtr = (Exi_ISO_CurrentDemandReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CurrentDemandReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 7);
    Exi_Encode_ISO_CurrentDemandReq(EncWsPtr, CurrentDemandReqPtr);
    /* EE(CurrentDemandReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_CURRENT_DEMAND_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_CURRENT_DEMAND_RES) && (EXI_ENCODE_ISO_CURRENT_DEMAND_RES == STD_ON))
    Exi_ISO_CurrentDemandResType* CurrentDemandResPtr = (Exi_ISO_CurrentDemandResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CurrentDemandRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_CurrentDemandRes(EncWsPtr, CurrentDemandResPtr);
    /* EE(CurrentDemandRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(CurrentDemandRes) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_DC_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_DC_EVChargeParameterType* DC_EVChargeParameterPtr = (Exi_ISO_DC_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVChargeParameter(EncWsPtr, DC_EVChargeParameterPtr);
    /* EE(DC_EVChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_DC_EVPOWER_DELIVERY_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER) && (EXI_ENCODE_ISO_DC_EVPOWER_DELIVERY_PARAMETER == STD_ON))
    Exi_ISO_DC_EVPowerDeliveryParameterType* DC_EVPowerDeliveryParameterPtr = (Exi_ISO_DC_EVPowerDeliveryParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 7);
    Exi_Encode_ISO_DC_EVPowerDeliveryParameter(EncWsPtr, DC_EVPowerDeliveryParameterPtr);
    /* EE(DC_EVPowerDeliveryParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_DC_EVSECHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER) && (EXI_ENCODE_ISO_DC_EVSECHARGE_PARAMETER == STD_ON))
    Exi_ISO_DC_EVSEChargeParameterType* DC_EVSEChargeParameterPtr = (Exi_ISO_DC_EVSEChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVSEChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_DC_EVSEChargeParameter(EncWsPtr, DC_EVSEChargeParameterPtr);
    /* EE(DC_EVSEChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DC_EVSEChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_DC_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSESTATUS) && (EXI_ENCODE_ISO_DC_EVSESTATUS == STD_ON))
    Exi_ISO_DC_EVSEStatusType* DC_EVSEStatusPtr = (Exi_ISO_DC_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 7);
    Exi_Encode_ISO_DC_EVSEStatus(EncWsPtr, DC_EVSEStatusPtr);
    /* EE(DC_EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_DC_EVSTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_DC_EVSTATUS) && (EXI_ENCODE_ISO_DC_EVSTATUS == STD_ON))
    Exi_ISO_DC_EVStatusType* DC_EVStatusPtr = (Exi_ISO_DC_EVStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DC_EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 23, 7);
    Exi_Encode_ISO_DC_EVStatus(EncWsPtr, DC_EVStatusPtr);
    /* EE(DC_EVStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_EVCHARGE_PARAMETER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVCHARGE_PARAMETER) && (EXI_ENCODE_ISO_EVCHARGE_PARAMETER == STD_ON))
    Exi_ISO_EVChargeParameterType* EVChargeParameterPtr = (Exi_ISO_EVChargeParameterType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVChargeParameter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 27, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_EVChargeParameter(EncWsPtr, EVChargeParameterPtr);
    /* EE(EVChargeParameter) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(EVChargeParameter) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_EVSESTATUS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_EVSESTATUS) && (EXI_ENCODE_ISO_EVSESTATUS == STD_ON))
    Exi_ISO_EVSEStatusType* EVSEStatusPtr = (Exi_ISO_EVSEStatusType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 30, 7);
    Exi_Encode_ISO_EVSEStatus(EncWsPtr, EVSEStatusPtr);
    /* EE(EVSEStatus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_ENTRY) && (EXI_ENCODE_ISO_ENTRY == STD_ON))
    Exi_ISO_EntryType* EntryPtr = (Exi_ISO_EntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Entry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 32, 7);
    Exi_Encode_ISO_Entry(EncWsPtr, EntryPtr);
    /* EE(Entry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_METERING_RECEIPT_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_REQ) && (EXI_ENCODE_ISO_METERING_RECEIPT_REQ == STD_ON))
    Exi_ISO_MeteringReceiptReqType* MeteringReceiptReqPtr = (Exi_ISO_MeteringReceiptReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeteringReceiptReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 37, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_MeteringReceiptReq(EncWsPtr, MeteringReceiptReqPtr);
    /* EE(MeteringReceiptReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(MeteringReceiptReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_METERING_RECEIPT_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_METERING_RECEIPT_RES) && (EXI_ENCODE_ISO_METERING_RECEIPT_RES == STD_ON))
    Exi_ISO_MeteringReceiptResType* MeteringReceiptResPtr = (Exi_ISO_MeteringReceiptResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MeteringReceiptRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 38, 7);
    Exi_Encode_ISO_MeteringReceiptRes(EncWsPtr, MeteringReceiptResPtr);
    /* EE(MeteringReceiptRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PMAX_SCHEDULE_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY) && (EXI_ENCODE_ISO_PMAX_SCHEDULE_ENTRY == STD_ON))
    Exi_ISO_PMaxScheduleEntryType* PMaxScheduleEntryPtr = (Exi_ISO_PMaxScheduleEntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PMaxScheduleEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 42, 7);
    Exi_Encode_ISO_PMaxScheduleEntry(EncWsPtr, PMaxScheduleEntryPtr);
    /* EE(PMaxScheduleEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PAYMENT_DETAILS_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_REQ == STD_ON))
    Exi_ISO_PaymentDetailsReqType* PaymentDetailsReqPtr = (Exi_ISO_PaymentDetailsReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentDetailsReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 43, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PaymentDetailsReq(EncWsPtr, PaymentDetailsReqPtr);
    /* EE(PaymentDetailsReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(PaymentDetailsReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PAYMENT_DETAILS_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_DETAILS_RES) && (EXI_ENCODE_ISO_PAYMENT_DETAILS_RES == STD_ON))
    Exi_ISO_PaymentDetailsResType* PaymentDetailsResPtr = (Exi_ISO_PaymentDetailsResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentDetailsRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 44, 7);
    Exi_Encode_ISO_PaymentDetailsRes(EncWsPtr, PaymentDetailsResPtr);
    /* EE(PaymentDetailsRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_REQ == STD_ON))
    Exi_ISO_PaymentServiceSelectionReqType* PaymentServiceSelectionReqPtr = (Exi_ISO_PaymentServiceSelectionReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentServiceSelectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 45, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PaymentServiceSelectionReq(EncWsPtr, PaymentServiceSelectionReqPtr);
    /* EE(PaymentServiceSelectionReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(PaymentServiceSelectionReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PAYMENT_SERVICE_SELECTION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES) && (EXI_ENCODE_ISO_PAYMENT_SERVICE_SELECTION_RES == STD_ON))
    Exi_ISO_PaymentServiceSelectionResType* PaymentServiceSelectionResPtr = (Exi_ISO_PaymentServiceSelectionResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PaymentServiceSelectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 7);
    Exi_Encode_ISO_PaymentServiceSelectionRes(EncWsPtr, PaymentServiceSelectionResPtr);
    /* EE(PaymentServiceSelectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_POWER_DELIVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_REQ) && (EXI_ENCODE_ISO_POWER_DELIVERY_REQ == STD_ON))
    Exi_ISO_PowerDeliveryReqType* PowerDeliveryReqPtr = (Exi_ISO_PowerDeliveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PowerDeliveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 47, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_PowerDeliveryReq(EncWsPtr, PowerDeliveryReqPtr);
    /* EE(PowerDeliveryReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(PowerDeliveryReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_POWER_DELIVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_POWER_DELIVERY_RES) && (EXI_ENCODE_ISO_POWER_DELIVERY_RES == STD_ON))
    Exi_ISO_PowerDeliveryResType* PowerDeliveryResPtr = (Exi_ISO_PowerDeliveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PowerDeliveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 48, 7);
    Exi_Encode_ISO_PowerDeliveryRes(EncWsPtr, PowerDeliveryResPtr);
    /* EE(PowerDeliveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PRE_CHARGE_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_REQ) && (EXI_ENCODE_ISO_PRE_CHARGE_REQ == STD_ON))
    Exi_ISO_PreChargeReqType* PreChargeReqPtr = (Exi_ISO_PreChargeReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PreChargeReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 49, 7);
    Exi_Encode_ISO_PreChargeReq(EncWsPtr, PreChargeReqPtr);
    /* EE(PreChargeReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_PRE_CHARGE_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_PRE_CHARGE_RES) && (EXI_ENCODE_ISO_PRE_CHARGE_RES == STD_ON))
    Exi_ISO_PreChargeResType* PreChargeResPtr = (Exi_ISO_PreChargeResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PreChargeRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 50, 7);
    Exi_Encode_ISO_PreChargeRes(EncWsPtr, PreChargeResPtr);
    /* EE(PreChargeRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_RELATIVE_TIME_INTERVAL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL) && (EXI_ENCODE_ISO_RELATIVE_TIME_INTERVAL == STD_ON))
    Exi_ISO_RelativeTimeIntervalType* RelativeTimeIntervalPtr = (Exi_ISO_RelativeTimeIntervalType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RelativeTimeInterval) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 53, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_RelativeTimeInterval(EncWsPtr, RelativeTimeIntervalPtr);
    /* EE(RelativeTimeInterval) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(RelativeTimeInterval) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SASCHEDULE_LIST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SASCHEDULE_LIST) && (EXI_ENCODE_ISO_SASCHEDULE_LIST == STD_ON))
    Exi_ISO_SAScheduleListType* SAScheduleListPtr = (Exi_ISO_SAScheduleListType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SAScheduleList) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 55, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SAScheduleList(EncWsPtr, SAScheduleListPtr);
    /* EE(SAScheduleList) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SAScheduleList) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SALES_TARIFF_ENTRY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SALES_TARIFF_ENTRY) && (EXI_ENCODE_ISO_SALES_TARIFF_ENTRY == STD_ON))
    Exi_ISO_SalesTariffEntryType* SalesTariffEntryPtr = (Exi_ISO_SalesTariffEntryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SalesTariffEntry) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 58, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SalesTariffEntry(EncWsPtr, SalesTariffEntryPtr);
    /* EE(SalesTariffEntry) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SalesTariffEntry) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SERVICE_DETAIL_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_REQ) && (EXI_ENCODE_ISO_SERVICE_DETAIL_REQ == STD_ON))
    Exi_ISO_ServiceDetailReqType* ServiceDetailReqPtr = (Exi_ISO_ServiceDetailReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDetailReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 59, 7);
    Exi_Encode_ISO_ServiceDetailReq(EncWsPtr, ServiceDetailReqPtr);
    /* EE(ServiceDetailReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SERVICE_DETAIL_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DETAIL_RES) && (EXI_ENCODE_ISO_SERVICE_DETAIL_RES == STD_ON))
    Exi_ISO_ServiceDetailResType* ServiceDetailResPtr = (Exi_ISO_ServiceDetailResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDetailRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 60, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDetailRes(EncWsPtr, ServiceDetailResPtr);
    /* EE(ServiceDetailRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ServiceDetailRes) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SERVICE_DISCOVERY_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_REQ == STD_ON))
    Exi_ISO_ServiceDiscoveryReqType* ServiceDiscoveryReqPtr = (Exi_ISO_ServiceDiscoveryReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDiscoveryReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 61, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDiscoveryReq(EncWsPtr, ServiceDiscoveryReqPtr);
    /* EE(ServiceDiscoveryReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ServiceDiscoveryReq) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SERVICE_DISCOVERY_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES) && (EXI_ENCODE_ISO_SERVICE_DISCOVERY_RES == STD_ON))
    Exi_ISO_ServiceDiscoveryResType* ServiceDiscoveryResPtr = (Exi_ISO_ServiceDiscoveryResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ServiceDiscoveryRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 62, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_ServiceDiscoveryRes(EncWsPtr, ServiceDiscoveryResPtr);
    /* EE(ServiceDiscoveryRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(ServiceDiscoveryRes) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SESSION_SETUP_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_REQ) && (EXI_ENCODE_ISO_SESSION_SETUP_REQ == STD_ON))
    Exi_ISO_SessionSetupReqType* SessionSetupReqPtr = (Exi_ISO_SessionSetupReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionSetupReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 63, 7);
    Exi_Encode_ISO_SessionSetupReq(EncWsPtr, SessionSetupReqPtr);
    /* EE(SessionSetupReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SESSION_SETUP_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_SETUP_RES) && (EXI_ENCODE_ISO_SESSION_SETUP_RES == STD_ON))
    Exi_ISO_SessionSetupResType* SessionSetupResPtr = (Exi_ISO_SessionSetupResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionSetupRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 64, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_SessionSetupRes(EncWsPtr, SessionSetupResPtr);
    /* EE(SessionSetupRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SessionSetupRes) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SESSION_STOP_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_STOP_REQ) && (EXI_ENCODE_ISO_SESSION_STOP_REQ == STD_ON))
    Exi_ISO_SessionStopReqType* SessionStopReqPtr = (Exi_ISO_SessionStopReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionStopReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 65, 7);
    Exi_Encode_ISO_SessionStopReq(EncWsPtr, SessionStopReqPtr);
    /* EE(SessionStopReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_SESSION_STOP_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_SESSION_STOP_RES) && (EXI_ENCODE_ISO_SESSION_STOP_RES == STD_ON))
    Exi_ISO_SessionStopResType* SessionStopResPtr = (Exi_ISO_SessionStopResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SessionStopRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 66, 7);
    Exi_Encode_ISO_SessionStopRes(EncWsPtr, SessionStopResPtr);
    /* EE(SessionStopRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_V2G_MESSAGE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_V2G_MESSAGE) && (EXI_ENCODE_ISO_V2G_MESSAGE == STD_ON))
    Exi_ISO_V2G_MessageType* V2G_MessagePtr = (Exi_ISO_V2G_MessageType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(V2G_Message) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 76, 7);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_ISO_V2G_Message(EncWsPtr, V2G_MessagePtr);
    /* EE(V2G_Message) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(V2G_Message) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_WELDING_DETECTION_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_REQ) && (EXI_ENCODE_ISO_WELDING_DETECTION_REQ == STD_ON))
    Exi_ISO_WeldingDetectionReqType* WeldingDetectionReqPtr = (Exi_ISO_WeldingDetectionReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(WeldingDetectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 77, 7);
    Exi_Encode_ISO_WeldingDetectionReq(EncWsPtr, WeldingDetectionReqPtr);
    /* EE(WeldingDetectionReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_ISO_WELDING_DETECTION_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_ISO_WELDING_DETECTION_RES) && (EXI_ENCODE_ISO_WELDING_DETECTION_RES == STD_ON))
    Exi_ISO_WeldingDetectionResType* WeldingDetectionResPtr = (Exi_ISO_WeldingDetectionResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(WeldingDetectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 78, 7);
    Exi_Encode_ISO_WeldingDetectionRes(EncWsPtr, WeldingDetectionResPtr);
    /* EE(WeldingDetectionRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_ISO_SCHEMA_ROOT) && (EXI_ENCODE_ISO_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */
#endif /* (defined(EXI_ENABLE_ENCODE_ISO_MESSAGE_SET) && (EXI_ENABLE_ENCODE_ISO_MESSAGE_SET == STD_ON)) */

