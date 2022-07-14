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
 *        \file  Ea_Layer2_InvalidateInstance.h
 *        \brief  InvalidateInstance sub-component is responsible for invalidating an instance.
 *
 *      \details  InvalidateInstance sub-component is responsible for invalidating an instance.
 *                InvalidateInstance will check the instance's status before invalidation in order to determine
 *                the correct destination for the invalidation job.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER2_INVALIDATE_INSTANCE_H
# define EA_LAYER2_INVALIDATE_INSTANCE_H

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
 *  EaInvInst_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameter of InvalidateInstance sub-component
 * \details    Initializes internal parameter of InvalidateInstance sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Init(void);

/**********************************************************************************************************************
 *  EaInvInst_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of InvalidateInstance sub-component
 * \details    Returns current status of InvalidateInstance sub-component
 * \return     Status of InvalidateInstance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaInvInst_GetStatus(void);

/**********************************************************************************************************************
 *  EaInvInst_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of InvalidateInstance sub-component
 * \details    Returns current job result of InvalidateInstance sub-component
 * \return     Job result of InvalidateInstance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaInvInst_GetJobResult(void);

/**********************************************************************************************************************
 *  EaInvInst_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates InvalidateInstance sub-component's state machine
 * \details    Initiates InvalidateInstance sub-component's state machine and registers InvalidateInstance to TaskManager
 * \param[in]  Instance
 * \return     E_OK: Job was accepted by InvalidateInstance sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaInvInst_StartJob(EaIh_InstanceType Instance);

/**********************************************************************************************************************
 *  EaInvInst_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of InvalidateInstance sub-component
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Execute(void);

/**********************************************************************************************************************
 *  EaInvInst_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameter
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Cancel(void);


# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER2_INVALIDATE_INSTANCE_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer2_InvalidateInstance.h
 *********************************************************************************************************************/
