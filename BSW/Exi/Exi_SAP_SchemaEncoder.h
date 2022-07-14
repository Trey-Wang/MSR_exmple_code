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
 *         File:  Exi_SAP_SchemaEncoder.h
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


#if !defined (EXI_SAP_SCHEMA_ENCODER_H)
# define EXI_SAP_SCHEMA_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaEncoder.h"

#if (!defined (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET))
# if (defined (EXI_ENABLE_SAP_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_SAP_MESSAGE_SET   EXI_ENABLE_SAP_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_SAP_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_ENCODE_SAP_MESSAGE_SET) && (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_ENCODE_SAP_APP_PROTOCOL 0x103U
#define EXI_API_ID_ENCODE_SAP_PROTOCOL_NAMESPACE 0x104U
#define EXI_API_ID_ENCODE_SAP_RESPONSE_CODE 0x105U
#define EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ 0x106U
#define EXI_API_ID_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES 0x107U
#define EXI_API_ID_ENCODE_SAP_SCHEMA_FRAGMENT 0x179U
#define EXI_API_ID_ENCODE_SAP_SCHEMA_ROOT 0x17AU
#endif

/* Encoding default switches */
#ifndef EXI_ENCODE_SAP_SCHEMA_FRAGMENT
#define EXI_ENCODE_SAP_SCHEMA_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_SAP_SCHEMA_ROOT
#define EXI_ENCODE_SAP_SCHEMA_ROOT STD_ON
#endif
#ifndef EXI_ENCODE_SAP_APP_PROTOCOL
#define EXI_ENCODE_SAP_APP_PROTOCOL STD_OFF
#endif
#ifndef EXI_ENCODE_SAP_PROTOCOL_NAMESPACE
#define EXI_ENCODE_SAP_PROTOCOL_NAMESPACE STD_OFF
#endif
#ifndef EXI_ENCODE_SAP_RESPONSE_CODE
#define EXI_ENCODE_SAP_RESPONSE_CODE STD_OFF
#endif
#ifndef EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ
#define EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_REQ STD_OFF
#endif
#ifndef EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES
#define EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES STD_OFF
#endif


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
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_AppProtocol( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_AppProtocolType, AUTOMATIC, EXI_APPL_DATA) AppProtocolPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_protocolNamespace( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_protocolNamespaceType, AUTOMATIC, EXI_APPL_DATA) protocolNamespacePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_responseCode( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_responseCodeType, AUTOMATIC, EXI_APPL_DATA) responseCodePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_supportedAppProtocolReq( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_supportedAppProtocolReqType, AUTOMATIC, EXI_APPL_DATA) supportedAppProtocolReqPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_supportedAppProtocolRes( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_SAP_supportedAppProtocolResType, AUTOMATIC, EXI_APPL_DATA) supportedAppProtocolResPtr );
#endif /* (defined(EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES) && (EXI_ENCODE_SAP_SUPPORTED_APP_PROTOCOL_RES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_SAP_SCHEMA_FRAGMENT) && (EXI_ENCODE_SAP_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_SAP_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SAP_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SAP_SCHEMA_ROOT) && (EXI_ENCODE_SAP_SCHEMA_ROOT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_SAP_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_SAP_SCHEMA_ROOT) && (EXI_ENCODE_SAP_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined(EXI_ENABLE_ENCODE_SAP_MESSAGE_SET) && (EXI_ENABLE_ENCODE_SAP_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_SAP_SCHEMA_ENCODER_H */
