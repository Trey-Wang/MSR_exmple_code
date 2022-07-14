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
 *         File:  Exi_SchemaTypes.h
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

#if !defined (EXI_SCHEMA_TYPES_H)
# define EXI_SCHEMA_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi.h"
#include "Exi_Types.h"

#include "Exi_SAP_SchemaTypes.h"
#include "Exi_XMLSIG_SchemaTypes.h"
#include "Exi_ISO_SchemaTypes.h"
#include "Exi_DIN_SchemaTypes.h"
/* PRQA S 0779 IDENTIFIER_NAMES */ /* MD_Exi_5.1 */
/* PRQA S 0780 NAMESPACE */ /* MD_Exi_5.6 */
/* PRQA S 0750 UNION */ /* MD_Exi_18.4 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Generic */
#define EXI_SCHEMA_GENERIC_ELEMENT_TYPE (Exi_RootElementIdType)647U
#define EXI_SCHEMA_GENERIC_STRING_TYPE (Exi_RootElementIdType)648U
#define EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE (Exi_RootElementIdType)649U
#define EXI_UNKNOWN_SCHEMA_SET_TYPE (Exi_RootElementIdType)7U
#ifndef EXI_MAXOCCURS_GENERICELEMENT
  #define EXI_MAXOCCURS_GENERICELEMENT EXI_MAXOCCURS_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_BASE64BINARY
  #define EXI_MAX_BUFFER_SIZE_BASE64BINARY EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif
#ifndef EXI_MAX_BUFFER_SIZE_STRING
  #define EXI_MAX_BUFFER_SIZE_STRING EXI_MAX_BUFFER_SIZE_UNBOUNDED
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct Exi_GenericElementType Exi_GenericElementType;

struct Exi_GenericElementType
{
  Exi_RootElementIdType QNameType;
  void* GenericQName;
  Exi_RootElementIdType ContentType;
  void* GenericContent;
  Exi_GenericElementType* NextGenericElement;
};

typedef struct Exi_GenericStringType Exi_GenericStringType;

struct Exi_GenericStringType
{
  uint16 Length;
  uint8 Buffer[128];
};

struct Exi_stringType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_STRING];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_STRING ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_STRING ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_STRING ) + 2 ) % 4)];
# endif
#endif 
};

struct Exi_base64BinaryType
{
  uint16 Length;
  uint8 Buffer[EXI_MAX_BUFFER_SIZE_BASE64BINARY];
#if (STD_ON == EXI_STRUCTURE_PADDING_16BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_BASE64BINARY ) ) % 2 ) )
  uint8 Exi_Padding16Bit[1];
# endif
#endif 
#if (STD_ON == EXI_STRUCTURE_PADDING_32BIT_ENABLE)
# if (0 != ( ( ( EXI_MAX_BUFFER_SIZE_BASE64BINARY ) + 2 ) % 4 ) )
  uint8 Exi_Padding32Bit[4 - (( ( EXI_MAX_BUFFER_SIZE_BASE64BINARY ) + 2 ) % 4)];
# endif
#endif 
};

/* PRQA L:IDENTIFIER_NAMES */
/* PRQA L:NAMESPACE */
/* PRQA L:UNION */

#endif
  /* EXI_SCHEMA_TYPES_H */
