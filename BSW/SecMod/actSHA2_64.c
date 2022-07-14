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
 ** Core SHA-384 and SHA-512 implementation.
 ** Supports SHA-512/224 and SHA-512/256 as well.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **   bit rotation and basic SHA-2 transformation macros (see bellow)
 **
 ** local functions:
 **   actSHA2_64_Transform
 **   actSHA2_64_Update
 **   actSHA2_64_Finalize
 **
 ** global functions:
 **   actSHA512_224Init
 **   actSHA512_224Update
 **   actSHA512_224Finalize
 **   actSHA512_256Init
 **   actSHA512_256Update
 **   actSHA512_256Finalize
 **   actSHA384Init
 **   actSHA384Update
 **   actSHA384Finalize
 **   actSHA512Init
 **   actSHA512Update
 **   actSHA512Finalize
 **
 ***************************************************************************/


#include "actISHA2_64.h"
#include "actUtilities.h"
#include "actConfig.h"
#include "actWatchdog.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actSHA512_WORD_SIZE     8


/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/* The SHA2_64 functions */
#define S0(x) (actRotr64(x,28)^actRotr64(x,34)^actRotr64(x,39))
#define S1(x) (actRotr64(x,14)^actRotr64(x,18)^actRotr64(x,41))
#define s0(x) (actRotr64(x,1)^actRotr64(x,8)^(x>>7))
#define s1(x) (actRotr64(x,19)^actRotr64(x,61)^(x>>6))

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

#define Ra(i,j) (h(i)+=S1(e(i))+Ch(e(i),f(i),g(i))+(K[(i)+(j)])+((j)?blk2(i):blk0(i)))
#define Rb(i)   (d(i)+=h(i))
#define Rc(i)   (h(i)+=S0(a(i))+Maj(a(i),b(i),c(i)))


/****************************************************************************
 ** Main local function: hash one 128 byte block of data
 ***************************************************************************/
CRYPTOCV_CONST( STATIC, actU64 ) K [80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
}; 

/****************************************************************************
 **
 ** FUNCTION:
 ** static void actSHA2_32_Transform(const actU8 *in, actU32 *H, void (* watchdog) (void))
 **
 **  This function hashes one 128 byte block of data.
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
static CRYPTOCV_FUNC( void ) actSHA2_64_Transform (
   CRYPTOCV_P2CONST_PARA( actU8 ) in,
   CRYPTOCV_P2VAR_PARA( actU64 ) H,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   actU64 W[16];
   actU64 T[8];
   actU32 i, j;

   /* Copy state to working vars */
   for (i=0; i<8; ++i) { T[i] = H[i]; }

   actL3trigger (watchdog);
   /* Copy 128 byte block 'in' (big endian format) to workspace W[] */
   for (i=0; i<16; ++i) { \
      W[i] = (((((((actU64) in[i<<3]<<8      | in [(i<<3)+1])<<8 | in [(i<<3)+2])<<8 | in [(i<<3)+3])<<8
                          | in[(i<<3)+4])<<8 | in [(i<<3)+5])<<8 | in [(i<<3)+6])<<8 | in [(i<<3)+7]; \
   }

   actL2trigger( watchdog );

   /* 80 operations of 16 rounds */
   for (j = 0; j < 80; j += 16)
   {
#if (actSHA512_SPEED_UP > 0)
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
 ** actRETURNCODE actSHA2_64_Update (actSHA512STRUCT*  info,
 **                            const actU8*            dataIN,
 **                                  int               length,
 **                                  void            (*watchdog) (void))
 **
 **  This function hashes the given data and can be called arbitrarily
 **  often between an initialize and finalize of the SHA-2 algorithm.
 **  Uses any data already in the actSHA512STRUCT structure and leaves
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
 ** - returns:    actEXCEPTION_LENGTH   total input more than 2^128 - 1 bit
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
static CRYPTOCV_FUNC( actRETURNCODE ) actSHA2_64_Update (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   CRYPTOCV_P2VAR_PARA( actU64 ) H = info->H;
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer = info->buffer;
   int diff=0, blocks, bcount;

   actL1trigger (watchdog);

   /* update byte count */
   if( (info->low_count + (actU64)length) < info->low_count ) {
      if ( (++info->hi_count)==((actU64)1<<29) ) { /* add carry */
         return actEXCEPTION_LENGTH; /* overflow: 2^128 bit input */
      }
   }
   info->low_count += (actU64)length;

   /* left overs of the last update */
   if ( info->buffer_used>0 ) {
      diff = actMin(length, actHASH_BLOCK_SIZE_SHA512-info->buffer_used);
      actMemcpy(buffer+info->buffer_used, dataIN, (unsigned int)diff);
      info->buffer_used += (actU16) diff;   /* actHASH_BLOCK_SIZE_SHA512 (128) fits in actU16 */
      if( info->buffer_used<actHASH_BLOCK_SIZE_SHA512 ) {
         return actOK;
      } else {
         actSHA2_64_Transform(buffer, H, watchdog);
      }
   }

   /* complete blocks */
   blocks = (length-diff)/actHASH_BLOCK_SIZE_SHA512;
   dataIN += diff;
   for (bcount=0; bcount<blocks; ++bcount, dataIN+=actHASH_BLOCK_SIZE_SHA512) {
      actSHA2_64_Transform(dataIN, H, watchdog);
   }

   /* copy left overs into buffer */
   info->buffer_used = (actU16) (length - (diff + actHASH_BLOCK_SIZE_SHA512*blocks)); /* actHASH_BLOCK_SIZE_SHA512 (128) fits in actU16 */
   actMemcpy(buffer, dataIN, (unsigned int)(info->buffer_used));

   actL1trigger (watchdog);

   return actOK;
}


/****************************************************************************
 **
 ** FUNCTION:
 ** actRETURNCODE actSHA2_64_Finalize (actSHA512STRUCT*  info,
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
 ** - actHASH_SIZE_SHA512
 **
 ***************************************************************************/
static CRYPTOCV_FUNC( actRETURNCODE ) actSHA2_64_Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   actU64  low_bitcount  = info->low_count << 3;
   actU64  hi_bitcount   = (info->hi_count << 3) | (info->low_count >> 29);
   CRYPTOCV_P2VAR_PARA( actU8 ) buffer   = info->buffer;
   CRYPTOCV_P2VAR_PARA( actU64 ) H        = info->H;
   int     len      = info->low_count & 0x7F;     /*  0..127 bytes */
   int     pad_len  = actHASH_BLOCK_SIZE_SHA512 - 2* actSHA512_WORD_SIZE;

   actL1trigger (watchdog);

   /* pad byte '0x80' */
   buffer [len++] = 0x80;

   /* pad zeros */
   actMemset (buffer + len, 0, (unsigned int) (actHASH_BLOCK_SIZE_SHA512-len));
   if (len > pad_len) {
      actSHA2_64_Transform (buffer, H, watchdog);
      actMemset (buffer, 0, (unsigned int) pad_len);
   }

   /* pad 128-bit input count to last 16 bytes in big endian format */
#ifdef LITTLE_ENDIAN
   *((CRYPTOCV_P2VAR_PARA(actU64)) (&buffer [pad_len]))                       = actSwap64(hi_bitcount);
   *((CRYPTOCV_P2VAR_PARA(actU64)) (&buffer [pad_len + actSHA512_WORD_SIZE])) = actSwap64(low_bitcount);
#else
   *((CRYPTOCV_P2VAR_PARA(actU64)) (&buffer [pad_len]))                       = hi_bitcount;
   *((CRYPTOCV_P2VAR_PARA(actU64)) (&buffer [pad_len + actSHA512_WORD_SIZE])) = low_bitcount;
#endif

   /* transform last block */
   actSHA2_64_Transform (buffer, H, watchdog);

#ifdef LITTLE_ENDIAN
   /* convert little endian SHA-512-Digest to big endian byte order */
   for (len = 0; len < (info->result_length + actSHA512_WORD_SIZE -1) /actSHA512_WORD_SIZE; len++) {
      H [len] = actSwap64 (H [len]);
   }
#endif

   /* write message digest to output 'hash' */
   actMemcpy (hash, (CRYPTOCV_P2CONST_PARA(actU8)) (&H [0]), info->result_length);

   /* wipe buffer */
   actMemset (info->buffer, 0, actHASH_BLOCK_SIZE_SHA512);

   actL1trigger (watchdog);

   return actOK;
}


/****************************************************************************
 ** Global Functions
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Init (
   CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info )
{
   /* initialize message digest */
   info->H [0] = 0x8C3D37C819544DA2ULL;
   info->H [1] = 0x73E1996689DCD4D6ULL;
   info->H [2] = 0x1DFAB7AE32FF9C82ULL;
   info->H [3] = 0x679DD514582F9FCFULL;
   info->H [4] = 0x0F6D2B697BD44DA8ULL;
   info->H [5] = 0x77E36F7304C48942ULL;
   info->H [6] = 0x3F9D85A86A1D36C8ULL;
   info->H [7] = 0x1112E6AD91D692A1ULL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA512_224;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Update (
   CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Update      ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT))    info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Finalize (
  CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info,
  CRYPTOCV_P2VAR_PARA( actU8 ) hash,
  CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Finalize      ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT))    info,       hash, watchdog);
}


CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Init (
   CRYPTOCV_P2VAR_PARA( actSHA512_256STRUCT ) info )
{
   /* initialize message digest */
   info->H [0] = 0x22312194FC2BF72CULL;
   info->H [1] = 0x9F555FA3C84C64C2ULL;
   info->H [2] = 0x2393B86B6F53B151ULL;
   info->H [3] = 0x963877195940EABDULL;
   info->H [4] = 0x96283EE2A88EFFE3ULL;
   info->H [5] = 0xBE5E1E2553863992ULL;
   info->H [6] = 0x2B0199FC2C85B8AAULL;
   info->H [7] = 0x0EB72DDC81C52CA2ULL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA512_256;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Update (
   CRYPTOCV_P2VAR_PARA( actSHA512_256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Update      ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT))    info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Finalize (
   CRYPTOCV_P2VAR_PARA(actSHA512_256STRUCT ) info,
   CRYPTOCV_P2VAR_PARA(actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Finalize      ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT))    info,       hash, watchdog);
}


CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Init (
   CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info )
{
   /* initialize message digest */
   info->H [0] = 0xcbbb9d5dc1059ed8ULL;
   info->H [1] = 0x629a292a367cd507ULL;
   info->H [2] = 0x9159015a3070dd17ULL;
   info->H [3] = 0x152fecd8f70e5939ULL;
   info->H [4] = 0x67332667ffc00b31ULL;
   info->H [5] = 0x8eb44a8768581511ULL;
   info->H [6] = 0xdb0c2e0d64f98fa7ULL;
   info->H [7] = 0x47b5481dbefa4fa4ULL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA384;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Update (
   CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Update  ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Finalize  ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) info,       hash, watchdog);
}


CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Init (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info)
{
   /* initialize message digest */
   info->H [0] = 0x6a09e667f3bcc908ULL;
   info->H [1] = 0xbb67ae8584caa73bULL;
   info->H [2] = 0x3c6ef372fe94f82bULL;
   info->H [3] = 0xa54ff53a5f1d36f1ULL;
   info->H [4] = 0x510e527fade682d1ULL;
   info->H [5] = 0x9b05688c2b3e6c1fULL;
   info->H [6] = 0x1f83d9abfb41bd6bULL;
   info->H [7] = 0x5be0cd19137e2179ULL;

   /* initialize count variables */
   info->low_count      =
   info->hi_count       =
   info->buffer_used    = 0;
   info->result_length  = actHASH_SIZE_SHA512;

   return actOK;
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Update (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Update  ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) info,              dataIN,     length, watchdog);
}

CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) )
{
   return actSHA2_64_Finalize  ((CRYPTOCV_P2VAR_PARA(actSHA512STRUCT)) info,       hash, watchdog);
}
