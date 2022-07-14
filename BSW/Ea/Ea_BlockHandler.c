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
 *        \file  Ea_BlockHandler.c
 *         \brief  Ea Sub-component BlockHandler source file
 *
 *       \details  BlockHandler sub-component is responsible for management and initialization of block relevant data.
 *                 BlockHandler provides public services to other EA sub-components to access block relevant data.
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

#include "Ea_BlockHandler.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef struct
{
  uint16 BlockIndex;
  Ea_AddressType AbsoluteBlockAddress;
}EaBh_BlockType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

EA_LOCAL VAR(EaBh_BlockType, EA_APPL_DATA) EaBh_BlockInfo;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaBh_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaBh_Init(uint16 BlockNumber, Ea_AddressType PartitionStartAddress)
{
  /* #10 Initialize local parameter */
  EaBh_BlockInfo.BlockIndex = EaBh_GetBlockIndex(BlockNumber);
  EaBh_BlockInfo.AbsoluteBlockAddress = PartitionStartAddress +
          Ea_BlockConfigList[EaBh_BlockInfo.BlockIndex].BlockStartAddress;
}

/**********************************************************************************************************************
  EaBh_GetBlockStartAddress()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaBh_GetBlockStartAddress(void)
{
  return EaBh_BlockInfo.AbsoluteBlockAddress;
}

/**********************************************************************************************************************
  EaBh_GetDataLength()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaBh_GetDataLength(uint16 BlockNumber)
{
  return Ea_BlockConfigList[EaBh_GetBlockIndex(BlockNumber)].DataLength;
}

/**********************************************************************************************************************
  EaBh_GetNumberOfInstances()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint8, EA_PUBLIC_CODE) EaBh_GetNumberOfInstances(void)
{
  return Ea_BlockConfigList[EaBh_BlockInfo.BlockIndex].NumberOfInstances;
}

#if (EA_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  EaBh_IsImmediateData()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (boolean, EA_PUBLIC_CODE) EaBh_IsImmediateData(uint16 BlockNumber)
{
  return Ea_BlockConfigList[EaBh_GetBlockIndex(BlockNumber)].IsImmediateData;
}
#endif

/**********************************************************************************************************************
  EaBh_HasVerificationEnabled()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (boolean, EA_PUBLIC_CODE) EaBh_HasVerificationEnabled(void)
{
  return Ea_BlockConfigList[EaBh_BlockInfo.BlockIndex].HasVerificationEnabled;
}

/**********************************************************************************************************************
  EaBh_GetBlockIndex()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PRIVATE_CODE) EaBh_GetBlockIndex(uint16 BlockNumber)
{
  return (uint16)((BlockNumber >> Ea_DatasetSelectionBits) - 1u);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  END OF FILE: Ea_BlockHandler.c
 *********************************************************************************************************************/
