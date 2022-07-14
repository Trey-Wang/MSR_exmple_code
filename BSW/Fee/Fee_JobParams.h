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
 *         File:  Fee_JobParams.h
 *    Component:  If_AsrIfFee
 *       Module:  Fee
 *    Generator:  -
 *
 *  Description:  The module Fee provides an abstraction from the device specific addressing scheme and
 *                segmentation. It also offers a virtual addressing scheme and segmentation as well as
 *                a virtually unlimited number of erase cycles to the upper layer.
 *
 *                This file defines internal job parameter structures. It shall not be included directly by other
 *                SW!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#ifndef FEE_JOBPARAMS_H_
#define FEE_JOBPARAMS_H_

/******************************************************************************
 * VERSION IDENTIFICATION
 *****************************************************************************/
# define FEE_JOB_PARAMS_MAJOR_VERSION (8u)
# define FEE_JOB_PARAMS_MINOR_VERSION (1u)
# define FEE_JOB_PARAMS_PATCH_VERSION (3u)

/* asynchronous job types. Note that ordering matters; write-class jobs are grouped and occur before read jobs */
enum Fee_tagReqestedJob 
{
  /* Attention:
   * The order of the values must not be changed!!!
   * It is used in the QryReadClassJob, QryWriteClassJob and also in
   * the JobCache functionality.
   */
  FEE_JOB_NONE = 0,
  FEE_JOB_ERASE_IMMEDIATE_BLOCK,
  FEE_JOB_INVALIDATE_BLOCK,
#if (FEE_DATA_CONVERSION_API == STD_ON)
  FEE_JOB_CONVERT_DATA_BLOCKS,
#endif
  FEE_JOB_WRITE, /*<! must be the last write-class job! */
# if (FEE_GET_ERASE_CYCLE_API == STD_ON)
    FEE_JOB_GET_ERASE_CYCLE,
# endif
# if (FEE_GET_WRITE_CYCLE_API == STD_ON)
    FEE_JOB_GET_WRITE_CYCLE,
# endif
  FEE_JOB_READ
};

/* Force implizit cast for enum Fee_RequestFlsJobIdType into uint8.
 * But only for production mode in order to see not only numbers
 * when debugging.
 */
# if (STD_ON == FEE_DEV_ERROR_DETECT)
  typedef enum Fee_tagReqestedJob Fee_RequestedJobType;
# else
  typedef uint8 Fee_RequestedJobType;
# endif

/*! sector Identification; holds decomposed SectorIdParam from Fee_GetEraseCycle */
struct Fee_tagSectorIdentification
{
  Fee_BitFieldType partitionIdx:7; /*<! passed Partition Index; to enable checking partion's index, use 7 bits
                                        rather than only 2. => a SectorIdParam is invalid, if partition index is out of
                                        range (configured number of partitions). */
  Fee_BitFieldType sectorIdx:1;    /*<! sector index, can only be lower(=0) or upper (=1). It cannot be out of range. */
};

/*! Within Blockspecific requests, identify a Fee Block by its config entry and data index */
typedef struct Fee_tagBlockCfgDatasetPair
{
    Fee_BlockConfigRefType config;
    uint8 dataIndex;
} Fee_BlockCfgDatasetPair;

struct Fee_tagReadParams /*<! Fee_Read specifc parameters */
{
      Fee_UserDataPtrType readBuffer; /*<! must not be NULL_PTR */
      uint16 length;
      uint16 offset;
};

struct Fee_tagWriteParams /*<! Fee_Write/Invalidate/Erase specific parameters */
{
      Fee_UserDataConstPtrType writeBuffer; /* may be NULL_PTR -> Invalidate/Erase */
};

struct Fee_tagEraseCycleParams /*<! Fee_GetEraseCycle specific parameters */
{
    Fee_UserData32PtrType resultBuffer;
    struct Fee_tagSectorIdentification sectorIdent;
};

struct Fee_tagWriteCycleParams /*<! Fee_GetWriteCycle specific parameters */
{
    Fee_UserData32PtrType resultBuffer;
};

struct Fee_tagBlockSpecificJob /* <! Gathers Block specfic job parameters */
{
    struct Fee_tagBlockCfgDatasetPair blockRef; /*<! Block identification; common to all jobs */
    union /* PRQA S 0750 1 *//* MD_MSR_18.4 */ /*<! only one of the jobs is valid at any time */
    {
        struct Fee_tagReadParams read;    
        struct Fee_tagWriteParams write;
        struct Fee_tagWriteCycleParams writeCc;
        /* invalidate and erase don't additional params */
    } add;
};

union Fee_tagJobSpecificParams /* PRQA S 0750 1 *//* MD_MSR_18.4 */ /*<! Gathers all possible asynchronous job params */
{
    struct Fee_tagBlockSpecificJob blockJob;
    struct Fee_tagEraseCycleParams eraseCc;
    Fee_ConversionOptionsType conv;
};

/* PRQA S 0750 2 *//* MD_MSR_18.4 */
struct Fee_FeeJobParameterStruct /*<! the final job params structure, completely describing a job request */
{
    union Fee_tagJobSpecificParams params;
    sint8 partitionId;                      /*<! the Partion ID the job is valid for;
                                                 it's just a common, unified storage, since different jobs have different
                                                 ways to get the partition (via block/via "sectorId", ...) */
    Fee_RequestedJobType requestedJob;      /*<! kind of job; discriminates the actual parameter (sub-)structures
                                                 (union members) to be used. */
};
#endif /* FEE_JOBPARAMS_H_ */

/**********************************************************************************************************************
 *  END OF FILE: Fee_JobParams.h
 *********************************************************************************************************************/
