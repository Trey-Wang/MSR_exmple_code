/****************************************************************************
 ** Main author: UBR                     Creation date: 10/17/03
 ** $Author:: mgnida                   $ $JustDate:: 10/17/03             $
 ** $Workfile:: actBigNum.h            $ $Revision:: 2010                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2005                          )
 **
 ** \version(1.0 (beta )                                                  )
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: Core Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: A basic (unsigned) integer and modulo arithmetic
 **                     used for elliptic curve point arithmetic.
 **
 ** constants:
 **   actBN_BITS_PER_DIGIT
 **
 ** types:
 **   actBNRING
 **
 ** macros:
 **   actBNGetDigitLength
 **   actBNGetBitOfDigit
 **   actBNGetBit
 **   actBNIsOdd
 **   actBNIsEven
 **
 ** functions:
 **
 ** - unsigned integer functions:
 **
 **   actBNSetOctetString
 **   actBNOctetString
 **   actBNCopy
 **   actBNSetZero
 **   actBNIsZero
 **   actBNCompare
 **   actBNAdd
 **   actBNSub
 **   actBNDiv2
 **
 ** - modulo arithmetic functions:
 **
 **   actBNModAdd
 **   actBNModSub
 **   actBNModDiv2
 **   actBNMontMul
 **   actBNModExp
 **
 ** - field arithmetic function:
 **
 **   actBNFieldInversion
 **
 ***************************************************************************/


#ifndef ACTBIGNUM_H
#define ACTBIGNUM_H


#include "actITypes.h"
#include "actConfig.h"


/****************************************************************************
 ** Constants
 ***************************************************************************/


/* DDIGIT arithmetic constant ( 0x00010000 for actBN_BITS_PER_DIGIT==16) */
#define actBN_BASE     ( (actBNDDIGIT)(1) << actBN_BITS_PER_DIGIT )
/* DIGIT arithmetic constant ( 0xFFFF for actBN_BITS_PER_DIGIT==16) */
#define actBN_MAX_DIGIT ( (actBNDIGIT)(actBN_BASE - 1) )
/* DIGIT arithmetic constant ( 0x8000 for actBN_BITS_PER_DIGIT==16) */
#define actBN_MAX_BIT ( (actBNDDIGIT)(1) << (actBN_BITS_PER_DIGIT-1) )
/* DDIGIT arithmetic constant ( 0xFFFF0000 for actBN_BITS_PER_DIGIT==16) */
#define actBN_HIGH_MAX  ( (actBNDDIGIT)(actBN_MAX_DIGIT) << actBN_BITS_PER_DIGIT )

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/*
   BigNum modular ring type

   Contains:
   - the modulus (MUST BE ODD for Montgomery !!!)
   - length information of the modulus
   - additional constants for Montgomery
   - optimization switch for Montgomery
*/
typedef struct
{
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) m;  /* ring modulus */
   actBNLENGTH m_length;                                               /* length of m in digit */
   actBNLENGTH m_byte_length;                                          /* length of m in byte */
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) RR; /* RR = R^2, R = base^m_length mod m */
   actBNDIGIT m_bar;                                                   /* mbar = -(m^-1) mod base */
#if ( actBN_MONT_MUL_VERSION==1 )
   int prime_structure;       /* flag for optimization in MontMul */
#endif
#if ( actBN_MOD_EXP_WINDOW_SIZE==0 )   /* variable k-bit window algorithm */
   int window_size;                 /* is used for modular exponentiation */
#endif
} actBNRING;


/****************************************************************************
 ** Macros
 ***************************************************************************/


/* calculate digit length from byte length */
#define actBNGetDigitLength(byte_len) \
( ((byte_len)+actBN_BYTES_PER_DIGIT-1)/actBN_BYTES_PER_DIGIT )
/* test a bit of a digit */
#define actBNGetBitOfDigit(digit, i) \
( ((digit) & ((actBNDIGIT)(1)<<(i)))? ((actU8)1):((actU8)0) )
/* test a bit of a BigNum */
#define actBNGetBit(a, i) \
(actBNGetBitOfDigit( (a)[(i)/actBN_BITS_PER_DIGIT], ((i)%actBN_BITS_PER_DIGIT) ))
#define actBNIsOdd(a)  ( ( ((a)[0]) & ((actBNDIGIT)(1)) )? 1:0 )
#define actBNIsEven(a) ( ( ((a)[0]) & ((actBNDIGIT)(1)) )? 0:1 )

/* Get the lower word of a ddigit */
#define actBNLoWord(ddigit) ( (actBNDIGIT)(ddigit) )

/* Get the higher word of a dword */
#define actBNHiWord(ddigit) ( (actBNDIGIT)((ddigit) >> actBN_BITS_PER_DIGIT) )


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/


#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNSetOctetString(actBNDIGIT *a,
 **                           actBNLENGTH length,
 **                           const actU8 *in,
 **                           const int in_len)
 **
 **   Big Endian Byte array to BigNum conversion according to IEEE 1363
 **   OS2FEP (octet string to field element presentation).
 **
 ** input:                                              (length in digits)
 ** - a:       the destination number                        length
 ** - length:  length of a
 ** - in:      the source octet string
 ** - in_len:  the length in bytes of the octet string
 **
 ** output:
 ** - a:       the number representing the octet string      length
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNSetOctetString(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actBNLENGTH length,
   CRYPTOCV_P2CONST_PARA( actU8 ) in,const int in_len );

CRYPTOCV_FUNC( void ) actBNSetOctetStringROM(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actBNLENGTH length,
   actPKey in, const int in_len );

/****************************************************************************
 **
 ** FUNCTION:
 ** void actBNOctetString(actU8 *out,
 **                       int out_len,
 **                       const actBNDIGIT *a,
 **                       const actBNLENGTH length)
 **
 **   BigNum to Big Endian Byte array conversion according to IEEE 1363
 **   FE2OSP (field element to octet string presentation).
 **
 ** input:                                              (length in digits)
 ** - out:     the destination byte array
 ** - out_len: byte length of the destination byte array
 ** - a:       the source number                             length
 ** - length:  length of a
 **
 ** output:
 ** - out:     the octet string representing the out_len
 **            lowest bytes of the number a
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNOctetString(
   CRYPTOCV_P2VAR_PARA( actU8 ) out, int out_len,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNCopy(actBNDIGIT* a,
 **                 const actBNDIGIT* b,
 **                 const actBNLENGTH length)
 **
 **   Copies the BigNum b into a ( a = b ).
 **
 ** input:                                              (length in digits)
 ** - a:       the destination number                        length
 ** - b:       the source number                             length
 ** - length:  length of a and b
 **
 ** output:
 ** - a:       a == b                                        length
 **
 ** assumes:
 ** - a_length == b_length == length
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNCopy(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **   actBNGetBitLength
 **
 **   Calculate exact length in bits of a BigNum.
 **
 ** input:                                              (length in digits)
 ** - a:       the number to be evaluated                    length
 ** - length:  length of a
 **
 ** output:
 ** - returns:   bit_length(a)
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNGetBitLength(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const int length );

/****************************************************************************
 **
 ** FUNCTION:
 **  actBNSetBit
 **
 **   Sets a bit of a BigNum.
 **
 ** input:                                              (length in digits)
 ** - a:       the input number                              length
 ** - length:  length of a
 ** - bit_pos: position of the bit to be set
 **
 ** output:
 ** - a:       a | 2^bit_pos                                 length
 ** - returns:   -1      if bit_pos >= length*BITS_PER_DIGIT
 **               0      else (success)
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNSetBit(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, const actBNLENGTH length, const int bit_pos );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNSetZero(actBNDIGIT* a,
 **                    const actBNLENGTH length)
 **
 **   Sets a BigNum to zero.
 **
 ** input:                                              (length in digits)
 ** - a:       the number to be zeroized                     length
 ** - length:  length of a
 **
 ** output:
 ** - a:       a == 0                                        length
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNSetZero( CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNIsZero(const actBNDIGIT* a,
 **                  const actBNLENGTH length)
 **
 **   Compares a BigNum against zero.
 **
 ** input:                                              (length in digits)
 ** - a:       the number to be checked                      length
 ** - length:  length of a
 **
 ** output:
 ** - returns:    1   if a == 0
 **               0   if a != 0
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNIsZero( CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNCompare(const actBNDIGIT* a,
 **                   const actBNDIGIT* b,
 **                   const actBNLENGTH length)
 **
 **   Compares two BigNums.
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  length
 ** - b:       second number                                 length
 ** - length:  length of a and b
 **
 ** output:
 ** - returns:    1   if a > b
 **               0   if a == b
 **              -1   if a < b
 **
 ** assumes:
 ** - a_length == b_length == length
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNCompare(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNAdd(const actBNDIGIT* a,
 **               const actBNDIGIT* b,
 **               actBNDIGIT* c,
 **               const actBNLENGTH length)
 **
 **   BigNum Addition ( c = a+b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  length
 ** - b:       second number                                 length
 ** - c:       the result                                    length
 ** - length:  length of a and b
 **
 ** output:
 ** - c:       the result a + b                              length
 ** - returns: the last carry ( 0 or 1 )
 **
 ** assumes:
 ** - a_length == b_length == length
 **
 ** remark:
 ** - c may overlap with a or b ( a+=b or b+=a is possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNAdd(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNSub(const actBNDIGIT* a,
 **                const actBNDIGIT* b,
 **                actBNDIGIT* c,
 **                const actBNLENGTH length)
 **
 **   BigNum Subtraction ( c = a-b for a>=b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number                                  length
 ** - b:       second number                                 length
 ** - c:       the result                                    length
 ** - length:  length of a and b
 **
 ** output:
 ** - c:       the result a - b                              length
 **
 ** assumes:
 ** - a_length == b_length == length
 ** - a >= b
 **
 ** remark:
 ** - c may overlap with a or b ( a-=b or b-=a is possible )
 ** - a>=b means the last carry is allways zero, thus actBNSub(..)
 **   is of type void
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNSub(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, const actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMult(const actBNDIGIT* a,
 **                 const actBNLENGTH a_length,
 **                 const actBNDIGIT* b,
 **                 const actBNLENGTH b_length,
 **                 actBNDIGIT* c);
 **
 **   BigNum Multiply ( c = a*b ).
 **
 ** input:                                              (length in digits)
 ** - a:          first number                               a_length
 ** - b:          second number                              b_length
 ** - c:          the result                              a_length+b_length
 **
 ** output:
 ** - c:       the result a * b                           a_length+b_length
 **
 ** assumes:
 **
 ** remark:
 ** - c cannot overlap with a or b ( a*=b or b*=a is not possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMult(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, const actBNLENGTH a_length,
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b, const actBNLENGTH b_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 ** void actBNDiv2(actBNDIGIT* a,
 **                actU8 first_carry,
 **                const actBNLENGTH length)
 **
 **  Division by 2 ( a /= 2  ).
 **
 ** input:                                              (length in digits)
 ** - a:            number to be divided                   length (+ 1 bit)
 ** - first_carry:  the bit_length of 'a' is
 **                     digits * bits_per_digit + 1,
 **                 and the leading bit passed as 'first_carry'
 ** - length:       length of a in digits
 **
 ** output:
 ** - a:      the result a / 2                              m_length
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNDiv2( CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actU8 first_carry, actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 ** void actBNMult2(actU8* a,
 **                 const actBNLENGTH length)
 **
 **  Multiplication by 2 ( a *= 2  ).
 **
 ** input:
 ** - a:            number to be multiplied
 ** - length:       length of a in digits
 **
 ** output:
 ** - a:      the result a * 2
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMult2( CRYPTOCV_P2VAR_PARA( actU8 ) a, actBNLENGTH length );

/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNReduce(actBNDIGIT* a,
 **                  actBNLENGTH a_length,
 **                  actBNDIGIT* b,
 **                  actBNLENGTH b_length,
 **                  actBNDIGIT* r,
 **                  void (*watchdog) (void));
 **
 **   BigNum reduce ( r = a mod b ).
 **
 ** input:                                              (length in digits)
 ** - a:       first number (will be distroyed)              a_length+1
 ** - b:       second number (will be returned const         b_length
 **            but touched by the normalization)
 ** - r:       result buffer                                 b_length+1
 **
 ** output:
 ** - a:       the normalized result a mod b                 b_length
 ** - b:       second number                                 b_length
 ** - r:       the result a mod b                            b_length
 ** - returns: -1    if exact b_length <= 1 (error)
 **             0    else (success)
 **
 ** assumes:
 **
 ** remark:
 ** - a, b and r cannot overlap
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNReduce(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, actBNLENGTH a_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) b, actBNLENGTH b_length,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) r, CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModAdd(const actBNDIGIT* a,
 **                   const actBNDIGIT* b,
 **                   actBNDIGIT* c,
 **                   const actBNRING* Ring)
 **
 **  Modular Addition ( c = (a+b) mod m, m=Ring->m ).
 **
 ** input:                                              (length in digits)
 ** - a:      first ring element                            m_length
 ** - b:      second ring element                           m_length
 ** - c:      ring element to store the result              m_length
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the modulus m                         m_length
 ** - Ring->m_length: equals to m_length
 **
 ** output:
 ** - c:      the result a + b mod m                        m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Field' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 **
 ** remark:
 ** - c may overlap with a or b ( a+=b or b+=a is possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModAdd(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModSub(const actBNDIGIT* a,
 **                   const actBNDIGIT* b,
 **                   actBNDIGIT* c,
 **                   const actBNRING* Ring)
 **
 **  Modular Subtraction ( c = (a-b) mod m, m=Ring->m ).
 **
 ** input:                                              (length in digits)
 ** - a:      first ring element                            m_length
 ** - b:      second ring element                           m_length
 ** - c:      ring element to store the result              m_length
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the modulus m                         m_length
 ** - Ring->m_length: equals to m_length
 **
 ** output:
 ** - c:      the result a - b mod m                        m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Field' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 **
 ** remark:
 ** - c may overlap with a or b ( a-=b or b-=a is possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModSub(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) b,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) c, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModDiv2(actBNDIGIT* a,
 **                    const actBNRING* Ring)
 **
 **  Modular Division by 2 ( a /= 2 mod m, m=Ring->m ).
 **
 ** input:                                              (length in digits)
 ** - a:      ring element to be divided                    m_length
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the modulus m                         m_length
 ** - Ring->m_length: equals to m_length
 **
 ** output:
 ** - a:      the result a / 2 mod m                        m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Field' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModDiv2(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) a, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMontMul(const actBNDIGIT* x,
 **                    const actBNDIGIT* y,
 **                    actBNDIGIT* A,
 **                    const actBNRING* Ring)
 **
 **  Montgomery Multiplication ( A = (x*y*R^-1) mod m, m=Ring->m is odd ).
 **  Implementation Reference: MOV'97, algo 14.36
 **
 ** input:                                              (length in digits)
 ** - x:      first ring element                            m_length
 ** - y:      second ring element                           m_length
 ** - A:      ring element to store the result              m_length+1
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the odd(!) modulus m                  m_length
 ** - Ring->m_length: equals to m_length
 ** - Ring->m_bar:    -(m^-1) mod base                      1
 ** - Ring->prime_structure==curveid if m is the curve prime
 ** - Ring->prime_structure==0       else
 **
 ** output:
 ** - A:      the product (x*y*R^-1) mod m                  m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Field' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 ** - Ring->m must be odd
 **
 ** remark:
 ** - A cannot overlap with x or y ( x*=y or y*=x is not possible )
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMontMul(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) A, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNMontMulCopy(actBNDIGIT* x,
 **                        const actBNDIGIT* y,
 **                        actBNDIGIT* A,
 **                        const actBNRING* Ring)
 **
 **  Montgomery Multiplication ( see actBNMontMul, the result is copied
 **  to x ).
 **
 ** input:                                              (length in digits)
 ** - see actBNMontMul
 **
 ** output:
 ** - A:      the product (x*y*R^-1) mod m                  m_length
 ** - x:      the product (x*y*R^-1) mod m                  m_length
 **
 ** assumes:
 ** - see actBNMontMul
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNMontMulCopy(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) y,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) A, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNModExp(const actBNDIGIT* gR,
 **                   const actBNDIGIT* e,
 **                   actBNDIGIT* AR,
 **                   const actBNRING* Ring,
 **                   actBNDIGIT* t[])
 **
 **  Modular Exponentiation ( AR = gR^e mod m, m=Ring->m ).
 **  The in- and output is in Montgomery Representation.
 **
 ** input:                                              (length in digits)
 ** - gR:     basis                                         m_length
 ** - e:      exponent ( e > 0 )                            m_length
 ** - AR:     ring element to store the result              m_length+1
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the odd(!) modulus m                  m_length
 ** - Ring->m_length: equals to m_length
 ** - Ring->RR:       constant for Montgomery               m_length
 ** - Ring->m_bar:    constant for Montgomery               1
 ** - Ring->prime_structure==curveid if m is the curve prime
 ** - Ring->prime_structure==0       else
 ** - Ring->window_size (=:k)  for the sliding window algorithm
 ** - t[0..(i-1)]:  i temporary BigNum variables,           i*(m_length+1)
 **                 where i = 2^(k-1)
 **
 ** output:
 ** - AR:     the power gR^e mod m                          m_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Ring' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 ** - Ring->m must be odd
 ** - e > 0
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNModExp(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) gR, CRYPTOCV_P2CONST_PARA( actBNDIGIT ) e,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) AR, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t[], CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) );

/****************************************************************************
 **
 ** FUNCTION:
 **  int actBNModRandomize(actBNDIGIT* x,
 **                        const actBNRING* Ring,
 **                        actBNDIGIT* t[])
 **
 **  Modular Randomization ( x will randomized with 0 < x < m, m=Ring->m ).
 **
 ** input:                                              (length in digits)
 ** - x:      ring element to store the result              m_length
 ** - Ring:   the underlying ring (see assumptions)
 ** - Ring->m:        the odd(!) modulus m                  m_length
 ** - Ring->m_length: equals to m_length
 ** - Ring->RR:       constant for Montgomery               m_length
 ** - Ring->m_bar:    constant for Montgomery               1
 ** - Ring->prime_structure==curveid if m is the curve prime
 ** - Ring->prime_structure==0       else
 ** - t[0,1]:  2 temporary BigNum variables,                m_length+1
 **
 ** output:
 ** - x:     random x with 0 < x < m                        m_length
 ** - returns: 0 or errorcode of the get random callback function
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Ring' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 ** - Ring->m must be odd
 **
 ***************************************************************************/
CRYPTOCV_FUNC( int ) actBNModRandomize(
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) x, CRYPTOCV_P2CONST_PARA( actBNRING ) Ring,
   CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t[] );

/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNFieldInversion(const actBNDIGIT* gR,
 **                           actBNDIGIT* g_invR,
 **                           const actBNRING* Field,
 **                           actBNDIGIT* t[]))
 **
 **  Field inversion ( g_invR = gR^-1 mod p, p=Ring->m must be prime ).
 **  The in- and output is in Montgomery Representation.
 **
 ** input:                                              (length in digits)
 ** - gR:     field element to be inverted (gR != 0)        p_length
 ** - g_invR: field element to store the result             p_length+1
 ** - Field:  the underlying field GF(p)  (see assumptions)
 ** - Field->m:        the prime p                          p_length
 ** - Field->m_length: equals to p_length
 ** - Field->RR:       constant for Montgomery              p_length
 ** - Field->m_bar:    constant for Montgomery              1
 ** - Field->prime_structure==curveid if p is the curve prime
 ** - Field->prime_structure==0       else
 ** if 'actEUCLID' is defined:
 ** - t[0..4]:  5 temporary BigNum variables                5*(p_length+1)
 ** else:
 ** - Field->window_size (=:k)  for modular exponentiation
 ** - t[0..i]:  i+1 temporary BigNum variables,         (i+1)*(p_length+1)
 **                 where i = 2^(k-1)
 **
 ** output:
 ** - g_invR: the result gR^-1 mod m                        p_length
 **
 ** assumes:
 ** - The actBNRING structure parameter 'Field' holds all necessary
 **   information. It has to be initialized as far as listed above.
 **   Please have a look at the actBNRING definition; an example for
 **   a complete initialization is the actECDSAVerify(..) implementation
 **   in actIECDSA.c.
 ** - Field->m must be prime
 ** - gR != 0
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNFieldInversion(
   CRYPTOCV_P2CONST_PARA( actBNDIGIT ) gR, CRYPTOCV_P2VAR_PARA( actBNDIGIT ) g_invR,
   CRYPTOCV_P2CONST_PARA( actBNRING ) Field, CRYPTOCV_P2VAR_PARA( actBNDIGIT ) t[] );


/****************************************************************************
 **
 ** FUNCTION:
 **  void actBNOddInvModBase(actBNDIGIT *inverse, actBNDIGIT value)
 **
 **   Inversion of an odd digit mod base.
 **
 ** input:                                              (length in digits)
 ** - value:      odd digit to be inverted                    1
 ** - inverse     result digit                                1
 **
 ** output:
 ** - inverse     value^-1 mod 2^bits_per_digit               1
 **
 ** assumes:
 **
 ** remark:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( void ) actBNOddInvModBase( CRYPTOCV_P2VAR_PARA( actBNDIGIT ) inverse, actBNDIGIT value );


#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* ACTBIGNUM_H */
