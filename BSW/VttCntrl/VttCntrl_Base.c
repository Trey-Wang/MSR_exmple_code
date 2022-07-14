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
/**        \file  VTTCntrl_Base.c
 *        \brief  Implements the public API of the vVIRTUALtarget base functionalities.
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

#define VTTCNTRL_BASE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/ 

#include "VttCntrl_Base.h"
#include "VttCntrl_Cfg.h"
#include "CANoeAPI.h" 

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of vVIRTUALtarget is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Base header file. */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Base.c and VttCntrl_Base.h are inconsistent"
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

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

STATIC void VttCntrl_Base_MCALInitMemory(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
*  VttCntrl_Base_MCALInitMemory()
**********************************************************************************************************************/
/*! \brief     Initializes the memory (e.g. variables) used by the vVIRTUALtarget COM/MCAL modules.
*
*  \context    Called on start of measurement and before an ECU is reset.
**********************************************************************************************************************/
STATIC void VttCntrl_Base_MCALInitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#if (STD_ON == VTTCNTRL_MODULE_VTTADC_EXISTS)
  Adc_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTCAN_EXISTS)
  Can_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTFR_EXISTS)
  Fr_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTLIN_EXISTS)
  Lin_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTETH_EXISTS)
  Eth_30_Vtt_InitMemory();
  EthTrcv_30_Vtt_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTMCU_EXISTS)
  Mcu_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTDIO_EXISTS)
  Dio_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTWDG_EXISTS)
  Wdg_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTFLS_EXISTS)
  Fls_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTGPT_EXISTS)
  Gpt_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTPWM_EXISTS)
  Pwm_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTEEP_EXISTS)
  Eep_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTICU_EXISTS)
  Icu_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTSPI_EXISTS)
  Spi_InitMemory();
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTPORT_EXISTS)
  Port_InitMemory();
#endif

} /* VttCntrl_Base_MCALInitMemory() */

/***********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/* \brief Custom callout a user can implement to invoke actions during ECU state changes. */
#if defined(VTTCNTRL_USERCALLOUT_ONSTATECHANGE)
#if (STD_ON == VTTCNTRL_USERCALLOUT_ONSTATECHANGE)
extern void VttCntrl_Base_UserCallout_OnStateChange(uint8 action, uint8 oldState, uint8 newState);
#endif
#endif

/***********************************************************************************************************************
*  VttCntrl_Base_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform vVIRTUALtarget about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttCntrl_Base_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)newState;
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  switch(action)
  {
    case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttCntrl_Base_MCALInitMemory();
    }
    break;
    case CANOEAPI_ECUACTION_LOAD:
    case CANOEAPI_ECUACTION_SWITCHON:
    case CANOEAPI_ECUACTION_RESET:
    {
      VttCntrl_Base_MCALInitMemory();
    }
    break;
    case CANOEAPI_ECUACTION_GOTOSLEEP:
    case CANOEAPI_ECUACTION_WAKEUP:
    case CANOEAPI_ECUACTION_SWITCHOFF:
    case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    case CANOEAPI_ECUACTION_UNLOAD:
      break;
  }

  /* Call the user-defined callout if configured. */
#if defined(VTTCNTRL_USERCALLOUT_ONSTATECHANGE)
#if (STD_ON == VTTCNTRL_USERCALLOUT_ONSTATECHANGE)
  VttCntrl_Base_UserCallout_OnStateChange(action, oldState, newState);
#endif
#endif
  
} /* VttCntrl_Base_OnStateChange() */

/***********************************************************************************************************************
 *  VttCntrl_Base_SetCheckedInterrupt()
 **********************************************************************************************************************/
/*! \brief      Set interrupt flag in the virtual interrupt controller. 
 *  \details    Only if the OS is present, the interrupt request is delegated to the virtual interrupt controller.
 *
 *  \param[in]  irq        Interrupt request number of the to be triggered interrupt.
 *
 *  \context    Called for e.g. by module XCP.
 **********************************************************************************************************************/
void VttCntrl_Base_SetCheckedInterrupt(sint32 irq)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* Forward the interrupt request if and only if the OS is present. */
#if (VTTCNTRL_MODULE_VTTOS_EXISTS == STD_ON)
#ifdef OS_HAL_OS_H
  int sourceId;
  if(irq < IRQ_COUNT)
  {
    sourceId = irqMap[irq];
    if(sourceId == IRQ_UNHANDLED)
    {
      CANoeAPI_Printf("Fatal Error: VttCntrl_Base_SetCheckedInterrupt(): Unhandled IRQ (stop)");
      CANoeEmuProcessor_PowerOff();
    }
    
    CANoeAPI_SetInterrupt(sourceId);
  }
#else
  CANoeAPI_SetInterrupt(irq);
#endif
#endif
} /* End of VttCntrl_Base_SetCheckedInterrupt() */

/***********************************************************************************************************************
*  VttCntrl_Base_MaskInterrupt()
**********************************************************************************************************************/
/*! \brief     Set interrupt mask flag in the virtual interrupt controller.
*  \details    
*
*  \param[in]  irq        IRQ number of the interrupt to be masked
*
*  \context    Called for e.g. by module CAN.
**********************************************************************************************************************/
void VttCntrl_Base_MaskInterrupt(sint32 irq)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* Forward the interrupt request if and only if the OS is present. */
#if (VTTCNTRL_MODULE_VTTOS_EXISTS == STD_ON)
#ifdef OS_HAL_OS_H
  int sourceId;
  if (irq < IRQ_COUNT)
  {
    sourceId = irqMap[irq];
    if (sourceId != IRQ_UNHANDLED)
    {
      CANoeEmuProcessor_MaskInterrupt(sourceId);
    }
  }
#else
  CANoeEmuProcessor_MaskInterrupt(irq);
#endif
#endif
} /* End of VttCntrl_Base_MaskInterrupt() */

/***********************************************************************************************************************
*  VttCntrl_Base_UnmaskInterrupt()
**********************************************************************************************************************/
/*! \brief     Clear interrupt mask flag in the virtual interrupt controller.
*  \details    
*
*  \param[in]  irq        IRQ number of the interrupt to be unmasked
*
*  \context    Called for e.g. by module CAN.
**********************************************************************************************************************/
void VttCntrl_Base_UnmaskInterrupt(sint32 irq)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* Forward the interrupt request if and only if the OS is present. */
#if (VTTCNTRL_MODULE_VTTOS_EXISTS == STD_ON)
#ifdef OS_HAL_OS_H
  int sourceId;
  if (irq < IRQ_COUNT)
  {
    sourceId = irqMap[irq];
    if (sourceId != IRQ_UNHANDLED)
    {
      CANoeEmuProcessor_UnmaskInterrupt(sourceId);
    }
  }
#else
  CANoeEmuProcessor_UnmaskInterrupt(irq);
#endif
#endif
} /* End of VttCntrl_Base_UnmaskInterrupt() */

/***********************************************************************************************************************
 *  VttCntrl_Base_GetBaseAddressOfDll()
 **********************************************************************************************************************/
/*! \brief      Returns the base address where the SUT DLL is loaded into memory. 
 *  \details    Windows DLLs are typically relocated in  memory when they are loaded into a process. For instance, 
 *              XCP uses this function to adjust the addresses from the  A2L file to the relocation effect.
 *
 *  \context    Called for e.g. by module XCP.
 **********************************************************************************************************************/
uint32 VttCntrl_Base_GetBaseAddressOfDll(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return CANoeAPI_BaseAddressOfDLL();

} /* VttCntrl_Base_GetBaseAddressOfDll() */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Base.c
 *********************************************************************************************************************/
