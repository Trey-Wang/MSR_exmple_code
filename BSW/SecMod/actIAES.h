/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/04/04
 ** $Author:: mns                      $ $JustDate:: 02/09/04             $
 ** $Workfile:: actIAES.h              $ $Revision:: 1068                 $
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
 ** This file contains: The interface for AES blockcipher algorithm.
 **
 ** constants:
 **   actAES_BLOCK_SIZE
 **   actAES_[mode_id]
 **
 ** types:
 **   actAESSTRUCT
 **
 ** macros:
 **
 ** functions:
 **   actAESInitEncrypt
 **   actAESInitDecrypt
 **   actAESEncrypt
 **   actAESDecrypt
 **
 ***************************************************************************/


#ifndef ACTIAES_H
#define ACTIAES_H


#include "actITypes.h"
#include "actPadding.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actAES_BLOCK_SIZE   16u

/* Context structure of the AES algorithm */
typedef struct {
   actU32 e_key[64];
   int key_dword_len;
   int mode;
   actU8 prev_block[actAES_BLOCK_SIZE];
   int buffer_used;
   actU8 buffer[actAES_BLOCK_SIZE];
} actAESSTRUCT;


/****************************************************************************
 ** AES mode identifiers
 **
 **    actAES_[mode_id]
 ***************************************************************************/
/* Block mode */
/* ECB */
#define actAES_BM_ECB            0x00u
/* CBC */
#define actAES_BM_CBC            0x01u

#define actAES_BM_MASK           0x0Fu

/* Check block mode mask against padding mode mask (== 0) */
#if ((actAES_BM_MASK & actPADDING_PM_MASK) != 0x00u)
# error  Bits of actAES_BM_MASK and actPADDING_PM_MASK overlap
#endif

/* Compatibility modes */
/* ECB with PKCS #7 */
#define actAES_ECB               (actAES_BM_ECB | actPADDING_PM_PKCS7)
/* CBC with PKCS #7 */
#define actAES_CBC               (actAES_BM_CBC | actPADDING_PM_PKCS7)

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitEncrypt()
 **
 **  This function initializes the AES encryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 ** - iv:         pointer to initialization vector (only in CBC mode;
 **               of length actAES_BLOCK_SIZE; if iv==NULL, a zeroized
 **               block is used as iv)
 ** - mode:       ECB, CBC mode switch
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actEXCEPTION_ALGOID    mode is invalid
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitEncrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2CONST_PARA( actU8 ) iv, actU8 mode,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESEncrypt()
 **
 **  This function encrypts input data of any length.
 **  If final==0, ((in_length+buffered_bytes)/16) blocks will be encrypted
 **  and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will
 **  be buffered, where 0 <= buffered_bytes < 16.
 **  If final!=0, (((in_length+buffered_bytes)/16)+1) blocks will be
 **  encrypted and written to out_buffer. PKCS 5 padding is used for the
 **  last block padding.
 **
 ** input:
 ** - info:        pointer to initialized AES context structure
 ** - in:          pointer to data to be encrypted
 ** - in_length:   length of data in bytes
 ** - out_buffer:  pointer to encrypted data buffer
 ** - *out_length: out_buffer size in bytes, a save length is:
 **                  (((in_length+15)/16)+1)*16       (see output)
 ** - final:       flag for padding
 **
 ** output:
 ** - info->prev_block:   updated previous block
 ** - out_buffer:         encrypted data blocks
 ** - *out_length:        number of encrypted bytes:
 **                  ((in_length+buffered_bytes)/16)*16       if final==0
 **                  (((in_length+buffered_bytes)/16)+1)*16   else
 ** - returns:    actEXCEPTION_LENGTH   *out_length is too small
 **               actOK                 else
 **
 ** assumes:
 ** - actAESInitEncrypt() is called once before calling this function
 ** - in!=NULL, out_buffer!=NULL are a valid pointers
 ** - in_length>=0, *out_length>=0 (at input)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESEncrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, int in_length,
   CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer, CRYPTOCV_P2VAR_PARA( int ) out_length,
   int final, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitDecrypt()
 **
 **  This function initializes the AES decryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 ** - iv:         pointer to initialization vector (only in CBC mode;
 **               of length actAES_BLOCK_SIZE; if iv==NULL, a zeroized
 **               block is used as iv)
 ** - mode:       ECB, CBC mode switch
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actEXCEPTION_ALGOID    mode is invalid
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitDecrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len, CRYPTOCV_P2CONST_PARA( actU8 ) iv,
   actU8 mode, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESDecrypt()
 **
 **  This function decrypts input data of any length.
 **  If final==0, ((in_length+buffered_bytes)/16) blocks will be decrypted
 **  and written to out_buffer, ((in_length+buffered_bytes)%16) bytes will
 **  be buffered, where 0 <= buffered_bytes < 16.
 **  If final!=0, ((in_length+buffered_bytes)/16) blocks will be decrypted
 **  and written to out_buffer, where (in_length+buffered_bytes) is a
 **  multiple of 16. PKCS 5 padding is used to unpad the last block.
 **
 ** input:
 ** - info:        pointer to initialized AES context structure
 ** - in:          pointer to data to be decrypted
 ** - in_length:   length of data in bytes ( multiple of 16 )
 ** - out_buffer:  pointer to decrypted data buffer of length in_length
 ** - *out_length: out_buffer size in bytes, a save length is:
 **                  ((in_length+15)/16)*16       (see output)
 ** - final:       flag for unpadding
 **
 ** output:
 ** - info->prev_block:   updated previous block
 ** - out_buffer:         decrypted data blocks
 ** - *out_length:        number of decrypted bytes:
 **    ((in_length+buffered_bytes)/16)*16          if final==0
 **    (in_length+buffered_bytes)/16)*16 - pad     else, where 1<=pad<=16
 ** - returns:    actEXCEPTION_LENGTH   *out_length is too small
 ** (if final!=0) actEXCEPTION_INPUT_LENGTH (in_length+buffered_bytes)%16!=0
 **               actEXCEPTION_PADDING  PKCS 5 unpad error
 **               actOK                 else
 **
 ** assumes:
 ** - actAESInitDecrypt() is called once before calling this function
 ** - in!=NULL, out_buffer!=NULL are a valid pointers
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESDecrypt(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, int in_length,
   CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer, CRYPTOCV_P2VAR_PARA( int ) out_length,
   int final, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitEncryptBlock()
 **
 **
 **  This function initializes the AES block encryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitEncryptBlock(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actAESEncryptBlock()
 **
 **  This function encrypts the given block.
 **
 ** input:
 ** - info->e_key: pointer to expanded encryption key
 ** - in_block:    pointer to block to be encrypted
 ** - out_block:   pointer to allocated output block
 **
 ** output:
 ** - out_block:  encrypted output block
 **
 ** assumes:
 ** - actAESInitEncryptBlock(info) is called once before calling
 **   this function
 ** - 'in_block' and 'out_block' are of size actAES_BLOCK_SIZE
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actAESEncryptBlock(
   CRYPTOCV_P2CONST_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in_block, CRYPTOCV_P2VAR_PARA( actU8 ) out_block,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actAESInitDecryptBlock()
 **
 **
 **  This function initializes the AES block decryption.
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actOK                  else
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actAESInitDecryptBlock(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actAESDecryptBlock()
 **
 **  This function decrypts the given block.
 **
 ** input:
 ** - info->e_key: pointer to expanded decryption key
 ** - in_block:    pointer to block to be decrypted
 ** - out_block:   pointer to allocated output block
 **
 ** output:
 ** - out_block:  decrypted output block
 **
 ** assumes:
 ** - actAESInitDecryptBlock(info) is called once before calling
 **   this function
 ** - 'in_block' and 'out_block' are of size actAES_BLOCK_SIZE
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actAESDecryptBlock(
   CRYPTOCV_P2CONST_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in_block, CRYPTOCV_P2VAR_PARA( actU8 ) out_block,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTIAES_H */
