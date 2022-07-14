/****************************************************************************
 ** Main author: Ubr                     Creation date: 04/18/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_HashMACSHA256.c $
 ** $Rev: 1620 $
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
 ** This file contains: Hash MAC SHA-256 implementation file.
 **                     Currently the actClib version is used.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initHashMACSHA256
 **   esl_updateHashMACSHA256
 **   esl_finalizeHashMACSHA256
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIHashMACSHA256.h"
#include "actUtilities.h"


/****************************************************************************
 **
 ** FUNCTION: esl_initHashMACSHA256
 **
 **  This function initializes the HashMAC calculation according
 **  to RFC2104 based on the hash function SHA-256.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 ** - keylength:   the byte length of the key to be used
 ** - key:         pointer to user key
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE      the given key length is zero
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initHashMACSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHMACSHA256 ) workSpace,
   const eslt_Length keyLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) key )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!key) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_HMACSHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Check keyLength */
   if ( keyLength == 0 )
   {
      return ESL_ERC_HMAC_KEY_LENGTH_OUT_OF_RANGE;
   }

   /* Init actCLib HMAC-SHA-256 */
   (void)actHashMACSHA256Init( (CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ))workSpace->wsHMACSHA256,
                               key, (int)keyLength, workSpace->header.watchdog );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_HMACSHA256 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updateHashMACSHA256
 **
 **  This function updates the HashMAC calculation according
 **  to RFC2104 based on the hash function SHA-256.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initHashMACSHA256(..) function
 ** - inputLength: length of input data in bytes
 ** - input:       pointer to data to be handled
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW   total input length overflow
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initHashMACSHA256(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateHashMACSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHMACSHA256 ) workSpace,
   const eslt_Length inputLength, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
   actRETURNCODE result;

   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_HMACSHA256 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( inputLength % ESL_SIZEOF_SHA256_BLOCK == 0 )
   {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_HMACSHA256 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }
   else
   {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_HMACSHA256 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   /* update actCLib HMAC-SHA-256 */
   result = actHashMACSHA256Update( (CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ))workSpace->wsHMACSHA256,
                                    input, (int)inputLength, workSpace->header.watchdog );
   if ( result == actEXCEPTION_LENGTH )
   {
      return ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_finalizeHashMACSHA256
 **
 **  This function finalizes the HashMAC calculation by returning
 **  the calculated HashMAC according to RFC2104 based on SHA-256
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initHashMACSHA256(..) function
 ** - messageHashMAC: pointer to buffer for HashMAC (at least 32 bytes)
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - messageHashMAC: pointer to computed HashMAC
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initHashMACSHA256(..)
 ** - esl_updateHashMACSHA256 called before
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeHashMACSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHMACSHA256 ) workSpace, CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageHashMAC )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!messageHashMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_HMACSHA256 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_HMACSHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Finalize actCLib HMAC-SHA-1 */
   (void)actHashMACSHA256Finalize( (CRYPTOCV_P2VAR_PARA( actHASHMACSHA256STRUCT ))workSpace->wsHMACSHA256,
                                   messageHashMAC, workSpace->header.watchdog );

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( workSpace->wsHMACSHA256, 0, (unsigned int)(workSpace->header.size) );

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_verifyHashMACSHA256
 **
 **  This function finalizes the verification of a given HashMAC
 **  according to RFC2104 based on SHA-256.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initHashMACSHA256(..) function
 ** - messageHashMAC: pointer to the given HashMAC (32 bytes)
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_HMAC_INCORRECT_MAC   the given HashMAC is incorrect
 **                ESL_ERC_NO_ERROR          HashMAC is correct
 **
 ** assumes:
 ** - workSpace is initialized by esl_initHashMACSHA256(..)
 ** - esl_updateHashMACSHA256 called before
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifyHashMACSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHMACSHA256 ) workSpace, CRYPTOCV_P2CONST_PARA( eslt_Byte ) messageHashMAC )
{
   eslt_Byte tmpMAC[ESL_SIZEOF_SHA256_DIGEST];

   /* Any NULLs ? */
   if ( (!workSpace) || (!messageHashMAC) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_HMACSHA256 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_HMACSHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* calculate MAC */
   (void)esl_finalizeHashMACSHA256(workSpace, tmpMAC);
   if ( actMemcmp(tmpMAC, messageHashMAC, ESL_SIZEOF_SHA256_DIGEST) != 0 )
   {
      return ESL_ERC_HMAC_INCORRECT_MAC;
   }

   return ESL_ERC_NO_ERROR;
}
