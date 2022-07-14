/****************************************************************************
 ** Main author: msr                     Creation date: 03/21/06
 ** $LastChangedBy: mns $
 ** $Date: 2008-02-06 16:20:38 +0100 (Mi, 06. Feb 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSA_V15_DecCRT.c $
 ** $Rev: 1435 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
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
 ** This file contains: RSA V1.5 (Encryption/Decryption) implementation file.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **    esl_initDecryptRSACRT_V15
 **    esl_decryptRSACRT_V15
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actUtilities.h"


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_initDecryptRSACRT_V15(       eslt_WorkSpaceRSACRTdec *workSpace,
 **                                   eslt_Length              keyPairPrimePSize,
 **                             const eslt_Byte               *keyPairPrimeP,
 **                                   eslt_Length              keyPairPrimeQSize,
 **                             const eslt_Byte               *keyPairPrimeQ,
 **                                   eslt_Length              privateKeyExponentDPSize,
 **                             const eslt_Byte               *privateKeyExponentDP,
 **                                   eslt_Length              privateKeyExponentDQSize,
 **                             const eslt_Byte               *privateKeyExponentDQ,
 **                                   eslt_Length              privateKeyInverseQISize,
 **                             const eslt_Byte               *privateKeyInverseQI)
 **
 ** input:
 ** - workSpace
 ** - keyPairPrimePSize
 ** - keyPairPrimeP
 ** - keyPairPrimeQSize
 ** - keyPairPrimeQ
 ** - privateKeyExponentDPSize
 ** - privateKeyExponentDP
 ** - privateKeyExponentDQSize
 ** - privateKeyExponentDQ
 ** - privateKeyInverseQISize
 ** - privateKeyInverseQI
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_PARAMETER_INVALID   format problems, nullpointer, .. 
 **      ESL_ERC_PRIVKEY_INVALID
 **
 ** assumes:
 ** - PrimeP and Q are a prime
 ** - privateKeyExpDP<PrimeP
 ** - privateKeyExpDQ<PrimeQ
 ** - QI is inverse to Q, could be checked, then move this line to errorcodes
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initDecryptRSACRT_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec ) workSpace,
   eslt_Length keyPairPrimePSize, eslt_pRomByte keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, eslt_pRomByte keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, eslt_pRomByte privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, eslt_pRomByte privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, eslt_pRomByte privateKeyInverseQI )
{
   eslt_ErrorCode               returnValue;
   CRYPTOCV_P2VAR_PARA( eslt_Byte )            wsRAW;
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;
   eslt_Length                  keyPairModuleSize;
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ) wsPRIM;

   returnValue = ESL_ERC_NO_ERROR;

   if ( !workSpace )
   {
      returnValue = ESL_ERC_PARAMETER_INVALID;
   }
   else
   {
      /* get underlying workspace pointer */
      wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
      wsHeaderV15       = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
      keyPairModuleSize = (eslt_Length)(keyPairPrimePSize + keyPairPrimeQSize);
      wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ))(wsRAW + ESL_WS_RSA_V15_ED_OFFSET_WS_RSA_PRIM(keyPairModuleSize));

      if ( wsHeaderV15->size < (ESL_SIZEOF_WS_RSA_V15_ED_V15OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)) )
      {
         returnValue = ESL_ERC_WS_TOO_SMALL;
      }
      else
      {
         wsPRIM->header.size = (eslt_Length)(wsHeaderV15->size - (ESL_SIZEOF_WS_RSA_V15_ED_V15OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)));
         wsPRIM->header.watchdog = wsHeaderV15->watchdog;
         returnValue = esl_initDecryptRSACRT_prim(wsPRIM,
                                       keyPairPrimePSize, keyPairPrimeP,
                                       keyPairPrimeQSize, keyPairPrimeQ,
                                       privateKeyExponentDPSize, privateKeyExponentDP,
                                       privateKeyExponentDQSize, privateKeyExponentDQ,
                                       privateKeyInverseQISize, privateKeyInverseQI);
      }
      if (ESL_ERC_NO_ERROR == returnValue)
      {
         wsHeaderV15->status = (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
         /* store message buffer length in workspace */
         wsRAW[ESL_WS_RSA_V15_ED_OFFSET_BUFFERLENGTH]     = (eslt_Byte)(keyPairModuleSize >> 8);
         wsRAW[ESL_WS_RSA_V15_ED_OFFSET_BUFFERLENGTH + 1] = (eslt_Byte)(keyPairModuleSize);
      }
   }
   return returnValue;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_decryptRSACRT_V15(       eslt_WorkSpaceRSACRTdec *workSpace,
 **                               eslt_Length              cipherSize,
 **                         const eslt_Byte               *cipher,
 **                               eslt_Length             *messageSize,
 **                               eslt_Byte               *message)
 **
 ** input:
 ** - workSpace
 ** - cipherSize
 ** - cipher
 **
 ** output:
 ** - messageSize
 ** - message
 ** - errorCode:
 **      ESL_ERC_NO_ERRO
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_RSA_CODE_OUT_OF_RANGE
 **      - 0 <= (int)cipher <= modul-1, where module = PrimeP*PrimeQ
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptRSACRT_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message )
{

    eslt_ErrorCode returnValue;
    eslt_Length    keyPairModuleSize;
    eslt_Length    i;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ) wsPRIM;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageBuf;

    returnValue = ESL_ERC_NO_ERROR;

    if ( !workSpace )
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15       = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        keyPairModuleSize = (eslt_Length)(((eslt_Length)wsRAW[ESL_WS_RSA_V15_ED_OFFSET_BUFFERLENGTH] << 8) + (wsRAW[ESL_WS_RSA_V15_ED_OFFSET_BUFFERLENGTH + 1]));
        wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTdec_prim ))(wsRAW + ESL_WS_RSA_V15_ED_OFFSET_WS_RSA_PRIM(keyPairModuleSize));
        messageBuf        = wsRAW + ESL_WS_RSA_V15_ED_OFFSET_BUFFER;
        keyPairModuleSize = (eslt_Length)( ((CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ))wsPRIM->wsRSA)->n_bytes );

        if ( ESL_WST_ALGO_RSA != (wsHeaderV15->status & ESL_WST_M_ALGO) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else if ( 0 == (wsHeaderV15->status & ESL_WST_M_RUNNING) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else
        {
            i = keyPairModuleSize;
            returnValue = esl_decryptRSACRT_prim( wsPRIM, cipherSize, cipher, &i, messageBuf );
            if ( ESL_ERC_NO_ERROR == returnValue )
            {
                if ( (i != keyPairModuleSize) || (0x00 != messageBuf[0]) || (0x02 != messageBuf[1]) )
                {
                    returnValue = ESL_ERC_RSA_ENCODING_INVALID;
                }
                else
                {
                    i = 2;
                    while ( (0x00 != messageBuf[i]) && (i < keyPairModuleSize) )
                    {
                        i++;
                    }
                    if ( 0x00 == messageBuf[i] )
                    {
                        i++;
                        if ( ASN1_SIZEOF_MINIMAL_PADDING <= i )
                        {
                            *messageSize = (eslt_Length)(keyPairModuleSize - i);
                            actMemcpy( message, messageBuf + (keyPairModuleSize - *messageSize), (unsigned int) *messageSize );
                        }
                        else
                        {
                            returnValue = ESL_ERC_RSA_ENCODING_INVALID;
                        }
                    }
                    else
                    {
                        returnValue = ESL_ERC_RSA_ENCODING_INVALID;
                    }
                }
            }
        }
    }
    return returnValue;
}
