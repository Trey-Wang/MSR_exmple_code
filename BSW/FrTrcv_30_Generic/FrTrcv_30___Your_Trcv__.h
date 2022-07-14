
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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  FrTrcv_30___Your_Trcv__.h
 *    Component:  MICROSAR FR Transceiver Driver
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  FlexRay transceiver driver implementation, according to:
 *                AUTOSAR FlexRay Transceiver Driver, AUTOSAR Release 3.0
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ATTENTION: Replace the placeholders __Your_Trcv__ and __YOUR_TRCV__ with the according name of the used transceiver.
 *            __YOUR_TRCV__ is used for definitions in upper case for defines.
 *            __Your_Trcv__ is used for variables in camel case.
 * EXAMPLE:   Replace __YOUR_TRCV__ to TJA1041 and __Your_Trcv__ to Tja1041.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Herkommer             hr            Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2010-01-02  hr      -             Creation
 *            2010-04-21  hr      ESCAN00041515 Move FrTrcv_TrcvModeType and FrTrcv_TrcvWUReasonType to Fr_GeneralTypes.h
 *  1.01.00   2011-01-11  hr      ESCAN00047305 Adapt MainFunction for usage with IdentityManagerConfig
 *            2011-01-31  hr      ESCAN00048304 Compile error because of unknown Memmap sections
 *                                ESCAN00048376 Compiler warning in the FR transceiver implementation
 *                                ESCAN00048396 Compile error for the exclusive areas
 *  1.01.01   2011-02-17  hr      ESCAN00048742 Missing START_SEC_CONST_32BIT in FrTrcv_30___Your_Trcv___MemMap.inc
 *  1.01.02   2011-03-02  hr      ESCAN00049012 "Dem_"  prefix is missing for FRTRCV_E_FR_NO_TRCV_CONTROL
 *            2011-05-04  hr      ESCAN00049932 Wakeup detection is checked even though the option is not enabled in GENy
 *  1.01.03   2012-01-09  hr      ESCAN00053416 AR3-2069: Remove non-SchM code for critical section handling
 *  1.01.04   2013-11-12  hr      ESCAN00071791 The MISRA justifications are not implemented according WI_MISRAC2004_PES.pdf
 *  2.00.00   2014-01-13  hr      ESCAN00073038 Critical Section not left if SetTransceiverMode called with invalid mode
 *                                ESCAN00073041 EcuM_SetWakeupEvent call with active interrupt locks
 *                                ESCAN00072928 Support ASR4
 *  2.00.01   2015-05-19  hr      ESCAN00083046 Compile error: DEM error constant renamed for ASR4
 *********************************************************************************************************************/
#ifndef FRTRCV_30___YOUR_TRCV___H
#define FRTRCV_30___YOUR_TRCV___H

/**************************************************************************************************
* Include files
**************************************************************************************************/
#include "ComStack_Types.h"
#include "Fr_GeneralTypes.h"
#include "EcuM_Cbk.h"
#include "FrTrcv_30___Your_Trcv___Cfg.h"

/* FrTrcv335:, FrTrcv107: */
#if ( FRTRCV_30_GENERIC_PROD_ERROR_DETECT == STD_ON )
  #include "Dem.h"
#endif

#include "SchM_FrTrcv_30___Your_Trcv__.h"

/**************************************************************************************************
* Version
**************************************************************************************************/
/* BCD coded version number */
/* ##V_CFG_MANAGEMENT ##CQProject : DrvTrans_GenericFrAsr CQComponent : Implementation */
#define DRVTRANS_GENERICFRASR_VERSION         0x0200u /* BCD coded version number */
#define DRVTRANS_GENERICFRASR_RELEASE_VERSION 0x01u   /* BCD coded version number */

/* supported Autosar version */
#define FRTRCV_30___YOUR_TRCV___AR_MAJOR_VERSION     (4u)
#define FRTRCV_30___YOUR_TRCV___AR_MINOR_VERSION     (0u)
#define FRTRCV_30___YOUR_TRCV___AR_PATCH_VERSION     (3u)
#define FRTRCV_30___YOUR_TRCV___SW_MAJOR_VERSION     (2u)
#define FRTRCV_30___YOUR_TRCV___SW_MINOR_VERSION     (0u)
#define FRTRCV_30___YOUR_TRCV___SW_PATCH_VERSION     (1u)

#define FRTRCV_30___YOUR_TRCV___VENDOR_ID (30u) /* Vector = 30 */
#define FRTRCV_30___YOUR_TRCV___MODULE_ID (71u) /* FlexRay Trcv = 71 */

#define FRTRCV_30___YOUR_TRCV___INSTANCE_ID (0u)

/**************************************************************************************************
* Global defines
**************************************************************************************************/

/* Development errors */
/* Definition of the API ID */
#define FRTRCV_30___YOUR_TRCV___TRCVINIT_SERVICE_ID                     0u
#define FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID           1u
#define FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERMODE_SERVICE_ID           5u
#define FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERWUREASON_SERVICE_ID       6u
#define FRTRCV_30___YOUR_TRCV___GETVERSIONINFO_SERVICE_ID               7u
#define FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERERROR_SERVICE_ID          8u
#define FRTRCV_30___YOUR_TRCV___DISABLETRANSCEIVERWAKEUP_SERVICE_ID    10u
#define FRTRCV_30___YOUR_TRCV___ENABLETRANSCEIVERWAKEUP_SERVICE_ID     11u
#define FRTRCV_30___YOUR_TRCV___CLEARTRANSCEIVERWAKEUP_SERVICE_ID      12u
#define FRTRCV_30___YOUR_TRCV___MAINFUNCTION_SERVICE_ID                13u
#define FRTRCV_30___YOUR_TRCV___CBK_WAKEUPBYTRANSCEIVER_SERVICE_ID     14u
#define FRTRCV_30___YOUR_TRCV___DISABLETRANSCEIVERBRANCH_SERVICE_ID    15u
#define FRTRCV_30___YOUR_TRCV___ENABLETRANSCEIVERBRANCH_SERVICE_ID     16u

/* FrTrcv085: Definition of errors/exceptions */
#define FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX                 0x01u
#define FRTRCV_30___YOUR_TRCV___E_FR_UNINIT                          0x10u
#define FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_STANDBY                0x11u
#define FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_NORMAL                 0x12u
#define FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_SLEEP                  0x13u
#define FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_RECEIVEONLY            0x14u
/* Additional development errors, not officially specified yet */
#define FRTRCV_30___YOUR_TRCV___E_FR_INVALID_POINTER                 0x20u

/* Interrupt enable/disable call-backs. Used to disable interrupts in
   Normal operation mode and enable them in low-power modes. Useful if
   Trcv Driver is used in interrupt mode.

   Activating this will enable the call-backs:
      Appl_FrTrcv_30___Your_Trcv___EnableIcuNotification(uint8 FrTrcv_TrcvIdx)
      Appl_FrTrcv_30___Your_Trcv___DisableIcuNotification(uint8 FrTrcv_TrcvIdx)
*/
#define FRTRCV_30___YOUR_TRCV___USE_ICU                        STD_OFF

/* Timer configuration */

/* example for TJA1080
   Activate this to enable the usage of hardware timers. For the Tja1080
   these timeres are needed to wait some time after some mode transitions.
   
   Activating this will enable the call-back:
      Appl_FrTrcv_30___Your_Trcv___Waitus(uint8 delay)
*/
# define FRTRCV_30___YOUR_TRCV___USE_TIMERS                    STD_ON

<Your_Trcv_Code>

/* Predefined timer indexes (0x01 - 0x0f)                                   */
# define kFrTrcv_30___Your_Trcv___SetMode                      0x01

/* Aditional timer indexes (0x10 - 0xff)                                    */
/* example 
# define kFrTrcv_30___Your_Trcv___<identifier>                 0x10
*/
<Your_Trcv_Code>

/**************************************************************************************************
* Global data types and structures
**************************************************************************************************/

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
  #if !defined ( BUSTRCV_E_OK )
    #define BUSTRCV_E_OK E_OK
  #endif
  #if !defined ( BUSTRCV_E_ERROR )
    #define BUSTRCV_E_ERROR E_NOT_OK
  #endif
  #define BTR_30_GENERIC_RETURN_TYPE Std_ReturnType
#else
  #define BTR_30_GENERIC_RETURN_TYPE BusTrcvErrorType
#endif

typedef struct
{
  FrTrcv_TrcvModeType    FrTrcv_InitState;       /* The initial operation mode */
  EcuM_WakeupSourceType  FrTrcv_WakeupSourceRef; /* Wake Up Source Reference */
  uint8                  FrTrcv_WakeupSupported; /* Is Wakeup detection supported */
  uint8                  FrTrcv_WakeupPolling; /* Is Wakeup detected by polling oder isr */
} FrTrcv_30_Generic_GenConfigType;

/**************************************************************************************************
* Prototypes of export variables
**************************************************************************************************/

/****************************************************************************/
/* Constants for export with unspecified size                               */
/****************************************************************************/
#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_CONST_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

extern CONST( FrTrcv_30_Generic_GenConfigType, FRTRCV_30___YOUR_TRCV___CONST ) FrTrcv_30___Your_Trcv___Config[FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES];
#if ( FRTRCV_30_GENERIC_PROD_ERROR_DETECT == STD_ON )
#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
  extern CONST(Dem_EventIdType, FRTRCV_30___YOUR_TRCV___CONST) FrTrcv_30_Generic_DEM_E_NoControl;
# else
  extern CONST( Dem_EventIdType, FRTRCV_30___YOUR_TRCV___CONST ) FrTrcv_DEM_E_NoControl;
# endif
#endif
extern CONST( uint32, FRTRCV_30___YOUR_TRCV___CONST ) FrTrcv_GeneratorVersion;

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**************************************************************************************************
* Prototypes of export functions
**************************************************************************************************/

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_CODE
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___InitMemory( void );
#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
  extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___Init( void );
#else
  extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___TrcvInit( uint8 FrTrcv_TrcvIdx );
#endif

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
#else
  #if ( FRTRCV_30_GENERIC_MAIN_FUNCTION_CYCLE_TIME > 0 )
    extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___MainFunction( void );
  #else
    #define FrTrcv_30___Your_Trcv___MainFunction()
  #endif
#endif

#if ( FRTRCV_30_GENERIC_VERSION_INFO_API == STD_ON )
  extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) versioninfo);
#endif
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___SetTransceiverMode(uint8 FrTrcv_TrcvIdx, FrTrcv_TrcvModeType FrTrcv_TrcvMode);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetTransceiverMode(uint8 FrTrcv_TrcvIdx, P2VAR(FrTrcv_TrcvModeType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) FrTrcv_TrcvModePtr);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetTransceiverWUReason(uint8 FrTrcv_TrcvIdx, P2VAR(FrTrcv_TrcvWUReasonType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) FrTrcv_TrcvWUReasonPtr); /* PRQA S 0777 */ /* MD_FRTRCV_0777 */
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___DisableTransceiverWakeup(uint8 FrTrcv_TrcvIdx);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___EnableTransceiverWakeup(uint8 FrTrcv_TrcvIdx);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___ClearTransceiverWakeup(uint8 FrTrcv_TrcvIdx);

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_CODE) FrTrcv_30___Your_Trcv___DisableTransceiverBranch(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_CODE) FrTrcv_30___Your_Trcv___EnableTransceiverBranch(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx);
extern FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_CODE) FrTrcv_30___Your_Trcv___GetTransceiverError(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx, P2VAR(uint32, AUTOMATIC, FRTRCV_APPL_DATA) FrTrcv_BusErrorState);
#endif

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_CODE
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/***** end of header file ************************************************************************/
#endif
  /* ifndef FRTRCV_30___YOUR_TRCV___H */
