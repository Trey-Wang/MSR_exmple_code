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
/**        \file  VTTCntrl_Pwm.h
 *        \brief  Declares the public API of the virtual PWM controller integrated in the vVIRTUALtarget.
 *
 *      \details  Virtual PWM controller that produces pulse-width-modulated signals with variable duty cycle.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp                 visdap       Vector Informatik GmbH
 *  Max-Ferdinand Suffel           vismsl       Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-07-07  visdap  -             Initial version extracted from VTTCntrl generator.
 *  03.00.01  2015-07-27  vismsl  -             Component version number changed.
 *  03.00.04  2015-09-22  visdap  ESCAN00085394 Added pragmas to protect VTTCntrl during simulated ECU reset.
 *  03.00.05  2015-12-18  vismsl  -       Incremented version number since VTTCntrl_Base changed.
 *  03.00.06  2016-01-25  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *********************************************************************************************************************/

#if !defined(VTTCNTRL_PWM_H)
# define VTTCNTRL_PWM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "Pwm.h"
#include "VttCntrl_Base.h"
#include "VTTCntrl_Pwm_Cfg.h"

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTPWM_SW_MAJOR_VERSION (3u)
#define VTTPWM_SW_MINOR_VERSION (0u)
#define VTTPWM_SW_PATCH_VERSION (6u)

#define VTTPWM_FALSE  0
#define VTTPWM_TRUE   1

#define VTTPWM_NEWPERIOD_NO_UPDATE     (0xFFffFFff)
#define VTTPWM_NEWDUTYCYCLE_NO_UPDATE   (0xFFff)

#define VTTPWM_DUTYCYCLE_MAXVAL       (0x8000)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint16 VttPwm_Value_Type;
typedef VttPwm_Value_Type (*VttPwm_Value_Function)(uint16 dutycycle, uint32 period, uint8 outputEnable, 
  Pwm_OutputStateType polarity);
typedef uint32 VttPwm_Frequency_Type;

/*! \brief Internal PWM controller structure associated with a PWM channel. */
typedef struct
{
  const char* ChName;        /* Name of the PWM channel. */
  uint8 ChIntEnabled;        /* Flag to indicate whether notifications are enabled for this channel 
                   * (0 == no notifications). */
  uint8 ChOutputEnabled;      /* Flag to indicate whether the pin driver is enabled 
                   * (i.e., pin outputs a voltage level). */
  Pwm_EdgeNotificationType ChIntTriggerEdge;    /* Indicated on which edge the notification should occur. */
  sint32 ChSysVarHandle;      /* Handle of the CANoeEmu system variable that implements the PWM channel. */
  uint8 ChTimerHandle;        /* Handle of the CANoeEmu user timer that is used to implement the duty cycle. */
  long long ChStartTime;      /* Time at which the PWM was last started or modified. */
  uint16 ChDutyCycle;        /* Current Duty cycle in percent: 0x0..0x8000 */
  Pwm_PeriodType ChPeriod;      /* Period of the duty cycle in microseconds [us]. */
  Pwm_OutputStateType ChPolarity;  /* Polarity of the PWM signal. */
  uint16 ChNewDutyCycle;      /* Duty cycle in percent: 0x0..0x8000 to be set at start of next period. */
  Pwm_PeriodType ChNewPeriod;    /* Period of the duty cycle in microseconds [us] to be set when current period
                    finishes. */
  long long ChNewPeriodNumber;    /* Period number in which the last update was submitted. */
  sint32 ChIsrNr;          /* IRQ that is associated with this PWM channel. */
  VttPwm_Value_Function ChValueFunction;   /* Function used by CANoeEmu to compute the actual PWM signal
                        of the channel. */
} VttPwm_CntrlType;

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttPwm_StartPwmGenerator(Pwm_ChannelType channel, uint16 dutycycle, Pwm_PeriodType period, 
  Pwm_OutputStateType polarity);
void VttPwm_StopPwmGenerator(Pwm_ChannelType channel, Pwm_OutputStateType idlestate);

void VttPwm_UpdateDutycycle(Pwm_ChannelType channel, uint16 dutycycle, uint8 immediate);
void VttPwm_UpdateDutycycleAndPeriod(Pwm_ChannelType channel, uint16 dutycycle, Pwm_PeriodType period, uint8 immediate);

void VttPwm_DisableOutput(Pwm_ChannelType channel, Pwm_OutputStateType idlestate);

void VttPwm_EnableInterrupt(Pwm_ChannelType channel, Pwm_EdgeNotificationType edge);
void VttPwm_DisableInterrupt(Pwm_ChannelType channel);

Pwm_OutputStateType VttPwm_GetPwmState(Pwm_ChannelType channel);

void VttPwm_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif /* VTTCNTRL_PWM_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Pwm.h
 *********************************************************************************************************************/
