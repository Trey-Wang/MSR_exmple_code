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
 *         File:  Fee_ChunkInfo.h
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
#ifndef FEE_CHUNKINFO_H
#define FEE_CHUNKINFO_H

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_CHUNKINFO_MAJOR_VERSION                                   (8u)
# define FEE_CHUNKINFO_MINOR_VERSION                                   (1u)
/* BCD coded! */
# define FEE_CHUNKINFO_PATCH_VERSION                                   (3u)

#include "Fee_Sector.h" /* also includes Fee_Cfg.h */

#define FEE_INSTANCE_HEADER_SIZE                                      (1u)
#define FEE_INSTANCE_TRAILER_SIZE                                     (1u)

#define FEE_CHUNK_HEADER_SIZE                                         (16u)
#define FEE_CHUNK_HEADER_SIZE_READ                                    (8u)

#if (FEE_GET_WRITE_CYCLE_API == STD_ON)
# define FEE_CHUNK_HEADER_SIZE_WRITE                                  (FEE_CHUNK_HEADER_SIZE)
/* offset of WCC in 32bit buffer array */
# define FEE_CHUNK_HEADER_WCC_WORD_OFFSET                             (2u)
# define FEE_WCC_BYTE_COUNT                                           (4u)
#else
# define FEE_CHUNK_HEADER_SIZE_WRITE                                  (FEE_CHUNK_HEADER_SIZE_READ)
#endif

/* In order keep dependencies simple, the Sector defines its own chunk link size.
 * Chunk already depends on Sector; we just define CHUNK_TRAILER_SIZE to have same value
 */
#define FEE_CHUNK_TRAILER_SIZE (FEE_SECTOR_LINKTABLE_ELEMENT_SIZE)

/*! Pointer/reference types for ChunkInfo. Each of the functions declared below, whose first parameter is of such
 *  reference type, can be seen as member function (method) of "class ChunkInfo".
 *  The parameter's type (constness) denotes the method's constness, i.e. whether it is supposed to change obect's
 *  state.
 *  The reference to the object ("this" in C++ and Java) is usually named "self" (in the style of smalltalk).
 *  Reason: It is not recommended to use "this" in C (we use C++ for testing!).
 */
typedef P2VAR(struct Fee_ChunkInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_ChunkInfoRefType;
typedef P2CONST(struct Fee_ChunkInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_ChunkInfoConstRefType;

/*********************************************************************************
 * ChunkInfo Functions
 *********************************************************************************/
#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */



/**********************************************************************************************************************
 * Fee_ChunkInfo_Clone
 *********************************************************************************************************************/
/*!
 * \brief      Deep copies given source chunk into given target chunk
 * \details    Deep copies given source chunk into given target chunk
 * \param[in,out]  target: pointer to target structure (copy to), not NULL
 * \param[in]      source: pointer to source structure (copy from), not NULL
 * \pre
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Clone(Fee_ChunkInfoRefType target, Fee_ChunkInfoConstRefType source);


/**********************************************************************************************************************
 * Fee_ChunkInfo_Clear
 *********************************************************************************************************************/
/*!
 * \brief      Clears chunk info by setting all its variables to 0
 * \details    Clears chunk info by setting all its variables to 0
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Clear(Fee_ChunkInfoRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_GetSector
 *********************************************************************************************************************/
/*!
 * \brief      Returns sector of given chunk info
 * \details    Returns sector of given chunk info
 * \param[in]  self
 * \pre        self is not null
 * \return     sector
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_ChunkInfo_GetSector(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_GetBaseAddress
 *********************************************************************************************************************/
/*!
 * \brief      Returns chunk base offset of the chunk within its sector
 * \details    chunk's base offset is its start address relative to sector's begin
 * \param[in]  self
 * \pre        self is not null
 * \return     baseAddress
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetBaseAddress(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_Equals
 *********************************************************************************************************************/
/*!
 * \brief      Checks if two given chunks are equal
 * \details    Compares sector, base address, instance exponent, chunk type and payload size
 * \param[in]  self
 * \pre        self is not null, other is not null
 * \return     FALSE: Chunks are not equal
 * \return     TRUE: Chunks are equal
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool)  Fee_ChunkInfo_Equals(Fee_ChunkInfoConstRefType self, Fee_ChunkInfoConstRefType other);



/**********************************************************************************************************************
 * Fee_ChunkInfo_Construct
 *********************************************************************************************************************/
/*!
 * \brief      Creates initial chunk info and sets given sector
 * \details    Creates initial chunk info and sets given sector
 * \param[in,out]  self
 * \param[in]  sector_pt
 * \pre        self is not null, sector_pt is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Construct(Fee_ChunkInfoRefType, Fee_SectorRefType);



/**********************************************************************************************************************
 * Fee_ChunkInfo_AssignSector
 *********************************************************************************************************************/
/*!
 * \brief      Allocates chunk in given sector
 * \details    Allocates chunk in given sector
 * \param[in,out]  self
 * \param[in,out]  sector_pt
 * \pre        self is not null, sector_pt is not null
 * \return     FALSE: Chunk does not fit into sector
 * \return     TRUE: Chunk fits into sector
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_AssignSector(Fee_ChunkInfoRefType, Fee_SectorRefType);



/**********************************************************************************************************************
 * Fee_ChunkInfo_EvaluateHeaderData
 *********************************************************************************************************************/
/*!
 * \brief      Constructs chunk info out of chunk header stored in given buffer and checks its status
 * \details    Updates next free address when chunk header is not erased
 * \param[in,out]  self
 * \param[in,out]  blockId_pt
 * \param[in]  intBuf
 * \param[in]  ReadOk
 * \pre        self is not null
 * \return     FEE_MNGMT_ERASED: Chunk header is erased
 * \return     FEE_MNGMT_CONSISTENT: Chunk header is consistent
 * \return     FEE_MNGMT_DEFECTIVE: Chunk header is defective
 * \return     TRUE: Chunk fits into sector
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_EvaluateHeaderData(
    Fee_ChunkInfoRefType self,
    Fee_BlockIdRefType blockId_pt,
    Fee_IntBuffer_u32ConstPtrType intBuf_au32,
    Fee_Bool ReadOk);



/**********************************************************************************************************************
 * Fee_ChunkInfo_InitFromLinkData
 *********************************************************************************************************************/
/*!
 * \brief      Constructs chunk info out of chunk link stored in given buffer and checks its status
 * \details    Updates next free address when chunk header is not erased
 * \param[in,out] self: Chunk info that shall be constructed (Chunk link points to)
 * \param[in]  referrer_pt: Chunk info of the chunk the link belongs to (selfs predecessor)
 * \param[in]  intBuf
 * \param[in]  ReadOk
 * \pre        self is not null
 * \return     FEE_MNGMT_ERASED: Chunk link is erased
 * \return     FEE_MNGMT_CONSISTENT: Chunk link is consistent
 * \return     FEE_MNGMT_DEFECTIVE: Chunk link is defective
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_InitFromLinkData(
    Fee_ChunkInfoRefType self,
    Fee_ChunkInfoConstRefType referrer_pt,
    Fee_IntBuffer_u32ConstPtrType intBuf,
    Fee_Bool ReadOk);



/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateNewDataChunk
 *********************************************************************************************************************/
/*!
 * \brief      Creates new chunk info, sets chunk type to data chunk, but does not assign sector
 * \details    Creates new chunk info, sets chunk type to data chunk, but does not assign sector
 * \param[in,out]  self
 * \param[in]  payload
 * \param[in]  instanceExponent
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_CreateNewDataChunk(Fee_ChunkInfoRefType self,
                                                               uint16 payload,
                                                               uint8 instanceExponent);


/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateNewLinktableChunk
 *********************************************************************************************************************/
/*!
 * \brief      Creates new chunk info, sets chunk type to linktable chunk, but does not assign sector
 * \details    Creates new chunk info, sets chunk type to linktable chunk, but does not assign sector
 * \param[in,out]  self
 * \param[in]  datasetCount
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_CreateNewLinktableChunk(Fee_ChunkInfoRefType self, uint8 datasetCount);



/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceCount
 *********************************************************************************************************************/
/*!
 * \brief      Gets number of instances stored in chunk info (number of instances a empty chunk could store)
 * \details    Gets number of instances stored in chunk info (number of instances a empty chunk could store)
 * \param[in]  self
 * \pre        self is not null
 * \return     instance count
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint16)   Fee_ChunkInfo_GetInstanceCount(Fee_ChunkInfoConstRefType self);



/**********************************************************************************************************************
 * Fee_ChunkInfo_SwitchToSuccessor
 *********************************************************************************************************************/
/*!
 * \brief      Sets base address of the given chunk to the next following chunk
 * \details    Adds the chunk size to the base address. Base address points to next chunk
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void ) Fee_ChunkInfo_SwitchToSuccessor(Fee_ChunkInfoRefType);



/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceStartAddress
 *********************************************************************************************************************/
/*!
 * \brief      Returns the aligned address within the sector where the instance at given instance index starts
 * \details    Returns the aligned address within the sector where the instance at given instance index starts
 * \param[in]  chunk
 * \param[in]  instanceIndex
 * \pre        chunk is not null
 * \return     instanceStartAddress
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetInstanceStartAddress(
                                                     Fee_ChunkInfoConstRefType chunk, uint16 instanceIndex);



/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleHeader
 *********************************************************************************************************************/
/*!
 * \brief      Constructs chunk header buffer out of given chunk info and given blockId
 * \details    Constructs chunk header buffer out of given chunk info and given blockId
 * \param[in]  self
 * \param[in]  blockId_pt
 * \param[in,out]  buffer_au32
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_AssembleHeader(Fee_ChunkInfoConstRefType self,
                                                          Fee_BlockIdConstRefType blockId_pt, 
                                                          Fee_IntBuffer_u32PtrType buffer_au32);



/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleLink
 *********************************************************************************************************************/
/*!
 * \brief      Constructs chunk link buffer out of given chunk info
 * \details    Constructs chunk link buffer out of given chunk info
 * \param[in]  self
 * \param[in,out]  buffer_au32
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_AssembleLink(Fee_ChunkInfoConstRefType self, Fee_IntBuffer_u32PtrType buffer_au32);



/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadChunkHeader
 *********************************************************************************************************************/
/*!
 * \brief      Reads chunk header from flash into given buffer
 * \details    Reads chunk header from flash into given buffer
 * \param[in]  self
 * \param[in,out] buffer_au32
 * \pre        self is not null
 * \return     FEE_REQ_SYNC_USELESS: Chunk is a pseudo chunk. It has no header
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadChunkHeader(Fee_ChunkInfoConstRefType,
                                                                                       Fee_IntBuffer_u32PtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadChunkLink
 *********************************************************************************************************************/
/*!
 * \brief      Reads chunk link from flash into given buffer
 * \details    Reads chunk link from flash into given buffer
 * \param[in]  self
 * \param[in]  blockId
 * \param[in,out] buffer_au32
 * \pre        self is not null
 * \return     FEE_REQ_SYNC_USELESS: Reading chunk link is not allowed. Block does not fit into linktable
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadChunkLink(Fee_ChunkInfoConstRefType,
                                                                                     Fee_BlockIdConstRefType,
                                                                                     Fee_IntBuffer_u32PtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_WriteHeader
 *********************************************************************************************************************/
/*!
 * \brief      Writes chunk header from given buffer into flash
 * \details    Writes chunk header from given buffer into flash
 * \param[in]  self
 * \param[in,out] buffer_au32
 * \pre        self is not null
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_WriteHeader(Fee_ChunkInfoConstRefType,
                                                                                   Fee_IntBuffer_u32ConstPtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_WriteChunkLink
 *********************************************************************************************************************/
/*!
 * \brief      Writes chunk link from given buffer into flash
 * \details    Writes chunk link from given buffer into flash
 * \param[in]  self
 * \param[in]  blockId
 * \param[in] buffer_au32
 * \pre        self is not null
 * \return     FEE_REQ_SYNC_OK: Writing chunk link is not allowed. Block does not fit into linktable. Fake successful write.
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_WriteChunkLink(Fee_ChunkInfoConstRefType,
                                                                                     Fee_BlockIdConstRefType,
                                                                                     Fee_IntBuffer_u32ConstPtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_isDataChunk
 *********************************************************************************************************************/
/*!
 * \brief      Checks if given chunk is a data chunk
 * \details    Checks if given chunk is a data chunk
 * \param[in]  self
 * \pre        self is not null
 * \return     FALSE: Chunk is not a data chunk
 * \return     TRUE: Chunk is a data chunk
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isDataChunk(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_isInitialLinkTable
 *********************************************************************************************************************/
/*!
 * \brief      Checks if given chunk is initial link table
 * \details    Checks if given chunk is not a data chunk and a pseudo chunk
 * \param[in]  self
 * \pre        self is not null
 * \return     FALSE: Chunk is not initial linktable
 * \return     TRUE: Chunk is initial linktable
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isInitialLinkTable(Fee_ChunkInfoConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinkAllowed
 *********************************************************************************************************************/
/*!
 * \brief      Checks if writting the link is allowed
 * \details    Checks if writting the link is allowed
 * \param[in]  self
 * \pre        self is not null
 * \return     FALSE: Chunk was already written. It may not be written again.
 * \return     TRUE: Chunk was not written yet. It is allowed be written
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkAllowed(Fee_ChunkInfoConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceSize
 *********************************************************************************************************************/
/*!
 * \brief      Returns the unaligned size of one instance
 * \details    Returns the unaligned size of one instance
 * \param[in]  self
 * \pre        self is not null
 * \return     instanceSize
 * \context    TASK
 * \reentrant  FALSE
 * \note       For data chunks instance size means instance header + instance payload + instance trailer.
 *             For linktable chunks instance size means only payload.
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetInstanceSize(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_getPayload
 *********************************************************************************************************************/
/*!
 * \brief      Returns instance payload size of given chunk
 * \details    Returns instance payload size of given chunk
 * \param[in]  self
 * \pre        self is not null
 * \return     instancePayloadSize
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint16) Fee_ChunkInfo_getPayload(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_unconfirmLink
 *********************************************************************************************************************/
/*!
 * \brief      Sets chunk infos link defective and already written. A defective link is never writeable
 * \details    Sets chunk infos link defective and already written. A defective link is never writeable
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_unconfirmLink(Fee_ChunkInfoRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_confirmEmptyLink
 *********************************************************************************************************************/
/*!
 * \brief      Sets chunk infos link  not defective and not written.
 * \details    Sets chunk infos link  not defective and not written.
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_confirmEmptyLink(Fee_ChunkInfoRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinkConsistent
 *********************************************************************************************************************/
/*!
 * \brief      Checks if chunk infos link is not defective
 * \details    Checks if chunk infos link is not defective
 * \param[in]  self
 * \pre        self is not null
 * \return     FALSE: Link is defective and so inconsistent.
 * \return     TRUE: Link is not defective and so consistent
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkConsistent(Fee_ChunkInfoConstRefType self);


/*************** Instance specific functions **********************************/
/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstanceHeader
 *********************************************************************************************************************/
/*!
 * \brief      Reads instance header from flash into given buffer
 * \details    Uses given chunk info and instance index to determine address and read length
 * \param[in]  self
 * \param[in]  instanceIndex
 * \param[in,out] buffer_au32
 * \pre        self is not null, buffer_au32 is not null
 * \return     FEE_REQ_SYNC_OK: Chunk is not a data chunk, there is no instance header. Fake successful read.
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstanceHeader(Fee_ChunkInfoConstRefType, uint16, Fee_IntBuffer_u32PtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstanceTrailer
 *********************************************************************************************************************/
/*!
 * \brief      Reads instance trailer from flash into given buffer
 * \details    Uses given chunk info and instance index to determine address and read length
 * \param[in]  self
 * \param[in]  instanceIndex
 * \param[in,out] buffer_au32
 * \pre        self is not null, buffer_au32 is not null
 * \return     FEE_REQ_SYNC_OK: Chunk is not a data chunk, there is no instance trailer. Fake successful read.
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstanceTrailer(Fee_ChunkInfoConstRefType, uint16, Fee_IntBuffer_u32PtrType);


/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstancePayload
 *********************************************************************************************************************/
/*!
 * \brief      Reads instance payload according to given offset and length from flash into given buffer
 * \details    Uses given chunk info and instance index to determine address
 * \param[in]  self
 * \param[in]  instanceIndex
 * \param[in,out] buffer_au32
 * \param[in]  offset
 * \param[in]  length
 * \pre        self is not null, buffer_au32 is not null
 * \return     FEE_REQ_SYNC_USELESS: Chunk is not a data chunk, there is no instance payload. Fake successful read.
 * \return     FEE_REQ_ACCEPTED: Flash accepted the request
 * \return     FEE_REQ_REJECTED: Flash rejected the request
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstancePayload(Fee_ChunkInfoConstRefType, uint16, Fee_UserDataPtrType, uint16, uint16);



/**********************************************************************************************************************
 * Fee_ChunkInfo_GetEndAddress
 *********************************************************************************************************************/
/*!
 * \brief      Returns the end address of the given chunk
 * \details    Returns the end address of the given chunk
 * \param[in]  self
 * \pre        self is not null
 * \return     endAddress
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetEndAddress(Fee_ChunkInfoConstRefType self);


/**********************************************************************************************************************
 * Fee_ChunkInfo_CheckHeaderConsistency
 *********************************************************************************************************************/
/*!
 * \brief      Checks if chunk link and the chunk header it points to are consistent
 * \details    Checks if the given chunk info(originated from a link) fits to given buffer (originated from a header)
 * \param[in]  headerBuffer_au32
 * \param[in]  mostRecentChunk_pt
 * \param[in]  searchBlock: expected blockId (originated from link)
 * \param[in]  flsReadOk
 * \pre        headerBuffer_au32 is not null, mostRecentChunk_pt is not null
 * \return     FEE_MNGMT_DEFECTIVE: Header is not consistent
 * \return     FEE_MNGMT_CONSISTENT: Header is consistent
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_CheckHeaderConsistency(Fee_IntBuffer_u32ConstPtrType headerBuffer_au32,
                                                                  Fee_ChunkInfoConstRefType mostRecentChunk_pt,
                                                                  struct Fee_BlockIdStruct searchBlock, Fee_Bool flsReadOk);

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* FEE_CHUNKINFO_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee_ChunkInfo.h
 *********************************************************************************************************************/
