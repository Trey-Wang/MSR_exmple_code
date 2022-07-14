
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
 *         File:  FrTrcv_30___Your_Trcv__.c
 *    Component:  MICROSAR FR Transceiver Driver
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  FlexRay transceiver driver implementation, according to:
 *                AUTOSAR FlexRay Transceiver Driver, AUTOSAR Release 3.0
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ATTENTION: Replace the placeholders __Your_Trcv__ and __YOUR_TRCV__ with the according name of the used transceiver.
 *            __YOUR_TRCV__ is used for definitions in upper case.
 *            __Your_Trcv__ is used for variables in camel case.
 * EXAMPLE:   Replace __YOUR_TRCV__ to TJA1080 and __Your_Trcv__ to Tja1080.
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

/**************************************************************************************************
* Include files
**************************************************************************************************/
/* FlexRay transceiver driver declarations */
#include "FrTrcv_30___Your_Trcv__.h"
#include "FrTrcv_30___Your_Trcv___Cbk.h"
#include "dio.h"

#if ( FRTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"
#endif

/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
/* Check consistency of source and header file. */
#if defined ( DRVTRANS_GENERICFRASR_VERSION )
  #if ( DRVTRANS_GENERICFRASR_VERSION != 0x0200u )
    #error "Source and Header file are inconsistent!"
  #endif
  #if ( DRVTRANS_GENERICFRASR_RELEASE_VERSION != 0x01u )
    #error "Source and Header file are inconsistent!"
  #endif
#endif

/**************************************************************************************************
* Defines / Constants
**************************************************************************************************/

/**************************************************************************************************
* Typedef and Struct definitions
**************************************************************************************************/
typedef uint8 FrTrcv_InitStatusType;

typedef struct
{
  Dio_ChannelType   TrcvPinEN;
  Dio_ChannelType   TrcvPinSTBN;
  Dio_ChannelType   TrcvPinERRN;
  Dio_ChannelType   TrcvPinRXEN;
} FrTrcvChannelType;

/****************************************************************************/
/* Constants with unspecified size                                          */
/****************************************************************************/
#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_CONST_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/* These are the I/O pins used for Transceiver access */
/* Currently one Transceiver channel is defined. Adjust this to your needs */
<Your_Trcv_Code>
STATIC CONST( FrTrcvChannelType, FRTRCV_30___YOUR_TRCV___CONST ) FrTrcv_30___Your_Trcv___Channel[] =
{
  { /* I/O used for Transceiver 1 */
    (Dio_ChannelType)FRTRCV_CHANNEL_EN_0,
    (Dio_ChannelType)FRTRCV_CHANNEL_STBN_0,
    (Dio_ChannelType)FRTRCV_CHANNEL_ERRN_0,
    (Dio_ChannelType)FRTRCV_CHANNEL_RXEN_0,
  }
  /* ..... */
  /* Add more Transceivers, if required */
};

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_CONST_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**************************************************************************************************
* Macros
**************************************************************************************************/
/* FrTrcv051:, FrTrcv061:, FrTrcv037: */
#if (FRTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
/* If DET is enabled, a MISRA-C:2004 analysis will yield deviations to the rules:
*  - 14.3 Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment provided that the first character following the null statement is a white-space character.
*  - 14.7 A function shall have a single point of exit at the end of the function
*  - 19.4 C macros shall only expand to a braced initializer, a constant, a parenthesized expression, a type qualifier, a storage class specifier, or a do-while-zero construct
*  - 19.7 A function should be used in preference to a function-like macro.
*  These deviations are caused by design of the runtime/ressource optimized DET handling and are globally justified.
*/
#define FrTrcv_30_Generic_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  Det_ReportError( FRTRCV_30___YOUR_TRCV___MODULE_ID, FRTRCV_30___YOUR_TRCV___INSTANCE_ID, (API_ID), (ERROR_CODE)); return; } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FrTrcv_30_Generic_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) { if(!(CONDITION)) { \
  Det_ReportError( FRTRCV_30___YOUR_TRCV___MODULE_ID, FRTRCV_30___YOUR_TRCV___INSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FrTrcv_30_Generic_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  Det_ReportError( FRTRCV_30___YOUR_TRCV___MODULE_ID, FRTRCV_30___YOUR_TRCV___INSTANCE_ID, (API_ID), (ERROR_CODE)); } } /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTrcv_30_Generic_CallDetReportError( API_ID, ERROR_CODE ) Det_ReportError( FRTRCV_30___YOUR_TRCV___MODULE_ID, FRTRCV_30___YOUR_TRCV___INSTANCE_ID, (API_ID),(ERROR_CODE))
#else
#define FrTrcv_30_Generic_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
#define FrTrcv_30_Generic_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) 
#define FrTrcv_30_Generic_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) 
#define FrTrcv_30_Generic_CallDetReportError( API_ID, ERROR_CODE ) 
#endif

/* FrTrcv052: */
#if ( FRTRCV_30_GENERIC_PROD_ERROR_DETECT == STD_ON )
  #define FrTrcv_30_Generic_DemReportError(eventId) ((void)Dem_ReportErrorStatus((Dem_EventIdType)(eventId), (uint8)(DEM_EVENT_STATUS_FAILED))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
  #define FrTrcv_30_Generic_DemReportError(eventId)
#endif

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
  #define FrTrcv_30_Generic_EnterCritical() SchM_Enter_FrTrcv_FRTRCV_30_GENERIC_EXCLUSIVE_AREA_0
  #define FrTrcv_30_Generic_LeaveCritical() SchM_Exit_FrTrcv_FRTRCV_30_GENERIC_EXCLUSIVE_AREA_0
#else
  #define FrTrcv_30_Generic_EnterCritical() SchM_Enter_FrTrcv(FRTRCV_30_EXCLUSIVE_AREA_0)
  #define FrTrcv_30_Generic_LeaveCritical() SchM_Exit_FrTrcv(FRTRCV_30_EXCLUSIVE_AREA_0)
#endif

#define FRTRCV_30___YOUR_TRCV___UNINIT  (FrTrcv_InitStatusType)0u /* The FrTrcv is not initialized and is not usable. */
#define FRTRCV_30___YOUR_TRCV___INIT    (FrTrcv_InitStatusType)1u /* The FrTrcv is initialized and is usable. */

/**************************************************************************************************
* Data definitions
**************************************************************************************************/

/******************************************************************************/
/* Local Data definitions with unspecified size                               */
/******************************************************************************/
#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_VAR_NOINIT_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

STATIC VAR( FrTrcv_TrcvWUReasonType, FRTRCV_30___YOUR_TRCV___VAR_NOINIT ) FrTrcv_30___Your_Trcv___TrcvWUReason[FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES];
STATIC VAR( uint8, FRTRCV_30___YOUR_TRCV___VAR_NOINIT ) FrTrcv_30___Your_Trcv___WakeupCbEnabled[FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES];
STATIC VAR( uint8, FRTRCV_30___YOUR_TRCV___VAR_NOINIT ) FrTrcv_30___Your_Trcv___WakeupDetected[FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES];
STATIC VAR( FrTrcv_InitStatusType, FRTRCV_30___YOUR_TRCV___VAR_NOINIT ) FrTrcv_30___Your_Trcv___IsInitialized[FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES];

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_VAR_NOINIT_UNSPECIFIED
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**************************************************************************************************
* Prototypes for local functions
**************************************************************************************************/

/**************************************************************************************************
*  Implementation
**************************************************************************************************/

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_CODE
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___InitMemory
 **********************************************************************************************************************/
/*! \brief      Initialize static variables
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___InitMemory( void )
{
  uint8_least kTrcvChannel;

  for(kTrcvChannel = (uint8_least)0u; kTrcvChannel < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES; kTrcvChannel++ )
  {
    FrTrcv_30___Your_Trcv___IsInitialized[kTrcvChannel] = FRTRCV_30___YOUR_TRCV___UNINIT;
  }
}

/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___TrcvInit
 **********************************************************************************************************************/
/*! \brief      Initialize the FlexRay Transceiver
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___Init( void )
#else
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___TrcvInit( uint8 FrTrcv_TrcvIdx )
#endif
{
  uint8 i;

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
    for(i = 0; i < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES; i++)
#else
    i = FrTrcv_TrcvIdx;
    FrTrcv_30_Generic_CheckDetErrorReturnVoid(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                                                  FRTRCV_30___YOUR_TRCV___TRCVINIT_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX)
#endif
  {
    /* FrTrcv271: */
    FrTrcv_30___Your_Trcv___IsInitialized[i]   = FRTRCV_30___YOUR_TRCV___UNINIT;
    FrTrcv_30___Your_Trcv___WakeupDetected[i]  = (uint8)0u;
    FrTrcv_30___Your_Trcv___WakeupCbEnabled[i] = (uint8)0u;
    FrTrcv_30___Your_Trcv___TrcvWUReason[i]    = FRTRCV_WU_NOT_SUPPORTED;
    FrTrcv_30___Your_Trcv___IsInitialized[i]   = FRTRCV_30___YOUR_TRCV___INIT;
    if( FrTrcv_30___Your_Trcv___Config[i].FrTrcv_WakeupSupported > (uint8)0)
    {
      FrTrcv_30___Your_Trcv___WakeupCbEnabled[i] = (uint8)1u;
      FrTrcv_30___Your_Trcv___TrcvWUReason[i] = FRTRCV_WU_RESET;
      /* FrTrcv269: Check for pending wakeup events */
      FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver( i );
    }

    /* FrTrcv291: */
    /* FrTrcv270: Set the transceiver to a default state */
    if(BUSTRCV_E_OK != FrTrcv_30___Your_Trcv___SetTransceiverMode(i, FrTrcv_30___Your_Trcv___Config[i].FrTrcv_InitState))
    {
#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
      FrTrcv_30_Generic_DemReportError(FrTrcv_30_Generic_DEM_E_NoControl);
#else
      FrTrcv_30_Generic_DemReportError(FrTrcv_DEM_E_NoControl);
#endif
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv373: */
#if ( FRTRCV_30_GENERIC_MAIN_FUNCTION_CYCLE_TIME > 0 )
/* FrTrcv330:, FrTrcv122: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___MainFunction
 **********************************************************************************************************************/
/*! \brief      Cyclic task
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___MainFunction( void )
{
  uint8 kTrcvChannel;

  for(kTrcvChannel = (uint8)0u; kTrcvChannel < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES; kTrcvChannel++ )
  { 
    if(FRTRCV_30___YOUR_TRCV___INIT == FrTrcv_30___Your_Trcv___IsInitialized[kTrcvChannel])
    {
      if( FrTrcv_30___Your_Trcv___Config[kTrcvChannel].FrTrcv_WakeupPolling > (uint8)0 )
      {
        /* Check for pending wakeup events */
        FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver(kTrcvChannel);
      }
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/* FrTrcv339: */
#if ( FRTRCV_30_GENERIC_VERSION_INFO_API == STD_ON )
/* FrTrcv326: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      Query module version information
 *  \param[out] versioninfo             Pointer to version information structure
 *  \pre        Input parameter must not be NULL.
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) versioninfo)
{
  FrTrcv_30_Generic_CheckDetErrorReturnVoid(NULL_PTR != versioninfo,
               FRTRCV_30___YOUR_TRCV___GETVERSIONINFO_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_POINTER)
  /* FrTrcv285: */
  versioninfo->vendorID         = (uint16)FRTRCV_30___YOUR_TRCV___VENDOR_ID;
  versioninfo->moduleID         = (uint8)FRTRCV_30___YOUR_TRCV___MODULE_ID;
  #if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
  #else
    versioninfo->instanceID       = (uint8)0u;
  #endif
  versioninfo->sw_major_version = (uint8)FRTRCV_30___YOUR_TRCV___SW_MAJOR_VERSION;
  versioninfo->sw_minor_version = (uint8)FRTRCV_30___YOUR_TRCV___SW_MINOR_VERSION;
  versioninfo->sw_patch_version = (uint8)FRTRCV_30___YOUR_TRCV___SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/* FrTrcv323: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___SetTransceiverMode
 **********************************************************************************************************************/
/*! \brief      Changes the operation mode of the selected transceiver
 *  \param[in]  FrTrcv_TrcvIdx                 The selected FlexRay bus transceiver
 *  \param[in]  FrTrcv_TrcvModeType            The Transceive mode that shall be set
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___SetTransceiverMode(uint8 FrTrcv_TrcvIdx, FrTrcv_TrcvModeType FrTrcv_TrcvMode)
{
  FrTrcv_TrcvModeType FrTrcv_ActualTrcvMode;
  /* FrTrcv275: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv277: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)

  FrTrcv_30_Generic_EnterCritical();
  switch(FrTrcv_TrcvMode)
  {
    case FRTRCV_TRCVMODE_NORMAL :
                  <Your_Trcv_Code>
                  /* example for TJA1080 */
/*
# if (FRTRCV_30___YOUR_TRCV___USE_ICU == STD_ON)
                  Appl_FrTrcv_30___Your_Trcv___DisableIcuNotification(FrTrcv_TrcvIdx);
# endif
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN, STD_HIGH);
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinEN, STD_HIGH);
*/
                  /* Wait 80us to counter delay of EN */
/*                  Appl_FrTrcv_30___Your_Trcv___Wait(kFrTrcv_30___Your_Trcv___SetMode);
*/
                  break;

    case FRTRCV_TRCVMODE_STANDBY :
                  <Your_Trcv_Code>
                  /* example for TJA1080 */
/*                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinEN, STD_LOW);
*/
                  /* Wait 80us to counter delay of EN */
/*                  Appl_FrTrcv_30___Your_Trcv___Wait(kFrTrcv_30___Your_Trcv___SetMode);
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN, STD_LOW);
# if (FRTRCV_30___YOUR_TRCV___USE_ICU == STD_ON)
                  Appl_FrTrcv_30___Your_Trcv___EnableIcuNotification(FrTrcv_TrcvIdx);
# endif
*/
                  break;

    case FRTRCV_TRCVMODE_SLEEP :
                  <Your_Trcv_Code>
                  /* example for TJA1080 */
/*                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN, STD_LOW);
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinEN, STD_HIGH);
*/
                  /* Wait 80us to counter delay of EN */
/*                  Appl_FrTrcv_30___Your_Trcv___Wait(kFrTrcv_30___Your_Trcv___SetMode);
# if (FRTRCV_30___YOUR_TRCV___USE_ICU == STD_ON)
                  Appl_FrTrcv_30___Your_Trcv___EnableIcuNotification(FrTrcv_TrcvIdx);
# endif
*/
                  break;

    case FRTRCV_TRCVMODE_RECEIVEONLY :
                  <Your_Trcv_Code>
                  /* example for TJA1080 */
/*
# if (FRTRCV_30___YOUR_TRCV___USE_ICU == STD_ON)
                  Appl_FrTrcv_30___Your_Trcv___DisableIcuNotification(FrTrcv_TrcvIdx);
# endif
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN, STD_HIGH);
                  Dio_WriteChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinEN, STD_LOW);
*/
                  /* Wait 80us to counter delay of EN */
/*                  Appl_FrTrcv_30___Your_Trcv___Wait(kFrTrcv_30___Your_Trcv___SetMode);
*/
                  break;
    default :
      FrTrcv_30_Generic_LeaveCritical();
      /* FrTrcv273: */
      return( (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR );
  }
  FrTrcv_30_Generic_LeaveCritical();

  /* FrTrcv356:, FrTrcv237: */
  if(BUSTRCV_E_ERROR == FrTrcv_30___Your_Trcv___GetTransceiverMode(FrTrcv_TrcvIdx, &FrTrcv_ActualTrcvMode))
  {
    return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR);
  }
  if(FrTrcv_ActualTrcvMode != FrTrcv_TrcvMode)
  {
    #if (FRTRCV_30_GENERIC_DEV_ERROR_DETECT == STD_ON)
    switch(FrTrcv_TrcvMode)
    {
      case FRTRCV_TRCVMODE_NORMAL : FrTrcv_30_Generic_CallDetReportError( FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_NORMAL ); break;
      case FRTRCV_TRCVMODE_STANDBY : FrTrcv_30_Generic_CallDetReportError( FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_STANDBY ); break;
      case FRTRCV_TRCVMODE_SLEEP : FrTrcv_30_Generic_CallDetReportError( FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_SLEEP ); break;
      case FRTRCV_TRCVMODE_RECEIVEONLY : FrTrcv_30_Generic_CallDetReportError( FRTRCV_30___YOUR_TRCV___SETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_TRCV_NOT_RECEIVEONLY ); break;
      default : return( (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR ); /* PRQA S 2018 */ /* MD_FRTRCV_2018 */
    }
    #endif
    return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR);
  }

  if( FrTrcv_30___Your_Trcv___Config[FrTrcv_TrcvIdx].FrTrcv_WakeupSupported > (uint8)0)
  {
    /* FrTrcv375: */
    FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver(FrTrcv_TrcvIdx);
  }

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv324: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___GetTransceiverMode
 **********************************************************************************************************************/
/*! \brief      Reads the operation mode of the selected transceiver
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetTransceiverMode(uint8 FrTrcv_TrcvIdx, P2VAR(FrTrcv_TrcvModeType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) FrTrcv_TrcvModePtr)
{
  uint8 trcvState;
  /* FrTrcv279: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv280: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  FrTrcv_30_Generic_CheckDetErrorReturnValue(NULL_PTR != FrTrcv_TrcvModePtr,
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERMODE_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_POINTER, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)

  /* FrTrcv231: */

  trcvState = 0;
  <Your_Trcv_Code>
  /* example for TJA1080 */
/*
  if(STD_HIGH == Dio_ReadChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN))
  {
    trcvState |= 0x01;
  }
  if(STD_HIGH == Dio_ReadChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinEN))
  {
    trcvState |= 0x02;
  }
*/
  switch(trcvState)
  {  /* FrTrcv253: */
    case 0: *FrTrcv_TrcvModePtr = FRTRCV_TRCVMODE_STANDBY; break;
    case 1: *FrTrcv_TrcvModePtr = FRTRCV_TRCVMODE_RECEIVEONLY; break;
    case 2: *FrTrcv_TrcvModePtr = FRTRCV_TRCVMODE_SLEEP; break;
    case 3: *FrTrcv_TrcvModePtr = FRTRCV_TRCVMODE_NORMAL; break;
    default: return( (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR ); /* PRQA S 2018 */ /* MD_FRTRCV_2018 */
  }

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv325: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___GetTransceiverWUReason
 **********************************************************************************************************************/
/*! \brief      Returns the wakeup reason of the selected bus
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___GetTransceiverWUReason(uint8 FrTrcv_TrcvIdx, P2VAR(FrTrcv_TrcvWUReasonType, AUTOMATIC, FRTRCV_30___YOUR_TRCV___APPL_DATA) FrTrcv_TrcvWUReasonPtr)
{ 
  /* FrTrcv282: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERWUREASON_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv283: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERWUREASON_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  FrTrcv_30_Generic_CheckDetErrorReturnValue(NULL_PTR != FrTrcv_TrcvWUReasonPtr,
                  FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERWUREASON_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_POINTER, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv232 */
  *FrTrcv_TrcvWUReasonPtr = FrTrcv_30___Your_Trcv___TrcvWUReason[FrTrcv_TrcvIdx];
  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv327: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___DisableTransceiverWakeup
 **********************************************************************************************************************/
/*! \brief      Disables the notification for wakeup events on the selected bus
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \pre        Input parameter must not be NULL.
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___DisableTransceiverWakeup(uint8 FrTrcv_TrcvIdx)
{
  /* FrTrcv288: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___DISABLETRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv289: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___DISABLETRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv286: */
  FrTrcv_30___Your_Trcv___WakeupCbEnabled[FrTrcv_TrcvIdx] = (uint8)0u;
  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv328: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___EnableTransceiverWakeup
 **********************************************************************************************************************/
/*! \brief      Enables the notification for wakeup events on the selected bus
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___EnableTransceiverWakeup(uint8 FrTrcv_TrcvIdx)
{
  /* FrTrcv301: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___ENABLETRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv302: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___ENABLETRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv245 */
  FrTrcv_30___Your_Trcv___WakeupCbEnabled[FrTrcv_TrcvIdx] = (uint8)1u;
  /* FrTrcv300: */
  FrTrcv_30_Generic_EnterCritical();
  /* FrTrcv370: */
  if( (uint8)1u == FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx] )
  {
    FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx] = (uint8)0u;
    FrTrcv_30_Generic_LeaveCritical();
    /* FrTrcv300: */
    EcuM_SetWakeupEvent(FrTrcv_30___Your_Trcv___Config[FrTrcv_TrcvIdx].FrTrcv_WakeupSourceRef);
  }
  else
  {
    FrTrcv_30_Generic_LeaveCritical();
  }

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/* FrTrcv329: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___ClearTransceiverWakeup
 **********************************************************************************************************************/
/*! \brief      Clears pending wakeup events
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \return     BUSTRCV_E_ERROR
 *  \return     BUSTRCV_E_OK
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___ClearTransceiverWakeup(uint8 FrTrcv_TrcvIdx)
{
  /* FrTrcv304: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
                  FRTRCV_30___YOUR_TRCV___CLEARTRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv305: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
                  FRTRCV_30___YOUR_TRCV___CLEARTRANSCEIVERWAKEUP_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)

  FrTrcv_30_Generic_EnterCritical();
  /* FrTrcv247:, FrTrcv371: */
  FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx] = (uint8)0u;
  FrTrcv_30___Your_Trcv___TrcvWUReason[FrTrcv_TrcvIdx]   = FRTRCV_WU_RESET;
  FrTrcv_30_Generic_LeaveCritical();

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if defined (DRVTRANS_30_GENERIC_AUTOSARVERSION) && (DRVTRANS_30_GENERIC_AUTOSARVERSION == 4)
/* FrTrcv442: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___DisableTransceiverBranch
 **********************************************************************************************************************/
/*! \brief      Disables a specific transceiver branch
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \param[in]  FrTrcv_BranchIdx        The selected FlexRay bus transceiver branch
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___DisableTransceiverBranch(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx)
{
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
    /* avoid compiler warning due to unused parameters */
    FrTrcv_TrcvIdx = FrTrcv_TrcvIdx;
    FrTrcv_BranchIdx = FrTrcv_BranchIdx;
  #endif

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
}

/* FrTrcv443: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___EnableTransceiverBranch
 **********************************************************************************************************************/
/*! \brief      Enables a specific transceiver branch
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \param[in]  FrTrcv_BranchIdx        The selected FlexRay bus transceiver branch
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___EnableTransceiverBranch(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx)
{
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
    /* avoid compiler warning due to unused parameters */
    FrTrcv_TrcvIdx = FrTrcv_TrcvIdx;
    FrTrcv_BranchIdx = FrTrcv_BranchIdx;
  #endif

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
}

/* FrTrcv419: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___GetTransceiverError
 **********************************************************************************************************************/
/*! \brief      Returns the error status of a specified branch
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \param[in]  FrTrcv_BranchIdx        The selected FlexRay bus transceiver branch
 *  \param[in]  FrTrcv_BusErrorState    The detailed bus error state
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(BTR_30_GENERIC_RETURN_TYPE, FRTRCV_CODE) FrTrcv_30___Your_Trcv___GetTransceiverError(uint8 FrTrcv_TrcvIdx, uint8 FrTrcv_BranchIdx, P2VAR(uint32, AUTOMATIC, FRTRCV_APPL_DATA) FrTrcv_BusErrorState)
{
  /* FrTrcv459: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],  
               FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERERROR_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv460: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,  
               FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERERROR_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)
  /* FrTrcv439: */
  FrTrcv_30_Generic_CheckDetErrorReturnValue(NULL_PTR != FrTrcv_BusErrorState,  
               FRTRCV_30___YOUR_TRCV___GETTRANSCEIVERERROR_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_POINTER, (BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_ERROR)

  *FrTrcv_BusErrorState = 0u;
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
    FrTrcv_TrcvIdx = FrTrcv_TrcvIdx;
    FrTrcv_BranchIdx = FrTrcv_BranchIdx;
  #endif

  return((BTR_30_GENERIC_RETURN_TYPE)BUSTRCV_E_OK);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* DRVTRANS_30_GENERIC_AUTOSARVERSION */

/* FrTrcv364, FrTrcv367:, FrTrcv331:, FrTrcv379:, FrTrcv380: */
/***********************************************************************************************************************
 *  FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver
 **********************************************************************************************************************/
/*! \brief      Is triggered by interrupts or used for polling
 *  \param[in]  FrTrcv_TrcvIdx          The selected FlexRay bus transceiver
 *  \context    Function could be called from interrupt level or from task level
 **********************************************************************************************************************/
FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver(uint8 FrTrcv_TrcvIdx)
{
  /* FrTrcv312: */
  FrTrcv_30_Generic_CheckDetErrorReturnVoid(FrTrcv_TrcvIdx < FRTRCV_30_GENERIC_MAX_NUM_OF_BTR_BUSSES,
               FRTRCV_30___YOUR_TRCV___CBK_WAKEUPBYTRANSCEIVER_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_INVALID_TRCVIDX)
  /* FrTrcv313: */
  FrTrcv_30_Generic_CheckDetErrorReturnVoid(FRTRCV_30___YOUR_TRCV___UNINIT != FrTrcv_30___Your_Trcv___IsInitialized[FrTrcv_TrcvIdx],
               FRTRCV_30___YOUR_TRCV___CBK_WAKEUPBYTRANSCEIVER_SERVICE_ID, FRTRCV_30___YOUR_TRCV___E_FR_UNINIT)

  /* We only detect FRTRCV_REMOTEWAKEUP for now, see ESCAN00027346 for details */
  /* The TrcvDrv does not differentiate between different wake up events but generates the same wakeup notification
     for any of them. This is in accordance with the AUTOSAR SWS 1.2.1 but leads to the problem that the ECUM and the
     ComM start the FlexRay bus even if only a POWER_UP wake up is detected. */
  /* Verify Wakeup */
  <Your_Trcv_Code>
  /* example for TJA1080 */
  FrTrcv_30_Generic_EnterCritical();
  /* FrTrcv287: */
/*
  if(STD_LOW == Dio_ReadChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinSTBN))
  {
    if(STD_LOW == Dio_ReadChannel(FrTrcv_30___Your_Trcv___Channel[FrTrcv_TrcvIdx].TrcvPinRXEN))
    {
      FrTrcv_30___Your_Trcv___TrcvWUReason[FrTrcv_TrcvIdx]   = FRTRCV_WU_BY_BUS;
      FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx] = (uint8)1u;
    }
  }
*/
  FrTrcv_30_Generic_LeaveCritical();

  /* FrTrcv369: */
  if( ((uint8)1u == FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx])
   && ((uint8)1u == FrTrcv_30___Your_Trcv___WakeupCbEnabled[FrTrcv_TrcvIdx]) )
  { /* Wakeup callbacks enabled? */
    /* FrTrcv374: */
    FrTrcv_30___Your_Trcv___WakeupDetected[FrTrcv_TrcvIdx] = (uint8)0u;
    /* FrTrcv362, FrTrcv363 */
    EcuM_SetWakeupEvent(FrTrcv_30___Your_Trcv___Config[FrTrcv_TrcvIdx].FrTrcv_WakeupSourceRef);
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_CODE
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/* module specific MISRA deviations:
  MD_FRTRCV_2018: Rule 14.1
      Reason:     Improve robustness by implicitly checking for illegal values.
      Risk:       No Risk.
      Prevention: Covered by code review.
  MD_FRTRCV_0777: Rule 5.1
      Reason:     Naming is given by AUTOSAR.
      Risk:       Maintainability reduced due to AUTOSAR standard conformance.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: FrTrcv_30___Your_Trcv__.c
 *********************************************************************************************************************/
