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
 ** Core Curve25519 implementation.
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
 **   actCurve25519_initCore
 **   actCurve25519_eval
 **
 ***************************************************************************/

#ifndef     actX25519core_h
    #define actX25519core_h

    #include "act25519util.h"

    #include <actIX25519.h>


    #ifdef __cplusplus
        extern "C" {
    #endif


    /* Curve25519 interface */
    extern CRYPTOCV_FUNC( void ) actX25519_initCore (
       CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws );

    extern CRYPTOCV_FUNC( int ) actX25519_eval (
       CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


    #ifdef __cplusplus
        }
    #endif

#endif  /* actX25519core_h */
