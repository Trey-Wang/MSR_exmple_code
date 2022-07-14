/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNSub.c             $ $Revision:: 1414                 $
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
 ** This file contains: Implementation file for actBigNum.h
 **
 ** functions:
 **
 **   actBNSub
 **
 ***************************************************************************/


#include "actBigNum.h"
#include "actUtilities.h"

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNSub(const actBNDIGIT* a,
 **                const actBNDIGIT* b,
 **                actBNDIGIT* c,
 **                const actBNLENGTH length)
 **
 **   BigNum Subtraction ( c = a-b for a>=b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  length
 ** - b:       second number                                 length
 ** - c:       the result                                    length
 ** - length:  length of a and b
 **
 ** output:
 ** - c:       the result a - b                              length
 **
 ** assumes:
 ** - a_length == b_length == length
 ** - a >= b
 **
 ** remark:
 ** - c may overlap with a or b ( a-=b or b-=a is possible )
 ** - a>=b means the last carry is allways zero, thus actBNSub(..)
 **   is of type void
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNSub(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, const actBNLENGTH length )
{
   actBNLENGTH i;
   actBNDDIGIT tmp = actBN_BASE;

   for ( i=0; i<length; ++i )
   {
      /* tmp = a[i] - b[i] - carry */
      tmp = ( actBN_BASE | a[i] ) - b[i] - (1 - actBNHiWord(tmp));
      c[i] = actBNLoWord(tmp);
   }
}

