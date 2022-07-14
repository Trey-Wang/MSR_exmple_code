/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/04/04
 ** $Author:: alf                      $ $JustDate:: 01/05/05             $
 ** $Workfile:: actAES.c               $ $Revision:: 1851                 $
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
 ** This file contains: A GCM implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** local functions:
 **   basic GCM transformation functions
 **
 ** global functions:
 **   inc32
 **   actGCTR
 **   actGCM_GHash
 **   actGCMInit
 **   actGCMAuthEnc
 **
 ***************************************************************************/


#include "actConfig.h"
#include "actIGCM.h"
#include "actIGHash.h"
#include "actIAES.h"
#include "actUtilities.h"
#include "actWatchdog.h"



CRYPTOCV_FUNC( void ) inc32 (     /* this should work on actU32* ?   At least if endianess is choosen correctly */
   CRYPTOCV_P2VAR_PARA( actU8 ) block )
{
   actU32 counter;

   counter = ((actU32)block[0]<<24 | (actU32)block[1]<<16 | (actU32)block[2]<<8 | (actU32)block[3]);
   counter++;
   block[0] = (actU8)(counter>>24);
   block[1] = (actU8)(counter>>16);
   block[2] = (actU8)(counter>>8);
   block[3] = (actU8)(counter);
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMInit(actGCMSTRUCT* info,
                         const actU8* K,
                         int K_len,
                         const actU8* IV,
                         int IV_len,
                         void (*watchdog) (void))
 **
 **  This function initializes the Galois/Counter-Mode.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - key:        master key
 ** - key_length: length of key in bytes
 ** - iv:         initialization vector
 ** - iv_length:  length of the initialization vector (max 2^32 bit)
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       initialized context structure
 ** - returns:    actEXCEPTION_LENGTH    if key_len < 1
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMInit (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) K,
   int K_len,
   CRYPTOCV_P2CONST_PARA( actU8 ) IV,
   int IV_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) KeyGen = &info->aes;
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) ghash = &info->ghash;
   actU8 H[actGCM_BLOCK_SIZE];
   actU32 iv_lo_bitcount = (actU32)IV_len << 3;
   actU32 iv_hi_bitcount = 0; /* adjust this if longer iv sizes should be possible */

   if ( K_len<1 ) {
      return actEXCEPTION_LENGTH;
   }
   
   info->state = STATE_AAD;
   info->buffer_used = 0;
   info->aad_lo_bytecount = info->aad_hi_bytecount = 0;
   info->data_lo_bytecount = info->data_hi_bytecount = 0;
   

   /* AES encrypt zero block for subkey H and use that to init GHash*/
   actAESInitEncryptBlock(KeyGen, K, K_len, watchdog);
   actMemset(KeyGen->prev_block, 0, actGCM_BLOCK_SIZE);
   actAESEncryptBlock(KeyGen, KeyGen->prev_block, H, watchdog);
   actGHashInit(ghash, H, watchdog);

   if (IV_len == actGCM_IV_DEFAULT_SIZE) {
      /* Set J0 = IV || 0^31 || 2 */
      actMemcpy(info->J, IV, actGCM_IV_DEFAULT_SIZE);
      info->J[actGCM_IV_DEFAULT_SIZE] = 0u;
      info->J[actGCM_IV_DEFAULT_SIZE+1] = 0u;
      info->J[actGCM_IV_DEFAULT_SIZE+2] = 0u;
      info->J[actGCM_IV_DEFAULT_SIZE+3] = 1u;
   }
   else {
      /* J0 = GHASH_H(IV || 0^s+64 || len_64(IV) )*/
      actU8 lenBlock[actGCM_BLOCK_SIZE];
      
      actGHashUpdate(ghash, IV, IV_len, watchdog);
      actGHashZeroPad(ghash, watchdog);

      /* pad 64-bit input count to last block in big endian format */
      actMemset(lenBlock, 0, 8);
      
#ifdef LITTLE_ENDIAN
      *((CRYPTOCV_P2VAR_PARA( actU32 ))(&lenBlock[8])) = actSwap(iv_hi_bitcount);
      *((CRYPTOCV_P2VAR_PARA( actU32 ))(&lenBlock[12])) = actSwap(iv_lo_bitcount);
#else
      *((CRYPTOCV_P2VAR_PARA( actU32 ))(&lenBlock[8])) = iv_hi_bitcount;
      *((CRYPTOCV_P2VAR_PARA( actU32 ))(&lenBlock[12])) = iv_lo_bitcount;
#endif      
      
      actGHashUpdate(ghash, lenBlock, actGCM_BLOCK_SIZE, watchdog);
      actGHashFinalize(ghash, info->J, watchdog);
      /* Init again to clear internal state */
      actGHashReset(ghash);
   }

   /* Encrypt the initial counter block for XOR in tag generation */
   actAESEncryptBlock(KeyGen, info->J, info->J0, watchdog);
   
   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actGCMTransformBlock(actGCMSTRUCT *info, actU8 *X, actU8 *output, int encORdec, void (*watchdog) (void))
 **
 **  This function encrypts/decrypts one complete or partial block and updates the GHash accordingly.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - output:     pointer to the output buffer
 ** - encORdec:   switch to distinguish between encryption and decryption
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
static CRYPTOCV_FUNC( void ) actGCMTransformBlock (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   int encORdec,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actU8 block[actAES_BLOCK_SIZE];
   int block_len;
   
   if (info->buffer_used == 0) {
      /* buffer empty ==> complete block */
      block_len = actGCM_BLOCK_SIZE;
   } else { 
      /* buffer not empty ==> partial block */
      block_len = info->buffer_used;
   }
   
   if (encORdec == MODE_DEC)
      actGHashUpdate(&info->ghash, X, block_len, watchdog);
   
   inc32(&info->J[12]);
   actAESEncryptBlock(&info->aes, info->J, block, watchdog);
   actXOR(block, X, block_len);
   actMemcpy(output, block, block_len);
   
   if (encORdec == MODE_ENC)
      actGHashUpdate(&info->ghash, output, block_len, watchdog);
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMTransform( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            int encORdec,
                            void (*watchdog) (void))
 **
 **  This function encrypts/decrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - encORdec:   switch to distinguish between encryption and decryption
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     encrypted/decrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    actEXCEPTION_LENGTH   aad length exceeds 2^64 - 1 bit or
 **                                     plaintext/ciphertext  2^39 - 256 bit
 **               actOK                 else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMTransform (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   int encORdec,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   switch( info->state ) {
      case STATE_AAD: {
         /* update byte count */
         if( (info->aad_lo_bytecount + (actU32)X_len) < info->aad_lo_bytecount ) {
            if ( (++info->aad_hi_bytecount)==((actU32)1<<29) ) { /* add carry */
               return actEXCEPTION_LENGTH; /* overflow: 2^64 bit input */
            }
         }
         info->aad_lo_bytecount += (actU32)X_len;
         
         actGHashUpdate(&info->ghash, X, X_len, watchdog);
         /* actMemcpy(*output, *X, X_len);  */ /* ?! */
         /* output = X; ?! */
         break;
      }
      case STATE_CIPHERTEXT: {
         int i, blocks, diff = 0;
         
         /* update byte count */
         if ( (info->data_lo_bytecount + (actU32)X_len == (actU32)0xffffff00) && (info->data_hi_bytecount == (actU32)0xffff)) {
            return actEXCEPTION_LENGTH; /* overflow: 2^39-256 bit input */
         } else if ( (info->data_lo_bytecount + (actU32)X_len) < info->data_lo_bytecount )
            info->data_hi_bytecount++;
         
         info->data_lo_bytecount += (actU32)X_len;
         
         /* left overs of the last update */
         if ( info->buffer_used > 0 ) {
            diff = actMin(X_len, (int)(actGCM_BLOCK_SIZE - info->buffer_used));
            actMemcpy(info->buffer + info->buffer_used, X, (unsigned int)diff);
            info->buffer_used += diff;
            if( info->buffer_used < actGCM_BLOCK_SIZE ) {
               return actOK;
            } else {
               actGCMTransformBlock(info, info->buffer, output, encORdec, watchdog);
               output += actGCM_BLOCK_SIZE;
               *bytes_out += actGCM_BLOCK_SIZE;
            }
         }

         /* loop through (remaining) X, blockwise */
         blocks = (X_len - diff) / actGCM_BLOCK_SIZE;
         X += diff;
         
         for (i = blocks; i > 0; --i, X += actGCM_BLOCK_SIZE, output += actGCM_BLOCK_SIZE) {
            actGCMTransformBlock(info, X, output, encORdec, watchdog);
         }   

         /* copy left overs into buffer */
         info->buffer_used = X_len - (diff + actGCM_BLOCK_SIZE * blocks);
         actMemcpy(info->buffer, X, (unsigned int)(info->buffer_used));
         
         *bytes_out += blocks*actGCM_BLOCK_SIZE;
         
         break;
      }
   }
   
   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMEncryptUpdate( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            void (*watchdog) (void))
 **
 **  This function encrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - output:     encrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    return value of actGCMTransform
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMEncryptUpdate (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actGCMTransform(info, X, X_len, output, bytes_out, MODE_ENC, watchdog);
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMDecryptUpdate( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            void (*watchdog) (void))
 **
 **  This function decrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - output:     decrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    return value of actGCMTransform
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMDecryptUpdate (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actGCMTransform(info, X, X_len, output, bytes_out, MODE_DEC, watchdog);
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMTriggerState(actGCMSTRUCT *info,
 **                                  void (*watchdog) (void))
 **
 **  This function switches the internal state from AAD to P / C, 
 **  so actGCM*Update now expects plaintext/ciphertext.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - returns:    actEXCEPTION_UNKNOWN  if called from an infalid state
 **                                     (everything other than STATE_AAD)
 **               actOK                 else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMTriggerState (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   if (info->state != STATE_AAD)
      return actEXCEPTION_UNKNOWN;
   /* zero-pad and hash the authenticated data */
   actGHashZeroPad(&info->ghash, watchdog);
   /* switch over to ciphertext input now */
   info->state = STATE_CIPHERTEXT;
   
   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGMACFinalize(actGCMSTRUCT *info, 
 **                               actU8 *tag, 
 **                               void (*watchdog) (void))
 **
 **  This function finishes the plaintext / ciphertext section and computes the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGMACFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actU8 tmpBlock[actGCM_BLOCK_SIZE];
   actU32 aad_lo_bitcount = info->aad_lo_bytecount << 3;
   actU32 aad_hi_bitcount = (info->aad_hi_bytecount << 3) | (info->aad_lo_bytecount >> 29);
   actU32 data_lo_bitcount = info->data_lo_bytecount << 3;
   actU32 data_hi_bitcount = (info->data_hi_bytecount << 3) | (info->data_lo_bytecount >> 29);
   
   /* Zero-Pad the ciphertext hash */
   actGHashZeroPad(&info->ghash, watchdog);
   
   /* pad 64-bit input counts to last block in big endian format */
#ifdef LITTLE_ENDIAN
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[0])) = actSwap(aad_hi_bitcount);
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[4])) = actSwap(aad_lo_bitcount);
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[8])) = actSwap(data_hi_bitcount);
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[12])) = actSwap(data_lo_bitcount);
#else
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[0])) = aad_hi_bitcount;
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[4])) = aad_lo_bitcount;
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[8])) = data_hi_bitcount;
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&tmpBlock[12])) = data_lo_bitcount;
#endif
   
   
   
   actGHashUpdate(&info->ghash, tmpBlock, actGCM_BLOCK_SIZE, watchdog);
   actGHashFinalize(&info->ghash, tmpBlock, watchdog);

   /* use tmpBlock for the full tag now */
   actXOR(tmpBlock, info->J0, actGCM_BLOCK_SIZE);
   /* return the truncated tag */
   actMemcpy(tag, tmpBlock, actGCM_TAG_LENGTH);
   
   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMEncryptFinalize(actGCMSTRUCT *info, 
 **                                 actU8 *output, 
 **                                 int *bytes_out,
 **                                 actU8 *tag, 
 **                                 void (*watchdog) (void))
 **
 **  This function finalizes encryption and returns the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - output:     pointer to output buffer for remaining data bytes
 ** - bytes_out:  pointer to output counter
 ** - tag:        pointer to output buffer for the tag
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     remaining data bytes
 ** - bytes_out:  incrementecd by no. of remaining bytes
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMEncryptFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   /* If there is still input in the buffer, encrypt it as the last block */
   if (info->buffer_used > 0) {
      *bytes_out += info->buffer_used;
      actGCMTransformBlock(info, info->buffer, output, MODE_ENC, watchdog);
   }
   
   return actGMACFinalize(info, tag, watchdog);
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMDecryptFinalize(actGCMSTRUCT *info, 
 **                                 actU8 *output, 
 **                                 int *bytes_out,
 **                                 actU8 *tag, 
 **                                 void (*watchdog) (void))
 **
 **  This function finalizes decryption and returns the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - output:     pointer to output buffer for remaining data bytes
 ** - bytes_out:  pointer to output counter
 ** - tag:        pointer to output buffer for the tag
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     remaining data bytes
 ** - bytes_out:  incrementecd by no. of remaining bytes
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMDecryptFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ))
{
   /* If there is still input in the buffer, decrypt it as the last block */
   if (info->buffer_used > 0) {
      *bytes_out += info->buffer_used;
      actGCMTransformBlock(info, info->buffer, output, MODE_DEC, watchdog);
   }
   
   return actGMACFinalize(info, tag, watchdog);
}
