/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CryIf_Cbk.h
 *        \brief  MICROSAR Crypto Interface (CRYIF)
 *
 *      \details  The Crypto Interface module provides a unique interface to manage different Crypto HW and SW
 *                solutions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYIF_CBK_H)
# define CRYIF_CBK_H

#include "CryIf.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CryIf_CallbackNotification()
 *********************************************************************************************************************/
/*! \brief       CryIf Callback Notification
 *  \details     Notifies the CRYIF about the completion of the request with the result of the cryptographic operation.
 *  \param[in]   job                       Points to the completed job's information structure. It contains a
 *                                         callbackID to identify which job is finished.
 *  \param[in]   result                    Contains the result of the cryptographic operation.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-127574
 *********************************************************************************************************************/
FUNC(void, CRYIF_CODE) CryIf_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job,
  Std_ReturnType result);

# define CRYIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* CRYIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: CryIf_Cbk.h
 *********************************************************************************************************************/
