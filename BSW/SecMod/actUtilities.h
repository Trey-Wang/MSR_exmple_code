/****************************************************************************
 ** Main author: Ubr                     Creation date: 01/28/04
 ** $Author:: mgnida                   $ $JustDate:: 03/18/05             $
 ** $Workfile:: actUtilities.h         $ $Revision:: 2019                 $
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
 ** This file contains: Interface for some usefull functions.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   actMin, actMax
 **
 ** functions:
 **   actMemset
 **   actMemcpy
 **
 ***************************************************************************/


#ifndef ACTUTILITIES_H
#define ACTUTILITIES_H


#include "actITypes.h"
#include "actConfig.h"


/****************************************************************************
 ** Macros
 ***************************************************************************/


#define actMin(x, y) ( ((x)<(y))? (x):(y) )
#define actMax(x, y) ( ((x)>(y))? (x):(y) )

/* get a byte out of a long */
#ifndef OPTIMIZE_actGetByte
   #define actGetByte(x, n)  ( ((x)>>((n)<<3)) & 0xFFul )
#else
   /* You may try this one also. The version may result in CODE size or
   performance benefits. CAUTION: With some compilers, this macro does
   not work correctly! */
   /* MUST use this version with TI's CCS ARM-7 compiler */
   #ifdef LITTLE_ENDIAN
      #define actGetByte(x, n)  ( *(((CRYPTOCV_P2VAR_PARA(actU8))&(x))+(n)) )
   #else
      #define actGetByte(x, n)  ( *(((CRYPTOCV_P2VAR_PARA(actU8))&(x))+3-(n)) )
   #endif
#endif

/* rotate functions for dwords */
#ifdef _MSC_VER
   #include <stdlib.h>
   #pragma intrinsic(_lrotr,_lrotl)
   #define actRotr(x,n) ( _lrotr((x),(n)) )
   #define actRotl(x,n) ( _lrotl((x),(n)) )
#else
   #define actRotr(x,n) ( ((x) >> ((int)(n))) | ((x) << (32 - (int)(n))) )
   #define actRotl(x,n) ( ((x) << ((int)(n))) | ((x) >> (32 - (int)(n))) )
#endif

/* rotate functions for words */
#define actRotr16(x,n) ( ((x) >> ((int)(n))) | ((x) << (16 - (int)(n))) )
#define actRotl16(x,n) ( ((x) << ((int)(n))) | ((x) >> (16 - (int)(n))) )

/* rotate functions for qwords */
#define actRotr64(x,n) ( ((x) >> ((int)(n))) | ((x) << (64 - (int)(n))) )
#define actRotl64(x,n) ( ((x) << ((int)(n))) | ((x) >> (64 - (int)(n))) )

/* byte swap for dwords */
#define actSwap(x) ((actRotr((x), 8) & 0xff00ff00) | (actRotl((x), 8) & 0x00ff00ff))

/* byte swap for qwords */
#define actSwap64(x) ((((actU64) actSwap((x) & 0xFFFFFFFF)) << 32) | ((actU64) (actSwap((x) >> 32))))

/* byte to qwords */
#define actU8toU64(x) \
   (((actU64)((x)[0])) | ((actU64)((x)[1]) << 8) | \
    ((actU64)((x)[2]) << 16) | ((actU64)((x)[3]) << 24) | \
    ((actU64)((x)[4]) << 32) | ((actU64)((x)[5]) << 40) | \
    ((actU64)((x)[6]) << 48) | ((actU64)((x)[7]) << 56))

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/* memset */
CRYPTOCV_FUNC( void ) actMemset( CRYPTOCV_P2VAR_PARA( void ) dest, actU8 fill, unsigned int count );

/* memcpy */
CRYPTOCV_FUNC( void ) actMemcpy( CRYPTOCV_P2VAR_PARA( void ) dest, CRYPTOCV_P2CONST_PARA( void ) src,
   unsigned int count );
CRYPTOCV_FUNC( void ) actMemcpy_ROM( CRYPTOCV_P2VAR_PARA( void ) dest, CRYPTOCV_P2CONST_PARA( void ) src, unsigned int count);

CRYPTOCV_FUNC( void ) actMemcpy_ROMRAM( CRYPTOCV_P2VAR_PARA( void ) dest, actPROMU8 src, unsigned int count );

/* memcmp */
CRYPTOCV_FUNC( int ) actMemcmp( CRYPTOCV_P2CONST_PARA( void ) buf1, CRYPTOCV_P2CONST_PARA( void ) buf2,
   unsigned int count );

CRYPTOCV_FUNC( int ) actMemcmp_ROM ( actPROMU8 buf1, actPROMU8 buf2, unsigned int count );

CRYPTOCV_FUNC( int ) actMemcmp_ROMRAM( CRYPTOCV_P2CONST_PARA( void ) buf1, actPROMU8 buf2, unsigned int count );

/* XOR (buf1 ^= buf2) */
CRYPTOCV_FUNC( void ) actXOR( CRYPTOCV_P2VAR_PARA( void ) buf1, CRYPTOCV_P2CONST_PARA( void ) buf2,
   unsigned int count );

/* (big endian) byte to long conversion */
CRYPTOCV_FUNC( actU32 ) actByteToLong( CRYPTOCV_P2VAR_PARA( void ) buf );

/* long to (big endian) byte conversion */
CRYPTOCV_FUNC( void ) actLongToByte( CRYPTOCV_P2VAR_PARA( void ) buf, const actU32 x );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTUTILITIES_H */
