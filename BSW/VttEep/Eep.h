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
 *         File:  Eep.h
 *      Project:  DrvEep_VttCanoe01Asr
 *       Module:  MICROSAR Eep
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Eep implements a EEPROM driver for the CANoe Emulation.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Stefan Eckl                   virsel        Vector Informatik GmbH
 *  Christian Leder               virchl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-01-28  virsel     -          First Version of Eep Driver for VIPCANoe
 *  02.00.00  2015-01-20  virchl  FEAT-254      Global Renaming of VIP to VTT
 *  02.00.01  2015-04-07  virchl  ESCAN00082264 Remove of VttEep_Init in Eep_Init
 *  02.01.00  2015-07-08  virchl  FEAT-1479     Support of automatic registration of Eep_Init (if VTTOnly-SIP)
 *                                              Eep_Init renamed to Eep_Init_Vtt and a define in Eep_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl  FEAT-1372     Update of SwVersions due to increase of generator version
 *  02.02.00  2015-10-05  virchl  ESCAN00085648 Include of SchM_Eep.h added
 *  02.02.01  1016-08-12  virbmz  FEAT-1842     Support of external driver
 *  02.03.00  2017-01-12  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 *                                              instead of MemIf_AddressType, MemIf_LengthType use Fls_30_Stub_AddressType, 
 *                                              Fls_30_Stub_LengthType, the MemIf types were valid, only for older ASR versions
 *  02.04.00  2017-05-18  virbmz  TASK-33340    Version number incremented
 *  02.04.01  2017-07-07  virbmz  ESCAN00095796 removed (EepAddress < 0) check, because EepAddress can not get smalle than zero
 *  02.04.02  2017-07-13  virbmz  ESCAN00095796 removed  uint8 Value = EEP_ERASE_VALUE; because Value is never used
 **********************************************************************************************************************/

#if !defined EEP_H
#define EEP_H

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define DRVEEP_VTTCANOE01ASR_VERSION            (0x0204u)
#define DRVEEP_VTTCANOE01ASR_RELEASE_VERSION    (0x02u)

/* Vendor and module identification */
#define EEP_VENDOR_ID                           (30u)
#define EEP_MODULE_ID                           (90u)
#define EEP_INSTANCE_ID                         (0u)

/* AUTOSAR Software specification version information */
#define EEP_AR_RELEASE_MAJOR_VERSION            (4u)
#define EEP_AR_RELEASE_MINOR_VERSION            (0u)
#define EEP_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define EEP_SW_MAJOR_VERSION                    (DRVEEP_VTTCANOE01ASR_VERSION >> 8u)
#define EEP_SW_MINOR_VERSION                    (DRVEEP_VTTCANOE01ASR_VERSION & 0x00FF)
#define EEP_SW_PATCH_VERSION                    (DRVEEP_VTTCANOE01ASR_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define EEP_SID_INIT                            (0x00u)   /* Eep_Init()             */
#define EEP_SID_SETMODE                         (0x01u)   /* Eep_SetMode()          */
#define EEP_SID_READ                            (0x02u)   /* Eep_Read()             */
#define EEP_SID_WRITE                           (0x03u)   /* Eep_Write()            */
#define EEP_SID_ERASE                           (0x04u)   /* Eep_Erase()            */
#define EEP_SID_COMPARE                         (0x05u)   /* Eep_Compare()          */
#define EEP_SID_CANCEL                          (0x06u)   /* Eep_Cancel()           */
#define EEP_SID_GETSTATUS                       (0x07u)   /* Eep_GetStatus()        */
#define EEP_SID_GETJOBRESULT                    (0x08u)   /* Eep_GetJobResult()     */
#define EEP_SID_MAINFUNCTION                    (0x09u)   /* Eep_MainFunction()     */
#define EEP_SID_GETVERSIONINFO                  (0x0Au)   /* Eep_GetVersionInfo()   */

/* ----- Error codes ----- */
#define EEP_E_PARAM_CONFIG                      (0x10u)   /* Pointer to configurations was NULL_PTR */
#define EEP_E_PARAM_ADDRESS                     (0x11u)   /* Address not aligned to a segment address or out of range */
#define EEP_E_PARAM_DATA                        (0x12u)   /* Wrong pointer parameter                */
#define EEP_E_PARAM_LENGTH                      (0x13u)   /* Length is not greater than 0           */
#define EEP_E_PARAM_POINTER                     (0x23u)   /* API service called with a NULL pointer */
#define EEP_E_PARAM_VINFO                       (0x15u)   /* VersionInfo-Ptr references NULL_PTR    */
#define EEP_E_UNINIT                            (0x20u)   /* Error code for not initialized module  */
#define EEP_E_BUSY                              (0x21u)   /* Error code if flash is current busy    */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Used as address offset from the configured EEPROM base address to access a certain EEPROM memory area */
typedef uint32 Eep_AddressType;

/* Specifies the number of bytes to read/write/erase/compare */
typedef uint32 Eep_LengthType;

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 **********************************************************************************************************************/
#include "Eep_Cfg.h"
#include "VttCntrl_Eep.h"
/* include of SchM_Eep.h moves into Eep_Cfg.h because the file name is API-Infix dependent */


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, EEP_CODE) Eep_InitMemory (void);
      
FUNC(void, EEP_CODE) Eep_Init_Vtt (P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr);

FUNC(void, EEP_CODE) Eep_SetMode (MemIf_ModeType Mode);

FUNC(Std_ReturnType, EEP_CODE) Eep_Read
(
  Eep_AddressType  EepAddress,
  Eep_VarBytePtrType DataBufferPtr,
  Eep_LengthType   Length
);

FUNC(Std_ReturnType, EEP_CODE) Eep_Write
(
  Eep_AddressType EepAddress,
  Eep_ConstBytePtrType DataBufferPtr,
  Eep_LengthType Length
);

FUNC(Std_ReturnType, EEP_CODE) Eep_Erase
(
  Eep_AddressType  EepAddress,
  Eep_LengthType   Length
);

FUNC(Std_ReturnType, EEP_CODE) Eep_Compare
(
  Eep_AddressType EepAddress,
  Eep_ConstBytePtrType DataBufferPtr,
  Eep_LengthType Length
);

FUNC(void, EEP_CODE) Eep_Cancel (void);

FUNC(MemIf_StatusType, EEP_CODE) Eep_GetStatus (void);

FUNC(MemIf_JobResultType, EEP_CODE) Eep_GetJobResult (void);

/*Function declaration should be provided by the SCHM*/
//FUNC(void, EEP_CODE) Eep_MainFunction(void);

FUNC(Std_ReturnType, EEP_CODE) Eep_SimulateError (void);
 
#if (STD_ON == EEP_VERSION_INFO_API)
  FUNC(void, EEP_CODE) Eep_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) versioninfo);
#endif /* (STD_ON == EEP_VERSION_INFO_API) */

#endif /* EEP_H */

/**********************************************************************************************************************
 *  END OF FILE: Eep.h
 *********************************************************************************************************************/
