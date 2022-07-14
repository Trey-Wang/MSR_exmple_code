/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNAdd.c             $ $Revision:: 1414                 $
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
#include "actWatchdog.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModExp(const actBNDIGIT* gR,
 **                   const actBNDIGIT* e,
 **                   actBNDIGIT* AR,
 **                   const actBNRING* Ring,
 **                   actBNDIGIT* t[])
 **
 **  Modular Exponentiation ( AR = gR^e mod m, m=Ring->m ).
 **  The in- and output is in Montgomery Representation.
 **
 ** input:                                              (length in digits)
 ** - gR:     basis                                         m_length
 ** - e:      exponent ( e > 0 )                            m_length
 ** - AR:     ring element to store the result              m_length+1
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the odd(!) modulus m                  m_length
 ** - Ring->m_length: equals to m_length
 ** - Ring->RR:       constant for Montgomery               m_length
 ** - Ring->m_bar:    constant for Montgomery               1
 ** - Ring->prime_structure==curveid if m is the curve prime
 ** - Ring->prime_structure==0       else
 ** - Ring->window_size (=:k)  for the sliding window algorithm
 ** - t[0..(i-1)]:  i temporary BigNum variables,           i*(m_length+1)
 **                 where i = 2^(k-1)
 ** output:
 ** - AR:     the power gR^e mod m                          m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Ring' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 ** - Ring->m must be odd
 ** - e > 0
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModExp(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) gR, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) e,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) AR, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t[], CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
#if ( actBN_MOD_EXP_WINDOW_SIZE==1 )

   /*
      left-to-right square-and-multiply algorithm
   */

   actBNLENGTH m_length = Ring->m_length;
   actBNLENGTH i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) tmp = t[0];

   actL1trigger( watchdog );

   /* go to msb of 'e' */
   while ( actBNGetBit(e, i) == 0 )
   {
      if ( (i & 0x07) == 0x00 )
      {
         actL3trigger( watchdog );
      }
      --i;
   }

   actL1trigger( watchdog );

   /* AR = gR */
   actBNCopy( AR, gR, m_length );

   actL1trigger( watchdog );

   /* square-and-multiply loop */
   for (--i; i>=0; --i)
   {
      actL3trigger( watchdog );

      /* tmp = AR^2 */
      actBNMontMul( AR, AR, tmp, Ring, watchdog );

      actL3trigger( watchdog );

      /* test bit i of e */
      if ( actBNGetBit(e, i) == 1 )
      {
         actL3trigger( watchdog );

         /* AR = tmp * gR */
         actBNMontMul( tmp, gR, AR, Ring, watchdog );
      }
      else
      {
         actL3trigger( watchdog );

         /* AR = tmp */
         actBNCopy( AR, tmp, m_length );
      }
   }

#elif ( actBN_MOD_EXP_WINDOW_SIZE==2 )

   /*
      left-to-right sliding window algorithm (fixed 2-bit window)
   */

   actBNLENGTH i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) tmp = t[0];
   actU8 e1_bit, e2_bit;

   actL1trigger( watchdog );

   /* go to msb of 'e' */
   while ( actBNGetBit(e, i) == 0 )
   {
      if ( (i & 0x07) == 0x00 )
      {
         actL3trigger( watchdog );
      }
      --i;
   }

   actL1trigger( watchdog );

   if ( (unsigned)i & 1 )
   {
      /* AR = R ( 1-element in Montgomery representation ) */
      actBNSetZero(AR, Ring->m_length); AR[0] = (actBNDIGIT)1;

      actL3trigger( watchdog );

      actBNMontMulCopy( AR, Ring->RR, tmp, Ring, watchdog );
   }
   else
   {
      /* AR = gR */
      actBNCopy( AR, gR, Ring->m_length );
      --i;
   }

   /* tmp = g^2 * R */
   actBNMontMul( gR, gR, tmp, Ring, watchdog );

   actL1trigger( watchdog );

   /* t[1] = g^3 * R */
   actBNMontMul(gR, tmp, t[1], Ring, watchdog);

   /* sliding window loop */
   while ( i>=0 )
   {
      /* test bit i and (i-1) of e */
      e1_bit = (actU8) actBNGetBit(e, i);
      --i;
      e2_bit = (actU8) actBNGetBit(e, i);
      --i;

      actL3trigger( watchdog );

      /* tmp = AR^2 */
      actBNMontMul(AR, AR, tmp, Ring, watchdog);

      actL3trigger( watchdog );

      if ( e1_bit )
      {
         if ( e2_bit )
         {
            /* AR = AR^2 * gR^3 */
            actBNMontMul(tmp, tmp, AR, Ring, watchdog);

            actL3trigger( watchdog );

            actBNMontMulCopy(AR, t[1], tmp, Ring, watchdog);
         }
         else
         {
            /* AR = (AR * gR)^2 */
            actBNMontMul(tmp, gR, AR, Ring, watchdog);

            actL3trigger( watchdog );

            actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
         }
      }
      else
      {
         actBNMontMul(tmp, tmp, AR, Ring, watchdog);

         actL3trigger( watchdog );

         if ( e2_bit )
         {
            /* AR = AR^2 * gR */
            actBNMontMulCopy(AR, gR, tmp, Ring, watchdog);
         }
      }
   }

#elif ( actBN_MOD_EXP_WINDOW_SIZE==3 )

   /*
      left-to-right sliding window algorithm (fixed 3-bit window)
   */

   actBNLENGTH i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) tmp = t[0];
   actU8 e1_bit, e2_bit, e3_bit;

   actL1trigger( watchdog );

   /* go to msb of 'e' */
   while ( actBNGetBit(e, i)==0 )
   {
      if ((i & 0x07) == 0x00)
      {
         actL3trigger( watchdog );
      }
      --i;
   }

   actL1trigger( watchdog );

   /* AR = R ( 1-element in Montgomery representation ) */
   actBNSetZero(AR, Ring->m_length); AR[0] = (actBNDIGIT)1;

   actL1trigger( watchdog );

   actBNMontMulCopy(AR, Ring->RR, tmp, Ring, watchdog);

   /* tmp = g^2 * R */
   actBNMontMul(gR, gR, tmp, Ring, watchdog);

   actL1trigger( watchdog );

   /* t[1] = g^3 * R */
   actBNMontMul(gR, tmp, t[1], Ring, watchdog);

   actL1trigger( watchdog );

   /* t[2] = g^5 * R */
   actBNMontMul(t[1], tmp, t[2], Ring, watchdog);

   actL1trigger( watchdog );

   /* t[3] = g^7 * R */
   actBNMontMul(t[2], tmp, t[3], Ring, watchdog);

   /* sliding window loop */
   while ( i>=2 )
   {
      actL3trigger( watchdog );

      /* test bit i,(i-1) and (i-2) of e */
      e1_bit = (actU8) actBNGetBit(e, i);
      --i;
      e2_bit = (actU8) actBNGetBit(e, i);
      --i;
      e3_bit = (actU8) actBNGetBit(e, i);
      --i;

      actL3trigger( watchdog );

      /* tmp = AR^2 */
      actBNMontMul(AR, AR, tmp, Ring, watchdog);

      actL3trigger( watchdog );

      if ( e1_bit )
      {
         if ( e2_bit )
         {
            /* AR = tmp^2 */
            actBNMontMul(tmp, tmp, AR, Ring, watchdog);

            actL3trigger( watchdog );

            if ( e3_bit )
            {
               /* AR = AR^2 * gR^7 */
               actBNMontMul(AR, AR, tmp, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(tmp, t[3], AR, Ring, watchdog);
            }
            else
            {
               /* AR = (AR * gR^3)^2 */
               actBNMontMul(AR, t[1], tmp, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(tmp, tmp, AR, Ring, watchdog);
            }
         }
         else
         {
            if ( e3_bit )
            {
               /* AR = AR^4 * gR^5 */
               actBNMontMul(tmp, tmp, AR, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(AR, AR, tmp, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(tmp, t[2], AR, Ring, watchdog);
            }
            else
            {
               /* AR = (AR * gR)^4 */
               actBNMontMul(tmp, gR, AR, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(AR, AR, tmp, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(tmp, tmp, AR, Ring, watchdog);
            }
         }
      }
      else
      {
         actL3trigger( watchdog );

         /* AR = tmp^2 */
         actBNMontMul(tmp, tmp, AR, Ring, watchdog);

         actL3trigger( watchdog );

         if ( e3_bit )
         {
            /* tmp = AR^2 */
            actBNMontMul(AR, AR, tmp, Ring, watchdog);

            actL3trigger( watchdog );

            if ( e2_bit )
            {
               /* AR = tmp * gR^3 */
               actBNMontMul(tmp, t[1], AR, Ring, watchdog);
            }
            else
            {
               /* AR = tmp * gR */
               actBNMontMul(tmp, gR, AR, Ring, watchdog);
            }
         }
         else
         {
            if ( e2_bit )
            {
               /* AR = (AR * gR)^2 */
               actBNMontMul(AR, gR, tmp, Ring, watchdog);

               actL3trigger( watchdog );

               actBNMontMul(tmp, tmp, AR, Ring, watchdog);
            }
            else
            {
               /* AR = AR^2 */
               actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
            }
         }
      }
   }

   for ( ; i>=0; --i)
   {
      actL3trigger( watchdog );

      /* AR = AR^2 */
      actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);

      actL3trigger( watchdog );

      /* test bit i of e */
      if ( actBNGetBit(e, i)==1 )
      {
         /* AR = AR * gR */
         actBNMontMulCopy(AR, gR, tmp, Ring, watchdog);
      }
   }

#else

   /*
      left-to-right sliding window algorithm ( (Ring->window_size)-bit window )
   */

   actBNLENGTH i = actBNGetBitLength(Ring->m, Ring->m_length) - 1;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) tmp = t[0];
   actBNLENGTH j, l, index, len;
   actBNLENGTH k = Ring->window_size - 1;
   actBNLENGTH num_of_tmps = 1 << k; /* 2^k powers to precompute */

   actL1trigger( watchdog );

   /* go to msb of 'e' */
   while ( actBNGetBit(e, i)==0 ) { --i;}

   actL3trigger( watchdog );

   /* AR = R ( 1-element in Montgomery representation ) */
   actBNSetZero(AR, Ring->m_length); AR[0] = (actBNDIGIT)1;
   actBNMontMulCopy(AR, Ring->RR, tmp, Ring, watchdog);

   actL3trigger( watchdog );

   /* tmp = g^2 * R */
   actBNMontMul(gR, gR, tmp, Ring, watchdog);
   /* t[1] = g^3 * R */
   actBNMontMul(gR, tmp, t[1], Ring, watchdog);

   actL3trigger( watchdog );

   for (j=2; j<num_of_tmps; ++j)
   {
      /* 'odd powers' from (g^5 * R) to (g^(2^k-1) * R) */
      actBNMontMul(t[j-1], tmp, t[j], Ring, watchdog);
   }
   /* set t[0] pointer to gR */
   t[0] = (CRYPTOCV_P2VAR_PARA(actBNDIGIT)) gR;

   while (i >= 0)
   {
      actL3trigger( watchdog );

      if ( actBNGetBit(e, i)==0 )
      {
         /* AR = AR^2 */
         actBNMontMulCopy(AR, AR, tmp, Ring, watchdog);
         --i;
      }
      else
      {
         /* calculate actuall window_length */
         l = i-k;
         l = actMax(l,0);
         while ( actBNGetBit(e, l)==0 ) {
            ++l;
         }
         len = i-l;

         actL3trigger( watchdog );

         /* AR = AR^(2*window_length) */
         for (j=0; j<=len; j++)
         {
            actBNMontMulCopy( AR, AR, tmp, Ring, watchdog );
         }

         actL3trigger( watchdog );

         /* calculate index for t[] */
         index = 1 << len;
         for (j=l+1; j<i; j++)
         {
            if ( actBNGetBit(e, j)==1 )
            {
               index += (1<<(j-l));
            }
         }

         actL3trigger( watchdog );

         /* AR *= t[index>>1] */
         actBNMontMulCopy( AR, t[(unsigned)index>>1], tmp, Ring, watchdog );

         i = l-1;
      }
   }

   /* reset t[0] pointer */
   t[0] = tmp;

#endif

   actL1trigger( watchdog );
}
