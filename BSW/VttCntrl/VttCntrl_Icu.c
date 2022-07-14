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
/**        \file  VTTCntrl_Icu.c
 *        \brief  Implements the public API of the virtual ICU controller integrated in the vVIRTUALtarget.
 *
 *     \details   Virtual ICU controller that enables edge detection and time measurement of signal changes.
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

# define VTTICU_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Icu.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the ICU controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Icu header file. */
#if (  (VTTICU_SW_MAJOR_VERSION != (0x03u)) \
    || (VTTICU_SW_MINOR_VERSION != (0x00u)) \
    || (VTTICU_SW_PATCH_VERSION != (0x06u)) )
# error "Vendor specific version numbers of VttCntrl_Icu.c and VttCntrl_Icu.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VTTICU_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTICU_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTICU_CFG_PATCH_VERSION != (0x01u)))
  #error "Version numbers of VttCntrl_Icu.c and VttCntrl_Icu_Cfg.h are inconsistent!"
#endif

/* Check the version of VttCntrl_Base header file.  */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Icu.c and VttCntrl_Base.h are inconsistent"
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
STATIC uint8 VttIcu_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal ICU controller state of each channel. */
extern VttIcu_CntrlType VttIcu_Cntrl[VTTICU_CHANNELS];

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC uint8 VttIcu_FindChannel(uint8 channelId);
STATIC unsigned char VttIcu_CheckTriggeredEdge(sint32 channelId, Icu_ActivationType *detectedEdge);
STATIC unsigned char VttIcu_EdgeDetect(sint32 channelId, sint32 value, Icu_ActivationType *detectedEdge);
STATIC void VttIcu_SysVarHandler(sint32 channelId, sint32 value);

STATIC void VttIcu_OnLoad(void);
STATIC void VttIcu_OnStartMeasurement(void);
STATIC void VttIcu_OnReset(void);

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttIcu_FindChannel
**********************************************************************************************************************/
/*! \brief Checks if a ICU channel exists.
*  \param[in]   channelId     ICU channel number.
*  \return      ICU channel number if the channel is in use, otherwise VTTICU_CHANNELS + 1.
**********************************************************************************************************************/
STATIC uint8 VttIcu_FindChannel(uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint32 isrNumber;
  uint8 i;
  
  /* ----- Implementation ----------------------------------------------- */
  isrNumber = channelId + IRQ_IcuIsr_0;

  for (i = 0; i < VTTICU_CHANNELS; ++i)
  {
    if (VttIcu_Cntrl[i].ChIsrNr == isrNumber)
    {
      return i;
    }
  }

  return VTTICU_CHANNELS + 1;
  
} /* VttIcu_FindChannel() */

/***********************************************************************************************************************
*  VttIcu_CheckTriggeredEdge
**********************************************************************************************************************/
/*! \brief      Checks if the current channel is triggered by the current edge.
*  \param[in]   channelId     ICU channel number.
*  \context     Called by function VttIcu_SysVarHandler.
**********************************************************************************************************************/
STATIC unsigned char VttIcu_CheckTriggeredEdge(sint32 channelId, Icu_ActivationType *detectedEdge)
{
  /* ----- Local Variables ---------------------------------------------- */
  Icu_ActivationType edgeToCheck;
  
  /* ----- Implementation ----------------------------------------------- */  
  switch( VttIcu_Cntrl[channelId].ChState )
  {
  case VttIcu_State_Measurement_First:
    {
      edgeToCheck = VttIcu_Cntrl[channelId].ChStartEdge;
      VttIcu_Cntrl[channelId].ChState = VttIcu_State_Measurement_Running;
    }
    break;    
    case VttIcu_State_Edge_x:
    case VttIcu_State_Measurement_Running:
    {
      edgeToCheck = VttIcu_Cntrl[channelId].ChTriggerEdge;
    }
    break;
    default:
    {
      edgeToCheck = *detectedEdge;
    }
    break;
  }
  
  if( ICU_BOTH_EDGES == edgeToCheck )
  {
    return VTTICU_TRUE;
  }
  else if( *detectedEdge == edgeToCheck )
  {
    return VTTICU_TRUE;
  }
  else
  {
    return VTTICU_FALSE;
  }
  
} /* VttIcu_CheckTriggeredEdge() */

/***********************************************************************************************************************
*  VttIcu_EdgeDetect
**********************************************************************************************************************/
/*! \brief      Checks if the current SysVar change is a valid edge.
*  \param[in]   channelId   ICU channel number.
*   \param[in]  value       Edge threshold.
*  \context     Called by function VttIcu_SysVarHandler.
**********************************************************************************************************************/
STATIC unsigned char VttIcu_EdgeDetect(sint32 channelId, sint32 value, Icu_ActivationType *detectedEdge)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if( (0 == value) && (0 < VttIcu_Cntrl[channelId].ChLastValue) )
  {
    /* Falling edge */
    *detectedEdge = ICU_FALLING_EDGE;
    return VTTICU_VALID_EDGE;
  }
  else if( (0 < value) && (0 == VttIcu_Cntrl[channelId].ChLastValue) )
  {
    /* Rising edge */
    *detectedEdge = ICU_RISING_EDGE;
    return VTTICU_VALID_EDGE;
  }
  else
  {
    /* No valid edge */
    return VTTICU_INVALID_EDGE;
  }
  
} /* VttIcu_EdgeDetect() */

/***********************************************************************************************************************
*  VttIcu_SysVarHandler
**********************************************************************************************************************/
/*! \brief      Requests a interrupt if a valid edge was detected for the given channel.
*  \param[in]   channelId  ICU channel number.
*  \param[in]   value      Current value of ICU channel.
*  \context    Called by CANoeEmu when a ICU channel's value changes.
**********************************************************************************************************************/
STATIC void VttIcu_SysVarHandler(sint32 channelId, sint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  Icu_ActivationType detectedEdge;

  /* ----- Implementation ----------------------------------------------- */
  if( VTTICU_INVALID_EDGE == VttIcu_EdgeDetect( channelId, value, &detectedEdge ) )
  {
    /* SysVar-Change which does not correspond to a regular edge (x->0, 0->x) */
    VttIcu_Cntrl[channelId].ChLastValue = value;
    return;
  }
  
  VttIcu_Cntrl[channelId].ChLastValue = value;

  if( VTTICU_FALSE == VttIcu_CheckTriggeredEdge(channelId, &detectedEdge) )
  {
    /* Channel does not trigger on the current edge */
    return;
  }

  if( VTTICU_FALSE == VttIcu_Cntrl[channelId].ChIsActive )
  {
    /* Channel is not measured */
    return;
  }

  switch (VttIcu_Cntrl[channelId].ChState)
  {
  case VttIcu_State_Edge_x:
    break;
    case VttIcu_State_Measurement_First:
    case VttIcu_State_Measurement_Running:
    {
      VttIcu_Cntrl[channelId].ChTimerValue = CANoeAPI_UserTimer_CurrentTime();
    }
    break;
    default:
      return;
  }

  if( CANOEAPI_ECUSTATE_PROCESSING == VttIcu_EcuState )
  {
    VttCntrl_Base_SetCheckedInterrupt(VttIcu_Cntrl[channelId].ChIsrNr);
  }
  else if( CANOEAPI_ECUSTATE_SLEEPING == VttIcu_EcuState )
  {
    if( VTTICU_TRUE == VttIcu_Cntrl[channelId].ChWakeupCapability )
    {
      VttCntrl_Base_SetCheckedInterrupt(VttIcu_Cntrl[channelId].ChIsrNr);
      CANoeAPI_WakeupProcessor();
    }
  }
  
} /* VttIcu_SysVarHandler() */

/***********************************************************************************************************************
*  VttIcu_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual ICU controller.
*  \context    Called by VttIcu_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttIcu_OnLoad(void)
{
    /* ----- Local Variables ---------------------------------------------- */
  int channelId;
  
  /* ----- Implementation ----------------------------------------------- */
  for(channelId = 0; channelId < VTTICU_CHANNELS; channelId++)
  {
    VttIcu_Cntrl[channelId].ChIsrNr = IRQ_IcuIsr_0 + VttIcu_Cntrl[channelId].ChIsrNr;
    VttIcu_Cntrl[channelId].ChSysVarHandle = CANoeAPI_SysVar_DeclareInt ("Icu", VttIcu_Cntrl[channelId].ChName, 0);
    CANoeAPI_SysVar_SetHandlerInt(VttIcu_Cntrl[channelId].ChSysVarHandle, VttIcu_SysVarHandler, channelId);
    CANoeAPI_SysVar_SetInt(VttIcu_Cntrl[channelId].ChSysVarHandle, 0);
  }
  
} /* VttIcu_OnLoad() */

/***********************************************************************************************************************
*  VttIcu_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual ICU controller.
*  \context    Called by VttIcu_OnStateChange handler right before simulation is started.
**********************************************************************************************************************/
STATIC void VttIcu_OnStartMeasurement(void)
{
    /* ----- Local Variables ---------------------------------------------- */
  int channelId;
  
  /* ----- Implementation ----------------------------------------------- */
  for(channelId = 0; channelId < VTTICU_CHANNELS; channelId++)
  {
    VttIcu_Cntrl[channelId].ChLastValue = CANoeAPI_SysVar_GetInt(VttIcu_Cntrl[channelId].ChSysVarHandle);
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_FALSE;
  }
  
} /* VttIcu_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttIcu_OnReset
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual ICU controller.
*  \context    Called by VttIcu_OnStateChange handler right before ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttIcu_OnReset(void)
{
    /* ----- Local Variables ---------------------------------------------- */
  int channelId;
  
  /* ----- Implementation ----------------------------------------------- */
  for(channelId = 0; channelId < VTTICU_CHANNELS; channelId++)
  {
    VttIcu_Cntrl[channelId].ChLastValue = CANoeAPI_SysVar_GetInt(VttIcu_Cntrl[channelId].ChSysVarHandle);
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_FALSE;
  }
  
} /* VttIcu_OnReset() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttIcu_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual ICU controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttIcu_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;
  
  /* ----- Implementation ----------------------------------------------- */
  VttIcu_EcuState = newState;  

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttIcu_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttIcu_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttIcu_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }
  
} /* VttIcu_OnStateChange() */

/***********************************************************************************************************************
*  VttIcu_StartEdgeMonitoring
**********************************************************************************************************************/
/*! \brief      Activates edge monitoring for the given channel and edge.
*  \param[in]   channelId    ICU channel number.
*  \context     Called by ICU driver.
**********************************************************************************************************************/
void VttIcu_StartEdgeMonitoring(uint8 channelId, Icu_ActivationType triggerEdge)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  channelId = VttIcu_FindChannel(channelId);
  
  if(channelId >= VTTICU_CHANNELS)
  {
    return;
  }

  if(VttIcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_TRUE;
    VttIcu_Cntrl[channelId].ChTriggerEdge = triggerEdge;
    VttIcu_Cntrl[channelId].ChStartEdge = triggerEdge;
    VttIcu_Cntrl[channelId].ChState = VttIcu_State_Edge_x;
  }
  
} /* VttIcu_StartEdgeMonitoring() */

/***********************************************************************************************************************
*  VttIcu_StopEdgeMonitoring
**********************************************************************************************************************/
/*! \brief      Deactivates edge monitoring for the given channel- and edge.
*  \param[in]   channelId    ICU channel number.
*  \context     Called by ICU driver.
**********************************************************************************************************************/
void VttIcu_StopEdgeMonitoring (uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  channelId = VttIcu_FindChannel(channelId);
  
  if( channelId >= VTTICU_CHANNELS )
  {
    return;
  }

  if(VttIcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_FALSE;
  }
  
} /* VttIcu_StopEdgeMonitoring() */

/***********************************************************************************************************************
*  VttIcu_StartTimeMeasurement
**********************************************************************************************************************/
/*! \brief      Activates time measurement for the given channel- and edge.
*  \param[in]   channelId     ICU channel number.
*  \context     Called by ICU driver.
**********************************************************************************************************************/
void VttIcu_StartTimeMeasurement(uint8 channelId, Icu_ActivationType startingEdge, Icu_ActivationType triggerEdge)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  channelId = VttIcu_FindChannel(channelId);
  
  if(channelId >= VTTICU_CHANNELS)
  {
    return;
  }

  if(VttIcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_TRUE;
    VttIcu_Cntrl[channelId].ChStartEdge = startingEdge;
    VttIcu_Cntrl[channelId].ChTriggerEdge = triggerEdge;
    VttIcu_Cntrl[channelId].ChState = VttIcu_State_Measurement_First;
  }
  
} /* VttIcu_StartTimeMeasurement() */

/***********************************************************************************************************************
*  VttIcu_StopTimeMeasurement
**********************************************************************************************************************/
/*! \brief      Deactivates time measurement for the given channel- and edge.
*  \param[in]   channelId     ICU channel number.
*  \context     Called by ICU driver.
**********************************************************************************************************************/
void VttIcu_StopTimeMeasurement(uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  channelId = VttIcu_FindChannel(channelId);
  
  if(channelId >= VTTICU_CHANNELS)
  {
    return;
  }

  if(VttIcu_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    VttIcu_Cntrl[channelId].ChIsActive = VTTICU_FALSE;
  }
  
} /* VttIcu_StopTimeMeasurement() */

/*********************************************************************************************************************
*  VttIcu_GetTimerValue
**********************************************************************************************************************/
/*! \brief      Returns the current value of ICU's measurement timer.
*  \param[in]   channelId     ICU channel number.
*  \return      Timestamp in nanoseconds [ns].
*  \context    Called by ICU driver.
**********************************************************************************************************************/
unsigned long long VttIcu_GetTimerValue(uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  channelId = VttIcu_FindChannel(channelId);
  
  if(channelId >= VTTICU_CHANNELS)
  {
    return 0;
  }
  
  return VttIcu_Cntrl[channelId].ChTimerValue;
  
} /* VttIcu_GetTimerValue() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Icu.c
***********************************************************************************************************************/
