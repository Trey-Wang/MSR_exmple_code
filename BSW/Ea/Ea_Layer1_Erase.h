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
 *        \file  Ea_Layer1_Erase.h
 *        \brief  EaErase sub-component is responsible for erasing immediate data in EEPROM
 *
 *      \details  EaErase sub-component is responsible for erasing entire data area (Dataset) in EEPROM which belongs
 *                to passed BlockNumber. Internally, EEP driver's erase service is used to perform the job.
 *
 *********************************************************************************************************************/

#ifndef EA_LAYER1_ERASE_H
# define EA_LAYER1_ERASE_H

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
 *  EaEr_Init()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes internal parameters of EaErase sub-component
 * \details    Initializes internal parameters of EaErase sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Init(void);

/**********************************************************************************************************************
 *  EaEr_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of EaErase sub-component
 * \details    Returns current status of EaErase sub-component
 * \return     Sub-component's status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaEr_GetStatus(void);

/**********************************************************************************************************************
 *  EaEr_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of EaErase sub-component
 * \details    Returns current job result of EaErase sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaEr_GetJobResult(void);


/**********************************************************************************************************************
 *  EaEr_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates EaErase sub-component's state machine
 * \details    Initiates EaErase sub-component's state machine and registers EaErase sub-component to TaskManager
 * \return     E_OK: Job was accepted by EaErase sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaEr_StartJob(void);

/**********************************************************************************************************************
 *  EaEr_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of EaErase sub-component, only if status is busy
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Execute(void);

/**********************************************************************************************************************
 *  EaEr_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameters
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Cancel(void);


# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER1_ERASE_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Erase.h
 *********************************************************************************************************************/
