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
 *        \file  Ea_BlockHandler.h
 *         \brief  Ea Sub-component BlockHandler header file
 *
 *       \details  BlockHandler sub-component is responsible for management and initialization of block relevant data.
 *                 BlockHandler provides public services to other EA sub-components to access block relevant data.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/

#ifndef EA_BLOCK_HANDLER_H
# define EA_BLOCK_HANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define EA_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
  EaBh_Init()
**********************************************************************************************************************/
/*!
 * \brief       Initializes Block Handler
 * \details     BlockIndex and AbsoluteBlockAddress are initialized
 * \param[in]   BlockNumber
 * \param[in]   PartitionStartAddress
 * \pre         Module is uninitialized
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaBh_Init(uint16 BlockNumber, Ea_AddressType PartitionStartAddress);

/**********************************************************************************************************************
  EaBh_GetBlockStartAddress()
**********************************************************************************************************************/
/*!
 * \brief      Returns start address of block
 * \details    Returns absolute start address of block in EEPROM
 * \return     AbsoluteBlockAddress
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaBh_GetBlockStartAddress(void);

/**********************************************************************************************************************
  EaBh_GetDataLength()
**********************************************************************************************************************/
/*!
 * \brief      Returns data length
 * \details    Returns data length of single instance entity of block
 * \param[in]  BlockNumber
 * \return     DataLength of a single instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (uint16, EA_PUBLIC_CODE) EaBh_GetDataLength(uint16 BlockNumber);

/**********************************************************************************************************************
  EaBh_GetNumberOfInstances()
**********************************************************************************************************************/
/*!
 * \brief      Returns number of instances
 * \details    Returns number of instances configured for current block
 * \return     NumberOfInstances
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (uint8, EA_PUBLIC_CODE) EaBh_GetNumberOfInstances(void);

#if (EA_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  EaBh_IsImmediateData()
**********************************************************************************************************************/
/*!
 * \brief      Returns if the block is configured for immediate data
 * \details    Returns if the block is configured for immediate data
 * \param[in]  BlockNumber
 * \return     IsImmediateData
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (boolean, EA_PUBLIC_CODE) EaBh_IsImmediateData(uint16 BlockNumber);
#endif

/**********************************************************************************************************************
  EaBh_HasVerificationEnabled()
**********************************************************************************************************************/
/*!
 * \brief      Returns if the block is configured to verify written data
 * \details    Returns if the block is configured to verify written data
 * \return     HasVerificationEnabled
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (boolean, EA_PUBLIC_CODE) EaBh_HasVerificationEnabled(void);

/**********************************************************************************************************************
  EaBh_GetBlockIndex()
**********************************************************************************************************************/
/*!
 * \brief      Calculates block index from BlockNumber
 * \details    Calculates block index from BlockNumber by removing DatasetSelectionBits from BlockNumber
 * \param[in]  BlockNumber
 * \return     BlockIndex
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (uint16, EA_PRIVATE_CODE) EaBh_GetBlockIndex(uint16 BlockNumber);

# define EA_STOP_SEC_CODE
# include "MemMap.h"

#endif /* EA_BLOCK_HANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_BlockHandler.h
 *********************************************************************************************************************/
