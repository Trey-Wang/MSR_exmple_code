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
 *         File:  Wdg.c
 *    Component:  DrvWd_VttCanoe01Asr
 *       Module:  DrvWd
 *    Generator:  -
 *
 *  Description:  MICROSAR CAL Module Wdg implements a driver for the internal Watchdog
 *
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#define WDG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Wdg.h"
#include "VttCntrl_Wdg.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of Wdg header file */
#if (  (WDG_SW_MAJOR_VERSION != (0x03u)) \
    || (WDG_SW_MINOR_VERSION != (0x03u)) \
    || (WDG_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (WDG_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (WDG_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Wdg.c and Wdg.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (WDG_CFG_MAJOR_VERSION != (0x04u)) \
    || (WDG_CFG_MINOR_VERSION != (0x04u)) )
  #error "Version numbers of Wdg.c and Wdg_Cfg.h are inconsistent!"
#endif
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*Type is used to replace WdgIf_StatusType*/
typedef enum
{
  WDG_UNINIT = 0,
  WDG_IDLE,
  WDG_BUSY
} Wdg_StatusType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/* Global pointer to the configuration */
STATIC P2CONST(Wdg_ConfigType, AUTOMATIC, WDG_APPL_CONST) Wdg_ConfigPtr_t = NULL_PTR;

STATIC VAR(Wdg_StatusType, WDG_VAR) Wdg_DriverState_t = WDG_UNINIT;

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  Wdg_InitMemory
 **********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 **********************************************************************************************************************/
FUNC(void, WDG_CODE) Wdg_InitMemory(void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* Initialize all module-global variables */
  Wdg_ConfigPtr_t = NULL_PTR;
  Wdg_DriverState_t = WDG_UNINIT;
  
  return;
} /* End of Wdg_InitMemory() */

/***********************************************************************************************************************
 *  Wdg_Init
 **********************************************************************************************************************/
/*! \brief      This routine initializes the watchdog driver
 *  \param[in]  ConfigPtr               Pointer to a configuration set (runtime)
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \context    Function is called during startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, WDG_CODE) Wdg_Init_Vtt (P2CONST(Wdg_ConfigType, AUTOMATIC, WDG_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Check whether checking of Config Parameter is enabled */
#if (STD_ON == WDG_DEV_ERROR_DETECT)
  /* check the configuration for being NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Wdg_Errorhook(WDG_SID_INIT, WDG_E_PARAM_CONFIG);
    return;
  }
#endif /* (STD_ON == WDG_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Assign ConfigPtr to the global configuration pointer */
  Wdg_ConfigPtr_t = ConfigPtr;

  if (Wdg_ConfigPtr_t->Wdg_DefaultMode_t == WDGIF_OFF_MODE)
  {
    /* Disable Watchdog if it is possible */
    #if (STD_ON == WDG_DISABLE_ALLOWED)
      /* Disable Watchdog in OFF_MODE */
      VttWdg_Stop();
    #else
      /* call of DEM with parameter 'disable rejected' */
      Dem_ReportErrorStatus (WDG_E_DISABLE_REJECTED, DEM_EVENT_STATUS_FAILED);
    #endif /* (STD_ON == WDG_DISABLE_ALLOWED) */
  }
  else if (Wdg_ConfigPtr_t->Wdg_DefaultMode_t == WDGIF_FAST_MODE)
  {
    VttWdg_Start(Wdg_ConfigPtr_t->Wdg_FastModeTimeout);
    VttWdg_SetTriggerWindow(Wdg_ConfigPtr_t->Wdg_InitialTimeout);
  }
  else
  {
    VttWdg_Start(Wdg_ConfigPtr_t->Wdg_SlowModeTimeout);
    VttWdg_SetTriggerWindow(Wdg_ConfigPtr_t->Wdg_InitialTimeout);
  }
  /* Set default operation mode */
  *(Wdg_ConfigPtr_t->Wdg_CurrentMode_pt) = Wdg_ConfigPtr_t->Wdg_DefaultMode_t;

  /* Set The Driver State Idle */
  Wdg_DriverState_t = WDG_IDLE;

  return;
} /* End of Wdg_Init() */


/***********************************************************************************************************************
 *  Wdg_SetMode
 **********************************************************************************************************************/
/*! \brief      The routine switches the watchdog driver from the current watchdog mode to the watchdog mode configured
 *              in the given parameter. It returns return E_OK if this mode switch has been executed completely and
 *              successfully, i.e. all parameters of the watchdog driver have been set to the values configured in the
 *              mode structure.
 *  \param[in]  Wdg_Mode                New operation mode
 *  \return     Std_ReturnType          E_OK: Function successfully executed
 *                                      E_NOT_OK: Error occured
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, WDG_CODE) Wdg_SetMode (WdgIf_ModeType Wdg_Mode)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == WDG_DEV_ERROR_DETECT)
  /* Check whether the module state is Idle */
  if (WDG_IDLE == Wdg_DriverState_t)
  {
    Wdg_DriverState_t = WDG_BUSY;
  }
  else
  {
    Wdg_Errorhook(WDG_SID_SET_MODE, WDG_E_DRIVER_STATE);
    return (E_NOT_OK);
  }
  
  /* Check of Mode Parameter */
  if ((WDGIF_FAST_MODE == Wdg_Mode) || (WDGIF_OFF_MODE == Wdg_Mode) || (WDGIF_SLOW_MODE == Wdg_Mode))
  {
    /* intentional left free */
  }
  else
  {
    /* Call Errorhook */
    Wdg_Errorhook(WDG_SID_SET_MODE, WDG_E_PARAM_MODE);

    /* Set The Driver State Idle */
    Wdg_DriverState_t = WDG_IDLE;

    return (E_NOT_OK);
  }


  #if (STD_OFF == WDG_DISABLE_ALLOWED)
    /* Provided configuration disables watchdog */
    if (Wdg_Mode == WDGIF_OFF_MODE)
    {
      /* Call of DEM with parameter 'disable rejected' */
      Dem_ReportErrorStatus (WDG_E_DISABLE_REJECTED, DEM_EVENT_STATUS_FAILED);
      return E_NOT_OK;
    }
    else
    {
      /* Wdg can be set in WDGIF_OFF_MODE */
    }
  #endif /* (STD_OFF == WDG_DISABLE_ALLOWED) */
#endif /* (STD_ON == WDG_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  if (Wdg_Mode == WDGIF_OFF_MODE)
  {
    /* Stop CANoe Timer */
    VttWdg_Stop();
  }
  else if (Wdg_Mode == WDGIF_FAST_MODE)
  {
    /* Set timer to the timeout configured in FastMode */
    VttWdg_Start(Wdg_ConfigPtr_t->Wdg_FastModeTimeout);
  }
  else
  {
    /* Set timer to the timeout configured in SlowMode */
    VttWdg_Start(Wdg_ConfigPtr_t->Wdg_SlowModeTimeout);
  }

  /* Set current operation mode to the parameter passed in the function call. */
  *(Wdg_ConfigPtr_t->Wdg_CurrentMode_pt) = Wdg_Mode;

  /* Set the Driver State IDLE */
  Wdg_DriverState_t = WDG_IDLE;

  return E_OK;
} /* End of Wdg_SetMode() */


/***********************************************************************************************************************
 *  Wdg_SetTriggerCondition
 **********************************************************************************************************************/
/*! \brief      This function will be called from the User to set the timeoutvalue
 *  \param[in]  timeout               Timeout value
 *  \return     void
 *  \note
 **********************************************************************************************************************/
FUNC(void, WDG_CODE) Wdg_SetTriggerCondition (uint16 timeout)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Creating the timing window in which the triggering is allowed        */
  VttWdg_SetTriggerWindow (timeout);
  
  return;
} /* End of Wdg_SetTriggerCondition() */


#if (STD_ON == WDG_VERSION_INFO_API)
/***********************************************************************************************************************
 *  Wdg_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[out] versioninfo             Pointer to where to store the version information of this module
 *  \return     void
 *  \note
 **********************************************************************************************************************/
FUNC(void, WDG_CODE) Wdg_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, WDG_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Check Versioninfo pointer for referencing NULL_PTR */
#if (STD_ON == WDG_DEV_ERROR_DETECT)
  if (NULL_PTR == versioninfo)
  {
    /* Call of errorhook  */
    Wdg_Errorhook (WDG_SID_GET_VERSION_INFO, WDG_E_PARAM_VINFO);

    return;
  }
#endif /* (STD_ON == WDG_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID = (WDG_VENDOR_ID);
  versioninfo->moduleID = (WDG_MODULE_ID);
  versioninfo->sw_major_version = (WDG_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (WDG_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (WDG_SW_PATCH_VERSION);

  return;
} /* End of Wdg_GetVersionInfo() */
#endif  /* (STD_ON == WDG_VERSION_INFO_API) */


/***********************************************************************************************************************
 *  Wdg_Cbk_GptNotification
 **********************************************************************************************************************/
/*! \brief      This routine triggers the watchdog.
 *  \param[in]  void
 *  \param[out] void
 *  \return     void
 *  \note
 **********************************************************************************************************************/
FUNC(void, WDG_CODE) Wdg_Cbk_GptNotification (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == WDG_DEV_ERROR_DETECT)
  /* Check whether the module state is Idle */
  if (WDG_IDLE == Wdg_DriverState_t)
  {
    Wdg_DriverState_t = WDG_BUSY;
  }
  else
  {
    Wdg_Errorhook(WDG_SID_CBK_GPT_NOTIFICATION, WDG_E_DRIVER_STATE);
    return;
  }
#endif /* (STD_ON == WDG_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* nothing to be done */
  Wdg_DriverState_t = WDG_IDLE;

  return;
} /* End of Wdg_Trigger() */


/**********************************************************************************************************************
 *  END OF FILE: Wdg.c
 **********************************************************************************************************************/
