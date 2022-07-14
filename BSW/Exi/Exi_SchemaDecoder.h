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
 *         File:  Exi_SchemaDecoder.h
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

#if !defined (EXI_SCHEMA_DECODER_H)
# define EXI_SCHEMA_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SAP_SchemaDecoder.h"
#include "Exi_XMLSIG_SchemaDecoder.h"
#include "Exi_ISO_SchemaDecoder.h"
#include "Exi_DIN_SchemaDecoder.h"

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_DECODE_BASE64BINARY 0x171U
#define EXI_API_ID_DECODE_STRING 0x172U
#endif

/* Decoding default switches */
#ifndef EXI_DECODE_BASE64BINARY
#define EXI_DECODE_BASE64BINARY STD_OFF
#endif
#ifndef EXI_DECODE_STRING
#define EXI_DECODE_STRING STD_OFF
#endif

#define EXI_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

extern CONST(Exi_DecoderFctType, EXI_CODE)Exi_DecoderFcts[7];

extern CONST(uint8, EXI_CONST) Exi_UriPartitionSize[7];

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
extern FUNC(void, EXI_CODE) Exi_Decode_base64Binary( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_BASE64BINARY) && (EXI_DECODE_BASE64BINARY == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_Decode_string
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_stringType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_string( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_STRING) && (EXI_DECODE_STRING == STD_ON)) */

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif
  /* EXI_SCHEMA_DECODER_H */
