/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Pwm_Irq.c
 *    Component:  DrvPwm_VttCanoe01Asr
 *       Module:  DrvPwm
 *    Generator:  -
 *
 *   Description: Module for Pulse Width Modulation
 *
 *********************************************************************************************************************/

#define PWM_IRQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Pwm.h"
#include "Os.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (  (PWM_SW_MAJOR_VERSION != (2U)) \
    || (PWM_SW_MINOR_VERSION != (5U)) )
    #error "Version numbers of Pwm_Irq.c and Pwm.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  FUNCTION PROTOTYPE OF PWM ISR HANDLER
 **********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_IsrHandler (Pwm_InterruptSourceType Irq_Source);


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
ISR (PwmIsr_0)
{
  Pwm_IsrHandler(0);
  return;
}

ISR (PwmIsr_1)
{
  Pwm_IsrHandler(1);
  return;
}

ISR (PwmIsr_2)
{
  Pwm_IsrHandler(2);
  return;
}

ISR (PwmIsr_3)
{
  Pwm_IsrHandler(3);
  return;
}

ISR (PwmIsr_4)
{
  Pwm_IsrHandler(4);
  return;
}

ISR (PwmIsr_5)
{
  Pwm_IsrHandler(5);
  return;
}

ISR (PwmIsr_6)
{
  Pwm_IsrHandler(6);
  return;
}

ISR (PwmIsr_7)
{
  Pwm_IsrHandler(7);
  return;
}

ISR (PwmIsr_8)
{
  Pwm_IsrHandler(8);
  return;
}

ISR (PwmIsr_9)
{
  Pwm_IsrHandler(9);
  return;
}

ISR (PwmIsr_10)
{
  Pwm_IsrHandler(10);
  return;
}

ISR (PwmIsr_11)
{
  Pwm_IsrHandler(11);
  return;
}

ISR (PwmIsr_12)
{
  Pwm_IsrHandler(12);
  return;
}

ISR (PwmIsr_13)
{
  Pwm_IsrHandler(13);
  return;
}

ISR (PwmIsr_14)
{
  Pwm_IsrHandler(14);
  return;
}

ISR (PwmIsr_15)
{
  Pwm_IsrHandler(15);
  return;
}

ISR (PwmIsr_16)
{
  Pwm_IsrHandler(16);
  return;
}

ISR (PwmIsr_17)
{
  Pwm_IsrHandler(17);
  return;
}

ISR (PwmIsr_18)
{
  Pwm_IsrHandler(18);
  return;
}

ISR (PwmIsr_19)
{
  Pwm_IsrHandler(19);
  return;
}

ISR (PwmIsr_20)
{
  Pwm_IsrHandler(20);
  return;
}

ISR (PwmIsr_21)
{
  Pwm_IsrHandler(21);
  return;
}

ISR (PwmIsr_22)
{
  Pwm_IsrHandler(22);
  return;
}

ISR (PwmIsr_23)
{
  Pwm_IsrHandler(23);
  return;
}

ISR (PwmIsr_24)
{
  Pwm_IsrHandler(24);
  return;
}

ISR (PwmIsr_25)
{
  Pwm_IsrHandler(25);
  return;
}

ISR (PwmIsr_26)
{
  Pwm_IsrHandler(26);
  return;
}

ISR (PwmIsr_27)
{
  Pwm_IsrHandler(27);
  return;
}

ISR (PwmIsr_28)
{
  Pwm_IsrHandler(28);
  return;
}

ISR (PwmIsr_29)
{
  Pwm_IsrHandler(29);
  return;
}

ISR (PwmIsr_30)
{
  Pwm_IsrHandler(30);
  return;
}

ISR (PwmIsr_31)
{
  Pwm_IsrHandler(31);
  return;
}

ISR (PwmIsr_32)
{
  Pwm_IsrHandler(32);
  return;
}

ISR (PwmIsr_33)
{
  Pwm_IsrHandler(33);
  return;
}

ISR (PwmIsr_34)
{
  Pwm_IsrHandler(34);
  return;
}

ISR (PwmIsr_35)
{
  Pwm_IsrHandler(35);
  return;
}

ISR (PwmIsr_36)
{
  Pwm_IsrHandler(36);
  return;
}

ISR (PwmIsr_37)
{
  Pwm_IsrHandler(37);
  return;
}

ISR (PwmIsr_38)
{
  Pwm_IsrHandler(38);
  return;
}

ISR (PwmIsr_39)
{
  Pwm_IsrHandler(39);
  return;
}

ISR (PwmIsr_40)
{
  Pwm_IsrHandler(40);
  return;
}

ISR (PwmIsr_41)
{
  Pwm_IsrHandler(41);
  return;
}

ISR (PwmIsr_42)
{
  Pwm_IsrHandler(42);
  return;
}

ISR (PwmIsr_43)
{
  Pwm_IsrHandler(43);
  return;
}

ISR (PwmIsr_44)
{
  Pwm_IsrHandler(44);
  return;
}

ISR (PwmIsr_45)
{
  Pwm_IsrHandler(45);
  return;
}

ISR (PwmIsr_46)
{
  Pwm_IsrHandler(46);
  return;
}

ISR (PwmIsr_47)
{
  Pwm_IsrHandler(47);
  return;
}

ISR (PwmIsr_48)
{
  Pwm_IsrHandler(48);
  return;
}

ISR (PwmIsr_49)
{
  Pwm_IsrHandler(49);
  return;
}



/**********************************************************************************************************************
 *  END OF FILE: Pwm_Irq.c
 *********************************************************************************************************************/
