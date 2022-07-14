/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/21/06
 ** $Author:: mns                      $ $JustDate:: 03/21/06             $
 ** $Workfile:: actIRSA.c              $ $Revision:: 1435                 $
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
 **     Layer: User Module
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: Implementation file for actIRSA.h
 **
 ** constants:
 **
 ** types:
 **
 ** global variables:
 **
 ** macros:
 **
 ** functions:
 **   actRSAInitExponentiation
 **   actRSAExponentiation
 **
 ***************************************************************************/
#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"



/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 **  actRSAInitExponentiation
 **
 **  This function initializes the RSA workspace.
 **
 ** input:
 ** - modulus:        pointer to the public modulus
 ** - modulus_len:    length of modulus
 ** - exponent:       pointer to the exponent (private or public)
 ** - exponent_len:   length of exponent
 ** - wksp:           workspace
 ** - wksp_len:       length of workspace in bytes
 ** - key_flag:       actRSA_PRIVATE(PUBLIC)_KEY_OPERATION(_CRT)
 **
 ** output:
 ** - wksp:       initialized workspace
 ** - returns:    actEXCEPTION_NULL       if an input parameter is NULL
 **               actEXCEPTION_MEMORY     wksp_len to small
 **               actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 **               actEXCEPTION_PRIVKEY
 **            or actEXCEPTION_PUBKEY     if exponent==0 || exponent>=n
 **               actOK                   else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAInitExponentiation(
   actPKey modulus, int modulus_len, actPKey exponent, int exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len, int key_flag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actRETURNCODE key_out_of_range = ( (key_flag==actRSA_PUBLIC_KEY_OPERATION)? actEXCEPTION_PUBKEY : actEXCEPTION_PRIVKEY );
   CRYPTOCV_P2VAR_PARA( actBNRING ) n_ring =
     (CRYPTOCV_P2VAR_PARA( actBNRING ))wksp;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) wksp_ptr =
     (CRYPTOCV_P2VAR_PARA( actBNDIGIT ))(wksp+sizeof(actBNRING)); /* points to BigNum workpace */
   int n_bytes = modulus_len;
   int n_length = actBNGetDigitLength(n_bytes), wksp_digits;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) e;

   /* any null pointers ? */
   if ( (!modulus) || (!exponent) )
   {
      return actEXCEPTION_NULL;
   }
   /* check exponent length */
   if ( exponent_len > modulus_len )
   {
      return key_out_of_range;
   }

   /* check workspace length for exponentiation */
   /* n, R^2, exponent, c (of n_length) and m, i*tmp (of n_length+1, i=2^(window_size-1)) */
   wksp_digits = 4*n_length + (1+actRSA_MOD_EXP_TMP)*(n_length+1);
   if ( ( wksp_len - (int)sizeof(actBNRING) ) < ( wksp_digits * actBN_BYTES_PER_DIGIT ) )
   {
      return actEXCEPTION_MEMORY;
   }

   /* assign temporary bignum workspace for n, R^2 */
   n_ring->m  = wksp_ptr;       wksp_ptr += n_length;
   n_ring->RR = wksp_ptr;

   actL1trigger( watchdog );

   /* initialize n_ring structure */
   n_ring->m_length = n_length;
   n_ring->m_byte_length = n_bytes;
   actBNSetOctetStringROM( n_ring->m, n_length, modulus, n_bytes );
#if ( actBN_MONT_MUL_VERSION == 1 )
   n_ring->prime_structure = 0;
#endif
#if ( actBN_MOD_EXP_WINDOW_SIZE == 0 )   /* variable k-bit window algorithm */
   n_ring->window_size = actRSA_MOD_EXP_WINDOW_SIZE;
#endif

   actL1trigger( watchdog );

   /* calculate Montgomery constants R^2 mod n and n_bar and store them in n_ring structure */
   {
      CRYPTOCV_P2VAR_PARA( actBNDIGIT ) tmp = wksp_ptr + n_length + 1; /* reserve n_length+1 digits for R^2 */
      int tmp_len = 2*n_length + 1;
      /* tmp = 2^(bits_per_digit*n_length) */
      actBNSetZero( tmp, tmp_len );
      tmp[tmp_len-1] = 1;

      actL2trigger( watchdog );

      /* calculate R^2 mod n = tmp mod n */
      if ( actBNReduce( tmp, tmp_len, n_ring->m, n_length, n_ring->RR, watchdog ) == -1 )
      {
         return actRSA_PARAM_OUT_OF_RANGE;
      }

      actL2trigger( watchdog );

      /* calculate n_bar = -(m^-1) mod 2^bits_per_digit */
      actBNOddInvModBase( &(n_ring->m_bar), (n_ring->m)[0] );
   }

   /* assign temporary bignum workspace for exponent e */
   wksp_ptr += n_length; /* used by R^2 */
   e = wksp_ptr;

   actL1trigger( watchdog );

   /* initialize exponent e */
   actBNSetOctetStringROM( e, n_length, exponent, exponent_len );

   actL1trigger( watchdog );

   if ( key_flag != actRSA_PRIVATE_KEY_OPERATION_CRT ) /* checks are done in actRSAInitPrivateKeyOperationCRT */
   {
      if ( key_flag == actRSA_PRIVATE_KEY_OPERATION )
      {
         /* e is the private exponent: check e > 0 before continue */
         if ( actBNIsZero( e, n_length ) )
         {
            return key_out_of_range;
         }
      }
      else
      {
         /* e is the public exponent: check e odd, e >= 3 before continue */
         if ( e[0] <= 1 )
         {
            if ( actBNIsZero( e+1, n_length-1 ) ) /* e<=1 */
            {
               return key_out_of_range;
            }
         }
         else if ( actBNIsEven( e ) ) /* e is even */
         {
            return key_out_of_range;
         }
      }

      actL2trigger( watchdog );

      /* check e < n before continue */
      if ( actBNCompare( e, n_ring->m, n_length ) >= 0 )
      {
         return key_out_of_range;
      }
   }

   actL1trigger( watchdog );

   return actOK;
}


/****************************************************************************
 **
 **  actRSAExponentiation
 **
 **  This function calculates m = c^exponent mod n.
 **
 ** input:
 ** - cipher:        pointer to the cipher text cipher c
 ** - cipher_len:    length of cipher
 ** - message:       buffer to store the decrypted chipher text message m
 ** - *message_len:  length of message buffer
 ** - wksp:          workspace
 **
 ** output:
 ** - message:       the decrypted cipher text message
 ** - *message_len:  length of message buffer used
 ** - returns:    actEXCEPTION_NULL          if an input parameter is NULL
 **               actEXCEPTION_MEMORY        *message_len < n_bytes
 **               actRSA_PARAM_OUT_OF_RANGE  cipher >= modulus
 **               actOK                      else
 **
 ** assumes:
 ** - wksp is initialized by actRSAInitExponentiation() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAExponentiation(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actBNRING ) n_ring =
      (CRYPTOCV_P2VAR_PARA( actBNRING ))wksp;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) wksp_ptr =
      (CRYPTOCV_P2VAR_PARA( actBNDIGIT ))(wksp+sizeof(actBNRING)); /* points to BigNum workpace */
   int n_length = n_ring->m_length;
   int n_bytes = n_ring->m_byte_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) e;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) m;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t[actRSA_MOD_EXP_TMP];
   int i;

   actL1trigger( watchdog );

   /* any null pointers ? */
   if ( !cipher )
   {
      return actEXCEPTION_NULL;
   }
   /* check output buffer length */
   if ( ( message != 0) && ( *message_len < n_bytes ) )
   {
      return actEXCEPTION_MEMORY;
   }
   /* check cipher length */
   if ( cipher_len > n_bytes )
   {
      return actRSA_PARAM_OUT_OF_RANGE;
   }

   /* assign temporary bignum workspace, size has already been checked by init function,
   space for n, R^2 and e has been initialized */
   wksp_ptr += 2*n_length; /* n, R^2 pointers are stored in n_ring */
   e = wksp_ptr;          wksp_ptr += n_length;
   c = wksp_ptr;          wksp_ptr += n_length;
   m = wksp_ptr;          wksp_ptr += (n_length+1);
   for ( i = 0; i < actRSA_MOD_EXP_TMP; ++i )
   {
      t[i] = wksp_ptr;    wksp_ptr += (n_length + 1);
   }

   actL3trigger( watchdog );

   /* initialize cipher c */
   actBNSetOctetString( c, n_length, cipher, cipher_len );

   actL3trigger( watchdog );

   /* check 0 <= c < n before continue */
   if ( actBNCompare( c, n_ring->m, n_length ) >= 0 )
   {
      return actRSA_PARAM_OUT_OF_RANGE;
   }

   actL3trigger( watchdog );

   /* convert c to Montgomery representation cR */
   actBNMontMulCopy( c, n_ring->RR, t[0], n_ring, watchdog );

   actL3trigger( watchdog );

   /* calculate mR = cR^d mod n (basic modular exponentiation) */
   actBNModExp( c, e, m, n_ring, t, watchdog );

   actL3trigger( watchdog );

   /* reconvert mR from Montgomery representation */
   {
      CRYPTOCV_P2VAR_PARA( actBNDIGIT ) one = e; /* use e space as 1-Element of the field, this also wipes the (private) exponent */
      actBNSetZero(one, n_length);
      one[0] = (actBNDIGIT)1;
      actBNMontMulCopy( m, one, t[0], n_ring, watchdog );
   }

   actL3trigger( watchdog );

   /* write m to message buffer */
   if ( message!=0 )
   {
      actBNOctetString( message, n_bytes, m, n_length );
      *message_len = n_bytes;
   }

   actL1trigger( watchdog );

   return actOK;
}

