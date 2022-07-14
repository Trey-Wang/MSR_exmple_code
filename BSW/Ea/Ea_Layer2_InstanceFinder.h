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
 *         \file  Ea_Layer2_InstanceFinder.h
 *        \brief  InstanceFinder is responsible for finding the most recent data
 *
 *      \details  InstanceFinder is responsible for determination of the address of recently written data (here: instance)
 *                in EEPROM according to specific BlockNumber (UserJobParameter). InstanceFinder is responsible for
 *                evaluating the determined instance's status and stores it to the instance object which is passed by caller.
 *
 *********************************************************************************************************************/


#ifndef EA_SUB_INSTANCE_FINDER_H
# define EA_SUB_INSTANCE_FINDER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"
# include "Ea_InstanceHandler.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaIf_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameter of InstanceFinder sub-component
 * \details    Initializes internal parameter of InstanceFinder sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Init(void);

/**********************************************************************************************************************
 *  EaIf_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of InstanceFinder sub-component
 * \details    Returns current status of InstanceFinder sub-component
 * \return     Status of InstanceFinder
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaIf_GetStatus(void);

/**********************************************************************************************************************
 *  EaIf_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of InstanceFinder sub-component
 * \details    Returns current job result of InstanceFinder sub-component
 * \return     Job result of InstanceFinder
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaIf_GetJobResult(void);

/**********************************************************************************************************************
 *  EaIf_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes InstanceFinder sub-component's state machine
 * \details    Initializes InstanceFinder sub-component's state machine and registers sub-component to TaskManager
 * \param[in]  Instance
 * \return     E_OK: Job was accepted by InstanceFinder sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaIf_StartJob(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIf_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of InstanceFinder sub-component, only if status is busy
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Execute(void);

/**********************************************************************************************************************
 *  EaIf_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameter
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Cancel(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_SUB_INSTANCE_FINDER_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer2_InstanceFinder.h
 *********************************************************************************************************************/
