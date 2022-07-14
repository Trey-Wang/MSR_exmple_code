/****************************************************************************
 ** Main author: Ubr                     Creation date: 04/21/05
 ** $Author:: mns                      $ $JustDate:: 04/21/05             $
 ** $Workfile:: actFIPS186.h           $ $Revision:: 928                  $
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
 **     Layer: Core Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: The FIPS186 PRNG interface.
 **                     Implementation reference is algorithm A.4.1,
 **                     ANSI X9.62, using base 2^b, b = 160.
 **
 ** constants:
 **   actFIPS186_SEED_SIZE
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actInitializeFIPS186
 **   actGetFIPS186
 **   actWriteFIPS186
 **
 ***************************************************************************/

#ifndef ACTFIPS186_H
#define ACTFIPS186_H


#include "actITypes.h"
#include "actISHA.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/

#define actFIPS186_BASE_LENGTH   20   /* byte_length of base 2^b, b = 160  */
#define actFIPS186_SEED_SIZE     actFIPS186_BASE_LENGTH

/* Workspace structure for FIPS186 */
typedef struct
{
   actU8 X_KEY[actFIPS186_BASE_LENGTH];
   actSHASTRUCT sha1;
   int bytes_remain;
} actFIPS186STRUCT;


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/


#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 **   actInitializeFIPS186
 **
 **  This function initializes the FIPS186 pseudo random number generator.
 **  It must be called at least once before using the generator.
 **
 ** input:
 ** - info:        pointer to FIPS context structure
 ** - seed:        Initial value of the generator. The seed is the 'true'
 **                random input of a PRNG. We need a 160 bit seed.
 **
 ** output:
 ** - info:        initialized FIPS context structure
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actInitializeFIPS186(
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info, CRYPTOCV_P2CONST_PARA( actU8 ) seed );

/****************************************************************************
 **
 ** FUNCTION:
 **   actGetFIPS186
 **
 **  The function to provide random bytes.
 **
 ** input:
 ** - info:        pointer to FIPS context structure
 ** - output:      pointer to the data to be randomized
 ** - length:      length of the data to be randomized in bytes
 **
 ** output:
 ** - info:        updated FIPS context structure
 ** - output:      the randomized data
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actGetFIPS186(
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) output, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **   actWriteFIPS186
 **
 **  This function puts additional entropy into the FIPS186 generator.
 **  The new input seed is added (XOR) to the internal state of the PRNG.
 **
 ** input:
 ** - info:        pointer to FIPS context structure
 ** - new_seed:    new seed value to be added to the internal state.
 **
 ** output:
 ** - info:        updated FIPS context structure
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actWriteFIPS186(
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info, CRYPTOCV_P2CONST_PARA( actU8 ) new_seed );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTFIPS186_H */
