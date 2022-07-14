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
 *         File:  Fee_Sector.c
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  Internal representation of a FEE-Sector. Provides address translation, access to Sector-Header
 *                (including assembly and checks), maintenance of fill level (incl. allocation of space) alignment stuff
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#define FEE_SECTOR_NEED_STRUCT_DEFS

#include "Fee_IntBase.h" /* PRQA S 3313 */ /*MD_FEE_18.1_MissingStructDefinition */
#include "Fee_Sector.h"
#include "Fee_SectorDefs.h"

#include "Fee_PrivateCfg.h" /* Fls API is stord there */
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* We don't do a cross-version check here */

/* index of Erase Cycle within sector header (in 32bit words) */
#define FEE_ERASE_CYCLE_OFFSET_ADDRESS                                (1u)

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_Sector_getAlignmentMask
 *********************************************************************************************************************/
/*!
 * \brief      Creates bit mask for aligning
 * \details    Creates bit mask for aligning
 * \param[in]  alignBits: Number of bits that shall be reset
 * \pre        -
 * \return     alignementMask
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(uint32) Fee_Sector_getAlignmentMask(uint8 alignBits);

/**********************************************************************************************************************
 * Fee_Sector_alignTo
 *********************************************************************************************************************/
/*!
 * \brief      Aligns given value
 * \details    Aligns given value
 * \param[in]  val
 * \param[in]  alignBit
 * \return     alignementMask
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(uint32) Fee_Sector_alignTo(uint32 val, uint8 alignBits);

/**********************************************************************************************************************
 * Fee_Sector_isWriteable
 *********************************************************************************************************************/
/*!
 * \brief      Checks if sector is writeable
 * \details    A sector is writeable when it is usable and not full
 * \param[in]  self
 * \return     FALSE: Sector is not writeable
 * \return     TRUE: Sector is writeable
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Sector_isWriteable(Fee_SectorConstRefType self);

/**********************************************************************************************************************
 * Fee_Sector_getAlignmentMask
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(uint32) Fee_Sector_getAlignmentMask(uint8 alignBits)
{
    return (0xFFFFFFFFuL << alignBits);
}

/**********************************************************************************************************************
 * Fee_Sector_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_Init(Fee_SectorRefType self, Fee_SectorConfigRefType cfg)
{
    struct Fee_SectorInfoStruct sectorInfo = {0};
    sectorInfo.isFull = TRUE;
    sectorInfo.isEmpty = TRUE;
    sectorInfo.cfg_pt = cfg;

    *self = sectorInfo; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_alignTo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(uint32) Fee_Sector_alignTo(uint32 val, uint8 alignBits)
{
    val += ~Fee_Sector_getAlignmentMask(alignBits);
    return (val  & Fee_Sector_getAlignmentMask(alignBits));
}

/**********************************************************************************************************************
 * Fee_Sector_calcFirstFreeAddress
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_calcFirstFreeAddress(Fee_SectorConstRefType self)
{
    /*#10 Compute aligned size of one linktable entry*/
    const Fee_LengthType linkTableEntry = Fee_Sector_alignWrite(self, FEE_SECTOR_LINKTABLE_ELEMENT_SIZE); /* SBSW_FEE_ConstPointer_refParam */

    return Fee_Sector_alignAddress(self, FEE_SECTOR_HEADER_SIZE) /* SBSW_FEE_ConstPointer_refParam */
        + Fee_Sector_alignAddress(self, (Fee_LengthType)(self->linkTableSize_t) * linkTableEntry); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_evaluateHeaderData
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
 */
FEE_INTERN_FUNC(uint8) Fee_Sector_evaluateHeaderData(
    Fee_SectorRefType self,
    Fee_IntBuffer_u32ConstPtrType intBuf,
    Fee_Bool ReadOk)
{
#if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
    const uint32 reservedMask = 0xFFF00000uL;
    const uint8 sectorId = (uint8)(*intBuf & 0xFFu);
#else  /* ECO_DISABLE_GENERATION : only variable definitions */
    const uint32 reservedMask = 0x0000F0FFuL;
    const uint8 sectorId = (uint8)(*intBuf >> 24);
#endif

    /* PRQA S 3344 1 */ /* MD_FEE_13.2_boolean */
    if (ReadOk)
    {
        /*#30 Construct local sector info*/
        struct Fee_SectorInfoStruct sectorInfo;
        sectorInfo = *self;

        if(Fee_Sector_getEraseValueU32(&sectorInfo) == intBuf[0]) /* SBSW_FEE_Func_passParamByReference */
        {
            sectorInfo.isNfaConfirmed_bl = TRUE;
        }
        else if((sectorId != 0xFFu) && (sectorId != 0) && ((*intBuf & reservedMask) == 0)) 
        {
            Fee_LengthType tempNfa;
            /*Lower half of linktable size is bit 3 downto 0 of Byte 2
              Upper half of linktable size is bit 7 downto 0 of Byte 1*/
            const uint16 linkTableSize =
            #if(CPU_BYTE_ORDER == LOW_BYTE_FIRST)  /* ECO_DISABLE_GENERATION : only variable definitions */
                (uint16)(((*intBuf >> 4) & 0xFF0u) | ((*intBuf >> 16) & 0x0F));
            #else  /* ECO_DISABLE_GENERATION : only variable definitions */
                (uint16)(((*intBuf >> 12) & 0xFF0u) | ((*intBuf >> 8) & 0x0F));
            #endif

#if(FEE_GET_ERASE_CYCLE_API == STD_ON)
            sectorInfo.EraseCycleCounter_u32 = intBuf[FEE_ERASE_CYCLE_OFFSET_ADDRESS];
#endif
            sectorInfo.linkTableSize_t = linkTableSize;

            tempNfa = Fee_Sector_calcFirstFreeAddress(&sectorInfo); /* SBSW_FEE_Func_passParamByReference */

            if(tempNfa < (sectorInfo.cfg_pt->logicalRange_t.size_t >> 2))
            {
                sectorInfo.NextFreeAddress_t = tempNfa;
                sectorInfo.isEmpty = FALSE;
                sectorInfo.isNfaConfirmed_bl = FALSE;
                sectorInfo.isFull = FALSE;
            }
        } 
        else 
        {
            /* Nothing to do */
        }

        /*#110 Commit local constructed sector info by assigning it to given pointer*/
        *self = sectorInfo; /* SBSW_FEE_PointerWrite_refParam */
    } /* End of ReadOk check. */

    return sectorId;
}

/**********************************************************************************************************************
 * Fee_Sector_assembleHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_assembleHeader(Fee_SectorRefType self,
                                                              Fee_IntBuffer_u32PtrType headerBuffer_au32,
                                                              uint8 sectorId,
                                                              uint16 linkTableSize)
{
    uint32 firstWord;

    /*#10 Prepare buffer with enough space for sector header*/
    Fee_Sector_ClearBuffer(self, headerBuffer_au32, FEE_SECTOR_HEADER_SIZE); /* SBSW_FEE_PointerWrite_refParam */

    self->linkTableSize_t = linkTableSize; /* SBSW_FEE_InternalDataBuffer_writeAccess */

    /*Sector id is stored in byte 0*/
    /*Linktable size upper half is stored in byte 1 bits 7 downto 0*/
    /*Linktable size lower half is stored in byte 2 bits 3 downto 0*/
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    firstWord = sectorId;
    firstWord |= ((uint32)linkTableSize << 4) & 0xFF00u;
    firstWord |= ((uint32)linkTableSize & 0x0F) << 16;
#else
    firstWord = (uint32)sectorId << 24;
    firstWord |= ((uint32)linkTableSize & 0x0FF0) << 12;
    firstWord |= ((uint32)linkTableSize & 0x0F) << 8;
#endif

    headerBuffer_au32[0] = firstWord; /* SBSW_FEE_InternalDataBuffer_writeAccess */

#if (STD_ON == FEE_GET_ERASE_CYCLE_API)
    ++self->EraseCycleCounter_u32; /* SBSW_FEE_PointerWrite_refParam */
    headerBuffer_au32[1u] = self->EraseCycleCounter_u32; /* SBSW_FEE_InternalDataBuffer_writeAccess */
#endif
}

/**********************************************************************************************************************
 * Fee_Sector_updateFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_updateFillLevel(Fee_SectorRefType self, Fee_LengthType alignedNfa)
{
    if((alignedNfa > self->NextFreeAddress_t))
    {
        struct Fee_SectorInfoStruct sectorInfo;
        sectorInfo = *self;

        sectorInfo.NextFreeAddress_t = alignedNfa;
        sectorInfo.isEmpty = FALSE;
        sectorInfo.isNfaConfirmed_bl = FALSE;

        *self = sectorInfo; /* SBSW_FEE_PointerWrite_refParam */
    }
}

/**********************************************************************************************************************
 * Fee_Sector_confirmFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_confirmFillLevel(Fee_SectorRefType self, Fee_LengthType alignedNfa)
{
    struct Fee_SectorInfoStruct sectorInfo;
    sectorInfo = *self;

    sectorInfo.NextFreeAddress_t = alignedNfa;
    sectorInfo.isNfaConfirmed_bl = TRUE;
    sectorInfo.isEmpty = FALSE;

    *self = sectorInfo; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_allocSpace
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
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_allocSpace(Fee_SectorRefType self, Fee_LengthType byteCount)
{
    Fee_LengthType addr = 0;
    
    if(Fee_Sector_isWriteable(self)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_ConstPointer_refParam */
    {
        struct Fee_SectorInfoStruct si;
        si = *self;

        /* It is safe as long as we're far away from wrap-arounds. With uint32 there is currently no risk (up to 2GBytes are no problem). */
        if(si.cfg_pt->logicalRange_t.size_t >= (byteCount + si.NextFreeAddress_t))
        {
            si.isNfaConfirmed_bl = FALSE;
            addr = si.NextFreeAddress_t;
        }
        else
        {
            si.isFull = TRUE;
        }

        *self = si; /* SBSW_FEE_PointerWrite_refParam */
    }

    return addr;

}

/**********************************************************************************************************************
 * Fee_Sector_erase
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_erase(Fee_SectorConstRefType self)
{
    return (Fee_FlsErase(self->cfg_pt->physicalRange_t.baseAddress_t, self->cfg_pt->physicalRange_t.size_t) == E_OK) ?
                                                                                FEE_REQ_ACCEPTED : FEE_REQ_REJECTED;
}

/**********************************************************************************************************************
 * Fee_Sector_writeData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_writeData(Fee_SectorConstRefType self,
                                                                          Fee_LengthType offset,
                                                                          Fee_IntBuffer_u8ConstPtrType data,
                                                                          Fee_LengthType byteCount)
{
    const uint32 len = Fee_Sector_alignWrite(self, byteCount); /* SBSW_FEE_ConstPointer_refParam */
    return (Fee_FlsWrite(self->cfg_pt->logicalRange_t.baseAddress_t + offset, data, len) == E_OK) /* SBSW_FEE_ConstPointer_refParam */
                        ? FEE_REQ_ACCEPTED : FEE_REQ_REJECTED;
}

/**********************************************************************************************************************
 * Fee_Sector_writeMngmtData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_writeMngmtData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_IntBuffer_u32ConstPtrType data,
                                                                              Fee_LengthType byteCount)
{
    /* PRQA S 310 1 */ /* MD_FEE_11.4_pointer_cast */
    return Fee_Sector_writeData(self, offset, (Fee_UserDataConstPtrType)data, byteCount); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_readData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_readData(Fee_SectorConstRefType self,
                                                                      Fee_LengthType offset,
                                                                      Fee_IntBuffer_u8PtrType data,
                                                                      Fee_LengthType byteCount)
{
    return (Fee_FlsRead(self->cfg_pt->logicalRange_t.baseAddress_t + offset, data, byteCount) == E_OK) /* SBSW_FEE_PointerWrite_refParam */
                        ? FEE_REQ_ACCEPTED : FEE_REQ_REJECTED;
}

/**********************************************************************************************************************
 * Fee_Sector_readMngmtData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_FlsRequestStatus) Fee_Sector_readMngmtData(Fee_SectorConstRefType self,
                                                                              Fee_LengthType offset,
                                                                              Fee_IntBuffer_u32PtrType data,
                                                                              Fee_LengthType byteCount)
{
    /* PRQA S 310 2 */ /* MD_FEE_11.4_pointer_cast */
    return Fee_Sector_readData(self, offset, (Fee_UserDataPtrType)data, byteCount); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_free
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_free(Fee_SectorRefType self)
{
    struct Fee_SectorInfoStruct si;
    si = *self;

    si.isFull = TRUE;
    si.isEmpty = TRUE;
    si.isNfaConfirmed_bl = TRUE;
    si.NextFreeAddress_t = 0;
 
    *self = si; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Sector_reset(Fee_SectorRefType self)
{
    struct Fee_SectorInfoStruct si;
    si = *self;

    si.isFull = FALSE;
    si.isEmpty = TRUE;
    si.isNfaConfirmed_bl = TRUE;
    si.NextFreeAddress_t = Fee_Sector_calcFirstFreeAddress(self); /* SBSW_FEE_ConstPointer_refParam */

    *self = si; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Sector_alignAddress
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_alignAddress(Fee_SectorConstRefType self, uint32 value)
{
    return Fee_Sector_alignTo(value, self->cfg_pt->alignments_t.address_u8);
}

/**********************************************************************************************************************
 * Fee_Sector_alignWrite
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_alignWrite(Fee_SectorConstRefType self, uint32 value)
{
    return Fee_Sector_alignTo(value, self->cfg_pt->alignments_t.write_u8);
}

/**********************************************************************************************************************
 * Fee_Sector_currentLinktableEntryCount
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint16) Fee_Sector_currentLinktableEntryCount(Fee_SectorConstRefType self)
{
    return (uint16)(self->linkTableSize_t);
}

/**********************************************************************************************************************
 * Fee_Sector_getEraseValueU32
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getEraseValueU32(Fee_SectorConstRefType self)
{
    return (self->cfg_pt->eraseValue_u32);
}

/**********************************************************************************************************************
 * Fee_Sector_getSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_getSize(Fee_SectorConstRefType self)
{
    return self->cfg_pt->logicalRange_t.size_t;
}

#if(FEE_GET_ERASE_CYCLE_API == STD_ON)
/**********************************************************************************************************************
 * Fee_Sector_getEraseCycleCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getEraseCycleCounter(Fee_SectorConstRefType self)
{
    return self->EraseCycleCounter_u32;
}
#endif

/**********************************************************************************************************************
 * Fee_Sector_getFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint32) Fee_Sector_getFillLevel(Fee_SectorConstRefType self)

{
    return (!!self->isEmpty) ? 0 : self->NextFreeAddress_t;
}

/**********************************************************************************************************************
 * Fee_Sector_isUsable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isUsable(Fee_SectorConstRefType self)
{
    return !((self->isEmpty) && (self->isFull));
}

/**********************************************************************************************************************
 * Fee_Sector_isDefective
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isDefective(Fee_SectorConstRefType self)
{
    return (!Fee_Sector_isUsable(self)) /* SBSW_FEE_ConstPointer_refParam */
            && (!self->isNfaConfirmed_bl);
}

/**********************************************************************************************************************
 * Fee_Sector_isFillLevelKnown
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_isFillLevelKnown(Fee_SectorConstRefType self)
{
    /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    return ((!Fee_Sector_isUsable(self)) /* SBSW_FEE_ConstPointer_refParam */
            || (self->isNfaConfirmed_bl));
}

/**********************************************************************************************************************
 * Fee_Sector_isWriteable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INLINE_FUNC(Fee_Bool) Fee_Sector_isWriteable(Fee_SectorConstRefType self)
{
    return (Fee_Sector_isUsable(self)) /* SBSW_FEE_ConstPointer_refParam */
            && (!self->isFull);
}

/**********************************************************************************************************************
 * Fee_Sector_AbsAddr2Offset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_LengthType) Fee_Sector_AbsAddr2Offset(Fee_SectorConstRefType self, Fee_SectorAddressType addr)
{
    return addr - self->cfg_pt->logicalRange_t.baseAddress_t;
}

/**********************************************************************************************************************
 * Fee_Sector_Offset2AbsAddr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_SectorAddressType) Fee_Sector_Offset2AbsAddr(Fee_SectorConstRefType self, Fee_LengthType ofs)
{
    return self->cfg_pt->logicalRange_t.baseAddress_t + ofs;
}

/**********************************************************************************************************************
 * Fee_Sector_containsAbsAddr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Sector_containsAbsAddr(Fee_SectorConstRefType self, Fee_SectorAddressType addr)
{
    const Fee_SectorAddressType base = self->cfg_pt->logicalRange_t.baseAddress_t;
    const Fee_SectorAddressType end = base + Fee_Sector_getSize(self); /* SBSW_FEE_ConstPointer_refParam */
    return ((base <= addr) && (addr < end));
}

/**********************************************************************************************************************
 * Fee_Sector_markFull
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

FEE_INTERN_FUNC(void) Fee_Sector_markFull(Fee_SectorRefType self)
{
    struct Fee_SectorInfoStruct s;
    s = *self;

    s.isEmpty = FEE_FALSE;
    s.isFull = FEE_TRUE;
    s.NextFreeAddress_t  = Fee_Sector_getSize(&s); /* SBSW_FEE_ConstPointer_refParam */

    *self = s; /* SBSW_FEE_PointerWrite_refParam */
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Sector.c
 *********************************************************************************************************************/
