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
 *         File:  Exi_XMLSIG_SchemaDecoder.h
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
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if defined(C_COMMENT_VECTOR)
/*  TODOs:
 *  - *
 */
#endif


#if !defined (EXI_XMLSIG_SCHEMA_DECODER_H)
# define EXI_XMLSIG_SCHEMA_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaDecoder.h"

#if (!defined (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET))
# if (defined (EXI_ENABLE_XMLSIG_MESSAGE_SET))
#  define EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET   EXI_ENABLE_XMLSIG_MESSAGE_SET
# else
#  define EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM 0x14DU
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ENCODING 0x14EU
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_ID 0x14FU
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_MIME 0x150U
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_TARGET 0x151U
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE 0x152U
#define EXI_API_ID_DECODE_XMLSIG_ATTRIBUTE_URI 0x153U
#define EXI_API_ID_DECODE_XMLSIG_CANONICALIZATION_METHOD 0x154U
#define EXI_API_ID_DECODE_XMLSIG_CRYPTO_BINARY 0x155U
#define EXI_API_ID_DECODE_XMLSIG_DSAKEY_VALUE 0x156U
#define EXI_API_ID_DECODE_XMLSIG_DIGEST_METHOD 0x157U
#define EXI_API_ID_DECODE_XMLSIG_DIGEST_VALUE 0x158U
#define EXI_API_ID_DECODE_XMLSIG_GENERIC_ELEMENT 0x159U
#define EXI_API_ID_DECODE_XMLSIG_KEY_INFO 0x15AU
#define EXI_API_ID_DECODE_XMLSIG_KEY_NAME 0x15BU
#define EXI_API_ID_DECODE_XMLSIG_KEY_VALUE 0x15CU
#define EXI_API_ID_DECODE_XMLSIG_MANIFEST 0x15DU
#define EXI_API_ID_DECODE_XMLSIG_MGMT_DATA 0x15EU
#define EXI_API_ID_DECODE_XMLSIG_OBJECT 0x15FU
#define EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 0x160U
#define EXI_API_ID_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 0x161U
#define EXI_API_ID_DECODE_XMLSIG_PGPDATA 0x162U
#define EXI_API_ID_DECODE_XMLSIG_RSAKEY_VALUE 0x163U
#define EXI_API_ID_DECODE_XMLSIG_REFERENCE 0x164U
#define EXI_API_ID_DECODE_XMLSIG_RETRIEVAL_METHOD 0x165U
#define EXI_API_ID_DECODE_XMLSIG_SPKIDATA 0x166U
#define EXI_API_ID_DECODE_XMLSIG_SIGNATURE_METHOD 0x167U
#define EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTIES 0x168U
#define EXI_API_ID_DECODE_XMLSIG_SIGNATURE_PROPERTY 0x169U
#define EXI_API_ID_DECODE_XMLSIG_SIGNATURE 0x16AU
#define EXI_API_ID_DECODE_XMLSIG_SIGNATURE_VALUE 0x16BU
#define EXI_API_ID_DECODE_XMLSIG_SIGNED_INFO 0x16CU
#define EXI_API_ID_DECODE_XMLSIG_TRANSFORM 0x16DU
#define EXI_API_ID_DECODE_XMLSIG_TRANSFORMS 0x16EU
#define EXI_API_ID_DECODE_XMLSIG_X509DATA 0x16FU
#define EXI_API_ID_DECODE_XMLSIG_X509ISSUER_SERIAL 0x170U
#define EXI_API_ID_DECODE_SCHEMA_SET_XMLSIG 0x174U
#endif

/* Decoding default switches */
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM
#define EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING
#define EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_ID
#define EXI_DECODE_XMLSIG_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_MIME
#define EXI_DECODE_XMLSIG_ATTRIBUTE_MIME STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET
#define EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE
#define EXI_DECODE_XMLSIG_ATTRIBUTE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_ATTRIBUTE_URI
#define EXI_DECODE_XMLSIG_ATTRIBUTE_URI STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD
#define EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_CRYPTO_BINARY
#define EXI_DECODE_XMLSIG_CRYPTO_BINARY STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_DSAKEY_VALUE
#define EXI_DECODE_XMLSIG_DSAKEY_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_DIGEST_METHOD
#define EXI_DECODE_XMLSIG_DIGEST_METHOD STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_DIGEST_VALUE
#define EXI_DECODE_XMLSIG_DIGEST_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_GENERIC_ELEMENT
#define EXI_DECODE_XMLSIG_GENERIC_ELEMENT STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_KEY_INFO
#define EXI_DECODE_XMLSIG_KEY_INFO STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_KEY_NAME
#define EXI_DECODE_XMLSIG_KEY_NAME STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_KEY_VALUE
#define EXI_DECODE_XMLSIG_KEY_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_MANIFEST
#define EXI_DECODE_XMLSIG_MANIFEST STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_MGMT_DATA
#define EXI_DECODE_XMLSIG_MGMT_DATA STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_OBJECT
#define EXI_DECODE_XMLSIG_OBJECT STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0
#define EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1
#define EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_PGPDATA
#define EXI_DECODE_XMLSIG_PGPDATA STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_RSAKEY_VALUE
#define EXI_DECODE_XMLSIG_RSAKEY_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_REFERENCE
#define EXI_DECODE_XMLSIG_REFERENCE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_RETRIEVAL_METHOD
#define EXI_DECODE_XMLSIG_RETRIEVAL_METHOD STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SPKIDATA
#define EXI_DECODE_XMLSIG_SPKIDATA STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNATURE_METHOD
#define EXI_DECODE_XMLSIG_SIGNATURE_METHOD STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES
#define EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY
#define EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNATURE
#define EXI_DECODE_XMLSIG_SIGNATURE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNATURE_VALUE
#define EXI_DECODE_XMLSIG_SIGNATURE_VALUE STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_SIGNED_INFO
#define EXI_DECODE_XMLSIG_SIGNED_INFO STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_TRANSFORM
#define EXI_DECODE_XMLSIG_TRANSFORM STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_TRANSFORMS
#define EXI_DECODE_XMLSIG_TRANSFORMS STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_X509DATA
#define EXI_DECODE_XMLSIG_X509DATA STD_OFF
#endif
#ifndef EXI_DECODE_XMLSIG_X509ISSUER_SERIAL
#define EXI_DECODE_XMLSIG_X509ISSUER_SERIAL STD_OFF
#endif
#ifndef EXI_DECODE_SCHEMA_SET_XMLSIG
#define EXI_DECODE_SCHEMA_SET_XMLSIG STD_OFF
#endif


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
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeAlgorithm( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ALGORITHM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeEncoding
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeEncodingType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeEncoding( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ENCODING == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeId
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeIdType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeId( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_ID) && (EXI_DECODE_XMLSIG_ATTRIBUTE_ID == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeMime
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeMimeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeMime( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_MIME) && (EXI_DECODE_XMLSIG_ATTRIBUTE_MIME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeTarget
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeTargetType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeTarget( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET) && (EXI_DECODE_XMLSIG_ATTRIBUTE_TARGET == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Attribute
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Attribute( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE) && (EXI_DECODE_XMLSIG_ATTRIBUTE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_AttributeURI
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_AttributeURIType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_AttributeURI( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_ATTRIBUTE_URI) && (EXI_DECODE_XMLSIG_ATTRIBUTE_URI == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_CanonicalizationMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_CanonicalizationMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_CanonicalizationMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD) && (EXI_DECODE_XMLSIG_CANONICALIZATION_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_CryptoBinary
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_CryptoBinaryType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_CryptoBinary( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_CRYPTO_BINARY) && (EXI_DECODE_XMLSIG_CRYPTO_BINARY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DSAKeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DSAKeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DSAKeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_DSAKEY_VALUE) && (EXI_DECODE_XMLSIG_DSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DigestMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DigestMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DigestMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_METHOD) && (EXI_DECODE_XMLSIG_DIGEST_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_DigestValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_DigestValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_DigestValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_DIGEST_VALUE) && (EXI_DECODE_XMLSIG_DIGEST_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_GenericElement
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_GenericElementType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_GenericElement( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_GENERIC_ELEMENT) && (EXI_DECODE_XMLSIG_GENERIC_ELEMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_INFO) && (EXI_DECODE_XMLSIG_KEY_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyName
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyNameType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyName( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_NAME) && (EXI_DECODE_XMLSIG_KEY_NAME == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_KeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_KeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_KeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_KEY_VALUE) && (EXI_DECODE_XMLSIG_KEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Manifest
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ManifestType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Manifest( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_MANIFEST) && (EXI_DECODE_XMLSIG_MANIFEST == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_MgmtData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_MgmtDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_MgmtData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_MGMT_DATA) && (EXI_DECODE_XMLSIG_MGMT_DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Object
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ObjectType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Object( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_OBJECT) && (EXI_DECODE_XMLSIG_OBJECT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPDataChoiceSeq0
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq0Type object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPDataChoiceSeq0( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ0 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPDataChoiceSeq1
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataChoiceSeq1Type object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPDataChoiceSeq1( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1) && (EXI_DECODE_XMLSIG_PGPDATA_CHOICE_SEQ1 == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_PGPData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_PGPDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_PGPData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_PGPDATA) && (EXI_DECODE_XMLSIG_PGPDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_RSAKeyValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_RSAKeyValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_RSAKeyValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_RSAKEY_VALUE) && (EXI_DECODE_XMLSIG_RSAKEY_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Reference
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_ReferenceType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Reference( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_REFERENCE) && (EXI_DECODE_XMLSIG_REFERENCE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_RetrievalMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_RetrievalMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_RetrievalMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_RETRIEVAL_METHOD) && (EXI_DECODE_XMLSIG_RETRIEVAL_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SPKIData
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SPKIDataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SPKIData( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SPKIDATA) && (EXI_DECODE_XMLSIG_SPKIDATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureMethod
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureMethodType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureMethod( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_METHOD) && (EXI_DECODE_XMLSIG_SIGNATURE_METHOD == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureProperties
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignaturePropertiesType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureProperties( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTIES == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureProperty
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignaturePropertyType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureProperty( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY) && (EXI_DECODE_XMLSIG_SIGNATURE_PROPERTY == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Signature
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Signature( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE) && (EXI_DECODE_XMLSIG_SIGNATURE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignatureValue
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignatureValueType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignatureValue( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNATURE_VALUE) && (EXI_DECODE_XMLSIG_SIGNATURE_VALUE == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_SignedInfo
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_SignedInfoType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_SignedInfo( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_SIGNED_INFO) && (EXI_DECODE_XMLSIG_SIGNED_INFO == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Transform
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_TransformType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Transform( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORM) && (EXI_DECODE_XMLSIG_TRANSFORM == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_Transforms
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_TransformsType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_Transforms( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_TRANSFORMS) && (EXI_DECODE_XMLSIG_TRANSFORMS == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_X509Data
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_X509DataType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_X509Data( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_X509DATA) && (EXI_DECODE_XMLSIG_X509DATA == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_XMLSIG_X509IssuerSerial
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_XMLSIG_X509IssuerSerialType object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_XMLSIG_X509IssuerSerial( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_DECODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Decode_SchemaSet_XMLSIG
 *********************************************************************************************************************/
/*! \brief         decodes an EXI stream representing an Exi_SchemaSet_XMLSIG object
 *  \param[in,out] DecWsPtr                    pointer to EXI decoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_DECODE_SCHEMA_SET_XMLSIG) && (EXI_DECODE_SCHEMA_SET_XMLSIG == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Decode_SchemaSet_XMLSIG( \
                                       P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr );
#endif /* (defined(EXI_DECODE_SCHEMA_SET_XMLSIG) && (EXI_DECODE_SCHEMA_SET_XMLSIG == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_DECODE_XMLSIG_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_XMLSIG_SCHEMA_DECODER_H */
