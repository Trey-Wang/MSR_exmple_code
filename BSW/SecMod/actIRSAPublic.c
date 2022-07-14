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
 **   actRSAInitPublicKeyOperation
 **   actRSAPublicKeyOperation
 **
 ***************************************************************************/


#include "actIRSA.h"
#include "actIRSAExp.h"
#include "actBigNum.h"
#include "actUtilities.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAInitPublicKeyOperation
 **
 **  This function initializes the RSA workspace.
 **
 ** input:
 ** - modulus:                pointer to the public modulus
 ** - modulus_len:            length of modulus
 ** - public_exponent:        pointer to the public exponent e
 ** - public_exponent_len:    length of public_exponent
 ** - wksp:                   workspace
 ** - wksp_len:               length of workspace in bytes
 **
 ** output:
 ** - wksp:       initialized workspace
 ** - returns:    actEXCEPTION_NULL       if an input parameter is NULL
 **               actEXCEPTION_MEMORY     wksp_len to small
 **               actRSA_PARAM_OUT_OF_RANGE   n has leading zero digits
 **               actEXCEPTION_PUBKEY     e==0 || e>=n
 **               actOK                   else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAInitPublicKeyOperation(
   actPKey modulus, int modulus_len,
   actPKey public_exponent, int public_exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actRSAInitExponentiation( modulus, modulus_len, public_exponent, public_exponent_len, wksp, wksp_len, actRSA_PUBLIC_KEY_OPERATION, watchdog );
}


/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAPublicKeyOperation
 **
 **  This function calculates c = m^e mod n.
 **
 ** input:
 ** - message:       pointer to the message m to be encrypted
 ** - message_len:   length of message
 ** - cipher:        buffer to store the cipher text cipher c
 ** - *cipher_len:   length of cipher buffer
 ** - wksp:          workspace
 **
 ** output:
 ** - cipher:        the encrypted message
 ** - *cipher_len:   length of cipher buffer used (n_bytes)
 ** - returns:    actEXCEPTION_NULL          if an input parameter is NULL
 **               actEXCEPTION_MEMORY        *cipher_len < n_bytes
 **               actRSA_PARAM_OUT_OF_RANGE  message >= n
 **               actOK                      else
 **
 ** assumes:
 ** - wksp is initialized by actRSAInitPublicKeyOperation() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAPublicKeyOperation(
   CRYPTOCV_P2CONST_PARA( actU8 ) message, int message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) cipher, CRYPTOCV_P2VAR_PARA( int ) cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   return actRSAExponentiation( message, message_len, cipher, cipher_len, wksp, watchdog );
}

/****************************************************************************
 **
 ** FUNCTION:
 **  actRSAPublicKeyGetBitLength
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
 ** - wksp is initialized by actRSAInitPublicKeyOperation() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actRSAPublicKeyGetBitLength( CRYPTOCV_P2CONST_PARA( actU8 ) wksp )
{
   CRYPTOCV_P2CONST_PARA( actBNRING )n_ring =
      (CRYPTOCV_P2CONST_PARA( actBNRING ))wksp;

   return actBNGetBitLength(n_ring->m, n_ring->m_length);
}
