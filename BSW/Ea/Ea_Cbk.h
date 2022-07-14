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
 *         \file  Ea_Cbk.h
 *        \brief  EA callback header provides callback services of EA module to EEPROM driver
 *
 *      \details  EA callback header provides callback services of EA module to EEPROM driver. These services are called
 *                by EEPROM driver upon job end if Polling Mode is deactivated. In case the EEPROM driver finishes a job
 *                successfully Ea_JobEndNotification() is called. On the other hand, in case of erroneous job end
 *                Ea_JobErrorNotification() is called.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#ifndef EA_CBK_H
# define EA_CBK_H

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define EA_CBK_MAJOR_VERSION    (3)
# define EA_CBK_MINOR_VERSION    (2)
# define EA_CBK_PATCH_VERSION    (2)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES   \trace SPEC-41673
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h"

/**********************************************************************************************************************
 *  Ea_JobEndNotification()
 *********************************************************************************************************************/
/*!
 * \brief      Sets the EA internal job result of EEPROM driver
 * \details    Sets the EA internal job result of EEPROM driver in case of successful job ending
 * \pre        EA_EEP_POLLING_MODE is enabled
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-715
 * \trace SPEC-41656
 */
FUNC(void, EA_PUBLIC_CODE) Ea_JobEndNotification(void);

/**********************************************************************************************************************
 *  Ea_JobErrorNotification()
 *********************************************************************************************************************/
/*!
 * \brief      Sets the EA internal job result of EEPROM driver
 * \details    Sets the EA internal job result of EEPROM driver in case of erroneous job ending
 * \pre        EA_EEP_POLLING_MODE is enabled
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace CREQ-716
 * \trace SPEC-41717
 */
FUNC(void, EA_PUBLIC_CODE) Ea_JobErrorNotification(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h"

#endif /* EA_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: Ea_Cbk.h
 *********************************************************************************************************************/
