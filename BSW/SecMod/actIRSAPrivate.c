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
 **   actRSAInitPrivateKeyOperation
 **   actRSAPrivateKeyOperation
 **
 ***************************************************************************/


#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"

/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAInitPrivateKeyOperation
 **
 **  This function initializes the RSA workspace.
 **
 ** input:
 ** - modulus:                pointer to the public modulus
 ** - modulus_len:            length of modulus
 ** - private_exponent:       pointer to the private exponent d
 ** - private_exponent_len:   length of private_exponent
 ** - wksp:                   workspace
 ** - wksp_len:               length of workspace in bytes
 **
 ** output:
 ** - wksp:       initialized workspace
 ** - returns:    actEXCEPTION_NULL       if an input parameter is NULL
 **               actEXCEPTION_MEMORY     wksp_len to small
 **               actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 **               actEXCEPTION_PRIVKEY    d==0 || d>=n
 **               actOK                   else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAInitPrivateKeyOperation(
   actPKey modulus, int modulus_len,
   actPKey private_exponent, int private_exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actRSAInitExponentiation(modulus, modulus_len, private_exponent, private_exponent_len, wksp, wksp_len, actRSA_PRIVATE_KEY_OPERATION, watchdog);
}


/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAPrivateKeyOperation
 **
 **  This function calculates m = c^d mod n.
 **
 ** input:
 ** - cipher:        pointer to the cipher text cipher c
 ** - cipher_len:    length of cipher
 ** - message:       buffer to store the decrypted cipher text message m
 ** - *message_len:  length of message buffer
 ** - wksp:          workspace
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
 ** - wksp is initialized by actRSAInitPrivateKeyOperation() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE )  actRSAPrivateKeyOperation(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actRSAExponentiation( cipher, cipher_len, message, message_len, wksp, watchdog );
}

/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAPrivateKeyGetBitLength
 **
 **  This function returns the exact length in bits of the modulus.
 **
 ** input:
 ** - wksp:          workspace
 **
 ** output:
 ** - returns:    Exact length in bits of the modulus
 **
 ** assumes:
 ** - wksp is initialized by actRSAInitPrivateKeyOperation() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actRSAPrivateKeyGetBitLength( CRYPTOCV_P2CONST_PARA( actU8 ) wksp )
{
   CRYPTOCV_P2CONST_PARA( actBNRING )n_ring =
      (CRYPTOCV_P2CONST_PARA( actBNRING ))wksp;

   return actBNGetBitLength(n_ring->m, n_ring->m_length);
}
