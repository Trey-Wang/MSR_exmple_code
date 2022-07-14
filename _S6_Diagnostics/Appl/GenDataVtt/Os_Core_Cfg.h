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
 *              File: Os_Core_Cfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_CORE_CFG_H)                                                         /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_CORE_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Number of user barriers. */
# define OS_CFG_NUM_BARRIERS                     (0UL)

/*! Number of ISR2 nesting levels. */
# define OS_CFG_NUM_ISRLEVELS                    (1UL)

/*! Number of all spinlocks. */
# define OS_CFG_NUM_SYSTEM_SPINLOCKS             (0UL)

/*! Number of physical core identifiers. */
# define OS_CFG_COREPHYSICALID_COUNT             (1UL)

/* Number of core objects: OsCore */
# define OS_CFG_NUM_CORE_OSCORE_APPS             (0UL)
# define OS_CFG_NUM_CORE_OSCORE_HOOKS            (2UL)
# define OS_CFG_NUM_CORE_OSCORE_IOCS             (0UL)
# define OS_CFG_NUM_CORE_OSCORE_RESOURCES        (1UL)
# define OS_CFG_NUM_CORE_OSCORE_STACKS           (9UL)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


#endif /* OS_CORE_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Core_Cfg.h
 *********************************************************************************************************************/
