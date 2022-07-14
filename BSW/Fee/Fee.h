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
 *         File:  Fee.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and 
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as 
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *               A U T H O R   I D E N T I T Y
 *--------------------------------------------------------------------------------------------------------------------
 * Initials     Name                      Company
 * --------     ---------------------     ----------------------------------------------------------------------------
 * Ck           Christian Kaiser          Vector Informatik GmbH
 * Du           Manfred Duschinger        Vector Informatik GmbH
 * Jwa          Johannes Wagner           Vector Informatik GmbH
 *--------------------------------------------------------------------------------------------------------------------
 *              R E V I S I O N   H I S T O R Y
 *--------------------------------------------------------------------------------------------------------------------
 *  Version     Date        Author      Change Id      Description
 * -------------------------------------------------------------------------------------------------------------------
 *                                                       
 * 8.00.00      2012-06-22  Ck                          Removed previous revision history entries
 *                                      ESCAN00059643   changed signature of Fee_Init to be ASR compliant;
 *                                                      added Fee_InitEx.
 *                                      ESCAN00059332   Fee_ForceSectorSwitch is performed only once
 *                                      ESCAN00057779   AR3-2408 - Split Data Flash into Partitions
 * 8.00.01      2012-08-27  Ck          ESCAN00059980   Corrected Fee_Partition_Init (RejectAllMode flag)
 *                                      ESCAN00059751   Removed redundant function declarations from Fee_Partition.h
 *                                      ESCAN00060047   Corrected BlockNumber reset after sector switch.
 *                                      ESCAN00060015   Corrected Instance Header Evaluation (if alignment < 4)
 *                                      ESCAN00060003   Modified WriteProcessing FSM; reinit ChunkAllocContext in
 *                                                      Fee_ActAllocationInCurrentSetor()
 *                                      ESCAN00060054   Write Cycle Counter Handling
 *                                      ESCAN00060084   No implicit pointer type definitions - typedefs used instead
 *                                      ESCAN00060087   Corrected updating a sector's next free address.
 *                                      ESCAN00060887   Sector Switch in FBL mode: Corrected initialization of Chunk
 *                                                      Hopping; corrected initialization of chunk allocation,
 *                                                      Handling of Link-Table chunks during chunk hopping (Fee_ChunkInfo)
 *                                      ESCAN00060893   Corrected internal function declaration
 *                                      ESCAN00061061   Variable declarations in Fee_ChunkInfo.c (typos)
 *                                      ESCAN00060999   Variable initialization and in Fee_InternalScheduleProcess
 *                                                      Context Initialization in Fee_ActInitInstanceWriteDcFsm
 *                                      ESCAN00061094   Avoid Compiler Warnings
 * 8.00.02      2012-09-14  Ck          ESCAN00061421   MISRA compliance / QAC findings
 *                                      ESCAN00061327   Corrected Fee_ActFinalizeDataConversion,
 *                                                                Fee_InternalInitDataConversionFsm
 * 8.00.03      2012-12-19  Ck          ESCAN00063033   corrected Assembly of ChunkHeader-/Link (ChunkType info)
 *                                                      in Fee_ChunkInfo.c
 *                                      ESCAN00063064   Made sector space check (chunk allocation) more robust.
 *                                                      Changed ChunkHopping limit check (=> check End address)
 *                                      ESCAN00063833   Refactoring of Chunk-Hopping Initialization.
 *                                      ESCAN00063881   Initialization of local variable in Fee_InternalScheduleProcess
 *                                      ESCAN00064116   Corrected Fee_ChunkInfo_InitFromLinkData(),
 *                                                      Fee_ChunkInfo_EvaluateHeaderData, Fee_ChunkInfo_CreateFromRawData;
 *                                      ESCAN00063791   Fee_InternalIsCancelableOperation changed (avoid "NULL Sector"): 
 *                                                      It allows canceling almost every thing
 *                                      ESCAN00064507   Avoid compiler warning (Fee_ActFinalizeChunkAllocation)
 *                                                  -   Insignificant modifications to increase MISRA compliance
 * 8.00.04     2013-01-05    Ck         ESCAN00064769   Updated Fee_Sector_allocSpace - handle allocation attempts for 
 *                                                      <sector size> (and more).
 *                                                      Mark old sector as "full" when block copy starts.
 *                                                      (by attempting to allocate whole sector at once).
 *                                                      Fixed condition to perform a sector switch
 *                                                      (old sector "unusable", new sector full or no job pending
 *                                                      or sector switch forced)
 * 8.00.05     2013-03-21    Ck         ESCAN00066078   Fixed Fee_ActSwitchOperatingSector to correctly re-initialize
 *                                                      Search in old sector -> use Fee_ChunkSearchContext_Init().
 * 8.00.06     2013-06-24    Ck         ESCAN00065913   Made sure Fee_ChunkInfo.c ends with "newline".
 *                                      ESCAN00066070   Added "base vs. link offset" check to Fee_ChunkInfo_InitFromLinkData
 *                                      ESCAN00066124   Failed chunk-link write (Fee_ActFinalizeChunkAllocation) results
 *                                                      in FEE_LOCAL_JOB_RESULT_FAILED
 *                                      ESCAN00066316   Removed excluded code (#if 0 ... #endif) from Fee_Partition.c
 *                                      ESCAN00066318   Removed Fee_QryLocalJobResultInconsistent (unused)
 *                                      ESCAN00066317   Removed code depending on obsolete definitions of Fee_FlsReadSync/Fee_FlsCopy
 *                                      ESCAN00067914   Surround declaration of Fee_ChunkInfo_NullChunk with memory section. 
 *                                      ESCAN00067913   Added ECO-Labels (comments) to aid coverage analysis using ECO
 * 8.00.07     2013-10-07    Ck         ESCAN00070922   Added conditional definitions of Fee_GlobalSuspendLong/Fee_GlobalRestoreLong
 *                                                      to Fee.c
 *                                      ESCAN00071245   MISRA Justification (Rule 19.7, Fee_ChunkInfo.c)
 *                                      ESCAN00071273   AUTOSAR4 compliant version identification (but AUTOSAR3 macros remain.)
 * 8.00.08     2014-02-04    Ck         ESCAN00070359   Improved robustness of SectorId updates -
 *                                                      1. Fee_Partition_getNextSectorId,
 *                                                      2. Fee_Partition_updateSectorId
 *                                      ESCAN00072977   Partitions remain uninitialized if no requests on them occur.
 *                                                      Fee_InternalScheduleProcess, Fee_InternalInitBusyFsm,
 *                                                      Fee_InternalInitPartitionInitializationFsm
 *                                      ESCAN00073366   Fee_Globals_t got external linkage (Debugging purposes).
 *                                      ESCAN00071246   Code Metrics - did refactoring on Fee_MainFunction,
 *                                                      Fee_InternalGetNextBlockToCopy, Fee_ActPrepareChunkLinkWrite,
 *                                                      Fee_Partition_getFillLevel
 * 8.00.09     2015-01-08    Ck         ESCAN00078523   fixed declaration of Fee_GetBlockCfgByBlockId 
 *                                      ESCAN00076637   Fee_Globals_t's declaration surrounded by MemMap stuff
 *                                      ESCAN00078905   ChunkHeaderChecks; when and how to initialize ChunkHopping
 *                                                      ChunkHopping is fallback - don't switch back to link-based search.
 *                                                      chunkHoppingForced, chunkLinkPresent in Fee_ChunkSearchContextStruct
 *                                                      as well as related handling, in order to allow switching back from
 *                                                      chunk hopping to link-based search (performance), and to distinguish
 *                                                      non-existent from defective chunk links (=> within link-tables)
 *                                      ESCAN00078909   Fee_ActValidateChunkLink - condition to accept Link-Table Chunks.
 *                                      ESCAN00080317   Explicit re-initialization of SectorSwitch progress variables
 *                                                      (Fee_SectorSwitchData) after SectorOverflow.
 *                                                      Factored out that re-init into function Fee_InternalResetSectorSwitch
 *                                                      Added FEE_STATE_SECTOR_SWITCH_IDLE
 *                                                      Fee_FinalizeBlockCopy conditionally advances SectorSwitch progress
 *                                                      Dedicated result codes for chunk allocation, allowing to distinguish
 *                                                      different failure conditions.
 *                                      ESCAN00081069   FEE_STATE_SECTOR_SWITCH_ALLOCATE_LINKTABLE_CHUNK -> detect/report
 *                                                      SectorOverflows
 *                                      ESCAN00081179   Fee_ActFinalizeChunkSearch to retrieve result (chunk info) added 
 *                                                      to FEE_STATE_SEARCH_UPWARDS_CHUNK_HOPPING 
 * 8.00.10     2015-02-28    Ck         ESCAN00081213   Fee_ChunkInfo_EvaluateHeaderData first makes a copy if "self" chunk,
 *                                                      to be able to restore it.
 * 8.00.11     2015-04-08    Ck         ESCAN00081636   Fee_InternalGetNextBlockToCopy (FblMode path) checks Fls address
 *                                                      before attempting to read chunk header.
 * 8.01.00     2015-07-06    Ck         ESCAN00083410   Avoid compiler warnings
 *                                      ESCAN00080062   Restructuring of code /source files
 *                                      ESCAN00081774   SafeBsw:
 *                                                      - API / DET checks
 *                                                      - MISRA compliance
 *                                                      - Silent Analysis / Justification
 * 8.01.01      2016-03-03   Ck         ESCAN00088490   Fee_InternalInitChunkHoppingFsm -> fixed end condition
 *                           Jwa        ESCAN00088158    SafeBsw:Added CDD (i.e. doxygen comments)
 *                           Ck         ESCAN00087027   Don't swap new/old sectors in case of FEE_ERRCBK_RESOLVE_AUTOMATICALLY,
 *                                                         if FEE_SECTOR_FORMAT_FAILED was reported: Currently old sector
 *                                                         shall be retried (in contrast to FEE_SECTOR_OVERFLOW)
 *                                      ESCAN00089691   Reset "job deferred" flag, upon job cancellation to avoid
 *                                                         delaying an immediate write job due to critical block handling
 *                                      ESCAN00089683   Added "BlockCopy required" to ensure that Fee_ForceSectorSwitch
 *                                                         always performs a copy process.
 * 8.01.02      2016-08-23   Ck         ESCAN00088456   copy inconsistent instances (though never written);
 *                                                      force chunk allocation in this case
 *                                      ESCAN00091314   Make sure, Fee_MainFunction does not change variables is uninit
 *                                      ESCAN00090536   Avoid link-writing, if it already exists or there's no link-field
 *                                      ESCAN00091684   Fee_InternalInitSectorSwitchFsm restructured -> init partition only
 *                                                      if sector switch was forced.
 *                                      ESCAN00088158   CDD rework, SilentAnalysis - justification, MISRA, Coverage
 *                                                      refactoring due to these activities
 *                                      ESCAN00091697   Fee_InternalHandleCancelRequest: Always clean-up FSM; don't wait
 *                                                      for next write request.
 * 8.01.03      2016-11-15   Ck         ESCAN00091216   Sector Switches: Chunk allocation only if a copy source exists
 * 8.01.04      2017-01-13   Ck         ESCAN00093266   FEE skips Datasets during Block Copy
 *                                      ESCAN00093379   MISRA deviation: MISRA-C:2004 Rule 10.1
 * 8.01.05      2017-03-08   Ck         ESCAN00094273   Compiler warning:  unreferenced formal parameter
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined (FEE_H)
# define FEE_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : If_AsrIfFee CQComponent : Implementation */
/* Version - BCD coded version number - Main- and Sub-Version - Release-Version */
# define IF_ASRIFFEE_VERSION                                          0x0801u
# define IF_ASRIFFEE_RELEASE_VERSION                                  0x05u

/* FEE is still compatible with AUTOSAR 3 and AUTOSAR4!
 * This implementation shall remain usable in AUTOSAR3 environments.
 * Therefore define macros according to both major releases in parallel. */
/* ASR3.1.5 - the version of AUTOSAR_SWS_Flash_EEPROM_Emulation */
# define FEE_AR_MAJOR_VERSION                                         (1u)
# define FEE_AR_MINOR_VERSION                                         (2u)
# define FEE_AR_PATCH_VERSION                                         (1u)

/* AUTOSAR4 - the overall AUTOSAR release */
# define FEE_AR_RELEASE_MAJOR_VERSION                                 (4u)
# define FEE_AR_RELEASE_MINOR_VERSION                                 (0u)
# define FEE_AR_RELEASE_REVISION_VERSION                              (3u)

# define FEE_VENDOR_ID                                                (30u)
# define FEE_MODULE_ID                                                (21u)
# define FEE_INSTANCE_ID                                              (0u)

# define FEE_SW_MAJOR_VERSION                                         (IF_ASRIFFEE_VERSION >> 8u)
# define FEE_SW_MINOR_VERSION                                         (IF_ASRIFFEE_VERSION & 0x00FFu)
# define FEE_SW_PATCH_VERSION                                         IF_ASRIFFEE_RELEASE_VERSION

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fee_Types.h"
#include "Fee_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define FEE_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
 * Fee_Init
 *********************************************************************************************************************/
/*!
 * \brief      Initialization of module Fee and resetting of the state machine.
 * \details    Initialization of module Fee and resetting of the state machine.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \note       The FEE does not initialize the underlying Flash driver, but this shall be done by the ECUM module. (This is
               no AUTOSAR deviation.)
 */
FUNC(void, FEE_API_CODE) Fee_Init(void);

/**********************************************************************************************************************
 * Fee_SetMode
 *********************************************************************************************************************/
/*!
 * \brief      Setting the mode of the Fee to influence the way how a sector switch is done
 * \details    In deviation from AUTOSAR this service does not call the underlying Flash driver's related function
 *             Fls_SetMode
 * \param[in]  Mode: Mode to which FEE shall be set
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \note       Fee_SetMode does not disable foreground sector switches.
 *             Mode: MEMIF_MODE_SLOW (Sector switch in background is allowed) or MEMIF_MODE_FAST (Sector switch in
 *             background is not allowed). Will not be checked for validity. Any other value will be treated as
 *             MEMIF_MODE_FAST
 */
FUNC(void, FEE_API_CODE) Fee_SetMode(MemIf_ModeType Mode);

/**********************************************************************************************************************
 * Fee_Read
 *********************************************************************************************************************/
/*! \brief      Reads data from Flash to the given buffer.
 *  \details    Reads data from Flash to the given buffer.
 *
 *  \param[in]  BlockNumber         Unique identifier of block
 *  \param[in]  BlockOffset         Offset in the block/dataset.
 *  \param[out] DataBufferPtr       Pointer to buffer from upper layer.
 *  \param[in]  Length              Count of bytes which should be read.
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \pre        Module must be initialized.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_Read(uint16 BlockNumber,
                                            uint16 BlockOffset,
                                            Fee_UserDataPtrType DataBufferPtr,
                                            uint16 Length);

/**********************************************************************************************************************
 * Fee_Write
 *********************************************************************************************************************/
/*! \brief      Writes content of given buffer to Flash.
 *  \details    Writes content of given buffer to Flash.
 *  \param[in]  BlockNumber         Unique identifier of block
 *  \param[in]  DataBufferPtr       Pointer to buffer from upper layer.
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \pre        Module must be initialized.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_Write(uint16 BlockNumber, Fee_UserDataPtrType DataBufferPtr);

/**********************************************************************************************************************
 * Fee_Cancel
 *********************************************************************************************************************/
/*! \brief      Cancels a currently pending job
 *  \details    Cancels a currently pending job
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *  \note       A running sector switch (on any partition) will not be cancelled in destructive manner,
 *              if FSS threshold was exceeded. Therefore a subsequent write job will be deferred, until a running copy
 *              process (for one single block) has been finished.
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_Cancel(void);

# if (STD_ON == FEE_FORCE_SECTOR_SWITCH_API)
/**********************************************************************************************************************
 * Fee_ForceSectorSwitch
 *********************************************************************************************************************/
/*! \brief      Forces a sector switch to be performed on all configured partitions in 'Foreground Mode'
 *  \details    Forces a sector switch to be performed on all configured partitions in 'Foreground Mode'
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \config     FEE_FORCE_SECTOR_SWITCH_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_ForceSectorSwitch(void);
# endif

/**********************************************************************************************************************
 * Fee_GetStatus
 *********************************************************************************************************************/
/*! \brief      Gets the current status of the module.
 *  \details    Gets the current status of the module.
 *  \return     Current module status.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(MemIf_StatusType, FEE_API_CODE) Fee_GetStatus(void);

/**********************************************************************************************************************
 * Fee_GetJobResult
 *********************************************************************************************************************/
/*! \brief      Get the result most recently accepted job.
 *  \details    Either the job is currently being processed, or it recently terminated.
 *  \return     Previously or currently running job result.
 *  \pre        Module must be initialized.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(MemIf_JobResultType, FEE_API_CODE) Fee_GetJobResult(void);

/**********************************************************************************************************************
 * Fee_InvalidateBlock
 *********************************************************************************************************************/
/*! \brief      Invalidates the block BlockNumber.
 *  \details    This service invokes the invalidation procedure for the selected block. If the service succeeds the most
 *              recent data block is marked as INVALID.
 *  \param[in]  BlockNumber         Unique identifier of block
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \pre        Module must be initialized.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_InvalidateBlock(uint16 BlockNumber);

# if (STD_ON == FEE_VERSION_INFO_API)
/**********************************************************************************************************************
 * Fee_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Get the module version information.
 *  \details    Get the module version information.
 *  \param[out] versioninfo         Pointer to where to store the version information of this module.
 *  \config     FEE_VERSION_INFO_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_GetVersionInfo(Fee_StdVersionInfoRefType versioninfo);
# endif

/**********************************************************************************************************************
 * Fee_EraseImmediateBlock
 *********************************************************************************************************************/
/*! \brief      Erases the block which is referenced by BlockNumber.
 *  \details    Erases the block which is referenced by BlockNumber.
 *  \param[in]  BlockNumber         Unique identifier of block
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *  \note       This function doesn't erase flash memory. The addressed block is marked as invalid, thus a subsequent
 *              read request on the invalidated block completes with MEMIF_BLOCK_INVALID.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_EraseImmediateBlock(uint16 BlockNumber);

/**********************************************************************************************************************
 * Fee_MainFunction
 *********************************************************************************************************************/
/*! \brief      Service to handle the requested jobs and the internal management operations.
 *  \details    Service to handle the requested jobs and the internal management operations.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_MainFunction(void);

# if (STD_ON == FEE_GET_ERASE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_GetEraseCycle
 *********************************************************************************************************************/
/*! \brief      Get the erase cycle of the specified sector.
 *  \details    Get the erase cycle of the specified sector.
 *  \param[in]  SectorNumber        Sector number of which to read the erase cycle.
 *  \param[out] DataPtr             Pointer to where to store the erase cycle.
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \config     FEE_GET_ERASE_CYCLE_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_GetEraseCycle(uint8 SectorNumber, Fee_UserData32PtrType DataPtr);
# endif

# if (STD_ON == FEE_GET_WRITE_CYCLE_API)
/**********************************************************************************************************************
 * Fee_GetWriteCycle
 *********************************************************************************************************************/
/*! \brief      Get the write cycle of the specified block/dataset.
 *  \details    Get the write cycle of the specified block/dataset.
 *  \param[in]  BlockNumber         Unique identifier of block
 *  \param[out] DataPtr             Pointer to where to store the write cycle.
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \config     FEE_GET_WRITE_CYCLE_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_GetWriteCycle(uint16 BlockNumber, Fee_UserData32PtrType DataPtr);
# endif

/**********************************************************************************************************************
 * Fee_GetSectorSwitchStatus
 *********************************************************************************************************************/
/*! \brief      Get the current status of the sector switch.
 *  \details    Get the current status of the sector switch.
 *  \return     Status of the sector switch.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Fee_SectorSwitchStatusType, FEE_API_CODE) Fee_GetSectorSwitchStatus(void);

/**********************************************************************************************************************
 * Fee_SuspendWrites
 *********************************************************************************************************************/
/*! \brief      Blocks all write jobs
 *  \details    Blocks all write jobs. Pending write jobs are paused until writes are resumed again.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_SuspendWrites(void);

/**********************************************************************************************************************
 * Fee_ResumeWrites
 *********************************************************************************************************************/
/*! \brief      Resumes previously suspended write jobs
 *  \details    Resumes previously suspended write jobs
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_ResumeWrites(void);

#if (FEE_FSS_CONTROL_API == STD_ON)
/**********************************************************************************************************************
 * Fee_DisableFss
 *********************************************************************************************************************/
/*! \brief      This function disables execution of foreground sector switch when threshold is reached.
 *  \details    This function disables execution of foreground sector switch when threshold is reached.
 *  \config     FEE_FSS_CONTROL_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_DisableFss(void);

/**********************************************************************************************************************
 * Fee_EnableFss
 *********************************************************************************************************************/
/*! \brief      This function enables execution of foreground sector switch when threshold is reached.
 *  \details    This function enables execution of foreground sector switch when threshold is reached.
 *  \config     FEE_FSS_CONTROL_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FEE_API_CODE) Fee_EnableFss(void);
#endif

#if (FEE_DATA_CONVERSION_API == STD_ON)
/**********************************************************************************************************************
 * Fee_ConvertBlockConfig
 *********************************************************************************************************************/
/*! \brief      Requests conversion of blocks regarding to specified options
 *  \details    Requests conversion of blocks regarding to specified options
 *  \param[in]  options_pt            Pointer to structure, containing the pointer to the user buffer and to the callback
 *                                  to be invoked for each block.
 *  \return     E_OK                Job accepted.
 *  \return     E_NOT_OK            Job rejected.
 *  \config     FEE_DATA_CONVERSION_API is enabled via pre-compile switch
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FEE_API_CODE) Fee_ConvertBlockConfig(Fee_ConversionOptionsConstRefType options_pt);
#endif

# define FEE_STOP_SEC_CODE
# include "MemMap.h"

/*!
 * \exclusivearea FEE_EXCLUSIVE_AREA_0
 * Ensures consistency of internal data while/if FEE's APIs may interrupt each other,
 * i.e. if the OS tasks they're called from can preempt each other.
 * Entering/Leaving is wrapped by Fee_GlobalSuspend, Fee_GlobalRestore.
 *
 * \protects Different variables that are accessed from API as well as from inside Fee_MainFunction, such as:
 * Fee_Globals_t.Fee_JobParams; Fee_Globals_t.WritesSuspended_bl, Fee_Globals_t.ApiFlags, Fee_Globals_t.FeeModuleStatus_t
 *
 * \usedin ALL functions, except: Fee_Init, Fee_InitEx, Fee_GetStatus, Fee_GetJobResult, Fee_GetVersionInfo
 * \exclude N/A
 * \length SHORT only few instructions to synchronize access to mentioned variables.
 * \endexclusivearea
 */

#endif /* FEE_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee.h
 *********************************************************************************************************************/
