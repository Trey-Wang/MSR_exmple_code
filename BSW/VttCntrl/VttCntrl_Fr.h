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
/**      \file    VTTCntrl_Fr.h
*        \brief   Declares the public API of the virtual FlexRay controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual FlexRay controller that enables to send and receive FlexRay frames.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  AUTHOR IDENTITY
*  -------------------------------------------------------------------------------------------------------------------
*  Name                          Initials      Company
*  -------------------------------------------------------------------------------------------------------------------
*  Max-Ferdinand Suffel           vismsl       Vector Informatik GmbH
*  Damian Philipp                 visdap       Vector Informatik GmbH
*  -------------------------------------------------------------------------------------------------------------------
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Version   Date        Author  Change Id     Description
*  -------------------------------------------------------------------------------------------------------------------
*  03.01.00  2015-11-03  visdap  -             Initial version extracted from VTTCntrl generator.
*            2015-12-01  vismsl                Revised Flexray timer handling and reset behaviour.
*            2015-12-18  vismsl  -             Interrupts are always unmasked when measurement starts due to CANoeEmu.
*  03.01.01  2016-01-18  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*  03.01.02  2016-02-11  vismsl  FEAT-1687     Included header file VttCntrl_Fr_Cfg.h, needed for PB-Support.
*  03.01.03  2017-02-03  visjth  ESCAN00093766 Ignore POC states by "Coldstart Helper" in VN box
*********************************************************************************************************************/

#if !defined (VTTCNTRL_FR_H)
#define VTTCNTRL_FR_H

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Fr_Cfg.h"

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTFR_SW_MAJOR_VERSION    (3u)
#define VTTFR_SW_MINOR_VERSION    (2u)
#define VTTFR_SW_PATCH_VERSION    (0u)

/*! \brief Irq Status lines of FlexRay controller */
# define VTTFR_STATUS_IRQ_LINE_0  (0)
# define VTTFR_STATUS_IRQ_LINE_1  (1)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

#define VttFr_AtomicBegin()    CANoeAPI_AtomicBegin()
#define VttFr_AtomicEnd()      CANoeAPI_AtomicEnd()

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttFr_AbsoluteTimer(uint8 channelId, uint32 offset, uint8 cycle, uint32 macroticksPerCycle, uint32 macrotickLength);

void VttFr_RegisterFlexRayMessage(uint8 channel, uint32 channelABMask, uint32 slotId, uint32 baseCycle,
                                                                uint32 cycleRepetition, uint32 flags, uint16 payloadLen);

void VttFr_SendFlexRayMessage(uint8 channel, uint32 channelABMask, uint32 slotId, uint32 baseCycle, 
                                            uint32 cycleRepetition, uint32 flags, uint16 payloadLen, const uint8 data[]);

void VttFr_SetFlexRayMode(uint8 channel, uint32 channelMask, uint32 mode);
void VttFr_ConfigureInterrupts(uint8 enable);

void VttFr_ResetFlexRayCC (uint8 channel);
void VttFr_ResetFlexRayCCAndSendWUP(uint8 channel, int wuChMask, uint16 wuCount, uint16 wuTxIdle, uint16 wuTxLow,
                                                                                                              char* cfg);

void VttFr_SetFlexRayPOCState (uint8 channel, sint32 ccNo, sint32 pocState);
void VttFr_SetFlexRayKeySlot(uint8 channel, uint32 channelMask, uint32 keySlotIndex, uint32 slotId, uint32 keySlotUsage);
void VttFr_SetRequestedFlexRaySlotIDs(uint32 channelId, uint32 numOfSlotNumbers, uint16 * slotNumbers);

unsigned long long VttFr_GetCurrentSimTime(void);
uint8 VttFr_GetSimulationMode(void);
void VttFr_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_FR_H */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Fr.h
*********************************************************************************************************************/
