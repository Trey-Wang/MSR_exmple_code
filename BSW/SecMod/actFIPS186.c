/****************************************************************************
 ** Main author: Ubr                     Creation date: 10/17/03
 ** $Author:: mns                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actFIPS186.c            $ $Revision:: 928                  $
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
 **     Layer: Core Module
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: The FIPS 186 PRNG implementation file.
 **                     Implementation reference is algorithm A.4.1,
 **                     ANSI X9.62, using base 2^b, b = 160.
 **
 ** constants:
 **
 **
 ** types:
 **
 ** macros:
 **
 ** global variables:
 **   see below
 **
 ** functions:
 **   (local) pseudoRandom
 **   actInitializeFIPS186
 **   actGetFIPS186
 **   actWriteFIPS186
 **
 ***************************************************************************/


#include "actFIPS186.h"
#include "actUtilities.h"


/****************************************************************************
 ** Local Functions
 ***************************************************************************/
static CRYPTOCV_FUNC( void ) pseudoRandom(
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) sha1  = &(info->sha1);
   CRYPTOCV_P2VAR_PARA( actU8 )        X     = info->sha1.buffer;
   CRYPTOCV_P2VAR_PARA( actU8 )        X_KEY = info->X_KEY;
   CRYPTOCV_P2CONST_PARA( actU8 )      H_ptr =
      (CRYPTOCV_P2CONST_PARA( actU8 ))(&(info->sha1.H[0]));
   int i;

   /* X = XKEY mod 2^b, b = 160 */
   for (i=0; i<actFIPS186_BASE_LENGTH; ++i) { X[i] = X_KEY[i]; }

   /* X = SHA1(X) */

   /* we don't finalize SHA-1 and use the zero padding of X */
   actSHAInit( sha1 );
   actSHAUpdate( sha1,
                 (CRYPTOCV_P2CONST_PARA( actU8 ))X,
                 actHASH_BLOCK_SIZE_SHA,
                 watchdog );
#ifdef LITTLE_ENDIAN
   /* correct endianess of SHA-1 message digest H[] */
   { int w; for(w=0; w<5; ++w) { sha1->H[w] = actSwap(sha1->H[w]); }}
#endif
   for (i=0; i<actFIPS186_BASE_LENGTH; ++i) { X[i] = H_ptr[i]; }
   info->bytes_remain = actFIPS186_BASE_LENGTH;

   /* XKEY = (1 + XKEY + X) mod 2^160 */
   {
      actU8 carry = 1;
      actU16 tmp;

      for (i=actFIPS186_BASE_LENGTH-1; i>=0; --i) {
         tmp = (actU16) (((actU16)X_KEY[i]) + X[i] + carry);
         X_KEY[i] = (actU8)tmp;
         carry = (actU8)(tmp>>8);
      }
   }
}


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** void actInitializeFIPS186 (actFIPS186STRUCT *info,
 **                            const actU8 seed[actFIPS186_SEED_SIZE])
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
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info, CRYPTOCV_P2CONST_PARA( actU8 ) seed )
{
   actMemcpy( info->X_KEY, seed, actFIPS186_BASE_LENGTH );
   actMemset( info->sha1.buffer, 0, actHASH_BLOCK_SIZE_SHA );
   info->bytes_remain = 0;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  void actGetFIPS186 (actFIPS186STRUCT *info,
 **                      actU8* output,
 **                      int length);
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
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2CONST_PARA( actU8 ) tmp_iter;
   CRYPTOCV_P2VAR_PARA( actU8 )   X = info->sha1.buffer;
   int i = 0;

   while (i < length) {
      tmp_iter = X + actFIPS186_BASE_LENGTH - info->bytes_remain;
      for( ; (info->bytes_remain != 0) && (i < length) ; ++i, --info->bytes_remain ) {
         output[i] = *tmp_iter++;
      }
      if (info->bytes_remain == 0) {
         pseudoRandom(info, watchdog);
      }
   }
}


/****************************************************************************
 **
 ** FUNCTION:
 **  void actWriteFIPS186 (actFIPS186STRUCT *info,
 **                        const actU8 new_seed[actFIPS186_SEED_SIZE])
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
   CRYPTOCV_P2VAR_PARA( actFIPS186STRUCT ) info, CRYPTOCV_P2CONST_PARA( actU8 ) new_seed )
{
   int i=0;
   for ( ; i<actFIPS186_BASE_LENGTH; ++i) { info->X_KEY[i] ^= new_seed[i]; }
}

