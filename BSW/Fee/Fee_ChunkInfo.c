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
 *         File:  Fee_ChunkInfo.c
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
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 3313 incomplete_struct */ /*MD_FEE_18.1_MissingStructDefinition */
#include "Fee_IntBase.h"
#include "Fee_ChunkInfo.h"
#include "Fee_ChunkInfoDefs.h"
/* PRQA L: incomplete_struct */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* We don't do a cross-version check here */

enum Fee_ChunkType /*<! an expressive and type-safe (compared to macros!) definition of possible chunk types */
{
    FEE_CHUNK_LINKTABLE,
    FEE_CHUNK_DATA
};

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetChunkSize
 *********************************************************************************************************************/
/*!
 * \brief      Computes the amount of memory needed by one chunk
 * \details    Computes the amount of memory needed by one chunk
 * \param[in]  chunk
 * \pre        chunk is not null
 * \return     chunkSize
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_LOCAL_FUNC(Fee_LengthType) Fee_ChunkInfo_GetChunkSize( Fee_ChunkInfoConstRefType chunk);

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetAlignedInstanceSize
 *********************************************************************************************************************/
/*!
 * \brief      Returns the aligned size of chunks instance
 * \details    Returns the aligned size of chunks instance. Uses address alignment.
 * \param[in]  chunk
 * \pre        chunk is not null
 * \return     alignedSize
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetAlignedInstanceSize(Fee_ChunkInfoConstRefType);

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetLinkAddress
 *********************************************************************************************************************/
/*!
 * \brief      Returns the address of the link belonging to given chunk
 * \details    Returns the address of the link belonging to given chunk
 * \param[in]  chunk
 * \param[in]  blockId
 * \pre        chunk is not null
 * \return     0: There is no chunk link (because it is out of linktable)
 * \return     linkAddress
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetLinkAddress(Fee_ChunkInfoConstRefType, Fee_BlockIdConstRefType);

/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinktable
 *********************************************************************************************************************/
/*!
 * \brief      Checks whether chunk is linktable chunk or pseudo chunk
 * \details    Checks whether chunk is linktable chunk or pseudo chunk
 * \param[in]  self
 * \pre        self is not null
 * \return     FALSE: Self is data chunk
 * \return     TRUE: Self is linktable chunk or initial linktable
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkTable(Fee_ChunkInfoConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_NewChunkInfo
 *********************************************************************************************************************/
/*!
 * \brief      Fill given chunk info with inital values
 * \details    Initialize a chunk info ov given type and length; without address and no sector assigned.
 * \param[out] self
 * \param[in]  chunk type
 * \param[in]  payload
 * \param[in]  instanceExponent
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_ChunkInfo_NewChunkInfo(Fee_ChunkInfoRefType , enum Fee_ChunkType, uint16, uint8);

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetContentLength
 *********************************************************************************************************************/
/*!
 * \brief      Computes the content length of given chunk
 * \details    Content length is length of one instance * number of instances
 * \param[in]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetContentLength(Fee_ChunkInfoConstRefType);

/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateFromRawData
 *********************************************************************************************************************/
/*!
 * \brief      Creates chunk info out of buffer
 * \details    Creates chunk info out of buffer which contains either chunk header or chunk trailer
 * \param[in,out]  self
 * \param[in]  intBuf
 * \param[in]  ReadOk
 * \pre        self is not null, intBuf is not null
 * \return     FEE_MNGMT_ERASED: chunk header/chunk link is erased
 * \return     FEE_MNGMT_DEFECTIVE: chunk header/chunk link is defective
 * \return     FEE_MNGMT_CONSISTENT: chunk header/ chunk link is consistent
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_CreateFromRawData(Fee_ChunkInfoRefType,
                                                                            Fee_IntBuffer_u32ConstPtrType, Fee_Bool);

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleChunkTypeInfo
 *********************************************************************************************************************/
/*!
 * \brief      Returns chunk info's chunk type for writing it to flash
 * \details    Depending on byte order current value is shifted accordingly
 * \param[in]  self
 * \pre        self is not null
 * \return     chunkTypeInfo
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(uint32) Fee_ChunkInfo_AssembleChunkTypeInfo(Fee_ChunkInfoConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssemblePayloadInfo
 *********************************************************************************************************************/
/*!
 * \brief      Returns chunk info's payload size for writing it to Flash:
 * \details    Depending on byte order current value is shifted accordingly
 * \param[in]  self
 * \pre        self is not null
 * \return     payloadInfo
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(uint32) Fee_ChunkInfo_AssemblePayloadInfo(Fee_ChunkInfoConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_calcSectorLinkTableBytes
 *********************************************************************************************************************/
/*!
 * \brief      Calculates the number of bytes the linktable needs
 * \details    Multiplies the number of linktable entries with the  aligned size of one entry
 * \param[in]  self
 * \pre        self is not null
 * \return     linkTableBytes
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_calcSectorLinkTableBytes(Fee_SectorConstRefType self);

/**********************************************************************************************************************
 * Fee_ChunkInfo_setLinkStatus
 *********************************************************************************************************************/
/*!
 * \brief      Sets status of link within given chunk info
 * \details    Sets defective and written flag according to given values
 * \param[in,out]  self
 * \param[in]  defective
 * \param[in]  written
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(void) Fee_ChunkInfo_setLinkStatus(Fee_ChunkInfoRefType self, Fee_Bool defective, Fee_Bool written);

/**********************************************************************************************************************
 * Fee_ChunkInfo_NewChunkInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_ChunkInfo_NewChunkInfo(Fee_ChunkInfoRefType info_pt, enum Fee_ChunkType chType, uint16 payload, uint8 instanceExponent)
{
    struct Fee_ChunkInfoStruct temp = {0}; /* most values are 0/NULL/FALSE by default - initialize all */

    temp.isDataChunk_bl = (chType == FEE_CHUNK_DATA);
    temp.InstanceExponent_b8 = instanceExponent;
    temp.PayloadSize_b16 = payload;

    *info_pt = temp; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetContentLength
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetContentLength(Fee_ChunkInfoConstRefType chunk_pt)
{
    uint32 retVal = Fee_ChunkInfo_GetAlignedInstanceSize(chunk_pt); /* SBSW_FEE_ConstPointer_refParam */

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(chunk_pt->isDataChunk_bl)
    {
        retVal = (retVal << chunk_pt->InstanceExponent_b8) - retVal;
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetAlignedInstanceSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetAlignedInstanceSize(Fee_ChunkInfoConstRefType self)
{
    return Fee_Sector_alignAddress(self->Sector_pt, Fee_ChunkInfo_GetInstanceSize(self)); /* SBSW_FEE_ConstPointer_refParam */ /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetChunkSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_LOCAL_FUNC(Fee_LengthType) Fee_ChunkInfo_GetChunkSize(Fee_ChunkInfoConstRefType chunk)
{
    const Fee_SectorConstRefType sector = chunk->Sector_pt;
    
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    return (chunk->isPseudoChunk_bl) ? 
        (Fee_Sector_calcFirstFreeAddress(Fee_ChunkInfo_GetSector(chunk))) /* SBSW_FEE_ConstPointer_refParam */ /* SBSW_FEE_ConstPointer_refParam */
           : (Fee_Sector_alignAddress(sector, FEE_CHUNK_HEADER_SIZE)           /* SBSW_FEE_ConstPointer_refParam */
               + Fee_ChunkInfo_GetContentLength(chunk)                          /* SBSW_FEE_ConstPointer_refParam */
               + Fee_Sector_alignAddress(sector, FEE_CHUNK_TRAILER_SIZE));      /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceCount
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(uint16) Fee_ChunkInfo_GetInstanceCount(Fee_ChunkInfoConstRefType self)
{
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    return (uint16)((self->isDataChunk_bl) ? ((1u << self->InstanceExponent_b8) - 1u) : 1u);
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceStartAddress
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetInstanceStartAddress(
                                                    Fee_ChunkInfoConstRefType chunk, uint16 instanceIndex)
{
    Fee_LengthType retVal;
    const Fee_SectorConstRefType sector = chunk->Sector_pt;

    if(chunk->isPseudoChunk_bl)  /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        retVal = (instanceIndex == 0) ? 
            Fee_Sector_alignAddress(sector, FEE_SECTOR_HEADER_SIZE) /* SBSW_FEE_ConstPointer_refParam */
                : (Fee_Sector_calcFirstFreeAddress(sector)); /* SBSW_FEE_ConstPointer_refParam */
    }
    else
    {
        retVal = Fee_ChunkInfo_GetBaseAddress(chunk) /* SBSW_FEE_ConstPointer_refParam */
               + Fee_Sector_alignAddress(sector, FEE_CHUNK_HEADER_SIZE) /* SBSW_FEE_ConstPointer_refParam */
               + (Fee_ChunkInfo_GetAlignedInstanceSize(chunk) * instanceIndex); /* SBSW_FEE_ConstPointer_refParam */
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetLinkAddress
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
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_GetLinkAddress(Fee_ChunkInfoConstRefType chunk,
                                                                     Fee_BlockIdConstRefType blockId)
{
    Fee_LengthType retVal = Fee_ChunkInfo_GetInstanceStartAddress(chunk, 0); /* SBSW_FEE_ConstPointer_refParam */

    /* PRQA S 3344 3 */ /* MD_FEE_13.2_boolean */
    if((!chunk->isDataChunk_bl) && (blockId != NULL_PTR))
    {
        const Fee_BitFieldType index = (Fee_BitFieldType)((!!chunk->isPseudoChunk_bl) ? blockId->Tag : blockId->DataIndex);
        const Fee_LengthType ofs = Fee_Sector_alignWrite(chunk->Sector_pt, FEE_CHUNK_TRAILER_SIZE) * index; /* SBSW_FEE_ConstPointer_refParam */

        if(ofs < (Fee_LengthType)chunk->PayloadSize_b16)
        {
            retVal += ofs;
        }
        else
        {
            retVal = 0;
        }
    }
    else
    {
        retVal += Fee_ChunkInfo_GetContentLength(chunk); /* SBSW_FEE_ConstPointer_refParam */
    }
    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateFromRawData
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
FEE_INLINE_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_CreateFromRawData(
                                            Fee_ChunkInfoRefType self,
                                            Fee_IntBuffer_u32ConstPtrType intBuf,
                                            Fee_Bool readOk)
{
    const Fee_SectorConstRefType sector = self->Sector_pt;
    enum Fee_MngmtStatus mngmtStatus = FEE_MNGMT_DEFECTIVE;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(readOk)
    {
        const uint32 eraseVal = Fee_Sector_getEraseValueU32(sector); /* SBSW_FEE_ConstPointer_refParam */
        if((eraseVal == intBuf[0]) && (eraseVal == intBuf[1]))
        {
            mngmtStatus = FEE_MNGMT_ERASED;
        }
        else
        {
            Fee_Bool instExpOk;
            const Fee_LengthType sectorEnd = Fee_Sector_getSize(sector) - 1u; /* SBSW_FEE_ConstPointer_refParam */
            Fee_LengthType chunkEndAddress;

            /*Byte 3 bit 7 downto 5 and Byte 6 and 7 are the reserved Bits (must be 0)
              Byte 3 bit 4 signals the chunk type ( 1 means linktable chunk)
              Byte 3 Bit 3 downto 0 is the instance exponent
              Byte 4 and 5 are the payload size*/
#      if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
            const uint32 reservedBits = (intBuf[0] & 0xE0000000uL) | (intBuf[1] & 0x00FF0000uL);
            const uint32 linkChunkFlag= (intBuf[0] & 0x10000000uL);
            const uint8 instExp       = (uint8)((intBuf[0] >> 24) & 0x0Fu);
            const uint16 payload      = (uint16)(intBuf[1] & 0xFFFFu);
#      else
            const uint32 reservedBits = (intBuf[0] & 0x00000E0uL) | (intBuf[1] & 0x0000FF00uL);
            const uint32 linkChunkFlag= (intBuf[0] & 0x0000010uL);
            const uint8 instExp       = (uint8)(intBuf[0] & 0x0Fu);
            const uint16 payload      = (uint16)((intBuf[1] >> 16) & 0xFFFFu);

#       endif
            
            /*#60 Initialize chunk info with parsed values*/
            /* we cannot use our "constructors" (NewChunkInfo) as don't want "create" an object,  *
             * but we want to initialize it (complete initialization)                             */
            self->PayloadSize_b16 = payload;              /* SBSW_FEE_PointerWrite_refParam */
            self->isDataChunk_bl = (linkChunkFlag == 0);  /* SBSW_FEE_PointerWrite_refParam */
            self->InstanceExponent_b8 = instExp;          /* SBSW_FEE_PointerWrite_refParam */

            chunkEndAddress = Fee_ChunkInfo_GetEndAddress(self); /* SBSW_FEE_ConstPointer_refParam */

            instExpOk = (((self->isDataChunk_bl) && (instExp > 0)) ||
                         ((!self->isDataChunk_bl) && (instExp == 0)));

            /*#70 Check if reserved bits, payload size, instance exponent and chunk start and end address are plausible*/
            /* Note: ChunkEndAddress, calculated from Base + Size, cannot overflow (32bit arithmetic)! */
            /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
            if((reservedBits == 0) && (self->PayloadSize_b16 > 0) && (instExpOk) &&
                (self->ChunkBaseOffset_t < sectorEnd ) &&
                (chunkEndAddress <= sectorEnd))
            {
                /*#80 If all checks passed, set status consistent*/
                mngmtStatus = FEE_MNGMT_CONSISTENT;
            }
        } /* erase value check */
    } /* readError */

    return mngmtStatus;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateNewLinktableChunk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_CreateNewLinktableChunk(Fee_ChunkInfoRefType self, uint8 datasetCount)
{
    Fee_ChunkInfo_NewChunkInfo(self, FEE_CHUNK_LINKTABLE, 0, datasetCount); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_CreateNewDataChunk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_CreateNewDataChunk(Fee_ChunkInfoRefType self,
                                                               uint16 payload,
                                                               uint8 instanceExponent)
{
    Fee_ChunkInfo_NewChunkInfo((self), FEE_CHUNK_DATA, payload, instanceExponent); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssignSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_AssignSector(Fee_ChunkInfoRefType self, Fee_SectorRefType sector_pt)
{
    self->Sector_pt = sector_pt; /* SBSW_FEE_PointerWrite_refParam */
    
    if(!self->isDataChunk_bl)
    {
        /*Compute linktable chunks payload size: number of dataset (it is stored in instance exponent) * chunk trailer size
          Cast to uint8 is actually unnecessary, but without QAC complained (incorrectly!) */
        self->PayloadSize_b16 = /* SBSW_FEE_PointerWrite_refParam */
                (uint16)((uint8)self->InstanceExponent_b8
                * Fee_Sector_alignWrite(sector_pt, FEE_CHUNK_TRAILER_SIZE)); /* SBSW_FEE_ConstPointer_refParam */
    }

    self->ChunkBaseOffset_t = /* SBSW_FEE_PointerWrite_refParam */
            Fee_Sector_allocSpace(sector_pt, Fee_ChunkInfo_GetChunkSize(self)); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */

    return (self->ChunkBaseOffset_t != 0);
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_InitFromLinkData
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
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_InitFromLinkData(Fee_ChunkInfoRefType self,
                                                                     Fee_ChunkInfoConstRefType referrer_pt,
                                                                     Fee_IntBuffer_u32ConstPtrType intBuf,
                                                                     Fee_Bool ReadOk)

{
    enum Fee_MngmtStatus linkStatus;
    const Fee_SectorRefType sector = referrer_pt->Sector_pt;
    /* Lower part is byte 3 downto 0 and higher part is byte 7. Mind byte order*/
    Fee_SectorAddressType linkAddress =
#   if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
        (Fee_SectorAddressType)((intBuf[0] & 0x00FFFFFFuL) | (intBuf[1] & 0xFF000000uL));
#   else  /* ECO_DISABLE_GENERATION : FSM condition check. No test necessary. */
        (Fee_SectorAddressType)((intBuf[0] >> 8) | (intBuf[1] << 24));
#   endif

    Fee_ChunkInfo_Clone(self, referrer_pt); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */
    Fee_ChunkInfo_SwitchToSuccessor(self);  /* SBSW_FEE_PointerWrite_refParam */

    linkStatus = Fee_ChunkInfo_CreateFromRawData(self, intBuf, ReadOk); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */

    if(linkStatus == FEE_MNGMT_CONSISTENT)
    {
        const Fee_LengthType base = self->ChunkBaseOffset_t;

        self->ChunkBaseOffset_t = /* SBSW_FEE_PointerWrite_refParam */
                Fee_Sector_AbsAddr2Offset(sector, linkAddress)             /* SBSW_FEE_ConstPointer_refParam */
                 - Fee_ChunkInfo_GetContentLength(self)                    /* SBSW_FEE_ConstPointer_refParam */
                 - Fee_Sector_alignAddress(sector, FEE_CHUNK_HEADER_SIZE); /* SBSW_FEE_ConstPointer_refParam */

        linkAddress += Fee_Sector_alignAddress(sector, FEE_CHUNK_TRAILER_SIZE); /* SBSW_FEE_ConstPointer_refParam */
        /* PRQA S 3415 2 */ /* MD_FEE_12.4_queries */
        if((!Fee_Sector_containsAbsAddr(sector, linkAddress - 1u))          /* SBSW_FEE_ConstPointer_refParam */
           || (Fee_Sector_alignAddress(sector, linkAddress) != linkAddress) /* SBSW_FEE_ConstPointer_refParam */
           || (base > self->ChunkBaseOffset_t)
           || (self->ChunkBaseOffset_t >= Fee_Sector_AbsAddr2Offset(sector, linkAddress))) /* SBSW_FEE_ConstPointer_refParam */
        {
            linkStatus = FEE_MNGMT_DEFECTIVE;
        }
        else
        {
            Fee_Sector_updateFillLevel(sector, Fee_ChunkInfo_GetEndAddress(self) + 1u); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */
        }
    }

    return linkStatus;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */

/* Reserved Bits in a chunk header. Defining it here is better for ECO (coverage analysis). */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#    define FEE_CHUNKINFO_HEADER_MASK 0xFFFF0000uL
#else
#    define FEE_CHUNKINFO_HEADER_MASK 0x0000FFFFuL
#endif

/**********************************************************************************************************************
 * Fee_ChunkInfo_EvaluateHeaderData
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
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_EvaluateHeaderData(
                                                                        Fee_ChunkInfoRefType self,
                                                                        Fee_BlockIdRefType blockId_pt,
                                                                        Fee_IntBuffer_u32ConstPtrType intBuf_au32,
                                                                        Fee_Bool ReadOk)
{
    enum Fee_MngmtStatus headerStatus = FEE_MNGMT_CONSISTENT;

    if(!self->isPseudoChunk_bl)
    {
        /*#30 Create a backup chunk to restore later in case link was defective*/
        const Fee_SectorRefType sector = self->Sector_pt;
        struct Fee_ChunkInfoStruct backupChunk;

        Fee_ChunkInfo_Clone(&backupChunk, self); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */

        headerStatus =
            Fee_ChunkInfo_CreateFromRawData(self, intBuf_au32, ReadOk); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */

        if(headerStatus == FEE_MNGMT_CONSISTENT)
        {
            const uint32 headerMask = FEE_CHUNKINFO_HEADER_MASK;

    #if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
            blockId_pt->Tag = (uint16)(intBuf_au32[0] & 0xFFFFu);             /* SBSW_FEE_PointerWrite_refParam */
            blockId_pt->DataIndex = (uint8)((intBuf_au32[0] >> 16) & 0xFFu);  /* SBSW_FEE_PointerWrite_refParam */
    #else
            blockId_pt->Tag = (uint16)(intBuf_au32[0] >> 16);                 /* SBSW_FEE_PointerWrite_refParam */
            blockId_pt->DataIndex  = (uint8)((intBuf_au32[0] >> 8) & 0xFFu);  /* SBSW_FEE_PointerWrite_refParam */
    #endif
            
            if((intBuf_au32[1] & headerMask) != 0)
            {
                headerStatus = FEE_MNGMT_DEFECTIVE;
            }
            
            if(Fee_ChunkInfo_isLinkTable(self)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_ConstPointer_refParam */
            {
                const uint32 linkTablePayload = blockId_pt->DataIndex * Fee_Sector_alignWrite(sector, FEE_CHUNK_TRAILER_SIZE); /* SBSW_FEE_ConstPointer_refParam */

                blockId_pt->DataIndex--; /* SBSW_FEE_PointerWrite_refParam */

                if(((uint32)self->PayloadSize_b16) != linkTablePayload)
                {
                    headerStatus = FEE_MNGMT_DEFECTIVE;
                }
            }
        }

        /*#60 Continue according to header status*/
        switch(headerStatus)
        {
            case FEE_MNGMT_ERASED: 
                break;
            case FEE_MNGMT_CONSISTENT:
                Fee_Sector_updateFillLevel(sector, Fee_ChunkInfo_GetEndAddress(self) + 1u); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */
                break;
            default:
                Fee_ChunkInfo_Clone(self, &backupChunk); /* SBSW_FEE_ConstPointer_refParam */
                self->ChunkBaseOffset_t += /* SBSW_FEE_PointerWrite_refParam */
                        Fee_Sector_alignAddress(sector, FEE_CHUNK_HEADER_SIZE); /* SBSW_FEE_ConstPointer_refParam */
            
                Fee_Sector_updateFillLevel(sector, self->ChunkBaseOffset_t); /* SBSW_FEE_PointerWrite_refParam */
                break;
        }
    }
     
    return headerStatus;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleChunkTypeInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(uint32) Fee_ChunkInfo_AssembleChunkTypeInfo(Fee_ChunkInfoConstRefType self)
{
    uint32 retVal = 0x10;
    if(self->isDataChunk_bl) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        retVal = self->InstanceExponent_b8;
    }
#if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    retVal <<= 24;
#endif

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssemblePayloadInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(uint32) Fee_ChunkInfo_AssemblePayloadInfo(Fee_ChunkInfoConstRefType self)
{
    uint32 retVal = (uint32)self->PayloadSize_b16;
#if(CPU_BYTE_ORDER != LOW_BYTE_FIRST) /* only distinguish LOW_BYTE_FIRST and !LOW_BYTE_FIRST -> use only one name */
    retVal <<= 16;
#endif
    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_AssembleHeader(Fee_ChunkInfoConstRefType self,
                                                          Fee_BlockIdConstRefType blockId_pt, 
                                                          Fee_IntBuffer_u32PtrType buffer_au32)
{
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    /*For data chunks index field is just the data index
      For linktable chunks index field is the number of datasets. It was stored as instance exponent*/
    const uint32 dataIndexField = (uint32)((!!self->isDataChunk_bl) ? blockId_pt->DataIndex : self->InstanceExponent_b8);
    uint32 temp;

    Fee_Sector_ClearBuffer(Fee_ChunkInfo_GetSector(self), buffer_au32, FEE_CHUNK_HEADER_SIZE_WRITE); /* SBSW_FEE_ConstPointer_refParam */

    /*BlockTag is Byte 1 and 0, DataIndex Byte 2*/
    #if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
        temp = (uint32)blockId_pt->Tag  | (dataIndexField << 16);
    #else  /* ECO_DISABLE_GENERATION : only variable definitions */
        temp = ((uint32)blockId_pt->Tag << 16) | (dataIndexField << 8);
    #endif

    /*Chunk type is Byte 3 bit 4*/
    /* PRQA S 3344 2 */ /* MD_FEE_13.2_boolean */
    buffer_au32[0] = /* SBSW_FEE_InternalDataBuffer_writeAccess */
                temp | Fee_ChunkInfo_AssembleChunkTypeInfo(self); /* SBSW_FEE_ConstPointer_refParam */
    /*Payload size is byte 5 and 4*/
    buffer_au32[1] = /* SBSW_FEE_InternalDataBuffer_writeAccess */
            Fee_ChunkInfo_AssemblePayloadInfo(self); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_AssembleLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_AssembleLink(Fee_ChunkInfoConstRefType self, Fee_IntBuffer_u32PtrType buffer_au32)
{
    const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self); /* SBSW_FEE_ConstPointer_refParam */
    /*#10 Get current chunk links address*/
    /* passing no blockId means "current chunk's link address, rather than some specific link-table index */
    const Fee_SectorAddressType link = Fee_Sector_Offset2AbsAddr(sector,                  /* SBSW_FEE_ConstPointer_refParam */
                                           Fee_ChunkInfo_GetLinkAddress(self, NULL_PTR)); /* SBSW_FEE_ConstPointer_refParam */
    uint32 temp0, temp1;

    /*#20 Clear the buffer*/
    Fee_Sector_ClearBuffer(sectr , buffer, FEE_CHUNK_TRAILER_SIZE);

    /*#30 Assemble bytes of chunk link*/
#if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
        /*Chunk links lower part (byte 2 downto 0) is byte 2 downto 0*/
        temp0 = (0x00FFFFFFuL & link);
        
        /*Chunk links upper part (byte 3) is byte 7*/
        temp1 = (0xFF000000uL & link);
    #else  /* ECO_DISABLE_GENERATION : only variable definitions */
        temp0 = link << 8;
        /* put highest address byte into last byte of chunk link */
        temp1 = (link >> 24);
    #endif

    /*Chunk type is byte 3 bit 4*/
    /* PRQA S 3344 2 */ /* MD_FEE_13.2_boolean */
    buffer_au32[0] = /* SBSW_FEE_InternalDataBuffer_writeAccess */
            temp0 | Fee_ChunkInfo_AssembleChunkTypeInfo(self); /* SBSW_FEE_ConstPointer_refParam */
    /*Payload size is byte 5 and 4*/
    buffer_au32[1] = /* SBSW_FEE_InternalDataBuffer_writeAccess */
            temp1 | Fee_ChunkInfo_AssemblePayloadInfo(self); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstanceHeader
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
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstanceHeader(
                            Fee_ChunkInfoConstRefType self, uint16 instanceIndex, Fee_IntBuffer_u32PtrType buffer)
{
    enum Fee_FlsRequestStatus result = FEE_REQ_SYNC_OK;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(self->isDataChunk_bl)
    {
        const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self);                    /* SBSW_FEE_ConstPointer_refParam */
        const Fee_LengthType addr = Fee_ChunkInfo_GetInstanceStartAddress(self, instanceIndex); /* SBSW_FEE_ConstPointer_refParam */
        const Fee_LengthType readLen = Fee_Sector_alignWrite(sector, FEE_INSTANCE_HEADER_SIZE); /* SBSW_FEE_ConstPointer_refParam */

        /* We have to prepare data buffer's first 4 bytes.
         * It is important for the case "Fls page size < 4"
         * Otherwise (if page size > 4 Bytes), we expect 2^n bytes to be read
         *  => we will always have multiples of 32bits (it is important during checks) */
        buffer[0] = /* SBSW_FEE_InternalDataBuffer_writeAccess */
                Fee_Sector_getEraseValueU32(sector); /* SBSW_FEE_ConstPointer_refParam */

        result = Fee_Sector_readMngmtData(sector, addr, buffer, readLen); /* SBSW_FEE_ConstPointer_refParam */
    }

    return result;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstanceTrailer
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
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstanceTrailer(Fee_ChunkInfoConstRefType self,
                                                                                    uint16 instanceIndex,
                                                                                    Fee_IntBuffer_u32PtrType buffer_au32)
{
    enum Fee_FlsRequestStatus result = FEE_REQ_SYNC_OK;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(self->isDataChunk_bl)
    {
        const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self); /* SBSW_FEE_ConstPointer_refParam */
        const Fee_LengthType addr = Fee_ChunkInfo_GetInstanceStartAddress(self, instanceIndex) /* SBSW_FEE_ConstPointer_refParam */
            + Fee_ChunkInfo_getPayload(self) + FEE_INSTANCE_HEADER_SIZE; /* SBSW_FEE_ConstPointer_refParam */

        result = Fee_Sector_readMngmtData(sector, addr, buffer_au32, FEE_INSTANCE_TRAILER_SIZE); /* SBSW_FEE_ConstPointer_refParam */
    }

    return result;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadInstancePayload
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
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadInstancePayload(Fee_ChunkInfoConstRefType self ,
                                                                                    uint16 instanceIndex,
                                                                                    Fee_UserDataPtrType buffer_au8, uint16 offset, uint16 length)
{
    enum Fee_FlsRequestStatus result = FEE_REQ_SYNC_USELESS;

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if(self->isDataChunk_bl)
    {
        Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self); /* SBSW_FEE_ConstPointer_refParam */
        const Fee_LengthType addr = Fee_ChunkInfo_GetInstanceStartAddress(self, instanceIndex) /* SBSW_FEE_ConstPointer_refParam */
            + FEE_INSTANCE_HEADER_SIZE + offset;

        result = Fee_Sector_readData(sector, addr, buffer_au8, length); /* SBSW_FEE_ConstPointer_refParam */
    }

    return result;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_Construct
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Construct(Fee_ChunkInfoRefType self, Fee_SectorRefType sector_pt)
{
    struct Fee_ChunkInfoStruct temp = {0};

    /* set the few values that need to differ from 0/NULL/FALSE */
    temp.Sector_pt = sector_pt;
    temp.PayloadSize_b16 =
        (uint16)Fee_ChunkInfo_calcSectorLinkTableBytes(sector_pt); /* SBSW_FEE_ConstPointer_refParam */

    temp.isPseudoChunk_bl = FEE_TRUE;

    *self = temp; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadChunkHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadChunkHeader(Fee_ChunkInfoConstRefType self,
                                                                                       Fee_IntBuffer_u32PtrType buffer_au32)
{
    enum Fee_FlsRequestStatus retVal;
    
    if(self->isPseudoChunk_bl) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {

        retVal = FEE_REQ_SYNC_USELESS; 
    }
    else
    {
        retVal = Fee_Sector_readMngmtData(Fee_ChunkInfo_GetSector(self), /* SBSW_FEE_ConstPointer_refParam */
                                    Fee_ChunkInfo_GetBaseAddress(self),  /* SBSW_FEE_ConstPointer_refParam */
                                    buffer_au32,                         /* SBSW_FEE_InternalDataBuffer_writeAccess */
                                    FEE_CHUNK_HEADER_SIZE_READ);
    }
    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_ReadChunkLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_ReadChunkLink(Fee_ChunkInfoConstRefType self,
                                                                                     Fee_BlockIdConstRefType blockId,
                                                                                     Fee_IntBuffer_u32PtrType buffer_au32)
{
    enum Fee_FlsRequestStatus retVal;
    const Fee_LengthType linkAddress = Fee_ChunkInfo_GetLinkAddress(self, blockId); /* SBSW_FEE_ConstPointer_refParam */

    /*LinkAddress 0 means block has no linktable entry and it is initial link*/
    if(linkAddress != 0)
    {
        retVal = Fee_Sector_readMngmtData(Fee_ChunkInfo_GetSector(self), /* SBSW_FEE_ConstPointer_refParam */
                                    linkAddress,
                                    buffer_au32, /* SBSW_FEE_InternalDataBuffer_writeAccess */
                                    FEE_CHUNK_HEADER_SIZE_READ);
    }
    else
    {
        retVal = FEE_REQ_SYNC_USELESS;
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_SwitchToSuccessor
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void ) Fee_ChunkInfo_SwitchToSuccessor(Fee_ChunkInfoRefType self)
{
    struct Fee_ChunkInfoStruct temp;
    temp = *self;

    temp.ChunkBaseOffset_t += Fee_ChunkInfo_GetChunkSize(self); /* SBSW_FEE_ConstPointer_refParam */
    temp.PayloadSize_b16 = 1u;
    temp.InstanceExponent_b8 = 1u;
    temp.isPseudoChunk_bl = FEE_FALSE;

    *self = temp; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_Equals
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool)  Fee_ChunkInfo_Equals(Fee_ChunkInfoConstRefType self, Fee_ChunkInfoConstRefType other)
{
    return (
        (self->Sector_pt == other->Sector_pt) &&
        (self->ChunkBaseOffset_t == other->ChunkBaseOffset_t) &&
        (self->InstanceExponent_b8 == other->InstanceExponent_b8) &&
        (self->isDataChunk_bl == other->isDataChunk_bl) &&
        (self->PayloadSize_b16 == other->PayloadSize_b16));
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_WriteHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_WriteHeader(Fee_ChunkInfoConstRefType self,
                                                                                   Fee_IntBuffer_u32ConstPtrType buffer_au32)
{
    const Fee_LengthType address = Fee_ChunkInfo_GetBaseAddress(self);   /* SBSW_FEE_ConstPointer_refParam */
    const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self); /* SBSW_FEE_ConstPointer_refParam */

    /*Linktable chunk header must be always written with FEE_CHUNK_HEADER_SIZE_READ byte length*/
    Fee_LengthType len = FEE_CHUNK_HEADER_SIZE_READ;
#if(FEE_GET_WRITE_CYCLE_API == STD_ON)

    if(self->isDataChunk_bl) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {   /*Data chunk header must be written with FEE_CHUNK_HEADER_SIZE length, when wcc api is active*/
        len = FEE_CHUNK_HEADER_SIZE;
    }
#endif

    return Fee_Sector_writeMngmtData(sector, address, buffer_au32, len); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_WriteChunkLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_ChunkInfo_WriteChunkLink(Fee_ChunkInfoConstRefType self,
                                                                                     Fee_BlockIdConstRefType blockId_pt,
                                                                                     Fee_IntBuffer_u32ConstPtrType buffer_au32)
{
    enum Fee_FlsRequestStatus retVal;
    /*#10 Get address where link shall be written to*/
    const Fee_LengthType address = Fee_ChunkInfo_GetLinkAddress(self, blockId_pt); /* SBSW_FEE_ConstPointer_refParam */
    
    /* LinkAddress 0 means block has no link-table entry and it is initial link*/
    /* PRQA S 3415 1 */ /* MD_FEE_12.4_queries */
    if((address > 0) && (Fee_ChunkInfo_isLinkAllowed(self) == FEE_TRUE)) /* SBSW_FEE_ConstPointer_refParam */
    {
        const Fee_SectorConstRefType sector = Fee_ChunkInfo_GetSector(self); /* SBSW_FEE_ConstPointer_refParam */
        retVal = Fee_Sector_writeMngmtData(sector, address, buffer_au32, FEE_CHUNK_TRAILER_SIZE); /* SBSW_FEE_ConstPointer_refParam */
    }
    else
    {
        retVal = FEE_REQ_SYNC_OK;
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_Clone
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Clone(Fee_ChunkInfoRefType target, Fee_ChunkInfoConstRefType source)
{
    *target = *source; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_Clear
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_Clear(Fee_ChunkInfoRefType self)
{
    struct Fee_ChunkInfoStruct NullChunk = {0};
    Fee_ChunkInfo_Clone(self, &NullChunk); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_ChunkInfo_GetSector(Fee_ChunkInfoConstRefType self)
{
    return self->Sector_pt;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetBaseAddress
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetBaseAddress(Fee_ChunkInfoConstRefType self)
{
    return self->ChunkBaseOffset_t;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_isDataChunk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isDataChunk(Fee_ChunkInfoConstRefType self)
{
    return !!self->isDataChunk_bl; /* !! makes a a boolean expression -> no casts, etc */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinkTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkTable(Fee_ChunkInfoConstRefType self)
{
    return !Fee_ChunkInfo_isDataChunk(self); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_isInitialLinkTable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isInitialLinkTable(Fee_ChunkInfoConstRefType self)
{
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    return (Fee_ChunkInfo_isLinkTable(self)) && (self->isPseudoChunk_bl); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinkAllowed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkAllowed(Fee_ChunkInfoConstRefType self)
{
    return !self->wasLinkWritten_bl;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_isLinkConsistent
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_ChunkInfo_isLinkConsistent(Fee_ChunkInfoConstRefType self)
{
    return !self->isLinkDefective_bl;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_confirmEmptyLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_confirmEmptyLink(Fee_ChunkInfoRefType self)
{
    Fee_ChunkInfo_setLinkStatus(self, FEE_FALSE, FEE_FALSE); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_unconfirmLink
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_ChunkInfo_unconfirmLink(Fee_ChunkInfoRefType self)
{
    /* an "unconfirmed", i.e. defective, link cannot be writable! */
    Fee_ChunkInfo_setLinkStatus(self, FEE_TRUE, FEE_TRUE); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_getPayload
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint16) Fee_ChunkInfo_getPayload(Fee_ChunkInfoConstRefType self)
{
    return (uint16)self->PayloadSize_b16;
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetInstanceSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetInstanceSize(Fee_ChunkInfoConstRefType self)
{
    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    const Fee_LengthType managementLength = (Fee_LengthType)((self->isDataChunk_bl)
            ? (FEE_INSTANCE_HEADER_SIZE + FEE_INSTANCE_TRAILER_SIZE) : 0u);

    return managementLength + Fee_ChunkInfo_getPayload(self); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_GetEndAddress
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_ChunkInfo_GetEndAddress(Fee_ChunkInfoConstRefType self)
{
    return Fee_ChunkInfo_GetBaseAddress(self) /* SBSW_FEE_ConstPointer_refParam */
            + (Fee_ChunkInfo_GetChunkSize(self) /* SBSW_FEE_ConstPointer_refParam */
            - 1u);
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_SetLinkStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(void) Fee_ChunkInfo_setLinkStatus(Fee_ChunkInfoRefType self, Fee_Bool defective, Fee_Bool written)
{
        self->wasLinkWritten_bl = (written == FEE_TRUE);    /* SBSW_FEE_PointerWrite_refParam */
        self->isLinkDefective_bl = (defective == FEE_TRUE); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_calcSectorLinkTableBytes
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_LengthType) Fee_ChunkInfo_calcSectorLinkTableBytes(Fee_SectorConstRefType self)
{
    return (Fee_Sector_currentLinktableEntryCount(self) /* SBSW_FEE_ConstPointer_refParam */
                * Fee_Sector_alignWrite(self, FEE_SECTOR_LINKTABLE_ELEMENT_SIZE)); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_ChunkInfo_CheckHeaderConsistency
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_MngmtStatus) Fee_ChunkInfo_CheckHeaderConsistency(Fee_IntBuffer_u32ConstPtrType headerBuffer_au32,
                                                                  Fee_ChunkInfoConstRefType mostRecentChunk_pt,
                                                                  struct Fee_BlockIdStruct searchBlock, Fee_Bool flsReadOk)
{
    struct Fee_BlockIdStruct blockId_tloc;
    struct Fee_ChunkInfoStruct chunk;
    enum Fee_MngmtStatus headerStatus = FEE_MNGMT_DEFECTIVE;
    Fee_ChunkInfo_Clone(&chunk, mostRecentChunk_pt); /* SBSW_FEE_PointerWrite_refParam */

    /* PRQA S 3415 4 */ /* MD_FEE_12.4_queries */
    if((Fee_ChunkInfo_EvaluateHeaderData(&chunk, &blockId_tloc, headerBuffer_au32, flsReadOk) == FEE_MNGMT_CONSISTENT)
                    && ((searchBlock.Tag) == (blockId_tloc.Tag))
                    && (((searchBlock.DataIndex) == (blockId_tloc.DataIndex)) || (Fee_ChunkInfo_isLinkTable(&chunk))) /* SBSW_FEE_ConstPointer_refParam */
                    && (Fee_ChunkInfo_Equals(&chunk, mostRecentChunk_pt))) /* SBSW_FEE_ConstPointer_refParam */ /* SBSW_FEE_ConstPointer_refParam */
    {
        headerStatus = FEE_MNGMT_CONSISTENT;
    }

    return headerStatus;
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Fee_ChunkInfo.c
 *********************************************************************************************************************/
