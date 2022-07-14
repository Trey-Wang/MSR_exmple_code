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
 *         File:  Adc_Irq.c
 *    Component:  DrvAdc_VttCanoe01Asr
 *       Module:  DrvAdc
 *    Generator:  -
 *
 *   Description: Module for Adc
 *
 **********************************************************************************************************************/

#define ADC_IRQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Adc.h"
#include "Os.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
#if (  (ADC_SW_MAJOR_VERSION != (2U)) \
    || (ADC_SW_MINOR_VERSION != (5U)) )
    #error "Version numbers of Adc_Irq.c and Adc.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  FUNCTION PROTOTYPE OF ADC ISR HANDLER
 **********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_IsrHandler (Adc_HwUnitEnumType HwUnit);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
ISR (AdcIsr_0)
{
  Adc_IsrHandler(Hw0);
}

ISR (AdcIsr_1)
{
  Adc_IsrHandler(Hw1);
}

ISR (AdcIsr_2)
{
  Adc_IsrHandler(Hw2);
}

ISR (AdcIsr_3)
{
  Adc_IsrHandler(Hw3);
}

ISR (AdcIsr_4)
{
  Adc_IsrHandler(Hw4);
}

ISR (AdcIsr_5)
{
  Adc_IsrHandler(Hw5);
}

ISR (AdcIsr_6)
{
  Adc_IsrHandler(Hw6);
}

ISR (AdcIsr_7)
{
  Adc_IsrHandler(Hw7);
}

ISR (AdcIsr_8)
{
  Adc_IsrHandler(Hw8);
}

ISR (AdcIsr_9)
{
  Adc_IsrHandler(Hw9);
}

ISR (AdcIsr_10)
{
  Adc_IsrHandler(Hw10);
}

ISR (AdcIsr_11)
{
  Adc_IsrHandler(Hw11);
}

ISR (AdcIsr_12)
{
  Adc_IsrHandler(Hw12);
}

ISR (AdcIsr_13)
{
  Adc_IsrHandler(Hw13);
}

ISR (AdcIsr_14)
{
  Adc_IsrHandler(Hw14);
}

ISR (AdcIsr_15)
{
  Adc_IsrHandler(Hw15);
}

ISR (AdcIsr_16)
{
  Adc_IsrHandler(Hw16);
}

ISR (AdcIsr_17)
{
  Adc_IsrHandler(Hw17);
}

ISR (AdcIsr_18)
{
  Adc_IsrHandler(Hw18);
}

ISR (AdcIsr_19)
{
  Adc_IsrHandler(Hw19);
}

ISR (AdcIsr_20)
{
  Adc_IsrHandler(Hw20);
}

ISR (AdcIsr_21)
{
  Adc_IsrHandler(Hw21);
}

ISR (AdcIsr_22)
{
  Adc_IsrHandler(Hw22);
}

ISR (AdcIsr_23)
{
  Adc_IsrHandler(Hw23);
}

ISR (AdcIsr_24)
{
  Adc_IsrHandler(Hw24);
}

ISR (AdcIsr_25)
{
  Adc_IsrHandler(Hw25);
}

ISR (AdcIsr_26)
{
  Adc_IsrHandler(Hw26);
}

ISR (AdcIsr_27)
{
  Adc_IsrHandler(Hw27);
}

ISR (AdcIsr_28)
{
  Adc_IsrHandler(Hw28);
}

ISR (AdcIsr_29)
{
  Adc_IsrHandler(Hw29);
}

ISR (AdcIsr_30)
{
  Adc_IsrHandler(Hw30);
}

ISR (AdcIsr_31)
{
  Adc_IsrHandler(Hw31);
}

ISR (AdcIsr_32)
{
  Adc_IsrHandler(Hw32);
}

ISR (AdcIsr_33)
{
  Adc_IsrHandler(Hw33);
}

ISR (AdcIsr_34)
{
  Adc_IsrHandler(Hw34);
}

ISR (AdcIsr_35)
{
  Adc_IsrHandler(Hw35);
}

ISR (AdcIsr_36)
{
  Adc_IsrHandler(Hw36);
}

ISR (AdcIsr_37)
{
  Adc_IsrHandler(Hw37);
}

ISR (AdcIsr_38)
{
  Adc_IsrHandler(Hw38);
}

ISR (AdcIsr_39)
{
  Adc_IsrHandler(Hw39);
}

ISR (AdcIsr_40)
{
  Adc_IsrHandler(Hw40);
}

ISR (AdcIsr_41)
{
  Adc_IsrHandler(Hw41);
}

ISR (AdcIsr_42)
{
  Adc_IsrHandler(Hw42);
}

ISR (AdcIsr_43)
{
  Adc_IsrHandler(Hw43);
}

ISR (AdcIsr_44)
{
  Adc_IsrHandler(Hw44);
}

ISR (AdcIsr_45)
{
  Adc_IsrHandler(Hw45);
}

ISR (AdcIsr_46)
{
  Adc_IsrHandler(Hw46);
}

ISR (AdcIsr_47)
{
  Adc_IsrHandler(Hw47);
}

ISR (AdcIsr_48)
{
  Adc_IsrHandler(Hw48);
}

ISR (AdcIsr_49)
{
  Adc_IsrHandler(Hw49);
}


/**********************************************************************************************************************
 *  END OF FILE: Adc_Irq.c
 **********************************************************************************************************************/
