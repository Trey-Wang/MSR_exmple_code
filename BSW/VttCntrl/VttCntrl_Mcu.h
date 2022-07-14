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
*  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VTTCntrl_Mcu.h
*        \brief   Declares the public API of the virtual microcontroller unit (MCU) integrated in the vVIRTUALtarget.
*
*      \details   Virtual MCU that enables to let the virtual CPU reset, sleep or power down.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel          vismsl        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-24  vismsl  -             Initial version extracted from VTTCntrl generator.
 *  03.00.04  2015-09-22  visdap  -             ESCAN00085394: Added pragmas to protect VTTCntrl during simulated
 *                                              ECU reset.
 *  03.00.06  2015-10-15  vismsl  -             Removed consumption of simulation time when performing a MCU reset.
 *  03.00.07  2015-10-22  vismsl  -             Added a MCU_NORMAL_MODE branch to Mcu_SetMode. Set SysVar ResetReason 
 *                                              to MCU_SW_RESET in VttMcu_SetMode in case of a MCU reset.
 *  03.00.08  2015-11-16  vismsl  -             Added debug printout to inform user about a MCU reset.
 *  03.00.09  2015-12-22  vismsl  -             Defined VTT Mcu Modes as part of VTTCntrl_Mcu instead of
 *                                              VTTMcu driver. Analog for VTT Mcu Reset Sources.
 *  03.00.10  2016-01-08  vismsl  ESCAN00087641 GetReasonReason, access to system variable fixed.
 *  03.00.11  2016-01-18  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*********************************************************************************************************************/

#if !defined (VTTCNTRL_MCU_H)
#define VTTCNTRL_MCU_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Component Version Information */
#define VTTMCU_SW_MAJOR_VERSION       (3u)
#define VTTMCU_SW_MINOR_VERSION       (0u)
#define VTTMCU_SW_PATCH_VERSION       (11u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/* ----- Reset reasons ----- */
typedef enum
{
  VTTMCU_POWER_ON_RESET = 0,        /*! \brief Caused due power on of MCU (processor). */
  VTTMCU_WATCHDOG_RESET = 1,        /*! \brief Caused due reset by watchdog. */
  VTTMCU_SW_RESET = 2,              /*! \brief Caused due reset by software (e.g. EcuM). */
  VTTMCU_ILLEGALADDRESS_RESET = 3,  /*! \brief Caused due access to illegal memory address. */
  VTTMCU_LOW_VOLTAGE_RESET = 4,     /*! \brief Caused when voltage supply for MCU is too low. */
  VTTMCU_EXTERNAL_RESET = 5,        /*! \brief Caused due to external source (e.g. reset pin triggered). */
  VTTMCU_CLOCK_MONITOR_RESET = 6,   /*! \brief Caused when monitoring of MCU clock failed. */
  VTTMCU_RESET_UNDEFINED = 7        /*! \brief Undefined reset reason. */
} VttMcu_ResetType;

/* ----- Virtual MCU operation modes ----- */
typedef enum
{
  VTTMCU_MODE_UNDEF = 0,    /*! \brief Undefined mode, only for internal use. */
  VTTMCU_MODE_NORMAL = 1,   /*! \brief Normal mode, MCU is initialized an running. */
  VTTMCU_MODE_SLEEP = 2,    /*! \brief Sleep mode, processor is sleeping. */
  VTTMCU_MODE_RESET = 3,    /*! \brief Rest mode, invokes a reset of the processor, restart of the ECU stack. */
  VTTMCU_MODE_POWER_OFF = 4 /*! \brief Power off mode, shuts down the power supply for the processor. */
} VttMcu_ModeType;

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

void VttMcu_SetMode(VttMcu_ModeType mcuMode);
void VttMcu_PerformReset(void);
VttMcu_ResetType  VttMcu_GetResetReason(void);
void VttMcu_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_MCU_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Mcu.h
 *********************************************************************************************************************/
