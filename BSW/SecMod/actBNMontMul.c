/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: alf                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBNMontMul.c         $ $Revision:: 1414                 $
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
 **   actBNMontMul
 **   actBNMontMulCopy
 **
 ***************************************************************************/


#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMontMul(const actBNDIGIT* x,
 **                    const actBNDIGIT* y,
 **                    actBNDIGIT* A,
 **                    const actBNRING* Ring)
 **
 **  Montgomery Multiplication ( A = (x*y*R^-1) mod m, m=Ring->m is odd ).
 **  Implementation Reference: MOV'97, algo 14.36
 **
 ** input:                                              (length in digits)
 ** - x:      first ring element                            m_length
 ** - y:      second ring element                           m_length
 ** - A:      ring element to store the result              m_length+1
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the odd(!) modulus m                  m_length
 ** - Ring->m_length: equals to m_length
 ** - Ring->m_bar:    -(m^-1) mod base                      1
 **   For actBN_MONT_MUL_VERSION 1:
 ** - Ring->prime_structure==curveid if m is the curve prime
 ** - Ring->prime_structure==0       else
 **
 ** output:
 ** - A:      the product (x*y*R^-1) mod m                  m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Ring' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition in actBigNum.h;
 **   an example for a complete initialization is the actECDSAVerify(..)
 **   implementation in actIECDSA.c.
 ** - Ring->m must be odd
 **
 ** remark:
 ** - A cannot overlap with x or y ( x*=y or y*=x is not possible )
 **
 ***************************************************************************/

/***************************************************************************/
#if ( actBN_MONT_MUL_VERSION==0 )
/***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMontMul(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) A, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) m = Ring->m;
   actBNLENGTH n    = Ring->m_length;
   actBNDIGIT m_bar = Ring->m_bar;
   actBNDIGIT y_0   = y[0];
   actBNDIGIT m_0   = m[0];
   CRYPTOCV_P2VAR_PARA( actBNDIGIT )   A_j = A;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) m_1 = m + 1;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) m_j;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y_1 = y + 1;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y_j;
   actBNDIGIT u_i, x_i;
   actBNDDIGIT tmp1, tmp2;
   actBNLENGTH i, j;

   actL1trigger( watchdog );

   /* Step 1: A = 0 */
   for (j=n+1; j!=0; --j)
   {
      *A_j++ = 0;
   }

   /* Step 2: */
   for (i=n; i!=0; --i)
   {
      /* 2.1: u_i = (a_0 + x_i*y_0) * m' mod b */
      /* in step 2.2 */

      /* 2.2: A = (A + x_i*y + u_i*m) / b */

      /* unroll the (j=0)-iteration of the scalar mult loop and */
      /* calculate u_i (step 2.1) */
      A_j = A;
      x_i = *x++;
      tmp1 = ((actBNDDIGIT)y_0) * x_i + (*A_j);
      u_i = (*A_j) = actBNLoWord(tmp1);
      u_i *= m_bar;
      tmp2 = ((actBNDDIGIT)m_0) * u_i + (*A_j++);

      /* set y_j to y, m_j to m */
      y_j = y_1; m_j = m_1;

      /* scalar mult loop */
      for (j=n-1; j!=0; --j)
      {
         /* 2.2.1: A += x_i*y   (scalar mult) */
         /* tmp = y[j] * x_i + A[j] + carry; */
         tmp1 = ((actBNDDIGIT)(*y_j++)) * x_i + (*A_j++) + actBNHiWord(tmp1);
         /* 2.2.2: A = (A + u_i*m) / b   (scalar mult with 1-digit-right-shift) */
         /* tmp = m[j] * u_i + A[j] + carry; */
         tmp2 = ((actBNDDIGIT)(*m_j++)) * u_i + actBNLoWord(tmp1) + actBNHiWord(tmp2);
         (*(A_j-2)) = actBNLoWord(tmp2);
         actL3trigger( watchdog );
      }
      /* unroll the (j=n)-iteration of the scalar mult loop (A < 2m-1) */
      tmp1 = ((actBNDDIGIT)(*A_j)) + actBNHiWord(tmp1) + actBNHiWord(tmp2);
      (*(A_j-1)) = actBNLoWord(tmp1);
      (*A_j) = actBNHiWord(tmp1);
      /* A_j points to A[n] now */

      actL2trigger( watchdog );
   }

   /* Step 3: if A >= m then A -= m */
   if ( (*A_j)==1 ) /* A[n]==1 */
   {
      /* A has one Bit more than m, so A>m, do A-=m */
      actBNSub( A, m, A, n ); (*A_j) = 0;
   }
   else
   {
      if ( actBNCompare(A, m, n)>=0 )
      {
         actL2trigger( watchdog );
         /* A>=m, do A-=m */
         actBNSub( A, m, A, n );
      }
   }

   actL1trigger( watchdog );
}

/***************************************************************************/
#elif ( actBN_MONT_MUL_VERSION==1 )
/***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMontMul(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) A, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) m = Ring->m;
   actBNLENGTH n         = Ring->m_length;
   actBNDIGIT m_bar      = Ring->m_bar;
   int prime_structure = Ring->prime_structure;
   actBNDIGIT y_0        = y[0];
   actBNDIGIT m_0        = m[0];
   CRYPTOCV_P2VAR_PARA( actBNDIGIT )   A_j = A;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) m_1 = m + 1;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y_1 = y + 1;
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) tmp_j;
   actBNDIGIT u_i, x_i;
   actBNDDIGIT tmp;
   actBNDIGIT hi_carry;
   actBNLENGTH i, j;

   actL1trigger( watchdog );

   /* Step 1: A = 0 */
   for (j=n+1; j!=0; --j)
   {
      *A_j++ = 0;
   }

   /* Step 2: */
   for (i=n; i!=0; --i)
   {
      /* 2.1: u_i = (a_0 + x_i*y_0) * m' mod b */
      /* in step 2.2 */

      /* 2.2: A = (A + x_i*y + u_i*m) / b */

      /* unroll the (j=0)-iteration of the scalar mult loop and */
      /* calculate u_i (step 2.1) */
      A_j = A;
      x_i = *x++;
      tmp = (actBNDDIGIT) y_0 * x_i + (*A_j);
      u_i = (*A_j++) = actBNLoWord(tmp);
      u_i *= m_bar;

      /* set tmp_j to y */
      tmp_j = y_1;

      /* scalar mult loop */
      for (j=n-1; j!=0; --j)
      {
         /* tmp = y[j] * x_i + A[j] + carry; */
         tmp = ((actBNDDIGIT)(*tmp_j++)) * x_i + (*A_j) + actBNHiWord(tmp);
         (*A_j++) = actBNLoWord(tmp);
      }
      /* unroll the (j=n)-iteration of the scalar mult loop; A and (x_i*y) */
      /* may have n+1 digits (A < 2m-1) */
      tmp = ((actBNDDIGIT)(*A_j)) + actBNHiWord(tmp);
      (*A_j) = actBNLoWord(tmp);
      hi_carry = actBNHiWord(tmp);  /* is 0 or 1 (add-carry) */
      /* A_j points to A[n] now */

      /* 2.2.2: A = (A + u_i*m) / b   (scalar mult with 1-digit-right-shift) */

      if ( prime_structure )
      {
         /* A_j has to point to A[n] before entering this macro */
         /* not supported so far */
         /* actBNOptimizedScalarMult(prime_structure); */
         /* A_j points to A[n] after leaving this macro */
      }
      else
      {
         /* unroll the (j=0)-iteration of the scalar mult loop; because */
         /* of the shift, we only need the high word of tmp */
         A_j = A;
         tmp = ((actBNDDIGIT)m_0) * u_i + (*A_j++);

         /* set tmp_j to m */
         tmp_j = m_1;

         /* scalar mult loop with implicit 1-digit-right-shift */
         for (j=n-1; j!=0; --j)
         {
            actL3trigger( watchdog );
            /* tmp = m[j] * u_i + A[j] + carry; */
            tmp = ((actBNDDIGIT)(*tmp_j++)) * u_i + (*A_j++) + actBNHiWord(tmp);
            (*(A_j-2)) = actBNLoWord(tmp);
         }
         /* unroll the (j=n)-iteration of the scalar mult loop (A < 2m-1) */
         tmp = ((actBNDDIGIT)(*A_j)) + actBNHiWord(tmp);
         (*(A_j-1)) = actBNLoWord(tmp);
         (*A_j) = actBNHiWord(tmp) + hi_carry;
         /* A_j points to A[n] now */
      }

      actL2trigger( watchdog );
   }

   /* Step 3: if A >= m then A -= m */
   if ( (*A_j)==1 ) /* A[n]==1 */
   {
      /* A has one Bit more than m, so A>m, do A-=m */
      actBNSub( A, m, A, n ); (*A_j) = 0;
   }
   else
   {
      if ( actBNCompare( A, m, n ) >= 0 )
      {
         /* A>=m, do A-=m */
         actBNSub( A, m, A, n );
      }
   }

   actL1trigger( watchdog );
}

/***************************************************************************/
#else
   #error actBN_MONT_MUL_VERSION is out of range
#endif
/***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMontMulCopy(actBNDIGIT* x,
 **                        const actBNDIGIT* y,
 **                        actBNDIGIT* A,
 **                        const actBNRING* Ring)
 **
 **  Montgomery Multiplication ( see actBNMontMul, the result is copied
 **  to x ).
 **
 ** input:                                              (length in digits)
 ** - see actBNMontMul
 **
 ** output:
 ** - A:      the product (x*y*R^-1) mod m                  m_length
 ** - x:      the product (x*y*R^-1) mod m                  m_length
 **
 ** assumes:
 ** - see actBNMontMul
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMontMulCopy(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) A, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actBNLENGTH i = Ring->m_length;

   actBNMontMul( x, y, A, Ring, watchdog );
   for( ; i>0; --i)
   {
      *x++ = *A++;
   }
}
