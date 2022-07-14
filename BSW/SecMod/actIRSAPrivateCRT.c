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
 **   actRSAInitPrivateKeyOperationCRT
 **   actRSAPrivateKeyOperationCRT

 **
 ***************************************************************************/


#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actWatchdog.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAInitPrivateKeyOperationCRT
 **
 **  This function initializes the RSA workspace.
 **
 ** input:
 ** - prime_p:                   pointer to the prime p
 ** - prime_p_len:               length of prime_p
 ** - prime_q:                   pointer to the prime q
 ** - prime_q_len:               length of prime_q
 ** - private_exponent_dp:       pointer to the private exponent dp
 ** - private_exponent_dp_len:   length of private_exponent_dp
 ** - private_exponent_dq:       pointer to the private exponent dq
 ** - private_exponent_dq_len:   length of private_exponent_dq
 ** - q_inverse_mod_p:           pointer to the q inverse mod p
 ** - q_inverse_mod_p_len:       length of q_inverse_mod_p
 ** - wksp:                      workspace
 ** - wksp_len:                  length of workspace in bytes
 **
 ** output:
 ** - wksp:       initialized workspace
 ** - returns:    actEXCEPTION_NULL       if an input parameter is NULL
 **               actEXCEPTION_MEMORY     wksp_len to small
 **               actRSA_PARAM_OUT_OF_RANGE   p or q has leading zero digits
 **               actEXCEPTION_PRIVKEY    dp==0 || dp>=p-1 || dq==0 || dq>=q-1
 **                               or q_inverse_mod_p==0 || q_inverse_mod_p>=p
 **               actOK                   else
 **
 ** assumes:
 ** - the CRTKey (p, q, dp, dq, qInv) must stay in memory after the call
 **   of this function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAInitPrivateKeyOperationCRT(
   actPKey prime_p, int prime_p_len, actPKey prime_q, int prime_q_len,
   actPKey private_exponent_dp, int private_exponent_dp_len,
   actPKey private_exponent_dq, int private_exponent_dq_len,
   actPKey q_inverse_mod_p, int q_inverse_mod_p_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   /*=========================================================================
      Used by   | Workspace             |   Length in digits (offset in bytes)
      ========================================================================
      -----------------------------------
      CRTInit   | offset: CRT struct    |   sizeof(actRSACRTSTRUCT)
      -----------------------------------
      RSAModExp | offset: ring struct   |   sizeof(actBNRING)
                -------------------------
                | t1                    |   max_length (p, q)
                -------------------------
                | t2                    |   max_length (R^2)
                -------------------------
                | t3                    |   max_length (dp, dq)
                -------------------------
                | t4                    |   max_length (cp, cq)
                -------------------------
                | t5                    |   max_length+1 (exp result)
                -------------------------
                | t5+1                  |   max_length+1 (exp tmp)
                ...                         ...
                | t5+actRSA_MOD_EXP_TMP |   max_length+1 (exp tmp)
     ------------------------------------
     CRT        | t6+actRSA_MOD_EXP_TMP |   max_length (CRT tmp)
     ------------------------------------
     Total size:
     ===========
     offset + 5*max_length + (1+actRSA_MOD_EXP_TMP)*(max_length+1)
   =========================================================================*/

   CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ) crt_param =
      (CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ))wksp;
   int p_length = actBNGetDigitLength(prime_p_len), q_length = actBNGetDigitLength(prime_q_len);
   int max_length = actMax(p_length, q_length);
   int wksp_offset = sizeof(actRSACRTSTRUCT) + sizeof(actBNRING);
   int wksp_digits;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) wksp_ptr =
      (CRYPTOCV_P2VAR_PARA( actBNDIGIT ))(wksp+wksp_offset); /* BigNum workpace starts here */
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) p = wksp_ptr;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) q = p + p_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) dp = q + q_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) dq = dp + p_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) qInv = dq + q_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t6 = wksp_ptr + 4*max_length + (max_length+1);
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) one;

   actL1trigger( watchdog );

   /* any null pointers ? */
   if ( (!prime_p) || (!prime_q) || (!private_exponent_dp) || (!private_exponent_dq) || (!q_inverse_mod_p) )
   {
      return actEXCEPTION_NULL;
   }
   /* length checks  */
   if ( private_exponent_dp_len>prime_p_len ) { return actEXCEPTION_PRIVKEY;}
   if ( private_exponent_dq_len>prime_q_len ) { return actEXCEPTION_PRIVKEY;}
   if ( q_inverse_mod_p_len>prime_p_len ) { return actEXCEPTION_PRIVKEY;}

   /* check workspace length for both exponentiations (refer to actRSAInitExponentiation) */
   wksp_digits = 4*max_length + (1+actRSA_MOD_EXP_TMP)*(max_length+1);
   wksp_digits += max_length; /* one further bignum variable for CRT stuff */
   if ( ( wksp_len - wksp_offset ) < ( wksp_digits * actBN_BYTES_PER_DIGIT ) )
   {
      return actEXCEPTION_MEMORY;
   }

   /* BigNum initialization of p, q, dp, dq, qInv */
   actBNSetOctetStringROM( p, p_length, prime_p, prime_p_len );
   actBNSetOctetStringROM( q, q_length, prime_q, prime_q_len );
   actBNSetOctetStringROM( dp, p_length, private_exponent_dp, private_exponent_dp_len );
   actBNSetOctetStringROM( dq, q_length, private_exponent_dq, private_exponent_dq_len );
   actBNSetOctetStringROM( qInv, p_length, q_inverse_mod_p, q_inverse_mod_p_len );

   actL1trigger( watchdog );

   /* calculate n = p*q */
   /* n is saved in t6 for size checks in actRSAPrivateKeyOperationCRT() function */
   actBNMult( p, p_length, q, q_length, t6, CRYPTOCV_FUNC_NULL_PTR );
   /* calculate exact byte length of n */
   crt_param->n_bytes = (actBNGetBitLength(t6, p_length+q_length)+7)/8;

   /* check 0 < qInv < p  before continue */
   if ( actBNIsZero( qInv, p_length ) )
   {
      return actEXCEPTION_PRIVKEY;
   }
   if ( actBNCompare( qInv, p, p_length ) >= 0 )
   {
      return actEXCEPTION_PRIVKEY;
   }

   one = qInv;/* use qInv space as 1-Element of the field */
   actBNSetZero( one, actMax(p_length, q_length) ); one[0] = (actBNDIGIT)1;
   /* check 0 < dp < (p-1)  before continue */
   actBNSub( p, one, p, p_length ); /* p = p-1 */
   if ( actBNIsZero(dp, p_length) ) { return actEXCEPTION_PRIVKEY; }
   if ( actBNCompare(dp, p, p_length)>=0 ) { return actEXCEPTION_PRIVKEY; }
    /* check 0 < dq < (q-1)  before continue */
   actBNSub( q, one, q, q_length ); /* q = q-1 */
   if ( actBNIsZero(dq, q_length) ) { return actEXCEPTION_PRIVKEY; }
   if ( actBNCompare(dq, q, q_length)>=0 ) { return actEXCEPTION_PRIVKEY; }

   /* store parameters in structure (at &wksp[0]) */
   crt_param->p = prime_p; crt_param->p_bytes = prime_p_len;
   crt_param->q = prime_q; crt_param->q_bytes = prime_q_len;
   crt_param->dp = private_exponent_dp; crt_param->dp_bytes = private_exponent_dp_len;
   crt_param->dq = private_exponent_dq; crt_param->dq_bytes = private_exponent_dq_len;
   crt_param->q_inv = q_inverse_mod_p; crt_param->q_inv_bytes = q_inverse_mod_p_len;

   actL1trigger( watchdog );

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAPrivateKeyOperationCRT
 **
 **  This function calculates m from c using CRT.
 **
 ** input:
 ** - cipher:        pointer to the cipher text cipher c
 ** - cipher_len:    length of cipher
 ** - message:       buffer to store the decrypted cipher text message m
 ** - *message_len:  length of message buffer
 ** - wksp:          workspace
 ** - wksp_len:      length of workspace in bytes
 **
 ** output:
 ** - message:       the decrypted cipher text message
 ** - *message_len:  length of message buffer used (n_bytes)
 ** - returns:    actEXCEPTION_NULL          if an input parameter is NULL
 **               actEXCEPTION_MEMORY        *message_len < n_bytes
 **               actRSA_PARAM_OUT_OF_RANGE  cipher >= n
 **               actOK                      else
 **
 ** assumes:
 ** - wksp is initialized by actRSAInitPrivateKeyOperationCRT(CRTKey) function,
 **   the CRTKey must stay in memory after the call of the init function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAPrivateKeyOperationCRT(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   /* NOTE: see actRSAInitPrivateKeyOperationCRT() for workspace layout */

   actRETURNCODE rc;
   int crt_offset = sizeof(actRSACRTSTRUCT), total_offset = crt_offset+sizeof(actBNRING);
   CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ) crt_param =
      (CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ))wksp;
   CRYPTOCV_P2VAR_PARA( actBNRING ) ring =
      (CRYPTOCV_P2VAR_PARA( actBNRING ))(wksp+crt_offset); /* q_ring or p_ring */
   int p_bytes = crt_param->p_bytes, q_bytes = crt_param->q_bytes, n_bytes = crt_param->n_bytes;
   int p_length = actBNGetDigitLength(p_bytes), q_length = actBNGetDigitLength(q_bytes), n_length = actBNGetDigitLength(n_bytes);
   int max_length = actMax(p_length, q_length);
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t1 = (CRYPTOCV_P2VAR_PARA( actBNDIGIT )) (wksp+total_offset); /* BigNum workpace starts here */
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t4 = t1 + 3*max_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t5 = t4 + max_length;
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t6 = t5 + (max_length+1);
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t7 = t6 + (actRSA_MOD_EXP_TMP)*(max_length+1);
   CRYPTOCV_P2VAR_PARA( actU8 ) t5_byte_ptr = wksp+total_offset+(4*max_length*actBN_BYTES_PER_DIGIT); /* avoid cast of t5 */

   actL1trigger( watchdog );

   /* any null pointers ? */
   if ( (!cipher) || (!message) ) { return actEXCEPTION_NULL; }
   /* check cipher length */
   if ( cipher_len > n_bytes ) { return actRSA_PARAM_OUT_OF_RANGE; }
   /* check output buffer length */
   if ( *message_len < n_bytes ) { return actEXCEPTION_MEMORY; }

   /*
      CRT STEP 1: calculate m2 = cq^dq mod q
   */

   /* init q_ring exponentiation */
   /* t1 - t3 will be initialized here (q, R^2, dq: each of q_length) */
   rc = actRSAInitExponentiation( crt_param->q, q_bytes, crt_param->dq, crt_param->dq_bytes, wksp+crt_offset,
                                  wksp_len-crt_offset, actRSA_PRIVATE_KEY_OPERATION_CRT, watchdog );
   if ( rc != actOK ) { return rc; }

   /* check 0 < cipher c < n and calculate t6 = cq = c mod q */
   actBNSetOctetString( t4, n_length, cipher, cipher_len ); /* t4 = c */
   if ( actBNIsZero( t4, n_length ) ) { return actRSA_PARAM_OUT_OF_RANGE; }
   if ( actBNCompare( t4, t6, n_length ) >= 0 ) { return actRSA_PARAM_OUT_OF_RANGE; } /* n is saved in t6 by actRSAInitPrivateKeyOperationCRT() function */
   actBNReduce( t4, n_length, ring->m, q_length, t6, watchdog ); /* returns ok, because q_length>=2 is checked in actRSAInitExponentiation */

   /* calculate m2 = cq^dq mod q (CRT STEP 1) */
   actBNOctetString( t5_byte_ptr, q_bytes, t6, q_length ); /* t5 = cq_octet_string for exponentiation */
   actRSAExponentiation( t5_byte_ptr, q_bytes,
                         (CRYPTOCV_P2VAR_PARA( actU8 )) 0,
                         (CRYPTOCV_P2VAR_PARA( int )) 0,
                         wksp+crt_offset, watchdog ); /* no output buffer is passed */
   actBNCopy( t7, t1+4*q_length, q_length ); /* copy m2 from t5_q_steps to t7 (max_steps) */

   /*
      CRT STEP 2: calculate m1 = cp^dp mod p
   */

   /* init p_ring exponentiation */
   /* t1 - t3 will be initialized here (p, R^2, dp: each of p_length) */
   rc = actRSAInitExponentiation( crt_param->p, p_bytes, crt_param->dp, crt_param->dp_bytes, wksp+crt_offset,
                                  wksp_len-crt_offset, actRSA_PRIVATE_KEY_OPERATION_CRT, watchdog );
   if ( rc != actOK ) { return rc; }

   /* calculate t6 = cp = c mod p */
   actBNSetOctetString( t4, n_length, cipher, cipher_len ); /* t4 = c */
   actBNReduce( t4, n_length, ring->m, p_length, t6, watchdog ); /* returns ok, because p_length>=2 is checked in actRSAInitExponentiation */

   /* calculate m1 = cp^dp mod p (CRT STEP 2) */
   actBNOctetString( t5_byte_ptr, p_bytes, t6, p_length ); /* t5 = cp_octet_string for exponentiation */
   actRSAExponentiation( t5_byte_ptr, p_bytes,
                         (CRYPTOCV_P2VAR_PARA( actU8 )) 0,
                         (CRYPTOCV_P2VAR_PARA( int )) 0,
                         wksp+crt_offset, watchdog ); /* no output buffer is passed */

   /*
      CRT STEP 3: calculate h = (m1-m2)*qInv mod p
   */

   actL1trigger( watchdog );

   /* m1 is in t5_p_steps, m2 is in t7 (max_steps), */
   /* save unreduced m2 to t4 and reduce m2 mod p first (t5 = m2 mod p) */
   actBNCopy( t6, t1+4*p_length, p_length ); /* move m1 to t6, because t4 may overlap with t5_p_steps */
   actBNCopy( t4, t7, q_length ); /* t4 = t7 = m2 */
   actBNReduce( t7, q_length, ring->m, p_length, t5, watchdog ); /* returns ok, because p_length>=2 is checked in actRSAInitExponentiation */

   /* calculate m1-m2 mod p */
   actBNModSub(t6, t5, t7, ring, CRYPTOCV_FUNC_NULL_PTR); /* t7 = t6 - t5 = m1 - m2 mod p */

   /* calculate h = (m1-m2)*qInv mod p */
   actBNSetOctetStringROM( t5, p_length, crt_param->q_inv, crt_param->q_inv_bytes ); /* t5 = qInv */
   actBNMontMul( t7, t5, t6, ring, CRYPTOCV_FUNC_NULL_PTR ); /* t6 = MontMul(t7, t5) = (m1-m2)*qInv*R^(-1) mod p */
   actBNMontMul( t6, ring->RR, t5, ring, CRYPTOCV_FUNC_NULL_PTR ); /* h = t5 = MontMul(t6, R^2) = (m1-m2)*qInv mod p */

   /*
      CRT STEP 4: calculate m = m2 + q*h
   */

   /* t6 = q * h */
   actBNSetOctetStringROM( t1, q_length, crt_param->q, crt_param->q_bytes ); /* t1 = q */
   actBNMult( t1, q_length, t5, p_length, t6, CRYPTOCV_FUNC_NULL_PTR ); /* t6 = q * h */

   /* t6 += m2 */
   actBNSetZero( t4+q_length, n_length-q_length ); /* pad t4 = m2 with leading zeros to n_length */
   actBNAdd( t6, t4, t6, n_length ); /* t6 += t4 */

   /* write t6 to message buffer */
   actBNOctetString( message, n_bytes, t6, n_length );
   *message_len = n_bytes;

   actL1trigger( watchdog );

   return actOK;
}

