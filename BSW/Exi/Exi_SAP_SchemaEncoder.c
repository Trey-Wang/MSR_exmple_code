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
 *         File:  Exi_SAP_SchemaEncoder.c
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


#define EXI_SAP_SCHEMA_ENCODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_SAP_SchemaEncoder.h"
#include "Exi_BSEncoder.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_SAP_SchemaEncoder.c are inconsistent"
#endif

#if (!defined (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET))
# if (defined (EXI_ENABLE_SAP_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_SAP_MESSAGE_SET   EXI_ENABLE_SAP_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_SAP_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET) && (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET == STD_ON))

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Encode_SAP_AppProtocol
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_AppProtocolType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AppProtocolPtr              pointer to Exi_SAP_AppProtocolType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_AppProtocol( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_AppProtocolType, AUTOMATIC, EXI_APPL_DATA) AppProtocolPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AppProtocolPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(((1 <= AppProtocolPtr->Priority) && (AppProtocolPtr->Priority <= 20)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ProtocolNamespace) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
  Exi_Encode_SAP_protocolNamespace(EncWsPtr, (AppProtocolPtr->ProtocolNamespace));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */
  /* EE(ProtocolNamespace) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(VersionNumberMajor) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(AppProtocolPtr->VersionNumberMajor));
  /* EE(VersionNumberMajor) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(VersionNumberMinor) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUInt(&EncWsPtr->EncWs, (Exi_BitBufType)(AppProtocolPtr->VersionNumberMinor));
  /* EE(VersionNumberMinor) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(SchemaID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(AppProtocolPtr->SchemaID), 8);
  /* EE(SchemaID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(Priority) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(AppProtocolPtr->Priority - 1), 5);
  /* EE(Priority) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_protocolNamespace
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_protocolNamespaceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     protocolNamespacePtr        pointer to Exi_SAP_protocolNamespaceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_protocolNamespace( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_protocolNamespaceType, AUTOMATIC, EXI_APPL_DATA) protocolNamespacePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != protocolNamespacePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((protocolNamespacePtr->Length <= sizeof(protocolNamespacePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &protocolNamespacePtr->Buffer[0], protocolNamespacePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_responseCode
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_responseCodeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     responseCodePtr             pointer to Exi_SAP_responseCodeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_responseCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_responseCodeType, AUTOMATIC, EXI_APPL_DATA) responseCodePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != responseCodePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(EXI_SAP_RESPONSE_CODE_TYPE_OK_SUCCESSFUL_NEGOTIATION == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
  }
  else if(EXI_SAP_RESPONSE_CODE_TYPE_OK_SUCCESSFUL_NEGOTIATION_WITH_MINOR_DEVIATION == *responseCodePtr)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_supportedAppProtocolReq
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_supportedAppProtocolReqType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     supportedAppProtocolReqPtr  pointer to Exi_SAP_supportedAppProtocolReqType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_supportedAppProtocolReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_supportedAppProtocolReqType, AUTOMATIC, EXI_APPL_DATA) supportedAppProtocolReqPtr )
{
  uint8 i = 0;
  Exi_SAP_AppProtocolType* nextPtr = (Exi_SAP_AppProtocolType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != supportedAppProtocolReqPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_SAP_AppProtocolType*)supportedAppProtocolReqPtr->AppProtocol;
  for(i=0; i<20; i++)
  {
    /* SE(AppProtocol) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON))
    Exi_Encode_SAP_AppProtocol(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON)) */
    /* EE(AppProtocol) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_SAP_AppProtocolType*)(nextPtr->NextAppProtocolPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_SAP_AppProtocolType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(supportedAppProtocolReq) */
  if( i < 20)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_supportedAppProtocolRes
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_supportedAppProtocolResType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     supportedAppProtocolResPtr  pointer to Exi_SAP_supportedAppProtocolResType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_supportedAppProtocolRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_supportedAppProtocolResType, AUTOMATIC, EXI_APPL_DATA) supportedAppProtocolResPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != supportedAppProtocolResPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON))
  Exi_Encode_SAP_responseCode(EncWsPtr, &(supportedAppProtocolResPtr->ResponseCode));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON)) */
  /* EE(ResponseCode) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == supportedAppProtocolResPtr->SchemaIDFlag)
  {
    /* SE(SchemaID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, (Exi_BitBufType)(supportedAppProtocolResPtr->SchemaID), 8);
    /* EE(SchemaID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(supportedAppProtocolRes) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Fragment AppProtocol urn:iso:15118:2:2010:AppProtocol */
  if(EXI_SAP_APP_PROTOCOL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON))
    Exi_SAP_AppProtocolType* AppProtocolPtr = (Exi_SAP_AppProtocolType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(AppProtocol) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 4);
    Exi_Encode_SAP_AppProtocol(EncWsPtr, AppProtocolPtr);
    /* EE(AppProtocol) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_SAP_APP_PROTOCOL) && (EXI_ENCODE_SAP_APP_PROTOCOL == STD_ON)) */
  }
  /* Fragment Priority urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_PRIORITY_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment ProtocolNamespace urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_PROTOCOL_NAMESPACE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
    Exi_SAP_protocolNamespaceType* ProtocolNamespacePtr = (Exi_SAP_protocolNamespaceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ProtocolNamespace) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 4);
    Exi_Encode_SAP_protocolNamespace(EncWsPtr, ProtocolNamespacePtr);
    /* EE(ProtocolNamespace) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_SAP_PROTOCOL_NAMESPACE) && (EXI_ENCODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */
  }
  /* Fragment ResponseCode urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_RESPONSE_CODE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON))
    Exi_SAP_responseCodeType* ResponseCodePtr = (Exi_SAP_responseCodeType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(ResponseCode) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 4);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_SAP_responseCode(EncWsPtr, ResponseCodePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(ResponseCode) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_SAP_RESPONSE_CODE) && (EXI_ENCODE_SAP_RESPONSE_CODE == STD_ON)) */
  }
  /* Fragment SchemaID urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_SCHEMA_ID_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment VersionNumberMajor urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_VERSION_NUMBER_MAJOR_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment VersionNumberMinor urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_VERSION_NUMBER_MINOR_TYPE == EncWsPtr->InputData.RootElementId)
  {
    /* Not supported */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_NOT_OK;
    }
  }
  /* Fragment supportedAppProtocolReq urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_SUPPORTED_APP_PROTOCOL_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
    Exi_SAP_supportedAppProtocolReqType* supportedAppProtocolReqPtr = (Exi_SAP_supportedAppProtocolReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(supportedAppProtocolReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 4);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_SAP_supportedAppProtocolReq(EncWsPtr, supportedAppProtocolReqPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(supportedAppProtocolReq) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON)) */
  }
  /* Fragment supportedAppProtocolRes urn:iso:15118:2:2010:AppProtocol */
  else if(EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
    Exi_SAP_supportedAppProtocolResType* supportedAppProtocolResPtr = (Exi_SAP_supportedAppProtocolResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(supportedAppProtocolRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 4);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_SAP_supportedAppProtocolRes(EncWsPtr, supportedAppProtocolResPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(supportedAppProtocolRes) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 4);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */
  }
  else
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SCHEMA_ROOT) && (EXI_ENCODE_SAP_SCHEMA_ROOT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_SAP_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_ROOT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE >= EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SAP_SCHEMA_ROOT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(EXI_SAP_SUPPORTED_APP_PROTOCOL_REQ_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
    Exi_SAP_supportedAppProtocolReqType* supportedAppProtocolReqPtr = (Exi_SAP_supportedAppProtocolReqType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(supportedAppProtocolReq) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_SAP_supportedAppProtocolReq(EncWsPtr, supportedAppProtocolReqPtr);
    /* EE(supportedAppProtocolReq) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(supportedAppProtocolReq) already encoded in subfunction call */
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
  else if(EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
    Exi_SAP_supportedAppProtocolResType* supportedAppProtocolResPtr = (Exi_SAP_supportedAppProtocolResType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(supportedAppProtocolRes) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_SAP_supportedAppProtocolRes(EncWsPtr, supportedAppProtocolResPtr);
    /* EE(supportedAppProtocolRes) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(supportedAppProtocolRes) already encoded in subfunction call */
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
  else
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_INV_EVENT_CODE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_SAP_SCHEMA_ROOT) && (EXI_ENCODE_SAP_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */
#endif /* (defined(EXI_ENABLE_ENCODE_SAP_MESSAGE_SET) && (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET == STD_ON)) */

