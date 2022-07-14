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
 *        \file  Ea_Layer1_Write.h
 *        \brief  This sub-component of EA module is processing write jobs
 *
 *      \details  EaWrite sub-component is responsible for writing data from a DataBufferPtr to EEPROM. Destination in
 *                EEPROM is specified by passed BlockNumber. Internally, InstanceFinder is used to find most recent write
 *                destination and WriteVerifyInstance is used to perform the write job on the determined instance object.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER1_WRITE_H
# define EA_LAYER1_WRITE_H

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
 *  EaWr_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameters of EaWrite sub-component
 * \details    Initializes internal parameters of EaWrite sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Init(void);

/**********************************************************************************************************************
 *  EaWr_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of EaWrite sub-component
 * \details    Returns current status of EaWrite sub-component
 * \return     Sub-component's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaWr_GetStatus(void);

/**********************************************************************************************************************
 *  EaWr_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of EaWrite sub-component
 * \details    Returns current job result of EaWrite sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaWr_GetJobResult(void);


/**********************************************************************************************************************
 *  EaWr_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates EaWrite sub-component's state machine
 * \details    Initiates EaWrite sub-component's state machine and registers EaWrite to TaskManager
 * \param[in]  Ea_UserJobParameter
 * \return     E_OK: Job was accepted by EaWrite sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaWr_StartJob(Ea_UserJobParameterType Ea_UserJobParameter);

/**********************************************************************************************************************
 *  EaWr_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of EaWrite sub-component, only if status is busy
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Execute(void);

/**********************************************************************************************************************
 *  EaWr_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameters
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Cancel(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER1_WRITE_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Write.h
 *********************************************************************************************************************/
