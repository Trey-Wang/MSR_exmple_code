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
 *         File:  Gpt_Irq.c
 *      Project:  DrvGpt_VttCanoe01Asr
 *       Module:  MICROSAR Gpt
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Gpt implements a general purpose timer driver.
 *
 *********************************************************************************************************************/

#define GPT_IRQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Gpt.h"
#include "Os.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (  (GPT_SW_MAJOR_VERSION != (2U)) \
    || (GPT_SW_MINOR_VERSION != (5U)) )
  #error "Version numbers of Gpt_Irq.c and Gpt.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  FUNCTION PROTOTYPE OF GPT ISR HANDLER
 **********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_IrqFunction(uint8 Index);


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
ISR (GptIsr_0)
{
  Gpt_IrqFunction(0);
  return;
}
ISR (GptIsr_1)
{
  Gpt_IrqFunction(1);
  return;
}
ISR (GptIsr_2)
{
  Gpt_IrqFunction(2);
  return;
}
ISR (GptIsr_3)
{
  Gpt_IrqFunction(3);
  return;
}
ISR (GptIsr_4)
{
  Gpt_IrqFunction(4);
  return;
}
ISR (GptIsr_5)
{
  Gpt_IrqFunction(5);
  return;
}
ISR (GptIsr_6)
{
  Gpt_IrqFunction(6);
  return;
}
ISR (GptIsr_7)
{
  Gpt_IrqFunction(7);
  return;
}
ISR (GptIsr_8)
{
  Gpt_IrqFunction(8);
  return;
}
ISR (GptIsr_9)
{
  Gpt_IrqFunction(9);
  return;
}
ISR (GptIsr_10)
{
  Gpt_IrqFunction(10);
  return;
}
ISR (GptIsr_11)
{
  Gpt_IrqFunction(11);
  return;
}
ISR (GptIsr_12)
{
  Gpt_IrqFunction(12);
  return;
}
ISR (GptIsr_13)
{
  Gpt_IrqFunction(13);
  return;
}
ISR (GptIsr_14)
{
  Gpt_IrqFunction(14);
  return;
}
ISR (GptIsr_15)
{
  Gpt_IrqFunction(15);
  return;
}
ISR (GptIsr_16)
{
  Gpt_IrqFunction(16);
  return;
}
ISR (GptIsr_17)
{
  Gpt_IrqFunction(17);
  return;
}
ISR (GptIsr_18)
{
  Gpt_IrqFunction(18);
  return;
}
ISR (GptIsr_19)
{
  Gpt_IrqFunction(19);
  return;
}
ISR (GptIsr_20)
{
  Gpt_IrqFunction(20);
  return;
}
ISR (GptIsr_21)
{
  Gpt_IrqFunction(21);
  return;
}
ISR (GptIsr_22)
{
  Gpt_IrqFunction(22);
  return;
}
ISR (GptIsr_23)
{
  Gpt_IrqFunction(23);
  return;
}
ISR (GptIsr_24)
{
  Gpt_IrqFunction(24);
  return;
}
ISR (GptIsr_25)
{
  Gpt_IrqFunction(25);
  return;
}
ISR (GptIsr_26)
{
  Gpt_IrqFunction(26);
  return;
}
ISR (GptIsr_27)
{
  Gpt_IrqFunction(27);
  return;
}
ISR (GptIsr_28)
{
  Gpt_IrqFunction(28);
  return;
}
ISR (GptIsr_29)
{
  Gpt_IrqFunction(29);
  return;
}
ISR (GptIsr_30)
{
  Gpt_IrqFunction(30);
  return;
}
ISR (GptIsr_31)
{
  Gpt_IrqFunction(31);
  return;
}
ISR (GptIsr_32)
{
  Gpt_IrqFunction(32);
  return;
}
ISR (GptIsr_33)
{
  Gpt_IrqFunction(33);
  return;
}
ISR (GptIsr_34)
{
  Gpt_IrqFunction(34);
  return;
}
ISR (GptIsr_35)
{
  Gpt_IrqFunction(35);
  return;
}
ISR (GptIsr_36)
{
  Gpt_IrqFunction(36);
  return;
}
ISR (GptIsr_37)
{
  Gpt_IrqFunction(37);
  return;
}
ISR (GptIsr_38)
{
  Gpt_IrqFunction(38);
  return;
}
ISR (GptIsr_39)
{
  Gpt_IrqFunction(39);
  return;
}
ISR (GptIsr_40)
{
  Gpt_IrqFunction(40);
  return;
}
ISR (GptIsr_41)
{
  Gpt_IrqFunction(41);
  return;
}
ISR (GptIsr_42)
{
  Gpt_IrqFunction(42);
  return;
}
ISR (GptIsr_43)
{
  Gpt_IrqFunction(43);
  return;
}
ISR (GptIsr_44)
{
  Gpt_IrqFunction(44);
  return;
}
ISR (GptIsr_45)
{
  Gpt_IrqFunction(45);
  return;
}
ISR (GptIsr_46)
{
  Gpt_IrqFunction(46);
  return;
}
ISR (GptIsr_47)
{
  Gpt_IrqFunction(47);
  return;
}
ISR (GptIsr_48)
{
  Gpt_IrqFunction(48);
  return;
}
ISR (GptIsr_49)
{
  Gpt_IrqFunction(49);
  return;
}


/**********************************************************************************************************************
 *  END OF FILE: Gpt_Irq.c
 *********************************************************************************************************************/
 
