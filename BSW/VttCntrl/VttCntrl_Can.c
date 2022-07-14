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
/**      \file    VTTCntrl_Can.c
*        \brief   Implements the public API of the virtual CAN controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual CAN controller that enables to send and receive CAN frames via different CAN channels.
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

#define VTTCAN_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Can.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the CAN controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Can header file. */
#if (  (VTTCAN_SW_MAJOR_VERSION != (3u)) \
    || (VTTCAN_SW_MINOR_VERSION != (1u)) \
    || (VTTCAN_SW_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of VttCntrl_Can.c and VttCntrl_Can.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/*! \brief Offset of IRQ numbers in os.h for CAN controller interrupts. */
# define CAN_IRQ_OFFSET IRQ_CanIsrTx_0

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

/*! \brief Internal CAN controller structure associated with a CAN channel. */
typedef struct
{
  sint32  IsrTx_Nr;
  sint32  IsrRx_Nr;
  sint32  IsrErr_Nr;
  sint32  IsrWakeup_Nr;
  uint8  Mode;
  boolean IsrTx_Enabled;
  boolean IsrRx_Enabled;
  boolean IsrErr_Enabled;
  boolean IsrWakeup_Enabled;
} VttCan_CntrlType;

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttCan_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal CAN controller state of each channel. */
STATIC VttCan_CntrlType VttCan_Cntrl[VTTCAN_CHANNELS];
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
/*! \brief Pointer to the VttCntrl_Can configuration. */
STATIC VttCan_ChannelsStruct * VttCan_CfgPtr;
#endif

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

extern void Can_RxIndication(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data);
extern void Can_TxConfirmation(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data);
extern void Can_ErrIndication(uint8 channelId, uint32 errCode, uint8 errCntTx, uint8 errCntRx);

STATIC void VttCan_OnMsgHandler(uint8 channelId, uint32 id, uint8 dir, uint32 flags, uint8 dataLength, const uint8* data);
STATIC void VttCan_OnRxMessage(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data);
STATIC void VttCan_OnTxMessage(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data);
STATIC void VttCan_OnErrHandler(uint8 channelId, uint32 errCode, uint8 errCntTx, uint8 errCntRx);

STATIC void VttCan_DisableAllISR(uint8 channelId);
STATIC void VttCan_MapChannel(void);
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
STATIC void VttCan_SelectVariant(void);
#endif
STATIC void VttCan_AssignIRQ(void);
STATIC void VttCan_RegisterHandlers(void);

STATIC void VttCan_OnLoad(void);
STATIC void VttWdg_OnStartMeasurement(void);
STATIC void VttWdg_OnReset(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  VttCan_OnMsgHandler()
**********************************************************************************************************************/
/*! \brief     Transmission callout of CANOeEmu when CAN frames are send.
*  \details    Handles the reception and transmission-acknowledgement of CAN frames. 
*              In case the ECU is sleeping, an arriving CAN frame will wakeup the processor to handle the CAN frame.
*
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that shall
*                               receive the CAN frame.
*  \param[in]  id               CAN frame identifier.
*  \param[in]  dir              Transmission direction, either CANOEAPI_DIR_RX or CANOEAPI_DIR_TX (ACKs).
*  \param[in]  flags            CAN frame flags, refer to the CANoeAPI header for more information.
*  \param[in]  dataLength       Number of bytes contained in the CAN frame.
*  \param[in]  data             Pointer to buffer holding the CAN frame data.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttCan_OnMsgHandler(uint8 channelId, uint32 id, uint8 dir, uint32 flags, uint8 dataLength, const uint8* data)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttCan_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if (dir == CANOEAPI_DIR_RX)
    {
      VttCan_OnRxMessage(channelId, id, flags, dataLength, data);
    }
    else if (dir == CANOEAPI_DIR_TX)
    {
      VttCan_OnTxMessage(channelId, id, flags, dataLength, data);
    }
    else
    {
      CANoeAPI_WriteString("VTT CAN Controller - Frame cannot be processed since transmission direction unknown!");
    }
  }
  else if ((VttCan_EcuState == CANOEAPI_ECUSTATE_SLEEPING) && (dir == CANOEAPI_DIR_RX))
  {
    VttCan_OnRxMessage(channelId, id, flags, dataLength, data);
    CANoeAPI_WakeupProcessor();
  }

} /* VttCan_OnMsgHandler() */

/**********************************************************************************************************************
*  VttCan_OnRxMessage()
**********************************************************************************************************************/
/*! \brief     Processes reception of CAN frames.
*  \details    Handles the reception of CAN frames. In case the CAN controller sleeps, the CAN controller is woken up.
*
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that shall
*                               receive the incoming CAN frame.
*  \param[in]  id               CAN frame identifier.
*  \param[in]  flags            CAN frame flags, refer to the CANoeAPI header for more information.
*  \param[in]  dataLength       Number of bytes containt in the CAN frame.
*  \param[in]  data             Pointer to buffer holding the CAN frame data.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttCan_OnRxMessage(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_SLEEP)
  {
    /* Wakeup sleeping CAN controller. */
    Can_RxIndication(channelId, id, flags, dataLength, data);
    if (VttCan_Cntrl[channelId].IsrWakeup_Enabled)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttCan_Cntrl[channelId].IsrWakeup_Nr);
    }    
  }
  else if (VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_START)
  {
    /* Notify channel's CAN controller about incoming CAN frame to be processed.
     * Copy data of CAN frame into corresponding CAN cell. */
    Can_RxIndication(channelId, id, flags, dataLength, data);
    if (VttCan_Cntrl[channelId].IsrRx_Enabled)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttCan_Cntrl[channelId].IsrRx_Nr);
    }    
  }

}  /* VttCan_OnRxMessage() */

/**********************************************************************************************************************
*  VttCan_OnTxMessage()
**********************************************************************************************************************/
/*! \brief     Processes transmission acknowledgements of CAN frames.
*  \details    Delegates an acknowledgement to the CAN driver when a CAN frame was successfully transmitted.
*
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that had send
*                               the CAN frame.
*  \param[in]  id               CAN frame identifier.
*  \param[in]  flags            CAN frame flags, refer to the CANoeAPI header for more information.
*  \param[in]  dataLength       Number of bytes containt in the CAN frame.
*  \param[in]  data             Pointer to buffer holding the CAN frame data.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttCan_OnTxMessage(uint8 channelId, uint32 id, uint32 flags, uint8 dataLength, const uint8* data)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_START)
  {
    /* Notify channel's CAN controller about successful CAN frame transmission. */
    Can_TxConfirmation(channelId, id, flags, dataLength, data);
    if (VttCan_Cntrl[channelId].IsrTx_Enabled)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttCan_Cntrl[channelId].IsrTx_Nr);
    }
  }

}  /* VttCan_OnTxMessage() */

/***********************************************************************************************************************
*  VttCan_OnErrHandler()
**********************************************************************************************************************/
/*! \brief     Handles CAN bus errors.
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that shall
*                               handle the CAN bus error.
*  \param[in]  errCode          Error code, refer to the header for more information.
*  \param[in]  errCntTx         Tx error counter.
*  \param[in]  errCntRx         Rx error counter.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttCan_OnErrHandler(uint8 channelId, uint32 errCode, uint8 errCntTx, uint8 errCntRx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttCan_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if (VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_START)
    {
      Can_ErrIndication(channelId, errCode, errCntTx, errCntRx);
      if (VttCan_Cntrl[channelId].IsrErr_Enabled)
      {
        VttCntrl_Base_SetCheckedInterrupt(VttCan_Cntrl[channelId].IsrErr_Nr);
      }
    }
  }

} /* VttCan_OnErrHandler() */

/***********************************************************************************************************************
*  VttCan_DisableAllISR()
**********************************************************************************************************************/
/*! \brief     Disables interrupt processing of hardware events (Rx, Tx, Wakeup and BusOff) caused by a CAN channel.
*
*  \param[in]  channelId        Channel number identified with the CAN controller that shall not cause
*                               interrupts when Rx, Tx, Wakeup or BusOff events are triggered by the channel.
*
*  \context    Called by VttCan_OnStartMeasurement and VttCan_OnReset.
**********************************************************************************************************************/
STATIC void VttCan_DisableAllISR(uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttCan_Cntrl[channelId].IsrTx_Enabled = FALSE;
  VttCan_Cntrl[channelId].IsrRx_Enabled = FALSE;
  VttCan_Cntrl[channelId].IsrWakeup_Enabled = FALSE;
  VttCan_Cntrl[channelId].IsrErr_Enabled = FALSE;

}  /* VttCan_DisableAllISR() */

/***********************************************************************************************************************
*  VttCan_MapChannel()
**********************************************************************************************************************/
/*! \brief     Maps the CAN channel numbers identified with the CAN controllers to the CAN buses provided by CANoeEmu.
*   \context   Called by VttCan_OnLoad.
**********************************************************************************************************************/
STATIC void VttCan_MapChannel(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channelId = 0;
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  uint8 variantId = 0;
#endif

  /* ----- Implementation ----------------------------------------------- */
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  for (channelId = 0; channelId < VttCan_CfgPtr->number_of_channels; channelId++)
  {
    CANoeAPI_MapChannel((VttCan_CfgPtr->bus_names)[channelId], CANOEAPI_BUSTYPE_CAN, channelId);
  }
#else
  for (channelId = 0; channelId < VTTCAN_CHANNELS; channelId++)
  {
    CANoeAPI_MapChannel(VttCan_BusName[channelId], CANOEAPI_BUSTYPE_CAN, channelId);
  }  
#endif

}  /* VttCan_MapChannel() */

/***********************************************************************************************************************
*  VttCan_AssignIRQ()
**********************************************************************************************************************/
/*! \brief      Assigns each CAN channel's CAN controller the IRQs required for interrupt-based
*               processing of hardware events.
*   \context    Called by VttCan_OnLoad.
**********************************************************************************************************************/
STATIC void VttCan_AssignIRQ(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channelId;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  for (channelId = 0; channelId < VttCan_CfgPtr->number_of_channels; channelId++)
#else
  for (channelId = 0; channelId < VTTCAN_CHANNELS; channelId++)
#endif
  {
    VttCan_Cntrl[channelId].IsrTx_Nr      = CAN_IRQ_OFFSET + channelId * 4 + 0;
    VttCan_Cntrl[channelId].IsrRx_Nr      = CAN_IRQ_OFFSET + channelId * 4 + 1;
    VttCan_Cntrl[channelId].IsrErr_Nr     = CAN_IRQ_OFFSET + channelId * 4 + 2;
    VttCan_Cntrl[channelId].IsrWakeup_Nr  = CAN_IRQ_OFFSET + channelId * 4 + 3;
  }
  CANoeAPI_AtomicEnd();

}  /* VttCan_AssignIRQ() */

/***********************************************************************************************************************
*  VttCan_RegisterHandlers()
**********************************************************************************************************************/
/*! \brief     Registers the hardware event handlers at CANoeEmu for processing the CAN channels.
*   \context   Called by VttCan_OnStartMeasurement.
**********************************************************************************************************************/
STATIC void VttCan_RegisterHandlers(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_SetCanErrHandler(&VttCan_OnErrHandler);
  CANoeAPI_SetCanMsgHandler(&VttCan_OnMsgHandler);

}  /* VttCan_RegisterHandlers() */

/***********************************************************************************************************************
*  VttCan_OnLoad()
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual CAN controller.
*
*         Tasks:
*            - Assigns each channel's CAN controller the IRQ numbers.
*            - Maps the channel numbers to the corresponding CAN bus names.
*            - Sets each channel's CAN controller to mode INIT.
*
*  \context    Called by VttCan_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttCan_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channelId;

  /* ----- Implementation ----------------------------------------------- */
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  VttCan_SelectVariant();
#endif
  VttCan_AssignIRQ();
  VttCan_MapChannel();

  CANoeAPI_AtomicBegin();
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  for (channelId = 0; channelId < VttCan_CfgPtr->number_of_channels; channelId++)
#else
  for (channelId = 0; channelId < VTTCAN_CHANNELS; channelId++)
#endif
  {
    VttCan_Cntrl[channelId].Mode = VTTCAN_MODE_INIT;
  }
  CANoeAPI_AtomicEnd();

}  /* VttCan_OnLoad() */

#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
STATIC void VttCan_SelectVariant(void)
{
  VttCan_CfgPtr = getVttCanCfgPtr();
}
#endif

/***********************************************************************************************************************
*  VttCan_OnStartMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual CAN controller.
*
*         Tasks: 
*            - Disables the triggering of interrupts by all virtual CAN controllers.
*            - Registers the hardware event handlers at CANoeEmu.
*
*  \context    Called by VttCan_OnStateChange handler immediately before simulation is started.
**********************************************************************************************************************/
STATIC void VttCan_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channelId;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  for (channelId = 0; channelId < VttCan_CfgPtr->number_of_channels; channelId++)
#else
  for (channelId = 0; channelId < VTTCAN_CHANNELS; channelId++)
#endif
  {
    VttCan_DisableAllISR(channelId);
  }
  CANoeAPI_AtomicEnd();

  VttCan_RegisterHandlers();

}  /* VttCan_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttCan_OnReset()
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual CAN controller.
*
*         Tasks: 
*            - Sets each channel's CAN controller to mode STOP.
*            - Disables the triggering of interrupts by all virtual CAN controllers.
*
*  \context    Called by VttCan_OnStateChange handler right before ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void  VttCan_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channelId;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  for (channelId = 0; channelId < VttCan_CfgPtr->number_of_channels; channelId++)
#else
  for (channelId = 0; channelId < VTTCAN_CHANNELS; channelId++)
#endif
  {
    VttCan_Cntrl[channelId].Mode = VTTCAN_MODE_STOP;
    VttCan_DisableAllISR(channelId);
  }
  CANoeAPI_AtomicEnd();

}  /* VttCan_OnReset() */

/***********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttCan_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual CAN controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttCan_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttCan_EcuState = newState;

  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttCan_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttCan_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttCan_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

}  /* VttCan_OnStateChange() */

/***********************************************************************************************************************
*  VttCan_SendMessage()
**********************************************************************************************************************/
/*! \brief     Send CAN frame via virtual CAN controller.
*
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that shall
*                               send the CAN frame.
*  \param[in]  id               CAN frame identifier.
*  \param[in]  dlc              Data length code.  
*  \param[in]  data             Pointer to buffer holding the data to be embedded into the CAN frame.
*  \param[in]  flags            CAN frame flags, see CANoeAPI header for more information.
*
*  \context    Called by CAN driver.
*  \note       CAN frame is only sent when the ECU is in state Processing (Emulation of RUN mode).
**********************************************************************************************************************/
void VttCan_SendMessage(uint8 channelId, uint32 id, uint8 dlc, uint8 data[], uint32 flags)        
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  if (channelId < VttCan_CfgPtr->number_of_channels)
#else
  if (channelId < VTTCAN_CHANNELS)
#endif
  { 
    if(VttCan_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
    {
      if(VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_START)
      {
        CANoeAPI_SendMessage(channelId, id, dlc, data, flags);
      }
    }
  }

}  /* VttCan_SendMessage() */

/***********************************************************************************************************************
*  VttCan_SendCanMessage()
**********************************************************************************************************************/
/*! \brief     Send CAN frame via virtual CAN controller.
*
*  \param[in]  channelId        Channel number identified with the corresponding CAN controller that shall
*                               send the CAN frame.
*  \param[in]  id               CAN frame identifier.
*  \param[in]  flags            CAN frame flags, see CANoeAPI header for more information.
*  \param[in]  txReqCount       Range: [0,15], maximum number of transmission attempts.                                        
*                               Value 0: Transmission will be repeated until it has been sent successfully once.
*  \param[in]  dataLength       Number of bytes contains in the CAN frame.
*  \param[in]  data             Pointer to buffer holding the data to be embedded into the CAN frame.
*
*  \context    Called by CAN driver.
*  \note       CAN frame is only sent when the ECU is in state Processing (Emulation of RUN mode).
**********************************************************************************************************************/
void VttCan_SendCanMessage(uint8 channelId, 
  uint32 id, uint32 flags, uint8 txReqCount, uint8 dataLength, const uint8* data)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttCan_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if (VttCan_Cntrl[channelId].Mode == VTTCAN_MODE_START)
    {
      CANoeAPI_SendCanMessage(channelId, id, flags, txReqCount, dataLength, data);
    }
  }

}  /* VttCan_SendCanMessage() */

/***********************************************************************************************************************
*  VttCan_SetCanBtr()
**********************************************************************************************************************/
/*! \brief     Set baud-rate for a CAN channel the corresponding virtual controller has to use to process it.
*
*  \param[in]  channelId       Channel number identified with the corresponding CAN controller.
*              btr0            Baud-rate 0.
*              btr1            Baud-rate 1.
*
*  \context    Called by CAN driver.
**********************************************************************************************************************/
void  VttCan_SetCanBtr(uint8 channelId, uint8 btr0, uint8 btr1)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();

  CANoeAPI_SetCanBtr(channelId, btr0, btr1);
  /* CAN channel has to be reset after baud-rate changes to enable the effect, refer to CANoeAPI header. */
  CANoeAPI_ResetCan(channelId);

  CANoeAPI_AtomicEnd();

}  /* VttCan_SetCanBtr() */

/***********************************************************************************************************************
*  VttCan_SetMode()
**********************************************************************************************************************/
/*! \brief     Changes the mode of the virtual controller associated with a CAN channel.
*
*  \param[in]  channelId         Channel number identified with the corresponding CAN controller.
*  \param[in]  mode              Virtual CAN controller mode, refer to header for possible values.
*
*  \context    Called by CAN driver.
**********************************************************************************************************************/
void  VttCan_SetMode(uint8 channelId, uint8 mode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  VttCan_Cntrl[channelId].Mode = mode;
  CANoeAPI_AtomicEnd();

}  /* VttCan_SetMode() */

/***********************************************************************************************************************
*  VttCan_EnableISR()
**********************************************************************************************************************/
/*! \brief     Enables a virtual CAN controller to process the hardware events (Tx, Rx, Wakuep and BusOff) caused by
*          the corresponding CAN channel via interrupts.
*   \details   Can be used to mask/unmask interrupts temporarily if previously enabled, too.
*
*   \param[in]  channelId        Channel number identified with the corresponding CAN controller.
*   \param[in]  enableTx         TRUE iff Tx interrupt shall be enabled or unmasked; FALSE to mask Tx interrupt.
*   \param[in]  enableRx         TRUE iff Rx interrupt shall be enabled or unmasked; FALSE to mask Rx interrupt.
*   \param[in]  enableError      TRUE iff BusOff error interrupt shall be enabled or unmasked; FALSE to mask BusOff
*                                 error interrupt.
*   \param[in]  enableWakeup     TRUE iff wakeup interrupt shall be enabled or unmasked; FALSE to mask wakeup interrupt.
*
*   \context    Called by CAN driver.
**********************************************************************************************************************/
void VttCan_EnableISR(uint8 channelId, boolean enableTx, boolean enableRx, boolean enableError, boolean enableWakeup)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
  if (channelId >= VttCan_CfgPtr->number_of_channels)
#else
  if (channelId >= VTTCAN_CHANNELS)
#endif
  {
    return;
  }
  
  /* To support processing of hardware events via interrupts or individual polling, remember for each hardware event
     * if the interrupt source was enabled at least once. If for a hardware event the interrupt source was enabled,
   * the virtual CAN controller will trigger an interrupt to process this event. Otherwise, the event is processed
   * via polling, that is, we enable a global interrupt blockade in case of polling. */
  VttCan_Cntrl[channelId].IsrTx_Enabled |= enableTx;
  VttCan_Cntrl[channelId].IsrRx_Enabled |= enableRx;
  VttCan_Cntrl[channelId].IsrErr_Enabled |= enableError;
  VttCan_Cntrl[channelId].IsrWakeup_Enabled |= enableWakeup;
  
    /* Independent of the fact that interrupt sources are enabled, unmask/mask the interrupt source. */
  if(enableTx == TRUE)
  {
    VttCntrl_Base_UnmaskInterrupt(VttCan_Cntrl[channelId].IsrTx_Nr);
  }
  else if(enableTx == FALSE)
  {
    VttCntrl_Base_MaskInterrupt(VttCan_Cntrl[channelId].IsrTx_Nr);
  }

  if(enableRx == TRUE)
  {
    VttCntrl_Base_UnmaskInterrupt(VttCan_Cntrl[channelId].IsrRx_Nr);
  }
  else if(enableRx == FALSE)
  {
    VttCntrl_Base_MaskInterrupt(VttCan_Cntrl[channelId].IsrRx_Nr);
  }
  
  if(enableError == TRUE)
  {
    VttCntrl_Base_UnmaskInterrupt(VttCan_Cntrl[channelId].IsrErr_Nr);
  }
  else if(enableError == FALSE)
  {
    VttCntrl_Base_MaskInterrupt(VttCan_Cntrl[channelId].IsrErr_Nr);
  }
  
  if(enableWakeup == TRUE)
  {
    VttCntrl_Base_UnmaskInterrupt(VttCan_Cntrl[channelId].IsrWakeup_Nr);
  }
  else if(enableWakeup == FALSE)
  {
    VttCntrl_Base_MaskInterrupt(VttCan_Cntrl[channelId].IsrWakeup_Nr);
  }

}   /* VttCan_EnableISR() */

/***********************************************************************************************************************
*  VttCan_SetCanConfiguration()
**********************************************************************************************************************/
/*! \brief      Configure the virtual CAN controller's parameters for arbitration and data phase.
*  \details     Additionally the acknowledgement can be switch on or off.

*   \param[in]  channelId         Channel number identified with the corresponding CAN controller.
*   \param[in]  arbBaudrate       Baud-rate for arbitration phase in Baud/s.
*   \param[in]  arbTseg1          Bit timing configuration for arbitration phase (TSEG1).
*   \param[in]  arbTseg2          Bit timing configuration for arbitration phase (TSEG2).
*   \param[in]  arbSjw            Bit timing configuration for arbitration phase (SJW).
*   \param[in]  arbSam            Samples for arbitration phase (0: one sample, 1: three samples).
*   \param[in]  arbFlags          Flags for arbitration arbitration phase (normal or silent mode).
*   \param[in]  dbrBaudrate       Baud-rate for data phase in Baud/s.
*   \param[in]  dbrTseg1          Bit timing configuration for data phase (TSEG1).
*   \param[in]  dbrTseg2          Bit timing configuration for data phase (TSEG2).
*   \param[in]  dbrSjw            Bit timing configuration for data phase (SJW).
*   \param[in]  dbrSam            Samples for data phase (0: one sample, 1: three samples).
*   \param[in]  dbrFlags          Flags for data arbitration phase:
*                                 Bit 0:  CANOEAPI_CAN_NORMALMODE  normal mode, acknowledge is created.
*                                         CANOEAPI_CAN_SILENTMODE  silent mode, acknowledge is not created.
*                                 Bits 1-31:  unused, must be set to zero.
*  \context     Called by CAN driver.
*  \notes       The function performs an automatic reset of the CAN controller.
**********************************************************************************************************************/
void VttCan_SetCanConfiguration(uint8 channel,
                                         double arbBaudrate, uint8 arbTseg1, uint8 arbTseg2,
                                         uint8 arbSjw, uint8 arbSam, uint32 arbFlags,
                                         double dbrBaudrate, uint8 dbrTseg1, uint8 dbrTseg2,
                                         uint8 dbrSjw, uint8 dbrSam, uint32 dbrFlags)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_SetCanConfiguration(channel, arbBaudrate, arbTseg1, arbTseg2, arbSjw, arbSam, arbFlags,
    dbrBaudrate, dbrTseg1, dbrTseg2, dbrSjw, dbrSam, dbrFlags);

}   /* VttCan_SetCanConfiguration() */

/***********************************************************************************************************************
*  VttCan_AtomicBegin()
**********************************************************************************************************************/
/*! \brief     Enters exclusive area of CANoeEmu, used to ensure mutual exclusion between MSR stack and CANoe.
**********************************************************************************************************************/
void VttCan_AtomicBegin(void)
{
  CANoeAPI_AtomicBegin();

}   /* VttCan_SetCanConfiguration() */

/***********************************************************************************************************************
*  VttCan_AtomicEnd()
**********************************************************************************************************************/
/*! \brief     Leaves exclusive area of CANoeEmu, used to ensure mutual exclusion between MSR stack and CANoe.
**********************************************************************************************************************/
void VttCan_AtomicEnd(void)
{
  CANoeAPI_AtomicEnd();

}   /* VttCan_AtomicEnd() */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Can.h
*********************************************************************************************************************/
