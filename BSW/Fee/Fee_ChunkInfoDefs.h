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
 *         File:  Fee_ChunkInfo_Defs.h
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
#ifndef FEE_CHUNKINFODEFS_H_
#define FEE_CHUNKINFODEFS_H_

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_CHUNKINFO_DEFS_MAJOR_VERSION                                   (8u)
# define FEE_CHUNKINFO_DEFS_MINOR_VERSION                                   (1u)
/* BCD coded! */
# define FEE_CHUNKINFO_DEFS_PATCH_VERSION                                   (3u)

struct Fee_ChunkInfoStruct /*<! Describes a chunk; initial link-table is handled as a special (pseudo) chunk */
{
    Fee_SectorRefType Sector_pt;             /*<! The sector the chunk resides in  */
    Fee_LengthType ChunkBaseOffset_t;        /*<! start offset, relative to sector */
    Fee_BitFieldType PayloadSize_b16:16;     /*<! amount of user data in bytes */
    Fee_BitFieldType InstanceExponent_b8:8;  /*<! current instance count's exponent */
    Fee_BitFieldType isDataChunk_bl:1;       /*<! True <=> data chunk, False <=> link-lable */
    Fee_BitFieldType isPseudoChunk_bl:1;     /*<! True <=> initial "pseudo chunk"; implies !isDataChunk_bl */
    Fee_BitFieldType wasLinkWritten_bl:1;    /*<! Was link already written? */
    Fee_BitFieldType isLinkDefective_bl:1;   /*<! Was a read link defective? */
};

#endif /* FEE_CHUNKINFODEFS_H_ */

/**********************************************************************************************************************
 *  END OF FILE: Fee_ChunkInfoDefs.h
 *********************************************************************************************************************/
