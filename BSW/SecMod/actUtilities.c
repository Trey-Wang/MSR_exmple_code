/****************************************************************************
 ** Main author: Ubr                     Creation date: 01/28/04
 ** $Author:: men                      $ $JustDate:: 01/28/04             $
 ** $Workfile:: actUtilities.c         $ $Revision:: 1620                 $
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
 **     Layer: Core Module
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: Some usefull functions.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   actMin, actMax
 **
 ** global functions:
 **   actMemset
 **   actMemcpy
 **
 ***************************************************************************/


#include "actUtilities.h"


/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/* memset */
CRYPTOCV_FUNC( void ) actMemset( CRYPTOCV_P2VAR_PARA( void ) dest, actU8 fill, unsigned int count )
{
   CRYPTOCV_P2VAR_PARA(actU8) dest_tmp = dest;
   for ( ; count > 0; --count )
   {
      dest_tmp[count - 1] = fill;
   }
}

/* memcpy */
CRYPTOCV_FUNC( void ) actMemcpy( CRYPTOCV_P2VAR_PARA( void ) dest, CRYPTOCV_P2CONST_PARA( void ) src,
   unsigned int count )
{
   CRYPTOCV_P2VAR_PARA(actU8) dest_tmp = dest;
   CRYPTOCV_P2CONST_PARA(actU8) src_tmp = src;
   for ( ; count > 0; --count )
   {
      dest_tmp[count - 1] = src_tmp[count - 1];
   }
}

CRYPTOCV_FUNC( void ) actMemcpy_ROM( CRYPTOCV_P2VAR_PARA( void ) dest, CRYPTOCV_P2CONST_PARA( void ) src, unsigned int count)
{
   CRYPTOCV_P2VAR_PARA(actU8) dest_tmp = dest;
   CRYPTOCV_P2CONST_PARA(actU8) src_tmp = src;
   for (; count>0; --count) {
     dest_tmp[count - 1] = ACT_ROM_BYT(&src_tmp[count - 1]);
   }
}

CRYPTOCV_FUNC( void ) actMemcpy_ROMRAM( CRYPTOCV_P2VAR_PARA( void ) dest, actPROMU8 src, unsigned int count )
{
   CRYPTOCV_P2VAR_PARA(actU8) dest_tmp = dest;
   for ( ; count > 0; --count )
   {
      dest_tmp[count - 1] = src[count-1];
   }
}

/* memcmp */
CRYPTOCV_FUNC( int ) actMemcmp( CRYPTOCV_P2CONST_PARA( void ) buf1, CRYPTOCV_P2CONST_PARA( void ) buf2,
   unsigned int count )
{
   CRYPTOCV_P2CONST_PARA(actU8) buf1_tmp = buf1;
   CRYPTOCV_P2CONST_PARA(actU8) buf2_tmp = buf2;
   while ( (count > 0) && (buf1_tmp[count - 1] == buf2_tmp[count - 1]) )
   {
      --count;
   }
   return count ? ((int)(buf1_tmp[count - 1] - buf2_tmp[count - 1])) : 0;
}


CRYPTOCV_FUNC( int ) actMemcmp_ROM ( actPROMU8 buf1, actPROMU8 buf2, unsigned int count )
{
   while ( (count > 0) && (ACT_ROM_BYT(&buf1[count - 1]) == ACT_ROM_BYT(&buf2[count - 1])) )
   {
      --count;
   }
   return count ? ((int)(ACT_ROM_BYT(&buf1[count - 1]) - ACT_ROM_BYT(&buf2[count - 1]))) : 0;
}

CRYPTOCV_FUNC( int ) actMemcmp_ROMRAM( CRYPTOCV_P2CONST_PARA( void ) buf1, actPROMU8 buf2, unsigned int count )
{
  CRYPTOCV_P2CONST_PARA(actU8) buf1_tmp = buf1;
   while ( (count > 0) && (buf1_tmp[count-1] == ACT_ROM_BYT(&buf2[count-1])) )
   {
      --count;
   }
   return count ? ((int)(buf1_tmp[count - 1] - ACT_ROM_BYT(&buf2[count - 1]))) : 0;
}

/* XOR (buf1 ^= buf2) */
CRYPTOCV_FUNC( void ) actXOR( CRYPTOCV_P2VAR_PARA( void ) buf1, CRYPTOCV_P2CONST_PARA( void ) buf2,
   unsigned int count )
{
   CRYPTOCV_P2VAR_PARA(actU8) buf1_tmp = buf1;
   CRYPTOCV_P2CONST_PARA(actU8) buf2_tmp = buf2;
   for ( ; count > 0; --count)
   {
      buf1_tmp[count-1] ^= buf2_tmp[count-1];
   }
}

/* long to (big endian) byte conversion */
CRYPTOCV_FUNC( void ) actLongToByte( CRYPTOCV_P2VAR_PARA( void ) buf, const actU32 x )
{
   CRYPTOCV_P2VAR_PARA(actU8) buf_tmp = buf;
   buf_tmp[0] = (actU8)(x>>24);
   buf_tmp[1] = (actU8)(x>>16);
   buf_tmp[2] = (actU8)(x>>8);
   buf_tmp[3] = (actU8)(x);
}
