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
 *         File:  Exi_SAP_SchemaDecoder.c
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


#define EXI_SAP_SCHEMA_DECODER

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_SAP_SchemaDecoder.h"
#include "Exi_BSDecoder.h"
#include "Exi_Priv.h"
/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_SAP_SchemaDecoder.c are inconsistent"
#endif

#if (!defined (EXI_ENABLE_DECODE_SAP_MESSAGE_SET))
# if (defined (EXI_ENABLE_SAP_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_SAP_MESSAGE_SET   EXI_ENABLE_SAP_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_SAP_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_SAP_MESSAGE_SET) && (EXI_ENABLE_DECODE_SAP_MESSAGE_SET == STD_ON))

/* PRQA S 0715 NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */ /* MD_Exi_1.1 */


#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_Decode_SAP_AppProtocol
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_AppProtocolType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SAP_AppProtocol( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_SAP_AppProtocolType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_SAP_AppProtocolType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(ProtocolNamespace) */
  {
    #if (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
    structPtr->ProtocolNamespace = (Exi_SAP_protocolNamespaceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_SAP_protocolNamespace(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_APP_PROTOCOL, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(ProtocolNamespace) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(VersionNumberMajor) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
      Exi_VBSDecodeUInt32(&DecWsPtr->DecWs, &structPtr->VersionNumberMajor);
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(VersionNumberMajor) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(VersionNumberMinor) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
      Exi_VBSDecodeUInt32(&DecWsPtr->DecWs, &structPtr->VersionNumberMinor);
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(VersionNumberMinor) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(SchemaID) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
      Exi_VBSDecodeUInt8(&DecWsPtr->DecWs, &structPtr->SchemaID);
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(SchemaID) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(Priority) */
  {
    Exi_BitBufType value = 0;
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &value, 5);
      structPtr->Priority = (uint8)value + 1;
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(Priority) */
  if(0 != ExiEventCode)
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
    }
    return;
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_protocolNamespace
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_protocolNamespaceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SAP_protocolNamespace( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_SAP_protocolNamespaceType* structPtr;
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_SAP_protocolNamespaceType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
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
#endif /* (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_responseCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_responseCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SAP_responseCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 12 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_RESPONSE_CODE, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(Exi_SAP_responseCodeType) >= (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* enum does not fit into remaining storage buffer */
    DecWsPtr->DecWs.StatusCode = EXI_E_NOT_OK;
    return;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  if(0 == ExiEventCode)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
    DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset] = (Exi_SAP_responseCodeType)ExiEventCode;
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
#endif /* (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_supportedAppProtocolReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_supportedAppProtocolReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SAP_supportedAppProtocolReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_SAP_supportedAppProtocolReqType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;
  Exi_SAP_AppProtocolType* lastAppProtocol = (Exi_SAP_AppProtocolType*)NULL_PTR;
  uint16 i=0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_SAP_supportedAppProtocolReqType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
    DecWsPtr->OutputData.RootElementId = EXI_SAP_SUPPORTED_APP_PROTOCOL_REQ_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(AppProtocol) */
  {
    #if (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON))
    structPtr->AppProtocol = (Exi_SAP_AppProtocolType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
    Exi_Decode_SAP_AppProtocol(DecWsPtr);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON)) */
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
  structPtr->AppProtocol->NextAppProtocolPtr = (Exi_SAP_AppProtocolType*)NULL_PTR;
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(AppProtocol) */
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
  lastAppProtocol = structPtr->AppProtocol;
  #if (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON))
  for(i=0; i<19; i++) /* PRQA S 3356,3359 */ /* MD_Exi_13.7 */
  { /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(2 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* SE(AppProtocol) */
    {
      lastAppProtocol->NextAppProtocolPtr = (Exi_SAP_AppProtocolType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      Exi_Decode_SAP_AppProtocol(DecWsPtr);
      lastAppProtocol = lastAppProtocol->NextAppProtocolPtr;
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
    /* check ExiEventCode equals EE(AppProtocol) */
    if(0 != ExiEventCode)
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      return;
    }
  }
  #endif /* (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON)) */
  lastAppProtocol->NextAppProtocolPtr = (Exi_SAP_AppProtocolType*)NULL_PTR;
  
  if(TRUE == isFirstElement)
  {
    ExiEventCode = 0;
    if(19 == i)
    {
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    }
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)/* EE(supportedAppProtocolReq) */
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
#endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_supportedAppProtocolRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_supportedAppProtocolResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SAP_supportedAppProtocolRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_SAP_supportedAppProtocolResType* structPtr;
  Exi_BitBufType ExiEventCode = 0;
  boolean isFirstElement = FALSE;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  if(sizeof(*structPtr) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
  {
    /* struct fits into remaining storage buffer */
    structPtr = (Exi_SAP_supportedAppProtocolResType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
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
    DecWsPtr->OutputData.RootElementId = EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE;
  }
  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
  if(1 == ExiEventCode)
  {
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, FALSE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(ResponseCode) */
  {
    #if (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON))
    Exi_Decode_SAP_responseCode(DecWsPtr);
    structPtr->ResponseCode = (Exi_SAP_responseCodeType)(DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]);
    #else
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES, EXI_E_INV_PARAM);
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON)) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  /* check ExiEventCode equals EE(ResponseCode) */
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
    Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 2, 2, TRUE, TRUE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
  }
  #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
  if(0 == ExiEventCode)/* SE(SchemaID) */
  {
    /* read start content */
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, TRUE, TRUE, TRUE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    if(0 == ExiEventCode)
    {
        structPtr->SchemaIDFlag = 1;
      Exi_VBSDecodeUInt8(&DecWsPtr->DecWs, &structPtr->SchemaID);
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
  if(1 == structPtr->SchemaIDFlag)
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
    if(1 == ExiEventCode)
    {
      Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
    }
    #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
    /* check ExiEventCode equals EE(SchemaID) */
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
    if(1 == structPtr->SchemaIDFlag)
    {
      ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
      #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
      if(1 == ExiEventCode)
      {
        Exi_VBSDecodeSchemaDeviation(DecWsPtr, &ExiEventCode, 1, 1, FALSE, FALSE, FALSE, 0, EXI_SCHEMA_SET_SAP_TYPE);
      }
      #endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */
      if(0 == ExiEventCode)/* EE(supportedAppProtocolRes) */
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
      /* EE(supportedAppProtocolRes) already found, EXI stream end reached */
    }
  }
  
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_SAP
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_SAP object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_SAP) && (EXI_DECODE_SCHEMA_SET_SAP == STD_ON))
FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_SAP( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr )
{
  Exi_BitBufType ExiEventCode = 0;

  #if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
  /* ------------------------------------------------- */
  /* DET: */
  /* PRQA S 3109 13 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid((EXI_STATE_UNINIT != Exi_State), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_NOT_INITIALIZED);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->OutputData.StoragePtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_POINTER);
  #if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.PBufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_POINTER);
  #else
  Exi_CheckInternalDetErrorReturnVoid((NULL_PTR != DecWsPtr->DecWs.BufPtr), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->DecWs.BytePos < DecWsPtr->DecWs.BufLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
  #endif
  Exi_CheckInternalDetErrorReturnVoid((EXI_UNKNOWN_SCHEMA_SET_TYPE != DecWsPtr->OutputData.SchemaSetId), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((0 != DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid((DecWsPtr->OutputData.StorageOffset < DecWsPtr->OutputData.StorageLen), EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
  /* ------------------------------------------------- */
  #endif /* (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON) */

  ExiEventCode = 0;
  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 2);
  if(0 == ExiEventCode)/* SE(supportedAppProtocolReq) */
  {
    #if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
    Exi_Decode_SAP_supportedAppProtocolReq(DecWsPtr);
    #else
    Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
    }
    return;
    #endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON)) */
  }
    else if(1 == ExiEventCode) /* SE(supportedAppProtocolRes) */
    {
      #if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
      Exi_Decode_SAP_supportedAppProtocolRes(DecWsPtr);
      #else
      Exi_CallInternalDetReportError(EXI_DECODER_INSTANCE_ID, EXI_API_ID_DECODE_SCHEMA_SET_SAP, EXI_E_INV_PARAM);
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_DISABLED_FEATURE;
      }
      return;
      #endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */
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
#endif /* (defined(EXI_DECODE_SCHEMA_SET_SAP) && (EXI_DECODE_SCHEMA_SET_SAP == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:NESTING_OF_CONTROL_STRUCTURES_EXCEEDED */

#endif /* (defined (EXI_ENABLE_DECODE_SAP_MESSAGE_SET) && (EXI_ENABLE_DECODE_SAP_MESSAGE_SET == STD_ON)) */

