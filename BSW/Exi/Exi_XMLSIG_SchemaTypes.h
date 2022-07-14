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
 *         File:  Exi_XMLSIG_SchemaTypes.h
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


#if !defined (EXI_XMLSIG_SCHEMA_TYPES_H)
# define EXI_XMLSIG_SCHEMA_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"

/* PRQA S 0779 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
/* PRQA S 0780 NAMESPACE */ /* MD_Exi_5.6 */
/* PRQA S 0750 UNION */ /* MD_Exi_18.4 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Root elements */
#define EXI_XMLSIG_CANONICALIZATION_METHOD_TYPE (Exi_RootElementIdType)176U
#define EXI_XMLSIG_DSAKEY_VALUE_TYPE (Exi_RootElementIdType)177U
#define EXI_XMLSIG_DIGEST_METHOD_TYPE (Exi_RootElementIdType)178U
#define EXI_XMLSIG_DIGEST_VALUE_TYPE (Exi_RootElementIdType)179U
#define EXI_XMLSIG_KEY_INFO_TYPE (Exi_RootElementIdType)180U
#define EXI_XMLSIG_KEY_NAME_TYPE (Exi_RootElementIdType)181U
#define EXI_XMLSIG_KEY_VALUE_TYPE (Exi_RootElementIdType)182U
#define EXI_XMLSIG_MANIFEST_TYPE (Exi_RootElementIdType)183U
#define EXI_XMLSIG_MGMT_DATA_TYPE (Exi_RootElementIdType)184U
#define EXI_XMLSIG_OBJECT_TYPE (Exi_RootElementIdType)185U
#define EXI_XMLSIG_PGPDATA_TYPE (Exi_RootElementIdType)186U
#define EXI_XMLSIG_RSAKEY_VALUE_TYPE (Exi_RootElementIdType)187U
#define EXI_XMLSIG_REFERENCE_TYPE (Exi_RootElementIdType)188U
#define EXI_XMLSIG_RETRIEVAL_METHOD_TYPE (Exi_RootElementIdType)189U
#define EXI_XMLSIG_SPKIDATA_TYPE (Exi_RootElementIdType)190U
#define EXI_XMLSIG_SIGNATURE_METHOD_TYPE (Exi_RootElementIdType)191U
#define EXI_XMLSIG_SIGNATURE_PROPERTIES_TYPE (Exi_RootElementIdType)192U
#define EXI_XMLSIG_SIGNATURE_PROPERTY_TYPE (Exi_RootElementIdType)193U
#define EXI_XMLSIG_SIGNATURE_TYPE (Exi_RootElementIdType)194U
#define EXI_XMLSIG_SIGNATURE_VALUE_TYPE (Exi_RootElementIdType)195U
#define EXI_XMLSIG_SIGNED_INFO_TYPE (Exi_RootElementIdType)196U
#define EXI_XMLSIG_TRANSFORM_TYPE (Exi_RootElementIdType)197U
#define EXI_XMLSIG_TRANSFORMS_TYPE (Exi_RootElementIdType)198U
#define EXI_XMLSIG_X509DATA_TYPE (Exi_RootElementIdType)199U
/* Elements */
#define EXI_XMLSIG_EXPONENT_TYPE (Exi_RootElementIdType)207U
#define EXI_XMLSIG_G_TYPE (Exi_RootElementIdType)208U
#define EXI_XMLSIG_HMACOUTPUT_LENGTH_TYPE (Exi_RootElementIdType)209U
#define EXI_XMLSIG_J_TYPE (Exi_RootElementIdType)210U
#define EXI_XMLSIG_MODULUS_TYPE (Exi_RootElementIdType)211U
#define EXI_XMLSIG_P_TYPE (Exi_RootElementIdType)212U
#define EXI_XMLSIG_PGPKEY_ID_TYPE (Exi_RootElementIdType)213U
#define EXI_XMLSIG_PGPKEY_PACKET_TYPE (Exi_RootElementIdType)214U
#define EXI_XMLSIG_PGEN_COUNTER_TYPE (Exi_RootElementIdType)215U
#define EXI_XMLSIG_Q_TYPE (Exi_RootElementIdType)216U
#define EXI_XMLSIG_SPKISEXP_TYPE (Exi_RootElementIdType)217U
#define EXI_XMLSIG_SEED_TYPE (Exi_RootElementIdType)218U
#define EXI_XMLSIG_X509CRL_TYPE (Exi_RootElementIdType)219U
#define EXI_XMLSIG_X509CERTIFICATE_TYPE (Exi_RootElementIdType)220U
#define EXI_XMLSIG_X509ISSUER_NAME_TYPE (Exi_RootElementIdType)221U
#define EXI_XMLSIG_X509ISSUER_SERIAL_TYPE (Exi_RootElementIdType)222U
#define EXI_XMLSIG_X509SKI_TYPE (Exi_RootElementIdType)223U
#define EXI_XMLSIG_X509SERIAL_NUMBER_TYPE (Exi_RootElementIdType)224U
#define EXI_XMLSIG_X509SUBJECT_NAME_TYPE (Exi_RootElementIdType)225U
#define EXI_XMLSIG_XPATH_TYPE (Exi_RootElementIdType)226U
#define EXI_XMLSIG_Y_TYPE (Exi_RootElementIdType)227U
#define EXI_SCHEMA_SET_XMLSIG_TYPE (Exi_RootElementIdType)1U
#ifndef EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE
  #define EXI_MAXOCCURS_XMLSIG_KEYINFOCHOICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_OBJECT
  #define EXI_MAXOCCURS_XMLSIG_OBJECT EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_REFERENCE
  #define EXI_MAXOCCURS_XMLSIG_REFERENCE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY
  #define EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTY EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE
  #define EXI_MAXOCCURS_XMLSIG_SIGNATUREPROPERTYCHOICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_TRANSFORM
  #define EXI_MAXOCCURS_XMLSIG_TRANSFORM EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE
  #define EXI_MAXOCCURS_XMLSIG_TRANSFORMCHOICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAXOCCURS_XMLSIG_X509DATACHOICE
  #define EXI_MAXOCCURS_XMLSIG_X509DATACHOICE EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE
  #define EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct Exi_XMLSIG_AttributeIdType Exi_XMLSIG_AttributeIdType;

struct Exi_XMLSIG_AttributeIdType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEID ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeAlgorithmType Exi_XMLSIG_AttributeAlgorithmType;

struct Exi_XMLSIG_AttributeAlgorithmType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEALGORITHM ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_GenericElementType Exi_XMLSIG_GenericElementType;

struct Exi_XMLSIG_GenericElementType
{
  void* GenericElement;
  struct Exi_XMLSIG_GenericElementType* NextGenericElementPtr;
  Exi_RootElementIdType GenericElementId;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_CanonicalizationMethodType Exi_XMLSIG_CanonicalizationMethodType;

struct Exi_XMLSIG_CanonicalizationMethodType
{
  Exi_XMLSIG_AttributeAlgorithmType* Algorithm;
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_BitType GenericElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignatureMethodType Exi_XMLSIG_SignatureMethodType;

struct Exi_XMLSIG_SignatureMethodType
{
  Exi_XMLSIG_AttributeAlgorithmType* Algorithm;
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_BigIntType* HMACOutputLength;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType HMACOutputLengthFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeURIType Exi_XMLSIG_AttributeURIType;

struct Exi_XMLSIG_AttributeURIType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEURI ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeType Exi_XMLSIG_AttributeType;

struct Exi_XMLSIG_AttributeType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETYPE ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_stringType Exi_stringType;

typedef struct Exi_XMLSIG_TransformChoiceType Exi_XMLSIG_TransformChoiceType;

struct Exi_XMLSIG_TransformChoiceType
{
  union
  {
    Exi_XMLSIG_GenericElementType* GenericElement;
    Exi_stringType* XPath;
  } ChoiceValue;
  struct Exi_XMLSIG_TransformChoiceType* NextChoiceElementPtr;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType XPathFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_TransformType Exi_XMLSIG_TransformType;

struct Exi_XMLSIG_TransformType
{
  Exi_XMLSIG_AttributeAlgorithmType* Algorithm;
  Exi_XMLSIG_TransformChoiceType* ChoiceElement;
  struct Exi_XMLSIG_TransformType* NextTransformPtr;
  Exi_BitType ChoiceElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_TransformsType Exi_XMLSIG_TransformsType;

struct Exi_XMLSIG_TransformsType
{
  Exi_XMLSIG_TransformType* Transform;
};

typedef struct Exi_XMLSIG_DigestMethodType Exi_XMLSIG_DigestMethodType;

struct Exi_XMLSIG_DigestMethodType
{
  Exi_XMLSIG_AttributeAlgorithmType* Algorithm;
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_BitType GenericElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_DigestValueType Exi_XMLSIG_DigestValueType;

struct Exi_XMLSIG_DigestValueType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_DIGESTVALUE ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_ReferenceType Exi_XMLSIG_ReferenceType;

struct Exi_XMLSIG_ReferenceType
{
  Exi_XMLSIG_DigestMethodType* DigestMethod;
  Exi_XMLSIG_DigestValueType* DigestValue;
  Exi_XMLSIG_AttributeIdType* Id;
  struct Exi_XMLSIG_ReferenceType* NextReferencePtr;
  Exi_XMLSIG_TransformsType* Transforms;
  Exi_XMLSIG_AttributeType* Type;
  Exi_XMLSIG_AttributeURIType* URI;
  Exi_BitType IdFlag : 1;
  Exi_BitType TransformsFlag : 1;
  Exi_BitType TypeFlag : 1;
  Exi_BitType URIFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignedInfoType Exi_XMLSIG_SignedInfoType;

struct Exi_XMLSIG_SignedInfoType
{
  Exi_XMLSIG_CanonicalizationMethodType* CanonicalizationMethod;
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_XMLSIG_ReferenceType* Reference;
  Exi_XMLSIG_SignatureMethodType* SignatureMethod;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignatureValueType Exi_XMLSIG_SignatureValueType;

struct Exi_XMLSIG_SignatureValueType
{
  Exi_XMLSIG_AttributeIdType* Id;
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE];
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + ( EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + ( EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + ( EXI_MAX_BUFFER_SIZE_XMLSIG_SIGNATUREVALUE ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_KeyNameType Exi_XMLSIG_KeyNameType;

struct Exi_XMLSIG_KeyNameType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_KEYNAME ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_CryptoBinaryType Exi_XMLSIG_CryptoBinaryType;

struct Exi_XMLSIG_CryptoBinaryType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_CRYPTOBINARY ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_DSAKeyValueType Exi_XMLSIG_DSAKeyValueType;

struct Exi_XMLSIG_DSAKeyValueType
{
  Exi_XMLSIG_CryptoBinaryType* G;
  Exi_XMLSIG_CryptoBinaryType* J;
  Exi_XMLSIG_CryptoBinaryType* P;
  Exi_XMLSIG_CryptoBinaryType* PgenCounter;
  Exi_XMLSIG_CryptoBinaryType* Q;
  Exi_XMLSIG_CryptoBinaryType* Seed;
  Exi_XMLSIG_CryptoBinaryType* Y;
  Exi_BitType GFlag : 1;
  Exi_BitType JFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_RSAKeyValueType Exi_XMLSIG_RSAKeyValueType;

struct Exi_XMLSIG_RSAKeyValueType
{
  Exi_XMLSIG_CryptoBinaryType* Exponent;
  Exi_XMLSIG_CryptoBinaryType* Modulus;
};

typedef struct Exi_XMLSIG_KeyValueChoiceType Exi_XMLSIG_KeyValueChoiceType;

struct Exi_XMLSIG_KeyValueChoiceType
{
  union
  {
    Exi_XMLSIG_DSAKeyValueType* DSAKeyValue;
    Exi_XMLSIG_GenericElementType* GenericElement;
    Exi_XMLSIG_RSAKeyValueType* RSAKeyValue;
  } ChoiceValue;
  Exi_BitType DSAKeyValueFlag : 1;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType RSAKeyValueFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_KeyValueType Exi_XMLSIG_KeyValueType;

struct Exi_XMLSIG_KeyValueType
{
  Exi_XMLSIG_KeyValueChoiceType* ChoiceElement;
};

typedef struct Exi_XMLSIG_RetrievalMethodType Exi_XMLSIG_RetrievalMethodType;

struct Exi_XMLSIG_RetrievalMethodType
{
  Exi_XMLSIG_TransformsType* Transforms;
  Exi_XMLSIG_AttributeType* Type;
  Exi_XMLSIG_AttributeURIType* URI;
  Exi_BitType TransformsFlag : 1;
  Exi_BitType TypeFlag : 1;
  Exi_BitType URIFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_X509IssuerSerialType Exi_XMLSIG_X509IssuerSerialType;

struct Exi_XMLSIG_X509IssuerSerialType
{
  struct Exi_XMLSIG_X509IssuerSerialType* NextRootCertificateIDPtr;
  Exi_stringType* X509IssuerName;
  Exi_BigIntType* X509SerialNumber;
};

typedef struct Exi_base64BinaryType Exi_base64BinaryType;

typedef struct Exi_XMLSIG_X509DataChoiceType Exi_XMLSIG_X509DataChoiceType;

struct Exi_XMLSIG_X509DataChoiceType
{
  union
  {
    Exi_XMLSIG_GenericElementType* GenericElement;
    Exi_base64BinaryType* X509CRL;
    Exi_base64BinaryType* X509Certificate;
    Exi_XMLSIG_X509IssuerSerialType* X509IssuerSerial;
    Exi_base64BinaryType* X509SKI;
    Exi_stringType* X509SubjectName;
  } ChoiceValue;
  struct Exi_XMLSIG_X509DataChoiceType* NextChoiceElementPtr;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType X509CRLFlag : 1;
  Exi_BitType X509CertificateFlag : 1;
  Exi_BitType X509IssuerSerialFlag : 1;
  Exi_BitType X509SKIFlag : 1;
  Exi_BitType X509SubjectNameFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_X509DataType Exi_XMLSIG_X509DataType;

struct Exi_XMLSIG_X509DataType
{
  Exi_XMLSIG_X509DataChoiceType* ChoiceElement;
};

typedef struct Exi_XMLSIG_PGPDataChoiceSeq0Type Exi_XMLSIG_PGPDataChoiceSeq0Type;

struct Exi_XMLSIG_PGPDataChoiceSeq0Type
{
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_base64BinaryType* PGPKeyID;
  Exi_base64BinaryType* PGPKeyPacket;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType PGPKeyPacketFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_PGPDataChoiceSeq1Type Exi_XMLSIG_PGPDataChoiceSeq1Type;

struct Exi_XMLSIG_PGPDataChoiceSeq1Type
{
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_base64BinaryType* PGPKeyPacket;
  Exi_BitType GenericElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_PGPDataChoiceType Exi_XMLSIG_PGPDataChoiceType;

struct Exi_XMLSIG_PGPDataChoiceType
{
  union
  {
    Exi_XMLSIG_PGPDataChoiceSeq0Type* ChoiceSequence0;
    Exi_XMLSIG_PGPDataChoiceSeq1Type* ChoiceSequence1;
  } ChoiceValue;
  Exi_BitType ChoiceSequence0Flag : 1;
  Exi_BitType ChoiceSequence1Flag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_PGPDataType Exi_XMLSIG_PGPDataType;

struct Exi_XMLSIG_PGPDataType
{
  Exi_XMLSIG_PGPDataChoiceType* ChoiceElement;
};

typedef struct Exi_XMLSIG_SPKIDataType Exi_XMLSIG_SPKIDataType;

struct Exi_XMLSIG_SPKIDataType
{
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_base64BinaryType* SPKISexp;
  Exi_BitType GenericElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_MgmtDataType Exi_XMLSIG_MgmtDataType;

struct Exi_XMLSIG_MgmtDataType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_MGMTDATA ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_KeyInfoChoiceType Exi_XMLSIG_KeyInfoChoiceType;

struct Exi_XMLSIG_KeyInfoChoiceType
{
  union
  {
    Exi_XMLSIG_GenericElementType* GenericElement;
    Exi_XMLSIG_KeyNameType* KeyName;
    Exi_XMLSIG_KeyValueType* KeyValue;
    Exi_XMLSIG_MgmtDataType* MgmtData;
    Exi_XMLSIG_PGPDataType* PGPData;
    Exi_XMLSIG_RetrievalMethodType* RetrievalMethod;
    Exi_XMLSIG_SPKIDataType* SPKIData;
    Exi_XMLSIG_X509DataType* X509Data;
  } ChoiceValue;
  struct Exi_XMLSIG_KeyInfoChoiceType* NextChoiceElementPtr;
  Exi_BitType GenericElementFlag : 1;
  Exi_BitType KeyNameFlag : 1;
  Exi_BitType KeyValueFlag : 1;
  Exi_BitType MgmtDataFlag : 1;
  Exi_BitType PGPDataFlag : 1;
  Exi_BitType RetrievalMethodFlag : 1;
  Exi_BitType SPKIDataFlag : 1;
  Exi_BitType X509DataFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_KeyInfoType Exi_XMLSIG_KeyInfoType;

struct Exi_XMLSIG_KeyInfoType
{
  Exi_XMLSIG_KeyInfoChoiceType* ChoiceElement;
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeMimeType Exi_XMLSIG_AttributeMimeType;

struct Exi_XMLSIG_AttributeMimeType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEMIMETYPE ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeEncodingType Exi_XMLSIG_AttributeEncodingType;

struct Exi_XMLSIG_AttributeEncodingType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTEENCODING ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_ObjectType Exi_XMLSIG_ObjectType;

struct Exi_XMLSIG_ObjectType
{
  Exi_XMLSIG_AttributeEncodingType* Encoding;
  Exi_XMLSIG_GenericElementType* GenericElement;
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_XMLSIG_AttributeMimeType* MimeType;
  struct Exi_XMLSIG_ObjectType* NextObjectPtr;
  Exi_BitType EncodingFlag : 1;
  Exi_BitType IdFlag : 1;
  Exi_BitType MimeTypeFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignatureType Exi_XMLSIG_SignatureType;

struct Exi_XMLSIG_SignatureType
{
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_XMLSIG_KeyInfoType* KeyInfo;
  Exi_XMLSIG_ObjectType* Object;
  Exi_XMLSIG_SignatureValueType* SignatureValue;
  Exi_XMLSIG_SignedInfoType* SignedInfo;
  Exi_BitType IdFlag : 1;
  Exi_BitType KeyInfoFlag : 1;
  Exi_BitType ObjectFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_ManifestType Exi_XMLSIG_ManifestType;

struct Exi_XMLSIG_ManifestType
{
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_XMLSIG_ReferenceType* Reference;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_AttributeTargetType Exi_XMLSIG_AttributeTargetType;

struct Exi_XMLSIG_AttributeTargetType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_XMLSIG_ATTRIBUTETARGET ) + 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignaturePropertyChoiceType Exi_XMLSIG_SignaturePropertyChoiceType;

struct Exi_XMLSIG_SignaturePropertyChoiceType
{
  union
  {
    Exi_XMLSIG_GenericElementType* GenericElement;
  } ChoiceValue;
  struct Exi_XMLSIG_SignaturePropertyChoiceType* NextChoiceElementPtr;
  Exi_BitType GenericElementFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignaturePropertyType Exi_XMLSIG_SignaturePropertyType;

struct Exi_XMLSIG_SignaturePropertyType
{
  Exi_XMLSIG_SignaturePropertyChoiceType* ChoiceElement;
  Exi_XMLSIG_AttributeIdType* Id;
  struct Exi_XMLSIG_SignaturePropertyType* NextSignaturePropertyPtr;
  Exi_XMLSIG_AttributeTargetType* Target;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

typedef struct Exi_XMLSIG_SignaturePropertiesType Exi_XMLSIG_SignaturePropertiesType;

struct Exi_XMLSIG_SignaturePropertiesType
{
  Exi_XMLSIG_AttributeIdType* Id;
  Exi_XMLSIG_SignaturePropertyType* SignatureProperty;
  Exi_BitType IdFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) ) % 4)];
# endif
#endif 
};

/* PRQA L:IDENTIFIER_NAMES */
/* PRQA L:NAMESPACE */
/* PRQA L:UNION */


#endif
  /* EXI_XMLSIG_SCHEMA_TYPES_H */
