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
 *              File: Os_Types_Lcfg.h
 *   Generation Time: 2020-03-02 16:11:32
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_TYPES_LCFG_H)                                                       /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_TYPES_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */

/* Os hal dependencies */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! OS-Application identifier. */
typedef enum
{
  SystemApplication_OsCore = 0, /* 0x00000001 */
  OS_APPID_COUNT = 1,
  INVALID_OSAPPLICATION = OS_APPID_COUNT
} ApplicationType;

/*! Trusted function identifier. */
typedef enum
{
  OS_TRUSTEDFUNCTIONID_COUNT = 0
} TrustedFunctionIndexType;

/*! Non-Trusted function identifier. */
typedef enum
{
  OS_NONTRUSTEDFUNCTIONID_COUNT = 0
} Os_NonTrustedFunctionIndexType;

/*! Fast Trusted function identifier. */
typedef enum
{
  OS_FASTTRUSTEDFUNCTIONID_COUNT = 0
} Os_FastTrustedFunctionIndexType;

/*! Task identifier. */
typedef enum
{
  IO_Task = 0,
  IdleTask_OsCore = 1,
  Init_Task = 2,
  My_Task = 3,
  SchM_Task = 4,
  OS_TASKID_COUNT = 5,
  INVALID_TASK = OS_TASKID_COUNT
} TaskType;

/*! Category 2 ISR identifier. */
typedef enum
{
  CanIsrErr_0 = 0,
  CanIsrRx_0 = 1,
  CanIsrTx_0 = 2,
  CanIsrWakeup_0 = 3,
  CounterIsr_SystemTimer = 4,
  OS_ISRID_COUNT = 5,
  INVALID_ISR = OS_ISRID_COUNT
} ISRType;

/*! Alarm identifier. */
typedef enum
{
  Rte_Al_TE2_SchM_Task_0_10ms = 0,
  Rte_Al_TE2_SchM_Task_0_20ms = 1,
  Rte_Al_TE2_SchM_Task_0_5ms = 2,
  Rte_Al_TE_CpApMySwc_RCtApMySwcCode = 3,
  Rte_Al_TE_IO_Task_0_200ms = 4,
  OS_ALARMID_COUNT = 5
} AlarmType;

/*! Counter identifier. */
typedef enum
{
  SystemTimer = 0,
  OS_COUNTERID_COUNT = 1
} CounterType;

/*! ScheduleTable identifier. */
typedef enum
{
  OS_SCHTID_COUNT = 0
} ScheduleTableType;

/*! Resource identifier. */
typedef enum
{
  OsResource = 0,
  OS_RESOURCEID_COUNT = 1
} ResourceType;

/*! Spinlock identifier. */
typedef enum
{
  OS_SPINLOCKID_COUNT = 0,
  INVALID_SPINLOCK = OS_SPINLOCKID_COUNT
} SpinlockIdType;

/*! Peripheral identifier. */
typedef enum
{
  OS_PERIPHERALID_COUNT = 0
} Os_PeripheralIdType;

/*! Trace thread identifier (Tasks and ISRs inclusive system objects). */
typedef enum
{
  OS_TRACE_THREADID_COUNT = 0,
  OS_TRACE_INVALID_THREAD = OS_TRACE_THREADID_COUNT + 1
} Os_TraceThreadIdType;

/*! Trace spinlock identifier (All spinlocks inclusive system objects). */
typedef enum
{
  OS_TRACE_NUMBER_OF_CONFIGURED_SPINLOCKS = OS_SPINLOCKID_COUNT,
  OS_TRACE_NUMBER_OF_ALL_SPINLOCKS = OS_SPINLOCKID_COUNT + 0,
  OS_TRACE_INVALID_SPINLOCK = OS_TRACE_NUMBER_OF_ALL_SPINLOCKS + 1
} Os_TraceSpinlockIdType;

/*! Barrier identifiers. */
typedef enum
{
  OS_BARRIERID_COUNT = 0
} Os_BarrierIdType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


#endif /* OS_TYPES_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Types_Lcfg.h
 *********************************************************************************************************************/
