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
 *            Module: VTTCntrl
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: VttCntrl_SysVar_Cfg.c
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

# define VTTSYSVAR_CFG_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
# include "VttCntrl_SysVar.h"
# include "VttCntrl_Base.h"
# include "CANoeAPI.h"
# include "VttCntrl_SysVar_Cbk.h"


/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/
/* The following pragmas ensure that all variables of this module are places in memory that is not cleared during a
 * simulated ECU reset/poweron */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
/* Check the version of VttCntrl_SysVar header file */
#if (  (VTTCNTRL_SYSVAR_MAJOR_VERSION != (0x03u)) \
    || (VTTCNTRL_SYSVAR_MINOR_VERSION != (0x00u)) \
    || (VTTCNTRL_SYSVAR_PATCH_VERSION != (0x06u)) )
# error "Vendor specific version numbers of VttCntrl_SysVar_Cfg.c and VttCntrl_SysVar.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VTTSYSVAR_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTSYSVAR_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTSYSVAR_CFG_PATCH_VERSION != (0x01u)))
  #error "Version numbers of VttCntrl_SysVar_Cfg.c and VttCntrl_SysVar_Cfg.h are inconsistent!"
#endif

/* Check the version of the VttCntrl_Base header file */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_SysVar_Cfg.c and VttCntrl_Base.h are inconsistent"
#endif


/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

void VttSysVar_ChangeHandlerUInt64(sint32 param, uint64 value);


/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

extern uint64 VttSysVar_NumPendingChanges; /* Number of Channel IDs with a pending change. */

#if VTTSYSVAR_CHANNELS > 0
extern uint64 VttSysVar_PendingChanges[VTTSYSVAR_CHANNELS]; /* List of IDs. Contains IDs of channels that have a pending change. */
#endif

#if VTTSYSVAR_CHANNELS > 0
VttSysVar_CntrlType VttSysVar_Channel_Cfg[VTTSYSVAR_CHANNELS] =
{
  { /* System Variable 'Display::Display_LightOnOffCounter' */
    CANOEAPI_SYSVAR_INVALIDHANDLE,  /* Handle to CANoeEmu system variable. */
    NULL_PTR, /* NotificationForwarder (Notification disabled) */
    0, /* hasChange flag */
    VTTSYSVAR_TYPE_ID_uint64 /* System Variable is of type uint64 */
  },
  { /* System Variable 'Display::Display_Odometer' */
    CANOEAPI_SYSVAR_INVALIDHANDLE,  /* Handle to CANoeEmu system variable. */
    NULL_PTR, /* NotificationForwarder (Notification disabled) */
    0, /* hasChange flag */
    VTTSYSVAR_TYPE_ID_uint64 /* System Variable is of type uint64 */
  },
  { /* System Variable 'Display::Display_OdometerWriteReqPending' */
    CANOEAPI_SYSVAR_INVALIDHANDLE,  /* Handle to CANoeEmu system variable. */
    NULL_PTR, /* NotificationForwarder (Notification disabled) */
    0, /* hasChange flag */
    VTTSYSVAR_TYPE_ID_uint64 /* System Variable is of type uint64 */
  },
};
#endif

void VttSysVar_ChangeHandlerUInt64(sint32 param, uint64 value)
{
  if ((param < VTTSYSVAR_CHANNELS) && (param >= 0))
  {
    CANoeAPI_AtomicBegin();
    VttSysVar_PendingChanges[VttSysVar_NumPendingChanges] = param;
    ++VttSysVar_NumPendingChanges;
    VttSysVar_Channel_Cfg[param].hasChange = 1;
    CANoeAPI_AtomicEnd();
    
    VttCntrl_Base_SetCheckedInterrupt(VttSysVar_IRQ);
  }
}


/* OnLoad */
void VttSysVar_OnLoad(void)
{
  VttSysVar_Channel_Cfg[0].handle = CANoeAPI_SysVar_DeclareUInt64("UserDefined::Display", "Display_LightOnOffCounter", 0U);
  VttSysVar_Channel_Cfg[1].handle = CANoeAPI_SysVar_DeclareUInt64("UserDefined::Display", "Display_Odometer", 0U);
  VttSysVar_Channel_Cfg[2].handle = CANoeAPI_SysVar_DeclareUInt64("UserDefined::Display", "Display_OdometerWriteReqPending", 0U);
}

/* Read/Write function for types */

Std_ReturnType VttSysVar_Read_uint64(uint32 sysVar, uint64 * value)
{
  /* Verify validity of system variable */
  if ((sysVar > VTTSYSVAR_CHANNELS) || (CANOEAPI_SYSVAR_INVALIDHANDLE == VttSysVar_Channel_Cfg[sysVar].handle))
  {
    return E_NOT_OK;
  }
  else if (VTTSYSVAR_TYPE_ID_uint64 != VttSysVar_Channel_Cfg[sysVar].typeId) /* Verify type of channel */
  {
    return E_NOT_OK;
  }
  else /* Get the actual value */
  {
    (*value) = CANoeAPI_SysVar_GetUInt64(VttSysVar_Channel_Cfg[sysVar].handle);
    return E_OK;
  }
}

Std_ReturnType VttSysVar_Write_uint64(uint32 sysVar, uint64 value)
{
  /* Verify validity of system variable */
  if ((sysVar > VTTSYSVAR_CHANNELS) || (CANOEAPI_SYSVAR_INVALIDHANDLE == VttSysVar_Channel_Cfg[sysVar].handle))
  {
    return E_NOT_OK;
  }
  else if (VTTSYSVAR_TYPE_ID_uint64 != VttSysVar_Channel_Cfg[sysVar].typeId) /* Verify type of channel */
  {
    return E_NOT_OK;
  }
  else
  {
    CANoeAPI_SysVar_SetUInt64(VttSysVar_Channel_Cfg[sysVar].handle, value);
    return E_OK;
  }
}



/* Notification forwarder for each system variable */





/**********************************************************************************************************************
*  END OF FILE: VttCntrl_SysVar_Cfg.c
*********************************************************************************************************************/


