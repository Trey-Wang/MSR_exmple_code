/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/actCLib/source/actSHA2-32.c $
 ** $Revision: 2277 $
 ** $Date: 2016-02-10 13:54:05 +0100 (Mi, 10 Feb 2016) $
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
 ** Core SHA-224 and SHA-256 implementation.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   bit rotation and basic SHA-2 transformation macros (see bellow)
 **
 ** local functions:
 **   actSHA2_32_Transform
 **   actSHA2_32_Update
 **   actSHA2_32_Finalize
 **
 ** global functions:
 **   actSHA224Init
 **   actSHA224Update
 **   actSHA224Finalize
 **   actSHA256Init
 **   actSHA256Update
 **   actSHA256Finalize
 **
 ***************************************************************************/


#include "actISHA2_32.h"
#include "actUtilities.h"
#include "actConfig.h"
#include "actWatchdog.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actSHA256_WORD_SIZE     4


/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/* The SHA2_32 functions */
#define S0(x) (actRotr((x),2)^actRotr((x),13)^actRotr((x),22))
#define S1(x) (actRotr((x),6)^actRotr((x),11)^actRotr((x),25))
#define s0(x) (actRotr((x),7)^actRotr((x),18)^((x)>>3))
#define s1(x) (actRotr((x),17)^actRotr((x),19)^((x)>>10))

/* basic macros */
#define blk0(i) (W[(i)])
#define blk2(i) (W[(i)&15]+=s1(W[((i)-2)&15])+W[((i)-7)&15]+s0(W[((i)-15)&15]))

#define Ch(x,y,z) ((z)^((x)&((y)^(z))))
#define Maj(x,y,z) (((x)&(y))|((z)&((x)|(y))))

#define a(i) (T[(0-(i))&7])
#define b(i) (T[(1-(i))&7])
#define c(i) (T[(2-(i))&7])
#define d(i) (T[(3-(i))&7])
#define e(i) (T[(4-(i))&7])
#define f(i) (T[(5-(i))&7])
#define g(i) (T[(6-(i))&7])
#define h(i) (T[(7-(i))&7])

#define Ra(i,j) (h(i)+=S1(e(i))+Ch(e(i),f(i),g(i))+ACT_ROM_R_DWD(K[(i)+(j)])+((j)?blk2(i):blk0(i)))
#define Rb(i)   (d(i)+=h(i))
#define Rc(i)   (h(i)+=S0(a(i))+Maj(a(i),b(i),c(i)))


/****************************************************************************
 ** Main local function: hash one 64 byte block of data
 ***************************************************************************/

CRYPTOCV_CONST( STATIC, actU32) K[64] = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/****************************************************************************
 **
 ** FUNCTION:
 ** static void actSHA2_32_Transform(const actU8 *in, actU32 *H, void (* watchdog) (void))
 **
 **  This function hashes one 64 byte block of data.
 **
 ** input:
 ** - in:         input data
 ** - H:          current hash value (state)
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - H:          updated hash value (state)
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
static CRYPTOCV_FUNC( void ) actSHA2_32_Transform (
   CRYPTOCV_P2CONST_PARA( actU8 ) in,
   CRYPTOCV_P2VAR_PARA( actU32 ) H,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   actU32 W[16];
   actU32 T[8];
   actU32 i, j;

   /* Copy state to working vars */
   for (i=0; i<8; ++i) { T[i] = H[i]; }

   actL3trigger (watchdog);

   /* Copy 64 byte block 'in' to workspace W[] in big endian format */
   for (i=0; i<16; ++i) { \
      W[i] = (((actU32) in[i<<2]<<8 | in [(i<<2)+1])<<8 | in [(i<<2)+2])<<8 | in [(i<<2)+3]; \
   }

   actL2trigger( watchdog );

   /* 64 operations of 16 rounds */
   for (j=0; j<64; j+=16)
   {
#if ( actSHA256_SPEED_UP>0 )
      /* rounds unrolled */
      Ra( 0u,j); Rb( 0u); Rc( 0u);   Ra( 1u,j); Rb( 1u); Rc( 1u);
      Ra( 2u,j); Rb( 2u); Rc( 2u);   Ra( 3u,j); Rb( 3u); Rc( 3u);
      Ra( 4u,j); Rb( 4u); Rc( 4u);   Ra( 5u,j); Rb( 5u); Rc( 5u);
      Ra( 6u,j); Rb( 6u); Rc( 6u);   Ra( 7u,j); Rb( 7u); Rc( 7u);
      Ra( 8u,j); Rb( 8u); Rc( 8u);   Ra( 9u,j); Rb( 9u); Rc( 9u);
      Ra(10u,j); Rb(10u); Rc(10u);   Ra(11u,j); Rb(11u); Rc(11u);
      Ra(12u,j); Rb(12u); Rc(12u);   Ra(13u,j); Rb(13u); Rc(13u);
      Ra(14u,j); Rb(14u); Rc(14u);   Ra(15u,j); Rb(15u); Rc(15u);
      actL3trigger( watchdog );
#else
      for (i = 0; i < 16; ++i)
      {
         Ra(i,j);
         Rb(i);
         Rc(i);
         actL3trigger( watchdog );
      }
#endif
      actL2trigger (watchdog);
   }

   actL1trigger (watchdog);

   /* Add the working vars back into state H[] */
   for (i = 0; i < 8; ++i)
   {
      H[i] += T[i];
   }
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHA2_32_Update (actSHA256STRUCT*  info,
 **                            const actU8*            dataIN,
 **                                  int               length,
 **                                  void            (*watchdog) (void))
 **
 **  This function hashes the given data and can be called arbitrarily
 **  often between an initialize and finalize of the SHA-2 algorithm.
 **  Uses any data already in the actSHA256STRUCT structure and leaves
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
 ** - actSHAxxxInit() has been called before calling this function
 ** - dataIN != NULL is a valid pointer
 ** - length >= 0
 **
 ** uses:
 **
 ***************************************************************************/
static CRYPTOCV_FUNC( actRETURNCODE ) actSHA2_32_Update (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   CRYPTOCV_P2VAR_PARA( actU32 ) H = info->H;
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer = info->buffer;
   int diff=0, blocks, bcount;

   actL1trigger (watchdog);

   /* update byte count */
   if( (info->low_count + (actU32)length) < info->low_count ) {
      if ( (++info->hi_count)==((actU32)1<<29) ) { /* add carry */
         return actEXCEPTION_LENGTH; /* overflow: 2^64 bit input */
      }
   }
   info->low_count += (actU32)length;

   /* left overs of the last update */
   if ( info->buffer_used>0 ) {
      diff = actMin(length, actHASH_BLOCK_SIZE_SHA256-info->buffer_used);
      actMemcpy(buffer+info->buffer_used, dataIN, (unsigned int)diff);
      info->buffer_used += (actU16) diff;   /* actHASH_BLOCK_SIZE_SHA256 (64) fits in actU16 */
      if( info->buffer_used<actHASH_BLOCK_SIZE_SHA256 ) {
         return actOK;
      } else {
         actSHA2_32_Transform(buffer, H, watchdog);
      }
   }

   /* complete blocks */
   blocks = (length-diff)/actHASH_BLOCK_SIZE_SHA256;
   dataIN += diff;
   for (bcount=0; bcount<blocks; ++bcount, dataIN+=actHASH_BLOCK_SIZE_SHA256) {
      actSHA2_32_Transform(dataIN, H, watchdog);
   }

   /* copy left overs into buffer */
   info->buffer_used = (actU16) (length - (diff + actHASH_BLOCK_SIZE_SHA256*blocks)); /* actHASH_BLOCK_SIZE_SHA256 (64) fits in actU16 */
   actMemcpy(buffer, dataIN, (unsigned int)(info->buffer_used));

   actL1trigger (watchdog);

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHA2_32_Finalize (actSHA256STRUCT*  info,
 **                                    actU8*            hash,
 **                                    void            (*watchdog) (void))
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
 ** - hash:       the final hash value of required length
 ** - returns:    actOK always
 **
 ** assumes:
 ** - actSHAxxxInit() has been called before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHA256
 **
 ***************************************************************************/
static CRYPTOCV_FUNC( actRETURNCODE ) actSHA2_32_Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   actU32 low_bitcount = info->low_count << 3;
   actU32 hi_bitcount = (info->hi_count << 3) | (info->low_count >> 29);
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer = info->buffer;
   CRYPTOCV_P2VAR_PARA( actU32 ) H = info->H;
   int len = info->low_count & 0x3F;
   int pad_len = actHASH_BLOCK_SIZE_SHA256-2*actSHA256_WORD_SIZE;

   actL1trigger (watchdog);

   /* pad byte '0x80' */
   buffer[len++] = 0x80;

   /* pad zeros */
   actMemset(buffer+len, 0, (unsigned int)(actHASH_BLOCK_SIZE_SHA256-len));
   if( len > pad_len ) {
      actSHA2_32_Transform(buffer, H, watchdog);
      actMemset(buffer, 0, (unsigned int)pad_len);
   }

   /* pad 64-bit input count to last 8 bytes in big endian format */
#ifdef LITTLE_ENDIAN
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len])) = actSwap(hi_bitcount);
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len+actSHA256_WORD_SIZE])) = actSwap(low_bitcount);
#else
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len])) = hi_bitcount;
   *((CRYPTOCV_P2VAR_PARA( actU32 ))(&buffer[pad_len+actSHA256_WORD_SIZE])) = low_bitcount;
#endif

   /* transform last block */
   actSHA2_32_Transform(buffer, H, watchdog);

#ifdef LITTLE_ENDIAN
   /* convert little endian SHA-256-Digest to big endian byte order */
   for (len=0; len<info->result_length/actSHA256_WORD_SIZE; len++) {
      H[len] = actSwap(H[len]);
   }
#endif

   /* write message digest to output 'hash' */
   actMemcpy(hash, (CRYPTOCV_P2CONST_PARA(actU8))(&H[0]), info->result_length);

   /* wipe buffer */
   actMemset(info->buffer, 0, actHASH_BLOCK_SIZE_SHA256);

   actL1trigger (watchdog);

   return actOK;
}


/****************************************************************************
 ** Global Functions
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Init (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info )
{
   /* initialize message digest */
   info->H [0] = 0xc1059ed8UL;
   info->H [1] = 0x367cd507UL;
   info->H [2] = 0x3070dd17UL;
   info->H [3] = 0xf70e5939UL;
   info->H [4] = 0xffc00b31UL;
   info->H [5] = 0x68581511UL;
   info->H [6] = 0x64f98fa7UL;
   info->H [7] = 0xbefa4fa4UL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA224;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Update (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_32_Update  ((CRYPTOCV_P2VAR_PARA(actSHA256STRUCT)) info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_32_Finalize  ((CRYPTOCV_P2VAR_PARA(actSHA256STRUCT)) info,       hash, watchdog);
}


CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Init (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info )
{
   /* initialize message digest */
   info->H [0] = 0x6a09e667UL;
   info->H [1] = 0xbb67ae85UL;
   info->H [2] = 0x3c6ef372UL;
   info->H [3] = 0xa54ff53aUL;
   info->H [4] = 0x510e527fUL;
   info->H [5] = 0x9b05688cUL;
   info->H [6] = 0x1f83d9abUL;
   info->H [7] = 0x5be0cd19UL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA256;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Update (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
    return actSHA2_32_Update ((CRYPTOCV_P2VAR_PARA(actSHA256STRUCT)) info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_32_Finalize  ((CRYPTOCV_P2VAR_PARA(actSHA256STRUCT)) info,       hash, watchdog);
}
