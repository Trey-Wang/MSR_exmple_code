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
/**      \file    VTTCntrl_Gpt.h
*        \brief   Declares the public API of the virtual GPT controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual GPT controller that enables to setup general purpose timers in continous or one shot mode.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel          vismsl        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-24  vismsl  -             Initial version extracted from VTTCntrl generator.
 *  03.00.04  2015-09-22  visdap  -             ESCAN00085394: Added pragmas to protect VTTCntrl during simulated
 *                                              ECU reset
 *  04.00.00  2015-11-23  vismsl  -             Revised timer management. Refactored implementation.
 *  04.00.01  2015-01-18  vismsl  ESCAN00087527 Channels are automatically stopped/restarted independent of 
 *                                              wakeup-functionality enabled or not.
 *  04.00.02  2016-01-18  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *********************************************************************************************************************/

#if !defined (VTTCNTRL_GPT_H)
#define VTTCNTRL_GPT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Gpt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
# define VTTGPT_SW_MAJOR_VERSION (4u)
# define VTTGPT_SW_MINOR_VERSION (0u)
# define VTTGPT_SW_PATCH_VERSION (2u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/*! \brief Internal GPT controller structure associated with a GPT channel. */
typedef struct
{
  sint32 irq;         /* IRQ of the interrupt service routine triggered when the GPT channel timeouts. */
  uint8 handle;       /* CANoe user timer handle identified with the timer that implements the GPT channel. */
  const char *name;   /* Name of the CANoe user timer implementing the GPT channel. */
} VttGpt_CntrlType;

/*! \brief GPT channel modes, either continuous or one-shot. */
typedef enum
{
  VTTGPT_CH_MODE_NONE = 0,
  VTTGPT_CH_MODE_CONTINUOUS,  /* ISR associated with the GPT channel is trigger periodically. */
  VTTGPT_CH_MODE_ONESHOT      /* ISR associated with the GPT channel is triggered only once. */
} VttGpt_ChannelModeType;

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/
   
void VttGpt_StartTimer(uint8 index, uint32 microSeconds, VttGpt_ChannelModeType Mode);
void VttGpt_StopTimer(uint8 index);
uint32 VttGpt_GetRemainingTime(uint8 index);

void VttGpt_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_GPT_H */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Gpt.h
*********************************************************************************************************************/
