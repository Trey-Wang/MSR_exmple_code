/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/18/05
 ** $Author:: mneuhaus                 $ $JustDate:: 03/18/05             $
 ** $Workfile:: actIHashMAC.h          $ $Revision:: 2276                 $
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
 ** This file contains: The interface for GHash algorithm
 **
 ** constants:
 **
 ** types:
 **   actHASHMACSTRUCT
 **
 ** macros:
 **
 ** functions:
 **   actGHashInit
 **   actGHashUpdate
 **   actGHashZeroPad
 **   actGHashFinalize
 **
 ***************************************************************************/


#ifndef ACTIGHASH_H
#define ACTIGHASH_H


#include "actConfig.h"
#include "actITypes.h"
#include "actIAES.h"


/*
 ###########################################################################
 ##
 ##   LIMIT SPEED UP SETTING!
 ##   Please read the "library/es Porting Guide"
 ##   on how to further speed up GHash!
 ##
 ###########################################################################
*/
#if (actGHASH_SPEED_UP > 1)
  #define actGHASH_SPEED_UP   1   /*  limit speedup  */
#endif


/****************************************************************************
 ** Types and constants
 ***************************************************************************/
 
#define actGHASH_BLOCK_SIZE        16u
#define actGHASH_WORD_SIZE         4u
#define actGHASH_WORDS_PER_BLOCK   actGHASH_BLOCK_SIZE/actGHASH_WORD_SIZE

/* the polynomial is 1+a+a^2+a^7+a^128 => 111000010...01 in little endian representation. */
#define GF128POLY                  (0xE1000000u)

/* Workspace structure for GHash */
typedef struct {
   int buffer_used;                     /* number of bytes saved in buffer */
   actU8 buffer[actGHASH_BLOCK_SIZE];   /* the input buffer */
   actU32 H[actGHASH_WORDS_PER_BLOCK];  /* the hash subkey */
   actU8 Y[actGHASH_BLOCK_SIZE];        /* the hash state buffer */
#if   (actGHASH_SPEED_UP >= 4)
   actU32 M[256][4];                    /* the precomputation table */
#elif (actGHASH_SPEED_UP >= 1)
   actU32 M[16][4];                     /* the precomputation table */
#endif
} actGHASHSTRUCT;



/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGHashReset(actGHASHSTRUCT* info)
 **
 **  This function resets the hash-block Y to zero
 **  It does NOT import the hash-subkey
 **  
 ** input:
 ** - info        pointer to context structure
 **
 ** output:
 ** - info:       initialized context structure
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGHashReset (
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) info );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGHashInit(actGHASHSTRUCT* info,
 **                            const actU8 H[actAES_BLOCK_SIZE],
 **                            void (*watchdog) (void))
 **
 **  This function initializes the hash-block Y to zero
 **  
 ** input:
 ** - info        pointer to context structure
 ** - H:          the hash subkey
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       initialized context structure
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGHashInit (
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) H,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGHashUpdate(actGHASHSTRUCT* info,
 **                              const actU8* X,
 **                              int X_len,
 **                              void (*watchdog) (void))
 **
 **  This function hashes the given data and can be called arbitrarily
 **  often between an initialize and finalize of the GHash algorithm.
 **  Incomplete blocks are padded with zeroes.
 **  
 ** input:
 ** - info:       pointer to initialized context structure
 ** - X:          the input string
 ** - X_len:      length of X in bytes
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       the updated context structure
 ** - returns:    actOk always
 **
 ** assumes:
 ** - actGHashInit() is called once before calling this function
 ** - X != NULL is a valid pointer
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGHashUpdate (
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   int X_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGHashZeroPad(actGHASHSTRUCT* info,
 **                              void (*watchdog) (void))
 **
 **  If there is input left in the buffer, this function pads it with 
 **  zeroes and hashes it, otherwise the hash doesn't need to be padded. 
 **  
 ** input:
 ** - info:       pointer to initialized context structure
 ** - X:          the input string
 ** - X_len:      length of X in bytes
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       the updated context structure
 ** - returns:    actOk always
 **
 ** assumes:
 ** - actGHashInit() is called once before calling this function
 ** - X != NULL is a valid pointer
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGHashZeroPad (
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) info,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGHashFinalize(actGHASHSTRUCT* info,
 **                               actU8 hash[actGHASH_BLOCK_SIZE],
 **                               void (*watchdog) (void))
 **
 **  This function finalizes the GHash algorithm and outputs the hash value
 **  
 ** input:
 ** - info:       pointer to initialized context structure
 ** - hash:       byte array to contain the hash value
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - hash:       the final hash value
 ** - returns:    actOK always
 **
 ** assumes:
 ** - actGHashInit() is called once before calling this function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGHashFinalize (
   CRYPTOCV_P2VAR_PARA( actGHASHSTRUCT ) info,
   actU8 hash[actGHASH_BLOCK_SIZE],
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTIGHASH_H */
