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
 *         File:  Fee_Partition.c
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
#include "Fee_IntBase.h"
#include "Fee_Sector.h"
#include "Fee_PrivateCfg.h"

#include "Fee_Partition.h"
#include "Fee_PartitionDefs.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* We don't do a cross-version check here */

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Fee_Partition_determineRealFillLevel
 *********************************************************************************************************************/
/*!
 * \brief      Compute the fill level of given partition by evaluating its sector infos
 * \details    Compute the fill level of given partition by evaluating its sector infos
 * \param[in]  self
 * \return     FEE_PART_FILL_LEVEL_FSS: FSS threshold exceeded or both sectors need to be formated, FSS
 *             must be executed
 * \return     FEE_PART_FILL_LEVEL_BSS: BSS threshold exceeded, BSS can be executed
 * \return     FEE_PART_FILL_LEVEL_NORMAL: Both sectors in use, no threshold exceeded, no sector switch necessary
 * \return     FEE_PART_FILL_LEVEL_SINGLE_SECTOR: Only one sector in use, no sector switch necessary
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INLINE_FUNC(enum Fee_Partition_FillLevelEnum) Fee_Partition_determineRealFillLevel(Fee_PartitionConstRefType self);

/**********************************************************************************************************************
 * Fee_Partition_init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Partition_init(Fee_ConfigRefTypeInternal cfg, sint8_least idx)
{
    struct Fee_PartitionsRamDataStruct prd = {0};

    /* This structure is static (memory was assigned by compiler); hence the pointer is always valid */
    prd.partition_t.cfg_pt = &cfg->PartitionConfig_at[idx];

    Fee_Sector_Init(&prd.sector_at[0], &prd.partition_t.cfg_pt->associatedSectors_at[0]); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_PartitionConfig */
    Fee_Sector_Init(&prd.sector_at[1], &prd.partition_t.cfg_pt->associatedSectors_at[1]); /* SBSW_FEE_Func_passParamByReference */ /* SBSW_FEE_PartitionConfig */
    *prd.partition_t.cfg_pt->ramData_pt = prd; /* SBSW_FEE_PartitionConfig */
}

/**********************************************************************************************************************
 * Fee_Partition_initSector
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
FEE_INTERN_FUNC(void) Fee_Partition_initSector(Fee_PartitionRefType self,
                                                                Fee_SectorRefType sector_pt,
                                                                uint8 sectorId)
{
    struct Fee_PartitionInfoStruct pi;
    pi = *self;

    if(pi.currentSector_pt == NULL_PTR)
    {
        pi.currentSector_pt = sector_pt;
        pi.sectorId_u8 = sectorId;
    }

    else if(Fee_Sector_isUsable(sector_pt)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_ConstPointer_refParam */
    {

        sint16_least idDiff = (sint16_least)(sectorId - pi.sectorId_u8);
        if((!Fee_Sector_isUsable(pi.currentSector_pt)) /* SBSW_FEE_ConstPointer_refParam */
                || ((idDiff > 0) && (idDiff <= 127)) || (idDiff <= -127))
        {
            pi.currentSector_pt = sector_pt;
            pi.sectorId_u8 = sectorId;
        }
    }
    else if(!Fee_Sector_isUsable(pi.currentSector_pt)) /* SBSW_FEE_ConstPointer_refParam */
    { 
        pi.sectorId_u8 = 0; /* subsequent sector format will increment Id to a valid value */
        /* PRQA S 3344,3415 1 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
        if((Fee_Sector_isDefective(sector_pt))                     /* SBSW_FEE_ConstPointer_refParam */
                || (Fee_Sector_isDefective(pi.currentSector_pt)))  /* SBSW_FEE_ConstPointer_refParam */
        { 
            Fee_Partition_handleError(&pi, FEE_SECTORS_CORRUPTED); /* SBSW_FEE_Func_passParamByReference */
        }
    }
    else
    {
    }

    *self = pi; /* SBSW_FEE_PointerWrite_refParam */

}

/**********************************************************************************************************************
 * Fee_Partition_getNextSectorId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(uint8) Fee_Partition_getNextSectorId(Fee_PartitionConstRefType self)
{
    return (uint8)((self->sectorId_u8 < 254) ? (self->sectorId_u8 + 1) : 1);
}

/**********************************************************************************************************************
 * Fee_Partition_updateSectorId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Partition_updateSectorId(Fee_PartitionRefType self)
{
    /*#10 Set sector id to the next valid value*/
    self->sectorId_u8 = /* SBSW_FEE_PointerWrite_refParam */
            Fee_Partition_getNextSectorId(self); /* SBSW_FEE_ConstPointer_refParam */
}

/**********************************************************************************************************************
 * Fee_Partition_getOldestSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getOldestSector(Fee_PartitionConstRefType self)
{
    Fee_SectorRefType retVal = NULL_PTR;
    if(self->currentSector_pt != NULL_PTR)
    {
        const Fee_SectorRefType sectors = self->cfg_pt->ramData_pt->sector_at;

        retVal = ((&sectors[0] == self->currentSector_pt) ? (&sectors[1]) : (&sectors[0]));
    }

    return retVal;
}

/**********************************************************************************************************************
 * Fee_Partition_handleError
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
FEE_INTERN_FUNC(void) Fee_Partition_handleError(Fee_PartitionRefType self, Fee_SectorErrorType errorCode)
{
    struct Fee_PartitionInfoStruct pi;
    pi = *self;

    switch(Fee_ErrorCallbackNotification(pi.cfg_pt->ownIndex_u8, errorCode))
    {
        case FEE_ERRCBK_RESOLVE_AUTOMATICALLY:
#if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
/* default error callback won't return FEE_ERRCBK_RESOLVE_AUTOMATICALLY, if formatting failed */
            if(errorCode != FEE_SECTOR_FORMAT_FAILED)
#endif
            {
                pi.currentSector_pt = Fee_Partition_getOldestSector(&pi); /* SBSW_FEE_Func_passParamByReference */
            }
            Fee_Sector_free(Fee_Partition_getOldestSector(&pi)); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_Func_passParamByReference */

            break;
        case FEE_ERRCBK_REJECT_WRITE:
            pi.readOnlyMode_bl = TRUE;
            break;
        default: /* COV_FEE_DEFAULT_CASE *//* FEE_ERRCBK_REJECT_ALL or any other return value */
            /* our own/default error callback does not deliver REJECT_ALL. Though we need the default, due to coding rules,
             * we don't need to perform any actions -> this code would be dead.  */
#        if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
            pi.rejectAllMode_bl = TRUE;
            pi.readOnlyMode_bl = TRUE;
#        endif
            break;

    }

    *self = pi; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Partition_getFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FEE_INTERN_FUNC(enum Fee_Partition_FillLevelEnum) Fee_Partition_getFillLevel(Fee_PartitionConstRefType self)
{
    enum Fee_Partition_FillLevelEnum level;

    if(self->currentSector_pt == NULL_PTR) /* COV_FEE_PART_CONFIG */
    {
        level = FEE_PART_FILL_LEVEL_UNKNOWN;
    }    
    else if(self->readOnlyMode_bl) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */
    {
        level = FEE_PART_FILL_LEVEL_NORMAL;
    }
    else
    {
        level = Fee_Partition_determineRealFillLevel(self); /* SBSW_FEE_ConstPointer_refParam */
    }

    return level;
}

/**********************************************************************************************************************
 * Fee_Partition_determineRealFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FEE_INLINE_FUNC(enum Fee_Partition_FillLevelEnum) Fee_Partition_determineRealFillLevel(Fee_PartitionConstRefType self)
{
    const Fee_SectorConstRefType newSector = Fee_Partition_getNewestSector(self); /* SBSW_FEE_ConstPointer_refParam */
    const Fee_SectorConstRefType oldSector = Fee_Partition_getOldestSector(self); /* SBSW_FEE_ConstPointer_refParam */

    enum Fee_Partition_FillLevelEnum level;

    /* PRQA S 3344,3415 1 */ /* MD_FEE_13.2_boolean, MD_FEE_12.4_queries */
    if((Fee_Sector_isUsable(oldSector))              /* SBSW_FEE_ConstPointer_refParam */
            && (Fee_Sector_isUsable(newSector)))     /* SBSW_FEE_ConstPointer_refParam */
    { 
        const Fee_LengthType newFillLevel = Fee_Sector_getFillLevel(newSector); /* SBSW_FEE_ConstPointer_refParam */

        if(newFillLevel > self->cfg_pt->thresholds_t.fss_t)
        {
            level = FEE_PART_FILL_LEVEL_FSS;
        }
        else if(newFillLevel > self->cfg_pt->thresholds_t.bss_t)
        {
            level = FEE_PART_FILL_LEVEL_BSS;
        }
        else
        {
            level = FEE_PART_FILL_LEVEL_NORMAL;
        }
    }
    else if(Fee_Sector_isUsable(newSector)) /* PRQA S 3344 */ /* MD_FEE_13.2_boolean */ /* SBSW_FEE_ConstPointer_refParam */
    {
        level = FEE_PART_FILL_LEVEL_SINGLE_SECTOR;
    }
    else
    { /*No sector is usable and they must be formated*/
        level = FEE_PART_FILL_LEVEL_FSS;
    }

    return level;
}

/**********************************************************************************************************************
 * Fee_Partiton_getId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint8) Fee_Partition_getId(Fee_PartitionConstRefType self)
{
    return (self->cfg_pt->ownIndex_u8);
}

/**********************************************************************************************************************
 * Fee_Partition_getSectorByPos
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getSectorByPos(Fee_PartitionConstRefType self, uint8 pos)
{

    return &self->cfg_pt->ramData_pt->sector_at[pos & 0x01u];
}

/**********************************************************************************************************************
 * Fee_Partition_getNewestSector
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getNewestSector(Fee_PartitionConstRefType self)
{
    return self->currentSector_pt;
}

/**********************************************************************************************************************
 * Fee_Partition_getConfiguredLinktableSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(uint16) Fee_Partition_getConfiguredLinktableSize(Fee_PartitionConstRefType self)
{
    return self->cfg_pt->linkTableEntries_u16;
}

/**********************************************************************************************************************
 * Fee_Partition_switchSectors
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Partition_switchSectors(Fee_PartitionRefType self)
{
    self->currentSector_pt = Fee_Partition_getOldestSector(self); /* SBSW_FEE_PointerWrite_refParam */ /* SBSW_FEE_ConstPointer_refParam */
    Fee_Sector_reset(self->currentSector_pt); /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Partition_isWriteable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_isWriteable(Fee_PartitionConstRefType self)
{
    return !self->readOnlyMode_bl;
}

/* Partitions are always readable, unless a user provided error callback actually returns REJECT_ALL
   Default callback does not do so. */
#if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
/**********************************************************************************************************************
 * Fee_Partition_isReadable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_isReadable(Fee_PartitionConstRefType self)
{
    return !self->rejectAllMode_bl;
}
#endif

/**********************************************************************************************************************
 * Fee_Partition_getFromConfigById
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_PartitionRefType) Fee_Partition_getFromConfigById(Fee_ConfigRefTypeInternal cfg, sint8_least idx)
{
    return &cfg->PartitionConfig_at[idx].ramData_pt->partition_t;
}

/**********************************************************************************************************************
 * Fee_Partition_requiresSectorSwitch
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_requiresSectorSwitch(Fee_PartitionConstRefType self)
{
    return (!!self->requiresBlockCopy);
}

/**********************************************************************************************************************
 * Fee_Partition_setBlockCopyRequired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Partition_setBlockCopyRequired(Fee_PartitionRefType self)
{
    self->requiresBlockCopy = TRUE; /* SBSW_FEE_PointerWrite_refParam */
}

/**********************************************************************************************************************
 * Fee_Partition_clearBlockCopyRequired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FEE_INTERN_FUNC(void) Fee_Partition_clearBlockCopyRequired(Fee_PartitionRefType self)
{
    self->requiresBlockCopy = FALSE; /* SBSW_FEE_PointerWrite_refParam */
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Partition.c
 *********************************************************************************************************************/
