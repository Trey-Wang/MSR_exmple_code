/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: VTTOs
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Os_Hook_Cfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_HOOK_CFG_H)                                                         /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_HOOK_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Defines whether the OS shall call startup hooks (STD_ON) or not (STD_OFF). */
# define OS_CFG_STARTUPHOOKS                     (STD_OFF)

/*! Defines whether the OS shall call shutdown hooks (STD_ON) or not (STD_OFF). */
# define OS_CFG_SHUTDOWNHOOKS                    (STD_OFF)

/*! Defines whether the OS shall call error hooks (STD_ON) or not (STD_OFF). */
# define OS_CFG_ERRORHOOKS                       (STD_ON)

/*! Defines whether the OS shall call the protection hook (STD_ON) or not (STD_OFF). */
# define OS_CFG_PROTECTIONHOOK                   (STD_OFF)

/*! Number of all configured Hooks. */
# define OS_CFG_NUM_HOOKS                        (2UL)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


#endif /* OS_HOOK_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hook_Cfg.h
 *********************************************************************************************************************/
