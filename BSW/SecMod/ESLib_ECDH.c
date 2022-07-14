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
 ** EC-D/H programming interface
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initECDH
 **   esl_generateEphemeralKeyPairECDH
 **   esl_importStaticPrivateKeyECDH
 **   esl_generateSharedSecretECDH
 **
 ***************************************************************************/

#include "ESLib_Config.h"
#include "ESLib.h"
#include "ESLib_types.h"

/* actCLib includes */
#include "actIX25519.h"
#include "actUtilities.h"


/****************************************************************************
 *  LOCAL functions
 ***************************************************************************/
static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateWS(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace);

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateKeyLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length publicKeyLength);

static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateCSLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length sharedSecretLength);


/****************************************************************************
 **
 ** FUNCTION: esl_initECDH
 **
 **   This function initializes the EC-D/H key-exchange.
 **
 ** input:
 ** - workSpace:    algorithm context buffer, initialized by
 **                 esl_initWorkSpaceHeader(..) function
 ** - curve:        underlying curve; supports:
 **                 - ESL_Curve25519
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

CRYPTOCV_FUNC(eslt_ErrorCode) esl_initECDH(CRYPTOCV_P2VAR_PARA(
    eslt_WorkSpace) workSpace,
    eslt_Size16 curve ) {

    /* validate parameters */
    if ((NULL == workSpace))
        return ESL_ERC_PARAMETER_INVALID;


    switch (curve) {

#ifdef  ESL_SUPPORT_Curve25519
    case ESL_Curve25519: {
        eslt_WorkSpaceX25519    *ws = (CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceX25519)) workSpace;

        /* validate work space */
        if (ESL_SIZEOF_WS_X25519 > ws->header.size)
            return ESL_ERC_WS_TOO_SMALL;

        /* initialize actCLib X25519 */
        actX25519Init ((CRYPTOCV_P2VAR_PARA(actX25519Struct)) ws->wsX25519);

        /* set workSpace state */
        workSpace->header.status = (ESL_WST_ALGO_X25519 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
    } break;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_Curve448: {
        eslt_WorkSpaceX448   *ws = (CRYPTOCV_P2VAR_PARA(eslt_WorkSpaceX448)) workSpace;

        /* validate work space */
        if (ESL_SIZEOF_WS_X448 > ws->header.size)
            return ESL_ERC_WS_TOO_SMALL;

        /* initialize actCLib X448 */
/*      actX448Init ((CRYPTOCV_P2VAR_PARA(actX448Struct)) ws->wsX448, preHash, ws->header.watchdog); */

        /* set workSpace state */
        workSpace->header.status = (ESL_WST_ALGO_X448 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
    } break;
#endif

    default:
        return ESL_ERC_CURVE_NOT_SUPPORTED;
    }

    return ESL_ERC_NO_ERROR;
}


/****************************************************************************
 **
 ** FUNCTION: esl_generateEphemeralKeyPairECDH
 **
 **  This function generates an ephemeral key-pair for EC-D/H.
 **
 ** input:
 ** - workSpace:            algorithm context buffer, initialized by
 **                         esl_initWorkSpaceHeader(..) function
 ** - publicKey:            pointer to public key memory
 ** - publicKeyLength:      pointer to public key memory size
 **
 ** output:
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
CRYPTOCV_FUNC(eslt_ErrorCode) esl_generateEphemeralKeyPairECDH(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) publicKey,
  CRYPTOCV_P2VAR_PARA(eslt_Length) publicKeyLength ) {

    eslt_ErrorCode  ec;


    /*  validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == publicKey)     ||  (NULL == publicKeyLength))
        return ESL_ERC_PARAMETER_INVALID;

    if (ESL_ERC_NO_ERROR != (ec = esl_validateKeyLength (workSpace, *publicKeyLength)))
        return ec;

    /*  validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;

    switch (workSpace->header.status & ESL_WST_M_ALGO) {
        actRETURNCODE   rc;

#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_X25519:
        if (actOK == (rc = actX25519GenKeyPair ((CRYPTOCV_P2VAR_PARA(actX25519Struct)) &(workSpace->ws), publicKey, workSpace->header.watchdog))) {
            *publicKeyLength = ESL_SIZEOF_X25519_PUBLIC_KEY;
            return ESL_ERC_NO_ERROR;
        }

        else if (actEXCEPTION_PRIVKEY == rc)
            return ESL_ERC_RNG_FAILED;
        
        return ESL_ERC_UNRECOVERABLE_ERROR;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_X448:
        if (actOK == (rc = actX448GenKeyPair ((CRYPTOCV_P2VAR_PARA(actX448Struct)) &(workSpace->ws), publicKey, workSpace->header.watchdog))) {
            *publicKeyLength  = ESL_SIZEOF_X448_PUBLIC_KEY;
            return ESL_ERC_NO_ERROR;
        }
        
        else if (actEXCEPTION_PUBKEY == rc) {
            *publicKeyLength = ESL_SIZEOF_X448_PUBLIC_KEY;
            return ESL_WARNING_WEAK_KEY;
        }

        else if (actEXCEPTION_PRIVKEY == rc)
            return ESL_ERC_RNG_FAILED;

        return ESL_ERC_UNRECOVERABLE_ERROR;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
    }
}


/****************************************************************************
 **
 ** FUNCTION: esl_importStaticPrivateKeyECDH
 **
 **  This function imports a static private key for EC-D/H.
 **
 ** input:
 ** - workSpace:            algorithm context buffer, initialized by
 **                         esl_initWorkSpaceHeader(..) function
 ** - privateKey:           pointer to private key
 **
 ** output:
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
CRYPTOCV_FUNC(eslt_ErrorCode) esl_importStaticPrivateKeyECDH(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2CONST_PARA(eslt_Byte) privateKey ) {

    eslt_ErrorCode  ec;


    /* validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == privateKey))
        return ESL_ERC_PARAMETER_INVALID;

    /* validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;

    switch (workSpace->header.status & ESL_WST_M_ALGO) {
        actRETURNCODE   rc;

#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_X25519:
        if (actOK == (rc = actX25519ImportPrivateKey  ((CRYPTOCV_P2VAR_PARA(actX25519Struct)) &(workSpace->ws), privateKey, workSpace->header.watchdog)))
            return ESL_ERC_NO_ERROR;

        return ESL_ERC_UNRECOVERABLE_ERROR;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_X448:
        if (actOK == (rc = actX448ImportPrivateKey ((CRYPTOCV_P2VAR_PARA(actX448Struct)) &(workSpace->ws), privateKey, workSpace->header.watchdog)))
            return ESL_ERC_NO_ERROR;
        
        else if (actEXCEPTION_PUBKEY == rc)
            return ESL_WARNING_WEAK_KEY;

        return ESL_ERC_UNRECOVERABLE_ERROR;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler */
    }
}


/****************************************************************************
 **
 ** FUNCTION: esl_generateSharedSecretECDH
 **
 **  This function generates the shared secret EC-D/H.
 **
 ** input:
 ** - workSpace:            algorithm context buffer, initialized by
 **                         esl_initWorkSpaceHeader(..) function
 ** - publicKey:            pointer to public key
 ** - sharedSecret:         pointer to shared secret memory
 ** - sharedSecretLength:   pointer to shared secret memory size
 **
 ** output:
 ** - sharedSecret:         shared secret
 ** - sharedSecretLength:   shared secret length
 ** - returns:              ESL_ERC_PARAMETER_INVALID   some parameter is NULL
 **                         ESL_ERC_WS_STATE_INVALID    work space state invalid
 **                         ESL_ERC_WS_TOO_SMALL        work space too small
 **                         ESL_ERC_BUFFER_TOO_SMALL    buffer for shared secret too small
 **                         ESL_ERC_NO_ERROR            else
 **
 ** assumes:
 ** - workSpace is initialized by esl_initWorkSpaceHeader(..)
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC(eslt_ErrorCode) esl_generateSharedSecretECDH(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) publicKey,
  CRYPTOCV_P2VAR_PARA(eslt_Byte) sharedSecret,
  CRYPTOCV_P2VAR_PARA(eslt_Length) sharedSecretLength ) {

    eslt_ErrorCode  ec;


    /* validate parameters */
    if ((NULL == workSpace)
    ||  (NULL == publicKey)
    ||  (NULL == sharedSecret) || (NULL == sharedSecretLength))
        return ESL_ERC_PARAMETER_INVALID;

    if (ESL_ERC_NO_ERROR != (ec = esl_validateCSLength (workSpace, *sharedSecretLength)))
        return ec;

    /* validate workSpace */
    if (ESL_ERC_NO_ERROR != (ec = esl_validateWS (workSpace)))
        return ec;

    switch (workSpace->header.status & ESL_WST_M_ALGO) {
#ifdef  ESL_SUPPORT_Curve25519
    case ESL_WST_ALGO_X25519:
        actX25519GenSecret ((CRYPTOCV_P2VAR_PARA(actX25519Struct)) &(workSpace->ws), publicKey, sharedSecret, workSpace->header.watchdog);
        *sharedSecretLength = ESL_SIZEOF_X25519_SHARED_SECRET;

        return ESL_ERC_NO_ERROR;
#endif

#ifdef  ESL_SUPPORT_Curve448
    case ESL_WST_ALGO_X448:
        actX448GenSecret ((CRYPTOCV_P2VAR_PARA(actX448Struct)) &(workSpace->ws), publicKey, sharedSecret, workSpace->header.watchdog);
        *sharedSecretLength = ESL_SIZEOF_X448_shared_SECRET;

        return ESL_ERC_NO_ERROR;
#endif

    default:
        return ESL_ERC_WS_STATE_INVALID;        /* has been checked; just soothe the compiler  */
    }
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
		((ESL_WST_ALGO_X25519 != (workSpace->header.status & ESL_WST_M_ALGO)) &&
		(ESL_WST_ALGO_X448 != (workSpace->header.status & ESL_WST_M_ALGO)))
#elif defined(ESL_SUPPORT_Curve25519)
		||
		(ESL_WST_ALGO_X25519 != (workSpace->header.status & ESL_WST_M_ALGO))
#elif defined(ESL_SUPPORT_Curve448)
		||
		(ESL_WST_ALGO_X448 != (workSpace->header.status & ESL_WST_M_ALGO))
#endif	
		)
	{
		return ESL_ERC_WS_STATE_INVALID;
	}

#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_X25519 > workSpace->header.size)) ||
		((ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_X448   > workSpace->header.size))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_X25519 > workSpace->header.size)
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_WS_X448   > workSpace->header.size)
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
  eslt_Length publicKeyLength)
{
	
#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X25519_PUBLIC_KEY > publicKeyLength)) ||
		((ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X448_PUBLIC_KEY   > publicKeyLength))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X25519_PUBLIC_KEY > publicKeyLength)
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X448_PUBLIC_KEY   > publicKeyLength)
# endif
		)
	{
		return ESL_ERC_BUFFER_TOO_SMALL;
	}
#endif

    return ESL_ERC_NO_ERROR;
}


static CRYPTOCV_FUNC(eslt_ErrorCode) esl_validateCSLength(
  CRYPTOCV_P2VAR_PARA(eslt_WorkSpace) workSpace,
  eslt_Length sharedSecretLength)
{
#if (defined(ESL_SUPPORT_Curve25519) || defined(ESL_SUPPORT_Curve448))
	if (
# if (defined(ESL_SUPPORT_Curve25519) && defined(ESL_SUPPORT_Curve448))
		((ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X25519_SHARED_SECRET > sharedSecretLength)) ||
		((ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X448_shared_SECRET   > sharedSecretLength))
# elif defined(ESL_SUPPORT_Curve25519)
		(ESL_WST_ALGO_X25519 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X25519_SHARED_SECRET > sharedSecretLength)
# elif defined(ESL_SUPPORT_Curve448)
		(ESL_WST_ALGO_X448 == (workSpace->header.status & ESL_WST_M_ALGO)) && (ESL_SIZEOF_X448_shared_SECRET   > sharedSecretLength)
# endif
		)
	{
		return ESL_ERC_BUFFER_TOO_SMALL;
	}
#endif

    return ESL_ERC_NO_ERROR;
}
