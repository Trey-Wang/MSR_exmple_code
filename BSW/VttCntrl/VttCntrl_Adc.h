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
/**      \file    VTTCntrl_Adc.h
*        \brief   Declares the public API of the virtual ADC controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual ADC controller that enables to read/write (group-wise) to ADC ports (channels).
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel           vismsl       Vector Informatik GmbH
 *  Damian Philipp           visdap    Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-23  vismsl  -             Initial version extracted from VTTCntrl generator.
 *  03.00.04  2015-09-22  visdap  ESCAN00085394 Added pragmas to protect VTTCntrl during simulated ECU reset.
 *  03.01.00  2015-08-09  visdap  ESCAN00085048 Extended for 63bit resolution.
 *  03.01.01  2016-01-22  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*********************************************************************************************************************/

#if !defined (VTTCNTRL_ADC_H)
# define VTTCNTRL_ADC_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VTTCntrl_Base.h"
#include "VTTCntrl_Adc_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
# define VTTADC_SW_MAJOR_VERSION (3u)
# define VTTADC_SW_MINOR_VERSION (1u)
# define VTTADC_SW_PATCH_VERSION (1u)

# define VTTADC_IRQ_OFFSET     IRQ_AdcIsr_0 

/*! \brief Group trigger source. */
#define VTTADC_TRIGG_SRC_HW           (0x01) /* Trigger is set to extern (Timer, PWM, ANx channels) */
#define VTTADC_TRIGG_SRC_SW           (0x00) /* Trigger is set to software */

/*! \brief Hardware trigger signal. */
#define VTTADC_HW_TRIG_FALLING_EDGE   (0x00) /* Falling edge detected */
#define VTTADC_HW_TRIG_RISING_EDGE    (0x01) /* Rising edge detected */
#define VTTADC_HW_TRIG_BOTH_EDGES     (0x02) /* Rising edge detected */

#define VTTADC_FALSE  0
#define VTTADC_TRUE   1

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef uint8 Adc_GroupType;
typedef uint8 Adc_HwTriggerEventType;
typedef uint64 Adc_ValueType;

/*! \brief VttAdc Operation Modes: When OneShot is started, all channels have to be processed consecutively,
*                                  the current operation mode is stored by VttAdc_OpMode */
typedef enum
{
  VttAdc_OpMode_Stopped = 0,
  VttAdc_OpMode_OneShot,
  VttAdc_OpMode_ToBeHWTriggered,
  VttAdc_OpMode_Continuous,
  VttAdc_Opmode_Finished,
} VttAdc_OpModeType;

typedef struct
{
  const char* ChannelName;          /* Structure: "GroupName" + "ChannelName" */
  uint32 Resolution;                /* (/MICROSAR/Adc/AdcConfigSet/AdcHwUnit/AdcChannel/AdcChannelResolution) */
  double PhysicalValue;             /* Current value of system variable.*/
  double VoltageLowReference;       /* (/MICROSAR/Adc/AdcConfigSet/AdcHwUnit/AdcChannel/AdcChannelRefVoltsrcHigh/Low)  */
  double VoltageHighReference;      /* (/MICROSAR/Adc/AdcConfigSet/AdcHwUnit/AdcChannel/AdcChannelRefVoltsrcHigh/High) */
  Adc_ValueType RegisterValue;      /* Current ADC channel value. */
  sint32 CANoeSysVarHandle;         /* CANoeEmu system variable that implements the ADC channel. */
  uint32 ConversionSampleTime;      /* (.../AdcChannel/AdcChannelConvTime + .../AdcChannel/AdcChannelSampTime) */
} VttAdc_ChannelType;

typedef struct
{
  const char* GroupName;
  uint8 NumberOfChannels;
  sint32 Isr_Nr;                    /* The ISR-Number is derived from the HW Unit */
  uint8 CANoeTimerHandle;
  sint32 SysVarHandle_HwTrigger;
  sint32 SysVarHandle_Param;        /* Needed by SystemVariable Handler to identify the correct trigger. */
  uint8 GroupHwTriggerEdge;
  uint8 CurrentProcessingChannel;
  VttAdc_OpModeType CurrentMode;
  Adc_ValueType* ChannelValueArray;
  VttAdc_ChannelType** ChannelArray;
} VttAdc_CntrlType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

void VttAdc_StartOneShotImmediate(Adc_GroupType group);
void VttAdc_StartOneShotHwTriggered(Adc_GroupType group, Adc_HwTriggerEventType edge);
void VttAdc_StartContinuous(Adc_GroupType group);
void VttAdc_StopConversion(Adc_GroupType group);
void VttAdc_GetAdcValue(Adc_GroupType group, Adc_ValueType** ValueArray);

void VttAdc_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_ADC_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Adc.h
 *********************************************************************************************************************/
