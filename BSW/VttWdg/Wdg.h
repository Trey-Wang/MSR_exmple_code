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
 *         File:  Wdg.h
 *    Component:  DrvWd_VttCanoe01Asr
 *       Module:  DrvWd
 *    Generator:  -
 *
 *  Description:  MICROSAR CAL Module Wdg implements a driver for the internal Watchdog
 *
 **********************************************************************************************************************/

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
 *  01.00.00  2013-09-16  virlg   ESCAN00068495 Initial version 
 *  01.00.01  2014-05-16  virchl                Version numbers update
 *  02.00.00  2015-01-223 virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Wdg_Init (if VTTOnly-SIP)
 *                                              Wdg_Init renamed to Wdg_Init_Vtt and a define in Wdg_Cfg.h reroutes it
 *                                ESCAN00083436 Swtich WDG_TRIGGERED_IN_GPT_NOTIFICATION added Wdg_SetTriggerCondition()
 *  02.01.01  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *  03.00.00  2015-09-08  virchl  ESCAN00085045 VttWdg is independent of Gpt
 *  03.01.00  2016-07-06  virchl  ESCAN00090844 Dependency to Dem set optional
 *  03.01.01  1016-08-12  virbmz   FEAT-1842    Support of external driver 
 *                                              defines for versioncheck are mapped in Wdg_Cfg.h
 *  03.02.00  2016-12-06  virbmz   FEAT2095     Support Multiple Instances - first steps:
 *                                              update the WDG CFG Version check in .c file
 *  03.03.00  2017-05-18  virbmz   TASK-33340   Version number incremented
 **********************************************************************************************************************/

#if !defined WDG_H
#define WDG_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "WdgIf_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVWD_VTTCANOE01ASR_VERSION             (0x0303u)
#define DRVWD_VTTCANOE01ASR_RELEASE_VERSION     (0x00u)

/* Vendor and module identification */
#define WDG_VENDOR_ID                           (30u)
#define WDG_MODULE_ID                           (102u)
#define WDG_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define WDG_AR_RELEASE_MAJOR_VERSION            (4u)
#define WDG_AR_RELEASE_MINOR_VERSION            (0u)
#define WDG_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define WDG_SW_MAJOR_VERSION                    (DRVWD_VTTCANOE01ASR_VERSION >> 8)
#define WDG_SW_MINOR_VERSION                    (DRVWD_VTTCANOE01ASR_VERSION & 0x00FF)
#define WDG_SW_PATCH_VERSION                    (DRVWD_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define WDG_SID_INIT                            (0x00u)
#define WDG_SID_SET_MODE                        (0x01u)
#define WDG_SID_SET_TRIGGER_CONDITION           (0x03u)
#define WDG_SID_GET_VERSION_INFO                (0x04u)
#define WDG_SID_CBK_GPT_NOTIFICATION            (0x05u)

/* ----- Error codes ----- */
#define WDG_E_DRIVER_STATE                      (0x10u)   /* API service used in wrong context */
#define WDG_E_PARAM_MODE                        (0x11u)   /* API service called with wrong inconsistent parameter(s) */
#define WDG_E_PARAM_CONFIG                      (0x12u)   /* API service called with wrong inconsistent parameter(s) */
#define WDG_E_PARAM_VINFO                       (0x14u)   /* API service called with wrong inconsistent parameter(s) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Wdg_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, WDG_CODE) Wdg_InitMemory (void);

FUNC(void, WDG_CODE) Wdg_Init_Vtt (P2CONST(Wdg_ConfigType, AUTOMATIC, WDG_APPL_CONST) ConfigPtr);

FUNC(Std_ReturnType, WDG_CODE) Wdg_SetMode (WdgIf_ModeType Wdg_Mode);

FUNC(void, WDG_CODE) Wdg_SetTriggerCondition (uint16 timeout);

FUNC(void, WDG_CODE) Wdg_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, WDG_APPL_DATA) versioninfo);

FUNC(void, WDG_CODE) Wdg_Cbk_GptNotification (void);


#endif /* WDG_H */

/**********************************************************************************************************************
 *  END OF FILE: Wdg.h
 **********************************************************************************************************************/
