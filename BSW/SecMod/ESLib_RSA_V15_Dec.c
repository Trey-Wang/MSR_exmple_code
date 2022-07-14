/****************************************************************************
 ** Main author: msr                     Creation date: 03/21/06
 ** $LastChangedBy: mns $
 ** $Date: 2008-02-06 16:20:38 +0100 (Mi, 06. Feb 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSA_V15_Dec.c $
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
 **    esl_initDecryptRSA_V15
 **    esl_decryptRSA_V15
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actUtilities.h"

/****************************************************************************
 ** Local Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_decryptInternal(      eslt_WorkSpaceRSAdec *workSpace,
 **                            eslt_Length           cipherSize,
 **                      const eslt_Byte            *cipher,
 **                            eslt_Length          *messageSize,
 **                            eslt_Byte            *message
 **                            eslt_Byte             blockType)
 **
 ** input:
 ** - workSpace
 ** - cipherSize
 ** - cipher
 ** - blockType
 **
 ** output:
 ** - messageSize
 ** - message
 ** - errorCode:
 **      ESL_ERC_NO_ERRO
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_RSA_CODE_OUT_OF_RANGE
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 ** - keyExponent  < keyPairModule
 ** - Module is a prime
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptInternal(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message,
   eslt_Byte blockType )
{

    eslt_ErrorCode            returnValue;
    eslt_Length               keyPairModuleSize;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec_prim ) wsPRIM;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageBuf;
    eslt_Length               i;

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
        wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec_prim ))(wsRAW + ESL_WS_RSA_V15_ED_OFFSET_WS_RSA_PRIM(keyPairModuleSize));
        messageBuf        = wsRAW + ESL_WS_RSA_V15_ED_OFFSET_BUFFER;

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
            returnValue = esl_decryptRSA_prim( wsPRIM, cipherSize, cipher, &i, messageBuf );
            if ( ESL_ERC_NO_ERROR == returnValue )
            {
                if ( (i != keyPairModuleSize) || (ESL_ERC_NO_ERROR != esl_verifyPaddingRSAEM_V15(messageBuf, &i, blockType)) )
                {
                    returnValue = ESL_ERC_RSA_ENCODING_INVALID;
                }
                else
                {
                    *messageSize = (eslt_Length)(keyPairModuleSize - i);
                    actMemcpy(message, messageBuf + (keyPairModuleSize - *messageSize), (unsigned int) *messageSize);
                }
            }
        }
    }
    return returnValue;
}

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_initDecryptRSA_V15(       eslt_WorkSpaceRSAdec *workSpace,
 **                                eslt_Length           keyPairModuleSize,
 **                          const eslt_Byte            *keyPairModule,
 **                                eslt_Length           keyExponentSize,
 **                          const eslt_Byte            *keyExponent)
 **
 ** input:
 ** - workSpace
 ** - keyPairModuleSize
 ** - keyPairModule
 ** - keyExponentSize
 ** - keyExponent
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **
 ** assumes:
 ** - keyExponent  < keyPairModule
 ** - Module is a prime
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initDecryptRSA_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec ) workSpace,
   eslt_Length keyPairModuleSize, eslt_pRomByte keyPairModule,
   eslt_Length privateKeyExponentSize, eslt_pRomByte privateKeyExponent )
{

    eslt_ErrorCode returnValue;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec_prim ) wsPRIM;

    returnValue = ESL_ERC_NO_ERROR;

    if ( !workSpace )
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        /* get underlying workspace pointer */
        wsRAW       = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15 = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        wsPRIM      = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec_prim ))(wsRAW + ESL_WS_RSA_V15_ED_OFFSET_WS_RSA_PRIM(keyPairModuleSize));

        if ( wsHeaderV15->size < (ESL_SIZEOF_WS_RSA_V15_ED_V15OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)) )
        {
            returnValue = ESL_ERC_WS_TOO_SMALL;
        }
        else
        {
            wsPRIM->header.size = (eslt_Length)(wsHeaderV15->size - (ESL_SIZEOF_WS_RSA_V15_ED_V15OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)));
            wsPRIM->header.watchdog = wsHeaderV15->watchdog;
            returnValue = esl_initDecryptRSA_prim( wsPRIM, keyPairModuleSize, keyPairModule, privateKeyExponentSize, privateKeyExponent );
        }
        if ( ESL_ERC_NO_ERROR == returnValue )
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
 **  esl_decryptPubRSA_V15(    eslt_WorkSpaceRSAdec *workSpace,
 **                            eslt_Length           cipherSize,
 **                      const eslt_Byte            *cipher,
 **                            eslt_Length          *messageSize,
 **                            eslt_Byte            *message)
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
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 ** - public keyExponent  < keyPairModule
 ** - Module is a prime
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptPubRSA_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message )
{
   /* Cipher was encrypted using private key */
   return esl_decryptInternal(workSpace, cipherSize, cipher, messageSize, message, ASN1_PADDING_BLOCK_TYPE_PRIVATE);
}

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_decryptPrivRSA_V15(   eslt_WorkSpaceRSAdec *workSpace,
 **                            eslt_Length           cipherSize,
 **                      const eslt_Byte            *cipher,
 **                            eslt_Length          *messageSize,
 **                            eslt_Byte            *message)
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
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 ** - private keyExponent  < keyPairModule
 ** - Module is a prime
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptPrivRSA_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message )
{
   /* Cipher was encrypted using public key */
   return esl_decryptInternal(workSpace, cipherSize, cipher, messageSize, message, ASN1_PADDING_BLOCK_TYPE_PUBLIC);
}

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_decryptRSA_V15(       eslt_WorkSpaceRSAdec *workSpace,
 **                            eslt_Length           cipherSize,
 **                      const eslt_Byte            *cipher,
 **                            eslt_Length          *messageSize,
 **                            eslt_Byte            *message)
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
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 ** - private keyExponent  < keyPairModule
 ** - Module is a prime
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_decryptRSA_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAdec ) workSpace,
   eslt_Length cipherSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) cipher,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) messageSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) message )
{
   /* Default uses private key for decryption */
   return esl_decryptPrivRSA_V15(workSpace, cipherSize, cipher, messageSize, message);
}
