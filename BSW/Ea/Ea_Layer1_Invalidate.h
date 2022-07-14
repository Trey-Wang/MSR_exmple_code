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
 *        \file  Ea_Layer1_Invalidate.h
 *        \brief  This sub-component is responsible for invalidating data
 *
 *      \details  EaInvalidate sub-component is responsible for invalidating data in EEPROM which belongs to passed
 *                BlockNumber. Internally, sub-component InstanceFinder is used to determine correct destination of
 *                invalidation job. Additionally InvalidateInstance sub-component is used to actually perform the
 *                invalidation of the most recent instance in EEPROM.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER1_INVALIDATE_H
# define EA_LAYER1_INVALIDATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaInv_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameters of EaInvalidate sub-component
 * \details    Initializes internal parameters of EaInvalidate sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Init(void);

/**********************************************************************************************************************
 *  EaInv_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of EaInvalidate sub-component
 * \details    Returns current status of EaInvalidate sub-component
 * \return     Sub-component's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaInv_GetStatus(void);

/**********************************************************************************************************************
 *  EaInv_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of EaInvalidate sub-component
 * \details    Returns current job result of EaInvalidate sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaInv_GetJobResult(void);


/**********************************************************************************************************************
 *  EaInv_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates EaInvalidate sub-component's state machine
 * \details    Initiates EaInvalidate sub-component's state machine and registers EaInvalidate sub-component to TaskManager
 * \return     E_OK: Job was accepted by EaInvalidate sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaInv_StartJob(void);

/**********************************************************************************************************************
 *  EaInv_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of EaInvalidate sub-component
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Execute(void);

/**********************************************************************************************************************
 *  EaInv_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameters
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Cancel(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER1_INVALIDATE_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Invalidate.h
 *********************************************************************************************************************/
