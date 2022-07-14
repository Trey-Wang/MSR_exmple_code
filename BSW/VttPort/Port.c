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
 *         File:  Port.c
 *    Component:  DrvPort_VttCanoe01Asr
 *       Module:  DrvPort
 *    Generator:  -
 *
 *  Description:  Due to the fact, that the CANoe-emulated MCAL does not provide physical "Ports", this module provides
 *                no functionality.
 *********************************************************************************************************************/

#define PORT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Port.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of Port header file */
#if (  (PORT_SW_MAJOR_VERSION != (0x02u)) \
    || (PORT_SW_MINOR_VERSION != (0x03u)) \
    || (PORT_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (PORT_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (PORT_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Vendor specific version numbers of Pwm.c and Pwm.h are inconsistent"
#endif

/* configuration version info check has to match definition in header file */
#if(   (PORT_CFG_MAJOR_VERSION != (0x04u)) \
    || (PORT_CFG_MINOR_VERSION != (0x01u)) )
  #error "Version numbers of Port.c and Port_Cfg.h are inconsistent!"
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

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/* Global buffer for the pointer to the current configuration */
STATIC P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) Port_CurrConfig_p = NULL_PTR;

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
 *  Port_InitMemory
 **********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Initialize all module-global variables */
  Port_CurrConfig_p = NULL_PTR;
  
  return;
} /* End of Port_InitMemory() */


/***********************************************************************************************************************
 *  Port_Init
 **********************************************************************************************************************/
/*! \brief      Initialization of whole used ports and pins.
 *              
 *  \param[in]  ConfigPtr        pointer to the port configuration struct.
 *  \pre        Input parameter must not be NULL_PTR.
 *  \context    Function could be called from interrupt level or from task level
  **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_Init_Vtt (P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PORT_DEV_ERROR_DETECT)
  /* Check the config pointer for being a NULL_PTR. */
  if (NULL_PTR == ConfigPtr)
  {
    Port_Errorhook (PORT_SID_INIT, PORT_E_PARAM_CONFIG);
    return;
  }
  else
  {
    /* config pointer seems to be valid */
  }
#endif /* (STD_ON == PORT_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the pointer to the current variant */
  Port_CurrConfig_p = ConfigPtr;

  return;
} /* End of Port_Init() */


#if (STD_ON == PORT_SET_PIN_DIRECTION_API)
/***********************************************************************************************************************
 *  Port_SetPinDirection
 **********************************************************************************************************************/
/*! \brief      This function sets the direction of each configured pin
 *              if allowed.
 *
 *  \param[In]  Pin               Configured handle of a port pin.
 *  \param[In]  Direction         Required pin direction
 *  \pre        Module have to be initialized.
 *  \context    Function could be called from interrupt level or from task level
  **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_SetPinDirection (Port_PinType Pin, Port_PinDirectionType Direction)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PORT_DEV_ERROR_DETECT)
  /* Check if module is initialized */
  if (NULL_PTR == Port_CurrConfig_p)
  {
    Port_Errorhook (PORT_SID_SET_PIN_DIR, PORT_E_UNINIT);
    return;
  }
  else
  {
    /* module is initialized */
  }

  /*  Is the parameter "pin" a part of the list */
  if (Pin < (Port_CurrConfig_p->Port_CurrNoOfPinChannelCfg_u8))
  {
    /* Parameter is obviously a part of the pin handle list */
  }
  else
  {
    Port_Errorhook (PORT_SID_SET_PIN_DIR, PORT_E_PARAM_PIN);
    return;
  }

  /* check if pin direction is changeable */
  if (TRUE == Port_CurrConfig_p->PinDirectionChangeable_pt[Pin])
  {
    /* Pin direction is changeable. No error. */
  }
  else
  {
    Port_Errorhook (PORT_SID_SET_PIN_DIR, PORT_E_DIRECTION_UNCHANGEABLE);
    return;
  }
#endif /* (STD_ON == PORT_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* No functionality implemented */

  /* Assignments to avoid compiler warnings in production mode */
  Pin = Pin;
  Direction = Direction;

  return;
} /* End of Port_SetPinDirection() */
#endif  /* (STD_ON == PORT_SET_PIN_DIRECTION_API) */


/***********************************************************************************************************************
 *  Port_RefreshPortDirection
 **********************************************************************************************************************/
/*! \brief      This function refreshs the pin direction. 
 *              
 *  \pre        Module have to be initialized.
 *  \context    Function could be called from interrupt level or from task level.
 **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_RefreshPortDirection (void)
{
  /* ----- Development Error Checks ------------------------------------- */  
#if (STD_ON == PORT_DEV_ERROR_DETECT)
  /* Check if module has been initialzied. */
    if ( NULL_PTR == Port_CurrConfig_p)
    {
      Port_Errorhook (PORT_SID_REFRESH_PORT_DIR, PORT_E_UNINIT);
      return;
    }
    else
    {
      /* module is initialized */
    }
#endif /* (STD_ON == PORT_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* no functionality yet */
  
  return;
} /* End of Port_RefreshPortDirection() */


#if (STD_ON == PORT_SET_PIN_MODE_API)
/***********************************************************************************************************************
 *  Port_SetPinMode
 **********************************************************************************************************************/
/*! \brief      This function allows to change the Pin Mode during runtime;
 *              At present this feature is not supported.
 *
 *  \param[In]  Pin               Configured handle of a port pin.
 *  \param[In]  Mode              The Mode the port pin should setted
 *  \pre        Module have to be initialized.
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_SetPinMode (Port_PinType Pin, Port_PinModeType Mode)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PORT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Port_CurrConfig_p)
  {
    Port_Errorhook (PORT_SID_SET_PIN_MODE, PORT_E_UNINIT);
    return;
  }
  else
  {
    /* module is initialized */
  }

  /*  Is the parameter "pin" a part of the list */
  if (Pin < (Port_CurrConfig_p->Port_CurrNoOfPinChannelCfg_u8))
  {
    /* Parameter is obviously a part of the pin handle list */
  }
  else
  {
    Port_Errorhook (PORT_SID_SET_PIN_MODE, PORT_E_PARAM_PIN);
    return;
  }
#endif /* (STD_ON == PORT_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Assignment to avoid compiler warnings */
  Pin = Pin;
  Mode = Mode;

  /* No functionality implemented */
  return;
} /* End of Port_SetPinMode() */
#endif /* (STD_ON == PORT_SET_PIN_MODE_API) */


#if (STD_ON == PORT_VERSION_INFO_API)
/***********************************************************************************************************************
 *  Port_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      This service returns the version information of this module. 
 *              The version information includes:
 *                 - Module Id
 *                 - Vendor Id
 *                 - Vendor specific version numbers
 *
 *  \param[out] versioninfo        pointer to struct of type Std_VersionInfoType
 *  \pre        Output parameter must not be NULL_PTR.
 *  \context    Function could be called from interrupt level or from task level
  **********************************************************************************************************************/
FUNC(void, PORT_CODE) Port_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PORT_DEV_ERROR_DETECT)
  /* Check the versioninfo pointer for being a NULL_PTR. */
  if (NULL_PTR == versioninfo)
  {
    Port_Errorhook (PORT_SID_GET_VERSION_INFO, PORT_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* versioninfo pointer seems to valid */
  }
#endif /* (STD_ON == PORT_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID         = (PORT_VENDOR_ID);
  versioninfo->moduleID         = (PORT_MODULE_ID);
  versioninfo->sw_major_version = (PORT_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (PORT_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (PORT_SW_PATCH_VERSION);

  return;
} /* End of Port_GetVersionInfo() */
#endif  /* (STD_ON == PORT_VERSION_INFO_API) */


/**********************************************************************************************************************
 *  END OF FILE: Port.c
 *********************************************************************************************************************/
