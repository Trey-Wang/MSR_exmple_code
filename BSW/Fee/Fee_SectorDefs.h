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
 *         File:  Fee_SectorDefs.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *                This file provides definitions of types and structures meant to be private to Fee_Sector
 *
 *********************************************************************************************************************/

#ifndef FEE_SECTORDEFS_H_
#define FEE_SECTORDEFS_H_

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_SECTOR_DEFS_MAJOR_VERSION                                   (8u)
# define FEE_SECTOR_DEFS_MINOR_VERSION                                   (1u)
# define FEE_SECTOR_DEFS_PATCH_VERSION                                   (2u)

struct Fee_AddressRangeStruct
{
    Fee_SectorAddressType baseAddress_t;
    Fee_LengthType size_t;
};

struct Fee_SectorInfoStruct {   /*<! describes a logical sector's run-time data */
#if (FEE_GET_ERASE_CYCLE_API == STD_ON)
    uint32 EraseCycleCounter_u32;   /*<! current erase cycle counter */
#endif
    Fee_SectorConfigRefType cfg_pt;   /*<! pointer to config data; must be setup during initialization, then kept constant */
    Fee_LengthType NextFreeAddress_t; /*<! earliest location a chunk can be allocated to; offset within sector */
    Fee_BitFieldType linkTableSize_t : 12; /*<! current number of link table entries, as read from sector header */
    Fee_BitFieldType isNfaConfirmed_bl:1; /*<! NextFreeAddress is actually the start of used area */
    Fee_BitFieldType isEmpty:1;           /*<! Chunk is empty (or to be erased, if isFull is also TRUE) */
    Fee_BitFieldType isFull:1;            /*<! Chunk is full (shall not be written to, anymore) */
};

/*! Config -> alignments within sector (stored sector-wise, not by partition (as configured). Easier to access */
struct Fee_SectorAlignmentBitsStruct 
{
    uint8 write_u8;   /* <! write alignment -> number of least significant address and length bits that must be 0 */
    uint8 address_u8; /* <! address alignment -> number of least significant address bits that must be 0 */
};

/*! structure to describe the base/start address and size of a logical Fee sector. */
struct Fee_SectorConfigStruct
{
    uint32 eraseValue_u32;                         /* sector's (FLS's) erase/blank value as 32bit */
    struct Fee_AddressRangeStruct physicalRange_t; /*< physical Fls range; aligned to FLS's sector boundaries */
    struct Fee_AddressRangeStruct logicalRange_t;  /*< logical range, that is a sub-range of physical range, 
                                                       i.e. a logical sector may be misaligned */
    struct Fee_SectorAlignmentBitsStruct alignments_t; /*<! alignments; see above*/
};
#endif /* FEE_SECTORDEFS_H_ */

/**********************************************************************************************************************
 *  END OF FILE: Fee_SectorDefs.h
 *********************************************************************************************************************/
