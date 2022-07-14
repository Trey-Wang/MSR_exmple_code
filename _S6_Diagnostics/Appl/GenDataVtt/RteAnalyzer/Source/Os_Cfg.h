/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Os_Cfg.h
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Os definitions
 *********************************************************************************************************************/

#ifndef _OS_CFG_H_
# define _OS_CFG_H_

/* Os definitions */

/* Tasks */
# define IO_Task (0U)
# define My_Task (1U)
# define SchM_Task (2U)

/* Alarms */
# define Rte_Al_TE_IO_Task_0_200ms (0U)
# define Rte_Al_TE_CpApMySwc_RCtApMySwcCode (1U)
# define Rte_Al_TE2_SchM_Task_0_10ms (2U)
# define Rte_Al_TE2_SchM_Task_0_20ms (3U)
# define Rte_Al_TE2_SchM_Task_0_5ms (4U)

/* Events */
# define Rte_Ev_Cyclic2_SchM_Task_0_10ms (0x02)
# define Rte_Ev_Cyclic2_SchM_Task_0_20ms (0x04)
# define Rte_Ev_Cyclic2_SchM_Task_0_5ms (0x01)
# define Rte_Ev_Cyclic_IO_Task_0_200ms (0x08)
# define Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode (0x02)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcCode (0x01)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry (0x04)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit (0x08)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode (0x10)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling (0x04)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling (0x02)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue (0x20)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending (0x10)
# define Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight (0x01)

/* Spinlocks */

/* Resources */

/* ScheduleTables */

/* Cores */

/* Trusted Functions */


#endif /* _OS_CFG_H_ */
