/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/21/06
 ** $Author:: men                      $ $JustDate:: 03/21/06             $
 ** $Workfile:: actExternRandom.c      $ $Revision:: 1648                 $
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
 ** This file contains: The PRNG callback implementation file.
 **
 ** constants:
 **
 **
 ** types:
 **
 ** macros:
 **
 ** global variables:
 **
 ** functions:
 **   actGetExternRandom
 **
 ***************************************************************************/

#include "actConfig.h"
#include "actExternRandom.h"
#include "ESLib_ERC.h"

/****************************************************************************
 ** Extern functions
 ***************************************************************************/
#ifdef __cplusplus
  extern "C" {
#endif

#ifdef actPRNG_CALLBACK
   extern CRYPTOCV_FUNC( int ) actGetRandom ( /* actCLib own callback */
      CRYPTOCV_P2VAR_PARA( actU8 ) target,
      int target_length );
#else
   extern CRYPTOCV_FUNC(eslt_ErrorCode) esl_getBytesRNG(
     actU16 target_length,
     CRYPTOCV_P2VAR_PARA( actU8 ) target );
#endif

#ifdef __cplusplus
  } /* extern "C" */
#endif



/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** int actGetExternRandom (actU8* output, int length)
 **
 **  The function to provide random bytes.
 **
 ** input:
 ** - output:     pointer to the data to be randomized
 ** - length:     length of the data to be randomized in bytes
 **
 ** output:
 ** - output:     the randomized data
 **
 ** assumes:
 **
 ** uses:
 **  (above defined globals)
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actGetExternRandom( CRYPTOCV_P2VAR_PARA( actU8 ) output, int length )
{
#ifdef actPRNG_CALLBACK
   return actGetRandom(output, length); /* actCLib own callback */
#else
  actU8 retVal = actEXCEPTION_UNKNOWN;

  if ( esl_getBytesRNG(((actU16)length), output) == ESL_ERC_NO_ERROR )
  {
    retVal = actOK;
  }

   return retVal;
#endif
}

