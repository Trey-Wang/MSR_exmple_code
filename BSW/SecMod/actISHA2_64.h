/****************************************************************************
 **
 ** $HeadURL$
 ** $Revision$
 ** $Date$
 ** $Author$
 **
 ** \copyright(cv cryptovision GmbH, 2015)
 **
 ** \version(cv library/es 1.2)
 **
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: User Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** Interface for SHA-384 and SHA-512 hash algorithms.
 **
 ** constants:
 **  actHASH_SIZE_SHA512_224
 **  actHASH_SIZE_SHA512_256
 **  actHASH_SIZE_SHA384
 **  actHASH_SIZE_SHA512
 **  actHASH_BLOCK_SIZE_SHA512
 **
 ** types:
 **   actSHA512_224STRUCT
 **   actSHA512_256STRUCT
 **   actSHA384STRUCT
 **   actSHA512STRUCT
 **
 ** macros:
 **
 ** functions:
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


#ifndef ACTISHA2_64_H
#define ACTISHA2_64_H


#include "actITypes.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actHASH_SIZE_SHA512_224      28
#define actHASH_SIZE_SHA512_256      32
#define actHASH_SIZE_SHA384          48
#define actHASH_SIZE_SHA512          64
#define actHASH_BLOCK_SIZE_SHA512   128

/* Workspace structure for SHA-384 & SHA-512 */
typedef struct {
   actU64   H [8];                  /* message digest state buffer */
   actU64   low_count, hi_count;    /* 128 bit input count */
   actU16   buffer_used;            /* number of bytes saved in buffer */
   actU16   result_length;          /* number of bytes to return as hash-value */
   actU8    buffer [actHASH_BLOCK_SIZE_SHA512];   /* remaining data buffer */
} actSHA512_224STRUCT, actSHA512_256STRUCT, actSHA384STRUCT, actSHA512STRUCT;


/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
 **
 ** FUNCTION:
 **
 ** actRETURNCODE actSHAxxxInit (actSHAxxxSTRUCT* info)
 **
 **  This function initializes the SHA-384 / SHA-512 algorithm.
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
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Init(
  CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info);
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Init(
  CRYPTOCV_P2VAR_PARA( actSHA512_256STRUCT ) info);
CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Init(
  CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info);
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Init(
  CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info);

/****************************************************************************
 **
 ** FUNCTION:
 **
 ** actRETURNCODE actSHAxxxUpdate (actSHAxxxSTRUCT* info,
 **                          const actU8*           dataIN,
 **                                int              length,
 **                                void           (*watchdog) (void))
 **
 **  This function hashes the given data and can be called arbitrarily
 **  often between an initialize and finalize of the hash algorithm.
 **  Uses any data already in the actSHAxxxSTRUCT structure and leaves
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
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Update (
   CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Update (
   CRYPTOCV_P2VAR_PARA( actSHA512_256STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Update (
   CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Update (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );



/****************************************************************************
 **
 ** FUNCTION:
 **
 ** actRETURNCODE actSHAxxxFinalize (actSHAxxxSTRUCT*   info,
 **                                  actU8              hash [actHASH_SIZE_SHAxxx],
 **                                  void             (*watchdog) (void))
 **
 **  This function finalizes the hash algorithm and delivers the hash value.
 **
 ** input:
 ** - info:       pointer to hash context structure
 ** - hash:       byte array to contain the hash value
 ** - watchdog:   pointer to watchdog reset function
 **
 ** output:
 ** - info:       finalized hash context structure
 ** - hash:       the final hash value (of length actHASH_SIZE_SHAxxx)
 ** - returns:    actOK allways
 **
 ** assumes:
 ** - actSHAxxxInit() has been called before calling this function
 **
 ** uses:
 ** - actHASH_SIZE_SHAxxx
 **
 ***************************************************************************/
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_224Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA512_224STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512_256Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA512_256STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ),
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA384Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA384STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ),
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA512Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA512STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ),
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );


#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTISHA2_64_H */
