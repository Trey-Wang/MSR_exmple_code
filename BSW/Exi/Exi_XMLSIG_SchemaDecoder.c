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
 *         File:  Exi_XMLSIG_SchemaDecoder.c
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


#define EXI_XMLSIG_SCHEMA_DECODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_XMLSIG_SchemaDecoder.h"
#include "Exi_BSDecoder.h"
#include "Exi_Priv.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_XMLSIG_SchemaDecoder.c are inconsistent"
#endif

#if (!defined (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET))
# if (defined (EXI_ENABLE_XMLSIG_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET   EXI_ENABLE_XMLSIG_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET == STD_ON))

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeAlgorithm
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeAlgorithmType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeAlgorithm( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeAlgorithmType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeAlgorithmType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeEncoding
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeEncodingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeEncoding( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeEncodingType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeEncodingType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeId
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeIdType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeId( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeIdType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeMime
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeMimeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeMime( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeMimeType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeMimeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeTarget
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeTargetType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeTarget( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeTargetType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeTargetType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Attribute
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Attribute( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeURI
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeURIType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeURI( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_AttributeURIType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_AttributeURIType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  structPtr->Length = sizeof(structPtr->Buffer);
  Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_CanonicalizationMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_CanonicalizationMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_CanonicalizationMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_CanonicalizationMethodType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_GenericElementType* lastGenericElement = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_CanonicalizationMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_CANONICALIZATION_METHOD_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Algorithm) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
    structPtr->Algorithm = (Exi_XMLSIG_AttributeAlgorithmType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeAlgorithm(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->GenericElementFlag)
  {
    structPtr->GenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastGenericElement = structPtr->GenericElement;
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_GENERICELEMENT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(GenericElement) */
      {
        lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastGenericElement = lastGenericElement->NextGenericElementPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(GenericElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_GENERICELEMENT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->GenericElementFlag)
    {
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* last element unbounded, EE(CanonicalizationMethod) already found, EXI stream end reached */
      }
      else
      {
        /* unexpected state */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
      }
    }
    else
    {
      /* EE(CanonicalizationMethod) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_CryptoBinary
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_CryptoBinaryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_CryptoBinary( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_CryptoBinaryType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)
  {
    structPtr->Length = sizeof(structPtr->Buffer);
    Exi_VBSDecodeBytes(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DSAKeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DSAKeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DSAKeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_DSAKeyValueType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_DSAKeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_DSAKEY_VALUE_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(P) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->P = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(P) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Q) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->Q = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Q) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(G) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->G = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    structPtr->GFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->GFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(G) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(0 == structPtr->GFlag)
  {
    /* SE(Y) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->Y = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Y) */
    {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
      structPtr->Y = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Y) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(J) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->J = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    structPtr->JFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->JFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(J) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(0 == structPtr->JFlag)
  {
    /* SE(Seed) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->Seed = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Seed) */
    {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
      structPtr->Seed = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Seed) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(PgenCounter) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->PgenCounter = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(PgenCounter) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(DSAKeyValue) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DigestMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DigestMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DigestMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_DigestMethodType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_GenericElementType* lastGenericElement = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_DigestMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_DIGEST_METHOD_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Algorithm) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
    structPtr->Algorithm = (Exi_XMLSIG_AttributeAlgorithmType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeAlgorithm(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->GenericElementFlag)
  {
    structPtr->GenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastGenericElement = structPtr->GenericElement;
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_GENERICELEMENT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(GenericElement) */
      {
        lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastGenericElement = lastGenericElement->NextGenericElementPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(GenericElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_GENERICELEMENT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->GenericElementFlag)
    {
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* last element unbounded, EE(DigestMethod) already found, EXI stream end reached */
      }
      else
      {
        /* unexpected state */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
      }
    }
    else
    {
      /* EE(DigestMethod) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DigestValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DigestValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DigestValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_DigestValueType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_DigestValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)
  {
    structPtr->Length = sizeof(structPtr->Buffer);
    Exi_VBSDecodeBytes(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_GenericElement
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_GenericElementType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_GenericElement( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_GenericElementType* structPtr;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_KeyInfoType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_KeyInfoChoiceType* lastChoiceElement = (Exi_XMLSIG_KeyInfoChoiceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_KeyInfoType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_KEY_INFO_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 4);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(9 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 9, 4, FALSE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(KeyName) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 2 == ExiEventCode) /* SE(KeyValue) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 3 == ExiEventCode) /* SE(RetrievalMethod) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 4 == ExiEventCode) /* SE(X509Data) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 5 == ExiEventCode) /* SE(PGPData) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 6 == ExiEventCode) /* SE(SPKIData) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 7 == ExiEventCode) /* SE(MgmtData) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
    else if( 8 == ExiEventCode) /* SE(GenericElement) */
    {
      /* This is an EXI choice element. Event code is evalulated later, do not set Flag here because buffer check and ChoiceElement initialization is required first */
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* Start of Choice Element */
    if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      structPtr->ChoiceElement = (Exi_XMLSIG_KeyInfoChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
    if(1 == ExiEventCode) /* SE(KeyName) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.KeyName = (Exi_XMLSIG_KeyNameType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyName(DecWsPtr);
      structPtr->ChoiceElement->KeyNameFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)) */
    }
    else if(2 == ExiEventCode) /* SE(KeyValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.KeyValue = (Exi_XMLSIG_KeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyValue(DecWsPtr);
      structPtr->ChoiceElement->KeyValueFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)) */
    }
    else if(3 == ExiEventCode) /* SE(RetrievalMethod) */
    {
      #if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.RetrievalMethod = (Exi_XMLSIG_RetrievalMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_RetrievalMethod(DecWsPtr);
      structPtr->ChoiceElement->RetrievalMethodFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
    }
    else if(4 == ExiEventCode) /* SE(X509Data) */
    {
      #if (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.X509Data = (Exi_XMLSIG_X509DataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_X509Data(DecWsPtr);
      structPtr->ChoiceElement->X509DataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON)) */
    }
    else if(5 == ExiEventCode) /* SE(PGPData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.PGPData = (Exi_XMLSIG_PGPDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_PGPData(DecWsPtr);
      structPtr->ChoiceElement->PGPDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON)) */
    }
    else if(6 == ExiEventCode) /* SE(SPKIData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.SPKIData = (Exi_XMLSIG_SPKIDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_SPKIData(DecWsPtr);
      structPtr->ChoiceElement->SPKIDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)) */
    }
    else if(7 == ExiEventCode) /* SE(MgmtData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.MgmtData = (Exi_XMLSIG_MgmtDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_MgmtData(DecWsPtr);
      structPtr->ChoiceElement->MgmtDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)) */
    }
    else if(8 == ExiEventCode) /* SE(GenericElement) */
    {
      #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      structPtr->ChoiceElement->GenericElementFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* Start of Choice Element */
    if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      structPtr->ChoiceElement = (Exi_XMLSIG_KeyInfoChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 4);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(8 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 8, 4, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(KeyName) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.KeyName = (Exi_XMLSIG_KeyNameType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyName(DecWsPtr);
      structPtr->ChoiceElement->KeyNameFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)) */
    }
    else if(1 == ExiEventCode) /* SE(KeyValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.KeyValue = (Exi_XMLSIG_KeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyValue(DecWsPtr);
      structPtr->ChoiceElement->KeyValueFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)) */
    }
    else if(2 == ExiEventCode) /* SE(RetrievalMethod) */
    {
      #if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.RetrievalMethod = (Exi_XMLSIG_RetrievalMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_RetrievalMethod(DecWsPtr);
      structPtr->ChoiceElement->RetrievalMethodFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
    }
    else if(3 == ExiEventCode) /* SE(X509Data) */
    {
      #if (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.X509Data = (Exi_XMLSIG_X509DataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_X509Data(DecWsPtr);
      structPtr->ChoiceElement->X509DataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON)) */
    }
    else if(4 == ExiEventCode) /* SE(PGPData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.PGPData = (Exi_XMLSIG_PGPDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_PGPData(DecWsPtr);
      structPtr->ChoiceElement->PGPDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON)) */
    }
    else if(5 == ExiEventCode) /* SE(SPKIData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.SPKIData = (Exi_XMLSIG_SPKIDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_SPKIData(DecWsPtr);
      structPtr->ChoiceElement->SPKIDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)) */
    }
    else if(6 == ExiEventCode) /* SE(MgmtData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.MgmtData = (Exi_XMLSIG_MgmtDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_MgmtData(DecWsPtr);
      structPtr->ChoiceElement->MgmtDataFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)) */
    }
    else if(7 == ExiEventCode) /* SE(GenericElement) */
    {
      #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      structPtr->ChoiceElement->GenericElementFlag = 1;
      #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  structPtr->ChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_KeyInfoChoiceType*)NULL_PTR;
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(ChoiceElement) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastChoiceElement = structPtr->ChoiceElement;
  #if ((EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 4);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(9 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 9, 4, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(8 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    /* Start of Choice Element */
    if(sizeof(*lastChoiceElement->NextChoiceElementPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_KeyInfoChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*lastChoiceElement->NextChoiceElementPtr));
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    DecWsPtr->OutputData.StorageOffset += sizeof(*lastChoiceElement->NextChoiceElementPtr);
    if(0 == ExiEventCode) /* SE(KeyName) */
    {
    #if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.KeyName = (Exi_XMLSIG_KeyNameType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyName(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->KeyNameFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && EXI_DECODE_XMLSIG_KEY_NAME == STD_ON) */
    }
    else if(1 == ExiEventCode) /* SE(KeyValue) */
    {
    #if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.KeyValue = (Exi_XMLSIG_KeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_KeyValue(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->KeyValueFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON) */
    }
    else if(2 == ExiEventCode) /* SE(RetrievalMethod) */
    {
    #if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.RetrievalMethod = (Exi_XMLSIG_RetrievalMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_RetrievalMethod(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->RetrievalMethodFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON) */
    }
    else if(3 == ExiEventCode) /* SE(X509Data) */
    {
    #if (defined(EXI_DECODE_XMLSIG_X509DATA) && EXI_DECODE_XMLSIG_X509DATA == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509Data = (Exi_XMLSIG_X509DataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_X509Data(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509DataFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && EXI_DECODE_XMLSIG_X509DATA == STD_ON) */
    }
    else if(4 == ExiEventCode) /* SE(PGPData) */
    {
    #if (defined(EXI_DECODE_XMLSIG_PGPDATA) && EXI_DECODE_XMLSIG_PGPDATA == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.PGPData = (Exi_XMLSIG_PGPDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_PGPData(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->PGPDataFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && EXI_DECODE_XMLSIG_PGPDATA == STD_ON) */
    }
    else if(5 == ExiEventCode) /* SE(SPKIData) */
    {
    #if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.SPKIData = (Exi_XMLSIG_SPKIDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_SPKIData(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->SPKIDataFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && EXI_DECODE_XMLSIG_SPKIDATA == STD_ON) */
    }
    else if(6 == ExiEventCode) /* SE(MgmtData) */
    {
    #if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.MgmtData = (Exi_XMLSIG_MgmtDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_MgmtData(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->MgmtDataFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON) */
    }
    else if(7 == ExiEventCode) /* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->GenericElementFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON) */
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* EE(ChoiceElement) already decoded */
      DecWsPtr->DecWs.EERequired = TRUE;
      continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE - 1) > 0) */
  lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_KeyInfoChoiceType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 4);
    if(8 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(KeyInfo) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_KeyNameType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_NAME, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_KeyNameType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)
  {
    structPtr->Length = sizeof(structPtr->Buffer);
    Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_KeyValueType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_KeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_KEY_VALUE_TYPE;
  }
  /* Start of Choice Element */
  if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr->ChoiceElement = (Exi_XMLSIG_KeyValueChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(3 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(DSAKeyValue) */
  {
    #if (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.DSAKeyValue = (Exi_XMLSIG_DSAKeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_DSAKeyValue(DecWsPtr);
    structPtr->ChoiceElement->DSAKeyValueFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */
  }
  else if(1 == ExiEventCode) /* SE(RSAKeyValue) */
  {
    #if (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.RSAKeyValue = (Exi_XMLSIG_RSAKeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_RSAKeyValue(DecWsPtr);
    structPtr->ChoiceElement->RSAKeyValueFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */
  }
  else if(2 == ExiEventCode) /* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->ChoiceElement->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_KEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(ChoiceElement) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(KeyValue) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Manifest
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ManifestType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Manifest( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_ManifestType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_ReferenceType* lastReference = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_ManifestType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_MANIFEST_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* SE(Reference) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
    structPtr->Reference = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Reference(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Reference) */
    {
    #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
      structPtr->Reference = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Reference(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MANIFEST, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  structPtr->Reference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Reference) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastReference = structPtr->Reference;
  #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
  #if ((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_REFERENCE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Reference) */
    {
      lastReference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Reference(DecWsPtr);
      lastReference = lastReference->NextReferencePtr;
    }
    else if(1 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(Reference) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) > 0) */
  #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
  lastReference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    if(1 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(Manifest) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_MgmtData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_MgmtDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_MgmtData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_MgmtDataType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_MGMT_DATA, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_MgmtDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)
  {
    structPtr->Length = sizeof(structPtr->Buffer);
    Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Object
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ObjectType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Object( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_ObjectType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_ObjectType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_OBJECT_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(4 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 4, 3, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Encoding) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON))
    structPtr->Encoding = (Exi_XMLSIG_AttributeEncodingType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeEncoding(DecWsPtr);
    structPtr->EncodingFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(Id) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->IdFlag = 1;
    }
    else if( 2 == ExiEventCode) /* SE(MimeType) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->MimeTypeFlag = 1;
    }
    else if( 3 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if((0 == structPtr->EncodingFlag) && (1 == structPtr->IdFlag))
  {
    /* SE(Id) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
  else if(1 == structPtr->EncodingFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(3 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 3, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(Id) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
      structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
      structPtr->IdFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
    }
      else if( 1 == ExiEventCode) /* SE(MimeType) */
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
        structPtr->MimeTypeFlag = 1;
      }
      else if( 2 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if((0 == structPtr->IdFlag) && (1 == structPtr->MimeTypeFlag))
  {
    /* SE(MimeType) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
    structPtr->MimeType = (Exi_XMLSIG_AttributeMimeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeMime(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */
  }
  else if(1 == structPtr->IdFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(MimeType) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
      structPtr->MimeType = (Exi_XMLSIG_AttributeMimeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_AttributeMime(DecWsPtr);
      structPtr->MimeTypeFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(0 == structPtr->MimeTypeFlag)
  {
    /* SE(GenericElement) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_OBJECT, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(GenericElement) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(Object) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPDataChoiceSeq0
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq0Type object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPDataChoiceSeq0( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_PGPDataChoiceSeq0Type* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  Exi_XMLSIG_GenericElementType* lastGenericElement = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_PGPDataChoiceSeq0Type*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(PGPKeyID) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->PGPKeyID = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(PGPKeyID) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(3 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(PGPKeyPacket) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->PGPKeyPacket = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    structPtr->PGPKeyPacketFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(GenericElement) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->GenericElementFlag = 1;
    }
    else if( 2 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->PGPKeyPacketFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(PGPKeyPacket) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if((0 == structPtr->PGPKeyPacketFlag) && (1 == structPtr->GenericElementFlag))
  {
    /* SE(GenericElement) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else if(1 == structPtr->PGPKeyPacketFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      structPtr->GenericElementFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(1 == structPtr->GenericElementFlag)
  {
    structPtr->GenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastGenericElement = structPtr->GenericElement;
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_GENERICELEMENT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(GenericElement) */
      {
        lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastGenericElement = lastGenericElement->NextGenericElementPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(GenericElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_GENERICELEMENT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPDataChoiceSeq1
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq1Type object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPDataChoiceSeq1( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_PGPDataChoiceSeq1Type* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  Exi_XMLSIG_GenericElementType* lastGenericElement = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_PGPDataChoiceSeq1Type*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(PGPKeyPacket) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->PGPKeyPacket = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(PGPKeyPacket) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->GenericElementFlag)
  {
    structPtr->GenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastGenericElement = structPtr->GenericElement;
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_GENERICELEMENT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(GenericElement) */
      {
        lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastGenericElement = lastGenericElement->NextGenericElementPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(GenericElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_GENERICELEMENT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_PGPDataType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_PGPDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_PGPDATA_TYPE;
  }
  /* Start of Choice Element */
  if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr->ChoiceElement = (Exi_XMLSIG_PGPDataChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(ChoiceSequence0) */
  {
    #if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.ChoiceSequence0 = (Exi_XMLSIG_PGPDataChoiceSeq0Type*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    DecWsPtr->DecWs.EERequired = TRUE;
    Exi_Decode_XMLSIG_PGPDataChoiceSeq0(DecWsPtr);
    structPtr->ChoiceElement->ChoiceSequence0Flag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON)) */
  }
  else if(1 == ExiEventCode) /* SE(ChoiceSequence1) */
  {
    #if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.ChoiceSequence1 = (Exi_XMLSIG_PGPDataChoiceSeq1Type*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    DecWsPtr->DecWs.EERequired = TRUE;
    Exi_Decode_XMLSIG_PGPDataChoiceSeq1(DecWsPtr);
    structPtr->ChoiceElement->ChoiceSequence1Flag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_PGPDATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(ChoiceElement) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(PGPData) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_RSAKeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_RSAKeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_RSAKeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_RSAKeyValueType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_RSAKeyValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_RSAKEY_VALUE_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Modulus) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->Modulus = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Modulus) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Exponent) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
    structPtr->Exponent = (Exi_XMLSIG_CryptoBinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CryptoBinary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Exponent) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(RSAKeyValue) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Reference
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ReferenceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Reference( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_ReferenceType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_REFERENCE_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(5 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 5, 3, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(Type) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->TypeFlag = 1;
    }
    else if( 2 == ExiEventCode) /* SE(URI) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->URIFlag = 1;
    }
    else if( 3 == ExiEventCode) /* SE(Transforms) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->TransformsFlag = 1;
    }
    else if( 4 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if((0 == structPtr->IdFlag) && (1 == structPtr->TypeFlag))
  {
    /* SE(Type) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON))
    structPtr->Type = (Exi_XMLSIG_AttributeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Attribute(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON)) */
  }
  else if(1 == structPtr->IdFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(4 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 4, 3, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(Type) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON))
      structPtr->Type = (Exi_XMLSIG_AttributeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Attribute(DecWsPtr);
      structPtr->TypeFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON)) */
    }
      else if( 1 == ExiEventCode) /* SE(URI) */
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
        structPtr->URIFlag = 1;
      }
      else if( 2 == ExiEventCode) /* SE(Transforms) */
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
        structPtr->TransformsFlag = 1;
      }
      else if( 3 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if((0 == structPtr->TypeFlag) && (1 == structPtr->URIFlag))
  {
    /* SE(URI) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
    structPtr->URI = (Exi_XMLSIG_AttributeURIType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeURI(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
  }
  else if(1 == structPtr->TypeFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(3 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, FALSE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(URI) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
      structPtr->URI = (Exi_XMLSIG_AttributeURIType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_AttributeURI(DecWsPtr);
      structPtr->URIFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
    }
      else if( 1 == ExiEventCode) /* SE(Transforms) */
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
        structPtr->TransformsFlag = 1;
      }
      else if( 2 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if((0 == structPtr->URIFlag) && (1 == structPtr->TransformsFlag))
  {
    /* SE(Transforms) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->Transforms = (Exi_XMLSIG_TransformsType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Transforms(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */
  }
  else if(1 == structPtr->URIFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, TRUE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Transforms) */
    {
    #if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->Transforms = (Exi_XMLSIG_TransformsType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Transforms(DecWsPtr);
      structPtr->TransformsFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(1 == structPtr->TransformsFlag)
  {
    if(TRUE == DecWsPtr->DecWs.EERequired)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      /* check ExiEventCode equals EE(Transforms) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    else
    {
      /* EE(Transforms) already decoded in subfunction call */
      DecWsPtr->DecWs.EERequired = TRUE;
    }
  }
  if(0 == structPtr->TransformsFlag)
  {
    /* SE(DigestMethod) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->DigestMethod = (Exi_XMLSIG_DigestMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_DigestMethod(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(DigestMethod) */
    {
    #if (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->DigestMethod = (Exi_XMLSIG_DigestMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_DigestMethod(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(DigestMethod) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(DigestMethod) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 3, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(DigestValue) */
  {
    #if (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON))
    structPtr->DigestValue = (Exi_XMLSIG_DigestValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_DigestValue(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_REFERENCE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(DigestValue) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(Reference) */
    {
    /* EXI stream end reached */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_RetrievalMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_RetrievalMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_RetrievalMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_RetrievalMethodType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_RetrievalMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_RETRIEVAL_METHOD_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(4 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 4, 3, FALSE, TRUE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Type) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON))
    structPtr->Type = (Exi_XMLSIG_AttributeType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Attribute(DecWsPtr);
    structPtr->TypeFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(URI) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->URIFlag = 1;
    }
    else if( 2 == ExiEventCode) /* SE(Transforms) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->TransformsFlag = 1;
    }
    else if( 3 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if((0 == structPtr->TypeFlag) && (1 == structPtr->URIFlag))
  {
    /* SE(URI) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
    structPtr->URI = (Exi_XMLSIG_AttributeURIType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeURI(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
  }
  else if(1 == structPtr->TypeFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(3 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, FALSE, TRUE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(URI) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
      structPtr->URI = (Exi_XMLSIG_AttributeURIType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_AttributeURI(DecWsPtr);
      structPtr->URIFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */
    }
      else if( 1 == ExiEventCode) /* SE(Transforms) */
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
        structPtr->TransformsFlag = 1;
      }
      else if( 2 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if((0 == structPtr->URIFlag) && (1 == structPtr->TransformsFlag))
  {
    /* SE(Transforms) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->Transforms = (Exi_XMLSIG_TransformsType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Transforms(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */
  }
  else if(1 == structPtr->URIFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Transforms) */
    {
    #if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->Transforms = (Exi_XMLSIG_TransformsType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Transforms(DecWsPtr);
      structPtr->TransformsFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(1 == structPtr->TransformsFlag)
  {
    if(TRUE == DecWsPtr->DecWs.EERequired)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      /* check ExiEventCode equals EE(Transforms) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    else
    {
      /* EE(Transforms) already decoded in subfunction call */
      DecWsPtr->DecWs.EERequired = TRUE;
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->TransformsFlag)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* EE(RetrievalMethod) */
      {
        /* EXI stream end reached */
      }
      else
      {
        /* unsupported event code */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    else
    {
      /* EE(RetrievalMethod) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SPKIData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SPKIDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SPKIData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SPKIDataType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SPKIDataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SPKIDATA_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(SPKISexp) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->SPKISexp = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(SPKISexp) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SPKIDATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->GenericElementFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->GenericElementFlag)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* EE(SPKIData) */
      {
        /* EXI stream end reached */
      }
      else
      {
        /* unsupported event code */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    else
    {
      /* EE(SPKIData) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignatureMethodType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_GenericElementType* lastGenericElement = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignatureMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SIGNATURE_METHOD_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Algorithm) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
    structPtr->Algorithm = (Exi_XMLSIG_AttributeAlgorithmType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeAlgorithm(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(3 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 3, TRUE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(HMACOutputLength) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, TRUE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
        structPtr->HMACOutputLengthFlag = 1;
      if(sizeof(*structPtr->HMACOutputLength) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
      {
        /* struct fits into remaining storage buffer */
        structPtr->HMACOutputLength = (Exi_BigIntType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->HMACOutputLength));
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
        }
        return;
      }
      DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->HMACOutputLength);
      Exi_VBSDecodeBigInt(&DecWsPtr->DecWs, structPtr->HMACOutputLength);
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
    else if( 1 == ExiEventCode) /* SE(GenericElement) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->GenericElementFlag = 1;
    }
    else if( 2 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->HMACOutputLengthFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(HMACOutputLength) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if((0 == structPtr->HMACOutputLengthFlag) && (1 == structPtr->GenericElementFlag))
  {
    /* SE(GenericElement) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else if(1 == structPtr->HMACOutputLengthFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
      structPtr->GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      structPtr->GenericElementFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(1 == structPtr->GenericElementFlag)
  {
    structPtr->GenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(GenericElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastGenericElement = structPtr->GenericElement;
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_GENERICELEMENT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(GenericElement) */
      {
        lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastGenericElement = lastGenericElement->NextGenericElementPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(GenericElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_GENERICELEMENT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
    lastGenericElement->NextGenericElementPtr = (Exi_XMLSIG_GenericElementType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_GENERICELEMENT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->GenericElementFlag)
    {
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* last element unbounded, EE(SignatureMethod) already found, EXI stream end reached */
      }
      else
      {
        /* unexpected state */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
      }
    }
    else
    {
      /* EE(SignatureMethod) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureProperties
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignaturePropertiesType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureProperties( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignaturePropertiesType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_SignaturePropertyType* lastSignatureProperty = (Exi_XMLSIG_SignaturePropertyType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignaturePropertiesType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SIGNATURE_PROPERTIES_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* SE(SignatureProperty) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->SignatureProperty = (Exi_XMLSIG_SignaturePropertyType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_SignatureProperty(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(SignatureProperty) */
    {
    #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->SignatureProperty = (Exi_XMLSIG_SignaturePropertyType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_SignatureProperty(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  structPtr->SignatureProperty->NextSignaturePropertyPtr = (Exi_XMLSIG_SignaturePropertyType*)NULL_PTR;
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(SignatureProperty) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(SignatureProperty) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastSignatureProperty = structPtr->SignatureProperty;
  #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
  #if ((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(SignatureProperty) */
    {
      lastSignatureProperty->NextSignaturePropertyPtr = (Exi_XMLSIG_SignaturePropertyType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_SignatureProperty(DecWsPtr);
      lastSignatureProperty = lastSignatureProperty->NextSignaturePropertyPtr;
    }
    else if(1 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* EE(SignatureProperty) already decoded */
      DecWsPtr->DecWs.EERequired = TRUE;
      continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(SignatureProperty) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY - 1) > 0) */
  #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
  lastSignatureProperty->NextSignaturePropertyPtr = (Exi_XMLSIG_SignaturePropertyType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    if(1 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(SignatureProperties) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureProperty
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignaturePropertyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureProperty( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignaturePropertyType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_SignaturePropertyChoiceType* lastChoiceElement = (Exi_XMLSIG_SignaturePropertyChoiceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignaturePropertyType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SIGNATURE_PROPERTY_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* SE(Target) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
    structPtr->Target = (Exi_XMLSIG_AttributeTargetType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeTarget(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* AT(Target) */
    {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
      structPtr->Target = (Exi_XMLSIG_AttributeTargetType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_AttributeTarget(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  /* Start of Choice Element */
  if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr->ChoiceElement = (Exi_XMLSIG_SignaturePropertyChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 2, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->ChoiceElement->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  structPtr->ChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*)NULL_PTR;
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(ChoiceElement) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastChoiceElement = structPtr->ChoiceElement;
  #if ((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(1 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    /* Start of Choice Element */
    if(sizeof(*lastChoiceElement->NextChoiceElementPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*lastChoiceElement->NextChoiceElementPtr));
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    DecWsPtr->OutputData.StorageOffset += sizeof(*lastChoiceElement->NextChoiceElementPtr);
    if(0 == ExiEventCode) /* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->GenericElementFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON) */
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* EE(ChoiceElement) already decoded */
      DecWsPtr->DecWs.EERequired = TRUE;
      continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE - 1) > 0) */
  lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_SignaturePropertyChoiceType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    if(1 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(SignatureProperty) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Signature
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Signature( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignatureType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_ObjectType* lastObject = (Exi_XMLSIG_ObjectType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignatureType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SIGNATURE_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* SE(SignedInfo) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->SignedInfo = (Exi_XMLSIG_SignedInfoType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_SignedInfo(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(SignedInfo) */
    {
    #if (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->SignedInfo = (Exi_XMLSIG_SignedInfoType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_SignedInfo(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(SignedInfo) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(SignedInfo) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(SignatureValue) */
  {
    #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
    structPtr->SignatureValue = (Exi_XMLSIG_SignatureValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_SignatureValue(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(SignatureValue) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(3 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, TRUE, TRUE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(KeyInfo) */
  {
    #if (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->KeyInfo = (Exi_XMLSIG_KeyInfoType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_KeyInfo(DecWsPtr);
    structPtr->KeyInfoFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON)) */
  }
    else if( 1 == ExiEventCode) /* SE(Object) */
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      structPtr->ObjectFlag = 1;
    }
    else if( 2 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->KeyInfoFlag)
  {
    if(TRUE == DecWsPtr->DecWs.EERequired)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      /* check ExiEventCode equals EE(KeyInfo) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    else
    {
      /* EE(KeyInfo) already decoded in subfunction call */
      DecWsPtr->DecWs.EERequired = TRUE;
    }
  }
  if((0 == structPtr->KeyInfoFlag) && (1 == structPtr->ObjectFlag))
  {
    /* SE(Object) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
    structPtr->Object = (Exi_XMLSIG_ObjectType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Object(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */
  }
  else if(1 == structPtr->KeyInfoFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Object) */
    {
    #if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
      structPtr->Object = (Exi_XMLSIG_ObjectType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Object(DecWsPtr);
      structPtr->ObjectFlag = 1;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */
    }
      else if( 1 == ExiEventCode)
      {
        /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
      }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* nothing to do */
  }
  if(1 == structPtr->ObjectFlag)
  {
    structPtr->Object->NextObjectPtr = (Exi_XMLSIG_ObjectType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(Object) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastObject = structPtr->Object;
    #if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
    #if ((EXI_MAXOCCURS_XMLSIG_OBJECT - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_XMLSIG_OBJECT - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(2 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* SE(Object) */
      {
        lastObject->NextObjectPtr = (Exi_XMLSIG_ObjectType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_Object(DecWsPtr);
        lastObject = lastObject->NextObjectPtr;
      }
      else if(1 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(Object) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_XMLSIG_OBJECT - 1) > 0) */
    #endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */
    lastObject->NextObjectPtr = (Exi_XMLSIG_ObjectType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_XMLSIG_OBJECT - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(1 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->ObjectFlag)
    {
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* last element unbounded, EE(Signature) already found, EXI stream end reached */
      }
      else
      {
        /* unexpected state */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
      }
    }
    else
    {
      /* EE(Signature) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignatureValueType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignatureValueType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  if(1 == structPtr->IdFlag)
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  }
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)
  {
    structPtr->Length = sizeof(structPtr->Buffer);
    Exi_VBSDecodeBytes(&DecWsPtr->DecWs, &structPtr->Buffer[0], &structPtr->Length);
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignedInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignedInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignedInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_SignedInfoType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_ReferenceType* lastReference = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_SignedInfoType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_SIGNED_INFO_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(2 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Id) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
    structPtr->Id = (Exi_XMLSIG_AttributeIdType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeId(DecWsPtr);
    structPtr->IdFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */
  }
    else if( 1 == ExiEventCode)
    {
      /* Optional element not included. Setting Flag to 0 is not requiered because of clear memory call at the beginning of this function */
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(0 == structPtr->IdFlag)
  {
    /* SE(CanonicalizationMethod) already decoded */
  #if (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->CanonicalizationMethod = (Exi_XMLSIG_CanonicalizationMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_CanonicalizationMethod(DecWsPtr);
  #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
  #endif /* (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(CanonicalizationMethod) */
    {
    #if (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
      DecWsPtr->DecWs.EERequired = TRUE;
      structPtr->CanonicalizationMethod = (Exi_XMLSIG_CanonicalizationMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_CanonicalizationMethod(DecWsPtr);
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
    }
    else
    {
      /* unsupported event code */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(CanonicalizationMethod) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(CanonicalizationMethod) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(SignatureMethod) */
  {
    #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->SignatureMethod = (Exi_XMLSIG_SignatureMethodType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_SignatureMethod(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(SignatureMethod) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(SignatureMethod) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Reference) */
  {
    #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
    structPtr->Reference = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Reference(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  structPtr->Reference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Reference) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastReference = structPtr->Reference;
  #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
  #if ((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_REFERENCE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Reference) */
    {
      lastReference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_Reference(DecWsPtr);
      lastReference = lastReference->NextReferencePtr;
    }
    else if(1 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(Reference) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) > 0) */
  #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
  lastReference->NextReferencePtr = (Exi_XMLSIG_ReferenceType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_REFERENCE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    if(1 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(SignedInfo) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Transform
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_TransformType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Transform( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_TransformType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_TransformChoiceType* lastChoiceElement = (Exi_XMLSIG_TransformChoiceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_TransformType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_TRANSFORM_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* AT(Algorithm) */
  {
    #if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON))
    structPtr->Algorithm = (Exi_XMLSIG_AttributeAlgorithmType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_AttributeAlgorithm(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  /* Start of Choice Element */
  if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr->ChoiceElement = (Exi_XMLSIG_TransformChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(3 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 3, TRUE, TRUE, TRUE, 1, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->ChoiceElement->GenericElementFlag = 1;
    structPtr->ChoiceElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else if(1 == ExiEventCode) /* SE(XPath) */
  {
    #if (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.XPath = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_string(DecWsPtr);
    structPtr->ChoiceElement->XPathFlag = 1;
    structPtr->ChoiceElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON)) */
  }
    else if(2 == ExiEventCode)
    {
      structPtr->ChoiceElementFlag = 0;
    }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(1 == structPtr->ChoiceElementFlag)
  {
    structPtr->ChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_TransformChoiceType*)NULL_PTR;
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
    {
      /* an error occured in a previous step -> abort to avoid not required loop */
      return;
    }
    lastChoiceElement = structPtr->ChoiceElement;
    #if ((EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE - 1) > 0)
    for(i=0; i<(EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
    { /* PRQA S 3201 */ /* MD_MSR_14.1 */
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(3 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 3, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
      }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(2 == ExiEventCode)/* reached next Tag */
      {
        DecWsPtr->DecWs.EERequired = FALSE;
        break;
      }
      /* Start of Choice Element */
      if(sizeof(*lastChoiceElement->NextChoiceElementPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
      {
        /* struct fits into remaining storage buffer */
        lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_TransformChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*lastChoiceElement->NextChoiceElementPtr));
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
        }
        return;
      }
      DecWsPtr->OutputData.StorageOffset += sizeof(*lastChoiceElement->NextChoiceElementPtr);
      if(0 == ExiEventCode) /* SE(GenericElement) */
      {
      #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)
        lastChoiceElement->NextChoiceElementPtr->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
        lastChoiceElement->NextChoiceElementPtr->GenericElementFlag = 1;
        lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
      #else
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
        }
        return;
      #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON) */
      }
      else if(1 == ExiEventCode) /* SE(XPath) */
      {
      #if (defined(EXI_DECODE_STRING) && EXI_DECODE_STRING == STD_ON)
        lastChoiceElement->NextChoiceElementPtr->ChoiceValue.XPath = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_Decode_string(DecWsPtr);
        lastChoiceElement->NextChoiceElementPtr->XPathFlag = 1;
        lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
      #else
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
        Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORM, EXI_E_INV_PARAM);
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
        }
        return;
      #endif /* (defined(EXI_DECODE_STRING) && EXI_DECODE_STRING == STD_ON) */
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          /* unsupported event code */
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* EE(ChoiceElement) already decoded */
        DecWsPtr->DecWs.EERequired = TRUE;
        continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
      }
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      /* check ExiEventCode equals EE(ChoiceElement) */
      if(0 != ExiEventCode)
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
    #endif /* #if ((EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE - 1) > 0) */
    lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_TransformChoiceType*)NULL_PTR;
    ExiEventCode = 0;
    if((EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
      if(2 == ExiEventCode)
      {
        DecWsPtr->DecWs.EERequired = FALSE;
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        return;
      }
    }
  }
  else
  {
    /* EE encoding for calling function not required */
    DecWsPtr->DecWs.EERequired = FALSE;
  }
  if(TRUE == isFirstElement)
  {
    if(1 == structPtr->ChoiceElementFlag)
    {
      if(FALSE == DecWsPtr->DecWs.EERequired)
      {
        /* last element unbounded, EE(Transform) already found, EXI stream end reached */
      }
      else
      {
        /* unexpected state */
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
      }
    }
    else
    {
      /* EE(Transform) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Transforms
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_TransformsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Transforms( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_TransformsType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_TransformType* lastTransform = (Exi_XMLSIG_TransformType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_TransformsType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_TRANSFORMS_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Transform) */
  {
    #if (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON))
    DecWsPtr->DecWs.EERequired = TRUE;
    structPtr->Transform = (Exi_XMLSIG_TransformType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_Transform(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_TRANSFORMS, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  structPtr->Transform->NextTransformPtr = (Exi_XMLSIG_TransformType*)NULL_PTR;
  if(TRUE == DecWsPtr->DecWs.EERequired)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(Transform) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* EE(Transform) already decoded in subfunction call */
    DecWsPtr->DecWs.EERequired = TRUE;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastTransform = structPtr->Transform;
  #if (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON))
  #if ((EXI_MAXOCCURS_XMLSIG_TRANSFORM - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_TRANSFORM - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(Transform) */
    {
      lastTransform->NextTransformPtr = (Exi_XMLSIG_TransformType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      DecWsPtr->DecWs.EERequired = TRUE;
      Exi_Decode_XMLSIG_Transform(DecWsPtr);
      lastTransform = lastTransform->NextTransformPtr;
    }
    else if(1 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* EE(Transform) already decoded */
      DecWsPtr->DecWs.EERequired = TRUE;
      continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(Transform) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_TRANSFORM - 1) > 0) */
  #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON)) */
  lastTransform->NextTransformPtr = (Exi_XMLSIG_TransformType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_TRANSFORM - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    if(1 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(Transforms) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_X509Data
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_X509DataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_X509Data( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_X509DataType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_XMLSIG_X509DataChoiceType* lastChoiceElement = (Exi_XMLSIG_X509DataChoiceType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_X509DataType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  if(EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE == DecWsPtr->OutputData.RootElementId)
  {
    /* this is the first start tag in the stream */
    isFirstElement = TRUE;
    DecWsPtr->OutputData.RootElementId = EXI_XMLSIG_X509DATA_TYPE;
  }
  /* Start of Choice Element */
  if(sizeof(*structPtr->ChoiceElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr->ChoiceElement = (Exi_XMLSIG_X509DataChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->ChoiceElement));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->ChoiceElement);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(6 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 6, 3, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(X509IssuerSerial) */
  {
    #if (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.X509IssuerSerial = (Exi_XMLSIG_X509IssuerSerialType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_X509IssuerSerial(DecWsPtr);
    structPtr->ChoiceElement->X509IssuerSerialFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */
  }
  else if(1 == ExiEventCode) /* SE(X509SKI) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.X509SKI = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    structPtr->ChoiceElement->X509SKIFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else if(2 == ExiEventCode) /* SE(X509SubjectName) */
  {
    #if (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.X509SubjectName = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_string(DecWsPtr);
    structPtr->ChoiceElement->X509SubjectNameFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON)) */
  }
  else if(3 == ExiEventCode) /* SE(X509Certificate) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.X509Certificate = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    structPtr->ChoiceElement->X509CertificateFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else if(4 == ExiEventCode) /* SE(X509CRL) */
  {
    #if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.X509CRL = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_base64Binary(DecWsPtr);
    structPtr->ChoiceElement->X509CRLFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */
  }
  else if(5 == ExiEventCode) /* SE(GenericElement) */
  {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
    structPtr->ChoiceElement->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
    structPtr->ChoiceElement->GenericElementFlag = 1;
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  structPtr->ChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_X509DataChoiceType*)NULL_PTR;
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(ChoiceElement) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  if(EXI_E_OK != DecWsPtr->DecWs.StatusCode)
  {
    /* an error occured in a previous step -> abort to avoid not required loop */
    return;
  }
  lastChoiceElement = structPtr->ChoiceElement;
  #if ((EXI_MAXOCCURS_XMLSIG_X509DATACHOICE - 1) > 0)
  for(i=0; i<(EXI_MAXOCCURS_XMLSIG_X509DATACHOICE - 1); i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(7 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 7, 3, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(6 == ExiEventCode)/* reached next Tag */
    {
      DecWsPtr->DecWs.EERequired = FALSE;
      break;
    }
    /* Start of Choice Element */
    if(sizeof(*lastChoiceElement->NextChoiceElementPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_X509DataChoiceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*lastChoiceElement->NextChoiceElementPtr));
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    DecWsPtr->OutputData.StorageOffset += sizeof(*lastChoiceElement->NextChoiceElementPtr);
    if(0 == ExiEventCode) /* SE(X509IssuerSerial) */
    {
    #if (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509IssuerSerial = (Exi_XMLSIG_X509IssuerSerialType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_X509IssuerSerial(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509IssuerSerialFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON) */
    }
    else if(1 == ExiEventCode) /* SE(X509SKI) */
    {
    #if (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509SKI = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_base64Binary(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509SKIFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON) */
    }
    else if(2 == ExiEventCode) /* SE(X509SubjectName) */
    {
    #if (defined(EXI_DECODE_STRING) && EXI_DECODE_STRING == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509SubjectName = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_string(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509SubjectNameFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_STRING) && EXI_DECODE_STRING == STD_ON) */
    }
    else if(3 == ExiEventCode) /* SE(X509Certificate) */
    {
    #if (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509Certificate = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_base64Binary(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509CertificateFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON) */
    }
    else if(4 == ExiEventCode) /* SE(X509CRL) */
    {
    #if (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.X509CRL = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_base64Binary(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->X509CRLFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_BASE64BINARY) && EXI_DECODE_BASE64BINARY == STD_ON) */
    }
    else if(5 == ExiEventCode) /* SE(GenericElement) */
    {
    #if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)
      lastChoiceElement->NextChoiceElementPtr->ChoiceValue.GenericElement = (Exi_XMLSIG_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_XMLSIG_GenericElement(DecWsPtr);
      lastChoiceElement->NextChoiceElementPtr->GenericElementFlag = 1;
      lastChoiceElement = lastChoiceElement->NextChoiceElementPtr;
    #else
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509DATA, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
    #endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON) */
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* EE(ChoiceElement) already decoded */
      DecWsPtr->DecWs.EERequired = TRUE;
      continue; /* PRQA S 0770 */ /* MD_Exi_14.5 */
    }
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    /* check ExiEventCode equals EE(ChoiceElement) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* #if ((EXI_MAXOCCURS_XMLSIG_X509DATACHOICE - 1) > 0) */
  lastChoiceElement->NextChoiceElementPtr = (Exi_XMLSIG_X509DataChoiceType*)NULL_PTR;
  ExiEventCode = 0;
  if((EXI_MAXOCCURS_XMLSIG_X509DATACHOICE - 1) == i) /* PRQA S 3355,3358 */ /* MD_Exi_13.7 */
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 3);
    if(6 == ExiEventCode)
    {
      DecWsPtr->DecWs.EERequired = FALSE;
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  if(TRUE == isFirstElement)
  {
    if(FALSE == DecWsPtr->DecWs.EERequired)
    {
      /* last element unbounded, EE(X509Data) already found, EXI stream end reached */
    }
    else
    {
      /* unexpected state */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_X509IssuerSerial
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_X509IssuerSerialType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_X509IssuerSerial( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_XMLSIG_X509IssuerSerialType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_XMLSIG_X509IssuerSerialType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr));
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
    }
    return;
  }
  DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr);
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(X509IssuerName) */
  {
    #if (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON))
    structPtr->X509IssuerName = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_string(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON)) */
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(X509IssuerName) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(X509SerialNumber) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
      if(sizeof(*structPtr->X509SerialNumber) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
      {
        /* struct fits into remaining storage buffer */
        structPtr->X509SerialNumber = (Exi_BigIntType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
        Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*structPtr->X509SerialNumber));
      }
      else
      {
        if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
        }
        return;
      }
      DecWsPtr->OutputData.StorageOffset += sizeof(*structPtr->X509SerialNumber);
      Exi_VBSDecodeBigInt(&DecWsPtr->DecWs, structPtr->X509SerialNumber);
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* unsupported event code */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_XMLSIG_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(X509SerialNumber) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_XMLSIG
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_XMLSIG object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_XMLSIG) && (EXI_DECODE_SCHEMA_SET_XMLSIG == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_XMLSIG( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 5);
  if(0 == ExiEventCode)/* SE(CanonicalizationMethod) */
  {
    #if (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
    Exi_Decode_XMLSIG_CanonicalizationMethod(DecWsPtr);
    #else
    Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */
  }
    else if(1 == ExiEventCode) /* SE(DSAKeyValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON))
      Exi_Decode_XMLSIG_DSAKeyValue(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */
    }
  
    else if(2 == ExiEventCode) /* SE(DigestMethod) */
    {
      #if (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON))
      Exi_Decode_XMLSIG_DigestMethod(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON)) */
    }
  
    else if(3 == ExiEventCode) /* SE(DigestValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON))
      Exi_Decode_XMLSIG_DigestValue(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON)) */
    }
  
    else if(4 == ExiEventCode) /* SE(KeyInfo) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON))
      Exi_Decode_XMLSIG_KeyInfo(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON)) */
    }
  
    else if(5 == ExiEventCode) /* SE(KeyName) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON))
      Exi_Decode_XMLSIG_KeyName(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)) */
    }
  
    else if(6 == ExiEventCode) /* SE(KeyValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON))
      Exi_Decode_XMLSIG_KeyValue(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)) */
    }
  
    else if(7 == ExiEventCode) /* SE(Manifest) */
    {
      #if (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON))
      Exi_Decode_XMLSIG_Manifest(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON)) */
    }
  
    else if(8 == ExiEventCode) /* SE(MgmtData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON))
      Exi_Decode_XMLSIG_MgmtData(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)) */
    }
  
    else if(9 == ExiEventCode) /* SE(Object) */
    {
      #if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
      Exi_Decode_XMLSIG_Object(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */
    }
  
    else if(10 == ExiEventCode) /* SE(PGPData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON))
      Exi_Decode_XMLSIG_PGPData(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON)) */
    }
  
    else if(11 == ExiEventCode) /* SE(RSAKeyValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON))
      Exi_Decode_XMLSIG_RSAKeyValue(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */
    }
  
    else if(12 == ExiEventCode) /* SE(Reference) */
    {
      #if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
      Exi_Decode_XMLSIG_Reference(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */
    }
  
    else if(13 == ExiEventCode) /* SE(RetrievalMethod) */
    {
      #if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
      Exi_Decode_XMLSIG_RetrievalMethod(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */
    }
  
    else if(14 == ExiEventCode) /* SE(SPKIData) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON))
      Exi_Decode_XMLSIG_SPKIData(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)) */
    }
  
    else if(15 == ExiEventCode) /* SE(Signature) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON))
      Exi_Decode_XMLSIG_Signature(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON)) */
    }
  
    else if(16 == ExiEventCode) /* SE(SignatureMethod) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
      Exi_Decode_XMLSIG_SignatureMethod(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */
    }
  
    else if(17 == ExiEventCode) /* SE(SignatureProperties) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
      Exi_Decode_XMLSIG_SignatureProperties(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON)) */
    }
  
    else if(18 == ExiEventCode) /* SE(SignatureProperty) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
      Exi_Decode_XMLSIG_SignatureProperty(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */
    }
  
    else if(19 == ExiEventCode) /* SE(SignatureValue) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
      Exi_Decode_XMLSIG_SignatureValue(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */
    }
  
    else if(20 == ExiEventCode) /* SE(SignedInfo) */
    {
      #if (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON))
      Exi_Decode_XMLSIG_SignedInfo(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON)) */
    }
  
    else if(21 == ExiEventCode) /* SE(Transform) */
    {
      #if (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON))
      Exi_Decode_XMLSIG_Transform(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON)) */
    }
  
    else if(22 == ExiEventCode) /* SE(Transforms) */
    {
      #if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
      Exi_Decode_XMLSIG_Transforms(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */
    }
  
    else if(23 == ExiEventCode) /* SE(X509Data) */
    {
      #if (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON))
      Exi_Decode_XMLSIG_X509Data(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON)) */
    }
  
  else
  {
    /* unsupported event code */
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_SCHEMA_SET_XMLSIG) && (EXI_DECODE_SCHEMA_SET_XMLSIG == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */

#endif /* (defined (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET == STD_ON)) */

