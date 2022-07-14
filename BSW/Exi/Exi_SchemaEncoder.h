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
 *         File:  Exi_SchemaEncoder.h
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

#if !defined (EXI_SCHEMA_ENCODER_H)
# define EXI_SCHEMA_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SAP_SchemaEncoder.h"
#include "Exi_XMLSIG_SchemaEncoder.h"
#include "Exi_ISO_SchemaEncoder.h"
#include "Exi_DIN_SchemaEncoder.h"

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_ENCODE_BASE64BINARY 0x175U
#define EXI_API_ID_ENCODE_STRING 0x176U
#define EXI_API_ID_ENCODE_SCHEMA_FRAGMENT 0x177U
#define EXI_API_ID_ENCODE_SCHEMA_ROOT 0x178U
#endif

/* Encoding default switches */
#ifndef EXI_ENCODE_SCHEMA_FRAGMENT
#define EXI_ENCODE_SCHEMA_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_SCHEMA_ROOT
#define EXI_ENCODE_SCHEMA_ROOT STD_ON
#endif
#ifndef EXI_ENCODE_BASE64BINARY
#define EXI_ENCODE_BASE64BINARY STD_OFF
#endif
#ifndef EXI_ENCODE_STRING
#define EXI_ENCODE_STRING STD_OFF
#endif

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
extern FUNC(void, EXI_CODE) Exi_Encode_base64Binary( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_base64BinaryType, AUTOMATIC, EXI_APPL_DATA) se64BinaryPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_string( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_stringType, AUTOMATIC, EXI_APPL_DATA) ringPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       Exi_NamespaceIdType Namespace );
#endif /* (defined(EXI_ENCODE_SCHEMA_FRAGMENT) && (EXI_ENCODE_SCHEMA_FRAGMENT == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Encode_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_SCHEMA_ROOT) && (EXI_ENCODE_SCHEMA_ROOT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_SCHEMA_ROOT) && (EXI_ENCODE_SCHEMA_ROOT == STD_ON)) */

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif
  /* EXI_SCHEMA_ENCODER_H */
