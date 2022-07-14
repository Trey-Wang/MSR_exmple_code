/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Mcu.h
 *        \brief  Mcu header file
 *
 *      \details  The Mcu Driver provides services for basic microcontroller initialization,
 *                power down functionality, reset and microcontroller specific functions
 *                required from other MICROSAR CAL software modules.
 *                This is an emulated driver for VTT
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Peter Lang                    virlg         Vector Informatik GmbH
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-09-13  virlg   ESCAN00068491 Initial Development 
 *  01.00.01  2014-02-25  virchl                Missing semi colon added in line 188
 *  01.00.02  2014-05-15  virchl                Version Numbers adapted
 *  02.00.00  2015-01-21  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Mcu_Init (if VTTOnly-SIP)
 *                                              Mcu_Init renamed to Mcu_Init_Vtt and a define in Mcu_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *  02.02.00  2015-10-17  virchl  ESCAN00085049 McuModeMapping added
 *  02.03.00  2017-01-13  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update  
 *  02.04.00  2017-05-19  virbmz  ESCAN00094865 Version Numbers adapted
 **********************************************************************************************************************/


/* Protection against multiple inclusion */
#if !defined MCU_H
#define MCU_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "VttCntrl_Mcu.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVMCU_VTTCANOE01ASR_VERSION            (0x0204u)
#define DRVMCU_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define MCU_VENDOR_ID                           (30u)
#define MCU_MODULE_ID                           (101u)
#define MCU_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define MCU_AR_RELEASE_MAJOR_VERSION            (4u)
#define MCU_AR_RELEASE_MINOR_VERSION            (0u)
#define MCU_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define MCU_SW_MAJOR_VERSION                    (DRVMCU_VTTCANOE01ASR_VERSION >> 8)
#define MCU_SW_MINOR_VERSION                    (DRVMCU_VTTCANOE01ASR_VERSION & 0x00FF)
#define MCU_SW_PATCH_VERSION                    (DRVMCU_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define MCU_SID_INIT                  (0x00u)
#define MCU_SID_INIT_RAMSECTION       (0x01u)
#define MCU_SID_INIT_CLOCK            (0x02u)
#define MCU_SID_DISTRIBUTE_PLL_CLOCK  (0x03u)
#define MCU_SID_GET_PLL_STATUS        (0x04u)
#define MCU_SID_GET_RESET_REASON      (0x05u)
#define MCU_SID_GET_RESET_RAW_VALUE   (0x06u)
#define MCU_SID_PERFORM_RESET         (0x07u)
#define MCU_SID_SET_MODE              (0x08u)
#define MCU_SID_GET_VERSION_INFO      (0x09u)
#define MCU_SID_INIT_RAMSTATE         (0x04u)

/* ----- Error codes ----- */
#define MCU_E_PARAM_CONFIG            (0x0Au)
#define MCU_E_PARAM_CLOCK             (0x0Bu)
#define MCU_E_PARAM_MODE              (0x0Cu)
#define MCU_E_PARAM_RAMSECTION        (0x0Du)
#define MCU_E_PLL_NOT_LOCKED          (0x0Eu)
#define MCU_E_UNINIT                  (0x0Fu)
#define MCU_E_PARAM_VINFO             (0x15u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef uint8 Mcu_ClockType;

typedef uint8 Mcu_ModeType;

typedef uint8 Mcu_RawResetType;

typedef uint8 Mcu_RamSectionType;

typedef enum
{
  MCU_POWER_ON_RESET       = 0,
  MCU_WATCHDOG_RESET       = 1,
  MCU_SW_RESET             = 2,
  MCU_ILLEGALADDRESS_RESET = 3,
  MCU_LOW_VOLTAGE_RESET    = 4,
  MCU_EXTERNAL_RESET       = 5,
  MCU_CLOCK_MONITOR_RESET  = 6,
  MCU_RESET_UNDEFINED      = 7
} Mcu_ResetType;

typedef enum
{
  MCU_PLL_LOCKED = 0,
  MCU_PLL_UNLOCKED,
  MCU_PLL_STATUS_UNDEFINED
} Mcu_PllStatusType;

typedef enum
{
  MCU_RAMSTATE_INVALID = 0,
  MCU_RAMSTATE_VALID
} Mcu_RamStateType ;

typedef struct
{
  P2CONST(VttMcu_ModeType, AUTOMATIC, MCU_APPL_CONST) Mcu_ModeMapping_pt;
  Mcu_ClockType               NumberOfClockSettings_u8;  /* Number of Clock Settings configured */
  Mcu_RamSectionType          NumberOfRamSectors_u8;   /* Number of RAM Sectors */
} Mcu_ConfigType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Mcu_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, MCU_CODE) Mcu_InitMemory (void);
 
FUNC(void, MCU_CODE) Mcu_Init_Vtt (P2CONST(Mcu_ConfigType, AUTOMATIC, MCU_APPL_CONST) ConfigPtr);

#if(MCU_INIT_CLOCK == STD_ON)
  FUNC(Std_ReturnType, MCU_CODE) Mcu_InitClock (Mcu_ClockType ClockSetting);
#endif

FUNC(void, MCU_CODE) Mcu_SetMode (Mcu_ModeType McuMode);

#if(MCU_NO_PLL == STD_OFF)
  FUNC(void, MCU_CODE) Mcu_DistributePllClock (void);
#endif

#if (STD_ON == MCU_PERFORM_RESET_API)
  FUNC(void, MCU_CODE)  Mcu_PerformReset (void);
#endif

FUNC(Mcu_PllStatusType, MCU_CODE) Mcu_GetPllStatus (void);

FUNC(Mcu_ResetType, MCU_CODE) Mcu_GetResetReason (void);

FUNC(Mcu_RawResetType, MCU_CODE) Mcu_GetResetRawValue (void);

#if (STD_ON == MCU_VERSION_INFO_API)
  FUNC(void, MCU_CODE) Mcu_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, MCU_APPL_DATA) versioninfo);
#endif

FUNC(Std_ReturnType, MCU_CODE) Mcu_InitRamSection (Mcu_RamSectionType RamSection);

#if (MCU_GET_RAM_STATE_API == STD_ON)
  FUNC(Mcu_RamStateType, MCU_CODE) Mcu_GetRamState (void);
#endif


#endif /* !defined MCU_H */

/**********************************************************************************************************************
 *  END OF FILE: Mcu.h
 **********************************************************************************************************************/
