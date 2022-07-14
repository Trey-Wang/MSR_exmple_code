/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNModSub.c          $ $Revision:: 1414                 $
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
 **   actBNModSub
 **
 ***************************************************************************/


#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModSub(const actBNDIGIT* a,
 **                   const actBNDIGIT* b,
 **                   actBNDIGIT* c,
 **                   const actBNRING* Ring)
 **
 **  Modular Subtraction ( c = (a-b) mod m, m=Ring->m ).
 **
 ** input:                                              (length in digits)
 ** - a:      first ring element                            m_length
 ** - b:      second ring element                           m_length
 ** - c:      ring element to store the result              m_length
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the modulus m                         m_length
 ** - Ring->m_length: equals to m_length
 **
 ** output:
 ** - c:      the result a - b mod m                        m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Ring' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition in actBigNum.h;
 **   an example for a complete initialization is the actECDSAVerify(..)
 **   implementation in actIECDSA.c.
 **
 ** remark:
 ** - c may overlap with a or b ( a-=b or b-=a is possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModSub(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b, CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c,
   CRYPTOCV_P2CONST_PARA( actBNRING ) Ring, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actBNLENGTH length = Ring->m_length; /* modulus length */

   actL2trigger( watchdog );

   if ( actBNCompare( a, b, length ) == -1 )
   {
      /* a<b, do c=m-(b-a) */
      actBNSub( b, a, c, length );
      actBNSub( Ring->m, c, c, length );
   }
   else
   {
      /* a>=b, do c=a-b */
      actBNSub( a, b, c, length );
   }
}

