/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_SHA224.c $
 ** $Revision: 2176 $
 ** $Date: 2015-12-21 12:59:36 +0100 (Mo, 21 Dez 2015) $
 ** $Author: mneuhaus $
 **
 ** \copyright(cv cryptovision GmbH, 2015)
 **
 ** \version(cv library/es 1.2)
 **
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
 ** SHA-512 implementation.
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
#include "ESLib_types.h"  /* renamed by Vector */

/* actCLib includes */
#include "actISHA2_64.h"
#include "actUtilities.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/*---------------------------------------------------------------------------
 *    ESL_ERC_...
 *      - workSpace too small
 */
CRYPTOCV_FUNC(eslt_ErrorCode) esl_initSHA512(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace)
{
   /* any NULLs ? */
   if (! workSpace) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if (workSpace->header.size < ESL_MAXSIZEOF_WS_SHA512) {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* init actCLib SHA-512 */
   actSHA512Init ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) workSpace->wsSHA512);

   /* set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_SHA512 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}



/*---------------------------------------------------------------------------
 * inputSize:
 *     The bytelength of the string referenced by *input
 * input:
 *     A pointer to inputSize bytes, that shall be hashed.
 *     Bytes that can not be hashed imediately will be stored
 *     internaly (in workSpace)
 * errorCode:
 *     ESL_ERC_...
 *      - total message length overflow error
 *     ESL_ERC_...
 *      - workSpace too small
 */
CRYPTOCV_FUNC(eslt_ErrorCode) esl_updateSHA512(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace,
  const eslt_Length inputSize,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) input)
{
   /* any NULLs ? */
   if ((! workSpace) || (! input)) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA512) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if (! (workSpace->header.status & ESL_WST_M_RUNNING)) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if (workSpace->header.size < ESL_MAXSIZEOF_WS_SHA512) {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* update actCLib SHA-512 */
   {
      actRETURNCODE result = actSHA512Update ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) workSpace->wsSHA512, input, (int) inputSize, workSpace->header.watchdog);
      if (result == actEXCEPTION_LENGTH) {
         return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
      }
   }

   return ESL_ERC_NO_ERROR;
}



/*---------------------------------------------------------------------------
 * messageDigest:
 *     A pointer to the output of the HASH is the digest of the message.
 *     message = all inputs passed to updateSHA256 during preceeding calls
 *     The size of the messageDigest is always the same for a HASH-algoithm
 *     SHA256 has always 20Byte output (160 bit)
 * errorCode:
 *     ESL_ERC_...
 *      - workSpace too small
 *      - workSpace wrong state
 */
CRYPTOCV_FUNC(eslt_ErrorCode) esl_finalizeSHA512(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceSHA512) workSpace,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) messageDigest)
{
   /* any NULLs ? */
   if ((! workSpace) || (! messageDigest)) {
      return ESL_ERC_PARAMETER_INVALID;
   }

   /* check workSpace */
   if ((workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_SHA512) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if (! (workSpace->header.status & ESL_WST_M_RUNNING)) {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if (workSpace->header.size < ESL_MAXSIZEOF_WS_SHA512) {
      return ESL_ERC_WS_TOO_SMALL;
   }

   /* finalize actCLib SHA-512 */
   actSHA512Finalize ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) workSpace->wsSHA512, messageDigest, workSpace->header.watchdog);

   /* reset and clear workspace */
   workSpace->header.status = ESL_WST_ALLOCATED;
   actMemset (workSpace->wsSHA512, 0, (unsigned int) (workSpace->header.size));

   return ESL_ERC_NO_ERROR;
}
