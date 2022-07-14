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
/**      \file    VTTCntrl_SysVar.c
*        \brief   Implements the public API of vVIRTUALtarget for handling user-defined CANoe system variables.
*
*      \details   Each configured CANoe system variable may trigger an notification function if value changes. 
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

#define VTTSYSVAR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
 
#include "VttCntrl_SysVar.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_SysVar header file. */
#if (  (VTTCNTRL_SYSVAR_MAJOR_VERSION != (3u)) \
    || (VTTCNTRL_SYSVAR_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SYSVAR_PATCH_VERSION != (6u)) )
  #error "Vendor specific version numbers of VttCntrl_SysVar.c and VttCntrl_SysVar.h are inconsistent"
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

/*! \brief Configuration of each user-defined CANoe system variable. */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0
extern VttSysVar_CntrlType VttSysVar_Channel_Cfg[VTTSYSVAR_CHANNELS];
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/*! \brief Number of channels associated with user-defined CANoe system variables that have pending changes. */
uint64 VttSysVar_NumPendingChanges;

/*! \brief List of IDs associated with user-defined CANoe system variables that have pending changes. */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0
uint64 VttSysVar_PendingChanges[VTTSYSVAR_CHANNELS];
#endif
 
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

extern void VttSysVar_OnLoad(void);
STATIC void VttSysVar_OnStartMeasurement(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/ 

/***********************************************************************************************************************
*  VttSysVar_OnStartMeasurement()
**********************************************************************************************************************/
/*! \brief     Function to set an interrupt in with check, whether the interrupt will be handled
*  \details    This function wraps CANoeAPI_SetInterrupt(). Before the call to CANoeAPI_SetInterrupt() is made,
*              this function checks the ISR table to see whether an ISR is configured for this interrupt.
*              Is no interrupt is configured, an information text is printed to the CANoe write window.
**********************************************************************************************************************/
STATIC void VttSysVar_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0
  uint64 i;
#endif

  /* ----- Implementation ----------------------------------------------- */
  
  /* Clear all pending changes of user-defined CANoe system variables. */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0
  for (i = 0; i < VTTSYSVAR_CHANNELS; ++i)
  {
    VttSysVar_PendingChanges[i] = 0;
  }
#endif
  VttSysVar_NumPendingChanges = 0;

} /* VttSysVar_OnStartMeasurement() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttCntrl_SysVar_OnStateChange()
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
void VttCntrl_SysVar_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;
  (void)newState;

  /* ----- Implementation ----------------------------------------------- */
  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttSysVar_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttSysVar_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
  case CANOEAPI_ECUACTION_RESET:
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

} /* VttCntrl_SysVar_OnStateChange() */

/***********************************************************************************************************************
*  VttSysVarIsr_0
**********************************************************************************************************************/
/*! \brief     Interrupt service routine triggered when a user-defined CANoe system variable changed.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
void VttSysVarIsr_0(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0
  uint64 pendingChanges[VTTSYSVAR_CHANNELS];
  uint64 numPendingChanges;
  uint64 i;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if defined(VTTSYSVAR_CHANNELS) && VTTSYSVAR_CHANNELS > 0

/* Create a local copy of the pending user-defined CANoe system variable changes. */
  CANoeAPI_AtomicBegin();
  
  numPendingChanges = VttSysVar_NumPendingChanges;
  VttSysVar_NumPendingChanges = 0;
  
  for (i = 0; i < numPendingChanges; ++i)
  {
    pendingChanges[i] = VttSysVar_PendingChanges[i];
    VttSysVar_PendingChanges[i] = 0;
    VttSysVar_Channel_Cfg[pendingChanges[i]].hasChange = 0;
  }
  
  CANoeAPI_AtomicEnd();

  /* Call configured notification function of each user-defined CANoe system variable that has changed. */
  for (i = 0; i < numPendingChanges; ++i) {
    VttSysVar_Channel_Cfg[pendingChanges[i]].notification();
  }
#endif

} /* VttSysVarIsr_0() */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_SysVar.c
 *********************************************************************************************************************/
