/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/18/05
 ** $Author:: mns                      $ $JustDate:: 03/18/05             $
 ** $Workfile:: actIHashMAC.h          $ $Revision:: 1435                 $
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
 **   actHASHMACSTRUCT
 **
 ** macros:
 **
 ** functions:
 **   actHashMACInit
 **   actHashMACUpdate
 **   actHashMACFinalize
 **
 ***************************************************************************/


#ifndef ACTIHASHMAC_H
#define ACTIHASHMAC_H


#include "actITypes.h"
#include "actISHA2_32.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/


/* Workspace structure for HashMAC SHA-256 */
typedef struct
{
   actSHA256STRUCT sha256;
   actU8 key_buf[actHASH_BLOCK_SIZE_SHA256];
   int key_length;
} actHASHMACSHA256STRUCT;



/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Init(actHASHMACSHA256STRUCT* info,
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
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Update(actHASHMACSHA256STRUCT* info,
 **                   const actU8* dataIN,
 **                   int length,
 **                   void (*watchdog) (void));
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
 ** - actHashMACSHA256Init() is called once before calling this function
 ** - dataIN != NULL is a valid pointer
 ** - length >= 0
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACSHA256Update(
   CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actHashMACSHA256Finalize(actHASHMACSHA256STRUCT* info,
 **                     actU8 hash[actHASH_SIZE_SHASHA256]
 **                     void (*watchdog) (void))
 **
 **  This function finalizes the HashMAC algorithm and
 **  delivers the hash value.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - hash:       byte array to contain the hash value
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
CRYPTOCV_FUNC( actRETURNCODE ) actHashMACSHA256Finalize(
   CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTIHASHMAC_H */

