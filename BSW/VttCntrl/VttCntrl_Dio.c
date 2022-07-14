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
/**      \file    VTTCntrl_Dio.h
*        \brief   Implements the public API of the virtual DIO controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual DIO controller that enables to read/write single IO ports or complete groups at once.
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

#define VTTDIO_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Dio.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the DIO controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Dio header file. */
#if (  (VTTDIO_SW_MAJOR_VERSION != (3u)) \
    || (VTTDIO_SW_MINOR_VERSION != (0u)) \
    || (VTTDIO_SW_PATCH_VERSION != (6u)) )
# error "Vendor specific version numbers of VttCntrl_Dio.c and VttCntrl_Dio.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VTTDIO_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTDIO_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTDIO_CFG_PATCH_VERSION != (0x02u)))
  #error "Version numbers of VttCntrl_Dio.c and VttCntrl_Dio_Cfg.h are inconsistent!"
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

#ifdef VTTDIO_CHANNELGROUPS
extern sint32 VttDio_GroupHandle[VTTDIO_CHANNELGROUPS];
#endif
extern sint32 VttDio_PortHandle[VTTDIO_PORTS];

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
uint8 VttDio_EcuState = CANOEAPI_ECUSTATE_INITIAL;

/**********************************************************************************************************************
*  LOCAL FUNCTIONS PROTOTYPES
**********************************************************************************************************************/

STATIC void VttDio_OnLoad(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttDio_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual DIO controller.
*
*         Tasks:
*            - Assign each DIO channel's controller an IRQ for triggering channel changes.
*
*  \context    Called by VttDio_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttDio_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttDio_MapChannel();
  
} /* VttDio_OnLoad() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttDio_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual DIO controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttDio_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttDio_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttDio_OnLoad();
    }
    break;
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
  
} /* VttDio_OnStateChange() */

/***********************************************************************************************************************
*  VttDio_ReadPort
**********************************************************************************************************************/
/*! \brief     Reads the voltage level (LOW or HIGH) of a single input port (channel).
*  \param[in]  portId      Port number.
*  \return     Voltage level of the input port (channel).
*  \context    Called by DIO driver.
**********************************************************************************************************************/
Dio_PortLevelType VttDio_ReadPort(Dio_PortType portId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if((portId >= VTTDIO_PORTS) || (VttDio_EcuState != CANOEAPI_ECUSTATE_PROCESSING))
  {
    return 0 ;
  }

  return CANoeAPI_SysVar_GetInt(VttDio_PortHandle[portId]);
  
} /* VttDio_ReadPort() */

/***********************************************************************************************************************
*  VttDio_WritePort
**********************************************************************************************************************/
/*! \brief     Write a new voltage level (LOW or HIHG) to a single output port (channel).
*  \param[in]  portId     Port number.
*  \param[in]  level      New voltage level of the output port (channel).
*  \context    Called by DIO driver.
**********************************************************************************************************************/
void VttDio_WritePort(Dio_PortType portId, Dio_PortLevelType level)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if((portId >= VTTDIO_PORTS) || (VttDio_EcuState != CANOEAPI_ECUSTATE_PROCESSING))
  {
    return;
  }

  CANoeAPI_SysVar_SetInt(VttDio_PortHandle[portId], level);
  
} /* VttDio_WritePort() */

#ifdef VTTDIO_CHANNELGROUPS
/***********************************************************************************************************************
*  VttDio_ReadChannelGroup
**********************************************************************************************************************/
/*! \brief     Reads the current voltage levels (LOW or HIGH) of multiple input ports (channels) at once.
*  \param[in]  groupId    Group number.
*  \return     Voltage levels of the input ports (channels) forming the group.
*  \context    Called by DIO driver.
**********************************************************************************************************************/
uint32 VttDio_ReadChannelGroup(sint32 groupId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if((groupId >= VTTDIO_CHANNELGROUPS) || (VttDio_EcuState != CANOEAPI_ECUSTATE_PROCESSING))
  {
    return 0;
  }

  return CANoeAPI_SysVar_GetInt(VttDio_GroupHandle[groupId]);
  
} /* VttDio_ReadChannelGroup() */

/***********************************************************************************************************************
*  VttDio_WriteChannelGroup
**********************************************************************************************************************/
/*! \brief     Writes new voltage levels (LOW or HIGH) to multiple output ports (channels) at once.
*  \param[in]  groupId    Group number.
*  \param[in]  levels     New voltage levels of the output ports (channels) forming the group.
*  \context    Called by DIO driver.
**********************************************************************************************************************/
void VttDio_WriteChannelGroup(sint32 groupId, uint32 levels)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */  
  if((groupId >= VTTDIO_CHANNELGROUPS) || (VttDio_EcuState != CANOEAPI_ECUSTATE_PROCESSING))
  {
    return;
  }
  
  CANoeAPI_SysVar_SetInt(VttDio_GroupHandle[groupId], levels);
  
} /* VttDio_WriteChannelGroup() */
#endif 

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Dio.c
*********************************************************************************************************************/
