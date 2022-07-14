/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNOddInvModBase.c   $ $Revision:: 1414                 $
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
 ** This file contains: modular inversion of BigNum type
 **
 **
 ** functions:
 **   actBNOddInvModBase
 **
 ***************************************************************************/


#include "actBigNum.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNOddInvModBase(actBNDIGIT *inverse, actBNDIGIT value)
 **
 **   Inversion of an odd digit mod base.
 **
 ** input:                                              (length in digits)
 ** - value:      odd digit to be inverted                    1
 ** - inverse     result digit                                1
 **
 ** output:
 ** - inverse     value^-1 mod 2^bits_per_digit               1
 **
 ** assumes:
 **
 ** remark:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNOddInvModBase( CRYPTOCV_P2VAR_PARA( actBNDIGIT ) inverse, actBNDIGIT value )
{
   actBNDDIGIT d = 1;
   actBNDDIGIT pow = 1;
   actU8 i;

   *inverse = 0;
   for ( i=0; i<actBN_BITS_PER_DIGIT; ++i )
   {
      pow <<= 1;
      if( d%pow!=0 )
      {
         d = (actBNDDIGIT)(d+value);
         (*inverse)= (actBNDIGIT)((*inverse)+(actBNDIGIT)(pow>>1));
      }
      value<<= 1;
   }
}

