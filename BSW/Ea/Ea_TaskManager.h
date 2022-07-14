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
 *        \file  Ea_TaskManager.h
 *         \brief  Ea Sub-component TaskManager source file
 *
 *       \details  TaskManager is responsible for processing asynchronous sub-components in correct order by calling the
 *                appropriate execution services. TaskManager is also responsible for canceling all active sub-components
 *                in case Ea_Cancel was called by user.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#ifndef TASKMANAGER_H
# define TASKMANAGER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES
 *********************************************************************************************************************/

# define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef P2FUNC(void, EA_PUBLIC_CODE, EaTm_ExecuteServicePointerType)(void);
typedef P2FUNC(void, EA_PUBLIC_CODE, EaTm_CancelServicePointerType)(void);

# define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
  EaTm_Init()
**********************************************************************************************************************/
/*!
 * \brief      Initializes TaskManager
 * \details    Initializes TaskManager by clearing job stack and cancel stack
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_Init (void);

/**********************************************************************************************************************
  EaTm_AddTask()
**********************************************************************************************************************/
/*!
 * \brief      Adds passed execution and cancel services to job stack and cancel stack
 * \details    Adds passed execution and cancel services to job stack and cancel stack
 * \param[in]  ExecuteService
 * \param[in]  CancelService
 * \param[in]  Layer
 * \return     E_OK: Services were added correctly
 * \return     E_NOT_OK: Adding services failed. Stack position (layer) is not empty.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaTm_AddTask (EaTm_ExecuteServicePointerType ExecuteService,
        EaTm_CancelServicePointerType CancelService, uint8 Layer);

/**********************************************************************************************************************
  EaTm_RemoveTask()
**********************************************************************************************************************/
/*!
 * \brief      Removes services from job stack and cancel stack
 * \details    Removes execution and cancel services from certain layer in job stack and cancel stack
 * \param[in]  ExecuteService
 * \param[in]  CancelService
 * \param[in]  Layer
 * \return     E_OK: Services were removed correctly
 * \return     E_NOT_OK: Containing services in stacks differ from passed services. No service was removed.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_RemoveTask (EaTm_ExecuteServicePointerType ExecuteService,
        EaTm_CancelServicePointerType CancelService, uint8 Layer);

/**********************************************************************************************************************
  EaTm_ExecuteTask()
**********************************************************************************************************************/
/*!
 * \brief      Calls most recent execution service in job stack
 * \details    Calls service in job stack which was put there last
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_ExecuteTask (void);

/**********************************************************************************************************************
  EaTm_CancelTasks()
**********************************************************************************************************************/
/*!
 * \brief      Calls all cancel services in cancel stack
 * \details    Calls all cancel services in cancel stack beginning with lowest layer (layer 3)
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea23452
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_CancelTasks (void);


# define EA_STOP_SEC_CODE
# include "MemMap.h"

#endif /* TASKMANAGER_H */
/**********************************************************************************************************************
 *  END OF FILE: Ea_BlockHandler.h
 *********************************************************************************************************************/
