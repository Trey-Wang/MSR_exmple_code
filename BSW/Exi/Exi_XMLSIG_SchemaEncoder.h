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
 *         File:  Exi_XMLSIG_SchemaEncoder.h
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


#if !defined (EXI_XMLSIG_SCHEMA_ENCODER_H)
# define EXI_XMLSIG_SCHEMA_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_SchemaTypes.h"
#include "Exi_SchemaEncoder.h"

#if (!defined (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET))
# if (defined (EXI_ENABLE_XMLSIG_MESSAGE_SET))
#  define EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET   EXI_ENABLE_XMLSIG_MESSAGE_SET
# else
#  define EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET   STD_OFF
# endif
#endif

#if (defined(EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET == STD_ON))

/* PRQA S 0777 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
/* EXI internal API Ids */
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM 0x151U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ENCODING 0x152U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_ID 0x153U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_MIME 0x154U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_TARGET 0x155U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE 0x156U
#define EXI_API_ID_ENCODE_XMLSIG_ATTRIBUTE_URI 0x157U
#define EXI_API_ID_ENCODE_XMLSIG_CANONICALIZATION_METHOD 0x158U
#define EXI_API_ID_ENCODE_XMLSIG_CRYPTO_BINARY 0x159U
#define EXI_API_ID_ENCODE_XMLSIG_DSAKEY_VALUE 0x15AU
#define EXI_API_ID_ENCODE_XMLSIG_DIGEST_METHOD 0x15BU
#define EXI_API_ID_ENCODE_XMLSIG_DIGEST_VALUE 0x15CU
#define EXI_API_ID_ENCODE_XMLSIG_GENERIC_ELEMENT 0x15DU
#define EXI_API_ID_ENCODE_XMLSIG_KEY_INFO 0x15EU
#define EXI_API_ID_ENCODE_XMLSIG_KEY_NAME 0x15FU
#define EXI_API_ID_ENCODE_XMLSIG_KEY_VALUE 0x160U
#define EXI_API_ID_ENCODE_XMLSIG_MANIFEST 0x161U
#define EXI_API_ID_ENCODE_XMLSIG_MGMT_DATA 0x162U
#define EXI_API_ID_ENCODE_XMLSIG_OBJECT 0x163U
#define EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 0x164U
#define EXI_API_ID_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 0x165U
#define EXI_API_ID_ENCODE_XMLSIG_PGPDATA 0x166U
#define EXI_API_ID_ENCODE_XMLSIG_RSAKEY_VALUE 0x167U
#define EXI_API_ID_ENCODE_XMLSIG_REFERENCE 0x168U
#define EXI_API_ID_ENCODE_XMLSIG_RETRIEVAL_METHOD 0x169U
#define EXI_API_ID_ENCODE_XMLSIG_SPKIDATA 0x16AU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_METHOD 0x16BU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTIES 0x16CU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_PROPERTY 0x16DU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNATURE 0x16EU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNATURE_VALUE 0x16FU
#define EXI_API_ID_ENCODE_XMLSIG_SIGNED_INFO 0x170U
#define EXI_API_ID_ENCODE_XMLSIG_TRANSFORM 0x171U
#define EXI_API_ID_ENCODE_XMLSIG_TRANSFORMS 0x172U
#define EXI_API_ID_ENCODE_XMLSIG_X509DATA 0x173U
#define EXI_API_ID_ENCODE_XMLSIG_X509ISSUER_SERIAL 0x174U
#define EXI_API_ID_ENCODE_XMLSIG_SCHEMA_FRAGMENT 0x17BU
#define EXI_API_ID_ENCODE_XMLSIG_SCHEMA_ROOT 0x17CU
#endif

/* Encoding default switches */
#ifndef EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT
#define EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SCHEMA_ROOT
#define EXI_ENCODE_XMLSIG_SCHEMA_ROOT STD_ON
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_ALGORITHM STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_ENCODING STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_ID
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_ID STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_MIME STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_TARGET STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE
#define EXI_ENCODE_XMLSIG_ATTRIBUTE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_ATTRIBUTE_URI
#define EXI_ENCODE_XMLSIG_ATTRIBUTE_URI STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD
#define EXI_ENCODE_XMLSIG_CANONICALIZATION_METHOD STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_CRYPTO_BINARY
#define EXI_ENCODE_XMLSIG_CRYPTO_BINARY STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_DSAKEY_VALUE
#define EXI_ENCODE_XMLSIG_DSAKEY_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_DIGEST_METHOD
#define EXI_ENCODE_XMLSIG_DIGEST_METHOD STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_DIGEST_VALUE
#define EXI_ENCODE_XMLSIG_DIGEST_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_GENERIC_ELEMENT
#define EXI_ENCODE_XMLSIG_GENERIC_ELEMENT STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_KEY_INFO
#define EXI_ENCODE_XMLSIG_KEY_INFO STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_KEY_NAME
#define EXI_ENCODE_XMLSIG_KEY_NAME STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_KEY_VALUE
#define EXI_ENCODE_XMLSIG_KEY_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_MANIFEST
#define EXI_ENCODE_XMLSIG_MANIFEST STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_MGMT_DATA
#define EXI_ENCODE_XMLSIG_MGMT_DATA STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_OBJECT
#define EXI_ENCODE_XMLSIG_OBJECT STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0
#define EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ0 STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1
#define EXI_ENCODE_XMLSIG_PGPDATA_CHOICE_SEQ1 STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_PGPDATA
#define EXI_ENCODE_XMLSIG_PGPDATA STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_RSAKEY_VALUE
#define EXI_ENCODE_XMLSIG_RSAKEY_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_REFERENCE
#define EXI_ENCODE_XMLSIG_REFERENCE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD
#define EXI_ENCODE_XMLSIG_RETRIEVAL_METHOD STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SPKIDATA
#define EXI_ENCODE_XMLSIG_SPKIDATA STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNATURE_METHOD
#define EXI_ENCODE_XMLSIG_SIGNATURE_METHOD STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES
#define EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTIES STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY
#define EXI_ENCODE_XMLSIG_SIGNATURE_PROPERTY STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNATURE
#define EXI_ENCODE_XMLSIG_SIGNATURE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNATURE_VALUE
#define EXI_ENCODE_XMLSIG_SIGNATURE_VALUE STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_SIGNED_INFO
#define EXI_ENCODE_XMLSIG_SIGNED_INFO STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_TRANSFORM
#define EXI_ENCODE_XMLSIG_TRANSFORM STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_TRANSFORMS
#define EXI_ENCODE_XMLSIG_TRANSFORMS STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_X509DATA
#define EXI_ENCODE_XMLSIG_X509DATA STD_OFF
#endif
#ifndef EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL
#define EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL STD_OFF
#endif


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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeAlgorithm( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeAlgorithmType, AUTOMATIC, EXI_APPL_DATA) AttributeAlgorithmPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeEncoding( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeEncodingType, AUTOMATIC, EXI_APPL_DATA) AttributeEncodingPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeId( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeIdType, AUTOMATIC, EXI_APPL_DATA) AttributeIdPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeMime( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeMimeType, AUTOMATIC, EXI_APPL_DATA) AttributeMimePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeTarget( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeTargetType, AUTOMATIC, EXI_APPL_DATA) AttributeTargetPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Attribute( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeType, AUTOMATIC, EXI_APPL_DATA) AttributePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_AttributeURI( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_AttributeURIType, AUTOMATIC, EXI_APPL_DATA) AttributeURIPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_CanonicalizationMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_CanonicalizationMethodType, AUTOMATIC, EXI_APPL_DATA) CanonicalizationMethodPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_CryptoBinary( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_CryptoBinaryType, AUTOMATIC, EXI_APPL_DATA) CryptoBinaryPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DSAKeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DSAKeyValueType, AUTOMATIC, EXI_APPL_DATA) DSAKeyValuePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DigestMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DigestMethodType, AUTOMATIC, EXI_APPL_DATA) DigestMethodPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_DigestValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_DigestValueType, AUTOMATIC, EXI_APPL_DATA) DigestValuePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_GenericElement( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_GenericElementType, AUTOMATIC, EXI_APPL_DATA) GenericElementPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyInfoType, AUTOMATIC, EXI_APPL_DATA) KeyInfoPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyName( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyNameType, AUTOMATIC, EXI_APPL_DATA) KeyNamePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_KeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_KeyValueType, AUTOMATIC, EXI_APPL_DATA) KeyValuePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Manifest( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ManifestType, AUTOMATIC, EXI_APPL_DATA) ManifestPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_MgmtData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_MgmtDataType, AUTOMATIC, EXI_APPL_DATA) MgmtDataPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Object( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ObjectType, AUTOMATIC, EXI_APPL_DATA) ObjectPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPDataChoiceSeq0( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataChoiceSeq0Type, AUTOMATIC, EXI_APPL_DATA) PGPDataChoiceSeq0Ptr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPDataChoiceSeq1( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataChoiceSeq1Type, AUTOMATIC, EXI_APPL_DATA) PGPDataChoiceSeq1Ptr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_PGPData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_PGPDataType, AUTOMATIC, EXI_APPL_DATA) PGPDataPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_RSAKeyValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_RSAKeyValueType, AUTOMATIC, EXI_APPL_DATA) RSAKeyValuePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Reference( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_ReferenceType, AUTOMATIC, EXI_APPL_DATA) ReferencePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_RetrievalMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_RetrievalMethodType, AUTOMATIC, EXI_APPL_DATA) RetrievalMethodPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SPKIData( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SPKIDataType, AUTOMATIC, EXI_APPL_DATA) SPKIDataPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureMethod( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureMethodType, AUTOMATIC, EXI_APPL_DATA) SignatureMethodPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureProperties( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignaturePropertiesType, AUTOMATIC, EXI_APPL_DATA) SignaturePropertiesPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureProperty( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignaturePropertyType, AUTOMATIC, EXI_APPL_DATA) SignaturePropertyPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Signature( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureType, AUTOMATIC, EXI_APPL_DATA) SignaturePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignatureValue( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignatureValueType, AUTOMATIC, EXI_APPL_DATA) SignatureValuePtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SignedInfo( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_SignedInfoType, AUTOMATIC, EXI_APPL_DATA) SignedInfoPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Transform( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_TransformType, AUTOMATIC, EXI_APPL_DATA) TransformPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_Transforms( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_TransformsType, AUTOMATIC, EXI_APPL_DATA) TransformsPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_X509Data( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_X509DataType, AUTOMATIC, EXI_APPL_DATA) X509DataPtr );
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
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_X509IssuerSerial( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr, \
                                       P2CONST(Exi_XMLSIG_X509IssuerSerialType, AUTOMATIC, EXI_APPL_DATA) X509IssuerSerialPtr );
#endif /* (defined(EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL) && (EXI_ENCODE_XMLSIG_X509ISSUER_SERIAL == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SchemaFragment
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SchemaFragment object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SchemaFragment( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT) && (EXI_ENCODE_XMLSIG_SCHEMA_FRAGMENT == STD_ON)) */


/**********************************************************************************************************************
 *  Exi_Encode_XMLSIG_SchemaRoot
 *********************************************************************************************************************/
/*! \brief         generates an EXI stream representing an Exi_XMLSIG_SchemaRoot object
 *  \param[in,out] EncWsPtr                    pointer to EXI encoding workspace
 *  \context       task level
 *********************************************************************************************************************/
#if (defined(EXI_ENCODE_XMLSIG_SCHEMA_ROOT) && (EXI_ENCODE_XMLSIG_SCHEMA_ROOT == STD_ON))
extern FUNC(void, EXI_CODE) Exi_Encode_XMLSIG_SchemaRoot( \
                                       P2VAR(Exi_EncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr );
#endif /* (defined(EXI_ENCODE_XMLSIG_SCHEMA_ROOT) && (EXI_ENCODE_XMLSIG_SCHEMA_ROOT == STD_ON)) */


#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:IDENTIFIER_NAMES */

#endif /* (defined(EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET) && (EXI_ENABLE_ENCODE_XMLSIG_MESSAGE_SET == STD_ON)) */

#endif
  /* EXI_XMLSIG_SCHEMA_ENCODER_H */
