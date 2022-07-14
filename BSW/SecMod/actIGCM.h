/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/04/04
 ** $Author:: alf                      $ $JustDate:: 01/05/05             $
 ** $Workfile:: actAES.h               $ $Revision:: 1794                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006                          )
 **
 ** \version(1.0 (beta)                                                   )
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: User Module - Interface
 **
 ***************************************************************************/


#ifndef ACTGCM_H
#define ACTGCM_H


#include "actITypes.h"
#include "actIAES.h"
#include "actIGHash.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actGCM_BLOCK_SIZE        16u
#define actGCM_BYTE_SIZE         8u
#define actGCM_IV_DEFAULT_SIZE   12u
#define actGCM_TAG_LENGTH        16u

#define STATE_AAD         0u
#define STATE_CIPHERTEXT  1u

#define MODE_ENC         0u
#define MODE_DEC         1u


/* Workspace structure for GCM */
typedef struct {
   actAESSTRUCT aes;                  /* the aes context structure */
   actGHASHSTRUCT ghash;              /* the ghash context structure */
   actU8 J0[actGCM_BLOCK_SIZE];       /* the first counter block depending on the iv for the tag computation */
   actU8 J[actGCM_BLOCK_SIZE];        /* the counter block that is iterated */
   int buffer_used;                   /* number of bytes saved in buffer */
   actU8 buffer[actGCM_BLOCK_SIZE];   /* the input buffer */
   int state;                         /* the current internal state (authdata or ciphertext) */
   actU32 aad_lo_bytecount, aad_hi_bytecount;     /* 64 bit input count */
   actU32 data_lo_bytecount, data_hi_bytecount;   /* 64 bit input count */
} actGCMSTRUCT;


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMInit(actGCMSTRUCT* info,
                         const actU8* K,
                         int K_len,
                         const actU8* IV,
                         int IV_len,
                         void (*watchdog) (void))
 **
 **  This function initializes the Galois/Counter-Mode.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - key:        master key
 ** - key_length: length of key in bytes
 ** - iv:         initialization vector
 ** - iv_length:  length of the initialization vector (max 2^32 bit)
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       initialized context structure
 ** - returns:    actEXCEPTION_LENGTH    if key_len < 1
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMInit (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) K,
   int K_len,
   CRYPTOCV_P2CONST_PARA( actU8 ) IV,
   int IV_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** static actRETURNCODE actGCMTransform( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            int encORdec,
                            void (*watchdog) (void))
 **
 **  This function encrypts/decrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - encORdec:   switch to distinguish between encryption and decryption
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     encrypted/decrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    actEXCEPTION_LENGTH   aad length exceeds 2^64 - 1 bit or
 **                                     plaintext/ciphertext  2^39 - 256 bit
 **               actOK                 else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMTransform (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   int encORdec,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMEncryptUpdate( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            void (*watchdog) (void))
 **
 **  This function encrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - output:     encrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    return value of actGCMTransform
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMEncryptUpdate (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMDecryptUpdate( actGCMSTRUCT *info,
                            actU8 *X, int X_len,
                            actU8 *output, int *bytes_out,
                            void (*watchdog) (void))
 **
 **  This function decrypts any amount of data
 **  and can be called arbitrarily often. It returns, how many
 **  bytes have actually been handled so far.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - X:          input
 ** - X_len:      length of input in bytes
 ** - output:     pointer to output buffer
 ** - bytes_out:  pointer to output counter
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - output:     decrypted data
 ** - bytes_out:  amount of bytes already handled
 ** - returns:    return value of actGCMTransform
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMDecryptUpdate (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) X,
   const int X_len,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMTriggerState(actGCMSTRUCT *info,
 **                                  void (*watchdog) (void))
 **
 **  This function switches the internal state from AAD to P / C, 
 **  so actGCM*Update now expects plaintext/ciphertext.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       updated GHash and internal buffers
 ** - returns:    actEXCEPTION_UNKNOWN  if called from an infalid state
 **                                     (everything other than STATE_AAD)
 **               actOK                 else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMTriggerState (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGMACFinalize(actGCMSTRUCT *info, 
 **                               actU8 *tag, 
 **                               void (*watchdog) (void))
 **
 **  This function finishes the plaintext / ciphertext section and computes the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGMACFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMEncryptFinalize(actGCMSTRUCT *info, 
 **                                 actU8 *output, 
 **                                 int *bytes_out,
 **                                 actU8 *tag, 
 **                                 void (*watchdog) (void))
 **
 **  This function finalizes encryption and returns the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - output:     pointer to output buffer for remaining data bytes
 ** - bytes_out:  pointer to output counter
 ** - tag:        pointer to output buffer for the tag
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     remaining data bytes
 ** - bytes_out:  incrementecd by no. of remaining bytes
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMEncryptFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actGCMDecryptFinalize(actGCMSTRUCT *info, 
 **                                 actU8 *output, 
 **                                 int *bytes_out,
 **                                 actU8 *tag, 
 **                                 void (*watchdog) (void))
 **
 **  This function finalizes decryption and returns the tag.
 **  If there is still data in the buffer, it is written to output.
 **
 ** input:
 ** - info:       pointer to context structure
 ** - output:     pointer to output buffer for remaining data bytes
 ** - bytes_out:  pointer to output counter
 ** - tag:        pointer to output buffer for the tag
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - output:     remaining data bytes
 ** - bytes_out:  incrementecd by no. of remaining bytes
 ** - tag:        the authentication tag
 ** - returns:    actOK always
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actGCMDecryptFinalize (
   CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) output,
   CRYPTOCV_P2VAR_PARA( int ) bytes_out,
   CRYPTOCV_P2VAR_PARA( actU8 ) tag,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTIAES_H */
