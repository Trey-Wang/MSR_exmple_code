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
 ** Core X25519 implementation.
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
 **   actX25519Init
 **   actX25519GenKeyPair
 **   actX25519GenSecret
 **
 ***************************************************************************/


#include "actIX25519.h"
#include "actUtilities.h"
#include "actExternRandom.h"
#include "actConfig.h"
#include "actWatchdog.h"


/* use cv core */
#include    "actX25519core.h"

#define     x25519_initCore(ws)             actX25519_initCore (ws)
#define     x25519_exchangeKeys(ws, wd)     actX25519_eval ((ws), (wd))



/****************************************************************************
 ** Types and constants
 ***************************************************************************/


/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


CRYPTOCV_FUNC( actRETURNCODE ) actX25519Init (
   CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws )
{
    x25519_initCore (ws);

    return actOK;
}


CRYPTOCV_FUNC( actRETURNCODE ) actX25519GenKeyPair (
   CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
   CRYPTOCV_P2VAR_PARA( actU8 ) publicKey,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    /* generate random ephemeral private key */
    if (actOK != actGetExternRandom (ws->privateKey, actX25519_KEY_SIZE)) {
        actMemset (ws->privateKey, 0, actX25519_KEY_SIZE);
        return actEXCEPTION_PRIVKEY;
    }
    ws->privateKey  [0] &= 0xF8;
    ws->privateKey [31] &= 0x7F;
    ws->privateKey [31] |= 0x40;

    /* evaluate curve equation using base point and ephemeral privat key */
    actMemset (ws->uCoord, 0, actX25519_KEY_SIZE);
    ws->uCoord [0] = 9;    /* base point = 9 */
    x25519_exchangeKeys (ws, watchdog);

    actMemcpy (publicKey, ws->uCoord, actX25519_KEY_SIZE);
    return actOK;     
}


CRYPTOCV_FUNC( actRETURNCODE ) actX25519ImportPrivateKey (
   CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) privateKey,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy (ws->privateKey, privateKey, actX25519_KEY_SIZE);
    ws->privateKey  [0] &= 0xF8;
    ws->privateKey [31] &= 0x7F;
    ws->privateKey [31] |= 0x40;

    return actOK;     
}


CRYPTOCV_FUNC(actRETURNCODE ) actX25519GenSecret (
   CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
   CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
   CRYPTOCV_P2VAR_PARA( actU8 ) sharedSecret,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy (ws->uCoord, publicKey, actX25519_KEY_SIZE);
    x25519_exchangeKeys (ws, watchdog);

    actMemcpy (sharedSecret, ws->uCoord, actX25519_KEY_SIZE);
    return actOK;
}
