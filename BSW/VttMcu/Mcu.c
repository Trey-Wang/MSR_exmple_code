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
/**        \file  Mcu.c
 *        \brief  Mcu source file
 *
 *      \details  The Mcu Driver provides services for basic microcontroller initialization,
 *                power down functionality, reset and microcontroller specific functions
 *                required from other MICROSAR CAL software modules.
 *                This is an emulated driver for VTT
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define MCU_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Mcu.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Mcu header file */
#if (  (MCU_SW_MAJOR_VERSION != (0x02u)) \
    || (MCU_SW_MINOR_VERSION != (0x04u)) \
    || (MCU_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (MCU_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (MCU_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Mcu.c and Mcu.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (MCU_CFG_MAJOR_VERSION != (0x04u)) \
    || (MCU_CFG_MINOR_VERSION != (0x02u)) )
  #error "Version numbers of Mcu.c and Mcu_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define MCU_INVALID_RAW_RESET_VALUE   (0xFFu)


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/* Pointer to the configuration */
STATIC P2CONST(Mcu_ConfigType, AUTOMATIC, MCU_APPL_CONST) Mcu_ConfigPtr_pt = NULL_PTR;

/* Global variable that assures, that the hint regarding function Mcu_GetPllStatus is written to CANoe only once. */
STATIC VAR(uint8, MCU_VAR) Mcu_PllHintIsAlreadyWritten = 0u;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 

/*********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/



/*********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  Mcu_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Initialize all module-global variables */
  Mcu_ConfigPtr_pt = NULL_PTR;
  
  Mcu_PllHintIsAlreadyWritten = 0u;
  
  return;
} /* End of Mcu_InitMemory() */


/**********************************************************************************************************************
 *  Mcu_Init
 *********************************************************************************************************************/
/*! \brief      This routine initializes the MCU driver.
 *  \param[in]  ConfigPtr               Pointer to a configuration set.
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \context    Function is called in startup context.
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_Init_Vtt (P2CONST(Mcu_ConfigType, AUTOMATIC, MCU_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  if (ConfigPtr == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_INIT, MCU_E_PARAM_CONFIG);
    return;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Allow writing to CANoe console for function Mcu_GetPllStatus again */
  Mcu_PllHintIsAlreadyWritten = 0u;

  /* Save the pointer to configuration */
  Mcu_ConfigPtr_pt = ConfigPtr;
  return;
} /* End of Mcu_Init() */


/**********************************************************************************************************************
 *  Mcu_GetPllStatus
 *********************************************************************************************************************/
/*! \brief      This function provides the lock status of the PLL.
 *  \param[in]  void
 *  \return     Mcu_PllStatusType       Function will always return MCU_PLL_LOCKED, if no det error is detected
 *  \note
 *********************************************************************************************************************/
Mcu_PllStatusType Mcu_GetPllStatus (void)
{ 
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_GET_PLL_STATUS, MCU_E_UNINIT);
    return MCU_PLL_STATUS_UNDEFINED;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* No Pll available */
  return (MCU_PLL_LOCKED);
} /* End of Mcu_GetPllStatus */

#if (STD_ON == MCU_INIT_CLOCK)
/**********************************************************************************************************************
 *  Mcu_InitClock
 *********************************************************************************************************************/
/*! \brief      This function normally initializes the PLL. It has no functionality in this emulation and is only
 *              provided for compatibility reasons.
 *  \param[in]  ClockSetting            Parameter is ignored
 *  \return     E_OK                    Normal operation
 *  \return     E_NOT_OK                Development error called
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MCU_CODE) Mcu_InitClock (Mcu_ClockType ClockSetting)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if(Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_INIT_CLOCK, MCU_E_UNINIT);
    return E_NOT_OK;
  }

  /* Paramter ClockSetting check */
  if(ClockSetting >= Mcu_ConfigPtr_pt->NumberOfClockSettings_u8)
  {
    Mcu_Errorhook (MCU_SID_INIT_CLOCK, MCU_E_PARAM_CLOCK);
    return E_NOT_OK;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Statement to suppress compiler warning */
  ClockSetting = ClockSetting;

  return (E_OK);
} /* End of Mcu_InitClock */

#endif /* (STD_ON == MCU_INIT_CLOCK) */

#if (STD_OFF == MCU_NO_PLL)
/**********************************************************************************************************************
 *  Mcu_DistributePllClock
 *********************************************************************************************************************/
/*! \brief      This function normally activates the PLL clock to the MCU clock distribution. It has no functionality
 *              in this emulation and is only provided for compatibility reasons.
 *  \param[in]  void
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_DistributePllClock (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_DISTRIBUTE_PLL_CLOCK, MCU_E_UNINIT);
    return;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  return;
} /* End of Mcu_DistributePllClock() */
#endif /* (STD_OFF == MCU_NO_PLL) */

/**********************************************************************************************************************
 *  Mcu_GetResetReason
 *********************************************************************************************************************/
/*! \brief      The function reads the reset type from the hardware (VTT).
 *  \return     MCU_POWER_ON_RESET      In case of normal operation, because the emulation does not support this
 *                                      functionality
 *  \return     MCU_RESET_UNDEFINED     In case of a development error
 *  \note
 *********************************************************************************************************************/
FUNC(Mcu_ResetType, MCU_CODE) Mcu_GetResetReason (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_GET_RESET_REASON, MCU_E_UNINIT);
    return MCU_RESET_UNDEFINED;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  return (Mcu_ResetType)VttMcu_GetResetReason();
} /* End of Mcu_GetResetReason() */


/**********************************************************************************************************************
 *  Mcu_SetMode
 *********************************************************************************************************************/
/*! \brief      This function activates the MCU power modes.
 *  \param[in]  McuMode                 Parameter is ignored
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_SetMode (Mcu_ModeType McuMode)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_SET_MODE, MCU_E_UNINIT);
    return;
  }

  /* Check parameter McuMode */
  if ( (McuMode > MCU_HIGHEST_MODE_NUMBER) || \
       (Mcu_ConfigPtr_pt->Mcu_ModeMapping_pt[McuMode] == VTTMCU_MODE_UNDEF) )
  {
    /* Parameter McuMode is invalid */
    Mcu_Errorhook (MCU_SID_SET_MODE, MCU_E_PARAM_MODE);
    return;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Statement to suppress compiler warning */
  VttMcu_SetMode (Mcu_ConfigPtr_pt->Mcu_ModeMapping_pt[McuMode]);

  return;
} /* End of Mcu_SetMode() */


/**********************************************************************************************************************
 *  Mcu_GetResetRawValue
 *********************************************************************************************************************/
/*! \brief      This function reads the reset type.
 *  \param[in]  void
 *  \return     0                       This function always returns zero.
 *  \note
 *********************************************************************************************************************/
FUNC(Mcu_RawResetType, MCU_CODE) Mcu_GetResetRawValue (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_GET_RESET_RAW_VALUE, MCU_E_UNINIT);
    return MCU_INVALID_RAW_RESET_VALUE;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  return (Mcu_RawResetType)VttMcu_GetResetReason();
} /* End of Mcu_GetResetRawValue() */


#if (STD_ON == MCU_PERFORM_RESET_API)
/**********************************************************************************************************************
 *  Mcu_PerformReset
 *********************************************************************************************************************/
/*! \brief      This function performs a microcontroller reset (VTT). I
 *  \param[in]  void
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_PerformReset (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_PERFORM_RESET, MCU_E_UNINIT);
    return;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  VttMcu_PerformReset();

  return;
} /* End of the Mcu_Performreset() */
#endif /* (STD_ON == MCU_PERFORM_RESET_API) */



/**********************************************************************************************************************
 *  Mcu_InitRamSection
 *********************************************************************************************************************/
/*! \brief      This function initializes the RAM. It has no functionality in this emulation and is only
 *              provided for compatibility reasons.
 *  \param[in]  RamSection              Parameter is ignored
 *  \return     E_OK                    No development error occured
 *  \return     E_NOT_OK                Development error occured
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, MCU_CODE) Mcu_InitRamSection (Mcu_RamSectionType RamSection)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_INIT_RAMSECTION, MCU_E_UNINIT);
    return E_NOT_OK;
  }

  /* Check parameter RamSection */
  if (RamSection >= Mcu_ConfigPtr_pt->NumberOfRamSectors_u8)
  {
    /* Parameter RamSection is invalid */
    Mcu_Errorhook (MCU_SID_INIT_RAMSECTION, MCU_E_PARAM_RAMSECTION);
    return E_NOT_OK;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Statement to suppress compiler warning */
  RamSection = RamSection;

  return E_OK;
} /* End of Mcu_InitRamSection() */


#if (STD_ON == MCU_GET_RAM_STATE_API)
/**********************************************************************************************************************
 *  Mcu_GetRamState
 *********************************************************************************************************************/
/*! \brief      This function returns the status of the Ram
 *  \param[in]  void
 *  \return     MCU_RAMSTATE_INVALID
 *  \note
 *********************************************************************************************************************/
FUNC(Mcu_RamStateType, MCU_CODE) Mcu_GetRamState (void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  /* Check whether the module was initialized */
  if (Mcu_ConfigPtr_pt == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_INIT_RAMSTATE, MCU_E_UNINIT);
    return MCU_RAMSTATE_INVALID;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  return MCU_RAMSTATE_INVALID;
} /* End of Mcu_GetRamState() */
#endif /* (STD_ON == MCU_GET_RAM_STATE_API) */


#if (STD_ON == MCU_VERSION_INFO_API)
/**********************************************************************************************************************
 *  Mcu_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *              The version information includes:
 *              - Module Id
 *              - Vendor Id
 *              - Vendor specific version numbers
 *  \param[out] versioninfo             Pointer to where to store the version information of this module.
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, MCU_CODE) Mcu_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, MCU_APPL_DATA)versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Check versioninfo pointer */
#if (STD_ON == MCU_DEV_ERROR_DETECT)
  if (versioninfo == NULL_PTR)
  {
    Mcu_Errorhook (MCU_SID_GET_VERSION_INFO, MCU_E_PARAM_VINFO);
    return;
  }
#endif /* (STD_ON == MCU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID = (MCU_VENDOR_ID);
  versioninfo->moduleID = (MCU_MODULE_ID);
  versioninfo->sw_major_version = (MCU_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (MCU_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (MCU_SW_PATCH_VERSION);

  return;
  } /* End of Mcu_GetVersionInfo() */
#endif  /* (STD_ON == MCU_VERSION_INFO_API) */


/**********************************************************************************************************************
 *  END OF FILE: Mcu.c
 *********************************************************************************************************************/
 