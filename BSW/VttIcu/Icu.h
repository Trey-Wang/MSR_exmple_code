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
 *         File:  Icu.h
 *      Project:  DrvIcu_VttCanoe01Asr
 *       Module:  MICROSAR Icu
 *
 *  Description:  The ICU driver is a module using the input capture unit (ICU) for demodulation of a PWM signal,
 *                counting pulses, measuring of frequency and duty cycle, generating simple interrupts and wakeup
 *                interrupts. The ICU driver provides services for
 *                -    Signal edge notification
 *                -    Controlling wakeup interrupts
 *                -    Periodic signal time measurement
 *                -    Edge timestamping, usable for the acquisition of non-periodic signals
 *                -    Edge counting
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
 *  01.00.00  2014-01-09  virchl      -         Ported Icu driver from DrvIcu_CanoeemuCanoe01Asr Ver. 03.00.01 to Vip
 *  01.00.01  2014-05-06  virchl  ESCAN00075257 Macros removed
 *            2014-06-11  virchl  ESCAN00076204 Renaming of the Isrs - mo modification here
 *  01.00.02  2014-12-04  virchl  ESCAN00080013 Range of IcuChannelId set to 0:31 and modification of all impacts
 *  02.00.00  2015-01-21  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Icu_Init (if VTTOnly-SIP)
 *                                              Icu_Init renamed to Icu_Init_Vtt and a define in Icu_Cfg.h reroutes it
 *                                              Preprocessor for static variable Icu_WakeupActive added
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *                                ESCAN00083487 Number of  max IcuChannels extended to 50
 *  02.02.00  2015-12-10  virchl  ESCAN00087077 Parameter of EcuM_CheckWakeup fixed according AUTOSAR specification
 *  02.03.00  2016-02-25  virchl  ESCAN00088376 Update of SwVersions due to increase of generator version
 *  02.03.01  2016-08-01  virchl  ESCAN00091214 Constraint added for definition of local variable in Icu_SetMode()
 *                                ESCAN00091201 NULL_PTR check added before notification calls
 *  02.04.00  2016-09-05  virchl  ESCAN00091737 Usage of CAT2 ISRs 
 *  02.05.00  2017-01-13  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 *********************************************************************************************************************/

#if !defined ICU_H
#define ICU_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Icu_Cfg.h"
#include "VttCntrl_Icu.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define DRVICU_VTTCANOE01ASR_VERSION            (0x0205u)
#define DRVICU_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define ICU_VENDOR_ID                           (30u)
#define ICU_MODULE_ID                           (122u)
#define ICU_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define ICU_AR_RELEASE_MAJOR_VERSION            (4u)
#define ICU_AR_RELEASE_MINOR_VERSION            (0u)
#define ICU_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define ICU_SW_MAJOR_VERSION                    (DRVICU_VTTCANOE01ASR_VERSION >> 8u)
#define ICU_SW_MINOR_VERSION                    (DRVICU_VTTCANOE01ASR_VERSION & 0x00FF)
#define ICU_SW_PATCH_VERSION                    (DRVICU_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define ICU_SID_INIT                            (0x00u)
#define ICU_SID_DE_INIT                         (0x01u)
#define ICU_SID_SET_MODE                        (0x02u)
#define ICU_SID_DISABLE_WAKEUP                  (0x03u)
#define ICU_SID_ENABLE_WAKEUP                   (0x04u)
#define ICU_SID_SET_ACTIVATION_CONDITION        (0x05u)
#define ICU_SID_DISABLE_NOTIFICATION            (0x06u)
#define ICU_SID_ENABLE_NOTIFICATION             (0x07u)
#define ICU_SID_GET_INPUT_STATE                 (0x08u)
#define ICU_SID_START_TIMESTAMP                 (0x09u)
#define ICU_SID_STOP_TIMESTAMP                  (0x0Au)
#define ICU_SID_GET_TIMESTAMP_INDEX             (0x0Bu)
#define ICU_SID_RESET_EDGE_COUNT                (0x0Cu)
#define ICU_SID_ENABLE_EDGE_COUNT               (0x0Du)
#define ICU_SID_DISABLE_EDGE_COUNT              (0x0Eu)
#define ICU_SID_GET_EDGE_NUMBERS                (0x0Fu)
#define ICU_SID_GET_TIME_ELAPSED                (0x10u)
#define ICU_SID_GET_DUTY_CYCLE_VALUES           (0x11u)
#define ICU_SID_GET_VERSION_INFO                (0x12u)
#define ICU_SID_START_SIG_MEASUREMENT           (0x13u)
#define ICU_SID_STOP_SIG_MEASUREMENT            (0x14u)
#define ICU_SID_CHECK_WAKEUP                    (0x15u)
#define ICU_SID_ENABLE_EDGE_DETECTION           (0x16u)
#define ICU_SID_DISABLE_EDGE_DETECTION          (0x17u)

/* ----- Error codes ----- */
#define ICU_E_PARAM_CONFIG                      (0x0Au)   /* API Icu_Init service called with wrong parameter */
#define ICU_E_PARAM_CHANNEL                     (0x0Bu)   /* API service used with an invalid channel identifier or 
                                                           * channel was not configured for the functionality of the 
                                                           * calling API 
                                                           */
#define ICU_E_PARAM_ACTIVATION                  (0x0Cu)   /* API service used with an invalid activation */
#define ICU_E_PARAM_BUFFER_PTR                  (0x0Du)   /* API service used with an invalid buffer pointer */
#define ICU_E_PARAM_BUFFER_SIZE                 (0x0Eu)   /* API service used with an invalid buffer size */
#define ICU_E_PARAM_MODE                        (0x0Fu)   /* API service Icu_SetMode used with an invalid mode */
#define ICU_E_UNINIT                            (0x14u)   /* API service used without module initialization */
#define ICU_E_NOT_STARTED                       (0x15u)   /* API service Icu_StopTimestamp called on a channel which was
                                                           * not started or already stopped
                                                           */
#define ICU_E_BUSY_OPERATION                    (0x16u)   /* API service Icu_SetMode called while running operation */
#define ICU_E_ALREADY_INITIALIZED               (0x17u)   /* API Icu_Init service called when already initialized */
#define ICU_E_PARAM_NOTIFY_INTERVAL             (0x18u)   /* API service used with invalid notify interval in timestamp
                                                           * mode 
                                                           */
#define ICU_E_PARAM_VINFO                       (0x19u)   /* API service Icu_GetVersionInfo() called with NULL_PTR */


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Allow enabling / disabling of all interrupts which are not required for the ECU wakeup. */
typedef enum
{
  ICU_MODE_NORMAL = 0,
  ICU_MODE_SLEEP
} Icu_ModeType;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, ICU_CODE) Icu_InitMemory (void);

FUNC(void, ICU_CODE) Icu_Init_Vtt (P2CONST( Icu_ConfigType, AUTOMATIC, ICU_APPL_CONST) ConfigPtr);

#if (ICU_DE_INIT_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_DeInit (void);
#endif

#if (ICU_SET_MODE_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_SetMode (Icu_ModeType Mode);
#endif

#if (ICU_DISABLE_WAKEUP_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_DisableWakeup (Icu_ChannelType Channel);
#endif

#if (ICU_ENABLE_WAKEUP_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_EnableWakeup (Icu_ChannelType Channel);
#endif

FUNC(void, ICU_CODE) Icu_SetActivationCondition (Icu_ChannelType Channel, Icu_ActivationType Activation);

FUNC(void, ICU_CODE) Icu_DisableNotification(Icu_ChannelType Channel);

FUNC(void, ICU_CODE) Icu_EnableNotification(Icu_ChannelType Channel);

#if (ICU_GET_INPUT_STATE_API == STD_ON)
  FUNC(Icu_InputStateType, ICU_CODE) Icu_GetInputState(Icu_ChannelType Channel);
#endif


#if (ICU_TIMESTAMP_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_StartTimestamp
  (
    Icu_ChannelType Channel,
    P2VAR(Icu_ValueType, ICU_VAR, ICU_APPL_DATA) BufferPtr,
    uint16          BufferSize,
    uint16          NotifyInterval
  );

  FUNC(void, ICU_CODE) Icu_StopTimestamp (Icu_ChannelType Channel);
  
  FUNC(Icu_IndexType, ICU_CODE) Icu_GetTimestampIndex (Icu_ChannelType Channel);
#endif


#if (ICU_EDGE_COUNT_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_ResetEdgeCount (Icu_ChannelType Channel);

  FUNC(void, ICU_CODE) Icu_EnableEdgeCount (Icu_ChannelType Channel);

  FUNC(void, ICU_CODE) Icu_DisableEdgeCount (Icu_ChannelType Channel);

  FUNC(Icu_EdgeNumberType, ICU_CODE) Icu_GetEdgeNumbers (Icu_ChannelType Channel);
#endif

#if (ICU_EDGE_DETECT_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_EnableEdgeDetection (Icu_ChannelType Channel);
  
  FUNC(void, ICU_CODE) Icu_DisableEdgeDetection (Icu_ChannelType Channel);
#endif

#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_StartSignalMeasurement (Icu_ChannelType Channel);
  
  FUNC(void, ICU_CODE) Icu_StopSignalMeasurement (Icu_ChannelType Channel);
#endif

#if (ICU_GET_TIME_ELAPSED_API == STD_ON)
  FUNC(Icu_ValueType, ICU_CODE) Icu_GetTimeElapsed (Icu_ChannelType Channel);
#endif


#if (ICU_GET_DUTY_CYCLE_VALUES_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_GetDutyCycleValues
  (
    Icu_ChannelType     Channel,
    P2VAR(Icu_DutyCycleType, ICU_VAR, ICU_APPL_DATA) DutyCycleValues
  );
#endif


#if (ICU_GET_VERSION_INFO_API == STD_ON)
  FUNC(void, ICU_CODE) Icu_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, ICU_APPL_DATA) versioninfo);
#endif

#if(ICU_REPORT_WAKEUP_SOURCE == STD_ON)
  FUNC(void, ICU_CODE) Icu_CheckWakeup (EcuM_WakeupSourceType WakeupSource);
#endif


#endif /* ICU_H */

/**********************************************************************************************************************
 *  END OF FILE: Icu.h
 *********************************************************************************************************************/
