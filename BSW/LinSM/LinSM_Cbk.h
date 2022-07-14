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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  LinSM_Cbk.h
 *        \brief  LIN State Manager ASR4
 *
 *      \details  Implementation of the LIN State Manager AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the LinSM.h
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the LinSM.h
 *********************************************************************************************************************/


#if !defined (LINSM_CBK_H)
#define LINSM_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "LinSM_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define LINSM_CBK_MAJOR_VERSION   0x05u
#define LINSM_CBK_MINOR_VERSION   0x02u
#define LINSM_CBK_PATCH_VERSION   0x00u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define LINSM_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * LinSM_WakeupConfirmation
 *********************************************************************************************************************/
/*! \brief       Handles the confirmation of a wake-up request
 *  \details     The LinIf calls this callback when the wake-up signal command has been sent successfully on the
 *               network. The LinSM stops the confirmation timeout timer and changes to full communication state.
 *  \param[in]   network         System channel handle
 *  \param[in]   success         True if wake-up was successfully sent, false otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-10416, SPEC-10421, SPEC-10447, SPEC-10449, SPEC-10483, SPEC-10492, SPEC-17342, SPEC-17371,
 *               SPEC-17385, SPEC-17416
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_WakeupConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                        VAR(boolean, AUTOMATIC) success );

/**********************************************************************************************************************
 * LinSM_GotoSleepConfirmation
 *********************************************************************************************************************/
/*! \brief       Handles the confirmation of a go-to-sleep request
 *  \details     The LinIf calls this callback when the go-to-sleep command has been sent successfully on the
 *               network. The LinSM stops the confirmation timeout timer, switches the transceiver to standby mode (if
 *               the transceiver handling is enabled) and changes to state LINSM_NO_COM.
 *  \param[in]   network         System channel handle
 *  \param[in]   success         True if goto sleep was successfully sent, false otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-10390, SPEC-10430, SPEC-10449, SPEC-10464, SPEC-10492, SPEC-17342, SPEC-17368, SPEC-17390, 
 *               SPEC-17398, SPEC-17438, SPEC-17886
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_GotoSleepConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                           VAR(boolean, AUTOMATIC) success );

/**********************************************************************************************************************
 * LinSM_ScheduleRequestConfirmation
 *********************************************************************************************************************/
/*! \brief       Handles the confirmation of a schedule request
 *  \details     The LinIf module calls this callback when the new requested schedule table is active.
 *  \param[in]   network         System channel handle
 *  \param[in]   schedule        Index of the new active Schedule table
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-10449, SPEC-17342, SPEC-17349, SPEC-17387, SPEC-17397, SPEC-17401, SPEC-17419, SPEC-17427
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_ScheduleRequestConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                                 VAR(LinIf_SchHandleType, AUTOMATIC) schedule );

#if ( LINSM_SCHEDULE_END_NOTIFICATION == STD_ON )
/* ESCAN00080029 */
/**********************************************************************************************************************
 * LinSM_ScheduleEndNotification()
 *********************************************************************************************************************/
/*! \brief       Handles the schedule table end notification
 *  \details     The LinIf module calls this callback at the end of each schedule table.
 *  \param[in]   network         System channel handle
 *  \param[in]   schedule        Index of the current ending Schedule table
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINSM_SCHEDULE_END_NOTIFICATION == STD_ON
 *  \trace       SPEC-10492
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_ScheduleEndNotification( VAR(NetworkHandleType, AUTOMATIC) network,
                                                             VAR(LinIf_SchHandleType, AUTOMATIC) schedule );
#endif

#define LINSM_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif  /* LINSM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: LinSM_Cbk.h
 *********************************************************************************************************************/
