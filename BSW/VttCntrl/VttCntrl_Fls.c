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
/**      \file    VTTCntrl_Fls.h
*        \brief   Implements the public API of the virtual Flash controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Flash controller that enables to read and write bytes to a simulated Flash device.
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

#define VTTFLS_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Fls.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the Flash controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of VttCntrl_Fls header file. */
#if (  (VTTFLS_SW_MAJOR_VERSION != (3u)) \
    || (VTTFLS_SW_MINOR_VERSION != (0u)) \
    || (VTTFLS_SW_PATCH_VERSION != (6u)) )
# error "Vendor specific version numbers of VttCntrl_Fls.c and VttCntrl_Fls.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTFLS_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTFLS_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTFLS_CFG_PATCH_VERSION != (0x02u)))
  #error "Version numbers of VttCntrl_Fls.c and VttCntrl_Fls_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
#define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttFls_EcuState = CANOEAPI_ECUSTATE_INITIAL;

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/*! \brief Base address of the non-volatile RAM (NvRAM) file that stores the Flash data.
*          Technically, the NvRAM file is memory-mapped for fast random-access. */
extern MemIf_AddressType VttFls_NvRamAddress;

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

extern void VttFls_Init(void);
extern void VttFls_Deinit(void);
extern void VttFls_OnReset(void);

STATIC void VttFls_OnSwitchOn(void);
STATIC void VttFls_OnSwitchOff(void);
STATIC void VttFls_OnStopMeasurement(void);
STATIC void VttFls_OnUnload(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttFls_OnSwitchOn()
**********************************************************************************************************************/
/*! \brief     OnSwitchOn callout of virtual Flash controller.
*
*         Tasks:
*            - Sets the base address of the NvRAM file to zero.
*            - Initializes the virtual Flash controller.
*
*  \context    Called by VttFls_OnStateChange handler when the virtual ECU is powered on.
**********************************************************************************************************************/
STATIC void VttFls_OnSwitchOn(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFls_NvRamAddress = 0;
  VttFls_Init();

} /* VttFls_OnSwitchOn() */

/***********************************************************************************************************************
*  VttFls_OnSwitchOff()
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual Flash controller.
*
*         Tasks:
*            - De-initializes the virtual Flash controller.
*
*  \context    Called by VttFls_OnStateChange handler right before the virtual ECU is powered off.
**********************************************************************************************************************/
STATIC void VttFls_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFls_Deinit();

} /* VttFls_OnSwitchOff() */

/***********************************************************************************************************************
*  VttFls_OnStopMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual Flash controller.
*
*         Tasks:
*            - De-initializes the virtual Flash controller.
*
*  \context    Called by VttFls_OnStateChange handler right simulation of the virtual ECU is stopped.
**********************************************************************************************************************/
STATIC void VttFls_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFls_Deinit();

} /* VttFls_OnStopMeasurement() */

/***********************************************************************************************************************
*  VttFls_OnUnload()
**********************************************************************************************************************/
/*! \brief     OnUnload callout of virtual Flash controller.
*
*         Tasks:
*            - De-initializes the virtual Flash controller.
*
*  \context    Called by VttFls_OnStateChange when the DLL of the ECU is unloaded in CANoe.
**********************************************************************************************************************/
STATIC void VttFls_OnUnload(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFls_Deinit();

} /* VttFls_OnUnload() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttFls_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual Flash controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttFls_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttFls_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    {
      VttFls_OnSwitchOn();
    }    
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttFls_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttFls_OnSwitchOff();
    }    
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttFls_OnStopMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
    {
      VttFls_OnUnload();
    }
    break;
  }

} /* VttFls_OnStateChange() */

/**********************************************************************************************************************
*  VttFls_Read()
**********************************************************************************************************************/
/*! \brief     Reads a byte from the Flash.
*  \param[in]  address        Memory address of the Flash to be read from.
*  \return     0 if the initialization of the Flash failed, otherwise the byte value stored at given memory address.
*  \context    Called by Fls driver.
*********************************************************************************************************************/
uint8 VttFls_Read(MemIf_AddressType address)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttFls_NvRamAddress == 0)
  {
    return 0;
  }

  /* Compute the effective memory address and read the byte value from the Flash. */
  return *((uint8*)(VttFls_NvRamAddress + address));

} /* VttFls_Read() */

/**********************************************************************************************************************
*  VttFls_Write()
**********************************************************************************************************************/
/*! \brief     Writes a byte to the Flash.
*
*  \param[in]  address      Memory address of the Flash to be written to.
*  \param[in]  value        Value (byte) that shall be stored at the given memory address of Flash.
*
*  \context    Called by Fls driver.
*  \note      If the initialization of the Flash failed, the write action is skipped.
*********************************************************************************************************************/
void VttFls_Write(MemIf_AddressType address, uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
    if (VttFls_NvRamAddress == 0)
    {
        return;
    }

  /* Compute the effective memory address and write given byte value to the Flash. */
    *((uint8*)(VttFls_NvRamAddress + address)) = value;

} /* VttFls_Write() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Fls.c
*********************************************************************************************************************/
