/****************************************************************************
 ** Main author: Ubr                     Creation date: 10/17/03
 ** $Author:: mns                      $ $JustDate:: 03/18/05             $
 ** $Workfile:: actSHA.c               $ $Revision:: 1435                 $
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
 ** This file contains: Aval SHA-1 implementation.
 **
 ** constants:
 **  actHASH_BLOCK_SIZE_SHA
 **  actSHA_WORD_SIZE
 **
 ** types:
 **
 ** macros:
 **   bit rotation and basic SHA-1 transformation macros (see bellow)
 **
 ** local functions:
 **   actSHATransform
 **
 ** global functions:
 **   actSHAInit
 **   actSHAUpdate
 **   actSHAFinalize
 **
 ***************************************************************************/


#include "actISHA.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actSHA_WORD_SIZE     4


/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/* The SHA f()-functions */
#define f1(x,y,z) ((z)^((x)&((y)^(z))))
#define f2(x,y,z) ((x)^(y)^(z))
#define f3(x,y,z) (((x)&(y))|((z)&((x)|(y))))
#define f4(x,y,z) ((x)^(y)^(z))

/* blk0(), blk1() functions working on actSHATransform() local workspace 'W' */
#define blk0(W, i) ((W)[(i)])
#if ( actSHA_SPEED_UP>2 )
   /* blk1() as macro */
   #define blk1(W, i) ((W)[(i)&15u] = actRotl((W)[((i)+13u)&15u]^(W)[((i)+8u)&15u]^(W)[((i)+2u)&15u]^(W)[(i)&15u],1u))
#else
   /* blk1() as function */
   STATIC CRYPTOCV_FUNC( actU32 ) blk1( CRYPTOCV_P2VAR_PARA( actU32 ) W,
                                                       unsigned int i )
   {
      actU32 t = W[(i+13u)&15u] ^ W[(i+8u)&15u] ^ W[(i+2u)&15u] ^ W[i&15u];
      t = (t << 1) | ((t & 0x80000000UL)? 1:0);
      return W[i&15u] = t;
   }
#endif

/* SHA-1 bit rotate functions */
#if ( actSHA_SPEED_UP>0 )
   #define actRotl5(x) ( actRotl((x),5) )
   #define actRotl30(x) ( actRotl((x),30) )
#else
   STATIC CRYPTOCV_FUNC( actU32 ) actRotl5(actU32 x)
   {
      actU8 t_hi5 = (actU8)actGetByte(x, 3);
      t_hi5 >>= 3;
      return (x << 5) | (actU32)t_hi5;
   }
   STATIC actU32 actRotl30(actU32 x)
   {
      actU8 t_lo2 = (actU8)actGetByte(x, 0);
      t_lo2 <<= 6;
      return ((actU32)t_lo2 << 24) | (x >> 2);
   }
#endif


/* R1, R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) ((z)+=f1((w),(x),(y))+blk0(W,(i))+0x5A827999L+actRotl5((v)))
#define R1(v,w,x,y,z,i) ((z)+=f1((w),(x),(y))+blk1(W,(i))+0x5A827999L+actRotl5((v)))
#define R2(v,w,x,y,z,i) ((z)+=f2((w),(x),(y))+blk1(W,(i))+0x6ED9EBA1L+actRotl5((v)))
#define R3(v,w,x,y,z,i) ((z)+=f3((w),(x),(y))+blk1(W,(i))+0x8F1BBCDCL+actRotl5((v)))
#define R4(v,w,x,y,z,i) ((z)+=f4((w),(x),(y))+blk1(W,(i))+0xCA62C1D6L+actRotl5((v)))

#define RL(w)           ((w)=actRotl30((w)))

/****************************************************************************
 ** Main local function: hash one 64 byte block of data
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** static void actSHATransform(const actU8 *in, actU32 *H)
 **
 **  This function hashes one 64 byte block of data.
 **
 ** input:
 ** - info:       pointer to hash context structure
 **
 ** output:
 ** - info:       actualized hash context structure
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
STATIC CRYPTOCV_FUNC( void ) actSHATransform(
   CRYPTOCV_P2CONST_PARA( actU8 ) in, CRYPTOCV_P2VAR_PARA( actU32 ) H,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actU32 W[16];
   unsigned int i=actSHA_SPEED_UP;
#if ( actSHA_SPEED_UP<2 )
   actU32 t;
#endif

   /* Copy H[] to working vars */
   actU32 Aval = H[0];
   actU32 Bval = H[1];
   actU32 Cval = H[2];
   actU32 Dval = H[3];
   actU32 Eval = H[4];

   actL1trigger( watchdog );

   /* Copy 64 byte block 'in' to workspace W[] in big endian format */
   for (i=0; i<16; ++i)
   {
      W[i] = (((actU32) in[i<<2]<<8 | in [(i<<2)+1])<<8 | in [(i<<2)+2])<<8 | in [(i<<2)+3];
   }

#if ( actSHA_SPEED_UP>1 )
   /* 4 rounds of 20 operations each. Loops unrolled. */
   /* round 1 */
   R0(Aval,Bval,Cval,Dval,Eval, 0); RL(Bval); actL3trigger( watchdog );   R0(Eval,Aval,Bval,Cval,Dval, 1); RL(Aval); actL3trigger( watchdog );   R0(Dval,Eval,Aval,Bval,Cval, 2); RL(Eval); actL3trigger( watchdog );   R0(Cval,Dval,Eval,Aval,Bval, 3); RL(Dval); actL3trigger( watchdog );
   R0(Bval,Cval,Dval,Eval,Aval, 4); RL(Cval); actL3trigger( watchdog );   R0(Aval,Bval,Cval,Dval,Eval, 5); RL(Bval); actL3trigger( watchdog );   R0(Eval,Aval,Bval,Cval,Dval, 6); RL(Aval); actL3trigger( watchdog );   R0(Dval,Eval,Aval,Bval,Cval, 7); RL(Eval); actL3trigger( watchdog );
   R0(Cval,Dval,Eval,Aval,Bval, 8); RL(Dval); actL3trigger( watchdog );   R0(Bval,Cval,Dval,Eval,Aval, 9); RL(Cval); actL3trigger( watchdog );   R0(Aval,Bval,Cval,Dval,Eval,10); RL(Bval); actL3trigger( watchdog );   R0(Eval,Aval,Bval,Cval,Dval,11); RL(Aval); actL3trigger( watchdog );
   R0(Dval,Eval,Aval,Bval,Cval,12); RL(Eval); actL3trigger( watchdog );   R0(Cval,Dval,Eval,Aval,Bval,13); RL(Dval); actL3trigger( watchdog );   R0(Bval,Cval,Dval,Eval,Aval,14); RL(Cval); actL3trigger( watchdog );   R0(Aval,Bval,Cval,Dval,Eval,15); RL(Bval); actL3trigger( watchdog );
   R1(Eval,Aval,Bval,Cval,Dval,16); RL(Aval); actL3trigger( watchdog );   R1(Dval,Eval,Aval,Bval,Cval,17); RL(Eval); actL3trigger( watchdog );   R1(Cval,Dval,Eval,Aval,Bval,18); RL(Dval); actL3trigger( watchdog );   R1(Bval,Cval,Dval,Eval,Aval,19); RL(Cval);
   actL2trigger( watchdog );

   /* round 2 */
   R2(Aval,Bval,Cval,Dval,Eval,20); RL(Bval); actL3trigger( watchdog );   R2(Eval,Aval,Bval,Cval,Dval,21); RL(Aval); actL3trigger( watchdog );   R2(Dval,Eval,Aval,Bval,Cval,22); RL(Eval); actL3trigger( watchdog );   R2(Cval,Dval,Eval,Aval,Bval,23); RL(Dval); actL3trigger( watchdog );
   R2(Bval,Cval,Dval,Eval,Aval,24); RL(Cval); actL3trigger( watchdog );   R2(Aval,Bval,Cval,Dval,Eval,25); RL(Bval); actL3trigger( watchdog );   R2(Eval,Aval,Bval,Cval,Dval,26); RL(Aval); actL3trigger( watchdog );   R2(Dval,Eval,Aval,Bval,Cval,27); RL(Eval); actL3trigger( watchdog );
   R2(Cval,Dval,Eval,Aval,Bval,28); RL(Dval); actL3trigger( watchdog );   R2(Bval,Cval,Dval,Eval,Aval,29); RL(Cval); actL3trigger( watchdog );   R2(Aval,Bval,Cval,Dval,Eval,30); RL(Bval); actL3trigger( watchdog );   R2(Eval,Aval,Bval,Cval,Dval,31); RL(Aval); actL3trigger( watchdog );
   R2(Dval,Eval,Aval,Bval,Cval,32); RL(Eval); actL3trigger( watchdog );   R2(Cval,Dval,Eval,Aval,Bval,33); RL(Dval); actL3trigger( watchdog );   R2(Bval,Cval,Dval,Eval,Aval,34); RL(Cval); actL3trigger( watchdog );   R2(Aval,Bval,Cval,Dval,Eval,35); RL(Bval); actL3trigger( watchdog );
   R2(Eval,Aval,Bval,Cval,Dval,36); RL(Aval); actL3trigger( watchdog );   R2(Dval,Eval,Aval,Bval,Cval,37); RL(Eval); actL3trigger( watchdog );   R2(Cval,Dval,Eval,Aval,Bval,38); RL(Dval); actL3trigger( watchdog );   R2(Bval,Cval,Dval,Eval,Aval,39); RL(Cval);
   actL2trigger( watchdog );

   /* round 3 */
   R3(Aval,Bval,Cval,Dval,Eval,40); RL(Bval); actL3trigger( watchdog );   R3(Eval,Aval,Bval,Cval,Dval,41); RL(Aval); actL3trigger( watchdog );   R3(Dval,Eval,Aval,Bval,Cval,42); RL(Eval); actL3trigger( watchdog );   R3(Cval,Dval,Eval,Aval,Bval,43); RL(Dval); actL3trigger( watchdog );
   R3(Bval,Cval,Dval,Eval,Aval,44); RL(Cval); actL3trigger( watchdog );   R3(Aval,Bval,Cval,Dval,Eval,45); RL(Bval); actL3trigger( watchdog );   R3(Eval,Aval,Bval,Cval,Dval,46); RL(Aval); actL3trigger( watchdog );   R3(Dval,Eval,Aval,Bval,Cval,47); RL(Eval); actL3trigger( watchdog );
   R3(Cval,Dval,Eval,Aval,Bval,48); RL(Dval); actL3trigger( watchdog );   R3(Bval,Cval,Dval,Eval,Aval,49); RL(Cval); actL3trigger( watchdog );   R3(Aval,Bval,Cval,Dval,Eval,50); RL(Bval); actL3trigger( watchdog );   R3(Eval,Aval,Bval,Cval,Dval,51); RL(Aval); actL3trigger( watchdog );
   R3(Dval,Eval,Aval,Bval,Cval,52); RL(Eval); actL3trigger( watchdog );   R3(Cval,Dval,Eval,Aval,Bval,53); RL(Dval); actL3trigger( watchdog );   R3(Bval,Cval,Dval,Eval,Aval,54); RL(Cval); actL3trigger( watchdog );   R3(Aval,Bval,Cval,Dval,Eval,55); RL(Bval); actL3trigger( watchdog );
   R3(Eval,Aval,Bval,Cval,Dval,56); RL(Aval); actL3trigger( watchdog );   R3(Dval,Eval,Aval,Bval,Cval,57); RL(Eval); actL3trigger( watchdog );   R3(Cval,Dval,Eval,Aval,Bval,58); RL(Dval); actL3trigger( watchdog );   R3(Bval,Cval,Dval,Eval,Aval,59); RL(Cval);
   actL2trigger( watchdog );

   /* round 4 */
   R4(Aval,Bval,Cval,Dval,Eval,60); RL(Bval); actL3trigger( watchdog );   R4(Eval,Aval,Bval,Cval,Dval,61); RL(Aval); actL3trigger( watchdog );   R4(Dval,Eval,Aval,Bval,Cval,62); RL(Eval); actL3trigger( watchdog );   R4(Cval,Dval,Eval,Aval,Bval,63); RL(Dval); actL3trigger( watchdog );
   R4(Bval,Cval,Dval,Eval,Aval,64); RL(Cval); actL3trigger( watchdog );   R4(Aval,Bval,Cval,Dval,Eval,65); RL(Bval); actL3trigger( watchdog );   R4(Eval,Aval,Bval,Cval,Dval,66); RL(Aval); actL3trigger( watchdog );   R4(Dval,Eval,Aval,Bval,Cval,67); RL(Eval); actL3trigger( watchdog );
   R4(Cval,Dval,Eval,Aval,Bval,68); RL(Dval); actL3trigger( watchdog );   R4(Bval,Cval,Dval,Eval,Aval,69); RL(Cval); actL3trigger( watchdog );   R4(Aval,Bval,Cval,Dval,Eval,70); RL(Bval); actL3trigger( watchdog );   R4(Eval,Aval,Bval,Cval,Dval,71); RL(Aval); actL3trigger( watchdog );
   R4(Dval,Eval,Aval,Bval,Cval,72); RL(Eval); actL3trigger( watchdog );   R4(Cval,Dval,Eval,Aval,Bval,73); RL(Dval); actL3trigger( watchdog );   R4(Bval,Cval,Dval,Eval,Aval,74); RL(Cval); actL3trigger( watchdog );   R4(Aval,Bval,Cval,Dval,Eval,75); RL(Bval); actL3trigger( watchdog );
   R4(Eval,Aval,Bval,Cval,Dval,76); RL(Aval); actL3trigger( watchdog );   R4(Dval,Eval,Aval,Bval,Cval,77); RL(Eval); actL3trigger( watchdog );   R4(Cval,Dval,Eval,Aval,Bval,78); RL(Dval); actL3trigger( watchdog );   R4(Bval,Cval,Dval,Eval,Aval,79); RL(Cval);
#else
   /* 4 rounds of 20 operations each. */
   /* round 1 */
   for ( i = 0; i < 16; ++i )
   {
      R0(Aval,Bval,Cval,Dval,Eval,i); t=Eval; Eval=Dval; Dval=Cval; Cval=actRotl30(Bval); Bval=Aval; Aval=t;
      actL3trigger( watchdog );
   }
   for ( ; i < 20; ++i )
   {
      R1(Aval,Bval,Cval,Dval,Eval,i); t=Eval; Eval=Dval; Dval=Cval; Cval=actRotl30(Bval); Bval=Aval; Aval=t;
      actL3trigger( watchdog );
   }
   actL2trigger( watchdog );

   /* round 2 */
   for ( ; i < 40; ++i )
   {
      R2(Aval,Bval,Cval,Dval,Eval,i); t=Eval; Eval=Dval; Dval=Cval; Cval=actRotl30(Bval); Bval=Aval; Aval=t;
      actL3trigger( watchdog );
   }
   actL2trigger( watchdog );

   /* round 3 */
   for ( ; i < 60; ++i )
   {
      R3(Aval,Bval,Cval,Dval,Eval,i); t=Eval; Eval=Dval; Dval=Cval; Cval=actRotl30(Bval); Bval=Aval; Aval=t;
      actL3trigger( watchdog );
   }
   actL2trigger( watchdog );

   /* round 4 */
   for ( ; i < 80; ++i )
   {
      R4(Aval,Bval,Cval,Dval,Eval,i); t=Eval; Eval=Dval; Dval=Cval; Cval=actRotl30(Bval); Bval=Aval; Aval=t;
      actL3trigger( watchdog );
   }
#endif

   actL1trigger( watchdog );

   /* Add the working vars back into state H[] */
   H[0] += Aval;
   H[1] += Bval;
   H[2] += Cval;
   H[3] += Dval;
   H[4] += Eval;
}



/****************************************************************************
 ** Global Functions
 ***************************************************************************/


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAInit(actSHASTRUCT* info)
 **
 **  This function initializes the SHA-1 algorithm.
 **
 ** input:
 ** - info:       pointer to hash context structure
 **
 ** output:
 ** - info:       initialized hash context structure
 ** - returns:    actOK allways
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAInit( CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info )
{
   /* init message digest */
   info->H[0] = 0x67452301L;
   info->H[1] = 0xEFCDAB89L;
   info->H[2] = 0x98BADCFEL;
   info->H[3] = 0x10325476L;
   info->H[4] = 0xC3D2E1F0L;
   /* init count variables */
   info->low_count = info->hi_count = info->buffer_used = 0;

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAUpdate(actSHASTRUCT* info,
 **                            const actU8* dataIN,
 **                            int length,
 **                            void (*watchdog) (void))
 **
 **  This function hashes the given data and can be called arbitrary
 **  often between an initialize and finalize of the SHA-1 algorithm.
 **  Uses any data already in the actSHASTRUCT structure and leaves
 **  any partial data block there.
 **
 ** input:
 ** - info:       pointer to hash context structure
 ** - dataIN:     pointer to data to be hashed
 ** - length:     length of data in bytes
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       actualized hash context structure
 ** - returns:    actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 **               actOK                 else
 **
 ** assumes:
 ** - actSHAInit() is called once before calling this function
 ** - dataIN != NULL is a valid pointer
 ** - length >= 0
 **
 ** uses:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAUpdate(
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN, int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   CRYPTOCV_P2VAR_PARA( actU32 ) H = info->H;
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer = info->buffer;
   int diff=0, blocks, bcount;

   actL1trigger( watchdog );

   /* update byte count */
   if( (info->low_count + (actU32)length) < info->low_count )
   {
      if ( (++info->hi_count) == ((actU32)1 << 29) ) /* add carry */
      {
         return actEXCEPTION_LENGTH; /* overflow: 2^64 bit input */
      }
   }
   info->low_count += (actU32)length;

   /* left overs of the last update */
   if ( info->buffer_used > 0 )
   {
      diff = actMin(length, (int)(actHASH_BLOCK_SIZE_SHA-info->buffer_used));
      actMemcpy(buffer+info->buffer_used, dataIN, (unsigned int)diff);
      info->buffer_used += diff;
      if( info->buffer_used<actHASH_BLOCK_SIZE_SHA )
      {
         return actOK;
      }
      else
      {
         actSHATransform(buffer, H, watchdog);
      }
   }

   /* complete blocks */
   blocks = (length-diff)/actHASH_BLOCK_SIZE_SHA;
   dataIN += diff;
   for ( bcount = 0; bcount < blocks; ++bcount, dataIN += actHASH_BLOCK_SIZE_SHA )
   {
      actSHATransform(dataIN, H, watchdog);
   }

   /* copy left overs into buffer */
   info->buffer_used = length - (diff + actHASH_BLOCK_SIZE_SHA*blocks);
   actMemcpy( buffer, dataIN, (unsigned int)(info->buffer_used) );

   actL1trigger( watchdog );

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHAFinalize(actSHASTRUCT* info,
 **                              actU8 hash[actHASH_SIZE_SHA],
 **                              void (*watchdog) (void))
 **
 **  This function finalizes the SHA algorithm and delivers the hash value.
 **
 ** input:
 ** - info:       pointer to hash context structure
 ** - hash:       byte array to contain the hash value
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       finalized hash context structure
 ** - hash:       the final hash value,
 **                  (big endian of length actHASH_SIZE_SHA)
 ** - returns:    actOK allways
 **
 ** assumes:
 ** - actSHAInit() is called once before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHA
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHAFinalize(
   CRYPTOCV_P2VAR_PARA( actSHASTRUCT ) info, CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog )( void ) )
{
   actU32 low_bitcount = info->low_count << 3;
   actU32 hi_bitcount = (info->hi_count << 3) | (info->low_count >> 29);
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer = info->buffer;
   CRYPTOCV_P2VAR_PARA( actU32 ) H = info->H;
   int len = info->low_count & 0x3F;
   int pad_len = actHASH_BLOCK_SIZE_SHA-2*actSHA_WORD_SIZE;

   actL1trigger( watchdog );

   /* pad byte '0x80' */
   buffer[len++] = 0x80;

   /* pad zeros */
   actMemset(buffer+len, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA-len));
   if( len > pad_len ) {
      actSHATransform( (CRYPTOCV_P2CONST_PARA( actU8 ))buffer, H, watchdog );
      actMemset( buffer, 0, (unsigned int)pad_len );
   }

   /* pad 64-bit input count to last 8 bytes in big endian format */
#ifdef LITTLE_ENDIAN
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len])) = actSwap(hi_bitcount);
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len+actSHA_WORD_SIZE])) = actSwap(low_bitcount);
#else
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len])) = hi_bitcount;
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len+actSHA_WORD_SIZE])) = low_bitcount;
#endif

   /* transform last block */
   actSHATransform( (CRYPTOCV_P2CONST_PARA( actU8 ))buffer, H, watchdog );

   /* convert big endian SHA-1-Digest to native byte order */
#ifdef LITTLE_ENDIAN
   for (len=0; len<actHASH_SIZE_SHA/actSHA_WORD_SIZE; len++) {
      H[len] = actSwap(H[len]);
   }
#endif

   /* write message digest to output 'hash' */
   actMemcpy( hash, (CRYPTOCV_P2CONST_PARA( actU8 ))(&H[0]), actHASH_SIZE_SHA );

   /* wipe buffer */
   actMemset( info->buffer, 0, actHASH_BLOCK_SIZE_SHA );

   actL1trigger( watchdog );

   return actOK;
}
