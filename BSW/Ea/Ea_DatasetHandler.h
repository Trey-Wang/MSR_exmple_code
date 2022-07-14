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
 *        \file  Ea_DatasetHandler.h
 *         \brief  Ea Sub-component DatasetHandler header file
 *
 *       \details  DatasetHandler sub-component is responsible for management and initialization of dataset relevant data.
 *                 DatasetHandler provides public services to other EA sub-components to access block dataset data.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/

#ifndef EA_DATASET_HANDLER_H
# define EA_DATASET_HANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"
# include "Ea_InstanceHandler.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
  EaDh_Init()
**********************************************************************************************************************/
/*!
 * \brief      Initializes dataset relevant data
 * \details    Initializes dataset relevant data
 * \param[in]  BlockNumber
 * \param[in]  DataLength
 * \param[in]  NumberOfInstances
 * \param[in]  BlockStartAddress
 * \param[in]  AddressAlignment
 * \pre        Module is initialized
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea22974
 */
FUNC (void, EA_PUBLIC_CODE) EaDh_Init(uint16 BlockNumber, uint16 DataLength,
        uint8 NumberOfInstances, Ea_AddressType BlockStartAddress, uint16 AddressAlignment);

/**********************************************************************************************************************
  EaDh_GetDataLength()
**********************************************************************************************************************/
/*!
 * \brief      Returns data length of one instance contained in current dataset
 * \details    Returns data length of one instance contained in current dataset
 * \return     DataLength
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (uint16, EA_PUBLIC_CODE) EaDh_GetDataLength(void);

/**********************************************************************************************************************
  EaDh_GetDatasetStartAddress()
**********************************************************************************************************************/
/*!
 * \brief      Returns dataset start address
 * \details    Returns absolute start address of dataset in EEPROM
 * \return     DatasetStartAddress
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaDh_GetDatasetStartAddress(void);

/**********************************************************************************************************************
  EaDh_StoreNextInstanceAddress()
**********************************************************************************************************************/
/*!
 * \brief      Stores address of next instance to Instance object
 * \details    Stores address of next instance to Instance object also considering a wrap around if necessary
 * \param[in]  Instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaDh_StoreNextInstanceAddress(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
  EaDh_EraseDataset()
**********************************************************************************************************************/
/*!
 * \brief      Erases entire dataset
 * \details    Erases entire dataset by calling erase service of EEP driver
 * \return     E_OK: Erase request to EEP driver was successful
 * \return     E_NOT_OK: Erase request to EEP driver failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaDh_EraseDataset(void);

/**********************************************************************************************************************
  EaDh_IsLastInstance()
**********************************************************************************************************************/
/*!
 * \brief      Checks if passed instance is last instance in dataset
 * \details    Checks if passed instance is last instance in dataset
 * \param[in]  Instance
 * \return     TRUE: Instance is last instance index in dataset
 * \return     FALSE: Instance is not last instance index in dataset
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (boolean, EA_PRIVATE_CODE) EaDh_IsLastInstance(EaIh_InstanceConstPointerType Instance);

# define EA_STOP_SEC_CODE
# include "MemMap.h"

#endif /* EA_DATASET_HANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_DatasetHandler.h
 *********************************************************************************************************************/
