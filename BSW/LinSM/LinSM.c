/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file   LinSM.c
 *        \brief  LIN State Manager ASR4
 *
 *      \details  Implementation of the LIN State Manager AUTOSAR Release 4.0
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

#define LINSM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "LinSM_Cbk.h"
#include "LinSM.h"
#include "BswM_LinSM.h"
#include "ComM_BusSM.h"
#include "SchM_LinSM.h"
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif
#if ( LINSM_USE_INIT_POINTER == STD_ON ) && ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Header version check. 
 * \trace SPEC-17422 */
#if (  (LINSM_SW_MAJOR_VERSION != (0x05u)) \
    || (LINSM_SW_MINOR_VERSION != (0x02u)) \
    || (LINSM_SW_PATCH_VERSION != (0x00u)) )
# error "Vendor specific version numbers of LinSM.c and LinSM.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if (  (LINSM_AR_RELEASE_MAJOR_VERSION != (0x04u)) \
    || (LINSM_AR_RELEASE_MINOR_VERSION != (0x00u)) \
    || (LINSM_AR_RELEASE_PATCH_VERSION != (0x03u)) )
# error "AUTOSAR Specification Version numbers of LinSM.c and LinSM.h are inconsistent!"
#endif

/* Callback header version check 
 * \trace SPEC-17422 */
#if (  (LINSM_CBK_MAJOR_VERSION != (0x05u)) \
    || (LINSM_CBK_MINOR_VERSION != (0x02u)) \
    || (LINSM_CBK_PATCH_VERSION != (0x00u)) )
# error "Vendor specific version numbers of LinSM.c and LinSM_Cbk.h are inconsistent!"
#endif

/* LinSM Types header version check 
 * \trace SPEC-17422 */
#if (  (LINSM_TPS_MAJOR_VERSION != (0x05u)) \
    || (LINSM_TPS_MINOR_VERSION != (0x02u)) \
    || (LINSM_TPS_PATCH_VERSION != (0x00u)) )
# error "Vendor specific version numbers of LinSM.c and LinSM_Types.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Defines for LinSM_RequestedComMode */
#define LINSM_FULL_COM_REQUESTED          0x01u
#define LINSM_NO_COM_REQUESTED            0x02u
#define LINSM_NO_STATE_CHANGE_REQUESTED   0x03u

# if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/* Defines for parameter trcvOnOff of function LinSM_SwitchTransceiver */
# define LINSM_SWITCH_TRCV_OFF             0x00u
# define LINSM_SWITCH_TRCV_ON              0x01u
#endif

/* ESCAN00063508 */
#if !defined (LINSM_LOCAL)  /* COV_MSR_COMPATIBILITY */
# define LINSM_LOCAL static
#endif

#if !defined (LINSM_LOCAL_INLINE)  /* COV_MSR_COMPATIBILITY */
# define LINSM_LOCAL_INLINE LOCAL_INLINE
#endif

#if !defined LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG
/* For MISRA compliance LINSM_ISDEF_<param> needs to be defined in case LINSM_<param> is STD_OFF, i.e. param is not generated */
# define LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG STD_OFF
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/* Used parameter type for function LinSM_SwitchTransceiver */
typedef uint8 LinSM_SwitchTrcvOnOffType;
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define LINSM_START_SEC_CONST_8BIT
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_19.1 */

/* LinSM version constants */
CONST(uint8, LINSM_CONST) LinSM_MainVersion      = LINSM_SW_MAJOR_VERSION;
CONST(uint8, LINSM_CONST) LinSM_SubVersion       = LINSM_SW_MINOR_VERSION;
CONST(uint8, LINSM_CONST) LinSM_ReleaseVersion   = LINSM_SW_PATCH_VERSION;

#define LINSM_STOP_SEC_CONST_8BIT
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_19.1 */

#define LINSM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( LINSM_USE_INIT_POINTER == STD_ON )
P2CONST(LinSM_ConfigType, LINSM_VAR_ZERO_INIT, LINSM_INIT_DATA) LinSM_ConfigDataPtr = NULL_PTR;
#endif

#define LINSM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define LINSM_START_SEC_CODE
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_ConfirmationTimerTask()
 *********************************************************************************************************************/
/*! \brief      Handles the confirmation timer
 *  \details    In case of a confirmation timeout a development error is reported and further error handling is done.
 *  \pre        -
 *  \param[in]  channel       Local channel handle
 *  \param[in]  network       System channel handle
 *  \trace      SPEC-10447, SPEC-17343, SPEC-17370, SPEC-17393, SPEC-17415, SPEC-17418, SPEC-17440
**********************************************************************************************************************/
LINSM_LOCAL_INLINE FUNC( void, LINSM_CODE ) LinSM_ConfirmationTimerTask( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                         CONST( NetworkHandleType, AUTOMATIC ) network );
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */

/**********************************************************************************************************************
 * LinSM_TriggerFullCom()
 *********************************************************************************************************************/
/*! \brief      Triggers the transition to full communication mode
 *  \details    Transceiver is switched to normal mode and the full communication request is forwarded to LinIf.
 *  \pre        -
 *  \param[in]  channel         Local channel handle
 *  \param[in]  network         System channel handle
 *  \return     E_OK     - Request accepted
 *              E_NOT_OK - Wake-up is not accepted by LinIf, i.e. LinIf_Wakeup returned E_NOT_OK
 *  \trace      SPEC-10447, SPEC-17380
**********************************************************************************************************************/
LINSM_LOCAL_INLINE FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerFullCom( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                            CONST( NetworkHandleType, AUTOMATIC ) network );

/**********************************************************************************************************************
 * LinSM_TriggerGotoSleep()
 *********************************************************************************************************************/
/*! \brief      Triggers the go to sleep process
 *  \details    Forwards the go to sleep request to the LinIf.
 *  \pre        -
 *  \param[in]  channel         Local channel handle
 *  \param[in]  network         System channel handle
 *  \return     E_OK     - Request accepted
 *              E_NOT_OK - Go to sleep is not accepted by LinIf, i.e. LinIf_GotoSleep returned E_NOT_OK.
 *  \trace      SPEC-10447, SPEC-17403, SPEC-17420
**********************************************************************************************************************/
LINSM_LOCAL FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerGotoSleep( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                       CONST( NetworkHandleType, AUTOMATIC ) network );

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_SwitchTransceiver()
 *********************************************************************************************************************/
/*! \brief      Switches the transceiver to Normal or Sleep/Standby state
 *  \details    -
 *  \pre        -
 *  \param[in]  channel       Local channel handle
 *  \param[in]  trcvOnOff     LINSM_SWITCH_TRCV_OFF or LINSM_SWITCH_TRCV_ON
**********************************************************************************************************************/
LINSM_LOCAL FUNC( void, LINSM_CODE ) LinSM_SwitchTransceiver( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                              CONST( LinSM_SwitchTrcvOnOffType, AUTOMATIC ) trcvOnOff );
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LinSM_TriggerGotoSleep()
 *********************************************************************************************************************/
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
 *
 *
 */
LINSM_LOCAL FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerGotoSleep( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                              CONST( NetworkHandleType, AUTOMATIC ) network )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* #21 Check if Sleep Support is enabled on this channel */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  if( LinSM_IsSleepSupportOfChannelConfig(channel) )
# endif
  {
    /* #211 Store that go-to-sleep is pending */
    LinSM_SetRequestedComMode( channel, LINSM_NO_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    /* #212 Start confirmation timeout */
    LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig(channel) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

    /* #213 Call LinIf_GotoSleep and store return value */
    retVal = LinIf_GotoSleep( network );

    /* #214 Check if LinIf_GotoSleep has returned E_NOT_OK */
    if( retVal == E_NOT_OK )
    {
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
      /* #2141 Stop confirmation timeout */
      LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

      /* #2142 Set state to no communication immediately */
      LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
      /* #2143 Call LinSM_SwitchTransceiver to switch transceiver off */
      LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
# endif

      /* #2144 Notify new state to ComM and BswM */
      ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState( channel ) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
    }
  }
  /* #22 Else: Sleep Support is disabled on this channel */
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  else  /* LinSM_IsSleepSupportOfChannelConfig(channel) == FALSE */
# endif
#endif
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) || ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
  {
    /* #221 Change state immediately to LINSM_NO_COM */
    LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
    /* #222 Call LinSM_SwitchTransceiver to switch transceiver off */
    LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
# endif

    /* #223 Notify new state to ComM and BswM */
    ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState(channel) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
    BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
  }
#endif

  return retVal;

} /* LinSM_TriggerGotoSleep() */

/**********************************************************************************************************************
 * LinSM_TriggerFullCom()
 *********************************************************************************************************************/
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
LINSM_LOCAL_INLINE FUNC( Std_ReturnType, LINSM_CODE ) LinSM_TriggerFullCom( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                            CONST( NetworkHandleType, AUTOMATIC ) network )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
  /* #2411 Check if wake-up process is not already in process */
  /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
    *                if Sleep Support is disabled. */
  if( LinSM_GetRequestedComMode(channel) != LINSM_FULL_COM_REQUESTED )
#endif
  {

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
    /* #24111 Call LinSM_SwitchTransceiver() to switch transceiver on */
    LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_ON );
#endif

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    /* #24112 Check if Sleep Support is enabled */
    if( LinSM_IsSleepSupportOfChannelConfig(channel) )
# endif
    {
      /* #241121 Store that wake-up is pending */
      LinSM_SetRequestedComMode( channel, LINSM_FULL_COM_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
      /* #241122 Start confirmation timeout */
      LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig(channel) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif
      /* #241123 Call LinIf_Wakeup() */
      /* ESCAN00074303 Just return the return value of LinIf_Wakeup() with no further action, because error
        * handling is either done on negative confirmation or on confirmation timeout. */
      retVal = LinIf_Wakeup( network );
    }
# if ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    else
    /* #24113 Else: Sleep support is disabled */
# endif
#endif
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) || ( LINSM_ISDEF_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF )
    {
      /* #241131 Change state immediately to LINSM_FULL_COM */
      LinSM_SetComState( channel, LINSM_STATE_FULL_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

      /* #241132 Notify ComM and BswM about new state */
      ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState(channel) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
      BswM_LinSM_CurrentState( network, LINSM_BSWM_FULL_COM );
    }
#endif
  }

  return retVal;
} /* LinSM_TriggerFullCom() */

#if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_SwitchTransceiver()
 *********************************************************************************************************************/
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
LINSM_LOCAL FUNC( void, LINSM_CODE ) LinSM_SwitchTransceiver( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                         CONST( LinSM_SwitchTrcvOnOffType, AUTOMATIC ) trcvOnOff )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  NetworkHandleType network = LinSM_GetComMChannelHandleOfChannelConfig( channel );
#else
  NetworkHandleType network = channel;
# endif

# if ( LINSM_ISDEF_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_OFF )
  /* #2 Check if transceiver handling is configured for this channel */
  if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) != LINSM_TRCV_NONE )
# endif
  {
    LinTrcv_TrcvModeType curTrcvMode;
    LinTrcv_TrcvModeType newTrcvMode;

    /* #21 Determine new transceiver mode to be set */
    if( trcvOnOff == LINSM_SWITCH_TRCV_OFF )
    {
# if ( LINSM_ISDEF_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
#  if ( LINSM_EQ2_TRANSCEIVERHANDLINGOFCHANNELCONFIG == LINSM_TRCV_STANDBY )
      newTrcvMode = LINTRCV_TRCV_MODE_STANDBY;
#  else
      newTrcvMode = LINTRCV_TRCV_MODE_SLEEP;
#  endif
# else
      /* #211 Check if transceiver shall be switched off and transceiver handling is configured to LINSM_TRCV_STANDBY */
      if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) == LINSM_TRCV_STANDBY )
      {
        /* #2111 Set new transceiver mode to standby */
        newTrcvMode = LINTRCV_TRCV_MODE_STANDBY;
      }
      /* #2112 Else: Set new transceiver mode to sleep */
      else /* LinSM_GetTransceiverHandlingOfChannelConfig( channel ) == LINSM_TRCV_SLEEP */
      {
        newTrcvMode = LINTRCV_TRCV_MODE_SLEEP;
      }
# endif
    }
    /* #212 Else: Set new transceiver mode to normal */
    else
    {
      newTrcvMode = LINTRCV_TRCV_MODE_NORMAL;
    }

    /* #22 Get current transceiver mode */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    if( LinIf_GetTrcvMode( network, &curTrcvMode ) == E_NOT_OK ) /* SBSW_LINSM_LINIFGETTRCVMODE_CALL */
    {
      errorId = LINSM_E_GETTRCVMODE_FAILED;
    }
# else
    (void)LinIf_GetTrcvMode( network, &curTrcvMode ); /* SBSW_LINSM_LINIFGETTRCVMODE_CALL */
# endif

    if( curTrcvMode != newTrcvMode )
    {
      /* #23 Set new transceiver mode, if not already active */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
      if( LinIf_SetTrcvMode( network, newTrcvMode ) == E_NOT_OK )
      {
        errorId = LINSM_E_SETTRCVMODE_FAILED;
      }
# else
      (void)LinIf_SetTrcvMode( network, newTrcvMode );
# endif
    } /* if (curTrcvMode != newTrcvMode) */
  } /* if( LinSM_GetTransceiverHandlingOfChannelConfig( channel ) != LINSM_TRCV_NONE ) */

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SWITCH_TRANSCEIVER, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  LINSM_DUMMY_STATEMENT_CONST( channel ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
} /* LinSM_SwitchTransceiver() */
#endif /* ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) */

#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
 * LinSM_ConfirmationTimerTask()
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 */
LINSM_LOCAL_INLINE FUNC( void, LINSM_CODE ) LinSM_ConfirmationTimerTask( CONST( NetworkHandleType, AUTOMATIC ) channel,
                                                                         CONST( NetworkHandleType, AUTOMATIC ) network )
{
  /* #23 Check if confirmation timer is running */
  if( LinSM_GetConfirmationTimer(channel) != 0u )
  {
    /* #231 Decrement confirmation timer */
    LinSM_DecConfirmationTimer( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
    /* #232 Check if confirmation timer has elapsed */
    if( LinSM_GetConfirmationTimer(channel) == 0u )
    {
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
      /* #233 Call DET Error in any case */
      (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_MAIN_FUNCTION, LINSM_E_CONFIRMATION_TIMEOUT );
# endif

# if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
      /* #234 Check if it is a wake-up timeout */
      /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
        *                if Sleep Support is disabled. */
      if( LinSM_GetRequestedComMode(channel) == LINSM_FULL_COM_REQUESTED )
      {
#  if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
        /* #2341 Check if max number of wake-up repetitions is reached */
        if( LinSM_GetWakeUpRetryCounter(channel) < LinSM_GetModeRequestRepetitionMax() )
        {
          /* #23411 Increment wake-up counter */
          /* ESCAN00074303 Retry wake-up */
          LinSM_IncWakeUpRetryCounter( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

          /* #23412 Restart confirmation timer */
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig(channel) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

          /* #23113 Call LinIf_WakeUp */
          /* ESCAN00074303 Return value of LinIf_Wakeup() can be ignored, because error handling is either handled
            *               on negative confirmation or on confirmation timeout. */
          (void)LinIf_Wakeup( network );
        }
        else
#  endif
        /* #2342 Else: Go back to sleep */
        {
          /* #23421 Call LinSM_TriggerGotoSleep */
          /* Wake-up timeout -> trigger go to sleep */
          (void)LinSM_TriggerGotoSleep( channel, network );
        }
      }
      /* #235 Check if it is a go-to-sleep timeout */
      else if( LinSM_GetRequestedComMode(channel) == LINSM_NO_COM_REQUESTED )
      {
        /* #2351 Change to LINSM_NO_COM immediately */
        LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

#  if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
        /* #2352 Call LinSM_SwithTransceiver to switch transceiver off */
        LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
#  endif
        /* #2353 Notify no communication state to ComM and BswM */
        ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState(channel) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
      }
      else /* LinSM_RequestedComMode == LINSM_NO_STATE_CHANGE_REQUESTED */
# else
        LINSM_DUMMY_STATEMENT_CONST( network ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON */
      /* #236 Else: It is a schedule request timeout */
      {
        /* #2361 Nothing to do. Det error already notified above */
      }
    }
  }
} /* LinSM_ConfirmationTimerTask() */
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LinSM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_InitMemory( void )
{
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #1 Set global state to not initialized */
  LinSM_SetInitialized( FALSE );
#endif
} /* LinSM_InitMemory() */

/**********************************************************************************************************************
 * LinSM_Init
 *********************************************************************************************************************/
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
 */
FUNC(void, LINSM_CODE) LinSM_Init( P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_INIT_DATA) ConfigPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;
  uint8 ecumErrorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  /* #11 Check that module is not initialized */
  if( LinSM_IsInitialized() )
  {
    errorId = LINSM_E_INIT_FAILED;
  }
  else
#endif
#if ( LINSM_USE_INIT_POINTER == STD_ON )
  /* #12 Check that configuration parameter 'ConfigPtr' is not NULL */
  if (ConfigPtr == NULL_PTR)
  {
# if ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
    ecumErrorId = ECUM_BSWERROR_NULLPTR;
# endif
    errorId = LINSM_E_PARAM_CONFIG;
  }
  else
#endif
  {
#if ( LINSM_USE_INIT_POINTER == STD_ON )
    /* #13 Store pointer to configuration */
    LinSM_ConfigDataPtr = ConfigPtr;

# if ( LINSM_FINALMAGICNUMBER == STD_ON )
    /* #14 Check Magic Number */
    /* Magic number check needs to be done after storing the config pointer, because local config pointer is used in access macro. */
    if( LinSM_GetFinalMagicNumber() != LINSM_FINAL_MAGIC_NUMBER )
    {
      ecumErrorId = ECUM_BSWERROR_MAGICNUMBER;
      errorId = LINSM_E_PARAM_CONFIG;
    }
    else
# endif
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      NetworkHandleType channel;

      /* #21 Initialize all global and local variables */
      for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ )
      {
        LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
        /* ESCAN00060620: LinSM_RequestedComMode is only used if Sleep Support is enabled. */
        LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
        LinSM_SetWakeUpRetryCounter( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        LinSM_SetNegativeConfirmation( channel, FALSE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif
#endif
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif
      }
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
      /* #22 Set global state to initialized */
      LinSM_SetInitialized( TRUE );
#endif
    }
  }

  /* ----- EcuM Error Hook --------------------------------------- */
#if ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
  /* #4 Report EcuM error if applicable */
  if (ecumErrorId != LINSM_E_NO_ERROR)
  {
    (void)EcuM_BswErrorHook( LINSM_MODULE_ID, ecumErrorId );
  }
#else
  LINSM_DUMMY_STATEMENT( ecumErrorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_INIT, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

#if ( LINSM_USE_INIT_POINTER == STD_OFF )
  LINSM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* LinSM_Init() */

/**********************************************************************************************************************
 * LinSM_MainFunction()
 *********************************************************************************************************************/
/*!
 *
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
 */
FUNC(void, LINSM_CODE) LinSM_MainFunction( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #1 Check that module is initialized. */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    NetworkHandleType channel;

    /* #2 Iterate over all LinSM channels */
    for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ )
    {
# if ( ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) ) || ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
      NetworkHandleType network = LinSM_GetComMChannelHandleOfChannelConfig( channel );
#  else
      NetworkHandleType network = channel;
#  endif
# endif

      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) && ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
      /* #22 Check if negative wake-up confirmation occurred */
      if( LinSM_IsNegativeConfirmation( channel ) )
      {
        /* #221 Handle negative wake-up confirmation -> Retry wake-up */
        LinSM_SetNegativeConfirmation( channel, FALSE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

        /* #2211 Increment wake-up counter */
        /* ESCAN00074303 Retry wake-up */
        LinSM_IncWakeUpRetryCounter( channel ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        /* #2212 Start confirmation timer */
        LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig(channel) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

        /* #2213 Call LinIf_WakeUp */
        /* ESCAN00074303 Return value of LinIf_Wakeup() can be ignored, because error handling is either done
         *               on negative confirmation or on confirmation timeout. */
        (void)LinIf_Wakeup( network );
      }
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
      /* #23 Else: Handle confirmation timeout */
      else
# endif
#endif
      {
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        /* #231 Call LinSM_ConfirmationTimerTask() */
        LinSM_ConfirmationTimerTask( channel, network );
#endif /* LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON */
      }
      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    } /* for( channel = 0u; channel < LinSM_GetSizeOfChannelConfig(); channel++ ) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_MAIN_FUNCTION, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* LinSM_MainFunction() */

#if( LINSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 * LinSM_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, LINSM_APPL_DATA) versionInfo )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #1 Check that passed versionInfo parameter is not NULL. */
  if ( versionInfo == NULL_PTR )
  {
    errorId = LINSM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #2 Read out vendor ID, module ID and version data */
    versionInfo->vendorID         = (uint16)LINSM_VENDOR_ID; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->moduleID         = (uint16)LINSM_MODULE_ID; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_major_version = (uint8) LINSM_SW_MAJOR_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_minor_version = (uint8) LINSM_SW_MINOR_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
    versionInfo->sw_patch_version = (uint8) LINSM_SW_PATCH_VERSION; /* SBSW_LINSM_VERSIONINFO_PTRWRITE */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (LINSM_DEV_ERROR_REPORT == STD_ON)
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GET_VERSION_INFO, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* LinSM_GetVersionInfo() */
#endif /* LINSM_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 * LinSM_RequestComMode
 *********************************************************************************************************************/
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
 *
 *
 */
FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode( VAR(NetworkHandleType, AUTOMATIC) network,
                                                       VAR(ComM_ModeType, AUTOMATIC) mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  /* #13 Check that requested mode has valid value */
  /* [SPEC-17383] If COMM_SILENT_COMMUNICATION is requested the function shall return E_NOT_OK directly without action. */
  if( ( mode != COMM_NO_COMMUNICATION ) && ( mode != COMM_FULL_COMMUNICATION ) )
  {
    errorId = LINSM_E_PARAMETER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #21 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #23 Check if requested mode is already active */
    if( ( LinSM_GetComState(channel) == mode )
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
      /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
       *                if Sleep Support is disabled. */
      && (LinSM_GetRequestedComMode(channel) == LINSM_NO_STATE_CHANGE_REQUESTED)
#endif
      )
    {
      /* #231 Report with Det error that mode is already active */
      /* [SPEC-17425] If the requested mode is the same as the current active mode,
       *             the function shall return E_NOT_OK directly without action. */
      errorId = LINSM_E_MODE_ALREADY_ACTIVE;
    }
    /* #24 Else: Handle new mode request */
    else /* LinSM_GetComState( channel ) != mode || LinSM_GetRequestedComMode( channel ) != LINSM_NO_STATE_CHANGE_REQUESTED */
    {
      retVal = E_OK;

      /* #241 Check if full communication is requested */
      if( mode == COMM_FULL_COMMUNICATION )
      {
        retVal = LinSM_TriggerFullCom( channel, network );
      }
      /* #242 Else: No communication is requested */
      else
      {
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
        /* #2421 Check if goto-sleep is not already in process */
        /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
          *                if Sleep Support is disabled. */
        if( LinSM_GetRequestedComMode(channel) != LINSM_NO_COM_REQUESTED )
#endif
        {
          /* #24211 Trigger goto-sleep process by calling LinSM_TriggerGotoSleep() */
          retVal = LinSM_TriggerGotoSleep( channel, network );
        }
      }
    }

    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_REQUEST_COM_MODE, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* LinSM_RequestComMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LinSM_GetCurrentComMode()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode( VAR(NetworkHandleType, AUTOMATIC) network,
                                                          P2VAR(ComM_ModeType, AUTOMATIC, LINSM_APPL_DATA) mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that passed mode pointer is not NULL */
  if( mode == NULL_PTR )
  {
    errorId = LINSM_E_PARAM_POINTER;
  }
  /* #12 Check that module is initialized */
  else if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
    /* [SPEC-17432] If active state is LINSM_UNINIT the state COMM_NO_COMMUNICATION shall be returned */
    (*mode) = COMM_NO_COMMUNICATION; /* SBSW_LINSM_MODE_PTRWRITE */
  }
  /* #13 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #21 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

    /* #22 Return current ComMode */
    (*mode) = LinSM_GetComState( channel ); /* SBSW_LINSM_MODE_PTRWRITE */

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GET_CURRENT_COM_MODE, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* LinSM_GetCurrentComMode() */

/**********************************************************************************************************************
 * LinSM_ScheduleRequest()
 *********************************************************************************************************************/
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
 *
 */
FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest( VAR(NetworkHandleType, AUTOMATIC) network,
                                                        VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else

#endif
  {
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #13 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    /* #14 Check that schedule handle is valid */
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig(channel) )
    {
      errorId = LINSM_E_PARAMETER;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #22 Check if LinSM is not in full communication state */
      if( LinSM_GetComState(channel) != LINSM_STATE_FULL_COM )
      {
        /* #221 Report with Det error that LinSM was not in correct state */
        /* [SPEC-17378] If the function LinSM_ScheduleRequest is called and the state is NOT LINSM_FULL_COM,
         *             the LinSM_ScheduleRequest shall return directly with E_NOT_OK. */
        errorId = LINSM_E_NOT_IN_FULL_COM;
      }
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
      /* #23 Check if wake-up or sleep request is still in process */
      /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
       *                if Sleep Support is disabled. */
      else if( LinSM_GetRequestedComMode(channel) != LINSM_NO_STATE_CHANGE_REQUESTED )
      {
        /* #231 Report with Det error that another request is still in process */
        /* [SPEC-17423] If the function LinSM_ScheduleRequest is called and another request is in process
         *            on the same network, the LinSM_ScheduleRequest shall return directly with E_NOT_OK. */
        errorId = LINSM_E_REQUEST_IN_PROCESS;
      }
#endif
      /* #24 Else: Execute the schedule request */
      else
      {
        /* #241 Call LinIf_ScheduleRequest() to request new schedule */
        retVal = LinIf_ScheduleRequest( network, schedule );
        if( retVal == E_OK )
        {
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          /* #242 Start confirmation timer */
          LinSM_SetConfirmationTimer( channel, LinSM_GetConfirmationTimeoutOfChannelConfig(channel) ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif
        }
      }

      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* LinSM_ScheduleRequest() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LinSM_WakeupConfirmation
 *********************************************************************************************************************/
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
FUNC(void, LINSM_CODE) LinSM_WakeupConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                 VAR(boolean, AUTOMATIC) success )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
    /* ESCAN00060620: LinSM_WakeupConfirmation will only be called if Sleep Support is enabled. */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #21 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
# else
    NetworkHandleType channel = network;
# endif

    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #23 Check if confirmation timeout is running or disabled (configured to 0) on this channel */
    /* [SPEC-17342] If LinIf module calls the confirmation callback after the timer has
     *            elapsed, no further notification shall be made to the ComM module,
     *            i.e. the confirmation is ignored. */
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
  if(    (LinSM_GetConfirmationTimer(channel) != 0u)
#  if ( LINSM_ISDEF_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF )
      /* Confirmation Timeout will be generated with 0, only if it is not equal on all channels. */
        || (LinSM_GetConfirmationTimeoutOfChannelConfig(channel) == 0u)
#  endif
      )
# else
    /* Confirmation timeout is 0 on all channels. */
# endif
    {
      /* #231 Check if Wake-up request is pending */
      if( (LinSM_GetComState(channel) == LINSM_STATE_NO_COM)
        || (LinSM_GetRequestedComMode(channel) != LINSM_NO_STATE_CHANGE_REQUESTED) )
      {
        /* #2311 Check if full communication is requested */
        if( LinSM_GetRequestedComMode(channel) == LINSM_FULL_COM_REQUESTED )
        {
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
          /* #23111 Stop confirmation timer */
          LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif

          /* #23112 Check if LinIf reported wake-up success */
          if( success == TRUE )
          {
            /* #231121 Set state to full communication */
            LinSM_SetComState( channel, LINSM_STATE_FULL_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

            /* #231122 Notify new state to ComM and BswM */
            ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState(channel) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            BswM_LinSM_CurrentState( network, LINSM_BSWM_FULL_COM );
          }
          /* #2313 Else: LinIf reported NO wake-up success */
          else
          {
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
            /* #23131 Call DET Error for not successful wake-up confirmation. */
            (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_WAKEUP_CONFIRMATION, LINSM_E_WAKEUPCONF_NO_SUCCESS );
# endif

# if ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON )
            /* #23132 Check if max number of wake-up repetitions is not reached */
            if( LinSM_GetWakeUpRetryCounter(channel) < LinSM_GetModeRequestRepetitionMax() )
            {
              /* Set set flag to indicate negative confirmation and to force wake-up retry on next main function call. */
              LinSM_SetNegativeConfirmation( channel, TRUE ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
            }
            /* 23133 Else: Max number of wake-up repetitions is reached */
            else
# endif
            {
              /* #231331 Call LinSM_TriggerGotoSleep() to go back to no communication state */
              /* Wake-up was not accepted -> Goto Sleep */
              (void)LinSM_TriggerGotoSleep( channel, network );
            }
          }
        }
        /* #2312 Else: Full communication is not requested */
        else /* LinSM_GetRequestedComMode( channel ) != LINSM_FULL_COM_REQUESTED */
        {
          /* #23121 Report with Det error that full communication is not requested */
          errorId = LINSM_E_FULLCOM_NOT_REQUESTED;
        }
      }
      /* #232 Else: Wake-up request is not pending */
      else /* LinSM_GetComState( channel ) == LINSM_STATE_FULL_COM */
      {
        /* #2221 Report with Det error that mode is already active */
        errorId = LINSM_E_MODE_ALREADY_ACTIVE;
      }
    }
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    /* #24 Else: Confirmation already timed out */
    else
    {
      /* #241 Ignore confirmation. Timeout actions already done in main function */
    }
# endif

    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#else
    LINSM_DUMMY_STATEMENT( success ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    LINSM_DUMMY_STATEMENT( network ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_WAKEUP_CONFIRMATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* LinSM_WakeupConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LinSM_GotoSleepConfirmation
 *********************************************************************************************************************/
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
 *
 *
 *
 */
FUNC(void, LINSM_CODE) LinSM_GotoSleepConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                    VAR(boolean, AUTOMATIC) success )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
    /* ESCAN00060620: LinSM_GotoSleepConfirmation will only be called if Sleep Support is enabled. */

    /* ----- Implementation ----------------------------------------------- */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #21 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
# else
    NetworkHandleType channel = network;
# endif

    SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #23 Check if confirmation timeout is running or disabled (configured to 0) on this channel */
    /* [SPEC-17342] If LinIf module calls the confirmation callback after the timer has
     *            elapsed, no further notification shall be made to the ComM module,
     *            i.e. the confirmation is ignored. */
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
  if(    (LinSM_GetConfirmationTimer(channel) != 0u)
#  if ( LINSM_ISDEF_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF )
      /* Confirmation Timeout will be generated with 0, only if it is not equal on all channels. */
      || (LinSM_GetConfirmationTimeoutOfChannelConfig(channel) == 0u)
#  endif
    )
# else
    /* Confirmation timeout is configured to 0 on all channels. */
# endif
    {
      /* #231 Check if 'no communication mode' request is pending */
      if( LinSM_GetRequestedComMode(channel) == LINSM_NO_COM_REQUESTED )
      {
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        /* #2311 Stop confirmation timeout */
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
# endif
        /* #2312 Set new mode */
        LinSM_SetComState( channel, LINSM_STATE_NO_COM ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        LinSM_SetRequestedComMode( channel, LINSM_NO_STATE_CHANGE_REQUESTED ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */

# if ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON )
        /* #2313 Call LinSM_SwithTransceiver() to switch transceiver off */
        LinSM_SwitchTransceiver( channel, LINSM_SWITCH_TRCV_OFF );
# endif
        /* #2314 Notify new mode to ComM and BswM */
        ComM_BusSM_ModeIndication( network, LinSM_GetAddrComState(channel) ); /* SBSW_LINSM_COMMMODEINDICATION_CALL */ /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
        BswM_LinSM_CurrentState( network, LINSM_BSWM_NO_COM );
      }
      /* #232 Else: 'no communication mode' was not requested before */
      else /* LinSM_GetRequestedComMode( channel ) != LINSM_NO_COM_REQUESTED */
      {
        /* #2321 Report with Det error that no communication mode was not requested */
        errorId = LINSM_E_NOCOM_NOT_REQUESTED;
      }
    }
# if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
    /* #24 Else: Confirmation already timed out */
    else
    {
      /* Ignore confirmation. Timeout actions already done in main function */
    }
# endif

    SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#else
    LINSM_DUMMY_STATEMENT( network ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  }

  /* LinIf will enter sleep state in any case independently from success value */
  LINSM_DUMMY_STATEMENT( success ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_GOTO_SLEEP_CONFIRMATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* LinSM_GotoSleepConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LinSM_ScheduleRequestConfirmation
 *********************************************************************************************************************/
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
 */
FUNC(void, LINSM_CODE) LinSM_ScheduleRequestConfirmation( VAR(NetworkHandleType, AUTOMATIC) network,
                                                          VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
# if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
# else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
# endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
#endif
  {
#if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #13 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#else
    NetworkHandleType channel = network;
#endif

#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
    /* #14 Check that schedule handle is valid */
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig(channel) )
    {
      errorId = LINSM_E_PARAMETER;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #22 Check if current state is full communication and no go-to-sleep request is pending */
      if( (LinSM_GetComState(channel) == LINSM_STATE_FULL_COM)
#if ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON )
        /* ESCAN00060620: LinSM_RequestedComMode is always LINSM_NO_STATE_CHANGE_REQUESTED
         *                if Sleep Support is disabled. */
        && (LinSM_GetRequestedComMode(channel) == LINSM_NO_STATE_CHANGE_REQUESTED)
#endif
        )
      {
#if ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON )
        /* #221 Stop confirmation timer */
        LinSM_SetConfirmationTimer( channel, 0u ); /* SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS */
#endif
      }
      /* #23 Else: Either LinSM is in Sleep state or go to sleep request is pending */
      else /* LinSM_RequestComMode != LINSM_NO_STATE_CHANGE_REQUESTED || LinSM_GetComState( channel ) == LINSM_STATE_NO_COM */
      {
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
        /* #231 Report with Det error that LinSM was not in full communication state */
        (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST_CONFIRMATION, LINSM_E_NOT_IN_FULL_COM );
#endif
      }

      SchM_Exit_LinSM_LINSM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #24 Notify BswM in any case */
      BswM_LinSM_CurrentSchedule( network, schedule );
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_REQUEST_CONFIRMATION, errorId );
  }
#else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* LinSM_ScheduleRequestConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( LINSM_SCHEDULE_END_NOTIFICATION == STD_ON )
/* ESCAN00080029 */
/**********************************************************************************************************************
 * LinSM_ScheduleEndNotification()
 *********************************************************************************************************************/
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
FUNC(void, LINSM_CODE) LinSM_ScheduleEndNotification( VAR(NetworkHandleType, AUTOMATIC) network, VAR(LinIf_SchHandleType, AUTOMATIC) schedule )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
  /* #11 Check that module is initialized */
  if( !LinSM_IsInitialized() )
  {
    errorId = LINSM_E_UNINIT;
  }
  /* #12 Check that network handle is valid */
#  if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
  else if( network >= LinSM_GetSizeOfComMToLinSMChannel() )
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else if( LinSM_GetComMToLinSMChannel( network ) >= LinSM_GetSizeOfChannelConfig() )
#  else
  else if( network >= LinSM_GetSizeOfChannelConfig() )
#  endif
  {
    errorId = LINSM_E_NONEXISTENT_NETWORK;
  }
  else
# endif
  {
# if ( LINSM_DEV_ERROR_DETECT == STD_ON )
#  if ( LINSM_ISDEF_SCHEDULETABLEIDRANGEOFCHANNELPOSTBUILDCONFIG == STD_OFF )
#   if ( LINSM_COMMTOLINSMCHANNEL == STD_ON )
    /* #13 Store local channel handle */
    NetworkHandleType channel = LinSM_GetComMToLinSMChannel( network );
#   else
    NetworkHandleType channel = network;
#   endif
#  endif

    /* #14 Check that schedule handle is valid */
    if( schedule >= LinSM_GetScheduleTableIdRangeOfChannelPostBuildConfig( channel ) )
    {
      errorId = LINSM_E_PARAMETER;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #2 Forward notification to BswM. */
      BswM_LinSM_ScheduleEndNotification( network, schedule );
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINSM_DEV_ERROR_REPORT == STD_ON )
  /* #3 Report error if applicable */
  if (errorId != LINSM_E_NO_ERROR)
  {
    (void)Det_ReportError( LINSM_MODULE_ID, LINSM_INSTANCE_ID_DET, LINSM_SID_SCHEDULE_END_NOTIFICATION, errorId );
  }
# else
  LINSM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* LinSM_ScheduleEndNotification() */
#endif /* LINSM_SCHEDULE_END_NOTIFICATION == STD_ON */

#define LINSM_STOP_SEC_CODE
#include "MemMap.h"/* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Verify configuration
 *********************************************************************************************************************/

#if !( defined ( LINSM_CONFIGURATION_VARIANT ) ) /* COV_MSR_CONFIG */
# error "LinSM configuration issue: LINSM_CONFIGURATION_VARIANT must be defined."
#else
# if !( ( LINSM_CONFIGURATION_VARIANT == LINSM_CONFIGURATION_VARIANT_PRECOMPILE ) || ( LINSM_CONFIGURATION_VARIANT == LINSM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) )
#  error "LinSM configuration issue: Only configuration variants pre-compile and post-build-loadable are supported."
# endif
#endif

#if !( ( defined ( LINSM_POSTBUILD_VARIANT_SUPPORT ) ) && ( ( LINSM_POSTBUILD_VARIANT_SUPPORT == STD_ON ) || ( LINSM_POSTBUILD_VARIANT_SUPPORT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_POSTBUILD_VARIANT_SUPPORT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_DEV_ERROR_DETECT ) ) && ( ( LINSM_DEV_ERROR_DETECT == STD_ON ) || ( LINSM_DEV_ERROR_DETECT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_DETECT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_DEV_ERROR_REPORT ) ) && ( ( LINSM_DEV_ERROR_REPORT == STD_ON ) || ( LINSM_DEV_ERROR_REPORT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_REPORT has to be defined STD_OFF or STD_ON"
#endif

#if ( ( LINSM_DEV_ERROR_REPORT == STD_ON ) && ( LINSM_DEV_ERROR_DETECT == STD_OFF ) )
# error "LinSM configuration issue: LINSM_DEV_ERROR_DETECT has to be defined STD_ON if LINSM_DEV_ERROR_REPORT == STD_ON"
#endif

#if !( ( defined ( LINSM_VERSION_INFO_API ) ) && ( ( LINSM_VERSION_INFO_API == STD_ON ) || ( LINSM_VERSION_INFO_API == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_VERSION_INFO_API has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG ) ) && ( ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_ON ) || ( LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_TRANSCEIVERHANDLINGOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG ) ) && ( ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_ON ) || ( LINSM_SLEEPSUPPORTOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_SLEEPSUPPORTOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_DUMMY_STATEMENT ) ) && ( ( LINSM_USE_DUMMY_STATEMENT == STD_ON ) || ( LINSM_USE_DUMMY_STATEMENT == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_USE_DUMMY_STATEMENT has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_MODEREQUESTREPETITIONMAX ) ) && ( ( LINSM_MODEREQUESTREPETITIONMAX == STD_ON ) || ( LINSM_MODEREQUESTREPETITIONMAX == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_MODEREQUESTREPETITIONMAX has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_INIT_POINTER ) ) && ( ( LINSM_USE_INIT_POINTER == STD_ON ) || ( LINSM_USE_INIT_POINTER == STD_OFF ) ) )
# error "LinSM configuration issue:LINSM_USE_INIT_POINTER  has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_USE_ECUM_BSW_ERROR_HOOK ) ) && ( ( LINSM_USE_ECUM_BSW_ERROR_HOOK == STD_ON ) || (LINSM_USE_ECUM_BSW_ERROR_HOOK  == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_USE_ECUM_BSW_ERROR_HOOK has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_FINALMAGICNUMBER ) ) && ( ( LINSM_FINALMAGICNUMBER == STD_ON ) || ( LINSM_FINALMAGICNUMBER == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_FINALMAGICNUMBER has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_COMMTOLINSMCHANNEL ) ) && ( ( LINSM_COMMTOLINSMCHANNEL == STD_ON ) || ( LINSM_COMMTOLINSMCHANNEL == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_COMMTOLINSMCHANNEL has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG ) ) && ( ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_ON ) || ( LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_CONFIRMATIONTIMEOUTOFCHANNELCONFIG has to be defined STD_OFF or STD_ON"
#endif

#if !( ( defined ( LINSM_SCHEDULE_END_NOTIFICATION ) ) && ( ( LINSM_SCHEDULE_END_NOTIFICATION == STD_ON ) || ( LINSM_SCHEDULE_END_NOTIFICATION == STD_OFF ) ) )
# error "LinSM configuration issue: LINSM_SCHEDULE_END_NOTIFICATION has to be defined STD_OFF or STD_ON"
#endif

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_LINSM_CSL_CHANNEL_INDEX_ACCESS
\DESCRIPTION Access of ConfirmationTimer, RequestedComMode, WakeUpRetryCounter, ComState and NegativeConfirmation via
             indirection over ChannelConfig structure.
             The 'channel' variable which is used as index is always checked.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID SBSW_LINSM_VERSIONINFO_PTRWRITE
\DESCRIPTION The function LinSM_GetVersionInfo writes to the object referenced by parameter versionInfo.
\COUNTERMEASURE \S The caller must ensure that the pointer passed to the parameter versionInfo references an object of
                   type Std_VersionInfoType. [SMI-16]
                \R A check of versionInfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINSM_MODE_PTRWRITE
\DESCRIPTION The function LinSM_GetCurrentComMode writes to the object referenced by parameter mode.
\COUNTERMEASURE \S The caller must ensure that the pointer passed to the parameter mode references an object of
                   type ComM_ModeType. [SMI-16]
                \R A check of mode against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINSM_LINIFGETTRCVMODE_CALL
\DESCRIPTION Call to LinIf_GetTrcvMode with network handle and pointer to local stack variable curTrcvMode as parameter.
\COUNTERMEASURE \N The passed pointer references the local stack variable curTrcvMode and is therefore always valid.

\ID SBSW_LINSM_COMMMODEINDICATION_CALL
\DESCRIPTION Call to ComM_BusSM_ModeIndication function with network handle and pointer to ComState variable.
\COUNTERMEASURE \N The passed pointer references the modules internal variable ComState via ComStackLib access macro and
                   is therefore always valid.

SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: LinSM.c
 *********************************************************************************************************************/
