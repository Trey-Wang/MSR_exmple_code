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
 *         File:  Fee_PartitionDefs.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *********************************************************************************************************************/

#ifndef FEE_PARTITIONDEFS_H_
#define FEE_PARTITIONDEFS_H_

#include "Fee_SectorDefs.h"

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_PARTITION_DEFS_MAJOR_VERSION                                   (8u)
# define FEE_PARTITION_DEFS_MINOR_VERSION                                   (1u)
# define FEE_PARTITION_DEFS_PATCH_VERSION                                   (2u)

typedef P2VAR(struct Fee_PartitionsRamDataStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_PartitionRamDataRefType;
typedef P2CONST(struct Fee_PartitionsRamDataStruct, AUTOMATIC, FEE_VAR_NOINIT) Fee_PartitionRamDataConstRefType;

struct Fee_PartitionInfoStruct /* describes run-time parameters of a partition */
{
    Fee_PartitionConfigRefType cfg_pt;      /*<! pointer to partition's config (setup during init; don't change later */
    Fee_SectorRefType currentSector_pt;     /*<! pointer currently newer sector's info structure */

    Fee_BitFieldType sectorId_u8 : 8;       /*<! newer sector's sector ID, used during sector format */
    Fee_BitFieldType readOnlyMode_bl : 1;   /*<! Read-Only mode; i.e. don't attempt to write (serious errors) */
    Fee_BitFieldType rejectAllMode_bl  : 1; /*<! Do not even attempt to read (serious errors) */
    Fee_BitFieldType requiresBlockCopy : 1; /*<! BlockCopy was explicitly requested (crtical blocks, force sector switch) */
};

struct Fee_PartitionConfigStruct /*<!One single partition config entry (generated) */ 
{
    Fee_PartitionRamDataRefType ramData_pt; /* <! pointer to partition's and associated sectors' RAM data */
    struct Fee_PartitionThresholdsStruct    /* <! the sector switch thresholds as offets within a sector */
    {
        Fee_LengthType fss_t; /*<! Foreground sector switch */
        Fee_LengthType bss_t; /*<! Background sector switch */
    } thresholds_t;
    struct Fee_SectorConfigStruct associatedSectors_at[2]; /*<! both sectors' config params */
    uint16 linkTableEntries_u16; /*<! size of initial link-table (number of entries), when setting up a new sector */
    uint8 ownIndex_u8; /* partitions own index -> useful to get back from pointer to index */
};

/*! Structure allowing RAM reservation for a complete Partition,    *
 *! i.e. RAM data for the partition as well as its sectors          *
 *! Shall only be used for purpose of RAM allocation                */
struct Fee_PartitionsRamDataStruct
{
    struct Fee_PartitionInfoStruct partition_t; /*<! partition */
    struct Fee_SectorInfoStruct sector_at[2];   /*<! both sectors */
};

#endif /* FEE_PARTITIONDEFS_H_ */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Partition.h
 *********************************************************************************************************************/
