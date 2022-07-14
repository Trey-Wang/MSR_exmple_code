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
 *         File:  Exi_Types.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  types header
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

#if !defined (EXI_TYPES_H)
# define EXI_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Cfg.h"
#if EXI_ENABLE_PBUF_SUPPORT == STD_ON
#include "IpBase_Types.h"
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#ifndef EXI_STRUCTURE_PADDING_16BIT_ENABLE
# define EXI_STRUCTURE_PADDING_16BIT_ENABLE STD_OFF
#endif
#ifndef EXI_STRUCTURE_PADDING_32BIT_ENABLE
# define EXI_STRUCTURE_PADDING_32BIT_ENABLE STD_OFF
#endif
#ifndef EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE
# define EXI_STRUCTURE_PADDING_BIT_FIELD_SIZE_BYTE 1u
#endif
#ifndef EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE
# define EXI_STRUCTURE_PADDING_ENUM_SIZE_BYTE 1u
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if (CPU_TYPE_32 == CPU_TYPE)
# define Exi_UInt64_Available  /* unsigned 64 bit type */
typedef unsigned long long Exi_UInt64;
# define Exi_SInt64_Available  /* signed 64 bit type */
typedef long long   Exi_SInt64;
#else
  /* CPU: no 64 bit support available */
typedef struct
{
  uint32 HiWord;
  uint32 LoWord;
} Exi_UInt64Type;
typedef struct
{
  sint32 HiWord;
  uint32 LoWord;
} Exi_SInt64Type;
typedef Exi_UInt64Type Exi_UInt64;
typedef Exi_SInt64Type Exi_SInt64;
#endif

#if defined (EXI_MAX_NUM_BYTES_BIGINT)
typedef struct
{
  uint16 Length;
  boolean IsNegative;
  uint8 Value[EXI_MAX_NUM_BYTES_BIGINT];
} Exi_BigIntType;
#endif

typedef uint8     Exi_StateType;   /* Exi needs state also without DET */
#if defined (EXI_CONFIG_VARIANT) && (3u == EXI_CONFIG_VARIANT)
typedef struct
{
# if ( STD_ON == EXI_CRC_CHECK )
  uint32       Exi_PrecompileCRC;
  uint32       Exi_LinkCRC;
# endif
} Exi_PostBuildConfigType;
typedef Exi_PostBuildConfigType   Exi_ConfigType;
#else
typedef uint8   Exi_ConfigType;
#endif
  /* defined (EXI_CONFIG_VARIANT) && (3u == EXI_CONFIG_VARIANT) */

/* this type defines the maximum number of bits that can be read or written with a single call to Exi_VBSReadBits or Exi_VBSWriteBits. */
typedef uint32 Exi_BitBufType; 

/* Exi_ReturnType */
typedef uint8  Exi_ReturnType;

#define EXI_E_OK                         0U /* E_OK */
#define EXI_E_NOT_OK                     1U /* undefined error */
#define EXI_E_INV_HEADER              0x10U /* invalid exi header */
#define EXI_E_EOS                     0x11U /* end of exi stream buffer */
#define EXI_E_INT_OVERFLOW            0x12U /* decoded integer too big for target buffer */
#define EXI_E_ARR_OVERFLOW            0x13U /* decoded array too big for target array */
#define EXI_E_STRING_TABLE_LOCAL_HIT  0x14U /* string empty, must be in local table */
#define EXI_E_STRING_TABLE_GLOBAL_HIT 0x15U /* string empty, must be in global table */
#define EXI_E_INV_EVENT_CODE          0x16U /* invalid/unknown event code */
#define EXI_E_BUFFER_SIZE             0x17U /* target buffer to small */
#define EXI_E_DISABLED_FEATURE        0x18U /* feature disabled */

typedef uint8 Exi_NamespaceIdType; /* namespace identifier */
typedef uint16 Exi_RootElementIdType; /* message type identifier */

typedef struct 
{
  /* EXI internal data */
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON) /* Data elements are duplicated in different sort order, to be more memory efficient. */
  P2VAR(IpBase_PbufType, AUTOMATIC, EXI_APPL_VAR) PBufPtr; /* buffer segments of underlying ring-buffer */
  uint32                                          PBufProcessedElementBytes; /* Processed bytes of all PBuf segments (including start offset) */
  uint8                                           PBufIdx; /* current buffer segment */
  /* position in buffer (encoding process) */
  uint8                                           BitPos;  /* bit position in current byte (0-7, 0 = msb) */
  uint16                                          BytePos; /* byte position in exi stream buffer segment */
  
  uint16                                          StartOffset;  /* Buffer start offset, required to calculate stream length */
#else
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR)           BufPtr;  /* exi stream buffer */
  uint16                                          BufLen;  /* exi stream buffer length */
  
  uint16                                          StartOffset;  /* Buffer start offset, required to calculate stream length */
  
  /* position in buffer (encoding process) */
  uint16                                          BytePos; /* byte position in exi stream buffer segment */
  uint8                                           BitPos;  /* bit position in current byte (0-7, 0 = msb) */
#endif

#if (EXI_ENABLE_STREAMING_SUPPORT == STD_ON)
  /* pending operation data */
  uint8                                           BitsComplete;
  uint8                                           CurByte;
#else
# if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
  /* Simple TX streaming relevant data */
  boolean                                         WritingToBufferIsActive;
  uint32                                          StartWriteAtBytePos; /* Start writing at stream byte position, skip writing bytes until this position */
  uint32                                          OverallStreamBytePos; /* Position in the current EXI stream */
  boolean                                         CalculateStreamLength; /* Is enabled in case the user wants to encode the first segment and calculate the stream length at once */
# endif
#endif

  Exi_ReturnType                                  StatusCode; /* this status will be set in case an error was detected */

  boolean                                         EERequired; /* TRUE when calling function needs to encode the next EE tag */

  /* External data, may be used by the application */
  uint32                                          TotalStreamLength; /* EXI stream length of the encoded stream, valid after finalize */
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
  /* Simple TX streaming relevant data */
  uint32                                          CurrentStreamSegmentLen; /* Length of the written segment */
  boolean                                         StreamComplete; /* TRUE in case this was the last segment of the stream */
#endif

} Exi_BSEncodeWorkspaceType;

typedef struct
{
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA) StoragePtr;    /* pointer to the message that will be encoded */
  Exi_RootElementIdType                    RootElementId; /* message type identifier */
} Exi_EncoderInputDataType;

typedef struct 
{
  Exi_BSEncodeWorkspaceType EncWs;
  Exi_EncoderInputDataType  InputData;
} Exi_EncodeWorkspaceType; /* public type used by exi users */

typedef struct 
{
  /* buffer */
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  P2CONST(IpBase_PbufType, AUTOMATIC, EXI_APPL_DATA) PBufPtr; /* buffer segments of underlying ring-buffer */
  uint8                                              PBufIdx; /* current buffer segment */
  uint32                                             PBufProcessedElementBytes; /* Processed bytes of all PBuf segments (including start offset) */
#else
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA)           BufPtr;  /* exi stream buffer */
  uint16                                             BufLen;  /* exi stream buffer length */
#endif

  /* position in buffer (decoding process) */
  uint16                                             BytePos; /* byte position in exi stream buffer segment */
  uint8                                              BitPos;  /* bit position in current byte (0-7) */
  
  uint32                                             TotalStreamLength; /* EXI stream length of the decoded stream, valid after encode is complete */
  uint16                                             StartOffset;  /* Buffer start offset, required to calculate stream length */

#if (EXI_ENABLE_STREAMING_SUPPORT == STD_ON)
  /* pending operation data */
  uint8                                              BitsComplete;
  uint8                                              CurByte;
#endif
  Exi_ReturnType                                     StatusCode;  /* this status will be set in case an error was detected */

  boolean                                            EERequired; /* TRUE when calling function needs to encode the next EE tag */

} Exi_BSDecodeWorkspaceType;

typedef struct 
{

  P2VAR(uint8, AUTOMATIC, EXI_APPL_DATA)   StoragePtr;    /* location where the decoded message will be stored */
  uint16                                   StorageLen;    /* available storage length in bytes */
  uint16                                   StorageOffset; /* offset in storage memory in bytes */
  Exi_RootElementIdType                    SchemaSetId;   /* schema set identifier */
  Exi_RootElementIdType                    RootElementId; /* message type identifier */

} Exi_DecoderOutputDataType;

typedef struct 
{

  Exi_BSDecodeWorkspaceType DecWs;
  Exi_DecoderOutputDataType OutputData;

} Exi_DecodeWorkspaceType; /* public type used by exi users */

typedef unsigned int Exi_BitType;
typedef void (*Exi_EncoderFctType)(Exi_EncodeWorkspaceType* EncWsPtr);
typedef void (*Exi_DecoderFctType)(Exi_DecodeWorkspaceType* DecWsPtr);

#endif 
  /* EXI_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi_Types.h
 *********************************************************************************************************************/
