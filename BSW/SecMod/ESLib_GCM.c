/****************************************************************************
 ** Main author: mga                     Creation date: 04/18/11
 ** $LastChangedBy: mgnida $
 ** $Date: 2011-05-20 14:37:53 +0200 (Fr, 20 Mai 2011) $
 ** $URL: https://scm/svn/library-es/branches/library-es/1.2.n/development/common/lib_es/source/ESLib_CMACAES.c $
 ** $Rev: 2006 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
 **
 ** \version(cvActLibES 1.0)
 ***************************************************************************/

/****************************************************************************
 **
 **     This file is part of the embedded systems library cvActLib/ES.
 **
 **     Layer: core level
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: A GCM implementation
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initEncryptGCM
 **   esl_updateMessageEncryptGCM
 **   esl_updateAuthDataEncryptGCM
 **   esl_finalizeEncryptGCM
 **   esl_initDecryptGCM
 **   esl_updateMessageDecryptGCM
 **   esl_updateAuthDataDecryptGCM
 **   esl_finalizeDecryptGCM
 **
 ***************************************************************************/

#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIGCM.h"
#include "actUtilities.h"


/****************************************************************************
 **
 ** FUNCTION: esl_initEncryptGCM
 **
 **  This function initializes the GCM encryption.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - key:         the symmetric AES master key
 ** - keyLength:   the length of the AES key in bytes, must be 16, 24 or 32
 ** - iv:          the initialization vector
 ** - ivLength:    the length of the initilization vector in bytes
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL            work space too small
 **                ESL_ERC_GCM_INVALID_KEY_LENGTH  no valid AES keylength
 **                ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initEncryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) key,
   const eslt_Length keyLength,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) iv,
   const eslt_Length ivLength )
{
   /* any NULLs ? */
   if ( (!workSpace) || (!key) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* check keyLength */
   if (( keyLength!=ESL_SIZEOF_AES128_KEY ) && ( keyLength!=ESL_SIZEOF_AES192_KEY ) && ( keyLength!=ESL_SIZEOF_AES256_KEY )) {
      return ESL_ERC_GCM_INVALID_KEY_LENGTH;
   }

   /* check ivLength */
   if (1 > ivLength) {
      return ESL_ERC_INVALID_IV_LENGTH;
   }

   /* init actCLib GCM */
   actGCMInit((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, key, (int)keyLength, iv, (int)ivLength, workSpace->header.watchdog);

   /* set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_GCM | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updateAuthDataEncryptGCM
 **
 **  This function hashes additional authenticated data using AES-GCM.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - input:       additional authenticated data
 ** - inputLength: the length of the input
 **
 ** output:
 ** - returns:     ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID           work space state invalid
 **                ESL_ERC_WS_TOO_SMALL               work space too small
 **                ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 **                ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateAuthDataEncryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) input,
   const eslt_Length inputLength )
{
   CRYPTOCV_P2VAR_PARA( actU8 ) output = NULL;
   int    bytes_out;


   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   if (STATE_AAD != ((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM)->state) {
      return ESL_ERC_GCM_STATE_INVALID;
   }
   
   /* update actCLib GCM */
   if (actEXCEPTION_LENGTH == actGCMEncryptUpdate((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, input, (int)inputLength, output, &bytes_out, workSpace->header.watchdog)) {
      return ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updatePlaintextEncryptGCM
 **
 **  This function encrypts the plaintext message using AES-GCM.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - input:       plaintext message
 ** - inputLength: the length of the input
 ** - output:      a pointer to the output buffer
 ** - bytes_out:   a pointer to an output counter variable
 **
 ** output:
 ** - output:      the encrypted message
 ** - bytes_out:   this variable is incremented by the number of bytes that
 **                were handled in this iteration of esl_updatePlaintextEncryptGCM
 ** - returns:     ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID           work space state invalid
 **                ESL_ERC_WS_TOO_SMALL               work space too small
 **                ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 **                ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updatePlaintextEncryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) input,
   const eslt_Length inputLength,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) output,
   CRYPTOCV_P2VAR_PARA( eslt_Size32 ) bytes_out )
{
   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   if (STATE_CIPHERTEXT != ((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM)->state) {
      actGCMTriggerState((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, workSpace->header.watchdog);  /* switch: AAD -> plaintext message */
   }

   /* update actCLib GCM */
   if (actEXCEPTION_LENGTH == actGCMEncryptUpdate((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, input, (int)inputLength, output, (CRYPTOCV_P2VAR_PARA( int )) bytes_out, workSpace->header.watchdog)) {
      return ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_finalizeEncryptGCM
 **
 **  This function finalizes the GCM encryption.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 ** - output:      a pointer to the output buffer for a possible last block
 ** - tag:         a pointer to a buffer for the authentication tag
 **
 ** output:
 ** - output:      if there is still data in the internal buffer, the
 **                remaining bytes are written here
 ** - bytes_out:   incremented by number of remaining bytes
 ** - tag:         this variable is incremented by the number of bytes that
 **                were handled in this iteration of updateEncryptGCM
 ** - returns:     ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID           work space state invalid
 **                ESL_ERC_WS_TOO_SMALL               work space too small
 **                ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeEncryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) output,
   CRYPTOCV_P2VAR_PARA( eslt_Size32 ) bytes_out,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) tag )
{
   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   /* finalize actCLib GCM */
   actGCMEncryptFinalize((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, output, (CRYPTOCV_P2VAR_PARA( int )) bytes_out, tag, workSpace->header.watchdog);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_initDecryptGCM
 **
 **  This function initializes the GCM decryption.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - key:         the symmetric AES master key
 ** - keyLength:   the length of the AES key in bytes, must be 16, 24 or 32
 ** - iv:          the initialization vector
 ** - ivLength:    the length of the initilization vector in bytes
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL            work space too small
 **                ESL_ERC_GCM_INVALID_KEY_LENGTH  no valid AES keylength
 **                ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initDecryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) key,
   const eslt_Length keyLength,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) iv,
   const eslt_Length ivLength )
{
   /* any NULLs ? */
   if ( (!workSpace) || (!key) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* check keyLength */
   if (( keyLength!=ESL_SIZEOF_AES128_KEY ) && ( keyLength!=ESL_SIZEOF_AES192_KEY ) && ( keyLength!=ESL_SIZEOF_AES256_KEY )) {
      return ESL_ERC_GCM_INVALID_KEY_LENGTH;
   }

   /* check ivLength */
   if (1 > ivLength) {
      return ESL_ERC_INVALID_IV_LENGTH;
   }

   /* init actCLib GCM */
   actGCMInit((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, key, (int)keyLength, iv, (int)ivLength, workSpace->header.watchdog);

   /* set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_GCM | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updateAuthDataDecryptGCM
 **
 **  This function decrypts the ciphertext using AES-GCM.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - input:       additional authenticated data
 ** - inputLength: the length of the input
 **
 ** output:
 ** - returns:     ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID           work space state invalid
 **                ESL_ERC_WS_TOO_SMALL               work space too small
 **                ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 **                ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateAuthDataDecryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) input,
   const eslt_Length inputLength )
{
   actU8 *output = NULL;
   int    bytes_out;


   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   if (STATE_AAD != ((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM)->state) {
      return ESL_ERC_GCM_STATE_INVALID;
   }

   /* update actCLib GCM */
   if (actEXCEPTION_LENGTH == actGCMDecryptUpdate((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, input, (int)inputLength, output, &bytes_out, workSpace->header.watchdog)) {
      return ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updateCiphertextDecryptGCM
 **
 **  This function decrypts the ciphertext using AES-GCM.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - input:       ciphertext
 ** - inputLength: the length of the input
 ** - output:      a pointer to the output buffer
 ** - bytes_out:   a pointer to an output counter variable
 **
 ** output:
 ** - output:      decrypted ciphertext
 ** - bytes_out:   this variable is incremented by the number of bytes that
 **                were handled in this iteration of esl_updateCiphertextDecryptGCM
 ** - returns:     ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID           work space state invalid
 **                ESL_ERC_WS_TOO_SMALL               work space too small
 **                ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW  the input exceeded the length limit
 **                ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateCiphertextDecryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) input,
   const eslt_Length inputLength,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) output,
   CRYPTOCV_P2VAR_PARA( eslt_Size32 ) bytes_out )
{
   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   if (STATE_CIPHERTEXT != ((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM)->state) {
      actGCMTriggerState((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, workSpace->header.watchdog);  /* switch: AAD -> plaintext message */
   }

   /* update actCLib GCM */
   if (actEXCEPTION_LENGTH == actGCMDecryptUpdate((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, input, (int)inputLength, output, (CRYPTOCV_P2VAR_PARA( int )) bytes_out, workSpace->header.watchdog)) {
      return ESL_ERC_GCM_TOTAL_LENGTH_OVERFLOW;
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_finalizeDecryptGCM
 **
 **  This function finalizes the GCM operation.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 ** - output:      a pointer to the output buffer for a possible last block
 ** - tag:         a pointer to a buffer containing the authentication tag
 **
 ** output:
 ** - output:      if there is still data in the internal buffer, the
 **                remaining bytes are written here
 ** - bytes_out:   incremented by number of remaining bytes
 ** - returns:     ESL_ERC_PARAMETER_INVALID            input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID             work space state invalid
 **                ESL_ERC_WS_TOO_SMALL                 work space too small
 **                ESL_ERC_GCM_TAG_VERIFICATION_FAILED  the provided tag does not 
 **                                                     match the computed one
 **                ESL_ERC_NO_ERROR                     else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeDecryptGCM (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceGCM ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) output,
   CRYPTOCV_P2VAR_PARA( eslt_Size32 ) bytes_out,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) tag )
{
   eslt_Byte tmpTag[ESL_SIZEOF_GCM_TAG];
   
   
   /* any NULLs ? */
   if ( (!workSpace) ) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO)!=ESL_WST_ALGO_GCM ) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) ) {
      return ESL_ERC_WS_STATE_INVALID;
   } else {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_GCM ) {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   /* finalize actCLib GCM and compare provided tag with computed one */
   actGCMDecryptFinalize((CRYPTOCV_P2VAR_PARA( actGCMSTRUCT ))workSpace->wsGCM, output, (CRYPTOCV_P2VAR_PARA( int )) bytes_out, tmpTag, workSpace->header.watchdog);
   if (actMemcmp (tmpTag, tag, ESL_SIZEOF_GCM_TAG) != 0)
      return ESL_ERC_GCM_TAG_VERIFICATION_FAILED;

   return ESL_ERC_NO_ERROR;
}
