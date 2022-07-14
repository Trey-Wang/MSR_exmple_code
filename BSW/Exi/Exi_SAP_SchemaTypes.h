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
 *         File:  Exi_SAP_SchemaTypes.h
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


#if !defined (EXI_SAP_SCHEMA_TYPES_H)
# define EXI_SAP_SCHEMA_TYPES_H

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
#define EXI_SAP_SUPPORTED_APP_PROTOCOL_REQ_TYPE (Exi_RootElementIdType)136U
#define EXI_SAP_SUPPORTED_APP_PROTOCOL_RES_TYPE (Exi_RootElementIdType)137U
/* Elements */
#define EXI_SAP_APP_PROTOCOL_TYPE (Exi_RootElementIdType)200U
#define EXI_SAP_PRIORITY_TYPE (Exi_RootElementIdType)201U
#define EXI_SAP_PROTOCOL_NAMESPACE_TYPE (Exi_RootElementIdType)202U
#define EXI_SAP_RESPONSE_CODE_TYPE (Exi_RootElementIdType)203U
#define EXI_SAP_SCHEMA_ID_TYPE (Exi_RootElementIdType)204U
#define EXI_SAP_VERSION_NUMBER_MAJOR_TYPE (Exi_RootElementIdType)205U
#define EXI_SAP_VERSION_NUMBER_MINOR_TYPE (Exi_RootElementIdType)206U
#define EXI_SCHEMA_SET_SAP_TYPE (Exi_RootElementIdType)0U

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct Exi_SAP_protocolNamespaceType Exi_SAP_protocolNamespaceType;

struct Exi_SAP_protocolNamespaceType
{
  uint16 Length;
  uint8 Buffer[100];
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_SAP_AppProtocolType Exi_SAP_AppProtocolType;

struct Exi_SAP_AppProtocolType
{
  struct Exi_SAP_AppProtocolType* NextAppProtocolPtr;
  Exi_SAP_protocolNamespaceType* ProtocolNamespace;
  uint32 VersionNumberMajor;
  uint32 VersionNumberMinor;
  uint8 Priority;
  uint8 SchemaID;
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( 2 ) % 4)];
# endif
#endif 
};

typedef struct Exi_SAP_supportedAppProtocolReqType Exi_SAP_supportedAppProtocolReqType;

struct Exi_SAP_supportedAppProtocolReqType
{
  Exi_SAP_AppProtocolType* AppProtocol;
};

typedef enum
{
  EXI_SAP_RESPONSE_CODE_TYPE_OK_SUCCESSFUL_NEGOTIATION,
  EXI_SAP_RESPONSE_CODE_TYPE_OK_SUCCESSFUL_NEGOTIATION_WITH_MINOR_DEVIATION,
  EXI_SAP_RESPONSE_CODE_TYPE_FAILED_NO_NEGOTIATION
} Exi_SAP_responseCodeType;

typedef struct Exi_SAP_supportedAppProtocolResType Exi_SAP_supportedAppProtocolResType;

struct Exi_SAP_supportedAppProtocolResType
{
  Exi_SAP_responseCodeType ResponseCode;
  uint8 SchemaID;
  Exi_BitType SchemaIDFlag : 1;
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( (1 * EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE) + (1 * EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE) + 1 ) % 4)];
# endif
#endif 
};

/* PRQA L:IDENTIFIER_NAMES */
/* PRQA L:NAMESPACE */
/* PRQA L:UNION */


#endif
  /* EXI_SAP_SCHEMA_TYPES_H */
