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
 *         File:  Exi_SAP_SchemaDecoder.h
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


#if !defined (EXI_SAP_SCHEMA_DECODER_H)
# define EXI_SAP_SCHEMA_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaDecoder.h"

#if (!defined (EXI_ENABLE_DECODE_SAP_MESSAGE_SET))
# if (defined (EXI_ENABLE_SAP_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_SAP_MESSAGE_SET   EXI_ENABLE_SAP_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_SAP_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_SAP_MESSAGE_SET) && (EXI_ENABLE_DECODE_SAP_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_DECODE_SAP_APP_PROTOCOL 0xFFU
#define EXI_API_ID_DECODE_SAP_PROTOCOL_NAMESPACE 0x100U
#define EXI_API_ID_DECODE_SAP_RESPONSE_CODE 0x101U
#define EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ 0x102U
#define EXI_API_ID_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES 0x103U
#define EXI_API_ID_DECODE_SCHEMA_SET_SAP 0x173U
#endif

/* Decoding default switches */
#ifndef EXI_DECODE_SAP_APP_PROTOCOL
#define EXI_DECODE_SAP_APP_PROTOCOL STD_OFF
#endif
#ifndef EXI_DECODE_SAP_PROTOCOL_NAMESPACE
#define EXI_DECODE_SAP_PROTOCOL_NAMESPACE STD_OFF
#endif
#ifndef EXI_DECODE_SAP_RESPONSE_CODE
#define EXI_DECODE_SAP_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ
#define EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ STD_OFF
#endif
#ifndef EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES
#define EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES STD_OFF
#endif
#ifndef EXI_DECODE_SCHEMA_SET_SAP
#define EXI_DECODE_SCHEMA_SET_SAP STD_OFF
#endif


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
extern FUNC(void, EXI_CODE) Exi_Decode_SAP_AppProtocol( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SAP_APP_PROTOCOL) && (EXI_DECODE_SAP_APP_PROTOCOL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_protocolNamespace
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_protocolNamespaceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SAP_protocolNamespace( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SAP_PROTOCOL_NAMESPACE) && (EXI_DECODE_SAP_PROTOCOL_NAMESPACE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_responseCode
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_responseCodeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SAP_responseCode( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SAP_RESPONSE_CODE) && (EXI_DECODE_SAP_RESPONSE_CODE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_supportedAppProtocolReq
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_supportedAppProtocolReqType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SAP_supportedAppProtocolReq( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_REQ == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SAP_supportedAppProtocolRes
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SAP_supportedAppProtocolResType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SAP_supportedAppProtocolRes( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_DECODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_SAP
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_SAP object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_SAP) && (EXI_DECODE_SCHEMA_SET_SAP == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_SAP( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SCHEMA_SET_SAP) && (EXI_DECODE_SCHEMA_SET_SAP == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined (EXI_ENABLE_DECODE_SAP_MESSAGE_SET) && (EXI_ENABLE_DECODE_SAP_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_SAP_SCHEMA_DECODER_H */
