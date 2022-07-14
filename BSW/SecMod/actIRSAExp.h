/****************************************************************************
 ** Main author: Ubr                     Creation date: 10/17/03
 ** $Author:: mns                      $ $JustDate:: 10/17/03             $
 ** $Workfile:: actECDH.h              $ $Revision:: 1435                 $
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
 **     Layer: Core Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: An RSA exponentiation interface.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actECDHp
 **
 ***************************************************************************/


#ifndef ACTIRSAEXP_H
#define ACTIRSAEXP_H


#include "actITypes.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

#define actRSA_PRIVATE_KEY_OPERATION      0
#define actRSA_PRIVATE_KEY_OPERATION_CRT  1
#define actRSA_PUBLIC_KEY_OPERATION       2


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/


#ifdef  __cplusplus
extern "C" {
#endif

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
extern CRYPTOCV_FUNC( actRETURNCODE ) actRSAInitExponentiation(
   actPKey modulus, int modulus_len, actPKey exponent, int exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   int key_flag, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
extern CRYPTOCV_FUNC( actRETURNCODE ) actRSAExponentiation(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* ACTIRSAEXP_H */
