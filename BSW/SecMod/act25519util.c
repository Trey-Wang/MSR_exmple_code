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
 ** references:
 **   https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08
 **   https://tools.ietf.org/html/rfc7748
 **
 ** constants:
 **   25519 curve data
 **
 ** local functions:
 **
 ** global functions:
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

#include  "act25519util.h"
#include  "actWatchdog.h"
#include "actUtilities.h"


/*  ********************************************
 *  Ed25519 curve data:
 *    ref: https://tools.ietf.org/html/rfc7748
 *  ********************************************/

/* prime:
 * p = 2^255 - 19
 */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) curvePrime [BNDIGITS_256] = {
    DIGITs (0xFFFFFFED), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF),
    DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0x7FFFFFFF)
};

/* curve constant (Edwards):
 * d = -121665 / 121666 % p
 *   = 37095705934669439343138083508754565189542113879843219016388785533085940283555
 */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) curveConst [BNDIGITS_256] = {
    DIGITs (0x135978A3), DIGITs (0x75EB4DCA), DIGITs (0x4141D8AB), DIGITs (0x00700A4D),
    DIGITs (0x7779E898), DIGITs (0x8CC74079), DIGITs (0x2B6FFE73), DIGITs (0x52036CEE)
};

/* group order:
 * q = 2^252 + 27742317777372353535851937790883648493
 */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) groupOrder [BNDIGITS_256] = {
    DIGITs (0x5CF5D3ED), DIGITs (0x5812631A), DIGITs (0xA2F79CD6), DIGITs (0x14DEF9DE),
    DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x10000000)
};

/* base point in extended twisted Edwards coordinates
 * bX = 15112221349535400772501151409588531511454012693041857206046113283949847762202
 * bY = 46316835694926478169428394003475163141307993866256225615783033603165251855960
 */
CRYPTOCV_CONST( CRYPTOCV_NONE, Point ) basePoint = {
    { DIGITs (0x8F25D51A), DIGITs (0xC9562D60), DIGITs (0x9525A7B2), DIGITs (0x692CC760),
      DIGITs (0xFDD6DC5C), DIGITs (0xC0A4E231), DIGITs (0xCD6E53FE), DIGITs (0x216936D3)  },    /*  X */
    { DIGITs (0x66666658), DIGITs (0x66666666), DIGITs (0x66666666), DIGITs (0x66666666),
      DIGITs (0x66666666), DIGITs (0x66666666), DIGITs (0x66666666), DIGITs (0x66666666)  },    /*  Y */
    { DIGITs (0x00000001), DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000),
      DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000)  },    /*  Z (== 1) */
    { DIGITs (0xA5B7DDA3), DIGITs (0x6DDE8AB3), DIGITs (0x775152F5), DIGITs (0x20F09F80),
      DIGITs (0x64ABE37D), DIGITs (0x66EA4E8E), DIGITs (0xD78B7665), DIGITs (0x67875F0F)  }     /*  T (== X*Y) */
};


/* 2^512 / q       (used in reduction) */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) gOBarrett [BNDIGITS_256 +1]  = {
    DIGITs (0x0A2C131B), DIGITs (0xED9CE5A3), DIGITs (0x086329A7), DIGITs (0x2106215D),
    DIGITs (0xFFFFFFEB), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF),
    0x0F
};

/* 2* d mod p      (used in point addition) */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) cCtimes2 [BNDIGITS_256] = {
    DIGITs (0x26B2F159), DIGITs (0xEBD69B94), DIGITs (0x8283B156), DIGITs (0x00E0149A),
    DIGITs (0xEEF3D130), DIGITs (0x198E80F2), DIGITs (0x56DFFCE7), DIGITs (0x2406D9DC)
};

/* (p + 3) / 8     (used in sqrt (and inversion)) */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) prime38 [BNDIGITS_256] = {
    DIGITs (0xFFFFFFFE), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF),
    DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0xFFFFFFFF), DIGITs (0x0FFFFFFF)
};

/* sqrt (-1) mod p (used in sqrt (and inversion)) */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) sqrtM1 [BNDIGITS_256] = {
    DIGITs (0x4A0EA0B0), DIGITs (0xC4EE1B27), DIGITs (0xAD2FE478), DIGITs (0x2F431806),
    DIGITs (0x3DFBD7A7), DIGITs (0x2B4D0099), DIGITs (0x4FC1DF0B), DIGITs (0x2B832480)
};

/* (A - 2) / 4     (used in Curve25519 evaluation)
 * A (= 486662) is the Montgomery curve constant
 */
CRYPTOCV_CONST( CRYPTOCV_NONE, actBNDIGIT ) a24 [BNDIGITS_256] = {
    DIGITs (0x0001DB41), DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000),
    DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000), DIGITs (0x00000000)
};



/*  LOAD little endian byte string into little endian actBIGNUM
 *
 *  only works correctly, if
 *  * byteLenght is a multiple of actBN_BYTES_PER_DIGIT
 *  * *bigNum can store byteLength
 */
CRYPTOCV_FUNC( void) actBNLoadLE (
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) bigNum,
   CRYPTOCV_P2CONST_PARA( actU8 ) bytes,
   const actU32 byteLength )
{
    register int   b;

    for (b = (byteLength / actBN_BYTES_PER_DIGIT); b; b--) {
        register actBNDIGIT tmp;

        #if   (4 == actBN_BYTES_PER_DIGIT)
            tmp  = ((actBNDIGIT) (*bytes++));
            tmp += ((actBNDIGIT) (*bytes++)) <<  8;
            tmp += ((actBNDIGIT) (*bytes++)) << 16;
            tmp += ((actBNDIGIT) (*bytes++)) << 24;
        #elif (2 == actBN_BYTES_PER_DIGIT)
            tmp  = ((actBNDIGIT) (*bytes++));
            tmp += ((actBNDIGIT) (*bytes++)) <<  8;
        #elif (1 == actBN_BYTES_PER_DIGIT)
            tmp  = ((actBNDIGIT) (*bytes++));
        #else
            #error actBN_BYTES_PER_DIGIT value not supported
        #endif

        *bigNum++ = tmp;
    }
}

/*  STORE little endian actBIGNUM into little endian byte string
 *
 *  only works correctly, if
 *  * *bytes can store (bnLength * actBN_BYTES_PER_DIGIT)
 */
CRYPTOCV_FUNC( void ) actBNStoreLE (
   CRYPTOCV_P2VAR_PARA( actU8 ) bytes,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) bigNum,
   const actU32 bnLength )
{
    register int     b;

    for (b = bnLength; b; b--) {
        register actBNDIGIT tmp = *bigNum++;

        #if   (4 == actBN_BYTES_PER_DIGIT)
            *bytes++ = (actU8) (tmp);
            *bytes++ = (actU8) (tmp >>  8);
            *bytes++ = (actU8) (tmp >> 16);
            *bytes++ = (actU8) (tmp >> 24);
        #elif (2 == actBN_BYTES_PER_DIGIT)
            *bytes++ = (actU8) (tmp);
            *bytes++ = (actU8) (tmp >>  8);
        #elif (1 == actBN_BYTES_PER_DIGIT)
            *bytes++ = (actU8) (tmp);
        #else
            #error actBN_BYTES_PER_DIGIT value not supported
        #endif
    }
}



/* initialize core data structure */
CRYPTOCV_FUNC( void ) act25519_initCore(
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws)
{
    CRYPTOCV_P2VAR_PARA( actBNRING ) ring;

    actMemcpy_ROM (ws->prime,  curvePrime, sizeof (curvePrime));
    actMemcpy_ROM (ws->order,  groupOrder, sizeof (groupOrder));
    actMemcpy_ROM (ws->orderB, gOBarrett,  sizeof (gOBarrett));

    ring = &(ws->pRing);
    ring->m             = ws->prime;
    ring->m_length      = BNDIGITS_256;

    ring = &(ws->qRing);
    ring->m             = ws->order;
    ring->m_length      = BNDIGITS_256;
}



/* r = a mod n
 *
 *    ref:  https://en.wikipedia.org/wiki/Barrett_reduction
 */


/* common steps ...
 *
 *  input
 *    q = ((a * m) / 2^512)
 */
static CRYPTOCV_FUNC( void ) act25519_reduce (
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) q,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) n )
{
    int         d;
    actBNDDIGIT temp;


    /* (2)   r = q * n = ((a * m) / 2^512) * n
     *
     *    we just calculate the BNDIGITS_256 LSdigits:
     *    * the MSdigits will NOT influence the final result
     *      (wich is just BNDIGITS_256 "long")
     *
     */
    actMemset (r, 0, BNBYTES_256);
    for (d = 0; d < BNDIGITS_256; d++) {
        int         j;
        actBNDDIGIT digit;

        digit = n [d];
        temp  = 0;
        for (j = 0; j < ((int) BNDIGITS_256 -d); j++) {
            temp += ((actBNDDIGIT) q [j]) * digit;
            temp += r [d + j];
            r [d + j] = actBNLoWord (temp);
            temp  = actBNHiWord (temp);
        }
    }


    /* (3)   r = a - r = a - (((a * m) / 2^512) * n)
     *
     *    we just calculate the BNDIGITS_256 LSdigits:
     *    * the MSdigits will turn to ZERO anyway!
     */
    actBNSub (a, r, r, BNDIGITS_256);


    /* (4)   r -= n    (while still too large)
     *
     *    ONE subtraction should be enough:
     *    * just in case we'll repeat this...
     */
    while (1 == actBNCompare (r, n, BNDIGITS_256))
        actBNSub (r, n, r, BNDIGITS_256);                                                       
}


/* r = a mod p */
CRYPTOCV_FUNC( void ) act25519_reduceP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a )
{
    CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a76  = a;
    CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a2   = a;
    CRYPTOCV_P2VAR_PARA( actBNDIGIT )   q    = ws->tempQ;
    actBNDDIGIT temp = 0;
    int         d;


    /* (1)   q = (a * m) / 2^512   (the division is implicit)
     *
     *    we're taking advantage of the structure of m:
     *    * m has just two digits which aren't ZERO
     *    * this holds true for all digit sizes (8, 16, 32)!
     *    -> we multiply just these two digits (2 and 76)
     *       with matching offset (of BNDIGITS_256)
     *
     *    in addition we only store the MSdigits:
     *    -> this implicitely divides the result into 2^512
     */
    for (d = 0; d <    BNDIGITS_256; d++) {
        temp += ((actBNDDIGIT) *a76++) *76;
        temp  = actBNHiWord (temp);
    }

    for (     ; d < 2* BNDIGITS_256; d++) {
        temp += ((actBNDDIGIT) *a76++) *76;
        temp += ((actBNDDIGIT) *a2++)  *2;
        temp  = actBNHiWord (temp);
    }

    for (     ; d < 3* BNDIGITS_256; d++) {
        temp += ((actBNDDIGIT) *a2++)  *2;
        *q++  = actBNLoWord (temp);             /* overwrite MSdigits of a */
        temp  = actBNHiWord (temp);
    }

    act25519_reduce (r, a, ws->tempQ, ws->prime);
}

/* r = a mod q */
CRYPTOCV_FUNC( void ) act25519_reduceQ (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a )
{
    CRYPTOCV_P2VAR_PARA( actBNDIGIT ) q = ws->tempE;  /* tempE .. tempH are unused during protocol calculation!!! */


    /* (1)   q = (a * m) / 2^512   (the division is implicit)
     *
     *    just a plain multiplication (not used that often!)
     *
     *    in addition we only use the MSdigits for further calculation:
     *    -> this implicitely divides the result into 2^512
     *
     *    (memory) optimization potential:
     *    * multiply in three chunks @ 256 bits result each
     *      and store just the last one (like: reduceP)
     */
    actBNMult (a, BNDIGITS_512, ws->orderB, BNDIGITS_256 +1, q, NULL);

    act25519_reduce (r, a, q +BNDIGITS_512, ws->order);
}


/* r = (a + b) mod p */
CRYPTOCV_FUNC( void ) act25519_addModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b )
{
    actBNModAdd (a, b, r, &(ws->pRing), NULL);
}

/* r = (a - b) mod p */
CRYPTOCV_FUNC( void ) act25519_subModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b )
{
    actBNModSub (a, b, r, &(ws->pRing), NULL);
}

/* r = (a * b) mod p */
CRYPTOCV_FUNC( void ) act25519_mulModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b )
{
    actBNMult        (a, BNDIGITS_256, b, BNDIGITS_256, ws->temp1, NULL);
    act25519_reduceP (ws, r, ws->temp1);
}

/* r = (a ^ b) mod p
 *
 *    a WILL be destroyed!!
 */
CRYPTOCV_FUNC( void ) act25519_powModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actU8       dig;


    actMemset (r, 0, BNBYTES_256);  r [0] = 1;         /* r = 1 */

    /* iterate over the digits of "b" */
    for (dig = 0; dig < BNDIGITS_256; dig++) {
        actBNDIGIT  digit = b [dig];
        actU8       bit;

        /* iterate over the bits of "digit" */
        for (bit = 0; bit < actBN_BITS_PER_DIGIT; bit++) {
            if (digit & 1) {
                act25519_mulModP (ws, r, a, r);     /* multiply */
            }
            act25519_mulModP (ws, a, a, a);         /* square */
            digit >>= 1;

            ifWDisL2 (0 == (bit %2), watchdog);     /* every 2..4 multiplications */
        }
    }
}

/* r = (1 / a) mod p
 *
 *    (1 / a) mod p = a^(-1) mod p = a^(p -2) mod p
 *
 *    ref: https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.1
 */
CRYPTOCV_FUNC( void ) act25519_invModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy (ws->tempE, a,         BNBYTES_256);
    actMemcpy (ws->tempF, ws->prime, BNBYTES_256);
    ws->tempF [0] -= 2;     /*  tempD = p -2 */

    act25519_powModP (ws, r, ws->tempE, ws->tempF, watchdog);
}

/* r = sqrt (a) mod p
 *
 *    1. candidate: r = a ^ ((p + 3) / 8) mod p
 *    2. square; if identical -> sqrt found
 *    3. multiply by -1
 *    4. square; if identical -> sqrt found
 *    5. NO square-root found
 *
 *    ref: https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.3
 */
CRYPTOCV_FUNC(int) act25519_sqrtModP (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy   (ws->tempG, a,       BNBYTES_256);
    actMemcpy_ROM (ws->tempH, prime38, BNBYTES_256);

    act25519_powModP (ws, ws->tempE, ws->tempG, ws->tempH, watchdog);   /* 1.  tempE = candidate */

    act25519_mulModP (ws, ws->tempF, ws->tempE, ws->tempE);             /* 2.1 square */
    if (0 == actMemcmp (ws->tempF, a, BNBYTES_256)) {                      /* 2.2 found! */
        actMemcpy (r, ws->tempE, BNBYTES_256);

        actL2trigger (watchdog);
        return 1;
    }

    actMemcpy_ROM    (ws->tempH, sqrtM1, BNBYTES_256);
    act25519_mulModP (ws, ws->tempE, ws->tempE, ws->tempH);             /* 3.  tempE = candidate * -1 */

    act25519_mulModP (ws, ws->tempF, ws->tempE, ws->tempE);             /* 4.1 square */
    if (0 == actMemcmp (ws->tempF, a, BNBYTES_256)) {                      /* 4.2 found! */
        actMemcpy (r, ws->tempE, BNBYTES_256);

        actL2trigger (watchdog);
        return 1;
    }

    actL2trigger (watchdog);    /* 3 multiplications */
    return 0;                                                                           /* 5.  NO square-root */
}


/* r = (a + b) mod q */
CRYPTOCV_FUNC( void ) act25519_addModQ (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b )
{
    actBNModAdd (a, b, r, &(ws->qRing), NULL);
}

/* r = (a * b) mod q */
CRYPTOCV_FUNC( void ) act25519_mulModQ (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b )
{
    actBNMult        (a, BNDIGITS_256, b, BNDIGITS_256, ws->temp1, NULL);
    act25519_reduceQ (ws, r, ws->temp1);
}


/* encode point
 *
 *    projective -> affine (divide by z)
 *    MSBit (y) = lsBit (x)
 *    store as LE byte array
 *
 *    ref: https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.2
 */
CRYPTOCV_FUNC( void ) act25519_encodePoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer,
   CRYPTOCV_P2CONST_PARA(Point) point,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy   (ws->tempA, point->z, BNBYTES_256);

    act25519_invModP (ws, ws->tempA, point->z,             watchdog);   /* 1 / z */
    act25519_mulModP (ws, ws->tempB, point->x, ws->tempA);              /* x / z */
    act25519_mulModP (ws, ws->tempC, point->y, ws->tempA);              /* y / z */

    /* MSbit (y) will be ZERO, because p = 2^255 -19 */
    /* MSBit (y) = lsBit (x) */
    ws->tempC [BNDIGITS_256 -1] |= ws->tempB [0] << (actBN_BITS_PER_DIGIT -1);

    actBNStoreLE (buffer, ws->tempC, BNDIGITS_256);

    actL2trigger (watchdog);    /* 2 multiplications */
}

/* decode point
 *
 *    1. load from LE byte array
 *       x_0 = lsBit (x) = MSBit (y)
 *       y = clear MSBit (y)
 *       z = 1
 *    2. evaluate curve equation to get x^2
 *       x^2 = (u/v) = (y^2 -1) / (d * y^2 +1) (mod p)
 *       x = sqrt (u/v) (mod p)
 *       (NOT using the integrated inversion / sqrt as described in "ref",
 *        but rather calculate the inversion & square-root separately)
 *    3. evaluation is part of sqrt function!
 *    4. if (x == 0 && x_0) => NOT a valid point
 *       if (x_0 != lsBit (x))  x = (0 - x) (mod p)
 *
 *    *. t = x * y  (not in "ref")
 *
 *    return (true) if decoding successful, (false) otherwise.
 *
 *    ref: https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.3
 */
CRYPTOCV_FUNC( int ) act25519_decodePoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( Point ) point,
   CRYPTOCV_P2CONST_PARA( actU8 ) buffer,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actU8       x_0, d;
    actBNDIGIT  xNOTzero;

    actBNLoadLE (point->y, buffer, BNBYTES_256);                        /* load from LE byte array */
    x_0 = point->y [BNDIGITS_256 -1] >> (actBN_BITS_PER_DIGIT -1);      /* x_0 = lsBit (x) = MSBit (y) */
    point->y [BNDIGITS_256 - 1] &= MASK_MSBit;                          /* y = clear MSBit (y) */
    actMemset (point->z, 0, sizeof (point->z)); point->z [0] = 1;          /* z = 1 */

    act25519_mulModP (ws, ws->tempD, point->y,  point->y);
    act25519_subModP (ws, ws->tempA, ws->tempD, point->z);              /* tempA = u = y^2 -1 */
    actMemcpy               (ws->tempC, curveConst, BNBYTES_256);
    act25519_mulModP (ws, ws->tempB, ws->tempD, ws->tempC);
    act25519_addModP (ws, ws->tempB, ws->tempB, point->z);              /* tempB = v = d * y^2 +1 */
    actL2trigger (watchdog);    /*  2 multiplications + 2 additions */

    act25519_invModP (ws, ws->tempC, ws->tempB,            watchdog);
    act25519_mulModP (ws, ws->tempD, ws->tempA, ws->tempC);             /* tempD = (u/v) = x^2 */
    if (0 == act25519_sqrtModP (ws,  point->x,  ws->tempD, watchdog))   /* p.x = x */
        return 0;                                                       /* NO square-root found */

    for (xNOTzero = 0, d = 0; d < BNDIGITS_256; ++d)
        xNOTzero |= point->x [d];
    if (! xNOTzero && x_0)                                              /* (x == 0 && x_0) ? */
        return 0;                                                       /* NOT a valid point */

    if (x_0 != (point->x [0] & 1)) {                                    /* (x_0 != lsBit (x)) ? */
        actMemset (ws->tempB, 0, BNBYTES_256);
        act25519_subModP (ws, point->x, ws->tempB, point->x);           /* x = (0 - x) (mod p) */
    }

    act25519_mulModP (ws, point->t, point->x, point->y);                /* t = x * y */

    actL2trigger (watchdog);    /* 1 multiplication + 1 addition */
    return 1;
}


/* add two curve points
 *  r += p
 *
 *    ref: https://tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.4
 */
CRYPTOCV_FUNC( void ) act25519_addPoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( Point ) r,
   CRYPTOCV_P2CONST_PARA(Point) p,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actMemcpy_ROM (ws->tempD, cCtimes2, sizeof (cCtimes2));         /* D = 2*d (curve constant) */

    act25519_subModP (ws,  ws->tempA,  r->y,       r->x);
    act25519_subModP (ws,  ws->tempC,  p->y,       p->x);
    act25519_mulModP (ws,  ws->tempA,  ws->tempA,  ws->tempC);      /* A = (p.y - p.x) * (q.y - q.x) */
    act25519_addModP (ws,  ws->tempB,  r->y,       r->x);
    act25519_addModP (ws,  ws->tempC,  p->y,       p->x);
    act25519_mulModP (ws,  ws->tempB,  ws->tempB,  ws->tempC);      /* B = (p.y + p.x) * (q.y + q.x) */
    act25519_mulModP (ws,  ws->tempC,  r->t,       ws->tempD);
    act25519_mulModP (ws,  ws->tempC,  ws->tempC,  p->t);           /* C = p.t * 2*d * q.t */
    act25519_mulModP (ws,  ws->tempD,  r->z,       p->z);
    act25519_addModP (ws,  ws->tempD,  ws->tempD,  ws->tempD);      /* D = p.z * 2 * q.z */

    actL2trigger (watchdog);    /* 5 multiplications (and 5 additions) */

    act25519_subModP (ws,  r->t,       ws->tempB,  ws->tempA);      /* E = B - A */
    act25519_subModP (ws,  r->z,       ws->tempD,  ws->tempC);      /* F = D - C */
    act25519_addModP (ws,  ws->tempD,  ws->tempD,  ws->tempC);      /* G = D + C */
    act25519_addModP (ws,  ws->tempB,  ws->tempB,  ws->tempA);      /* H = B + A */

    act25519_mulModP (ws,  r->x,       r->t,       r->z);           /* p.x = E * F */
    act25519_mulModP (ws,  r->y,       ws->tempD,  ws->tempB);      /* p.y = G * H */
    act25519_mulModP (ws,  r->z,       r->z,       ws->tempD);      /* p.z = F * G */
    act25519_mulModP (ws,  r->t,       r->t,       ws->tempB);      /* p.t = E * H */

    actL2trigger (watchdog);    /* 4 multiplications (and 4 additions) */
}


/* multiply curve point by scalar
 *  r = s * p
 *
 *  p WILL be destroyed!
 */
CRYPTOCV_FUNC( void ) act25519_mulPoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( Point ) r,
   CRYPTOCV_P2VAR_PARA( Point ) p,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    /* initialize the result to (0, 1, 1, 0) */
    actMemset (r, 0, sizeof (Point));
    r->y [0] = 1;
    r->z [0] = 1;

    act25519_mulAddPoint (ws, r, p, s, watchdog);
}

/* multiply curve point by scalar and add to result
 *  r += s * p
 *
 *  p WILL be destroyed!
 */
CRYPTOCV_FUNC( void ) act25519_mulAddPoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( Point ) r,
   CRYPTOCV_P2VAR_PARA( Point ) p,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    actU8   dig;

    /* iterate over the digits of "s" */
    for (dig = 0; dig < BNDIGITS_256; dig++) {
        actU8       bit;
        actBNDIGIT  digit = *s++;

        /* iterate over the bits of "digit" */
        for (bit = 0; bit < actBN_BITS_PER_DIGIT; bit++) {
            if (digit & 1) {
                act25519_addPoint (ws, r, p, watchdog);     /* add */
            }
            act25519_addPoint (ws, p, p, watchdog);         /* double */
            digit >>= 1;
        }
    }

/* actL2trigger (watchdog);   */  /* addPoint DID reset the watchdog */
}


/* multiply base point by scalar
 *  r = s * BP
 */
CRYPTOCV_FUNC( void ) act25519_mulBasePoint (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2VAR_PARA( Point ) r,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) s,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    CRYPTOCV_P2VAR_PARA( Point ) p = (CRYPTOCV_P2VAR_PARA( Point )) ws->tempE;

    actMemcpy_ROM          (p, &basePoint, sizeof (basePoint));
    act25519_mulPoint (ws, r, p, s, watchdog);
}


/* compare two points
 *
 *    compare x and y coordinates;
 *    to speed things up, cross-wise multiply by Z instead of dividing by Z
 *       q.x / q.z == p.x / p.z
 *    => q.x * p.z == p.x * q.z
 */
CRYPTOCV_FUNC( int ) act25519_comparePoints (
   CRYPTOCV_P2VAR_PARA( actCurve25519ws ) ws,
   CRYPTOCV_P2CONST_PARA(Point) p,
   CRYPTOCV_P2CONST_PARA(Point) q,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    int result = 1;

    act25519_mulModP (ws, ws->tempA, p->x, q->z);
    act25519_mulModP (ws, ws->tempB, q->x, p->z);
    result &= ! actMemcmp (ws->tempA, ws->tempB, BNBYTES_256);

    act25519_mulModP (ws, ws->tempA, p->y, q->z);
    act25519_mulModP (ws, ws->tempB, q->y, p->z);
    result &= ! actMemcmp (ws->tempA, ws->tempB, BNBYTES_256);

    actL2trigger (watchdog);    /* 4 multiplications */
    return result;
}
