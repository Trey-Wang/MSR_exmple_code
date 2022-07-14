/****************************************************************************
 ** Main author: Ubr                     Creation date: 10/17/03
 ** $Author:: mns                      $ $JustDate:: 03/18/05             $
 ** $Workfile:: actISHA.h              $ $Revision:: 1435                 $
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
 ** This file contains: The interface for SHA-1 hash algorithm.
 **
 ** constants:
 **  actHASH_SIZE_SHA
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actSHAInit
 **   actSHAUpdate
 **   actSHAFinalize
 **
 ***************************************************************************/


#ifndef ACTISHA_H
#define ACTISHA_H


#include "actITypes.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actHASH_SIZE_SHA         20u
#define actHASH_BLOCK_SIZE_SHA   64u

/* Workspace structure for SHA-1 */
typedef struct {
   actU32 H[5];                  /* message digest state buffer */
   actU32 low_count, hi_count;   /* 64 bit input count */
   int buffer_used;               /* number of bytes saved in buffer */
   actU8 buffer[actHASH_BLOCK_SIZE_SHA];     /* remaining data buffer */
} actSHASTRUCT;


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAInit(actSHASTRUCT* info)
 **
 **  This function initializes the SHA-1 algorithm.
 **
 ** input:
 ** - info:       pointer to hash context structure
 **
 ** output:
 ** - info:       initialized hash context structure
 ** - returns:    actOK allways
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAInit( CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAUpdate(actSHASTRUCT* info,
 **                            const actU8* dataIN,
 **                            int length,
 **                            void (*watchdog) (void));
 **
 **  This function hashes the given data and can be called arbitrary
 **  often between an initialize and finalize of the SHA-1 algorithm.
 **  Uses any data already in the actSHASTRUCT structure and leaves
 **  any partial data block there.
 **
 ** input:
 ** - info:       pointer to hash context structure
 ** - dataIN:     pointer to data to be hashed
 ** - length:     length of data in bytes
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       actualized hash context structure
 ** - returns:    actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 **               actOK                 else
 **
 ** assumes:
 ** - actSHAInit() is called once before calling this function
 ** - dataIN != NULL is a valid pointer
 ** - length >= 0
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAUpdate(
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAFinalize(actSHASTRUCT* info,
 **                              actU8 hash[actHASH_SIZE_SHA],
 **                              void (*watchdog) (void));
 **
 **  This function finalizes the SHA algorithm and delivers the hash value.
 **
 ** input:
 ** - info:       pointer to hash context structure
 ** - hash:       byte array to contain the hash value
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       finalized hash context structure
 ** - hash:       the final hash value,
 **                  (big endian of length actHASH_SIZE_SHA)
 ** - returns:    actOK allways
 **
 ** assumes:
 ** - actSHAInit() is called once before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHA
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAFinalize(
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 )  hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTISHA_H */
