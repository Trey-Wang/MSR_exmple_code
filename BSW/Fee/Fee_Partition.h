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
 *         File:  Fee_Partition.h
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
#if !defined (FEE_PARTITION_H)
# define FEE_PARTITION_H

#include "Fee_Sector.h"

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_PARTITION_MAJOR_VERSION                                   (8u)
# define FEE_PARTITION_MINOR_VERSION                                   (1u)
# define FEE_PARTITION_PATCH_VERSION                                   (1u)

enum Fee_Partition_FillLevelEnum /*<! abstract fill levels */
{
    FEE_PART_FILL_LEVEL_UNKNOWN       = -1, /*<! partition was not initialized, yet - sector headers were not read */
    FEE_PART_FILL_LEVEL_SINGLE_SECTOR = 0,  /*<! only one sector is in use, the other one may/shall be formatted */
    FEE_PART_FILL_LEVEL_NORMAL        = 1,  /*<! both sectors in use, no thresholds exceeded */
    FEE_PART_FILL_LEVEL_BSS           = 2,  /*<! BSS threshold exceeded */
    FEE_PART_FILL_LEVEL_FSS           = 3,  /*<! FSS threshold exceeded */
    FEE_PART_FILL_LEVEL_OVERFLOW      = 4   /*<! both sectors are full  */
};

/*! Pointer/reference types for PartitionInfo. Each of the functions declared below, whose first parameter is of such
 *  reference type, can be seen as member function (method) of "class Partition(Info)".
 *  The parameter's type (constness) denotes the method's constness, i.e. whether it is supposed to change obect's
 *  state.
 *  The reference to the object ("this" in C++ and Java) is usually named "self" (in the style of smalltalk).
 *  Reason: It is not recommended to use "this" in C (we use C++ for testing!).
 */
typedef P2VAR(struct Fee_PartitionInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_PartitionRefType;
typedef P2CONST(struct Fee_PartitionInfoStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_PartitionConstRefType;

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_PARTITION_API_MAJOR_VERSION                                   (8u)
# define FEE_PARTITION_API_MINOR_VERSION                                   (1u)
# define FEE_PARTITION_API_PATCH_VERSION                                   (2u)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*********************************************************************************
 * Partition Functions
 *********************************************************************************/

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Fee_Partition_init
 *********************************************************************************************************************/
/*!
 * \brief      Initializes one partition
 * \details    Triggers initialization of partition's both sectors and the partition itself
 * \param[in,out]  cfg: Data structure that holds configured parameters
 * \param[in]  idx: Index of partition in config's partition array that shall be initialized
 * \pre        cfg is not null
 * \pre        idx is within number of partitions
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_init(Fee_ConfigRefTypeInternal cfg, sint8_least idx);


/**********************************************************************************************************************
 * Fee_Partition_getFromConfigById
 *********************************************************************************************************************/
/*!
 * \brief      Gets partition at given index from given config
 * \details    Gets partition at given index from given config
 * \param[in]  cfg: Data structure that holds configured parameters
 * \param[in]  idx: Index of partition in configs partition array that shall be returned
 * \return     partition at given index
 * \pre        cfg is not null
 * \pre        idx is within number of partitions
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_PartitionRefType) Fee_Partition_getFromConfigById(Fee_ConfigRefTypeInternal cfg, sint8_least idx);



/**********************************************************************************************************************
 * Fee_Partition_initSector
 *********************************************************************************************************************/
/*!
 * \brief      Conditionally updates current sector of partition
 * \details    Given sector becomes partitions current sector, when it has an higher id than the previous current sector
 *             Checks both sectors and triggers error resolution, when both sectors of partition are defective
 * \param[in,out]  self
 * \param[in,out]  sector_pt: Sector that shall become partitions current sector
 * \param[in]  sectorId: Id that shall become partitions current sectors new id
 * \pre        self is not null,sector_pt is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_initSector(Fee_PartitionRefType self,
                                                                   Fee_SectorRefType sector_pt,
                                                                   uint8 sectorId);


/**********************************************************************************************************************
 * Fee_Partition_getOldestSector
 *********************************************************************************************************************/
/*!
 * \brief      Gets oldest sector of partition.
 * \details    Gets oldest sector of partition.
 * \param[in]  self
 * \return     null: There is only one sector.
 * \return     sector: The oldest sector
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getOldestSector(Fee_PartitionConstRefType self);


/**********************************************************************************************************************
 * Fee_Partition_getFillLevel
 *********************************************************************************************************************/
/*!
 * \brief      Compute the fill level of given partition
 * \details    Compute the fill level of given partition
 * \param[in]  self
 * \return     FEE_PART_FILL_LEVEL_FSS: FSS threshold exceeded or both sectors need to be formated, FSS
 *             must be executed
 * \return     FEE_PART_FILL_LEVEL_BSS: BSS threshold exceeded, BSS can be executed
 * \return     FEE_PART_FILL_LEVEL_NORMAL: Both sectors in use, no threshold exceeded, no sector switch necessary
 * \return     FEE_PART_FILL_LEVEL_SINGLE_SECTOR: Only one sector in use, no sector switch necessary
 * \return     FEE_PART_FILL_LEVEL_UNKNOWN: Partition is not initialized
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(enum Fee_Partition_FillLevelEnum) Fee_Partition_getFillLevel(Fee_PartitionConstRefType self);


/**********************************************************************************************************************
 * Fee_Partition_handleError
 *********************************************************************************************************************/
/*!
 * \brief      Handles error that occurred at given partition
 * \details    The way how the error shall be handled is determined by calling a callback function
 * \param[in,out]  self
 * \param[in]  errorCode
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_handleError(Fee_PartitionRefType self, Fee_SectorErrorType errorCode);


/**********************************************************************************************************************
 * Fee_Partition_getNextSectorId
 *********************************************************************************************************************/
/*!
 * \brief      Computes next SectorId based on current internal data (does not update anything)
 * \details    Computes next SectorId based on current internal data (does not update anything)
 * \param[in]  self
 * \param[in]  errorCode
 * \pre        self is not null
 * \return     sectorId
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint8) Fee_Partition_getNextSectorId(Fee_PartitionConstRefType self);


/**********************************************************************************************************************
 * Fee_Partition_updateSectorId
 *********************************************************************************************************************/
/*!
 * \brief      Updates sectorId
 * \details    Updates sectorId
 * \param[in]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_updateSectorId(Fee_PartitionRefType self);


/**********************************************************************************************************************
 * Fee_Partition_getId
 *********************************************************************************************************************/
/*!
 * \brief      Gets partitionId of given partition
 * \details    Gets partitionId of given partition
 * \param[in]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint8) Fee_Partition_getId(Fee_PartitionConstRefType self);



/**********************************************************************************************************************
 * Fee_Partition_getSectorByPos
 *********************************************************************************************************************/
/*!
 * \brief      Gets sector of given partition at given position
 * \details    Gets sector of given partition at given position
 * \param[in]  self
 * \param[in]  pos: Position of the sector, only bit 0 is relevant
 * \pre        self is not null
 * \return     Upper sector: pos == 1
 * \return     Lower sector: pos == 0
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getSectorByPos(Fee_PartitionConstRefType self, uint8 pos);



/**********************************************************************************************************************
 * Fee_Partition_getNewestSector
 *********************************************************************************************************************/
/*!
 * \brief      Gets current sector of given partition
 * \details    Gets current sector of given partition
 * \param[in]  self
 * \return     current sector
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_SectorRefType) Fee_Partition_getNewestSector(Fee_PartitionConstRefType self);



/**********************************************************************************************************************
 * Fee_Partition_getConfiguredLinktableSize
 *********************************************************************************************************************/
/*!
 * \brief      Gets number of linktable entries of given partition
 * \details    Gets number of linktable entries of given partition
 * \param[in]  self
 * \return     number of linktable entries
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(uint16) Fee_Partition_getConfiguredLinktableSize(Fee_PartitionConstRefType self);



/**********************************************************************************************************************
 * Fee_Partition_switchSectors
 *********************************************************************************************************************/
/*!
 * \brief      Assigns oldest sector to current sector and set its members to default values
 * \details    Assigns oldest sector to current sector and set its members to default values
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_switchSectors(Fee_PartitionRefType self);



/**********************************************************************************************************************
 * Fee_Partition_isWriteable
 *********************************************************************************************************************/
/*!
 * \brief      Checks if partition is still writeable
 * \details    Evaluates read only flag
 * \param[in]  self
 * \pre        self is not null
 * \return     TRUE: Partition is writable
 * \return     FALSE: Partition is not writeable
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_isWriteable(Fee_PartitionConstRefType self);


/* Currently partitions are always readable, unless an external error callback actually returns REJECT_ALL */
#if(FEE_USE_APPL_ERROR_CALLBACK == STD_ON)
/**********************************************************************************************************************
 * Fee_Partition_isReadable
 *********************************************************************************************************************/
/*!
 * \brief      Checks if partition is still readable
 * \details    Evaluates reject all flag
 * \param[in]  self
 * \config     FEE_USE_APPL_ERROR_CALLBACK == STD_ON
 * \pre        self is not null
 * \return     TRUE: Partition is readable
 * \return     FALSE: Partition is not readable
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_isReadable(Fee_PartitionConstRefType self);
#endif


/**********************************************************************************************************************
 * Fee_Partition_requiresSectorSwitch
 *********************************************************************************************************************/
/*!
 * \brief      Checks whether a sector switch is necessary (in favor of subsequent writes)
 * \details    Checks whether a sector switch is necessary (in favor of subsequent writes)
 * \param[in]  self
 * \return     TRUE: Sector switch is necessary
 * \return     FALSE: Sector switch is not necessary
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_Bool) Fee_Partition_requiresSectorSwitch(Fee_PartitionConstRefType self);


/**********************************************************************************************************************
 * Fee_Partition_setBlockCopyRequired
 *********************************************************************************************************************/
/*!
 * \brief      Sets block copy required flag in order to make sector switch necessary
 * \details    Sets block copy required flag in order to make sector switch necessary
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_setBlockCopyRequired(Fee_PartitionRefType self);


/**********************************************************************************************************************
 * Fee_Partition_clearBlockCopyRequired
 *********************************************************************************************************************/
/*!
 * \brief      Resets block copy required flag
 * \details    Resets block copy required flag
 * \param[in,out]  self
 * \pre        self is not null
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(void) Fee_Partition_clearBlockCopyRequired(Fee_PartitionRefType self);



/* External Function, REQUIRED by Fee_Partition. Declared here, since it use used in implementation only. *
 * However, MISRA requires declaration in a header file (Fee_Partition.c would be a better place)         */
/**********************************************************************************************************************
 * Fee_ErrorCallbackNotification
 *********************************************************************************************************************/
/*!
 * \brief      Determines how a error shall be handled
 * \details    Determines how a error shall be handled
 * \param[in]  partitionIndex
 * \param[in]  errorCode
 * \return     FEE_ERRCBK_RESOLVE_AUTOMATICALLY: Error shall be resolved automatically
 * \return     FEE_ERRCBK_REJECT_ALL: Fee shall reject all further jobs
 * \return     FEE_ERRCBK_REJECT_WRITE: Fee shall operate in read only mode
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
FEE_INTERN_FUNC(Fee_ErrorCallbackReturnType) Fee_ErrorCallbackNotification(uint8, Fee_SectorErrorType);

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* FEE_PARTITION_H */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Partition.h
 *********************************************************************************************************************/
