
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  LinNm.c
 *    Component:  AUTOSAR LIN Network Management
 *       Module:  AUTOSAR Network Management
 *    Generator:  -
 *
 *  Description:  Implementation of AUTOSAR LIN Network Management
 *                AUTOSAR Release 4
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Pick                  visap         Vector Informatik GmbH
 *  Bastian Molkenthin            visbmo        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-07-16  visap   ESCAN00060143 Creation of component based on FrNm
 *  02.00.00  2013-09-26  visbmo  -             No changes, solely major version increased for R8 due to consistency
 *  02.01.00  2014-12-08  visbmo  ESCAN00080297 FEAT-427: SafeBSW Step I
 *  02.02.00  2015-06-16  visbmo  ESCAN00081785 FEAT-1275: SafeBSW Step 2
 *                                ESCAN00083441 Add support for multiple call contexts
 *  02.02.01  2015-11-04  visbmo  -             Adapted coverage justifications
 *  02.02.02  2016-02-15  visbmo  ESCAN00087803 FEAT-1688: SafeBSW Step 4
 *********************************************************************************************************************/

#define LINNM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "LinNm.h"                                                                 /* Include LIN Network Management */
#include "LinNm_Cbk.h"                                                             /* Include LIN NM callback header */
#include "Nm_Cbk.h"                                                          /* Include NM Interface callback header */
#include "SchM_LinNm.h"                                                 /* Include Header of BSW Scheduler for LinNm */

#if ( LINNM_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"                                                                /* Include Development Error Tracer */
#endif

#if ( LINNM_COM_USER_DATA_SUPPORT == STD_ON )
/* # include "PduR_LinNm.h" this file does currently not exist */
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information check, decimal coded */
#if (  (LINNM_SW_MAJOR_VERSION != (2u)) \
    || (LINNM_SW_MINOR_VERSION != (2u)) \
    || (LINNM_SW_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of LinNm.c and LinNm.h are inconsistent!"
#endif

/* vendor specific version information check, decimal coded */
#if (  (LINNM_CBK_MAJOR_VERSION != (2u)) \
    || (LINNM_CBK_MINOR_VERSION != (2u)) \
    || (LINNM_CBK_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of LinNm.c and LinNm_Cbk.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Instance ID */
#define LINNM_INSTANCE_ID               ((uint8)0x00u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

#if ! defined ( LINNM_LOCAL ) /* COV_LINNM_COMPATIBILITY TX */
# define LINNM_LOCAL                    static
#endif

#if ! defined ( LINNM_LOCAL_INLINE ) /* COV_LINNM_COMPATIBILITY TX */
# define LINNM_LOCAL_INLINE             LOCAL_INLINE
#endif

#if ! defined ( LINNM_DUMMY_STATEMENT ) /* COV_LINNM_COMPATIBILITY XF */
# define LINNM_DUMMY_STATEMENT(c)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define LINNM_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinNm_InitMemory
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC( void, LINNM_CODE ) LinNm_InitMemory( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least i;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set state of each channel to value UNINIT */
  for( i = 0u; i < LinNm_NumberOfLinNmChannels; i++ )
  {
    LinNm_NmState[i] = NM_STATE_UNINIT; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOOP_ACCESS */
  }
}

/**********************************************************************************************************************
 * LinNm_Init
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC( void, LINNM_CODE ) LinNm_Init( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least i;

  /* ----- Implementation ----------------------------------------------- */
  /* Only if LinNm_NumberOfLinNmChannels is undefined or has value 0 initialization is not possible and will generate a compile error */
  /* No need to set NM_STATE_UNINIT */

  /* Initialize each channel. */
  /* #10 Initialize state and mode of each channel to bus sleep state */
  for( i = 0u; i < LinNm_NumberOfLinNmChannels; i++ )
  {
    /* State machine initialization without notification in Bus Sleep. */
    LinNm_NmState[i] = NM_STATE_BUS_SLEEP; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOOP_ACCESS */
    LinNm_NmMode [i] = NM_MODE_BUS_SLEEP; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOOP_ACCESS */
#if ( LINNM_STATE_CHANGE_IND_ENABLED == STD_ON )
    /* no callout because it is not clear if NM is initialized: Nm_StateChangeNotification(i, NM_STATE_UNINIT, NM_STATE_BUS_SLEEP ); */
#endif
  }
}

/***********************************************************************************************************************
 *  LinNm_PassiveStartUp
 **********************************************************************************************************************/
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
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_PassiveStartUp( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                      retVal;
  NetworkHandleType                   channelTmp;
  uint8                               errorId;
  uint8                               stateChanged;

  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINNM_E_NO_ERROR;
#if ( LINNM_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter nmChannelHandle */
  if( nmChannelHandle >= LinNm_NumberOfSystemChannels )
  {
    errorId = LINNM_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    channelTmp = LinNm_SystemToLinNmChannel[nmChannelHandle];

#if ( LINNM_DEV_ERROR_DETECT == STD_ON )
    /* #20 Check if parameter nmChannelHandle references a LinNm channel */
    if ( channelTmp >= (NetworkHandleType)LinNm_NumberOfLinNmChannels )
    {
      errorId = LINNM_E_INVALID_CHANNEL;
    }
    /* #30 Check if the NM is initialized. */
    else if( LinNm_NmState[channelTmp] == NM_STATE_UNINIT )
    {
      errorId = LINNM_E_NO_INIT;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 If network mode is Bus-Sleep, switch to Network mode */
      stateChanged = 0;
      if ( LinNm_NmMode[channelTmp] == NM_MODE_BUS_SLEEP )
      {
        LinNm_NmMode[channelTmp] = NM_MODE_NETWORK; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        /* #50 If network state is Bus-Sleep, switch to Normal Operation mode */
        if ( LinNm_NmState[channelTmp] == NM_STATE_BUS_SLEEP ) /* COV_LINNM_INV_STATE */
        {
          LinNm_NmState[channelTmp] = NM_STATE_NORMAL_OPERATION; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        }
        stateChanged = 1;
      }

      SchM_Exit_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #60 Check if state transition happened: */
      if ( stateChanged == 1 )
      {
        /* #70 If network mode state changed, indicate Network mode to Nm  */
        Nm_NetworkMode( nmChannelHandle );
        /* #80 If network state changed, indicate state change to Nm  */
#if ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON && LINNM_PASSIVE_MODE_ENABLED == STD_OFF)
        Nm_RemoteSleepIndication( nmChannelHandle );
#endif
#if ( LINNM_STATE_CHANGE_IND_ENABLED == STD_ON )
        Nm_StateChangeNotification( nmChannelHandle, NM_STATE_BUS_SLEEP, NM_STATE_NORMAL_OPERATION );
#endif
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINNM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINNM_E_NO_ERROR )
  {
    (void)Det_ReportError( LINNM_MODULE_ID, LINNM_INSTANCE_ID, LINNM_SID_PASSIVESTARTUP, errorId);
  }
#else
  LINNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
/***********************************************************************************************************************
 *  LinNm_NetworkRequest
 **********************************************************************************************************************/
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
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_NetworkRequest( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType      retVal;
  NetworkHandleType   channelTmp;
  uint8               errorId;
  uint8               stateChanged;

  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINNM_E_NO_ERROR;
# if ( LINNM_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter nmChannelHandle */
  if( nmChannelHandle >= LinNm_NumberOfSystemChannels )
  {
    errorId = LINNM_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    channelTmp  = LinNm_SystemToLinNmChannel[nmChannelHandle];

# if ( LINNM_DEV_ERROR_DETECT == STD_ON )
    /* #20 Check if parameter nmChannelHandle references a LinNm channel */
    if ( channelTmp >= (NetworkHandleType)LinNm_NumberOfLinNmChannels)
    {
      errorId = LINNM_E_INVALID_CHANNEL;
    }
    /* #30 Check if the NM is initialized. */
    else if( LinNm_NmState[channelTmp] == NM_STATE_UNINIT )
    {
      errorId = LINNM_E_NO_INIT;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 If network mode is Bus-Sleep, switch to Network mode */
      stateChanged = 0;
      if (LinNm_NmMode[channelTmp] == NM_MODE_BUS_SLEEP)
      {
        LinNm_NmMode[channelTmp] = NM_MODE_NETWORK; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        /* #50 If network state is Bus-Sleep, switch to Normal Operation mode */
        if (LinNm_NmState[channelTmp] == NM_STATE_BUS_SLEEP) /* COV_LINNM_INV_STATE */
        {
          LinNm_NmState[channelTmp] = NM_STATE_NORMAL_OPERATION; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        }
        stateChanged = 1;
      }

      SchM_Exit_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #60 Check if state transition happened: */
      if ( stateChanged == 1 )
      {
        /* #70 If network mode state changed, indicate Network mode to Nm  */
        Nm_NetworkMode( nmChannelHandle );
        /* #80 If network state changed, indicate state change to Nm  */
# if ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
        Nm_RemoteSleepIndication( nmChannelHandle );
# endif
# if ( LINNM_STATE_CHANGE_IND_ENABLED == STD_ON )
        Nm_StateChangeNotification( nmChannelHandle, NM_STATE_BUS_SLEEP, NM_STATE_NORMAL_OPERATION );
# endif
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINNM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINNM_E_NO_ERROR )
  {
    (void)Det_ReportError( LINNM_MODULE_ID, LINNM_INSTANCE_ID, LINNM_SID_NETWORKREQUEST, errorId);
  }
# else
  LINNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinNm_NetworkRelease
 **********************************************************************************************************************/
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
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_NetworkRelease( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType      retVal;
  NetworkHandleType   channelTmp;
  uint8               errorId;
  uint8               stateChanged;

  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINNM_E_NO_ERROR;
# if ( LINNM_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter nmChannelHandle */
  if( nmChannelHandle >= LinNm_NumberOfSystemChannels )
  {
    errorId = LINNM_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    channelTmp  = LinNm_SystemToLinNmChannel[nmChannelHandle];

# if ( LINNM_DEV_ERROR_DETECT == STD_ON )
    /* #20 Check if parameter nmChannelHandle references a LinNm channel */
    if ( channelTmp >= (NetworkHandleType)LinNm_NumberOfLinNmChannels )
    {
      errorId = LINNM_E_INVALID_CHANNEL;
    }
    /* #30 Check if the NM is initialized. */
    else if( LinNm_NmState[channelTmp] == NM_STATE_UNINIT )
    {
      errorId = LINNM_E_NO_INIT;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 If network mode is Network, switch to Bus-Sleep mode */
      stateChanged = 0;
      if (LinNm_NmMode[channelTmp] == NM_MODE_NETWORK)
      {
        LinNm_NmMode[channelTmp] = NM_MODE_BUS_SLEEP; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        /* #50 If network state is Normal Operation mode, switch to Bus-Sleep mode and indicate new state to Nm */
        if (LinNm_NmState[channelTmp] == NM_STATE_NORMAL_OPERATION) /* COV_LINNM_INV_STATE */
        {
          LinNm_NmState[channelTmp] = NM_STATE_BUS_SLEEP; /* SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS */
        }
        stateChanged = 1;
      }

      SchM_Exit_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #60 Check if state transition happened: */
      if ( stateChanged == 1 )
      {
        /* #70 If network mode changed, indicate Bus-Sleep mode to Nm  */
        Nm_BusSleepMode( nmChannelHandle );
        /* #80 If network state changed, indicate state change to Nm  */
# if ( LINNM_STATE_CHANGE_IND_ENABLED == STD_ON )
        Nm_StateChangeNotification( nmChannelHandle, NM_STATE_NORMAL_OPERATION, NM_STATE_BUS_SLEEP );
# endif
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINNM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINNM_E_NO_ERROR )
  {
    (void)Det_ReportError( LINNM_MODULE_ID, LINNM_INSTANCE_ID, LINNM_SID_NETWORKRELEASE, errorId);
  }
# else
  LINNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

/***********************************************************************************************************************
 *  LinNm_GetState
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetState( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                   CONSTP2VAR( Nm_StateType, AUTOMATIC, LINNM_APPL_VAR ) nmStatePtr,
                                                   CONSTP2VAR( Nm_ModeType, AUTOMATIC, LINNM_APPL_VAR ) nmModePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType      retVal;
  NetworkHandleType   channelTmp;
  uint8               errorId;

  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINNM_E_NO_ERROR;
#if ( LINNM_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter nmChannelHandle */
  if( nmChannelHandle >= LinNm_NumberOfSystemChannels )
  {
    errorId = LINNM_E_INVALID_CHANNEL;
  }
  /* #20 Check validity of parameters nmStatePtr and nmModePtr */
  else if( ( nmStatePtr == NULL_PTR ) || ( nmModePtr == NULL_PTR ) )
  {
    errorId = LINNM_E_PARAM_POINTER;
  }
  else
#endif
  {
    channelTmp  = LinNm_SystemToLinNmChannel[nmChannelHandle];

#if ( LINNM_DEV_ERROR_DETECT == STD_ON )
    /* #30 Check if parameter nmChannelHandle references a LinNm channel */
    if ( channelTmp >= (NetworkHandleType)LinNm_NumberOfLinNmChannels )
    {
      errorId = LINNM_E_INVALID_CHANNEL;
    }
    /* #40 Check if the NM is initialized. */
    else if( LinNm_NmState[channelTmp] == NM_STATE_UNINIT )
    {
      errorId = LINNM_E_NO_INIT;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #50 Set given parameters with current channel state and network mode */
      *nmStatePtr = LinNm_NmState[channelTmp]; /* SBSW_LINNM_GETSTATE_PTR */
      *nmModePtr  = LinNm_NmMode[channelTmp]; /* SBSW_LINNM_GETSTATE_PTR */

      SchM_Exit_LinNm_LINNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINNM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINNM_E_NO_ERROR )
  {
    (void)Det_ReportError( LINNM_MODULE_ID, LINNM_INSTANCE_ID, LINNM_SID_GETSTATE, errorId);
  }
#else
  LINNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

#if ( LINNM_USER_DATA_ENABLED == STD_ON )
# if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
#  if ( LINNM_COM_USER_DATA_SUPPORT == STD_OFF )
/***********************************************************************************************************************
 *  LinNm_SetUserData
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_SetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                      CONSTP2CONST( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmUserDataPtr )
{
  LINNM_DUMMY_STATEMENT_CONST(nmUserDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#  endif
# endif /* ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

/***********************************************************************************************************************
 *  LinNm_GetUserData
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                      CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmUserDataPtr ) /* PRQA S 3673 */ /* MD_LIN_NM_16.7 */
{
  LINNM_DUMMY_STATEMENT_CONST(nmUserDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif  /* ( LINNM_USER_DATA_ENABLED == STD_ON ) */

#if ( LINNM_NODE_ID_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_GetNodeIdentifier
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                            CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmNodeIdPtr ) /* PRQA S 3673 */ /* MD_LIN_NM_16.7 */
{
  LINNM_DUMMY_STATEMENT_CONST(nmNodeIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}

/***********************************************************************************************************************
 *  LinNm_GetLocalNodeIdentifier
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetLocalNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                 CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmNodeIdPtr ) /* PRQA S 3673 */ /* MD_LIN_NM_16.7 */
{
  LINNM_DUMMY_STATEMENT_CONST(nmNodeIdPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif /* ( LINNM_NODE_ID_ENABLED  == STD_ON ) */

#if ( LINNM_NODE_DETECTION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_RepeatMessageRequest
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_RepeatMessageRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif /* ( LINNM_NODE_DETECTION_ENABLED == STD_ON ) */

#if ( ( LINNM_NODE_ID_ENABLED == STD_ON ) || ( LINNM_USER_DATA_ENABLED == STD_ON ) )
/***********************************************************************************************************************
 *  LinNm_GetPduData
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_GetPduData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                     CONSTP2VAR( uint8, AUTOMATIC, LINNM_APPL_VAR ) nmPduDataPtr ) /* PRQA S 3673 */ /* MD_LIN_NM_16.7 */
{
  LINNM_DUMMY_STATEMENT_CONST(nmPduDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif /* ( LINNM_NODE_ID_ENABLED  == STD_ON ) || ( LINNM_USER_DATA_ENABLED == STD_ON ) */

#if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_RequestBusSynchronization
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_RequestBusSynchronization( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle )
{
  LINNM_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif /* ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) */

#if ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_CheckRemoteSleepIndication
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_CheckRemoteSleepIndication( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                                     CONSTP2VAR( boolean, AUTOMATIC, LINNM_APPL_VAR ) nmRemoteSleepIndPtr )
{
  LINNM_DUMMY_STATEMENT_CONST(nmRemoteSleepIndPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif /* ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON ) */

#if ( LINNM_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinNm_GetVersionInfo
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, LINNM_CODE ) LinNm_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, LINNM_APPL_VAR ) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;

  errorId = LINNM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINNM_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter versioninfo. Note: no uninit check is performed */
  if (versioninfo == NULL_PTR)
  {
    errorId = LINNM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo parameter with component information */
    versioninfo->vendorID = (uint16) LINNM_VENDOR_ID; /* SBSW_LINNM_VERSIONINFO_PTR */
    versioninfo->moduleID = (uint16) LINNM_MODULE_ID; /* SBSW_LINNM_VERSIONINFO_PTR */
    versioninfo->sw_major_version = (uint8)LINNM_SW_MAJOR_VERSION; /* SBSW_LINNM_VERSIONINFO_PTR */
    versioninfo->sw_minor_version = (uint8)LINNM_SW_MINOR_VERSION; /* SBSW_LINNM_VERSIONINFO_PTR */
    versioninfo->sw_patch_version = (uint8)LINNM_SW_PATCH_VERSION; /* SBSW_LINNM_VERSIONINFO_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINNM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINNM_E_NO_ERROR )
  {
    (void)Det_ReportError( LINNM_MODULE_ID, LINNM_INSTANCE_ID, LINNM_SID_GETVERSIONINFO, errorId);
  }
# else
  LINNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
}
#endif  /* ( LINNM_VERSION_INFO_API == STD_ON ) */

#if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
/***********************************************************************************************************************
 *  LinNm_TxConfirmation
 **********************************************************************************************************************/
/* No caller specified: FUNC( void, LINNM_CODE ) LinNm_TxConfirmation( VAR( PduIdType, AUTOMATIC ) TxPduId )          */
/* { */
/* } */
#endif /* ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

#if ( LINNM_COM_USER_DATA_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinNm_Transmit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_Transmit( VAR( PduIdType, AUTOMATIC ) LinTxPduId,
                                                   P2CONST( PduInfoType, AUTOMATIC, LINNM_APPL_VAR ) PduInfoPtr)
{
  LINNM_DUMMY_STATEMENT(LinTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_NOT_OK;
}
#endif

#if ( LINNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinNm_SetSleepReadyBit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_SetSleepReadyBit( CONST( NetworkHandleType, AUTOMATIC ) nmChannelHandle,
                                                           CONST( boolean, AUTOMATIC ) nmSleepReadyBit)
{
  LINNM_DUMMY_STATEMENT_CONST(nmSleepReadyBit); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINNM_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif

#if ( LINNM_COM_CONTROL_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  LinNm_EnableCommunication
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_EnableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}

/***********************************************************************************************************************
 *  LinNm_DisableCommunication
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, LINNM_CODE ) LinNm_DisableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  LINNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Empty implementation, just return E_OK */
  return E_OK;
}
#endif

#define LINNM_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*******************************************************************************
  Verify the configuration of the LIN Network Management
*******************************************************************************/

/* Check Error Detection / Diagnostic switches */
#if ( defined LINNM_DEV_ERROR_DETECT ) && ( ( LINNM_DEV_ERROR_DETECT == STD_OFF ) || ( LINNM_DEV_ERROR_DETECT == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Development Error Detection Enabled has to be defined STD_OFF or STD_ON"
#endif

/* Check Channel Configuration switch */
#if ( defined LinNm_NumberOfLinNmChannels ) && ( LinNm_NumberOfLinNmChannels > 0u ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Number of LIN Channels has to be defined greater 0."
#endif

#if ( defined LinNm_NumberOfSystemChannels ) && ( LinNm_NumberOfSystemChannels > 0u ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Number of system channels has to be defined greater 0."
#endif

/* Check LIN Nm Configuration switches */
#if ( defined LINNM_VERSION_INFO_API ) && ( ( LINNM_VERSION_INFO_API == STD_OFF ) || ( LINNM_VERSION_INFO_API == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Version Info Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_NODE_DETECTION_ENABLED ) && ( ( LINNM_NODE_DETECTION_ENABLED == STD_OFF ) || ( LINNM_NODE_DETECTION_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Node Detection Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_REMOTE_SLEEP_INDICATION_ENABLED ) && ( ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_OFF ) || ( LINNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Remote Sleep Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_USER_DATA_ENABLED ) && ( ( LINNM_USER_DATA_ENABLED == STD_OFF ) || ( LINNM_USER_DATA_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: User Data Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_BUS_SYNCHRONIZATION_ENABLED ) && ( ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_OFF ) || ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Bus Synchronization Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_PASSIVE_MODE_ENABLED ) && ( ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF ) || ( LINNM_PASSIVE_MODE_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Passive Mode Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_NODE_ID_ENABLED ) && ( ( LINNM_NODE_ID_ENABLED  == STD_OFF ) || ( LINNM_NODE_ID_ENABLED  == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: Node Identifier Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined LINNM_STATE_CHANGE_IND_ENABLED ) && ( ( LINNM_STATE_CHANGE_IND_ENABLED == STD_OFF ) || ( LINNM_STATE_CHANGE_IND_ENABLED == STD_ON ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: State Change Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if ( defined ( LINNM_COM_USER_DATA_SUPPORT ) && ( ( LINNM_COM_USER_DATA_SUPPORT == STD_OFF ) || ( LINNM_COM_USER_DATA_SUPPORT == STD_ON ) ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: User Data Support via Com has to be defined STD_OFF or STD_ON"
#endif
#if ( defined ( LINNM_SYNCHRONIZATIONPOINT_ENABLED ) && ( LINNM_SYNCHRONIZATIONPOINT_ENABLED == STD_OFF ) ) /* COV_LINNM_CONFIG */
#else
# error: "LIN NM configuration issue: LINNM_SYNCHRONIZATIONPOINT_ENABLED shall always be disabled."
#endif

#if ( LINNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Check configuration of Node Detection */
# if ( LINNM_NODE_DETECTION_ENABLED == STD_ON ) && ( LINNM_NODE_ID_ENABLED == STD_OFF )
#  error: "LIN NM configuration issue: LINNM_NODE_DETECTION_ENABLED cannot be activated if LINNM_NODE_ID_ENABLED is STD_OFF."
# endif
#endif

#if ( LINNM_PASSIVE_MODE_ENABLED == STD_ON )
# if ( LINNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON )
#  error: "LIN NM LINNM_BUS_SYNCHRONIZATION_ENABLED must not be enabled when LINNM_PASSIVE_MODE_ENABLED is enabled."
# endif
#endif

/* Module specific MISRA deviations:

   MD_LIN_NM_16.7:
      Reason: The interface is specified by AUTOSAR.
      Risk: Unintentional modification of the object addressed by the pointer.
      Prevention: Code inspection and test of the different variants in the component test.

*/

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_LINNM_CHANNELSIZE_ARRAY_LOOP_ACCESS
    \DESCRIPTION Array write access to an array with size equal to the number of LinNm channels with the loop variable i.
                 The array is accessed by the loop variable ranging from 0 to LinNm_NumberOfLinNmChannels - 1.
                 The array is is declared with the same define LinNm_NumberOfLinNmChannels.
    \COUNTERMEASURE \N The for loop ensures that i never exceeds the size of the array.

\ID SBSW_LINNM_CHANNELSIZE_ARRAY_LOCALVAR_ACCESS
    \DESCRIPTION Array write access to an array with size equal to define LinNm_NumberOfLinNmChannels with the local variable channelTmp.
                 The array is accessed by a local variable containing an element of array LinNm_SystemToLinNmChannel.
    \COUNTERMEASURE \N A runtime checks ensures that the local variable is in range 0 to LinNm_NumberOfLinNmChannels - 1, so the index is always in range.

\ID SBSW_LINNM_GETSTATE_PTR
    \DESCRIPTION The function LinNm_GetState() writes to the objects referenced by parameters nmStatePtr and nmModePtr.
    \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameters <nmStatePtr> and <nmModePtr> are valid.

\ID SBSW_LINNM_VERSIONINFO_PTR
    \DESCRIPTION The function LinNm_GetVersionInfo() writes to the object referenced by parameter versioninfo.
    \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter <versioninfo> is valid.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_LINNM_CONFIG
  \ACCEPT XF xf xf xf
  \ACCEPT XF tx tf tf
  \ACCEPT XF tx tx
  \REASON [COV_MSR_CONFIG]

\ID COV_LINNM_INV_STATE
  \ACCEPT TX
  \REASON [COV_MSR_INV_STATE]

\ID COV_LINNM_COMPATIBILITY
  \REASON [COV_MSR_COMPATIBILITY]

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: LinNm.c
 *********************************************************************************************************************/
