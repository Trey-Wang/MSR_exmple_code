/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/testBench/source/testSHA512-256.c $
 ** $Revision: 2198 $
 ** $Date: 2016-01-13 08:46:05 +0100 (Mi, 13 Jan 2016) $
 ** $Author: mneuhaus $
 **
 ** \copyright(cv cryptovision GmbH, 2016)
 **
 ** \version(cv library/es 1.2)
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** EdDSA programming interface
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initSignEdDSA
 **   esl_signEdDSA
 **   esl_initVerifyEdDSA
 **   esl_verifyEdDSA
 **   esl_updateEdDSA
 **   esl_initGenerateKeyPairEdDSA
 **   esl_generateKeyPairEdDSA
 **
 ***************************************************************************/

#include "ESLib_Config.h"
#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIEd25519.h"
#include "actUtilities.h"

#define     _INSTANCE_KeyGen    0x55

/****************************************************************************
 *  LOCAL functions
 ***************************************************************************/
static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateWS(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace);

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateKeyLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length secretKeyLength,
  eslt_Length publicKeyLength);

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateSigLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length signatureLength);

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_initEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  const eslt_Size16 curve,
  const eslt_Byte instance,
   CRYPTOCV_P2CONST_PARA(eslt_Byte) context,
   const eslt_Length contextLength);


/****************************************************************************
 **
 ** FUNCTION: esl_initSignEdDSA
 **
 **  This function initializes the EdDSA signature.
 **
 ** input:
 ** - workSpace:        algorithm context buffer, initialized by
 **                     esl_initWorkSpaceHeader(..) function
 ** - curve:            underlying curve; supports:
 **                     - ESL_Curve25519
 ** - instance:         EdDSA instance to use; one of:
 **                     - ESL_INSTANCE_Ed25519
 **                     - ESL_INSTANCE_Ed25519ctx
 **                     - ESL_INSTANCE_Ed25519ph
 ** - context:          context data
 ** - contextLength:    the length of the context data
 **
 ** output:
 ** - returns:          ESL_ERC_PARAMETER_INVALID       some parameter is NULL
 **                     ESL_ERC_CURVE_NOT_SUPPORTED     unknown curve
 **                     ESL_ERC_INSTANCE_NOT_SUPPORTED  unknown EdDSA instance
 **                     ESL_ERC_WS_TOO_SMALL            work space too small
 **                     ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_initSignEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Size16 curve,
  const eslt_Byte instance,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength)
{

    /* validate parameters */
    if ((NULL == workSpace))
        return ESL_ERC_PARAMETER_INVALID;

    return esl_initEdDSA (workSpace, curve, instance, context, contextLength);
}


/****************************************************************************
 **
 ** FUNCTION: esl_signEdDSA
 **
 **  This function signs the given input data.
 **
 ** input:
 ** - workSpace:        algorithm context buffer, initialized by
 **                     esl_initWorkSpaceHeader(..) function
 ** - input:            input data
 ** - inputLength:      the length of the input data
 ** - secretKey:        secret key to use for signature
 ** - publicKey:        public key to use for signature (optional!)
 ** - signature:        pointer to signature memory
 ** - signatureLength:  pointer to signature memory size
 **
 ** output:
 ** - signatureLength:  signature length
 ** - returns:          ESL_ERC_PARAMETER_INVALID       some parameter is NULL
 **                     ESL_ERC_WS_STATE_INVALID        work space state invalid
 **                     ESL_ERC_WS_TOO_SMALL            work space too small
 **                     ESL_ERC_BUFFER_TOO_SMALL        buffer for signature too small
 **                     ESL_ERC_TOTAL_LENGTH_OVERFLOW   internal overflow (== message too long - SHA-512)
 **                     ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_signEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) secretKey,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) publicKey,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) signature,
  CRYPTOCV_P2VAR_PARA(eslt_Length) signatureLength )
{

    eslt_ErrorCode  ec;


    /* validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == input)
    ||  (NULL == secretKey)
    ||  (NULL == signature) || (NULL == signatureLength))
        return ESL_ERC_PARAMETER_INVALID;

    if (ESL_ERC_NO_ERROR != (ec = esl_validateSigLength (workSpace, *signatureLength)))
        return ESL_ERC_BUFFER_TOO_SMALL;

    /* validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;

    switch (workSpace->header.status & ESL_WST_M_ALGO) {
#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_Ed25519:
        if (actEXCEPTION_LENGTH == (ec = actEd25519Sign ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) &(workSpace->ws), input, inputLength, secretKey, publicKey, signature, workSpace->header.watchdog)))
            /* SHA-512 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actOK != ec)
            return ESL_ERC_ERROR;

        *signatureLength = ESL_SIZEOF_Ed25519_SIGNATURE;
        break;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_Ed448:
        if (actEXCEPTION_LENGTH == (ec = actEd448Sign ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) &(workSpace->ws), input, inputLength, secretKey, publicKey, signature, workSpace->header.watchdog)))
            /* Shake-256 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actOK != ec)
            return ESL_ERC_ERROR;

        *signatureLength = ESL_SIZEOF_EdDSA448_SIGNATURE;
        break;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
    }

    return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_initVerifyEdDSA
 **
 **  This function initializes the EdDSA verification.
 **
 ** input:
 ** - workSpace:        algorithm context buffer, initialized by
 **                     esl_initWorkSpaceHeader(..) function
 ** - curve:            underlying curve; supports:
 **                     - ESL_Curve25519
 ** - instance:         EdDSA instance to use; one of:
 **                     - ESL_INSTANCE_Ed25519
 **                     - ESL_INSTANCE_Ed25519ctx
 **                     - ESL_INSTANCE_Ed25519ph
 ** - context:          context data
 ** - contextLength:    the length of the context data
 **
 ** output:
 ** - returns:          ESL_ERC_PARAMETER_INVALID       some parameter is NULL
 **                     ESL_ERC_CURVE_NOT_SUPPORTED     unknown curve
 **                     ESL_ERC_INSTANCE_NOT_SUPPORTED  unknown EdDSA instance
 **                     ESL_ERC_WS_TOO_SMALL            work space too small
 **                     ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_initVerifyEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Size16 curve,
  const eslt_Byte instance, 
  CRYPTOCV_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength)
{

    /* validate parameters */
    if ((NULL == workSpace))
        return ESL_ERC_PARAMETER_INVALID;

    return esl_initEdDSA (workSpace, curve, instance, context, contextLength);
}


/****************************************************************************
 **
 ** FUNCTION: esl_verifyEdDSA
 **
 **  This function verifies given signature against the given data.
 **
 ** input:
 ** - workSpace:        algorithm context buffer, initialized by
 **                     esl_initWorkSpaceHeader(..) function
 ** - input:            input data
 ** - inputLength:      the length of the input data
 ** - publicKey:        public key to use for verification
 ** - signature:        signature
 ** - signatureLength:  the length of the signature
 **
 ** output:
 ** - returns:          ESL_ERC_PARAMETER_INVALID       some parameter is NULL
 **                     ESL_ERC_WS_STATE_INVALID        work space state invalid
 **                     ESL_ERC_WS_TOO_SMALL            work space too small
 **                     ESL_ERC_TOTAL_LENGTH_OVERFLOW   internal overflow (== message too long - SHA-512)
 **                     ESL_ERC_SIGNATURE_INVALID       the signature does NOT verify
 **                     ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_verifyEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) publicKey,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) signature,
  eslt_Length signatureLength ) {

    eslt_ErrorCode  ec;


    /* validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == input)
    ||  (NULL == publicKey)
    ||  (NULL == signature))
	{
        return ESL_ERC_PARAMETER_INVALID;
    }

    if (ESL_ERC_NO_ERROR != (ec = esl_validateSigLength (workSpace, signatureLength)))
    {
        return ESL_ERC_PARAMETER_INVALID;
    }

    /* validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;


    switch (workSpace->header.status & ESL_WST_M_ALGO) {
#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_Ed25519:
        if (actEXCEPTION_LENGTH == (ec = actEd25519Verify ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) &(workSpace->ws), input, inputLength, publicKey, signature, workSpace->header.watchdog)))
            /* SHA-512 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actOK != ec)
            /* treat ANY other error as "signature invalid" */
            return ESL_ERC_SIGNATURE_INVALID;
        break;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_Ed448:
        if (actEXCEPTION_LENGTH == (ec = actEd448Verify ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) &(workSpace->ws), input, inputLength, publicKey, signature, workSpace->header.watchdog)))
            /* Shake-256 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actOK != ec)
            /* treat ANY other error as "signature invalid" */
            return ESL_ERC_SIGNATURE_INVALID;
        break;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
    }

    return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_updateEdDSA
 **
 **  This function updates the internal hash value in a pre-hash EdDSA
 **  signature OR verification.
 **
 ** input:
 ** - workSpace:    algorithm context buffer, initialized by
 **                 esl_initWorkSpaceHeader(..) function
 ** - input:        input data
 ** - inputLength:  the length of the input data
 **
 ** output:
 ** - returns:      ESL_ERC_PARAMETER_INVALID       some parameter is NULL
 **                 ESL_ERC_WS_STATE_INVALID        work space state invalid
 **                 ESL_ERC_WS_TOO_SMALL            work space too small
 **                 ESL_ERC_TOTAL_LENGTH_OVERFLOW   internal overflow (== message too long - SHA-512)
 **                 ESL_ERC_NO_ERROR                else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_updateEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) input,
  eslt_Length inputLength ) {

    eslt_ErrorCode  ec;


    /*  validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == input))
        return ESL_ERC_PARAMETER_INVALID;

    /*  validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;


    switch (workSpace->header.status & ESL_WST_M_ALGO) {
#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_Ed25519:
        if (actEXCEPTION_LENGTH == (ec = actEd25519Update ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) &(workSpace->ws), input, inputLength, workSpace->header.watchdog)))
            /* SHA-512 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actEXCEPTION_ALGOID == ec)
            return ESL_ERC_WS_STATE_INVALID;
        break;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_Ed448:
        if (actEXCEPTION_LENGTH == (ec = actEd448Update ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) &(workSpace->ws), input, inputLength, workSpace->header.watchdog)))
            /* Shake-256 length overflow */
            return ESL_ERC_TOTAL_LENGTH_OVERFLOW;
        else if (actEXCEPTION_ALGOID == ec)
            return ESL_ERC_WS_STATE_INVALID;
        break;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
    }

    return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_initGenerateKeyPairEdDSA
 **
 **  This function initializes the EdDSA key-pair generation.
 **
 ** input:
 ** - workSpace:    algorithm context buffer, initialized by
 **                 esl_initWorkSpaceHeader(..) function
 **
 ** output:
 ** - returns:      ESL_ERC_PARAMETER_INVALID     some parameter is NULL
 **                 ESL_ERC_CURVE_NOT_SUPPORTED   unknown curve
 **                 ESL_ERC_WS_TOO_SMALL          work space too small
 **                 ESL_ERC_NO_ERROR              else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_initGenerateKeyPairEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Size16 curve ) {

    /* validate parameters */
    if (NULL == workSpace)
        return ESL_ERC_PARAMETER_INVALID;

    return esl_initEdDSA (workSpace, curve, _INSTANCE_KeyGen, NULL, 0);
}


/****************************************************************************
 **
 ** FUNCTION: esl_generateKeyPairEdDSA
 **
 **  This function generates an EdDSA key-pair.
 **
 ** input:
 ** - workSpace:            algorithm context buffer, initialized by
 **                         esl_initWorkSpaceHeader(..) function
 ** - secretKey:            pointer to secret key memory
 ** - secretKeyLength:      pointer to secret key memory size
 ** - publicKey:            pointer to public key memory
 ** - publicKeyLength:      pointer to public key memory size
 **
 ** output:
 ** - secretKey:            secret key
 ** - secretKeyLength:      secret key length
 ** - publicKey:            public key
 ** - publicKeyLength:      public key length
 ** - returns:              ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 **                         ESL_ERC_WS_STATE_INVALID    work space state invalid
 **                         ESL_ERC_WS_TOO_SMALL        work space too small
 **                         ESL_ERC_BUFFER_TOO_SMALL    buffer for private or public key too small
 **                         ESL_ERC_RNG_FAILED          the random number generator failed
 **                         ESL_ERC_NO_ERROR            else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_generateKeyPairEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) secretKey,
  CRYPTOCV_P2VAR_PARA(eslt_Length) secretKeyLength,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) publicKey,
  CRYPTOCV_P2VAR_PARA(eslt_Length) publicKeyLength ) {
  eslt_ErrorCode retVal = ESL_ERC_NO_ERROR;
  eslt_ErrorCode  ec;
  actRETURNCODE   rc;


    /* validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == secretKey)     ||  (NULL == secretKeyLength)
    ||  (NULL == publicKey)     ||  (NULL == publicKeyLength))
    {
        retVal = ESL_ERC_PARAMETER_INVALID;
    }
    else if (ESL_ERC_NO_ERROR != (ec = esl_validateKeyLength (workSpace, *secretKeyLength, *publicKeyLength)))
    {
        retVal = ec;
    }
    /* validate workSpace */
    else if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
    {
        retVal = ec;
    }
    else
    {

        switch (workSpace->header.status & ESL_WST_M_ALGO)
        {
#ifdef  ESL_SUPPORT_Curve25519
            case ESL_WST_ALGO_Ed25519:
                if (actOK == (rc = actEd25519GenKeyPair ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) &(workSpace->ws), secretKey, publicKey, workSpace->header.watchdog)))
                {
                    *secretKeyLength = ESL_SIZEOF_Ed25519_SECRET_KEY;
                    *publicKeyLength = ESL_SIZEOF_Ed25519_PUBLIC_KEY;
                    retVal = ESL_ERC_NO_ERROR;
                }
                else
                {
                    retVal = ESL_ERC_RNG_FAILED;
                }
                break;
#endif

#ifdef  ESL_SUPPORT_Curve448
            case ESL_WST_ALGO_Ed448:
                if (actOK == (rc = actEd448GenKeyPair ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) &(workSpace->ws), secretKey, publicKey, workSpace->header.watchdog)))
                {
                    *secretKeyLength = ESL_SIZEOF_EdDSA448_SECRET_KEY;
                    *publicKeyLength = ESL_SIZEOF_EdDS448_PUBLIC_KEY;
                    retVal = ESL_ERC_NO_ERROR;
                }
                else
                {
                    retVal = ESL_ERC_RNG_FAILED;
                }
                break;
#endif

            default:
                retVal = ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
                break;
        }
    }

    return retVal;
}



/****************************************************************************
 *  LOCAL functions
 ***************************************************************************/

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateWS(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace)
{
	if (
		(ESL_WST_M_RUNNING != (workSpace->header.status & ESL_WST_M_RUNNING))
#if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		||
		((ESL_WST_ALGO_Ed25519 != (workSpace->header.status & ESL_WST_M_ALGO)) &&
		(ESL_WST_ALGO_Ed448 != (workSpace->header.status & ESL_WST_M_ALGO)))
#elif defined(ESL_SUPPORT_Curve25519)
		||
		(ESL_WST_ALGO_Ed25519 != (workSpace->header.status & ESL_WST_M_ALGO))
#elif defined(ESL_SUPPORT_Curve448)
		||
		(ESL_WST_ALGO_Ed448 != (workSpace->header.status & ESL_WST_M_ALGO))
#endif	
		)
	{
		return ESL_ERC_WS_STATE_INVALID;
	}

#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_Ed25519 > workSpace->header.size)) ||
		((ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_Ed448   > workSpace->header.size))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_Ed25519 > workSpace->header.size)
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_Ed448   > workSpace->header.size)
# endif
		)
	{
		return ESL_ERC_WS_TOO_SMALL;
	}
#endif

    return ESL_ERC_NO_ERROR;
}


static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateKeyLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length secretKeyLength,
  eslt_Length publicKeyLength)
{

#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && ((ESL_SIZEOF_Ed25519_SECRET_KEY > secretKeyLength) || (ESL_SIZEOF_Ed25519_PUBLIC_KEY > publicKeyLength))) ||
		((ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && ((ESL_SIZEOF_Ed448_SECRET_KEY   > secretKeyLength) || (ESL_SIZEOF_EdDSA448_PUBLIC_KEY   > publicKeyLength)))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && ((ESL_SIZEOF_Ed25519_SECRET_KEY > secretKeyLength) || (ESL_SIZEOF_Ed25519_PUBLIC_KEY > publicKeyLength))
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && ((ESL_SIZEOF_Ed448_SECRET_KEY   > secretKeyLength) || (ESL_SIZEOF_EdDSA448_PUBLIC_KEY   > publicKeyLength))
# endif
		)
	{
		return ESL_ERC_BUFFER_TOO_SMALL;
	}
#endif

    return ESL_ERC_NO_ERROR;
}


static CRYPTOCV_FUNC( eslt_ErrorCode ) esl_validateSigLength (
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpace ) workSpace,
   eslt_Length signatureLength )
{
#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_Ed25519_SIGNATURE > signatureLength)) ||
		((ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_Ed448_SIGNATURE   > signatureLength))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_Ed25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_Ed25519_SIGNATURE > signatureLength)
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_Ed448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_Ed448_SIGNATURE   > signatureLength)
# endif
		)
	{
		return ESL_ERC_BUFFER_TOO_SMALL;
	}
#endif

    return ESL_ERC_NO_ERROR;
}


static CRYPTOCV_FUNC(eslt_ErrorCode) esl_initEdDSA(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  const eslt_Size16 curve,
  const eslt_Byte instance,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength)
{
    switch (curve) {

#ifdef  ESL_SUPPORT_Curve25519
    case ESL_Curve25519: {
      CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceEd25519) ws = (CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceEd25519)) workSpace;

        /* validate work space */
        if (ESL_SIZEOF_WS_Ed25519 > ws->header.size)
            return ESL_ERC_WS_TOO_SMALL;

        if (_INSTANCE_KeyGen == instance) {
        /* initialize actCLib Ed25519 */
        actEd25519Init ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) ws->wsEd25519, ESL_INSTANCE_Ed25519, NULL, 0);
        }
        else
        {

            /* validate instance */
            if (ESL_INSTANCE_Ed25519    != instance
            &&  ESL_INSTANCE_Ed25519ctx != instance
            &&  ESL_INSTANCE_Ed25519ph  != instance)
                return ESL_ERC_INSTANCE_NOT_SUPPORTED;

            /* validate context */
            if (ESL_INSTANCE_Ed25519 != instance
            &&  context              != NULL
            &&  contextLength         > 255)
                return ESL_ERC_PARAMETER_INVALID;
            /* initialize actCLib Ed25519 */
            actEd25519Init ((CRYPTOCV_P2VAR_PARA(actEd25519Struct)) ws->wsEd25519, instance, context, contextLength);
        }
        /* set workSpace state */
        workSpace->header.status = (ESL_WST_ALGO_Ed25519 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
    } break;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_Curve448: {
      CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceEd448) ws = (CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceEd448)) workSpace;

        /* validate work space */
        if (ESL_SIZEOF_WS_Ed448 > ws->header.size)
            return ESL_ERC_WS_TOO_SMALL;


        if (_INSTANCE_KeyGen == instance)
        {
            /* no more parameter checking!! */

            /* initialize actCLib Ed25519 */
            actEd488Init ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) ws->wsEd488, ESL_INSTANCE_Ed488, context, contextLength);
        } else {

            /* validate instance */
            if (ESL_INSTANCE_Ed488   != instance
            &&  ESL_INSTANCE_Ed488ph != instance)
                return ESL_ERC_INSTANCE_NOT_SUPPORTED;

            /* validate context */
            if (context              != NULL
            &&  contextLength         > 255)
                return ESL_ERC_PARAMETER_INVALID;
            /* initialize actCLib Ed448 */
            actEd448Init ((CRYPTOCV_P2VAR_PARA(actEd448Struct)) ws->wsEd448, instance, ws->header.watchdog);
        }

        /* set workSpace state */
        workSpace->header.status = (ESL_WST_ALGO_Ed448 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
    } break;
#endif

    default:
        return ESL_ERC_CURVE_NOT_SUPPORTED;
    }

    return ESL_ERC_NO_ERROR;
}
