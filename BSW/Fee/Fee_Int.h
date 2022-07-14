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
#ifndef FEE_INT_H
# define FEE_INT_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_INTERNAL_MAJOR_VERSION                                   (8u)
# define FEE_INTERNAL_MINOR_VERSION                                   (1u)
# define FEE_INTERNAL_PATCH_VERSION                                   (5u)

#include "Fee_Types.h"
#include "Fee_PrivateCfg.h"
#include "Fee_IntBase.h"
#include "Fee_Cfg.h"
#include "Fee_Sector.h"
#include "Fee_Partition.h"
#include "Fee_ChunkInfo.h"
#include "Fee_ChunkInfoDefs.h"
#include "Fee_JobParams.h"

/******************************************************************************
 * GENERAL CONFIGURATION PARAMETER
 *****************************************************************************/
# if (STD_ON == FEE_FORCE_SECTOR_SWITCH_API)
/* Define the depth of the FSM stack. 
 * Determined from design: largest depth is reached when the Fee performs ChunkHopping during a read/write job:
 *   1. Main FSM
 *   2. SectorSwitchForced FSM
 *   3. SectorSwitch FSM
 *   4. SearchConsistent FSM
 *   5. ChunkSearch FSM
 *   6. ChunkHopping FSM
 */
#  define FEE_FSM_STACK_SIZE                                           (6u)
# else
/* Define the depth of the FSM stack. 
 * Determined from design: largest depth is reached when the Fee performs ChunkHopping during a read/write job:
 *   1. Main FSM
 *   2. Busy FSM
 *   3. SearchConsistent FSM
 *   4. ChunkSearch FSM
 *   5. ChunkHopping FSM
 */
#  define FEE_FSM_STACK_SIZE                                           (5u)
# endif

/**************************************************************************
 *  GENERAL TYPES
 *************************************************************************/
typedef uint8 Fee_FlagType;

/**************************************************************************
 *  STATES
 *************************************************************************/
/* PRQA S 0779 state_enumerators  *//* MD_FEE_5.1_identifiers */
enum Fee_StateEnumType
{    /* Top-level States */
    FEE_STATE_IDLE = 0, /*<! IDLE is a generic state; it may be entered in all FSM stack levels */

    /* internal processing */
    FEE_STATE_MAIN_SECTOR_SWITCH,

    FEE_STATE_MAIN_PREPARE_JOB,
    /* job processing */
    FEE_STATE_MAIN_BUSY,
    /* Lower Level States */

#if((FEE_FORCE_SECTOR_SWITCH_API == STD_ON) || (FEE_DATA_CONVERSION_API == STD_ON))
    FEE_STATE_FORCE_SS_PART_INIT,
#endif

    FEE_STATE_ALLOCATE_CHUNK_CHUNK_HOPPING_FSM,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_STATE_ALLOCATE_CHUNK_GET_WCC,
#endif
    FEE_STATE_ALLOCATE_CHUNK_WRITE_CHUNK_HEADER,
    FEE_STATE_ALLOCATE_CHUNK_WRITE_LINK,
    FEE_STATE_BUSY_PROCESS_PAYLOAD_DATA,
#if (STD_ON == FEE_GET_WRITE_CYCLE_API)
    FEE_STATE_BUSY_READ_CHUNK_HEADER,
    FEE_STATE_BUSY_SEARCH_WCC,
#endif
    FEE_STATE_BUSY_SEARCH_CONSISTENT,
    FEE_STATE_BUSY_SEARCH_WRITTEN,
    FEE_STATE_BUSY_WRITE_PROCESSING_1,
    FEE_STATE_BUSY_WRITE_PROCESSING_2,
    FEE_STATE_CHUNK_CONSISTENCY_CHECK,
    FEE_STATE_CHUNK_LINK_PROCESSING,
    FEE_STATE_CHUNK_HOPPING_CHUNK_HEADER_SEARCH,
    FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_1,
    FEE_STATE_CHUNK_SEARCH_CHECK_RECENT_CHUNK_2,
    FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_1,
    FEE_STATE_CHUNK_SEARCH_CHUNK_HOPPING_2,
    FEE_STATE_CHUNK_SEARCH_FOLLOW_CHUNK_LINK,

    FEE_STATE_COPY_ALLOCATE_DATA_CHUNK,
    FEE_STATE_COPY_ALLOCATE_LINK_CHUNK,

    FEE_STATE_COPY_INSTANCE_COPY,
    FEE_STATE_CREATE_INSTANCE_1_REMAINING_WRITE_STREAM,
    FEE_STATE_CREATE_INSTANCE_2_REMAINING_WRITE_STREAMS,
    FEE_STATE_CREATE_INSTANCE_3_REMAINING_WRITE_STREAMS,
    FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_READ,
    FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_1,
    FEE_STATE_CREATE_INSTANCE_BLOCK_COPY_WRITE_2,
    FEE_STATE_GET_BLOCK_EVAL_HEADER,
    FEE_STATE_INSTANCE_EVALUATION_READ_TRAILER,
    FEE_STATE_INSTANCE_SEARCH_BINARY,
    FEE_STATE_INSTANCE_SEARCH_LINEAR,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_1,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_2,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_3,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_1,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_2,
    FEE_STATE_SEARCH_CONSISTENT_CHUNK_SEARCH_RCH_3,
    FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_1,
    FEE_STATE_SEARCH_CONSISTENT_INSTANCE_EVALUATION_2,
    FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_BINARY,
    FEE_STATE_SEARCH_CONSISTENT_INSTANCE_SEARCH_LINEARY,
    FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_1,
    FEE_STATE_SEARCH_UPWARDS_CHECK_CHUNK_2,
    FEE_STATE_SEARCH_UPWARDS_CHUNK_HOPPING,
    FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_1,
    FEE_STATE_SEARCH_UPWARDS_INSTANCE_EVALUATION_2,
    FEE_STATE_SEARCH_UPWARDS_READ_INSTANCE_HEADER,
    FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_1,
    FEE_STATE_SEARCH_UPWARDS_FOLLOW_CHUNK_LINK_2,
    FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_CURRENT,
    FEE_STATE_SEARCH_WRITTEN_CHUNK_SEARCH_OLD,
    FEE_STATE_SEARCH_WRITTEN_FIND_NFA,
    FEE_STATE_SEARCH_WRITTEN_INSTANCE_SEARCH_BINARY,
    FEE_STATE_SECTOR_SWITCH_ALLOCATE_LINKTABLE_CHUNK,
    FEE_STATE_SECTOR_SWITCH_COPY_PROCESSING,
    FEE_STATE_SECTOR_SWITCH_GET_BLOCK_1,
    FEE_STATE_SECTOR_SWITCH_GET_BLOCK_2,
    FEE_STATE_SECTOR_SWITCH_SEARCH_CONSISTENT,
    FEE_STATE_SECTOR_SWITCH_SEARCH_TARGET_CHUNK,
    FEE_STATE_SECTOR_SWITCH_SETUP_SECTORS,
    FEE_STATE_SETUP_SECTORS_ERASING,
    FEE_STATE_SETUP_SECTORS_HEADER_WRITING,
    FEE_STATE_STARTUP_READ_SECTOR_HEADER_LOWER,
    FEE_STATE_STARTUP_READ_SECTOR_HEADER_UPPER,
    FEE_STATE_WRITE_PROCESSING_ALLOC_DATACHUNK,
    FEE_STATE_WRITE_PROCESSING_ALLOC_LINKTABLE,
    FEE_STATE_WRITE_PROCESSING_CREATE_INSTANCE,

#if (FEE_DATA_CONVERSION_API == STD_ON)
    FEE_STATE_DC_GET_NEXT_BLOCK,
    FEE_STATE_DC_SEARCH_UPWARDS_FSM,
    FEE_STATE_DC_SEARCH_CONSISTENT_FSM,
    FEE_STATE_DC_GET_INSTANCE_DATA,
    FEE_STATE_DC_WRITE_PROCESSING_FSM,

    FEE_STATE_DC_LINKCHUNK_ALLOC_FSM,
    FEE_STATE_DC_DATA_CHUNK_ALLOC_FSM,
    FEE_STATE_DC_WRITE_PROC_WR_DATA,
    FEE_STATE_DC_SETUP_SECTORS,
#endif

    FEE_STATE_WAIT_FOR_WRITE_RESUME,

    FEE_STATE_NEVER_ENTERED /*<! shall always be the last value; shall not get an entry in FSM description table,
                                 used for compile-time checks */
};
/* PRQA L:state_enumerators */

/* Force implicit cast for enum Fee_StateIdType into uint8 when using Fee_StateQueryType as data type.
 * But only for production mode in order to see not only numbers when debugging.
 */
# if (STD_ON == FEE_DEV_ERROR_DETECT)
typedef enum Fee_StateEnumType Fee_StateType;
# else
typedef uint8 Fee_StateType;
# endif

/**************************************************************************
 *  Type to determine the LocalJobResult values, utilized by the Fee
 *************************************************************************/
enum Fee_LocalJobResultEnumType
{
    FEE_LOCAL_JOB_RESULT_OK = 0u,
    FEE_LOCAL_JOB_RESULT_FAILED,
    FEE_LOCAL_JOB_RESULT_EMPTY_CHUNKS,
    FEE_LOCAL_JOB_RESULT_INCONSISTENT,
    FEE_LOCAL_JOB_RESULT_INVALID,
    FEE_LOCAL_JOB_RESULT_BLOCK_COPY
};

/* Force implizit cast for enum Fee_LocalJobResultIdType into uint8. 
 * But only for production mode in order to see not only numbers 
 * when debugging.
 */
# if (STD_ON == FEE_DEV_ERROR_DETECT)
typedef enum Fee_LocalJobResultEnumType Fee_LocalJobResultType;
# else
typedef uint8 Fee_LocalJobResultType;
# endif

/**************************************************************************
 *  Type to determine the status of the rightest instance within a chunk
 *************************************************************************/
typedef enum
{
    FEE_INSTANCE_STATUS_OK = 0xAu,
    FEE_INSTANCE_STATUS_FAILED = 0x0u,
    FEE_INSTANCE_STATUS_EMPTY = 0x6u,
    FEE_INSTANCE_STATUS_INCONSISTENT = 0x9u,
    FEE_INSTANCE_STATUS_INVALID = 0x5u
} Fee_InstanceStatusType;

/**************************************************************************
 * Type for the FSM stack.
 *************************************************************************/

 /* Imagine the FSM stack as follows:
  * |--------------------|                  n
  * | FSM[n]             |                 / \
  * | ...                |                  |
  * | FSM[1]             | < TOS (example)  | index
  * | FSM[0] (Main FSM)  |                  |
  * |--------------------|                  0
 */
 struct Fee_FsmStackStruct
{
    Fee_StateType Stack_at[FEE_FSM_STACK_SIZE]; /*<! the actual stack */
    uint8 TopOfStack_u8;                        /*<! current top of stack value (TOS), [0..FSM_STACK_SIZE) */
};

/**************************************************************************
 * Global struct which holds all RAM variables.
 *************************************************************************/

typedef P2VAR(struct Fee_GlobalVariableStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_GlobalsPtrType;
typedef P2CONST(struct Fee_GlobalVariableStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_GlobalsConstPtrType;

typedef P2FUNC(void, FEE_PRIVATE_CODE, Fee_DeferredFuncType) (Fee_GlobalsPtrType);

typedef P2VAR(union Fee_InternalSharedDataUnion, AUTOMATIC, FEE_VAR_NOINIT) Fee_InternalSharedDataRefType;

typedef P2VAR(struct Fee_ChunkHoppingContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_ChunkHoppingContextRefType;

typedef P2VAR(struct Fee_InstanceSearchContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceSearchContextRefType;
typedef P2CONST(struct Fee_InstanceSearchContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceSearchContextConstRefType;
typedef P2VAR(struct Fee_InstanceWriteContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceWriteContextRefType;
typedef P2VAR(struct Fee_InstanceCopyContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceCopyContextRefType;

typedef P2VAR(struct Fee_ChunkSearchContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_ChunkSearchContextRefType;
typedef P2VAR(struct Fee_ChunkAllocContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_ChunkAllocContextRefType;
typedef P2VAR(struct Fee_PartitionInitContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_PartitionInitContextRefType;
typedef P2VAR(struct Fee_SectorFormatContextStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_SectorFormatContextRefType;

typedef P2VAR(struct Fee_InstanceDescriptorStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceDescriptorRefType;
typedef P2CONST(struct Fee_InstanceDescriptorStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_InstanceDescriptorConstRefType;

typedef P2VAR(struct Fee_SectorSwitchData, AUTOMATIC, FEE_VAR_NOINIT) Fee_SectorSwitchDataRefType;

struct Fee_ChunkHoppingContextStruct   /*<! Context, i.e. parameters and run-time variables, of Chunk Hopping operations */
{
    struct Fee_ChunkInfoStruct tempChunk_t; /*<! Temporary chunk (whose header is to be evaluated) */

    Fee_LengthType upperSearchLimit_t;      /*<! where chunk  hopping shall end (=> reverse chunk search; limit decreases) */
    Fee_BlockIdRefType resultBlockId_pt;    /*<! OUT parameter: ID of result chunk (the chunk just found); may be NULL_PTR */

    struct Fee_BlockIdStruct searchBlockId_t; /*<! the blockId we are looking for */
    struct Fee_BlockIdStruct blockIdMask_t;   /*<! determine the parts (bits) of block ID that are relevant during search.
                                                   1 <=> bit of interest, 0 <=> ignore bit */
    uint32 headerBuffer_au32[FEE_INTERNAL_BUFFER_SIZE / 4]; /*<! Buffer to read a chenk header from flash into.
                                                                 uint32[] for easier access,
                                                                 size of INTERNAL_BUFFER_SIZE is always large enough */
};

struct Fee_InstanceWriteContextStruct /*<! Context, i.e. parameters and run-time variables, of an instance write operation */
{
    Fee_SectorConstRefType sector_pt; /*<! which sector do we want to write to? */
    Fee_LengthType targetOffset_t;    /*<! where within sector do we want to write to? (Current write position) */
    Fee_UserDataConstPtrType userData_pt; /*<! which user data (optional, may be NULL_PTR)? (Current buffer address) */
    uint32 ByteCount_u32;                 /*<! amount of remaining bytes Instance bytes (includes header and trailer) */
    uint8 BufferBytes[FEE_INTERNAL_BUFFER_SIZE]; /*<! internal buffer; used for 1st and 3rd (last) write streams */
};

struct Fee_tagInstanceDescrPair /*<! combine an instance (position within chunk) with its state (empty/valid/...) */
{
    uint16 index_t;
    Fee_InstanceStatusType status_t;
};

struct Fee_InstanceSearchContextStruct /*<! Context, i.e. parameters and run-time variables, of an instance search operation */
{
    Fee_InstanceDescriptorRefType instDescr_pt; /*<! IN/OUT -> chunk and Instance description */

    struct Fee_tagInstanceDescrPair currentInstance; /*! current instance's position and state */
    struct Fee_tagBinarySearch
    {
        struct Fee_tagInstanceDescrPair previousInstance;  /*<! previous pivot element */
        uint16 step_t; /*<! current step of binary search (size of sub-range) */
    } binary; /*!< only relevant for binary instance search */
    uint32 Buffer_au32[FEE_INTERNAL_BUFFER_SIZE / 4]; /*<! buffer to read an instance header or trailer into. Must be at least one single page, not just a byte or word! */
};

struct Fee_InstanceCopyContextStruct /*<! Context, i.e. parameters and run-time variables, of an instance copy operation */
{
    Fee_LengthType targetOffset_t; /*<! where to copy to (write position, implicitly relative to new sector) */
    Fee_LengthType sourceOffset_t; /*<! where to copy from (read position, implicitly relative to old sector) */
    Fee_LengthType ByteCount_t; /* <! Amount of (remaining) bytes to be copied; range [0..65537] (copy complete instances) */
    uint8 BufferBytes[FEE_INTERNAL_BUFFER_SIZE]; /*<! copy buffer: read into, write from */
};

struct Fee_ChunkAllocContextStruct  /*<! Context, i.e. parameters and run-time variables, of an chunk allocation operation */
{
#if(FEE_GET_WRITE_CYCLE_API == STD_ON)
    Fee_InstanceDescriptorConstRefType wccChunk_pt; /*<! pointer to most recent data chunk and instance descriptor; holds current WriteCycle counter; may not be NULL_PTR */
#endif    
    Fee_InstanceDescriptorRefType previousChunk_pt; /*<! IN/OUT pointer to previous instance descriptor structure.
                                                         initially it points the the chunk that gets the successor.
                                                         (It is intended to becom the "previous chunk")
                                                         Upon completion the target is updated to the true most recent chunk
                                                         (to be forwarded to instance handling, which typically follows); */

    struct Fee_ChunkHoppingContextStruct super_t;   /*<! Chunk-Hopping context; used for searching for NfA */

    struct Fee_ChunkInfoStruct newChunk_t;          /*<! IN: setup to hold the new chunk (to be created); everything except BaseOffset must be set. */
    struct Fee_BlockIdStruct blockId_t;             /*<! IN: chunk's blockId */
    Fee_BlockConfigRefType config_pt;               /*<! pointer to block config; may be NULL_PTR (during SectorSwitch in FBL mode) */
    enum FEE_tagChunkAllocResults                   /*<! detailed allocation result (compared to Fee_Globals_t.FeeLocalJobResult) */
    {
        FEE_CHUNK_ALLOC_OK,
        FEE_CHUNK_ALLOC_HEADER_FAILED,
        FEE_CHUNK_ALLOC_TRAILER_FAILED,
        FEE_CHUNK_ALLOC_SECTOR_FULL
    } ChunkAllocResult_t;
    uint32 buffer_au32[FEE_INTERNAL_BUFFER_SIZE / 4]; /*<! Buffer holding the chunk's byte-stream representation to be written to flash */
};

struct Fee_ChunkSearchContextStruct /*<! Context, i.e. parameters and run-time variables, of an chunk search operation */
{
    Fee_InstanceDescriptorRefType recentChunk_pt; /*<! OUT: points to the location that gets the validated recent chunk upon completion
                                                       during runtime it old holds the validated recent chunk found so far */
    Fee_ChunkInfoRefType prevChunk_pt;            /*<! OUT: get's the recent chunk's predecessor; may be NULL PTR, if not needed */
    struct Fee_ChunkInfoStruct mostRecentChunk_t; /* most recent chunk _candidate_ (not validated, yet), being currently checked */
    struct Fee_ChunkHoppingContextStruct super_t; /* for chunk-hopping we need a corresponding context; additionally it contains the read buffer */

    boolean chunkHoppingForced;                   /* denotes whether chunk-hopping must contiue, even if a chunk of the BlockId, we're looking for, was found */
    boolean chunkLinkPresent;                     /*<! for Linktables -> cleared if link-table does not contain current block's/dataset's entry */
};

struct Fee_SectorFormatContextStruct
{
    uint32 sectorHeader_au32[FEE_INTERNAL_BUFFER_SIZE / 4];
    Fee_SectorRefType sector_pt;
    uint16 linkTableSize_u16;
    uint8 sectorId_u8;
};

struct Fee_PartitionInitContextStruct /*<! Context, i.e. run-time variables, of partition initialization.
                                           Note: Partition it self, as well as its sectors are globally known */
{
    uint8 lowerSectorId;              /*<! temporary hold lower sector's Id */
    uint32 sectorHeaderData[FEE_INTERNAL_BUFFER_SIZE / 4]; /*<! sector header read buffer */
};

/* PRQA S 0750 2 *//* MD_MSR_18.4 */
union Fee_InternalSharedDataUnion /*<! aggregate (overlay) all contexts */
{
    struct Fee_InstanceWriteContextStruct  instanceWriteContext_t;
    struct Fee_InstanceCopyContextStruct   instanceCopyContext_t;
    struct Fee_InstanceSearchContextStruct instanceSearchContext_t;
    struct Fee_ChunkSearchContextStruct    chunkSearchContext_t;
    struct Fee_SectorFormatContextStruct   SectorFormatContext_t;
    struct Fee_PartitionInitContextStruct  PartitionInitContext_t;
    struct Fee_ChunkAllocContextStruct     ChunkAllocContext_t;
};

struct Fee_InstanceDescriptorStruct /*<! aggregate a chunk and instance information */
{
    struct Fee_ChunkInfoStruct chunk_t;
    struct Fee_tagInstanceDescrPair instance_t;
};

typedef P2VAR(struct Fee_tagInstanceDescrPair, AUTOMATIC, FEE_PRIVATE_DATA) Fee_InstanceDescrPairRef;

struct Fee_SectorSwitchData /*<! information to evaluate sector switch's progress */
{
    /* PRQA S 0750 2 *//* MD_MSR_18.4 */
    union
    {
        struct
        {
            uint16 BlockConfigIndex_u16;
            sint16 DataIndex_s16; /*<! current data index [0..NumberOfDataSets); -1 => done */
        } appl;                   /*<! in application mode, we scan the block config table enty by entry */
        struct Fee_ChunkInfoStruct fbl; /*<! in FBL mode we scan the older sector chunk by chunk */
    } mode;
    sint8 partitionId_s8; /* Id of partition, above information belongs to; i.e. the partition that is pending */
};

struct Fee_tagProcessDataStruct /*<! Central data of currently running processing */
{
    Fee_PartitionRefType Partition_pt; /*<! Partition */
    Fee_BlockConfigRefType BlockConfig_pt; /*<! Current block config; may be NULL (sector switch in FBL mode) */
    struct Fee_BlockIdStruct BlockId;  /*<! current block ID */
};

typedef P2VAR(struct Fee_tagProcessDataStruct, AUTOMATIC, AUTOMATIC) Fee_ProcessDataRefType;

typedef struct Fee_GlobalVariableStruct /*<! (Almost) all FEE variables are gathered here */
{
    Fee_ConfigRefTypeInternal Config_pt; /*<! Configuration FEE was initialized with */
    Fee_ChunkHoppingContextRefType chunkHoppingContext_pt; /*<! pointer chunk hopping context; chunk hopping may occur within different contexts -> point to the right one */
    union Fee_InternalSharedDataUnion FsmLocalVars_t;      /*<! all possible contexts; active one is implied by running FSM */
    Fee_DeferredFuncType DeferredWriteAction_pt;           /*<! write accesses can be deferred (suspended); holds a pointer to the function
                                                                that performs a write access. */
    struct Fee_ChunkInfoStruct PreviousChunk_t;            /*<! Many operations need to know the most recent chunk's predecessor */

    struct Fee_InstanceDescriptorStruct CurrentDataset_t;  /*<! Holds/Gets most recent chunk and most recent instance */
    struct Fee_InstanceDescriptorStruct SectorSwitchTarget_t; /*<! A backup of most recent chunk in new sector (usually the sector switch target) */

    struct Fee_tagProcessDataStruct CurrentProcessData;  /*<! see structure definition */

    struct Fee_FeeJobParameterStruct JobParams;          /*<! parameters of pending job */
    struct Fee_SectorSwitchData SectorSwitchData_t;      /*<! sector switch's progress data */
    struct
    {
        Fee_BitFieldType forceFblMode :1;       /* sector switch processing shall be done in FBL mode */
        Fee_BitFieldType forceSectorSwitch :1;  /*< force sector switch processing was requested */
        Fee_BitFieldType cancelReq :1;
        Fee_BitFieldType enableBss :1;
        Fee_BitFieldType enableFss :1;
        Fee_BitFieldType needProcessing :1; /*<! indicates whether processing is actual necessary. Can be thought of an event;
                                                  something happend that needs attention (within Fee_MainFunction) */
    } ApiFlags;                              /*<! Flags set/controlled by FEE API; exclusive area necessary */
    struct
    {
        Fee_BitFieldType leaveMain :1;      /*<! signals "leave main-function" (FSM processing is a loop) */
        Fee_BitFieldType brokenLink :1;     /*<! broken link was detected during chunk search */
        Fee_BitFieldType jobDeferred :1;    /*<! writes: job was deferred; some sector switch processing is necessary */
        Fee_BitFieldType forceSectorSwitch :1; /*<! force sector switch processing was started / is running */
    } InternalFlags_t; /*<! flags only written by MainFunctuion; no sync necessary */

    Fee_LocalJobResultType FeeLocalJobResult_t; /*<! local result of an FSM; ugly but necessary (legacy) */
    MemIf_StatusType FeeModuleStatus_t;         /*<! Fee's status as seen from outside (Fee_GetStatus) */
    MemIf_JobResultType FeeJobResult_t;         /*<! Most recent job's result as it can be queried (Fee_GetJobResult */
    MemIf_JobResultType FlsJobResult_t;
    struct Fee_FsmStackStruct FsmStack_t;      /*<! the FSM stack. */

    /* Flag indicating suspension of Write Accesses.                            *
     *  Separated from other flags to avoid necessity of access synchronization */
    boolean WritesSuspended_bl;               /* set if write accesses are suspended; controlled via API */
} Fee_GlobalVariableType;

#define FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(Fee_GlobalVariableType, FEE_VAR_NOINIT) Fee_Globals_t;

#define FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FEE_START_SEC_CODE
#include "MemMap.h"

/**********************************************************************************************************************
 * Fee_DoProcessing
 *********************************************************************************************************************/
/*!
 * \brief      Does aynchronous job processing.
 * \details    Includes Cancellation handling and polling for Fls jobs.
 * \param[in,out]  g
 * \pre        Fee must be initialized and not IDLE (ensured by caller - Fee_MainFunction)
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_DoProcessing(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_FsmReset
 *********************************************************************************************************************/
/*!
 *
 * \brief      Resets the state machine by setting the toplevel state machine idle
 * \details    Resets the state machine by setting the toplevel state machine idle
 * \param[in,out]  g
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_FsmReset(Fee_GlobalsPtrType g);

/**********************************************************************************************************************
 * Fee_SignalNeedProcessing
 *********************************************************************************************************************/
/*!
 * \brief      Sets the processing flag which allows the FEE to start the actual processing
 * \details    Sets the processing flag which allows the FEE to start the actual processing
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_SignalNeedProcessing(void);

/**********************************************************************************************************************
 * Fee_FsmIsSectorSwitchFsmRunning
 *********************************************************************************************************************/
/*!
 * \brief      Checks if sector switch toplevel state machine is running
 * \details    Checks if sector switch toplevel state machine is running
 * \param[in]  g
 * \return     TRUE: Sector switch toplevel state machine is running
 * \return     FALSE: Otherwise
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_FsmIsSectorSwitchFsmRunning(Fee_GlobalsConstPtrType g);

#define FEE_STOP_SEC_CODE
#include "MemMap.h"

#endif /* FEE_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Int.h
 *********************************************************************************************************************/
