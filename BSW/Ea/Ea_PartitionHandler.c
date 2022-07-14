/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  Ea_PartitionHandler.c
 *        \brief  Ea Sub-component PartitionHandler source file
 *
 *      \details  PartitionHandler is responsible for management and initialization of partition relevant data.
 *                PartitionHandler provides public services to other EA sub-components to access partition relevant data.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_PartitionHandler.h"

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR (uint16, EA_APPL_DATA) EaPh_PartitionIndex;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  EaPh_FindBlockNumberInList()
**********************************************************************************************************************/
/*! \brief     Checks if a certain BlockNumber is contained in a certain Partition
 * \details    Checks if BlockNumber is contained in Partition PartitionIndex is pointing to
 * \param[in]  PartitionIndex
 * \param[in]  BlockNumber
 * \return     E_OK: BlockNumber is contained in partition
 * \return     E_NOT_OK: BlockNumber is not contained in partition
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (Std_ReturnType, EA_PRIVATE_CODE) EaPh_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber);

/**********************************************************************************************************************
  EaPh_CleanBlockNumber()
**********************************************************************************************************************/
/*! \brief     Returns clean BlockNumber
 * \details    Returns BlockNumber without DatasetIndex part
 * \param[in]  BlockNumber
 * \return     BlockNumber
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint16, EA_PRIVATE_CODE) EaPh_CleanBlockNumber(uint16 BlockNumber);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaPh_FindBlockNumberInList()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC (Std_ReturnType, EA_PRIVATE_CODE) EaPh_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber)
{
  uint16 i;
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 Iterate over all blocks in certain partition */
  for (i = 0; i < EA_MAX_BLOCKS_PER_PARTITION; i++)
  {
    /* #20 Check if BlockNumber is contained in current partition */
    if (Ea_PartitionConfigList[PartitionIndex].BlockNumberList[i] == BlockNumber)
    {
      retVal = E_OK;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
  EaPh_CleanBlockNumber()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint16, EA_PRIVATE_CODE) EaPh_CleanBlockNumber(uint16 BlockNumber)
{
  return BlockNumber & (0xFFFFU - ((1u << Ea_DatasetSelectionBits) - 1u));
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaPh_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaPh_Init(uint16 BlockNumber)
{
  uint16 LoopCounter;
  /* #10 PartitionIndex has to be initialized with Partition BlockNumber is contained in */
  for (LoopCounter = 0; LoopCounter < EA_NUMBER_OF_PARTITIONS; LoopCounter++) /* COV_EA_CONDITION_ALWAYS_TRUE */
  {
    if (EaPh_FindBlockNumberInList(LoopCounter, EaPh_CleanBlockNumber(BlockNumber)) == E_OK)
    {
      /* #20 Partition, which contains BlockNumber, was found. Index is stored */
      EaPh_PartitionIndex = LoopCounter;
      break;
    }
  }
}

/**********************************************************************************************************************
  EaPh_GetPartitionStartAddress()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaPh_GetPartitionStartAddress(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionStartAddress;
}

/**********************************************************************************************************************
  EaPh_GetPartitionWriteAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionWriteAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionWriteAlignment;
}

/**********************************************************************************************************************
  EaPh_GetPartitionAddressAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionAddressAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionAddressAlignment;
}

/**********************************************************************************************************************
  EaPh_GetPartitionReadAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionReadAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionReadAlignment;
}

/**********************************************************************************************************************
  EaPh_GetEepApiPtr()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_EepApiPtrType, EA_PUBLIC_CODE) EaPh_GetEepApiPtr(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].EepApiPtr;
}

/**********************************************************************************************************************
  EaPh_GetEepEraseValue()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint8, EA_PUBLIC_CODE) EaPh_GetEepEraseValue(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].EepEraseValue;
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* START_COVERAGE_JUSTIFICATION
    \ID COV_EA_CONDITION_ALWAYS_TRUE
    \ACCEPT TX
    \REASON Condition "LoopCounter < EA_NUMBER_OF_PARTITIONS" is never false in TscStandard.
            'break' in if-clause prevents loop condition from getting false because even if last partition
            contains BlockNumber, LoopCounter will not exceed the condition. Only invalid BlockNumbers are able to
            trigger this condition, and only if DET is disabled, since DET checks validity of BlockNumber before.
END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Ea_PartitionHandler.c
 *********************************************************************************************************************/
