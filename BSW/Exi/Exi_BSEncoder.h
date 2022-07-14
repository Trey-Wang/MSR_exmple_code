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
 *         File:  Exi_BSEncoder.h
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

#if !defined (EXI_BS_ENCODER_H)
# define EXI_BS_ENCODER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"
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
 *  Exi_VBSEncodeInitWorkspace()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeInitWorkspace( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  P2VAR(IpBase_PbufType, AUTOMATIC, EXI_APPL_VAR) PBufPtr,
#else
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr,
  uint16 BufLen,
#endif
  uint16 ByteOffset
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
 ,uint32 StartWriteAtStreamPos
 ,boolean CalculateStreamLength
#endif
);

/**********************************************************************************************************************
 *  Exi_VBSEncodeGetWorkspaceBitPos()
 **********************************************************************************************************************/
extern FUNC(uint32, EXI_CODE) Exi_VBSEncodeGetWorkspaceBitPos( 
  P2CONST(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr);

/**********************************************************************************************************************
 *  Exi_VBSWriteHeader()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSWriteHeader( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr);

/**********************************************************************************************************************
 *  Exi_VBSWriteBits()
 **********************************************************************************************************************/
extern FUNC(uint8, EXI_CODE) Exi_VBSWriteBits( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType BitBuf,
  uint8 BitCount);

/**********************************************************************************************************************
 *  Exi_VBSEncodeBytes()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeBytes(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA) DataPtr,
  uint16 DataLen);

/**********************************************************************************************************************
 *  Exi_VBSEncodeBool()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeBool(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  boolean Value);

/**********************************************************************************************************************
 *  Exi_VBSEncodeInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeInt(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  sint32 Value);

/**********************************************************************************************************************
 *  Exi_VBSEncodeInt64()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeInt64(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_SInt64 Value);

/**********************************************************************************************************************
 *  Exi_VBSEncodeBigInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeBigInt(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(Exi_BigIntType, AUTOMATIC, EXI_APPL_DATA) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSEncodeUInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeUInt( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType Value);

/**********************************************************************************************************************
 *  Exi_VBSEncodeUInt64()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeUInt64( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_UInt64 Value);

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSDecodeInt( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint32, AUTOMATIC, EXI_APPL_VAR) ValuePtr);

/**********************************************************************************************************************
 *  Exi_VBSEncodeUIntN()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeUIntN( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType BitBuf,
  uint8 BitCount);

/**********************************************************************************************************************
 *  Exi_VBSEncodeStringOnly()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeStringOnly( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen);

/**********************************************************************************************************************
 *  Exi_VBSEncodeString()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeString( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen);

/**********************************************************************************************************************
 *  Exi_VBSEncodeStringValue()
 **********************************************************************************************************************/
extern FUNC(void, EXI_CODE) Exi_VBSEncodeStringValue( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen);

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */
#endif 
  /* EXI_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi.h
 **********************************************************************************************************************/
