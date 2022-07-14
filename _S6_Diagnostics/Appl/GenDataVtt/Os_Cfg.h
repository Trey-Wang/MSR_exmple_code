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
 *              File: Os_Cfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_CFG_H)                                                              /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */

/* Os module declarations */

/* Os kernel module dependencies */

/* Os hal dependencies */
# include "Os_Hal_Cfg.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Configuration major version identification. */
# define OS_CFG_MAJOR_VERSION                    (2U)

/*! Configuration minor version identification. */
# define OS_CFG_MINOR_VERSION                    (10U)


/*! Defines which platform is used. */
# define OS_CFG_PLATFORM_VTT     (STD_ON)

/*! Defines which derivative group is configured. */
# define OS_CFG_DERIVATIVEGROUP_VTT     (STD_ON)

/*! Defines which derivative is configured. */
# define OS_CFG_DERIVATIVE_CANOEEMU     (STD_ON)

/*! Defines which compiler is configured. */
# define OS_CFG_COMPILER_ANSI     (STD_ON)


/*! Defines whether access macros to get context related information in the error hook are enabled (STD_ON) or not (STD_OFF). */
# define OS_CFG_ERR_PARAMETERACCESS              (STD_OFF)

/*! Defines whether access macros to get service ID information in the error hook are enabled (STD_ON) or not (STD_OFF). */
# define OS_CFG_ERR_GETSERVICEID                 (STD_OFF)

/*! Defines whether the pre-task hook is active (STD_ON) or not (STD_OFF). */
# define OS_CFG_PRETASKHOOK                      (STD_OFF)

/*! Defines whether the post-task hook is active (STD_ON) or not (STD_OFF). */
# define OS_CFG_POSTTASKHOOK                     (STD_OFF)

/*! Defines whether the OS shall call the panic hook (STD_ON) or not (STD_OFF). */
# define OS_CFG_PANICHOOK                        (STD_OFF)

/*! Defines whether the system startup hook is configured (STD_ON) or not (STD_OFF). */
# define OS_CFG_STARTUPHOOK_SYSTEM               (STD_OFF)

/*! Defines whether the system shutdown hook is configured (STD_ON) or not (STD_OFF). */
# define OS_CFG_SHUTDOWNHOOK_SYSTEM              (STD_OFF)

/*! Defines whether the system error hook is configured (STD_ON) or not (STD_OFF). */
# define OS_CFG_ERRORHOOK_SYSTEM                 (STD_ON)

/*! Defines whether the system protection hook is configured (STD_ON) or not (STD_OFF). */
# define OS_CFG_PROTECTIONHOOK_SYSTEM            (STD_OFF)

/*! Defines whether backward compatibility defines are needed (STD_ON) or not (STD_OFF). */
# define OS_CFG_PERIPHERAL_COMPATIBILITY         (STD_OFF)


/* OS application modes */
# define DONOTCARE     ((AppModeType)0)
# define OS_APPMODE_NONE     ((AppModeType)0)
# define OSDEFAULTAPPMODE     ((AppModeType)1)
# define OS_APPMODE_ANY     ((AppModeType)255)


/* Event masks */
# define Rte_Ev_Cyclic2_SchM_Task_0_10ms     ((EventMaskType)1ULL)
# define Rte_Ev_Cyclic2_SchM_Task_0_20ms     ((EventMaskType)2ULL)
# define Rte_Ev_Cyclic2_SchM_Task_0_5ms     ((EventMaskType)4ULL)
# define Rte_Ev_Cyclic_IO_Task_0_200ms     ((EventMaskType)1ULL)
# define Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode     ((EventMaskType)1ULL)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcCode     ((EventMaskType)2ULL)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry     ((EventMaskType)4ULL)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit     ((EventMaskType)8ULL)
# define Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode     ((EventMaskType)16ULL)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling     ((EventMaskType)2ULL)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling     ((EventMaskType)4ULL)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue     ((EventMaskType)8ULL)
# define Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending     ((EventMaskType)16ULL)
# define Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight     ((EventMaskType)32ULL)


/* Software counter timing macros */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


#endif /* OS_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Cfg.h
 *********************************************************************************************************************/
