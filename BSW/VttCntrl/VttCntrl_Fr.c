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
/**      \file    VTTCntrl_Fr.c
*        \brief   Implements the public API of the virtual FlexRay controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual FlexRay controller that enables to send and receive FlexRay frames.
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

#define VTTFR_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Fr.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the FlexRay controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Fr header file */
#if (  (VTTFR_SW_MAJOR_VERSION != (3u)) \
    || (VTTFR_SW_MINOR_VERSION != (2u)) \
    || (VTTFR_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of VttCntrl_Fr.c and VttCntrl_Fr.h are inconsistent"
#endif

/* Check the version of VttCntrl_Fr config file */
#if (  (VTTFR_CFG_MAJOR_VERSION != (1u)) \
    || (VTTFR_CFG_MINOR_VERSION != (0u)) \
    || (VTTFR_CFG_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of VttCntrl_Fr.c and VttCntrl_Fr_Cfg.h are inconsistent"
#endif

/* Check the version of VttCntrl_Base header file */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Fr.c and VttCntrl_Base.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/*! \brief Name of FlexRay timers */
#define VTTFR_TIMER0_NAME    "VttFr_Timer0"
#ifdef FR_USED_CONTROLLER_1
# define VTTFR_TIMER1_NAME    "VttFr_Timer1"
#endif
/*! \brief Offset of IRQ numbers in os.h for FlexRay controller interrupts. */
#define VTTFR_IRQ_OFFSET    IRQ_Fr_IrqLine0
/*! \brief Absolute FlexRay timer timeouts (in milliseconds). */

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

/*! \brief Internal FlexRay controller structure. */
typedef struct
{
  sint32 IsrLine0_Nr;
  sint32 IsrTimer0_Nr;
} VttFr_CntrlType;

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttFr_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal FlexRay controller state. */
STATIC VttFr_CntrlType VttFr_Cntrl[FR_NUM_CTRL_USED] =
{
  {0, 0 }
#ifdef FR_USED_CONTROLLER_1
  , {0, 0 }
#endif
};
/*! \brief Internal FlexRay controller state. */
STATIC const uint8* VttFr_TimerName[FR_NUM_CTRL_USED] =
{
  VTTFR_TIMER0_NAME
#ifdef FR_USED_CONTROLLER_1
  , VTTFR_TIMER1_NAME
#endif
};
/*! \brief Index of the CANoeEmu user timer identified with the absolute FlexRay timer. */
STATIC uint8 VttFr_UserTimerIndex[FR_NUM_CTRL_USED] =
{
  CANOEAPI_USERTIMER_INVALID
#ifdef FR_USED_CONTROLLER_1
  , CANOEAPI_USERTIMER_INVALID
#endif
};
/*! \brief Current absolute time of the FlexRay bus in nanoseconds [ns]. */
STATIC long long VttFr_CurrentAbsoluteTimer[FR_NUM_CTRL_USED] =
{
  0
#ifdef FR_USED_CONTROLLER_1
  , 0
#endif
};
/*! \brief Current offset for scheduling a FlexRay job in nanoseconds [ns]. */
STATIC long long VttFr_OffsetInNanoseconds[FR_NUM_CTRL_USED] =
{
  0
#ifdef FR_USED_CONTROLLER_1
  , 0
#endif
};
/*! \brief Current FlexRay cycle number. */
STATIC uint16 VttFr_CurrentCycle[FR_NUM_CTRL_USED] =
{
  0
#ifdef FR_USED_CONTROLLER_1
  , 0
#endif
};


/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/*! \brief Name of the FlexRay bus. */
extern const char * VttFr_BusNames[];
/*! \brief Irq Status lines of FlexRay controllers */
extern const uint8 VttFr_StatusLines[];

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttFr_OnFlexRayFrameHandler(uint8 channelId, uint32 channelABMask, uint32 slotId, uint16 cycle,
                                                  sint16 segment, sint32 flags, uint16 payloadLen, const uint8 data[]);
STATIC void VttFr_OnFlexRayStartCycleHandler(uint8  channelId, uint16 cycle, long long time);
STATIC void VttFr_OnFlexRayNmVectorHandler(uint8  channelId, uint16 len, const  uint8 nmVec[12]);
STATIC void VttFr_OnFlexRayStatusHandler(uint8  channelId, uint32 state);
STATIC void VttFr_OnFlexRaySlotHandler(uint8 channelId, uint32 slotId, uint16  cycle);
STATIC void VttFr_OnFlexRayPOCStateHandler(uint8 channelId,  uint32 pocState, uint32 Fr_Info1, uint32 Fr_Info2);
STATIC void VttFr_OnFlexRayTimerHandler(uint8 index, const char* name);

STATIC void VttFr_MapChannel(void);
STATIC void VttFr_AssignIRQ(void);
STATIC void VttFr_RegisterHandlers(void);
STATIC void VttFr_InitTimer(void);
STATIC void VttFr_DeinitTimer(void);
STATIC void VttFr_StopTimer(void);
STATIC void VttFr_Deinit(void);

STATIC void VttFr_OnLoad(void);
STATIC void VttFr_OnStartMeasurement(void);
STATIC void VttFr_OnReset(void);
STATIC void VttFr_OnSwitchOff(void);
STATIC void VttFr_OnStopMeasurement(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  VttFr_OnFlexRayFrameHandler()
**********************************************************************************************************************/
/*! \brief     Processes a frame received from the FlexRay bus.
*
*  \param[in]  channelId      Number of the FlexRay cluster.
*  \param[in]  channelABMask  1 = Frame was received over channel A, 
*                             2 = Frame was received over channel B or 
*                             3 = Frame was received simultaneously on both channels A and B.
*  \param[in]  slotId         Frame slot number within the cycle.
*  \param[in]  cycle          Cycle number.
*  \param[in]  segment        0 = static frame, 1 = dynamic frame.
*  \param[in]  flags          Frame status flag:
*                             - Sync Frame:      (0x01)
*                             - Startup Frame:    (0x02)
*                             - Event Driven Frame:  (0x10)
*                             - Null Frame Indicator: (0x20)
*  \param[in]  payloadLen     Length of the frame's payload in number of bytes.
*  \param[in]  data           Pointer to byte array providing the frame's payload.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayFrameHandler(uint8 channelId, uint32 channelABMask, uint32 slotId, uint16 cycle, 
                                                  sint16 segment, sint32 flags, uint16 payloadLen, const uint8 data[])
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    Fr_OnFlexRayFrameHandler(channelId, channelABMask, slotId, cycle, segment, flags, payloadLen, data);
  }

} /* VttFr_OnFlexRayFrameHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRayStartCycleHandler()
**********************************************************************************************************************/
/*! \brief     Processes a start cycle event caused by the FlexRay bus.
*
*  \param[in]  channelId      Number of the FlexRay cluster.
*  \param[in]  cycle          Current value of the cycle counter.
*  \param[in]  time           Timestamp of the FlexRay frame in nanoseconds [ns].
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayStartCycleHandler(uint8 channelId, uint16 cycle, long long time)
{
  /* ----- Local Variables ---------------------------------------------- */
  long long CurrentSimTime = CANoeAPI_UserTimer_CurrentTime();
  (void)time;

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    Fr_OnFlexRayStartCycleHandler(channelId, cycle);
    VttFr_CurrentCycle[channelId] = cycle;

    VttCntrl_Base_SetCheckedInterrupt(VttFr_Cntrl[channelId].IsrLine0_Nr);
  }

  VttFr_CurrentAbsoluteTimer[channelId] = CurrentSimTime;

  if (VttFr_OffsetInNanoseconds != 0)
  {
      CANoeAPI_UserTimer_SetAbsolute(VttFr_UserTimerIndex[channelId],
      VttFr_CurrentAbsoluteTimer[channelId] + VttFr_OffsetInNanoseconds[channelId]);
  }

} /* VttFr_OnFlexRayStartCycleHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRayNmVectorHandler()
**********************************************************************************************************************/
/*! \brief     Processes a network management event caused by the FlexRay bus.
*
*  \param[in]  channelId      Number of the FlexRay cluster.
*  \param[in]  len            Length of the network management vector in number of bytes.
*  \param[in]  nmVec          Pointer to the byte array providing the network management vector.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayNmVectorHandler(uint8 channelId, uint16 len, const uint8 nmVec[12])
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    Fr_OnFlexRayNmVectorHandler(channelId, len, nmVec);
  }

} /* VttFr_OnFlexRayNmVectorHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRayStatusHandler()
**********************************************************************************************************************/
/*! \brief     Processes a status change event from the FlexRay controller.
*
*  \param[in]  channelId    Number of the FlexRay cluster.
*  \param[in]  state        0 = Not synchronized with the bus, passive,
*                           1 = Synchronized with the bus, active,
*                           2 = No synchronized with the bus,
*                           3 = Unknown state.
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayStatusHandler(uint8 channelId, uint32 state)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    Fr_OnFlexRayStatusHandler(channelId, state);
  }

} /* VttFr_OnFlexRayStatusHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRaySlotHandler()
**********************************************************************************************************************/
/*! \brief     Processes a slot update event caused by the FlexRay bus.
*
*  \param[in]  channelId      Number of the FlexRay cluster.
*  \param[in]  slotId         Current slot number within the cycle.
*  \param[in]  cycle          Current cycle number.
*
*  \context    Called by CANoeEmu.
*  \note       Currently, the information provided is ignored, the FlexRay driver tracks the information by itself.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRaySlotHandler(uint8 channelId, uint32 slotId, uint16 cycle)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)slotId;
  (void)cycle;

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    /* Currently, the information is not needed by FlexRay driver. */
  }

} /* VttFr_OnFlexRaySlotHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRayPOCStateHandler()
**********************************************************************************************************************/
/*! \brief     Processes a protocol operation control (POC) state change event from the FlexRay controller.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*  \param[in]  pocState         Next POC state, defined as:
*                    0 = Config
*                    1 = Default_Config
*                    2 = Halt
*                    3 = Normal_Active
*                    4 = Normal_Passiv
*                    5 = Ready
*                    6 = Startup
*                    7 = Wakeup
*                      
*  \param[in]  Fr_Info1         Controllers synchronization status:
*                    0 = Synchronized, but passive
*                    1 = Synchronized, and active
*                    2 = Not synchronized
*                    3 = Synchronization status unknown
*
*  \param[in]  Fr_Info2         Controllers wakeup status:
*                    0 = Wakeup undefined
*                    1 = Wakeup received, header  
*                    2 = Wakeup received, WUP
*                    3 = Wakeup collision, header
*                    4 = Wakeup collision, WUP
*                    5 = Wakeup collision, unknown
*                    6 = Wakeup transmitted
*                    7 = Wakeup received
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayPOCStateHandler(uint8 channelId, uint32 pocState, uint32 Fr_Info1, uint32 Fr_Info2)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  
  /* Ignore POC states by "Coldstart Helper" in VN box */
  if(pocState >= 254)
  {
    return;
  }
  
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    Fr_OnFlexRayPOCStateHandler(channelId, pocState, Fr_Info1, Fr_Info2);
  }

} /* VttFr_OnFlexRayPOCStateHandler() */

/**********************************************************************************************************************
*  VttFr_OnFlexRayTimerHandler()
**********************************************************************************************************************/
/*! \brief     Processes a timeout when the FlexRay timer expires.
*  \details    On timeout a interrupt is triggered such that the FlexRay driver processes the event.
*
*  \param[in]  index    CANoe user timer index identified with the FlexRay timer.
*  \param[in]  name     Name of the FlexRay timer (currently ignored).
*
*  \context    Called by CANoeEmu.
*********************************************************************************************************************/
STATIC void VttFr_OnFlexRayTimerHandler(uint8 index, const char *name)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;
  (void)name;

  /* ----- Implementation ----------------------------------------------- */
  if (VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
    {
      if (VttFr_UserTimerIndex[ctrl_id] == index)
      {
        VttCntrl_Base_SetCheckedInterrupt(VttFr_Cntrl[ctrl_id].IsrTimer0_Nr);
        break;
      }
    }
  }

} /* VttFr_OnFlexRayTimerHandler() */

/***********************************************************************************************************************
*  VttFr_AssignIRQ()
**********************************************************************************************************************/
/*! \brief     Assigns the FlexRay controller the IRQs required for interrupt-based processing of hardware events.
*   \context   Called by VttFr_OnLoad.
**********************************************************************************************************************/
STATIC void VttFr_AssignIRQ(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    /* ----- Implementation ----------------------------------------------- */
    VttFr_Cntrl[ctrl_id].IsrLine0_Nr = VTTFR_IRQ_OFFSET + VttFr_StatusLines[ctrl_id] * 2 + ctrl_id * 3;
    VttFr_Cntrl[ctrl_id].IsrTimer0_Nr = VTTFR_IRQ_OFFSET + 1 + ctrl_id * 3;
  }

} /* VttFr_AssignIRQ() */

/***********************************************************************************************************************
*  VttFr_MapChannel()
**********************************************************************************************************************/
/*! \brief     Maps the FlexRay channel (here, cluster) number to the configured the FlexRay bus provided by CANoeEmu.
*   \context   Called by VttFr_OnLoad.
**********************************************************************************************************************/
STATIC void VttFr_MapChannel(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    /* ----- Implementation ----------------------------------------------- */
    CANoeAPI_MapChannel(VttFr_BusNames[ctrl_id], CANOEAPI_BUSTYPE_FLEXRAY, ctrl_id);
  }

} /* VttFr_MapChannel() */

/***********************************************************************************************************************
*  VttFr_RegisterHandlers
**********************************************************************************************************************/
/*! \brief     Registers the hardware event handlers at CANoeEmu for processing the FlexRay communication.
*   \context   Called by VttFr_OnStart.
**********************************************************************************************************************/
STATIC void VttFr_RegisterHandlers(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_SetFlexRayFrameHandler(&VttFr_OnFlexRayFrameHandler);
  CANoeAPI_SetFlexRayCycleHandler(&VttFr_OnFlexRayStartCycleHandler, &VttFr_OnFlexRayNmVectorHandler);
  CANoeAPI_SetFlexRayStatusHandler(&VttFr_OnFlexRayStatusHandler);
  CANoeAPI_SetFlexRayPOCStateHandler(&VttFr_OnFlexRayPOCStateHandler);
  CANoeAPI_SetFlexRaySlotHandler(&VttFr_OnFlexRaySlotHandler);

} /* VttFr_RegisterHandlers() */

/***********************************************************************************************************************
*  VttFr_InitTimer
**********************************************************************************************************************/
/*! \brief     Allocates and initializes the FlexRay timer.
*   \context   Called by VttFr_OnStart.
**********************************************************************************************************************/
STATIC void VttFr_InitTimer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  /* ----- Implementation ----------------------------------------------- */
  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    VttFr_UserTimerIndex[ctrl_id] = CANoeAPI_UserTimer_Create(VttFr_TimerName[ctrl_id]);

    if (VttFr_UserTimerIndex[ctrl_id] == CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_WriteString("VTT FR Controller: Timer could not be created!");
    }
    else
    {
      CANoeAPI_UserTimer_SetCallbackFunction(VttFr_UserTimerIndex[ctrl_id], &VttFr_OnFlexRayTimerHandler);
    }
  }

} /* VttFr_InitTimer() */

/***********************************************************************************************************************
*  VttFr_StopTimer
**********************************************************************************************************************/
/*! \brief     Stops the FlexRay timer.
*   \context   Called by VttFr_OnReset and VttFr_OnSwitchOff.
**********************************************************************************************************************/
STATIC void VttFr_StopTimer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  /* ----- Implementation ----------------------------------------------- */
  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    if (VttFr_UserTimerIndex[ctrl_id] != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Cancel(VttFr_UserTimerIndex[ctrl_id]);
    }
  }
} /* VttFr_StopTimer() */

/***********************************************************************************************************************
*  VttFr_DeinitTimer
**********************************************************************************************************************/
/*! \brief     Frees the allocated FlexRay timer.
*   \context   Called by VttFr_OnStopMeasurement.
**********************************************************************************************************************/
STATIC void VttFr_DeinitTimer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  /* ----- Implementation ----------------------------------------------- */
  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    if (VttFr_UserTimerIndex[ctrl_id] != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Destroy(VttFr_UserTimerIndex[ctrl_id]);
      VttFr_UserTimerIndex[ctrl_id] = CANOEAPI_USERTIMER_INVALID;
    }
  }
} /* VttFr_DeinitTimer() */

/***********************************************************************************************************************
*  VttFr_Deinit
**********************************************************************************************************************/
/*! \brief     Deinitializes the internal VTT Fr control data.
*  \return     no return value
*  \context    Called by VttFr_OnLoad and VttFr_OnStopMeasurement.
**********************************************************************************************************************/
STATIC void VttFr_Deinit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrl_id;

  /* ----- Implementation ----------------------------------------------- */
  for (ctrl_id = 0; ctrl_id < FR_NUM_CTRL_USED; ctrl_id++)
  {
    /* ----- Implementation ----------------------------------------------- */
    VttFr_UserTimerIndex[ctrl_id] = CANOEAPI_USERTIMER_INVALID;
    VttFr_CurrentAbsoluteTimer[ctrl_id] = 0;
    VttFr_OffsetInNanoseconds[ctrl_id] = 0;
    VttFr_CurrentCycle[ctrl_id] = 0;
  }
} /* VttFr_Deinit() */

/***********************************************************************************************************************
*  VttFr_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual FlexRay controller.
*
*         Tasks:
*            - De-initializes the internal FlexRay controller state first for a clean startup.
*            - Maps the FlexRay channel (here, cluster) number to the configured FlexRay bus name in CANoe.
*            - Assigns the IRQ numbers for interrupt processing to the FlexRay controller.
*
*  \context    Called by VttFr_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttFr_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFr_Deinit();
  VttFr_MapChannel();
  VttFr_AssignIRQ();

} /* VttFr_OnLoad() */

/***********************************************************************************************************************
*  VttFr_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual FlexRay controller.
*
*         Tasks:
*            - Registers the hardware event handlers at CANoeEmu.
*            - Initializes the FlexRay timer.
*
*  \context    Called by VttFr_OnStateChange handler immediately before simulation is started.
**********************************************************************************************************************/
STATIC void VttFr_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFr_RegisterHandlers();
  VttFr_InitTimer();
} /* VttFr_OnStart() */

/***********************************************************************************************************************
*  VttFr_OnReset
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual FlexRay controller.
*
*         Tasks:
*            - Stops the FlexRay timer.
*
*  \context    Called by VttFr_OnStateChange handler right before when ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttFr_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFr_StopTimer();
} /* VttFr_OnReset() */

/***********************************************************************************************************************
*  VttFr_OnSwitchOff
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual FlexRay controller.
*
*         Tasks:
*            - Stops the FlexRay timer.
*
*  \context    Called by VttFr_OnStateChange handler right before ECU power supply is switched off.
**********************************************************************************************************************/
STATIC void VttFr_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFr_StopTimer();
} /* VttFr_OnSwitchOff() */

/***********************************************************************************************************************
*  VttFr_OnStopMeasurement
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual FlexRay controller.
*
*         Tasks:
*            - De-initializes the FlexRay timer.
*            - De-initializes the internal FlexRay controller state.
*
*  \context    Called by VttFr_OnStateChange handler right before simulation is stopped.
**********************************************************************************************************************/
STATIC void VttFr_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttFr_DeinitTimer();
  VttFr_Deinit();
} /* VttFr_OnStopMeasurement() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttFr_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual FlexRay controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttFr_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttFr_EcuState = newState;

  switch (action)
  {
    case CANOEAPI_ECUACTION_LOAD:
    {
      VttFr_OnLoad();
    }
    break;
    case CANOEAPI_ECUACTION_INITMEASUREMENT:
    {
      FlexRay_Init();
    }
    break;
    case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttFr_OnStartMeasurement();
    }
    break;
    case CANOEAPI_ECUACTION_RESET:
    {
      VttFr_OnReset();
    }
    break;
    case CANOEAPI_ECUACTION_SWITCHON:
    break;
    case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
    case CANOEAPI_ECUACTION_WAKEUP:
    break;
    case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttFr_OnSwitchOff();
    }
    break;
    case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttFr_OnStopMeasurement();
    }
    break;
    case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

} /* VttFr_OnStateChange() */


/***********************************************************************************************************************
*  VttFr_AbsoluteTimer
**********************************************************************************************************************/
/*! \brief     Sets the release time when the absolute FlexRay timer shall schedule the next FlexRay job.
 *
 *  \param[in]  channelId           Number of the FlexRay cluster.
 *  \param[in]  offset              Offset in nanoseconds [ns] from the start of the cycle, the job is scheduled.        
 *  \param[in]  cycle               The cycle number, the job is scheduled.
 *  \param[in]  macroticksPerCycle  Number of macrotricks a cycle shall comprise. (Currently not used).
 *  \param[in]  macrotickLength     Duration of a single macrotick in nanoseconds [ns]. (Currently not used).
 *
 *  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_AbsoluteTimer(uint8 channelId, uint32 offset, uint8 cycle, uint32 macroticksPerCycle, uint32 macrotickLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  int ret = CANOEAPI_USERTIMER_SET;
  (void)macroticksPerCycle;
  (void)macrotickLength;

  /* ----- Implementation ----------------------------------------------- */
  if ((63 == VttFr_CurrentCycle[channelId] && 0 == cycle) || (cycle > VttFr_CurrentCycle[channelId]))
  {
    VttFr_OffsetInNanoseconds[channelId] = offset;
  }
  else
  {
    ret = CANoeAPI_UserTimer_SetAbsolute(VttFr_UserTimerIndex[channelId], VttFr_CurrentAbsoluteTimer[channelId] + offset);
    VttFr_OffsetInNanoseconds[channelId] = 0;
  }
  
  if(CANOEAPI_USERTIMER_ALREADYEXPIRED == ret)
  {
    /* Absolute time already passed --> Request Interrupt NOW */
    if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
    {
      VttCntrl_Base_SetCheckedInterrupt(VttFr_Cntrl[channelId].IsrTimer0_Nr);
    }
  }

} /* VttFr_AbsoluteTimer() */

/***********************************************************************************************************************
*  VttFr_SendFlexRayMessage
**********************************************************************************************************************/
/*! \brief     Sends a frame over the FlexRay bus.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*  \param[in]  channelABMask      1 = Frame will be send over channel A,
*                    2 = Frame will be send over channel B or
*                    3 = Frame will be send simultaneously on both channels A and B.
*  \param[in]  slotId          Frame slot number within the cycle, the frame shall be send in.
*  \param[in]  baseCycle        Number of the cycle, the FlexRay controller will start sending the frame.
*  \param[in]  cycleRepetition      Repetition factor for periodic transmission. Possible values: [1,2,4,8,16,32,64].
*  \param[in]  flags          Frame status flag:
*                      - Sync Frame:      (0x01)
*                      - Startup Frame:    (0x02)
*                      - Event Driven Frame:  (0x10)
*                      - Null Frame Indicator: (0x20)
*  \param[in]  payloadLen        Length of the frame's payload in number of bytes.
*  \param[in]  data            Pointer to byte array providing the frame's payload.
*  
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_SendFlexRayMessage(uint8 channelId, uint32 channelABMask, uint32 slotId, uint32 baseCycle, 
                                            uint32 cycleRepetition, uint32 flags, uint16 payloadLen, const uint8 data[])
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
      CANoeAPI_SendFlexRayMessage(channelId, channelABMask, slotId, baseCycle, cycleRepetition, flags, payloadLen, data);
  }
} /* VttFr_SendFlexRayMessage() */

/***********************************************************************************************************************
*  VttFr_RegisterFlexRayMessage
**********************************************************************************************************************/
/*! \brief     Registers a frame for transmission over the FlexRay bus.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*  \param[in]  channelABMask      1 = Frame will be send over channel A,
*                    2 = Frame will be send over channel B or
*                    3 = Frame will be send simultaneously on both channels A and B.
*  \param[in]  slotId          Frame slot number within the cycle, the frame shall be send in.
*  \param[in]  baseCycle        Number of the cycle, the FlexRay controller will start sending the frame.
*  \param[in]  cycleRepetition      Repetition factor [1,2,4,8,16,32,64] for periodic transmission.
*  \param[in]  flags          Frame status flag:
*                      - Sync Frame:      (0x01)
*                      - Startup Frame:    (0x02)
*                      - Event Driven Frame:  (0x10)
*                      - Null Frame Indicator: (0x20)
*  \param[in]  payloadLen        Length of the frame's payload in number of bytes.
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_RegisterFlexRayMessage(uint8 channelId, uint32 channelABMask, uint32 slotId, uint32 baseCycle, 
                                                                uint32 cycleRepetition, uint32 flags, uint16 payloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if((VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING) || (VttFr_EcuState == CANOEAPI_ECUSTATE_PRESTART))
  {
    CANoeAPI_RegisterFlexRayMessage(channelId, channelABMask, slotId, baseCycle, cycleRepetition, flags, payloadLen);
  }
} /* VttFr_RegisterFlexRayMessage() */

/***********************************************************************************************************************
*  VttFr_ResetFlexRayCCAndSendWUP
**********************************************************************************************************************/
/*! \brief     Resets the FlexRay controller and sends a wakeup pattern.
*   \details   Initializes the FlexRay controller and generates the specified wakeup pattern before reintegration 
*              in the cluster or the startup.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*  \param[in]  wuChMask         1 = Wakeup pattern will be send over channel A,
*                               2 = Wakeup pattern will be send over channel B or
*                               3 = Wakeup pattern will be send simultaneously on both channels A and B.
*  \param[in]  wuCount          Number of repetitions [2-63] of the wakeup symbol in a wakeup pattern.
*  \param[in]  wuTxIdle         Number of idle bits in a wakeup symbol.
*  \param[in]  wuTxLow          Number of low bits in a wakeup symbol.
*  \param[in]  cfg              Pointer to a data array. (May set to NULL, parameter is internally ignored).
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_ResetFlexRayCCAndSendWUP(uint8 channelId, int wuChMask, uint16 wuCount, uint16 wuTxIdle, 
                                                                                              uint16 wuTxLow, char *cfg)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_ResetFlexRayCCAndSendWUP(channelId, wuChMask, wuCount, wuTxIdle, wuTxLow, cfg);
  }
} /* VttFr_ResetFlexRayCCAndSendWUP() */

/***********************************************************************************************************************
*  VttFr_ResetFlexRayCC
**********************************************************************************************************************/
/*! \brief    Resets the FlexRay controller.
*   \details  Initializes the FlexRay controller.
*
*  \param[in] channelId        Number of the FlexRay cluster.
*
*  \context   Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_ResetFlexRayCC(uint8 channelId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_ResetFlexRayCC(channelId);
  }
} /* VttFr_ResetFlexRayCC() */

/***********************************************************************************************************************
*  VttFr_SetFlexRayMode
**********************************************************************************************************************/
/*! \brief     Sets the operation mode of the FlexRay controller.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*
*  \param[in]  channelMask      1 = Set mode for channel A,
*                               2 = Set mode for channel B or
*                               3 = Set mode equally on both channels A and B.
*
*  \param[in]  mode             0 = Normal mode,
*                               1 = Sleep mode.
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_SetFlexRayMode(uint8 channelId, uint32 channelMask, uint32 mode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_SetFlexRayMode(channelId, channelMask, mode);
  }
} /* VttFr_SetFlexRayMode() */

/***********************************************************************************************************************
*  VttFr_SetFlexRayPOCState
**********************************************************************************************************************/
/*! \brief     Sets the protocol operation control (POC) state of the FlexRay controller.
*
*  \param[in]  channelId        Number of the FlexRay cluster.
*
*  \param[in]  ccNo             Type of communication controller (CC), defined as:
*                    0 = Architecture independent,
*                    1 = Reserved (for internal use only),
*                    2 = Cyclone I,
*                    3 = BUSDOCTOR,
*                    4 = Cyclone II,
*                    5 = VN Interface,
*                    6 = VN-Sync-Pulse (only in status event, for debugging purposes only).
*
*  \param[in]  pocState          Next POC state, defined as:
*                    0 = Config
*                    1 = Default_Config
*                    2 = Halt
*                    3 = Normal_Active
*                    4 = Normal_Passiv
*                    5 = Ready
*                    6 = Startup
*                    7 = Wakeup
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_SetFlexRayPOCState(uint8 channelId, sint32 ccNo, sint32 pocState)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_SetFlexRayPOCState(channelId, ccNo, pocState);
  }
} /* VttFr_SetFlexRayPOCState() */

/***********************************************************************************************************************
*  VttFr_SetFlexRayKeySlot
**********************************************************************************************************************/
/*! \brief    Sets the operation mode of the FlexRay controller.
*
*  \param[in]  channelId        Number of the FlexRay cluster.

*  \param[in]  channelMask      1 = Set key slot for channel A,
*                               2 = Set key slot for channel B or
*                               3 = Set key slot equally on both channels A and B.
*
*  \param[in]  keySlotIndex     Index of the key slot to be configured:
*                               1 = Key slot 1, 
*                               2 = Key slot 2.
*
*  \param[in]  slotId           Slot number of key slot.
*
*  \param[in]  keySlotUsage     Purpose of the key slot:
*                               0 = None,
*                               1 = Startup/Sync/LeadingColdstart,
*                               2 = Sync,
*                               3 = Startup/Sync.
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_SetFlexRayKeySlot(uint8 channelId, uint32 channelMask, uint32 keySlotIndex, uint32 slotId, 
                                                                                                   uint32 keySlotUsage)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_SetFlexRayKeySlot(channelId, channelMask, keySlotIndex, slotId, keySlotUsage);
  }
} /* VttFr_SetFlexRayKeySlot() */

/***********************************************************************************************************************
*  VttFr_SetRequestedFlexRaySlotIDs
**********************************************************************************************************************/
/*! \brief     Register the slot numbers for the slot event handler, refer to VttFr_OnFlexRaySlotHandler.
*
*  \param[in]  channelId          Number of the FlexRay cluster.
*  \param[in]  numOfSlotNumbers   Number of slots numbers to be registered.
*  \param[in]  slotNumbers        Array of slots numbers, length must equal numOfSlotNumbers.
*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
void VttFr_SetRequestedFlexRaySlotIDs(uint32 channelId, uint32 numOfSlotNumbers, uint16 * slotNumbers)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttFr_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_SetRequestedFlexRaySlotIDs(channelId, numOfSlotNumbers, slotNumbers);
  }
} /* VttFr_SetRequestedFlexRaySlotIDs() */

/***********************************************************************************************************************
*  VttFr_GetCurrentSimTime
**********************************************************************************************************************/
/*! \brief     Returns the current simulation time.
*  \return     Time expired since start of measurement in nanoseconds [ns].*
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
unsigned long long VttFr_GetCurrentSimTime(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return CANoeAPI_UserTimer_CurrentTime();
} /* VttFr_GetCurrentSimTime() */

/***********************************************************************************************************************
*  VttFr_GetSimulationMode
**********************************************************************************************************************/
/*! \brief     Returns the current CANoe simulation mode.
*  \return     0 - Unknown simulation mode.
*         1 - CANoe runs in simulation mode.
*         2 - CANoe runs in real mode, real network interface are attached: Rest bus simulation.
*  \context    Called by FlexRay driver.
**********************************************************************************************************************/
uint8 VttFr_GetSimulationMode(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return (uint8) CANoeAPI_GetSimulationMode();
} /* VttFr_GetSimulationMode() */

/*********************************************************************************************************************
*  END OF FILE: VttCntrl_Fr.c
*********************************************************************************************************************/
