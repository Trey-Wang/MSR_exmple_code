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
 *         File:  Exi_BSDecoder.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  module header
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

#if !defined (EXI_BS_DECODER_H)
# define EXI_BS_DECODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"
#include "Exi_SchemaDecoder.h"
#include "Exi_Lcfg.h"
#include "Exi_Priv.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeInitWorkspace()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInitWorkspace( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  P2CONST(IpBase_PbufType, AUTOMATIC, EXI_APPL_VAR) PBufPtr,
#else
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr,
  uint16 BufLen,
#endif
  uint16 ByteOffset
);

/**********************************************************************************************************************
 *  Exi_VBSDecodeSkipBits()
 **********************************************************************************************************************/
extern FUNC(uint32, EXI_CODE) Exi_VBSDecodeSkipBits( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint32 BitCount);

/**********************************************************************************************************************
 *  Exi_VBSDecodeGetWorkspaceBitPos()
 **********************************************************************************************************************/
extern FUNC(uint32, EXI_CODE) Exi_VBSDecodeGetWorkspaceBitPos( 
  P2CONST(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_DATA) DecWsPtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeSetWorkspaceBitPos()
 **********************************************************************************************************************/
extern FUNC(uint32, EXI_CODE) Exi_VBSDecodeSetWorkspaceBitPos( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint32 BitPos);

/**********************************************************************************************************************
 *  Exi_VBSReadHeader()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSReadHeader( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr);

/**********************************************************************************************************************
 *  Exi_VBSReadBits()
 **********************************************************************************************************************/
extern FUNC(uint8, EXI_CODE) Exi_VBSReadBits( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) BitBufPtr,
  uint8 BitCount);

/**********************************************************************************************************************
 *  Exi_VBSDecodeBytes()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeBytes(
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) BufLenPtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeBool()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeBool(
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(boolean, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt8()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInt8( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint8, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt16()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInt16( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint16, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt32()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInt32( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint32, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt64()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInt64( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_SInt64, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeBigInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeBigInt( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BigIntType, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUInt( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt8()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUInt8( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt16()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUInt16( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt32()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUInt32( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint32, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt64()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUInt64( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_UInt64, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeUIntN()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeUIntN( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) BitBufPtr,
  uint8 BitCount);

/**********************************************************************************************************************
 *  Exi_VBSDecodeStringOnly()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeStringOnly( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr,
  uint16 StrLen);

/**********************************************************************************************************************
 *  Exi_VBSDecodeString()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeString( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeStringValue()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeStringValue( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeQName()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeQName( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr);

/**********************************************************************************************************************
 *  Exi_VBSDecodeSchemaDeviation()
 **********************************************************************************************************************/
#if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
extern FUNC(void, EXI_CODE) Exi_VBSDecodeSchemaDeviation(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) ExiEventCode,
  Exi_BitBufType SchemaDeviationEventCode,
  uint8 EventCodeBitSize,
  boolean StartElement,
  boolean OptionalElement,
  boolean AttributesAllowed,
  uint8 KnownAttributesNum,
  uint8 SchemaSetId);
#endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_VBSDecodeGenericElement()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeGenericElement(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint8 SchemaSetId,
  boolean SkipElement);

/**********************************************************************************************************************
 *  Exi_VBSDecodeGenericElementContent()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeGenericElementContent(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint8 SchemaSetId,
  boolean SkipContent,
  Exi_GenericElementType* GenericElement);

/**********************************************************************************************************************
 *  Exi_CalculateBitSize()
 **********************************************************************************************************************/
extern FUNC(uint8, EXI_CODE) Exi_CalculateBitSize(uint8 MaxValue);

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */
#endif 
  /* EXI_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi.h
 **********************************************************************************************************************/
