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
 ** reference:
 **   https://tools.ietf.org/html/rfc7748
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
 **   actX25519_initCore
 **   actX25519_eval
 **
 ***************************************************************************/

#include  "actX25519core.h"
#include  "actWatchdog.h"
#include  "actUtilities.h"
#include  <actBigNum.h>


/****************************************************************************
 *  local functions
 ***************************************************************************/
static CRYPTOCV_FUNC( void ) cswap (
   int swap,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b );


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** void actX25519_initCore (actEd25519Struct *ws))
 **
 **  This function initializes the internal data structures.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 **
 ** output:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actX25519_initCore (
   CRYPTOCV_P2VAR_PARA( actX25519Struct ) ws )
{
    act25519_initCore ((CRYPTOCV_P2VAR_PARA( actCurve25519ws )) ws->coreWS );
}


/****************************************************************************
 **
 ** FUNCTION:
 **
 ** int actX25519_eval (actX25519Struct *ws, void (* watchdog) (void))
 **
 **  This function evaluates the raw Curve25519 function.
 **  The private key and u coordinate(s) are passed in the work-space.
 **
 ** input:
 ** - ws:           pointer to the work-space structure
 ** - watchdog:     pointer to watchdog reset function
 **
 ** output:
 **   returns true (!= 0) if the function was evaluated, and
 **   false if u is not a proper member of the field mod p.
 **
 ***************************************************************************/
CRYPTOCV_FUNC(int) actX25519_eval (
  CRYPTOCV_P2VAR_PARA(actX25519Struct) ws,
  CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    CRYPTOCV_P2VAR_PARA(actCurve25519ws) coreWS = (CRYPTOCV_P2VAR_PARA(actCurve25519ws)) ws->coreWS;

    #define     x_1     ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(ws->uCoord))      /* is overwritten in final step anyway */
    #define     x_2     ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempA))
    #define     z_2     ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempB))
    #define     x_3     ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempC))
    #define     z_3     ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempD))
    #define     A       ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempE))
    #define     B       ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempF))
    #define     C       ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempG))
    #define     D       ((CRYPTOCV_P2VAR_PARA(actBNDIGIT)) &(coreWS->tempH))

    #define     AA      A
    #define     BB      B
    #define     CB      C
    #define     DA      D
    #define     E       x_3

    int         k_t;
    int         swap;
    int         t;


    /* initialize */
    actBNLoadLE (x_1, ws->uCoord, BNBYTES_256);             /*  x_1 = u */
    x_1 [BNDIGITS_256 -1] &= MASK_MSBit;                    /*  mask MSbit of input u coordinate */
    actMemset   (x_2, 0,          BNBYTES_256); *x_2 = 1;   /*  x_2 = 1 */
    actMemset   (z_2, 0,          BNBYTES_256);             /*  x_2 = 0 */
    actMemcpy   (x_3, x_1,        BNBYTES_256);             /*  x_3 = u */
    actMemcpy   (z_3, x_2,        BNBYTES_256);             /*  x_3 = 1 */


    swap = 0;
    for (t = 254; t >= 0; t--) {
        k_t = (ws->privateKey [t /8] >> (t &0x07)) &0x01;

        /* conditional swap; see below. */
        swap ^= k_t;
        cswap (swap, x_2, x_3);
        cswap (swap, z_2, z_3);
        swap  = k_t;

        /* algorithm from https://tools.ietf.org/html/rfc7748
         * has been re-ordered to save memory!
         *
         *  1. calculate A,  B,  C  and D
         *  2. calculate DA, CB, BB and AA
         *  3. calculate E
         *  4. calculate x_2, z_2, x_3, z_3
         */

        act25519_addModP (coreWS,  A,   x_2, z_2);      /*  A   = x2 + z_2 */
        act25519_subModP (coreWS,  B,   x_2, z_2);      /*  B   = x2 - z_2 */
        act25519_addModP (coreWS,  C,   x_3, z_3);      /*  C   = x3 + z_3 */
        act25519_subModP (coreWS,  D,   x_3, z_3);      /*  D   = x3 - z_3 */
        act25519_mulModP (coreWS,  DA,  D,   A);        /*  DA  =  D * A */
        act25519_mulModP (coreWS,  CB,  C,   B);        /*  CB  =  C * B */
        act25519_mulModP (coreWS,  BB,  B,   B);        /*  BB  =  B * B */
        act25519_mulModP (coreWS,  AA,  A,   A);        /*  AA  =  A * A */
        act25519_subModP (coreWS,  E,   AA,  BB);       /*  E   = AA - BB */

        act25519_mulModP (coreWS,  x_2, AA,  BB);       /*  x_2 = AA * BB */

        actL2trigger (watchdog);    /* 5 multiplications + 5 additions */

        actMemcpy_ROM    (BB, a24, BNBYTES_256);        /* BB  =           a24 */
        act25519_mulModP (coreWS,  z_2, BB,  E);        /* z_2 =           a24 * E */
        act25519_addModP (coreWS,  z_2, AA,  z_2);      /* z_2 =      AA + a24 * E */
        act25519_mulModP (coreWS,  z_2, E,   z_2);      /* z_2 = E * (AA + a24 * E) */

        act25519_addModP (coreWS,  x_3, DA,  CB);       /* x_3 =  DA + CB */
        act25519_mulModP (coreWS,  x_3, x_3, x_3);      /* x_3 = (DA + CB) ^2 */

        act25519_subModP (coreWS,  z_3, DA,  CB);       /* z_3 =        DA - CB */
        act25519_mulModP (coreWS,  z_3, z_3, z_3);      /* z_3 =       (DA - CB) ^2 */
        act25519_mulModP (coreWS,  z_3, z_3, x_1);      /* z_3 = x_1 * (DA - CB) ^2 */

        actL2trigger (watchdog);    /* 5 multiplications + 3 additions */
    }

    /* conditional swap; see below. */
    cswap (swap, x_2, x_3);
    cswap (swap, z_2, z_3);

    /* return x_2 * z_2^-1 */
    act25519_invModP (coreWS, z_3, z_2, watchdog);         /* z_3 =      z_2^-1 */
    act25519_mulModP (coreWS, x_2, x_2, z_3);              /* x_2 = x2 * z_2^-1 */
    actBNStoreLE (ws->uCoord, x_2, BNDIGITS_256);

    #undef      x_1
    #undef      x_2
    #undef      z_2
    #undef      x_3
    #undef      z_3
    #undef      A
    #undef      B
    #undef      C
    #undef      D

    #undef      AA
    #undef      BB
    #undef      CB
    #undef      DA
    #undef      E


    actL2trigger (watchdog);    /* 1 multiplication */
    clearTemp (coreWS);
    return 0;
}


/****************************************************************************
 *  local functions
 ***************************************************************************/

/*  conditionally swap a and b
 *
 *   ref:  https://tools.ietf.org/html/rfc7748
 */
static CRYPTOCV_FUNC( void ) cswap (
   int swap,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b )
{
    actBNDIGIT  mask = (actBNDIGIT) 0 - swap;
    actBNDIGIT  digit;
    int         d;

    for (d = 0; d < BNDIGITS_256; d++) {
        digit = mask & (*a ^ *b);
        *a++ ^= digit;
        *b++ ^= digit;
    }
}
