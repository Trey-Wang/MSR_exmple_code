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
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** local functions:
 **
 ** global functions:
 **   actEd25519Init
 **   actEd25519Update
 **   actEd25519Sign
 **   actEd25519Verify
 **   actEd25519GenKeyPair
 **
 ***************************************************************************/

#include "actIEd25519.h"
#include "actUtilities.h"
#include "actExternRandom.h"
#include "actConfig.h"
#include "actWatchdog.h"

/*  use cv core */
#include    "actEd25519core.h"

#define     ed25519_initCore(ws)                    actEd25519_initCore (ws)
#define     ed25519_importKeyPair(ws, s, p, wd)     actEd25519_importKeyPair ((ws), (s), (p), (wd))
#define     ed25519_sign(ws, s, wd)                 actEd25519_sign   ((ws), (s), (wd))
#define     ed25519_verify(ws, s, wd)               actEd25519_verify ((ws), (s), (wd))


/****************************************************************************
 ** Types and constants
 ***************************************************************************/
#define     ACTED25519_DOM2LEN                      (32u)

/****************************************************************************
 ** Static data
 ***************************************************************************/
static CRYPTOCV_CONST( CRYPTOCV_NONE, actU8 ) dom2Const[] = "SigEd25519 no Ed25519 collisions";


/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

static CRYPTOCV_FUNC( actRETURNCODE ) importMessage (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) message,
   const int msgLen,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Init (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   const actU8 instance,
   CRYPTOCV_P2CONST_PARA( actU8 ) context,
   const int ctxLen )
{
    if (actEd25519ph == (ws->instance = instance))
    {
        actSHA512Init (&(ws->sha512)); /* no need to check for error - returns actOK anyway */
    }

    ed25519_initCore (ws);
    ws->context = context;
    ws->ctxLen  = ctxLen;

    return actOK;
}


CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Update (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) message,
   const int msgLen,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    if (actEd25519ph != ws->instance)
    {
        return actEXCEPTION_ALGOID;
    }

    return actSHA512Update (&(ws->sha512), message, msgLen, watchdog);
}


CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Sign (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) message,
   const int msgLen,
   CRYPTOCV_P2CONST_PARA( actU8 ) secretKey,
   CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
   CRYPTOCV_P2VAR_PARA( actU8 ) signature,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actRETURNCODE   ec;


    if (actOK != (ec = importMessage (ws, message, msgLen, watchdog)))
    {
        return ec;
    }

    ed25519_importKeyPair (ws, secretKey, publicKey, watchdog);
    ed25519_sign          (ws, signature,            watchdog);

    /*  clear local memory */
    actMemset (ws->privateKey, 0, sizeof (ws->privateKey));
    actMemset (ws->publicKey,  0, sizeof (ws->publicKey));

    actL1trigger (watchdog);

    return actOK;
}


CRYPTOCV_FUNC( actRETURNCODE ) actEd25519Verify (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) message,
   const int msgLen,
   CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
   CRYPTOCV_P2CONST_PARA( actU8 ) signature,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actRETURNCODE   ec;


    if (actOK != (ec = importMessage (ws, message, msgLen, watchdog)))
        return ec;

    ed25519_importKeyPair (ws, NULL, publicKey, watchdog);
    ec = ed25519_verify   (ws, signature,       watchdog);

    /*  clear local memory */
/*  actMemset (ws->privateKey, 0, sizeof (ws->privateKey));   */   /*  no need to clear: holds the signature (re-) calculated */
    actMemset (ws->publicKey,  0, sizeof (ws->publicKey));

    actL1trigger (watchdog);

    return ec;
}


CRYPTOCV_FUNC( actRETURNCODE ) actEd25519GenKeyPair (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2VAR_PARA( actU8 ) secretKey,
   CRYPTOCV_P2VAR_PARA( actU8 ) publicKey,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actRETURNCODE ec;

    if (! actOK == (ec = actGetExternRandom (secretKey, actEd25519_KEY_SIZE)))
    {
        actMemset (secretKey, 0, actEd25519_KEY_SIZE);
        return actEXCEPTION_PRIVKEY;
    }

    ed25519_importKeyPair (ws, secretKey, NULL, watchdog);
    actMemcpy (publicKey, ws->publicKey, actEd25519_KEY_SIZE);

    /*  clear local memory */
    actMemset (ws->privateKey, 0, sizeof (ws->privateKey));
    actMemset (ws->publicKey,  0, sizeof (ws->publicKey));

    actL1trigger (watchdog);

    return actOK;
}


/****************************************************************************
 ** Internal functions
 ***************************************************************************/
static CRYPTOCV_FUNC( void ) act25519_dom2 (
  CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
  CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void))
{
  if (actEd25519pure != ws->instance)
  {
    CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) sha512 = &(ws->sha512);
    actU8 ph_len [2];
    actU8 dom2[ACTED25519_DOM2LEN];

    ph_len [0] = (actEd25519ph == ws->instance) ? 1          : 0;
    ph_len [1] = (ws->context  != NULL)         ? (actU8)ws->ctxLen : 0;

    actMemcpy_ROMRAM(dom2, dom2Const, ACTED25519_DOM2LEN);

    actSHA512Update (sha512, dom2,   ACTED25519_DOM2LEN, watchdog);
    actSHA512Update (sha512, ph_len, 2,       watchdog);

    if (NULL != ws->context
    &&  0    != ws->ctxLen)
    {
      actSHA512Update (sha512, ws->context, ws->ctxLen, watchdog);
    }
  }
}

CRYPTOCV_FUNC( actRETURNCODE ) act25519_hashPM (
  CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
  CRYPTOCV_P2VAR_PARA( actU8 ) hash,
  CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void))
{
  CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) sha512 = &(ws->sha512);

  actSHA512Init     (sha512);
  act25519_dom2     (ws, watchdog);

  actSHA512Update   (sha512, ws->privateKey +32,  32,         watchdog); /* prefix */
  actSHA512Update   (sha512, ws->message,         ws->msgLen, watchdog);
  actSHA512Finalize (sha512, hash,                            watchdog);
  return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) act25519_hashRAM (
  CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
  CRYPTOCV_P2CONST_PARA( actU8 ) R,
  CRYPTOCV_P2VAR_PARA( actU8 ) hash,
  CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void))
{
  CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) sha512 = &(ws->sha512);

  actSHA512Init     (sha512);
  act25519_dom2     (ws, watchdog);

  actSHA512Update   (sha512, R,             32,         watchdog); /* R */
  actSHA512Update   (sha512, ws->publicKey, 32,         watchdog); /* A */
  actSHA512Update   (sha512, ws->message,   ws->msgLen, watchdog);
  actSHA512Finalize (sha512, hash,                      watchdog);
  return actOK;
}

/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/*
 *  import message into work-space
 */
static CRYPTOCV_FUNC( actRETURNCODE ) importMessage (
   CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) message,
   const int msgLen,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    if (actEd25519ph == ws->instance)
	{
        actRETURNCODE ec;

        if (actOK != (ec = actSHA512Update (&(ws->sha512), message, msgLen, watchdog)))
            return ec;

        if (actOK != (ec = actSHA512Finalize (&(ws->sha512), ws->hashValue, watchdog)))
            return ec;

        ws->message = ws->hashValue;
        ws->msgLen  = actHASH_SIZE_SHA512;
    } else {
        ws->message = message;
        ws->msgLen  = msgLen;
    }

    return actOK;
}
