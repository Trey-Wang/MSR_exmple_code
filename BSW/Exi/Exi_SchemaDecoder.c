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
 *         File:  Exi_SchemaDecoder.c
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

#define EXI_SCHEMA_DECODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_SchemaDecoder.h"
#include "Exi_BSDecoder.h"
#include "Exi_Priv.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_SchemaDecoder.c are inconsistent"
#endif

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */

#define EXI_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

CONST(Exi_DecoderFctType, EXI_CODE)Exi_DecoderFcts[7] = 
{
  #if (defined(EXI_DECODE_SCHEMA_SET_SAP) && EXI_DECODE_SCHEMA_SET_SAP == STD_ON)
  Exi_Decode_SchemaSet_SAP,
  #else
  NULL_PTR,
  #endif
  #if (defined(EXI_DECODE_SCHEMA_SET_XMLSIG) && EXI_DECODE_SCHEMA_SET_XMLSIG == STD_ON)
  Exi_Decode_SchemaSet_XMLSIG,
  #else
  NULL_PTR,
  #endif
  #if (defined(EXI_DECODE_SCHEMA_SET_ISO) && EXI_DECODE_SCHEMA_SET_ISO == STD_ON)
  Exi_Decode_SchemaSet_ISO,
  #else
  NULL_PTR,
  #endif
  #if (defined(EXI_DECODE_SCHEMA_SET_DIN) && EXI_DECODE_SCHEMA_SET_DIN == STD_ON)
  Exi_Decode_SchemaSet_DIN,
  #else
  NULL_PTR,
  #endif
  NULL_PTR,
  NULL_PTR,
  NULL_PTR,
};

CONST(uint8, EXI_CONST) Exi_UriPartitionSize[7] = 
{
  5,
  5,
  9,
  9,
  0,
  0,
  0
};

#define EXI_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Decode_base64Binary
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_base64BinaryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_base64Binary( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_base64BinaryType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_BASE64BINARY, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_base64BinaryType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
#endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Decode_string
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_stringType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_string( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_stringType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_STRING, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_stringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
#endif /* (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON)) */

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */

