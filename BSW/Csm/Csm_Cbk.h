/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Csm_Cbk.h
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file (Csm.h).
 *********************************************************************************************************************/

#if !defined (CSM_CBK_H)
#define CSM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPE
 *********************************************************************************************************************/
#define CSM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Csm_CallbackNotification()
 *********************************************************************************************************************/
/*! \brief         Callback Notification
 *  \details       Notifies the Csm that a job has finished. This function is used by the underlying layer (CryIf)
 *  \param[in]     job          Holds a pointer to the job, which has finished.
 *  \param[in]     result       Contains the result of the cryptographic operation.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         SPEC-2820476, SPEC-2820477, SPEC-2820478
 *********************************************************************************************************************/
FUNC(void, CSM_CODE) Csm_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
                                              Std_ReturnType result);

#define CSM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CSM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: CSM_CBK.H
 *********************************************************************************************************************/

