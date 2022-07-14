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
/**        \file  _InitFunctionConfiguration.h
 *        \brief  Template header file for initialization of DualTarget-Configurations.
 *
 *      \details  This file has to be extended with the initialization functions used for both real and virtual target.
 *                Adapt the file to the MCAL used by the derivative.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Gerber Wladimir               viswge        Vector Informatik GmbH
 *  Philipp Damian                visdap        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.04  2015-04-20  viswge  -             initial creation
 *********************************************************************************************************************/

#if !defined (INITFUNCTIONCONFIGURATION_H)
# define INITFUNCTIONCONFIGURATION_H

#include "Compiler.h"

/**********************************************************************************************************************
 * Set the following defines to the name of the configuration variable 
 *********************************************************************************************************************/

#ifdef _MICROSOFT_C_VTT_
  #define CAN_INIT_CFG      Can_Config_Ptr
  #define LIN_INIT_CFG      NULL_PTR
  #define ETH_INIT_CFG      NULL_PTR
  #define FR_INIT_CFG       NULL_PTR
  #define ADC_INIT_CFG      &AdcConfigSet
  #define DIO_INIT_CFG      &DioConfig
  #define EEP_INIT_CFG      &EepInitConfiguration
  #define FLS_INIT_CFG      &FlsConfigSet
  #define GPT_INIT_CFG      &GptChannelConfigSet
  #define ICU_INIT_CFG      &IcuConfigSet
  #define MCU_INIT_CFG      &McuModuleConfiguration
  #define PORT_INIT_CFG     &PortConfigSet
  #define PWM_INIT_CFG      &PwmChannelConfigSet
  #define SPI_INIT_CFG      &SpiDriver
  #define WDG_INIT_CFG      &WdgSettingsConfig
#else  
  #define CAN_INIT_CFG      
  #define LIN_INIT_CFG      
  #define ETH_INIT_CFG      
  #define FR_INIT_CFG       
  #define ADC_INIT_CFG      
  #define DIO_INIT_CFG      
  #define EEP_INIT_CFG      
  #define FLS_INIT_CFG      
  #define GPT_INIT_CFG      
  #define ICU_INIT_CFG      
  #define MCU_INIT_CFG      
  #define PORT_INIT_CFG     
  #define PWM_INIT_CFG      
  #define SPI_INIT_CFG      
  #define WDG_INIT_CFG      
#endif

#endif /* INITFUNCTIONCONFIGURATION_H */

/**********************************************************************************************************************
 *  END OF FILE: _InitFunctionConfiguration.h
 *********************************************************************************************************************/
