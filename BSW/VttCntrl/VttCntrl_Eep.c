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
/**      \file    VTTCntrl_Eep.c
*        \brief   Implements the public API of the virtual EEPROM controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual EEPROM controller that enables to read and write bytes to a simulated EEPROM device.
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

#define VTTEEP_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Eep.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the EEPROM controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of VttCntrl_Eep header file. */
#if (  (VTTEEP_SW_MAJOR_VERSION != (3u)) \
    || (VTTEEP_SW_MINOR_VERSION != (0u)) \
    || (VTTEEP_SW_PATCH_VERSION != (6u)) )
# error "Vendor specific version numbers of VttCntrl_Eep.c and VttCntrl_Eep.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTEEP_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTEEP_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTEEP_CFG_PATCH_VERSION != (0x02u)))
  #error "Vendor specific version numbers of VttCntrl_Eep.c and VttCntrl_Eep_Cfg.h are inconsistent!"
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
STATIC uint8 VttEep_EcuState = CANOEAPI_ECUSTATE_INITIAL;

/**********************************************************************************************************************
*  GLOBAL DATA
*********************************************************************************************************************/

/*! \brief Base address of the non-volatile RAM (NvRAM) file that stores the EEPROM data. 
*          Technically, the NvRAM file is memory-mapped for fast random-access. */
extern MemIf_AddressType VttEep_NvRamAddress;

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

extern void VttEep_Init(void);
STATIC void VttEep_Deinit(void);

extern void VttEep_OnReset(void);

STATIC void VttEep_OnSwitchOn(void);
STATIC void VttEep_OnSwitchOff(void);
STATIC void VttEep_OnStopMeasurement(void);
STATIC void VttEep_OnUnload(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttEep_Deinit()
**********************************************************************************************************************/
/*! \brief     De-initializes the NvRAM file used to simulate the EEPROM.
*  \context    Called by VttCan_OnSwitchOff and VttCan_OnStopMeasurement.
**********************************************************************************************************************/
STATIC void VttEep_Deinit(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEep_NvRamAddress = 0;
  CANoeAPI_CloseNvRam();

} /* VttEep_Deinit() */

/***********************************************************************************************************************
*  VttEep_OnSwitchOn()
**********************************************************************************************************************/
/*! \brief     OnSwitchOn callout of virtual EEPROM controller.
*
*         Tasks:
*            - Sets the base address of the NvRAM file to zero.
*            - Initializes the virtual EEPROM controller.
*
*  \context    Called by VttEep_OnStateChange handler when the virtual ECU is powered on.
**********************************************************************************************************************/
STATIC void VttEep_OnSwitchOn(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEep_NvRamAddress = 0;
  VttEep_Init();

} /* VttEep_OnSwitchOn() */

/***********************************************************************************************************************
*  VttEep_OnSwitchOff()
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual EEPROM controller.
*
*         Tasks:
*            - De-initializes the virtual EEPROM controller.
*
*  \context    Called by VttEep_OnStateChange handler right before the virtual ECU is powered off.
**********************************************************************************************************************/
STATIC void VttEep_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEep_Deinit();

} /* VttEep_OnSwitchOff() */

/***********************************************************************************************************************
*  VttEep_OnStopMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual EEPROM controller.
*
*         Tasks:
*            - De-initializes the virtual EEPROM controller.
*
*  \context    Called by VttEep_OnStateChange handler right simulation of the virtual ECU is stopped.
**********************************************************************************************************************/
STATIC void VttEep_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEep_Deinit();

} /* VttEep_OnStopMeasurement() */

/***********************************************************************************************************************
*  VttEep_OnUnload()
**********************************************************************************************************************/
/*! \brief     OnUnload callout of virtual EEPROM controller.
*
*         Tasks:
*            - De-initializes the virtual EEPROM controller.
*
*  \context    Called by VttEep_OnStateChange handler when the DLL of the ECU is unloaded in CANoe.
**********************************************************************************************************************/
STATIC void VttEep_OnUnload(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEep_Deinit();

} /* VttEep_OnUnload() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttEep_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual EEPROM controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttEep_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttEep_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
      break;
  case CANOEAPI_ECUACTION_SWITCHON:
    {
      VttEep_OnSwitchOn();
    }
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttEep_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttEep_OnSwitchOff();
    }
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttEep_OnStopMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
    {
      VttEep_OnUnload();
    }
    break;
  }
} /* VttEep_OnStateChange() */

/**********************************************************************************************************************
*  VttEep_Read()
**********************************************************************************************************************/
/*! \brief     Reads a byte from the EERPOM.
*  \param[in]  address        Memory address of the EEPROM to be read from.
*  \return     0 if the initialization of the EEPROM failed, otherwise the byte value stored at given memory address.
*  \context    Called by Eep driver.
*********************************************************************************************************************/
uint8 VttEep_Read(MemIf_AddressType address)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttEep_NvRamAddress == 0)
  {
    return 0;
  }

  /* Compute the effective memory address and read the byte value from the EEPROM. */
  return *((uint8*)(VTTEEP_FLASH_OFFSET + VttEep_NvRamAddress + address));

} /* VttEep_Read() */

/**********************************************************************************************************************
*  VttEep_Write()
**********************************************************************************************************************/
/*! \brief     Writes a byte to the EERPOM.
*
*  \param[in]  address        Memory address of the EEPROM to be written to.
*  \param[in]  value        Value (byte) that shall be stored at the given memory address of EEPROM.
*
*  \context    Called by Eep driver.
*  \note      If the initialization of the EEPROM failed, the write action is skipped.
*********************************************************************************************************************/
void VttEep_Write(MemIf_AddressType address, uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
    if (VttEep_NvRamAddress == 0)
    {
        return;
    }

  /* Compute the effective memory address and write given byte value to the EEPROM. */
    *((uint8*)(VTTEEP_FLASH_OFFSET + VttEep_NvRamAddress + address)) = value;

} /* VttEep_Write() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Eep.c
*********************************************************************************************************************/
