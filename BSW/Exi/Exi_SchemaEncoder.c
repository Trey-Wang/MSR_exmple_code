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
 *         File:  Exi_SchemaEncoder.c
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

#define EXI_SCHEMA_ENCODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_SchemaEncoder.h"
#include "Exi_BSEncoder.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_SchemaEncoder.c are inconsistent"
#endif

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */

#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Encode_base64Binary
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_base64BinaryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     se64BinaryPtr               pointer to Exi_base64BinaryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_base64Binary( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_base64BinaryType, AUTOMATIC, EXI_APPL_DATA) se64BinaryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != se64BinaryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((se64BinaryPtr->Length <= sizeof(se64BinaryPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_BASE64BINARY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &se64BinaryPtr->Buffer[0], se64BinaryPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Encode_string
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_stringType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ringPtr                     pointer to Exi_stringType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_string( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_stringType, AUTOMATIC, EXI_APPL_DATA) ringPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ringPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((ringPtr->Length <= sizeof(ringPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_STRING, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &ringPtr->Buffer[0], ringPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Encode_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     Namespace                   Namespace identifier to select the based schema namepace for this fragment
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SCHEMA_FRAGMENT) && (EXI_ENCODE_SCHEMA_FRAGMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       Exi_NamespaceIdType Namespace )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 5 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE > Namespace), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  #if (defined (EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON))
  if(EXI_SCHEMA_SET_SAP_TYPE == Namespace)
  {
    Exi_Encode_SAP_SchemaFragment(EncWsPtr);
  }
  else
  #endif /* (defined (EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON)) */
  #if (defined (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON))
  if(EXI_SCHEMA_SET_XMLSIG_TYPE == Namespace)
  {
    Exi_Encode_XMLSIG_SchemaFragment(EncWsPtr);
  }
  else
  #endif /* (defined (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON)) */
  #if (defined (EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON))
  if(EXI_SCHEMA_SET_ISO_TYPE == Namespace)
  {
    Exi_Encode_ISO_SchemaFragment(EncWsPtr);
  }
  else
  #endif /* (defined (EXI_ENCODE_ISO_SCHEMA_FRAGMENT) && (EXI_ENCODE_ISO_SCHEMA_FRAGMENT == STD_ON)) */
  #if (defined (EXI_ENCODE_DIN_SCHEMA_FRAGMENT) && (EXI_ENCODE_DIN_SCHEMA_FRAGMENT == STD_ON))
  if(EXI_SCHEMA_SET_DIN_TYPE == Namespace)
  {
    Exi_Encode_DIN_SchemaFragment(EncWsPtr);
  }
  else
  #endif /* (defined (EXI_ENCODE_DIN_SCHEMA_FRAGMENT) && (EXI_ENCODE_DIN_SCHEMA_FRAGMENT == STD_ON)) */
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SCHEMA_FRAGMENT) && (EXI_ENCODE_SCHEMA_FRAGMENT == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Encode_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SCHEMA_ROOT) && (EXI_ENCODE_SCHEMA_ROOT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((200 > EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

    if(EncWsPtr->InputData.RootElementId <= EXI_DIN_WELDING_DETECTION_RES_TYPE)
    {
      /* DIN */
  #if (defined(EXI_ENCODE_DIN_SCHEMA_ROOT) && (EXI_ENCODE_DIN_SCHEMA_ROOT == STD_ON))
      Exi_Encode_DIN_SchemaRoot(EncWsPtr);
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
    if(EncWsPtr->InputData.RootElementId <= EXI_ISO_WELDING_DETECTION_RES_TYPE)
    {
      /* ISO */
  #if (defined(EXI_ENCODE_ISO_SCHEMA_ROOT) && (EXI_ENCODE_ISO_SCHEMA_ROOT == STD_ON))
      Exi_Encode_ISO_SchemaRoot(EncWsPtr);
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
    if(EncWsPtr->InputData.RootElementId <= EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE)
    {
      /* SAP */
  #if (defined(EXI_ENCODE_SAP_SCHEMA_ROOT) && (EXI_ENCODE_SAP_SCHEMA_ROOT == STD_ON))
      Exi_Encode_SAP_SchemaRoot(EncWsPtr);
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
    if(EncWsPtr->InputData.RootElementId <= EXI_XMLSIG_X509DATA_TYPE)
    {
      /* XMLSIG */
  #if (defined(EXI_ENCODE_XMLSIG_SCHEMA_ROOT) && (EXI_ENCODE_XMLSIG_SCHEMA_ROOT == STD_ON))
      Exi_Encode_XMLSIG_SchemaRoot(EncWsPtr);
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
#endif /* (defined(EXI_ENCODE_SCHEMA_ROOT) && (EXI_ENCODE_SCHEMA_ROOT == STD_ON)) */

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */

