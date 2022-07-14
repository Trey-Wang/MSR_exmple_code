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
 *         File:  Exi_XMLSIG_SchemaEncoder.c
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


#define EXI_XMLSIG_SCHEMA_ENCODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_XMLSIG_SchemaEncoder.h"
#include "Exi_BSEncoder.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_XMLSIG_SchemaEncoder.c are inconsistent"
#endif

#if (!defined (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET))
# if (defined (EXI_ENABLE_XMLSIG_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET   EXI_ENABLE_XMLSIG_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET == STD_ON))

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeAlgorithm
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeAlgorithmType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeAlgorithmPtr       pointer to Exi_XMLSIG_AttributeAlgorithmType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeAlgorithm( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeAlgorithmType, AUTOMATIC, EXI_APPL_DATA) AttributeAlgorithmPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeAlgorithmPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeAlgorithmPtr->Length <= sizeof(AttributeAlgorithmPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeAlgorithmPtr->Buffer[0], AttributeAlgorithmPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeEncoding
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeEncodingType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeEncodingPtr        pointer to Exi_XMLSIG_AttributeEncodingType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeEncoding( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeEncodingType, AUTOMATIC, EXI_APPL_DATA) AttributeEncodingPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeEncodingPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeEncodingPtr->Length <= sizeof(AttributeEncodingPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeEncodingPtr->Buffer[0], AttributeEncodingPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeId
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeIdType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeIdPtr              pointer to Exi_XMLSIG_AttributeIdType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeId( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeIdType, AUTOMATIC, EXI_APPL_DATA) AttributeIdPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeIdPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeIdPtr->Length <= sizeof(AttributeIdPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeIdPtr->Buffer[0], AttributeIdPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeMime
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeMimeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeMimePtr            pointer to Exi_XMLSIG_AttributeMimeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeMime( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeMimeType, AUTOMATIC, EXI_APPL_DATA) AttributeMimePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeMimePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeMimePtr->Length <= sizeof(AttributeMimePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeMimePtr->Buffer[0], AttributeMimePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeTarget
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeTargetType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeTargetPtr          pointer to Exi_XMLSIG_AttributeTargetType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeTarget( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeTargetType, AUTOMATIC, EXI_APPL_DATA) AttributeTargetPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeTargetPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeTargetPtr->Length <= sizeof(AttributeTargetPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeTargetPtr->Buffer[0], AttributeTargetPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Attribute
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributePtr                pointer to Exi_XMLSIG_AttributeType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Attribute( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeType, AUTOMATIC, EXI_APPL_DATA) AttributePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributePtr->Length <= sizeof(AttributePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributePtr->Buffer[0], AttributePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_AttributeURI
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_AttributeURIType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     AttributeURIPtr             pointer to Exi_XMLSIG_AttributeURIType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeURI( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeURIType, AUTOMATIC, EXI_APPL_DATA) AttributeURIPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != AttributeURIPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((AttributeURIPtr->Length <= sizeof(AttributeURIPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &AttributeURIPtr->Buffer[0], AttributeURIPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_CanonicalizationMethod
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_CanonicalizationMethodType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CanonicalizationMethodPtr   pointer to Exi_XMLSIG_CanonicalizationMethodType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_CanonicalizationMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_CanonicalizationMethodType, AUTOMATIC, EXI_APPL_DATA) CanonicalizationMethodPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_GenericElementType* nextPtr = (Exi_XMLSIG_GenericElementType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CanonicalizationMethodPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Algorithm) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
  Exi_Encode_XMLSIG_AttributeAlgorithm(EncWsPtr, (CanonicalizationMethodPtr->Algorithm));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  
  if(1 == CanonicalizationMethodPtr->GenericElementFlag)
  {
    nextPtr = (Exi_XMLSIG_GenericElementType*)CanonicalizationMethodPtr->GenericElement;
    for(i=0; i<EXI_MAXOCCURS_GENERICELEMENT; i++)
    {
      /* SE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_GenericElementType*)(nextPtr->NextGenericElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_GenericElementType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(CanonicalizationMethod) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(CanonicalizationMethod) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_CryptoBinary
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_CryptoBinaryType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     CryptoBinaryPtr             pointer to Exi_XMLSIG_CryptoBinaryType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_CryptoBinary( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_CryptoBinaryType, AUTOMATIC, EXI_APPL_DATA) CryptoBinaryPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != CryptoBinaryPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((CryptoBinaryPtr->Length <= sizeof(CryptoBinaryPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &CryptoBinaryPtr->Buffer[0], CryptoBinaryPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_DSAKeyValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_DSAKeyValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DSAKeyValuePtr              pointer to Exi_XMLSIG_DSAKeyValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DSAKeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DSAKeyValueType, AUTOMATIC, EXI_APPL_DATA) DSAKeyValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DSAKeyValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(P) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->P));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(P) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(Q) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->Q));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(Q) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DSAKeyValuePtr->GFlag)
  {
    /* SE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->G));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
    /* EE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(Y) */
  if(0 == DSAKeyValuePtr->GFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->Y));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(Y) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == DSAKeyValuePtr->JFlag)
  {
    /* SE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->J));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
    /* EE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  /* SE(Seed) */
  if(0 == DSAKeyValuePtr->JFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->Seed));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(Seed) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(PgenCounter) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (DSAKeyValuePtr->PgenCounter));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(PgenCounter) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_DigestMethod
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_DigestMethodType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DigestMethodPtr             pointer to Exi_XMLSIG_DigestMethodType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DigestMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DigestMethodType, AUTOMATIC, EXI_APPL_DATA) DigestMethodPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_GenericElementType* nextPtr = (Exi_XMLSIG_GenericElementType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DigestMethodPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Algorithm) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
  Exi_Encode_XMLSIG_AttributeAlgorithm(EncWsPtr, (DigestMethodPtr->Algorithm));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  
  if(1 == DigestMethodPtr->GenericElementFlag)
  {
    nextPtr = (Exi_XMLSIG_GenericElementType*)DigestMethodPtr->GenericElement;
    for(i=0; i<EXI_MAXOCCURS_GENERICELEMENT; i++)
    {
      /* SE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_GenericElementType*)(nextPtr->NextGenericElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_GenericElementType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(DigestMethod) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(DigestMethod) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_DigestValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_DigestValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     DigestValuePtr              pointer to Exi_XMLSIG_DigestValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DigestValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DigestValueType, AUTOMATIC, EXI_APPL_DATA) DigestValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DigestValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((DigestValuePtr->Length <= sizeof(DigestValuePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &DigestValuePtr->Buffer[0], DigestValuePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_GenericElement
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_GenericElementType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     GenericElementPtr           pointer to Exi_XMLSIG_GenericElementType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_GenericElement( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_GenericElementType, AUTOMATIC, EXI_APPL_DATA) GenericElementPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != GenericElementPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_KeyInfo
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_KeyInfoType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     KeyInfoPtr                  pointer to Exi_XMLSIG_KeyInfoType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyInfoType, AUTOMATIC, EXI_APPL_DATA) KeyInfoPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_KeyInfoChoiceType* nextPtr = (Exi_XMLSIG_KeyInfoChoiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != KeyInfoPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != KeyInfoPtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == KeyInfoPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 4);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (KeyInfoPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  nextPtr = (Exi_XMLSIG_KeyInfoChoiceType*)KeyInfoPtr->ChoiceElement;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE; i++)
  {
    /* Start of Choice Element */
    if(1 == nextPtr->KeyNameFlag)
    {
      /* SE(KeyName) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 4);
      }
    #if (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON))
      Exi_Encode_XMLSIG_KeyName(EncWsPtr, (nextPtr->ChoiceValue.KeyName));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON)) */
      /* EE(KeyName) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->KeyValueFlag)
    {
      /* SE(KeyValue) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 4);
      }
    #if (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON))
      Exi_Encode_XMLSIG_KeyValue(EncWsPtr, (nextPtr->ChoiceValue.KeyValue));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON)) */
      /* EE(KeyValue) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->RetrievalMethodFlag)
    {
      /* SE(RetrievalMethod) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 4);
      }
      EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
      Exi_Encode_XMLSIG_RetrievalMethod(EncWsPtr, (nextPtr->ChoiceValue.RetrievalMethod));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
      /* EE(RetrievalMethod) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE() already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    }
    else if(1 == nextPtr->X509DataFlag)
    {
      /* SE(X509Data) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 4);
      }
      EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON))
      Exi_Encode_XMLSIG_X509Data(EncWsPtr, (nextPtr->ChoiceValue.X509Data));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON)) */
      /* EE(X509Data) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE() already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    }
    else if(1 == nextPtr->PGPDataFlag)
    {
      /* SE(PGPData) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 4);
      }
    #if (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON))
      Exi_Encode_XMLSIG_PGPData(EncWsPtr, (nextPtr->ChoiceValue.PGPData));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON)) */
      /* EE(PGPData) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->SPKIDataFlag)
    {
      /* SE(SPKIData) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 4);
      }
      EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON))
      Exi_Encode_XMLSIG_SPKIData(EncWsPtr, (nextPtr->ChoiceValue.SPKIData));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON)) */
      /* EE(SPKIData) */
      if(TRUE == EncWsPtr->EncWs.EERequired)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else
      {
        /* EE() already encoded in subfunction call */
        EncWsPtr->EncWs.EERequired = TRUE;
      }
    }
    else if(1 == nextPtr->MgmtDataFlag)
    {
      /* SE(MgmtData) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 4);
      }
    #if (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON))
      Exi_Encode_XMLSIG_MgmtData(EncWsPtr, (nextPtr->ChoiceValue.MgmtData));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON)) */
      /* EE(MgmtData) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->GenericElementFlag)
    {
      /* SE(GenericElement) */
      if((0 == i) && (0 == KeyInfoPtr->IdFlag)) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 4);
      }
      else
      { /* PRQA S 3201 */ /* MD_MSR_14.1 */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 4);
      }
    #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (nextPtr->ChoiceValue.GenericElement));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
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
    nextPtr = (Exi_XMLSIG_KeyInfoChoiceType*)(nextPtr->NextChoiceElementPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_KeyInfoChoiceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(KeyInfo) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 4);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_KeyName
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_KeyNameType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     KeyNamePtr                  pointer to Exi_XMLSIG_KeyNameType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyNameType, AUTOMATIC, EXI_APPL_DATA) KeyNamePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != KeyNamePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((KeyNamePtr->Length <= sizeof(KeyNamePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &KeyNamePtr->Buffer[0], KeyNamePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_KeyValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_KeyValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     KeyValuePtr                 pointer to Exi_XMLSIG_KeyValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyValueType, AUTOMATIC, EXI_APPL_DATA) KeyValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != KeyValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != KeyValuePtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Start of Choice Element */
  if(1 == KeyValuePtr->ChoiceElement->DSAKeyValueFlag)
  {
    /* SE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON))
    Exi_Encode_XMLSIG_DSAKeyValue(EncWsPtr, (KeyValuePtr->ChoiceElement->ChoiceValue.DSAKeyValue));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */
    /* EE(DSAKeyValue) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE() already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else if(1 == KeyValuePtr->ChoiceElement->RSAKeyValueFlag)
  {
    /* SE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  #if (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON))
    Exi_Encode_XMLSIG_RSAKeyValue(EncWsPtr, (KeyValuePtr->ChoiceElement->ChoiceValue.RSAKeyValue));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */
    /* EE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else if(1 == KeyValuePtr->ChoiceElement->GenericElementFlag)
  {
    /* SE(GenericElement) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
  #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (KeyValuePtr->ChoiceElement->ChoiceValue.GenericElement));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    /* EE(GenericElement) */
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
#endif /* (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Manifest
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_ManifestType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ManifestPtr                 pointer to Exi_XMLSIG_ManifestType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Manifest( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ManifestType, AUTOMATIC, EXI_APPL_DATA) ManifestPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_ReferenceType* nextPtr = (Exi_XMLSIG_ReferenceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ManifestPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == ManifestPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (ManifestPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  nextPtr = (Exi_XMLSIG_ReferenceType*)ManifestPtr->Reference;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_REFERENCE; i++)
  {
    /* SE(Reference) */
    if(0 == i)
    {
      if(0 == ManifestPtr->IdFlag)
      {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
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
    #if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
    Exi_Encode_XMLSIG_Reference(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON)) */
    /* EE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_XMLSIG_ReferenceType*)(nextPtr->NextReferencePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_ReferenceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(Manifest) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_MgmtData
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_MgmtDataType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     MgmtDataPtr                 pointer to Exi_XMLSIG_MgmtDataType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_MgmtData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_MgmtDataType, AUTOMATIC, EXI_APPL_DATA) MgmtDataPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != MgmtDataPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((MgmtDataPtr->Length <= sizeof(MgmtDataPtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeStringValue(&EncWsPtr->EncWs, &MgmtDataPtr->Buffer[0], MgmtDataPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Object
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_ObjectType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ObjectPtr                   pointer to Exi_XMLSIG_ObjectType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Object( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ObjectType, AUTOMATIC, EXI_APPL_DATA) ObjectPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ObjectPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == ObjectPtr->EncodingFlag)
  {
    /* AT(Encoding) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON))
    Exi_Encode_XMLSIG_AttributeEncoding(EncWsPtr, (ObjectPtr->Encoding));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON)) */
  
  }
  if(1 == ObjectPtr->IdFlag)
  {
    /* AT(Id) */
    if(0 == ObjectPtr->EncodingFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    }
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (ObjectPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  if(1 == ObjectPtr->MimeTypeFlag)
  {
    /* AT(MimeType) */
    if(0 == ObjectPtr->IdFlag)
    {
      if(0 == ObjectPtr->EncodingFlag)
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
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
    Exi_Encode_XMLSIG_AttributeMime(EncWsPtr, (ObjectPtr->MimeType));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */
  
  }
  /* SE(GenericElement) */
  if(0 == ObjectPtr->MimeTypeFlag)
  {
    if(0 == ObjectPtr->IdFlag)
    {
      if(0 == ObjectPtr->EncodingFlag)
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
  #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
  Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (ObjectPtr->GenericElement));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  /* EE(GenericElement) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_PGPDataChoiceSeq0
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq0Type object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PGPDataChoiceSeq0Ptr        pointer to Exi_XMLSIG_PGPDataChoiceSeq0Type data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPDataChoiceSeq0( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataChoiceSeq0Type, AUTOMATIC, EXI_APPL_DATA) PGPDataChoiceSeq0Ptr )
{
  uint16 i = 0;
  Exi_XMLSIG_GenericElementType* nextPtr = (Exi_XMLSIG_GenericElementType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PGPDataChoiceSeq0Ptr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(PGPKeyID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
  Exi_Encode_base64Binary(EncWsPtr, (PGPDataChoiceSeq0Ptr->PGPKeyID));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  /* EE(PGPKeyID) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == PGPDataChoiceSeq0Ptr->PGPKeyPacketFlag)
  {
    /* SE(PGPKeyPacket) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_Encode_base64Binary(EncWsPtr, (PGPDataChoiceSeq0Ptr->PGPKeyPacket));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
    /* EE(PGPKeyPacket) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == PGPDataChoiceSeq0Ptr->GenericElementFlag)
  {
    nextPtr = (Exi_XMLSIG_GenericElementType*)PGPDataChoiceSeq0Ptr->GenericElement;
    for(i=0; i<EXI_MAXOCCURS_GENERICELEMENT; i++)
    {
      /* SE(GenericElement) */
      if(0 == PGPDataChoiceSeq0Ptr->PGPKeyPacketFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      }
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_GenericElementType*)(nextPtr->NextGenericElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_GenericElementType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(PGPDataChoiceSeq0) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(PGPDataChoiceSeq0) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == PGPDataChoiceSeq0Ptr->PGPKeyPacketFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_PGPDataChoiceSeq1
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq1Type object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PGPDataChoiceSeq1Ptr        pointer to Exi_XMLSIG_PGPDataChoiceSeq1Type data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPDataChoiceSeq1( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataChoiceSeq1Type, AUTOMATIC, EXI_APPL_DATA) PGPDataChoiceSeq1Ptr )
{
  uint16 i = 0;
  Exi_XMLSIG_GenericElementType* nextPtr = (Exi_XMLSIG_GenericElementType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PGPDataChoiceSeq1Ptr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(PGPKeyPacket) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
  Exi_Encode_base64Binary(EncWsPtr, (PGPDataChoiceSeq1Ptr->PGPKeyPacket));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  /* EE(PGPKeyPacket) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == PGPDataChoiceSeq1Ptr->GenericElementFlag)
  {
    nextPtr = (Exi_XMLSIG_GenericElementType*)PGPDataChoiceSeq1Ptr->GenericElement;
    for(i=0; i<EXI_MAXOCCURS_GENERICELEMENT; i++)
    {
      /* SE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_GenericElementType*)(nextPtr->NextGenericElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_GenericElementType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(PGPDataChoiceSeq1) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(PGPDataChoiceSeq1) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_PGPData
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_PGPDataType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     PGPDataPtr                  pointer to Exi_XMLSIG_PGPDataType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataType, AUTOMATIC, EXI_APPL_DATA) PGPDataPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PGPDataPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != PGPDataPtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Start of Choice Element */
  if(1 == PGPDataPtr->ChoiceElement->ChoiceSequence0Flag)
  {
    /* SE(ChoiceSequence0) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON))
    Exi_Encode_XMLSIG_PGPDataChoiceSeq0(EncWsPtr, (PGPDataPtr->ChoiceElement->ChoiceValue.ChoiceSequence0));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON)) */
    /* EE(ChoiceSequence0) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE() already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else if(1 == PGPDataPtr->ChoiceElement->ChoiceSequence1Flag)
  {
    /* SE(ChoiceSequence1) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON))
    Exi_Encode_XMLSIG_PGPDataChoiceSeq1(EncWsPtr, (PGPDataPtr->ChoiceElement->ChoiceValue.ChoiceSequence1));
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON)) */
    /* EE(ChoiceSequence1) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE() already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
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
#endif /* (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_RSAKeyValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_RSAKeyValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     RSAKeyValuePtr              pointer to Exi_XMLSIG_RSAKeyValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_RSAKeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_RSAKeyValueType, AUTOMATIC, EXI_APPL_DATA) RSAKeyValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != RSAKeyValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(Modulus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (RSAKeyValuePtr->Modulus));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(Modulus) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(Exponent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
  Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, (RSAKeyValuePtr->Exponent));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  /* EE(Exponent) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Reference
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_ReferenceType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     ReferencePtr                pointer to Exi_XMLSIG_ReferenceType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Reference( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ReferenceType, AUTOMATIC, EXI_APPL_DATA) ReferencePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != ReferencePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == ReferencePtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (ReferencePtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  if(1 == ReferencePtr->TypeFlag)
  {
    /* AT(Type) */
    if(0 == ReferencePtr->IdFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    }
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON))
    Exi_Encode_XMLSIG_Attribute(EncWsPtr, (ReferencePtr->Type));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON)) */
  
  }
  if(1 == ReferencePtr->URIFlag)
  {
    /* AT(URI) */
    if(0 == ReferencePtr->TypeFlag)
    {
      if(0 == ReferencePtr->IdFlag)
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
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
    Exi_Encode_XMLSIG_AttributeURI(EncWsPtr, (ReferencePtr->URI));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
  
  }
  if(1 == ReferencePtr->TransformsFlag)
  {
    /* SE(Transforms) */
    if(0 == ReferencePtr->URIFlag)
    {
      if(0 == ReferencePtr->TypeFlag)
      {
        if(0 == ReferencePtr->IdFlag)
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
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON))
    Exi_Encode_XMLSIG_Transforms(EncWsPtr, (ReferencePtr->Transforms));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON)) */
    /* EE(Transforms) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Transforms) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  /* SE(DigestMethod) */
  if(0 == ReferencePtr->TransformsFlag)
  {
    if(0 == ReferencePtr->URIFlag)
    {
      if(0 == ReferencePtr->TypeFlag)
      {
        if(0 == ReferencePtr->IdFlag)
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
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON))
  Exi_Encode_XMLSIG_DigestMethod(EncWsPtr, (ReferencePtr->DigestMethod));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON)) */
  /* EE(DigestMethod) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(DigestMethod) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(DigestValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON))
  Exi_Encode_XMLSIG_DigestValue(EncWsPtr, (ReferencePtr->DigestValue));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON)) */
  /* EE(DigestValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_RetrievalMethod
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_RetrievalMethodType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     RetrievalMethodPtr          pointer to Exi_XMLSIG_RetrievalMethodType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_RetrievalMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_RetrievalMethodType, AUTOMATIC, EXI_APPL_DATA) RetrievalMethodPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != RetrievalMethodPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == RetrievalMethodPtr->TypeFlag)
  {
    /* AT(Type) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON))
    Exi_Encode_XMLSIG_Attribute(EncWsPtr, (RetrievalMethodPtr->Type));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE) && (EXI_ENCODE_XMLSIG_ATTRIBUTE == STD_ON)) */
  
  }
  if(1 == RetrievalMethodPtr->URIFlag)
  {
    /* AT(URI) */
    if(0 == RetrievalMethodPtr->TypeFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
    Exi_Encode_XMLSIG_AttributeURI(EncWsPtr, (RetrievalMethodPtr->URI));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_URI) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
  
  }
  if(1 == RetrievalMethodPtr->TransformsFlag)
  {
    /* SE(Transforms) */
    if(0 == RetrievalMethodPtr->URIFlag)
    {
      if(0 == RetrievalMethodPtr->TypeFlag)
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
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON))
    Exi_Encode_XMLSIG_Transforms(EncWsPtr, (RetrievalMethodPtr->Transforms));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON)) */
    /* EE(Transforms) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Transforms) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE(RetrievalMethod) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == RetrievalMethodPtr->URIFlag)
    {
      if(0 == RetrievalMethodPtr->TypeFlag)
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
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SPKIData
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SPKIDataType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SPKIDataPtr                 pointer to Exi_XMLSIG_SPKIDataType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SPKIData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SPKIDataType, AUTOMATIC, EXI_APPL_DATA) SPKIDataPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SPKIDataPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(SPKISexp) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
  Exi_Encode_base64Binary(EncWsPtr, (SPKIDataPtr->SPKISexp));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  /* EE(SPKISexp) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == SPKIDataPtr->GenericElementFlag)
  {
    /* SE(GenericElement) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (SPKIDataPtr->GenericElement));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    /* EE(GenericElement) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SPKIData) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SignatureMethod
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignatureMethodType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignatureMethodPtr          pointer to Exi_XMLSIG_SignatureMethodType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureMethodType, AUTOMATIC, EXI_APPL_DATA) SignatureMethodPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_GenericElementType* nextPtr = (Exi_XMLSIG_GenericElementType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignatureMethodPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Algorithm) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
  Exi_Encode_XMLSIG_AttributeAlgorithm(EncWsPtr, (SignatureMethodPtr->Algorithm));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  
  if(1 == SignatureMethodPtr->HMACOutputLengthFlag)
  {
    /* SE(HMACOutputLength) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    Exi_VBSEncodeBigInt(&EncWsPtr->EncWs, (SignatureMethodPtr->HMACOutputLength));
    /* EE(HMACOutputLength) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  if(1 == SignatureMethodPtr->GenericElementFlag)
  {
    nextPtr = (Exi_XMLSIG_GenericElementType*)SignatureMethodPtr->GenericElement;
    for(i=0; i<EXI_MAXOCCURS_GENERICELEMENT; i++)
    {
      /* SE(GenericElement) */
      if(0 == SignatureMethodPtr->HMACOutputLengthFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      }
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_GenericElementType*)(nextPtr->NextGenericElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_GenericElementType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(SignatureMethod) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(SignatureMethod) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == SignatureMethodPtr->HMACOutputLengthFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SignatureProperties
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignaturePropertiesType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignaturePropertiesPtr      pointer to Exi_XMLSIG_SignaturePropertiesType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureProperties( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignaturePropertiesType, AUTOMATIC, EXI_APPL_DATA) SignaturePropertiesPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_SignaturePropertyType* nextPtr = (Exi_XMLSIG_SignaturePropertyType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignaturePropertiesPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SignaturePropertiesPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (SignaturePropertiesPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  nextPtr = (Exi_XMLSIG_SignaturePropertyType*)SignaturePropertiesPtr->SignatureProperty;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY; i++)
  {
    /* SE(SignatureProperty) */
    if(0 == i)
    {
      if(0 == SignaturePropertiesPtr->IdFlag)
      {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
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
    #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
    Exi_Encode_XMLSIG_SignatureProperty(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
    /* EE(SignatureProperty) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SignatureProperty) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_XMLSIG_SignaturePropertyType*)(nextPtr->NextSignaturePropertyPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_SignaturePropertyType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SignatureProperties) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SignatureProperty
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignaturePropertyType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignaturePropertyPtr        pointer to Exi_XMLSIG_SignaturePropertyType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureProperty( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignaturePropertyType, AUTOMATIC, EXI_APPL_DATA) SignaturePropertyPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_SignaturePropertyChoiceType* nextPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignaturePropertyPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignaturePropertyPtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SignaturePropertyPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (SignaturePropertyPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* AT(Target) */
  if(0 == SignaturePropertyPtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
  Exi_Encode_XMLSIG_AttributeTarget(EncWsPtr, (SignaturePropertyPtr->Target));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */
  
  nextPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*)SignaturePropertyPtr->ChoiceElement;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE; i++)
  {
    /* Start of Choice Element */
    if(1 == nextPtr->GenericElementFlag)
    {
      /* SE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (nextPtr->ChoiceValue.GenericElement));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
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
    nextPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*)(nextPtr->NextChoiceElementPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_SignaturePropertyChoiceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SignatureProperty) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Signature
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignatureType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignaturePtr                pointer to Exi_XMLSIG_SignatureType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Signature( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureType, AUTOMATIC, EXI_APPL_DATA) SignaturePtr )
{
  uint16 i = 0;
  Exi_XMLSIG_ObjectType* nextPtr = (Exi_XMLSIG_ObjectType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignaturePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SignaturePtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (SignaturePtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* SE(SignedInfo) */
  if(0 == SignaturePtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON))
  Exi_Encode_XMLSIG_SignedInfo(EncWsPtr, (SignaturePtr->SignedInfo));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON)) */
  /* EE(SignedInfo) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SignedInfo) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(SignatureValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
  Exi_Encode_XMLSIG_SignatureValue(EncWsPtr, (SignaturePtr->SignatureValue));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */
  /* EE(SignatureValue) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  if(1 == SignaturePtr->KeyInfoFlag)
  {
    /* SE(KeyInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON))
    Exi_Encode_XMLSIG_KeyInfo(EncWsPtr, (SignaturePtr->KeyInfo));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON)) */
    /* EE(KeyInfo) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(KeyInfo) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
  }
  if(1 == SignaturePtr->ObjectFlag)
  {
    nextPtr = (Exi_XMLSIG_ObjectType*)SignaturePtr->Object;
    for(i=0; i<EXI_MAXOCCURS_XMLSIG_OBJECT; i++)
    {
      /* SE(Object) */
      if(0 == SignaturePtr->KeyInfoFlag)
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
      }
      else
      {
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
      }
      #if (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON))
      Exi_Encode_XMLSIG_Object(EncWsPtr, nextPtr);
      #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      #endif /* (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON)) */
      /* EE(Object) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      nextPtr = (Exi_XMLSIG_ObjectType*)(nextPtr->NextObjectPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_ObjectType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(Signature) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(Signature) */
    EncWsPtr->EncWs.EERequired = FALSE;
    if(0 == SignaturePtr->KeyInfoFlag)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    }
    else
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SignatureValue
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignatureValueType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignatureValuePtr           pointer to Exi_XMLSIG_SignatureValueType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureValueType, AUTOMATIC, EXI_APPL_DATA) SignatureValuePtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignatureValuePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((SignatureValuePtr->Length <= sizeof(SignatureValuePtr->Buffer)), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SignatureValuePtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (SignatureValuePtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* start content */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  Exi_VBSEncodeBytes(&EncWsPtr->EncWs, &SignatureValuePtr->Buffer[0], SignatureValuePtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SignedInfo
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SignedInfoType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     SignedInfoPtr               pointer to Exi_XMLSIG_SignedInfoType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignedInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignedInfoType, AUTOMATIC, EXI_APPL_DATA) SignedInfoPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_ReferenceType* nextPtr = (Exi_XMLSIG_ReferenceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != SignedInfoPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(1 == SignedInfoPtr->IdFlag)
  {
    /* AT(Id) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    Exi_Encode_XMLSIG_AttributeId(EncWsPtr, (SignedInfoPtr->Id));
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ID) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  
  }
  /* SE(CanonicalizationMethod) */
  if(0 == SignedInfoPtr->IdFlag)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  }
  else
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
  Exi_Encode_XMLSIG_CanonicalizationMethod(EncWsPtr, (SignedInfoPtr->CanonicalizationMethod));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
  /* EE(CanonicalizationMethod) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(CanonicalizationMethod) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  /* SE(SignatureMethod) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  EncWsPtr->EncWs.EERequired = TRUE;
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
  Exi_Encode_XMLSIG_SignatureMethod(EncWsPtr, (SignedInfoPtr->SignatureMethod));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */
  /* EE(SignatureMethod) */
  if(TRUE == EncWsPtr->EncWs.EERequired)
  {
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  }
  else
  {
    /* EE(SignatureMethod) already encoded in subfunction call */
    EncWsPtr->EncWs.EERequired = TRUE;
  }
  nextPtr = (Exi_XMLSIG_ReferenceType*)SignedInfoPtr->Reference;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_REFERENCE; i++)
  {
    /* SE(Reference) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    #if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
    Exi_Encode_XMLSIG_Reference(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON)) */
    /* EE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    nextPtr = (Exi_XMLSIG_ReferenceType*)(nextPtr->NextReferencePtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_ReferenceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(SignedInfo) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Transform
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_TransformType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     TransformPtr                pointer to Exi_XMLSIG_TransformType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Transform( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_TransformType, AUTOMATIC, EXI_APPL_DATA) TransformPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_TransformChoiceType* nextPtr = (Exi_XMLSIG_TransformChoiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != TransformPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* AT(Algorithm) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
  Exi_Encode_XMLSIG_AttributeAlgorithm(EncWsPtr, (TransformPtr->Algorithm));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  
  if(1 == TransformPtr->ChoiceElementFlag)
  {
    nextPtr = (Exi_XMLSIG_TransformChoiceType*)TransformPtr->ChoiceElement;
    for(i=0; i<EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE; i++)
    {
      /* Start of Choice Element */
      if(1 == nextPtr->GenericElementFlag)
      {
        /* SE(GenericElement) */
        if(i == 0) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
        {
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
        }
        else
        { /* PRQA S 3201 */ /* MD_MSR_14.1 */
          Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
        }
      #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
        Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (nextPtr->ChoiceValue.GenericElement));
      #else
        /* not supported in this configuration */
        Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
        if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
        {
          EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
        }
      #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
        /* EE(GenericElement) */
        Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
      }
      else if(1 == nextPtr->XPathFlag)
        {
          /* SE(XPath) */
          if(i == 0) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
          {
            Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
          }
          else
          { /* PRQA S 3201 */ /* MD_MSR_14.1 */
            Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
          }
        #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
          Exi_Encode_string(EncWsPtr, (nextPtr->ChoiceValue.XPath));
        #else
          /* not supported in this configuration */
          Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
          if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
          {
            EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
          }
        #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
          /* EE(XPath) */
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
      nextPtr = (Exi_XMLSIG_TransformChoiceType*)(nextPtr->NextChoiceElementPtr);
      if(NULL_PTR == nextPtr)
      {
        /* i holds the number of encoded Exi_XMLSIG_TransformChoiceType elements */
        i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
        break;
      }
    }
    /* EE(Transform) */
    /* Max Occurs is unbounded */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 2);
    EncWsPtr->EncWs.EERequired = FALSE;
  }
  else
  {
    /* EE(Transform) */
    EncWsPtr->EncWs.EERequired = FALSE;
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_Transforms
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_TransformsType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     TransformsPtr               pointer to Exi_XMLSIG_TransformsType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Transforms( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_TransformsType, AUTOMATIC, EXI_APPL_DATA) TransformsPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_TransformType* nextPtr = (Exi_XMLSIG_TransformType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != TransformsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_XMLSIG_TransformType*)TransformsPtr->Transform;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_TRANSFORM; i++)
  {
    /* SE(Transform) */
    if(0 == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 2);
    }
    EncWsPtr->EncWs.EERequired = TRUE;
    #if (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON))
    Exi_Encode_XMLSIG_Transform(EncWsPtr, nextPtr);
    #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON)) */
    /* EE(Transform) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Transform) already encoded in subfunction call */
      EncWsPtr->EncWs.EERequired = TRUE;
    }
    nextPtr = (Exi_XMLSIG_TransformType*)(nextPtr->NextTransformPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_TransformType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(Transforms) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 2);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_X509Data
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_X509DataType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     X509DataPtr                 pointer to Exi_XMLSIG_X509DataType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_X509Data( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_X509DataType, AUTOMATIC, EXI_APPL_DATA) X509DataPtr )
{
  uint16 i = 0;
  Exi_XMLSIG_X509DataChoiceType* nextPtr = (Exi_XMLSIG_X509DataChoiceType*) NULL_PTR;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 11 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != X509DataPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != X509DataPtr->ChoiceElement), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  nextPtr = (Exi_XMLSIG_X509DataChoiceType*)X509DataPtr->ChoiceElement;
  for(i=0; i<EXI_MAXOCCURS_XMLSIG_X509DATACHOICE; i++)
  {
    /* Start of Choice Element */
    if(1 == nextPtr->X509IssuerSerialFlag)
    {
      /* SE(X509IssuerSerial) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 3);
    #if (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
      Exi_Encode_XMLSIG_X509IssuerSerial(EncWsPtr, (nextPtr->ChoiceValue.X509IssuerSerial));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */
      /* EE(X509IssuerSerial) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->X509SKIFlag)
    {
      /* SE(X509SKI) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 3);
    #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
      Exi_Encode_base64Binary(EncWsPtr, (nextPtr->ChoiceValue.X509SKI));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
      /* EE(X509SKI) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->X509SubjectNameFlag)
    {
      /* SE(X509SubjectName) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 3);
    #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
      Exi_Encode_string(EncWsPtr, (nextPtr->ChoiceValue.X509SubjectName));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
      /* EE(X509SubjectName) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->X509CertificateFlag)
    {
      /* SE(X509Certificate) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 3);
    #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
      Exi_Encode_base64Binary(EncWsPtr, (nextPtr->ChoiceValue.X509Certificate));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
      /* EE(X509Certificate) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->X509CRLFlag)
    {
      /* SE(X509CRL) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 3);
    #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
      Exi_Encode_base64Binary(EncWsPtr, (nextPtr->ChoiceValue.X509CRL));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
      /* EE(X509CRL) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else if(1 == nextPtr->GenericElementFlag)
    {
      /* SE(GenericElement) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 3);
    #if (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      Exi_Encode_XMLSIG_GenericElement(EncWsPtr, (nextPtr->ChoiceValue.GenericElement));
    #else
      /* not supported in this configuration */
      Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
      {
        EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
    #endif /* (defined(EXI_ENCODE_XMLSIG_GENERIC_ELEMENT) && (EXI_ENCODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
      /* EE(GenericElement) */
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
    nextPtr = (Exi_XMLSIG_X509DataChoiceType*)(nextPtr->NextChoiceElementPtr);
    if(NULL_PTR == nextPtr)
    {
      /* i holds the number of encoded Exi_XMLSIG_X509DataChoiceType elements */
      i++; /* PRQA S 2469 */  /*  MD_Exi_13.6 */
      break;
    }
  }
  /* EE(X509Data) */
  /* Max Occurs is unbounded */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 3);
  EncWsPtr->EncWs.EERequired = FALSE;
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_X509IssuerSerial
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_X509IssuerSerialType object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \param[in]     X509IssuerSerialPtr         pointer to Exi_XMLSIG_X509IssuerSerialType data struct to be encoded
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_X509IssuerSerial( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_X509IssuerSerialType, AUTOMATIC, EXI_APPL_DATA) X509IssuerSerialPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.PBufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->EncWs.BufPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((EncWsPtr->EncWs.BytePos < EncWsPtr->EncWs.BufLen), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != X509IssuerSerialPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* SE(X509IssuerName) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  #if (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON))
  Exi_Encode_string(EncWsPtr, (X509IssuerSerialPtr->X509IssuerName));
  #else
  /* not supported in this configuration */
  Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  #endif /* (defined(EXI_ENCODE_STRING) && (EXI_ENCODE_STRING == STD_ON)) */
  /* EE(X509IssuerName) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* SE(X509SerialNumber) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  /* start content */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  Exi_VBSEncodeBigInt(&EncWsPtr->EncWs, (X509IssuerSerialPtr->X509SerialNumber));
  /* EE(X509SerialNumber) */
  Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_FRAGMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_FRAGMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE > EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  /* Fragment CanonicalizationMethod http://www.w3.org/2000/09/xmldsig# */
  if(EXI_XMLSIG_CANONICALIZATION_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
    Exi_XMLSIG_CanonicalizationMethodType* CanonicalizationMethodPtr = (Exi_XMLSIG_CanonicalizationMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CanonicalizationMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_CanonicalizationMethod(EncWsPtr, CanonicalizationMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(CanonicalizationMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
  }
  /* Fragment DSAKeyValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_DSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_DSAKeyValueType* DSAKeyValuePtr = (Exi_XMLSIG_DSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 6);
    Exi_Encode_XMLSIG_DSAKeyValue(EncWsPtr, DSAKeyValuePtr);
    /* EE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */
  }
  /* Fragment DigestMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_DIGEST_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON))
    Exi_XMLSIG_DigestMethodType* DigestMethodPtr = (Exi_XMLSIG_DigestMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DigestMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_DigestMethod(EncWsPtr, DigestMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(DigestMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 6);
    Exi_Encode_XMLSIG_DigestValue(EncWsPtr, DigestValuePtr);
    /* EE(DigestValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON)) */
  }
  /* Fragment Exponent http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_EXPONENT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* ExponentPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Exponent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, ExponentPtr);
    /* EE(Exponent) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment G http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_G_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* GPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, GPtr);
    /* EE(G) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
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
  /* Fragment J http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_J_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* JPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, JPtr);
    /* EE(J) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_KeyInfo(EncWsPtr, KeyInfoPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(KeyInfo) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 6);
    Exi_Encode_XMLSIG_KeyName(EncWsPtr, KeyNamePtr);
    /* EE(KeyName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 6);
    Exi_Encode_XMLSIG_KeyValue(EncWsPtr, KeyValuePtr);
    /* EE(KeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON)) */
  }
  /* Fragment Manifest http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_MANIFEST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON))
    Exi_XMLSIG_ManifestType* ManifestPtr = (Exi_XMLSIG_ManifestType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Manifest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Manifest(EncWsPtr, ManifestPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Manifest) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON)) */
  }
  /* Fragment MgmtData http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_MGMT_DATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON))
    Exi_XMLSIG_MgmtDataType* MgmtDataPtr = (Exi_XMLSIG_MgmtDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MgmtData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 6);
    Exi_Encode_XMLSIG_MgmtData(EncWsPtr, MgmtDataPtr);
    /* EE(MgmtData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, ModulusPtr);
    /* EE(Modulus) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment Object http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_OBJECT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON))
    Exi_XMLSIG_ObjectType* ObjectPtr = (Exi_XMLSIG_ObjectType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Object) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 6);
    Exi_Encode_XMLSIG_Object(EncWsPtr, ObjectPtr);
    /* EE(Object) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 15, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, PPtr);
    /* EE(P) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 6);
    Exi_Encode_XMLSIG_PGPData(EncWsPtr, PGPDataPtr);
    /* EE(PGPData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 6);
    Exi_Encode_base64Binary(EncWsPtr, PGPKeyIDPtr);
    /* EE(PGPKeyID) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 6);
    Exi_Encode_base64Binary(EncWsPtr, PGPKeyPacketPtr);
    /* EE(PGPKeyPacket) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment PgenCounter http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_PGEN_COUNTER_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* PgenCounterPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PgenCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, PgenCounterPtr);
    /* EE(PgenCounter) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment Q http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_Q_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* QPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Q) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, QPtr);
    /* EE(Q) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment RSAKeyValue http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_RSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_RSAKeyValueType* RSAKeyValuePtr = (Exi_XMLSIG_RSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 6);
    Exi_Encode_XMLSIG_RSAKeyValue(EncWsPtr, RSAKeyValuePtr);
    /* EE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */
  }
  /* Fragment Reference http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_REFERENCE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
    Exi_XMLSIG_ReferenceType* ReferencePtr = (Exi_XMLSIG_ReferenceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 6);
    Exi_Encode_XMLSIG_Reference(EncWsPtr, ReferencePtr);
    /* EE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON)) */
  }
  /* Fragment RetrievalMethod http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_RETRIEVAL_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
    Exi_XMLSIG_RetrievalMethodType* RetrievalMethodPtr = (Exi_XMLSIG_RetrievalMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RetrievalMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 23, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_RetrievalMethod(EncWsPtr, RetrievalMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(RetrievalMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
  }
  /* Fragment SPKIData http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SPKIDATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON))
    Exi_XMLSIG_SPKIDataType* SPKIDataPtr = (Exi_XMLSIG_SPKIDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SPKIData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 24, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SPKIData(EncWsPtr, SPKIDataPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SPKIData) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 25, 6);
    Exi_Encode_base64Binary(EncWsPtr, SPKISexpPtr);
    /* EE(SPKISexp) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON)) */
  }
  /* Fragment Seed http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SEED_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    Exi_XMLSIG_CryptoBinaryType* SeedPtr = (Exi_XMLSIG_CryptoBinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Seed) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 26, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, SeedPtr);
    /* EE(Seed) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  /* Fragment Signature http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_SIGNATURE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON))
    Exi_XMLSIG_SignatureType* SignaturePtr = (Exi_XMLSIG_SignatureType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Signature) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 27, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Signature(EncWsPtr, SignaturePtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Signature) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 28, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureMethod(EncWsPtr, SignatureMethodPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureMethod) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 29, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperties(EncWsPtr, SignaturePropertiesPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureProperties) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 30, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperty(EncWsPtr, SignaturePropertyPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignatureProperty) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 31, 6);
    Exi_Encode_XMLSIG_SignatureValue(EncWsPtr, SignatureValuePtr);
    /* EE(SignatureValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 32, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignedInfo(EncWsPtr, SignedInfoPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(SignedInfo) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON)) */
  }
  /* Fragment Transform http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_TRANSFORM_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON))
    Exi_XMLSIG_TransformType* TransformPtr = (Exi_XMLSIG_TransformType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Transform) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 33, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transform(EncWsPtr, TransformPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Transform) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 34, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transforms(EncWsPtr, TransformsPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(Transforms) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON)) */
  }
  /* Fragment X509CRL http://www.w3.org/2000/09/xmldsig# */
  else if(EXI_XMLSIG_X509CRL_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_BASE64BINARY) && (EXI_ENCODE_BASE64BINARY == STD_ON))
    Exi_base64BinaryType* X509CRLPtr = (Exi_base64BinaryType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509CRL) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 35, 6);
    Exi_Encode_base64Binary(EncWsPtr, X509CRLPtr);
    /* EE(X509CRL) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 36, 6);
    Exi_Encode_base64Binary(EncWsPtr, X509CertificatePtr);
    /* EE(X509Certificate) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 37, 6);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_X509Data(EncWsPtr, X509DataPtr);
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      /* EE(X509Data) */
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 38, 6);
    Exi_Encode_string(EncWsPtr, X509IssuerNamePtr);
    /* EE(X509IssuerName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 39, 6);
    Exi_Encode_XMLSIG_X509IssuerSerial(EncWsPtr, X509IssuerSerialPtr);
    /* EE(X509IssuerSerial) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 40, 6);
    Exi_Encode_base64Binary(EncWsPtr, X509SKIPtr);
    /* EE(X509SKI) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 42, 6);
    Exi_Encode_string(EncWsPtr, X509SubjectNamePtr);
    /* EE(X509SubjectName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 43, 6);
    Exi_Encode_string(EncWsPtr, XPathPtr);
    /* EE(XPath) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
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
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 44, 6);
    Exi_Encode_XMLSIG_CryptoBinary(EncWsPtr, YPtr);
    /* EE(Y) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    /* ED */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 46, 6);
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_FRAGMENT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif /* (defined(EXI_ENCODE_XMLSIG_CRYPTO_BINARY) && (EXI_ENCODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SCHEMA_ROOT) && (EXI_ENCODE_XMLSIG_SCHEMA_ROOT == STD_ON))
FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr )
{

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_ROOT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != EncWsPtr->InputData.StoragePtr), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_ROOT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((EXI_XMLSIG_X509DATA_TYPE >= EncWsPtr->InputData.RootElementId), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_XMLSIG_SCHEMA_ROOT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(EXI_XMLSIG_CANONICALIZATION_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
    Exi_XMLSIG_CanonicalizationMethodType* CanonicalizationMethodPtr = (Exi_XMLSIG_CanonicalizationMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(CanonicalizationMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_CanonicalizationMethod(EncWsPtr, CanonicalizationMethodPtr);
    /* EE(CanonicalizationMethod) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(CanonicalizationMethod) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_DSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_DSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_DSAKeyValueType* DSAKeyValuePtr = (Exi_XMLSIG_DSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 1, 5);
    Exi_Encode_XMLSIG_DSAKeyValue(EncWsPtr, DSAKeyValuePtr);
    /* EE(DSAKeyValue) */
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
  else if(EXI_XMLSIG_DIGEST_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_METHOD) && (EXI_ENCODE_XMLSIG_DIGEST_METHOD == STD_ON))
    Exi_XMLSIG_DigestMethodType* DigestMethodPtr = (Exi_XMLSIG_DigestMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DigestMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 2, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_DigestMethod(EncWsPtr, DigestMethodPtr);
    /* EE(DigestMethod) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(DigestMethod) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_DIGEST_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_DIGEST_VALUE) && (EXI_ENCODE_XMLSIG_DIGEST_VALUE == STD_ON))
    Exi_XMLSIG_DigestValueType* DigestValuePtr = (Exi_XMLSIG_DigestValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(DigestValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 3, 5);
    Exi_Encode_XMLSIG_DigestValue(EncWsPtr, DigestValuePtr);
    /* EE(DigestValue) */
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
  else if(EXI_XMLSIG_KEY_INFO_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_INFO) && (EXI_ENCODE_XMLSIG_KEY_INFO == STD_ON))
    Exi_XMLSIG_KeyInfoType* KeyInfoPtr = (Exi_XMLSIG_KeyInfoType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 4, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_KeyInfo(EncWsPtr, KeyInfoPtr);
    /* EE(KeyInfo) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(KeyInfo) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_KEY_NAME_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_NAME) && (EXI_ENCODE_XMLSIG_KEY_NAME == STD_ON))
    Exi_XMLSIG_KeyNameType* KeyNamePtr = (Exi_XMLSIG_KeyNameType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyName) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 5, 5);
    Exi_Encode_XMLSIG_KeyName(EncWsPtr, KeyNamePtr);
    /* EE(KeyName) */
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
  else if(EXI_XMLSIG_KEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_KEY_VALUE) && (EXI_ENCODE_XMLSIG_KEY_VALUE == STD_ON))
    Exi_XMLSIG_KeyValueType* KeyValuePtr = (Exi_XMLSIG_KeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(KeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 6, 5);
    Exi_Encode_XMLSIG_KeyValue(EncWsPtr, KeyValuePtr);
    /* EE(KeyValue) */
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
  else if(EXI_XMLSIG_MANIFEST_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MANIFEST) && (EXI_ENCODE_XMLSIG_MANIFEST == STD_ON))
    Exi_XMLSIG_ManifestType* ManifestPtr = (Exi_XMLSIG_ManifestType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Manifest) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 7, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Manifest(EncWsPtr, ManifestPtr);
    /* EE(Manifest) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Manifest) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_MGMT_DATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_MGMT_DATA) && (EXI_ENCODE_XMLSIG_MGMT_DATA == STD_ON))
    Exi_XMLSIG_MgmtDataType* MgmtDataPtr = (Exi_XMLSIG_MgmtDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(MgmtData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 8, 5);
    Exi_Encode_XMLSIG_MgmtData(EncWsPtr, MgmtDataPtr);
    /* EE(MgmtData) */
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
  else if(EXI_XMLSIG_OBJECT_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_OBJECT) && (EXI_ENCODE_XMLSIG_OBJECT == STD_ON))
    Exi_XMLSIG_ObjectType* ObjectPtr = (Exi_XMLSIG_ObjectType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Object) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 9, 5);
    Exi_Encode_XMLSIG_Object(EncWsPtr, ObjectPtr);
    /* EE(Object) */
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
  else if(EXI_XMLSIG_PGPDATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_PGPDATA) && (EXI_ENCODE_XMLSIG_PGPDATA == STD_ON))
    Exi_XMLSIG_PGPDataType* PGPDataPtr = (Exi_XMLSIG_PGPDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(PGPData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 10, 5);
    Exi_Encode_XMLSIG_PGPData(EncWsPtr, PGPDataPtr);
    /* EE(PGPData) */
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
  else if(EXI_XMLSIG_RSAKEY_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RSAKEY_VALUE) && (EXI_ENCODE_XMLSIG_RSAKEY_VALUE == STD_ON))
    Exi_XMLSIG_RSAKeyValueType* RSAKeyValuePtr = (Exi_XMLSIG_RSAKeyValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RSAKeyValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 11, 5);
    Exi_Encode_XMLSIG_RSAKeyValue(EncWsPtr, RSAKeyValuePtr);
    /* EE(RSAKeyValue) */
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
  else if(EXI_XMLSIG_REFERENCE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_REFERENCE) && (EXI_ENCODE_XMLSIG_REFERENCE == STD_ON))
    Exi_XMLSIG_ReferenceType* ReferencePtr = (Exi_XMLSIG_ReferenceType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Reference) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 12, 5);
    Exi_Encode_XMLSIG_Reference(EncWsPtr, ReferencePtr);
    /* EE(Reference) */
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
  else if(EXI_XMLSIG_RETRIEVAL_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
    Exi_XMLSIG_RetrievalMethodType* RetrievalMethodPtr = (Exi_XMLSIG_RetrievalMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(RetrievalMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 13, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_RetrievalMethod(EncWsPtr, RetrievalMethodPtr);
    /* EE(RetrievalMethod) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(RetrievalMethod) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_SPKIDATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SPKIDATA) && (EXI_ENCODE_XMLSIG_SPKIDATA == STD_ON))
    Exi_XMLSIG_SPKIDataType* SPKIDataPtr = (Exi_XMLSIG_SPKIDataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SPKIData) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 14, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SPKIData(EncWsPtr, SPKIDataPtr);
    /* EE(SPKIData) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SPKIData) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_SIGNATURE_METHOD_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_METHOD) && (EXI_ENCODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
    Exi_XMLSIG_SignatureMethodType* SignatureMethodPtr = (Exi_XMLSIG_SignatureMethodType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureMethod) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 16, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureMethod(EncWsPtr, SignatureMethodPtr);
    /* EE(SignatureMethod) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SignatureMethod) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_SIGNATURE_PROPERTIES_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
    Exi_XMLSIG_SignaturePropertiesType* SignaturePropertiesPtr = (Exi_XMLSIG_SignaturePropertiesType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureProperties) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 17, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperties(EncWsPtr, SignaturePropertiesPtr);
    /* EE(SignatureProperties) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SignatureProperties) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_SIGNATURE_PROPERTY_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
    Exi_XMLSIG_SignaturePropertyType* SignaturePropertyPtr = (Exi_XMLSIG_SignaturePropertyType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureProperty) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 18, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignatureProperty(EncWsPtr, SignaturePropertyPtr);
    /* EE(SignatureProperty) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SignatureProperty) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_SIGNATURE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE) && (EXI_ENCODE_XMLSIG_SIGNATURE == STD_ON))
    Exi_XMLSIG_SignatureType* SignaturePtr = (Exi_XMLSIG_SignatureType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Signature) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 15, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Signature(EncWsPtr, SignaturePtr);
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
  #else
    /* not supported in this configuration */
    Exi_CallInternalDetReportError(EXI_ENCODER_INSTANCE_ID, EXI_API_ID_ENCODE_SCHEMA_ROOT, EXI_E_INV_PARAM);
    if(EXI_E_OK == EncWsPtr->EncWs.StatusCode)
    {
      EncWsPtr->EncWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
  #endif
  }
  else if(EXI_XMLSIG_SIGNATURE_VALUE_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNATURE_VALUE) && (EXI_ENCODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
    Exi_XMLSIG_SignatureValueType* SignatureValuePtr = (Exi_XMLSIG_SignatureValueType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignatureValue) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 19, 5);
    Exi_Encode_XMLSIG_SignatureValue(EncWsPtr, SignatureValuePtr);
    /* EE(SignatureValue) */
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
  else if(EXI_XMLSIG_SIGNED_INFO_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_SIGNED_INFO) && (EXI_ENCODE_XMLSIG_SIGNED_INFO == STD_ON))
    Exi_XMLSIG_SignedInfoType* SignedInfoPtr = (Exi_XMLSIG_SignedInfoType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(SignedInfo) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 20, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_SignedInfo(EncWsPtr, SignedInfoPtr);
    /* EE(SignedInfo) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(SignedInfo) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_TRANSFORM_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_TRANSFORM) && (EXI_ENCODE_XMLSIG_TRANSFORM == STD_ON))
    Exi_XMLSIG_TransformType* TransformPtr = (Exi_XMLSIG_TransformType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Transform) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 21, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transform(EncWsPtr, TransformPtr);
    /* EE(Transform) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Transform) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_TRANSFORMS_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_TRANSFORMS) && (EXI_ENCODE_XMLSIG_TRANSFORMS == STD_ON))
    Exi_XMLSIG_TransformsType* TransformsPtr = (Exi_XMLSIG_TransformsType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(Transforms) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 22, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_Transforms(EncWsPtr, TransformsPtr);
    /* EE(Transforms) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(Transforms) already encoded in subfunction call */
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
  else if(EXI_XMLSIG_X509DATA_TYPE == EncWsPtr->InputData.RootElementId)
  {
  #if (defined(EXI_ENCODE_XMLSIG_X509DATA) && (EXI_ENCODE_XMLSIG_X509DATA == STD_ON))
    Exi_XMLSIG_X509DataType* X509DataPtr = (Exi_XMLSIG_X509DataType*)(EncWsPtr->InputData.StoragePtr); /* PRQA S 0310 */  /*  MD_Exi_11.4 */ /* PRQA S 0311 */  /*  MD_Exi_11.5 */
    /* SE(X509Data) */
    Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 23, 5);
    EncWsPtr->EncWs.EERequired = TRUE;
    Exi_Encode_XMLSIG_X509Data(EncWsPtr, X509DataPtr);
    /* EE(X509Data) */
    if(TRUE == EncWsPtr->EncWs.EERequired)
    {
      Exi_VBSEncodeUIntN(&EncWsPtr->EncWs, 0, 1);
    }
    else
    {
      /* EE(X509Data) already encoded in subfunction call */
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
#endif /* (defined(EXI_ENCODE_XMLSIG_SCHEMA_ROOT) && (EXI_ENCODE_XMLSIG_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */
#endif /* (defined(EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET == STD_ON)) */

