/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBigNumGCD.h         $ $Revision:: 1414                 $
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
 ** This file contains: A basic (unsigned) integer and modulo arithmetic
 **                     used for elliptic curve point arithmetic.
 **
 ** functions:
 **
 ** - unsigned integer functions:
 **
 **   actBNGCDix
 **
 ***************************************************************************/


#ifndef ACTEXTERNRANDOM_H
#define ACTEXTERNRANDOM_H


#include "actITypes.h"
#include "actConfig.h"


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/


#ifdef  __cplusplus
extern "C" {
#endif


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
 **
 **
 ***************************************************************************/

extern CRYPTOCV_FUNC( int ) actGetExternRandom( CRYPTOCV_P2VAR_PARA( actU8 ) output, int length );

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* ACTEXTERNRANDOM_H */

