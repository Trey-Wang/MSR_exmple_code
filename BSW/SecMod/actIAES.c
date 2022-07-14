/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/04/04
 ** $Author:: alf                      $ $JustDate:: 01/05/05             $
 ** $Workfile:: actIAES.c              $ $Revision:: 1414                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006                          )
 **
 ** \version(1.0 (beta )                                                  )
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: Core Module
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: A AES implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   bit rotation and basic AES transformation macros (see bellow)
 **
 ** local functions:
 **   basic AES transformation functions (see bellow)
 **   actAESInit
 **
 ** global functions:
 **   actAESInitEncryptBlock
 **   actAESInitDecryptBlock
 **   actAESEncryptBlock
 **   actAESDecryptBlock
 **   actAESInitEncrypt
 **   actAESInitDecrypt
 **   actAESEncrypt
 **   actAESDecrypt
 **
 ***************************************************************************/

#include "actIAES.h"
#include "actAES.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitEncryptBlock()
 **
 **
 **  This function initializes the AES block encryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitEncryptBlock(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actAESKeySetup( info, key, key_len, watchdog );
}

/****************************************************************************
 **
 ** FUNCTION:
 **  void actAESEncryptBlock()
 **
 **  This function encrypts the given block.
 **
 ** input:
 ** - info->e_key: pointer to expanded encryption key
 ** - in_block:    pointer to block to be encrypted
 ** - out_block:   pointer to allocated output block
 **
 ** output:
 ** - out_block:  encrypted output block
 **
 ** assumes:
 ** - actAESInitEncryptBlock(info) is called once before calling
 **   this function
 ** - 'in_block' and 'out_block' are of size actAES_BLOCK_SIZE
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actAESEncryptBlock(
   CRYPTOCV_P2CONST_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in_block, CRYPTOCV_P2VAR_PARA( actU8 ) out_block,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2CONST_PARA( actU32 ) k_ptr = info->e_key;
   actU32 b0[4], b1[4];
   int i, iterations = 4 + (info->key_dword_len-4)/2;

   actL1trigger( watchdog );

   /* b0 = in_block */
   BlockIO( (CRYPTOCV_P2CONST_PARA( void )) in_block,
            (CRYPTOCV_P2VAR_PARA( void )) b0 );

   /* encryption */
   for (i=0; i<4; ++i) { b0[i] ^= *k_ptr++;}
   for (i=0; i<iterations; ++i)
   {
      f_nround(b1, b0, k_ptr); k_ptr += 4;
      f_nround(b0, b1, k_ptr); k_ptr += 4;
      actL2trigger( watchdog );
   }
   f_nround(b1, b0, k_ptr); k_ptr += 4;
   f_lround(b0, b1, k_ptr);

   /* out_block = b0 */
   BlockIO( (CRYPTOCV_P2CONST_PARA( void )) b0,
            (CRYPTOCV_P2VAR_PARA( void )) out_block );

   actL1trigger(watchdog);
}

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitDecryptBlock()
 **
 **
 **  This function initializes the AES block decryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitDecryptBlock(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result = actAESKeySetup( info, key, key_len, watchdog );

   if ( result == actOK )
   {
      actAESMakeDecryptionKey( info );
   }

   return result;
}

/****************************************************************************
 **
 ** FUNCTION:
 **  void actAESDecryptBlock()
 **
 **  This function decrypts the given block.
 **
 ** input:
 ** - info->e_key: pointer to expanded decryption key
 ** - in_block:    pointer to block to be decrypted
 ** - out_block:   pointer to allocated output block
 **
 ** output:
 ** - out_block:  decrypted output block
 **
 ** assumes:
 ** - actAESInitDecryptBlock(info) is called once before calling
 **   this function
 ** - 'in_block' and 'out_block' are of size actAES_BLOCK_SIZE
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actAESDecryptBlock(
   CRYPTOCV_P2CONST_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in_block, CRYPTOCV_P2VAR_PARA( actU8 ) out_block,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2CONST_PARA( actU32 ) k_ptr = info->e_key + 4*info->key_dword_len + 20;
   actU32 b0[4], b1[4];
   int i, iterations = 4 + (info->key_dword_len-4)/2;

   actL1trigger( watchdog );

   /* b0 = in_block */
   BlockIO( (CRYPTOCV_P2CONST_PARA( void )) in_block,
            (CRYPTOCV_P2VAR_PARA( void )) b0 );

   /* decryption */
   for (i=0; i<4; ++i) { b0[i] ^= k_ptr[i+4];}
   for (i=0; i<iterations; ++i)
   {
      i_nround(b1, b0, k_ptr); k_ptr -= 4;
      i_nround(b0, b1, k_ptr); k_ptr -= 4;
      actL2trigger( watchdog );
   }
   i_nround(b1, b0, k_ptr); k_ptr -= 4;
   i_lround(b0, b1, k_ptr);

   /* out_block = b0 */
   BlockIO( (CRYPTOCV_P2CONST_PARA( void )) b0,
            (CRYPTOCV_P2VAR_PARA( void )) out_block );

   actL1trigger( watchdog );
}

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitEncrypt()
 **
 **  This function initializes the AES algorithm (key setup etc.).
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 ** - iv:         pointer to initialization vector (only in CBC mode;
 **               of length actAES_BLOCK_SIZE; if iv==NULL, a zeroized
 **               block is used as iv)
 ** - mode:       ECB, CBC mode switch
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actEXCEPTION_ALGOID    mode is invalid
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitEncrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len, CRYPTOCV_P2CONST_PARA( actU8 ) iv,
   actU8 mode, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;

   /* Check padding mode */
   result = actPaddingInit(mode & actPADDING_PM_MASK);

   if (result == actOK)
   {
      result = actAESInit( info, key, key_len, iv, mode, watchdog );
   }

   return result;
}

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESEncrypt()
 **
 **  This function encrypts input data of any length.
 **  If final==0, ((in_length+buffered_bytes)/16) blocks will be encrypted
 **  and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will
 **  be buffered, where 0 <= buffered_bytes < 16.
 **  If final!=0, (((in_length+buffered_bytes)/16)+1) blocks will be
 **  encrypted and written to out_buffer. PKCS 5 padding is used for the
 **  last block padding.
 **
 ** input:
 ** - info:        pointer to initialized AES context structure
 ** - in:          pointer to data to be encrypted
 ** - in_length:   length of data in bytes
 ** - out_buffer:  pointer to encrypted data buffer
 ** - *out_length: out_buffer size in bytes, a save length is:
 **                  (((in_length+15)/16)+1)*16       (see output)
 ** - final:       flag for padding
 **
 ** output:
 ** - info->prev_block:   updated previous block
 ** - out_buffer:         encrypted data blocks
 ** - *out_length:        number of encrypted bytes:
 **                  ((in_length+buffered_bytes)/16)*16       if final==0
 **                  (((in_length+buffered_bytes)/16)+1)*16   else
 ** - returns:    actEXCEPTION_LENGTH   *out_length is too small
 **               actOK                 else
 **
 ** assumes:
 ** - actAESInitEncrypt() is called once before calling this function
 ** - in!=NULL, out_buffer!=NULL are a valid pointers
 ** - in_length>=0, *out_length>=0 (at input)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESEncrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, int in_length,
   CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer, CRYPTOCV_P2VAR_PARA( int ) out_length,
   int final, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result = actOK;
   /* out_size = blocks*16 */
   int out_size = ((in_length + info->buffer_used) / actAES_BLOCK_SIZE) * actAES_BLOCK_SIZE;

   /* Check out_buffer size */
   if ((final == 0) || ((info->mode & actPADDING_PM_MASK) == actPADDING_PM_OFF))
   {
      if (*out_length < out_size)
      {
         return actEXCEPTION_LENGTH;
      }

      *out_length = out_size;
   }
   else
   {
      if (*out_length < (int)(out_size + actAES_BLOCK_SIZE))
      {
         return actEXCEPTION_LENGTH;
      }

      *out_length = out_size + actAES_BLOCK_SIZE;
   }

   /* transform input */
   if ( in_length>0 )
   {
      actAESTransform(info, in, in_length, out_buffer, actAESEncryptMultipleBlocks, watchdog);
   }

   /* Finalize? */
   if (final != 0)
   {
      result = actPaddingPad(info->mode & actPADDING_PM_MASK, actAES_BLOCK_SIZE, &info->buffer_used, info->buffer);

      if ((result == actOK) && (info->buffer_used > 0u))
      {
         /* Encryption of last block */
         actAESEncryptMultipleBlocks(info, info->buffer, &out_buffer[out_size], 1, watchdog);
      }
   }

   actL1trigger( watchdog );

   return result;
}

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitDecrypt()
 **
 **  This function initializes the AES decryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 ** - iv:         pointer to initialization vector (only in CBC mode;
 **               of length actAES_BLOCK_SIZE; if iv==NULL, a zeroized
 **               block is used as iv)
 ** - mode:       ECB, CBC mode switch
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actEXCEPTION_ALGOID    mode is invalid
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitDecrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len, CRYPTOCV_P2CONST_PARA( actU8 ) iv,
   actU8 mode, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result;

   /* Check padding mode */
   result = actPaddingInit(mode & actPADDING_PM_MASK);

   if (result == actOK)
   {
      result = actAESInit( info, key, key_len, iv, mode, watchdog );

      if ( result == actOK )
      {
         actAESMakeDecryptionKey( info );
      }
   }

   return result;
}

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESDecrypt()
 **
 **  This function decrypts input data of any length.
 **  If final==0, ((in_length+buffered_bytes)/16) blocks will be decrypted
 **  and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will
 **  be buffered, where 0 <= buffered_bytes < 16.
 **  If final!=0, ((in_length+buffered_bytes)/16) blocks will be decrypted
 **  and written to out_buffer, where (in_length+buffered_bytes) is a
 **  multiple of 16. PKCS 5 padding is used to unpad the last block.
 **
 ** input:
 ** - info:        pointer to initialized AES context structure
 ** - in:          pointer to data to be decrypted
 ** - in_length:   length of data in bytes ( multiple of 16 )
 ** - out_buffer:  pointer to decrypted data buffer of length in_length
 ** - *out_length: out_buffer size in bytes, a save length is:
 **                  ((in_length+15)/16)*16       (see output)
 ** - final:       flag for unpadding
 **
 ** output:
 ** - info->prev_block:   updated previous block
 ** - out_buffer:         decrypted data blocks
 ** - *out_length:        number of decrypted bytes:
 **    ((in_length+buffered_bytes)/16)*16          if final==0
 **    (in_length+buffered_bytes)/16)*16 - pad     else, where 1<=pad<=16
 ** - returns:    actEXCEPTION_LENGTH   *out_length is too small
 ** (if final!=0) actEXCEPTION_INPUT_LENGTH (in_length+buffered_bytes)%16!=0
 **               actEXCEPTION_PADDING  PKCS 5 unpad error
 **               actOK                 else
 **
 ** assumes:
 ** - actAESInitDecrypt() is called once before calling this function
 ** - in!=NULL, out_buffer!=NULL are a valid pointers
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESDecrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, int in_length,
   CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer, CRYPTOCV_P2VAR_PARA( int ) out_length,
   int final, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE result = actOK;
   int total_bytes = in_length + info->buffer_used;
   int out_size;

   /* if finalize: check (in_length+buffer_length)%16==0 */
   if ( final != 0 )
   {
      if ( (!total_bytes) || ((total_bytes%actAES_BLOCK_SIZE) != 0) )
      {
         return actEXCEPTION_INPUT_LENGTH;
      }
   }

   if (total_bytes > 0)
   {
     /* out_size = blocks*16 */
     out_size = ((total_bytes-1)/actAES_BLOCK_SIZE)*actAES_BLOCK_SIZE;
   }
   else
   {
     /* No ouput data will be produced as no input data present */
     out_size = 0;
   }

   /* check out_buffer size */
   if ( *out_length < out_size )
   {
      return actEXCEPTION_LENGTH;
   }

   /* transform input */
   if ( in_length > 0 )
   {
      if (total_bytes <= actAES_BLOCK_SIZE)
      {
         actMemcpy( info->buffer+info->buffer_used, in, (unsigned int)in_length );
         info->buffer_used += in_length;
      }
      else
      {
         if ((total_bytes % actAES_BLOCK_SIZE) == 0)
         {
            actAESTransform( info, in, (int) (in_length-actAES_BLOCK_SIZE), out_buffer, actAESDecryptMultipleBlocks, watchdog );
            actMemcpy( info->buffer, in+in_length-actAES_BLOCK_SIZE, actAES_BLOCK_SIZE );
            info->buffer_used = actAES_BLOCK_SIZE;
         }
         else
         {
            actAESTransform( info, in, in_length, out_buffer, actAESDecryptMultipleBlocks, watchdog );
         }
      }
   }

   if (final != 0)
   {
      /* decryption of the last buffered block */
      actAESDecryptMultipleBlocks( info, info->buffer, info->buffer, 1, watchdog );

      result = actPaddingUnpad(info->mode & actPADDING_PM_MASK, actAES_BLOCK_SIZE, &info->buffer_used, info->buffer);

      if (result == actOK)
      {
         if (*out_length < (out_size + info->buffer_used))
         {
            result = actEXCEPTION_LENGTH;
         }
         else
         {
            actMemcpy(&out_buffer[out_size], info->buffer, info->buffer_used);
            *out_length = out_size + info->buffer_used;
         }
      }
   }
   else
   {
      *out_length = out_size;
   }

   actL1trigger( watchdog );
   return result;
}
