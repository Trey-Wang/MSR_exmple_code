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
 *         File:  Gpt.h
 *      Project:  DrvGpt_VttCanoe01Asr
 *       Module:  MICROSAR Gpt
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Gpt implements a general purpose timer driver.
 *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Peter  Lang                   virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-09-20  virlg   ESCAN00068489 Initial Development
 *  01.01.00  2014-3-14   virlg                 Modification of the ISR Handling
 *  01.01.01  2014-05-15  virchl                version number updated because of of 00074741
 *  01.01.02  2014-08-26  virchl                Number of GPT ISRs enlarge to 20 (0:19)
 *                                              Modification of implementation because of an defect
 *  02.00.00  2015-01-21  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Gpt_Init (if VTTOnly-SIP)
 *                                              Adc_Init renamed to Adc_Init_Vtt and a define in Gpt_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *                                ESCAN00083485 Extended Number of Available Channels (1:50)
 *  02.03.00  2016-02-25  virchl  ESCAN00088374 Update of SwVersions due to increase of generator version
 *  02.04.00  2016-09-05  virchl  ESCAN00091736 Usage of CAT2 ISRs 
 *  02.05.00  2017-01-12  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 **********************************************************************************************************************/

#if !defined GPT_H
#define GPT_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVGPT_VTTCANOE01ASR_VERSION            (0x0205u)
#define DRVGPT_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define GPT_VENDOR_ID                           (30u)
#define GPT_MODULE_ID                           (100u)
#define GPT_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define GPT_AR_RELEASE_MAJOR_VERSION            (4u)
#define GPT_AR_RELEASE_MINOR_VERSION            (0u)
#define GPT_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define GPT_SW_MAJOR_VERSION                    (DRVGPT_VTTCANOE01ASR_VERSION >> 8)
#define GPT_SW_MINOR_VERSION                    (DRVGPT_VTTCANOE01ASR_VERSION & 0x00FF)
#define GPT_SW_PATCH_VERSION                    (DRVGPT_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define GPT_SID_GET_VERSION_INFO                (0x00u)
#define GPT_SID_INIT                            (0x01u)
#define GPT_SID_DEINIT                          (0x02u)
#define GPT_SID_GET_TIME_ELAPSED                (0x03u)
#define GPT_SID_GET_TIME_REMAINING              (0x04u)
#define GPT_SID_START_TIMER                     (0x05u)
#define GPT_SID_STOP_TIMER                      (0x06u)
#define GPT_SID_ENABLE_NOTIFICATION             (0x07u)
#define GPT_SID_DISABLE_NOTIFICATION            (0x08u)
#define GPT_SID_SET_MODE                        (0x09u)
#define GPT_SID_DISABLE_WAKEUP                  (0x0Au)
#define GPT_SID_ENABLE_WAKEUP                   (0x0Bu)
#define GPT_SID_CHECK_WAKEUP                    (0x0Cu)

/* ----- Error codes ----- */
#define GPT_E_UNINIT                            (0x0Au)   /* API service used without init     */
#define GPT_E_BUSY                              (0x0Bu)   /* Timer is already running          */
#define GPT_E_ALREADY_INITIALIZED               (0x0Du)   /* Gpt_Init called when already initialized */
#define GPT_E_PARAM_CHANNEL                     (0x14u)   /* API called with invalid channel   */
#define GPT_E_PARAM_VALUE                       (0x15u)   /* Invalid start timer parameter     */
#define GPT_E_PARAM_MODE                        (0x1Fu)   /* Wrong mode parameter              */
#define GPT_E_PARAM_CONFIG                      (0x21u)   /* Gpt_Init called with NULL_PTR     */
#define GPT_E_PARAM_VINFO                       (0x22u)   /* NULL_PTR passed to Gpt_GetVersionInfo */


/* Channel's wakeup capability */
#define GPT_CFG_ENABLE_WAKEUP                   (0x01u)
#define GPT_CFG_DISABLE_WAKEUP                  (0x00u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* A call back prototype */
typedef P2FUNC(void, GPT_APPL_CODE, Gpt_NotifyType) (void);

/* type describing the timeout value */
typedef uint32 Gpt_ValueType;

/* type describing the channel ID */
typedef uint8 Gpt_ChannelType;

/* List of process modes */
typedef enum
{
  GPT_MODE_NORMAL = 0,
  GPT_MODE_SLEEP
} Gpt_ModeType ;

typedef enum
{ 
  GPT_CH_MODE_CONTINUOUS = 0,
  GPT_CH_MODE_ONESHOT
} Gpt_ChannelModeType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Gpt_Cfg.h"
#include "VttCntrl_Gpt.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, GPT_CODE) Gpt_InitMemory(void);

FUNC(void, GPT_CODE) Gpt_Init_Vtt (P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST) ConfigPtr);

#if ( STD_ON == GPT_DEINIT_API )
  FUNC(void, GPT_CODE) Gpt_DeInit (void);
#endif

#if ( STD_ON == GPT_TIME_ELAPSED_API )
  FUNC(Gpt_ValueType, GPT_CODE) Gpt_GetTimeElapsed (Gpt_ChannelType channel);
#endif

#if ( STD_ON == GPT_TIME_REMAINING_API )
  FUNC(Gpt_ValueType, GPT_CODE) Gpt_GetTimeRemaining (Gpt_ChannelType channel);
#endif

FUNC(void, GPT_CODE) Gpt_StartTimer (Gpt_ChannelType channel, Gpt_ValueType value);

FUNC(void, GPT_CODE) Gpt_StopTimer (Gpt_ChannelType channel );

#if ( STD_ON == GPT_ENABLE_DISABLE_NOTIFICATION_API )
  FUNC(void, GPT_CODE) Gpt_EnableNotification (Gpt_ChannelType channel);
  
  FUNC(void, GPT_CODE) Gpt_DisableNotification (Gpt_ChannelType channel);
#endif

#if ( STD_ON == GPT_WAKEUP_FUNCTIONALITY_API )
  FUNC(void, GPT_CODE) Gpt_SetMode (Gpt_ModeType mode);
  
  FUNC(void, GPT_CODE) Gpt_DisableWakeup (Gpt_ChannelType channel);
 
  FUNC(void, GPT_CODE) Gpt_EnableWakeup (Gpt_ChannelType channel);
  
  FUNC(void, GPT_CODE) Gpt_CheckWakeup (EcuM_WakeupSourceType WakeupSource);
#endif

#if ( STD_ON == GPT_VERSION_INFO_API)
  FUNC(void, GPT_CODE) Gpt_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, GPT_APPL_DATA) versioninfo);
#endif


#endif /* GPT_H */


/**********************************************************************************************************************
 *  END OF FILE: Gpt.h
 **********************************************************************************************************************/

