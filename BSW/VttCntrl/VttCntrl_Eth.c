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
/**      \file    VTTCntrl_Eth.c
*        \brief   Implements the public API of the virtual Ethernet controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Ethernet controller that enables to send and receive Ethernet frames.
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

#define VTTETH_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Eth.h"
#include <string.h>  /* Declares function memcpy_s */
#include "Eth_30_Vtt_Lcfg.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the Ethernet controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Eth header file */
#if (  (VTTETH_SW_MAJOR_VERSION != (4u)) \
    || (VTTETH_SW_MINOR_VERSION != (3u)) \
    || (VTTETH_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of VttCntrl_Eth.c and VttCntrl_Eth.h are inconsistent"
#endif

/* Check the version of VttCntrl_Base header file */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
#error "Vendor specific version numbers of VttCntrl_Eth.c and VttCntrl_Base.h are inconsistent"
#endif

/**********************************************************************************************************************
*  GLOBAL VARIABLES
**********************************************************************************************************************/
uint32 VttEth_SysVar_TrcvLinkState;

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

/*! \brief Internal controller structure associated with each Ethernet channel. */
VttEth_CntrlType VttEth_Cntrl[VTTETH_CHANNELS];

/*! \brief Internal ECU state. */
STATIC uint8 VttEth_EcuState = CANOEAPI_ECUSTATE_INITIAL;

/*! \brief Name of bus associated with each Ethernet channel. */
extern const char* VttEth_BusName[VTTETH_CHANNELS]; /* PRQA S 0850,5013 */ /* MD_MSR_19.8,MD_Can_5013_LL */

STATIC VttEth_CntrlType VttData;
/* VttEth_*Descriptor is set by VttEth driver. Values must be lost during a reset. */
STATIC __declspec(allocate(".vbss")) uint8 VttEth_CurrentRxDescriptor[VTTETH_CHANNELS];
STATIC __declspec(allocate(".vbss")) uint8 VttEth_CurrentTxDescriptor[VTTETH_CHANNELS];
STATIC __declspec(allocate(".vbss")) VttEth_RxDescriptor* VttEth_RxDescriptorBaseAddress;
STATIC __declspec(allocate(".vbss")) VttEth_TxDescriptor* VttEth_TxDescriptorBaseAddress;

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttEth_OnEthernetStatusHandler(uint8 channel, uint32 status);
STATIC void VttEth_OnEthernetPacketHandler(uint8 channel, uint8 dir, uint32 packetSize, const uint8 *packetData);

STATIC void VttEth_AssignIRQ(void);
STATIC void VttEth_MapChannel(void);
STATIC void VttEth_SetHandler(void);

STATIC void VttEth_OnReset(void);
STATIC void VttEth_OnStartMeasurement(void);
STATIC void VttEth_OnStopMeasurement(void);
STATIC void VttEth_OnLoad(void);
STATIC void VttEth_OnGoToSleep(void);
STATIC void VttEth_OnWakeup(void);
STATIC void VttEth_OnSwitchOff(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

void VttEth_IncCurrentRxDescriptor(uint8 channel)
{
  if(VttEth_RxDescriptorBaseAddress[VttEth_CurrentRxDescriptor[channel]].LastElement == TRUE)
  {
    VttEth_CurrentRxDescriptor[channel] = 0;
  }
  else
  {
    VttEth_CurrentRxDescriptor[channel]++;
  }
}

void VttEth_IncCurrentTxDescriptor(uint8 channel)
{
  if(VttEth_TxDescriptorBaseAddress[VttEth_CurrentTxDescriptor[channel]].LastElement == TRUE)
  {
    VttEth_CurrentTxDescriptor[channel] = 0;
  }
  else
  {
    VttEth_CurrentTxDescriptor[channel]++;
  }
}

/***********************************************************************************************************************
*  VttEth_OnEthernetPacketHandler
**********************************************************************************************************************/
/*! \brief     Callback function of CAnoe.
*  \param[in]  channel                 Channel index
*  \param[in]  dir                     Direction of packet (either Tx or Rx)
*  \param[in]  packetSize              Size of packet.
*  \param[in]  packetData              Pointer to data.
*  \return     no return 
*  \pre        Input parameter are verified by CANoe.
*  \context    Function is called by CANoe.
**********************************************************************************************************************/
STATIC void VttEth_OnEthernetPacketHandler(uint8 channel, uint8 dir, uint32 packetSize, const uint8 *packetData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 *payload;

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  if(VttEth_Cntrl[channel].CtrlMode == ETH_MODE_ACTIVE)
  {
    CANoeAPI_AtomicBegin();
    if(VttEth_RxDescriptorBaseAddress[VttEth_CurrentRxDescriptor[channel]].SoftwareOwned == FALSE)
    {
      CANoeAPI_AtomicEnd();
      if(dir == ETH_DIRECTION_RX)
      {
        if(packetSize > VttEth_RxDescriptorBaseAddress[VttEth_CurrentRxDescriptor[channel]].AllowedPacketSize)
        {
          /* Frame is too long - dropped */
        }
        else
        {
          VttEth_RxDescriptorBaseAddress[VttEth_CurrentRxDescriptor[channel]].ReceivedPacketSize = (uint16)packetSize;
          payload = (uint8*)(Eth_30_Vtt_RxDescriptors[VttEth_CurrentRxDescriptor[channel]].DataPtr);

          /* Copy Ethernet frame data into Ethernet RxBuffer like the DMA would do */
          memcpy_s(&payload[2], VTTETH_FRAME_BUFFER_SIZE, packetData, packetSize);
          VttEth_RxDescriptorBaseAddress[VttEth_CurrentRxDescriptor[channel]].SoftwareOwned = TRUE;

          VttEth_IncCurrentRxDescriptor(channel);
        }
      }
      if(dir == ETH_DIRECTION_TX)
      {
        VttEth_TxDescriptorBaseAddress[VttEth_CurrentTxDescriptor[channel]].SoftwareOwned = TRUE;
        VttEth_IncCurrentTxDescriptor(channel);
      }
    }
    else
    {
      CANoeAPI_AtomicEnd();
      /* Descriptor ring is full - frame is dropped. */
      CANoeAPI_Printf("VttEth was not able to process all received frames. At least one ethernet frame is lost.\nPlease increase the number of Ethernet buffers in DaVinci Configurator Pro.");
    }

    /* Inform Ethernet driver about Tx/Rx event. */
    if(dir == ETH_DIRECTION_RX)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttEth_Cntrl[channel].IsrRx_No);
    }
    else if(dir == ETH_DIRECTION_TX)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttEth_Cntrl[channel].IsrTx_No);
    }
  }

} /* VttEth_OnEthernetPacketHandler() */

/***********************************************************************************************************************
*  VttEth_OnEthernetStatusHandler
**********************************************************************************************************************/
/*! \brief     Tracks status changes of Ethernet channels.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttEth_OnEthernetStatusHandler(uint8 channel, uint32 status)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }
  VttEth_Cntrl[channel].Status = status;

} /* VttEth_OnEthernetStatusHandler() */

/***********************************************************************************************************************
*  VttEth_SetHandler
**********************************************************************************************************************/
/*! \brief     Register callback functions in CANoe.
*  \return     no return 
*  \pre        -
*  \context    Function shall be called by VTT Ethernet controller during loading.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_SetHandler(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_RegisterEthernetPacket(&VttEth_OnEthernetPacketHandler);
  CANoeAPI_RegisterEthernetStatus(&VttEth_OnEthernetStatusHandler);

} /* VttEth_SetHandler() */

/***********************************************************************************************************************
*  VttEth_AssignIRQ
**********************************************************************************************************************/
/*! \brief     Store the IRQ numbers given by the OS in the Controller Structure.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_AssignIRQ(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  int channel;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    VttEth_Cntrl[channel].IsrRx_No = ETH_IRQ_OFFSET + channel * 2;
    VttEth_Cntrl[channel].IsrTx_No = ETH_IRQ_OFFSET + channel * 2 + 1;
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_AssignIRQ() */

/***********************************************************************************************************************
*  VttEth_MapChannel
**********************************************************************************************************************/
/*! \brief     Maps the channels defined by the User in DaVinci Configurator Pro on the
*              channels of CANoe.
*  \return     no return 
*  \context    Function shall be called in CANoeAPI_InitHook().
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_MapChannel(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    CANoeAPI_MapChannel(VttEth_BusName[channel], CANOEAPI_BUSTYPE_ETHERNET, channel);
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_MapChannel() */

/***********************************************************************************************************************
*  VttEth_OnLoad
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \param[in]  -
*  \return     no return 
*  \pre        -
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  VttEth_AssignIRQ();

  /* Set the Modes of all Controller on DOWN. */
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    VttEth_SetControllerMode(channel, ETH_MODE_DOWN);
  }

  VttEth_MapChannel();

  VttEth_SysVar_TrcvLinkState = CANoeAPI_SysVar_DeclareUInt32("EthTestApp", "TrcvLinkState", 0);
} /* VttEth_OnLoad() */

/***********************************************************************************************************************
*  VttEth_OnGoToSleep
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnGoToSleep(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnGoToSleep() */

/***********************************************************************************************************************
*  VttEth_OnWakeup
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnWakeup(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

} /* VttEth_OnWakeup() */

/***********************************************************************************************************************
*  VttEth_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     Initialization during the start of a measurement in CANoe.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       A message is only sent, when the ECU is in state Processing (Emulation of RUN Mode).
**********************************************************************************************************************/
STATIC void VttEth_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetHandler();

  /* Set all channels to have link up */
  for (channel = 0; channel < VTTETH_CHANNELS; ++channel)
  {
    VttEth_Cntrl[channel].Status = 0;
  }

} /* VttEth_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttEth_OnReset
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       
**********************************************************************************************************************/
STATIC void VttEth_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnReset() */

/***********************************************************************************************************************
*  VttEth_OnSwitchOff
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnSwitchOff() */

/***********************************************************************************************************************
*  VttEth_OnStopMeasurement
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnStopMeasurement() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttEth_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual Ethernet controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttEth_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  VttEth_EcuState = newState;
  oldState = oldState;

  /* ----- Implementation ----------------------------------------------- */
  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttEth_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttEth_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttEth_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    {
      VttEth_OnGoToSleep();
    }
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    {
      VttEth_OnWakeup();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttEth_OnSwitchOff();
    }
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttEth_OnStopMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  }

} /* VttEth_OnStateChange() */

/***********************************************************************************************************************
*  VttEth_SetDescriptorBaseAddress
**********************************************************************************************************************/
/*! \brief     Set Descriptor Base Address
*  \param[in]  Descriptor Base Address
*  \return     no return 
*  \pre        -
*  \context    Function is called by DrvEth_Vtt
*  \note       -
**********************************************************************************************************************/
void VttEth_SetDescriptorBaseAddress(VttEth_RxDescriptor* RxDescriptorBaseAddress, VttEth_TxDescriptor* TxDescriptorBaseAddress)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_RxDescriptorBaseAddress = RxDescriptorBaseAddress;
  VttEth_TxDescriptorBaseAddress = TxDescriptorBaseAddress;

} /* VttEth_SetDescriptorBaseAddress() */

/***********************************************************************************************************************
*  VttEth_SetControllerMode
**********************************************************************************************************************/
/*! \brief     Set the Mode of a particular Eth Controller.
*  \param[in]  channel                 Channel index
*              CtrlMode                Mode for the given Controller.
*  \return     no return 
*  \context    Function is called by Eth Driver.
*  \note       Two modes are defined for an Eth Controller.
*              - ETH_MODE_DOWN
*              - ETH_MODE_ACTIVE
**********************************************************************************************************************/
void VttEth_SetControllerMode(uint8 channel, Eth_ModeType CtrlMode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  CANoeAPI_AtomicBegin();
  VttEth_Cntrl[channel].CtrlMode = CtrlMode;
  if(VttEth_Cntrl[channel].CtrlMode == ETH_MODE_DOWN)
  {
    VttEth_CurrentRxDescriptor[channel] = 0;
    VttEth_CurrentTxDescriptor[channel] = 0;
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_SetControllerMode() */

/***********************************************************************************************************************
*  VttEth_GetControllerMode
**********************************************************************************************************************/
/*! \brief     Get the Mode of a particular Eth Controller.
*  \param[in]  channel                 Channel index
*  \param[out] CtrlModePtr             Mode for the given Controller.
*  \return     no return 
*  \context    Function is called by Eth Driver.
*  \note       Two modes are defined for an Eth Controller.
*              - ETH_MODE_DOWN
*              - ETH_MODE_ACTIVE
**********************************************************************************************************************/
void VttEth_GetControllerMode(uint8 channel, Eth_ModeType* CtrlModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  CANoeAPI_AtomicBegin();
  *CtrlModePtr = VttEth_Cntrl[channel].CtrlMode;
  CANoeAPI_AtomicEnd();

} /* VttEth_GetControllerMode() */

/***********************************************************************************************************************
*  VttEth_OutputEthernetPacket
**********************************************************************************************************************/
/*! \brief     Sends a message by using CANoeAPI_OutputEthernetPacket.
*  \param[in]  channel                 Channel index
*  \param[in]  packetSize              Size of packet.
*  \param[out] packetData              Pointer to data.
*  \return     no return 
*  \pre        Input parameter are verified by Eth Driver.
*  \context    Function is called by Eth Driver.
**********************************************************************************************************************/
void VttEth_OutputEthernetPacket(uint8 channel, uint32 packetSize, const uint8 *packetData)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  if((VttEth_EcuState == CANOEAPI_ECUSTATE_PROCESSING) && (VttEth_Cntrl[channel].CtrlMode == ETH_MODE_ACTIVE))
  {
    CANoeAPI_OutputEthernetPacket(channel, packetSize, packetData);
  }

} /* VttEth_OutputEthernetPacket() */

/***********************************************************************************************************************
*  VttEth_SetAllChannelsOnDownMode
**********************************************************************************************************************/
/*! \brief     Sets all Ethernet channels on Down Mode.
*  \return     no return 
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetAllChannelsOnDownMode(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  int channel;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    VttEth_Cntrl[channel].CtrlMode = ETH_MODE_DOWN;
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_SetAllChannelsOnDownMode() */

/***********************************************************************************************************************
*  VttEth_GetStatus
**********************************************************************************************************************/
/*! \brief     Returns the status of the given Ethernet channel.
*  \return     current channel status.
*              Possible states are:  status unknown  = VTTETH_STATE_UNKNOWN
*                                    not connected   = VTTETH_STATE_NOT_CONNECTED
*                                    connected       = VTTETH_STATE_CONNECTED
*                                    channel unknown = VTTETH_STATE_CHANNEL_UNKNOWN
*  \context    Function is called by EthTrcv_Vtt.
*  \note       -
**********************************************************************************************************************/
uint32 VttEth_GetStatus(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return VTTETH_STATE_CHANNEL_UNKNOWN;
  }
  return VttEth_Cntrl[channel].Status;

} /* VttEth_GetStatus() */

/***********************************************************************************************************************
*  VttEth_GetData
**********************************************************************************************************************/
/*! \brief     Returns the data of the corresponding Ethernet channel.
*  \return     data structure 
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
VttEth_CntrlType *VttEth_GetData(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel < VTTETH_CHANNELS)
  {
    CANoeAPI_AtomicBegin();
    VttData = VttEth_Cntrl[channel];
    CANoeAPI_AtomicEnd();
  }
  return &VttData;

} /* VttEth_GetData() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Eth.c
*********************************************************************************************************************/
