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
 *              File: Os_Hal_Vtt_Cfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (Os_Hal_Vtt_CFG_H)
# define Os_Hal_Vtt_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Defines whether VTT debug information is active (STD_ON) or not (STD_OFF). */
#define OS_CFG_HAL_DEBUG_VTT   (STD_OFF)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*******************
 * Category 2 ISRs
 *******************/

extern void Os_Hal_CanoeIrqHandler_CanIsrErr_0(void);
extern void Os_Hal_CanoeIrqTask_CanIsrErr_0(void);

extern void Os_Hal_CanoeIrqHandler_CanIsrRx_0(void);
extern void Os_Hal_CanoeIrqTask_CanIsrRx_0(void);

extern void Os_Hal_CanoeIrqHandler_CanIsrTx_0(void);
extern void Os_Hal_CanoeIrqTask_CanIsrTx_0(void);

extern void Os_Hal_CanoeIrqHandler_CanIsrWakeup_0(void);
extern void Os_Hal_CanoeIrqTask_CanIsrWakeup_0(void);

extern void Os_Hal_CanoeIrqHandler_CounterIsr_SystemTimer(void);
extern void Os_Hal_CanoeIrqTask_CounterIsr_SystemTimer(void);


/*******************
 * Category 1 ISRs
 *******************/

extern void VttSysVarIsr_0(void);
extern void Os_Hal_CanoeIrqHandler_VttSysVarIsr_0(void);


/*******************
 * Category 0 ISRs
 *******************/


#endif /* Os_Hal_Vtt_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Vtt_Cfg.h
 *********************************************************************************************************************/
