/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Fee_Int.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and 
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as 
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined (FEE_INTBASE_H)
# define FEE_INTBASE_H

#include "Fee_Types.h"

/* Must be done AFTER including _Cfg.h files */
#ifndef FEE_DUMMY_STATEMENT /* COV_FEE_COMPATIBILITY */
#define FEE_DUMMY_STATEMENT(v)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_INTBASE_MAJOR_VERSION                                   (8u)
# define FEE_INTBASE_MINOR_VERSION                                   (1u)
# define FEE_INTBASE_PATCH_VERSION                                   (2u)

/**********************************************************************************************************************
 *  GLOBAL MACROS
 *********************************************************************************************************************/
#ifndef FEE_LOCAL /* COV_FEE_COMPATIBILITY */
# define FEE_LOCAL static
#endif

#ifndef FEE_LOCAL_INLINE /* COV_FEE_COMPATIBILITY */
# define FEE_LOCAL_INLINE LOCAL_INLINE
#endif

/* Just for convenience: ease/shorten definition and declaration of local functions */
/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
#define FEE_INTERN_FUNC(retType) FUNC(retType, FEE_PRIVATE_CODE)
#define FEE_INLINE_FUNC(retType) FEE_LOCAL_INLINE FEE_INTERN_FUNC(retType)
#define FEE_LOCAL_FUNC(retType)  FEE_LOCAL FEE_INTERN_FUNC(retType)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef int Fee_Bool;
typedef unsigned int Fee_BitFieldType;


/* used for lengths as well as for offsets within sectors.
 * Since FEE internally treats addresses of chunks, instances, etc. as offsets within a ceartain sector,
 * type Fee_LengthType shall be used */
typedef uint32 Fee_LengthType;

/* It is intended to have opaque data types, in order to increase encapsulation and hiding of information
 * Nobody needs to know exact definition of the structures, in order to point to them, unless she accesses it.
 * E.g. a Fee's user using Fee_InitEx(ptr_to_config) shall not de-reference the config.
 * In fact, she does not even need to know, the defintion of Fee_ConfigStruct */
typedef P2CONST(struct Fee_ConfigStruct, AUTOMATIC, FEE_APPL_CONFIG) Fee_ConfigRefTypeInternal;

typedef P2CONST(struct Fee_PartitionConfigStruct, AUTOMATIC, FEE_APPL_CONFIG) Fee_PartitionConfigRefType;
typedef P2CONST(struct Fee_SectorConfigStruct,    AUTOMATIC, FEE_APPL_CONFIG) Fee_SectorConfigRefType;

typedef P2CONST(struct Fee_BlockConfigStruct,     AUTOMATIC, FEE_APPL_CONFIG) Fee_BlockConfigRefType;

struct Fee_BlockConfigStruct /*< Block Configuration -> filled by FEE generator */
{
    uint16 LinktableIndex_u16;    /*<! block tag, index in the linktable; may exceed initial link-table size */
    uint16 BlockPayloadSize_u16;  /*<! payload, i.e. amount of user data */
    Fee_BitFieldType NumberOfDatasets_u8 : 8; /*<! number of datasets per block */
    Fee_BitFieldType PartitionIndex : 2;      /*<! partition id (0..3) */
    Fee_BitFieldType NumberOfInstancesExponent_u8 : 4; /*<! the exponent n of the number of instances per chunk => (2^n)-1 */
    Fee_BitFieldType ImmediateData_bl : 1;    /*<! Immediate data, not used anymore; just for compatibility */
    Fee_BitFieldType CriticalData_bl : 1;     /*<! Data are critical -> critical data handling during writes */
};

struct Fee_ConfigStruct /* complete Fee configuration structure; gathers all data */
{
    Fee_BlockConfigRefType     BlockConfig_at;     /*<! reference to block config array */
    Fee_PartitionConfigRefType PartitionConfig_at; /*<! reference to partition configuration array */
    uint16          Fee_NumberOfBlocks_u16;        /*<! size of block config array (number of elements) */
    uint8           Fee_NumberOfPartitions_u8;     /*<! size of partition config array (number of elements) */
    uint8           Fee_DatasetSelectionBits_u8;   /*<! number of dataset selection bits; used to decompose a 
                                                        BlockNumber into BlockConfig index and Dataset parts. */
    boolean         Fee_ConfigurationIncomplete;   /*<! FBL configuration? (FBL configurations usually are incomplete;
                                                        they don't contain all user blocks -> special handling
                                                        during sector switches) */
};


/* AUTOSAR's TRUE and FALSE definitions do not  (may not) match boolean expressions' type (signed/unsigned).
 * Additionally we want to avoid macros; enums do it well. (unless we need to store values in global variables) */
enum Fee_tagBoolVals {
    FEE_FALSE = 0,
    FEE_TRUE = 1
};

typedef P2VAR(uint8, AUTOMATIC, FEE_VAR_NOINIT) Fee_IntBuffer_u8PtrType;
typedef P2VAR(uint32, AUTOMATIC, FEE_VAR_NOINIT) Fee_IntBuffer_u32PtrType;

typedef P2CONST(uint8, AUTOMATIC, FEE_VAR_NOINIT) Fee_IntBuffer_u8ConstPtrType;
typedef P2CONST(uint32, AUTOMATIC, FEE_VAR_NOINIT) Fee_IntBuffer_u32ConstPtrType;

typedef P2VAR(struct Fee_BlockIdStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_BlockIdRefType;
typedef P2CONST(struct Fee_BlockIdStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_BlockIdConstRefType;

struct Fee_BlockIdStruct /*<! that's the real, complete Fee BlockId! */
{
    uint16 Tag;
    uint8  DataIndex;
}; 

enum Fee_FlsRequestStatus
{
    FEE_REQ_ACCEPTED,
    FEE_REQ_SYNC_OK,
    FEE_REQ_REJECTED,
    FEE_REQ_SYNC_FAILED,
    FEE_REQ_SYNC_USELESS /* request cannot be executed, due to insufficient params (e.g. index out of link table) */
};

/* internal enum type, used to identify the status of management information */
enum Fee_MngmtStatus
{
    FEE_MNGMT_DEFECTIVE,
    FEE_MNGMT_ERASED,
    FEE_MNGMT_CONSISTENT
};
#endif /* FEE_INTBASE_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee_IntBase.h
 *********************************************************************************************************************/
