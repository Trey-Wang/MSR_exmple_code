/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNReduce.c          $ $Revision:: 1414                 $
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
#include "actWatchdog.h"


/****************************************************************************
 ** Local Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 **  static actBNDIGIT actBNMulDigit(const actBNDIGIT* a,
 **                                  const actBNLENGTH a_length,
 **                                  const actBNDIGIT b,
 **                                  actBNDIGIT* c)
 **
 **   BigNum multiply digit ( c = a*b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  a_length
 ** - b:       digit                                           1
 ** - c:       the result buffer                             a_length
 **
 ** output:
 ** - c:       the result a * b                            a_length
 ** - returns: the last carry digit                            1
 **
 ** assumes:
 **
 ** remark:
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( actBNDIGIT ) actBNMulDigit(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const actBNLENGTH a_length,
   const actBNDIGIT b, CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c )
{
   actBNLENGTH i;
   actBNDDIGIT tmp = 0;

   /* school method */
   for (i=0; i<a_length; ++i)
   {
      tmp = ((actBNDDIGIT)a[i]) * b + actBNHiWord(tmp);
      c[i] = actBNLoWord(tmp);
   }
   return actBNHiWord(tmp);
}



/****************************************************************************
 **
 ** FUNCTION:
 **  static void actBNNormalize(actBNDIGIT* a,
 **                             actBNLENGTH* a_length,
 **                             actBNDIGIT* b,
 **                             actBNLENGTH b_length,
 **                             int shift)
 **
 **   Normalization for actBNReduce function.
 **
 ** input:                                              (length in digits)
 ** - a:          number to be shifted                        *a_length+1
 ** - *a_length:  length of a
 ** - b:          number to be shifted                         b_length
 ** - b_length:   length of b
 ** - shift:      bits to be shifted:
 **               0 < shift < actBN_BITS_PER_DIGIT
 **
 ** output:
 ** - a:          shifted number                           *a_length(+1)
 ** - b:          shifted number                             b_length
 ** - *a_length:  length of shifted number
 **
 ** assumes:
 **
 ** remark:
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( void ) actBNNormalize(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actBNLENGTH* a_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b, actBNLENGTH b_length, int shift )
{
   actBNDIGIT tmp = (actBNDIGIT)( ((actBNDIGIT)1) << shift ); /* tmp = 2^shift */
   if ( ( a[*a_length] = actBNMulDigit( a, *a_length, tmp, a ) )!=0 ) { ++(*a_length);}  /* a *= tmp */
   (void)actBNMulDigit( b, b_length, tmp, b );  /* b *= tmp */
}


/****************************************************************************
 **
 ** FUNCTION:
 **  static void actBNReNormalize(actBNDIGIT* r,
 **                               actBNDIGIT* b,
 **                               actBNLENGTH b_length,
 **                               int shift)
 **
 **   Undo normalization for actBNReduce function.
 **
 ** input:                                              (length in digits)
 ** - r:          number to be shifted                         b_length+1
 ** - b:          number to be shifted                         b_length
 ** - b_length:   length of b and r
 ** - shift:      bits to be shifted:
 **               0 < shift < actBN_BITS_PER_DIGIT
 **
 ** output:
 ** - r:          shifted number                             b_length
 ** - b:          shifted number                             b_length
 **
 ** assumes:
 **
 ** remark:
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( void ) actBNReNormalize(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b, actBNLENGTH b_length, int shift )
{
   actBNDIGIT tmp = (actBNDIGIT)( ((actBNDIGIT)1) << (actBN_BITS_PER_DIGIT-shift) ); /* tmp = 2^(bits_per_digit-shift) */
   r[b_length] = actBNMulDigit(r, b_length, tmp, r);  /* r *= tmp */
   actBNCopy(r, r+1, b_length); /* shift r one digit to the right */
   tmp = actBNMulDigit(b, b_length, tmp, b);   /* b *= tmp, write last carry to tmp */
   actBNCopy(b, b+1, b_length-1); /* shift b one digit to the right */
   b[b_length-1] = tmp;
}



/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNReduce(actBNDIGIT* a,
 **                  actBNLENGTH a_length,
 **                  actBNDIGIT* b,
 **                  actBNLENGTH b_length,
 **                  actBNDIGIT* r,
 **                  void (*watchdog) (void));
 **
 **   BigNum reduce ( r = a mod b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number (will be distroyed)              a_length+1
 ** - b:       second number (will be returned const         b_length
 **            but touched by the normalization)
 ** - r:       result buffer                                 b_length+1
 **
 ** output:
 ** - a:       the normalized result a mod b                 b_length
 ** - b:       second number                                 b_length
 ** - r:       the result a mod b                            b_length
 ** - returns: -1    if exact b_length <= 1 (error)
 **             0    else (success)
 **
 ** assumes:
 **
 ** remark:
 ** - a, b and r cannot overlap
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNReduce(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actBNLENGTH a_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b, actBNLENGTH b_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actL1trigger( watchdog );

   /* init r, correct length of a and b */
   actBNSetZero(r, b_length);
   while ( (a_length>1) && (a[a_length-1]==0) ) --a_length;
   while ( (b_length>1) && (b[b_length-1]==0) ) --b_length;

   /* reference: MOV'97, Algo. 14.20 */
   if ( b_length<=1 )
   {
      return -1; /* error: b too short */
   }
   else
   {
      actBNLENGTH i, n, t, shift;
      actBNDIGIT qDig, Dig0, Dig1, Dig2;
      actBNDDIGIT tmp;

      /* abort, if a < b ? */
      if ( a_length==b_length )
      {
         if ( actBNCompare( a, b, a_length ) == -1 )
         {
            actBNCopy( r, a, a_length ); /* r = a */
            return 0;                    /* done  */
         }
      }
      if ( a_length < b_length )
      {
         actBNCopy( r, a, a_length );    /* r = a */
         return 0;                       /* done  */
      }

      /* [step 1] normalization */
      shift = 0;
      Dig0 = b[b_length-1];
      while ( Dig0<actBN_MAX_BIT ) { Dig0<<=1; ++shift;}
      if ( shift > 0 )
      {
         actBNNormalize(a, &a_length, b, b_length, shift);
      }

      /* init n, t */
      n = a_length - 1;
      t = b_length - 1;

      /* [step 2] subtract b from b_length most significant digits of a */
      a += (n-t);
      if ( actBNCompare( a, b, b_length ) >= 0 )
      {
         actBNSub(a, b, a, b_length);
      }
      a -= (n-t);

      /* [step 3] main division loop */
      for ( i=n; i>t; --i )
      {
         actL2trigger( watchdog );

         /* [3.1] estimation of quotient digit qDig[i-t-1] */
         if ( a[i]==b[b_length-1] )
         {
            qDig = 0;
         }
         else
         {
            tmp = ( ( ((actBNDDIGIT)a[i]) << actBN_BITS_PER_DIGIT ) + a[i-1] ) / b[b_length-1];
            qDig = (actBNDIGIT)( ((actBNDIGIT)tmp) + 1 ); /* hi word is zero */
         }

         /* [3.2] check: qDig[i-t-1]*(b[b_length-1]*base + b[b_length-2]) > a[i]*base^2 + a[i-1]*base + a[i-2] */
         do
         {
            --qDig;
            tmp = ((actBNDDIGIT)qDig) * b[b_length-2];
            Dig0 = actBNLoWord(tmp);
            tmp = ((actBNDDIGIT)qDig) * b[b_length-1] + actBNHiWord(tmp);
            Dig1 = actBNLoWord(tmp);
            Dig2 = actBNHiWord(tmp);

            actL3trigger( watchdog );
         } while ( (Dig2>a[i]) || ((Dig2==a[i])&&(Dig1>a[i-1])) || ((Dig2==a[i])&&(Dig1==a[i-1])&&(Dig0>a[i-2])) );

         /* [3.3 + 3.4] subtract r = qDig[i-t-1]*b from b_length+1 most significant digits of a */
         r[b_length] = actBNMulDigit(b, b_length, qDig, r);
         a += (i-t-1);

         actL3trigger( watchdog );

         if ( actBNCompare(a, r, b_length+1)==-1 )
         {
            actBNSub( r, b, r, b_length + 1 ); /* qDig is still too large (by 1) */
         }

         actL3trigger( watchdog );

         actBNSub( a, r, a, b_length + 1 );
         a -= (i-t-1);
      }

      actL3trigger( watchdog );

      /* [step 4] copy a to r */
      actBNCopy( r, a, b_length );

      actL3trigger( watchdog );

      /* renormalize r and b */
      if ( shift>0 )
      {
         actBNReNormalize( r, b, b_length, shift );
      }
   }

   actL1trigger( watchdog );

   return 0;
}

