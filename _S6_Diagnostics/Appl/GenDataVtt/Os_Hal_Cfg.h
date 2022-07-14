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
 *              File: Os_Hal_Cfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_HAL_CFG_H)                                                          /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_HAL_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! HAL configuration major version identification. */
# define OS_CFG_HAL_MAJOR_VERSION                (2U)

/*! HAL configuration minor version identification. */
# define OS_CFG_HAL_MINOR_VERSION                (7U)

/* ISR core and level definitions */
# define OS_ISR_CANISRERR_0_CORE      (0)
# define OS_ISR_CANISRERR_0_LEVEL     (1)
# define OS_ISR_CANISRRX_0_CORE      (0)
# define OS_ISR_CANISRRX_0_LEVEL     (1)
# define OS_ISR_CANISRTX_0_CORE      (0)
# define OS_ISR_CANISRTX_0_LEVEL     (1)
# define OS_ISR_CANISRWAKEUP_0_CORE      (0)
# define OS_ISR_CANISRWAKEUP_0_LEVEL     (1)
# define OS_ISR_COUNTERISR_SYSTEMTIMER_CORE      (0)
# define OS_ISR_COUNTERISR_SYSTEMTIMER_LEVEL     (1)
# define OS_ISR_VTTSYSVARISR_0_CORE      (0)
# define OS_ISR_VTTSYSVARISR_0_LEVEL     (2)

/* Hardware counter timing macros */

/* Counter timing macros and constants: SystemTimer */
# define OSMAXALLOWEDVALUE_SystemTimer     (2147483647UL) /* 0x7FFFFFFFUL */
# define OSMINCYCLE_SystemTimer            (1UL)
# define OSTICKSPERBASE_SystemTimer        (1UL)
# define OSTICKDURATION_SystemTimer        (1000000UL)

/* OSEK compatibility for the system timer. */
# define OSMAXALLOWEDVALUE     (OSMAXALLOWEDVALUE_SystemTimer)
# define OSMINCYCLE            (OSMINCYCLE_SystemTimer)
# define OSTICKSPERBASE        (OSTICKSPERBASE_SystemTimer)
# define OSTICKDURATION        (OSTICKDURATION_SystemTimer)

/*! Macro OS_NS2TICKS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_NS2TICKS_SystemTimer(x)     ( (TickType) (((((uint32)(x)) * 1) + 500000) / 1000000) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Macro OS_TICKS2NS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_TICKS2NS_SystemTimer(x)     ( (PhysicalTimeType) (((((uint32)(x)) * 1000000) + 0) / 1) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */

/*! Macro OS_US2TICKS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_US2TICKS_SystemTimer(x)     ( (TickType) (((((uint32)(x)) * 1) + 500) / 1000) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Macro OS_TICKS2US_SystemTimer was approximated with a deviation of 1.1102230246251565E-10ppm. */
# define OS_TICKS2US_SystemTimer(x)     ( (PhysicalTimeType) (((((uint32)(x)) * 1000) + 0) / 1) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */

/*! Macro OS_MS2TICKS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_MS2TICKS_SystemTimer(x)     ( (TickType) (((((uint32)(x)) * 1) + 0) / 1) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Macro OS_TICKS2MS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_TICKS2MS_SystemTimer(x)     ( (PhysicalTimeType) (((((uint32)(x)) * 1) + 0) / 1) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */

/*! Macro OS_SEC2TICKS_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_SEC2TICKS_SystemTimer(x)     ( (TickType) (((((uint32)(x)) * 1000) + 0) / 1) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Macro OS_TICKS2SEC_SystemTimer was approximated with a deviation of 0.0ppm. */
# define OS_TICKS2SEC_SystemTimer(x)     ( (PhysicalTimeType) (((((uint32)(x)) * 1) + 500) / 1000) )     /* PRQA S 3453 */ /* MD_MSR_19.7 */






#define OS_IRQ_CanIsrErr_0 0
#define OS_IRQ_CanIsrRx_0 1
#define OS_IRQ_CanIsrTx_0 2
#define OS_IRQ_CanIsrWakeup_0 3
#define OS_IRQ_CounterIsr_SystemTimer 4
#define OS_IRQ_VttSysVarIsr_0 5
#define OS_CANOE_IRQ_COUNT 6


#define OS_CFG_CAT1_ISR_COUNT         (1)
#define OS_CFG_MAX_INT_LEVEL          (2)

typedef enum
{
  /* OsCore */
  CANoeTaskId_IO_Task = 1,
  CANoeTaskId_IdleTask_OsCore = 2,
  CANoeTaskId_Init_Task = 3,
  CANoeTaskId_My_Task = 4,
  CANoeTaskId_SchM_Task = 5,
  CANoeTaskId_CanIsrErr_0 = 6,
  CANoeTaskId_CanIsrRx_0 = 7,
  CANoeTaskId_CanIsrTx_0 = 8,
  CANoeTaskId_CanIsrWakeup_0 = 9,
  CANoeTaskId_CounterIsr_SystemTimer = 10,
  CANoeTaskId_VttSysVarIsr_0 = 11,
  CANoeTaskId_Os_CoreInitHook_OsCore = 12,
  CANoeTaskId_ErrorHook_OsCore = 13,
  
} CANoeTaskIdType;

typedef enum
{
  CANoeHrtId_STM0_Ch0 = 1,
  OS_CANOE_HRT_COUNT = 2
} CANoeHrtIdType;

typedef enum
{
  CANoePitId_PIT0_Ch0  = 1,
  OS_CANOE_PIT_COUNT = 2
} CANoePitIdType;



/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


#endif /* OS_HAL_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Cfg.h
 *********************************************************************************************************************/
