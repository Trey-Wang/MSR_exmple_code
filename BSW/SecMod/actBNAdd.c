/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNAdd.c            $ $Revision:: 1414                 $
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
 **   actBNAdd
 **
 ***************************************************************************/


#include "actBigNum.h"
#include "actUtilities.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNAdd(const actBNDIGIT* a,
 **               const actBNDIGIT* b,
 **               actBNDIGIT* c,
 **               const actBNLENGTH length)
 **
 **   BigNum Addition ( c = a+b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  length
 ** - b:       second number                                 length
 ** - c:       the result                                    length
 ** - length:  length of a and b
 **
 ** output:
 ** - c:       the result a + b                              length
 ** - returns: the last carry ( 0 or 1 )
 **
 ** assumes:
 ** - a_length == b_length == length
 **
 ** remark:
 ** - c may overlap with a or b ( a+=b or b+=a is possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNAdd(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, const actBNLENGTH length )
{
   actBNLENGTH i;
   actBNDDIGIT tmp = 0;

   for(i=0; i<length; ++i)
   {
      /* tmp = a[i] + b[i] + carry */
      tmp = ((actBNDDIGIT)a[i]) + b[i] + actBNHiWord( tmp );
      c[i] = actBNLoWord( tmp );
   }

   /* return the last carry */
   return (int)actBNHiWord( tmp );
}
