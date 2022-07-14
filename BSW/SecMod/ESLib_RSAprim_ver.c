/****************************************************************************
 ** Main author: Sst                     Creation date: 06/10/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSAprim_ver.c $
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
 ** This file contains: RSA verify primitive
 **                     Currently the actClib version is used.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initVerifyRSA_prim
 **   esl_verifyRSA_prim
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIRSA.h"

/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION: esl_initVerifyRSA_prim
 **
 **  This function initializes the RSA verification primitive.
 **
 ** input:
 ** - workSpace:              algorithm context buffer, initialized by
 **                           esl_initWorkSpaceHeader(..) function
 ** - keyPairModuleSize:      the byte length of the modulus n
 ** - keyPairModule:          pointer to the modulus n
 ** - publicKeyExponentSize:  the byte length of the public exponent e
 ** - publicKeyExponent:      pointer to the public exponent e
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:  ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **             ESL_ERC_WS_TOO_SMALL               work space too small
 **             ESL_ERC_RSA_PUBKEY_INVALID         the public exponent is
 **                                                invalid
 **             ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initVerifyRSA_prim(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ) workSpace,
   eslt_Length   keyPairModuleSize, eslt_pRomByte keyPairModule,
   eslt_Length   publicKeyExponentSize, eslt_pRomByte publicKeyExponent )
{
   /* Init actCLib RSA public key initialization */
   {
      actRETURNCODE result = actRSAInitPublicKeyOperation( keyPairModule, (int)keyPairModuleSize, publicKeyExponent,
                                                           (int)publicKeyExponentSize, workSpace->wsRSA,
                                                           (int)(workSpace->header.size), workSpace->header.watchdog);
      if ( result != actOK )
      {
         if ( result == actEXCEPTION_NULL )
         {
            return ESL_ERC_PARAMETER_INVALID;
         }
         else if ( result == actEXCEPTION_MEMORY )
         {
            return ESL_ERC_WS_TOO_SMALL;
         }
         else if ( result == actRSA_PARAM_OUT_OF_RANGE )
         {
            return ESL_ERC_PARAMETER_INVALID;
         }
         else if ( result == actEXCEPTION_PUBKEY )
         {
            return ESL_ERC_RSA_PUBKEY_INVALID;
         }
      }
   }

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_verifyRSA_prim
 **
 **  This function performs the RSA verification primitive (RSA_VP1).
 **
 ** input:
 ** - workSpace:              algorithm context buffer, initialized by
 **                           esl_initWorkSpaceHeader(..) function
 ** - signatureSize:          the byte length of the given signature
 ** - signature:              pointer to the given signature
 ** - messageSize:            pointer to byte length of the output buffer
 ** - message:                pointer to the output buffer
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:  ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **             ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE (int)signature > n-1
 **             ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *codeSize is too small
 **             ESL_ERC_WS_STATE_INVALID           invalid Algo
 **             ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifyRSA_prim(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ) workSpace,
   eslt_Length signatureSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) signature,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message )
{
   /* Check workSpace */
   if ( (workSpace->header.status & ESL_WST_M_ALGO) != ESL_WST_ALGO_RSA )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }
   if ( !(workSpace->header.status & ESL_WST_M_RUNNING) )
   {
      return ESL_ERC_WS_STATE_INVALID;
   }

   /* Call actCLib RSA public key operation */
   {
      int messageLen = *messageSize;
      actRETURNCODE result = actRSAPublicKeyOperation( signature, (int)signatureSize, message, &messageLen,
                                                       workSpace->wsRSA, workSpace->header.watchdog );
      if ( result != actOK )
      {
         if ( result == actEXCEPTION_NULL )
         {
            return ESL_ERC_PARAMETER_INVALID;
         }
         else if ( result == actEXCEPTION_MEMORY )
         {
            return ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
         }
         else if ( result == actRSA_PARAM_OUT_OF_RANGE )
         {
            return ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE;
         }
      }
      *messageSize = (eslt_Length)(messageLen & 0xFFFFu);
   }

   return ESL_ERC_NO_ERROR;
}


