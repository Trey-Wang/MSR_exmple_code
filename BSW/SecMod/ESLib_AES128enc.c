/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/21/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_AES128enc.c $
 ** $Rev: 1620 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
 **
 ** \version(cvActLibES 1.0)
 ***************************************************************************/

/****************************************************************************
 **
 **     This file is part of the embedded systems library cvActLib/ES.
 **
 **     Layer: core level
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: AES128 implementation file.
 **                     Currently the actClib version is used.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initEncryptAES128
 **   esl_encryptAES128
 **   esl_finalizeEncryptAES128
 **   esl_initEncryptAES128Block
 **   esl_encryptAES128Block
 **
 ***************************************************************************/

#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIAES.h"
#include "actUtilities.h"

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION: esl_initEncryptAES128
 **
 **  This function initializes the AES stream encryption algorithm.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - key:         pointer to user key of length ESL_SIZEOF_AES128_KEY
 ** - blockMode:   ECB, CBC, etc. block mode switch
 ** - paddingMode: padding mode switch
 ** - initializationVector:   pointer to initialization vector of length
 **                ESL_SIZEOF_AES128_BLOCK (redundant in ECB mode,
 **                if iV==NULL, a trivial zero block is used as iV)
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_MODE_INVALID      block or padding mode is invalid
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initEncryptAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceAES128 ) workSpace, CRYPTOCV_P2CONST_PARA( eslt_Byte ) key,
   const eslt_BlockMode blockMode, const eslt_PaddingMode paddingMode,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) initializationVector )
{
   actU8 mode;

   /* Any NULLs ? */
   if ( (!workSpace) || (!key) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MINSIZEOF_WS_AES128 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Check paddingMode */
   switch (paddingMode)
   {
      case ESL_PM_PKCS5:
      {
         mode = actPADDING_PM_PKCS7;

         break;
      }
      case ESL_PM_OFF:
      {
         mode = actPADDING_PM_OFF;

         break;
      }
      case ESL_PM_ONEWITHZEROES:
      {
         mode = actPADDING_PM_ONEWITHZEROES;

         break;
      }
      default:
      {
         return ESL_ERC_MODE_INVALID;
      }
   }

   /* Check block mode */
   switch (blockMode)
   {
      case ESL_BM_ECB:
      {
         mode |= actAES_BM_ECB;

         break;
      }
      case ESL_BM_CBC:
      {
         mode |= actAES_BM_CBC;

         break;
      }
      default:
      {
         return ESL_ERC_MODE_INVALID;
      }
   }

   /* Init actCLib AES128 */
   actAESInitEncrypt( (CRYPTOCV_P2VAR_PARA( actAESSTRUCT ))workSpace->wsAES128,
                        key,
                        ESL_SIZEOF_AES128_KEY,
                        initializationVector,
                        mode,
                        workSpace->header.watchdog );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_AES128 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_encryptAES128
 **
 **  This function encrypts input data of any length and can be called
 **  arbitrary often after the algorithm initialization.
 **  Exact (inputSize+buffered_bytes)/16 blocks will be encrypted
 **  and written to the output buffer, (inputSize+buffered_bytes)%16
 **  remaining bytes will be buffered, where 0 <= buffered_bytes < 16.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initEncryptAES128(..) function
 ** - inputSize:   length of input data in bytes
 ** - input:       pointer to data to be encrypted
 ** - *outputSize: output buffer size in bytes, a save length is:
 **                  ((inputSize+15)/16)*16       (see output)
 ** - output:      pointer to encrypted data buffer
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - *outputSize: total size of encryted blocks:
 **                  ((inputSize+buffered_bytes)/16)*16
 ** - output:      pointer to encrypted data blocks
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initEncryptAES128(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_encryptAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceAES128 ) workSpace,
   const eslt_Length inputSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) outputSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) output )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!input) || (!output) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_AES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( inputSize % ESL_SIZEOF_AES128_BLOCK == 0 )
   {
      if ( workSpace->header.size < ESL_MINSIZEOF_WS_AES128 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }
   else
   {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_AES128 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   /* Update actCLib AES128 */
   {
      actRETURNCODE result;
      int enc_len = *outputSize;

      result = actAESEncrypt( (CRYPTOCV_P2VAR_PARA( actAESSTRUCT ))workSpace->wsAES128,
                              input,
                              (int)inputSize,
                              output,
                              &enc_len,
                              0,
                              workSpace->header.watchdog );

      if ( result == actEXCEPTION_LENGTH )
      {
         return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
         *outputSize = (eslt_Length)(enc_len & 0xFFFFu);
      }
   }

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_finalizeEncryptAES128
 **
 **  This function finalizes the AES encryption by proccessing the
 **  remaining input bytes in the internal buffer and padding the
 **  input according to the chosen padding in the corresponding
 **  init function.
 **
 ** input:
 ** - input:       pointer to data to be encrypted
 ** - *outputSize: output buffer size in bytes, a save length is:
 **                  2*AES_BLOCK_LENGTH
 ** - output:      pointer to encrypted data buffer
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - *outputSize: total size of encryted input:
 **                  16 or 32 bytes
 ** - output:      pointer to encrypted data blocks
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_OUTPUT_SIZE_TOO_SHORT   outputSize too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initEncryptAES128(..)
 ** - esl_encryptAES128 called before
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeEncryptAES128(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceAES128 ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) outputSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) output )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!output) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_AES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_AES128 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Update actCLib AES128 */
   {
      actRETURNCODE result;
      int enc_len = *outputSize;

      result = actAESEncrypt( (CRYPTOCV_P2VAR_PARA( actAESSTRUCT ))workSpace->wsAES128,
                              (CRYPTOCV_P2CONST_PARA( actU8 ))V_NULL,
                              0,
                              output,
                              &enc_len,
                              1,
                              workSpace->header.watchdog );

      if ( result == actEXCEPTION_LENGTH )
      {
         return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
         *outputSize = (eslt_Length)(enc_len & 0xFFFFu);
      }
   }

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( (CRYPTOCV_P2VAR_PARA( actU8 ))workSpace->wsAES128,
              0,
              (unsigned int)(workSpace->header.size) );

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_initEncryptAES128Block
 **
 **  This function initializes the AES block encryption algorithm.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - key:         pointer to user key of length ESL_SIZEOF_AES128_KEY
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_MODE_INVALID      block or padding mode is invalid
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initEncryptAES128Block(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceAES128Block ) workSpace, CRYPTOCV_P2CONST_PARA( eslt_Byte ) key )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!key) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_AES128BLOCK )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Init actCLib AES128 */
   actAESInitEncryptBlock( (CRYPTOCV_P2VAR_PARA( actAESSTRUCT ))workSpace->wsAES128Block,
                           key,
                           ESL_SIZEOF_AES128_KEY,
                           workSpace->header.watchdog );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_AES128 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_encryptAES128Block
 **
 **  This function encrypts an AES Block (= 16 bytes)
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initEncryptAES128Block(..) function
 ** - inputBlock:  pointer to the given input block (16 bytes)
 ** - outputBlock: pointer to output buffer (at least 16 bytes)
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - outputBlock: pointer to encrypted data block
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initEncryptAES128Block(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_encryptAES128Block(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceAES128Block ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) inputBlock, CRYPTOCV_P2VAR_PARA( eslt_Byte ) outputBlock )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!inputBlock) || (!outputBlock) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_AES128 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_AES128BLOCK )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Update actCLib AES128 */
   actAESEncryptBlock( (CRYPTOCV_P2CONST_PARA( actAESSTRUCT ))workSpace->wsAES128Block,
                       inputBlock,
                       outputBlock,
                       workSpace->header.watchdog );

   return ESL_ERC_NO_ERROR;
}

