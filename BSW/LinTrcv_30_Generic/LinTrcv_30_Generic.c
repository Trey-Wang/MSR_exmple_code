/**********************************************************************************************************************
 * COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 * Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *               This software is copyright protected and proprietary to Vector Informatik GmbH.
 *               Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *               All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 * FILE DESCRIPTION
 * -------------------------------------------------------------------------------------------------------------------
 *         File:  LinTrcv_30_Generic.c
 *      Project:  MICROSAR
 *       Module:  LIN Transceiver Driver
 *  Description:  Implementation of the MICROSAR LIN Transceiver Driver
 **********************************************************************************************************************/

#define LINTRCV_30_GENERIC_SOURCE

/**********************************************************************************************************************
 * MISRA
 *********************************************************************************************************************/
/* Disable the following MISRA warnings because they would appear too often for individual suppression.
 * See justification at the end of file */

/* PRQA S 0850 EOF */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#define LINTRCV_30_GENERIC_INCLUDE_INTERNAL
#include "LinIf.h"
#include "LinTrcv_30_Generic.h"
#include "Schm_LinTrcv_30_Generic.h"

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
# include "EcuM_Cbk.h"
#endif

#if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON)
# include "Icu.h"
#endif

#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
# include "LinIf_Cbk.h"
#endif

/* ADD YOUR ADDITIONAL INCLUDES HERE */

/* DO NOT CHANGE THE CODE BELOW */

/***********************************************************************************************************************
 * VERSION CHECK
 **********************************************************************************************************************/
#if ( DRVTRANS_GENERICDIOASR_VERSION != 0x0501u )  /* COV_LINTRCV_CONFIG XF */
# error "Module LinTrcv_30_Generic: Source and Header files are inconsistent!"
#endif
#if ( DRVTRANS_GENERICDIOASR_RELEASE_VERSION != 0x00u )  /* COV_LINTRCV_CONFIG XF */
# error "Module LinTrcv_30_Generic: Source and Header files are inconsistent!"
#endif
#if (DRVTRANS_LIN_30_GENERIC_GENTOOL_CFG5_MAJOR_VERSION != 0x06U) /* COV_LINTRCV_CONFIG XF */
# error "Module LinTrcv_30_Generic: Static and Generated files are inconsistent!"
#endif
#if (DRVTRANS_LIN_30_GENERIC_GENTOOL_CFG5_MINOR_VERSION != 0x01U) /* COV_LINTRCV_CONFIG XF */
# error "Module LinTrcv_30_Generic: Static and Generated files are inconsistent!"
#endif

/***********************************************************************************************************************
 * CONFIGURATION CHECK
 **********************************************************************************************************************/
#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
# if (LINTRCV_30_GENERIC_WAKEUPBYBUSUSEDOFCHANNEL == STD_ON) /* COV_LINTRCV_CONFIG XF */
# elif (LINTRCV_30_GENERIC_WAKEUPBYPINUSEDOFCHANNEL == STD_ON) /* COV_LINTRCV_CONFIG XX XF */
# else
#  error "The EcuM wakeup source of the LinTrcv channel should be enabled only if wakeup by bus or/and pin is enabled for same the channel"
# endif
#endif

#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
# if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON) /* COV_LINTRCV_CONFIG XF */
# else
#  error "LinTrcvWakeUpSupport in LinTrcvGeneral must be enabled if wakeup by busor/and pin is enabled for any channel"
# endif
#endif

#if ! defined (LINTRCV_30_GENERIC_DUMMY_STATEMENT)
# error "The define LINTRCV_30_GENERIC_DUMMY_STATEMENT shall be defined either in Compiler_Cfg.h or as parameter for the Compiler!"
#endif

#if ! defined (LINTRCV_30_GENERIC_LOCAL) /* COV_LINTRCV_COMPATIBILITY */
# define LINTRCV_30_GENERIC_LOCAL                         static
#endif

#if ! defined (LINTRCV_30_GENERIC_LOCAL_INLINE) /* COV_LINTRCV_COMPATIBILITY */
# define LINTRCV_30_GENERIC_LOCAL_INLINE                  LOCAL_INLINE
#endif

#if ! defined (LINTRCV_30_GENERIC_INSTANCE_ID_DET) /* COV_LINTRCV_COMPATIBILITY */
# define LINTRCV_30_GENERIC_INSTANCE_ID_DET               LINTRCV_30_GENERIC_INSTANCE_ID
#endif

/**********************************************************************************************************************
 * LOCAL DEFINES
 **********************************************************************************************************************/
#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
# define LinTrcv_30_Generic_Dev_InitValue                 ((uint8)0x00u)
# define LinTrcv_30_Generic_Dev_CheckValue                ((uint8)0xA5u)
#endif

/**********************************************************************************************************************
 * LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
/* ADD YOUR ADDITIONAL LOCAL CONSTANT MACROS HERE */

/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* ADD YOUR ADDITIONAL LOCAL FUNCTION MACROS HERE */

/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 * LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* ADD YOUR ADDITIONAL LOCAL DATA TYPES AND STRUCTURES HERE */

/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
# define LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* Variable LinTrcv_30_Generic_Dev_InitDetect must be initialized after reset for working Debug check */
LINTRCV_30_GENERIC_LOCAL VAR(uint8, LINTRCV_30_GENERIC_VAR_ZERO_INIT) LinTrcv_30_Generic_Dev_InitDetect = LinTrcv_30_Generic_Dev_InitValue;
# define LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#if (LINTRCV_30_GENERIC_USE_INIT_POINTER == STD_ON)
# define LINTRCV_30_GENERIC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* Pointer to Global Configuration */
LINTRCV_30_GENERIC_LOCAL P2CONST(LinTrcv_30_Generic_ConfigType, LINTRCV_30_GENERIC_VAR_ZERO_INIT, LINTRCV_30_GENERIC_INIT_DATA) LinTrcv_30_Generic_ConfigDataPtr = NULL_PTR;
# define LINTRCV_30_GENERIC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/* ADD YOUR ADDITIONAL LOCAL DATA PROTOTYPES HERE */

/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 * GLOBAL DATA
 **********************************************************************************************************************/
#define LINTRCV_30_GENERIC_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_MainVersion = (uint8)(LINTRCV_30_GENERIC_SW_MAJOR_VERSION);
CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_SubVersion = (uint8)(LINTRCV_30_GENERIC_SW_MINOR_VERSION);
CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_ReleaseVersion = (uint8)(LINTRCV_30_GENERIC_SW_PATCH_VERSION);
#define LINTRCV_30_GENERIC_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define LINTRCV_30_GENERIC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/
/* ADD YOUR ADDITIONAL LOCAL FUNCTION HERE */

/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/* ADD YOUR ADDITIONAL GLOBAL FUNCTIONS HERE */

/* DO NOT CHANGE THE CODE BELOW */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_InitMemory()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_InitMemory
(
  void
)
{
  /* PREINIT YOUR ADDITIONAL DATA HERE */

  /* DO NOT CHANGE THE CODE BELOW */

  /* #10 Set state to uninitialized */
#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  LinTrcv_30_Generic_Dev_InitDetect = LinTrcv_30_Generic_Dev_InitValue;
#endif

  /* #20 Uninitialize the local init pointer */
#if (LINTRCV_30_GENERIC_USE_INIT_POINTER == STD_ON)
  LinTrcv_30_Generic_ConfigDataPtr = NULL_PTR;
#endif
}

/***********************************************************************************************************************
 * LinTrcv_30_Generic_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_Init
(
  P2CONST(LinTrcv_30_Generic_ConfigType, AUTOMATIC, LINTRCV_30_GENERIC_INIT_DATA) ConfigPtr
)
{
  VAR(uint8, AUTOMATIC) LinTrcvIndex;
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;
  /* DECLARE YOUR ADDITIONAL VARIABLES HERE */

  /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if the component is already initialized */
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_InitValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
# if (LINTRCV_30_GENERIC_USE_INIT_POINTER == STD_ON)
  /* #20 Check the validity of the pointer ConfigPtr */
  else if (ConfigPtr == NULL_PTR)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_POINTER;
  }
# endif
  else
#endif
  {
#if (LINTRCV_30_GENERIC_USE_INIT_POINTER == STD_ON)
    LinTrcv_30_Generic_ConfigDataPtr = ConfigPtr;
#endif

    /* #25 Loop over all configured channels */
    for (LinTrcvIndex = 0x00u; LinTrcvIndex < LinTrcv_30_Generic_NrOfChannels; LinTrcvIndex++)
    {
      /* #30 If the channel is used */
      if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex))
      {
#if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
        /* #40 Enable Wakeup reporting for the channel, initialise WakeupReason value */
        LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupMode = LINTRCV_WUMODE_ENABLE; /* SBSW_LINTRCV_CHANNEL_DATA */
        LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_ERROR; /* SBSW_LINTRCV_CHANNEL_DATA */
#endif
        /* #50 Set the hardware OpMode according to the parameter LinTrcvInitState (normal or sleep) */
        switch(LinTrcv_30_Generic_GetInitStateOfChannel(LinTrcvIndex))
        {
          case LINTRCV_30_GENERIC_OP_MODE_NORMAL:
            /* INITIALIZE THE LINTRCV CHANNEL INTO NORMAL OPERATION MODE HERE */

            LinTrcv_ReturnValTmp = E_OK; /* Signal sucessful operation */
            /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON) || (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
            {
# if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON)
              /* #60 Normal mode successfully set: If Icu is used, disable Icu notification */
              if (LinTrcv_30_Generic_IsIcuChannelIdUsedOfChannel(LinTrcvIndex))
              {
                Icu_DisableNotification(LinTrcv_30_Generic_GetIcuChannelIdOfChannel(LinTrcvIndex));
              }
# endif
# if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
              /* #70 Set the WakeupReason to RESET */
              LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_RESET; /* SBSW_LINTRCV_CHANNEL_DATA */
# endif
            }
#endif
            break;

          case LINTRCV_30_GENERIC_OP_MODE_SLEEP:
            /* INITIALIZE THE LINTRCV CHANNEL INTO SLEEP OPERATION MODE HERE */

            LinTrcv_ReturnValTmp = E_OK; /* Signal sucessful operation */
            /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON)
            /* #80 Sleep mode successfully set: If Icu is used, enable Icu notification */
            {
              if (LinTrcv_30_Generic_IsIcuChannelIdUsedOfChannel(LinTrcvIndex))
              {
                Icu_EnableNotification(LinTrcv_30_Generic_GetIcuChannelIdOfChannel(LinTrcvIndex));
              }
            }
#endif
            break;

          default: /* PRQA S 2018 */ /* MD_MSR_14.1 */ /* COV_LINTRCV_MISRA */
            LinTrcv_ReturnValTmp = E_NOT_OK;
            break;
        }

        if (LinTrcv_ReturnValTmp == E_OK) /* COV_LINTRCV_CONFIG TX */
        {
#if (LINTRCV_30_GENERIC_WAIT_COUNT_USED == STD_ON)
          /* #90 Invoke the call-out function Appl_LinTrcv_30_Generic_Wait() */
          Appl_LinTrcv_30_Generic_Wait(LINTRCV_30_GENERIC_WAIT_COUNT);
#endif

          /* #100 Change the internal state to the initial operation mode */
          LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_State = LinTrcv_30_Generic_GetInitStateOfChannel(LinTrcvIndex); /* SBSW_LINTRCV_CHANNEL_DATA */
        }
        else
        {
          /* Break the init loop in case of any E_NOT_OK return */
          break;
        }
      }
    }
  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_INIT, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if(LinTrcv_ReturnValTmp == E_OK)
  {
    /* Initialization went through, set flag */
    LinTrcv_30_Generic_Dev_InitDetect = LinTrcv_30_Generic_Dev_CheckValue;
  }
#endif

#if (LINTRCV_30_GENERIC_USE_INIT_POINTER == STD_OFF)
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_SetOpMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_SetOpMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) OpMode
)
{
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

  /* DECLARE YOUR ADDITIONAL VARIABLES HERE */

  /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_CheckValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
  /* #10 Check the validity of the parameter LinTrcvIndex */
  else if (LinTrcvIndex >= LinTrcv_30_Generic_NrOfChannels)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex) != TRUE)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else
#endif
  {

    /* #20 Enter critical area */
    SchM_Enter_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();

    /* #30 Set OpMode according to the parameter OpMode (normal or sleep) */
    switch(OpMode)
    {

      case LINTRCV_30_GENERIC_OP_MODE_NORMAL:
        /* SET THE LINTRCV CHANNEL INTO NORMAL OPERATION MODE HERE */

        LinTrcv_ReturnValTmp = E_OK; /* Signal sucessful operation */
        /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON) || (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
        {
# if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON)
          /* #40 Normal mode sucessfully set: If Icu is used, disable Icu notification */
          if (LinTrcv_30_Generic_IsIcuChannelIdUsedOfChannel(LinTrcvIndex))
          {
            Icu_DisableNotification(LinTrcv_30_Generic_GetIcuChannelIdOfChannel(LinTrcvIndex));
          }
# endif
# if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
          /* #50 If Sleep->Normal transition */
          if (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_State == LINTRCV_30_GENERIC_OP_MODE_SLEEP)
          {
            /* #60 If no WakeupReason is stored: Set WakeupReason to internal */
#  if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
            /* This if statement is needed only when external wakeups are possible:
             * Unless external wakeups are possible, the LinTrcv channel was in sleep mode (WakeupReason == LINTRCV_WU_ERROR)
             * Making this if statement always true and useless. */
            if (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason == LINTRCV_WU_ERROR)
#  endif
            {
              LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_INTERNALLY; /* SBSW_LINTRCV_CHANNEL_DATA */
            }
          }
# endif
        }
#endif
        break;

      case LINTRCV_30_GENERIC_OP_MODE_SLEEP:
        /* SET THE LINTRCV CHANNEL INTO SLEEP OPERATION MODE HERE */

        LinTrcv_ReturnValTmp = E_OK; /* Signal sucessful operation */
        /* DO NOT CHANGE THE CODE BELOW */

#if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON) || (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
        {
# if (LINTRCV_30_GENERIC_ICUCHANNELIDOFCHANNEL == STD_ON)
          /* #70 Sleep mode sucessfully set: If Icu is used, enable Icu notification */
          if (LinTrcv_30_Generic_IsIcuChannelIdUsedOfChannel(LinTrcvIndex))
          {
            Icu_EnableNotification(LinTrcv_30_Generic_GetIcuChannelIdOfChannel(LinTrcvIndex));
          }
# endif
# if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
          /* #80 Clear WakeupReason */
          LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_ERROR; /* SBSW_LINTRCV_CHANNEL_DATA */
# endif
        }
#endif
        break;

      default:
#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
        LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_TRCV_OPMODE;
#endif
        break;
    }

    if(LinTrcv_ReturnValTmp == E_OK)
    {
      /* #90 Change the internal state to the requested operation mode */
      LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_State = OpMode; /* SBSW_LINTRCV_CHANNEL_DATA */
    }

    /* #100 Exit critical area */
    SchM_Exit_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();

#if (LINTRCV_30_GENERIC_WAIT_COUNT_USED == STD_ON)
    /* #110 Invoke the call-out function Appl_LinTrcv_30_Generic_Wait() */
    if(LinTrcv_ReturnValTmp == E_OK)
    {
      Appl_LinTrcv_30_Generic_Wait(LINTRCV_30_GENERIC_WAIT_COUNT);
    }
#endif

  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_SETOPMODE, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return LinTrcv_ReturnValTmp;
}  /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_GetOpMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetOpMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) OpMode
)
{
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_CheckValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
  /* #10 Check the validity of the parameter LinTrcvIndex */
  else if (LinTrcvIndex >= LinTrcv_30_Generic_NrOfChannels)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex) != TRUE)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  /* #20 Check the validity of the pointer OpMode */
  else if (OpMode == NULL_PTR)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* #30 Return the current OpMode */
    *OpMode = LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_State; /* SBSW_LINTRCV_PTR */
    LinTrcv_ReturnValTmp = E_OK;
  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_GETOPMODE, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return LinTrcv_ReturnValTmp;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_GetBusWuReason()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetBusWuReason
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) Reason
)
{
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_CheckValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
  /* #10 Check the validity of the parameter LinTrcvIndex */
  else if (LinTrcvIndex >= LinTrcv_30_Generic_NrOfChannels)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex) != TRUE)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  /* #20 Check the validity of the pointer Reason */
  else if (Reason == NULL_PTR)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_POINTER;
  }
  else
#endif
  {
#if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
    /* #30 Return the current WakeupReason */
    *Reason = LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason; /* SBSW_LINTRCV_PTR */

    /* #40 If the stored WakeupReason is valid: Return E_OK */
    if (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason != LINTRCV_WU_ERROR)
    {
      LinTrcv_ReturnValTmp = E_OK;
    }
#else
    *Reason = LINTRCV_WU_NOT_SUPPORTED; /* SBSW_LINTRCV_PTR */
    LINTRCV_30_GENERIC_DUMMY_STATEMENT (LinTrcvIndex); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_GETBUSWUREASON, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return LinTrcv_ReturnValTmp;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_SetWakeupMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_SetWakeupMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) TrcvWakeupMode
)
{
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_CheckValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
  /* #10 Check the validity of the parameter LinTrcvIndex */
  else if (LinTrcvIndex >= LinTrcv_30_Generic_NrOfChannels)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex) != TRUE)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else
#endif
  {
#if (LINTRCV_30_GENERIC_WAKEUPSUPPORT == STD_ON)
      /* #20 Enter critical area */
      SchM_Enter_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();

      /* #30 Set WakeupMode according to the parameter TrcvWakeupMode */
      switch(TrcvWakeupMode)
      {
        case LINTRCV_WUMODE_ENABLE:
          /* #40 For enable mode: enable WakeupReport and notify any stored Wakeup events */
          LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupMode = LINTRCV_WUMODE_ENABLE; /* SBSW_LINTRCV_CHANNEL_DATA */
# if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
          switch (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason)
          {
#  if (LINTRCV_30_GENERIC_WAKEUPBYBUSUSEDOFCHANNEL == STD_ON) /* COV_LINTRCV_WU_BY_BUS */
            case LINTRCV_WU_BY_BUS:
#  endif
#  if (LINTRCV_30_GENERIC_WAKEUPBYPINUSEDOFCHANNEL == STD_ON) /* COV_LINTRCV_WU_BY_PIN */
            case LINTRCV_WU_BY_PIN:
#  endif
              EcuM_SetWakeupEvent(LinTrcv_30_Generic_GetEcuMWakeupSourceIdOfChannel(LinTrcvIndex));
              LinIf_WakeupConfirmation(LinTrcv_30_Generic_GetEcuMWakeupSourceIdOfChannel(LinTrcvIndex));
              break;

            default:
              break;
          }
# endif

          LinTrcv_ReturnValTmp = E_OK;
          break;

        case LINTRCV_WUMODE_DISABLE:
          /* #50 For disable mode: disable WakeupReport */
          LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupMode = LINTRCV_WUMODE_DISABLE; /* SBSW_LINTRCV_CHANNEL_DATA */
          LinTrcv_ReturnValTmp = E_OK;
          break;

        case LINTRCV_WUMODE_CLEAR:
          /* #60 For clear mode: set the WakeupReason to WU_ERROR only if WakeupReport is disabled */
          if (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupMode == LINTRCV_WUMODE_DISABLE)
          {
            LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_ERROR; /* SBSW_LINTRCV_CHANNEL_DATA */
            LinTrcv_ReturnValTmp = E_OK;
          }
          break;

        default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
# if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
          LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_TRCV_WAKEUP_MODE;
# endif
          break;
      }

      /* #70 Exit critical area */
      SchM_Exit_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();
#else
    LINTRCV_30_GENERIC_DUMMY_STATEMENT (TrcvWakeupMode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
    LINTRCV_30_GENERIC_DUMMY_STATEMENT (LinTrcvIndex); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_SETWAKEUPMODE, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return LinTrcv_ReturnValTmp;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STMIF, MD_MSR_STCYC */

/***********************************************************************************************************************
 * LinTrcv_30_Generic_CheckWakeup()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_CheckWakeup
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex
)
{
  VAR(Std_ReturnType, AUTOMATIC) LinTrcv_ReturnValTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
  /* DECLARE YOUR ADDITIONAL VARIABLES HERE */

  /* Example:
   *
   * VAR(uint8, AUTOMATIC) LinTrcv_WakeupDetected = 0;
   */

  /* DO NOT CHANGE THE CODE BELOW */
#endif

#if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  if (LinTrcv_30_Generic_Dev_InitDetect != LinTrcv_30_Generic_Dev_CheckValue)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_UNINIT;
  }
  /* #10 Check the validity of the parameter LinTrcvIndex */
  else if (LinTrcvIndex >= LinTrcv_30_Generic_NrOfChannels)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else if (LinTrcv_30_Generic_IsChannelUsedOfChannel(LinTrcvIndex) != TRUE)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK;
  }
  else
#endif
  {
#if (LINTRCV_30_GENERIC_ECUMWAKEUPSOURCEIDOFCHANNEL == STD_ON)
    /* #20 If the channel is in sleep mode */
    if (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_State == LINTRCV_30_GENERIC_OP_MODE_SLEEP)
    {

      /* #30 Enter critical area */
      SchM_Enter_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();

      /* #40 Check for wakeups */
      /* CHECK FOR WAKEUP EVENTS ON THE LINTRCV CHANNEL HERE */

      /* Example:
       *
       * # if (LINTRCV_30_GENERIC_WAKEUPBYBUSUSEDOFCHANNEL == STD_ON)
       * if (LinTrcv_30_Generic_IsWakeupByBusUsedOfChannel(LinTrcvIndex))
       * {
       *   if (Wakeup by bus detected)
       *   {
       *     LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_BY_BUS;
       *     LinTrcv_WakeupDetected = 1;
       *   }
       * }
       * # endif
       *
       * # if (LINTRCV_30_GENERIC_WAKEUPBYPINUSEDOFCHANNEL == STD_ON)
       * if (LinTrcv_30_Generic_IsWakeupByPinUsedOfChannel(LinTrcvIndex))
       * {
       *   if (Wakeup by pin detected)
       *   {
       *     LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupReason = LINTRCV_WU_BY_PIN;
       *     LinTrcv_WakeupDetected = 1;
       *   }
       * }
       * # endif
       *
       * #50 If the WakeupMode is enabled: Notify EcuM/LinIf about detected wakeups (LL part)
       * if ((LinTrcv_WakeupDetected == 1) && (LinTrcv_30_Generic_ChannelData[LinTrcvIndex].LinTrcv_30_Generic_WakeupMode == LINTRCV_WUMODE_ENABLE))
       * {
       *   LinTrcv_ReturnValTmp = E_OK;
       *   EcuM_SetWakeupEvent(LinTrcv_30_Generic_GetEcuMWakeupSourceIdOfChannel(LinTrcvIndex));
       *   LinIf_WakeupConfirmation(LinTrcv_30_Generic_GetEcuMWakeupSourceIdOfChannel(LinTrcvIndex));
       * }
       */

      /* DO NOT CHANGE THE CODE BELOW */

      /* #60 Exit critical area */
      SchM_Exit_LinTrcv_30_Generic_LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0();

    }
# if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* Channel is not in sleep mode */
      LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_TRCV_NOT_SLEEP;
    }
# endif
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT (LinTrcvIndex); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
  }

#if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_CHECKWAKEUP, LinTrcv_ErrorId);
  }
#else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return LinTrcv_ReturnValTmp;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#if (LINTRCV_30_GENERIC_GET_VERSION_INFO == STD_ON)
/***********************************************************************************************************************
 * LinTrcv_30_Generic_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) VersionInfo
)
{
  VAR(uint8, AUTOMATIC) LinTrcv_ErrorId;
  LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_NO_ERROR;

# if (LINTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check the validity of the pointer VersionInfo. Note: no uninit check is performed */
  if (VersionInfo == NULL_PTR)
  {
    LinTrcv_ErrorId = LINTRCV_30_GENERIC_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* #20 Set VersionInfo with the component information */
    VersionInfo->vendorID = LINTRCV_30_GENERIC_VENDOR_ID; /* SBSW_LINTRCV_PTR */
    VersionInfo->moduleID = LINTRCV_30_GENERIC_MODULE_ID; /* SBSW_LINTRCV_PTR */
    VersionInfo->sw_major_version = LINTRCV_30_GENERIC_SW_MAJOR_VERSION; /* SBSW_LINTRCV_PTR */
    VersionInfo->sw_minor_version = LINTRCV_30_GENERIC_SW_MINOR_VERSION; /* SBSW_LINTRCV_PTR */
    VersionInfo->sw_patch_version = LINTRCV_30_GENERIC_SW_PATCH_VERSION; /* SBSW_LINTRCV_PTR */
  }

# if (LINTRCV_30_GENERIC_DEV_ERROR_REPORT == STD_ON)
  if (LinTrcv_ErrorId != LINTRCV_30_GENERIC_E_NO_ERROR)
  {
    (void)Det_ReportError(LINTRCV_30_GENERIC_MODULE_ID, LINTRCV_30_GENERIC_INSTANCE_ID_DET, LINTRCV_30_GENERIC_SID_GETVERSIONINFO, LinTrcv_ErrorId);
  }
# else
  LINTRCV_30_GENERIC_DUMMY_STATEMENT(LinTrcv_ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
}
#endif

#define LINTRCV_30_GENERIC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

  ADD FURTHER MISRA JUSTIFICATIONS HERE

*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_LINTRCV_CHANNEL_DATA
    \DESCRIPTION Array write access to LinTcv_ChannelData using index variable LinTrcvIndex.
    \COUNTERMEASURE \N The variable LinTrcvIndex is checked before to be less than LINTRCV_30_GENERIC_NROFCHANNELS.
                       The size of this array is always equal to number of channels configured. No out of bounds array access can occur
                       since LinTcv_ChannelData was created with the size equal to LINTRCV_30_GENERIC_NROFCHANNELS.

  \ID SBSW_LINTRCV_PTR
    \DESCRIPTION The function writes to the object referenced by parameter.
    \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter is valid.

  SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_LINTRCV_COMPATIBILITY
    \ACCEPT TX
    \REASON [COV_MSR_COMPATIBILITY]

  \ID COV_LINTRCV_MISRA
     \ACCEPT X
     \ACCEPT TX
     \REASON [MSR_COV_MISRA]

  \ID COV_LINTRCV_CONFIG
    \REASON [COV_MSR_CONFIG]

  \ID COV_LINTRCV_WU_BY_BUS
    \REASON N/A

  \ID COV_LINTRCV_WU_BY_PIN
    \REASON N/A

  END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 * END OF FILE: LinTrcv_30_Generic.c
 *********************************************************************************************************************/
