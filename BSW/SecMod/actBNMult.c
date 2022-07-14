/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNMult.c            $ $Revision:: 1414                 $
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
 **   actBNMult
 **
 ***************************************************************************/


#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMult(const actBNDIGIT* a,
 **                 const actBNLENGTH a_length,
 **                 const actBNDIGIT* b,
 **                 const actBNLENGTH b_length,
 **                 actBNDIGIT* c);
 **
 **   BigNum Multiply ( c = a*b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  a_length
 ** - b:       second number                                 b_length
 ** - c:       the result                                 a_length+b_length
 **
 ** output:
 ** - c:       the result a * b                           a_length+b_length
 **
 ** assumes:
 **
 ** remark:
 ** - c cannot overlap with a or b ( a*=b or b*=a is not possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMult(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const actBNLENGTH a_length,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b, const actBNLENGTH b_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actBNLENGTH i, j, c_length = a_length + b_length;
   actBNDDIGIT tmp;
   actBNDIGIT carry;

   actL2trigger( watchdog );

   actBNSetZero( c, c_length ); /* c = 0 */

   /* school method */
   for ( i=0; i<b_length; ++i )
   {
      carry = 0;
      for (j=0; j<a_length; ++j)
      {
         tmp = ((actBNDDIGIT)a[j]) * b[i] + c[i+j] + carry;
         c[i+j] = actBNLoWord( tmp );
         carry = actBNHiWord( tmp );
      }
      c[i+a_length] = carry;
   }
}

