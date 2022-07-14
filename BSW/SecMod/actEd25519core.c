/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/actCLib/source/actSHA-224-256.c $
 ** $Revision: 2170 $
 ** $Date: 2015-12-18 11:39:26 +0100 (Fr, 18 Dez 2015) $
 ** $Author: mneuhaus $
 **
 ** \copyright(cv cryptovision GmbH, 1999-2015)
 **
 ** \version(cv library/es 1.2)
 **
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: Core Module
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** Core Ed25519 implementation.
 **
 ** reference:
 **   https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** local functions:
 **
 ** global functions:
 **   actEd25519_initCore
 **   actEd25519_importKeyPair
 **   actEd25519_sign
 **   actEd25519_verify
 **
 ***************************************************************************/

#include  "actEd25519core.h"
#include  "actWatchdog.h"
#include  "actUtilities.h"
#include  "actISHA2_64.h"
#include  "actEd25519.h"


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** void actEd25519_initCore (actEd25519Struct *ws))
 **
 **  This function initializes the internal data structures.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 **
 ** output:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actEd25519_initCore (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws )
{
    act25519_initCore ((CRYPTOCV_P2VAR_PARA(actCurve25519ws)) ws->coreWS);
}


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** void actEd25519_sign (actEd25519Struct *ws, actU8 *signature, void (* watchdog) (void))
 **
 **  This function creates a signature over the data.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 ** - signature:    pointer to the signature buffer (64 bytes)
 ** - watchdog:     pointer to watchdog reset function
 **
 ** output:
 ** - signature:    signature
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actEd25519_sign (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2VAR_PARA( actU8 ) signature,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    CRYPTOCV_P2VAR_PARA( actCurve25519ws ) coreWS = (CRYPTOCV_P2VAR_PARA(actCurve25519ws)) ws->coreWS;

    actBNDIGIT  r [BNDIGITS_256];
    #define     a    ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempA))
    #define     k    ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempB))
    #define     s    ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempC))
    #define     rB   ((CRYPTOCV_P2VAR_PARA(Point))      &(ws->sha512))         /* need 128 bytes - actSHA512Struct is 212 bytes */
    #define     hash ((CRYPTOCV_P2VAR_PARA(actU8))      &(coreWS->tempA))      /* need  64 bytes - temA || tempB   is  72 bytes */


    /* hash the prefix and the message to derive r */
    act25519_hashPM   (ws,            hash,       watchdog);
    actBNLoadLE       (coreWS->temp1, hash, actHASH_SIZE_SHA512);
    act25519_reduceQ  (coreWS, r, coreWS->temp1);                           /* r = hash mod Q */

    /* encode rB into the first half of the signature buffer as R */
    act25519_mulBasePoint (coreWS, rB,        r,  watchdog);
    act25519_encodePoint  (coreWS, signature, rB, watchdog);                /* R */

    /* hash R, A, and the message to get k */
    act25519_hashRAM  (ws, signature, ws->hashValue, watchdog);

    actBNLoadLE       (coreWS->temp1, ws->hashValue, actHASH_SIZE_SHA512);
    act25519_reduceQ  (coreWS, k, coreWS->temp1);                           /* k = hash mod Q */

    /* compute s = (r + k * a) mod q */
    actBNLoadLE (a, ws->privateKey, 32);                                    /* private scalar -> a */
    act25519_mulModQ  (coreWS, s, k, a);                                    /* s =     (k * a) mod q */
    act25519_addModQ  (coreWS, s, r, s);                                    /* s = (r + k * a) mod q */

    actBNStoreLE (signature + 32, s, BNDIGITS_256);

    #undef      a
    #undef      k
    #undef      s
    #undef      rB
    #undef      hash

    /* clean up */
    actL2trigger (watchdog);    /* 1 reduction + 1 multiplication + 1 addition */
    clearTemp (coreWS);
}


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** int actEd25519_verify (actEd25519Struct *ws, const actU8 *signature, void (* watchdog) (void))
 **
 **  This function verifies a signature over the data.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 ** - signature:    pointer to the signature
 ** - watchdog:     pointer to watchdog reset function
 **
 ** output:
 ** - returns:      actOK                   if signature verifies
 **                 actVERIFICATION_FAILED  otherwise
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actEd25519_verify (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) signature,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    CRYPTOCV_P2VAR_PARA( actCurve25519ws ) coreWS = (CRYPTOCV_P2VAR_PARA(actCurve25519ws)) ws->coreWS;

    Point       R;
    #define     A   ((CRYPTOCV_P2VAR_PARA(Point))      &(ws->sha512))      /* need 128 bytes - actSHA512Struct is 212 bytes */
    #define     sB  ((CRYPTOCV_P2VAR_PARA(Point))      &(ws->sha512))      /* sB / A are NEVER used concurrently! */
    #define     s   ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(ws->hashValue))   /* need  32 bytes - hashValue       is  64 bytes */
    #define     k   ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(ws->hashValue))   /* k / s  are NEVER used concurrently! */

    int         result = actVERIFICATION_FAILED;


    /* Reconstruct the k value from the signing step. */
    act25519_hashRAM(ws, signature, ws->hashValue, watchdog);

    /* decode the public key and the R component of the signature */
    if (act25519_decodePoint (coreWS,  A, ws->publicKey, watchdog)
    &&  act25519_decodePoint (coreWS, &R, signature,     watchdog)) {
        /* calculate R + k * A. */
        actBNLoadLE          (coreWS->temp1, ws->hashValue, actHASH_SIZE_SHA512);
        act25519_reduceQ     (coreWS, k, coreWS->temp1);                /* k = hash mod Q */
        act25519_mulAddPoint (coreWS, &R, A, k, watchdog);              /* R = R + k * A */

        /* calculate s * B */
        actBNLoadLE           (s, signature + 32, 32);
        act25519_mulBasePoint (coreWS, sB, s, watchdog);                /* sB = s * B */

        /* compare s * B and R + k * A for equality */
        result = act25519_comparePoints (coreWS, sB, &R, watchdog) ? actOK : actVERIFICATION_FAILED;
    }

    #undef      A
    #undef      sB
    #undef      s
    #undef      k

    /* clean up and exit */
    /* actL2trigger (watchdog); */    /* decodePoint / comparePoints DID reset the watchdog */
    clearTemp (coreWS);
    return result;
}


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** void actEd25519_importKeyPair (actEd25519Struct *ws, const actU8 *secretKey, const actU8 *publicKey, void (* watchdog) (void))
 **
 **   This function imports the secret and / or public key for signature / verification.
 **   The private scalar is derived from the secret key, and when no public key is passed, that is calculated from the private scalar.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 ** - secretKey:    pointer to the secret key (32 bytes)
 ** - publicKey:    pointer to the public key (32 bytes)
 ** - watchdog:     pointer to watchdog reset function
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actEd25519_importKeyPair (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) secretKey,
   CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    if (NULL != secretKey) {
        /* Hash the secret key to get the private scalar "a" and the message prefix. */
        actSHA512Init     (&(ws->sha512));
        actSHA512Update   (&(ws->sha512), secretKey,      actEd25519_KEY_SIZE, watchdog);
        actSHA512Finalize (&(ws->sha512), ws->privateKey,                      watchdog);

        ws->privateKey  [0] &= 0xF8;
        ws->privateKey [31] &= 0x7F;
        ws->privateKey [31] |= 0x40;
    }

    if (NULL != publicKey) {
        /* copy public key */

        actMemcpy (ws->publicKey, publicKey, actEd25519_KEY_SIZE);
    } else {
        /*  calculate public key from private key
         *  to conserve (re-use) memory:
         *  - A is overlayed onto the SHA-512 work-space
         *  - a is overlayed onto the public key
         */

        CRYPTOCV_P2VAR_PARA(actCurve25519ws) coreWS    = (CRYPTOCV_P2VAR_PARA(actCurve25519ws)) ws->coreWS;

        #define     A   ((CRYPTOCV_P2VAR_PARA(Point))      &(ws->sha512))      /* need 128 bytes - actSHA512Struct is 212 bytes */
        #define     a   ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(ws->publicKey))   /* need  32 bytes - publicKey       is  32 bytes */


        /* compute  (a * BP)  and encode the point (-> public key) */
        actBNLoadLE           (a, ws->privateKey, actEd25519_KEY_SIZE);
        act25519_mulBasePoint (coreWS, A,             a, watchdog);
        act25519_encodePoint  (coreWS, ws->publicKey, A, watchdog);

        #undef      A
        #undef      a

        /* clean up */
        /* actL2trigger (watchdog); */   /*  encodePoint DID reset the watchdog */
        clearTemp (coreWS);
    }
}
