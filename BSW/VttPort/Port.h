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
 *         File:  Port.h
 *    Component:  DrvPort_VttCanoe01Asr
 *       Module:  DrvPort
 *    Generator:  -
 *
 *  Description:  Due to the fact, that the CANoe-emulated MCAL does not provide physical "Ports", this module provides
 *                no functionality.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Peter Lang                    virlg         Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-09-20  virlg   ESCAN00068492 Initial Development
 *  01.00.01  2014-05-15  virchl                Correct version numbers and checks adapted
 *  02.00.00  2015-01-22  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Port_Init (if VTTOnly-SIP)
 *                                              Port_Init renamed to Port_Init_Vtt and a define in Port_Cfg.h reroutes
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *  02.03.00  2017-01-13  virbmz   FEAT-1824    Support a component-specific SafeBSW configuration switch - version update  
 **********************************************************************************************************************/

#if !defined PORT_H
#define PORT_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVPORT_VTTCANOE01ASR_VERSION            (0x0203u)
#define DRVPORT_VTTCANOE01ASR_RELEASE_VERSION    (0x00u)

/* Vendor and module identification */
#define PORT_VENDOR_ID                           (30u)
#define PORT_MODULE_ID                           (124u)
#define PORT_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
#define PORT_AR_RELEASE_MAJOR_VERSION            (4u)
#define PORT_AR_RELEASE_MINOR_VERSION            (0u)
#define PORT_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define PORT_SW_MAJOR_VERSION                    (DRVPORT_VTTCANOE01ASR_VERSION >> 8)
#define PORT_SW_MINOR_VERSION                    (DRVPORT_VTTCANOE01ASR_VERSION & 0x00FF)
#define PORT_SW_PATCH_VERSION                    (DRVPORT_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define PORT_SID_INIT                             (0x00u)
#define PORT_SID_SET_PIN_DIR                      (0x01u)
#define PORT_SID_REFRESH_PORT_DIR                 (0x02u)
#define PORT_SID_GET_VERSION_INFO                 (0x03u)
#define PORT_SID_SET_PIN_MODE                     (0x04u)

/* ----- Error codes ----- */
#define PORT_E_PARAM_PIN                          (0x0Au)   /* Invalid Port Pin ID requested                        */
#define PORT_E_DIRECTION_UNCHANGEABLE             (0x0Bu)   /* Port Pin not configured as changeable                */
#define PORT_E_PARAM_CONFIG                       (0x0Cu)   /* API Port_Init service called with wrong parameter    */
#define PORT_E_PARAM_INVALID_MODE                 (0x0Du)   /* API Port_SetPinMode service called when mode is 
                                                             * unchangeable 
                                                             */
#define PORT_E_MODE_UNCHANGEABLE                  (0x0Eu)   /* API Port_SetPinMode service called when mode is 
                                                             * unchangeable 
                                                             */
#define PORT_E_UNINIT                             (0x0Fu)   /* API function is called before module initialization  */
#define PORT_E_PARAM_VINFO                        (0x20u)   /* Parameter versioninfo is referencing NULL_PTR        */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Type for a symbolic name of a Port pin */
typedef uint8 Port_PinType;
/* Type for all available Pin Modes */
typedef uint8 Port_PinModeType;

/* Type of possible directions a port pin can have */
typedef enum
{
  PORT_PIN_IN = 0,
  PORT_PIN_OUT
} Port_PinDirectionType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Port_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void,PORT_CODE) Port_InitMemory (void);

FUNC(void, PORT_CODE) Port_Init_Vtt (P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr);

#if (STD_ON == PORT_SET_PIN_DIRECTION_API)
  FUNC(void, PORT_CODE)Port_SetPinDirection (Port_PinType Pin, Port_PinDirectionType Direction);
#endif

FUNC(void, PORT_CODE) Port_RefreshPortDirection (void);

#if (STD_ON == PORT_SET_PIN_MODE_API)
  FUNC(void, PORT_CODE) Port_SetPinMode (Port_PinType Pin, Port_PinModeType Mode);
#endif

#if (STD_ON == PORT_VERSION_INFO_API)
  FUNC(void, PORT_CODE) Port_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) versioninfo);
#endif


#endif  /* PORT_H */


/**********************************************************************************************************************
 *  END OF FILE: Port.h
 **********************************************************************************************************************/
