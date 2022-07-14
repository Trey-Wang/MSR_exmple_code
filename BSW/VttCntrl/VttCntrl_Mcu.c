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
*        \brief   Implements the public API of the virtual microcontroller unit (MCU) integrated in the vVIRTUALtarget.
*
*      \details   Virtual MCU that enables to let the virtual CPU reset, sleep or power down.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the module's header file.
*
*  FILE VERSION
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the VERSION CHECK below.
*********************************************************************************************************************/

#define VTTMCU_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Mcu.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the virtual MCU is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Mcu header file. */
#if (  (VTTMCU_SW_MAJOR_VERSION != (3u)) \
    || (VTTMCU_SW_MINOR_VERSION != (0u)) \
    || (VTTMCU_SW_PATCH_VERSION != (11u)) )
  #error "Vendor specific version numbers of VttCntrl_Mcu.c and VttCntrl_Mcu.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttMcu_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Last reset reason. */
STATIC VttMcu_ResetType VttMcu_ResetReason = VTTMCU_POWER_ON_RESET;
/*! \brief CANoe handle for the system variable storing the reset reason. */
STATIC sint32 VttMcu_SysvarHandle_ResetReason = 0;

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttMcu_OnLoad(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttMcu_OnLoad()
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual MCU.
*
*         Tasks:
*            - Allocate a system variable for storing the last occurred reset reason.
*
*  \context    Called by VttMcu_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttMcu_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttMcu_SysvarHandle_ResetReason = CANoeAPI_SysVar_DeclareInt("Mcu", "ResetReason", VttMcu_ResetReason);

}  /* VttMcu_OnLoad() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttMcu_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual MCU about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttMcu_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttMcu_EcuState = newState;

  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
  {
    VttMcu_OnLoad();
    break;
  }
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
  case CANOEAPI_ECUACTION_SWITCHON:
  case CANOEAPI_ECUACTION_RESET:
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

}  /* VttMcu_OnStateChange() */

/***********************************************************************************************************************
*  VttMcu_PerformReset
**********************************************************************************************************************/
/*! \brief     Invokes a reset of the MCU.
*
*   \context   Called by Mcu driver. 
**********************************************************************************************************************/
void VttMcu_PerformReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttMcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_SysVar_SetInt(VttMcu_SysvarHandle_ResetReason, VTTMCU_SW_RESET);
    CANoeAPI_WriteString("VTT MCU - Performing reset!");
    CANoeAPI_ResetProcessor();
  }

}  /* VttMcu_PerformReset() */

/***********************************************************************************************************************
*  VttMcu_GetResetReason
**********************************************************************************************************************/
/*! \brief     Returns the last reset reason of the virtual MCU.
*
*  \return     Reset reason, refer to module's header file for a list of possible reset reasons (VttMcu_ResetType).
*  \context    Called by Mcu driver.
**********************************************************************************************************************/
VttMcu_ResetType VttMcu_GetResetReason(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return (VttMcu_ResetType)CANoeAPI_SysVar_GetInt(VttMcu_SysvarHandle_ResetReason);

}  /* VttMcu_GetResetReason() */

/***********************************************************************************************************************
*  VttMcu_SetMode
**********************************************************************************************************************/
/*! \brief      Changes the operation mode of the virtual MCU.
*   \details    Can be used to let the MCU sleep, reset or power down.
*
*  \param[in]   mcuMode     Operation mode of the virtual MCU:
*                           VTTMCU_MODE_NORMAL = 1,
*                           VTTMCU_MODE_SLEEP = 2,
*                           VTTMCU_MODE_RESET = 3,
*                           VTTMCU_MODE_POWER_OFF = 4
*  \context    Called by Mcu driver.
**********************************************************************************************************************/
void VttMcu_SetMode(VttMcu_ModeType mcuMode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttMcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if (mcuMode == VTTMCU_MODE_NORMAL)
    {
       /* Nothing to do here. May set a breakpoint for debugging purpose e.g. analyzing wakeup behaviour. */
      ;
    }
    else if(mcuMode == VTTMCU_MODE_SLEEP)
    {
      CANoeEmuProcessor_GotoSleep();
    }
    else if(mcuMode == VTTMCU_MODE_RESET)
    {
      CANoeAPI_SysVar_SetInt(VttMcu_SysvarHandle_ResetReason, VTTMCU_SW_RESET);
      CANoeAPI_WriteString("VTT MCU - Performing reset!");
      CANoeAPI_ResetProcessor();
    }
    else if(mcuMode == VTTMCU_MODE_POWER_OFF)
    {
      CANoeEmuProcessor_PowerOff();
    }
    else
    {
      /* Unknown MCU Mode */
    }
  }

}  /* VttMcu_SetMode() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Mcu.c
*********************************************************************************************************************/
