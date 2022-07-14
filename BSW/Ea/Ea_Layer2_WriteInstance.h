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
 *        \file  Ea_Layer2_WriteInstance.h
 *        \brief  WriteInstance sub-component is responsible for writing an instance and veryfing the written data.
 *
 *      \details  WriteInstance sub-component is responsible for writing an instance and veryfing the written data,
 *                if configured. WriteInstance will check the instance's status before writing in order to determine
 *                the correct destination for the write job. Verification of written data can be enabled/disabled by
 *                pre-compile switch.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER2_WRITE_INSTANCE_H
# define EA_LAYER2_WRITE_INSTANCE_H

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
 *  EaWrInst_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameter of WriteInstance sub-component
 * \details    Initializes internal parameter of WriteInstance sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Init(void);

/**********************************************************************************************************************
 *  EaWrInst_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of WriteInstance sub-component
 * \details    Returns current status of WriteInstance sub-component
 * \return     Status of WriteInstance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaWrInst_GetStatus(void);

/**********************************************************************************************************************
 *  EaWrInst_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of WriteInstance sub-component
 * \details    Returns current job result of WriteInstance sub-component
 * \return     Job result of WriteInstance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaWrInst_GetJobResult(void);

/**********************************************************************************************************************
 *  EaWrInst_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates WriteInstance sub-component's state machine
 * \details    Initiates WriteInstance sub-component's state machine and registers WriteInstance sub-component to TaskManager
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \return     E_OK: Job was accepted by WriteInstance sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaWrInst_StartJob(EaIh_InstanceType Instance, Ea_UserJobParameterType UserJobParameter);

/**********************************************************************************************************************
 *  EaWrInst_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of WriteInstance sub-component
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Execute(void);

/**********************************************************************************************************************
 *  EaWrInst_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameter
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Cancel(void);


# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER2_WRITE_INSTANCE_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer2_WriteInstance.h
 *********************************************************************************************************************/
