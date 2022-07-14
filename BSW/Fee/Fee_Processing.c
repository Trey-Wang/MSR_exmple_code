/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Fee_Processing.c
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  This file contains the internal processing of FEE, i.e. everything running in scope of Fee_MainFunction
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fee_Int.h" /* PRQA S 3313 */ /*MD_FEE_18.1_MissingStructDefinition */

/***************** FEE Notification functions ******************************/
#ifndef FEE_NFY_SS_THRESHOLD /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_SS_THRESHOLD(globals)
#endif

#ifndef FEE_NFY_SS_START /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_SS_START(globals)
#endif

#ifndef FEE_NFY_SS_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_SS_END(globals)
#endif

#ifndef FEE_NFY_SECTOR_ERASE /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_SECTOR_ERASE(globals)
#endif
#ifndef FEE_NFY_SECTOR_ERASE_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_SECTOR_ERASE_END(globals, result)
#endif
#ifndef FEE_NFY_CHUNK_HEADER_WRITE /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_CHUNK_HEADER_WRITE(globals)
#endif 
#ifndef FEE_NFY_CHUNK_HEADER_WRITE_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_CHUNK_HEADER_WRITE_END(globals, result)
#endif

#ifndef FEE_NFY_CHUNK_LINK_WRITE /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_CHUNK_LINK_WRITE(globals)
#endif 

#ifndef FEE_NFY_CHUNK_LINK_WRITE_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_CHUNK_LINK_WRITE_END(globals, result)
#endif

#ifndef FEE_NFY_INSTANCE_WRITE /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_INSTANCE_WRITE(globals)
#endif
#ifndef FEE_NFY_INSTANCE_WRITE_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_INSTANCE_WRITE_END(globals, result)
#endif

#ifndef FEE_NFY_INSTANCE_COPY /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_INSTANCE_COPY(globals)
#endif
#ifndef FEE_NFY_INSTANCE_COPY_END /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_INSTANCE_COPY_END(globals, result)
#endif

#ifndef FEE_NFY_WRITE_SUSPENDED /* COV_FEE_COMPATIBILITY */
#   define FEE_NFY_WRITE_SUSPENDED(globals)
#endif

/* Determining the location of the lower and upper sector header within the data buffer of the Fee. 
 * Needed not to overwrite the lower sector header after the startup to evaluate both sector headers content. 
 * The upper sector header location value is shifted, because the array is built up on an uint32 data type.
 */
#define FEE_BUFFER_LOCATION_LOWER_SECTOR_HEADER                       (0u)
#define FEE_BUFFER_LOCATION_UPPER_SECTOR_HEADER                       (FEE_SECTOR_HEADER_SIZE >> 2u)

/* If the WriteCycle could not be read from the chunk header the reason is described by the corresponding defines. */
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
# define FEE_WRITE_CYCLE_EMPTY                                        (0)
# define FEE_WRITE_CYCLE_FAILED                                       (0xFFFFFFFEuL)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Actually unused struct; it is only intended to ensure a minimum size of all internal buffers; compiler issues an error if on was too small
 *  Buffers' sizes depend on configuration, but generator is not considered being safe; we have to check generated code.
 *  Note that this check is not complete by itself; it just ensures the minimum, which is important if a flash write alignments are smaller than 16Bytes.
 *  However, we cannot check for alignments using the compiler; this must be done elsewhere.
 *  It could also be verified by reviews; but this way it gets automated (except the buffers to be considered).
 *  Additionally it doesn't consume any (embedded) resources, neither run-time nor memory.
 *  It only consumes few additional milliseconds of compile-time */
struct Fee_tagBufferSizes {
    Fee_BitFieldType ChunkAlloc : (sizeof(Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t.buffer_au32) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType ChunkSearch : (sizeof(Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.super_t.headerBuffer_au32) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType InstanceWrite : (sizeof(Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t.BufferBytes) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType InstanceCopy : (sizeof(Fee_Globals_t.FsmLocalVars_t.instanceCopyContext_t.BufferBytes) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType InstanceSearch : (sizeof(Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t.Buffer_au32) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType PartitionInit : (sizeof(Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.sectorHeaderData) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
    Fee_BitFieldType SectorFormat : (sizeof(Fee_Globals_t.FsmLocalVars_t.SectorFormatContext_t.sectorHeader_au32) >= FEE_CHUNK_HEADER_SIZE ? 1 : -1);
};

typedef P2CONST(struct Fee_StateDescrStruct, AUTOMATIC, FEE_PRIVATE_CONST) Fee_StateDescriptorConstRefType;

typedef P2VAR(struct Fee_FsmStackStruct, AUTOMATIC, FEE_VAR_NOINIT)   Fee_FsmStackRefType;
typedef P2CONST(struct Fee_FsmStackStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_FsmStackConstRefType;

enum {
    FEE_MAX_NUM_OF_QRY_AND = 2
};

/********************************************************************
 *  ACTIONS
 *******************************************************************/
/* PRQA S 0779 action_enumerators *//* MD_FEE_5.1_identifiers */
enum Fee_StateActionEnumType /*<! Enumerate all possible actions; function to be directly called by FSM processing */
{
    FEE_ACT_ID_CALC_INSTANCE_INDEX_BINARY,
    FEE_ACT_ID_DECR_TO_PREV_INSTANCE_INDEX,
    FEE_ACT_ID_EVALUATE_CHUNK_HOPPING,
    FEE_ACT_ID_EVALUATE_INSTANCE_TRAILER,
    FEE_ACT_ID_EVALUATE_SECTOR_HEADERS,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_ACT_ID_EVALUATE_WRITE_CYCLE,
#endif
    FEE_ACT_ID_FINALIZE_CHUNK_SEARCH,
    FEE_ACT_ID_FINALIZE_CHUNK_LINK_WRITE,
    FEE_ACT_ID_FINALIZE_CHUNK_ALLOCATION,
    FEE_ACT_ID_FINALIZE_GET_NEXT_BLOCK_FSM,
    FEE_ACT_ID_FINALIZE_INSTANCE_WRITE,
    FEE_ACT_ID_FINALIZE_INSTANCE_COPY,
    FEE_ACT_ID_FINALIZE_JOB,
    FEE_ACT_ID_FINALIZE_BLOCK_COPY,

    FEE_ACT_ID_FINALIZE_SS_COPYING,
    FEE_ACT_ID_FINALIZE_SETUP_SECTORS,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_ACT_ID_FINALIZE_WRITE_CYCLE_JOB,
#endif
    FEE_ACT_ID_GET_CURRENT_CHUNK_INFO,
    FEE_ACT_ID_INCR_TO_NEXT_INSTANCE_INDEX,
    FEE_ACT_ID_INIT_DATACHUNK_ALLOC_WRITE_FSM,
    FEE_ACT_ID_InitDataChunkAllocCopyFsm,
    FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM,
    FEE_ACT_ID_INIT_CHUNK_SEARCH_FSM,
    FEE_ACT_ID_INIT_CHUNK_HEADER_CHECK_FSM,
    FEE_ACT_ID_INIT_COPY_PROCESS_FSM,
    FEE_ACT_ID_INIT_CREATE_INSTANCE_FSM,
    FEE_ACT_ID_INITIALIZE_BLOCK_COPY,
    FEE_ACT_ID_INIT_INSTANCE_EVALUATION_FSM,
    FEE_ACT_ID_INIT_INSTANCE_EVALUATION_UPWARDS_FSM,
    FEE_ACT_ID_INIT_INSTANCE_SEARCH_BINARY_FSM,
    FEE_ACT_ID_INIT_INSTANCE_SEARCH_DOWNWARDS_FSM,
    FEE_ACT_ID_INIT_INSTANCE_SEARCH_UPWARDS,

    FEE_ACT_ID_INIT_REVERSE_CHUNK_SEARCH,
    FEE_ACT_ID_INIT_SEARCH_WRITTEN_IN_OLD_SECTOR,
    FEE_ACT_ID_INIT_SEARCH_UPWARDS_FSM,
    FEE_ACT_ID_CONTINUE_UPWARDS_CHUNK_SEARCH,
    FEE_ACT_ID_INIT_SETUP_SECTORS_FSM,
    FEE_ACT_ID_SETUP_SECTORS_WRITE_HEADER,
    FEE_ACT_ID_INIT_SEARCH_FOR_NFA_WRITE,
    FEE_ACT_ID_FINALIZE_SEARCH_FOR_NFA_WRITE,
    FEE_ACT_ID_PRE_INIT_WRITE_PROCESSING,
    FEE_ACT_ID_INIT_WRITE_PROCESSING_FSM,
    FEE_ACT_ID_ALLOC_IN_CURRENT_SECTOR,
    FEE_ACT_ID_PREPARE_BLOCK_COPY_READ,
    FEE_ACT_ID_PREPARE_BLOCK_COPY_WRITE,
    FEE_ACT_ID_PREPARE_CHUNK_HEADER_WRITE,
    FEE_ACT_ID_PREPARE_CHUNK_LINK_READ_OF_CURRENT_CHUNK,
    FEE_ACT_ID_PREPARE_CHUNK_LINK_WRITE,
    FEE_ACT_ID_PREPARE_INSTANCE_HEADER_READ,
    FEE_ACT_ID_PREPARE_PAYLOAD_DATA_PROCESSING,
    FEE_ACT_ID_CONTINUE_SEARCH_FOR_NFA,
    FEE_ACT_ID_PREPARE_SECTOR_HEADER_UPPER_READ,
#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
    FEE_ACT_ID_PREPARE_WCC_READ,
#endif
    FEE_ACT_ID_PREPARE_WRITE_STREAM_LAST,
    FEE_ACT_ID_PREPARE_WRITE_STREAM_SECOND,
    FEE_ACT_ID_REPORT_SECTOR_OVERFLOW,
    FEE_ACT_ID_ResetChunkSearch,
    FEE_ACT_ID_RETURN_FROM_SUB_FSM,
    FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED,
    FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID,
    FEE_ACT_ID_SET_LOCAL_JOB_RESULT_OK,

    FEE_ACT_ID_SETUP_SECTOR_SWITCH_SOURCE_SEARCH,

    FEE_ACT_ID_ActSetupJobProcessing,
    FEE_ACT_ID_SWITCH_OPERATING_SECTOR,
    FEE_ACT_ID_VALIDATE_CHUNK_LINK,
    FEE_ACT_ID_CHECK_CHUNK_CONSISTENCY,
#  if (FEE_DATA_CONVERSION_API == STD_ON)
    FEE_ACT_ID_GET_NEXT_BLOCK_TO_COPY,
    FEE_ACT_ID_INIT_CHUNK_ALLOC_FSM_DC,
    FEE_ACT_ID_PREPARE_READ_INSTANCE_DATA,
    FEE_ACT_ID_INIT_DC_WRITE_PROC_SUB_FSM,
    FEE_ACT_ID_INIT_INSTANCE_WRITE_DC_FSM,
    FEE_ACT_ID_FINALIZE_BLOCK_COPY_DC,
    FEE_ACT_ID_FINALIZE_DATA_CONVERSION,
#  endif
    FEE_ACT_ID_IssueDeferredRequest,
    Fee_ACT_ID_HandleWriteSuspended,

    FEE_ACT_ID_NOP, /*<! No operation; should be second last */
    FEE_ACT_ID_WAIT /*<! must be the last action */
};
/* PRQA L:action_enumerators */

/* Force implicit cast for enum Fee_StateActionIdType into uint8 when using Fee_StateActionType as data type.
 * But only for production mode in order to see not only numbers when debugging.
 */
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    typedef enum Fee_StateActionEnumType Fee_StateActionType;
#else
    typedef uint8 Fee_StateActionType;
#endif

/********************************************************************
 *  QUERIES
 *******************************************************************/
enum Fee_StateQueryEnumType /*<! Enumerate all possible queries; function to be directly called by FSM processing */
{
    FEE_QRY_ID_BLOCK_PAYLOAD_UPDATED,
    FEE_QRY_ID_BlockCopyBytesRemaining,
    FEE_QRY_ID_CHUNK_IS_DATA_CHUNK,
    FEE_QRY_ID_CHUNK_NOT_FULL,
    FEE_QRY_ID_ERASED_VALUE,
    FEE_QRY_ID_FLS_JOB_RESULT_OK,
    FEE_QRY_ID_LOCAL_JOB_RESULT_EMPTY_CHUNKS,
    FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED,
    FEE_QRY_ID_LOCAL_JOB_RESULT_INVALID,
    FEE_QRY_ID_LOCAL_JOB_RESULT_OK,
    FEE_QRY_ID_MAY_FOLLOW_LINKS,
    FEE_QRY_ID_PREVIOUS_INSTANCE_AVAILABLE,
  
    FEE_QRY_ID_RECENT_CHUNK_LINK_CONSISTENT,
    FEE_QRY_ID_SEARCHING_IN_CURRENT_SECTOR,
    FEE_QRY_ID_VALID_INSTANCE_WRITTEN,
    FEE_QRY_ID_WriteSuspended,
    FEE_QRY_ID_TRUE /*<! always true; must be the last value */
};

/* Force implicit cast for enum Fee_StateQueryIdType into uint8 when using Fee_StateQueryType as data type. 
 * But only for production mode in order to see not only numbers when debugging.
 */
#if (STD_ON == FEE_DEV_ERROR_DETECT)
    typedef enum Fee_StateQueryEnumType Fee_StateQueryType;
#else
    typedef uint8 Fee_StateQueryType;
#endif

/********************************************************************
 *  STATE MACHINE
 *******************************************************************/
/* Listing of the pointers to the action functions. */
typedef struct /*<! aggregates the actions to be performed upon state transition, in order (they have side-effects!) */
{
    Fee_StateActionType ActionHandler1_t; /*<! first, this action */
    Fee_StateActionType ActionHandler2_t; /*<! then, that action */
} Fee_StateChangeActionsType;

/* Pointer to a query table (defined later). */
typedef P2CONST(Fee_StateQueryType, AUTOMATIC, FEE_PRIVATE_CONST) Fee_StateQueryPtrType;

/* Describe the final state exit ("else") to be taken if no condition matched. */
typedef struct /*<! describes a state general transition */
{
    Fee_StateChangeActionsType Actions_t;
    Fee_StateType              NextState_t;
} Fee_StateChangeElseDescrType;

/* Describe one state exit with its condition, its actions and its following state. */
typedef struct /*<! associates a transition with the conditions that must be all fulfilled to take it */
{
    Fee_StateQueryType         Queries_at[FEE_MAX_NUM_OF_QRY_AND];
    Fee_StateChangeElseDescrType Next_t;
} Fee_StateChangeIfDescrType;

/* Describe a state with it's possible change conditions. */
struct Fee_StateDescrStruct /*<! describes a state, consisting of two transitions */
{
    Fee_StateChangeIfDescrType ifChange_t;     /* <! transition to be taken if associated conditions are all fulfilled. */
    Fee_StateChangeElseDescrType elseChange_t; /* <! alternative transition, if above one was not taken */
};

/* Type of a query function pointer. */
typedef P2FUNC(Fee_Bool, FEE_PRIVATE_CODE, Fee_QryFctPtrType)(void);

/* Type of an action function pointer. */
typedef P2FUNC(void, FEE_PRIVATE_CODE, Fee_ActFctPtrType)(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_InternalIsInFblMode
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks if FEE is in fbl mode
 * \details    Checks for incomplete configuration or forced fbl mode (in context of data conversion)
 * \param[in]  g
 * \return     TRUE: FEE is in fbl mode
 * \return     FALSE: FEE is not in fbl mode
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_InternalIsInFblMode(Fee_GlobalsConstPtrType g);

/**********************************************************************************************************************
 * Fee_Proc_readSectorHeader
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to sector module in order to read the sector header of given sector into given buffer
 * \details    Forwards to sector module in order to read the sector header of given sector into given buffer
 * \param[in]  sector
 * \param[in,out]  buf
 * \return     FEE_REQ_ACCEPTED: Flash accepted read request
 * \return     FEE_REQ_REJECTED: Flash rejected read request
 * \pre        sector not null, buf not null
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INLINE_FUNC(enum Fee_FlsRequestStatus) Fee_Proc_readSectorHeader(Fee_SectorConstRefType sector, Fee_IntBuffer_u32PtrType buf);

/**********************************************************************************************************************
 * Fee_Proc_writeSectorHeader
 *********************************************************************************************************************/
/*!
 * \brief      Requests flash to write the sector header of given sector from given buffer
 * \details    Requests flash to write the sector header of given sector from given buffer
 * \param[in]  sector
 * \param[in]  buf
 * \return     FEE_REQ_ACCEPTED: Flash accepted write request
 * \return     FEE_REQ_REJECTED: Flash rejected write request
 * \pre        sector not null, buf not null
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INLINE_FUNC(enum Fee_FlsRequestStatus) Fee_Proc_writeSectorHeader(Fee_SectorConstRefType sector, Fee_IntBuffer_u32ConstPtrType buf);

/**********************************************************************************************************************
 * Fee_Proc_getSectorEndOffset
 *********************************************************************************************************************/
/*!
 * \brief      Returns sector's end address as offset
 * \details    (start + size) of the sector minus 1.
 * \param[in]  sector
 * \return     sectorEndAddress
 * \pre        sector not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_Proc_getSectorEndOffset(Fee_SectorConstRefType sector);

/**********************************************************************************************************************
 * Fee_Proc_isNewestSector
 *********************************************************************************************************************/
/*!
 * \brief      Checks if given sector equals partition's new sector
 * \details    Checks if given sector equals partition's new sector
 * \param[in]  part
 * \param[in]  sector
 * \return     TRUE: Given sector is partition's new sector
 * \return     FALSE: Given sector is NOT partition's old sector
 * \pre        part not null, sector not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_isNewestSector(Fee_PartitionConstRefType part, Fee_SectorConstRefType sector);

/**********************************************************************************************************************
 * Fee_Proc_doesChunkEqualConfig
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks if given chunk's payload size equals the configured payload size
 * \details    A null-Config indicates FBL mode -> config implicitly matches.
 * \param[in]  chunk
 * \param[in]  config_pt (not NULL)
 * \return     TRUE: Chunk's payload size is as configured; FALSE otherwise
  * \pre       chunk not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_doesChunkEqualConfig(Fee_ChunkInfoConstRefType chunk, Fee_BlockConfigRefType config_pt);

/**********************************************************************************************************************
 * Fee_Proc_isLinktableChunkAllowed
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks if it is allowed to allocate a linktable chunk
 * \details    Checks if it is allowed to allocate a linktable chunk
 * \param[in]  chunk
 * \return     TRUE: Allocating the linktable chunk is allowed
 * \return     FALSE: Allocating the linktable chunk is not allowed
 * \pre        chunk is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_isLinkTableChunkAllowed(Fee_ChunkInfoConstRefType chunk);

/**********************************************************************************************************************
 * Fee_Proc_CreateChunkAllocContext
 *********************************************************************************************************************/
/*!
 * \brief      Creates the chunk allocation context out of the given information
 * \details    Creates the chunk allocation context out of the given information
 * \param[in,out]  ctx
 * \param[in,out]  recentChunk
 * \param[in]  blockId
 * \param[in]  cfg_pt; may be NULL
 * \pre        ctx not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Proc_CreateChunkAllocContext(Fee_ChunkAllocContextRefType ctx,
                                                      Fee_InstanceDescriptorRefType recentChunk,
                                                      struct Fee_BlockIdStruct blockId,
                                                      Fee_BlockConfigRefType cfg_pt);

/**********************************************************************************************************************
 * Fee_Proc_Write_SetupChunkAlloc
 *********************************************************************************************************************/
/*!
 * \brief      Starts the chunk allocation
 * \details    Decides whether data chunk or linktable chunk shall be allocated
 * \param[in,out]  g
 * \param[in]  ctx
 * \pre        ctx not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Proc_Write_SetupChunkAlloc(Fee_GlobalsPtrType g, Fee_ChunkAllocContextRefType ctx);

/**********************************************************************************************************************
 * Fee_Proc_Write_SetupAllocation
 *********************************************************************************************************************/
/*!
 * \brief      Decides whether chunk must be allocated or instance can be written immediately
 * \details    Decides whether chunk must be allocated or instance can be written immediately
 * \param[in,out]  g
 * \param[in,out]  ctx
 * \pre        ctx is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_Proc_Write_SetupAllocation(Fee_GlobalsPtrType g, Fee_ChunkAllocContextRefType ctx);

/**********************************************************************************************************************
 * Fee_Proc_InitReverseChunkSearch
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the reverse chunk search
 * \details    Initializes the reverse chunk search
 * \param[in,out]  g
 * \param[in,out]  sector
 * \param[in]  limit: Limit where the search shall stop. Otherwise FEE would find the same defect chunk over and over again
 * \pre        sector not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Proc_InitReverseChunkSearch(Fee_GlobalsPtrType g, Fee_SectorRefType sector, Fee_LengthType limit);

/**********************************************************************************************************************
 * Fee_Proc_InitReversChunkSearchInOlderSector
 *********************************************************************************************************************/
/*!
 * \brief      Starts reverse chunk search in the older sector
 * \details    Starts reverse chunk search only, when newer sector was searched before. Otherwise old sector has already
 *             been checked and searching it again makes no sense.
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       Sets local job result to failed in order to signal that reverse chunk search terminated unsuccessfully.
 *             Both sectors where checked, but nothing was found
 */
FEE_INLINE_FUNC(void) Fee_Proc_InitReversChunkSearchInOlderSector(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_Proc_InitReverseChunkSearchInSameSector
 *********************************************************************************************************************/
/*!
 * \brief      Starts reverse chunk search in the same sector where the defective chunk was found
 * \details    Starts reverse chunk search only, when previous chunk has already been checked
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       FEE always keeps track of the penultimate chunk. In case the most recent chunk is defective, FEE can check
 *             this previous chunk without any additional search effort. In case this chunk is defective too or there is no
 *             previous chunk, reverse chunk hopping is the last chance to find any data.
 */
FEE_INLINE_FUNC(void) Fee_Proc_InitReverseChunkSearchInSameSector(Fee_GlobalsPtrType g);

/******************************************************************************
 *  ACTIONS
 *****************************************************************************/

/**********************************************************************************************************************
 * Fee_ActCalcNextInstanceIndexBinary
 *********************************************************************************************************************/
/*!
 * \brief      Computes the next binary step and executes it if necessary. Otherwise search is terminated
 * \details    Calculates the next instance index and requests flash to read the instance header of this instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActCalcNextInstanceIndexBinary(void);

/**********************************************************************************************************************
 * Fee_ActDecrementToPrevInstanceIndex
 *********************************************************************************************************************/
/*!
 * \brief      Reads the next instance within lineary search.
 * \details    Reads the next instance within lineary search.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActDecrementToPrevInstanceIndex(void);

/**********************************************************************************************************************
 * Fee_InstSearch_InitContext
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the instance search context
 * \details    Initializes the instance search context
 * \param[in,out]  self
 * \param[in,out]  desc
 * \param[in]  startIndex
 * \pre        self not null, desc not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_InstSearch_InitContext(Fee_InstanceSearchContextRefType self, Fee_InstanceDescriptorRefType desc, uint16 startIndex);

/**********************************************************************************************************************
 * Fee_InstSearch_continue
 *********************************************************************************************************************/
/*!
 * \brief      Reads the instance header described in given instance search context
 * \details    Reads the instance header described in given instance search context
 * \param[in,out]  context_pt
 * \pre        context_pt is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_InstSearch_continue(Fee_InstanceSearchContextRefType context_pt);

/**********************************************************************************************************************
 * Fee_InstSearch_finalize
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes the instance search
 * \details    Finalizes the instance search
 * \param[in,out]  instDescr
 * \param[in]  inst: found instance
 * \pre        instDescr not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_InstSearch_finalize(Fee_InstanceDescriptorRefType instDescr, struct Fee_tagInstanceDescrPair inst);

/**********************************************************************************************************************
 * Fee_InstSearch_BinarySplitRange
 *********************************************************************************************************************/
/*!
 * \brief      Computes binary step and adapts instance index according to current instance status
 * \details    Computes binary step and adapts instance index according to current instance status
 * \param[in,out]  context_pt
 * \param[in]  status
 * \return     TRUE: Search is not finished, next instance must be read
 * \return     FALSE: Search is finished.
 * \pre        context_pt is not null
 * \context    TASK
 * \reentrant  FALSE
 * \note       The binary step is halved at every call of the function. According to the instance status FEE decides
 *             whether to step left or to step right. When current instance is empty, there can be no further valid instance
 *             right, the valid instance must be left, so FEE reduces the instance index by the computed binary steps.
 *             When the current instance is not empty, the most recent valid instance can't be left, so FEE increases the
 *             instance index by the computed binary steps.
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InstSearch_BinarySplitRange(Fee_InstanceSearchContextRefType context_pt, Fee_InstanceStatusType status);

/**********************************************************************************************************************
 * Fee_InstSearch_LinearDown
 *********************************************************************************************************************/
/*!
 *
 * \brief      Calculates the next instance index for lineary search
 * \details    Decrements the instance index as long as search must be continued (i.e. instance status is empty and
 *             instance index is greater than 0)
 * \param[in,out]  context_pt
 * \param[in]  status
 * \return     TRUE: Search must be continued with the computed instance index
 * \return     FALSE: Search end reached.
 * \pre        context_pt is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InstSearch_LinearDown(Fee_InstanceSearchContextRefType context_pt, Fee_InstanceStatusType status);

/**********************************************************************************************************************
 * Fee_ActContinueUpwardsChunkSearch
 *********************************************************************************************************************/
/*!
 * \brief      Starts chunk search in the new sector
 * \details    Starts chunk search in the new sector by requesting flash to read the first chunk link
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActContinueUpwardsChunkSearch(void);

/**********************************************************************************************************************
 * Fee_ActEvaluateChunkHopping
 *********************************************************************************************************************/
/*!
 *
 * \brief      This function evaluates the block information which is located in the chunk header, to decide
 *             about further chunk-hopping processing (or finalization).
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateChunkHopping(void);

/**********************************************************************************************************************
 * Fee_ActEvaluateInstanceTrailer
 *********************************************************************************************************************/
/*!
 *
 * \brief      Evaluates instance trailer
 * \details    Sets local job result to failed when trailer's status is not ok. Sets local job result to ok when trailer's
 *             status is ok.
 * \pre        instance header status is ok
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateInstanceTrailer(void);

/**********************************************************************************************************************
 * Fee_ActEvaluateSectorHeaders
 *********************************************************************************************************************/
/*!
 *
 * \brief      Creates upper sector's info out of buffer and sets the current sector according to both sector infos
 * \details    Creates upper sector's info out of buffer and sets the current sector according to both sector infos
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateSectorHeaders(void);

#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_ActEvaluateWriteCycle
 *********************************************************************************************************************/
/*!
 * \brief      Evaluates the WCC stored in the chunk header of the chunk
 * \details    Increases the WCC written in chunk header by the number of instances already written to this most recent
 *             chunk.
 * \config     FEE_GET_WRITE_CYCLE_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note        Only data chunks have a WCC. The WCC of linktable chunks is always reported 0.
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateWriteCycle(void);
#endif

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkSearch
 *********************************************************************************************************************/
/*!
 *
 * \brief      Copies the needed chunk infos (most recent chunk and previous chunk) and prepares binary search
 * \details    Copies the needed chunk infos (most recent chunk and previous chunk) and prepares binary search
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkSearch(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * \brief      Only calls the corresponding notification callback. No effect for the processing.
 * \details    Only calls the corresponding notification callback. No effect for the processing.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkLinkWrite(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkAllocation
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes the chunk allocation process.
 * \details    Finalizes the chunk allocation process.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkAllocation(void);

#if (STD_ON == FEE_GET_ERASE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_ActFinalizeEraseCycleJob
 *********************************************************************************************************************/
/*!
 *
 * \brief      This function writes the previously read Erase Cycle back to the pointer given by the API call.
 * \details    This function writes the previously read Erase Cycle back to the pointer given by the API call.
 * \param[in,out]  target
 * \param[in]  part
 * \param[in]  sectorId
 * \config     FEE_GET_ERASE_CYCLE_API is enabled via pre-compile switch
 * \pre        target not null, part not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC (void) Fee_IntFinalizeEraseCycleJob(Fee_UserData32PtrType target,
                                                                      Fee_PartitionConstRefType part, uint8 sectorId);
#endif

/**********************************************************************************************************************
 * Fee_ActFinalizeGetNextBlockFsm
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes 'get next block' FSM during sector switch
 * \details    Finalizes 'get next block' FSM during sector switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeGetNextBlockFsm(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeBlockCopy
 *********************************************************************************************************************/
/*!
 *
 * \brief      Finalizes the block copy process during sector switch or data conversion
 * \details    Advances to the next block to copy
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       The actual next block to copy will be determined by function Fee_ActGetNextBlockToCopy. Here FEE just
 *             signals that current block/dataset is finished and next block can be processed.
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeBlockCopy(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeSsCopying
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes sector switch
 * \details    Marks old sector to be erased and resets sector switch variables for next sector switch
 * \pre        All blocks are copied
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSsCopying(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeSetupSectors
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes the setup sectors sub state machine.
 * \details    Switches sectors and updates sector id when writing the sector header succeeded. Otherwise sector format
 *              error will be handled according to error callback.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSetupSectors(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeInstanceWrite
 *********************************************************************************************************************/
/*!
 *
 * \brief      Only calls the corresponding notification callback. No effect for the processing.
 * \details    Only calls the corresponding notification callback. No effect for the processing.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeInstanceWrite(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeInstanceCopy
 *********************************************************************************************************************/
/*!
 * \brief      Only calls the corresponding notification callback. No effect for the processing.
 * \details    Only calls the corresponding notification callback. No effect for the processing.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeInstanceCopy(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeJob
 *********************************************************************************************************************/
/*!
 *
 * \brief      Finalizes the job by setting the job results and by calling the client's callbacks
 * \details    Finalizes job only when it is really finished. Deferred jobs may be executed after a sector switch, so they are
 *             not allowed to be finalized yet.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       In case any threshold was exceeded during executing the job, this function requests a sector switch. FEE can
 *             execute the sector switch in background afterwards, when there is nothing else to do (no job).
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeJob(void);

#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_ActFinalizeWriteCycleJob
 *********************************************************************************************************************/
/*!
 *
 * \brief      This function writes the write cycle counter to the user buffer and sets the local job result accordingly
 * \details    This function writes the write cycle counter to the user buffer and sets the local job result accordingly
 * \config     FEE_GET_WRITE_CYCLE_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActFinalizeWriteCycleJob(void);

 /**********************************************************************************************************************
 * Fee_ActPrepareWccRead
 *********************************************************************************************************************/
  /*!
   * \brief      Requests Flash to read the write cycle counter out of current chunk's header
   * \details    Fakes a successful read when chunk is no data chunk. The WCC of a linktable chunk is always 0.
   * \config     GetWriteCycle API is enabled via pre-compile switch
   * \pre        -
   * \context    TASK
   * \reentrant  FALSE
   */
    FEE_LOCAL_FUNC(void) Fee_ActPrepareWccRead(void);
#endif

/**********************************************************************************************************************
 * Fee_ActGetCurrentChunkInfo
 *********************************************************************************************************************/
/*!
 * \brief      Checks if previous chunk link is defective and inherits this information by cloning the corresponding
 *             chunk infos.
 * \details    Checks if previous chunk link is defective and inherits this information by cloning the corresponding
 *             chunk infos.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActGetCurrentChunkInfo(void);

#  if (FEE_DATA_CONVERSION_API == STD_ON)

/**********************************************************************************************************************
 * Fee_ActGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * \brief      Just a wrapping function to call  Fee_InternalGetNextBlockToCopy
 * \details    Just a wrapping function to call  Fee_InternalGetNextBlockToCopy
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActGetNextBlockToCopy(void);

/**********************************************************************************************************************
 * Fee_ActInitChunkAllocFsmDc
 *********************************************************************************************************************/
/*!
 * \brief      Starts the chunk allocation sub state machine for data chunk
 * \details    Starts the chunk allocation sub state machine for data chunk
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       The data chunk that shall be allocated was stored in the previous chunk! The target chunk is used for the
 *             linktable chunk, which shall be allocated before, if necessary.
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitChunkAllocFsmDc(void);

 /**********************************************************************************************************************
 * Fee_ActPrepareReadInstanceData
 *********************************************************************************************************************/
/*!
 *
 * \brief      Requests flash to read the payload of the current instance
 * \details    Requests flash to read the payload of the current instance
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActPrepareReadInstanceData(void);

/**********************************************************************************************************************
 * Fee_ActInitDCWriteProcSubFsm
 *********************************************************************************************************************/
/*!
 * \brief      Gets configuration of current block, determines how to convert it and starts the actual conversion.
 * \details    The conversion starts by allocating the linktable chunk by starting the corresponding sub state machine
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitDCWriteProcSubFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitInstanceWriteDcFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initializes instance write context and starts the corresponding sub state machine to write the instance
 * \details    Initializes instance write context and starts the corresponding sub state machine to write the instance
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitInstanceWriteDcFsm(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeBlockCopyDc
 *********************************************************************************************************************/
/*!
 * \brief      Switches to current chunk's successor, which shall be searched next
 * \details    Switches to current chunk's successor, which shall be searched next
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActFinalizeBlockCopyDc(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeDataConversion
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes data conversion when all partitions are handled. Otherwise data conversion is continued with
 *             next partition.
 * \details    Partitions are handled in descending order.
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_ActFinalizeDataConversion(void);

/**********************************************************************************************************************
 * Fee_InternalInitDataConversionFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts searching the next block for current partition
 * \details    Starts searching the next block for current partition
 * \param[in,out]  globals
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
    FEE_LOCAL_FUNC(void) Fee_InternalInitDataConversionFsm(Fee_GlobalsPtrType globals);
#  endif

/**********************************************************************************************************************
 * Fee_ActIncrementToNextInstanceIndex
 *********************************************************************************************************************/
/*!
 *
 * \brief      Increments the instance index to be able to search upwards
 * \details    Increments the instance index to be able to search upwards
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActIncrementToNextInstanceIndex(void);

/**********************************************************************************************************************
 * Fee_ActInitChunkHoppingFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the chunk hopping sub state machine by preparing the chunk search context.
 * \details    Chunk search starts at the end address of the most recent chunk
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkHoppingFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitChunkSearchFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the chunk search sub state machine
 * \details    Initializes the chunk search sub state machine
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkSearchFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitChunkHeaderCheckFsm
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to read the corresponding chunk header and starts the chunk header consistency check
 * \details    Fakes successful read in case chunk is the linktable.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       When previous link and current chunk header are not consistent, chunk hopping will start at the very
 *             beginning of the sector.
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkHeaderCheckFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitCopyProcessFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts the block copying process and checks whether copying is allowed at all.
 * \details    Starts the block copying process and checks whether copying is allowed at all.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitCopyProcessFsm(void);

/**********************************************************************************************************************
 * Fee_InternalDoCopyInit
 *********************************************************************************************************************/
/*!
 *
 * \brief      Determine whether allocating a new chunk is necessary or if instance can be written immediately
 * \details    Starts the corresponding sub state machine or fakes successful copying in case chunk is a linktable chunk
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_InternalDoCopyInit(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_ActInitCreateInstanceFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts creating the instance in three write streams when job is a write job or writes an empty data instance
 *             when job is an invalidate/erase immediate job.
 * \details    This function does NOT write critical instances into newer sector. In such case the deferred job flag is
 *             NOT reset and the local job result is set to failed. The block will be written again after the sector switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       FEE is allowed to allocate a new chunk even for critical blocks in new sector. Critical block handling is
 *             done while instance creation. This means there is a temporary empty chunk of the critical block in new
 *             sector when sector switch starts. This is not an issue!
 */
FEE_LOCAL_FUNC(void) Fee_ActInitCreateInstanceFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitInstanceEvaluationFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts reading the instance trailer when instance header is ok. Otherwise the local job result is set
 *             according to instance status.
 * \details    Starts reading the instance trailer when instance header is ok. Otherwise the local job result is set
 *             according to instance status.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceEvaluationFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitInstanceEvaluationUpwardsFsm
 *********************************************************************************************************************/
/*!
 * \brief      Gets the current instance's status from instance header and starts the instance evaluation sub state machine
 * \details    Gets the current instance's status from instance header and starts the instance evaluation sub state machine
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceEvaluationUpwardsFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitInstanceSearchBinaryFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initiates binary search sub state machine by reading the first instance's header. Shall search the most
 *             recent valid instance.
 * \details    The first instance to be read is the middle instances (e.g. index 1 when there are 3 instances)
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchBinaryFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitInstanceSearchLinearyFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initiates the lineary search by reading the first instance's header. Shall search the most
 *             recent valid instance.
 * \details    The first instance to be read is the most right instance (i.e. the instance with the highest index)
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       <Particularities of the function. Remove if there are none.>
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchDownwardsFsm(void);

/**********************************************************************************************************************
 * Fee_Act_InitInstanceSearchUpwards
 *********************************************************************************************************************/
/*!
 * \brief      Initiates the instance upards search by reading the first instance's header. Shall search the first
 *             valid instance.
 * \details    The first instance to be read ist the most left instance (i.e. the instance with index 0)
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchUpwards(void);

/**********************************************************************************************************************
 * Fee_InternalInitLinkTableAllocationFsm
 *********************************************************************************************************************/
/*!
 * \brief      Checks if a linktable chunk is necessary and allowed and starts its allocation if so.
 * \details    If no linktable chunk has to be allocated, a successful allocation will be faked.
 * \param[in,out]      pointer to Fee_Globalst data structure (not NULL)
 * \pre        Fee_Globals_t has to be valid (especially CurrentDataSet_t, SectorSwitchTarget_t)
 * \context    TASK
 * \reentrant  FALSE
 * \note       Chunk which shall be allocated has to be stored in SectorSwitchTarget
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitLinkTableAllocationFsm(Fee_GlobalsPtrType);

/**********************************************************************************************************************
 * Fee_ActInitReverseChunkSearch
 *********************************************************************************************************************/
/*!
 * \brief      Initiates reverse chunk search
 * \details    Decides in which sector reverse chunk search shall be started
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       FEE always keeps track of previous chunk. If there is no previous chunk (i.e. the linktable), it would
 *             make no sense to search within the same sector. In such case the other sector must be used.
 */
FEE_LOCAL_FUNC(void) Fee_ActInitReverseChunkSearch(void);

/**********************************************************************************************************************
 * Fee_ActInitSearchWrittenInOldSector
 *********************************************************************************************************************/
/*!
 * \brief      Initiates variables needed for the search for the most recent instance in old sector
 * \details    Initiates variables needed for the search for the most recent instance in old sector
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       ChunkSearchContext is still valid, don't need to re-initialize it here.
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchWrittenInOldSector(void);

/**********************************************************************************************************************
 * Fee_ActInitSearchUpwardsFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initiates variables needed for the search for the first valid instance in new sector and starts search
 *             by reading the first chunk link.
 * \details    Initiates variables needed for the search for the first valid instance in new sector and starts search
 *             by reading the first chunk link.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchUpwardsFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitSearchWrittenFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts search for the most recent instance in new sector
 * \details    Starts search for the most recent instance in new sector
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchWrittenFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitSetupSectorsFsm
 *********************************************************************************************************************/
/*!
 * \brief      Computes the needed linktable size after setup and requests Flash to erase the old sector
 * \details    New linktable size is the maximum of old sectors linktable size and the configured linktable size
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSetupSectorsFsm(void);

/**********************************************************************************************************************
 * Fee_ActSetupSectorsWriteHeader
 *********************************************************************************************************************/
/*!
 * \brief      Assembles the sector header into buffer and requests Flash to write it
 * \details    Assembles the sector header into buffer and requests Flash to write it
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupSectorsWriteHeader(void);

/**********************************************************************************************************************
 * Fee_ActInitSearchForNfa_WriteFsm
 *********************************************************************************************************************/
/*!
 * \brief      Just a wrapping function: Calls  Fee_InternalInitSearchForNfaFsm for new sector
 * \details    Just a wrapping function: Calls  Fee_InternalInitSearchForNfaFsm for new sector
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchForNfa_WriteFsm(void);

/**********************************************************************************************************************
 * Fee_ActFinalizeSearchForNfa_WriteFsm
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes search for NFA by starting the binary search sub state machine
 * \details    In case of a linktable chunk search is finished and current sub state machine is left
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSearchForNfa_WriteFsm(void);

/**********************************************************************************************************************
 * Fee_ActPreInitWriteProcessing
 *********************************************************************************************************************/
/*!
 * \brief      Prepares chunk allocation context needed for writing
 * \details    Prepares chunk allocation context needed for writing
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPreInitWriteProcessing(void);

/**********************************************************************************************************************
 * Fee_ActInitWriteProcessingFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts the write processing in case FSS threshold is not exceeded and there is no broken link when FEE
 *             allocates in old sector.
 * \details    In case write processing is not allowed to be started, deferred job flag is set, if allowed (i.e. FSS is
 *             allowed).
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitWriteProcessingFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitDataChunkAllocWriteFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initiates allocating the data chunk
 * \details    Initiates allocating the data chunk
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitDataChunkAllocWriteFsm(void);

/**********************************************************************************************************************
 * Fee_ActInitDataChunkAllocCopyFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initiates allocating the data chunk during copy processing (sector switch)
 * \details    It's an Action executed by the internal FSM
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitDataChunkAllocCopyFsm(void);

/**********************************************************************************************************************
 * Fee_ActAllocationInCurrentSetor
 *********************************************************************************************************************/
/*!
 * \brief      Starts writing the block to newer sector in case FEE tried in old sector before but did not succeed
 * \details    Starts writing the block to newer sector in case FEE tried in old sector before but did not succeed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActAllocationInCurrentSector(void);

/**********************************************************************************************************************
 * Fee_ActInitializeBlockCopy
 *********************************************************************************************************************/
/*!
 * \brief      Initialize copying the current block by computing source and target address.
 * \details    Starts reading source instance when it is valid, copies just an empty instance when it is inconsistent and
 *             fakes successful copying when it is invalid.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActInitializeBlockCopy(void);

/**********************************************************************************************************************
 * Fee_ActNop
 *********************************************************************************************************************/
/*!
 * \brief      This function is just a dummy function needed by the state machine to pad unused action locations.
 * \details    This function is just a dummy function needed by the state machine to pad unused action locations.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActNop(void);

/**********************************************************************************************************************
 * Fee_ActPrepareBlockCopyRead
 *********************************************************************************************************************/
/*!
 * \brief      Reads the next bytes of instance that fit into buffer
 * \details    Instance is read in several steps depending on buffer size. Address for next step is adapted here too.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareBlockCopyRead(void);

/**********************************************************************************************************************
 * Fee_ActPrepareBlockCopyWrite
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write the next bytes of instance that fit into buffer
 * \details    Forwards to write the next bytes of instance that fit into buffer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareBlockCopyWrite(void);

/**********************************************************************************************************************
 * Fee_ActPrepareChunkHeaderWrite
 *********************************************************************************************************************/
/*!
 * \brief      Starts chunk header writing when current chunk fits into sector, otherwise local job result is set to failed
 * \details    Starts chunk header writing when current chunk fits into sector, otherwise local job result is set to failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkHeaderWrite(void);

/**********************************************************************************************************************
 * Fee_ActPrepareChunkLinkReadOfCurrentChunk
 *********************************************************************************************************************/
/*!
 * \brief      Sets up Flash request to read chunk link
 * \details    Some blocks have no link within the linktable (they don't fit into it). In such case the reading is faked and
 *             the 'link present' flag is reset.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkLinkReadOfCurrentChunk(void);

/**********************************************************************************************************************
 * Fee_ActPrepareChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write chunk link when previous flash job succeeded
 * \details    Forwards to write chunk link when previous flash job succeeded
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkLinkWrite(void);

/**********************************************************************************************************************
 * Fee_ActPrepareInstanceHeaderRead
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to read currents instance's header
 * \details    Forwards to read currents instance's header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareInstanceHeaderRead(void);

/**********************************************************************************************************************
 * Fee_ActPrepareInstanceTrailerRead
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to read current instance's trailer
 * \details    Requests Flash to read current instance's trailer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareInstanceTrailerRead(void);

/**********************************************************************************************************************
 * Fee_ActPreparePayloadDataProcessing
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to read current instance's payload
 * \details    Requests Flash to read current instance's payload
 * \pre        Instance is valid
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPreparePayloadDataProcessing(void);

/**********************************************************************************************************************
 * Fee_ActContinueSearchForNfA
 *********************************************************************************************************************/
/*!
 * \brief      Re-initializes chunk hopping in order to search for NfA
 * \details    Re-initializes chunk hopping in order to search for NfA
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActContinueSearchForNfA(void);

/**********************************************************************************************************************
 * Fee_ActPrepareSectorHeaderUpperRead
 *********************************************************************************************************************/
/*!
 * \brief      Creates lower sector's sector info out of buffer and requests Flash to read upper sector's header
 * \details    Creates lower sector's sector info out of buffer and requests Flash to read upper sector's header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareSectorHeaderUpperRead(void);

/**********************************************************************************************************************
 * Fee_ActPrepareWriteStreamLast
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write last write stream when there are still some bytes left to write
 * \details    Forwards  to write last write stream when there are still some bytes left to write
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       Depending on payload size the instances is written in upto 3 write streams, because instance header and
 *             trailer must be appended. This means user buffer can not be used and FEE must use a internal buffer with
 *             a restricted size.
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareWriteStreamLast(void);

/**********************************************************************************************************************
 * Fee_ActPrepareWriteStreamSecond
 *********************************************************************************************************************/
/*!
 * \brief      Requests Flash to write all remaining bytes that don't fit into internal buffer
 * \details    Requests Flash to write all remaining bytes that don't fit into internal buffer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       Depending on payload size the instances is written in upto 3 write streams, because instance header and
 *             trailer must be appended. This means user buffer can not be used and FEE must use a internal buffer with
 *             a restricted size.
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareWriteStreamSecond(void);

/**********************************************************************************************************************
 * Fee_ActReportSectorOverflow
 *********************************************************************************************************************/
/*!
 * \brief      Triggers sector overflow handling when chunk allocation failed due to a failed sector.
 * \details    Sector overflow is handled according to configured user callback
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActReportSectorOverflow(void);

/**********************************************************************************************************************
 * Fee_ActResetChunkSearch
 *********************************************************************************************************************/
/*!
 * \brief      Resets chunk search context.
 * \details    Resets chunk search context.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActResetChunkSearch(void);

/**********************************************************************************************************************
 * Fee_ActReturnFromSubFsm
 *********************************************************************************************************************/
/*!
 * \brief      Decrements the FSM stack pointer
 * \details    Decrements the FSM stack pointer
 * \pre        FSM stack pointer is not 0
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActReturnFromSubFsm(void);

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultFailed
 *********************************************************************************************************************/
/*!
 * \brief      Sets the LocalJobResult to Failed.
 * \details    Sets the LocalJobResult to Failed.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultFailed(void);

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultInvalid
 *********************************************************************************************************************/
/*!
 * \brief      Sets the LocalJobResult to Invalid.
 * \details    Sets the LocalJobResult to Invalid.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultInvalid(void);

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultOk
 *********************************************************************************************************************/
/*!
 * \brief      Sets the LocalJobResult to Ok.
 * \details    Sets the LocalJobResult to Ok.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultOk(void);

/**********************************************************************************************************************
 * Fee_ActSetupSectorSwitchSourceSearch
 *********************************************************************************************************************/
/*!
 * \brief      Initializes consistent chunk search in the old sector
 * \details    Initializes consistent chunk search in the old sector
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupSectorSwitchSourceSearch(void);

/**********************************************************************************************************************
 * Fee_ActSetupJobProcessing
 *********************************************************************************************************************/
/*!
 * \brief      Starts processing of the given user job if it is allowed
 * \details    Starts processing of the given user job if it is allowed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       Read jobs are allowed when FEE is NOT in 'reject all mode' and write like jobs are allowed when FEE is NEITHER
 *             in 'reject all mode' NOR in 'read only mode'
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupJobProcessing(void);

/**********************************************************************************************************************
 * Fee_ActSwitchOperatingSector
 *********************************************************************************************************************/
/*!
 * \brief      Initializes chunk search context to continue search in the old sector
 * \details    Initializes chunk search context to continue search in the old sector
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActSwitchOperatingSector(void);

/**********************************************************************************************************************
 * Fee_ActValidateChunkLink
 *********************************************************************************************************************/
/*!
 * \brief      Builds chunk info from buffer, evaluates it and continues chunk search according to chunk link's status
 * \details    Builds chunk info from buffer, evaluates it and continues chunk search according to chunk link's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActValidateChunkLink(void);

/**********************************************************************************************************************
 * Fee_ActCheckChunkConsistency
 *********************************************************************************************************************/
/*!
 * \brief      Checks if chunk header and previous chunk link are consistent.
 * \details    Starts chunk hopping at the beginning of the sector if chunk header and previous link are not consistent
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActCheckChunkConsistency(void);

/**********************************************************************************************************************
 * Fee_ActIssueDeferredRequest
 *********************************************************************************************************************/
/*!
 * \brief      Executes the deferred request
 * \details    Executes the deferred request
 * \pre        Writes are not suspended
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActIssueDeferredRequest(void);

/**********************************************************************************************************************
 * Fee_ActHandleWriteSuspended
 *********************************************************************************************************************/
/*!
 * \brief      Handles write suspension
 * \details    Handles write suspension.
 * \pre        Writes are suspended
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActHandleWriteSuspended(void);

/**********************************************************************************************************************
 * Fee_ActWait
 *********************************************************************************************************************/
/*!
 * \brief      Just wraps function Fee_SignalReEnterMainFunction. Sets wait flag and processing flag.
 * \details    Forces FEE to leave main function and enter the main function processing again
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_ActWait(void);

/**********************************************************************************************************************
 * Fee_Internal_SignalWait
 *********************************************************************************************************************/
/*!
 *
 * \brief      Sets wait flag.
 * \details    Forces FEE to leave main function
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_Internal_SignalWait(void);

/**********************************************************************************************************************
 * Fee_SignalReEnterMainFunction
 *********************************************************************************************************************/
/*!
 * \brief      Sets wait flag and processing flag.
 * \details    Forces FEE to leave main function and enter the main function processing again
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_SignalReEnterMainFunction(void);

/******************************************************************************
 *  QUERIES
 *****************************************************************************/

/**********************************************************************************************************************
 * Fee_QryBlockPayloadUpdated
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the payload has been updated for the current block.
 * \details    Checks, if the payload has been updated for the current block.
 * \return     TRUE: The block config of the current block has been updated.
 * \return     FALSE: The block config of the current block has (presumably) NOT been updated
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryBlockPayloadUpdated(void);

/**********************************************************************************************************************
 * Fee_QryBlockCopyBytesRemaining
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the BlockCopy procedure of the sector switch for a specific block/dataset has been finished.
 * \details    Checks, if the BlockCopy procedure of the sector switch for a specific block/dataset has been finished.
 * \return     TRUE: The BlockCopy procedure has not finished. Continue to copy the remaining parts
 *                   of the block/dataset or especially the instance.
 * \return     FALSE: The BlockCopy procedure has finished. The block/dataset or especially the
 *                    instance has been copied successfully from the old sector to the CurrentSector.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryBlockCopyBytesRemaining(void);

/**********************************************************************************************************************
 * Fee_QryChunkIsDataChunk
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the current chunk is a data chunk.
 * \details    Checks, if the current chunk is a data chunk.
 * \return     TRUE: The current chunk is a data chunk
 * \return     FALSE: The current chunk is a linktable chunk.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryChunkIsDataChunk(void);

/**********************************************************************************************************************
 * Fee_QryChunkNotFull
 *********************************************************************************************************************/
/*!
 *
 * \brief      Checks, if chunk is full or not.
 * \details    Checks, if the actual instance is empty or if an successor instance is available.
 * \return     TRUE: The chunk is not full.
 * \return     FALSE: The chunk is full.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryChunkNotFull(void);

/**********************************************************************************************************************
 * Fee_QryInstHeaderErasedValue
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if instance header is erased.
 * \details    Checks, if the first four bytes of the previously read bytes are equal to the ErasedValue.
 * \return     TRUE: First byte is equal to the ErasedValue.
 * \return     FALSE: First byte differs from ErasedValue.
 * \pre        Page size >= 4 or first bytes initialized with erase value when Page size < 4
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryInstHeaderErasedValue(void);

/**********************************************************************************************************************
 * Fee_QryFlsJobResultOk
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the FlsJobResult is Ok.
 * \details    Checks, if the FlsJobResult is Ok.
 * \return     TRUE: The FlsJobResult is Ok.
 * \return     FALSE: The FlsJobResult is not Ok.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryFlsJobResultOk(void);

/**********************************************************************************************************************
 * Fee_QryLocalJobResultEmptyChunks
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the LocalJobResult is equal to the EmptyChunks value.
 * \details    Checks, if the LocalJobResult is equal to the EmptyChunks value.
 * \return     TRUE: The LocalJobResult is equal to the EmptyChunks value.
 * \return     FALSE: The LocalJobResult is different from the EmptyChunks value.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultEmptyChunks(void);

/**********************************************************************************************************************
 * Fee_QryLocalJobResultFailed
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the LocalJobResult is equal to the Failed value.
 * \details    Checks, if the LocalJobResult is equal to the Failed value.
 * \return     TRUE: The LocalJobResult is equal to the Failed value.
 * \return     FALSE: The LocalJobResult is different from the Failed value.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultFailed(void);

/**********************************************************************************************************************
 * Fee_QryLocalJobResultInvalid
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the LocalJobResult is equal to the Invalid value.
 * \details    Checks, if the LocalJobResult is equal to the Invalid value.
 * \return     TRUE: The LocalJobResult is equal to the Invalid value.
 * \return     FALSE: The LocalJobResult is different from the Invalid value.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultInvalid(void);

/**********************************************************************************************************************
 * Fee_QryLocalJobResultOk
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the LocalJobResult is equal to the Ok value.
 * \details    Checks, if the LocalJobResult is equal to the Ok value.
 * \return     TRUE: The LocalJobResult is equal to the Ok value.
 * \return     FALSE: The LocalJobResult is different from the Ok value.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultOk(void);

/**********************************************************************************************************************
 * Fee_QryMayFollowLinks
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if following the chunk links is allowed
 * \details    Checks, if following the chunk links is allowed
 * \return     TRUE: Following the links is allowed
 * \return     FALSE: Following the links is not allowed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryMayFollowLinks(void);

/**********************************************************************************************************************
 * Fee_QryPreviousInstanceAvailable
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if a previous instance is available.
 * \details    A previous instance is available, if a previous chunk is available or the instance index does
 *             not point to the first instance within a chunk.
 * \return     TRUE: A previous instance is available.
 * \return     FALSE: A previous instance is not available. This includes, that no previous chunk is available.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryPreviousInstanceAvailable(void);

/**********************************************************************************************************************
 * Fee_QryRecentChunkLinkConsistent
 *********************************************************************************************************************/
/*!
 * \brief      Checks if the chunk link of the most recent chunk is consistent
 * \details    Checks if the chunk link of the most recent chunk is consistent
 * \return     TRUE: Chunk link is consistent
 * \return     FALSE: Chunk link is not consistent
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryRecentChunkLinkConsistent(void);

/**********************************************************************************************************************
 * Fee_QrySearchingInCurrentSector
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if the Fee is searching in the CurrentSector.
 * \details    Checks, if the OperatingSector is different from the CurrentSector.
 * \return     TRUE: The Fee searches in the CurrentSector.
 * \return     FALSE: The Fee searches in the old sector (= not CurrentSector).
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QrySearchingInCurrentSector(void);

/**********************************************************************************************************************
 * Fee_QryTrue
 *********************************************************************************************************************/
/*!
 * \brief      Just a dummy function needed by the state machine to pad unused query locations.
 * \details    Just a dummy function needed by the state machine to pad unused query locations.
 * \return     TRUE: Always
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryTrue(void);

/**********************************************************************************************************************
 * Fee_QryValidInstanceWritten
 *********************************************************************************************************************/
/*!
 * \brief      Checks, if a valid (i.e. consistent) Instance was found
 * \details    Checks, if a valid (i.e. consistent) Instance was found
 * \return     TRUE: Instance was written with valid status.
 * \return     FALSE: Instance was not written with valid status.
 * \pre        Instance Evaluation sub state machine was executed before
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryValidInstanceWritten(void);

/**********************************************************************************************************************
 * Fee_QryWriteSuspended
 *********************************************************************************************************************/
/*!
 * \brief      Checks if writes are suspended
 * \details    Checks if writes are suspended
 * \return     TRUE: Writes are suspended
 * \return     FALSE: Writes are not suspended
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryWriteSuspended(void);

/******************************************************************************
 *  INTERNAL FUNCTIONS
 *****************************************************************************/

/**********************************************************************************************************************
 * Fee_FsmProcess
 *********************************************************************************************************************/
/*!
 * \brief      Executes exactly one state of a state machine.
 * \details    Decides which state shall be the next state according to queries and executes the approproate actions
 * \param[in,out]  fsmStack
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_FsmProcess(Fee_FsmStackRefType fsmStack);

/**********************************************************************************************************************
 * Fee_FsmQuery
 *********************************************************************************************************************/
/*!
 * \brief      Executes both queries and ANDs their result
 * \details    Executes both queries and ANDs their result
 * \param[in]  Queries_at
 * \return     TRUE: All queries executed are true.
 * \return     FALSE: At least one query executed is false.
 * \pre        Queries_at not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_FsmQuery (Fee_StateQueryPtrType Queries_at);

/**********************************************************************************************************************
 * Fee_FsmIsIdle
 *********************************************************************************************************************/
/*!
 * \brief      Checks if top level sub state machine is the 'idle' sub state machine
 * \details    Checks if top level sub state machine is the 'idle' sub state machine
 * \param[in]  fsmStack
 * \return     TRUE: 'idle' sub state machine is running
 * \return     FALSE: 'idle' sub state machine is not running
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_FsmIsIdle(Fee_FsmStackConstRefType fsmStack);

/**********************************************************************************************************************
 * Fee_FsmSetupTopLevel
 *********************************************************************************************************************/
/*!
 * \brief      Sets the given top level sub state machine
 * \details    Sets fsmStack's stack pointer to 0 and sets the given top level sub state machine to fsmStack's bottom
 * \param[in,out]  fsmStack
 * \param[in]  state
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_FsmSetupTopLevel(Fee_FsmStackRefType fsmStack, Fee_StateType state);

/**********************************************************************************************************************
 * Fee_FsmInitSubFsm
 *********************************************************************************************************************/
/*!
 * \brief      Pushes the given sub state machine on the fsmStack (stack pointer is incremented)
 * \details    Pushes the given sub state machine on the fsmStack (stack pointer is incremented)
 * \param[in,out]  fsmStack
 * \param[in]  InitState_t
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_FsmInitSubFsm(Fee_FsmStackRefType fsmStack, Fee_StateType InitState_t);

/**********************************************************************************************************************
 * Fee_InternalGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to get next block to copy
 * \details    Forwards to get next block to copy
 * \param[in,out]  globals
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalGetNextBlockToCopy(Fee_GlobalsPtrType  globals);

/**********************************************************************************************************************
 * Fee_InternalFillInstanceBuffer
 *********************************************************************************************************************/
/*!
 * \brief      Fills the internal buffer with the number of given bytes from given user buffer
 * \details    Fills the internal buffer with the number of given bytes from given user buffer
 * \param[in,out]  instBuf
 * \param[in]  userBuf
 * \param[in]  length
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalFillInstanceBuffer(
    const Fee_IntBuffer_u8PtrType instBuf, Fee_UserDataConstPtrType userBuf, uint32 length);

/**********************************************************************************************************************
 * Fee_InternalEvaluateInstanceStatus
 *********************************************************************************************************************/
/*!
 * \brief      Gets instance's status out of instance's header within the given buffer
 * \details    Gets instance's status out of instance's header within the given buffer
 * \param[in]  sector_pt
 * \param[in]  intBuf_au8
 * \param[in]  flsResult
 * \return     instance's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_InstanceStatusType) Fee_InternalEvaluateInstanceStatus(
                                                Fee_SectorConstRefType sector_pt,
                                                Fee_IntBuffer_u8ConstPtrType intBuf_au8, MemIf_JobResultType flsResult);

/**********************************************************************************************************************
 * Fee_InternalEvalInstHdrStatus
 *********************************************************************************************************************/
/*!
 * \brief      Gets instances' status out of instance's header within given buffer and checks whether an empty instance
 *             is really empty.
 * \details    A instance with an erased instance header, but a non erased remaining page is not empty, but failed.
 * \param[in]  context_pt
 * \param[in]  flsResult
 * \return     instance's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_InstanceStatusType) Fee_InternalEvalInstHdrStatus(Fee_InstanceSearchContextConstRefType context_pt,
                                                MemIf_JobResultType flsResult);

/**********************************************************************************************************************
 * Fee_InternalPrepareEmptyInstanceWrite
 *********************************************************************************************************************/
/*!
 * \brief      Initiates writing an empty instance which has the given status
 * \details    Initiates writing an empty instance which has the given status
 * \param[in]  sector
 * \param[in]  addr
 * \param[in]  status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalPrepareEmptyInstanceWrite(Fee_SectorConstRefType sector, Fee_LengthType addr, Fee_InstanceStatusType status);

/**********************************************************************************************************************
 * Fee_InternalScheduleProcess
 *********************************************************************************************************************/
/*!
 * \brief      Does the process scheduling by deciding which process may be started next
 * \details    Does the process scheduling by deciding which process may be started next
 * \param[in,out]  g
 * \return     TRUE: Any process was started
 * \return     FALSE: No process was started. There is nothing to do.
 * \pre        'idle' sub state machine is running
 * \context    TASK
 * \reentrant  FALSE

 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalScheduleProcess(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalInitPartitionInitializationFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the partition and forwards to read the lower sector's header
 * \details    Initializes the partition an forwards to read the lower sector's header
 * \param[in,out]  g
 * \param[in,out]  part
 * \pre        Partition has not been initialied yet
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitPartitionInitializationFsm(Fee_GlobalsPtrType g, Fee_PartitionRefType part);

/**********************************************************************************************************************
 * Fee_Initialize_ProcessData
 *********************************************************************************************************************/
/*!
 * \brief      Initializes process data
 * \details    Initializes process data
 * \param[in,out]  self
 * \param[in,out]  part
 * \param[in]  block
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Initialize_ProcessData(Fee_ProcessDataRefType self, Fee_PartitionRefType part,
                                                                             struct Fee_tagBlockCfgDatasetPair block);

/**********************************************************************************************************************
 * Fee_InternalInitBusyFsm
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the job processing by preparing process data and starting the partition initialization if
 *             necessary
 * \details    Initializes the job processing by preparing process data and starting the partition initialization if
 *             necessary
 * \param[in,out]  globals
 * \param[in,out]  jobPartition_pt
 * \return     TRUE: Always
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalInitBusyFsm(Fee_GlobalsPtrType globals, Fee_PartitionRefType jobPartition_pt);

/**********************************************************************************************************************
 * Fee_InternalHandleCancelRequest
 *********************************************************************************************************************/
/*!
 * \brief      Cancels the state machine by resetting it
 * \details    Cancels Flash, if it is no idle
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalHandleCancelRequest(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalSetupToplevelFsm
 *********************************************************************************************************************/
/*!
 * \brief      Determines which toplevel sub state machine shall be started and initializes it
 * \details    Sets FEE's module status to idle when there is nothing to do
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalSetupToplevelFsm(Fee_GlobalsPtrType g);

#if (FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
/**********************************************************************************************************************
 * Fee_InternalForceSectorSwitchSetup
 *********************************************************************************************************************/
/*!
 * \brief      Demands block copying for every partition by setting the corresponding flag
 * \details    Demands block copying for every partition by setting the corresponding flag
 * \param[in]  cfg
 * \config     FEE_FORCE_SECTOR_SWITCH_API == STD_ON
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalForceSectorSwitchSetup(Fee_ConfigRefTypeInternal cfg);
#endif

/**********************************************************************************************************************
 * Fee_Internal_setToIdle
 *********************************************************************************************************************/
/*!
 * \brief      Sets module status to idle when FEE is not busy and no force sector switch was requested
 * \details    Sets module status to idle when FEE is not busy and no force sector switch was requested
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_Internal_setToIdle(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalInitSectorSwitchFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts sector switch processing if it is allowed and necessary
 * \details    Responsible for sector format, block copying and initializing the partitions in case it is a forced
 *             sector switch
 * \param[in,out]  globals
 * \param[in,out]  partition
 * \return     TRUE: Sector switch was started
 * \return     FALSE: No sector switch was started
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalInitSectorSwitchFsm(Fee_GlobalsPtrType globals, Fee_PartitionRefType partition);

/**********************************************************************************************************************
 * Fee_InternalRequestSectorSwitch
 *********************************************************************************************************************/
/*!
 * \brief      Signals that sector switch shall be processed on given partition and initializes the needed variables
 * \details    Signals that sector switch shall be processed on given partition and initializes the needed variables
 * \param[in,out]  g
 * \param[in]  partition
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalRequestSectorSwitch(Fee_GlobalsPtrType g, Fee_PartitionConstRefType partition);

/**********************************************************************************************************************
 * Fee_InternalResetSectorSwitch
 *********************************************************************************************************************/
/*!
 * \brief      Finalizes a sector switch by resetting the needed variables for next sector switch.
 * \details    Resets variables needed for searching the next block to copy
 * \param[in,out]  self
 * \param[in]  isFblMode
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void ) Fee_InternalResetSectorSwitch(Fee_SectorSwitchDataRefType self, Fee_Bool isFblMode);

/**********************************************************************************************************************
 * Fee_InternalInitSearchForNfaFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts searching for the next free address if the limit is not yet reached
 * \details    If the fill level is already known, nothing has to be done. Otherwise chunk hopping will be started.
 * \param[in,out]  globals_pt
 * \param[in,out]  sector_pt
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitSearchForNfaFsm(Fee_GlobalsPtrType globals_pt,
                                                                    Fee_SectorRefType sector_pt);

/**********************************************************************************************************************
 * Fee_InternalEvalFlsRetVal
 *********************************************************************************************************************/
/*!
 * \brief      Checks the return value of the flash api call and sets FEE's internal flash job result variable
 * \details    Checks the return value of the flash api call and sets FEE's internal flash job result variable
 * \param[in]
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalEvalFlsRetVal(enum Fee_FlsRequestStatus);

/**********************************************************************************************************************
 * Fee_InternalIsFlsBusy
 *********************************************************************************************************************/
/*!
 * \brief      Checks if the Flash driver is busy and gets the job result from it, if it is idle.
 * \details    Checks if the Flash driver is busy and gets the job result from it, if it is idle.
 * \param[in,out]  g
 * \return     TRUE: The flash driver is currently busy
 * \return     FALSE: The flash driver is currently idle and FEE's internal flash job result is updated
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_InternalIsFlsBusy(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_Proc_SetupWriteSuspendHandling
 *********************************************************************************************************************/
/*!
 * \brief      Does the write suspended handling by starting the 'write suspended' sub state machine
 * \details    The function which shall be called when writes are not supended is assigned to the corresponding variable
 *             via function pointer
 * \param[in,out]  globals
 * \param[in]  deferredRequest
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Proc_SetupWriteSuspendHandling(Fee_GlobalsPtrType, Fee_DeferredFuncType);

/**********************************************************************************************************************
 * Fee_InternalInitAllocateChunkFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts first step of the chunk allocation (i.e. searching the next free address via chunk hopping)
 * \details    Starts first step of the chunk allocation (i.e. searching the next free address via chunk hopping)
 * \param[in,out]  context_pt
 * \param[in]  newChunk
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitAllocateChunkFsm(Fee_ChunkAllocContextRefType context_pt, Fee_ChunkInfoConstRefType newChunk);

/**********************************************************************************************************************
 * Fee_InternalInitChunkHoppingFsm
 *********************************************************************************************************************/
/*!
 * \brief      Starts chunk hopping by reading next chunk header, if search limit is not reached yet
 * \details    Starts chunk hopping by reading next chunk header, if search limit is not reached yet
 * \param[in,out]  globals
 * \param[in,out]  context
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitChunkHoppingFsm(Fee_GlobalsPtrType, Fee_ChunkHoppingContextRefType);

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamFirst
 *********************************************************************************************************************/
/*!
 * \brief      Prepares the first write stream by filling the internal buffer with the instance header and the first
 *             payload data given by the upper layer
 * \details    Adapts source (user buffer) and target (internal buffer) for the next write stream, if needed.
 * \param[in,out]  globals
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \note       Depending on payload size the instances is written in upto 3 write streams, because instance header and
 *             trailer must be appended. This means user buffer can not be used and FEE must use a internal buffer with
 *             a restricted size.
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamFirst(Fee_GlobalsPtrType);

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamSecond
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write second write stream to Flash
 * \details    Forwards to write second write stream to Flash
 * \param[in,out]  g
 * \pre        Three write streams are needed
 * \context    TASK
 * \reentrant  FALSE
 * \note       Depending on payload size the instances is written in upto 3 write streams, because instance header and
 *             trailer must be appended. This means user buffer can not be used and FEE must use a internal buffer with
 *             a restricted size.
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamSecond(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamLast
 *********************************************************************************************************************/
/*!
 *
 * \brief      Forwards to write last write stream to Flash
 * \details    Fills internal buffer with remaining payload bytes, appends instance trailer and request Flash to write it
 * \param[in,out]  g
 * \pre        At least two write streams are needed
 * \context    TASK
 * \reentrant  FALSE
 * \note       Depending on payload size the instances is written in upto 3 write streams, because instance header and
 *             trailer must be appended. This means user buffer can not be used and FEE must use a internal buffer with
 *             a restricted size.
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamLast(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoBlockCopyWrite
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write content of internal buffer to Flash
 * \details    Fills internal buffer with remaining bytes to write, forwards to write it to Flash and adapts target address
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoBlockCopyWrite(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoEmptyInstanceWrite
 *********************************************************************************************************************/
/*!
 * \brief      Forwards to write an empty instance to Flash
 * \details    Forwards to write an empty instance to Flash
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoEmptyInstanceWrite(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoSectorErase
 *********************************************************************************************************************/
/*!
 *
 * \brief      Setup a sector erase request
 * \details    Forward to Fee_Sector and evaluate return value.
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoSectorErase(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoSectorHeaderWrite
 *********************************************************************************************************************/
/*!
 * \brief      Setup sector header write request.
 * \details    Forward to Fee_Sector and evaluate return value.
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoSectorHeaderWrite(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalDoChunkHeaderWrite
 *********************************************************************************************************************/
/*!
 * \brief      Setup Chunk Header Write request.
 * \details    Forward to Fee_ChunkInfo and evaluate return value.
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoChunkHeaderWrite(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_ChunkSearchContext_Init
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the chunk search context according to given information
 * \details    Initializes the chunk search context according to given information
 * \param[in,out]  self
 * \param[in]  blockId_pt
 * \param[in,out]  prevChunk_pt
 * \param[in,out]  recentChunk_pt
 * \param[in]  upperLimit_t
 * \pre        parameters are not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void ) Fee_ChunkSearchContext_Init(Fee_ChunkSearchContextRefType self,
                                                                 Fee_BlockIdConstRefType blockId_pt,
                                                                 Fee_ChunkInfoRefType prevChunk_pt,
                                                                 Fee_InstanceDescriptorRefType recentChunk_pt,
                                                                 Fee_LengthType upperLimit_t);

/**********************************************************************************************************************
 * Fee_InstanceDescr_Init
 *********************************************************************************************************************/
/*!
 * \brief      Initializes the instance description with the given index and the given status
 * \details    Initializes the instance description with the given index and the given status
 * \param[in]  self
 * \param[in]  index
 * \param[in]  status
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_InstanceDescr_Init(Fee_InstanceDescrPairRef self, uint16 index, Fee_InstanceStatusType status);

#if (FEE_DATA_CONVERSION_API == STD_ON)

/**********************************************************************************************************************
 * Fee_GetBlockCfgByBlockId
 *********************************************************************************************************************/
/*!
 * \brief      Gets block configuration for the given blockId and the given partition
 * \details    Gets block configuration for the given blockId and the given partition
 * \param[in,out]  cfg
 * \param[in]  blockId
 * \param[in]  partIndex
 * \return     NULL_PTR: Block is not configured
 * \return     blockCfg: Otherwise
 * \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_BlockConfigRefType) Fee_GetBlockCfgByBlockId(Fee_ConfigRefTypeInternal cfg,
                                                                               struct Fee_BlockIdStruct blockId,
                                                                               uint8 partIndex);
#endif

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define FEE_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

static CONST(uint8, FEE_PRIVATE_CONST) FEE_LOWER_SECTOR_NUMBER = 0u;
static CONST(uint8, FEE_PRIVATE_CONST) FEE_UPPER_SECTOR_NUMBER = 1u;

/******************************************************************************
 *  ACTIONS
 *****************************************************************************/
/* The action function pointer table; each enumerator in Fee_StateActionEnumType must have
 * its corresponding function, in order to serve as index into table.
 * Note that array's size must be inferred from the number of initializers.
 * Otherwise the, table size compile time check (refer to Fee_tagFsmTableSizesCheck) would not work. */
/* PRQA S 3218 1 */ /* MD_FEE_8.7_fsm_tables */
static CONST(Fee_ActFctPtrType, FEE_PRIVATE_CONST) Fee_ActionTable_ap[] =
{
    Fee_ActCalcNextInstanceIndexBinary,
    Fee_ActDecrementToPrevInstanceIndex,
    Fee_ActEvaluateChunkHopping,
    Fee_ActEvaluateInstanceTrailer,
    Fee_ActEvaluateSectorHeaders,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    Fee_ActEvaluateWriteCycle,
#endif
    Fee_ActFinalizeChunkSearch,
    Fee_ActFinalizeChunkLinkWrite,
    Fee_ActFinalizeChunkAllocation,
    Fee_ActFinalizeGetNextBlockFsm,
    Fee_ActFinalizeInstanceWrite,
    Fee_ActFinalizeInstanceCopy,
    Fee_ActFinalizeJob,
    Fee_ActFinalizeBlockCopy,
    Fee_ActFinalizeSsCopying,
    Fee_ActFinalizeSetupSectors,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    Fee_ActFinalizeWriteCycleJob,
#endif
    Fee_ActGetCurrentChunkInfo,
    Fee_ActIncrementToNextInstanceIndex,
    Fee_ActInitDataChunkAllocWriteFsm,
    Fee_ActInitDataChunkAllocCopyFsm,
    Fee_ActInitChunkHoppingFsm,
    Fee_ActInitChunkSearchFsm,
    Fee_ActInitChunkHeaderCheckFsm,
    Fee_ActInitCopyProcessFsm,
    Fee_ActInitCreateInstanceFsm,
    Fee_ActInitializeBlockCopy,
    Fee_ActInitInstanceEvaluationFsm,
    Fee_ActInitInstanceEvaluationUpwardsFsm,
    Fee_ActInitInstanceSearchBinaryFsm,
    Fee_ActInitInstanceSearchDownwardsFsm,
    Fee_ActInitInstanceSearchUpwards,
    Fee_ActInitReverseChunkSearch,
    Fee_ActInitSearchWrittenInOldSector,
    Fee_ActInitSearchUpwardsFsm,
    Fee_ActContinueUpwardsChunkSearch,
    Fee_ActInitSetupSectorsFsm,
    Fee_ActSetupSectorsWriteHeader,
    Fee_ActInitSearchForNfa_WriteFsm,
    Fee_ActFinalizeSearchForNfa_WriteFsm,
    Fee_ActPreInitWriteProcessing,
    Fee_ActInitWriteProcessingFsm,
    Fee_ActAllocationInCurrentSector,
    Fee_ActPrepareBlockCopyRead,
    Fee_ActPrepareBlockCopyWrite,
    Fee_ActPrepareChunkHeaderWrite,
    Fee_ActPrepareChunkLinkReadOfCurrentChunk,
    Fee_ActPrepareChunkLinkWrite,
    Fee_ActPrepareInstanceHeaderRead,
    Fee_ActPreparePayloadDataProcessing,
    Fee_ActContinueSearchForNfA,
    Fee_ActPrepareSectorHeaderUpperRead,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    Fee_ActPrepareWccRead,
#endif
    Fee_ActPrepareWriteStreamLast,
    Fee_ActPrepareWriteStreamSecond,
    Fee_ActReportSectorOverflow,
    Fee_ActResetChunkSearch,
    Fee_ActReturnFromSubFsm,
    Fee_ActSetLocalJobResultFailed,
    Fee_ActSetLocalJobResultInvalid,
    Fee_ActSetLocalJobResultOk,

    Fee_ActSetupSectorSwitchSourceSearch,

    Fee_ActSetupJobProcessing,
    Fee_ActSwitchOperatingSector,
    Fee_ActValidateChunkLink,
    Fee_ActCheckChunkConsistency,
#  if (FEE_DATA_CONVERSION_API == STD_ON)
    Fee_ActGetNextBlockToCopy,
    Fee_ActInitChunkAllocFsmDc,
    Fee_ActPrepareReadInstanceData,
    Fee_ActInitDCWriteProcSubFsm,
    Fee_ActInitInstanceWriteDcFsm,
    Fee_ActFinalizeBlockCopyDc,
    Fee_ActFinalizeDataConversion,
#  endif

    Fee_ActIssueDeferredRequest,
    Fee_ActHandleWriteSuspended,

    Fee_ActNop,
    Fee_ActWait /* must be the last action */
};

/******************************************************************************
 *  QUERIES
 *****************************************************************************/
/* The query function pointer table; each enumerator in Fee_StateQueryEnumType must have
 * its corresponding function, in order to serve as index into table.
 * Note that array's size must be inferred from the number of initializers.
 * Otherwise the, table size compile time check (refer to Fee_tagFsmTableSizesCheck) would not work. */
/* PRQA S 3218 1 */ /* MD_FEE_8.7_fsm_tables */
static CONST(Fee_QryFctPtrType, FEE_PRIVATE_CONST) Fee_QueryTable_ap[] =
{
    Fee_QryBlockPayloadUpdated,
    Fee_QryBlockCopyBytesRemaining,
    Fee_QryChunkIsDataChunk,
    Fee_QryChunkNotFull,
    Fee_QryInstHeaderErasedValue,
    Fee_QryFlsJobResultOk,
    Fee_QryLocalJobResultEmptyChunks,
    Fee_QryLocalJobResultFailed,
    Fee_QryLocalJobResultInvalid,
    Fee_QryLocalJobResultOk,
    Fee_QryMayFollowLinks,
    Fee_QryPreviousInstanceAvailable,
    Fee_QryRecentChunkLinkConsistent,
    Fee_QrySearchingInCurrentSector,
    Fee_QryValidInstanceWritten,
    Fee_QryWriteSuspended,
    Fee_QryTrue
};

/******************************************************************************
 *  STATE DESCRIPTION TABLE
 *****************************************************************************/
/* These macros shall increase the readability of the State Descriptor Table. */
#define FEE_STATE_IF
#define FEE_STATE_THEN
#define FEE_STATE_ELSE

/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define FEE_NEXT_STATE(state)                                         (state)

/* The state description table: It describes each state by conditions and transitions
 * (the latter consisting of actions to be performed and the subsequent state).
 * Each enumerator in Fee_StateEnumType must have its corresponding entry, in order to serve as index into table.
 * Note that array's size must be inferred from the number of initializers.
 * Otherwise the, table size compile time check (refer to Fee_tagFsmTableSizesCheck) would not work. */
/* PRQA S 3218 1 */ /* MD_FEE_8.7_fsm_tables */
static CONST(struct Fee_StateDescrStruct, FEE_PRIVATE_CONST) Fee_StateDescrTable_at[] =
{
    /* FEE_STATE_IDLE */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_WAIT, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_WAIT, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_MAIN_SECTOR_SWITCH */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_WAIT, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },

    /* FEE_STATE_MAIN_PREPARE_JOB */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_ActSetupJobProcessing, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_MAIN_BUSY)
            }
        },
        {
            FEE_STATE_THEN {FEE_ACT_ID_WAIT, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }

    },

    /* FEE_STATE_MAIN_BUSY */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_JOB, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_JOB, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
#if(FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
    /* FEE_STATE_FORCE_SS_PART_INIT */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_WAIT},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_THEN {FEE_ACT_ID_WAIT, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
#endif

    /* FEE_STATE_ALLOCATE_CHUNK_CHUNK_HOPPING_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_CONTINUE_SEARCH_FOR_NFA, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_CHUNK_HOPPING_FSM)
            }
        },
        {
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
            FEE_STATE_ELSE {FEE_ACT_ID_PREPARE_WCC_READ, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_GET_WCC)
#else
            FEE_STATE_ELSE {FEE_ACT_ID_PREPARE_CHUNK_HEADER_WRITE, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_WRITE_CHUNK_HEADER)
#endif
        }
    },
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    /* FEE_STATE_ALLOCATE_CHUNK_GET_WCC */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_EVALUATE_WRITE_CYCLE, FEE_ACT_ID_PREPARE_CHUNK_HEADER_WRITE},
                FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_WRITE_CHUNK_HEADER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_EVALUATE_WRITE_CYCLE, FEE_ACT_ID_PREPARE_CHUNK_HEADER_WRITE},
            FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_WRITE_CHUNK_HEADER)
        }
    },
#endif

    /* FEE_STATE_ALLOCATE_CHUNK_WRITE_CHUNK_HEADER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_ELSE {FEE_ACT_ID_PREPARE_CHUNK_LINK_WRITE, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_WRITE_LINK)
            },
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_PREPARE_CHUNK_LINK_WRITE, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_ALLOCATE_CHUNK_WRITE_LINK)
        }
    },
    /* FEE_STATE_ALLOCATE_CHUNK_WRITE_LINK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_CHUNK_LINK_WRITE, FEE_ACT_ID_FINALIZE_CHUNK_ALLOCATION},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_CHUNK_LINK_WRITE, FEE_ACT_ID_FINALIZE_CHUNK_ALLOCATION},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_BUSY_PROCESS_PAYLOAD_DATA */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    /* FEE_STATE_BUSY_READ_CHUNK_HEADER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_WRITE_CYCLE_JOB, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_BUSY_SEARCH_WCC */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PRE_INIT_WRITE_PROCESSING, FEE_ACT_ID_PREPARE_WCC_READ},
                FEE_NEXT_STATE(FEE_STATE_BUSY_READ_CHUNK_HEADER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
#endif
    /* FEE_STATE_BUSY_SEARCH_CONSISTENT */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_PAYLOAD_DATA_PROCESSING, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_BUSY_PROCESS_PAYLOAD_DATA)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_BUSY_SEARCH_WRITTEN */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PRE_INIT_WRITE_PROCESSING, FEE_ACT_ID_INIT_WRITE_PROCESSING_FSM},
                FEE_NEXT_STATE(FEE_STATE_BUSY_WRITE_PROCESSING_1)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_BUSY_WRITE_PROCESSING_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_ALLOC_IN_CURRENT_SECTOR, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_BUSY_WRITE_PROCESSING_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_BUSY_WRITE_PROCESSING_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CHUNK_CONSISTENCY_CHECK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_CHECK_CHUNK_CONSISTENCY, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },

    /* FEE_STATE_CHUNK_LINK_PROCESSING */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_VALIDATE_CHUNK_LINK, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_EVALUATE_CHUNK_HOPPING, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH)
            }
        },
        /* final case */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_EVALUATE_CHUNK_HOPPING, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH)
        }
    },
    /* FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_ResetChunkSearch, FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM},
            FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_1)
        }
    },
    /* FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_RECENT_CHUNK_LINK_CONSISTENT, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_CHUNK_SEARCH, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_1)
        },
    },
    /* FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_GET_CURRENT_CHUNK_INFO, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_CHUNK_SEARCH, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_2 */
    {
        /* Chunk found, continue following chunk-links, if allowd*/
        {
            FEE_STATE_IF {FEE_QRY_ID_MAY_FOLLOW_LINKS, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_CHUNK_LINK_READ_OF_CURRENT_CHUNK, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_FOLLOW_CHUNK_LINK)
            }
        },
        /* Chunk Hopping is FORCED */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_1)
        }
    },
    /* FEE_STATE_CHUNK_SEARCH_FOLLOW_LINK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_INVALID, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_CHUNK_LINK_READ_OF_CURRENT_CHUNK, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_FOLLOW_CHUNK_LINK)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_HEADER_CHECK_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_1)
        }
    },

    /* FEE_STATE_COPY_ALLOCATE_DATA_CHUNK */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_REPORT_SECTOR_OVERFLOW, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        /* Exit 2 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INITIALIZE_BLOCK_COPY, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_COPY_INSTANCE_COPY)
        },
    },

    /* FEE_STATE_COPY_ALLOCATE_LINK_CHUNK */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_REPORT_SECTOR_OVERFLOW, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        /* Exit 2 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_InitDataChunkAllocCopyFsm, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_COPY_ALLOCATE_DATA_CHUNK)
        },
    },

    /* FEE_STATE_COPY_INSTANCE_COPY */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_INSTANCE_COPY, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_1_REMAINING_WRITE_STREAM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_OK, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_2_REMAINING_WRITE_STREAMS */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_WRITE_STREAM_LAST, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CREATE_INSTANCE_1_REMAINING_WRITE_STREAM)
            }
        },
        /* Exit 3 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_3_REMAINING_WRITE_STREAMS */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_WRITE_STREAM_SECOND, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CREATE_INSTANCE_2_REMAINING_WRITE_STREAMS)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_READ */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_BLOCK_COPY_WRITE, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_1)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_BlockCopyBytesRemaining, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_BLOCK_COPY_READ, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_READ)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_OK, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_GET_BLOCK_EVAL_HEADER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_EVALUATE_CHUNK_HOPPING, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_GET_BLOCK_EVAL_HEADER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_EVALUATE_CHUNK_HOPPING, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_GET_BLOCK_EVAL_HEADER)
        }
    },
    /* FEE_STATE_INSTANCE_EVALUATION_READ_TRAILER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_EVALUATE_INSTANCE_TRAILER, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        { /* not used */
            FEE_STATE_ELSE {FEE_ACT_ID_EVALUATE_INSTANCE_TRAILER, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_INSTANCE_SEARCH_BINARY */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_CALC_INSTANCE_INDEX_BINARY, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_INSTANCE_SEARCH_BINARY)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_INSTANCE_SEARCH_LINEAR */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_DECR_TO_PREV_INSTANCE_INDEX, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_INSTANCE_SEARCH_LINEAR)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_BLOCK_PAYLOAD_UPDATED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_2)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_CHUNK_IS_DATA_CHUNK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_INSTANCE_SEARCH_BINARY_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_BINARY)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_3)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_3 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_SEARCHING_IN_CURRENT_SECTOR, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_SEARCH_WRITTEN_IN_OLD_SECTOR, FEE_ACT_ID_INIT_CHUNK_SEARCH_FSM},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_1)
            }
        },
        /* final case */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },

    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_CHUNK_IS_DATA_CHUNK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_3)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_BLOCK_PAYLOAD_UPDATED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            },
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_INSTANCE_SEARCH_DOWNWARDS_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_LINEARY)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_3 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_SEARCHING_IN_CURRENT_SECTOR, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SWITCH_OPERATING_SECTOR, FEE_ACT_ID_INIT_CHUNK_SEARCH_FSM},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_1)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_INVALID, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }

    },
    /* FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_VALID_INSTANCE_WRITTEN, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_2)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_PREVIOUS_INSTANCE_AVAILABLE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_INSTANCE_SEARCH_DOWNWARDS_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_LINEARY)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_REVERSE_CHUNK_SEARCH, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_1)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_BINARY */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_INSTANCE_EVALUATION_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_1)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_LINEARY */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_INSTANCE_EVALUATION_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_1)
            }
        },
        {
            FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_2)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_CHUNK_HOPPING)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_CHUNK_IS_DATA_CHUNK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_CHUNK_SEARCH, FEE_ACT_ID_INIT_INSTANCE_SEARCH_UPWARDS},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_READ_INSTANCE_HEADER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_CHUNK_SEARCH, FEE_ACT_ID_PREPARE_CHUNK_LINK_READ_OF_CURRENT_CHUNK},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_CHUNK_HOPPING */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_GET_CURRENT_CHUNK_INFO, FEE_ACT_ID_INIT_INSTANCE_SEARCH_UPWARDS},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_READ_INSTANCE_HEADER)
            }
        },
        /* Exit 2 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_CHUNK_SEARCH, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_VALID_INSTANCE_WRITTEN, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_2)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_CHUNK_NOT_FULL},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INCR_TO_NEXT_INSTANCE_INDEX, FEE_ACT_ID_PREPARE_INSTANCE_HEADER_READ},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_READ_INSTANCE_HEADER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INCR_TO_NEXT_INSTANCE_INDEX, FEE_ACT_ID_CONTINUE_UPWARDS_CHUNK_SEARCH},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1)
        }
    },

    /* FEE_STATE_SEARCH_UPWARDS_READ_INSTANCE_HEADER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_ERASED_VALUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_CONTINUE_UPWARDS_CHUNK_SEARCH, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1)
            }
        },
        /* Exit 3 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_INSTANCE_EVALUATION_UPWARDS_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_1)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_INVALID, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_CHUNK_HEADER_CHECK_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_1)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_2)
        }
    },
    /* FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_RECENT_CHUNK_LINK_CONSISTENT},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_HOPPING_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_UPWARDS_CHUNK_HOPPING)
        }
    },
    /* FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_CURRENT */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_CHUNK_IS_DATA_CHUNK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_SEARCH_FOR_NFA_WRITE, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_WRITTEN_FIND_NFA)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_SEARCH_WRITTEN_IN_OLD_SECTOR, FEE_ACT_ID_INIT_CHUNK_SEARCH_FSM},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_OLD)
        }
    },
    /* FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_OLD */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_SEARCH_FOR_NFA_WRITE, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_WRITTEN_FIND_NFA)
            }
        },
        /* final case, not used */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SEARCH_WRITTEN_FIND_NFA */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_SEARCH_FOR_NFA_WRITE, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SEARCH_WRITTEN_INSTANCE_SEARCH_BINARY)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_CONTINUE_SEARCH_FOR_NFA, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SEARCH_WRITTEN_FIND_NFA)
        }
    },

    /* FEE_STATE_SEARCH_WRITTEN_INSTANCE_SEARCH_BINARY */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_OK, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        /* final case - not used */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_ALLOCATE_LINKTABLE_CHUNK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_REPORT_SECTOR_OVERFLOW, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SETUP_SECTOR_SWITCH_SOURCE_SEARCH, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SECTOR_SWITCH_SEARCH_CONSISTENT)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_COPY_PROCESSING */
    {
        /* Exit 1 - Result Empty Chunks -> Copy was forbidden; don't advance to next Block, as we'll need to retry!!! */
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_EMPTY_CHUNKS, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_BLOCK_COPY, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_GET_BLOCK_1 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_GET_NEXT_BLOCK_FSM, FEE_ACT_ID_INIT_SEARCH_UPWARDS_FSM},
                FEE_NEXT_STATE(FEE_STATE_SECTOR_SWITCH_SEARCH_TARGET_CHUNK)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SECTOR_SWITCH_GET_BLOCK_2)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_GET_BLOCK_2 */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_EMPTY_CHUNKS, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_SS_COPYING, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_SEARCH_CONSISTENT */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_COPY_PROCESS_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SECTOR_SWITCH_COPY_PROCESSING)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_SEARCH_TARGET_CHUNK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_VALID_INSTANCE_WRITTEN, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_BLOCK_COPY, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SETUP_SECTOR_SWITCH_SOURCE_SEARCH, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_SECTOR_SWITCH_SEARCH_CONSISTENT)
        }
    },
    /* FEE_STATE_SECTOR_SWITCH_SETUP_SECTORS */
    {
        /* Exit 1 */
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_NOP, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        /* final case - not used */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SETUP_SECTORS_ERASING */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_SETUP_SECTORS_WRITE_HEADER, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_SETUP_SECTORS_HEADER_WRITING)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_SETUP_SECTORS, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_SETUP_SECTORS_HEADER_WRITING */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_SETUP_SECTORS, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_SETUP_SECTORS, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_STARTUP_READ_SECTOR_HEADER_LOWER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_SECTOR_HEADER_UPPER_READ, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_STARTUP_READ_SECTOR_HEADER_UPPER)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_FAILED, FEE_ACT_ID_PREPARE_SECTOR_HEADER_UPPER_READ},
            FEE_NEXT_STATE(FEE_STATE_STARTUP_READ_SECTOR_HEADER_UPPER)
        }
    },
    /* FEE_STATE_STARTUP_READ_SECTOR_HEADER_UPPER */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_EVALUATE_SECTOR_HEADERS, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
    /* FEE_STATE_WRITE_PROCESSING_ALLOC_DATACHUNK, */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CREATE_INSTANCE_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_WRITE_PROCESSING_CREATE_INSTANCE)
        }
    },
    /* FEE_STATE_WRITE_PROCESSING_ALLOC_LINKTABLE, */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_DATACHUNK_ALLOC_WRITE_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_WRITE_PROCESSING_ALLOC_DATACHUNK)
        }
    },
    /* FEE_STATE_WRITE_PROCESSING_CREATE_INSTANCE */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_INSTANCE_WRITE, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },
#if (FEE_DATA_CONVERSION_API == STD_ON)
    /* FEE_STATE_DC_GET_NEXT_BLOCK */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_GET_NEXT_BLOCK_FSM, FEE_ACT_ID_INIT_SEARCH_UPWARDS_FSM},
                FEE_NEXT_STATE(FEE_STATE_DC_SEARCH_UPWARDS_FSM)
            }
        },
        /* Exit 2 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SET_LOCAL_JOB_RESULT_OK, FEE_ACT_ID_INIT_SETUP_SECTORS_FSM},
            FEE_NEXT_STATE(FEE_STATE_DC_SETUP_SECTORS)
        }
    },

    /* FEE_STATE_DC_SEARCH_UPWARDS_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_FINALIZE_BLOCK_COPY_DC, FEE_ACT_ID_GET_NEXT_BLOCK_TO_COPY},
                FEE_NEXT_STATE(FEE_STATE_DC_GET_NEXT_BLOCK)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_SETUP_SECTOR_SWITCH_SOURCE_SEARCH, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_DC_SEARCH_CONSISTENT_FSM)
        }
    },

    /* FEE_STATE_DC_SEARCH_CONSISTENT_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_PREPARE_READ_INSTANCE_DATA, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_DC_GET_INSTANCE_DATA)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_BLOCK_COPY_DC, FEE_ACT_ID_GET_NEXT_BLOCK_TO_COPY},
            FEE_NEXT_STATE(FEE_STATE_DC_GET_NEXT_BLOCK)
        }
    },

    /* FEE_STATE_DC_GET_INSTANCE_DATA */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_FLS_JOB_RESULT_OK, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_DC_WRITE_PROC_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_DC_WRITE_PROCESSING_FSM)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_BLOCK_COPY_DC, FEE_ACT_ID_GET_NEXT_BLOCK_TO_COPY},
            FEE_NEXT_STATE(FEE_STATE_DC_GET_NEXT_BLOCK)
        }
    },
    /* FEE_STATE_DC_WRITE_PROCESSING_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_REPORT_SECTOR_OVERFLOW, FEE_ACT_ID_FINALIZE_DATA_CONVERSION},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_BLOCK_COPY_DC, FEE_ACT_ID_GET_NEXT_BLOCK_TO_COPY},
            FEE_NEXT_STATE(FEE_STATE_DC_GET_NEXT_BLOCK)
        }
    },

    /* FEE_STATE_DC_LINKCHUNK_ALLOC_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        /* Exit 2 */
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_CHUNK_ALLOC_FSM_DC, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_DC_DATA_CHUNK_ALLOC_FSM)
        }
    },

    /* FEE_STATE_DC_DATA_CHUNK_ALLOC_FSM */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_FAILED, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_INIT_INSTANCE_WRITE_DC_FSM, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_DC_WRITE_PROC_WR_DATA)
        }
    },

    /* FEE_STATE_DC_WRITE_PROC_WR_DATA */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_TRUE, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_RETURN_FROM_SUB_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_IDLE)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_NOP, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    },

    /* FEE_STATE_DC_SETUP_SECTORS */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_LOCAL_JOB_RESULT_EMPTY_CHUNKS, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {FEE_ACT_ID_INIT_SETUP_SECTORS_FSM, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_DC_SETUP_SECTORS)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_FINALIZE_DATA_CONVERSION, FEE_ACT_ID_NOP},
            FEE_NEXT_STATE(FEE_STATE_DC_GET_NEXT_BLOCK)
        }
    },
#endif

    /*   FEE_STATE_WAIT_FOR_WRITE_RESUME */
    {
        {
            FEE_STATE_IF {FEE_QRY_ID_WriteSuspended, FEE_QRY_ID_TRUE},
            {
                FEE_STATE_THEN {Fee_ACT_ID_HandleWriteSuspended, FEE_ACT_ID_NOP},
                FEE_NEXT_STATE(FEE_STATE_WAIT_FOR_WRITE_RESUME)
            }
        },
        {
            FEE_STATE_ELSE {FEE_ACT_ID_IssueDeferredRequest, FEE_ACT_ID_RETURN_FROM_SUB_FSM},
            FEE_NEXT_STATE(FEE_STATE_IDLE)
        }
    }
};

/* Just eases development; some kind of compile-time assertion: doe ActionTable, QueryTable and StateTable
 * have the exact number of entries, prescribed by their corresponding enums?
 * Note that according to ISO/ANSI an array may have less initializers than the explicitly given number of elements.
 * (Missing elements implicitly get value 0 / NULL).
 * The the compiler by itself only checks that no additional initializers were given. We need an exact match!
 * Therefore the arrays are defined without explicitly giving sizes (empty []).
 * We instruct the compiler to define a structure (bit-field). The checks's result denote the members' sizes (number of bits):
 * If the check fails the size gets -1, which is not allowed by standard - compilers issue an error.
 * (Note: some compilers accetpt a bit-field size of 0, though it is not allowed, either. Using -1 is safer...)
 * Otherwise, the bitfield's size gets 1, which is indeed valid. */
struct Fee_tagFsmTableSizesCheck {
    Fee_BitFieldType ActionTable : (sizeof(Fee_ActionTable_ap) == ((FEE_ACT_ID_WAIT + 1) * sizeof(Fee_ActFctPtrType)) ? 1 : -1);
    Fee_BitFieldType QueryTable  : (sizeof(Fee_QueryTable_ap) == ((FEE_QRY_ID_TRUE + 1) * sizeof(Fee_QryFctPtrType)) ? 1 : -1);
    Fee_BitFieldType StateTable  : (sizeof(Fee_StateDescrTable_at) == (FEE_STATE_NEVER_ENTERED * sizeof(struct Fee_StateDescrStruct)) ? 1 : -1);
};

#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_FsmIsSectorSwitchFsmRunning
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_FsmIsSectorSwitchFsmRunning(Fee_GlobalsConstPtrType g)
{
    return g->FsmStack_t.Stack_at[0] == FEE_STATE_MAIN_SECTOR_SWITCH;
}

/**********************************************************************************************************************
 * Fee_FsmIsIdle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_FsmIsIdle(Fee_FsmStackConstRefType fsmStack)
{
    return fsmStack->Stack_at[0] == FEE_STATE_IDLE;
}

/**********************************************************************************************************************
 * Fee_DoProcessing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_DoProcessing(Fee_GlobalsPtrType g)
{
    g->InternalFlags_t.leaveMain = 0; /* SBSW_FEE_PointerWrite_refParam */

    if(g->ApiFlags.cancelReq) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        Fee_InternalHandleCancelRequest(g); /* SBSW_FEE_PointerWrite_refParam */
    }

    if(Fee_FsmIsIdle(&g->FsmStack_t)) /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_InternalSetupToplevelFsm(g); /* SBSW_FEE_PointerWrite_refParam */
    }

    if(Fee_InternalIsFlsBusy(g)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_PointerWrite_refParam */
    {
        Fee_Internal_SignalWait();
#if(FEE_POLLING_MODE == STD_ON)
        Fee_SignalNeedProcessing();
#endif
    }

    while(!g->InternalFlags_t.leaveMain)
    {
        /* note: If g is valid (precondidion!), &g->FsmStack is also valid! */
        Fee_FsmProcess(&g->FsmStack_t); /* SBSW_FEE_Func_passParamByReference */
    }
} /* End of Fee_DoProcessing() */

/**********************************************************************************************************************
 * Fee_InternalHandleCancelRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalHandleCancelRequest(Fee_GlobalsPtrType g)
{
    Fee_FsmReset(g); /* SBSW_FEE_PointerWrite_refParam */

    g->InternalFlags_t.jobDeferred = FALSE; /* SBSW_FEE_PointerWrite_refParam */

    if(Fee_InternalIsFlsBusy(g) == FEE_TRUE) /* SBSW_FEE_PointerWrite_refParam */ /* COV_FEE_FLS_CANCEL */
    {
        Fee_FlsCancel();
    }

    /*Prevents from starting Sector Switch processing until new job arrives.They will be set again on demand*/
    g->SectorSwitchData_t.partitionId_s8 = -1; /* SBSW_FEE_PointerWrite_refParam */

    Fee_GlobalSuspend();
        g->ApiFlags.cancelReq = FALSE; /* SBSW_FEE_PointerWrite_refParam */
    Fee_GlobalRestore();
}

#if (FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
/**********************************************************************************************************************
 * Fee_InternalForceSectorSwitchSetup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalForceSectorSwitchSetup(Fee_ConfigRefTypeInternal cfg)
{
    sint8_least partIdx = 0;

    const sint8_least count = (sint8_least)cfg->Fee_NumberOfPartitions_u8;
    for(; partIdx < count; partIdx++) {
        Fee_Partition_setBlockCopyRequired(Fee_Partition_getFromConfigById(cfg, partIdx)); /* SBSW_FEE_PartitionConfig *//* SBSW_FEE_PointerWrite_refParam */
    }
}
#endif

/**********************************************************************************************************************
 * Fee_InternalSetupToplevelFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalSetupToplevelFsm(Fee_GlobalsPtrType g)
{
#if(FEE_FORCE_SECTOR_SWITCH_API == STD_ON)

    if(g->ApiFlags.forceSectorSwitch) /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    {
        g->SectorSwitchData_t.partitionId_s8 = (sint8)(g->Config_pt->Fee_NumberOfPartitions_u8 - 1); /* SBSW_FEE_PointerWrite_refParam */

        /* Config_pt was set during Fee_Init (always valid); if Fee_InitEx was used (passing a configuration pointer),
         * caller had to ensure validity */
        Fee_InternalForceSectorSwitchSetup(g->Config_pt); /* SBSW_FEE_GlobalConfigPointer */

        Fee_InternalResetSectorSwitch(&g->SectorSwitchData_t, Fee_InternalIsInFblMode(g)); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_PointerWrite_refParam */

        Fee_GlobalSuspend();
        g->InternalFlags_t.forceSectorSwitch = TRUE; /* SBSW_FEE_PointerWrite_refParam */
        g->ApiFlags.forceSectorSwitch = FALSE;          /* SBSW_FEE_PointerWrite_refParam */
        Fee_GlobalRestore();
    }
#endif
    if(!Fee_InternalScheduleProcess(g)) /* SBSW_FEE_PointerWrite_refParam */
    {
        Fee_Internal_SignalWait();

        Fee_Internal_setToIdle(g);     /* SBSW_FEE_PointerWrite_refParam */
    }
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_Internal_setToIdle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_Internal_setToIdle(Fee_GlobalsPtrType g) {

    if(g->FeeModuleStatus_t != MEMIF_IDLE)
    {
        Fee_GlobalSuspend();

        if((g->FeeModuleStatus_t != MEMIF_BUSY)
#if(FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
                && (!g->ApiFlags.forceSectorSwitch)
#endif
        ) /* COV_FEE_PREEMPTION */
        {
            g->FeeModuleStatus_t = MEMIF_IDLE; /* SBSW_FEE_PointerWrite_refParam */
        }

        Fee_GlobalRestore();

        /* Ensure no preempting call was lost (otherwise we might end up with a pending job, while processing flag is cleared) */
        Fee_SignalNeedProcessing();
    }
}

/**********************************************************************************************************************
 * Fee_FsmProcess
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_FsmProcess(Fee_FsmStackRefType fsmStack)
{
    const uint8 tos = fsmStack->TopOfStack_u8;
    const Fee_StateDescriptorConstRefType CurrentExit_ptloc = &Fee_StateDescrTable_at[fsmStack->Stack_at[tos]];

    Fee_StateChangeElseDescrType transition;
    /* Unfortunately, ISO C90 does not allow directly initializing structures with NON-CONSTANT initializers */
    transition =
            (Fee_FsmQuery(CurrentExit_ptloc->ifChange_t.Queries_at) == FEE_TRUE) /* SBSW_FEE_FunctionPointerCall_FsmTable */
                    ? CurrentExit_ptloc->ifChange_t.Next_t : CurrentExit_ptloc->elseChange_t;

    fsmStack->Stack_at[tos] = transition.NextState_t; /* SBSW_FEE_PointerWrite_FsmStack */

    Fee_ActionTable_ap[transition.Actions_t.ActionHandler1_t](); /* SBSW_FEE_FunctionPointerCall_FsmTable */
    Fee_ActionTable_ap[transition.Actions_t.ActionHandler2_t](); /* SBSW_FEE_FunctionPointerCall_FsmTable */
}

/**********************************************************************************************************************
 * Fee_FsmQuery
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_FsmQuery(Fee_StateQueryPtrType Queries_at)
{
    /*Currently there are no queries with side-effects, forcing execution in the defined order. */
    /* PRQA S 3344,3415 2 *//* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
    return (((Fee_QueryTable_ap[Queries_at[0u]]()) && (Fee_QueryTable_ap[Queries_at[1u]]()))); /* SBSW_FEE_FunctionPointerCall_FsmTable */ /* SBSW_FEE_FunctionPointerCall_FsmTable */
}

/**********************************************************************************************************************
 * Fee_FsmReset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_FsmReset(Fee_GlobalsPtrType g)
{
    Fee_FsmSetupTopLevel(&g->FsmStack_t, FEE_STATE_IDLE); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_FsmSetupTopLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_FsmSetupTopLevel(Fee_FsmStackRefType fsmStack, Fee_StateType state)
{
    fsmStack->TopOfStack_u8 = 0;    /* SBSW_FEE_PointerWrite_refParam */
    fsmStack->Stack_at[0] = state;  /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_FsmInitSubFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_FsmInitSubFsm(Fee_FsmStackRefType fsmStack, Fee_StateType InitState_t)
{
    fsmStack->TopOfStack_u8++; /* SBSW_FEE_PointerWrite_refParam */
    fsmStack->Stack_at[fsmStack->TopOfStack_u8] = InitState_t; /* SBSW_FEE_PointerWrite_FsmStack */
}

/**********************************************************************************************************************
 * Fee_InstSearch_BinarySplitRange
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InstSearch_BinarySplitRange(Fee_InstanceSearchContextRefType context_pt, Fee_InstanceStatusType status)
{
    Fee_Bool retVal = FEE_FALSE;

    context_pt->currentInstance.status_t = status; /* SBSW_FEE_PointerWrite_refParam */

    context_pt->binary.step_t /= 2u; /* SBSW_FEE_PointerWrite_refParam */

    if(context_pt->binary.step_t > 0)
    {
        retVal = FEE_TRUE;

        context_pt->binary.previousInstance = context_pt->currentInstance; /* SBSW_FEE_PointerWrite_refParam */

        if(context_pt->currentInstance.status_t == FEE_INSTANCE_STATUS_EMPTY)
        {
            context_pt->currentInstance.index_t -= context_pt->binary.step_t; /* SBSW_FEE_PointerWrite_refParam */
        }
        else
        {
            context_pt->currentInstance.index_t += context_pt->binary.step_t; /* SBSW_FEE_PointerWrite_refParam */
        }
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ActCalcNextInstanceIndexBinary
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActCalcNextInstanceIndexBinary(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(Fee_InstSearch_BinarySplitRange(context_pt, /* SBSW_FEE_Func_writeObjectByReference */
            Fee_InternalEvalInstHdrStatus(context_pt, Fee_Globals_t.FlsJobResult_t))) /* SBSW_FEE_Func_writeObjectByReference */
    {
        Fee_InstSearch_continue(context_pt); /* SBSW_FEE_Func_writeObjectByReference */
    }
    else
    {
        Fee_InstSearch_finalize(context_pt->instDescr_pt, /* SBSW_FEE_PointerWrite_refParam */
                ((context_pt->currentInstance.status_t == FEE_INSTANCE_STATUS_EMPTY)
                    && (context_pt->binary.previousInstance.status_t != FEE_INSTANCE_STATUS_EMPTY))
                                                        ? context_pt->binary.previousInstance : context_pt->currentInstance);
    }
}

/**********************************************************************************************************************
 * Fee_InstSearch_LinearDown
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InstSearch_LinearDown(Fee_InstanceSearchContextRefType context_pt, Fee_InstanceStatusType status)
{
    Fee_Bool retVal = FALSE;

    context_pt->currentInstance.status_t = status; /* SBSW_FEE_PointerWrite_refParam */

    if((context_pt->currentInstance.index_t > 0) && (status == FEE_INSTANCE_STATUS_EMPTY))
    {
        retVal = FEE_TRUE;
        context_pt->currentInstance.index_t--; /* SBSW_FEE_PointerWrite_refParam */
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ActDecrementToPrevInstanceIndex
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActDecrementToPrevInstanceIndex(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    if(!Fee_InstSearch_LinearDown(context_pt,   /* SBSW_FEE_Func_passParamByReference */
            Fee_InternalEvalInstHdrStatus(context_pt, Fee_Globals_t.FlsJobResult_t))) /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_InstSearch_finalize(context_pt->instDescr_pt, context_pt->currentInstance); /* SBSW_FEE_PointerWrite_refParam */
    }
    else
    {
       Fee_InstSearch_continue(context_pt); /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_InstSearch_continue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_FEE_16.7_PointerParam_IS_modified */
FEE_INLINE_FUNC(void) Fee_InstSearch_continue(Fee_InstanceSearchContextRefType context_pt)
{
     /* Fee_ChunkInfoReadInstanceHeader modifies context_pt>Buffer_au32, thus Fee_InstSearch_continue() modifies context_pt!
     * Some compilers would issue a warning, if context_pt was a pointer-to-const */
    Fee_InternalEvalFlsRetVal(
        Fee_ChunkInfo_ReadInstanceHeader(&context_pt->instDescr_pt->chunk_t, context_pt->currentInstance.index_t,
                                         context_pt->Buffer_au32)); /* SBSW_FEE_InternalDataBuffer_passedTo */
}

/**********************************************************************************************************************
 * Fee_InstSearch_finalize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_InstSearch_finalize(Fee_InstanceDescriptorRefType instDescr, struct Fee_tagInstanceDescrPair inst)
{
    instDescr->instance_t = inst; /* SBSW_FEE_PointerWrite_refParam */
    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
    Fee_ActReturnFromSubFsm();
}

/**********************************************************************************************************************
 * Fee_ActEvaluateChunkHopping
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateChunkHopping(void)
{
    struct Fee_BlockIdStruct blockId;

    const Fee_ChunkHoppingContextRefType context = Fee_Globals_t.chunkHoppingContext_pt;
    const Fee_ChunkInfoRefType chunk = &context->tempChunk_t;

    Fee_Bool continueSearch = FEE_FALSE;

    enum Fee_MngmtStatus headerStatus = Fee_ChunkInfo_EvaluateHeaderData(
        chunk,                      /* SBSW_FEE_Func_passParamByReference */
        &blockId,                   /* SBSW_FEE_Func_passParamByReference */
        context->headerBuffer_au32, /* SBSW_FEE_ConstPointer_refParam */
        (Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK));

    switch (headerStatus) 
    {
        case FEE_MNGMT_ERASED:

            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
            break;

        case FEE_MNGMT_CONSISTENT:

            /*Matching bits will be XORes to 0, irrelevant bits are forced to 0 => match if result is 0 */
            /* PRQA S 3344,3415 2 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
            if(((((context->searchBlockId_t.Tag) ^ (blockId.Tag)) & (context->blockIdMask_t.Tag)) != 0)
                    || ((Fee_ChunkInfo_isDataChunk(chunk)) /* SBSW_FEE_Func_passParamByReference */
                       && ((((context->searchBlockId_t.DataIndex) ^ (blockId.DataIndex)) & (context->blockIdMask_t.DataIndex)) != 0)))
            {

                continueSearch = FEE_TRUE;

                Fee_ChunkInfo_SwitchToSuccessor(chunk); /* SBSW_FEE_Func_passParamByReference */
            }
            else
            {

                Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
                if(context->resultBlockId_pt != NULL_PTR)
                {   /* deliver blockId, if space was supplied */
                    *(context->resultBlockId_pt) = blockId; /* SBSW_FEE_ChunkHoppingContext_ref */
                }
            }
            break;

        default:
            /*Search must be continued at the next possible address, i.e. inconsistent chunk headers end address
              Don't set this address here. It was already done by function EvaluateHeaderData*/
            continueSearch = FEE_TRUE;
            break;
    }

    if(continueSearch == FEE_FALSE)
    {

        Fee_ActReturnFromSubFsm();
    }
    else if(Fee_ChunkInfo_GetEndAddress(chunk) <= context->upperSearchLimit_t) /* SBSW_FEE_ChunkHoppingContext_ref */
    {
        Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadChunkHeader(chunk, context->headerBuffer_au32)); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
        
        Fee_ActReturnFromSubFsm();
    }
}

/**********************************************************************************************************************
 * Fee_ActEvaluateWriteCycle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_LOCAL_FUNC(void) Fee_ActEvaluateWriteCycle(void)
    {
        const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;
        const Fee_InstanceDescriptorConstRefType instDescr = context_pt->wccChunk_pt;
        const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(&instDescr->chunk_t); /* SBSW_FEE_ConstPointer_refParam */

        /* PRQA S 3415 3 */ /* MD_FEE_12.4_queries */
        const Fee_Bool wccErased = ((!Fee_ChunkInfo_isDataChunk(&instDescr->chunk_t)) /* SBSW_FEE_ConstPointer_refParam */
                || (Fee_Sector_getEraseValueU32(sector) == context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET])); /* SBSW_FEE_ConstPointer_refParam */

        if (MEMIF_JOB_OK != Fee_Globals_t.FlsJobResult_t)
        {
            context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET] = FEE_WRITE_CYCLE_FAILED; /* SBSW_FEE_InternalDataBuffer_writeAccess */
        }
        else if(wccErased) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
        {
            context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET] = FEE_WRITE_CYCLE_EMPTY; /* SBSW_FEE_InternalDataBuffer_writeAccess */
        }

        else
        {
            context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET] += /* SBSW_FEE_InternalDataBuffer_writeAccess */
                instDescr->instance_t.index_t + ((instDescr->instance_t.status_t != FEE_INSTANCE_STATUS_EMPTY) ? 1u : 0);
        }
    }
#endif /* (STD_ON == FEE_GET_WRITE_CYCLE_API) */

/**********************************************************************************************************************
 * Fee_ActEvaluateInstanceTrailer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateInstanceTrailer(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    /* PRQA S 310 4 */ /* MD_FEE_11.4_pointer_cast */
    const Fee_InstanceStatusType instStatus = 
                        Fee_InternalEvaluateInstanceStatus(
                            Fee_ChunkInfo_GetSector(&context_pt->instDescr_pt->chunk_t), /* SBSW_FEE_ConstPointer_refParam */
                            (Fee_IntBuffer_u8ConstPtrType)context_pt->Buffer_au32, /* SBSW_FEE_InternalDataBuffer_writeAccess */
                            Fee_Globals_t.FlsJobResult_t);

    Fee_LocalJobResultType result = FEE_LOCAL_JOB_RESULT_OK;

    /*Instance header is FEE_INSTANCE_STATUS_OK when we get up to here */
    if(instStatus != FEE_INSTANCE_STATUS_OK)
    {
        context_pt->instDescr_pt->instance_t.status_t = FEE_INSTANCE_STATUS_FAILED; /* SBSW_FEE_InstanceSearchContext */
        result = FEE_LOCAL_JOB_RESULT_FAILED;
    }

    Fee_Globals_t.FeeLocalJobResult_t = result;
}

/**********************************************************************************************************************
 * Fee_ActEvaluateSectorHeaders
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActEvaluateSectorHeaders(void)
{
    const Fee_PartitionRefType partition = Fee_Globals_t.CurrentProcessData.Partition_pt;

    const Fee_SectorRefType sector2 = Fee_Partition_getSectorByPos(partition, FEE_UPPER_SECTOR_NUMBER); /* SBSW_FEE_InitializedProcessData */

    const uint8 sectId =  Fee_Sector_evaluateHeaderData(sector2,
                                        Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.sectorHeaderData, /* SBSW_FEE_PartitionConfig */ /* SBSW_FEE_InternalDataBuffer_writeAccess */
                                        (Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK));

    Fee_Partition_initSector(partition, Fee_Partition_getSectorByPos(partition, FEE_LOWER_SECTOR_NUMBER), /* SBSW_FEE_InitializedProcessData */ /* SBSW_FEE_PartitionConfig */ /* SBSW_FEE_InitializedProcessData */
            Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.lowerSectorId);

    Fee_Partition_initSector(partition, sector2, sectId); /* SBSW_FEE_InitializedProcessData */ /* SBSW_FEE_PartitionConfig */

    Fee_InternalResetSectorSwitch(&Fee_Globals_t.SectorSwitchData_t,        /* SBSW_FEE_Func_passParamByReference */
                                  Fee_InternalIsInFblMode(&Fee_Globals_t)); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkLinkWrite(void)
{
    FEE_NFY_CHUNK_LINK_WRITE_END(&Fee_Globals_t, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkAllocation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkAllocation(void)
{
    Fee_LocalJobResultType result = FEE_LOCAL_JOB_RESULT_FAILED;

    if(Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK)
    {
        const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;

        context_pt->ChunkAllocResult_t = FEE_CHUNK_ALLOC_OK; /* SBSW_FEE_Func_writeObjectByReference */
        Fee_ChunkInfo_Clone(&context_pt->previousChunk_pt->chunk_t, &context_pt->newChunk_t); /* SBSW_FEE_ChunkAllocContext */ /* SBSW_FEE_Func_passParamByReference */

        Fee_InstanceDescr_Init(&context_pt->previousChunk_pt->instance_t, 0, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_ChunkAllocContext */ /* SBSW_FEE_InstanceSearchContext */

        result = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS;
    }

    Fee_Globals_t.FeeLocalJobResult_t = result;
    
    Fee_ActReturnFromSubFsm();
}

/**********************************************************************************************************************
 * Fee_InstanceDescr_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_InstanceDescr_Init(Fee_InstanceDescrPairRef self, uint16 index, Fee_InstanceStatusType status)
{
    self->index_t = index;    /* SBSW_FEE_PointerWrite_refParam */
    self->status_t = status;  /* SBSW_FEE_PointerWrite_refParam */
}

#if (STD_ON == FEE_GET_ERASE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_ActFinalizeEraseCycleJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_IntFinalizeEraseCycleJob(Fee_UserData32PtrType target, Fee_PartitionConstRefType part, uint8 sectorId)
    {
        const Fee_SectorConstRefType sector_ptloc = Fee_Partition_getSectorByPos(part, sectorId); /* SBSW_FEE_PointerWrite_refParam */

        *target = Fee_Sector_getEraseCycleCounter(sector_ptloc); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */

        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
    }
#endif

/**********************************************************************************************************************
 * Fee_ActFinalizeBlockCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeBlockCopy(void)
{ 
    if(Fee_InternalIsInFblMode(&Fee_Globals_t)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_ChunkInfo_SwitchToSuccessor(&Fee_Globals_t.SectorSwitchData_t.mode.fbl); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.SectorSwitchData_t.mode.appl.DataIndex_s16--;
    }
}

#if(FEE_DATA_CONVERSION_API == STD_ON)
/**********************************************************************************************************************
 * Fee_ActFinalizeBlockCopyDc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeBlockCopyDc(void)
{

    Fee_ChunkInfo_SwitchToSuccessor(&Fee_Globals_t.SectorSwitchData_t.mode.fbl); /* SBSW_FEE_Func_passParamByReference */
}
#endif

/**********************************************************************************************************************
 * Fee_ActFinalizeGetNextBlockFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeGetNextBlockFsm(void)
{
    const Fee_ChunkHoppingContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.super_t;

    if(Fee_InternalIsInFblMode(&Fee_Globals_t)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_ChunkInfo_Clone(&Fee_Globals_t.SectorSwitchData_t.mode.fbl, &context_pt->tempChunk_t); /* SBSW_FEE_Func_passParamByReference */
    }

    Fee_ChunkInfo_Clear(&Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActFinalizeSsCopying
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSsCopying(void)
{
    FEE_NFY_SS_END(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */

    /*All blocks have already been copied*/
    Fee_Sector_free(Fee_Partition_getOldestSector(Fee_Globals_t.CurrentProcessData.Partition_pt)); /* SBSW_FEE_Partitions_OldestNewestSectorRef */ /* SBSW_FEE_InitializedProcessData */
    Fee_Partition_clearBlockCopyRequired(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    Fee_Globals_t.InternalFlags_t.jobDeferred = FALSE;

    Fee_InternalResetSectorSwitch(&Fee_Globals_t.SectorSwitchData_t, Fee_InternalIsInFblMode(&Fee_Globals_t)); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Func_passParamByReference */

    /*Sector erase is still necessary*/
    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS;
}

/**********************************************************************************************************************
 * Fee_ActFinalizeSetupSectors
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSetupSectors(void)
{
    FEE_NFY_SECTOR_ERASE_END(&Fee_Globals_t, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_Globals_t.InternalFlags_t.jobDeferred = FALSE;

    if(Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK)
    {
        /*Last flash job was writting the sector header*/
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
    
        Fee_Partition_switchSectors(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
        Fee_Partition_updateSectorId(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
    }
    else
    { 
        Fee_Partition_handleError(Fee_Globals_t.CurrentProcessData.Partition_pt, FEE_SECTOR_FORMAT_FAILED); /* SBSW_FEE_InitializedProcessData */
    }
}

/**********************************************************************************************************************
 * Fee_ActFinalizeJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeJob(void)
{
    const enum Fee_Partition_FillLevelEnum fillLevel = Fee_Partition_getFillLevel(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    if(Fee_Partition_getFillLevel(Fee_Globals_t.CurrentProcessData.Partition_pt) > FEE_PART_FILL_LEVEL_NORMAL) /* SBSW_FEE_InitializedProcessData */
    {
        Fee_InternalRequestSectorSwitch(&Fee_Globals_t, Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_InitializedProcessData */
    }

    if((Fee_Globals_t.FeeLocalJobResult_t != FEE_LOCAL_JOB_RESULT_OK) &&
        (Fee_Globals_t.InternalFlags_t.jobDeferred == FEE_TRUE))
    {
        if (fillLevel == FEE_PART_FILL_LEVEL_NORMAL)
        {
            Fee_Partition_setBlockCopyRequired(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
        }
    }
    else
    {
        switch(Fee_Globals_t.FeeLocalJobResult_t)
        {
            case (Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_OK:
                Fee_Globals_t.FeeJobResult_t = MEMIF_JOB_OK;
                break;
            case (Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_INVALID:
                Fee_Globals_t.FeeJobResult_t = MEMIF_BLOCK_INVALID;
                break;
            case (Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_INCONSISTENT:
                Fee_Globals_t.FeeJobResult_t = MEMIF_BLOCK_INCONSISTENT;
                break;
            default:
                Fee_Globals_t.FeeJobResult_t = MEMIF_JOB_FAILED;
        }

    #if (STD_OFF == FEE_NVM_POLLING_MODE)
        if ((Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_OK == Fee_Globals_t.FeeLocalJobResult_t)
        {
            Fee_NvMJobEndNotification();
        }
        else
        {
            Fee_NvMJobErrorNotification();
        }
    #endif /* (STD_OFF == FEE_NVM_POLLING_MODE) */

        Fee_Globals_t.JobParams.requestedJob = FEE_JOB_NONE;

        Fee_Globals_t.FeeModuleStatus_t = MEMIF_BUSY_INTERNAL;
    }

    Fee_SignalReEnterMainFunction();
}

/**********************************************************************************************************************
 * Fee_ActFinalizeWriteCycleJob
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_LOCAL_FUNC(void) Fee_ActFinalizeWriteCycleJob(void)
    {
        const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;

        Fee_LocalJobResultType jobResult = FEE_LOCAL_JOB_RESULT_OK;

        Fee_ActEvaluateWriteCycle();

        switch(context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET])
        {

            case FEE_WRITE_CYCLE_FAILED:
                jobResult = FEE_LOCAL_JOB_RESULT_FAILED;
                break;

            case FEE_WRITE_CYCLE_EMPTY:
                jobResult = FEE_LOCAL_JOB_RESULT_INVALID;
                break;
            default:
                *Fee_Globals_t.JobParams.params.blockJob.add.writeCc.resultBuffer = /* SBSW_FEE_UserDataBuffer */
                                    context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET];
        }

        Fee_Globals_t.FeeLocalJobResult_t = jobResult;
    }     
#endif

/**********************************************************************************************************************
 * Fee_ActIncrementToNextInstanceIndex
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActIncrementToNextInstanceIndex(void)
{
    Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t.currentInstance.index_t++;
}

/**********************************************************************************************************************
 * Fee_ActInitAllocateChunkFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitAllocateChunkFsm(Fee_ChunkAllocContextRefType context_pt, Fee_ChunkInfoConstRefType newChunk)
{
    context_pt->ChunkAllocResult_t = FEE_CHUNK_ALLOC_SECTOR_FULL; /* SBSW_FEE_PointerWrite_refParam */

    Fee_ChunkInfo_Clone(&context_pt->newChunk_t, newChunk); /* SBSW_FEE_Func_passParamByReference */
    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_ALLOCATE_CHUNK_CHUNK_HOPPING_FSM); /* SBSW_FEE_Func_passParamByReference */

    Fee_InternalInitSearchForNfaFsm(&Fee_Globals_t, /* SBSW_FEE_Func_passParamByReference */
            Fee_ChunkInfo_GetSector(&context_pt->previousChunk_pt->chunk_t)); /* SBSW_FEE_ChunkAllocContext */
}

/**********************************************************************************************************************
 * Fee_ActInitChunkHoppingFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkHoppingFsm(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    /*Don't overwrite latest found chunk, because we may find chunks that don't belong to the current block*/
    Fee_ChunkInfo_Clone(&context_pt->super_t.tempChunk_t, &context_pt->mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_InternalInitChunkHoppingFsm(&Fee_Globals_t, &context_pt->super_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_InternalInitChunkHoppingFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitChunkHoppingFsm(Fee_GlobalsPtrType globals, Fee_ChunkHoppingContextRefType context)
{
    /* Chunk-Hopping FSM refers to  chunkHoppingContext_pt, because it may be run within different other FSMs, namely
     * Chunk Allocation and Chunk-Search. -> each context provides a chunk hopping structure, to be associated with chunk hopping
     * here */
    globals->chunkHoppingContext_pt = context; /* SBSW_FEE_PointerWrite_refParam */
    if(Fee_ChunkInfo_GetEndAddress(&context->tempChunk_t) < context->upperSearchLimit_t) /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH); /* SBSW_FEE_Func_passParamByReference */

        Fee_ChunkInfo_SwitchToSuccessor(&context->tempChunk_t);                         /* SBSW_FEE_Func_passParamByReference */
        Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadChunkHeader(&context->tempChunk_t,  /* SBSW_FEE_Func_passParamByReference */
                                                                context->headerBuffer_au32));
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
    }
}

/**********************************************************************************************************************
 * Fee_ActInitChunkSearchFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkSearchFsm(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    context_pt->chunkHoppingForced = FALSE; /* SBSW_FEE_Func_writeObjectByReference */

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(Fee_Sector_isUsable(Fee_ChunkInfo_GetSector(&context_pt->recentChunk_pt->chunk_t))) /* SBSW_FEE_Chunks_Sector_ref */ /* SBSW_FEE_ChunkSearchContext */
    {
        Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CHUNK_SEARCH_FOLLOW_CHUNK_LINK); /* SBSW_FEE_Func_passParamByReference */
        Fee_ActPrepareChunkLinkReadOfCurrentChunk();
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
    }
}

/**********************************************************************************************************************
 * Fee_ActInitChunkHeaderCheckFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitChunkHeaderCheckFsm(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    const enum Fee_FlsRequestStatus status = 
        Fee_ChunkInfo_ReadChunkHeader(&context_pt->mostRecentChunk_t, context_pt->super_t.headerBuffer_au32); /* SBSW_FEE_Func_passParamByReference */

    /*Above function returns FEE_REQ_SYNC_USELESS in case of linktable*/
    if(status != FEE_REQ_SYNC_USELESS)
    {
        Fee_InternalEvalFlsRetVal(status);
        Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CHUNK_CONSISTENCY_CHECK); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
    }
}

/**********************************************************************************************************************
 * Fee_ActInitCreateInstanceFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitCreateInstanceFsm(void)
{

    const Fee_GlobalsPtrType globals = &Fee_Globals_t;
    const Fee_PartitionConstRefType partition = globals->CurrentProcessData.Partition_pt;
    const Fee_InstanceDescriptorRefType target_pt = globals->FsmLocalVars_t.ChunkAllocContext_t.previousChunk_pt;
    const Fee_SectorRefType targetSector = Fee_ChunkInfo_GetSector(&target_pt->chunk_t); /* SBSW_FEE_ChunkAllocContext */

    /* PRQA S 3415,3344 3 */ /* MD_FEE_12.4_queries, MD_FEE_13.2_boolean */
    if((!globals->FsmLocalVars_t.ChunkAllocContext_t.config_pt->CriticalData_bl) ||
       (!Fee_Proc_isNewestSector(partition, targetSector)) || /* SBSW_FEE_InitializedProcessData */ /* SBSW_FEE_Chunks_Sector_ref */
       (Fee_Partition_getFillLevel(partition) == FEE_PART_FILL_LEVEL_SINGLE_SECTOR)) /* SBSW_FEE_InitializedProcessData */
    {  
        const Fee_InstanceWriteContextRefType context_pt = &globals->FsmLocalVars_t.instanceWriteContext_t;

        const Fee_LengthType targetAddr =
                                Fee_ChunkInfo_GetInstanceStartAddress(&target_pt->chunk_t, target_pt->instance_t.index_t); /* SBSW_FEE_ChunkAllocContext */

        context_pt->sector_pt = targetSector; /* SBSW_FEE_Func_writeObjectByReference */

        Fee_Sector_ClearBuffer(
                    context_pt->sector_pt, 
                    context_pt->BufferBytes,
                    FEE_INTERNAL_BUFFER_SIZE);

        /*Instance failure shall not lead to a retry/sector switch*/
        globals->InternalFlags_t.jobDeferred = FALSE; /* SBSW_FEE_Func_writeObjectByReference */

        FEE_NFY_INSTANCE_WRITE(globals);

        if(globals->JobParams.requestedJob == FEE_JOB_WRITE)
        {
            context_pt->targetOffset_t = targetAddr; /* SBSW_FEE_Func_writeObjectByReference */
            context_pt->userData_pt = Fee_Globals_t.JobParams.params.blockJob.add.write.writeBuffer; /* SBSW_FEE_Func_writeObjectByReference */

            context_pt->ByteCount_u32 = Fee_ChunkInfo_GetInstanceSize(&target_pt->chunk_t); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_ChunkAllocContext */

            /*Depending on the number of bytes FEE needs up to three write streams*/
            Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_CREATE_INSTANCE_3_REMAINING_WRITE_STREAMS); /* SBSW_FEE_Func_passParamByReference */

            Fee_Proc_SetupWriteSuspendHandling(globals, Fee_InternalDoWriteStreamFirst); /* SBSW_FEE_Func_passParamByReference */

        }

        else
        {
            Fee_InternalPrepareEmptyInstanceWrite(targetSector, targetAddr, FEE_INSTANCE_STATUS_INVALID); /* SBSW_FEE_Chunks_Sector_ref */
        }
    }

    else
    {
        globals->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_Func_writeObjectByReference */
    }
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_ActFinalizeInstanceWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeInstanceWrite(void)
{
    FEE_NFY_INSTANCE_WRITE_END(&Fee_Globals_t, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_passParamByReference */
}

/* Following two functions are only needed here... */
/**********************************************************************************************************************
 * Fee_InternalInitSsChunkSearchFbl
 *********************************************************************************************************************/
/*!
 * \brief      Prepares chunk search in FBL mode.
 * \details    Marks old sector as full and switches to fblChunk's successor
 * \param[in,out]  fblChunk
 * \param[in]  partitionRef
 * \pre        FEE is in FBL mode
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitSsChunkSearchFbl(Fee_ChunkInfoRefType fblChunk, Fee_PartitionConstRefType partitionRef);

/**********************************************************************************************************************
 * Fee_Internal_FblGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * \brief      Searches the next block to copy in FBL mode
 * \details    Next block will be searched via chunk hopping. If limit has reached, error will be reported via local job
 *             result.
 * \param[in,out]  g
 * \pre        FEE is in FBL mode
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Internal_FblGetNextBlockToCopy(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_Internal_ApplGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * \brief      Searches the next block to copy in application mode
 * \details    Prepares current process data which will be needed later for chunk search
 * \param[in,out]  g
 * \pre        FEE is not in FBL mode
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_Internal_ApplGetNextBlockToCopy(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_InternalGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalGetNextBlockToCopy(Fee_GlobalsPtrType globals)
{

    if(Fee_InternalIsInFblMode(globals)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_PointerWrite_refParam */
    {
        Fee_Internal_FblGetNextBlockToCopy(globals); /* SBSW_FEE_PointerWrite_refParam */
    }
    else
    {
        Fee_Internal_ApplGetNextBlockToCopy(globals); /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_InternalInitSsChunkSearchFbl
 *********************************************************************************************************************/
/*!

 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitSsChunkSearchFbl(Fee_ChunkInfoRefType fblChunk, Fee_PartitionConstRefType partitionRef)
{
        const Fee_SectorRefType olderSector =
                                        Fee_Partition_getOldestSector(partitionRef); /* SBSW_FEE_PointerWrite_refParam  */

        Fee_Sector_markFull(olderSector); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

        Fee_ChunkInfo_Construct(fblChunk, olderSector); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */

        Fee_ChunkInfo_SwitchToSuccessor(fblChunk); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Internal_FblGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Internal_FblGetNextBlockToCopy(Fee_GlobalsPtrType g)
{
    const Fee_ChunkInfoRefType fblChunk = &g->SectorSwitchData_t.mode.fbl;
    const Fee_ChunkHoppingContextRefType context_pt = &g->FsmLocalVars_t.chunkSearchContext_t.super_t;

    if(Fee_ChunkInfo_GetBaseAddress(fblChunk) == 0) /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_InternalInitSsChunkSearchFbl(fblChunk, g->CurrentProcessData.Partition_pt); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_InitializedProcessData */
        FEE_NFY_SS_START(globals);
    }

    g->chunkHoppingContext_pt = context_pt; /* SBSW_FEE_PointerWrite_refParam */
    g->CurrentProcessData.BlockConfig_pt = NULL_PTR; /* SBSW_FEE_PointerWrite_refParam */
    context_pt->upperSearchLimit_t = Fee_Proc_getSectorEndOffset(Fee_ChunkInfo_GetSector(fblChunk)); /* SBSW_FEE_PointerWrite_refParam *//* SBSW_FEE_Chunks_Sector_ref */ /* SBSW_FEE_Func_passParamByReference */
    context_pt->blockIdMask_t.DataIndex = 0; /* SBSW_FEE_PointerWrite_refParam */
    context_pt->blockIdMask_t.Tag = 0; /* SBSW_FEE_PointerWrite_refParam */
    context_pt->searchBlockId_t = context_pt->blockIdMask_t; /* SBSW_FEE_PointerWrite_refParam */
    context_pt->resultBlockId_pt = &g->CurrentProcessData.BlockId; /* SBSW_FEE_PointerWrite_refParam */

    Fee_ChunkInfo_Clone(&context_pt->tempChunk_t, fblChunk); /* SBSW_FEE_Func_passParamByReference */

    if(Fee_ChunkInfo_GetInstanceStartAddress(fblChunk, 0) < context_pt->upperSearchLimit_t) /* SBSW_FEE_Func_passParamByReference */
    {
       Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadChunkHeader(fblChunk, context_pt->headerBuffer_au32)); /* SBSW_FEE_Func_passParamByReference */
       Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_GET_BLOCK_EVAL_HEADER); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_PointerWrite_refParam */
    }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_Internal_ApplGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Internal_ApplGetNextBlockToCopy(Fee_GlobalsPtrType g)
{
    Fee_BlockConfigRefType blockConfig = g->Config_pt->BlockConfig_at;
    uint16 currBlockNumber = g->SectorSwitchData_t.mode.appl.BlockConfigIndex_u16;
    sint16 DataIndex_s16loc = g->SectorSwitchData_t.mode.appl.DataIndex_s16;
        
    if(DataIndex_s16loc < 0)
    {
        currBlockNumber++;
    }

    if(currBlockNumber == 0)
    {
        const Fee_SectorRefType olderSector =
                                        Fee_Partition_getOldestSector(g->CurrentProcessData.Partition_pt); /* SBSW_FEE_PartitionConfig */

        Fee_Sector_markFull(olderSector); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

        FEE_NFY_SS_START(g);
    }

    if(currBlockNumber < g->Config_pt->Fee_NumberOfBlocks_u16)
    {
        blockConfig = &blockConfig[currBlockNumber];

        if(blockConfig->PartitionIndex != Fee_Partition_getId(g->CurrentProcessData.Partition_pt)) /* SBSW_FEE_PartitionConfig */ /* COV_FEE_PART_CONFIG */
        {
            DataIndex_s16loc = -1;
            g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS; /* SBSW_FEE_Func_writeObjectByReference */
        }
        else
        {
            if(DataIndex_s16loc < 0)
            {
                DataIndex_s16loc = (sint16)(blockConfig->NumberOfDatasets_u8 - 1);
            }
                
            g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK; /* SBSW_FEE_Func_writeObjectByReference */
        }

        g->CurrentProcessData.BlockConfig_pt = blockConfig; /* SBSW_FEE_Func_writeObjectByReference */
        g->CurrentProcessData.BlockId.DataIndex = (uint8)((DataIndex_s16loc < 0) ? 0 : DataIndex_s16loc); /* SBSW_FEE_Func_writeObjectByReference *//* COV_FEE_PART_CONFIG */
        g->CurrentProcessData.BlockId.Tag = blockConfig->LinktableIndex_u16; /* SBSW_FEE_Func_writeObjectByReference */

    }
    else
    {
        currBlockNumber = 0xFFFFu;
        DataIndex_s16loc = -1;
        g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_Func_writeObjectByReference */
    }

    g->SectorSwitchData_t.mode.appl.BlockConfigIndex_u16 = currBlockNumber; /* SBSW_FEE_Func_writeObjectByReference */
    g->SectorSwitchData_t.mode.appl.DataIndex_s16 = DataIndex_s16loc;       /* SBSW_FEE_Func_writeObjectByReference */
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_ActInitInstanceEvaluationFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceEvaluationFsm(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;
    
    Fee_LocalJobResultType localResult = FEE_LOCAL_JOB_RESULT_FAILED;

    switch(context_pt->instDescr_pt->instance_t.status_t)
    {
        case FEE_INSTANCE_STATUS_OK:
            Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_INSTANCE_EVALUATION_READ_TRAILER); /* SBSW_FEE_Func_passParamByReference */

            Fee_ActPrepareInstanceTrailerRead();
 
            /*Don't set local job result here. Will be set by sub state machine*/
            break;
        case FEE_INSTANCE_STATUS_EMPTY:
            localResult = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS;
            break;
        case FEE_INSTANCE_STATUS_INCONSISTENT:
            localResult = FEE_LOCAL_JOB_RESULT_INCONSISTENT;
            break;
        case FEE_INSTANCE_STATUS_INVALID:
            localResult = FEE_LOCAL_JOB_RESULT_INVALID;
            break;
        default:
            break;
    }

    Fee_Globals_t.FeeLocalJobResult_t = localResult;
}

/**********************************************************************************************************************
 * Fee_ActInitInstanceEvaluationUpwardsFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceEvaluationUpwardsFsm(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    context_pt->currentInstance.status_t = Fee_InternalEvalInstHdrStatus(context_pt, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_Func_passParamByReference */
    context_pt->instDescr_pt->instance_t = context_pt->currentInstance; /* SBSW_FEE_Func_writeObjectByReference */

    Fee_ActInitInstanceEvaluationFsm();
}

/**********************************************************************************************************************
 * Fee_InstSearch_InitContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_InstSearch_InitContext(Fee_InstanceSearchContextRefType self, Fee_InstanceDescriptorRefType desc, uint16 startIndex)
{
    self->instDescr_pt = desc; /* SBSW_FEE_PointerWrite_refParam */
    Fee_InstanceDescr_Init(&desc->instance_t, 0, FEE_INSTANCE_STATUS_EMPTY);               /* SBSW_FEE_Func_passParamByReference */
    Fee_InstanceDescr_Init(&self->currentInstance, startIndex, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitInstanceSearchBinaryFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchBinaryFsm(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;
    const uint16 instanceCount = Fee_ChunkInfo_GetInstanceCount(&Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_INSTANCE_SEARCH_BINARY); /* SBSW_FEE_Func_passParamByReference */

    Fee_InstSearch_InitContext(context_pt, &Fee_Globals_t.CurrentDataset_t, instanceCount); /* SBSW_FEE_Func_passParamByReference */

    context_pt->binary.step_t = (uint16)(instanceCount + 1u); /* SBSW_FEE_Func_writeObjectByReference */

    (void)Fee_InstSearch_BinarySplitRange(context_pt, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */

    Fee_InstSearch_continue(context_pt); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitInstanceSearchDownwardsFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchDownwardsFsm(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;
    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_INSTANCE_SEARCH_LINEAR); /* SBSW_FEE_Func_passParamByReference */

    Fee_InstSearch_InitContext(context_pt, &Fee_Globals_t.CurrentDataset_t, Fee_Globals_t.CurrentDataset_t.instance_t.index_t); /* SBSW_FEE_Func_passParamByReference */

    (void)Fee_InstSearch_LinearDown(context_pt, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */
    Fee_InstSearch_continue(context_pt); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitLinkTableAllocationFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitLinkTableAllocationFsm(Fee_GlobalsPtrType g)
{
    const Fee_InstanceDescriptorRefType target = &g->SectorSwitchTarget_t;

    const uint8 datasets =  (uint8)((g->CurrentProcessData.BlockConfig_pt == NULL_PTR)
                                    ? (g->CurrentProcessData.BlockId.DataIndex + 1)
                                    : g->CurrentProcessData.BlockConfig_pt->NumberOfDatasets_u8);

    /* PRQA S 3344,3415 1 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
    if((datasets > 1u) && (Fee_Proc_isLinkTableChunkAllowed(&target->chunk_t))) /* SBSW_FEE_Func_passParamByReference */
    {
        const Fee_ChunkAllocContextRefType context_pt = &g->FsmLocalVars_t.ChunkAllocContext_t;

        Fee_Proc_CreateChunkAllocContext(context_pt, target, /* SBSW_FEE_Func_passParamByReference */
                                    g->CurrentProcessData.BlockId,
                                    g->CurrentProcessData.BlockConfig_pt); /* SBSW_FEE_InitializedProcessData */

#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
        context_pt->wccChunk_pt = &g->CurrentDataset_t; /* SBSW_FEE_Func_passParamByReference */
#endif
        {
            struct Fee_ChunkInfoStruct c;
            Fee_ChunkInfo_CreateNewLinktableChunk(&c, datasets); /* SBSW_FEE_Func_passParamByReference */
            Fee_InternalInitAllocateChunkFsm(context_pt, &c);    /* SBSW_FEE_Func_passParamByReference */
        }
    }
    else
    {
        g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK; /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_Proc_InitReverseChunkSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Proc_InitReverseChunkSearch(Fee_GlobalsPtrType g, Fee_SectorRefType sector, Fee_LengthType limit)
{
    Fee_ChunkInfo_Construct(&g->CurrentDataset_t.chunk_t, sector); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkSearchContext_Init(&g->FsmLocalVars_t.chunkSearchContext_t, /* SBSW_FEE_Func_passParamByReference */
        &g->CurrentProcessData.BlockId, &g->PreviousChunk_t, &g->CurrentDataset_t, limit); /* SBSW_FEE_Func_passParamByReference */
    Fee_ActInitChunkSearchFsm();
}

/**********************************************************************************************************************
 * Fee_Proc_InitReversChunkSearchInOlderSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_Proc_InitReversChunkSearchInOlderSector(Fee_GlobalsPtrType g)
{
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(Fee_Proc_isNewestSector(g->CurrentProcessData.Partition_pt, Fee_ChunkInfo_GetSector(&g->PreviousChunk_t))) /* SBSW_FEE_InitializedProcessData */ /* SBSW_FEE_Func_passParamByReference */
        {
            const Fee_SectorRefType oldSector = Fee_Partition_getOldestSector(g->CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

            Fee_Proc_InitReverseChunkSearch(g, oldSector, Fee_Proc_getSectorEndOffset(oldSector)); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */
        }
        else
        {
            Fee_ChunkInfo_Clone(&g->CurrentDataset_t.chunk_t, &g->PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */
            Fee_ChunkInfo_Clone(&g->FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t, &g->PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */
            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
        }
}

/**********************************************************************************************************************
 * Fee_Proc_InitReversChunkSearchInSameSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_Proc_InitReverseChunkSearchInSameSector(Fee_GlobalsPtrType g)
{

    /*FEE always tracks the predecessor of the latest chunk as previous chunk
      When latest chunk is erroneous, FEE copies the previous chunk info into chunk_t and starts search there
      In case it is defective too, reverse chunk hopping must finally be started*/
    if(Fee_ChunkInfo_Equals(&g->PreviousChunk_t, &g->CurrentDataset_t.chunk_t)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_Proc_InitReverseChunkSearch(g, Fee_ChunkInfo_GetSector(&g->CurrentDataset_t.chunk_t), /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_Chunks_Sector_ref */
                                                            Fee_ChunkInfo_GetBaseAddress(&g->CurrentDataset_t.chunk_t)); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
        Fee_ChunkInfo_Clone(&g->CurrentDataset_t.chunk_t, &g->PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */
        Fee_ChunkInfo_Clone(&g->FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t, &g->PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */
        Fee_InstanceDescr_Init(&g->CurrentDataset_t.instance_t, Fee_ChunkInfo_GetInstanceCount(&g->CurrentDataset_t.chunk_t), FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_ActInitReverseChunkSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitReverseChunkSearch(void)
{
    if(Fee_ChunkInfo_isDataChunk(&Fee_Globals_t.PreviousChunk_t)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_Proc_InitReverseChunkSearchInSameSector(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Proc_InitReversChunkSearchInOlderSector(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_ActInitSearchWrittenInOldSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchWrittenInOldSector(void)
{
    const Fee_SectorRefType oldSector = Fee_Partition_getOldestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.super_t.upperSearchLimit_t = Fee_Proc_getSectorEndOffset(oldSector); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

    /*Might be needed in case most recent chunk is defective*/
    Fee_ChunkInfo_Clear(&Fee_Globals_t.PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */

    /*In case FEE must write into new sector (old is full), this chunk can be used to write the link to.
      The info, whether link is writeable at all, will be copied implicitly*/
    Fee_ChunkInfo_Clone(&Fee_Globals_t.SectorSwitchTarget_t.chunk_t, &Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_InstanceDescr_Init(&Fee_Globals_t.SectorSwitchTarget_t.instance_t, 0, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkInfo_Construct(&Fee_Globals_t.CurrentDataset_t.chunk_t, oldSector); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkInfo_Clone(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t, /* SBSW_FEE_Func_passParamByReference */
                            &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.recentChunk_pt->chunk_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitSearchUpwardsFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchUpwardsFsm(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    const Fee_InstanceDescriptorRefType target = &Fee_Globals_t.SectorSwitchTarget_t; /* SBSW_FEE_Func_passParamByReference */

    const Fee_SectorRefType newSector =
                                    Fee_Partition_getNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    Fee_InstanceDescr_Init(&target->instance_t, 0, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkInfo_Construct(&target->chunk_t, newSector); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */

    Fee_ChunkSearchContext_Init(context_pt, /* SBSW_FEE_Func_passParamByReference */
                                        &Fee_Globals_t.CurrentProcessData.BlockId, /* SBSW_FEE_Func_passParamByReference */
                                        NULL_PTR, target, Fee_Proc_getSectorEndOffset(newSector)); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1); /* SBSW_FEE_Func_passParamByReference */
    Fee_ActPrepareChunkLinkReadOfCurrentChunk();
}

/**********************************************************************************************************************
 * Fee_ActContinueUpwardsChunkSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActContinueUpwardsChunkSearch(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    const Fee_InstanceDescriptorRefType target = &Fee_Globals_t.SectorSwitchTarget_t;
    const Fee_SectorRefType newSector = Fee_ChunkInfo_GetSector(&target->chunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkSearchContext_Init(context_pt, /* SBSW_FEE_Func_passParamByReference */
                                        &Fee_Globals_t.CurrentProcessData.BlockId, /* SBSW_FEE_Func_passParamByReference */
                                        NULL_PTR, target, Fee_Proc_getSectorEndOffset(newSector));

    Fee_ActPrepareChunkLinkReadOfCurrentChunk();
}

/**********************************************************************************************************************
 * Fee_ActInitSearchWrittenFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchWrittenFsm(void)
{
    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_CURRENT); /* SBSW_FEE_Func_passParamByReference */
    Fee_ActInitChunkSearchFsm();
}

/**********************************************************************************************************************
 * Fee_ActInitSetupSectorsFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSetupSectorsFsm(void)
{
    const Fee_GlobalsPtrType globals = &Fee_Globals_t;
    const Fee_SectorFormatContextRefType context_pt = &globals->FsmLocalVars_t.SectorFormatContext_t;

    const Fee_SectorRefType newSector = Fee_Partition_getNewestSector(globals->CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
    uint16 linkTableSize = Fee_Partition_getConfiguredLinktableSize(globals->CurrentProcessData.Partition_pt);   /* SBSW_FEE_InitializedProcessData */
    
    if(Fee_InternalIsInFblMode(globals)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {

        const uint16 minLinkTableSize = Fee_Sector_currentLinktableEntryCount(newSector); /* SBSW_FEE_Partitions_OldestNewestSectorRef */
        if(linkTableSize < minLinkTableSize)
        {
            linkTableSize = minLinkTableSize;
        }
    }

    context_pt->sector_pt = Fee_Partition_getOldestSector(globals->CurrentProcessData.Partition_pt); /* SBSW_FEE_Func_writeObjectByReference */  /* SBSW_FEE_InitializedProcessData */
    context_pt->linkTableSize_u16 = linkTableSize; /* SBSW_FEE_Func_writeObjectByReference */
    context_pt->sectorId_u8 = Fee_Partition_getNextSectorId(globals->CurrentProcessData.Partition_pt); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_InitializedProcessData */

    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_SETUP_SECTORS_ERASING); /* SBSW_FEE_Func_passParamByReference */

    FEE_NFY_SECTOR_ERASE(globals);
    Fee_Proc_SetupWriteSuspendHandling(globals, Fee_InternalDoSectorErase); /* SBSW_FEE_Func_passParamByReference */
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_InternalDoSectorErase
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_FEE_16.7_Pointer2ConstParams */
FEE_LOCAL_FUNC(void) Fee_InternalDoSectorErase(Fee_GlobalsPtrType g)
{
    /* SectorFormatContext_t was initialized in Fee_ActInitSetupSectorsFsm */
    Fee_InternalEvalFlsRetVal(Fee_Sector_erase(g->FsmLocalVars_t.SectorFormatContext_t.sector_pt)); /* SBSW_FEE_Partitions_OldestNewestSectorRef */
}

/**********************************************************************************************************************
 * Fee_ActSetupSectorsWriteHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupSectorsWriteHeader(void)
{
    const Fee_SectorFormatContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.SectorFormatContext_t;

    /* SectorFormatContext_t was initialized in Fee_ActInitSetupSectorsFsm */
    Fee_Sector_assembleHeader(context_pt->sector_pt, /* SBSW_FEE_Partitions_OldestNewestSectorRef */
                              context_pt->sectorHeader_au32,
                              context_pt->sectorId_u8,
                              context_pt->linkTableSize_u16);

    Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoSectorHeaderWrite); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_InternalDoSectorHeaderWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_FEE_16.7_Pointer2ConstParams */
FEE_LOCAL_FUNC(void) Fee_InternalDoSectorHeaderWrite(Fee_GlobalsPtrType g)
{
    Fee_InternalEvalFlsRetVal(
        Fee_Proc_writeSectorHeader(
            /* SectorFormatContext_t was initialized in Fee_ActInitSetupSectorsFsm */
            g->FsmLocalVars_t.SectorFormatContext_t.sector_pt, /* SBSW_FEE_Partitions_OldestNewestSectorRef */
                g->FsmLocalVars_t.SectorFormatContext_t.sectorHeader_au32));
}

/**********************************************************************************************************************
 * Fee_ActPreInitWriteProcessing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPreInitWriteProcessing(void)
{
    Fee_Proc_CreateChunkAllocContext(&Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t, /* SBSW_FEE_Func_passParamByReference */
                &Fee_Globals_t.CurrentDataset_t,                                        /* SBSW_FEE_Func_passParamByReference */
                Fee_Globals_t.CurrentProcessData.BlockId,
                Fee_Globals_t.CurrentProcessData.BlockConfig_pt); /* SBSW_FEE_InitializedProcessData */
}

/**********************************************************************************************************************
 * Fee_Proc_CreateChunkAllocContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Proc_CreateChunkAllocContext(Fee_ChunkAllocContextRefType ctx, Fee_InstanceDescriptorRefType recentChunk, struct Fee_BlockIdStruct blockId, Fee_BlockConfigRefType cfg_pt)
{
#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
    ctx->wccChunk_pt = recentChunk; /* SBSW_FEE_PointerWrite_refParam */
#endif

    ctx->previousChunk_pt = recentChunk; /* SBSW_FEE_PointerWrite_refParam */

    ctx->blockId_t = blockId;  /* SBSW_FEE_PointerWrite_refParam */

    ctx->config_pt = cfg_pt;  /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_Write_SetupChunkAlloc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Proc_Write_SetupChunkAlloc(Fee_GlobalsPtrType g, Fee_ChunkAllocContextRefType ctx)
{
    if((Fee_Proc_isLinkTableChunkAllowed(&ctx->previousChunk_pt->chunk_t)) /* SBSW_FEE_ChunkAllocContext */
            && (ctx->config_pt->NumberOfDatasets_u8 > 1))                  /* SBSW_FEE_ChunkAllocContext */
    {
        struct Fee_ChunkInfoStruct c;
        Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_WRITE_PROCESSING_ALLOC_LINKTABLE); /* SBSW_FEE_Func_passParamByReference */
        Fee_ChunkInfo_CreateNewLinktableChunk(&c, (uint8)ctx->config_pt->NumberOfDatasets_u8); /* SBSW_FEE_Func_passParamByReference */

        Fee_InternalInitAllocateChunkFsm(ctx, &c); /* SBSW_FEE_Func_passParamByReference */

    }
    else
    {
        Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_WRITE_PROCESSING_ALLOC_DATACHUNK); /* SBSW_FEE_Func_passParamByReference */
        Fee_ActInitDataChunkAllocWriteFsm();
    }
}

/**********************************************************************************************************************
 * Fee_Proc_Write_SetupAllocation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_Proc_Write_SetupAllocation(Fee_GlobalsPtrType g, Fee_ChunkAllocContextRefType ctx)
{
    const Fee_ChunkInfoRefType currChunk = &ctx->previousChunk_pt->chunk_t;

    /* Chunk allocation may be interrupted by a sector switch Deferred job will be executed after sector switch then*/
    /* PRQA S 3344,3415 4 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
    g->InternalFlags_t.jobDeferred = TRUE; /* SBSW_FEE_Func_writeObjectByReference */

    /* PRQA S 3415 4 */ /* MD_FEE_12.4_queries */
    if((Fee_ChunkInfo_isDataChunk(currChunk)) /* SBSW_FEE_ChunkAllocContext */
         && (ctx->previousChunk_pt->instance_t.index_t < Fee_ChunkInfo_GetInstanceCount(currChunk))  /* SBSW_FEE_ChunkAllocContext */
         && (Fee_Proc_doesChunkEqualConfig(currChunk, ctx->config_pt))  /* SBSW_FEE_ChunkAllocContext */
         && (g->InternalFlags_t.brokenLink == FALSE))
         {
         Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_WRITE_PROCESSING_CREATE_INSTANCE); /* SBSW_FEE_Func_passParamByReference */
         Fee_ActInitCreateInstanceFsm();
     }
     else

     {
         Fee_Proc_Write_SetupChunkAlloc(g, ctx); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_ChunkAllocContext */
     }
}

/**********************************************************************************************************************
 * Fee_ActInitWriteProcessingFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitWriteProcessingFsm(void)
{
    const enum Fee_Partition_FillLevelEnum criticality =
                    Fee_Partition_getFillLevel(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;

    const Fee_Bool isNewSector = Fee_Proc_isNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt,  /* SBSW_FEE_InitializedProcessData */
                                                      Fee_ChunkInfo_GetSector(&context_pt->previousChunk_pt->chunk_t));  /* SBSW_FEE_Chunks_Sector_ref */ /* SBSW_FEE_ChunkAllocContext */

    if(context_pt->previousChunk_pt->instance_t.status_t != FEE_INSTANCE_STATUS_EMPTY)
    {
        context_pt->previousChunk_pt->instance_t.index_t++; /* SBSW_FEE_ChunkAllocContext */
    }

    if((criticality >= FEE_PART_FILL_LEVEL_FSS) ||
       (((Fee_Globals_t.InternalFlags_t.brokenLink == TRUE)) && (!isNewSector)))
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;

        Fee_Globals_t.InternalFlags_t.jobDeferred = (Fee_Globals_t.ApiFlags.enableFss == TRUE);
    }
    else
    {
        Fee_Proc_Write_SetupAllocation(&Fee_Globals_t, context_pt); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_ChunkAllocContext */
    }
}

/**********************************************************************************************************************
 * Fee_ActInitSearchForNfa_WriteFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitSearchForNfa_WriteFsm(void)
{
    const Fee_SectorRefType newSector = Fee_Partition_getNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
    Fee_InternalInitSearchForNfaFsm(&Fee_Globals_t, newSector); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */
}

/**********************************************************************************************************************
 * Fee_InternalInitSearchForNfaFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalInitSearchForNfaFsm(Fee_GlobalsPtrType globals_pt,
                                                                    Fee_SectorRefType sector_pt)
{
    const Fee_ChunkHoppingContextRefType context_pt = &globals_pt->FsmLocalVars_t.ChunkAllocContext_t.super_t;

    globals_pt->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_PointerWrite_refParam */

    if(!Fee_Sector_isFillLevelKnown(sector_pt)) /* SBSW_FEE_PointerWrite_refParam */
    {
        context_pt->upperSearchLimit_t = Fee_Proc_getSectorEndOffset(sector_pt);  /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_PointerWrite_refParam */

        context_pt->blockIdMask_t.Tag       = 0xFFFFu; /* SBSW_FEE_Func_writeObjectByReference */
        context_pt->blockIdMask_t.DataIndex = 0xFFu;   /* SBSW_FEE_Func_writeObjectByReference */

        context_pt->searchBlockId_t = context_pt->blockIdMask_t;  /* SBSW_FEE_Func_writeObjectByReference */

        context_pt->resultBlockId_pt = NULL_PTR;  /* SBSW_FEE_Func_writeObjectByReference */

        Fee_ChunkInfo_CreateNewDataChunk(&context_pt->tempChunk_t, 1, 1); /* SBSW_FEE_Func_passParamByReference */

        /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
        if(Fee_ChunkInfo_AssignSector(&context_pt->tempChunk_t, sector_pt)) /* SBSW_FEE_Func_passParamByReference */
        {

            globals_pt->chunkHoppingContext_pt = context_pt; /* SBSW_FEE_PointerWrite_refParam */

            Fee_FsmInitSubFsm(&globals_pt->FsmStack_t, FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH); /* SBSW_FEE_Func_passParamByReference */

            Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadChunkHeader(&context_pt->tempChunk_t, context_pt->headerBuffer_au32)); /* SBSW_FEE_Func_passParamByReference */
        }
        else
        {

            globals_pt->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_PointerWrite_refParam */
        }
    }        
}

/**********************************************************************************************************************
 * Fee_ActFinalizeSearchForNfa_WriteFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeSearchForNfa_WriteFsm(void)
{
    const Fee_ChunkInfoRefType currChunk = &Fee_Globals_t.CurrentDataset_t.chunk_t;

    if(Fee_ChunkInfo_isDataChunk(currChunk)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_ActInitInstanceSearchBinaryFsm();
    }
    else
    {
        Fee_InstanceDescr_Init(&Fee_Globals_t.CurrentDataset_t.instance_t, 0, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */
        Fee_ActReturnFromSubFsm();
    }
}

/**********************************************************************************************************************
 * Fee_ActInitDataChunkAllocWriteFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitDataChunkAllocWriteFsm(void)
{
    const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;
    const Fee_BlockConfigRefType blockConfig = context_pt->config_pt;
    struct Fee_ChunkInfoStruct c;

    Fee_ChunkInfo_CreateNewDataChunk(&c, /* SBSW_FEE_Func_passParamByReference */
                                    blockConfig->BlockPayloadSize_u16,
                                    (uint8)blockConfig->NumberOfInstancesExponent_u8);

    Fee_InternalInitAllocateChunkFsm(context_pt, &c); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitDataChunkAllocCopyFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitDataChunkAllocCopyFsm(void)
{
    const Fee_InstanceDescriptorRefType target = &Fee_Globals_t.SectorSwitchTarget_t;
    const Fee_InstanceDescriptorRefType source = &Fee_Globals_t.CurrentDataset_t;
    const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;
    const Fee_BlockConfigRefType blockConfig = Fee_Globals_t.CurrentProcessData.BlockConfig_pt;
    uint16 payload = Fee_ChunkInfo_getPayload(&source->chunk_t); /* SBSW_FEE_Func_passParamByReference */
    uint8 instanceExponent;
    Fee_Bool chunkNecessary;

    const uint16 targetInstances = Fee_ChunkInfo_GetInstanceCount(&target->chunk_t);       /* SBSW_FEE_Func_passParamByReference */

    if(target->instance_t.status_t != FEE_INSTANCE_STATUS_EMPTY)
    {
        target->instance_t.index_t++; /* SBSW_FEE_Func_writeObjectByReference */
        target->instance_t.status_t = FEE_INSTANCE_STATUS_EMPTY; /* SBSW_FEE_Func_writeObjectByReference */
    }

    if(blockConfig != NULL_PTR)
    {
        if(blockConfig->BlockPayloadSize_u16 != payload) /* SBSW_FEE_Func_passParamByReference */
        {
            source->instance_t.status_t = FEE_INSTANCE_STATUS_INVALID; /* SBSW_FEE_Func_writeObjectByReference */
            payload = blockConfig->BlockPayloadSize_u16;
        }

        instanceExponent = (uint8)blockConfig->NumberOfInstancesExponent_u8;
    }
    else
    {
        /*In FBL mode, during sector switch always chunks with one instance are created*/
        instanceExponent = 1u;
    }

    if(Fee_ChunkInfo_getPayload(&target->chunk_t) != payload) /* SBSW_FEE_Func_passParamByReference */
    {
        target->instance_t.index_t = targetInstances; /* SBSW_FEE_Func_writeObjectByReference */
    }

    /* PRQA S 3415 3 */ /* MD_FEE_12.4_queries */
    chunkNecessary = ((!Fee_ChunkInfo_isDataChunk(&target->chunk_t)) /* SBSW_FEE_Func_passParamByReference */
                                || (target->instance_t.index_t >= targetInstances));

#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
    chunkNecessary = ((!Fee_ChunkInfo_isDataChunk(&target->chunk_t)) || chunkNecessary); /* SBSW_FEE_Func_passParamByReference */
#endif

    if(chunkNecessary)   /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        struct Fee_ChunkInfoStruct c;

        context_pt->blockId_t = Fee_Globals_t.CurrentProcessData.BlockId;   /* SBSW_FEE_Func_writeObjectByReference */
        context_pt->previousChunk_pt = &Fee_Globals_t.SectorSwitchTarget_t; /* SBSW_FEE_Func_writeObjectByReference */

        /* now we can store our new chunk's parameters */
        Fee_ChunkInfo_CreateNewDataChunk(&c, payload, instanceExponent); /* SBSW_FEE_Func_passParamByReference */

        Fee_InternalInitAllocateChunkFsm(context_pt, &c); /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_ActAllocationInCurrentSetor
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActAllocationInCurrentSector(void)
{
    /* PRQA S 3415 2 */ /* MD_FEE_12.4_queries */
    if((Fee_Globals_t.FeeLocalJobResult_t != FEE_LOCAL_JOB_RESULT_OK) &&
       (!Fee_Proc_isNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt, /* SBSW_FEE_InitializedProcessData */
                                      Fee_ChunkInfo_GetSector(&Fee_Globals_t.CurrentDataset_t.chunk_t)))) /* SBSW_FEE_Func_passParamByReference */
    {

        Fee_Proc_CreateChunkAllocContext(&Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t, /* SBSW_FEE_Func_passParamByReference */
                &Fee_Globals_t.SectorSwitchTarget_t, /* SBSW_FEE_Func_passParamByReference */
                Fee_Globals_t.CurrentProcessData.BlockId, Fee_Globals_t.CurrentProcessData.BlockConfig_pt); /* SBSW_FEE_InitializedProcessData */

#       if(FEE_GET_WRITE_CYCLE_API == STD_ON)
        Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t.wccChunk_pt = &Fee_Globals_t.CurrentDataset_t;
        #endif

        Fee_ActInitWriteProcessingFsm();
    }
}

/**********************************************************************************************************************
 * Fee_ActInitCopyProcessFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitCopyProcessFsm(void)
{

    /*It has to be checked here again, because Disable_FSS could have been called after the start of SS*/
#if(FEE_FSS_CONTROL_API == STD_ON)
    if((Fee_Partition_getFillLevel(Fee_Globals_t.CurrentProcessData.Partition_pt) < FEE_PART_FILL_LEVEL_FSS) /* SBSW_FEE_InitializedProcessData */
            || (Fee_Globals_t.ApiFlags.enableFss == TRUE))
    {
        Fee_InternalDoCopyInit(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS;
    }
#else
    Fee_InternalDoCopyInit(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
#endif

}

/**********************************************************************************************************************
 * Fee_InternalDoCopyInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_InternalDoCopyInit(Fee_GlobalsPtrType g)
{
    const Fee_InstanceDescriptorRefType source = &g->CurrentDataset_t;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(Fee_ChunkInfo_isDataChunk(&source->chunk_t)) /* SBSW_FEE_Func_passParamByReference */
    {
        /* Always go through Link-table allocation (initialization decides, whether it is necessary or not */
        Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_COPY_ALLOCATE_LINK_CHUNK); /* SBSW_FEE_Func_passParamByReference */
        Fee_InternalInitLinkTableAllocationFsm(g); /* SBSW_FEE_PointerWrite_refParam */
    }
    else
    {
        g->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK; /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_ActInitializeBlockCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitializeBlockCopy(void)
{
    const Fee_InstanceDescriptorRefType target = &Fee_Globals_t.SectorSwitchTarget_t;
    const Fee_InstanceDescriptorRefType source = &Fee_Globals_t.CurrentDataset_t;
    const Fee_InstanceCopyContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceCopyContext_t;

    context_pt->sourceOffset_t = /* SBSW_FEE_Func_writeObjectByReference */
        Fee_ChunkInfo_GetInstanceStartAddress(&source->chunk_t, source->instance_t.index_t); /* SBSW_FEE_Func_passParamByReference */
    context_pt->targetOffset_t = /* SBSW_FEE_Func_writeObjectByReference */
        Fee_ChunkInfo_GetInstanceStartAddress(&target->chunk_t, target->instance_t.index_t); /* SBSW_FEE_Func_passParamByReference */
    context_pt->ByteCount_t = Fee_ChunkInfo_GetInstanceSize(&source->chunk_t); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Func_writeObjectByReference */

    switch(source->instance_t.status_t)
    {
        case FEE_INSTANCE_STATUS_OK:
            FEE_NFY_INSTANCE_COPY(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */

            Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_READ); /* SBSW_FEE_Func_passParamByReference */

            Fee_ActPrepareBlockCopyRead();
            break;
        case FEE_INSTANCE_STATUS_INCONSISTENT:
            Fee_InternalPrepareEmptyInstanceWrite(
                                            Fee_ChunkInfo_GetSector(&target->chunk_t), /* SBSW_FEE_Chunks_Sector_ref */ /* SBSW_FEE_Func_passParamByReference */
                                            context_pt->targetOffset_t,
                                            source->instance_t.status_t);
            break;
        default:
            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
            break;
    }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_ActFinalizeInstanceCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeInstanceCopy(void)
{
    FEE_NFY_INSTANCE_COPY_END(&Fee_Globals_t, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActNop
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActNop(void)
{
    /* Do nothing. */
}

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamFirst
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamFirst(Fee_GlobalsPtrType globals)
{
    const Fee_InstanceWriteContextRefType context = &globals->FsmLocalVars_t.instanceWriteContext_t;
    const Fee_IntBuffer_u8PtrType DataBuffer_pu8loc = context->BufferBytes;        
    const Fee_LengthType writeAddr = context->targetOffset_t;

    Fee_LengthType writeLength;
    uint32 PayloadBytesToCopy;

    if (context->ByteCount_u32 <= FEE_INTERNAL_BUFFER_SIZE)
    {
        writeLength = context->ByteCount_u32;
        PayloadBytesToCopy = (context->ByteCount_u32 - (FEE_INSTANCE_HEADER_SIZE + FEE_INSTANCE_TRAILER_SIZE));

        /* complete instance; no bytes left */
        context->ByteCount_u32 = 0;  /* SBSW_FEE_Func_writeObjectByReference */

        DataBuffer_pu8loc[PayloadBytesToCopy + FEE_INSTANCE_HEADER_SIZE] = FEE_INSTANCE_STATUS_OK; /* SBSW_FEE_InternalDataBuffer_writeAccess */
    }
    else
    {
        writeLength = FEE_INTERNAL_BUFFER_SIZE;
        PayloadBytesToCopy = FEE_INTERNAL_BUFFER_SIZE - FEE_INSTANCE_HEADER_SIZE;
        /*Write full buffer length in first write stream*/
        /*Set Remaining Bytes, includes the INSTANCE_TRAILER (it must also be written!) */
        context->ByteCount_u32 -= writeLength;  /* SBSW_FEE_Func_writeObjectByReference */
    }

    DataBuffer_pu8loc[0u] = FEE_INSTANCE_STATUS_OK; /* SBSW_FEE_Func_writeObjectByReference */ /* Note: Accessing [0] cannot go out of bounds! */

    Fee_InternalFillInstanceBuffer(&DataBuffer_pu8loc[1u], context->userData_pt, PayloadBytesToCopy); /* SBSW_FEE_InternalDataBuffer_passedTo */ /* SBSW_FEE_ConstPointer_refParam */

    context->userData_pt =  /* SBSW_FEE_Func_writeObjectByReference */
      &context->userData_pt[PayloadBytesToCopy];

    context->targetOffset_t += writeLength;  /* SBSW_FEE_Func_writeObjectByReference */
    
    Fee_InternalEvalFlsRetVal(Fee_Sector_writeData(context->sector_pt, writeAddr, DataBuffer_pu8loc, writeLength)); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ActPrepareWriteStreamSecond
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareWriteStreamSecond(void)
{
    const Fee_InstanceWriteContextRefType context = &Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t;

    if(context->ByteCount_u32 > FEE_INTERNAL_BUFFER_SIZE)
    {
        Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoWriteStreamSecond); /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamSecond
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamSecond(Fee_GlobalsPtrType g)
{
    const Fee_InstanceWriteContextRefType context = &g->FsmLocalVars_t.instanceWriteContext_t;
    const Fee_UserDataConstPtrType writeBuf = context->userData_pt;

    /* write Length must be an integral multiple of internal buffer size (round down).
     * Only consider user data bytes; exclude trailer => it MUST be written in last stream! */
    const uint32 writeLength = (context->ByteCount_u32 - FEE_INSTANCE_TRAILER_SIZE) & (~(FEE_INTERNAL_BUFFER_SIZE - 1u));

    const Fee_LengthType writeAddr = context->targetOffset_t;

    context->targetOffset_t += writeLength;  /* SBSW_FEE_Func_writeObjectByReference */
    context->ByteCount_u32 -= writeLength;   /* SBSW_FEE_Func_writeObjectByReference */

    context->userData_pt = &writeBuf[writeLength];  /* SBSW_FEE_Func_writeObjectByReference */

    Fee_InternalEvalFlsRetVal(Fee_Sector_writeData(context->sector_pt, writeAddr, writeBuf, writeLength)); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ActPrepareBlockCopyRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareBlockCopyRead(void)
{
    const Fee_InstanceCopyContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceCopyContext_t;
    const Fee_SectorConstRefType oldSector =
                                    Fee_Partition_getOldestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    const Fee_LengthType readLength = 
            (context_pt->ByteCount_t > FEE_INTERNAL_BUFFER_SIZE) ? FEE_INTERNAL_BUFFER_SIZE : context_pt->ByteCount_t;

    Fee_Sector_ClearBuffer(oldSector, context_pt->BufferBytes, FEE_INTERNAL_BUFFER_SIZE);

    Fee_InternalEvalFlsRetVal(
        Fee_Sector_readData(oldSector, context_pt->sourceOffset_t, context_pt->BufferBytes, readLength)); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

    context_pt->sourceOffset_t += readLength;  /* SBSW_FEE_Func_writeObjectByReference */
}

/**********************************************************************************************************************
 * Fee_ActPrepareBlockCopyWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareBlockCopyWrite(void)
{
    Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoBlockCopyWrite); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_InternalDoBlockCopyWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoBlockCopyWrite(Fee_GlobalsPtrType g)
{
    const Fee_InstanceCopyContextRefType context_pt = &g->FsmLocalVars_t.instanceCopyContext_t;
    const Fee_SectorConstRefType newSector_pt = Fee_Partition_getNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    /*WriteLength must be computed the same way as readLength was before*/
    const Fee_LengthType writeLength = 
        (context_pt->ByteCount_t > FEE_INTERNAL_BUFFER_SIZE) ? FEE_INTERNAL_BUFFER_SIZE : context_pt->ByteCount_t;

    Fee_InternalEvalFlsRetVal(Fee_Sector_writeData(newSector_pt, /* SBSW_FEE_Partitions_OldestNewestSectorRef */
                         context_pt->targetOffset_t,
                         context_pt->BufferBytes, writeLength));

    context_pt->targetOffset_t += writeLength; /* SBSW_FEE_Func_writeObjectByReference */
    context_pt->ByteCount_t -= writeLength;    /* SBSW_FEE_Func_writeObjectByReference */
}

#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_ActPrepareWccRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareWccRead(void)
{
    const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;
    const Fee_ChunkInfoConstRefType chunk = &context_pt->wccChunk_pt->chunk_t;

    /* PRQA S 3344,3415 1 */ /* MD_FEE_13.2_boolean */
    if(Fee_ChunkInfo_isDataChunk(chunk)) /* SBSW_FEE_ChunkAllocContext */

    {
        /*WCC immediately follows the 'actual chunk header'*/
        const Fee_LengthType addr =
            Fee_ChunkInfo_GetBaseAddress(chunk) + FEE_CHUNK_HEADER_SIZE_READ; /* SBSW_FEE_ChunkAllocContext */
     
        Fee_InternalEvalFlsRetVal(Fee_Sector_readMngmtData(Fee_ChunkInfo_GetSector(chunk), /* SBSW_FEE_Chunks_Sector_ref */ /* SBSW_FEE_ChunkAllocContext */
                                                           addr,
                                                           &context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET], /* SBSW_FEE_InternalDataBuffer_passedTo */
                                                           FEE_WCC_BYTE_COUNT));
    }
    else
    {
        Fee_Globals_t.FlsJobResult_t = MEMIF_JOB_OK;
        context_pt->buffer_au32[FEE_CHUNK_HEADER_WCC_WORD_OFFSET] = FEE_WRITE_CYCLE_EMPTY; /* SBSW_FEE_InternalDataBuffer_writeAccess */
    }
}
#endif

/**********************************************************************************************************************
 * Fee_ActPrepareChunkHeaderWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkHeaderWrite(void)
{
    const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;

    /* PRQA S 3344 2*/ /* MD_FEE_13.2_boolean */
    if(Fee_ChunkInfo_AssignSector(&context_pt->newChunk_t, /* SBSW_FEE_Func_passParamByReference */
                                  Fee_ChunkInfo_GetSector(&context_pt->previousChunk_pt->chunk_t))) /* SBSW_FEE_ChunkAllocContext */ /* SBSW_FEE_Chunks_Sector_ref */
    {

        context_pt->ChunkAllocResult_t = FEE_CHUNK_ALLOC_HEADER_FAILED; /* SBSW_FEE_Func_writeObjectByReference */

        Fee_ChunkInfo_AssembleHeader(&context_pt->newChunk_t, /* SBSW_FEE_Func_passParamByReference */
                                     &context_pt->blockId_t,  /* SBSW_FEE_Func_passParamByReference */
                                     context_pt->buffer_au32);

        FEE_NFY_CHUNK_HEADER_WRITE(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
        Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoChunkHeaderWrite); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
        Fee_ActReturnFromSubFsm();
    }
}

/**********************************************************************************************************************
 * Fee_InternalDoChunkHeaderWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_FEE_16.7_Pointer2ConstParams */
FEE_LOCAL_FUNC(void) Fee_InternalDoChunkHeaderWrite(Fee_GlobalsPtrType g)
{
    Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_WriteHeader(&g->FsmLocalVars_t.ChunkAllocContext_t.newChunk_t, /* SBSW_FEE_Func_passParamByReference */
                                                        g->FsmLocalVars_t.ChunkAllocContext_t.buffer_au32));
}

/**********************************************************************************************************************
 * Fee_ActPrepareChunkLinkReadOfCurrentChunk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkLinkReadOfCurrentChunk(void)
{
    enum Fee_FlsRequestStatus ret;
    Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CHUNK_LINK_PROCESSING); /* SBSW_FEE_Func_passParamByReference */

    ret = Fee_ChunkInfo_ReadChunkLink(&context_pt->mostRecentChunk_t, /* SBSW_FEE_Func_passParamByReference */
                                         &context_pt->super_t.searchBlockId_t, /* SBSW_FEE_Func_passParamByReference */
                                         context_pt->super_t.headerBuffer_au32);

    /*In case there was no link to read (i.e. block not in linktable), reading was faked and ret set to FEE_REQ_SYNC_USELESS*/
    context_pt->chunkLinkPresent = (boolean)(ret != FEE_REQ_SYNC_USELESS); /* SBSW_FEE_Func_writeObjectByReference */

    Fee_InternalEvalFlsRetVal(ret);
}

/* Following function is only used by Fee_ActPrepareChunkLinkWrite */
/**********************************************************************************************************************
 * Fee_InternalSetupChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * \brief      Assembles link from given context and requests Flash to write it
 * \details    Assembles link from given context and requests Flash to write it
 * \param[in,out]  context_pt
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(void) Fee_InternalSetupChunkLinkWrite(Fee_ChunkAllocContextRefType);

/**********************************************************************************************************************
 * Fee_ActPrepareChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareChunkLinkWrite(void)
{
    FEE_NFY_CHUNK_HEADER_WRITE_END(&Fee_Globals_t, Fee_Globals_t.FlsJobResult_t);  /* SBSW_FEE_Func_passParamByReference */

    if(Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK)
    {
        FEE_NFY_CHUNK_LINK_WRITE(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
        Fee_InternalSetupChunkLinkWrite(&Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
        Fee_ActReturnFromSubFsm();
    }
}

/**********************************************************************************************************************
 * Fee_InternalSetupChunkLinkWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalSetupChunkLinkWrite(Fee_ChunkAllocContextRefType context_pt)
{
    Fee_Sector_confirmFillLevel(Fee_ChunkInfo_GetSector(&context_pt->newChunk_t), /* SBSW_FEE_ChunkSearchContext */ /* SBSW_FEE_Func_passParamByReference */
                            Fee_ChunkInfo_GetEndAddress(&context_pt->newChunk_t) + 1u); /* SBSW_FEE_Func_passParamByReference */

    context_pt->ChunkAllocResult_t = FEE_CHUNK_ALLOC_TRAILER_FAILED; /* SBSW_FEE_PointerWrite_refParam */

    Fee_ChunkInfo_AssembleLink(&context_pt->newChunk_t, context_pt->buffer_au32); /* SBSW_FEE_Func_passParamByReference */

    /* Fee_ChunkInfoWriteChunkLink checks whether the link is writeable */
    Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_WriteChunkLink(&context_pt->previousChunk_pt->chunk_t, /* SBSW_FEE_ChunkAllocContext */
                                            &context_pt->blockId_t, /* SBSW_FEE_Func_passParamByReference */
                                            context_pt->buffer_au32));
}

/**********************************************************************************************************************
 * Fee_ActPrepareInstanceHeaderRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareInstanceHeaderRead(void)
{
    Fee_InstSearch_continue(&Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActPrepareInstanceTrailerRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareInstanceTrailerRead(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadInstanceTrailer(
        &context_pt->instDescr_pt->chunk_t, context_pt->instDescr_pt->instance_t.index_t, context_pt->Buffer_au32)); /* SBSW_FEE_InstanceSearchContext */
}

/**********************************************************************************************************************
 * Fee_ActPrepareWriteStreamLast
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareWriteStreamLast(void)
{
    const Fee_InstanceWriteContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t;

    if(context_pt->ByteCount_u32 > 0)
    {

        Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoWriteStreamLast); /* SBSW_FEE_Func_passParamByReference */
    }
}

/**********************************************************************************************************************
 * Fee_InternalDoWriteStreamLast
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalDoWriteStreamLast(Fee_GlobalsPtrType g)
{
    const Fee_InstanceWriteContextRefType context_pt = &(g->FsmLocalVars_t.instanceWriteContext_t);
    const uint32  writeLength = context_pt->ByteCount_u32;
    const Fee_LengthType writeAddr   = context_pt->targetOffset_t;

    Fee_Sector_ClearBuffer(context_pt->sector_pt, context_pt->BufferBytes, context_pt->ByteCount_u32);

    context_pt->BufferBytes[writeLength - FEE_INSTANCE_TRAILER_SIZE] = FEE_INSTANCE_STATUS_OK; /* SBSW_FEE_InternalDataBuffer_writeAccess */

    Fee_InternalFillInstanceBuffer(context_pt->BufferBytes, context_pt->userData_pt, (writeLength - FEE_INSTANCE_TRAILER_SIZE)); /* SBSW_FEE_InternalDataBuffer_passedTo */ /* SBSW_FEE_ConstPointer_refParam */

    Fee_InternalEvalFlsRetVal(
                    Fee_Sector_writeData(context_pt->sector_pt, writeAddr, context_pt->BufferBytes, writeLength)); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_InternalPrepareEmptyInstanceWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalPrepareEmptyInstanceWrite(Fee_SectorConstRefType sector, Fee_LengthType addr, Fee_InstanceStatusType status)
{
    const Fee_IntBuffer_u8PtrType bufPtr = Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t.BufferBytes;
    
    Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t.targetOffset_t = addr;
    Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t.sector_pt = sector;

    /*An empty instance consists of 4 Byte, all set to the corresponding status (inconsistent or invalid)*/
    bufPtr[0]          /* SBSW_FEE_Func_writeObjectByReference */ /* Note: Accessing [0] is always safe */
           = bufPtr[1] /* SBSW_FEE_InternalDataBuffer_writeAccess */
           = bufPtr[2] /* SBSW_FEE_InternalDataBuffer_writeAccess */
           = bufPtr[3] /* SBSW_FEE_InternalDataBuffer_writeAccess */
                    = (uint8)status;

    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CREATE_INSTANCE_1_REMAINING_WRITE_STREAM); /* SBSW_FEE_Func_passParamByReference */

    Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoEmptyInstanceWrite); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_InternalDoEmptyInstanceWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3673 1 */ /* MD_FEE_16.7_Pointer2ConstParams */
FEE_LOCAL_FUNC(void) Fee_InternalDoEmptyInstanceWrite(Fee_GlobalsPtrType g)
{
    Fee_InternalEvalFlsRetVal(Fee_Sector_writeData(
                         g->FsmLocalVars_t.instanceWriteContext_t.sector_pt,
                         g->FsmLocalVars_t.instanceWriteContext_t.targetOffset_t,
                         g->FsmLocalVars_t.instanceWriteContext_t.BufferBytes,
                         FEE_INSTANCE_HEADER_SIZE)); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ActPreparePayloadDataProcessing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPreparePayloadDataProcessing(void)
{
    const Fee_InstanceDescriptorRefType instDescr_ptloc = &Fee_Globals_t.CurrentDataset_t;

    Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadInstancePayload(
                         &instDescr_ptloc->chunk_t, /* SBSW_FEE_Func_passParamByReference */
                         instDescr_ptloc->instance_t.index_t,
                         Fee_Globals_t.JobParams.params.blockJob.add.read.readBuffer, /* SBSW_FEE_UserDataBuffer */
                         Fee_Globals_t.JobParams.params.blockJob.add.read.offset,
                         Fee_Globals_t.JobParams.params.blockJob.add.read.length));
}

/**********************************************************************************************************************
 * Fee_ActContinueSearchForNfA
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActContinueSearchForNfA(void)
{
    const Fee_ChunkHoppingContextRefType context_pt = Fee_Globals_t.chunkHoppingContext_pt;

    Fee_InternalInitChunkHoppingFsm(&Fee_Globals_t, context_pt); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActPrepareSectorHeaderUpperRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActPrepareSectorHeaderUpperRead(void)
{
    const Fee_PartitionRefType partition = Fee_Globals_t.CurrentProcessData.Partition_pt; /* SBSW_FEE_InitializedProcessData */
    
    const Fee_Bool readOk = (Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK);

    Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.lowerSectorId = Fee_Sector_evaluateHeaderData(
                       Fee_Partition_getSectorByPos(partition, FEE_LOWER_SECTOR_NUMBER), /* SBSW_FEE_PartitionConfig */ /* SBSW_FEE_InitializedProcessData */
                       Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.sectorHeaderData, readOk);

    Fee_InternalEvalFlsRetVal(Fee_Proc_readSectorHeader(Fee_Partition_getSectorByPos(partition, FEE_UPPER_SECTOR_NUMBER), /* SBSW_FEE_PartitionConfig */ /* SBSW_FEE_InitializedProcessData */
                                                Fee_Globals_t.FsmLocalVars_t.PartitionInitContext_t.sectorHeaderData));
}

/**********************************************************************************************************************
 * Fee_ActReportSectorOverflow
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActReportSectorOverflow(void)
{
    if(Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t.ChunkAllocResult_t == FEE_CHUNK_ALLOC_SECTOR_FULL)
    {
        Fee_Partition_handleError(Fee_Globals_t.CurrentProcessData.Partition_pt, FEE_SECTOR_OVERFLOW); /* SBSW_FEE_InitializedProcessData */

        Fee_InternalResetSectorSwitch(&Fee_Globals_t.SectorSwitchData_t, Fee_InternalIsInFblMode(&Fee_Globals_t)); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Func_passParamByReference */
        
        /*The job result is abused here to signal function Fee_ActFinalizeBlockCopy, that sector switch data may not be updated*/
        Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS; 
    }
}

/**********************************************************************************************************************
 * Fee_ActResetChunkSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActResetChunkSearch(void) {
    Fee_ChunkInfo_Construct(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t, /* SBSW_FEE_Func_passParamByReference */
        Fee_ChunkInfo_GetSector(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t)); /* SBSW_FEE_Func_passParamByReference */
    Fee_ChunkInfo_unconfirmLink(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */
    Fee_ChunkInfo_Clear(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.recentChunk_pt->chunk_t); /* SBSW_FEE_ChunkSearchContext */
}

/**********************************************************************************************************************
 * Fee_ActReturnFromSubFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActReturnFromSubFsm(void)
{
    Fee_Globals_t.FsmStack_t.TopOfStack_u8--;
}

/**********************************************************************************************************************
 * Fee_ActGetCurrentChunkInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActGetCurrentChunkInfo(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    /*At this place, if previous chunk's link was defective, current chunk might be an orphan chunk.*/
    /*This information needs to be inherited in order to go on with search. */
    if(!Fee_ChunkInfo_isLinkConsistent(&context_pt->recentChunk_pt->chunk_t)) /* SBSW_FEE_ChunkSearchContext */
    {
        Fee_ChunkInfo_unconfirmLink(&context_pt->mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */
    }

    Fee_ChunkInfo_Clone(&context_pt->recentChunk_pt->chunk_t, &context_pt->mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */
    Fee_ChunkInfo_Clone(&context_pt->mostRecentChunk_t, &context_pt->super_t.tempChunk_t);     /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActInitInstanceSearchUpwards
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActInitInstanceSearchUpwards(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;

    Fee_ChunkInfo_Clone(&Fee_Globals_t.SectorSwitchTarget_t.chunk_t,  /* SBSW_FEE_Func_passParamByReference */
        &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_InstSearch_InitContext(&Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t, &Fee_Globals_t.SectorSwitchTarget_t, 0u); /* SBSW_FEE_Func_passParamByReference */

    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS;

    Fee_InstSearch_continue(context_pt); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultFailed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultFailed(void)
{
    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED;
}

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultInvalid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultInvalid(void)
{
    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_INVALID;
}

/**********************************************************************************************************************
 * Fee_ActSetLocalJobResultOk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetLocalJobResultOk(void)
{
    Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
}

/**********************************************************************************************************************
 * Fee_ActSetupSectorSwitchSourceSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupSectorSwitchSourceSearch(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    /* FblSectorSwitch Dataset holds the address of a chunk belonging to the current block */
    Fee_SectorConstRefType oldSector;

    Fee_ChunkInfo_Clear(&Fee_Globals_t.PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */

    if(Fee_InternalIsInFblMode(&Fee_Globals_t)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_Func_passParamByReference */
    {
        Fee_ChunkInfo_Clone(&Fee_Globals_t.CurrentDataset_t.chunk_t, &Fee_Globals_t.SectorSwitchData_t.mode.fbl); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        Fee_ChunkInfo_Construct(&Fee_Globals_t.CurrentDataset_t.chunk_t, /* SBSW_FEE_Func_passParamByReference */
                                 Fee_Partition_getOldestSector(Fee_Globals_t.CurrentProcessData.Partition_pt)); /* SBSW_FEE_Partitions_OldestNewestSectorRef */ /* SBSW_FEE_InitializedProcessData */
    }
    
    oldSector = Fee_ChunkInfo_GetSector(&Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */
    
    Fee_ChunkSearchContext_Init(context_pt, 
                                &Fee_Globals_t.CurrentProcessData.BlockId,  /* SBSW_FEE_Func_passParamByReference */
                                &Fee_Globals_t.PreviousChunk_t,             /* SBSW_FEE_Func_passParamByReference */
                                &Fee_Globals_t.CurrentDataset_t,            /* SBSW_FEE_Func_passParamByReference */
                                Fee_Proc_getSectorEndOffset(oldSector));

    Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_1); /* SBSW_FEE_Func_passParamByReference */
    Fee_ActInitChunkSearchFsm();
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_InternalEvalFlsRetVal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalEvalFlsRetVal(enum Fee_FlsRequestStatus apiRetVal)
{

    switch(apiRetVal)
    {
        case FEE_REQ_SYNC_OK:
            Fee_Globals_t.FlsJobResult_t = MEMIF_JOB_OK;
            break;

        case FEE_REQ_ACCEPTED:
            Fee_Globals_t.FlsJobResult_t = MEMIF_JOB_PENDING;
            Fee_Internal_SignalWait();
#if(FEE_POLLING_MODE == STD_ON)

            Fee_SignalReEnterMainFunction();
#endif
            break;
        default:
            Fee_Globals_t.FlsJobResult_t = MEMIF_JOB_FAILED;
            break;
    }
}

/**********************************************************************************************************************
 * Fee_ActSwitchOperatingSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSwitchOperatingSector(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    const Fee_SectorRefType oldSector = Fee_Partition_getOldestSector(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

    Fee_ChunkInfo_Construct(&Fee_Globals_t.CurrentDataset_t.chunk_t, oldSector); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */
    Fee_ChunkInfo_Clear(&Fee_Globals_t.PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */

    Fee_ChunkSearchContext_Init(context_pt,            /* SBSW_FEE_Func_passParamByReference */
            &Fee_Globals_t.CurrentProcessData.BlockId, /* SBSW_FEE_Func_passParamByReference */
            &Fee_Globals_t.PreviousChunk_t,            /* SBSW_FEE_Func_passParamByReference */
            &Fee_Globals_t.CurrentDataset_t,           /* SBSW_FEE_Func_passParamByReference */
            Fee_Proc_getSectorEndOffset(oldSector));   /* SBSW_FEE_Partitions_OldestNewestSectorRef */
}

/**********************************************************************************************************************
 * Fee_ActValidateChunkLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActValidateChunkLink(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    const Fee_ChunkInfoRefType currentChunk_pt = &context_pt->mostRecentChunk_t;
    const Fee_ChunkInfoRefType newChunk_pt = &context_pt->super_t.tempChunk_t;

    enum Fee_MngmtStatus linkStatus = 
                    Fee_ChunkInfo_InitFromLinkData(newChunk_pt,       /* SBSW_FEE_Func_passParamByReference */
                                                   currentChunk_pt,   /* SBSW_FEE_Func_passParamByReference */
                                                   context_pt->super_t.headerBuffer_au32,
                                                   (Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK));

    switch(linkStatus)
    {
        case FEE_MNGMT_ERASED:
            /*Latest chunk is found*/
            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
            Fee_ChunkInfo_confirmEmptyLink(currentChunk_pt); /* SBSW_FEE_Func_passParamByReference */
            break;

        case FEE_MNGMT_DEFECTIVE: 
            Fee_Globals_t.InternalFlags_t.brokenLink = TRUE;
            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_INCONSISTENT;
            Fee_ChunkInfo_unconfirmLink(currentChunk_pt); /* SBSW_FEE_Func_passParamByReference */

            /*A link of a block that does not fit into the linktable is just absent, don't force chunk hopping then*/
            if(context_pt->chunkLinkPresent) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
            {
                context_pt->chunkHoppingForced = TRUE; /* SBSW_FEE_Func_writeObjectByReference */
            }

            break;

        default: /*FEE_MNGMT_CONSISTENT*/

            /* PRQA S 3344,3415 3 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
            if(((context_pt->super_t.searchBlockId_t.DataIndex) != 0) &&
                      (Fee_ChunkInfo_isInitialLinkTable(currentChunk_pt)) && /* SBSW_FEE_Func_passParamByReference */
                            (Fee_ChunkInfo_isDataChunk(newChunk_pt))) /* SBSW_FEE_Func_passParamByReference */
            {
                /*The only allowed initial link-table entry pointing to a data chunk is for DataIndex == 0 */
                
                Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_INCONSISTENT;
                Fee_Globals_t.InternalFlags_t.brokenLink = TRUE;
                Fee_ChunkInfo_unconfirmLink(currentChunk_pt); /* SBSW_FEE_Func_passParamByReference */
                context_pt->chunkLinkPresent = FALSE; /* SBSW_FEE_Func_writeObjectByReference */
            }
            else if(Fee_ChunkInfo_GetBaseAddress(newChunk_pt) < context_pt->super_t.upperSearchLimit_t) /* SBSW_FEE_Func_passParamByReference */
            {
                Fee_ChunkInfo_Clone(&context_pt->recentChunk_pt->chunk_t, currentChunk_pt); /* SBSW_FEE_Func_passParamByReference */
                Fee_ChunkInfo_Clone(currentChunk_pt, newChunk_pt); /* SBSW_FEE_Func_passParamByReference */

                Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_INVALID;
            }
            else
            {
                Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
            }
            break;
    }
}

/**********************************************************************************************************************
 * Fee_ActCheckChunkConsistency
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActCheckChunkConsistency(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    Fee_LocalJobResultType localJobResult;

    Fee_Bool flsReadOk = Fee_Globals_t.FlsJobResult_t == MEMIF_JOB_OK;

    enum Fee_MngmtStatus headerStatus = Fee_ChunkInfo_CheckHeaderConsistency(
            context_pt->super_t.headerBuffer_au32, &context_pt->mostRecentChunk_t, context_pt->super_t.searchBlockId_t, flsReadOk); /* SBSW_FEE_Func_passParamByReference */

    if (headerStatus == FEE_MNGMT_CONSISTENT)
    {
        localJobResult = FEE_LOCAL_JOB_RESULT_OK;
    }
    else
    {
      localJobResult = FEE_LOCAL_JOB_RESULT_INCONSISTENT;
      
      Fee_Globals_t.InternalFlags_t.brokenLink = TRUE;
      context_pt->chunkHoppingForced = TRUE; /* SBSW_FEE_Func_writeObjectByReference */

      Fee_ChunkInfo_unconfirmLink(&context_pt->recentChunk_pt->chunk_t); /* SBSW_FEE_ChunkSearchContext */
      Fee_ChunkInfo_Clone(&context_pt->mostRecentChunk_t, &context_pt->recentChunk_pt->chunk_t); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_ChunkSearchContext */
    }

    Fee_Globals_t.FeeLocalJobResult_t = localJobResult;
}

/**********************************************************************************************************************
 * Fee_ActFinalizeChunkSearch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActFinalizeChunkSearch(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;

    if(context_pt->prevChunk_pt != NULL_PTR)
    {
        Fee_ChunkInfo_Clone(context_pt->prevChunk_pt, &context_pt->recentChunk_pt->chunk_t); /* SBSW_FEE_ChunkSearchContext */
    }
    
    Fee_ChunkInfo_Clone(&context_pt->recentChunk_pt->chunk_t, &context_pt->mostRecentChunk_t);  /* SBSW_FEE_ChunkSearchContext */ /* SBSW_FEE_Func_passParamByReference */

    Fee_InstanceDescr_Init(&context_pt->recentChunk_pt->instance_t, /* SBSW_FEE_ChunkSearchContext */
        Fee_ChunkInfo_GetInstanceCount(&context_pt->recentChunk_pt->chunk_t), FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_ChunkSearchContext */
}

/**********************************************************************************************************************
 * Fee_QryWriteSsupended
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryWriteSuspended(void) {
    return (Fee_Globals_t.WritesSuspended_bl == TRUE);
}

/**********************************************************************************************************************
 * Fee_ActIssueDeferredRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActIssueDeferredRequest(void) {
    Fee_Globals_t.DeferredWriteAction_pt(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_ActHandleWriteSuspended
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActHandleWriteSuspended(void)
{
        FEE_NFY_WRITE_SUSPENDED(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
        Fee_Internal_SignalWait();
}

/**********************************************************************************************************************
 * Fee_ActWait
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActWait(void)
{
    Fee_SignalReEnterMainFunction();
}

/**********************************************************************************************************************
 * Fee_Internal_SignalWait
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_Internal_SignalWait(void)
{
    Fee_Globals_t.InternalFlags_t.leaveMain = TRUE;
}

/**********************************************************************************************************************
 * Fee_SignalReEnterMainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_SignalReEnterMainFunction(void)
{
    Fee_Internal_SignalWait();
    Fee_SignalNeedProcessing();
}

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS STATE MACHINE - QUERIES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Fee_QryBlockCopyBytesRemaining
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryBlockCopyBytesRemaining(void)
{
    return (Fee_Globals_t.FsmLocalVars_t.instanceCopyContext_t.ByteCount_t > 0);
}

/**********************************************************************************************************************
 * Fee_QryChunkNotFull
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryChunkNotFull(void)
{
    return ((Fee_Globals_t.SectorSwitchTarget_t.instance_t.index_t + 1u) <
                                        (Fee_ChunkInfo_GetInstanceCount(&Fee_Globals_t.SectorSwitchTarget_t.chunk_t))); /* SBSW_FEE_Func_passParamByReference */
}  

/**********************************************************************************************************************
 * Fee_QryChunkIsDataChunk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryChunkIsDataChunk(void)
{
    return Fee_ChunkInfo_isDataChunk(&Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.mostRecentChunk_t); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_QryInstHeaderErasedValue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryInstHeaderErasedValue(void)
{
    const Fee_InstanceSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t;
    const Fee_InstanceStatusType status = Fee_InternalEvalInstHdrStatus(context_pt, Fee_Globals_t.FlsJobResult_t); /* SBSW_FEE_Func_passParamByReference */
    return (FEE_INSTANCE_STATUS_EMPTY == status);
}

/**********************************************************************************************************************
 * Fee_InternalIsFlsBusy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_InternalIsFlsBusy(Fee_GlobalsPtrType g)
{
    Fee_Bool isBusy = FALSE;

    if (g->FlsJobResult_t == MEMIF_JOB_PENDING)
    {
        /* --> Flash had accepted the job and FEE is still waiting. */

        /*Flash was busy, but FEE's internal Flash job result may be out of date*/
        isBusy = (Fee_FlsGetStatus() == MEMIF_BUSY);
        if (!isBusy)
        {
            g->FlsJobResult_t = Fee_FlsGetJobResult(); /* SBSW_FEE_PointerWrite_refParam */
        }
    }

    return isBusy;
}

/**********************************************************************************************************************
 * Fee_QryFlsJobResultOk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryFlsJobResultOk(void)
{
    return (MEMIF_JOB_OK == Fee_Globals_t.FlsJobResult_t);
}

/**********************************************************************************************************************
 * Fee_QryValidInstanceWritten
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryValidInstanceWritten(void)
{
    return ((Fee_Globals_t.FeeLocalJobResult_t != FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS) &&
                     (Fee_Globals_t.FeeLocalJobResult_t != FEE_LOCAL_JOB_RESULT_FAILED));
}  

/**********************************************************************************************************************
 * Fee_QryBlockPayloadUpdated
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryBlockPayloadUpdated(void)
{
    const Fee_ChunkInfoConstRefType chunk_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.recentChunk_pt->chunk_t;
    const Fee_BlockConfigRefType cfg_pt = Fee_Globals_t.CurrentProcessData.BlockConfig_pt;
    Fee_Bool retVal = FALSE;

    if((Fee_ChunkInfo_isDataChunk(chunk_pt)) && (cfg_pt != NULL_PTR)) /* SBSW_FEE_ChunkSearchContext */
    {
        retVal = (Fee_ChunkInfo_getPayload(chunk_pt) != cfg_pt->BlockPayloadSize_u16); /* SBSW_FEE_ChunkSearchContext */
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_QryLocalJobResultEmptyChunks
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultEmptyChunks(void)
{
    return (
     (Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS == Fee_Globals_t.FeeLocalJobResult_t);
}

/**********************************************************************************************************************
 * Fee_QryLocalJobResultFailed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultFailed(void)
{
    return (
     (Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_FAILED == Fee_Globals_t.FeeLocalJobResult_t);
}

/**********************************************************************************************************************
 * Fee_QryLocalJobResultInvalid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultInvalid(void)
{
    return ((Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_INVALID == Fee_Globals_t.FeeLocalJobResult_t);
}

/**********************************************************************************************************************
 * Fee_QryLocalJobResultOk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryLocalJobResultOk(void)
{
    return ((Fee_LocalJobResultType)FEE_LOCAL_JOB_RESULT_OK == Fee_Globals_t.FeeLocalJobResult_t);
}

/**********************************************************************************************************************
 * Fee_QryMayFollowLinks
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryMayFollowLinks(void) {
    return (Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t.chunkHoppingForced == FALSE);
}

/**********************************************************************************************************************
 * Fee_QryPreviousInstanceAvailable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryPreviousInstanceAvailable(void)
{
    return (Fee_Globals_t.FsmLocalVars_t.instanceSearchContext_t.instDescr_pt->instance_t.index_t > 0u);
}

/**********************************************************************************************************************
 * Fee_QryRecentChunkLinkConsistent
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryRecentChunkLinkConsistent(void)
{
    const Fee_ChunkSearchContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.chunkSearchContext_t;
    return (Fee_ChunkInfo_isLinkConsistent(&context_pt->mostRecentChunk_t)); /* SBSW_FEE_Func_passParamByReference */
}

/**********************************************************************************************************************
 * Fee_QrySearchingInCurrentSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QrySearchingInCurrentSector(void)
{
    const Fee_SectorRefType sector_ptloc = Fee_ChunkInfo_GetSector(&Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */

    return (Fee_Proc_isNewestSector(Fee_Globals_t.CurrentProcessData.Partition_pt, sector_ptloc)); /* SBSW_FEE_InitializedProcessData */
}

/**********************************************************************************************************************
 * Fee_QryTrue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_QryTrue(void)
{
    return (TRUE);
}

/**********************************************************************************************************************
 * Fee_InternalFillInstanceBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalFillInstanceBuffer(
    const Fee_IntBuffer_u8PtrType instBuf, Fee_UserDataConstPtrType userBuf, uint32 length)
{
    uint16_least loopCount = 0;

    for(; loopCount < length; loopCount++)
    {
        instBuf[loopCount] = userBuf[loopCount]; /* SBSW_FEE_InternalDataBuffer_writeAccess */
    }
}

/**********************************************************************************************************************
 * Fee_InternalEvalInstHdrStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_InstanceStatusType) Fee_InternalEvalInstHdrStatus(Fee_InstanceSearchContextConstRefType context_pt, MemIf_JobResultType flsResult)
{
    const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(&context_pt->instDescr_pt->chunk_t); /* SBSW_FEE_InstanceSearchContext */

    /* PRQA S 310 2 */ /* MD_FEE_11.4_pointer_cast */
    Fee_InstanceStatusType retVal = Fee_InternalEvaluateInstanceStatus(sector, /* SBSW_FEE_Chunks_Sector_ref */
                                                                       (Fee_IntBuffer_u8ConstPtrType)context_pt->Buffer_au32, /* SBSW_FEE_ConstPointer_refParam */
                                                                       flsResult);
    if(retVal == FEE_INSTANCE_STATUS_EMPTY)
    {
        sint32 index = (sint32)((Fee_Sector_alignWrite(sector, FEE_INSTANCE_HEADER_SIZE) - 1u) >> 2); /* SBSW_FEE_Chunks_Sector_ref */
        
        do
        {
            if(Fee_Sector_getEraseValueU32(sector) != context_pt->Buffer_au32[index]) /* SBSW_FEE_Chunks_Sector_ref */
            {
                retVal = FEE_INSTANCE_STATUS_FAILED;
                break;
            }
            index--;
        }
        while(index >= 0);
    }
    return retVal;
}

/**********************************************************************************************************************
 * Fee_InternalEvaluateInstanceStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_InstanceStatusType) Fee_InternalEvaluateInstanceStatus(
                                                Fee_SectorConstRefType sector_pt,
                                                Fee_IntBuffer_u8ConstPtrType intBuf_au8, MemIf_JobResultType flsResult)
{
    Fee_InstanceStatusType retVal = FEE_INSTANCE_STATUS_FAILED;

    if(MEMIF_JOB_FAILED != flsResult)
    {
        if((uint8)Fee_Sector_getEraseValueU32(sector_pt) == intBuf_au8[0]) /* SBSW_FEE_ConstPointer_refParam */
        {
            retVal = FEE_INSTANCE_STATUS_EMPTY;
        }
        else
        {
            switch(intBuf_au8[0])
            {
                case FEE_INSTANCE_STATUS_INVALID:
                    retVal = FEE_INSTANCE_STATUS_INVALID;
                    break;
                case FEE_INSTANCE_STATUS_OK:
                    retVal = FEE_INSTANCE_STATUS_OK;
                    break;
                case FEE_INSTANCE_STATUS_INCONSISTENT: /* explicitly marked as INCONSISTENT */
                    retVal = FEE_INSTANCE_STATUS_INCONSISTENT;
                    break;
                default:
                    break; /* any other value means FAILED */
            }
        }
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_InternalScheduleProcess
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalScheduleProcess(Fee_GlobalsPtrType g)
{
    Fee_Bool processStarted = FEE_FALSE;

    const sint8 ssPartIndex = g->SectorSwitchData_t.partitionId_s8;
    const Fee_RequestedJobType reqJob = g->JobParams.requestedJob;

    /*Check that force sector switch flag is not set too, because a forced sector switch would have a higher priority*/
    if((reqJob != FEE_JOB_NONE)
#if(FEE_FORCE_SECTOR_SWITCH_API == STD_ON) /* ECO_IGNORE_BLOCK_BEGIN */
            &&        (!g->InternalFlags_t.forceSectorSwitch)
#endif /* ECO_IGNORE_BLOCK_END */
      )
    {
        const sint8 jobPartIndex = g->JobParams.partitionId;
        const Fee_PartitionRefType jobPartition_pt = Fee_Partition_getFromConfigById(g->Config_pt, jobPartIndex); /* SBSW_FEE_GlobalConfigPointer */

        if((reqJob <= FEE_JOB_WRITE) &&
          (g->InternalFlags_t.jobDeferred)) /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
        {

            /*Job has already been started before (otherwise it could have never been deferred*/
            /*Hence Partition does not have to be initialized*/
            processStarted = Fee_InternalInitSectorSwitchFsm(g, jobPartition_pt); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_PartitionConfig */
        }

        if(!processStarted)
        {
            processStarted = Fee_InternalInitBusyFsm(g, jobPartition_pt); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_PartitionConfig */
        }

    }
    else if((ssPartIndex >= 0) && (g->ApiFlags.enableBss == TRUE))
    {
        const Fee_PartitionRefType ssPart_pt = Fee_Partition_getFromConfigById(g->Config_pt, (sint8_least)ssPartIndex); /* SBSW_FEE_GlobalConfigPointer */

        processStarted = FEE_TRUE;

        /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
        if(!Fee_InternalInitSectorSwitchFsm(g, ssPart_pt)) /* SBSW_FEE_PointerWrite_refParam */
        {
            g->SectorSwitchData_t.partitionId_s8--;  /* SBSW_FEE_PointerWrite_refParam */
            Fee_SignalReEnterMainFunction();
        }
    } 
    else
    {
#if(FEE_FORCE_SECTOR_SWITCH_API == STD_ON)
        g->InternalFlags_t.forceSectorSwitch = FALSE; /* SBSW_FEE_PointerWrite_refParam */
#endif
    }

    return processStarted;
}

/**********************************************************************************************************************
 * Fee_InternalInitPartitionInitializationFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
 FEE_LOCAL_FUNC(void) Fee_InternalInitPartitionInitializationFsm(Fee_GlobalsPtrType g,
                                                                                  Fee_PartitionRefType part)
 {
        const Fee_PartitionInitContextRefType context_pt = &g->FsmLocalVars_t.PartitionInitContext_t;
        const Fee_SectorConstRefType sector1 = Fee_Partition_getSectorByPos(part, FEE_LOWER_SECTOR_NUMBER); /* SBSW_FEE_PointerWrite_refParam */

        g->CurrentProcessData.Partition_pt = part; /* SBSW_FEE_PointerWrite_refParam */

        Fee_FsmInitSubFsm(&g->FsmStack_t, FEE_STATE_STARTUP_READ_SECTOR_HEADER_LOWER); /* SBSW_FEE_Func_passParamByReference */

        Fee_InternalEvalFlsRetVal(Fee_Proc_readSectorHeader(sector1, context_pt->sectorHeaderData)); /* SBSW_FEE_PartitionConfig */
 }
 
/**********************************************************************************************************************
 * Fee_Initialize_ProcessData
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
FEE_LOCAL_FUNC(void) Fee_Initialize_ProcessData(Fee_ProcessDataRefType self, Fee_PartitionRefType part, struct Fee_tagBlockCfgDatasetPair block)
{
    self->BlockConfig_pt = block.config;                   /* SBSW_FEE_PointerWrite_refParam */
    self->BlockId.Tag = block.config->LinktableIndex_u16;  /* SBSW_FEE_PointerWrite_refParam */
    self->BlockId.DataIndex = block.dataIndex;             /* SBSW_FEE_PointerWrite_refParam */
    self->Partition_pt = part;                             /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_InternalInitBusyFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalInitBusyFsm(Fee_GlobalsPtrType globals,
                                                               Fee_PartitionRefType jobPartition_pt)
{
    /* This call makes CurrentProcessData (it's members) valid! */
    Fee_Initialize_ProcessData(&globals->CurrentProcessData, jobPartition_pt, globals->JobParams.params.blockJob.blockRef); /* SBSW_FEE_Func_passParamByReference */

    globals->InternalFlags_t.jobDeferred = FALSE; /* SBSW_FEE_PointerWrite_refParam */

    globals->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_PointerWrite_refParam */

    Fee_ChunkInfo_Clear(&globals->CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */
    Fee_ChunkInfo_Clear(&globals->PreviousChunk_t);          /* SBSW_FEE_Func_passParamByReference */

    Fee_FsmSetupTopLevel(&globals->FsmStack_t, FEE_STATE_MAIN_PREPARE_JOB); /* SBSW_FEE_Func_passParamByReference */

    if(Fee_Partition_getNewestSector(jobPartition_pt) == NULL_PTR) /* SBSW_FEE_PointerWrite_refParam */
    {
        Fee_InternalInitPartitionInitializationFsm(globals, jobPartition_pt); /* SBSW_FEE_PointerWrite_refParam */
    }

    return TRUE;
}

/**********************************************************************************************************************
 * Fee_ActSetupJobProcessing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ActSetupJobProcessing(void) {
    const Fee_GlobalsPtrType globals = &Fee_Globals_t;

    const Fee_PartitionRefType jobPartition_pt = globals->CurrentProcessData.Partition_pt;

    const Fee_SectorRefType newSector = Fee_Partition_getNewestSector(jobPartition_pt); /* SBSW_FEE_InitializedProcessData */

    /*Will be overwritten if job is actually executed*/
    globals->FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_FAILED; /* SBSW_FEE_Func_writeObjectByReference */

    Fee_ChunkInfo_Construct(&globals->CurrentDataset_t.chunk_t, newSector); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_Partitions_OldestNewestSectorRef */

    /*Necessary, because only the block with the broken link should be written in the new sector */
    globals->InternalFlags_t.brokenLink = FALSE; /* SBSW_FEE_Func_writeObjectByReference */

    switch(globals->JobParams.requestedJob)
    {
        case FEE_JOB_WRITE:
        case FEE_JOB_ERASE_IMMEDIATE_BLOCK:
        case FEE_JOB_INVALIDATE_BLOCK:
            if(Fee_Partition_isWriteable(jobPartition_pt)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_InitializedProcessData */
            {
                Fee_ChunkSearchContext_Init(&globals->FsmLocalVars_t.chunkSearchContext_t, /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentProcessData.BlockId,          /* SBSW_FEE_Func_passParamByReference */
                                            &globals->PreviousChunk_t,                     /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentDataset_t,                    /* SBSW_FEE_Func_passParamByReference */
                                            Fee_Proc_getSectorEndOffset(newSector));       /* SBSW_FEE_Partitions_OldestNewestSectorRef */

                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_BUSY_SEARCH_WRITTEN); /* SBSW_FEE_Func_passParamByReference */
                Fee_ActInitSearchWrittenFsm();
            }
            break;
#if (FEE_DATA_CONVERSION_API == STD_ON) /* ECO_DISABLE_GENERATION */
        case FEE_JOB_CONVERT_DATA_BLOCKS:
            Fee_InternalInitDataConversionFsm(globals);/* SBSW_FEE_Func_passParamByReference */
            break;
#endif
#if(FEE_GET_WRITE_CYCLE_API == STD_ON) /* ECO_DISABLE_GENERATION */
        case FEE_JOB_GET_WRITE_CYCLE:
#if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
            /* for coverage purposes: disable code that is not used. */
            if(Fee_Partition_isReadable(jobPartition_pt) == FEE_TRUE) /* SBSW_FEE_InitializedProcessData */
#endif
            {
                Fee_ChunkSearchContext_Init(&globals->FsmLocalVars_t.chunkSearchContext_t, /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentProcessData.BlockId,          /* SBSW_FEE_Func_passParamByReference */
                                            &globals->PreviousChunk_t,                     /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentDataset_t,                    /* SBSW_FEE_Func_passParamByReference */
                                            Fee_Proc_getSectorEndOffset(newSector));       /* SBSW_FEE_Partitions_OldestNewestSectorRef */
                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_BUSY_SEARCH_WCC);        /* SBSW_FEE_Func_passParamByReference */
                Fee_ActInitSearchWrittenFsm();
            }
            break;
#endif
        case FEE_JOB_READ:
#if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
            /* for coverage purposes: disable code that is not used. */

            if(Fee_Partition_isReadable(jobPartition_pt) == FEE_TRUE) /* SBSW_FEE_InitializedProcessData */
#endif
            {
                Fee_ChunkSearchContext_Init(&globals->FsmLocalVars_t.chunkSearchContext_t, /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentProcessData.BlockId,          /* SBSW_FEE_Func_passParamByReference */
                                            &globals->PreviousChunk_t,                     /* SBSW_FEE_Func_passParamByReference */
                                            &globals->CurrentDataset_t,                    /* SBSW_FEE_Func_passParamByReference */
                                            Fee_Proc_getSectorEndOffset(newSector));       /* SBSW_FEE_Partitions_OldestNewestSectorRef */
                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_BUSY_SEARCH_CONSISTENT);            /* SBSW_FEE_Func_passParamByReference */
                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_1);  /* SBSW_FEE_Func_passParamByReference */
                Fee_ActInitChunkSearchFsm();
            }
            break;

#if (STD_ON == FEE_GET_ERASE_CYCLE_API)  /* ECO_IGNORE_BLOCK_BEGIN : define needs no test, because it contains an else condition. */
        case FEE_JOB_GET_ERASE_CYCLE:
            Fee_IntFinalizeEraseCycleJob(globals->JobParams.params.eraseCc.resultBuffer, jobPartition_pt, /* SBSW_FEE_InitializedProcessData */
                    (uint8)(globals->JobParams.params.eraseCc.sectorIdent.sectorIdx));
            break;
#endif
        default: /* COV_FEE_DEFAULT_CASE */
            break;
    }
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * Fee_InternalInitSectorSwitchFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_Bool) Fee_InternalInitSectorSwitchFsm(Fee_GlobalsPtrType globals, Fee_PartitionRefType partition)
{
    /* just create local variables, to avoid repeated calls, and to have shorter notations later on */
    const enum Fee_Partition_FillLevelEnum fillLevel = Fee_Partition_getFillLevel(partition); /* SBSW_FEE_PointerWrite_refParam */
    const Fee_Bool ssBlocked = ((fillLevel >= FEE_PART_FILL_LEVEL_FSS) && (globals->ApiFlags.enableFss == FALSE));
    Fee_Bool processStarted = FALSE;

    Fee_FsmSetupTopLevel(&globals->FsmStack_t, FEE_STATE_MAIN_SECTOR_SWITCH); /* SBSW_FEE_Func_passParamByReference */

    /* updates CurrentProcess Data (only partition) -> during whole sector switch processing, it remains VALID
     * Block parameters will be updated later on, when it became clear which block to process next. */
    globals->CurrentProcessData.Partition_pt = partition; /* SBSW_FEE_PointerWrite_refParam */

    if(fillLevel > FEE_PART_FILL_LEVEL_UNKNOWN) /* COV_FEE_PART_CONFIG */
    {
        if((Fee_Partition_isWriteable(partition)) && (ssBlocked == FEE_FALSE)) /* SBSW_FEE_ConstPointer_refParam */
        {
            const Fee_SectorConstRefType olderSector = Fee_Partition_getOldestSector(partition); /* SBSW_FEE_Partitions_OldestNewestSectorRef */

            if((!Fee_Sector_isUsable(olderSector))) /* SBSW_FEE_ConstPointer_refParam */
            {
                processStarted = TRUE;

                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_SECTOR_SWITCH_SETUP_SECTORS); /* SBSW_FEE_Func_passParamByReference */
                Fee_ActInitSetupSectorsFsm();
            }
            /* check whether Block Copy is necessary (erase is not necessary):
               - Any threshold exceeded (actual BSS handling is done outside)
               - A job was deferred (actually it forces a sector switch).
               - Force SectorSwitch                                                */

            /* Here we can be sure that: Partition is initialized (outermost condition) and we have two valid sectors
             * Old is usable; implies new is usable too */
            else if((Fee_Partition_requiresSectorSwitch(partition) == FEE_TRUE) /* SBSW_FEE_ConstPointer_refParam */
                    || (fillLevel >= FEE_PART_FILL_LEVEL_BSS))
            {
                processStarted = TRUE;

                Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_SECTOR_SWITCH_GET_BLOCK_1); /* SBSW_FEE_Func_passParamByReference */
                Fee_InternalGetNextBlockToCopy(globals); /* SBSW_FEE_PointerWrite_refParam */
            }
            else
            {
                /* nothing to do */
            }
        }
    }
#if((FEE_FORCE_SECTOR_SWITCH_API == STD_ON) || (FEE_DATA_CONVERSION_API == STD_ON))
    else if(globals->InternalFlags_t.forceSectorSwitch == FEE_TRUE) /* COV_FEE_PART_CONFIG */
    {
        Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_FORCE_SS_PART_INIT); /* SBSW_FEE_Func_passParamByReference */
        Fee_InternalInitPartitionInitializationFsm(globals, partition); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_PointerWrite_refParam */
        processStarted = TRUE;
    }
#endif
    else
    {
            /* nothing to do */;
    }

    return processStarted;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

#if (FEE_DATA_CONVERSION_API == STD_ON)

/**********************************************************************************************************************
 * Fee_ActGetNextBlockToCopy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActGetNextBlockToCopy(void)
    {
        Fee_InternalGetNextBlockToCopy(&Fee_Globals_t); /* SBSW_FEE_Func_passParamByReference */
    }

/**********************************************************************************************************************
 * Fee_ActPrepareReadInstanceData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActPrepareReadInstanceData(void)
    {
        Fee_InternalEvalFlsRetVal(Fee_ChunkInfo_ReadInstancePayload(
            &Fee_Globals_t.CurrentDataset_t.chunk_t,                    /* SBSW_FEE_Func_passParamByReference */
            Fee_Globals_t.CurrentDataset_t.instance_t.index_t,
            Fee_Globals_t.JobParams.params.conv.userBuffer,
            0, Fee_ChunkInfo_getPayload(&Fee_Globals_t.CurrentDataset_t.chunk_t))); /* SBSW_FEE_Func_passParamByReference */
    }

/**********************************************************************************************************************
 * Fee_GetBlockCfgByBlockId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
    FEE_LOCAL_FUNC(Fee_BlockConfigRefType) Fee_GetBlockCfgByBlockId(Fee_ConfigRefTypeInternal cfg,
                                                                    struct Fee_BlockIdStruct blockId, uint8 partIndex)
    {
        Fee_BlockConfigRefType retVal = NULL_PTR;
        sint32 cfgIndex = 0;
        
        for(; cfgIndex < (sint32)(cfg->Fee_NumberOfBlocks_u16); cfgIndex++)
        {
            if((partIndex == cfg->BlockConfig_at[cfgIndex].PartitionIndex) &&
                (blockId.Tag == cfg->BlockConfig_at[cfgIndex].LinktableIndex_u16)) /* COV_FEE_PART_CONFIG */
            {
                retVal = &cfg->BlockConfig_at[cfgIndex];
                break;
            }
        }

        return retVal;
    }

/**********************************************************************************************************************
 * Fee_ActInitDCWriteProcSubFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitDCWriteProcSubFsm(void)
    {
        const Fee_BitFieldType blockTag = Fee_Globals_t.CurrentProcessData.BlockId.Tag;
        const uint8 partIndex = Fee_Partition_getId(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */
        Fee_BlockConfigRefType cfg =
                Fee_GetBlockCfgByBlockId(Fee_Globals_t.Config_pt, Fee_Globals_t.CurrentProcessData.BlockId, partIndex); /* SBSW_FEE_GlobalConfigPointer */

        uint16 newPayload = 0;
        uint8 conversionCbkRetVal;

        if((cfg != NULL_PTR) &&
           (Fee_Globals_t.CurrentProcessData.BlockId.DataIndex < cfg->NumberOfDatasets_u8))

        {
            newPayload = cfg->BlockPayloadSize_u16;
        }

        conversionCbkRetVal =
                Fee_Globals_t.JobParams.params.conv.notificationPtr( /* SBSW_FEE_Conversion_OptionsPointer */
                            Fee_Globals_t.JobParams.params.conv.userBuffer, /* SBSW_FEE_Conversion_OptionsPointer */
                            (((uint32)partIndex << 24) |
                            ((uint32)blockTag << 8) | (uint32)Fee_Globals_t.CurrentProcessData.BlockId.DataIndex),
                            Fee_ChunkInfo_getPayload(&Fee_Globals_t.CurrentDataset_t.chunk_t), /* SBSW_FEE_ConstPointer_refParam */
                            newPayload);

        switch(conversionCbkRetVal)
        {
            case FEE_CONVERSION_WRITE_OLD_LENGTH:
                newPayload = Fee_ChunkInfo_getPayload(&Fee_Globals_t.CurrentDataset_t.chunk_t); /* SBSW_FEE_Func_passParamByReference */
                cfg = NULL_PTR;
                break;
            case FEE_CONVERSION_WRITE_NEW_LENGTH:
                break;
            default:
                newPayload = 0;
        }

        Fee_InstanceDescr_Init(&Fee_Globals_t.SectorSwitchTarget_t.instance_t, 0, FEE_INSTANCE_STATUS_EMPTY); /* SBSW_FEE_Func_passParamByReference */

        Fee_Proc_CreateChunkAllocContext(&Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t, &Fee_Globals_t.SectorSwitchTarget_t, /* SBSW_FEE_Func_passParamByReference */
                Fee_Globals_t.CurrentProcessData.BlockId, cfg);

#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
        Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t.wccChunk_pt = &Fee_Globals_t.CurrentDataset_t;
#endif

        if(newPayload == 0)
        {
            Fee_Globals_t.FeeLocalJobResult_t = FEE_LOCAL_JOB_RESULT_OK;
        }
        else
        {

            Fee_ChunkInfo_CreateNewDataChunk(&Fee_Globals_t.PreviousChunk_t, newPayload, 1); /* SBSW_FEE_Func_passParamByReference */

            Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_DC_LINKCHUNK_ALLOC_FSM); /* SBSW_FEE_Func_passParamByReference */
            Fee_InternalInitLinkTableAllocationFsm(&Fee_Globals_t);                         /* SBSW_FEE_Func_passParamByReference */
        }
    } /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

 /**********************************************************************************************************************
 * Fee_ActInitInstanceWriteDcFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitInstanceWriteDcFsm(void)
    {
        const Fee_InstanceWriteContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.instanceWriteContext_t;

        const Fee_InstanceDescriptorRefType instDescr_ptloc = &Fee_Globals_t.SectorSwitchTarget_t;

        context_pt->sector_pt = Fee_ChunkInfo_GetSector(&instDescr_ptloc->chunk_t); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_Func_passParamByReference */
        context_pt->ByteCount_u32 = Fee_ChunkInfo_GetInstanceSize(&instDescr_ptloc->chunk_t); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_Func_passParamByReference */
        context_pt->targetOffset_t = Fee_ChunkInfo_GetInstanceStartAddress(&instDescr_ptloc->chunk_t, instDescr_ptloc->instance_t.index_t); /* SBSW_FEE_Func_writeObjectByReference */ /* SBSW_FEE_Func_passParamByReference */
        context_pt->userData_pt = Fee_Globals_t.JobParams.params.conv.userBuffer; /* SBSW_FEE_Func_writeObjectByReference */

        Fee_FsmInitSubFsm(&Fee_Globals_t.FsmStack_t, FEE_STATE_CREATE_INSTANCE_3_REMAINING_WRITE_STREAMS); /* SBSW_FEE_Func_passParamByReference */
        Fee_Proc_SetupWriteSuspendHandling(&Fee_Globals_t, Fee_InternalDoWriteStreamFirst); /* SBSW_FEE_Func_passParamByReference */
    }

/**********************************************************************************************************************
 * Fee_ActFinalizeDataConversion
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActFinalizeDataConversion(void)
    {
        uint8 pIndex = Fee_Partition_getId(Fee_Globals_t.CurrentProcessData.Partition_pt); /* SBSW_FEE_InitializedProcessData */

        if(pIndex == 0) /* COV_FEE_PART_CONFIG */
        {
            Fee_GlobalSuspend();
                Fee_Globals_t.ApiFlags.forceFblMode = FALSE;
            Fee_GlobalRestore();

            Fee_ActReturnFromSubFsm();
        }
        else
        {
            Fee_Globals_t.CurrentProcessData.Partition_pt = /* SBSW_FEE_GlobalConfigPointer */
                    Fee_Partition_getFromConfigById(Fee_Globals_t.Config_pt, (sint8_least)(pIndex - 1)); /*  */
        
            Fee_ChunkInfo_Clear(&Fee_Globals_t.SectorSwitchData_t.mode.fbl); /* SBSW_FEE_Func_passParamByReference */
            Fee_InternalGetNextBlockToCopy(&Fee_Globals_t);                  /* SBSW_FEE_Func_passParamByReference */
        }
    }

/**********************************************************************************************************************
 * Fee_ActInitChunkAllocFsmDc
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_ActInitChunkAllocFsmDc(void)
    {
        const Fee_ChunkAllocContextRefType context_pt = &Fee_Globals_t.FsmLocalVars_t.ChunkAllocContext_t;

        Fee_InternalInitAllocateChunkFsm(context_pt, &Fee_Globals_t.PreviousChunk_t); /* SBSW_FEE_Func_passParamByReference */
    }

/**********************************************************************************************************************
 * Fee_InternalInitDataConversionFsm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
    FEE_LOCAL_FUNC(void) Fee_InternalInitDataConversionFsm(Fee_GlobalsPtrType globals)
    {
        /* Request also initiated ForceSectorSwitch, which must have been completed, when this point is reached  *
           => ForceSectorSwitch always runs in foreground                                                        */

        globals->CurrentProcessData.Partition_pt = /* SBSW_FEE_PointerWrite_refParam */
                    Fee_Partition_getFromConfigById(globals->Config_pt, (sint8_least)(globals->Config_pt->Fee_NumberOfPartitions_u8 - 1u)); /* SBSW_FEE_GlobalConfigPointer */

        Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_DC_GET_NEXT_BLOCK); /* SBSW_FEE_Func_passParamByReference */

        Fee_ChunkInfo_Clear(&globals->SectorSwitchData_t.mode.fbl); /* SBSW_FEE_Func_passParamByReference */
        Fee_InternalGetNextBlockToCopy(globals); /* SBSW_FEE_Func_writeObjectByReference */
    }
#endif

/**********************************************************************************************************************
 * Fee_Proc_SetupWriteSuspendHandling
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_Proc_SetupWriteSuspendHandling(
                                                 Fee_GlobalsPtrType globals,
                                                 Fee_DeferredFuncType deferredRequest)
{
    Fee_FsmInitSubFsm(&globals->FsmStack_t, FEE_STATE_WAIT_FOR_WRITE_RESUME); /* SBSW_FEE_Func_passParamByReference */
    globals->DeferredWriteAction_pt = deferredRequest; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkSearchContext_init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_ChunkSearchContext_Init(Fee_ChunkSearchContextRefType self,
                                                                 Fee_BlockIdConstRefType blockId_pt,
                                                                 Fee_ChunkInfoRefType prevChunk_pt,
                                                                 Fee_InstanceDescriptorRefType recentChunk_pt,
                                                                 Fee_LengthType upperLimit_t)
{
    struct Fee_ChunkSearchContextStruct temp = {0};

    temp.super_t.searchBlockId_t = *blockId_pt;
    
    temp.super_t.blockIdMask_t.Tag = 0xFFFFu;
    temp.super_t.blockIdMask_t.DataIndex = 0xFFu;
    
    temp.super_t.resultBlockId_pt = NULL_PTR;

    temp.super_t.upperSearchLimit_t = upperLimit_t;

    temp.recentChunk_pt = recentChunk_pt;
    temp.prevChunk_pt = prevChunk_pt;

    Fee_ChunkInfo_Clone(&temp.mostRecentChunk_t, &recentChunk_pt->chunk_t); /* SBSW_FEE_Func_passParamByReference */

    *self = temp; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_InternalRequestSectorSwitch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalRequestSectorSwitch(Fee_GlobalsPtrType g, Fee_PartitionConstRefType partition)
{
    const sint8 pId = (sint8)Fee_Partition_getId(partition); /* SBSW_FEE_InitializedProcessData */

    if(pId != g->SectorSwitchData_t.partitionId_s8)
    {

        if(pId > g->SectorSwitchData_t.partitionId_s8) /* COV_FEE_PART_CONFIG */
        {
            g->SectorSwitchData_t.partitionId_s8 = pId; /* SBSW_FEE_PointerWrite_refParam */
        }

        Fee_InternalResetSectorSwitch(&g->SectorSwitchData_t, Fee_InternalIsInFblMode(g)); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_InternalResetSectorSwitch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(void) Fee_InternalResetSectorSwitch(Fee_SectorSwitchDataRefType self, Fee_Bool isFblMode)
{
    if(isFblMode) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        Fee_ChunkInfo_Clear(&self->mode.fbl); /* SBSW_FEE_Func_passParamByReference */
    }
    else
    {
        self->mode.appl.DataIndex_s16 = -1; /* SBSW_FEE_PointerWrite_refParam */
        self->mode.appl.BlockConfigIndex_u16 = 0xFFFF; /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_SignalNeedProcessing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_SignalNeedProcessing(void)
{
    if(Fee_Globals_t.ApiFlags.needProcessing == FALSE)
    {
        Fee_GlobalSuspend();
        Fee_Globals_t.ApiFlags.needProcessing = TRUE;
        Fee_GlobalRestore();
    }
}

/**********************************************************************************************************************
 * Fee_InternalIsInFblMode
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_InternalIsInFblMode(Fee_GlobalsConstPtrType g)
{
#  if (FEE_DATA_CONVERSION_API == STD_ON)
    return ((g->Config_pt->Fee_ConfigurationIncomplete == FEE_TRUE) || (g->ApiFlags.forceFblMode == FEE_TRUE));
#  else
    return (g->Config_pt->Fee_ConfigurationIncomplete == FEE_TRUE);
#  endif
}

/**********************************************************************************************************************
 * Fee_Proc_readSectorHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(enum Fee_FlsRequestStatus) Fee_Proc_readSectorHeader(Fee_SectorConstRefType sector, Fee_IntBuffer_u32PtrType buf)
{
    return Fee_Sector_readMngmtData(sector, 0, buf, FEE_SECTOR_HEADER_SIZE); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_writeSectorHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(enum Fee_FlsRequestStatus) Fee_Proc_writeSectorHeader(Fee_SectorConstRefType sector, Fee_IntBuffer_u32ConstPtrType buf)
{
    return Fee_Sector_writeMngmtData(sector, 0, (buf), FEE_SECTOR_HEADER_SIZE); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_getSectorOffset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_Proc_getSectorEndOffset(Fee_SectorConstRefType sector)
{
    return Fee_Sector_getSize(sector) - 1u; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_isNewestSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_isNewestSector(Fee_PartitionConstRefType part, Fee_SectorConstRefType sector)
{
    return (Fee_Partition_getNewestSector(part) == sector); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_doesChunkEqualConfig
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_doesChunkEqualConfig(Fee_ChunkInfoConstRefType chunk, Fee_BlockConfigRefType config_pt)
{
    return (Fee_ChunkInfo_getPayload(chunk) == config_pt->BlockPayloadSize_u16); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Proc_isLinkTableChunkAllowed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Proc_isLinkTableChunkAllowed(Fee_ChunkInfoConstRefType chunk)
{
    /* PRQA S 3344,3415 1 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
    return ((Fee_ChunkInfo_isInitialLinkTable(chunk)) /* SBSW_FEE_PointerWrite_refParam */
            && (Fee_ChunkInfo_isLinkAllowed(chunk))); /* SBSW_FEE_PointerWrite_refParam */
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Processing.c
 *********************************************************************************************************************/
