/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/04/04
 ** $Author:: alf                      $ $JustDate:: 01/05/05             $
 ** $Workfile:: actAES.h               $ $Revision:: 1414                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006                          )
 **
 ** \version(1.0 (beta)                                                   )
 ***************************************************************************/

#include "actIAES.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/

typedef CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, actAESTransformFctType )(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ), CRYPTOCV_P2CONST_PARA( actU8 ),
   CRYPTOCV_P2VAR_PARA( actU8 ), int,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, CRYPTOCV_NONE )( void ) );

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: Core Module
 **
 ***************************************************************************/

/* block I/O */
extern CRYPTOCV_FUNC( void ) BlockIO( CRYPTOCV_P2CONST_PARA( void ) in, CRYPTOCV_P2VAR_PARA( void ) out );

/* encryption round function */
extern CRYPTOCV_FUNC( void ) f_nround(
   CRYPTOCV_P2VAR_PARA( actU32 ) bo, CRYPTOCV_P2CONST_PARA( actU32 ) bi, CRYPTOCV_P2CONST_PARA( actU32 ) k );

/* decryption round function */
extern CRYPTOCV_FUNC( void ) i_nround(
   CRYPTOCV_P2VAR_PARA( actU32 ) bo, CRYPTOCV_P2CONST_PARA( actU32 ) bi, CRYPTOCV_P2CONST_PARA( actU32 ) k );

/* last encryption round function */
extern CRYPTOCV_FUNC( void ) f_lround(
   CRYPTOCV_P2VAR_PARA( actU32 ) bo, CRYPTOCV_P2CONST_PARA( actU32 ) bi, CRYPTOCV_P2CONST_PARA( actU32 ) k );

/* last decryption round function */
extern CRYPTOCV_FUNC( void ) i_lround(
   CRYPTOCV_P2VAR_PARA( actU32 ) bo, CRYPTOCV_P2CONST_PARA( actU32 ) bi, CRYPTOCV_P2CONST_PARA( actU32 ) k );

/****************************************************************************
 **
 ** FUNCTION:
 ** static actRETURNCODE actAESKeySetup()
 **
 **  This function intializes info->e_key
 **
 ** input:
 ** - info:       initialized AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 **
 ** output:
 ** - info:       modified AES context structure
 **
 ** assumes:
 ** - actAESInit(info) is called once before calling this function
 ** - returns:    actEXCEPTION_NULL      if key parameter is NULL
 **               actEXCEPTION_LENGTH    if key_len is not 16
 **               actOK                  else
 **
 ** uses:
 **
 ***************************************************************************/
extern CRYPTOCV_FUNC( actRETURNCODE ) actAESKeySetup(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info, CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actAESMakeDecryptionKey()
 **
 **  This function transforms the intialized encryption key info->e_key
 **  into a decryption key.
 **
 ** input:
 ** - info:       initialized AES context structure
 **
 ** output:
 ** - info:       modified AES context structure
 **
 ** assumes:
 ** - actAESKeySetup(info) is called once before calling this function
 **
 ** uses:
 **
 ***************************************************************************/
extern CRYPTOCV_FUNC( void ) actAESMakeDecryptionKey( CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info );


/****************************************************************************
 **
 ** FUNCTION:
 ** static actRETURNCODE actAESInit()
 **
 **  This function initializes the AES algorithm (key setup etc.).
 **
 ** input:
 ** - info:       pointer to AES context structure
 ** - key:        pointer to user key
 ** - key_len:    length of key in byte (16, 24 or 32)
 ** - iv:         pointer to initialization vector (only in CBC mode;
 **               of length actAES_BLOCK_SIZE; if iv==NULL, a zeroized
 **               block is used as iv)
 ** - mode:       ECB, CBC mode switch
 ** - key_setup_only:  if not 0: only key setup is done
 **
 ** output:
 ** - info:       initialized AES context structure
 ** - mode:       initialized mode switch of context structure
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
extern CRYPTOCV_FUNC( actRETURNCODE ) actAESInit(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) key, int key_len, CRYPTOCV_P2CONST_PARA( actU8 ) iv,
   actU8 mode, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actAESEncryptMultipleBlocks()
 **
 **  This function encrypts the given blocks in different modes.
 **
 ** input:
 ** - info:        pointer to AES context structure
 ** - in:          pointer to blocks to be encrypted
 ** - out_buffer:  pointer to allocated output blocks
 ** - blocks:      number of blocks
 **
 ** output:
 ** - out_block:  encrypted output blocks
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
extern CRYPTOCV_FUNC( void ) actAESEncryptMultipleBlocks(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer,
   int blocks, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actAESDecryptMultipleBlocks()
 **
 **  This function decrypts the given blocks in different modes.
 **
 ** input:
 ** - info:        pointer to AES context structure
 ** - in:          pointer to blocks to be decrypted
 ** - out_buffer:  pointer to allocated output blocks
 ** - blocks:      number of blocks
 **
 ** output:
 ** - out_block:  decrypted output blocks
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
extern CRYPTOCV_FUNC( void ) actAESDecryptMultipleBlocks(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer,
   int blocks, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actAESTransform()
 **
 **  This function transforms input data of any length.
 **  ((in_length+buffered_bytes)/16) blocks will be transformed and written
 **  to out_buffer, ((in_length+buffered_bytes)%16) bytes will be buffered,
 **  where 0 <= buffered_bytes < 16.
 **
 ** input:
 ** - info:        pointer to initialized AES context structure
 ** - in:          pointer to data to be transformed
 ** - in_length:   length of data in bytes
 ** - out_buffer:  pointer to encrypted data buffer
 ** - transform:   pointer to transformation function (Encrypt/Decrypt)
 **
 ** output:
 ** - info->prev_block:   updated previous block
 ** - out_buffer:         encrypted data blocks
 **
 ** assumes:
 ** - in!=NULL, out_buffer!=NULL are a valid pointers
 ** - in_length>=0
 **
 ** uses:
 **
 ***************************************************************************/
extern CRYPTOCV_FUNC( void ) actAESTransform(
   CRYPTOCV_P2VAR_PARA( actAESSTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) in, int in_length,
   CRYPTOCV_P2VAR_PARA( actU8 ) out_buffer, actAESTransformFctType transform,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

