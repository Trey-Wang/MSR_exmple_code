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
 *         \file  Ea_PartitionHandler.h
 *        \brief  Ea Sub-component PartitionHandler header file
 *
 *      \details  PartitionHandler is responsible for management and initialization of partition relevant data.
 *                PartitionHandler provides public services to other sub-components to access partition relevant data.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/

#ifndef EA_PARTITION_HANDLER_H
# define EA_PARTITION_HANDLER_H

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
  EaPh_Init()
**********************************************************************************************************************/
/*!
 * \brief       Initializes Partition Handler
 * \details     Local parameter EaPh_PartitionIndex is initialized with index of partition that holds the current BlockNumber
 * \param[in]   BlockNumber
 * \pre         Module is uninitialized
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaPh_Init(uint16 BlockNumber);

/**********************************************************************************************************************
  EaPh_GetPartitionStartAddress()
**********************************************************************************************************************/
/*!
 * \brief      Returns partition start address
 * \details    Returns partition start address belonging to partition, in which current block number is contained
 * \return     Partition start address
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23152
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaPh_GetPartitionStartAddress(void);

/**********************************************************************************************************************
  EaPh_GetPartitionWriteAlignment()
**********************************************************************************************************************/
/*!
 * \brief      Returns write alignment of partition
 * \details    Returns write alignment, belonging to partition in which current block number is contained
 * \return     Partition write alignment
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23152
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionWriteAlignment(void);

/**********************************************************************************************************************
  EaPh_GetPartitionAddressAlignment()
**********************************************************************************************************************/
/*!
 * \brief      Returns address alignment of partition
 * \details    Returns address alignment, belonging to partition in which current block number is contained
 * \return     Partition address alignment
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea22974
 * \trace DSGN-Ea23152
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionAddressAlignment(void);

/**********************************************************************************************************************
  EaPh_GetPartitionReadAlignment()
**********************************************************************************************************************/
/*!
 * \brief      Returns read alignment of partition
 * \details    Returns read alignment, belonging to partition in which current block number is contained
 * \return     Partition read alignment
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23152
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionReadAlignment(void);

/**********************************************************************************************************************
  EaPh_GetEepApiPtr()
**********************************************************************************************************************/
/*!
 * \brief      Returns reference to APIs of EEP driver
 * \details    Returns reference to APIs of EEP driver which is related to current BlockNumber via configured partition
 * \return     EEP API Pointer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23419
 * \trace DSGN-Ea23152
 */
FUNC (Ea_EepApiPtrType, EA_PUBLIC_CODE) EaPh_GetEepApiPtr(void);

/**********************************************************************************************************************
  EaPh_GetEepEraseValue()
**********************************************************************************************************************/
/*!
 * \brief      Returns erase value of EEP driver
 * \details    Returns erase value of EEP driver which is related to current BlockNumber via configured partition
 * \return     Eep Erase Value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23152
 */
FUNC (uint8, EA_PUBLIC_CODE) EaPh_GetEepEraseValue(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h"

#endif /* EA_PARTITION_HANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_PartitionHandler.h
 *********************************************************************************************************************/

