/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library - PKCS #1 encoding methods
 *
 *  \description  This file is part of the embedded systems library cvActLib/ES.
 *                Encoding methods for RSA encryption / decryption and signature verification / generation
 *                according to PKCS #1 v2.1
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

#include "ESLib.h"
#include "ESLib_types.h"
#include "ESLib_ASN_1.h"

/*****************************************************************************
  Types and constants
 *****************************************************************************/

#define ASN1_PADDING_BLOCK_TYPE_INVALID   0xFFu

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_verifyPaddingRSAEM_V15( const eslt_Byte   *message,
 **                                    eslt_Length *paddingLength,
 **                                    eslt_Byte    blockType )
 **
 ** input:
 ** - message:       Decrypted message
 ** - paddingLength: Length of message
 ** - blockType:     Block type according to PKCS #1 1.5
 **                  ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO
 **                  ASN1_PADDING_BLOCK_TYPE_PRIVATE
 **                  ASN1_PADDING_BLOCK_TYPE_PUBLIC
 **
 ** output:
 ** - paddingLength: Length of padding
 ** - errorCode:
 **      ESL_ERC_PARAMETER_INVALID     Message length smaller than minimum padding
 **      ESL_ERC_RSA_ENCODING_INVALID  Invalid padding
 **      ESL_ERC_NO_ERROR              else
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifyPaddingRSAEM_V15(
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) message, CRYPTOCV_P2VAR_PARA( eslt_Length ) paddingLength, eslt_Byte blockType )
{

   eslt_ErrorCode returnValue;
   eslt_Length    length;
   eslt_Length    index;
   eslt_Length    offset;
   eslt_Length    newOffset;
   eslt_Length    oldOffset;
   eslt_Byte      allowedMask;
   eslt_Byte      allowedValue;
   eslt_Byte      allowedResult;
   eslt_Byte      deniedMask;
   eslt_Byte      deniedValue;
   eslt_Byte      deniedResult;


   returnValue = ESL_ERC_NO_ERROR;
   length      = *paddingLength;

   /* Provided message has to be large enough to hold minimal padding */
   if (length < ASN1_SIZEOF_MINIMAL_PADDING)
   {
      returnValue = ESL_ERC_PARAMETER_INVALID;
   }
   else
   {
      /* Check PKCS header and expected block type */
      if ((0x00u != message[0]) || (blockType != message[1]))
      {
         /* No valid header present, use "invalid" block type */
         blockType = ASN1_PADDING_BLOCK_TYPE_INVALID;
      }

      /* Set masks for padding check */
      switch (blockType)
      {
         case ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO:
         {
            /* Padding bytes shall be zero */
            allowedMask    = 0xFFu;
            allowedValue   = 0x00u;
            deniedMask     = 0x00u;
            deniedValue    = 0xFFu;

            break;
         }
         case ASN1_PADDING_BLOCK_TYPE_PRIVATE:
         {
            /* Padding bytes shall be 0xFF */
            allowedMask    = 0xFFu;
            allowedValue   = 0xFFu;
            deniedMask     = 0x00u;
            deniedValue    = 0xFFu;

            break;
         }
         case ASN1_PADDING_BLOCK_TYPE_PUBLIC:
         {
            /* Padding bytes shall be non-zero value */
            allowedMask    = 0x00u;
            allowedValue   = 0x00u;
            deniedMask     = 0xFFu;
            deniedValue    = 0x00u;

            break;
         }
         case ASN1_PADDING_BLOCK_TYPE_INVALID:
         default:
         {
            /* All values forbidden */
            allowedMask    = 0x00u;
            allowedValue   = 0xFFu;
            deniedMask     = 0x00u;
            deniedValue    = 0x00u;

            /* Unoptimized to prevent timing analysis */
            returnValue = ESL_ERC_RSA_ENCODING_INVALID;

            break;
         }
      }

      offset = 1u;

      for (index = 2u; index < length; index++)
      {
         newOffset = offset + 1u;
         oldOffset = offset;

         /* Check against allowed values */
         if ((message[index] & allowedMask) == allowedValue)
         {
            allowedResult = 0x00u;
         }
         else
         {
            allowedResult = 0x01u;
         }

         /* Check against forbidden values */
         if ((message[index] & deniedMask) != deniedValue)
         {
            deniedResult = 0x00u;
         }
         else
         {
            deniedResult = 0x01u;
         }

         if (0x00u == (allowedResult | deniedResult))
         {
            /* Unoptimized to prevent timing analysis */
            if (newOffset == index)
            {
               offset = newOffset;
            }
            else
            {
               offset = oldOffset;
            }
         }
      }

      /* Block type 0 already includes trailing zero */
      if (ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO != blockType)
      {
         offset++;
      }

      /* Check minimal padding length and trailing zero */
      if ((ASN1_SIZEOF_MINIMAL_PADDING > offset) || (0x00u != message[offset]))
      {
         returnValue = ESL_ERC_RSA_ENCODING_INVALID;
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
         *paddingLength = offset + 1u;
      }
   }

   return returnValue;
}
