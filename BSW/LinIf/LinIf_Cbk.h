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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  LinIf_Cbk.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Callback Functions of the AUTOSAR LIN Interface
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Bastian Molkenthin            visbmo        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-07-15  visbmo  ESCAN00076480 AR4-832: External wake up via LIN transceiver
 *  04.01.00  2015-01-07  visbmo  ESCAN00080299 FEAT-427: SafeBSW Step I
 *  04.02.00  2015-05-18  visbmo  ESCAN00081787 FEAT-1275: SafeBSW Step 2
 *  05.00.00  2015-11-05  visbmo  ESCAN00084958 Adapt to updated R14 generators (breaking change and CSL)
 *                                ESCAN00085822 Compiler error: LinTp_IsInvalidHndOfTxNSdu undefined
 *                                ESCAN00086031 Compiler error: Linker cannot found symbol LinIf_WakeupConfirmation
 *                                ESCAN00086282 Rework wakeup source handling
 *  05.01.00  2016-02-04  visbmo  ESCAN00087805 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00087859 FEAT-1731: J2602 Frame Tolerance Support
 *  05.01.01  2016-07-07  visbmo  ESCAN00090787 Communication starts after external wakeup before wakeup delay has elapsed
 *  05.02.00  2017-01-19  visbmo  FEATC-565     FEAT-2161: Support for LIN drivers with different interface type
 *                                FEATC-568     FEAT-2263: LIN ScheduleTable change after frame transmission
 *                                ESCAN00093427 Support for callback to provide Lin status to application
 *                                ESCAN00093428 Support for schedule table handler information API
 *                                ESCAN00093616 Support for particular wakeup delay after external wakeup
 *********************************************************************************************************************/

#if !defined (LINIF_CBK_H)
# define LINIF_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"
/* required for type EcuM_WakeupSourceType */
# include "EcuM.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define LINIF_CBK_MAJOR_VERSION                    (5u)
# define LINIF_CBK_MINOR_VERSION                    (2u)
# define LINIF_CBK_PATCH_VERSION                    (0u)

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
**********************************************************************************************************************/

# define LINIF_START_SEC_CODE
# include "MemMap.h"

/***********************************************************************************************************************
 *  LinIf_WakeupConfirmation
 **********************************************************************************************************************/
/*! \brief       Called after an external wakeup event was detected.
 *  \details     This function is called by LIN Driver or LIN Transceiver Driver to report the wake up source after the
 *               successful wakeup detection during CheckWakeup or after power on by bus.
 *  \param[in]   WakeupSource            Source of the wakeup event.
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different wakeup sources
 *  \synchronous TRUE
 *  \pre         This module must be initialized before this service function is called.
 *  \trace       CREQ-572
 **********************************************************************************************************************/
extern FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource
);

# define LINIF_STOP_SEC_CODE
# include "MemMap.h"

#endif /* LINIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Cbk.h
 *********************************************************************************************************************/
