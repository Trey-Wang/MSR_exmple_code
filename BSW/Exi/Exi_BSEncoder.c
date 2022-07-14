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
 *         File:  Exi_BSEncoder.c
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  
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

#define EXI_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

#include "Exi.h"
#include "Exi_Priv.h"
#include "Exi_Types.h"
#include "Exi_BSEncoder.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_BSEncoder.c are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define EXI_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_VBSEncodeInitWorkspace()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeInitWorkspace( 
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
  )
{
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 9 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,         EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != PBufPtr,          EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(0 != PBufPtr->totLen,         EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid(ByteOffset < PBufPtr->totLen, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
# else
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != BufPtr,           EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(0 != BufLen,                  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid(ByteOffset < BufLen,          EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
# endif
#endif

  EncWsPtr->StartOffset = ByteOffset;
  EncWsPtr->TotalStreamLength = 0;
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  EncWsPtr->PBufIdx = 0;
  EncWsPtr->PBufPtr = PBufPtr;
  EncWsPtr->PBufProcessedElementBytes = 0;
  /* Check if offset position is in current PBuf segment */
  if ( ByteOffset >= PBufPtr->len )
  {
    /* Get the PBuf segment where the offset is inside */
    while (ByteOffset >= PBufPtr[EncWsPtr->PBufIdx].len)
    {
      EncWsPtr->PBufProcessedElementBytes += PBufPtr[EncWsPtr->PBufIdx].len;
      ByteOffset -= PBufPtr[EncWsPtr->PBufIdx].len;
      EncWsPtr->PBufIdx++;
    }
  }
#else
  EncWsPtr->BufPtr = BufPtr;
  EncWsPtr->BufLen = BufLen;
#endif

  EncWsPtr->BytePos = ByteOffset;
  EncWsPtr->BitPos = 0;
  
  EncWsPtr->EERequired = TRUE;
  EncWsPtr->StatusCode = EXI_E_OK;
  
#if (EXI_ENABLE_STREAMING_SUPPORT == STD_ON)
  EncWsPtr->BitsComplete = 0;
#else
# if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
  if (0 == StartWriteAtStreamPos)
  {
    EncWsPtr->WritingToBufferIsActive = TRUE;
    EncWsPtr->StartWriteAtBytePos = 0;
  }
  else
  {
    EncWsPtr->WritingToBufferIsActive = FALSE;
    EncWsPtr->StartWriteAtBytePos = StartWriteAtStreamPos;
  }
  EncWsPtr->OverallStreamBytePos = 0;
  EncWsPtr->CurrentStreamSegmentLen = 0;
  EncWsPtr->StreamComplete = FALSE;
  EncWsPtr->CalculateStreamLength = CalculateStreamLength;
# endif
#endif
  
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSWriteHeader()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSWriteHeader( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr)
{
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  (void)Exi_VBSWriteBits(EncWsPtr, (Exi_BitBufType)EXI_HEADER_BYTE, EXI_BITS_IN_BYTE);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSWriteBits()
 **********************************************************************************************************************/
FUNC(uint8, EXI_CODE) Exi_VBSWriteBits( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType BitBuf,
  uint8 BitCount)
{
  uint8 BitsAvail;
  uint8 BitsRemain;
  uint8 Mask;
  P2VAR(uint8, AUTOMATIC, EXI_APPL_DATA) BufPtr;
  uint32 BuffTotalLen;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != EncWsPtr,                     EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
  Exi_CheckInternalDetErrorReturnValue(BitCount <= (sizeof(Exi_BitBufType) * 8), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM,   0);
#endif
  
  BitsRemain = BitCount;
  
  /* Check status code for errors */
  if(EXI_E_OK == EncWsPtr->StatusCode)
  {
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    BufPtr = EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].payload;
    BuffTotalLen = EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].totLen;
#else
    BufPtr = EncWsPtr->BufPtr;
    BuffTotalLen = EncWsPtr->BufLen;
#endif

    while (0 < BitsRemain)
    {
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
      /* check if the current byte shall be written */
      if (   (EncWsPtr->StartWriteAtBytePos == EncWsPtr->OverallStreamBytePos)
          && ( (
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
                EncWsPtr->PBufProcessedElementBytes + 
#endif
                EncWsPtr->BytePos) < BuffTotalLen) )
      {
        /* Byte is in writable range */
        if (FALSE == EncWsPtr->WritingToBufferIsActive)
        {
          EncWsPtr->WritingToBufferIsActive = TRUE;
        }
#endif
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
        if (EncWsPtr->BytePos >= EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].len)
        {
          /* end of current PBuf segment reached */
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
          /* Check if data outside the current segment was read */
          Exi_CheckInternalDetErrorContinue(EncWsPtr->BytePos == EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].len, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INVALID_PBBUF_POS );
          Exi_CheckInternalDetErrorContinue(EncWsPtr->BitPos == 0, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INVALID_PBBUF_POS );
#endif
      
          /* mark current segment as processed */
          EncWsPtr->PBufProcessedElementBytes += EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].len;
          /* check if more PBuf segments are available */
          if (EncWsPtr->PBufProcessedElementBytes < EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].totLen)
          {
            /* switch from current PBuf segment to next segement */
            EncWsPtr->PBufIdx++;
            BufPtr = EncWsPtr->PBufPtr[EncWsPtr->PBufIdx].payload;
            /* set the byte pos to the beginning of the new PBuf segment */
            EncWsPtr->BytePos = 0;
          }
          else
          {
            /* end of PBuf segments reached, we cannot write remaining bits */
            break;
          }
        }
#else
        if (EncWsPtr->BytePos >= BuffTotalLen)
        {
          /* end of buffer reached, we cannot write remaining bits */
          break;
        }
#endif

        if (0 == EncWsPtr->BitPos)
        {
          /* start writing at beginning of current byte */
     
          if (EXI_BITS_IN_BYTE < BitsRemain)
          {
            /* we have to write more than 8 bits */
     
            BitsRemain -= EXI_BITS_IN_BYTE;
            
            /* write complete byte */
            BufPtr[EncWsPtr->BytePos] = (uint8)(BitBuf >> BitsRemain);
     
            /* continue with next byte */
            EncWsPtr->BytePos++;
          }
          else
          {
            /* all bits to write fit into current byte */
     
            if (EXI_BITS_IN_BYTE == BitsRemain)
            {
              BufPtr[EncWsPtr->BytePos] = (uint8)(BitBuf);
              /* complete byte has been written, continue writing at beginning of next byte */
              EncWsPtr->BytePos++;
            }
            else
            {
              BufPtr[EncWsPtr->BytePos] = (uint8)(BitBuf << (EXI_BITS_IN_BYTE - BitsRemain));
              /* there are free bits in current byte, only advance BitPos */
              EncWsPtr->BitPos = BitsRemain;
            }
     
            /* finished, all bits have been written */
            BitsRemain = 0;
          }
        }
        else
        {
          /* start writing somewhere in current byte */
     
          /* determine unused bits in current byte */
          BitsAvail = EXI_BITS_IN_BYTE - EncWsPtr->BitPos;
          Mask = (uint8)(0xFF >> EncWsPtr->BitPos);
     
          if (BitsRemain > BitsAvail)
          {
            /* we have to write more bits than left in current byte */
     
            BitsRemain -= BitsAvail;
     
            /* fill up current byte */
            BufPtr[EncWsPtr->BytePos] = (uint8)((BufPtr[EncWsPtr->BytePos] & (uint8)(~Mask)) | ((BitBuf >> BitsRemain) & Mask));
     
            /* all bits in current byte are used, continue reading at beginning of next byte */
            EncWsPtr->BitPos = 0;
            EncWsPtr->BytePos++;
          }
          else
          {
            /* all remaining bits fit into current byte */
            
            if (BitsAvail == BitsRemain)
            {
              BufPtr[EncWsPtr->BytePos] = (uint8)((BufPtr[EncWsPtr->BytePos] & (uint8)(~Mask)) | ((BitBuf) & Mask));
               /* all bits in current byte are used, continue reading at beginning of next byte */
              EncWsPtr->BytePos++;
              EncWsPtr->BitPos = 0;
            }
            else
            {
              BufPtr[EncWsPtr->BytePos] = (uint8)((BufPtr[EncWsPtr->BytePos] & (uint8)(~Mask)) | ((BitBuf << (BitsAvail - BitsRemain)) & Mask));
              /* there are unused bits left in current byte, only advance BitPos */
              EncWsPtr->BitPos += BitsRemain;
            }
     
            /* finished, all bits have been written */
            BitsRemain = 0;
          } 
        }
#if (EXI_ENABLE_SIMPLE_TX_STREAMING_SUPPORT == STD_ON)
      }
      else
      {
        /* this byte shall not be written */
  
        /* Check if the stream segment was finished with last byte */
        if (TRUE == EncWsPtr->WritingToBufferIsActive)
        {
          /* Add the written Length to the streaming parameters */
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
          EncWsPtr->CurrentStreamSegmentLen = EncWsPtr->PBufProcessedElementBytes + EncWsPtr->BytePos - EncWsPtr->StartOffset;
          EncWsPtr->OverallStreamBytePos += EncWsPtr->CurrentStreamSegmentLen;
#else
          EncWsPtr->CurrentStreamSegmentLen = EncWsPtr->BytePos - EncWsPtr->StartOffset;
          EncWsPtr->OverallStreamBytePos += EncWsPtr->CurrentStreamSegmentLen;
#endif
          /* Set streaming state info parameters */
          EncWsPtr->WritingToBufferIsActive = FALSE;
          EncWsPtr->StreamComplete = FALSE;

          /* Check if the stream length shall be calculated */
          if (FALSE == EncWsPtr->CalculateStreamLength)
          {
            /* No, skip writing here, do not waste runtime */
            EncWsPtr->StatusCode = EXI_E_EOS;
            break;
          }
        }
        
        if (0 == EncWsPtr->BitPos)
        {
          /* start writing at beginning of current byte */
     
          if (EXI_BITS_IN_BYTE < BitsRemain)
          {
            /* we have to write more than 8 bits */
     
            BitsRemain -= EXI_BITS_IN_BYTE;
            
            /* continue with next byte */
            EncWsPtr->OverallStreamBytePos++;
          }
          else
          {
            /* all bits to write fit into current byte */
     
            if (EXI_BITS_IN_BYTE == BitsRemain)
            {
              /* complete byte has been written, continue writing at beginning of next byte */
              EncWsPtr->OverallStreamBytePos++;
            }
            else
            {
              /* there are free bits in current byte, only advance BitPos */
              EncWsPtr->BitPos = BitsRemain;
            }
     
            /* finished, all bits have been written */
            BitsRemain = 0;
          }
        }
        else
        {
          /* start writing somewhere in current byte */
     
          /* determine unused bits in current byte */
          BitsAvail = EXI_BITS_IN_BYTE - EncWsPtr->BitPos;
     
          if (BitsRemain > BitsAvail)
          {
            /* we have to write more bits than left in current byte */
     
            BitsRemain -= BitsAvail;
    
            /* all bits in current byte are used, continue reading at beginning of next byte */
            EncWsPtr->BitPos = 0;
            EncWsPtr->OverallStreamBytePos++;
          }
          else
          {
            /* all remaining bits fit into current byte */
            
            if (BitsAvail == BitsRemain)
            {
              /* all bits in current byte are used, continue reading at beginning of next byte */
              EncWsPtr->OverallStreamBytePos++;
              EncWsPtr->BitPos = 0;
            }
            else
            {
              /* there are unused bits left in current byte, only advance BitPos */
              EncWsPtr->BitPos += BitsRemain;
            }
     
            /* finished, all bits have been written */
            BitsRemain = 0;
          } 
        }
      }
#endif
    }

    if((0 != BitsRemain) && (EXI_E_OK == EncWsPtr->StatusCode))
    {
      EncWsPtr->StatusCode = EXI_E_EOS;
    }
  }

  return BitCount - BitsRemain;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.1 BINARY === */

/* UInt + Bytes... (see 7.1.6) */

/**********************************************************************************************************************
 *  Exi_VBSEncodeBytes()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeBytes(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_DATA) DataPtr,
  uint16 DataLen)
{
  uint16 i;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DataPtr,   EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif
  
#if (EXI_ENABLE_STREAMING_SUPPORT == STD_ON)
#error "EXI_ENABLE_STREAMING_SUPPORT not supported by Exi_VBSEncodeBytes()"
#endif

    Exi_VBSEncodeUInt(EncWsPtr, DataLen);

    for (i = 0; i < DataLen; ++i)
    {
      if (EXI_BITS_IN_BYTE != Exi_VBSWriteBits(EncWsPtr, DataPtr[i], EXI_BITS_IN_BYTE))
      {
        /* StatusCode updated not required here, already set in Exi_VBSWriteBits */
        break;
      }
    }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.2 BOOLEAN === */

/**********************************************************************************************************************
 *  Exi_VBSEncodeBool()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeBool(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  boolean Value)
{
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  (void)Exi_VBSWriteBits(EncWsPtr, Value, 1);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.3 DECIMAL === */

/* The Decimal datatype representation is a Boolean sign (see 7.1.2 Boolean) followed by two Unsigned Integers (see 7.1.6 Unsigned Integer). */

/* === 7.1.4 FLOAT === */

/* Int + Int (see 7.1.5) */

/* === 7.1.5 INTEGER === */

/* Boolean (see 7.1.2) + UInt (see 7.1.6) */ 

/**********************************************************************************************************************
 *  Exi_VBSEncodeInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeInt(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  sint32 Value)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  if (0 > Value) 
  {
    /* write negative value */

    Exi_VBSEncodeBool(EncWsPtr, TRUE);
    Exi_VBSEncodeUInt(EncWsPtr, (uint32)((Value + 1) * -1));
  }
  else
  {
    /* write positive value */

    Exi_VBSEncodeBool(EncWsPtr, FALSE);
    Exi_VBSEncodeUInt(EncWsPtr, (uint32)(Value));
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSEncodeInt64()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeInt64(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_SInt64 Value)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

#if defined (Exi_SInt64_Available)
  if (0 > Value) 
#else
  if (0 > Value.HiWord) 
#endif
  {
    /* write negative value */

    Exi_VBSEncodeBool(EncWsPtr, TRUE);
#if defined (Exi_SInt64_Available)
    Exi_VBSEncodeUInt64(EncWsPtr, (Exi_UInt64)((Value + 1) * -1));
#else
    {
      Exi_UInt64 TempValue;
      TempValue.HiWord = (uint32)(~Value.HiWord);
      TempValue.LoWord = ~Value.LoWord;
      Exi_VBSEncodeUInt64(EncWsPtr, TempValue);
    }
#endif
  }
  else
  {
    /* write positive value */

    Exi_VBSEncodeBool(EncWsPtr, FALSE);
#if defined (Exi_SInt64_Available)
    Exi_VBSEncodeUInt64(EncWsPtr, (Exi_UInt64)(Value));
#else
    {
      Exi_UInt64 TempValue;
      TempValue.HiWord = (uint32)(Value.HiWord);
      TempValue.LoWord = Value.LoWord;
      Exi_VBSEncodeUInt64(EncWsPtr, TempValue);
    }
#endif
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSEncodeBigInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeBigInt(
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(Exi_BigIntType, AUTOMATIC, EXI_APPL_DATA) ValuePtr)
{
  uint16 Index;
  uint16 ValueLength;
  uint8 byte;
  uint8 Multiplier = 0;
  uint8 BitShift;
  boolean Decrement;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSEncodeBool(EncWsPtr, ValuePtr->IsNegative);
  Decrement = ValuePtr->IsNegative;
  Index = sizeof(ValuePtr->Value) - 1;
  ValueLength = ValuePtr->Length;
  if((TRUE == ValuePtr->IsNegative) && (ValueLength > 1))
  {
    /* if the last byte equals 0x01 and all other bytes equal 0x00 ValueLength need to be decremented */
    if(0x01 == ValuePtr->Value[sizeof(ValuePtr->Value) - ValueLength])
    {
      boolean AllZeros = TRUE;
      uint16 i = (sizeof(ValuePtr->Value) - ValueLength) + 1;
      for(; i <= (Index); i++)
      {
        if(0x00 != ValuePtr->Value[i])
        {
          AllZeros = FALSE;
          break;
        }
      }
      if(TRUE == AllZeros)
      {
        ValueLength--;
      }
    }
  }
  do
  {
    BitShift = (Multiplier % 8);
    byte = (uint8)(ValuePtr->Value[Index] >> BitShift);
    if(0 == BitShift)
    {
      byte &= 0x7F; /* Mask 7 least significant bits _1111111 */
    }
    else if((1 < BitShift) && (ValueLength > (sizeof(ValuePtr->Value) - Index)))
    {
      Index--;
      byte |= (uint8)((uint8)(ValuePtr->Value[Index] << (8 - BitShift)) & (uint8)0x7F); /* Mask 7 least significant bits _1111111 */
    }
    else if((1 == BitShift) && (ValueLength > (sizeof(ValuePtr->Value) - Index))) /* ESCAN00068523 */
    {
      Index--;
    }
    else
    {
      /* these are the last bits */
    }

    if(TRUE == Decrement)
    {
      if(0x00 != byte)
      {
        Decrement = FALSE;
        byte--;
      }
      else
      {
        byte = 0x7F;
      }
    }

    if ((ValueLength > (sizeof(ValuePtr->Value) - Index)) || ((0 == BitShift) && ((ValuePtr->Value[Index] & 0x80) != 0x00)))
    {
      byte |= (uint8)0x80; /* Set most significant bit (more bytes indication bit) */
    }
    Multiplier += 7; 

    if(EXI_BITS_IN_BYTE != Exi_VBSWriteBits(EncWsPtr, byte, EXI_BITS_IN_BYTE))
    {
      break;
    }
  }while (0x80 == (byte & 0x80));
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.6 UNSIGNED INTEGER === */

/**********************************************************************************************************************
 *  Exi_VBSEncodeUInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeUInt( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType Value)
{
  uint8 byte;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  do
  {
    byte = (uint8)(Value & 0x7F); /* Mask 7 least significant bits _1111111 */

    if (Value >= 128)
    {
      byte |= (uint8)0x80; /* Set most significant bit (more bytes indication bit) */
      Value >>= 7; 
    }
    
    if(EXI_BITS_IN_BYTE != Exi_VBSWriteBits(EncWsPtr, byte, EXI_BITS_IN_BYTE))
    {
      break;
    }
  } while (0x80 == (byte & 0x80));

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSEncodeUInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeUInt64( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_UInt64 Value)
{
  uint8 byte;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  do
  {
#if defined (Exi_UInt64_Available)
    byte = (uint8)(Value & 0x7F); /* Mask 7 least significant bits _1111111 */

    if (Value >= 128)
    {
      byte |= (uint8)0x80; /* Set most significant bit (more bytes indication bit) */
      Value >>= 7; 
    }
#else
    byte = (uint8)(Value.LoWord & 0x7F); /* Mask 7 least significant bits _1111111 */

    if ((Value.LoWord >= 128) || (Value.HiWord != 0))
    {
      byte |= (uint8)0x80; /* Set most significant bit (more bytes indication bit) */
      Value.LoWord = ((Value.HiWord << 25) | (Value.LoWord >> 7)); 
      Value.HiWord >>= 7;
    }
#endif
    
    if(EXI_BITS_IN_BYTE != Exi_VBSWriteBits(EncWsPtr, byte, EXI_BITS_IN_BYTE))
    {
      break;
    }
  } while (0x80 == (byte & 0x80));

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.7 QNAME === */

/* String (see 7.1.10) */

/* === 7.1.8 DATE TIME === */

/* Year           Int */
/* MonthDay       UInt(9) */
/* Time           UInt(17) */
/* FractionalSecs UInt */
/* TimeZone       UInt(11) */
/* presence       Boolean */

/* === 7.1.9 N-BIT UNSIGNED INTEGER === */

/**********************************************************************************************************************
 *  Exi_VBSEncodeUIntN()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeUIntN( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  Exi_BitBufType BitBuf,
  uint8 BitCount)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,                     EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(BitCount <= (sizeof(Exi_BitBufType) * 8), EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
#endif

  (void)Exi_VBSWriteBits(EncWsPtr, BitBuf, BitCount);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.10 STRING === */

/**********************************************************************************************************************
 *  Exi_VBSEncodeStringOnly()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeStringOnly( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen)
{
  uint16 i;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  i = 0;

  while (i < StrBufLen)
  {

    Exi_VBSEncodeUIntN(EncWsPtr, (Exi_BitBufType)StrBufPtr[i], EXI_BITS_IN_BYTE);

    i++;
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSEncodeString()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeString( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif
  
    Exi_VBSEncodeUInt(EncWsPtr, (Exi_BitBufType)StrBufLen);
    Exi_VBSEncodeStringOnly(EncWsPtr, StrBufPtr, StrBufLen);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSEncodeStringValue()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSEncodeStringValue( 
  P2VAR(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr,
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  uint16 StrBufLen)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr, EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSEncodeUInt(EncWsPtr, (Exi_BitBufType)(StrBufLen + 2));
  Exi_VBSEncodeStringOnly(EncWsPtr, StrBufPtr, StrBufLen);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.11 LIST === */

/**********************************************************************************************************************
 *  Exi_VBSEncodeGetWorkspaceBitPos()
 **********************************************************************************************************************/
FUNC(uint32, EXI_CODE) Exi_VBSEncodeGetWorkspaceBitPos( 
  P2CONST(Exi_BSEncodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) EncWsPtr)
{
  uint16 BytePosition;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != EncWsPtr,  EXI_ENCODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
#endif

  BytePosition = EncWsPtr->BytePos;

#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  if (EncWsPtr->PBufIdx != 0)
  {
    uint8 PBufIdx;

    /* Add length of all previouse PBuf segments */
    for (PBufIdx = 0; PBufIdx < EncWsPtr->PBufIdx; PBufIdx++)
    {
      BytePosition += EncWsPtr->PBufPtr[PBufIdx].len;
    }
  }
#endif

  return (BytePosition << 3) + EncWsPtr->BitPos;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Exi_BSEncoder.c
 **********************************************************************************************************************/
