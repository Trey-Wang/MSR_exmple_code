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
 *         File:  Pwm.h
 *    Component:  DrvPwm_VttCanoe01Asr 
 *       Module:  DrvPwm
 *    Generator:  -
 *
 *   Description: Module for Pulse Width Modulation
 *
 *********************************************************************************************************************/

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
 *  01.00.00  2014-01-30  virchl     -          Ported Pwm driver from DrvPwm_CanoeemuCanoe01Asr Ver. 2.00.02
 *  01.00.01  2014-05-16  virchl     -          Makro adaption in function Pwm_IsrHandler()
 *            2014-06-11  virchl  ESCAN00076207 Renaming of the Isrs
 *  02.00.00  2015-01-22  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Pwm_Init (if VTTOnly-SIP)
 *                                              Pwm_Init renamed to Pwm_Init_Vtt and a define in Pwm_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *                                ESCAN00083486 Number of max PwmChannels and ISRs extended to 50
 *  02.03.00  2016-02-25  virchl  ESCAN00088379 Update of SwVersions due to increase of generator version
 *  02.04.00  2016-09-05  virchl  ESCAN00091738 Usage of CAT2 ISRs 
 *  02.05.00  2017-01-19  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update
 *********************************************************************************************************************/

#if !defined PWM_H
#define PWM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define DRVPWM_VTTCANOE01ASR_VERSION            (0x0205u)
#define DRVPWM_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define PWM_VENDOR_ID                           (30u)
#define PWM_MODULE_ID                           (121u)
#define PWM_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define PWM_AR_RELEASE_MAJOR_VERSION            (4u)
#define PWM_AR_RELEASE_MINOR_VERSION            (0u)
#define PWM_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define PWM_SW_MAJOR_VERSION                    (DRVPWM_VTTCANOE01ASR_VERSION >> 8)
#define PWM_SW_MINOR_VERSION                    (DRVPWM_VTTCANOE01ASR_VERSION & 0x00FF)
#define PWM_SW_PATCH_VERSION                    (DRVPWM_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define PWM_SID_INIT                            (0x00u)
#define PWM_SID_DE_INIT                         (0x01u)
#define PWM_SID_SET_DUTY_CYCLE                  (0x02u)
#define PWM_SID_SET_PERIOD_AND_DUTY             (0x03u)
#define PWM_SID_SET_OUTPUT_TO_IDLE              (0x04u)
#define PWM_SID_GET_OUTPUT_STATE                (0x05u)
#define PWM_SID_DISABLE_NOTIFICATION            (0x06u)
#define PWM_SID_ENABLE_NOTIFICATION             (0x07u)
#define PWM_SID_GET_VERSION_INFO                (0x08u)

/* ----- Error codes ----- */
#define PWM_E_PARAM_CONFIG                      (0x10u)   /* API Pwm_Init service called with wrong parameter */
#define PWM_E_UNINIT                            (0x11u)   /* API service used without module initialization */
#define PWM_E_PARAM_CHANNEL                     (0x12u)   /* API service used with an invalid channel Identifier */
#define PWM_E_PERIOD_UNCHANGEABLE               (0x13u)   /* Usage of unauthorized PWM service on PWM channel configured 
                                                           * a fixed period 
                                                           */
#define PWM_E_ALREADY_INITIALIZED               (0x14u)   /* API Pwm_Init service called while the PWM driver has 
                                                           * already been initialised 
                                                           */
#define PWM_E_PARAM_POINTER                     (0x15u)   /* API is called with a NULL parameter */

#define PWM_E_PARAM_VINFO                       (0x16u)   /* API Pwm_GetVersionInfo is called with a NULL parameter */
#define PWM_E_PERIOD_ZERO                       (0x17u)   /* API is called with a period value of zero */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 *********************************************************************************************************************/
#include "Pwm_Cfg.h"
#include "VttCntrl_Pwm.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, PWM_CODE) Pwm_InitMemory (void);

FUNC(void, PWM_CODE) Pwm_Init_Vtt (P2CONST(Pwm_ConfigType, AUTOMATIC, PWM_APPL_CONST) ConfigPtr);

#if (STD_ON == PWM_DE_INIT_API)
  FUNC(void, PWM_CODE) Pwm_DeInit (void);
#endif

#if (STD_ON == PWM_SET_DUTY_CYCLE_API  )
  FUNC(void, PWM_CODE) Pwm_SetDutyCycle (Pwm_ChannelType ChannelNumber, uint16 DutyCycle);
#endif

#if (STD_ON == PWM_SET_PERIOD_AND_DUTY_API)
  FUNC(void, PWM_CODE) Pwm_SetPeriodAndDuty (Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle);
#endif

#if (STD_ON == PWM_SET_OUTPUT_TO_IDLE_API)
  FUNC(void, PWM_CODE) Pwm_SetOutputToIdle (Pwm_ChannelType ChannelNumber);
#endif

#if (STD_ON == PWM_GET_OUTPUT_STATE_API)
  FUNC(Pwm_OutputStateType, PWM_CODE) Pwm_GetOutputState (Pwm_ChannelType ChannelNumber);
#endif

#if (STD_ON == PWM_VERSION_INFO_API)
  FUNC(void, PWM_CODE) Pwm_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, PWM_APPL_DATA) versioninfo);
#endif

#if (STD_ON == PWM_NOTIFICATION_SUPPORTED)
  FUNC(void, PWM_CODE) Pwm_DisableNotification (Pwm_ChannelType ChannelNumber);
#endif

#if (STD_ON == PWM_NOTIFICATION_SUPPORTED)
FUNC(void, PWM_CODE) Pwm_EnableNotification (Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification);
#endif


#endif /* PWM_H */


/**********************************************************************************************************************
 *  END OF FILE: Pwm.h
 *********************************************************************************************************************/
