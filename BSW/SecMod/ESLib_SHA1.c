/****************************************************************************
 ** Main author: Ubr                     Creation date: 02/24/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_SHA1.c $
 ** $Rev: 1620 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2005)
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
 ** This file contains: Sha-1 implementation file.
 **                     Currently the actClib version is used.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initSHA1
 **   esl_updateSHA1
 **   esl_finalizeSHA1
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actISHA.h"
#include "actUtilities.h"


/****************************************************************************
 **
 ** FUNCTION: esl_initSHA1
 **
 **  This function initializes the SHA-1 hash calculation.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initWorkSpaceHeader(..) function
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initSHA1( CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ) workSpace )
{
   /* Any NULLs ? */
   if ( !workSpace )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MINSIZEOF_WS_SHA1 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Init actCLib SHA-1 */
   (void)actSHAInit( (CRYPTOCV_P2VAR_PARA( actSHASTRUCT ))workSpace->wsSHA1 );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_SHA1 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 **
 ** FUNCTION: esl_updateSHA1
 **
 **  This function updates the SHA-1 calculation for the given
 **  input message (or parts of it). This function cann be called
 **  several times until all input to be hashed is handled.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initSHA1(..) function
 ** - workSpace: length of input data in bytes
 ** - input:       pointer to data to be handled
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW   total input length overflow
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initSHA1(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateSHA1(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ) workSpace,
   const eslt_Length inputSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA1 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( inputSize % ESL_SIZEOF_SHA1_BLOCK == 0 )
   {
      if ( workSpace->header.size < ESL_MINSIZEOF_WS_SHA1 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }
   else
   {
      if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SHA1 )
      {
         return ESL_ERC_WS_TOO_SMALL;
      }
   }

   /* Update actCLib SHA-1 */
   {
      actRETURNCODE result = actSHAUpdate( (CRYPTOCV_P2VAR_PARA( actSHASTRUCT ))workSpace->wsSHA1,
                                           (CRYPTOCV_P2CONST_PARA( actU8 ))input,
                                           (int)inputSize,
                                           workSpace->header.watchdog );
      if ( result == actEXCEPTION_LENGTH )
      {
         return ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
      }
   }

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_finalizeSHA1
 **
 **  This function finalizes the SHA-1 calculation by doing the appropriate
 **  padding of the input value and returning the hash value.
 **
 ** input:
 ** - workSpace:   algorithm context buffer, initialized by
 **                esl_initSHA1(..) function
 ** - messageDigest: pointer to buffer for the hash value (20 bytes)
 **
 ** output:
 ** - workSpace:   actualized algorithm context structure
 ** - messageDigest: pointer to computed hash value
 ** - returns:     ESL_ERC_PARAMETER_INVALID input parameter is NULL
 **                ESL_ERC_WS_STATE_INVALID  work space state invalid
 **                ESL_ERC_WS_TOO_SMALL      work space too small
 **                ESL_ERC_NO_ERROR          else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initSHA1(..)
 ** - esl_updateSHA1 called before
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeSHA1(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageDigest )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!messageDigest) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA1 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SHA1 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Finalize actCLib SHA-1 */
   (void)actSHAFinalize( (CRYPTOCV_P2VAR_PARA( actSHASTRUCT ))workSpace->wsSHA1,
                         (CRYPTOCV_P2VAR_PARA( actU8 ))messageDigest,
                         workSpace->header.watchdog );

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( (CRYPTOCV_P2VAR_PARA( actU8 ))workSpace->wsSHA1,
              0,
              (unsigned int)(workSpace->header.size) );

   return ESL_ERC_NO_ERROR;
}

