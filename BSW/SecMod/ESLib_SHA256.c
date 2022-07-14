/****************************************************************************
 ** Main author: Uke                     Creation date: 02/24/05
 ** $LastChangedBy: alf $
 ** $Date: 2008-01-16 18:18:19 +0100 (Mi, 16. Jan 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_SHA256.c $
 ** $Rev: 1414 $
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
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actISHA2_32.h"
#include "actUtilities.h"

CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Init( CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info );

/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/*---------------------------------------------------------------------------
      ESL_ERC_...
        - workSpace too small */
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initSHA256( CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ) workSpace )
{
   /* Any NULLs ? */
   if ( !workSpace )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Init actCLib SHA-256 */
   (void)actSHA256Init( (CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ))workSpace->wsSHA256 );

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_SHA256 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}



/*---------------------------------------------------------------------------
  inputSize:
      The bytelength of the string referenced by *input
  input:
      A pointer to inputSize bytes, that shall be hashed.
      Bytes that can not be hashed imediately will be stored
      internaly (in workSpace)
   ErrorCode:
      ESL_ERC_...
       - total message length overflow error
         (2^64-1 bit ~= 2^61 byte = 2^21 Tbyte )
      ESL_ERC_...
       - workSpace too small (can not occure, as detected during init) */
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ) workSpace,
   const eslt_Length inputSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!input) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA256 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* Update actCLib SHA-256 */
   {
      actRETURNCODE result = actSHA256Update( (CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ))workSpace->wsSHA256,
                                                                    input, (int)inputSize, workSpace->header.watchdog );
      if ( result == actEXCEPTION_LENGTH )
      {
         return ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW;
      }
   }

   return ESL_ERC_NO_ERROR;
}



/*---------------------------------------------------------------------------
  messageDigest:
      A pointer to the output of the HASH is the digest of the message.
      message = all inputs passed to updateSHA256 during preceeding calls
      The size of the messageDigest is always the same for a HASH-algoithm
      SHA256 has always 32Byte output (256 bit)
   ErrorCode:
      ESL_ERC_...
       - workSpace too small (can not occure, as detected during init)
      ESL_ERC_...
       - no input data passed previously,
       - no call to esl_updateSHA256 has been done, or only with empty inputs */
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageDigest )
{
   /* Any NULLs ? */
   if ( (!workSpace) || (!messageDigest) )
   {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA256 )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( workSpace->header.size < ESL_MAXSIZEOF_WS_SHA256 )
   {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* finalize actCLib SHA-1 */
   (void)actSHA256Finalize( (CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ))workSpace->wsSHA256,
                      messageDigest, workSpace->header.watchdog );

   /* Reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset( workSpace->wsSHA256, 0, (unsigned int)(workSpace->header.size) );

   return ESL_ERC_NO_ERROR;
}

