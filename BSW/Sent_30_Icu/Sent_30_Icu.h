/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Sent_30_Icu.h
 *        \brief  Header file of the Vector Sent_30_Icu Handler
 *
 *      \details  The Sent_30_Icu handler is a software component which is meant to enable usage of SENT sensors on
 *                Vector ECUs. The SWC assists the application layer / sensor driver by providing the raw data which
 *                is received from the sensor.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Denis Althapp                 virdea        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  00.00.01  2015-05-12  virdea  -             Initial version
 *  00.01.00  2015-05-22  virdea  -             First working version for fast channel messages
 *  00.02.00  2015-05-27  virdea  -             Implemented slow channel functionality, added processing with a channel
 *                                              notification
 *  00.03.00  2015-05-28  virdea  -             Small bugfixes and more descriptions, FastChan+Both Slowchan modes
 *                                              tested and working
 *  00.04.00  2015-05-29  virdea  -             Bugfix for less than 6 data nibbles+pause pulse
 *  00.05.00  2015-05-29  virdea  -             Added GetVersionInfo, ASR3+4 support, DET
 *  00.06.00  2015-07-31  virdea  -             Added SENT_30_ICU_SOURCE define, reorganized init variables, fixed typo
 *                                              in compiler abstraction, added getter API instead of global variables
 *                                              and a notification function
 *  00.07.00  2015-08-05  virdea  -             Several QAC fixes
 *  01.00.00  2015-10-09  virdea  -             Added additional diagnostic features (ENABLE_DIAGNOSIS)
 *  
*********************************************************************************************************************/


#ifndef SENT_30_ICU_H
#define SENT_30_ICU_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Sent_30_Icu_Cfg.h"
#include "Icu.h"
#if (SENT_30_ICU_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"       /* Include Development Error Tracer */
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
#define SENT_30_ICU_VENDOR_ID                           (30u)
#define SENT_30_ICU_MODULE_ID                           (0x8001u)
#define SENT_30_ICU_INSTANCE_ID                         (0x00u)

/* AUTOSAR Software specification version information */
#define SENT_30_ICU_AR_RELEASE_MAJOR_VERSION            (0x04u)
#define SENT_30_ICU_AR_RELEASE_MINOR_VERSION            (0x00u)
#define SENT_30_ICU_AR_RELEASE_REVISION_VERSION         (0x03u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define SENT_30_ICU_SW_MAJOR_VERSION                    (1u)
#define SENT_30_ICU_SW_MINOR_VERSION                    (0u)
#define SENT_30_ICU_SW_PATCH_VERSION                    (0u)

#define SENT_30_ICU_INSTANCE_ID_DET                     (SENT_30_ICU_INSTANCE_ID)

/* ----- API service IDs ----- */
/*! \brief Service ID: Sent_30_Icu_Init() */
#define SENT_30_ICU_SID_INIT                            (0x00u)
/*! \brief Service ID: Sent_30_Icu_DeInit() */
#define SENT_30_ICU_SID_DEINIT                          (0x01u)
/*! \brief Service ID: Sent_30_Icu_IcuChannelCallback() */
#define SENT_30_ICU_SID_ICUCALLBACK                     (0x02u)
/*! \brief Service ID: Sent_30_Icu_MainFunction() */
#define SENT_30_ICU_SID_MAINFUNCTION                    (0x03u)
/*! \brief Service ID: Sent_30_Icu_GetVersionInfo() */
#define SENT_30_ICU_SID_GET_VERSION_INFO                (0x04u)
/*! \brief Service ID: Sent_30_Icu_GetFastChannelData() */
#define SENT_30_ICU_SID_GET_FASTCHANNEL_DATA            (0x05u)
/*! \brief Service ID: Sent_30_Icu_GetSlowChannelData() */
#define SENT_30_ICU_SID_GET_SLOWCHANNEL_DATA            (0x06u)

/* ----- Error codes ----- */
#define SENT_30_ICU_E_NULL_POINTER                      (0x00u)
#define SENT_30_ICU_E_NOT_INITIALIZED                   (0x01u)
#define SENT_30_ICU_E_ALREADY_INITIALIZED               (0x02u)
#define SENT_30_ICU_E_INVALID_CFG_SLOW_CH_MODE          (0x03u)
#define SENT_30_ICU_E_INVALID_CFG_TICK_TIME             (0x04u)
#define SENT_30_ICU_E_INVALID_CFG_CLOCK_TICKS           (0x05u)
#define SENT_30_ICU_E_INVALID_CFG_DATA_NIBBLES          (0x06u)

/* ----- Mapping of error reporting macro ----- */
#if (SENT_30_ICU_DEV_ERROR_DETECT == STD_ON)
#define Sent_30_Icu_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE) \
  {  \
    if (!(CONDITION)) \
    {  \
      (void)Det_ReportError((uint16)SENT_30_ICU_MODULE_ID, (uint8)SENT_30_ICU_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)); \
      return; \
    } \
  } /* PRQA S 3458, 3109 */ /* MD_MSR_19.4, MD_MSR_14.3 */
#define Sent_30_Icu_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL) \
  { \
    if (!(CONDITION)) \
    { \
      (void)Det_ReportError((uint16)SENT_30_ICU_MODULE_ID, (uint8)SENT_30_ICU_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)); \
      return (RET_VAL); \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#else
#define Sent_30_Icu_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE)
#define Sent_30_Icu_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL)
#endif

/* ----- Modes ----- */
# define SENT_30_ICU_M_UNINIT                           (0x00u)
# define SENT_30_ICU_M_INIT                             (0x01u)

/* ----- States ----- */
# define SENT_30_ICU_UNINIT                             (0x00u)
# define SENT_30_ICU_CALIBRATING                        (0x01u)
# define SENT_30_ICU_CALIBRATION_FOUND                  (0x02u)
# define SENT_30_ICU_SKIP_PAUSE_PULSE                   (0x03u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! \brief Basic SENT data type containing raw data from the fast channel without any metadata */
typedef struct
{
    uint32 FastData;
    uint8 StatusNibble;
} Sent_30_Icu_FastDataType;

#if (SENT_30_ICU_SERIAL_FORMAT == SENT_30_ICU_SHORT_SERIAL_MESSAGE) /* PRQA S 883 */ /* MD_SENT_883 */
 /*! \brief Basic SENT structure containing raw data from the slow channel */
typedef struct
{
  uint8 Data;
  uint8 MsgId;
} Sent_30_Icu_SlowDataType;

#elif (SENT_30_ICU_SERIAL_FORMAT == SENT_30_ICU_ENHANCED_SERIAL_MESSAGE)
/*! \brief Basic SENT structure containing raw data from the slow channel */
typedef struct
{
  uint16 Data;
  uint8 MsgId;
} Sent_30_Icu_SlowDataType;

#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
/*! \brief Sent diagnostic data type */
typedef struct
{
    /*! \brief The amount of valid fast frames received */
    uint32 ValidFastFramesReceived;
    /*! \brief The current correction factor of the frame */
    uint32 CorrectionFactor;
    /*! \brief The amount of valid fast frames received in succession */
    uint16 ValidFastFramesSuccession;
    /*! \brief The amount of invalid calibration pulses */
    uint8 InvalidCalibPulses;
    /*! \brief The amount of frames with an invalid CRC value */
    uint8 InvalidCrcFrames;
    /*! \brief The amount of invalid nibbles */
    uint8 InvalidNibbles;
    /*! \brief The amount of frames with an invalid nibble amount */
    uint8 InvalidNibCounts;
} Sent_30_Icu_DiagDataType;
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SENT_30_ICU_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Service functions */
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_InitMemory(void);
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_Init(void);
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_DeInit(void);
#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_IcuChannelCallback(void);
#else
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_MainFunction(void);
#endif

FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetFastChannelData
        (
        P2VAR(Sent_30_Icu_FastDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
        );

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || \
      (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetSlowChannelData
        (
        P2VAR(Sent_30_Icu_SlowDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
        );
#endif

#if (STD_ON == SENT_30_ICU_VERSION_INFO_API)
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetVersionInfo
        (
        P2VAR(Std_VersionInfoType, AUTOMATIC, SENT_30_ICU_APPL_DATA) versioninfo
        );
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetDiagnosticValues
        (
        P2VAR(Sent_30_Icu_DiagDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
        );
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_ResetDiagnosticValues(void);
#endif

#if (STD_ON == SENT_30_ICU_USE_NOTIFICATION)
FUNC(void, SENT_30_ICU_APPL_CODE) SENT_30_ICU_NOTIFICATION_FUNC_FASTCH(void);
FUNC(void, SENT_30_ICU_APPL_CODE) SENT_30_ICU_NOTIFICATION_FUNC_SLOWCH(void);
#endif

#define SENT_30_ICU_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */


#endif /* SENT_30_ICU_H */

/**********************************************************************************************************************
 *  END OF FILE: Sent_30_Icu.h
 *********************************************************************************************************************/
