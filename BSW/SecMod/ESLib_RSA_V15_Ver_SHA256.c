/****************************************************************************
 ** Main author: vishp                     Creation date: 2012-08-27
 ** $LastChangedBy: mns $
 ** $Date: 2012-08-27
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSA_V15_Ver_SHA256.c $
 ** $Rev: 1757 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
 ** Modified by vishp, Vector Informatik GmbH
 ** V01.00.00  vishp     creation
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
 ** This file contains: RSA V1.5 (Sign_SHA256/Verify_SHA256) implementation file.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:

 **    esl_initVerifyRSASHA256_V15
 **    esl_updateVerifyRSASHA256_V15
 **    esl_finalizeVerifyRSASHA256_V15
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actUtilities.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

CRYPTOCV_CONST( STATIC, eslt_Byte ) eslt_ASN1_DIGESTINFO_SHA256[] = ASN1_DIGESTINFO_SHA256;

/****************************************************************************
 ** Local Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_getKeyPairModuleSize( eslt_WorkSpaceRSAver *workSpace,
 **                            eslt_Length          *keyPairModuleSize)
 ** input:
 ** - workSpace
 **
 ** output:
 ** - keyPairModuleSize
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **
 ** assumes:
 ** - publicKeyExponent  < keyPairModule
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( eslt_ErrorCode ) esl_getKeyPairModuleSize(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) keyPairModuleSize )
{

    eslt_ErrorCode            returnValue;
    CRYPTOCV_P2VAR_PARA( eslt_Byte            ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;

    returnValue = ESL_ERC_NO_ERROR;

    if (!workSpace)
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        /* get underlying workspace pointer */
        wsRAW              = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15        = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        *keyPairModuleSize = (eslt_Length)(((eslt_Length)wsRAW[ESL_WS_RSA_V15_SV_SHA256_BUFFERLENGTH] << 8) + (wsRAW[ESL_WS_RSA_V15_SV_SHA256_BUFFERLENGTH + 1]));

        if ( (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING) != (wsHeaderV15->status & (ESL_WST_M_ALGO | ESL_WST_M_RUNNING)) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
    }

    return returnValue;
}

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_getWorkspaceSHA256( eslt_WorkSpaceRSAver *workSpace,
 **                        eslt_WorkSpaceSHA256  **wsSHA256)
 ** input:
 ** - workSpace
 **
 ** output:
 ** - wsSHA256
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **
 ** assumes:
 ** - publicKeyExponent  < keyPairModule
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( eslt_ErrorCode ) esl_getWorkspaceSHA256(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   CRYPTOCV_P2VAR_PARA( CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 )  ) wsSHA256 )
{

    eslt_ErrorCode returnValue;
    eslt_Length    keyPairModuleSize;
    CRYPTOCV_P2VAR_PARA( eslt_Byte ) wsRAW;

    /* Evaluate workspace status, value of key pair module size not used */
    returnValue = esl_getKeyPairModuleSize(workSpace, &keyPairModuleSize);

    if (ESL_ERC_NO_ERROR == returnValue) {
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        *wsSHA256         = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ))(wsRAW + ESL_WS_RSA_V15_SV_SHA256_WS_SHA256(keyPairModuleSize));
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
 **  esl_initVerifyRSASHA256_V15(       eslt_WorkSpaceRSAver *workSpace,
 **                                     eslt_Length           keyPairModuleSize,
 **                               const eslt_Byte            *keyPairModule,
 **                                     eslt_Length           publicKeyExponentSize,
 **                               const eslt_Byte            *publicKeyExponent)
 ** input:
 ** - workSpace
 ** - keyPairModuleSize
 ** - keyPairModule
 ** - publicKeyExponentSize
 ** - publicKeyExponent
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **
 ** assumes:
 ** - publicKeyExponent  < keyPairModule
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initVerifyRSASHA256_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   eslt_Length keyPairModuleSize, eslt_pRomByte keyPairModule,
   eslt_Length publicKeyExponentSize, eslt_pRomByte publicKeyExponent )
{
    eslt_ErrorCode returnValue;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                 ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader      ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256      ) wsSHA256;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ) wsPRIM;

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
        wsSHA256    = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ))(wsRAW + ESL_WS_RSA_V15_SV_SHA256_WS_SHA256(keyPairModuleSize));
        wsPRIM      = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ))(wsRAW + ESL_WS_RSA_V15_SV_SHA256_WS_RSA_PRIM(keyPairModuleSize));

        if ( wsHeaderV15->size < (ESL_SIZEOF_WS_RSA_V15_SV_SHA256_OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)) )
        {
            returnValue = ESL_ERC_WS_TOO_SMALL;
        }
        else
        {
            wsSHA256->header.size = (eslt_Length)(sizeof(eslt_WorkSpaceSHA256) - sizeof(eslt_WorkSpaceHeader));
            wsSHA256->header.watchdog = wsHeaderV15->watchdog;
            returnValue = esl_initSHA256( wsSHA256 );
            if ( ESL_ERC_NO_ERROR == returnValue )
            {
                wsPRIM->header.size = (eslt_Length)(wsHeaderV15->size - (ESL_SIZEOF_WS_RSA_V15_SV_SHA256_OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)));
                wsPRIM->header.watchdog = wsHeaderV15->watchdog;
                returnValue = esl_initVerifyRSA_prim( wsPRIM, keyPairModuleSize, keyPairModule, publicKeyExponentSize, publicKeyExponent );
            }
        }
        if ( ESL_ERC_NO_ERROR == returnValue )
        {
            wsHeaderV15->status = (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
            /* store message buffer length in workspace */
            wsRAW[ESL_WS_RSA_V15_SV_SHA256_BUFFERLENGTH]     = (eslt_Byte)(keyPairModuleSize >> 8);
            wsRAW[ESL_WS_RSA_V15_SV_SHA256_BUFFERLENGTH + 1] = (eslt_Byte)(keyPairModuleSize);
        }
    }
    return returnValue;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_updateVerifyRSASHA256_V15(       eslt_WorkSpaceRSAver *workSpace,
 **                                     eslt_Length           inputSize,
 **                               const eslt_Byte            *input)
 **
 ** input:
 ** - workSpace
 ** - inputSize
 ** - input
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID
 **      ESL_ERC_SHA256_TOTAL_LENGTH_OVERFLOW
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateVerifyRSASHA256_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   eslt_Length inputSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{
    eslt_ErrorCode returnValue;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ) wsSHA256;

    returnValue = esl_getWorkspaceSHA256(workSpace, &wsSHA256);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
        returnValue = esl_updateSHA256(wsSHA256, inputSize, input);
    }

    return returnValue;
}

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_finalizeHashVerifyRSASHA256_V15(   eslt_WorkSpaceRSAver *workSpace,
 **                                       eslt_Byte            *messageDigest)
 **
 ** input:
 ** - workSpace
 ** output:
 ** - messageDigest
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeHashVerifyRSASHA256_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Byte ) messageDigest )
{

    eslt_ErrorCode            returnValue;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA256 ) wsSHA256;

    returnValue = esl_getWorkspaceSHA256(workSpace, &wsSHA256);

    if (ESL_ERC_NO_ERROR == returnValue)
    {
        returnValue = esl_finalizeSHA256(wsSHA256, messageDigest);
    }

    return returnValue;
}

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_verifySigVerifyRSASHA256_V15(       eslt_WorkSpaceRSAver *workSpace,
 **                                    const eslt_Byte            *messageDigest,
 **                                          eslt_Length           signatureSize,
 **                                    const eslt_Byte            *signature)
 **
 ** input:
 ** - workSpace
 ** - messageDigest
 ** - signatureSize
 ** - signature
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 **      - 0 <= (int)signature <= modul-1, where module = PrimeP*PrimeQ
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_RSA_PUBKEY_INVALID
 **      ESL_ERC_RSA_NO_RSA_WITH_SHA256_SIGNATURE
 **      ESL_ERC_RSA_SIGNATURE_INVALID
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_verifySigVerifyRSASHA256_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   CRYPTOCV_P2CONST_PARA( eslt_Byte ) messageDigest,
   eslt_Length signatureSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) signature )
{

    eslt_ErrorCode returnValue;
    eslt_Length    keyPairModuleSize, i;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                 ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ) wsPRIM;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                 ) messageBuf;

    returnValue = ESL_ERC_NO_ERROR;

    returnValue = esl_getKeyPairModuleSize(workSpace, &keyPairModuleSize);

    if (ESL_ERC_NO_ERROR == returnValue) {
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver_prim ))(wsRAW + ESL_WS_RSA_V15_SV_SHA256_WS_RSA_PRIM(keyPairModuleSize));
        messageBuf        = wsRAW + ESL_WS_RSA_V15_SV_SHA256_BUFFER;

        returnValue = esl_verifyRSA_prim(wsPRIM, signatureSize, signature, &keyPairModuleSize, messageBuf);
        if (ESL_ERC_NO_ERROR == returnValue)
        {
            i = keyPairModuleSize - ESL_SIZEOF_SHA256_DIGEST - sizeof(eslt_ASN1_DIGESTINFO_SHA256);

            if ((signatureSize != keyPairModuleSize) || (ESL_ERC_NO_ERROR != esl_verifyPaddingRSAEM_V15(messageBuf, &i, ASN1_PADDING_BLOCK_TYPE_PRIVATE)))
            {
                returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
            }
            else
            {
                if ( 0 != actMemcmp_ROMRAM( (CRYPTOCV_P2CONST_PARA( actU8 ))&messageBuf[i],
                                            eslt_ASN1_DIGESTINFO_SHA256, sizeof(eslt_ASN1_DIGESTINFO_SHA256) ) )
                {
                   returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
                }
                else
                {
                   i += sizeof(eslt_ASN1_DIGESTINFO_SHA256);

                   if ( 0 != actMemcmp( (CRYPTOCV_P2CONST_PARA( actU8 ))messageDigest,
                                        (CRYPTOCV_P2CONST_PARA( actU8 ))&messageBuf[i], ESL_SIZEOF_SHA256_DIGEST ) )
                   {
                      returnValue = ESL_ERC_RSA_SIGNATURE_INVALID;
                   }
                }
            }
        }
    }

    return returnValue;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_finalizeVerifyRSASHA256_V15(     eslt_WorkSpaceRSAver *workSpace,
 **                                       eslt_Length           signatureSize,
 **                                 const eslt_Byte            *signature)
 **
 ** input:
 ** - workSpace
 ** - signatureSize
 ** - signature
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_RSA_SIGNATURE_OUT_OF_RANGE
 **      - 0 <= (int)signature <= modul-1, where module = PrimeP*PrimeQ
 **      ESL_ERC_RSA_ENCODING_INVALID
 **      ESL_ERC_RSA_PUBKEY_INVALID
 **      ESL_ERC_RSA_NO_RSA_WITH_SHA256_SIGNATURE
 **      ESL_ERC_RSA_SIGNATURE_INVALID
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeVerifyRSASHA256_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSAver ) workSpace,
   eslt_Length signatureSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) signature )
{
    eslt_ErrorCode   returnValue;
    eslt_Byte        hash[ESL_SIZEOF_SHA256_DIGEST];

    returnValue = esl_finalizeHashVerifyRSASHA256_V15( workSpace, hash );
    if ( ESL_ERC_NO_ERROR == returnValue )
    {
        returnValue = esl_verifySigVerifyRSASHA256_V15( workSpace, hash, signatureSize, signature );
    }

    return returnValue;
}

