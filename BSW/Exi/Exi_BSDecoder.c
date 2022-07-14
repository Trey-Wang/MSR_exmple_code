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
 *         File:  Exi_BSDecoder.c
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
#include "Exi_BSDecoder.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if ( (EXI_SW_MAJOR_VERSION != 3u) || (EXI_SW_MINOR_VERSION != 2u) || (EXI_SW_PATCH_VERSION != 5u) )
  #error "Vendor specific version numbers of Exi.h and Exi_BSDecoder.c are inconsistent"
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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  Exi_VBSDecodeInitWorkspace()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeInitWorkspace( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  P2CONST(IpBase_PbufType, AUTOMATIC, EXI_APPL_VAR) PBufPtr,
#else
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr,
  uint16 BufLen,
#endif
  uint16 ByteOffset)
{
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 9 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,         EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
# if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != PBufPtr,          EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(0 != PBufPtr->totLen,         EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid(ByteOffset < PBufPtr->totLen, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
# else
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != BufPtr,           EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(0 != BufLen,                  EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
  Exi_CheckInternalDetErrorReturnVoid(ByteOffset < BufLen,          EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
# endif
#endif
  
  DecWsPtr->StartOffset = ByteOffset;
  DecWsPtr->TotalStreamLength = 0;
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  DecWsPtr->PBufIdx = 0;
  DecWsPtr->PBufPtr = PBufPtr;
  DecWsPtr->PBufProcessedElementBytes = 0;
  /* Check if offset position is in current PBuf segment */
  if ( ByteOffset >= PBufPtr->len )
  {
    /* Get the PBuf segment where the offset is inside */
    while (ByteOffset >= PBufPtr[DecWsPtr->PBufIdx].len)
    {
      DecWsPtr->PBufProcessedElementBytes += PBufPtr[DecWsPtr->PBufIdx].len;
      ByteOffset -= PBufPtr[DecWsPtr->PBufIdx].len;
      DecWsPtr->PBufIdx++;
    }
  }
#else
  DecWsPtr->BufPtr = BufPtr;
  DecWsPtr->BufLen = BufLen;
#endif

  DecWsPtr->BytePos = ByteOffset;
  DecWsPtr->BitPos = 0;
  
  DecWsPtr->EERequired = TRUE;
  DecWsPtr->StatusCode = EXI_E_OK;

#if (EXI_ENABLE_STREAMING_SUPPORT == STD_ON)
  DecWsPtr->BitsComplete = 0;
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSReadHeader()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSReadHeader( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  /* read one byte header */
  (void)Exi_VBSReadBits(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE);
  if ((EXI_HEADER_BYTE != (uint8)BitBuf) && (EXI_E_OK == DecWsPtr->StatusCode))
  {
    /* header byte value does not match */
    DecWsPtr->StatusCode = EXI_E_INV_HEADER;
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSReadBits()
 **********************************************************************************************************************/
FUNC(uint8, EXI_CODE) Exi_VBSReadBits( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) BitBufPtr,
  uint8 BitCount)
{
  uint8 BitsRemain; /* number of bits that have to be read until BitCount is reached */
  uint8 BitsAvail;  /* number of available bits in current byte */
  P2CONST(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != DecWsPtr,                     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != BitBufPtr,                    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
  Exi_CheckInternalDetErrorReturnValue(BitCount <= (sizeof(Exi_BitBufType) * 8), EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM,   0);
#endif

  BitsRemain = BitCount; 

#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  BufPtr = DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].payload;
#else
  BufPtr = DecWsPtr->BufPtr;
#endif

  while (0 < BitsRemain)
  {
#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
    if (DecWsPtr->BytePos >= DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len)
    {
      /* end of current PBuf segment reached */
#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
      /* Check if data outside the current segment was read */
      Exi_CheckInternalDetErrorContinue(DecWsPtr->BytePos == DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INVALID_PBBUF_POS );
      Exi_CheckInternalDetErrorContinue(DecWsPtr->BitPos == 0, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INVALID_PBBUF_POS );
#endif
      
      /* mark current segment as processed */
      DecWsPtr->PBufProcessedElementBytes += DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len;
      /* check if more PBuf segments are available */
      if (DecWsPtr->PBufProcessedElementBytes < DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].totLen)
      {
        /* switch from current PBuf segment to next segement */
        DecWsPtr->PBufIdx++;
        BufPtr = DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].payload;
        /* set the byte pos to the beginning of the new PBuf segment */
        DecWsPtr->BytePos = 0;
      }
      else
      {
        /* end of PBuf segments reached, we cannot read remaining bits */
        break;
      }
    }
#else
    if (DecWsPtr->BytePos >= DecWsPtr->BufLen)
    {
      /* end of buffer reached, we cannot read remaining bits */
      break;
    }
#endif
    
    if (0 == DecWsPtr->BitPos)
    {
      /* start reading at beginning of current byte */

      if (EXI_BITS_IN_BYTE < BitsRemain)
      {
        /* we have to read more than 8 bits */
        
        /* read complete byte */
        *BitBufPtr <<= EXI_BITS_IN_BYTE;
        *BitBufPtr |= BufPtr[DecWsPtr->BytePos];
        
        BitsRemain -= EXI_BITS_IN_BYTE;
        
        /* continue with next byte */
        DecWsPtr->BytePos++;
      }
      else
      {
        /* all bits to read are in current byte */

        /* read all remaining bits */
        *BitBufPtr <<= BitsRemain;

        if (EXI_BITS_IN_BYTE == BitsRemain)
        {
          *BitBufPtr |= BufPtr[DecWsPtr->BytePos];
          /* we read the complete byte, continue reading at beginning of next byte */
          DecWsPtr->BytePos++;
        }
        else
        {
          *BitBufPtr |= (BufPtr[DecWsPtr->BytePos] >> (EXI_BITS_IN_BYTE - BitsRemain));
          /* there are unread bits left in current byte, only advance BitPos */
          DecWsPtr->BitPos = BitsRemain;
        }

        /* finished, all bits have been read */
        BitsRemain = 0;
      }
    }
    else
    {
      /* start reading somewhere in current byte */

      if (EXI_BITS_IN_BYTE < (DecWsPtr->BitPos + BitsRemain))
      {
        /* we have to read more bits than left in current byte */

        /* read all available bits in current byte */
        BitsAvail = EXI_BITS_IN_BYTE - DecWsPtr->BitPos;

        *BitBufPtr <<= BitsAvail;
        *BitBufPtr |= (uint8)(BufPtr[DecWsPtr->BytePos] & (0xFF >> DecWsPtr->BitPos));
        
        BitsRemain -= BitsAvail;
        
        /* all bits in current byte have been read, continue reading at beginning of next byte */
        DecWsPtr->BitPos = 0;
        DecWsPtr->BytePos++;
      }
      else
      {
       /* all remaining bits to read are in current byte */

        *BitBufPtr <<= BitsRemain;

        if (EXI_BITS_IN_BYTE == (DecWsPtr->BitPos + BitsRemain))
        {
          *BitBufPtr |= (uint8)(BufPtr[DecWsPtr->BytePos] & (0xFF >> DecWsPtr->BitPos));
          /* all bits of current byte have been read, continue reading at beginning of next byte */
          DecWsPtr->BytePos++;
          DecWsPtr->BitPos = 0;
        }
        else
        {
          *BitBufPtr |= (BufPtr[DecWsPtr->BytePos] >> (EXI_BITS_IN_BYTE - BitsRemain - DecWsPtr->BitPos)) & (uint8)((1 << BitsRemain) - 1);
          /* there are unread bits left in current byte, only advance BitPos */
          DecWsPtr->BitPos += BitsRemain;
        }

        /* finished, all bits have been read */
        BitsRemain = 0;
      } 
    }
  }

  if((0 != BitsRemain) && (EXI_E_OK == DecWsPtr->StatusCode))
  {
    DecWsPtr->StatusCode = EXI_E_EOS;
  }

  return BitCount - BitsRemain;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeSkipBits()
 **********************************************************************************************************************/
FUNC(uint32, EXI_CODE) Exi_VBSDecodeSkipBits( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint32 BitCount)
{
  uint32 NewBitPos;
  uint32 OldBitPos;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
#endif

  OldBitPos = Exi_VBSDecodeGetWorkspaceBitPos(DecWsPtr);
  NewBitPos = Exi_VBSDecodeSetWorkspaceBitPos(DecWsPtr, OldBitPos + BitCount);

  return (NewBitPos - OldBitPos);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* === 7.1.1 BINARY === */

/* UInt + Bytes... (see 7.1.6) */

/**********************************************************************************************************************
 *  Exi_VBSDecodeBytes()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeBytes(
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) BufLenPtr)
{
  Exi_BitBufType BitBuf = 0;
  Exi_BitBufType NumBytes = 0;  
  uint16 CopyByteLen = 0;
  uint16 i;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,  EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != BufPtr,    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != BufLenPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, &NumBytes);

  if (EXI_E_OK != DecWsPtr->StatusCode)
  {
    if (EXI_E_INT_OVERFLOW == DecWsPtr->StatusCode)
    {
      /* exact length of byte array unknown!*/
      DecWsPtr->StatusCode = EXI_E_NOT_OK;
    }
  }
  else
  {
    if (NumBytes > (*BufLenPtr))
    {
      /* not all bytes fit into the buffer, copy as much bytes as fit into the buffer */ 
      CopyByteLen = (*BufLenPtr);
    }
    else
    {
      /* buffer is large enough, copy all bytes */
      CopyByteLen = (uint16)NumBytes;
    }

    for(i = 0; i < CopyByteLen; i++)
    {
      Exi_VBSDecodeUIntN(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE);
      BufPtr[i] = (uint8)BitBuf;
    }

    if (NumBytes > CopyByteLen)
    {
#if (EXI_ENABLE_DECODE_STRING_TRUNCATION == STD_ON)
      /* ignore bytes that do not fit into the buffer */
      if(((NumBytes - CopyByteLen) * EXI_BITS_IN_BYTE) != Exi_VBSDecodeSkipBits(DecWsPtr, (NumBytes - CopyByteLen) * EXI_BITS_IN_BYTE))
      {
        /* failed to skip the required number of bits -> set error if not already done */
        if(EXI_E_OK == DecWsPtr->StatusCode)
        {
          DecWsPtr->StatusCode = EXI_E_EOS;
        }
      }
#else
      /* data does not fit into the buffer */
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_ARR_OVERFLOW;
      }
#endif
    }

    /* null-terminate array if possible */
    if (NumBytes < (*BufLenPtr))
    {
      BufPtr[NumBytes] = 0; 
    }

    *BufLenPtr = CopyByteLen;
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.2 BOOLEAN === */

/**********************************************************************************************************************
 *  Exi_VBSDecodeBool()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeBool(
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(boolean, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  uint32 BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  (void)Exi_VBSReadBits(DecWsPtr, &BitBuf, 1);
  *ValuePtr = (boolean)(0 != (BitBuf & 0x1)); /* get boolean value */

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.3 DECIMAL === */

/* The Decimal datatype representation is a Boolean sign (see 7.1.2 Boolean) followed by two Unsigned Integers (see 7.1.6 Unsigned Integer). */

/* === 7.1.4 FLOAT === */

/* Int + Int (see 7.1.5) */

/* === 7.1.5 INTEGER === */

/* Boolean (see 7.1.2) + UInt (see 7.1.6) */ 

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt8()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeInt8( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint8, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  Exi_BitBufType thisValue = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  /* Int8 values are represented as a 8-bit unsigned integer value with offset 128 */
  Exi_VBSDecodeUIntN(DecWsPtr, &thisValue, 8);
  *ValuePtr = (sint8)(thisValue - 128);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt16()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeInt16( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint16, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  boolean negative;
  Exi_BitBufType thisValue;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeBool(DecWsPtr, &negative);
  Exi_VBSDecodeUInt(DecWsPtr, &thisValue);

  if (TRUE == negative)
  {
    /* negative value */
    if (EXI_SINT16_MAX_NEG_NUM > thisValue)
    {
      *ValuePtr = (-(sint16)thisValue) - 1;
    }
    else
    {
      *ValuePtr = (-(sint16)EXI_SINT16_MAX_POS_NUM) - 1; /* (sint16)EXI_SINT16_MAX_NEG_NUM leads to MISRA warning */
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
  }
  else
  {
    /* positive value */
    if (EXI_SINT16_MAX_POS_NUM >= thisValue)
    {
      *ValuePtr = (sint16)thisValue;
    }
    else
    {
      *ValuePtr = EXI_SINT16_MAX_POS_NUM;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt32()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeInt32( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(sint32, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  boolean negative;
  Exi_BitBufType thisValue;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeBool(DecWsPtr, &negative);
  Exi_VBSDecodeUInt(DecWsPtr, &thisValue);

  if (TRUE == negative)
  {
    /* negative value */
    if (EXI_SINT32_MAX_NEG_NUM > thisValue)
    {
      *ValuePtr = (-(sint32)thisValue) -1;
    }
    else
    {
      *ValuePtr = (-(sint32)EXI_SINT32_MAX_POS_NUM) - 1; /* (sint32)EXI_SINT32_MAX_NEG_NUM leads to MISRA warning */
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
  }
  else
  {
    /* positive value */
    if (EXI_SINT32_MAX_POS_NUM >= thisValue )
    {
      *ValuePtr = (sint32)thisValue;
    }
    else
    {
      *ValuePtr = EXI_SINT32_MAX_POS_NUM;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeInt64()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeInt64( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_SInt64, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  boolean negative;
  Exi_UInt64 thisValue;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeBool(DecWsPtr, &negative);
  Exi_VBSDecodeUInt64(DecWsPtr, &thisValue);

  if (TRUE == negative)
  {
    /* negative value */
#if defined(Exi_SInt64_Available)
    if (EXI_SINT64_MAX_NEG_NUM > thisValue)
    {
      *ValuePtr = (-(Exi_SInt64)thisValue) -1;
    }
    else
    {
      *ValuePtr = (-(Exi_SInt64)EXI_SINT64_MAX_POS_NUM) - 1; /* (Exi_SInt64)EXI_SINT64_MAX_NEG_NUM leads to MISRA warning */
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
#else
    if (EXI_SINT32_MAX_NEG_NUM > thisValue.HiWord)
    {
      (*ValuePtr).HiWord = ~(thisValue.HiWord);
      (*ValuePtr).LoWord = ~(thisValue.LoWord);
    }
    else
    {
      (*ValuePtr).HiWord = EXI_SINT32_MAX_NEG_NUM;
      (*ValuePtr).LoWord = 0;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
#endif
  }
  else
  {
    /* positive value */
#if defined(Exi_SInt64_Available)
    if (EXI_SINT64_MAX_POS_NUM >= thisValue )
    {
      *ValuePtr = (Exi_SInt64)thisValue;
    }
    else
    {
      *ValuePtr = EXI_SINT64_MAX_POS_NUM;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
#else
    if (EXI_SINT32_MAX_POS_NUM >= thisValue.HiWord )
    {
      (*ValuePtr).HiWord = thisValue.HiWord;
      (*ValuePtr).LoWord = thisValue.LoWord;
    }
    else
    {
      (*ValuePtr).HiWord = EXI_SINT32_MAX_POS_NUM;
      (*ValuePtr).LoWord = EXI_UINT32_MAX_NUM;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
    }
#endif
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeBigInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeBigInt( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BigIntType, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  uint32 BitBuf = 0;
  uint8 Mult = 0;
  uint8 BitShift;
  uint16 ValueSize;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  ValueSize = sizeof(ValuePtr->Value);
  Exi_VBSDecodeBool(DecWsPtr, &ValuePtr->IsNegative);
  ValuePtr->Length = 1;
  do
  {
    BitBuf = 0;
    /* calculate numbers of bits we have to shift to the left */
    BitShift = (Mult % 8);
    if (EXI_BITS_IN_BYTE != Exi_VBSReadBits(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE))
    {
      /* reached the end of the stream, error status already written in Exi_VBSReadBits */
      break;
    }

    ValuePtr->Value[ValueSize - ValuePtr->Length] |= (uint8)((BitBuf & 0x7F) << BitShift);
    /* if BitShift is greater than 1 it is not possible to write 7 Bit into the current byte */
    if(1 < BitShift)
    {
      /* switch to next byte */
      ValuePtr->Length++;
      if(ValuePtr->Length <= ValueSize)
      {
        /* length not exceeded, write remaining bits */
        ValuePtr->Value[ValueSize - ValuePtr->Length] |= (uint8)((BitBuf & 0x7F) >> (8 - BitShift));
      }
      else
      {
        /* maximum configured length exceeded, write error status if not already done */
        if(EXI_E_OK == DecWsPtr->StatusCode)
        {
          DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
        }
        break; /* PRQA S 0771 */ /* MD_Exi_14.6 */
      }
    }
    else if(1 == BitShift) /* ESCAN00068523 */
    {
      /* switch to next byte */
      ValuePtr->Length++;
      if(ValuePtr->Length > ValueSize)
      {
        /* maximum configured length exceeded, write error status if not already done */
        if(EXI_E_OK == DecWsPtr->StatusCode)
        {
          DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
        }
        break; /* PRQA S 0771 */ /* MD_Exi_14.6 */
      }
    }
    else
    {
      /* nothing to do */
    }

    Mult += 7;

  } while (0x80 == (BitBuf & 0x80));

  if((TRUE == ValuePtr->IsNegative) && (EXI_E_OK == DecWsPtr->StatusCode))
  {
    /* add 1 to the value in case this is a negative integer value */
    uint8 i = 0;
    do
    {
      if(0xFF != ValuePtr->Value[ValueSize - 1 -i])
      {
        ValuePtr->Value[ValueSize - 1 -i]++;
        break;
      }
      else
      {
        ValuePtr->Value[ValueSize - 1 -i] = 0x00;
      }
      i++;
      if( i == ValuePtr->Length)
      {
        ValuePtr->Length++;
      }
    }while(i < (ValueSize - 1));
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* === 7.1.6 UNSIGNED INTEGER === */

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUInt( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  uint32 BitBuf = 0;
  uint8 Mult = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Mult = 0;
  *ValuePtr = 0;

  do
  {
    if (EXI_BITS_IN_BYTE != Exi_VBSReadBits(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE))
    {
      break;
    }

    if (((28 == Mult) && ((BitBuf & 0x7F) > 0x0F)) || (28 < Mult))
    {
      /* Exi_BitBufType has only 32 bits. return maximum possible value and set overflow indication flag */
      *ValuePtr = 0xFFFFFFFFU;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
      break; /* PRQA S 0771 */ /* MD_Exi_14.6 */
    }
    else
    {
      *ValuePtr |= (BitBuf & 0x7F) << Mult;
    }

    Mult += 7;

  } while (0x80 == (BitBuf & 0x80));
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt8()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUInt8( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif
  
  /* UInt8 values are represented as a 8-bit unsigned integer value */
  Exi_VBSDecodeUIntN(DecWsPtr, &BitBuf, 8);
  *ValuePtr = (uint8)BitBuf;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt16()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUInt16( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, &BitBuf);

  if ((BitBuf > 0xFFFFU) && (EXI_E_OK == DecWsPtr->StatusCode))
  {
    /* decoded value is bigger than decode target buffer */
    DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
  }

  *ValuePtr = (uint16)BitBuf;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt32()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUInt32( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint32, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, (Exi_BitBufType*)ValuePtr);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeUInt64()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUInt64( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_UInt64, AUTOMATIC, EXI_APPL_VAR) ValuePtr)
{
  uint32 BitBuf = 0;
  uint8 Mult = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != ValuePtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Mult = 0;
#if defined(Exi_UInt64_Available)
  *ValuePtr = 0;

  do
  {
    if (EXI_BITS_IN_BYTE != Exi_VBSReadBits(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE))
    {
      break;
    }

    if (((63 == Mult) && ((BitBuf & 0x03) > 0x01)) || (63 < Mult))
    {
      /* Maximum 64 bits. return maximum possible value and set overflow indication flag */
      *ValuePtr = 0xFFFFFFFFFFFFFFFFULL;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
      break; /* PRQA S 0771 */ /* MD_Exi_14.6 */
    }
    else
    {
      *ValuePtr |= ((Exi_UInt64)(BitBuf & 0x7F)) << Mult;
    }

    Mult += 7;

  } while (0x80 == (BitBuf & 0x80));
#else
  (*ValuePtr).HiWord = 0;
  (*ValuePtr).LoWord = 0;
  
  do
  {
    if (EXI_BITS_IN_BYTE != Exi_VBSReadBits(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE))
    {
      break;
    }

    if (((63 == Mult) && ((BitBuf & 0x03) > 0x01)) || (63 < Mult))
    {
      /* Maximum 64 bits. return maximum possible value and set overflow indication flag */
      (*ValuePtr).HiWord = EXI_UINT32_MAX_NUM;
      (*ValuePtr).LoWord = EXI_UINT32_MAX_NUM;
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_INT_OVERFLOW;
      }
      break;
    }
    else if(28 > Mult)
    {
      (*ValuePtr).LoWord |= (BitBuf & 0x7F) << Mult;
    }
    else if(28 == Mult)
    {
      (*ValuePtr).LoWord |= (BitBuf & 0x0F) << Mult;
      (*ValuePtr).HiWord |= (BitBuf & 0x70) >> 4;
    }
    else
    {
      (*ValuePtr).HiWord |= (BitBuf & 0x7F) << (Mult - 32);
    }

    Mult += 7;

  } while (0x80 == (BitBuf & 0x80));
#endif
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
 *  Exi_VBSDecodeUIntN()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeUIntN( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) BitBufPtr,
  uint8 BitCount)
{

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,                     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != BitBufPtr,                    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(BitCount <= (sizeof(Exi_BitBufType) * 8), EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_PARAM);
#endif

  (void)Exi_VBSReadBits(DecWsPtr, BitBufPtr, BitCount);

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.10 STRING === */

/**********************************************************************************************************************
 *  Exi_VBSDecodeStringOnly()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeStringOnly( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr,
  uint16 StrLen)
{
  Exi_BitBufType BitBuf = 0;
  uint16 CopyStrLen = 0;
  uint16 i;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr,    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufLenPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  if (StrLen > (*StrBufLenPtr))
  {
    /* string does not fit into the buffer. Copy as much bytes as fit into the buffer */ 
    CopyStrLen = (*StrBufLenPtr);
  }
  else
  {
    /* buffer is large enough, copy all bytes */
    CopyStrLen = StrLen;
  }

  /* copy characters */
  for (i = 0; i < CopyStrLen; i++)
  {
    /* decode character */
    Exi_VBSDecodeUIntN(DecWsPtr, &BitBuf, EXI_BITS_IN_BYTE);
    StrBufPtr[i] = (uint8)BitBuf;
  }

  if (StrLen > CopyStrLen)
  {
#if (EXI_ENABLE_DECODE_STRING_TRUNCATION == STD_ON)
    /* ignore bytes that do not fit into the buffer */
    if(((StrLen - CopyStrLen) * EXI_BITS_IN_BYTE) != Exi_VBSDecodeSkipBits(DecWsPtr, (StrLen - CopyStrLen) * EXI_BITS_IN_BYTE))
    {
      /* failed to skip the required number of bits -> set error if not already done */
      if(EXI_E_OK == DecWsPtr->StatusCode)
      {
        DecWsPtr->StatusCode = EXI_E_EOS;
      }
    }
#else
    /* data does not fit into the buffer */
    if(EXI_E_OK == DecWsPtr->StatusCode)
    {
      DecWsPtr->StatusCode = EXI_E_ARR_OVERFLOW;
    }
#endif
  }

  /* null-terminate string if possible */
  if (StrLen < (*StrBufLenPtr))
  {
    StrBufPtr[StrLen] = 0; 
  }

  *StrBufLenPtr = CopyStrLen;

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeString()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeString( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr,    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufLenPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, &BitBuf);

  if (EXI_E_OK != DecWsPtr->StatusCode)
  {
    if (EXI_E_INT_OVERFLOW == DecWsPtr->StatusCode)
    {
      /* we cannot read the whole string. length unknown! */
      DecWsPtr->StatusCode = EXI_E_NOT_OK;
    }
  }
  else
  {
    Exi_VBSDecodeStringOnly(DecWsPtr, StrBufPtr, StrBufLenPtr, (uint16)BitBuf);
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeStringValue()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeStringValue( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr,    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufLenPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, &BitBuf);

  if (EXI_E_OK != DecWsPtr->StatusCode)
  {
    if (EXI_E_INT_OVERFLOW == DecWsPtr->StatusCode)
    {
      /* we cannot read the whole string. length unknown! */
      DecWsPtr->StatusCode = EXI_E_NOT_OK;
    }
  }
  else
  {
    if (0 == BitBuf)
    {
      DecWsPtr->StatusCode = EXI_E_STRING_TABLE_LOCAL_HIT;
    }
    else if (1 == BitBuf)
    {
      DecWsPtr->StatusCode = EXI_E_STRING_TABLE_GLOBAL_HIT;
    }
    else
    {
      BitBuf -= 2;
      Exi_VBSDecodeStringOnly(DecWsPtr, StrBufPtr, StrBufLenPtr, (uint16)BitBuf);
    }
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeQName()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeQName( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(uint8, AUTOMATIC, EXI_APPL_VAR) StrBufPtr,
  P2VAR(uint16, AUTOMATIC, EXI_APPL_VAR) StrBufLenPtr)
{
  Exi_BitBufType BitBuf = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr,     EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufPtr,    EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != StrBufLenPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUInt(DecWsPtr, &BitBuf);
  
  if (EXI_E_OK != DecWsPtr->StatusCode)
  {
    if (EXI_E_INT_OVERFLOW == DecWsPtr->StatusCode)
    {
      /* we cannot read the whole string. length unknown! */
      DecWsPtr->StatusCode = EXI_E_NOT_OK;
    }
  }
  else
  {
    if (0 == BitBuf)
    {
      DecWsPtr->StatusCode = EXI_E_STRING_TABLE_LOCAL_HIT;
    }
    else
    {
      BitBuf -= 1;
      Exi_VBSDecodeStringOnly(DecWsPtr, StrBufPtr, StrBufLenPtr, (uint16)BitBuf);
    }
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* === 7.1.11 LIST === */

/**********************************************************************************************************************
 *  Exi_VBSDecodeSchemaDeviation()
 **********************************************************************************************************************/
#if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON))
FUNC(void, EXI_CODE) Exi_VBSDecodeSchemaDeviation(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  P2VAR(Exi_BitBufType, AUTOMATIC, EXI_APPL_VAR) ExiEventCode,
  Exi_BitBufType SchemaDeviationEventCode,
  uint8 EventCodeBitSize,
  boolean StartElement,
  boolean OptionalElement,
  boolean AttributesAllowed,
  uint8 KnownAttributesNum,
  uint8 SchemaSetId)
{
  uint8 readElement = 0;
  uint8 dummyString = 0;
  uint16 dummyStringLen = 0;
  boolean thisAttributesAllowed = AttributesAllowed;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  while((EXI_E_OK == DecWsPtr->DecWs.StatusCode) && (SchemaDeviationEventCode == *ExiEventCode))
  {
    /* handle schema daviations */
    readElement = 0;
    *ExiEventCode = 0;
    if(TRUE == StartElement)
    {
      /* in this case we expect SE(*) or CH[typed value] */
      if(TRUE == thisAttributesAllowed)
      {
        thisAttributesAllowed = FALSE;
        /* schema deviation could be EE, AT(xsi:type), AT(xsi:nil), AT(*), AT(QName), AT(*)[untyped value] SE(*) or CH[untyped value] */
        Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, ExiEventCode, Exi_CalculateBitSize(6));
        /* only SE(*) and CH[untyped value] accepted/skipped */
        if(((4 == *ExiEventCode) && (TRUE == OptionalElement)) || ((5 == *ExiEventCode) && (FALSE == OptionalElement)))
        {
          /* found SE(*) -> skip unexpected element */
          readElement = 1;
        }
        else if(((5 == *ExiEventCode) && (TRUE == OptionalElement)) || ((6 == *ExiEventCode) && (FALSE == OptionalElement)))
        {
          /* found CH[untyped value] -> skip */
          readElement = 2;
        }
        else if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        else
        {
          /* nothing to do, error already set */
        }
      }
      else
      {
        /* schema deviation could be EE, SE(*) or CH[untyped value] */
        if(TRUE == OptionalElement)
        {
          Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, ExiEventCode, 1);
        }
        else
        {
          Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, ExiEventCode, 2);
        }
        /* only SE(*) and CH[untyped value] accepted/skipped */
        if(((0 == *ExiEventCode) && (TRUE == OptionalElement)) || ((1 == *ExiEventCode) && (FALSE == OptionalElement)))
        {
          /* found SE(*) -> skip unexpected element */
          readElement = 1;
        }
        else if(((1 == *ExiEventCode) && (TRUE == OptionalElement)) || ((2 == *ExiEventCode) && (FALSE == OptionalElement)))
        {
          /* found CH[untyped value] -> skip */
          readElement = 2;
        }
        else if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
        {
          DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        }
        else
        {
          /* nothing to do, error already set */
        }
      }
    }
    else
    {
      /* in this case we expect EE */
      /* schema deviation could be SE(*) or CH[untyped value] */
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, ExiEventCode, 1);
      /* only SE(*) and CH[untyped value] accepted/skipped */
      if(0 == *ExiEventCode)
      {
        /* found SE(*) -> skip unexpected element */
        readElement = 1;
      }
      else if(1 == *ExiEventCode)
      {
        /* found CH[untyped value] -> skip */
        readElement = 2;
      }
      else if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      else
      {
        /* nothing to do, error already set */
      }
    }
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      switch(readElement)
      {
      case 1:
        Exi_VBSDecodeGenericElement(DecWsPtr, SchemaSetId, FALSE);
        break;
      case 2:
        /* found CH[untyped value] -> skip */
        Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &dummyString, &dummyStringLen);
        break;
      default:
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
        break;
      }
    }
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      *ExiEventCode = 0;
      Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, ExiEventCode, EventCodeBitSize);
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* #if (defined(EXI_ALLOW_SCHEMA_DEVIATION) && (EXI_ALLOW_SCHEMA_DEVIATION == STD_ON)) */

/**********************************************************************************************************************
 *  Exi_VBSDecodeGenericElement()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeGenericElement(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint8 SchemaSetId,
  boolean SkipElement)
{
  uint8 dummyString = 0;
  uint16 dummyStringLen = 0;
  Exi_BitBufType ExiEventCode = 0;
  Exi_GenericElementType* GenElement = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  if(FALSE == SkipElement)
  {
    if(sizeof(*GenElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
    {
      /* struct fits into remaining storage buffer */
      GenElement = (Exi_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
#if(defined(EXI_ENABLE_VSTDLIB) && (EXI_ENABLE_VSTDLIB == STD_OFF))
      memset(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], 0, sizeof(*GenElement));
#else
      Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*GenElement));
#endif
      DecWsPtr->OutputData.StorageOffset += sizeof(*GenElement);
    }
    else
    {
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
      }
      return;
    }
    GenElement->ContentType = EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE;
    GenElement->QNameType = EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE;
  }
  if(SchemaSetId < EXI_UNKNOWN_SCHEMA_SET_TYPE)
  {
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, Exi_CalculateBitSize(Exi_UriPartitionSize[SchemaSetId]));
    if(0 == ExiEventCode)
    {
      /* URI miss */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* decode uri name */
        Exi_VBSDecodeString(&DecWsPtr->DecWs, &dummyString, &dummyStringLen);
        /* decode local name */
        /* not supported */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
    else
    {
      /* URI hit, URI index is EXI Event -1 */
      uint8 uriIndex = (uint8)(ExiEventCode - 1);
      /* decode local name */
      if(TRUE == SkipElement)
      {
        Exi_VBSDecodeQName(&DecWsPtr->DecWs, &dummyString, &dummyStringLen);
      }
      else
      {
        Exi_GenericStringType* QName;
        if(sizeof(*QName) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
        {
          /* struct fits into remaining storage buffer */
          QName = (Exi_GenericStringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
#if(defined(EXI_ENABLE_VSTDLIB) && (EXI_ENABLE_VSTDLIB == STD_OFF))
          memset(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], 0, sizeof(*QName));
#else
          Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*QName));
#endif
          DecWsPtr->OutputData.StorageOffset += sizeof(*QName);
        }
        else
        { 
          if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
          {
            DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
          }
          return;
        }
        QName->Length = sizeof(QName->Buffer);
        Exi_VBSDecodeQName(&DecWsPtr->DecWs, &QName->Buffer[0], &QName->Length);
        if((EXI_E_OK == DecWsPtr->DecWs.StatusCode) && (NULL_PTR != GenElement))
        {
          GenElement->QNameType = EXI_SCHEMA_GENERIC_STRING_TYPE;
          GenElement->GenericQName = QName;
        }
      }
      if(EXI_E_STRING_TABLE_LOCAL_HIT == DecWsPtr->DecWs.StatusCode)
      {
        /* TODO: dervive type from URI and local name information and decode */
        uriIndex = uriIndex; /* avoid compiler warning */
        /* not supported */
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
      else if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        /* read element content */
        Exi_VBSDecodeGenericElementContent(DecWsPtr, SchemaSetId, SkipElement, GenElement);
      }
      else
      {
        /* error already detected in an earlier step -> nothing to do */
      }
    }
  }
  else
  {
    if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
    {
      DecWsPtr->DecWs.StatusCode = EXI_E_NOT_OK;
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeGenericElementContent()
 **********************************************************************************************************************/
FUNC(void, EXI_CODE) Exi_VBSDecodeGenericElementContent(
  P2VAR(Exi_DecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint8 SchemaSetId,
  boolean SkipContent,
  Exi_GenericElementType* GenericElement)
{
  /*
  ElementContent:
                  EE                      0
                  SE(*)  ElementContent   1.0
                  CH  ElementContent      1.1
  */
  uint8 dummyString = 0;
  uint16 dummyStringLen = 0;
  Exi_BitBufType ExiEventCode = 0;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnVoid(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER);
#endif

  Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
  if(0 == ExiEventCode)
  {
    /* EE -> finished */
  }
  else
  {
    ExiEventCode = 0;
    Exi_VBSDecodeUIntN(&DecWsPtr->DecWs, &ExiEventCode, 1);
    if(0 == ExiEventCode)
    {
      /* read SE(*) */
      if((FALSE == SkipContent) && (NULL_PTR != GenericElement))
      {
        GenericElement->ContentType = EXI_SCHEMA_GENERIC_ELEMENT_TYPE;
        GenericElement->GenericContent = (&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
      }
      Exi_VBSDecodeGenericElement(DecWsPtr, SchemaSetId, SkipContent);
      /* read ElementContent */
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_GenericElementType* nextElement = (Exi_GenericElementType*)NULL_PTR;
        if(FALSE == SkipContent)
        {
          if(sizeof(*nextElement) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
          {
            /* struct fits into remaining storage buffer */
            nextElement = (Exi_GenericElementType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
#if(defined(EXI_ENABLE_VSTDLIB) && (EXI_ENABLE_VSTDLIB == STD_OFF))
            memset(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], 0, sizeof(*nextElement));
#else
            Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*nextElement));
#endif
            /* do not increment StorageOffset here because this element will be allocated again in 
            Exi_VBSDecodeGenericElement if required. Then nextElement == GenericElement */
          }
          else
          {
            if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
            {
              DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
            }
            return;
          }
          nextElement->ContentType = EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE;
          nextElement->QNameType = EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE;
        }
        Exi_VBSDecodeGenericElementContent(DecWsPtr, SchemaSetId, SkipContent, nextElement); /* PRQA S 3670 */  /*  MD_Exi_16.2 */
        if((FALSE == SkipContent) && (NULL_PTR != nextElement))
        {
          if(((nextElement->ContentType != EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE) || (nextElement->QNameType != EXI_SCHEMA_UNKNOWN_ELEMENT_TYPE)) && (NULL_PTR != GenericElement))
          {
            ((Exi_GenericElementType*)GenericElement->GenericContent)->NextGenericElement = nextElement;
          }
        }
      }
    }
    else
    {
      /* read CH[untyped value] */
      if(TRUE == SkipContent)
      {
        Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &dummyString, &dummyStringLen);
      }
      else
      {
        Exi_GenericStringType* contentString;
        if(sizeof(*contentString) < (uint32)(DecWsPtr->OutputData.StorageLen - DecWsPtr->OutputData.StorageOffset))
        {
          /* struct fits into remaining storage buffer */
          contentString = (Exi_GenericStringType*)(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset]); /* PRQA S 0310 */  /*  MD_Exi_11.4 */
#if(defined(EXI_ENABLE_VSTDLIB) && (EXI_ENABLE_VSTDLIB == STD_OFF))
          memset(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], 0, sizeof(*contentString));
#else
          Exi_ClearMemory(&DecWsPtr->OutputData.StoragePtr[DecWsPtr->OutputData.StorageOffset], sizeof(*contentString));
#endif
          DecWsPtr->OutputData.StorageOffset += sizeof(*contentString);
        }
        else
        {
          if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
          {
            DecWsPtr->DecWs.StatusCode = EXI_E_BUFFER_SIZE;
          }
          return;
        }
        contentString->Length = sizeof(contentString->Buffer);
        Exi_VBSDecodeStringValue(&DecWsPtr->DecWs, &contentString->Buffer[0], &contentString->Length);
        if((EXI_E_OK == DecWsPtr->DecWs.StatusCode) && (NULL_PTR != GenericElement))
        {
          GenericElement->ContentType = EXI_SCHEMA_GENERIC_STRING_TYPE;
          GenericElement->GenericContent = contentString;
        }
      }
      if(EXI_E_OK == DecWsPtr->DecWs.StatusCode)
      {
        Exi_VBSDecodeGenericElementContent(DecWsPtr, SchemaSetId, SkipContent, GenericElement); /* PRQA S 3670 */  /*  MD_Exi_16.2 */
      }
      else
      {
        DecWsPtr->DecWs.StatusCode = EXI_E_INV_EVENT_CODE;
      }
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeGenericElementContent()
 **********************************************************************************************************************/
FUNC(uint8, EXI_CODE) Exi_CalculateBitSize(uint8 MaxValue)
{
  uint8 RequiredBitSize = 1;
  uint8 i;
  /* calculate the required bit size for the exi event */
  for(i=0; i<8;i++)
  {
    if((MaxValue & 0x01) == 0x01)
    {
      RequiredBitSize = i+1;
    }
    MaxValue = MaxValue >> 1;
  }
  return RequiredBitSize;
}

/**********************************************************************************************************************
 *  Exi_VBSDecodeGetWorkspaceBitPos()
 **********************************************************************************************************************/
FUNC(uint32, EXI_CODE) Exi_VBSDecodeGetWorkspaceBitPos( 
  P2CONST(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_DATA) DecWsPtr)
{
  uint32 BytePosition;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
#endif

  BytePosition = DecWsPtr->BytePos;

#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  if (DecWsPtr->PBufIdx != 0)
  {
    uint8 PBufIdx;

    /* Add length of all previouse PBuf segments */
    for (PBufIdx = 0; PBufIdx < DecWsPtr->PBufIdx; PBufIdx++)
    {
      BytePosition += DecWsPtr->PBufPtr[PBufIdx].len;
    }
  }
#endif

  return (BytePosition << 3) + DecWsPtr->BitPos;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Exi_VBSDecodeSetWorkspaceBitPos()
 **********************************************************************************************************************/
FUNC(uint32, EXI_CODE) Exi_VBSDecodeSetWorkspaceBitPos( 
  P2VAR(Exi_BSDecodeWorkspaceType, AUTOMATIC, EXI_APPL_VAR) DecWsPtr,
  uint32 BitPos)
{
  uint32 MaxBitPos;

#if (EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Exi_CheckInternalDetErrorReturnValue(NULL_PTR != DecWsPtr, EXI_DECODER_INSTANCE_ID, EXI_API_ID_V_INTERNAL_FUNCTION, EXI_E_INV_POINTER, 0);
#endif

#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  MaxBitPos = (DecWsPtr->PBufPtr[0].totLen << 3);
#else
  MaxBitPos = (DecWsPtr->BufLen << 3);
#endif

  if (BitPos > MaxBitPos)
  {
    BitPos = MaxBitPos;
  }
  
  DecWsPtr->BytePos = (uint16)(BitPos >> 3);   /* (BitPos / BITS_IN_BYTE) */
  DecWsPtr->BitPos  = (uint8)(BitPos & 0x07U); /* (BitPos % BITS_IN_BYTE) */

#if (EXI_ENABLE_PBUF_SUPPORT == STD_ON)
  /* Get the PBuf segment where the new position is inside */
  DecWsPtr->PBufIdx = 0;
  DecWsPtr->PBufProcessedElementBytes = 0;
  if ( DecWsPtr->BytePos >= DecWsPtr->PBufPtr[0].len )
  {
    /* Get the PBuf segment where the position is inside */
    while (DecWsPtr->BytePos >= DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len)
    {
      DecWsPtr->PBufProcessedElementBytes += DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len;
      if ( DecWsPtr->PBufProcessedElementBytes < DecWsPtr->PBufPtr[0].totLen )
      {
        DecWsPtr->BytePos -= DecWsPtr->PBufPtr[DecWsPtr->PBufIdx].len;
        DecWsPtr->PBufIdx++;
      }
      else
      {
        DecWsPtr->PBufProcessedElementBytes = DecWsPtr->PBufPtr[0].totLen;
        DecWsPtr->StatusCode = EXI_E_EOS;
        break;
      }
    }
  }
#endif

  return BitPos;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#define EXI_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Exi_BSDecoder.c
 **********************************************************************************************************************/
