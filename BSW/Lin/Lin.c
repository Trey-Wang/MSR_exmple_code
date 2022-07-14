
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
 *           File:  Lin.c
 *      Component:  AUTOSAR LIN Driver
 *         Module:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *      Generator:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *
 *  Target system:  Canoe
 *       Compiler:  Ansi

 *    Derivatives:  VTT/EMU
 *
 *    Description:  Implementation of the AUTOSAR LIN Driver
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2013-08-12  vislpr  -             Port Canoeemu to VIP
 *  05.01.00  2014-07-17  vislpr  -             Atomic access to VipCntrl, bugfixing
 *  06.00.00  2014-09-29  vislpr  -             Add Canoeemu to VIP, ComStackLib, R11, PB-S
 *  06.00.01  2014-11-20  vislpr  -             HL Part was updated
 *  07.00.00  2015-01-29  vislpr  ESCAN00080255 FEAT-254: Globale Umbennenung von VIP nach VTT
 *                        vislpr  ESCAN00081345 FEAT-427: SafeBSW Step I
 *  07.01.00  2015-06-15  vislpr  ESCAN00083109 VTT Lin shall derive its interrupt offset from the values configured in the OS
 *  07.02.00  2015-06-15  vislpr  -             Remove VTT dependencies 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Friedrich Kiesel              visfki        Vector Informatik GmbH
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY - CORE -
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2012-07-20  visfki  ESCAN00059300 Implementation of High Level AUTOSAR 4 LIN Driver
 *  04.01.00  2012-11-20  visfki  ESCAN00061782 AR4-220: Remove STATIC
 *                                ESCAN00063147 Rename Lin_WakeUp to Lin_Wakeup
 *  04.02.00  2013-02-08  visfki  ESCAN00064956 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00065149 Compiler error: LIN_CH_OPERATIONAL changed in ASR4 to LIN_OPERATIONAL
 *  05.00.00  2013-09-19  vislpr  ESCAN00066851 Improve include structure for EcuM headers
 *                                ESCAN00079533 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00069952 Justify MISRA Warnings on functions
 *                                ESCAN00067708 AR4-328: Predefined Runtime Measurement Points
 *  05.00.01  2013-11-19  vislpr  ESCAN00071647 Remove MISRA warning 0850/3453
 *                                ESCAN00072030 Change RTM macros
 *  05.00.02  2013-11-21  vislpr  ESCAN00072030 Fix MISRA Warnings and GNU Compiler error on RTM
 *  06.00.00  2014-10-01  vislpr  ESCAN00076482 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00078696 AR4-698: Post-Build Selectable (Identity Manager)
 *  06.00.01  2014-11-14  vislpr  ESCAN00079471 Compiler error: lin.c: 'Lin_Dev_InitDetect'/'LIN_DEV_INITVALUE'
 *  06.00.02  2014-11-14  vislpr  ESCAN00079588 Compiler error: Identifier Channel not declared
 *  06.00.03  2014-12-02  vislpr  -             Change wakeup handling
 *  07.00.00  2014-12-12  vislpr  ESCAN00080307 FEAT-427: SafeBSW Step I
 *  07.00.01  2015-01-27  vislpr  ESCAN00080823 Add channel DET check for out of bounds of channel id and channel idx
 *                        vislpr  ESCAN00080824 Use LOCAL_INLINE macro of Compiler.h instead "static inline"
 *  07.00.02  2015-03-26  vislpr  ESCAN00082080 Remove redundant code in Lin_GoToSleep
 *  07.00.03  2015-05-26  vislpr  ESCAN00083154 Dem.h not included if Dem repoting enabled
 *                        vislpr  ESCAN00083156 EcuM_BswErrorHook() are used on precompile with Selectable enabled
 *  07.01.00  2015-07-13  vislpr  ESCAN00081794 FEAT-1275: SafeBSW Step 2
 *  07.01.01  2015-09-15  vislpr  ESCAN00085267 Unkown return value 0x01u of Lin_GetStatus() in case of LIN_TX_BUSY error
 *  07.01.02  2015-10-23  vislpr  ESCAN00086056 Check data length parameter of Lin_SendFrame()
 *  07.02.00  2015-11-04  vislpr  ESCAN00086254 Unification the register base address struct and typedef for each driver
 *            2015-11-04  vislpr  ESCAN00086267 Code refactoring
 *            2015-11-16  vislpr  ESCAN00086493 Remove Lin_GetRxPin() Macro.
 *            2015-11-16  vislpr  ESCAN00086494 Do not set the state to wake after calling Lin_CheckWakeup()
 *            2015-11-17  vislpr  ESCAN00086546 Inconsistency of wakeup support between Lin, LinIf and EcuM in variant Post Build
 *********************************************************************************************************************/

#define LIN_SOURCE
/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* Disable the following MISRA warnings because they would appear too often for individual suppression.
   See justification at the end of file */

/* PRQA S 0303 EOF */ /* MD_LIN_0303 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Lin.h"
#include "EcuM_Cbk.h"
#include "LinIf_Cbk.h"

#if ( LIN_DEV_ERROR_DETECT == STD_ON )
# include "Det.h"
#endif

#if ( LIN_E_TIMEOUT_TYPE_DET == STD_OFF ) 
# include "Dem.h"
#endif

#if ( LIN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
# include "Rtm.h"
#endif

#if ( LIN_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
# include "EcuM_Error.h"
#endif
#include "Os.h"
#include "CANoeApi.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

#if ( LIN_SW_MAJOR_VERSION != 7 )
# error "Source and Header are inconsistent (LIN_SW_MAJOR_VERSION)!"
#endif
#if ( LIN_SW_MINOR_VERSION != 2 )
# error "Source and Header are inconsistent (LIN_SW_MINOR_VERSION)!"
#endif
#if ( LIN_SW_PATCH_VERSION != 0 )
# error "Source and Header are inconsistent (LIN_SW_PATCH_VERSION)!"
#endif

#if ( LIN_TPS_MAJOR_VERSION != LIN_SW_MAJOR_VERSION )
# error "Source and Types Header are inconsistent (LIN_TPS_MAJOR_VERSION)!"
#endif
#if ( LIN_TPS_MINOR_VERSION != LIN_SW_MINOR_VERSION )
# error "Source and Types Header are inconsistent (LIN_TPS_MINOR_VERSION)!"
#endif
#if ( LIN_TPS_PATCH_VERSION != LIN_SW_PATCH_VERSION )
# error "Source and Types Header are inconsistent (LIN_TPS_PATCH_VERSION)!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 **********************************************************************************************************************/

#if   ( LIN_CONFIGURATION_VARIANT != LIN_CONFIGURATION_VARIANT_PRECOMPILE ) && \
      ( LIN_CONFIGURATION_VARIANT != LIN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
# error "LIN_CONFIGURATION_VARIANT not in valid range, check settings in generation tool!"
#endif

#if ( LIN_HL_GENERATORMSR_IMPLEMENTATION != LIN_HL_GENERATORMSR_COMPATIBILITY_VERSION )
# error "Version of the Hl generator is not consistent with the implementation!"
#endif

#if ( LIN_LL_GENERATORMSR_IMPLEMENTATION != LIN_LL_GENERATORMSR_COMPATIBILITY_VERSION )
# error "Version of the LL generator is not consistent with the implementation!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/* DET Report Handling */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define Lin_Det_ReportError( Lin_ApiId, Lin_ErrorCode) (Det_ReportError(LIN_MODULE_ID, 0, (Lin_ApiId), (Lin_ErrorCode)))

/* RTM Support */
#if ( LIN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  /* PRQA S 3453, 342 2 */ /* MD_MSR_19.7, MD_MSR_19.13 */
# define Lin_Rtm_Start(RtmMeasurementPoint_id)  Rtm_Start( RtmConf_RtmMeasurementPoint_##RtmMeasurementPoint_id )
# define Lin_Rtm_Stop(RtmMeasurementPoint_id)   Rtm_Stop( RtmConf_RtmMeasurementPoint_##RtmMeasurementPoint_id )
#else
# define Lin_Rtm_Start(RtmMeasurementPoint_id)
# define Lin_Rtm_Stop(RtmMeasurementPoint_id)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* Define if not already defined */
#if !defined (STATIC)                                                                                                   /* COV_LIN_COMPATIBILITY */
# define STATIC static
#endif

#if !defined (LIN_LOCAL_INLINE)                                                                                         /* COV_LIN_COMPATIBILITY */
# define LIN_LOCAL_INLINE LOCAL_INLINE
#endif

/* Simulated Hardware Interface based on Special Function Register */
typedef struct Lin_CANoeCntrlTag
{
  uint32  FrameType;        /* Identifies the Frame Type to sent on Lin_CANoeFrameRequest() */
  uint32  Identifier;       /* ID of Frame header */
  uint8   DLC;              /* DLC of Frame Response */
  uint8   Data[8];          /* Data of Frame Response */
  uint32  ErrorFlags;       /* Flags for error identification */
  uint32  InterruptFlags;   /* Interrupt Flags */
  uint8   EnableInterrupts; /* Enable Interrupt Flag */
}Lin_CANoeCntrlType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define LIN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* Create "Hardware Register" on RAM */
STATIC  VAR(Lin_CANoeCntrlType, LIN_VAR_NOINIT) Lin_CANoeCntrl[Lin_GetMaxHwChannels() + 1];

#define LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (LIN_USE_INIT_POINTER == STD_ON)

# define LIN_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Pointer to Global Configuration */
P2CONST(Lin_ConfigType, LIN_VAR_NOINIT, LIN_PBCFG) Lin_ConfigDataPtr;

# define LIN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

#if ( LIN_DEV_ERROR_DETECT == STD_ON )
# define LIN_DEV_UNINITVALUE                              ((uint8)0x00u)
# define LIN_DEV_INITVALUE                                ((uint8)0xA5u)

# define LIN_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Variable Lin_Dev_InitDetect must be initialized after reset for working debug check */
STATIC VAR(uint8, LIN_VAR_ZERO_INIT) Lin_Dev_InitDetect = LIN_DEV_UNINITVALUE;
# define LIN_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"                                                                                                    /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
/* none */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
 
#define LIN_START_SEC_CODE
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Lin_CheckIrqWakeupEvent
 **********************************************************************************************************************/
/*! \brief      Checks an wake up event.
 *  \details    If the channel has a EcuM Wakeup Source ID the wake up event are set and EcuM_CheckWakeup() are called.
 *  \param[in]  ChannelConfigIdx Channel index of configuration.
 *  \pre        -
 *  \context    ANY
 **********************************************************************************************************************/
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_CheckIrqWakeupEvent( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx );

/***********************************************************************************************************************
 *  Lin_SetSleep
 **********************************************************************************************************************/
/*! \brief      Set sleep.
 *  \details    Set the hardware and software state to sleep.
 *  \param[in]  ChannelConfigIdx  Channel index of configuration.
 *  \param[in]  Lin_ApiId         Lin Service ID, needed only for some hardware.
 *  \pre        -
 *  \context    ANY
 **********************************************************************************************************************/
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_SetSleep( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx, 
                                                    uint8 Lin_ApiId );

/***********************************************************************************************************************
 *  Lin_SetHardwareWake
 **********************************************************************************************************************/
/*! \brief      Set hardware to wake.
 *  \details    Set the hardware to wake without sending a wakeup pulse. Software state does not change.
 *  \param[in]  ChannelConfigIdx  Channel index of configuration.
 *  \param[in]  Lin_ApiId         Lin Service ID, needed only for some hardware.
 *  \pre        -
 *  \context    ANY
 **********************************************************************************************************************/
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_SetHardwareWake( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx,
                                                           uint8 Lin_ApiId );

/* --- Platform specific local function prototypes() ---------------------------------------------------------------- */
STATIC FUNC(void, LIN_CODE) Lin_CANoeMapChannel( uint8 ChannelConfigIdx );
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnReset( void );
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnMessageHandler( uint8 channel, uint8 dir, uint32 id, uint8 dlc, uint8 data[] );
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnWakeupHandler( uint8 channel, uint8 external );
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnSleepHandler( uint8 channel, uint8 external, uint8 isAwake, uint8 wasAwake, uint8 reason );
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnErrorHandler( uint8 channel, uint32 errorCode, uint32 subcode, uint32 id );
STATIC FUNC(void, LIN_CODE) Lin_CANoeInitCallout( void );
STATIC FUNC(void, LIN_CODE) Lin_CANoeFrameRequest( uint8 Channel, uint32 FrameType, uint8 Pid, uint8 Dlc, uint8 Data[] );
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetData( uint8 channel, uint8 data[] );
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetClearInterruptFlags( uint8 channel, uint32* flags );
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetClearErrorFlags( uint8 channel, uint32* flags );
/* ------------------------------------------------------------------------------------------------------------------ */

#define LIN_STOP_SEC_CODE
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */ 

/**********************************************************************************************************************
 *  FUNCTIONS
 **********************************************************************************************************************/
 
#define LIN_START_SEC_CODE_ISR
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Lin_Interrupt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LIN_CODE_ISR) Lin_Interrupt( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx )
{
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  VAR(uint32, AUTOMATIC) IntFlags;
  VAR(uint32, AUTOMATIC) ErrFlags;

  /* Runtime Measurement start */
  Lin_Rtm_Start(Lin_Interrupt)

  /* #10 Check ChannelConfigIdx parameter validity against configuration */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( ChannelConfigIdx >= Lin_GetMaxVariantChannels() )
  {
    ErrorId = LIN_E_INVALID_CHANNEL;   /* Check if channel is allowed */
  }
  else
#endif
  {
/* --- Platform specific implementation of Lin_Interrupt() ---------------------------------------------------------- */
    /* get and clear interrupt flags */
    Lin_CANoeGetClearInterruptFlags( ChannelConfigIdx, &IntFlags );
    Lin_CANoeGetClearErrorFlags( ChannelConfigIdx, &ErrFlags );

    /* handling of current state */
    switch( Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) )
    {
      case LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA:
        Lin_CheckIrqWakeupEvent( ChannelConfigIdx );                                                                    /* SBSW_DRVLIN_FCT_INTERNAL */
        break;

      case LIN_ONSLEEP_PENDING_INTERNALSTATEOFCHANNELDATA:
        Lin_SetSleep( ChannelConfigIdx, LIN_SID_INTERRUPT_ID );                                                             /* SBSW_DRVLIN_FCT_INTERNAL */
        break;

      case LIN_ONWAKEUP_PENDING_INTERNALSTATEOFCHANNELDATA:
        Lin_SetHardwareWake( ChannelConfigIdx, LIN_SID_INTERRUPT_ID );                                                      /* SBSW_DRVLIN_FCT_INTERNAL */
        Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONWAKE_INTERNALSTATEOFCHANNELDATA );                   /* SBSW_DRVLIN_COMSTACKLIB */
        break;

      case LIN_ONTX_BUSY_INTERNALSTATEOFCHANNELDATA:
        if ( (IntFlags & LIN_CANOE_INTERRUPT_ERROR) == 0 )
        { /* TX message successful transmitted */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_OK_INTERNALSTATEOFCHANNELDATA );                /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else if ( (ErrFlags & (LIN_CANOE_ERROR_PID | LIN_CANOE_ERROR_HEADER)) == 0 )
        { /* No successful TX message but header was successful transmitted */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_ERROR_INTERNALSTATEOFCHANNELDATA );             /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else
        { /* No successful TX message and no header was successful transmitted */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_HEADER_ERROR_INTERNALSTATEOFCHANNELDATA );      /* SBSW_DRVLIN_COMSTACKLIB */
        }
        break;

      case LIN_ONRX_BUSY_INTERNALSTATEOFCHANNELDATA:
        if ( (IntFlags & LIN_CANOE_INTERRUPT_ERROR) == 0 )
        { /* Response successfully received */
          /* store RX Data */
          Lin_CANoeGetData( ChannelConfigIdx, Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );

          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONRX_OK_INTERNALSTATEOFCHANNELDATA );                /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else if ( (ErrFlags & (LIN_CANOE_ERROR_PID | LIN_CANOE_ERROR_HEADER)) != 0 )
        { /* no header was successful transmitted */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_HEADER_ERROR_INTERNALSTATEOFCHANNELDATA );      /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else if ( (ErrFlags & LIN_CANOE_ERROR_RESPONSE_TIMEOUT) != 0 )
        { /* no response byte received */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONRX_NO_RESPONSE_INTERNALSTATEOFCHANNELDATA );       /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else
        { /* response partly received */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONRX_ERROR_INTERNALSTATEOFCHANNELDATA );             /* SBSW_DRVLIN_COMSTACKLIB */
        }
        break;

      case LIN_ONS2S_BUSY_INTERNALSTATEOFCHANNELDATA:
        if ( (IntFlags & LIN_CANOE_INTERRUPT_ERROR) == 0 )
        { /* header was successful transmitted */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_OK_INTERNALSTATEOFCHANNELDATA );                /* SBSW_DRVLIN_COMSTACKLIB */
        }
        else
        { /* any error on slave to slave is an header error */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_HEADER_ERROR_INTERNALSTATEOFCHANNELDATA );      /* SBSW_DRVLIN_COMSTACKLIB */
        }
        break;

      default:
        /* interrupt not needed in other states than above --> ignore */
        break;
    }
/* ------------------------------------------------------------------------------------------------------------------ */
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_INTERRUPT_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  /* Runtime Measurement stop */
  Lin_Rtm_Stop(Lin_Interrupt)
}                                                                                                                       /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#define LIN_STOP_SEC_CODE_ISR
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define LIN_START_SEC_CODE
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Lin_SendFrame
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
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_SendFrame( uint8 Channel,
                                              P2VAR(Lin_PduType, AUTOMATIC, LIN_APPL_VAR) PduInfoPtr )                  /* PRQA S 3673 */ /* MD_LIN_HL_16.7 */
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) Lin_BufferCounterTmp;

  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check PduInfoPtr parameter for NULL pointer */
  else if ( (void*)PduInfoPtr == NULL_PTR )
  {
    ErrorId = LIN_E_PARAM_POINTER;
  }
  /* #30 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    /* Get channel RAM/ROM array index of Channel ID */ 
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );
    
    /* #35 Check if data length parameter is in range (1-8 data bytes) */
    if( (PduInfoPtr->Dl == 0) || (PduInfoPtr->Dl > 8)  )
    {      
      ErrorId = LIN_E_PARAM_VALUE;
    } 
    /* #40 Check if channel state not on sleep */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
    else if( (Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) & 0x0F) == LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA )
    {
      ErrorId = LIN_E_STATE_TRANSITION;
    }
#endif
    else
    {
      RetTmp = E_OK;
/* --- Platform specific implementation of Lin_SendFrame() ---------------------------------------------------------- */
      /* setup RX or TX message */
      switch( PduInfoPtr->Drc )
      {
        case (Lin_FrameResponseType)LIN_MASTER_RESPONSE:
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONTX_BUSY_INTERNALSTATEOFCHANNELDATA );              /* SBSW_DRVLIN_COMSTACKLIB */

          /* Copy data to local tx buffer */
          for (Lin_BufferCounterTmp = 0; Lin_BufferCounterTmp < PduInfoPtr->Dl; Lin_BufferCounterTmp++)
          {
            Lin_GetDataBufferOfChannelData( ChannelConfigIdx )[Lin_BufferCounterTmp] = PduInfoPtr->SduPtr[Lin_BufferCounterTmp];
          }

          /* start sending */
          Lin_CANoeFrameRequest( ChannelConfigIdx,
                                 LIN_CANOE_FRAME_TX,
                                 PduInfoPtr->Pid,
                                 PduInfoPtr->Dl,
                                 Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );
          break;

        case (Lin_FrameResponseType)LIN_SLAVE_RESPONSE:
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONRX_BUSY_INTERNALSTATEOFCHANNELDATA );              /* SBSW_DRVLIN_COMSTACKLIB */
          
          /* start sending */
          Lin_CANoeFrameRequest( ChannelConfigIdx,
                                 LIN_CANOE_FRAME_RX,
                                 PduInfoPtr->Pid,
                                 PduInfoPtr->Dl,
                                 Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );
          break;

        default: /* slave-to-slave frame */
          Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONS2S_BUSY_INTERNALSTATEOFCHANNELDATA );

          /* start sending */
          Lin_CANoeFrameRequest( ChannelConfigIdx,
                                 LIN_CANOE_FRAME_RX,
                                 PduInfoPtr->Pid,
                                 PduInfoPtr->Dl,
                                 Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );
      }
/* ------------------------------------------------------------------------------------------------------------------ */
    }
  }
#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_SENDFRAME_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
}

/***********************************************************************************************************************
 *  Lin_GetStatus
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
 */
FUNC(Lin_StatusType, LIN_CODE) Lin_GetStatus( uint8 Channel,
                                              P2VAR(Lin_u8PtrType, AUTOMATIC, LIN_APPL_VAR) Lin_SduPtr )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Lin_StatusType, AUTOMATIC) RetTmp = LIN_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Lin_SduPtr parameter for NULL pointer */
  else if ( (void*)Lin_SduPtr == NULL_PTR )
  {
    ErrorId = LIN_E_PARAM_POINTER;
  }
  /* #30 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );
/* --- Platform specific implementation of Lin_GetStatus() ---------------------------------------------------------- */
    /* #40 Handle hardware specific behaviour */
    switch( Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) )
    {
      case LIN_ONSLEEP_PENDING_INTERNALSTATEOFCHANNELDATA:
        Lin_SetSleep( ChannelConfigIdx, LIN_SID_GETSTATUS_ID );                                                             /* SBSW_DRVLIN_FCT_INTERNAL */
        break;

      case LIN_ONWAKEUP_PENDING_INTERNALSTATEOFCHANNELDATA:
        Lin_SetHardwareWake( ChannelConfigIdx, LIN_SID_GETSTATUS_ID );                                                      /* SBSW_DRVLIN_FCT_INTERNAL */
        Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONWAKE_INTERNALSTATEOFCHANNELDATA );                   /* SBSW_DRVLIN_COMSTACKLIB */
        break;

      case LIN_ONRX_OK_INTERNALSTATEOFCHANNELDATA:
        *Lin_SduPtr = Lin_GetDataBufferOfChannelData( ChannelConfigIdx );                                               /* SBSW_DRVLIN_PTR_NULL */
        break;

      default:
        /* current state valid or nothing to do */
        break;
    }

    RetTmp = (Lin_StatusType)Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) & 0x0F; /* mask non ASR sub-states */
/* ------------------------------------------------------------------------------------------------------------------ */
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_GETSTATUS_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
}

/***********************************************************************************************************************
 *  Lin_CheckWakeup
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_CheckWakeup( uint8 Channel )
{
#if( LIN_LEAST_ONE_CHANNEL_WAKEUP_SUPPORT_ENABLED == STD_ON )

  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    RetTmp = E_OK;
    
    /* disable callout's to prevent link time errors */
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );

    /* #30 Check if a wakeup event occurred */
    if ( Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) == LIN_ONWAKEUP_EXTERNAL_INTERNALSTATEOFCHANNELDATA )
    {
      /* #40 Set channel state to operational (according DSGN-Lin22334) */
      Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA );                    /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */

      /* #50 Inform EcuM and LinIf about wakeup */
      EcuM_SetWakeupEvent( Lin_GetEcuMWakeupSourceOfChannelConfig( ChannelConfigIdx ) );
      LinIf_WakeupConfirmation( Lin_GetEcuMWakeupSourceOfChannelConfig( ChannelConfigIdx ) );
    }
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_CHECKWAKEUP_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
#else
  return E_OK;
#endif
}

/***********************************************************************************************************************
 *  Lin_Wakeup
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_Wakeup( uint8 Channel )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;

  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );

    /* #30 Check if channel state on sleep */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
    if( (Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) & 0x0F) != LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA )
    {
      ErrorId = LIN_E_STATE_TRANSITION;
    }
    else
#endif
    {
      RetTmp = E_OK;

      /* #40 Set channel state to wakeup pending (according DSGN-Lin22334) */
      Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONWAKEUP_PENDING_INTERNALSTATEOFCHANNELDATA );           /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
/* --- Platform specific implementation of Lin_Wakeup() ------------------------------------------------------------- */
      /* #50 Set hardware to operational and transmit a wakeup frame (including configure intern interrupt flags) */
      /* Send wake up frame */
      Lin_CANoeFrameRequest( ChannelConfigIdx,
                             LIN_CANOE_FRAME_WAKEUP,
                             0,
                             0,
                             Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );

/* ------------------------------------------------------------------------------------------------------------------ */
    }
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_WAKEUP_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
}

/***********************************************************************************************************************
 *  Lin_GoToSleep
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_GoToSleep( uint8 Channel )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  VAR(Lin_PduType, AUTOMATIC) PduTmp;
  VAR(uint8, AUTOMATIC) DataTmp[8];
  
  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );
    
    /* #30 Set-up and transmit a go-to-sleep frame */     
    PduTmp.Cs        = LIN_CLASSIC_CS;
    PduTmp.Dl        = 8;
    PduTmp.Drc       = LIN_MASTER_RESPONSE;
    PduTmp.Pid       = 0x3Cu;
    PduTmp.SduPtr    = DataTmp;
    PduTmp.SduPtr[0] = 0x00u;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[1] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[2] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[3] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[4] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[5] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[6] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */
    PduTmp.SduPtr[7] = 0xFFu;                                                                                           /* SBSW_LIN_BUFFER_WRITE */

    RetTmp = Lin_SendFrame( Channel, &PduTmp );                                                                     /* SBSW_LIN_PTR_CALL */
    
    /* #40 Set channel state to sleep pending (according DSGN-Lin22334) */
    Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONSLEEP_PENDING_INTERNALSTATEOFCHANNELDATA );              /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_GOTOSLEEP_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
}

/***********************************************************************************************************************
 *  Lin_GoToSleepInternal
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_GoToSleepInternal( uint8 Channel )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;

  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    RetTmp = E_OK;
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );
    /* #30 Set hardware and software state to sleep */
    Lin_SetSleep( ChannelConfigIdx, LIN_SID_GOTOSLEEPINTERNAL_ID );
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_GOTOSLEEPINTERNAL_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  return(RetTmp);
}

/***********************************************************************************************************************
 *  Lin_WakeupInternal
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LIN_CODE) Lin_WakeupInternal( uint8 Channel )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(Std_ReturnType, AUTOMATIC) RetTmp = E_NOT_OK;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  /* #10 Check if component is initialized */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  if ( Lin_Dev_InitDetect != LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_UNINIT;
  }
  /* #20 Check Channel parameter validity against configuration */
  else if ( Lin_GetSizeOfChannelId() <= Channel )
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# if( LIN_INVALIDHNDOFCHANNELID == STD_ON )
  else if ( Lin_IsInvalidHndOfChannelId( Channel ) )                                                                    /* PRQA S 3325 */ /* MD_MSR_14.1 */ 
  {                                                                                                                     /* PRQA S 3201 */ /* MD_MSR_14.1 */
    ErrorId = LIN_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  {
    ChannelConfigIdx = Lin_GetChannelConfigIdxOfChannelId( Channel );
    /* #30 Check if channel state on sleep */
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
    if( (Lin_GetInternalStateOfChannelData( ChannelConfigIdx ) & 0x0F) != LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA )
    {
      ErrorId = LIN_E_STATE_TRANSITION;
    }
    else
#endif
    {
      RetTmp = E_OK;
      /* #40 Set channel state to operational (according DSGN-Lin22334) */
      Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONWAKE_INTERNALSTATEOFCHANNELDATA );                     /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
      /* #50 Set hardware to operational mode (without transmit a wakeup frame) */
      Lin_SetHardwareWake( ChannelConfigIdx, LIN_SID_WAKEUPINTERNAL_ID );
    }
  }
#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_WAKEUPINTERNAL_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  LIN_DUMMY_STATEMENT(Channel);

  return(RetTmp);
}

#if ( LIN_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  Lin_GetVersionInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LIN_CODE) Lin_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, LIN_APPL_VAR) versioninfo )
{
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;
  
# if ( LIN_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check versioninfo for NULL pointer */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LIN_APPL_VAR))NULL_PTR )
  {
    ErrorId = LIN_E_PARAM_POINTER; 
  }
  else
# endif
  {
    /* #20 Set versioninfo with corresponding macros from component header */
    versioninfo->vendorID           = LIN_VENDOR_ID;                                                                    /* SBSW_LIN_PTR */
    versioninfo->moduleID           = LIN_MODULE_ID;                                                                    /* SBSW_LIN_PTR */
    versioninfo->sw_major_version   = LIN_SW_MAJOR_VERSION;                                                             /* SBSW_LIN_PTR */
    versioninfo->sw_minor_version   = LIN_SW_MINOR_VERSION;                                                             /* SBSW_LIN_PTR */
    versioninfo->sw_patch_version   = LIN_SW_PATCH_VERSION;                                                             /* SBSW_LIN_PTR */
  }
  
#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_GETVERSIONINFO_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif
}
#endif /* LIN_VERSION_INFO_API == STD_ON */

/***********************************************************************************************************************
 *  Lin_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LIN_CODE) Lin_InitMemory( void )
{
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  /* #10 Set state to uninitialized (according DSGN-Lin22334) */
  Lin_Dev_InitDetect = LIN_DEV_UNINITVALUE;
#endif
}

/***********************************************************************************************************************
 *  Lin_Init
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
 */
FUNC(void, LIN_CODE) Lin_Init( P2CONST(Lin_ConfigType, AUTOMATIC, LIN_PBCFG) Config )
{
  VAR(Lin_ChannelConfigIdxOfChannelIdType, AUTOMATIC) ChannelConfigIdx;
  VAR(uint8, AUTOMATIC) ErrorId = LIN_E_NO_ERROR;

 /* Runtime Measurement start */
  Lin_Rtm_Start(Lin_Init)

#if ( LIN_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is uninitialized */
  if ( Lin_Dev_InitDetect == LIN_DEV_INITVALUE )
  {
    ErrorId = LIN_E_STATE_TRANSITION;
  }
  else
#endif
  {

#if (LIN_USE_INIT_POINTER == STD_ON)
    /* #20 Check Config parameter for NULL pointer */
    if ( Config == NULL_PTR )
    {
# if ( LIN_DEV_ERROR_DETECT == STD_ON )
      ErrorId = LIN_E_INVALID_POINTER;
# endif

# if( LIN_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
      EcuM_BswErrorHook((uint16) LIN_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    }
    else
#endif
    {

#if (LIN_USE_INIT_POINTER == STD_ON)      
      Lin_ConfigDataPtr = Config;
#else
      LIN_DUMMY_STATEMENT( Config );
#endif

#if( LIN_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
      /* #30 Check Generator compatibility version */
      if (Lin_GetHLGeneratorCompatibilityVersion() != (uint16) LIN_HL_GENERATORMSR_COMPATIBILITY_VERSION)
      {
        EcuM_BswErrorHook((uint16) LIN_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION);
      }
      else if (Lin_GetLLGeneratorCompatibilityVersion() != (uint16) LIN_LL_GENERATORMSR_COMPATIBILITY_VERSION)
      {
        EcuM_BswErrorHook((uint16) LIN_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION);
      }
      /* #40 Check version and magic number */
      else if (Lin_GetFinalMagicNumber() != LIN_FINAL_MAGIC_NUMBER)
      {
        EcuM_BswErrorHook((uint16) LIN_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
      }
      else
#endif
      {
#if ( LIN_DEV_ERROR_DETECT == STD_ON )
        /* #50 Set state to initialized (according DSGN-Lin22334) */
        Lin_Dev_InitDetect = LIN_DEV_INITVALUE;
#endif
        
/* --- Platform specific implementation of Lin_Init() --------------------------------------------------------------- */
        /* #60 Set channel independent hardware configuration (if applicable) */
        Lin_CANoeOnReset();
        Lin_CANoeInitCallout();
/* ------------------------------------------------------------------------------------------------------------------ */
        
        /* #70 Loop over all configured hardware channels */
        for (ChannelConfigIdx = 0x00u; ChannelConfigIdx < Lin_GetNrOfChannels(); ChannelConfigIdx++)
        {
          /* #80 Set channel state to operational (according DSGN-Lin22334) */
          Lin_SetInternalStateOfChannelData(  ChannelConfigIdx, LIN_ONWAKE_INTERNALSTATEOFCHANNELDATA );                /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
/* --- Platform specific implementation of Lin_Init() - Channel loop - ---------------------------------------------- */
          /* #90 Set channel dependent hardware configuration (baud rate, break field length, ...) */
          Lin_CANoeMapChannel( ChannelConfigIdx );
/* ------------------------------------------------------------------------------------------------------------------ */
        }
      }
    }
  }

#if ( LIN_DEV_ERROR_REPORT == STD_ON )
  if( ErrorId != LIN_E_NO_ERROR )
  {
    Lin_Det_ReportError( LIN_SID_INIT_ID, ErrorId );
  }
#else
  LIN_DUMMY_STATEMENT(ErrorId);
#endif

  /* Runtime Measurement stop */
  Lin_Rtm_Stop(Lin_Init)
}                                                                                                                       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Lin_CheckIrqWakeupEvent
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_CheckIrqWakeupEvent( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx )
{
  /* #10 If channel has a EcuM Wakeup Source ID */
#if( LIN_LEAST_ONE_CHANNEL_WAKEUP_SUPPORT_ENABLED == STD_ON )
# if( LIN_LEAST_ONE_CHANNEL_WAKEUP_SUPPORT_DISABLED == STD_ON )                                                         /* COV_LIN_DERIVATIVE */
  if ( Lin_GetEcuMWakeupSourceOfChannelConfig( ChannelConfigIdx ) != 0 )   
# endif
  {
    /* #20 Set channel state to external wakeup pending (according DSGN-Lin22334) */
    Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONWAKEUP_EXTERNAL_INTERNALSTATEOFCHANNELDATA );            /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
    /* #30 Call EcuM_CheckWakeup() to inform EcuM about wakeup event */
    EcuM_CheckWakeup( Lin_GetEcuMWakeupSourceOfChannelConfig( ChannelConfigIdx ) );
  }
#endif
}

/***********************************************************************************************************************
 *  Lin_SetSleep
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_SetSleep ( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx,
                                                     uint8 Lin_ApiId /* needed only for some hardware */ )
{
/* --- Platform specific implementation of Lin_SetSleep() ----------------------------------------------------------- */
  /* #10 Set hardware to sleep and enable hardware detection of wakeup pulse (over interrupt) */
  /* Wait for Wakeup Frame, abort any communication */
  Lin_CANoeFrameRequest( ChannelConfigIdx,
                         LIN_CANOE_FRAME_SILENT_SLEEP,
                         0,
                         0,
                         Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );
/* ------------------------------------------------------------------------------------------------------------------ */

  /* #20 Set channel state to sleep (according DSGN-Lin22334) */
  Lin_SetInternalStateOfChannelData( ChannelConfigIdx, LIN_ONSLEEP_INTERNALSTATEOFCHANNELDATA );                        /* SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID */
}

/***********************************************************************************************************************
 *  Lin_SetHardwareWake
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
LIN_LOCAL_INLINE FUNC(void, LIN_CODE) Lin_SetHardwareWake( Lin_ChannelConfigIdxOfChannelHwType ChannelConfigIdx,
                                                           uint8 Lin_ApiId /* needed only for some hardware */ )
{
/* --- Platform specific implementation of Lin_SetHardwareWake() ---------------------------------------------------- */
  /* #10 Set hardware to operational mode (disable detection of wakeup pulse) */
  Lin_CANoeFrameRequest( ChannelConfigIdx,
                         LIN_CANOE_FRAME_WAKEUP_INTERNAL,
                         0,
                         0,
                         Lin_GetDataBufferOfChannelData( ChannelConfigIdx ) );
/* ------------------------------------------------------------------------------------------------------------------ */
}

/* --- Platform specific implementation of internal functions ------------------------------------------------------- */

/**********************************************************************************************************************
 * Lin_CANoeMapChannel
 **********************************************************************************************************************/
/*! \brief      Mapping the channel index to the CANoe channel by Channel Name.
 *  \param[in]  ChannelConfigIdx   The index of channel.
 *  \return     -
 *  \pre        Interrupts must be disabled
 *  \context    System Startup (only called if no VTT controller --> Canoeemu)
 *  \note       Specification of module initialisation
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeMapChannel
(
  VAR(uint8, AUTOMATIC) ChannelConfigIdx
)
{
  /* PRQA S 5013, 0310, 0311 1 */ /* MD_LIN_CANoeAPI, MD_LIN_CANoeAPI, MD_LIN_CANoeAPI */
  CANoeAPI_WriteSysState( (char*)&(Lin_GetBusName( Lin_GetBusNameIdxOfChannelIdx(ChannelConfigIdx) )) );

  /* PRQA S 5013, 0310, 0311 1 */ /* MD_LIN_CANoeAPI, MD_LIN_CANoeAPI, MD_LIN_CANoeAPI */
  if( CANoeAPI_MapChannel( (char*)&(Lin_GetBusName(Lin_GetBusNameIdxOfChannelIdx(ChannelConfigIdx))),
                           CANOEAPI_BUSTYPE_LIN,
                           ChannelConfigIdx ) == -1 )
  {
    CANoeAPI_WriteSysState("AUTOSAR LIN Driver: Error in mapping LIN channel, check channel name!");
  }
  else
  {
    CANoeAPI_WriteSysState("AUTOSAR LIN Driver: LIN channel mapping successful!");
  }
}

/**********************************************************************************************************************
 * Lin_CANoeOnReset
 **********************************************************************************************************************/
/*! \brief      Resetting the simulated Hardware Register.
 *  \pre        Interrupts must be disabled
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnReset
(
  void
)
{
  VAR(uint8, AUTOMATIC) chnl;
  VAR(uint8, AUTOMATIC) i;

  for(chnl = 0; chnl < Lin_GetMaxHwChannels(); chnl++)
  {
    Lin_CANoeCntrl[chnl].DLC = 0;
    Lin_CANoeCntrl[chnl].ErrorFlags = 0;
    Lin_CANoeCntrl[chnl].Identifier = 0;
    Lin_CANoeCntrl[chnl].InterruptFlags = 0;
    for(i = 0; i < 8; i++)
    {
      Lin_CANoeCntrl[chnl].Data[i] = 0;
    }
  }
  ;
}

/**********************************************************************************************************************
 * Lin_CANoeOnMessageHandler
 **********************************************************************************************************************/
/*! \brief      Callback from CANoe. Assumption this is called if a Frame occurs on LIN Bus.
 *  \param[in]  Channel Undocumented parameter
 *  \param[in]  dir     Undocumented parameter
 *  \param[in]  id      Undocumented parameter
 *  \param[in]  dlc     Undocumented parameter
 *  \param[in]  data    Undocumented parameter
 *  \return     -
 *  \pre        -
 *  \context    -
 *  \note       ToDo: Parameter meaning
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnMessageHandler
(
  VAR(uint8,  AUTOMATIC) channel,
  VAR(uint8,  AUTOMATIC) dir,
  VAR(uint32, AUTOMATIC) id,
  VAR(uint8,  AUTOMATIC) dlc,
  VAR(uint8,  AUTOMATIC) data[] /* PRQA S 3673 */ /* MD_LIN_HL_16.7 */
)
{
  VAR(uint8, AUTOMATIC) i;
 
  Lin_CANoeCntrl[channel].Identifier = id;
  Lin_CANoeCntrl[channel].DLC = dlc;

  for( i = 0; i < dlc; i++ )
  {
    Lin_CANoeCntrl[channel].Data[i] = data[i];
  }

  Lin_CANoeCntrl[channel].InterruptFlags = LIN_CANOE_INTERRUPT_RX;

  Lin_Interrupt( channel );
}

/**********************************************************************************************************************
 * Lin_CANoeOnWakeupHandler
 **********************************************************************************************************************/
/*! \brief      Callback from CANoe. Assumption this is called if a Wake Up Frame occurs on LIN Bus.
 *  \param[in]  Channel  Undocumented parameter
 *  \param[in]  External Undocumented parameter
 *  \return     -
 *  \pre        -
 *  \context    -
 *  \note       ToDo: Parameter meaning
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnWakeupHandler( uint8 channel, uint8 external )
{

  LIN_DUMMY_STATEMENT(external);

  Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_WAKE;
  Lin_Interrupt( channel );
  
}

/**********************************************************************************************************************
 * Lin_CANoeOnSleepHandler
 **********************************************************************************************************************/
/*! \brief      This is a Callback handler and is called when Sleep Request Frame was sent on the bus.
 *  \param[in]  Channel   Channel Index.
 *  \param[in]  External  Undocumented parameter
 *  \param[in]  IsAwake   Undocumented parameter
 *  \param[in]  WasAwake  Undocumented parameter
 *  \param[in]  Reason    Undocumented parameter
 *  \return     -
 *  \pre        -
 *  \context    -
 *  \note       ToDo: Parameter meaning
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnSleepHandler
(
  VAR(uint8, AUTOMATIC) channel,
  VAR(uint8, AUTOMATIC) external,
  VAR(uint8, AUTOMATIC) isAwake,
  VAR(uint8, AUTOMATIC) wasAwake,
  VAR(uint8, AUTOMATIC) reason
)
{

  LIN_DUMMY_STATEMENT(channel);
  LIN_DUMMY_STATEMENT(external);
  LIN_DUMMY_STATEMENT(isAwake);
  LIN_DUMMY_STATEMENT(wasAwake);
  LIN_DUMMY_STATEMENT(reason);

/* not needed, because the go-to-sleep-command is indicated by the VLinMsgHandler or VLinErrorHandle */

}

/**********************************************************************************************************************
 * Lin_CANoeOnErrorHandler
 **********************************************************************************************************************/
/*! \brief      Event handler for error indication on the LIN bus. The handler is called in several situations.
 *
 * Transmission error on the LIN bus:
 *   The transmission error indication is called when no Slave responded to a transmission request.
 *   (see also 'VIAOnLinTransmissionError::OnTransmError' at VIA interface and CAPL event procedure 'on linTransmError')
 *
 * Checksum error on the LIN bus:
 *   The checksum error indication is called when a frame was transmitted without a failure, but with an incorrect
 *   checksum.
 *   (see also 'VIAOnLinCheckSumError::OnCsError' at VIA interface and CAPL event procedure 'on linCsError')
 *
 * Receive error on the LIN bus:
 *     The receive error indication is called when an error occurred during a frame transmission.
 *     (see also 'VIAOnLinReceiveError::OnReceiveError' at VIA interface and CAPL event procedure 'on linReceiveError')
 *
 * Sync error Frame on the LIN bus
 *     The sync error indication is called when the LIN hardware was unable to become synchronized to an external master.
 *     (see also 'VIAOnLinSyncError::OnSyncError' at VIA interface and CAPL event procedure 'on linSyncError')
 *
 *  \param[in]  Channel    The Channel Index.
 *
 *  \param[in]  ErrorCode  CANOEAPI_LINTRANSMITERROR (1) on Transmission error
 *                         CANOEAPI_LINCHECKSUMERROR (2) on Checksum error
 *                         CANOEAPI_LINRECEIVEERROR  (3) on Receive error
 *                         CANOEAPI_LINSYNCHERROR    (4) on Sync error
 *
 *  \param[in]  Subcode    Depends on ErrorCode. If ErrorCode
 *                              - CANOEAPI_LINTRANSMITERROR or
 *                                CANOEAPI_LINSYNCHERROR:    unused, value is always zero
 *                              - CANOEAPI_LINCHECKSUMERROR: CANoeAPI_DIR_TX (1): Frame was transmitted by LIN hardware
 *                                                           CANoeAPI_DIR_RX (0): Frame was received
 *                              - CANOEAPI_LINRECEIVEERROR:  State and reason of the error. The bits 0-3 indicate the
 *                                                           LIN hardware state at the time the error has occurred,
 *                                                           while the bits 4-7 indicate the reason of the error. (see
 *                                                           also CAPL selector lin_StateReason at linReceiveError).
 *                                                           Value for the states:
 *                                                            0: Bus idle
 *                                                            1: Waiting for SynchBreak
 *                                                            2: Waiting for SynchField
 *                                                            3: Waiting for frame ID
 *                                                            4-12: Waiting for data byte or checksum byte depending
 *                                                                  on the frame DLC (4: DLC 0,  12: DLC 8 - the
 *                                                                  checksum byte)
 *                                                            14: Consecutive event (i.e. event resulting from further
 *                                                                data interpretation, after already notified error for
 *                                                                first offending byte).
 *                                                            15: SleepMode. Only possible if the LIN hardware is in
 *                                                                Master mode. In Slave mode, the LIN hardware can be
 *                                                                wakened by any type of bus traffic.
 *                                                           Values for the reason:
 *                                                            0: Timeout
 *                                                            1: Received an unexpected byte violating protocol.
 *                                                            2: Received a byte with framing error (with dominant stop bit).
 *                                                            3: Unexpected Break field
 *                                                            4: Unidentified error
 *
 *  \param[in]  Id         LIN Frame identifier,  Value range: 0..63, 0xffffffff on Sync error
 *
 *  \return     -
 *  \pre        -
 *  \context    -
 *  \note       -
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeOnErrorHandler
(
 VAR(uint8, AUTOMATIC)  channel,
 VAR(uint32, AUTOMATIC) errorCode,
 VAR(uint32, AUTOMATIC) subcode,
 VAR(uint32, AUTOMATIC) id
)
{

  if( (Lin_CANoeCntrl[channel].InterruptFlags & LIN_CANOE_INTERRUPT_ERROR) != 0 )
  {
    Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_ERROR_OVERRUN; /* Received a Frame befor the last was processed (RX Interrupt Flag) */
    Lin_Interrupt( channel );
  }
  else
  {
    /* API function reverse engineering pattern */
    /* pattern occured on framing error: */
    if( (errorCode == 0x03) && (subcode >= 0x34) && (subcode <= 0x3b) && ( id <= 64 ) )
    {
      Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_ERROR;
      Lin_CANoeCntrl[channel].ErrorFlags     |= LIN_CANOE_ERROR_FRAMING;
      Lin_Interrupt( channel );
    }
    else if( ((errorCode == 0x03) && (subcode == 0x20) && ( id <= 64 )) ||
             ((errorCode == 0x03) && (subcode == 0x4d) && ( id <= 64 )) ||
             ((errorCode == 0x03) && (subcode == 0x10) && ( id <= 64 )) ||
             ((errorCode == 0x04) && (subcode == 0x00) && ( id == 0xFFFFFFFFu )) || /* CANOEAPI_LINSYNCHERROR  */
             ((errorCode == 0x03) && (subcode == 0x13) && ( id <= 64 )) ||
             ((errorCode == 0x03) && (subcode == 0x23) && ( id <= 64 )) )
    {
      Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_ERROR;
      Lin_CANoeCntrl[channel].ErrorFlags     |= LIN_CANOE_ERROR_HEADER; /*Header error, i.e. Sync-Field error, violation duration limit*/
      Lin_Interrupt( channel );
    }
    else if( ((errorCode == 0x03) && (subcode >= 0x05) && (subcode <= 0x0c) && ( id <= 64 )) || /* bit error */
             ((errorCode == 0x03) && (subcode >= 0x05) && (subcode <= 0x0c) && ( id <= 64 )) || /* incomplete response */
             ((errorCode == 0x03) && (subcode >= 0x05) && (subcode <= 0x0c) && ( id <= 64 )) || /* incomplete response */
             ((errorCode == 0x03) && (subcode >= 0x25) && (subcode <= 0x2b) && ( id <= 64 )) ||
             ((errorCode == 0x03) && (subcode >= 0x24) && (subcode <= 0x2a) && ( id <= 64 )) )  /* ?? */
    {
      Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_ERROR;
      Lin_CANoeCntrl[channel].ErrorFlags     |= LIN_CANOE_ERROR_RESPONSE_INCOMPLETE;
      Lin_Interrupt( channel );
    }
    else if( ((errorCode == 0x02) && (subcode == 0x00) && ( id <= 64 )) ||  /* checksum error */
             ((errorCode == 0x02) && (subcode == 0x01) && ( id <= 64 )) )   /* checksum error on go-to-sleep-command */
    {
      Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_ERROR;
      Lin_CANoeCntrl[channel].ErrorFlags     |= LIN_CANOE_ERROR_CHECKSUM;
      Lin_Interrupt( channel );
    }
    else if( (errorCode == 0x01) && (subcode == 0x00) && ( id <= 64 ) )  /* no response */
    {
      Lin_CANoeCntrl[channel].InterruptFlags |= LIN_CANOE_INTERRUPT_ERROR;
      Lin_CANoeCntrl[channel].ErrorFlags     |= LIN_CANOE_ERROR_RESPONSE_TIMEOUT;
      Lin_Interrupt( channel );
    }
    else
    {
      /* error pattern not needed */
    }
  }
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Lin_CANoeInitCallout
 **********************************************************************************************************************/
/*! \brief      Setting the CANoe Callback Handler.
 *  \pre        -
 *  \context    System Startup
 *  \note       -
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeInitCallout
(
  void
)
{
  CANoeAPI_SetLinHandler( Lin_CANoeOnMessageHandler,
                          Lin_CANoeOnWakeupHandler,
                          Lin_CANoeOnSleepHandler,
                          Lin_CANoeOnErrorHandler );
}

/**********************************************************************************************************************
 * Lin_CANoeFrameRequest
 **********************************************************************************************************************/
/*! \brief      Send a Frame on bus. The behaviour is like a hardware register.
 *  \param[in]  Channel
 *  \param[in]  FrameType  Identifies the Frame Type to sent on PreFix_FrameRequest()
 *  \param[in]  Pid        PID of Frame header ('id' of CANoeAPI_SendLinMessage()
 *  \param[in]  Dlc        DLC of Frame Response ('dlc' of CANoeAPI_SendLinMessage and 'bDLC' of ApplCANoe_LinMessageIndication)
 *  \param[in]  Data       Data of Frame Response ('data' of CANoeAPI_SendLinMessage and 'pbData' of ApplCANoe_LinMessageIndication)
 *  \return     -
 *  \pre
 *  \context    This function is called by the LIN Driver
 *  \note       If Frames could not send on bus (i.e. Dominant disturbance), all frames are queued until the bus is
 *              working. Normal hardware does not work on this way.
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeFrameRequest
(
  VAR(uint8,  AUTOMATIC) Channel,
  VAR(uint32, AUTOMATIC) FrameType,
  VAR(uint8,  AUTOMATIC) Pid,
  VAR(uint8,  AUTOMATIC) Dlc,
  VAR(uint8,  AUTOMATIC) Data[]
)
{
  
  switch( FrameType )
  {
  case LIN_CANOE_FRAME_TX:
    if( (Pid     == 0x3Cu) && 
        (Dlc     == 8    ) &&
        (Data[0] == 0x00u) &&
        (Data[1] == 0xFFu) &&
        (Data[2] == 0xFFu) &&
        (Data[3] == 0xFFu) &&
        (Data[4] == 0xFFu) &&
        (Data[5] == 0xFFu) &&
        (Data[6] == 0xFFu) &&
        (Data[7] == 0xFFu) )
    { /* go to sleep cmd */
      CANoeAPI_SendLinSleepModeFrame( Channel, 0x00, 0x00, 0xFF); 
    }
    else
    {      
      CANoeAPI_SendLinMessage( Channel, Pid & 0x3F, CANOEAPI_LINRECONFIGUREDATA | CANOEAPI_LINAPPLYHEADER,
                               CANOEAPI_LINRESPONSEALWAYS, Dlc, Data );
    }
    break;

  case LIN_CANOE_FRAME_RX:
    CANoeAPI_SendLinMessage( Channel, Pid & 0x3F, CANOEAPI_LINAPPLYHEADER, CANOEAPI_LINRESPONSEALWAYS, Dlc, Data);
    break;

  case LIN_CANOE_FRAME_SILENT_SLEEP:
    CANoeAPI_SendLinSleepModeFrame( Channel, 0x01, 0x00, 0xFF);
    break;

  case LIN_CANOE_FRAME_WAKEUP:
    CANoeAPI_SendLinWakeupFrame( Channel, 125, 0x01);
    break;

  case LIN_CANOE_FRAME_WAKEUP_INTERNAL:
    /* NOT SUPPORTED BY CANoe: AR4-832: External wake up via LIN transceiver */
    CANoeAPI_WriteSysState("AUTOSAR LIN Driver: External wake up via LIN transceiver not supported!");
    CANoeAPI_WriteSysState("AUTOSAR LIN Driver: Sending normal Wakeup Frame!");
    CANoeAPI_SendLinWakeupFrame( Channel, 125, 0x01);
    break;

  default:
    break;
  }

  Lin_CANoeCntrl[Channel].FrameType = LIN_CANOE_FRAME_DONE;
  ;
}

/**********************************************************************************************************************
 * Lin_CANoeGetData
 **********************************************************************************************************************/
/*! \brief      Read current data from Vip Lin Controller.
 *  \param[in]  Channel  The Channel Index.
 *  \param[out] data     The Data with length = 8.
 *  \pre
 *  \context    This function is called by the LIN Driver
 *  \note
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetData
(
  VAR(uint8, AUTOMATIC)  channel,
  VAR(uint8, AUTOMATIC)  data[]
)
{
  VAR(uint8, AUTOMATIC)  i;

  if( data != 0 )
  {
    
    for( i = 0; i < 8; i++ ) /* copy data */
    {
      data[i] = Lin_CANoeCntrl[channel].Data[i];
    }

    ;
  }
}

/**********************************************************************************************************************
 * Lin_CANoeGetClearInterruptFlags
 **********************************************************************************************************************/
/*! \brief      Read and clear current interrupt flags.
 *  \param[in]  Channel  The Channel Index.
 *  \param[out] flags    The Flags.
 *  \pre
 *  \context    This function is called by the LIN Driver
 *  \note
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetClearInterruptFlags
(
  VAR(  uint8,  AUTOMATIC )               channel,
  P2VAR(uint32, AUTOMATIC, LIN_APPL_VAR ) flags
)
{
  
  *flags = Lin_CANoeCntrl[channel].InterruptFlags;
  Lin_CANoeCntrl[channel].InterruptFlags = 0;      /* Flags are cleared, after Lin Driver has read them.*/

  ;
}

/**********************************************************************************************************************
 * Lin_CANoeGetClearErrorFlags
 **********************************************************************************************************************/
/*! \brief      Read and clear current error flags.
 *  \param[in]  Channel  The Channel Index.
 *  \param[out] flags    The Flags.
 *  \pre        -
 *  \context    This function is called by the LIN Driver
 *  \note       -
 **********************************************************************************************************************/
STATIC FUNC(void, LIN_CODE) Lin_CANoeGetClearErrorFlags
(
  VAR(  uint8,  AUTOMATIC )               channel,
  P2VAR(uint32, AUTOMATIC, LIN_APPL_VAR ) flags
)
{
  
  *flags = Lin_CANoeCntrl[channel].ErrorFlags;
  Lin_CANoeCntrl[channel].ErrorFlags = 0;       /* Flags are cleared, after Lin Driver has read them.*/

  ;
}
/* ------------------------------------------------------------------------------------------------------------------ */

#define LIN_STOP_SEC_CODE
#include "MemMap.h"                                                                                                     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:

   MD_LIN_HL_16.7:
     Reason: API specification given by AUTOSAR and must be kept.
     Risk: N/A
     Prevention: N/A
     
   MD_LIN_0303:
     Reason: The size of integer required to hold the result of a pointer cast is implementation-defined. This type
             of operation is not uncommon in embedded software applications where memory mapped I/O ports must be
             addressed.
     Risk: Cast a pointer to an integer type or vice versa may be non-portable.
     Prevention: The code inspection and tests on hardware ensure that the memory location referenced contains the
                 registers with the same structure as specified.

   MD_LIN_CANoeAPI:
     Reason:     CANoe API could not changed and is not defined well.
     Risk:       Driver not working.
     Prevention: No Prevention possible.
                 
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_LIN_PTR_HW 
  \DESCRIPTION The function accesses a hardware register using a pointer access.
  \COUNTERMEASURE \T The code inspection and tests on hardware ensure that the memory location
                     referenced contains the registers with the same structure as specified.
                  \S Verify that the values generated match the address of register of the used hardware.

  \ID SBSW_LIN_CSL03_INTERNALSTATE_CHANNELID
  \DESCRIPTION Access to InternalState via indirection over ChannelId.
  \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib.

  \ID SBSW_LIN_BUFFER_WRITE
  \DESCRIPTION Write to the array of constant size.
  \COUNTERMEASURE \R The index based access to the array uses constant index values. The buffer is statically allocated
                     with fixed size (e.g. 8 bytes). The size is sufficient for the largest used index value (e.g. 7).

  \ID SBSW_LIN_PTR
  \DESCRIPTION The function writes to the object referenced by parameter.
  \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter is valid.

  \ID SBSW_LIN_PTR_CALL
  \DESCRIPTION The function call uses a pointer parameter.
  \COUNTERMEASURE \N A local variable is used and Lin_SendFrame copy the data to internal buffer.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_LIN_COMPILER
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON The source code covers different compilers. There is however only a subset of the compilers released for
          safeBSW, therefore the code of the unsupported compilers is not covered.

  \ID COV_LIN_DERIVATIVE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON The source code covers different derivatives. There is however only a subset of the derivatives released for
          safeBSW, therefore the code of the unsupported derivatives features or existing/not existing erratas is not
          covered.

  \ID COV_LIN_TOOL_ISSUE
  \ACCEPT TX tx xf
  \REASON Tool issue. Condition depends on include path.

  \ID COV_LIN_INTEGRATION
  \ACCEPT XF
  \REASON The include of the transceiver types header is not tested in the LIN driver test suite because it is a stub
          test without transceiver driver. 

  \ID COV_LIN_COMPATIBILITY
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]

END_COVERAGE_JUSTIFICATION */ 

/**********************************************************************************************************************
 *  END OF FILE: Lin.c
 *********************************************************************************************************************/

