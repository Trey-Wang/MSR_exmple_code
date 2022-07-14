/****************************************************************************
 ** Main author: Ubr                     Creation date: 03/21/06
 ** $Author:: mns                      $ $JustDate:: 03/21/06             $
 ** $Workfile:: actIRSA.h              $ $Revision:: 1435                 $
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
 **     Layer: User Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: The interface for RSA primitives.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actRSAInitPublicKeyOperation
 **   actRSAPublicKeyOperation
 **   actRSAInitPrivateKeyOperation
 **   actRSAPrivateKeyOperation
 **   actRSAInitPrivateKeyOperationCRT
 **   actRSAPrivateKeyOperationCRT
 **
 ***************************************************************************/


#ifndef ACTIRSA_H
#define ACTIRSA_H


#include "actITypes.h"


/****************************************************************************
 ** Return and Errorcodes
 ***************************************************************************/
/* an input parameter x does not satisfy 0 <= x < n */
#define actRSA_PARAM_OUT_OF_RANGE   0x90u



/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/* struct for CRT key parameters */
typedef struct
{
   actPKey p; int p_bytes;
   actPKey q; int q_bytes;
   actPKey dp; int dp_bytes;
   actPKey dq; int dq_bytes;
   actPKey q_inv; int q_inv_bytes;
   int n_bytes;
} actRSACRTSTRUCT;



/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/


#ifdef  __cplusplus
extern "C" {
#endif

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
   actPKey modulus, int modulus_len, actPKey public_exponent, int public_exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
 ** - *cipher_len:   length of cipher buffer used
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
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
CRYPTOCV_FUNC( int ) actRSAPublicKeyGetBitLength( CRYPTOCV_P2CONST_PARA( actU8 ) wksp );

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
   actPKey modulus, int modulus_len, actPKey private_exponent, int private_exponent_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
 ** - *message_len:  length of message buffer used
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
CRYPTOCV_FUNC( actRETURNCODE ) actRSAPrivateKeyOperation(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
CRYPTOCV_FUNC( int ) actRSAPrivateKeyGetBitLength( CRYPTOCV_P2CONST_PARA( actU8 ) wksp );

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
 **               actEXCEPTION_PRIVKEY    dp==0 || dp>=p || dq==0 || dq>=q
 **                               or q_inverse_mod_p==0 || q_inverse_mod_p>=p
 **               actOK                   else
 **
 ** assumes:
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
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

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
 ** - *message_len:  length of message buffer used
 ** - returns:    actEXCEPTION_NULL          if an input parameter is NULL
 **               actEXCEPTION_MEMORY        *message_len < n_bytes
 **               actRSA_PARAM_OUT_OF_RANGE  cipher >= n
 **               actOK                      else
 **
 ** assumes:
 ** - wksp is initialized by actRSAInitPrivateKeyOperationCRT() function
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actRSAPrivateKeyOperationCRT(
   CRYPTOCV_P2CONST_PARA( actU8 ) cipher, int cipher_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) message, CRYPTOCV_P2VAR_PARA( int ) message_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) wksp, int wksp_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* ACTIRSA_H */
