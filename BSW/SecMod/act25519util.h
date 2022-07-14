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
 ** Curve25519 utilities
 **
 ** constants:
 **   25519 curve data
 **
 ** types:
 **   Point
 **   work space
 **
 ** macros:
 **
 ** functions:
 **   actBNLoadLE
 **   actBNStoreLE
 **
 **   act25519_initCore
 **
 **   act25519_reduceP
 **   act25519_reduceQ
 **
 **   act25519_addModP
 **   act25519_subModP
 **   act25519_mulModP
 **   act25519_powModP
 **   act25519_invModP
 **   act25519_sqrtModP  
 **
 **   act25519_addModQ
 **   act25519_mulModQ
 **
 **   act25519_encodePoint
 **   act25519_decodePoint
 **
 **   act25519_addPoint
 **   act25519_mulPoint
 **   act25519_mulAddPoint
 **   act25519_mulBasePoint
 **   act25519_comparePoints
 **
 ***************************************************************************/
#ifndef     act25519util_h
    #define act25519util_h

    #include <actBigNum.h>


    #ifdef __cplusplus
        extern "C" {
    #endif


    #define BNDIGITS_256    (256 /8 /sizeof (actBNDIGIT))
    #define BNDIGITS_512    (512 /8 /sizeof (actBNDIGIT))
    #define BNBYTES_256     (256 /8 /sizeof (actU8))
    #define BNBYTES_512     (512 /8 /sizeof (actU8))
    #define MASK_MSBit      (0x7FFFFFFF >> (32 - actBN_BITS_PER_DIGIT))

    /* Curve point represented in extended homogeneous coordinates. */
    typedef struct {
        actBNDIGIT  x [BNDIGITS_256];
        actBNDIGIT  y [BNDIGITS_256];
        actBNDIGIT  z [BNDIGITS_256];
        actBNDIGIT  t [BNDIGITS_256];
    } Point;


    typedef struct {
        actBNDIGIT  prime   [BNDIGITS_256];
        actBNDIGIT  order   [BNDIGITS_256];
        actBNDIGIT  orderB  [BNDIGITS_256 +1];  /* 2^512 / q (order) */

        actBNRING   pRing;
        actBNRING   qRing;

        actBNDIGIT  tempA   [BNDIGITS_256];     /* used in actEd25519* functions */
        actBNDIGIT  tempB   [BNDIGITS_256];
        actBNDIGIT  tempC   [BNDIGITS_256];
        actBNDIGIT  tempD   [BNDIGITS_256];
        actBNDIGIT  tempE   [BNDIGITS_256];     /* used in actCurve25519* functions */
        actBNDIGIT  tempF   [BNDIGITS_256];
        actBNDIGIT  tempG   [BNDIGITS_256];
        actBNDIGIT  tempH   [BNDIGITS_256];
        actBNDIGIT  temp1   [BNDIGITS_512];     /* 512 bit temp variable (used for multiplication) */
        actBNDIGIT  tempQ   [BNDIGITS_256];     /* 256 bit temp variable (used for reduction) */
    } actCurve25519ws;      /* size <= 520 bytes (64bit pointers) */


    /* expand a 32-bit value into a set of actBNDIGITs depending upon the actBNDIGIT size */
    #if (8 == actBN_BITS_PER_DIGIT)
        #define DIGITs(value)   ((actBNDIGIT) ((value) >>  0)), \
                                ((actBNDIGIT) ((value) >>  8)), \
                                ((actBNDIGIT) ((value) >> 16)), \
                                ((actBNDIGIT) ((value) >> 24))
    #elif (16 == actBN_BITS_PER_DIGIT)
        #define DIGITs(value)   ((actBNDIGIT) ((value) >>  0)), \
                                ((actBNDIGIT) ((value) >> 16))
    #elif (32 == actBN_BITS_PER_DIGIT)
        #define DIGITs(value)   (value)
    #endif

    CRYPTOCV_CONST( extern, actBNDIGIT ) curvePrime  [BNDIGITS_256]    ;     /* prime:          p = 2^255 - 19 */
    CRYPTOCV_CONST( extern, actBNDIGIT ) curveConst  [BNDIGITS_256]    ;     /* curve constant: d = -121665 / 121666 % p */
    CRYPTOCV_CONST( extern, actBNDIGIT ) groupOrder  [BNDIGITS_256]    ;     /* group order:    Q = 2^252 + 27742317777372353535851937790883648493 */
    CRYPTOCV_CONST( extern, Point )      basePoint                     ;     /* base point in extended twisted Edwards coordinates */
    CRYPTOCV_CONST( extern, actBNDIGIT ) gOBarrett   [BNDIGITS_256 +1] ;     /* order in Barrett representation:    oB = 2^512 / Q */
    CRYPTOCV_CONST( extern, actBNDIGIT ) cCtimes2    [BNDIGITS_256]    ;     /* 2* d mod p      (used in point addition) */
    CRYPTOCV_CONST( extern, actBNDIGIT ) prime38     [BNDIGITS_256]    ;     /* (p + 3) / 8     (used in sqrt) */
    CRYPTOCV_CONST( extern, actBNDIGIT ) sqrtM1      [BNDIGITS_256]    ;     /* sqrt (-1) mod p (used in sqrt) */
    CRYPTOCV_CONST( extern, actBNDIGIT ) a24         [BNDIGITS_256]    ;     /* (A - 2) / 4     (used in Curve25519 evaluation) */


    extern CRYPTOCV_FUNC( void ) actBNLoadLE (
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) bigNum,
       CRYPTOCV_P2CONST_PARA( actU8 ) bytes,
       const actU32 byteLength );
    extern CRYPTOCV_FUNC( void ) actBNStoreLE (
       CRYPTOCV_P2VAR_PARA( actU8 ) bytes,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) bigNum,
       const actU32 bnLength );

    extern CRYPTOCV_FUNC( void ) act25519_initCore (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws );

    extern CRYPTOCV_FUNC( void ) act25519_reduceP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a );

    extern CRYPTOCV_FUNC( void ) act25519_reduceQ (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a );

    extern CRYPTOCV_FUNC( void ) act25519_addModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b );

    extern CRYPTOCV_FUNC( void ) act25519_subModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b );

    extern CRYPTOCV_FUNC( void ) act25519_mulModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b );

    extern CRYPTOCV_FUNC( void ) act25519_powModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_invModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC(int) act25519_sqrtModP (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_addModQ (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b );

    extern CRYPTOCV_FUNC( void ) act25519_mulModQ (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b );

    extern CRYPTOCV_FUNC( void ) act25519_encodePoint (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( actU8 ) buffer,
       CRYPTOCV_P2CONST_PARA(Point) point,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC(int) act25519_decodePoint(
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( Point ) point,
       CRYPTOCV_P2CONST_PARA( actU8 ) buffer,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_addPoint (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( Point ) r,
       CRYPTOCV_P2CONST_PARA(Point) p,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_mulPoint (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( Point ) r,
       CRYPTOCV_P2VAR_PARA( Point ) p,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_mulAddPoint (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( Point ) r,
       CRYPTOCV_P2VAR_PARA( Point ) p,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( void ) act25519_mulBasePoint (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2VAR_PARA( Point ) r,
       CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    extern CRYPTOCV_FUNC( int ) act25519_comparePoints (
       CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
       CRYPTOCV_P2CONST_PARA( Point ) p,
       CRYPTOCV_P2CONST_PARA( Point ) q,
       CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

    #define sizeof_primes   ((BNDIGITS_256 *3 +1) * actBN_BYTES_PER_DIGIT + sizeof (actBNRING) *2)
    #define clearTemp(ws) actMemset ((CRYPTOCV_P2VAR_PARA(actU8)) ws + sizeof_primes, 0, sizeof (actCurve25519ws) - sizeof_primes)

    #ifdef __cplusplus
        }
    #endif

#endif  /* act25519util_h */
