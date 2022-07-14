/****************************************************************************
 **
 ** $HeadURL: https://subversion/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/actCLib/include/actISHA2-32.h $
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
 **     Layer: User Module - Interface
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** Interface for SHA-224 and SHA-256 hash algorithms.
 **
 ** constants:
 **  actHASH_SIZE_SHA224
 **  actHASH_SIZE_SHA256
 **  actHASH_BLOCK_SIZE_SHA256
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actSHA224Init
 **   actSHA224Update
 **   actSHA224Finalize
 **   actSHA256Init
 **   actSHA256Update
 **   actSHA256Finalize
 **
 ***************************************************************************/


#ifndef ACTISHA2_32_H
#define ACTISHA2_32_H


#include "actITypes.h"


/****************************************************************************
 ** Types and constants
 ***************************************************************************/


#define actHASH_SIZE_SHA224         28
#define actHASH_SIZE_SHA256         32
#define actHASH_BLOCK_SIZE_SHA256   64

/* Workspace structure for SHA-224 & SHA-256 */
typedef struct {
   actU32   H [8];                  /* message digest state buffer */
   actU32   low_count, hi_count;    /* 64 bit input count */
   actU16   buffer_used;            /* number of bytes saved in buffer */
   actU16   result_length;          /* number of bytes to retuirn as hash-value */
   actU8    buffer [actHASH_BLOCK_SIZE_SHA256];   /* remaining data buffer */
} actSHA224STRUCT, actSHA256STRUCT;


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
 **  This function initializes the SHA-224 / SHA-256 algorithm.
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
CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Init (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Init (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info );

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
CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Update (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info,
   CRYPTOCV_P2CONST_PARA( actU8 ) dataIN,
   int length,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Update (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
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
CRYPTOCV_FUNC( actRETURNCODE ) actSHA224Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA224STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );
CRYPTOCV_FUNC( actRETURNCODE ) actSHA256Finalize (
   CRYPTOCV_P2VAR_PARA( actSHA256STRUCT ) info,
   CRYPTOCV_P2VAR_PARA( actU8 ) hash,
   CRYPTOCV_P2FUNC( CRYPTOCV_NONE, void, watchdog)(void) );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* ACTISHA2_32_H */
