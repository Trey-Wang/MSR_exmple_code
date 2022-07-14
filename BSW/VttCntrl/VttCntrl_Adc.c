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
/**      \file    VTTCntrl_Adc.c
*        \brief   Implements the public API of the virtual ADC controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual ADC controller that enables to read/write (group-wise) to ADC ports (channels).
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

#define VTTADC_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Adc.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the ADC controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Adc header file. */
#if (  (VTTADC_SW_MAJOR_VERSION != (0x03u)) \
    || (VTTADC_SW_MINOR_VERSION != (0x01u)) \
    || (VTTADC_SW_PATCH_VERSION != (0x01u)) )
# error "Vendor specific version numbers of VttCntrl_Adc.c and VttCntrl_Adc.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTADC_CFG_MAJOR_VERSION != (0x14u)) \
    || (VTTADC_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTADC_CFG_PATCH_VERSION != (0x01u)))
  #error "Version numbers of VttCntrl_Adc.c and VttCntrl_Adc_Cfg.h are inconsistent!"
#endif

/* Check the version of VttCntrl_Base header file.  */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Adc.c and VttCntrl_Base.h are inconsistent"
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
STATIC uint8 VttAdc_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal ADC controller state of each channel. */
extern VttAdc_CntrlType VttAdc_Cntrl[VTTADC_GROUPS];
extern VttAdc_ChannelType *VttAdc_AllChannels[VTTADC_CHANNELS];

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttAdc_TimerVarHandler(uint8 index, const char* name);
STATIC void VttAdc_SysVarHWTriggerHandler(sint32 param, sint32 value);

STATIC uint32 VttAdc_GetChannelTimeout(Adc_GroupType group, uint8 channel);
STATIC uint8 VttAdc_IsGroupMeasurementFinished(Adc_GroupType group);

STATIC void VttAdc_CopyRegisterValuesToBuffer(Adc_GroupType group);
STATIC Adc_GroupType VttAdc_GetGroup(uint8 index);

STATIC void VttAdc_OnLoad(void);
STATIC void VttAdc_OnStartMeasurement(void);
STATIC void VttAdc_OnReset(void);

STATIC void VttAdc_MapChannel(void);
STATIC void VttAdc_AssignIRQ(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttAdc_GetChannelTimeout
**********************************************************************************************************************/
/*!  Return the conversion sample time of a ADC channel.
*   \param[in]  group     ADC group.
*   \param[in]  channel   ADC channel that is element of the ADC group.
*   \returns  Conversion sample time.
**********************************************************************************************************************/
STATIC uint32 VttAdc_GetChannelTimeout(Adc_GroupType group, uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
    return VttAdc_Cntrl[group].ChannelArray[channel]->ConversionSampleTime;
  
} /* VttAdc_GetChannelTimeout() */

/***********************************************************************************************************************
*  VttAdc_IsGroupMeasurementFinished
**********************************************************************************************************************/
/*! \brief  Checks whether all channels of one group have been processed.
*   \param[in]  group     ADC group.
*  \return VTTADC_TRUE if yes, otherwise VTTADC_FALSE.
**********************************************************************************************************************/
STATIC uint8 VttAdc_IsGroupMeasurementFinished(Adc_GroupType group)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttAdc_Cntrl[group].CurrentProcessingChannel == ((VttAdc_Cntrl[group].NumberOfChannels)-1))
  {
    return VTTADC_TRUE;
  }
  else
  {
    return VTTADC_FALSE;
  }
  
} /* VttAdc_IsGroupMeasurementFinished() */


/***********************************************************************************************************************
*  VttAdc_CopyRegisterValuesToBuffer
**********************************************************************************************************************/
/*! \brief  Copies the internal ADC register values into the local ADC buffer.
*   \param[in]  group     ADC group.
**********************************************************************************************************************/
STATIC void VttAdc_CopyRegisterValuesToBuffer(Adc_GroupType group)
{
    /* ----- Local Variables ---------------------------------------------- */
  int i;
  
  /* ----- Implementation ----------------------------------------------- */
  for(i = 0; i < VttAdc_Cntrl[group].NumberOfChannels; i++)
  {
    VttAdc_ChannelType *Ch = VttAdc_Cntrl[group].ChannelArray[i];
    Ch->RegisterValue = (Adc_ValueType)((((((Adc_ValueType)1) << Ch->Resolution) - 1) 
      * Ch->PhysicalValue) / Ch->VoltageHighReference);
    VttAdc_Cntrl[group].ChannelValueArray[i] = Ch->RegisterValue;
  }
  
} /* VttAdc_CopyRegisterValuesToBuffer */

/***********************************************************************************************************************
*  VttAdc_GetGroup
**********************************************************************************************************************/
/*! \brief     Returns the current ADC group belonging to the CANoeEmu user timer index.
*   \param[in] index     CANoeEmu user timer index.
**********************************************************************************************************************/
Adc_GroupType VttAdc_GetGroup(uint8 index)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  sint8 group = -1;
  
  /* ----- Implementation ----------------------------------------------- */
  /* Get current ADC group. */
  for(i = 0; i < VTTADC_GROUPS; i++)
  {
    if(index == VttAdc_Cntrl[i].CANoeTimerHandle)
    {
      group = i;
    }
  }
  
  return group;
  
} /* VttAdc_GetGroup() */

/***********************************************************************************************************************
*  VttAdc_TimerVarHandler
**********************************************************************************************************************/
/*! \brief     Timer handler for all ADC channel groups.
*   \param[in] index     CANoeEmu user timer index.
*   \param[in] name      Name associated with the CANoeEmu user timer.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttAdc_TimerVarHandler(uint8 index, const char* name)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint8 CurrentGroup = -1;
  uint8 CurrentChannel;
  
  /* ----- Implementation ----------------------------------------------- */
  CurrentGroup = VttAdc_GetGroup(index);
  CurrentChannel = VttAdc_Cntrl[CurrentGroup].CurrentProcessingChannel;
  
  if(VttAdc_IsGroupMeasurementFinished(CurrentGroup))
  {
    VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue = 
      (double) CANoeAPI_SysVar_GetFloat( VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->CANoeSysVarHandle);
    
    /* Set the physical value to the max value in case it is too high. */
    if(VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue > 
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->VoltageHighReference)
    {
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue = 
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->VoltageHighReference;
    }

    VttAdc_CopyRegisterValuesToBuffer(CurrentGroup);
    
    if (VttAdc_Cntrl[CurrentGroup].CurrentMode == VttAdc_OpMode_OneShot)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttAdc_Cntrl[CurrentGroup].Isr_Nr);
    }
    else if( VttAdc_Cntrl[CurrentGroup].CurrentMode == VttAdc_OpMode_Continuous)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttAdc_Cntrl[CurrentGroup].Isr_Nr);
      
      /* In Op_Mode_Continuous start measurement from beginning */
      VttAdc_Cntrl[CurrentGroup].CurrentProcessingChannel = 0;
      CANoeAPI_UserTimer_SetRelative(VttAdc_Cntrl[CurrentGroup].CANoeTimerHandle, VttAdc_GetChannelTimeout(CurrentGroup, 0));
    }
    else
    {
      CANoeAPI_WriteString("Group is running in an unknown mode");
    }
  }
  else
  {
    /* Store the value of processed channel */
    VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue = 
      (double) CANoeAPI_SysVar_GetFloat(VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->CANoeSysVarHandle);
    
    /* set the physical value to the max value in case it is too high */
    if(VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue > 
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->VoltageHighReference)
    {
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->PhysicalValue = 
      VttAdc_Cntrl[CurrentGroup].ChannelArray[CurrentChannel]->VoltageHighReference;
    }
    
    VttAdc_Cntrl[CurrentGroup].CurrentProcessingChannel++;
    CANoeAPI_UserTimer_SetRelative(VttAdc_Cntrl[CurrentGroup].CANoeTimerHandle, 
    VttAdc_GetChannelTimeout(CurrentGroup, VttAdc_Cntrl[CurrentGroup].CurrentProcessingChannel));
  }

} /* VttAdc_TimerVarHandler() */

/***********************************************************************************************************************
*  VttAdc_SysVarHWTriggerHandler
**********************************************************************************************************************/
/*! \brief     Handler dealing with CANoeEmu system variables changes that implements the ADC channels.
*   \param[in] handle   Handle of the CANoeEmu system variable that has pending changes.
*   \param[in] value    Current value of the CANoeEmu system variable.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttAdc_SysVarHWTriggerHandler(sint32 handle, sint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  
  /* ----- Implementation ----------------------------------------------- */
  for(i = 0; i < VTTADC_GROUPS; i++)
  {
    if((handle == VttAdc_Cntrl[i].SysVarHandle_Param) && (VttAdc_Cntrl[i].CurrentMode == VttAdc_OpMode_ToBeHWTriggered))
    {
      /* HW trigger has evoked to start measurement. */
      /* Start processing first channel of this group. */
      VttAdc_Cntrl[i].CurrentMode = VttAdc_OpMode_OneShot;
      CANoeAPI_UserTimer_SetRelative(VttAdc_Cntrl[i].CANoeTimerHandle, VttAdc_GetChannelTimeout(i, 0));
    }
  }
  
} /* VttAdc_SysVarHWTriggerHandler() */

/***********************************************************************************************************************
*  VttAdc_AssignIRQ
**********************************************************************************************************************/
/*! \brief     Initialization during the start of a measurement in CANoe.
*  \context    Function is called by State Machine of VTT.
*  \note       VttCntrl-Generator enters the HW Unit number in Isr_Nr. During start-up, the offset has to be added.
**********************************************************************************************************************/
STATIC void VttAdc_AssignIRQ(void)
{
    /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  
  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTADC_GROUPS; i++)
  {
    VttAdc_Cntrl[i].Isr_Nr = VTTADC_IRQ_OFFSET + VttAdc_Cntrl[i].Isr_Nr;
  }
  
} /* VttAdc_AssignIRQ() */

/***********************************************************************************************************************
*  VttAdc_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual ADC controller.
*  \context    Called by VttAdc_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttAdc_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  sint32 SysVarHWTriggerParam = 1;
  
  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTADC_CHANNELS; i++)
  {
    VttAdc_AllChannels[i]->CANoeSysVarHandle=CANoeAPI_SysVar_DeclareFloat("Adc", 
      VttAdc_AllChannels[i]->ChannelName, 0);
  }

  for (i = 0; i < VTTADC_GROUPS; i++)
  {
    /* Create System Variables for HW Trigger in case Group is configured as hw triggered */
    if (VttAdc_Cntrl[i].GroupHwTriggerEdge == VTTADC_TRIGG_SRC_HW)
    {
      VttAdc_Cntrl[i].SysVarHandle_HwTrigger = CANoeAPI_SysVar_DeclareInt("Adc",VttAdc_Cntrl[i].GroupName, 0);
      
      CANoeAPI_SysVar_SetHandlerInt(VttAdc_Cntrl[i].SysVarHandle_HwTrigger, 
        VttAdc_SysVarHWTriggerHandler, SysVarHWTriggerParam);
        
      VttAdc_Cntrl[i].SysVarHandle_Param = SysVarHWTriggerParam;
      
      SysVarHWTriggerParam++;
    }
  }
  
  VttAdc_AssignIRQ();

} /* VttAdc_OnLoad() */

/***********************************************************************************************************************
*  VttAdc_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual ADC controller.
*  \context    Called by VttAdc_OnStateChange handler right before simulation is started.
**********************************************************************************************************************/
STATIC void VttAdc_OnStartMeasurement(void)
{
    /* ----- Local Variables ---------------------------------------------- */
   int i;
   
  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTADC_GROUPS; i++)
  {
    /* Create CANoeEmu user timer for every ADC group. */
    VttAdc_Cntrl[i].CANoeTimerHandle = CANoeAPI_UserTimer_Create(VttAdc_Cntrl[i].GroupName);
    CANoeAPI_UserTimer_SetCallbackFunction(VttAdc_Cntrl[i].CANoeTimerHandle, VttAdc_TimerVarHandler);
  }
  
} /* VttAdc_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttAdc_OnReset
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual ADC controller.
*  \context    Called by VttAdc_OnStateChange handler right before ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttAdc_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  
} /* VttAdc_OnReset() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttAdc_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual ADC controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttAdc_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttAdc_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttAdc_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttAdc_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttAdc_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }
  
} /* VttAdc_OnStateChange() */

/***********************************************************************************************************************
*  VttAdc_StartOneShotImmediate
**********************************************************************************************************************/
/*! \brief     Starts immediately a single ADC group conversion.
*   \param[in]  group     ADC group.
*  \context   Called by ADC driver.
**********************************************************************************************************************/
void VttAdc_StartOneShotImmediate(Adc_GroupType group)  
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  
  /* One Shot Immediate: */
  /* Start processing first channel of this group. */
  VttAdc_Cntrl[group].CurrentProcessingChannel = 0;
  VttAdc_Cntrl[group].CurrentMode = VttAdc_OpMode_OneShot;
  CANoeAPI_UserTimer_SetRelative(VttAdc_Cntrl[group].CANoeTimerHandle, VttAdc_GetChannelTimeout(group, 0));

} /* VttAdc_StartOneShotImmediate() */

/***********************************************************************************************************************
*  VttAdc_StartOneShotHwTriggered
**********************************************************************************************************************/
/*! \brief     Starts a single ADC group conversion when HW trigger occurred.
*   \param[in]  group     ADC group.
*   \param[in]  edge    Hardware trigger event.
*  \context    Called by ADC driver.
**********************************************************************************************************************/
void VttAdc_StartOneShotHwTriggered(Adc_GroupType group, Adc_HwTriggerEventType edge)
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Implementation ----------------------------------------------- */
  
  /* Prepare group for OneShotHwTriggered. */
  VttAdc_Cntrl[group].GroupHwTriggerEdge = edge;
  VttAdc_Cntrl[group].CurrentMode = VttAdc_OpMode_ToBeHWTriggered;
  VttAdc_Cntrl[group].CurrentProcessingChannel = 0;
    
} /* VttAdc_StartOneShotHwTriggered() */

/***********************************************************************************************************************
*  VttAdc_StartContinuous
**********************************************************************************************************************/
/*! \brief     Starts continuous ADC group conversion.
*   \param[in]  group     ADC group.
*  \context     Called by ADC driver.
**********************************************************************************************************************/
void VttAdc_StartContinuous(Adc_GroupType group)
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Implementation ----------------------------------------------- */
  VttAdc_Cntrl[group].CurrentMode = VttAdc_OpMode_Continuous;
  VttAdc_Cntrl[group].CurrentProcessingChannel = 0;
  CANoeAPI_UserTimer_SetRelative(VttAdc_Cntrl[group].CANoeTimerHandle, VttAdc_GetChannelTimeout(group, 0));

} /* VttAdc_StartContinuous() */

/***********************************************************************************************************************
*  VttAdc_StopConversion
**********************************************************************************************************************/
/*! \brief     Stops a currently running ADC group conversion.
*   \param[in]  group     ADC group.
*  \context    Called by ADC driver.
**********************************************************************************************************************/
void VttAdc_StopConversion(Adc_GroupType group)
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Implementation ----------------------------------------------- */
  VttAdc_Cntrl[group].CurrentMode = VttAdc_OpMode_Stopped;
  CANoeAPI_UserTimer_Cancel(VttAdc_Cntrl[group].CANoeTimerHandle);
  VttAdc_CopyRegisterValuesToBuffer(group);
  
} /* VttAdc_StopConversion() */

/***********************************************************************************************************************
*  VttAdc_GetAdcValue
**********************************************************************************************************************/
/*! \brief     Returns the last ADC group conversion samples.
*   \param[in]  group     ADC group.
*   \param[in]  values    Pointer to array the samples of the ADC group shall be stored to.
*  \context    Called by ADC driver.
*  \note       Returns the last complete measurement series, not the current ongoing measurement values!
**********************************************************************************************************************/
void VttAdc_GetAdcValue(Adc_GroupType group, Adc_ValueType** values)
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Implementation ----------------------------------------------- */
  *values = VttAdc_Cntrl[group].ChannelValueArray;

} /* VttAdc_GetAdcValue() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Adc.c
*********************************************************************************************************************/
