/****************************************************************************
 ** Main author: Sst                     Creation date: 06/10/05
 ** $LastChangedBy: men $
 ** $Date: 2008-05-13 12:23:58 +0200 (Di, 13. Mai 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSAprim_decCRT.c $
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
 ** This file contains: RSA sign decrypt primitive (CRT version)
 **                     Currently the actClib version is used.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initDecryptRSACRT_prim
 **   esl_decryptRSACRT_prim
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
 ** FUNCTION: esl_initDecryptRSACRT_prim
 **
 **  This function initializes the RSA decryption primitive (CRT version).
 **
 ** input:
 ** - workSpace:                 algorithm context buffer, initialized by
 **                              esl_initWorkSpaceHeader(..) function
 ** - keyPairPrimePSize:         the byte length of the prime p
 ** - keyPairPrimeP:             pointer to prime p
 ** - keyPairPrimeQSize:         the byte length of the prime q
 ** - keyPairPrimeQ:             pointer to prime q
 ** - privateKeyExponentDPSize:  the byte length of dp
 ** - privateKeyExponentDP:      pointer to CRT key component dp
 ** - privateKeyExponentDQSize:  the byte length of dq
 ** - privateKeyExponentDQ:      pointer to CRT key component dq
 ** - privateKeyInverseQISize:   the byte length of qInv
 ** - privateKeyInverseQISize:   pointer to CRT key component qInv
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:  ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **             ESL_ERC_WS_TOO_SMALL               work space too small
 **             ESL_ERC_RSA_PRIVKEY_INVALID        the private key
 **                                                is invalid
 **             ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initDecryptRSACRT_prim(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ) workSpace,
   eslt_Length keyPairPrimePSize, eslt_pRomByte keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, eslt_pRomByte keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, eslt_pRomByte privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, eslt_pRomByte privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, eslt_pRomByte privateKeyInverseQI )
{
   /* Init actCLib RSA private key initialization */
   {
      actRETURNCODE result = actRSAInitPrivateKeyOperationCRT( keyPairPrimeP, (int)keyPairPrimePSize, keyPairPrimeQ,
                                                               (int)keyPairPrimeQSize, privateKeyExponentDP, (int)privateKeyExponentDPSize,
                                                               privateKeyExponentDQ, (int)privateKeyExponentDQSize,
                                                               privateKeyInverseQI, (int)privateKeyInverseQISize,
                                                               workSpace->wsRSA, (int)(workSpace->header.size), workSpace->header.watchdog );
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
         else if ( result == actEXCEPTION_PRIVKEY )
         {
            return ESL_ERC_RSA_PRIVKEY_INVALID;
         }
      }
   }

   /* Set workSpace state */
   workSpace->header.status = (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

   return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_decryptRSACRT_prim
 **
 **  This function performs the RSA decryption operation (RSA_DP1).
 **
 ** input:
 ** - workSpace:              algorithm context buffer, initialized by
 **                           esl_initWorkSpaceHeader(..) function
 ** - cipherSize:             the byte length of the input
 ** - cipher:                 pointer to the input
 ** - messageSize:            pointer to byte length of the output buffer
 ** - message:                pointer to the output buffer
 **
 ** output:
 ** - workSpace:   initialized algorithm context structure
 ** - returns:  ESL_ERC_PARAMETER_INVALID          input parameter is NULL
 **             ESL_ERC_RSA_CODE_OUT_OF_RANGE   (int)cipher > n-1
 **             ESL_ERC_OUTPUT_SIZE_TOO_SHORT      *messageSize is too small
 **             ESL_ERC_WS_STATE_INVALID           invalid Algo
 **             ESL_ERC_NO_ERROR                   else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptRSACRT_prim(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
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

   /* Call actCLib RSA private key operation (CRT) */
   {
      int messageLen = *messageSize;
      actRETURNCODE result = actRSAPrivateKeyOperationCRT ( cipher, (int)cipherSize, message, &messageLen, workSpace->wsRSA,
                                                            (int)(workSpace->header.size), workSpace->header.watchdog );
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
            return ESL_ERC_RSA_CODE_OUT_OF_RANGE;
         }
      }
      *messageSize = (eslt_Length)(messageLen & 0xFFFFu);
   }


   return ESL_ERC_NO_ERROR;
}

