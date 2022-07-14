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
 *         File:  Adc.h
 *    Component:  DrvAdc_VttCanoe01Asr
 *       Module:  DrvAdc
 *    Generator:  -
 *
 *  Description:  MICROSAR CAL Module Adc implements a driver for generating a digital code from an analogue voltage
 *                signal.
 *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-01-09  virchl     -          Ported Adc driver from DrvAdc_CanoeemuCanoe01Asr Ver. 03.00.02 to Vip
 *  01.00.01  2014-06-11  virchl  ESCAN00076198 Renaming of the Isrs
 *  02.00.00  2015-01-15  virchl   FEAT-254     Global renaming of VIP to VTT
 *  02.00.01  2015-04-28  virchl     -          No modification here
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Adc_Init (if VTTOnly-SIP)
 *                                              Adc_Init renamed to Adc_Init_Vtt and a define in Adc_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version 
 *                                ESCAN00085047 Resolution of Adc extended to 1:63 (Adc_ValueGroupType)
 *                                ESCAN00083484 Adc_HwUnitEnumType extended  to Hw49
 *  02.03.00  2016-02-25  virchl  ESCAN00088372 Update of SwVersions due to increase of generator version 
 *  02.04.00  2016-09-05  virchl  ESCAN00091733 Usage of CAT2 ISRs 
 *  02.05.00  2017-01-12  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 **********************************************************************************************************************/

#if !defined(ADC_H)
#define ADC_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVADC_VTTCANOE01ASR_VERSION            (0x0205u)
#define DRVADC_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define ADC_VENDOR_ID                           (30u)
#define ADC_MODULE_ID                           (123u)
#define ADC_INSTANCE_ID                         (0u)

/* AUTOSAR Software specification version information */
#define ADC_AR_RELEASE_MAJOR_VERSION            (4u)
#define ADC_AR_RELEASE_MINOR_VERSION            (0u)
#define ADC_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define ADC_SW_MAJOR_VERSION                    (DRVADC_VTTCANOE01ASR_VERSION >> 8)
#define ADC_SW_MINOR_VERSION                    (DRVADC_VTTCANOE01ASR_VERSION & 0x00FF)
#define ADC_SW_PATCH_VERSION                    (DRVADC_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define ADC_SID_INIT                            (0x00u)
#define ADC_SID_DEINIT                          (0x01u)
#define ADC_SID_START_GROUP_CONVERSION          (0x02u)
#define ADC_SID_STOP_GROUP_CONVERSION           (0x03u)
#define ADC_SID_READ_GROUP                      (0x04u)
#define ADC_SID_ENABLE_HARDWARE_TRIGGER         (0x05u)
#define ADC_SID_DISABLE_HARDWARE_TRIGGER        (0x06u)
#define ADC_SID_ENABLE_GROUP_NOTIFICATION       (0x07u)
#define ADC_SID_DISABLE_GROUP_NOTIFICATION      (0x08u)
#define ADC_SID_GET_GROUP_STATUS                (0x09u)
#define ADC_SID_GET_VERSION_INFO                (0x0Au)
#define ADC_SID_GET_STREAM_LAST_POINTER         (0x0Bu)
#define ADC_SID_SETUP_RESULT_BUFFER             (0x0Cu)

/* ----- Error codes ----- */
/* This error is raised when an API function is called, which needs an initialized ADC. */
#define ADC_E_UNINIT                            (0x0Au)
/* An API function can not be called because of an ongoing process. */
#define ADC_E_BUSY                              (0x0Bu)
/* This error is raised when an API function is called, which needs an ongoing or completed conversion. */
#define ADC_E_IDLE                              (0x0Cu)
/* This error is raised when Adc_Init() is called when the module is already initialized. */
#define ADC_E_ALREADY_INITIALIZED               (0x0Du)
/* This error is raised when Adc_Init() has been called with a NULL_PTR. */
#define ADC_E_PARAM_CONFIG                      (0x0Eu)
/* This error is raised when an API function is called with an invalid group ID. */
#define ADC_E_PARAM_GROUP                       (0x15u)
/* This error is raised when API function Adc_EnableHardwareTrigger or Adc_DisableHardwareTrigger is called
 * for a group with continuous conversion mode */
#define ADC_E_WRONG_CONV_MODE                   (0x16u)
/* This error is raised when a trigger-specific API function is called for group with wrong trigger source.
 * For example EnableHardwareTrigger() is called for a software triggered group */
#define ADC_E_WRONG_TRIGG_SRC                   (0x17u)
/* This error is raised when Adc_EnableNotificationFunction or Adc_DisableNotificationFunction is called for a
 * group whose configuration set has no notification available */
#define ADC_E_NOTIF_CAPABILITY                  (0x18u)
/* This error is raised when a conversion is started but the result buffer has not been initialized before. */
#define ADC_E_BUFFER_UNINIT                     (0x19u)
/* This error is raised when Adc_ReadGroup() is called with parameter DataBufferPtr referencing NULL_PTR. */
#define ADC_E_PARAM_READ_BUFFER                 (0x1Au)
/* This error is raised when Adc_SetupResultBuffer() is called with parameter DataBufferPtr referencing NULL_PTR. */
#define ADC_E_PARAM_RESULT_BUFFER               (0x1Bu)
/* This error is raised when Adc_GetStreamLastPointer() is called with parameter PtrToSamplePtr referencing NULL_PTR. */
#define ADC_E_PARAM_STREAM_PTR                  (0x1Cu)
/* This error is raised when Adc_GetVersionInfo() is called with parameter versioninfo referencing NULL_PTR. */
#define ADC_E_PARAM_VINFO                       (0x1Du)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Numeric ID of an ADC channel */
typedef uint8 Adc_ChannelType;

/* Numeric ID of an ADC channel group. Range: 0-254, 255 is reserved for ADC_INVALID_GRP_ID */
typedef uint8  Adc_GroupType;

/* Type for reading the converted values of a channel group (raw, without further scaling, right aligned). */
typedef uint64 Adc_ValueGroupType;

/* Reference type for Adc_ValueGroupType */
typedef P2VAR(Adc_ValueGroupType, AUTOMATIC, ADC_VAR) Adc_ValueGroupRefType;

/* Reference on a reference on Adc_ValueGroupType */
typedef P2VAR(Adc_ValueGroupRefType, AUTOMATIC, ADC_VAR) Adc_ValueGroupRefRefType;

/* Type of clock prescaler factor */
typedef uint8  Adc_PrescaleType;

/* Current status of the conversion of the requested ADC Channel group */
typedef enum
{
  ADC_IDLE,
  ADC_BUSY,
  ADC_COMPLETED,
  ADC_STREAM_COMPLETED
} Adc_StatusType;

/* Priority level of the channel */
typedef uint8 Adc_GroupPriorityType;

/* Type for configuring the number of group conversions in streaming access mode (in single access mode,
 * parameter is 1) */
typedef uint16  Adc_StreamNumSampleType;

/* Enumerated ID of an ADC Hw Unit */
typedef enum
{
  Hw0,
  Hw1,
  Hw2,
  Hw3,
  Hw4,
  Hw5,
  Hw6,
  Hw7,
  Hw8,
  Hw9,
  Hw10,
  Hw11,
  Hw12,
  Hw13,
  Hw14,
  Hw15,
  Hw16,
  Hw17,
  Hw18,
  Hw19,
  Hw20,
  Hw21,
  Hw22,
  Hw23,
  Hw24,
  Hw25,
  Hw26,
  Hw27,
  Hw28,
  Hw29,
  Hw30,
  Hw31,
  Hw32,
  Hw33,
  Hw34,
  Hw35,
  Hw36,
  Hw37,
  Hw38,
  Hw39,
  Hw40,
  Hw41,
  Hw42,
  Hw43,
  Hw44,
  Hw45,
  Hw46,
  Hw47,
  Hw48,
  Hw49,
} Adc_HwUnitEnumType;

/* Numeric ID of an ADC Hw Unit */
typedef uint8  Adc_HwUnitType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Adc_Cfg.h"
#include "VttCntrl_Adc.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, ADC_CODE) Adc_InitMemory (void);

FUNC(void, ADC_CODE) Adc_Init_Vtt (P2CONST(Adc_ConfigType, AUTOMATIC, ADC_APPL_CONST) ConfigPtr);

FUNC(Std_ReturnType, ADC_CODE) Adc_SetupResultBuffer
(
  Adc_GroupType Group,
  Adc_ValueGroupRefType DataBufferPtr
);

#if (STD_ON == ADC_DE_INIT_API)
  FUNC(void, ADC_CODE) Adc_DeInit(void);
#endif

#if (STD_ON == ADC_ENABLE_START_STOP_GROUP_API)
  FUNC(void, ADC_CODE) Adc_StartGroupConversion (Adc_GroupType Group);
  FUNC(void, ADC_CODE) Adc_StopGroupConversion (Adc_GroupType Group);
#endif

#if (STD_ON == ADC_READ_GROUP_API)
  FUNC(Std_ReturnType, ADC_CODE) Adc_ReadGroup
  (
    Adc_GroupType Group,
    Adc_ValueGroupRefType DataBufferPtr
  );
#endif

#if (STD_ON == ADC_HW_TRIGGER_API)
  FUNC(void, ADC_CODE) Adc_EnableHardwareTrigger (Adc_GroupType Group);
  
  FUNC(void, ADC_CODE) Adc_DisableHardwareTrigger (Adc_GroupType Group);
#endif

#if (STD_ON == ADC_GRP_NOTIF_CAPABILITY)
  FUNC(void, ADC_CODE) Adc_EnableGroupNotification (Adc_GroupType Group);
  
  FUNC(void, ADC_CODE) Adc_DisableGroupNotification (Adc_GroupType Group);
#endif

FUNC(Adc_StatusType, ADC_CODE) Adc_GetGroupStatus (Adc_GroupType Group);

FUNC(Adc_StreamNumSampleType, ADC_CODE) Adc_GetStreamLastPointer
(
  Adc_GroupType Group,
  Adc_ValueGroupRefRefType PtrToSamplePtr
);

#if (STD_ON == ADC_VERSION_INFO_API)
  FUNC(void, ADC_CODE) Adc_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, ADC_APPL_DATA) versioninfo);
#endif

#endif  /* ADC_H */

/**********************************************************************************************************************
 *  END OF FILE: Adc.h
 **********************************************************************************************************************/
