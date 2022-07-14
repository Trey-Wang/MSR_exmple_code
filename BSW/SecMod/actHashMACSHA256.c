/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/18/05
 ** $Author:: mns                      $ $JustDate:: 03/18/05             $
 ** $Workfile:: actIHashMACSHA256.c    $ $Revision:: 1435                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2005                          )
 **
 ** \version(1.0 (beta )                                                  )
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: User Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: The interface for SHA-256 based Hash MAC algorithm.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actHashMACInit
 **   actHashMACUpdate
 **   actHashMACFinalize
 **
 ***************************************************************************/


#include "actIHashMACSHA256.h"
#include "actUtilities.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Init(actHASHMACSHA256STRUCT* info),
 **                              const actU8* key,
 **                              int key_length,
 **                              void (*watchdog) (void))
 **
 **  This function initializes the HashMAC algorithm.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - key:        MAC key
 ** - key_length: length of key in bytes
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
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACSHA256Init(
   CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) sha256 = &info->sha256;
   CRYPTOCV_P2VAR_PARA( actU8 ) tmp_block = info->sha256.buffer;
   int i;

   /* store key */
   if ( key_length < 1 )
   {
      return actEXCEPTION_LENGTH;
   }
   else if ( key_length <= actHASH_BLOCK_SIZE_SHA256 )
   {
      /* copy key to key_buf */
      actMemcpy( info->key_buf, key, (unsigned int)key_length );
      info->key_length = key_length;
   }
   else
   {
      /* use SHA-256(key) */
      (void)actSHA256Init( sha256 );
      (void)actSHA256Update( sha256, key, key_length, watchdog );
      (void)actSHA256Finalize( sha256, info->key_buf, watchdog );
      info->key_length = actHASH_SIZE_SHA256;
   }

   /* ipad */
   actMemcpy( tmp_block, info->key_buf, (unsigned int)(info->key_length) );
   actMemset( tmp_block+info->key_length, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA256-info->key_length) );
   for( i=0; i < actHASH_BLOCK_SIZE_SHA256; ++i )
   {
      tmp_block[i] ^= (actU8)(0x36);
   }

   (void)actSHA256Init( sha256 );
   (void)actSHA256Update( sha256, tmp_block, actHASH_BLOCK_SIZE_SHA256, watchdog );

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Update(actHASHMACSHA256STRUCT* info,
 **                   const actU8* dataIN,
 **                   int length,
 **                   void (*watchdog) (void)))
 **
 **  This function hashes the given data and can be called arbitrary
 **  often between an initialize and finalize of the HashMAC algorithm.
 **  Uses any data already in the actSHA256STRUCT structure and leaves
 **  any partial data block there.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - dataIN:     pointer to data to be hashed
 ** - length:     length of data in bytes
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       actualized context structure
 ** - returns:    actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 **               actOK                 else
 **
 ** assumes:
 ** - actHashMACInit() is called once before calling this function
 ** - dataIN != NULL is a valid pointer
 ** - length >= 0
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACSHA256Update(
   CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actSHA256Update( &info->sha256, dataIN, length, watchdog );
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Finalize(actHASHMACSHA256STRUCT* info,
 **                     actU8 hash[actHASH_SIZE_SHA256]),
 **                     void (*watchdog) (void))
 **
 **  This function finalizes the HashMAC algorithm and
 **  delivers the hash value.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - hash:       byte array to contain the hash value
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       finalized context structure
 ** - hash:       the final hash value,
 **                  (big endian of length actHASH_SIZE_SHA256)
 ** - returns:    actOK allways
 **
 ** assumes:
 ** - actHashMACSHA256Init() is called once before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHA256
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACSHA256Finalize(
   CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) sha256 = &info->sha256;
   CRYPTOCV_P2VAR_PARA( actU8 ) tmp_block = info->sha256.buffer;
   int i;

   (void)actSHA256Finalize( sha256, hash, watchdog );

   /* opad */
   actMemcpy( tmp_block, info->key_buf, (unsigned int)(info->key_length) );
   actMemset( tmp_block+info->key_length, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA256-info->key_length) );
   for ( i=0; i < actHASH_BLOCK_SIZE_SHA256; ++i )
   {
      tmp_block[i] ^= (actU8)(0x5c);
   }

   (void)actSHA256Init( sha256 );
   (void)actSHA256Update( sha256, tmp_block, actHASH_BLOCK_SIZE_SHA256, watchdog );
   (void)actSHA256Update( sha256, hash, actHASH_SIZE_SHA256, watchdog );
   (void)actSHA256Finalize( sha256, hash, watchdog );

   return actOK;
}

