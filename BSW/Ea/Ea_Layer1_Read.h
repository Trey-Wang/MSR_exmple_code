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
 *        \file  Ea_Layer1_Read.h
 *        \brief  This sub-component of EA module is processing read jobs
 *
 *      \details  EaRead sub-component is responsible for processing read jobs. Upon job request it registers itself
 *                to TaskManager and after finishing the job, it de-registers itself from TaskManager. To process the
 *                internal state machine, sub-component InstanceFinder is used to find most recent data.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER1_READ_H
# define EA_LAYER1_READ_H

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
 *  EaRd_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameters of EaRead sub-component
 * \details    Initializes internal parameters of EaRead sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Init(void);

/**********************************************************************************************************************
 *  EaRd_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of EaRead sub-component
 * \details    Returns current status of EaRead sub-component
 * \return     Sub-component's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaRd_GetStatus(void);

/**********************************************************************************************************************
 *  EaRd_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of EaRead sub-component
 * \details    Returns current job result of EaRead sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaRd_GetJobResult(void);


/**********************************************************************************************************************
 *  EaRd_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates EaRead sub-component's state machine
 * \details    Initiates EaRead sub-component's state machine and registers EaRead sub-component to TaskManager
 * \param[in]  Ea_UserJobParameter
 * \return     E_OK: Job was accepted by EaRead sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaRd_StartJob(Ea_UserJobParameterType Ea_UserJobParameter);

/**********************************************************************************************************************
 *  EaRd_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of EaRead sub-component
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Execute(void);

/**********************************************************************************************************************
 *  EaRd_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameter
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Cancel(void);



# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER1_READ_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Read.h
 *********************************************************************************************************************/
