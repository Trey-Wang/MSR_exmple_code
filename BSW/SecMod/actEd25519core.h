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
 **   actEd25519_initCore
 **   actEd25519_importKeyPair
 **   actEd25519_sign
 **   actEd25519_verify
 **
 ***************************************************************************/
#ifndef     actEd25519core_h
    #define actEd25519core_h

    #include "act25519util.h"

    #include <actIEd25519.h>


    #ifdef __cplusplus
        extern "C" {
    #endif


    /*  Ed25519 interface */
    extern CRYPTOCV_FUNC( void ) actEd25519_initCore (
       CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws );

    extern CRYPTOCV_FUNC( void ) actEd25519_importKeyPair (
       CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
       CRYPTOCV_P2CONST_PARA( actU8 ) secretKey,
       CRYPTOCV_P2CONST_PARA( actU8 ) publicKey,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) actEd25519_sign (
       CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
       CRYPTOCV_P2VAR_PARA( actU8 ) signature,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( int ) actEd25519_verify (
       CRYPTOCV_P2VAR_PARA( actEd25519Struct ) ws,
       CRYPTOCV_P2CONST_PARA( actU8 ) signature,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


    #ifdef __cplusplus
        }
    #endif

#endif  /*  actEd25519core_h */
