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
 *        \file  Ea_DatasetHandler.c
 *         \brief  Ea Sub-component DatasetHandler source file
 *
 *       \details  DatasetHandler sub-component is responsible for management and initialization of dataset relevant data.
 *                 DatasetHandler provides public services to other EA sub-components to access block dataset data.
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

#include "Ea_DatasetHandler.h"
#include "Ea_PartitionHandler.h"
#include "Ea_EepCoordinator.h"

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
  Ea_AddressType DatasetStartAddress;
  uint16 DatasetSize;
  uint16 AlignedInstanceSize;
  uint16 DataLength;
  uint8 NumberOfInstances;
  uint8 DatasetIndex;
} EaDh_DatasetType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

EA_LOCAL VAR(EaDh_DatasetType, EA_APPL_DATA) EaDh_DatasetInfo;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 EaDh_GetDatasetIndex()
 **********************************************************************************************************************/
/*!
 * \brief      Returns dataset index
 * \details    Calculates and returns dataset index from BlockNumber
 * \param[in]  BlockNumber
 * \return     Dataset index
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_GetDatasetIndex(uint16 BlockNumber);

/**********************************************************************************************************************
 EaDh_CalcIndexOfCurrentInstance()
 **********************************************************************************************************************/
/*!
 * \brief      Calculates instance index related to passed instance start address
 * \details    Calculates instance index in current dataset related to passed instance start address
 * \param[in]  InstanceHeaderAddress
 * \return     InstanceIndex
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_CalcIndexOfCurrentInstance(Ea_AddressType InstanceHeaderAddress);

/**********************************************************************************************************************
 EaDh_GetNextInstanceIndex()
 **********************************************************************************************************************/
/*!
 * \brief      Calculates instance index of next instance
 * \details    Calculates instance index of next instance contained in dataset
 * \param[in]  Instance
 * \return     InstanceIndex
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_GetNextInstanceIndex(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 EaDh_GetAddressFromInstanceIndex()
 **********************************************************************************************************************/
/*!
 * \brief      Calculates start address of instance from instance index
 * \details    Calculates start address of instance related to passed instance index
 * \param[in]  IndexOfActiveInstance
 * \return     InstanceStartAddress
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (Ea_AddressType, EA_PRIVATE_CODE) EaDh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance);

/**********************************************************************************************************************
 EaDh_GetAlignedSizeOfInstance()
 **********************************************************************************************************************/
/*!
 * \brief      Returns aligned size of one instance contained in current dataset
 * \details    Returns aligned size of one instance contained in current dataset.
 *             Aligned size includes Management length, Data length and number of padding bytes
 * \return     Size of instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint16, EA_PRIVATE_CODE) EaDh_GetAlignedSizeOfInstance(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 EaDh_GetDatasetIndex()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_GetDatasetIndex(uint16 BlockNumber)
{
  return (uint8) (BlockNumber & ((1u << Ea_DatasetSelectionBits) - 1u));
}

/**********************************************************************************************************************
 EaDh_CalcIndexOfCurrentInstance()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_CalcIndexOfCurrentInstance(Ea_AddressType InstanceHeaderAddress)
{
  return (uint8) ((InstanceHeaderAddress - EaDh_DatasetInfo.DatasetStartAddress) / EaDh_GetAlignedSizeOfInstance());
}

/**********************************************************************************************************************
 EaDh_GetNextInstanceIndex()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint8, EA_PRIVATE_CODE) EaDh_GetNextInstanceIndex(EaIh_InstanceConstPointerType Instance)
{
  uint8 IndexOfInstance = EaDh_CalcIndexOfCurrentInstance(EaIh_GetInstanceStartAddress(Instance)); /* SBSW_EA_112 */

  if (EaDh_IsLastInstance(Instance) == TRUE) /* SBSW_EA_112 */
  {
    IndexOfInstance = 0u;
  }
  else
  {
    IndexOfInstance++;
  }

  return IndexOfInstance;
}

/**********************************************************************************************************************
 EaDh_GetAddressFromInstanceIndex()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (Ea_AddressType, EA_PRIVATE_CODE) EaDh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance)
{
  return EaDh_DatasetInfo.DatasetStartAddress + (IndexOfActiveInstance * EaDh_GetAlignedSizeOfInstance());
}

/**********************************************************************************************************************
 EaDh_GetAlignedSizeOfInstance()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint16, EA_PRIVATE_CODE) EaDh_GetAlignedSizeOfInstance(void)
{
  return EaDh_DatasetInfo.AlignedInstanceSize;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 EaDh_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaDh_Init(uint16 BlockNumber, uint16 DataLength,
        uint8 NumberOfInstances, Ea_AddressType BlockStartAddress, uint16 AddressAlignment)
{
  EaDh_DatasetInfo.NumberOfInstances = NumberOfInstances;
  EaDh_DatasetInfo.DatasetIndex = EaDh_GetDatasetIndex(BlockNumber);
  /* #20 DatasetSize is equal to the aligned size of one instance (2 * management size + data length of one instance) */
  /* #21 multiplied with the number of instances in the dataset */
  EaDh_DatasetInfo.DatasetSize = Ea_AlignValue((2u * EA_MANAGEMENT_SIZE) + DataLength, AddressAlignment) *
          EaDh_DatasetInfo.NumberOfInstances;
  EaDh_DatasetInfo.DatasetStartAddress = BlockStartAddress +
          (EaDh_DatasetInfo.DatasetSize * EaDh_DatasetInfo.DatasetIndex);
  EaDh_DatasetInfo.AlignedInstanceSize = EaDh_DatasetInfo.DatasetSize / EaDh_DatasetInfo.NumberOfInstances;
  EaDh_DatasetInfo.DataLength = DataLength;
}

/**********************************************************************************************************************
 EaDh_GetDataLength()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaDh_GetDataLength(void)
{
  return EaDh_DatasetInfo.DataLength;
}

/**********************************************************************************************************************
 EaDh_GetDatasetStartAddress()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaDh_GetDatasetStartAddress(void)
{
  return EaDh_DatasetInfo.DatasetStartAddress;
}

/**********************************************************************************************************************
 EaDh_StoreNextInstanceAddress()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaDh_StoreNextInstanceAddress(EaIh_InstanceVarPointerType Instance)
{
  uint8 tempInstanceIndex;
  Ea_AddressType InstanceTrailerAddress;
  Ea_AddressType InstanceHeaderAddress;

  /* #10 Index of subsequent instance is retrieved based on current instance object */
  tempInstanceIndex = EaDh_GetNextInstanceIndex(Instance); /* SBSW_EA_112 */

  /* #20 Header address of subsequent instance is calculated */
  InstanceHeaderAddress = EaDh_GetAddressFromInstanceIndex(tempInstanceIndex);

  /* #30 Trailer address of subsequent instance is calculated based on its header address */
  InstanceTrailerAddress = EaIh_GetTrailerAddressFromHeaderAddress(Instance, InstanceHeaderAddress); /* SBSW_EA_112 */

  /* #40 Current instance object is updated with information of subsequent instance */
  EaIh_SetHeaderAndTrailerAddress(Instance, InstanceHeaderAddress, InstanceTrailerAddress); /* SBSW_EA_111 */
}

/**********************************************************************************************************************
 EaDh_EraseDataset()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaDh_EraseDataset(void)
{
  return Ea_Eep_Erase(EaDh_DatasetInfo.DatasetStartAddress, EaDh_DatasetInfo.DatasetSize);
}

/**********************************************************************************************************************
 EaDh_IsLastInstance()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (boolean, EA_PRIVATE_CODE)
EaDh_IsLastInstance(EaIh_InstanceConstPointerType Instance)
{
  /* #10 Index of current instance is calculated based on passed Instance object */
  uint8 IndexOfInstance = EaDh_CalcIndexOfCurrentInstance(EaIh_GetInstanceStartAddress(Instance)); /* SBSW_EA_112 */

  /* #20 NumberOfInstances is reduced by (1u), because instance indexes are zero based */
  return ((IndexOfInstance == (EaDh_DatasetInfo.NumberOfInstances - 1u)) ? TRUE : FALSE);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_111
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures that parameter referenced
                         by pointer is valid.
    \ID SBSW_EA_112
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceConstPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures that parameter referenced
                         by pointer is valid.
SBSW_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  END OF FILE: Ea_DatasetHandler.c
 *********************************************************************************************************************/
