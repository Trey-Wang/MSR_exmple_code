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
 *         File:  Icu_Irq.c
 *    Component:  DrvIcu_VttCanoe01Asr
 *       Module:  DrvIcu
 *    Generator:  -
 *
 *   Description: Module for Icu
 *
 *********************************************************************************************************************/

#define ICU_IRQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Icu.h"
#include "Os.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (  (ICU_SW_MAJOR_VERSION != (2u)) \
    || (ICU_SW_MINOR_VERSION != (5u)) )
    #error "Version numbers of Icu_Irq.c and Icu.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  FUNCTION PROTOTYPE OF ICU ISR HANDLER
 **********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_IsrHandler (Icu_InterruptSourceType Irq_Source);


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
ISR (IcuIsr_0)
{
  Icu_IsrHandler(0);
  return;
}

ISR (IcuIsr_1)
{
  Icu_IsrHandler(1);
  return;
}

ISR (IcuIsr_2)
{
  Icu_IsrHandler(2);
  return;
}

ISR (IcuIsr_3)
{
  Icu_IsrHandler(3);
  return;
}

ISR (IcuIsr_4)
{
  Icu_IsrHandler(4);
  return;
}

ISR (IcuIsr_5)
{
  Icu_IsrHandler(5);
  return;
}

ISR (IcuIsr_6)
{
  Icu_IsrHandler(6);
  return;
}

ISR (IcuIsr_7)
{
  Icu_IsrHandler(7);
  return;
}

ISR (IcuIsr_8)
{
  Icu_IsrHandler(8);
  return;
}

ISR (IcuIsr_9)
{
  Icu_IsrHandler(9);
  return;
}

ISR (IcuIsr_10)
{
  Icu_IsrHandler(10);
  return;
}

ISR (IcuIsr_11)
{
  Icu_IsrHandler(11);
  return;
}

ISR (IcuIsr_12)
{
  Icu_IsrHandler(12);
  return;
}

ISR (IcuIsr_13)
{
  Icu_IsrHandler(13);
  return;
}

ISR (IcuIsr_14)
{
  Icu_IsrHandler(14);
  return;
}

ISR (IcuIsr_15)
{
  Icu_IsrHandler(15);
  return;
}

ISR (IcuIsr_16)
{
  Icu_IsrHandler(16);
  return;
}

ISR (IcuIsr_17)
{
  Icu_IsrHandler(17);
  return;
}

ISR (IcuIsr_18)
{
  Icu_IsrHandler(18);
  return;
}

ISR (IcuIsr_19)
{
  Icu_IsrHandler(19);
  return;
}

ISR (IcuIsr_20)
{
  Icu_IsrHandler(20);
  return;
  }

ISR (IcuIsr_21)
{
  Icu_IsrHandler(21);
  return;
}

ISR (IcuIsr_22)
{
  Icu_IsrHandler(22);
  return;
}

ISR (IcuIsr_23)
{
  Icu_IsrHandler(23);
  return;
}

ISR (IcuIsr_24)
{
  Icu_IsrHandler(24);
  return;
}

ISR (IcuIsr_25)
{
  Icu_IsrHandler(25);
  return;
}

ISR (IcuIsr_26)
{
  Icu_IsrHandler(26);
  return;
}

ISR (IcuIsr_27)
{
  Icu_IsrHandler(27);
  return;
}

ISR (IcuIsr_28)
{
  Icu_IsrHandler(28);
  return;
}

ISR (IcuIsr_29)
{
  Icu_IsrHandler(29);
  return;
}

ISR (IcuIsr_30)
{
  Icu_IsrHandler(30);
  return;
}

ISR (IcuIsr_31)
{
  Icu_IsrHandler(31);
  return;
}

ISR (IcuIsr_32)
{
  Icu_IsrHandler(32);
  return;
}

ISR (IcuIsr_33)
{
  Icu_IsrHandler(33);
  return;
}

ISR (IcuIsr_34)
{
  Icu_IsrHandler(34);
  return;
}

ISR (IcuIsr_35)
{
  Icu_IsrHandler(35);
  return;
}

ISR (IcuIsr_36)
{
  Icu_IsrHandler(36);
  return;
}

ISR (IcuIsr_37)
{
  Icu_IsrHandler(37);
  return;
}

ISR (IcuIsr_38)
{
  Icu_IsrHandler(38);
  return;
}

ISR (IcuIsr_39)
{
  Icu_IsrHandler(39);
  return;
}

ISR (IcuIsr_40)
{
  Icu_IsrHandler(40);
  return;
}

ISR (IcuIsr_41)
{
  Icu_IsrHandler(41);
  return;
}

ISR (IcuIsr_42)
{
  Icu_IsrHandler(42);
  return;
}

ISR (IcuIsr_43)
{
  Icu_IsrHandler(43);
  return;
}

ISR (IcuIsr_44)
{
  Icu_IsrHandler(44);
  return;
}

ISR (IcuIsr_45)
{
  Icu_IsrHandler(45);
  return;
}

ISR (IcuIsr_46)
{
  Icu_IsrHandler(46);
  return;
}

ISR (IcuIsr_47)
{
  Icu_IsrHandler(47);
  return;
}

ISR (IcuIsr_48)
{
  Icu_IsrHandler(48);
  return;
}

ISR (IcuIsr_49)
{
  Icu_IsrHandler(49);
  return;
}



/**********************************************************************************************************************
 *  END OF FILE: Icu_Irq.c
 *********************************************************************************************************************/
