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
 *        \file  Ea_Layer3_ReadManagementBytes.h
 *        \brief  ReadManagementBytes is responsible for reading both header and trailer information of passed instance.
 *
 *      \details  ReadManagementBytes is responsible for reading both header and trailer information of passed instance.
 *                It distinguishes between two ways of reading. Either the passed instance is very small and both header
 *                and trailer can be read at once, or EEP is accessed two times for reading header and trailer separately.
 *                Read values are stored in passed instance.
 *
 *********************************************************************************************************************/


#ifndef EA_LAYER3_READ_MANAGEMENT_BYTES_H
# define EA_LAYER3_READ_MANAGEMENT_BYTES_H

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
 *  EaRdMgmt_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes internal parameter of ReadManagementBytes sub-component
 *  \details     Initializes internal parameter of ReadManagementBytes sub-component
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Init(void);

/**********************************************************************************************************************
 *  EaRdMgmt_GetStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current status of ReadManagementBytes sub-component
 * \details    Returns current status of ReadManagementBytes sub-component
 * \return     Status of ReadManagementBytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaRdMgmt_GetStatus(void);

/**********************************************************************************************************************
 *  EaRdMgmt_GetJobResult()
 *********************************************************************************************************************/
/*!
 * \brief      Returns current job result of ReadManagementBytes sub-component
 * \details    Returns current job result of ReadManagementBytes sub-component
 * \return     Job result of ReadManagementBytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaRdMgmt_GetJobResult(void);

/**********************************************************************************************************************
 *  EaRdMgmt_StartJob()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates ReadManagementBytes sub-component's state machine
 * \details    Initiates ReadManagementBytes sub-component's state machine and registers ReadManagementBytes
 *             sub-component to TaskManager
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \return     E_OK: Job was accepted by ReadManagementBytes sub-component
 * \return     E_NOT_OK: Job request failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaRdMgmt_StartJob(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaRdMgmt_Execute()
 *********************************************************************************************************************/
/*!
 * \brief      Processes internal state machine
 * \details    Processes state machine of ReadManagementBytes sub-component
 * \pre        State machine is initialized.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Execute(void);

/**********************************************************************************************************************
 *  EaRdMgmt_Cancel()
 *********************************************************************************************************************/
/*!
 * \brief      Cancels currently running job
 * \details    Cancels currently running job and resets all internal parameter
 * \pre        Sub-component is not idle.
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Cancel(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_LAYER3_READ_MANAGEMENT_BYTES_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer3_ReadManagementBytes.h
 *********************************************************************************************************************/
