/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/18/05
 ** $Author:: mns                      $ $JustDate:: 03/18/05             $
 ** $Workfile:: actIHashMAC.c          $ $Revision:: 1435                 $
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
 ** This file contains: The interface for SHA-1 based Hash MAC algorithm.
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


#include "actIHashMAC.h"
#include "actUtilities.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACInit(actHASHMACSTRUCT* info),
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
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACInit(
   CRYPTOCV_P2VAR_PARA( actHASHMACSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) sha = &info->sha;
   CRYPTOCV_P2VAR_PARA( actU8 ) tmp_block = info->sha.buffer;
   int i;

   /* store key */
   if ( key_length < 1 )
   {
      return actEXCEPTION_LENGTH;
   }
   else if ( key_length <= actHASH_BLOCK_SIZE_SHA )
   {
      /* copy key to key_buf */
      actMemcpy( info->key_buf, key, (unsigned int)key_length );
      info->key_length = key_length;
   }
   else
   {
      /* use SHA-1(key) */
      (void)actSHAInit( sha );
      (void)actSHAUpdate( sha, key, key_length, watchdog );
      (void)actSHAFinalize( sha, info->key_buf, watchdog );
      info->key_length = actHASH_SIZE_SHA;
   }

   /* ipad */
   actMemcpy( tmp_block, info->key_buf, (unsigned int)(info->key_length) );
   actMemset( tmp_block+info->key_length, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA-info->key_length) );
   for( i=0; i < actHASH_BLOCK_SIZE_SHA; ++i )
   {
      tmp_block[i] ^= (actU8)(0x36);
   }

   (void)actSHAInit( sha );
   (void)actSHAUpdate( sha, tmp_block, actHASH_BLOCK_SIZE_SHA, watchdog );

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACUpdate(actHASHMACSTRUCT* info,
 **                   const actU8* dataIN,
 **                   int length,
 **                   void (*watchdog) (void)))
 **
 **  This function hashes the given data and can be called arbitrary
 **  often between an initialize and finalize of the HashMAC algorithm.
 **  Uses any data already in the actSHASTRUCT structure and leaves
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
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACUpdate(
   CRYPTOCV_P2VAR_PARA( actHASHMACSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actSHAUpdate( &info->sha, dataIN, length, watchdog );
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACFinalize(actHASHMACSTRUCT* info,
 **                     actU8 hash[actHASH_SIZE_SHA]),
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
 **                  (big endian of length actHASH_SIZE_SHA)
 ** - returns:    actOK allways
 **
 ** assumes:
 ** - actHashMACInit() is called once before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHA
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACFinalize(
   CRYPTOCV_P2VAR_PARA( actHASHMACSTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) sha = &info->sha;
   CRYPTOCV_P2VAR_PARA( actU8 ) tmp_block = info->sha.buffer;
   int i;

   (void)actSHAFinalize( sha, hash, watchdog );

   /* opad */
   actMemcpy( tmp_block, info->key_buf, (unsigned int)(info->key_length) );
   actMemset( tmp_block+info->key_length, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA-info->key_length) );
   for( i=0; i<actHASH_BLOCK_SIZE_SHA; ++i )
   {
      tmp_block[i] ^= (actU8)(0x5c);
   }

   (void)actSHAInit( sha );
   (void)actSHAUpdate( sha, tmp_block, actHASH_BLOCK_SIZE_SHA, watchdog );
   (void)actSHAUpdate( sha, hash, actHASH_SIZE_SHA, watchdog );
   (void)actSHAFinalize( sha, hash, watchdog );

   return actOK;
}

