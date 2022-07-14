 /**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
 /*!        \file  FrNm.c
 *         \brief  AUTOSAR FlexRay Network Management
 *
 *       \details  Source of AUTOSAR FlexRay Network Management
 *                 AUTOSAR Release 4.0
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

/* PRQA S 0857 EOF */ /* MD_MSR_1.1 */
/* PRQA S 0779 EOF */ /* MD_MSR_5.1 */
#define FRNM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "FrNm.h"                                                              /* Include FlexRay Network Management */
#include "FrNm_Cbk.h"                                                          /* Include FlexRay NM callback header */
#include "Nm_Cbk.h"                                                          /* Include NM Interface callback header */
#include "FrIf.h"                                                                       /* Include FlexRay Interface */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"                                                                /* Include Development Error Tracer */
#endif

#include "SchM_FrNm.h"                                             /* Include Scheduler Module for Critical Sections */

#if ( FRNM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
# include "EcuM_Error.h"                                            /* ESCAN00064310 */ /* Include EcuM Error Header */
#endif

#if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) || defined ( FRNM_PN_CALC_FEATURE_ENABLED )
# include "PduR_FrNm.h"
#endif

#if ( FRNM_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
# include "Rtm.h"                                                /* ESCAN00067702 */ /* Include Run Time Measurement */
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* vendor specific version information is BCD coded */
/* Check the version of FrNm header file */
#if (  (FRNM_SW_MAJOR_VERSION != (6U)) \
    || (FRNM_SW_MINOR_VERSION != (1U)) \
    || (FRNM_SW_PATCH_VERSION != (1U)) )
# error "Vendor specific version numbers of FrNm.c and FrNm.h are inconsistent!"
#endif

/* Check the version of FrNm callback header file */
#if (  (FRNM_CBK_MAJOR_VERSION != (6U)) \
    || (FRNM_CBK_MINOR_VERSION != (1U)) \
    || (FRNM_CBK_PATCH_VERSION != (1U)) )
# error "Vendor specific version numbers of FrNm.c and FrNm_Cbk.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (FRNM_CFG_MAJOR_VERSION != (6U)) \
    || (FRNM_CFG_MINOR_VERSION != (1U)) )
# error "Vendor specific version numbers of FrNm.c and FrNm_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Definition of channel handles */
#if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
# define FRNM_CHANNEL_IDX                         (0x00U)
#else
# define FRNM_CHANNEL_IDX                         channel
#endif

#if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
# define FRNM_CHANNEL_TYPE_ONLY                   void /* PRQA S 3460 */ /* MD_MSR_19.4 */
# define FRNM_CHANNEL_TYPE_FIRST
# define FRNM_CHANNEL_PARA_ONLY
# define FRNM_CHANNEL_PARA_FIRST
#else
# define FRNM_CHANNEL_TYPE_ONLY                   CONST( NetworkHandleType, AUTOMATIC ) channel
# define FRNM_CHANNEL_TYPE_FIRST                  CONST( NetworkHandleType, AUTOMATIC ) channel,
# define FRNM_CHANNEL_PARA_ONLY                   FRNM_CHANNEL_IDX
# define FRNM_CHANNEL_PARA_FIRST                  FRNM_CHANNEL_IDX,
#endif

#if ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON )
# define FRNM_TX_DATA_PDU_INDIRECTION_INDEX       txPduIndirectionIndex
# define FRNM_TX_VOTE_PDU_INDIRECTION_INDEX       txPduIndirectionIndex
#else
# define FRNM_TX_DATA_PDU_INDIRECTION_INDEX       FrNm_GetTxDataPduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)
# define FRNM_TX_VOTE_PDU_INDIRECTION_INDEX       FrNm_GetTxVotePduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)
#endif

/* Internal constant definitions */
/*! Repeat Message Masks */
#define FRNM_REPEAT_MSG_BIT_MASK                  (0x01U)
#define FRNM_REPEAT_MSG_BIT_INV_MASK              (0xFEU)

/*! Instance ID */
#define FRNM_INSTANCE_ID                          (0x00U)

/*! Restart Flags */
#define FRNM_NO_RESTART                           (0U)
#define FRNM_PASSIVE_RESTART                      (1U)
#define FRNM_ACTIVE_RESTART                       (2U)

/*! Vote Bit Masks */
#define FRNM_VOTE_BIT_MASK                        (0x80U)
#define FRNM_VOTE_BIT_INV_MASK                    (0x7FU)

/*! Active Wakeup Bit Masks */
#define FRNM_ACT_WU_BIT_MASK                      (0x10U)
#define FRNM_ACT_WU_BIT_INV_MASK                  (0xEFU)

/*! PN CRI Bit Masks */
#define FRNM_PN_CRI_BIT_MASK                      (0x40U)

/*! Message Transmission Ability is enabled / disabled */
#define MSG_TX_DISABLED                           (0x00U)
#define MSG_TX_ENABLED                            (0x01U)

#define FRNM_COORD_ASLEEP_OR_NOT_READY_TO_SLEEP   (0x00U)

#define FRNM_VOTE_POSITION                        (0x00U)

#if !defined ( FRNM_LOCAL ) /* COV_MSR_COMPATIBILITY */
# define FRNM_LOCAL static
#endif

#if !defined ( FRNM_LOCAL_INLINE ) /* COV_MSR_COMPATIBILITY */
# define FRNM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* ESCAN00067702 */
#if ( FRNM_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
/* PRQA S 0342,3109,3458 6 */ /* MD_FrNm_0342,MD_MSR_14.3,MD_MSR_19.4 */
# define FrNm_Rtm_Start( measurement )  { \
                                         Rtm_Start(RtmConf_##measurement); \
                                        }
# define FrNm_Rtm_Stop( measurement )   { \
                                         Rtm_Stop(RtmConf_##measurement); \
                                        }
#else
# define FrNm_Rtm_Start( measurement )
# define FrNm_Rtm_Stop( measurement )
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#if ( FRNM_USE_INIT_POINTER == STD_ON )
# define FRNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h"    /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Pointer to the current configuration */
FRNM_LOCAL P2CONST(FrNm_ConfigType, FRNM_VAR_ZERO_INIT, FRNM_INIT_DATA) FrNm_ConfigDataPtr = NULL_PTR;

# define FRNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h"    /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define FRNM_START_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* FlexRay NM version */
CONST(uint8, FRNM_CONST) FrNm_MainVersion      = (uint8)FRNM_SW_MAJOR_VERSION;
CONST(uint8, FRNM_CONST) FrNm_SubVersion       = (uint8)FRNM_SW_MINOR_VERSION;
CONST(uint8, FRNM_CONST) FrNm_ReleaseVersion   = (uint8)FRNM_SW_PATCH_VERSION;

#define FRNM_STOP_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define FRNM_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * FrNm_LocalInit()
 *********************************************************************************************************************/
/*! \brief       Initialization of internal variables
 *  \details     This function initializes the FrNm's internal variables
 *  \param[in]   channel         - Local channel handle
 *  \pre         -
 *  \note        Called by FrNm_Init
 *  \trace       SPEC-15891, SPEC-16219
 *********************************************************************************************************************/
FRNM_LOCAL FUNC( void, FRNM_CODE ) FrNm_LocalInit( FRNM_CHANNEL_TYPE_ONLY );

#if defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/**********************************************************************************************************************
  FrNm_PnLocal_Init()
**********************************************************************************************************************/
/*! \brief       Initialization of Partial Networking relevant variables
 *  \details     This function initializes the FrNm's Partial Networking related variables
 *  \pre         -
 *  \note        Called by FrNm_Init
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_PnLocal_Init( void );
#endif

#if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
/**********************************************************************************************************************
  FrNm_PnEraCalculation()
**********************************************************************************************************************/
/*! \brief       Process received Partial Networking ERA information
 *  \details     -
 *  \param[in]   channel          - Local channel handle
 *  \pre         -
 *  \note        Called by FrNm_LocalMainFunction
 *  \trace       SPEC-16089, SPEC-15988, SPEC-15871, SPEC-15916, SPEC-15985, SPEC-16139, SPEC-63412
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_PnEraCalculation( FRNM_CHANNEL_TYPE_ONLY );
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_CONTROL_ENABLED == STD_ON )
# if( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
    ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_7 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf xf xf */
/**********************************************************************************************************************
  FrNm_CommunicationControlHandling()
**********************************************************************************************************************/
/*! \brief      Process Communication Control requests
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_CommunicationControlHandling( FRNM_CHANNEL_TYPE_ONLY );
# endif
#endif

/**********************************************************************************************************************
  FrNm_StartupFromBusSleep()
**********************************************************************************************************************/
/*! \brief      Process the startup from Bus Sleep Mode
 *  \details    Startup is triggered by either a message reception or by a call of FrNm_PassiveStartup or
 *              FrNm_NetworkRequest.
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-16095, SPEC-50131, SPEC-16214, SPEC-16096, SPEC-50086
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_StartupFromBusSleep( FRNM_CHANNEL_TYPE_ONLY );

# if ( FRNM_PDU_SCHEDULE_VARIANT_ONLY == 0U ) || \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY > 2u ) || \
     ( ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 7U ) && \
       ( ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) || \
         ( ( FRNM_PDU_SCHEDULE_VARIANT_3 == STD_ON ) || \
           ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
           ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) ) ) ) ) && \
           ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY <= 1U ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf tx tf xf xf tf tf tf */
/**********************************************************************************************************************
  FrNm_CopyRxVoteData()
**********************************************************************************************************************/
/*! \brief      Process Data received in last NM message
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \param[in]  FrNmRxPduId      - ID of FR NM PDU that has been received
 *  \param[in]  controlBitVector - Content of the received Control Bit Vector Byte.
 *  \pre        -
 *  \note       Called by FrNm_RxIndication
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CopyRxVoteData( FRNM_CHANNEL_TYPE_FIRST  VAR( PduIdType, AUTOMATIC ) FrNmRxPduId, CONST( uint8, AUTOMATIC ) controlBitVector );
#endif

/**********************************************************************************************************************
  FrNm_CopyRxMessageData()
**********************************************************************************************************************/
/*! \brief      Process Data received in last NM message
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \param[in]  PduInfoDataPtr   - Pointer to FR PDU related data containing the FR SDU buffer.
 *  \pre        -
 *  \note       Called by FrNm_RxIndication
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CopyRxMessageData( FRNM_CHANNEL_TYPE_FIRST  CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) PduInfoDataPtr );

#if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
  FrNm_CoordReadyToSleepHandling()
**********************************************************************************************************************/
/*! \brief      Process CoordinatorReadyToSleep Indication
 *  \details    Evaluates the Sleep Ready Bit in the last received Nm message.
 *  \param[in]  channel          - Local channel handle
 *  \param[in]  controlBitVector - Currently received Control Bit Vector byte
 *  \pre        -
 *  \note       Called by FrNm_RxIndication
 *  \trace      SPEC-50095, SPEC-2622869
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CoordReadyToSleepHandling( FRNM_CHANNEL_TYPE_FIRST  CONST( uint8, AUTOMATIC ) controlBitVector);
#endif

#if ( FRNM_PN_FEATURE_ENABLED == STD_ON ) && defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/**********************************************************************************************************************
  FrNm_RxPnFilterHandling()
**********************************************************************************************************************/
/*! \brief      Process Partial Networking information
 *  \details    Filter received Partial Network Cluster requests and store remaining request for further processing.
 *  \param[in]  channel          - Local channel handle
 *  \param[in]  PduInfoPtr       - Pointer to FR PDU related data containing the FR SDU buffer.
 *  \pre        -
 *  \note       Called by FrNm_RxIndication
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_RxPnFilterHandling( FRNM_CHANNEL_TYPE_FIRST  CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) PduInfoDataPtr );
#endif

#if ( FRNM_CARWAKEUPRXENABLEDOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
  FrNm_CarWakeupIndication()
**********************************************************************************************************************/
/*! \brief      Process Car Wakeup Indication
 *  \details    Evaluates the Car Wakeup Bit in the last received Nm message.
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_RxIndication
 *  \trace      SPEC-50092
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CarWakeupIndication( FRNM_CHANNEL_TYPE_ONLY );
#endif

/**********************************************************************************************************************
  FrNm_IndicationAndVoteHandling()
**********************************************************************************************************************/
/*! \brief      Process received indications and vote information.
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-16179, SPEC-15990
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_IndicationAndVoteHandling( FRNM_CHANNEL_TYPE_ONLY );

/**********************************************************************************************************************
  FrNm_TransitionToNetworkMode()
**********************************************************************************************************************/
/*! \brief      Process the transition to Network Mode
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-15942, SPEC-50118, SPEC-16011
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNetworkMode( FRNM_CHANNEL_TYPE_ONLY );

/**********************************************************************************************************************
  FrNm_TransitionToNormalOperationOrReadySleep()
**********************************************************************************************************************/
/*! \brief      Process the transition to state Normal Operation or Ready Sleep depending on the local communication request state
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \param[in]  commRequested    -
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-50073, SPEC-50078, SPEC-50063
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNormalOperationOrReadySleep( FRNM_CHANNEL_TYPE_FIRST  CONST( uint8, AUTOMATIC ) commRequested );

/**********************************************************************************************************************
  FrNm_TransitionToBusSleepMode()
**********************************************************************************************************************/
/*! \brief      Process the transition to Bus Sleep Mode
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-50119, SPEC-15921, SPEC-15976, SPEC-50146, SPEC-50075
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToBusSleepMode( FRNM_CHANNEL_TYPE_ONLY );

#if ( FRNM_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
  FrNm_TransitionToRepeatMessage()
**********************************************************************************************************************/
/*! \brief      Process the transition to Repeat Message state
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-15919, SPEC-50080
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToRepeatMessage( FRNM_CHANNEL_TYPE_ONLY );
#endif

#if( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
  FrNm_TransitionToNormalOperation()
**********************************************************************************************************************/
/*! \brief      Process the transition to NormalOperation state
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-15888, SPEC-50068
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNormalOperation( FRNM_CHANNEL_TYPE_ONLY );
#endif

/**********************************************************************************************************************
  FrNm_TransitionToReadySleep()
**********************************************************************************************************************/
/*! \brief      Process the transition to ReadySleep state
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-15873
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToReadySleep( FRNM_CHANNEL_TYPE_ONLY );

#if ( FRNM_TRANSMITSEPARATEVOTEOFCHANNELCONFIG == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
/**********************************************************************************************************************
  FrNm_TransmitVotePdu()
**********************************************************************************************************************/
/*! \brief      Process the transmission of Vote PDU
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-16151, SPEC-16141
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransmitVotePdu( FRNM_CHANNEL_TYPE_ONLY );
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
  FrNm_TransmitDataPdu()
**********************************************************************************************************************/
/*! \brief      Process the transmission of Data PDU
 *  \details    -
 *  \param[in]  channel          - Local channel handle
 *  \pre        -
 *  \note       Called by FrNm_LocalMainFunction
 *  \trace      SPEC-16141, SPEC-50109, SPEC-2622868, SPEC-16148
 *********************************************************************************************************************/
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransmitDataPdu( FRNM_CHANNEL_TYPE_ONLY );
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/**********************************************************************************************************************
  FrNm_PnLocal_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_PnLocal_Init( void )
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
  uint8_least channel;
# endif
  uint8_least pnClusterResetTimerIndex;
  uint8_least pnClusterRequestByte;

  /* ----- Implementation ----------------------------------------------- */
# if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
  for( channel = 0U; channel < FrNm_GetSizeOfChannelConfig(); channel++ )
  {
    /* #10 Initialize ERA related variables */
#  if ( FRNM_ISDEF_PNERACALCENABLEDOFCHANNELCONFIG == STD_OFF )
    if( FrNm_IsPnEraCalcEnabledOfChannelConfig(channel) )
#  endif
    {
      for( pnClusterRequestByte = FrNm_GetPbPncClusterConfigExtStartIdxOfPbChannelConfig(channel);
           pnClusterRequestByte < FrNm_GetPbPncClusterConfigExtEndIdxOfPbChannelConfig(channel);
           pnClusterRequestByte++ )
      {
        FrNm_SetPnClusterReqExt(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_02 */
        FrNm_SetPnClusterReqExtNew(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_02 */

        for( pnClusterResetTimerIndex = FrNm_GetPnClusterResetTimerExtStartIdxOfPbPncClusterConfigExt(pnClusterRequestByte);
             pnClusterResetTimerIndex < FrNm_GetPnClusterResetTimerExtEndIdxOfPbPncClusterConfigExt(pnClusterRequestByte);
             pnClusterResetTimerIndex++ )
        {
          FrNm_SetPnClusterResetTimerExt(pnClusterResetTimerIndex, 0U); /* SBSW_FRNM_CSL_03 */
        }
      }
    }
  }
# endif
# if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
  /* #20 Initialize EIRA related variables */
  for( pnClusterRequestByte = 0U; pnClusterRequestByte < FrNm_GetSizeOfPnClusterReq(); pnClusterRequestByte++ )
  {
    FrNm_SetPnClusterReq(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_01 */
    FrNm_SetPnClusterReqNew(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_02 */

    for( pnClusterResetTimerIndex = FrNm_GetPnClusterResetTimerStartIdxOfPbPncClusterConfig(pnClusterRequestByte);
         pnClusterResetTimerIndex < FrNm_GetPnClusterResetTimerEndIdxOfPbPncClusterConfig(pnClusterRequestByte);
         pnClusterResetTimerIndex++)
    {
      FrNm_SetPnClusterResetTimer(pnClusterResetTimerIndex, 0U); /* SBSW_FRNM_CSL_03 */
    }
  }
# endif
}
#endif

#if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
/**********************************************************************************************************************
  FrNm_PnEiraCalculation()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \trace SPEC-16113, SPEC-15908, SPEC-16004, SPEC-16169, SPEC-63412
 */
FUNC( void, FRNM_CODE ) FrNm_PnEiraCalculation( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least pnClusterRequestByte;
  uint8_least pnClusterRequestBit;
  uint8_least pnClusterResetTimerIndex;
  FrNm_PnClusterReqNewType pnClusterRequestNew;
  boolean pnClusterRequestChanged;
  PduInfoType nmPnInfo;

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if( FrNm_IsModuleInitialized() )
# endif
  {
  /*
   * #20 If PnEiraCalcEnabled is enabled and PnEnabled is enabled on at least one channel
   */
    if ( FrNm_IsPnEnabledInVariant() ) /*lint !e506 */ /* COV_FRNM_PNFEATUREENABLED */
    {
  /* ----- Implementation ----------------------------------------------- */
# if ( FRNM_ISDEF_PNEIRACALCENABLED == STD_OFF )
      if ( FrNm_IsPnEiraCalcEnabled() ) /* COV_FRNM_PNFEATUREENABLED */
# endif
      {
        /* #30 Iterate over all Partial Networking Cluster Bytes
         *      If received Nm message contains new or repeated requests
         *       Copy PNC requests to local buffer to avoid long interrupt locks
         *       Clear request buffer to detect changes in next received NM message
         *       If received PNC Byte contains new EIRA requests.
         *        Update local EIRA requests
         *       Iterate over all PNC Bits in the current PNC Byte (and corresponding PnResetTimer)
         *        If PNC Bit is set
         *         Restart PnClusterResetTimer of the requested PNC with PnResetTime
         *          If PnClusterResetTimer of PNC Bit in PNC Byte has not elapsed
         *           Decrement PnClusterResetTimer by one
         *           If PnClusterResetTimer has elapsed
         *            Reset the local PNC request
         */

        pnClusterRequestChanged = FALSE;

        for( pnClusterRequestByte = 0U; pnClusterRequestByte < FrNm_GetSizeOfPnClusterReqNew(); pnClusterRequestByte++ )
        {
          pnClusterRequestNew = 0U;
          if( FrNm_GetPnClusterReqNew(pnClusterRequestByte) != 0U )
          {
            SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            pnClusterRequestNew = FrNm_GetPnClusterReqNew(pnClusterRequestByte);
            FrNm_SetPnClusterReqNew(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_01 */
            SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            pnClusterRequestBit = (uint8)(FrNm_GetPnClusterReq(pnClusterRequestByte) | pnClusterRequestNew);

            if( pnClusterRequestBit != FrNm_GetPnClusterReq(pnClusterRequestByte))
            {
              pnClusterRequestChanged = TRUE;
              FrNm_SetPnClusterReq(pnClusterRequestByte, pnClusterRequestBit); /* SBSW_FRNM_CSL_02 */
            }
          }

          /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
          for( pnClusterResetTimerIndex = FrNm_GetPnClusterResetTimerStartIdxOfPbPncClusterConfig(pnClusterRequestByte), pnClusterRequestBit = 0U;
               pnClusterResetTimerIndex < FrNm_GetPnClusterResetTimerEndIdxOfPbPncClusterConfig(pnClusterRequestByte);
               pnClusterResetTimerIndex++, pnClusterRequestBit++ )
          {
            if( (pnClusterRequestNew & (FrNm_PnClusterReqNewType)(1U << pnClusterRequestBit)) != 0U )
            {
              FrNm_SetPnClusterResetTimer(pnClusterResetTimerIndex, FrNm_GetPnResetTimeOfPbChannelConfig(0U)); /* SBSW_FRNM_CSL_03 */
            }
            if( FrNm_GetPnClusterResetTimer(pnClusterResetTimerIndex) != 0U )
            {
              FrNm_DecPnClusterResetTimer(pnClusterResetTimerIndex); /* SBSW_FRNM_CSL_03 */
              if( FrNm_GetPnClusterResetTimer(pnClusterResetTimerIndex) == 0U )
              {
                pnClusterRequestChanged = TRUE;
                FrNm_SetPnClusterReq(pnClusterRequestByte, (FrNm_PnClusterReqType)  /* SBSW_FRNM_CSL_02 */
                                      (FrNm_GetPnClusterReq(pnClusterRequestByte) & (FrNm_PnClusterReqType)(~(1U << pnClusterRequestBit))));
              }
            }
          }
        }
        /* #40 If an PNC EIRA request has changed, indicate this to the ComM (via PduR -> Com <- ComM)
         *      Pass the changed PnClusterRequest buffer to the Com by calling PduR_FrNmRxIndication
         */
        if( pnClusterRequestChanged == TRUE )
        {
          nmPnInfo.SduDataPtr = FrNm_GetAddrPnClusterReq(0U);
          nmPnInfo.SduLength = FrNm_GetPnInfoLength();
          PduR_FrNmRxIndication( FrNm_GetPnEiraRxPduId(), &nmPnInfo ); /* SBSW_FRNM_PDURCANNMRXINDICATION */
        }
      }
    }
  }
} /* PRQA S 6080,6030 */ /* MD_MSR_STMIF,MD_MSR_STCYC  */
#endif /* FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON */

#if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
/**********************************************************************************************************************
  FrNm_PnEraCalculation()
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
 *
 *
 *
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_PnEraCalculation( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least pnClusterRequestByte;
  uint8_least pnClusterRequestBit;
  uint8_least pnClusterResetTimerExtIndex;
  FrNm_PnClusterReqExtNewType pnClusterRequestExtNew;
  boolean pnClusterRequestChanged;
  PduInfoType nmPnInfo;

  /* ----- Implementation ----------------------------------------------- */
# if( FRNM_ISDEF_PNENABLEDOFPBCHANNELCONFIG == STD_OFF )
  if( FrNm_IsPnEnabledOfPbChannelConfig(FRNM_CHANNEL_IDX) )
# endif
  {
# if( FRNM_ISDEF_PNERACALCENABLEDOFCHANNELCONFIG == STD_OFF )
    if ( FrNm_IsPnEraCalcEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
# endif
    {
      /* #10 Iterate over all Partial Networking Cluster Bytes related to the current channel
       *      If received Nm message contains new or repeated requests
       *       Copy PNC requests to local buffer to avoid long interrupt locks
       *       Clear request buffer to detect changes in next received NM message
       *       If received PNC Byte contains new ERA requests.
       *        Update local ERA requests
       *       Iterate over all PNC Bits in the current PNC Byte (and corresponding PnResetTimer)
       *        If PNC Bit is set
       *         Restart PnClusterResetTimerExt of the requested PNC with PnResetTime
       *          If PnClusterResetTimerExt of PNC Bit in PNC Byte has not elapsed
       *           Decrement PnClusterResetTimerExt by one
       *           If PnClusterResetTimerExt has elapsed
       *            Reset the local PNC request
       */
      pnClusterRequestChanged = FALSE;

      for( pnClusterRequestByte = FrNm_GetPbPncClusterConfigExtStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
           pnClusterRequestByte < FrNm_GetPbPncClusterConfigExtEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
           pnClusterRequestByte++ )
      {
        pnClusterRequestExtNew = 0U;
        /* Check for new / repeated cluster requests */
        if( FrNm_GetPnClusterReqExtNew(pnClusterRequestByte) != 0U )
        {

          SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          pnClusterRequestExtNew = FrNm_GetPnClusterReqExtNew(pnClusterRequestByte);
          FrNm_SetPnClusterReqExtNew(pnClusterRequestByte, 0U); /* SBSW_FRNM_CSL_02 */
          SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          pnClusterRequestBit = (uint8)(FrNm_GetPnClusterReqExt(pnClusterRequestByte) | pnClusterRequestExtNew);
          if( pnClusterRequestBit != FrNm_GetPnClusterReqExt(pnClusterRequestByte))
          {
            pnClusterRequestChanged = TRUE;
            FrNm_SetPnClusterReqExt(pnClusterRequestByte, pnClusterRequestBit); /* SBSW_FRNM_CSL_02 */
          }
        }

        /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
        for( pnClusterResetTimerExtIndex = FrNm_GetPnClusterResetTimerExtStartIdxOfPbPncClusterConfigExt(pnClusterRequestByte), pnClusterRequestBit = 0U;
              pnClusterResetTimerExtIndex < FrNm_GetPnClusterResetTimerExtEndIdxOfPbPncClusterConfigExt(pnClusterRequestByte);
              pnClusterResetTimerExtIndex++, pnClusterRequestBit++ )
        {
          if( (pnClusterRequestExtNew & (FrNm_PnClusterReqExtNewType)(1U << pnClusterRequestBit)) != 0U )
          {
            FrNm_SetPnClusterResetTimerExt(pnClusterResetTimerExtIndex, FrNm_GetPnResetTimeOfPbChannelConfig(0U)); /* SBSW_FRNM_CSL_03 */
          }
          if( FrNm_GetPnClusterResetTimerExt(pnClusterResetTimerExtIndex) != 0U )
          {
            FrNm_DecPnClusterResetTimerExt(pnClusterResetTimerExtIndex); /* SBSW_FRNM_CSL_03 */
            if( FrNm_GetPnClusterResetTimerExt(pnClusterResetTimerExtIndex) == 0U )
            {
              pnClusterRequestChanged = TRUE;
              /* Clear bit by masking with inverted bit */
              FrNm_SetPnClusterReqExt(pnClusterRequestByte, (FrNm_PnClusterReqExtType) /* SBSW_FRNM_CSL_02 */
                                      (FrNm_GetPnClusterReqExt(pnClusterRequestByte) & (FrNm_PnClusterReqExtType)(~(1U << pnClusterRequestBit))));
            }
          }
        }
      }
      /* #20 If an PNC ERA request has changed, indicate this to the ComM (via PduR -> Com <- ComM)
       *      Pass the changed PnClusterRequestExt buffer to the Com by calling PduR_FrNmRxIndication
       */
      if( pnClusterRequestChanged == TRUE )
      {
        nmPnInfo.SduDataPtr = FrNm_GetAddrPnClusterReqExt(FrNm_GetPbPncClusterConfigExtStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)); /* ESCAN00078357 */
        nmPnInfo.SduLength = FrNm_GetPnInfoLength();
        PduR_FrNmRxIndication( FrNm_GetPnEraRxPduIdOfChannelConfig(FRNM_CHANNEL_IDX), &nmPnInfo ); /* SBSW_FRNM_PDURCANNMRXINDICATION */
      }
      FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    } /* ESCAN00060592 */
  }
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_CONTROL_ENABLED == STD_ON )
# if( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
    ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_7 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf xf xf */
/**********************************************************************************************************************
  FrNm_CommunicationControlHandling()
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
 *
 *
 */
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_CommunicationControlHandling( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  FrNm_TxControlStateType txControlStateReq;

  /* ----- Implementation ----------------------------------------------- */
  txControlStateReq = FrNm_GetTxControlStateRequest(FRNM_CHANNEL_IDX);
  if( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) != txControlStateReq ) /* COV_FRNM_COM_CONTROL_VARIANT */
  {
    FrNm_SetTxControlState(FRNM_CHANNEL_IDX, txControlStateReq); /* SBSW_FRNM_CHANNEL_IDX */

    if( txControlStateReq == MSG_TX_DISABLED ) /* COV_FRNM_COM_CONTROL_VARIANT */
    {
#  if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
      /* Note: Timeout Timer only stopped in Schedule Variants where FrNm_DisableCommunication has an effect. (In this case txControlStateReq cannot be MSG_TX_DISABLED ) */
      FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
#  endif
#  if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
      FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
      FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
#  endif
    }
    else
    {

#  if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
      if( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION )  /* COV_FRNM_COM_CONTROL_VARIANT */
      {
        FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, (FrNm_RemoteSleepTimerType)(FrNm_GetRemoteSleepIndTimeOfChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
      }
#  endif
#  if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
      if( ( FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) > 0U ) && 
          ( ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION ) ||
            ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_REPEAT_MESSAGE ) )
        ) /* COV_FRNM_MSGTIMEOUTTIME TF tx tf tf */
      {
        /* Note: Restart Nm Timeout Timer ( Timer is only restarted in Schedule Variants where FrNm_EnableCommunication has an effect. (2,5,6) */
        FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, /* SBSW_FRNM_CHANNEL_IDX */
                             (FrNm_TimeoutTimerType)(FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) + 1U));
      }
#  endif
    }
  }
}
# endif
#endif

/**********************************************************************************************************************
  FrNm_StartupFromBusSleep()
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
 *
 */
FRNM_LOCAL_INLINE FUNC( void, FRNM_CODE ) FrNm_StartupFromBusSleep( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  if ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_BUS_SLEEP )
  {
    if( FrNm_IsMsgIndicationFlag(FRNM_CHANNEL_IDX) )
    {
      FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
      /* SPEC-16135: Notify NM message reception in Sleep */
      Nm_NetworkStartIndication( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
    }
    /* Network Restart */

    if( FrNm_GetNetworkRestartFlag(FRNM_CHANNEL_IDX) != FRNM_NO_RESTART )
    {
      /* ESCAN00078812 */
#if ( FRNM_ACTIVEWAKEUPBITENABLEDOFCHANNELCONFIG == STD_ON )
      /* Set Active Wakeup Bit if NetRequest is pending  */
      if(
#  if ( FRNM_ISDEF_ACTIVEWAKEUPBITENABLEDOFCHANNELCONFIG == STD_OFF )
          ( FrNm_IsActiveWakeupBitEnabledOfChannelConfig(FRNM_CHANNEL_IDX) ) &&
#  endif
          (FrNm_GetNetworkRestartFlag(FRNM_CHANNEL_IDX) == FRNM_ACTIVE_RESTART) )
      {
        FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_01 */
                              (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | FRNM_ACT_WU_BIT_MASK));
      }
#endif
      FrNm_SetNetworkRestartFlag(FRNM_CHANNEL_IDX, FRNM_NO_RESTART); /* SBSW_FRNM_CHANNEL_IDX */
      /* SPEC-16214, SPEC-16096: Transition to Synchronize Mode */
      FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_SYNCHRONIZE); /* SBSW_FRNM_CHANNEL_IDX */
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
      /* SPEC-15950, SPEC-25146: Notify Nm Interface that state has changed */
      Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX), NM_STATE_BUS_SLEEP, NM_STATE_SYNCHRONIZE );
#endif
    }
  }
}

# if ( FRNM_PDU_SCHEDULE_VARIANT_ONLY == 0U ) || \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY > 2u ) || \
     ( ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 7U ) && \
       ( ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) || \
         ( ( FRNM_PDU_SCHEDULE_VARIANT_3 == STD_ON ) || \
           ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
           ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) ) ) ) ) && \
           ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY <= 1U ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf tx tf xf xf tf tf tf */
/**********************************************************************************************************************
  FrNm_CopyRxVoteData()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CopyRxVoteData( FRNM_CHANNEL_TYPE_FIRST  VAR( PduIdType, AUTOMATIC ) FrNmRxPduId, CONST( uint8, AUTOMATIC ) controlBitVector )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* Note: Due to restriction that schedule variants have to be the same for every ECU in the cluster
   * it is guaranteed that the vote is aggregated correctly.
   */
  /* SPEC-15968: Handle received PDU */

  /* No run-time check for FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) > 2U if
   *  FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF necessary since
   *  odd RX PDU IDs are only allowed for Pdu Schedule Variants > 2
   */
# if ( FRNM_PDU_SCHEDULE_VARIANT_ONLY == 0U ) || \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY > 2u ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf */
  if ( (FrNmRxPduId & 0x1U) == 0x1U ) /* COV_FRNM_NO_ODD_RXPDUID */
  {
#  if ( FRNM_HW_VOTE_ENABLED == STD_OFF )
    SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#   if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_ON )
#    if ( FRNM_PDU_SCHEDULE_VARIANT_5 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF xf xf */
    /* Schedule Variant 5,6: Only presence of the Vote PDU suffices to indicate whether positive vote is requested. */
    FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                          (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) |
                           (FrNm_RxMessageVoteType)FRNM_VOTE_BIT_MASK));
#    else
    /* Schedule Variant 3,4,7: Bit-wise OR of Vote PDU contents */
    FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                          (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) | controlBitVector));
#    endif
#   else
    if ( (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 5U) ||
         (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 6U) ) /* COV_FRNM_UNSUPPORTED_VARIANT XF xf xf */
    {
      /* Schedule Variant 5,6: Only presence of the Vote PDU suffices to indicate whether positive vote is requested. */
      FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                            (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) | FRNM_VOTE_BIT_MASK));
    }
    else
    {
      /* Schedule Variant 3,4,7: Bit-wise OR of Vote PDU contents */
      FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                            (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) | controlBitVector));
    }
#   endif
    SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#  endif
  }
  else
# endif
  {
# if ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 7U ) && \
     ( ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) || \
       ( ( FRNM_PDU_SCHEDULE_VARIANT_3 == STD_ON ) || \
         ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
         ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) ) ) ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tx tf xf xf tf */
#  if ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY <= 1U ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TX tf tf */
#   if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
    if ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 7U ) /* COV_FRNM_UNSUPPORTED_VARIANT TX */
#   endif
    {
#   if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
#    if ( FRNM_HW_VOTE_ENABLED == STD_ON )
      if ( (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 3U) ||
           (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 4U) ) /* COV_FRNM_UNSUPPORTED_VARIANT XF xf xf */
#    endif
#   endif
      {
        SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                              (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) | controlBitVector));
        SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
#  endif
# endif
  }
  FRNM_DUMMY_STATEMENT(FrNmRxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FRNM_DUMMY_STATEMENT_CONST(controlBitVector); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

#if( FRNM_RXMESSAGEDATA == STD_ON )
/**********************************************************************************************************************
  FrNm_CopyRxMessageData()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CopyRxMessageData( FRNM_CHANNEL_TYPE_FIRST  CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) PduInfoDataPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least rxMessageDataByte;
  uint8_least rxMessageDataBytePos;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy received NM message data to the Rx Message Data buffer */
  SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_5(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
  for( rxMessageDataBytePos = FrNm_GetRxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), rxMessageDataByte = 0U;
       rxMessageDataBytePos < FrNm_GetRxMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
       rxMessageDataBytePos++, rxMessageDataByte++ )
  {
    FrNm_SetRxMessageData(rxMessageDataBytePos, PduInfoDataPtr[rxMessageDataByte]); /* SBSW_FRNM_CSL_01 */
  }
  SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_5(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

#if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
  FrNm_CoordReadyToSleepHandling()
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
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CoordReadyToSleepHandling( FRNM_CHANNEL_TYPE_FIRST  CONST( uint8, AUTOMATIC ) controlBitVector )
{
  /* #10 If Sleep Ready Bit has changed and NmIf has not been notified yet
   *      If Sleep Ready Bit is set
   *       Notify Nm_CoordReadyToSleepIndication()
   *      otherwise
   *       Notify Nm_CoordReadyToSleepCancellation()
   *      Update the changed Sleep Ready Bit
   */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sleepReadyBit;

  /* ----- Implementation ----------------------------------------------- */
  /* SPEC-16213: Check for Coordinator Sleep Ready Bit */
  sleepReadyBit = (uint8)(controlBitVector & NM_COORD_BIT_SLEEP);
  if( FrNm_GetCoordReadyToSleepState(FRNM_CHANNEL_IDX) != sleepReadyBit )
  {
    if( sleepReadyBit != 0U )
    {
      Nm_CoordReadyToSleepIndication( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
    }
    else
    {
      Nm_CoordReadyToSleepCancellation( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
    }

    FrNm_SetCoordReadyToSleepState(FRNM_CHANNEL_IDX, (FrNm_CoordReadyToSleepStateType)sleepReadyBit); /* SBSW_FRNM_CHANNEL_IDX */
  }
}
#endif

#if ( FRNM_PN_FEATURE_ENABLED == STD_ON ) && defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/**********************************************************************************************************************
  FrNm_RxPnFilterHandling()
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
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_RxPnFilterHandling( FRNM_CHANNEL_TYPE_FIRST  CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) PduInfoDataPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least  rxMsgBytePos;
  uint8        reqCluster;
# if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
  uint8        pnClusterRequestIndex;
# endif

  /* ----- Implementation ----------------------------------------------- */
# if( FRNM_ISDEF_PNENABLEDOFPBCHANNELCONFIG == STD_OFF )
  if( FrNm_IsPnEnabledOfPbChannelConfig(FRNM_CHANNEL_IDX) ) /* COV_FRNM_PNFEATUREENABLED */
# endif
  {
    if( (PduInfoDataPtr[0U] & (uint8)FRNM_PN_CRI_BIT_MASK ) != 0U )
    {
      /* copy the requests */
      SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# if ( ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON ) && ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON ) )
      for( rxMsgBytePos = 0U; ( rxMsgBytePos < FrNm_GetSizeOfPnClusterReqNew() ) && ( rxMsgBytePos < FrNm_GetPbPncClusterConfigExtLengthOfPbChannelConfig(FRNM_CHANNEL_IDX) ); rxMsgBytePos++ ) /* PRQA S 3355 */ /* MD_FrNm_3355_3358 */ /* COV_FRNM_SILENT_CHECK_PNCLUSTERREQEXT TF tf tx */
# elif ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON ) /* ESCAN00089533 */
      for( rxMsgBytePos = 0U; rxMsgBytePos < FrNm_GetSizeOfPnClusterReqNew(); rxMsgBytePos++ )
# else
      for( rxMsgBytePos = 0U; rxMsgBytePos < FrNm_GetPbPncClusterConfigExtLengthOfPbChannelConfig(FRNM_CHANNEL_IDX); rxMsgBytePos++ )
# endif
      {
        reqCluster = (uint8)(PduInfoDataPtr[rxMsgBytePos + FrNm_GetPnInfoOffset()] & FrNm_GetPnFilterMask(rxMsgBytePos));

        if( reqCluster != 0U )
        {
# if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
#  if ( FRNM_ISDEF_PNEIRACALCENABLED == STD_OFF )
          if ( FrNm_IsPnEiraCalcEnabled() ) /* COV_FRNM_PNFEATUREENABLED */
#  endif
          {
            FrNm_SetPnClusterReqNew(rxMsgBytePos, (FrNm_PnClusterReqNewType)(FrNm_GetPnClusterReqNew(rxMsgBytePos) | reqCluster)); /* SBSW_FRNM_CSL_01 */
          }
# endif
# if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
#  if ( FRNM_ISDEF_PNERACALCENABLEDOFCHANNELCONFIG == STD_OFF )
          if ( FrNm_IsPnEraCalcEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
#  endif
          {
            pnClusterRequestIndex = FrNm_GetPbPncClusterConfigExtStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX) + rxMsgBytePos;
            FrNm_SetPnClusterReqExtNew(pnClusterRequestIndex, (FrNm_PnClusterReqExtNewType)(FrNm_GetPnClusterReqExtNew(pnClusterRequestIndex) | reqCluster)); /* SBSW_FRNM_CSL_02 */
          }
# endif
        }
      }
      SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

# if ( FRNM_CARWAKEUPRXENABLEDOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
  FrNm_CarWakeupIndication()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_CarWakeupIndication( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If CarWakeupBit is set in CarWakeupByte
   *      If CarWakeup Filtering is disabled or received NodeId matches the configured CarWakeup Filter NodeId
   *       Call Nm_CarWakeUpIndication
   */

#  if ( FRNM_ISDEF_CARWAKEUPRXENABLEDOFCHANNELCONFIG == STD_OFF )
  if ( FrNm_IsCarWakeUpRxEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
#  endif
  {
    if ( (FrNm_GetRxMessageData(FrNm_GetRxMessageData_CarWakeUpFilterByteIdxOfPbChannelConfig(FRNM_CHANNEL_IDX) )
         & (FrNm_GetCarWakeUpBitMaskOfChannelConfig(FRNM_CHANNEL_IDX))) != 0U)
    {
# if ( FRNM_CAR_WUP_FILTER_FEATURE_ENABLED == STD_ON )
      if( 
#if ( FRNM_ISDEF_CARWAKEUPFILTERENABLEDOFCHANNELCONFIG == STD_OFF )
          ( !FrNm_IsCarWakeUpFilterEnabledOfChannelConfig(FRNM_CHANNEL_IDX) ) ||
#endif
          ( FrNm_GetRxMessageData(FrNm_GetRxMessageData_NIDIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) ==
            FrNm_GetCarWakeUpFilterNodeIdOfChannelConfig(FRNM_CHANNEL_IDX) )
      )
# endif
      {
        Nm_CarWakeUpIndication(FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX));
      }
    }
  }
  FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
}
#endif

/**********************************************************************************************************************
  FrNm_IndicationAndVoteHandling()
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
 *
 *
 *
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_IndicationAndVoteHandling( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( FRNM_HW_VOTE_ENABLED == STD_ON )
  uint8 frNmVector[12] = { 0U, 0U, 0U, 0U, 0U, 0U,
                           0U, 0U, 0U, 0U, 0U, 0U };
#else
# if ( FRNM_RXMESSAGEVOTE == STD_ON )
  uint8 frNmVector[1] = {0U};
# endif
#endif
#if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
  boolean msgIndicationReceived;
#endif
  /* ----- Implementation ----------------------------------------------- */
  /* Hardware Vote not Supported in ScheduleVariant 2,5,6 */
  /* #2400 Copy Hardware Vector for reception of Vote Bit in Schedule Variants 1,3,4,7
   *       in case hardware voting is enabled. SPEC-16070 */
  /* #2410 Copy Rx Vote Data in case Schedule Variant is 5 or 6 . (Data & Vote Bit in dynamic segment) */
  /* #2420 Copy Rx Indication event in case Schedule Variant is 2. (Data & Vote Bit in dynamic segment) */
#if ( FRNM_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG == STD_ON )
# if ( FRNM_ISDEF_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
  if( FrNm_IsHwVoteSupportInScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) )
# endif
  {
    /* Get NmVector from FrIf and handle received bits */
    (void)FrIf_GetNmVector(FrNm_GetChannelHandleOfChannelConfig(FRNM_CHANNEL_IDX), frNmVector); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER */
  }
# if ( FRNM_ISDEF_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
  else
  {
    frNmVector[FRNM_VOTE_POSITION] = FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX); /* SBSW_FRNM_VOTE_POSITION */
  }
# endif
#else
# if ( FRNM_RXMESSAGEVOTE == STD_ON )
  frNmVector[FRNM_VOTE_POSITION] = FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX); /* SBSW_FRNM_VOTE_POSITION */
# endif
#endif
#if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
# if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
  if ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 2U )
# endif
  {
    msgIndicationReceived = FrNm_IsMsgIndicationFlag(FRNM_CHANNEL_IDX);
  }
# if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
  else
  {
    msgIndicationReceived = FALSE;
  }
# endif
#endif

  /* Due to restriction that schedule variants have to be the same for every ECU in the cluster
      it is guaranteed that the vote handling is correct */
  /* #2430 If state is Ready Sleep or Normal Operation
   *       and Vote has been received.
   *       If state is Ready Sleep
   *         Re-Start Ready Sleep Counter SPEC-16131
   *       otherwise
   *         Restart Remote Sleep Indication Timer
   *       Notify Remote Sleep Cancellation SPEC-15990
   *       Reset Message Indication Flag
   *       Reset Rx Message Vote Bit
   */
  if( ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_READY_SLEEP )
#if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
      || ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION )
#endif
    )
  {
    if(
#if ( FRNM_HW_VOTE_ENABLED == STD_ON ) || ( FRNM_RXMESSAGEVOTE == STD_ON )
        ( frNmVector[FRNM_VOTE_POSITION] > FRNM_VOTE_BIT_INV_MASK )
# if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
        ||
# endif
#endif
#if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
        ( msgIndicationReceived == TRUE )
#endif
        )
    {
#if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
      if( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_READY_SLEEP )
#endif
      {
        FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, /* SBSW_FRNM_CHANNEL_IDX */
                             (FrNm_NetworkTimerType)((FrNm_NetworkTimerType)FrNm_GetReadySleepCntOfChannelConfig(FRNM_CHANNEL_IDX) +
                                                     (FrNm_NetworkTimerType)1U));
      }
#if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
      else  /* Normal Operation */
      {
# if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
        if( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED ) /* COV_FRNM_COM_CONTROL_VARIANT */
# endif
        {
          FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, (FrNm_RemoteSleepTimerType)( FrNm_GetRemoteSleepIndTimeOfChannelConfig(FRNM_CHANNEL_IDX) + 1U ) ); /* SBSW_FRNM_CHANNEL_IDX */
        }
      }

      if( FrNm_IsRemoteSleepIndState(FRNM_CHANNEL_IDX) )
      {
        FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
        Nm_RemoteSleepCancellation( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
      }
#endif

#if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
#if ( FRNM_HW_VOTE_ENABLED == STD_ON ) || ( FRNM_RXMESSAGEVOTE == STD_ON ) /* Encapsulation necessary for coverage reasons */

      if ( msgIndicationReceived == TRUE )
#endif
      {
        FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
      }
# if ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) && ( FRNM_RXMESSAGEVOTE == STD_ON ) )
      else
# endif
#endif
# if ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) && ( FRNM_RXMESSAGEVOTE == STD_ON ) )
      {
        FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType)(FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) & FRNM_VOTE_BIT_INV_MASK)); /* SBSW_FRNM_CHANNEL_IDX */
      }
#endif
    }
  }

#if ( FRNM_HWREPEATMSGREQUESTBITOFCHANNELCONFIG == STD_ON )
# if ( FRNM_ISDEF_HWREPEATMSGREQUESTBITOFCHANNELCONFIG == STD_OFF )
  if ( FrNm_IsHwRepeatMsgRequestBitOfChannelConfig(FRNM_CHANNEL_IDX) )
# endif
  {
    /* #2440 Save Repeat Message Bit from HW Vote */
    FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, (FrNm_RxMessageVoteType)(FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) | frNmVector[FRNM_VOTE_POSITION])); /* SBSW_FRNM_CHANNEL_IDX */
  }
#endif
}

/**********************************************************************************************************************
  FrNm_TransitionToNetworkMode()
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
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNetworkMode( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Set Nm state to Repeat Message. SPEC-15919
   *      Start Repeat Message timer. SPEC-15934, SPEC-15997
   *      Restart Message Timeout Timer
   *      Set Vote Bit in own NM message. SPEC-16005
   *      Indicate RemoteSleepCancellation to the NmIf
   *      Notify state change to Nm Interface. SPEC-15950, SPEC-15942
   *      Notify Network Mode to Nm Interface.
  */
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_REPEAT_MESSAGE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, /* SBSW_FRNM_CHANNEL_IDX */
    (FrNm_NetworkTimerType)(((FrNm_GetRepeatMessageTimeOfChannelConfig(FRNM_CHANNEL_IDX))+
                              ((FrNm_GetRepetitionCycleOfChannelConfig(FRNM_CHANNEL_IDX))-1U))/
                              (FrNm_GetRepetitionCycleOfChannelConfig(FRNM_CHANNEL_IDX))));
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
# if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
  if( FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) > 0U ) /* COV_FRNM_MSGTIMEOUTTIME */
  {
    /* Note: Started in every Schedule Variant on Entry of Network Mode */
    FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, (FrNm_TimeoutTimerType) (FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
  }
# endif
  /*
   * Schedule Variant| Action ----------------------|
   * | 1             | Set Vote Bit in TxMessageData |
   * | 3,4,7         | Set Vote Bit in TxMessageVote |
   *
   */
# if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf */
  switch ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
#  if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT TX */
    case 1U:
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | FRNM_VOTE_BIT_MASK));
      break;
#  endif
#  if ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON )  /* COV_FRNM_UNSUPPORTED_VARIANT XF */
    case 3U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 4U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 7U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType)(FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) | FRNM_VOTE_BIT_MASK)); /* SBSW_FRNM_CHANNEL_IDX */
      break;
#  endif
    /* PRQA S 2018 1 */ /* MD_MSR_14.1 */
    default:  /* COV_FRNM_UNSUPPORTED_VARIANT */
      break;
  }
# endif

# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
  /* SPEC-15958: Set the Repeat Message Flag */
  /* No need to set FrNm_RepeatMessage[FRNM_CHANNEL_IDX] to TRUE here,
    * commented out for code optimization as not needed for the algorithm
    */
# endif
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
  /* Note that this could only happen when a short-term sync loss occurred
    * after a remote sleep indication.
    */
  if( FrNm_IsRemoteSleepIndState(FRNM_CHANNEL_IDX) )
  {
    FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
    Nm_RemoteSleepCancellation( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
  }
# endif
#endif

#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX),
                            NM_STATE_SYNCHRONIZE, NM_STATE_REPEAT_MESSAGE );
#endif

  /* ESCAN00078812 */
  Nm_NetworkMode( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
}

/**********************************************************************************************************************
  FrNm_TransitionToNormalOperationOrReadySleep()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNormalOperationOrReadySleep( FRNM_CHANNEL_TYPE_FIRST  CONST( uint8, AUTOMATIC ) commRequested )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
  FrNm_SetLastNetworkRequested(FRNM_CHANNEL_IDX, commRequested); /* SBSW_FRNM_CHANNEL_IDX */
  if (commRequested == TRUE)
  {
    FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_NORMAL_OPERATION); /* SBSW_FRNM_CHANNEL_IDX */
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
#  if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
    if( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED ) /* COV_FRNM_COM_CONTROL_VARIANT */
#  endif
    {
      FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, (FrNm_RemoteSleepTimerType)(FrNm_GetRemoteSleepIndTimeOfChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
    }
# endif
  }
  else
#else
  FRNM_DUMMY_STATEMENT_CONST(commRequested); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
#endif
  {
    FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_READY_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */
    FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, FrNm_GetReadySleepCntOfChannelConfig(FRNM_CHANNEL_IDX)); /* SBSW_FRNM_CHANNEL_IDX */

    switch (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX))
    {
# if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON )
      case 1U:
#  if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
      /* SPEC-15873: Clear Vote */
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) &
                             (FrNm_TxMessageDataType)FRNM_VOTE_BIT_INV_MASK));
#  endif
      break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_256 == STD_ON )
      case 2U:
        FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */

        /* no break intended */ /* PRQA S 2003 1 */ /* MD_FrNm_2003 */
      case 5U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
      case 6U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
#  if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
#   if( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
        /* Note: Stop Timeout Timer on transition to Ready Sleep state for Variants where voting is done in dynamic segment (2,5,6) */
        FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
#   endif
#  endif
        break;
# endif
# if ( FRNM_TXMESSAGEVOTE == STD_ON ) && ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF tf xf */
      case 7U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
#  if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )  /* COV_FRNM_UNSUPPORTED_VARIANT XX */
        FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                              (FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) &
                                  (FrNm_TxMessageVoteType)FRNM_REPEAT_MSG_BIT_INV_MASK));
        /* Fall-Through intended */ /* PRQA S 2003 2 */ /* MD_FrNm_2003 */
#  endif
      case 3U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
      case 4U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                              (FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) &
                                  (FrNm_TxMessageVoteType)FRNM_VOTE_BIT_INV_MASK));
        break;
# endif
      /* PRQA S 2018 1 */ /* MD_MSR_14.1 */
      default: /* COV_FRNM_UNSUPPORTED_VARIANT X */
        break;
    }
  }

#if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
  FrNm_SetRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                        (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) &
                            (FrNm_TxMessageDataType)FRNM_REPEAT_MSG_BIT_INV_MASK));
#endif

#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX),
                                NM_STATE_REPEAT_MESSAGE, FrNm_GetNmState(FRNM_CHANNEL_IDX) );
#endif
}

/**********************************************************************************************************************
  FrNm_TransitionToBusSleepMode()
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
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToBusSleepMode( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_BUS_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
# if ( FRNM_ACTIVEWAKEUPBITENABLEDOFCHANNELCONFIG == STD_ON )
#  if ( FRNM_ISDEF_ACTIVEWAKEUPBITENABLEDOFCHANNELCONFIG == STD_OFF )
  if ( FrNm_IsActiveWakeupBitEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
#  endif
  {
    FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                         (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) & FRNM_ACT_WU_BIT_INV_MASK));
  }
# endif
  FrNm_SetNetworkRequested(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetLastNetworkRequested(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
  FrNm_SetRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
# endif
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
  FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
# endif

# if( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
  /* Note: Clear Message Timeout Timer on transition to BusSleep in every Variant */
  FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# endif

#endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

#if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_OFF ) || ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
# if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
  if ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 2U )
# endif
  {
    FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  }
#endif

#if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
  /* No need to notify Nm about Ready To Sleep Cancellation, would be done by Nm_NetworkMode */
  FrNm_SetCoordReadyToSleepState(FRNM_CHANNEL_IDX, FRNM_COORD_ASLEEP_OR_NOT_READY_TO_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX),
                              NM_STATE_READY_SLEEP, NM_STATE_BUS_SLEEP );
#endif
  Nm_BusSleepMode( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
}

#if ( FRNM_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_ON )
/**********************************************************************************************************************
  FrNm_TransitionToRepeatMessage()
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
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToRepeatMessage( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Nm_StateType lastState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set state to Repeat Message
   *     Set Repeat Message Request Bit in own NM message
   *     Restart Repeat Message timer
   */
  lastState = FrNm_GetNmState(FRNM_CHANNEL_IDX);
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_REPEAT_MESSAGE); /* SBSW_FRNM_CHANNEL_IDX */
  /* Restart Repeat Message timer. */
  FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, (FrNm_NetworkTimerType) /* SBSW_FRNM_CHANNEL_IDX */
                       (((FrNm_GetRepeatMessageTimeOfChannelConfig(FRNM_CHANNEL_IDX)-1U) /
                         FrNm_GetRepetitionCycleOfChannelConfig(FRNM_CHANNEL_IDX))));

  if( FrNm_IsRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX) )
  {
    FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                          (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | FRNM_REPEAT_MSG_BIT_MASK));
# if ( FRNM_PDU_SCHEDULE_VARIANT_7 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
#  if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
    if ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 7U )
#  endif
    {
      FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType) /* SBSW_FRNM_CHANNEL_IDX */
                            (FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) |
                             (FrNm_TxMessageVoteType)FRNM_REPEAT_MSG_BIT_MASK)); /* ESCAN00081165 */
    }
# endif
  }
  /* Repeat message indication handling:
   * No need to set FrNm_RepeatMsgRequestIndFlag.
   * It is set when the Application requests Node Detection actively.
   * In case Repeat Message Request is received, the information is directly written to the TxMessageData buffer
   * and not set in the FrNm_RepeatMsgRequestIndFlag in addition.
   * Deviates SPEC-15958.
   */
  /* #20 If last state is Ready Sleep
   *      | Schedule Variant | Action
   *      | 1                | Set Vote Bit in Tx Message Data
   *      | 2,5,6            | Start Message Timeout Timer
   *      | 3,4,7            | Set Vote Bit in Tx Message Vote
   */
  if( lastState == NM_STATE_READY_SLEEP )
  {
    switch (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX))
    {
# if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON )
      case 1U:
        FrNm_SetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                              (FrNm_GetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | FRNM_VOTE_BIT_MASK));
        break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
      case 3U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
      case 4U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
      case 7U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType)(FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) | FRNM_VOTE_BIT_MASK)); /* SBSW_FRNM_CHANNEL_IDX */
        break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_256 == STD_ON ) && ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
      case 2U: /* COV_FRNM_REPEATMSGREQUEST_DISABLED */
        /* Fall-Through intended */
      case 5U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Fall-Through intended */
      case 6U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
        /* Note: Re-Start Message Timeout Timer ( Ready Sleep -> Repeat Message ) in variants (2,5,6).
         * Rational: In all other variants, Timer is only stopped in BusSleep.
         */
        if( FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) > 0U ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
        {
          FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, (FrNm_TimeoutTimerType) /* SBSW_FRNM_CHANNEL_IDX */
                               ((FrNm_TimeoutTimerType)FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) +
                               (FrNm_TimeoutTimerType)1U));
        }
        break;
# endif
      /* PRQA S 2018 1 */ /* MD_MSR_14.1 */
      default: /* COV_FRNM_UNSUPPORTED_VARIANT X */
        break;
    }
  }

  /* #30 Remote Sleep Handling in Normal Operation and Ready Sleep */
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
  /* Stop Remote Sleep Indication Timer */
  FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
  /* SPEC-16011: Remote Sleep Cancellation */
  if( FrNm_IsRemoteSleepIndState(FRNM_CHANNEL_IDX) )
  {
    FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
    Nm_RemoteSleepCancellation( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
  }
# endif

  /* #40 Notify Repeat Message Indication to Nm Interface */
  Nm_RepeatMessageIndication( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
# if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  /* #50 Notify Nm Interface that state has changed. SPEC-15950 */
  Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX), lastState, NM_STATE_REPEAT_MESSAGE );
# endif
}
#endif

#if( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
  FrNm_TransitionToNormalOperation()
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
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToNormalOperation( FRNM_CHANNEL_TYPE_ONLY ) /* COV_FRNM_READYSLEEPCNT */
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_NORMAL_OPERATION); /* SBSW_FRNM_CHANNEL_IDX */
  /* Stop Ready Sleep Counter */
  FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
#  if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
  if( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED ) /* COV_FRNM_COM_CONTROL_VARIANT */
#  endif
  {
    /* Restart Remote Sleep Timer, reset Flag */
    FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, (FrNm_RemoteSleepTimerType)(FrNm_GetRemoteSleepIndTimeOfChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
    FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  }
# endif

  switch (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX))
  {
# if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON )
    case 1U:
      /* SPEC-15888 */
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | FRNM_VOTE_BIT_MASK));
      break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_256 == STD_ON ) && ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
    /* PRQA S 2003 2 */ /* MD_FrNm_2003 */
    case 2U:
      /* Fall-Through intended */
    case 5U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 6U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Note: Re-Start Message Timeout Timer (Ready Sleep -> Normal Operation ) in variants (2,5,6).
       * Rational: In all other variants, Timer is only stopped in BusSleep.
       */
      if( FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) > 0U ) /* COV_FRNM_MSGTIMEOUTTIME */
      {
        FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, (FrNm_TimeoutTimerType) (FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
      }
      break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
    /* PRQA S 2003 2 */ /* MD_FrNm_2003 */
    case 3U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 4U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 7U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* SPEC-15888: Set Vote */
      FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType) (FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) | FRNM_VOTE_BIT_MASK)); /* SBSW_FRNM_CHANNEL_IDX */
      break;
# endif
    /* PRQA S 2018 1 */ /* MD_MSR_14.1 */
    default:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      break;
    }

# if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
    /* SPEC-15950 */
    Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX),
                                NM_STATE_READY_SLEEP, NM_STATE_NORMAL_OPERATION );
# endif
}

/**********************************************************************************************************************
  FrNm_TransitionToReadySleep()
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
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransitionToReadySleep( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_READY_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetNetworkTimer(FRNM_CHANNEL_IDX, FrNm_GetReadySleepCntOfChannelConfig(FRNM_CHANNEL_IDX)); /* SBSW_FRNM_CHANNEL_IDX */

  switch (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX))
  {
# if ( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON )
    case 1U:
      /* SPEC-15873: Clear Vote */
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_VOTEIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) & FRNM_VOTE_BIT_INV_MASK));
      break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON )
    case 2U:
#  if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
      /* Note: Stop Message Timeout Timer ( Normal Operation -> Ready Sleep ) in variants (2,5,6).
       * Rational: In all other variants, Timer is only stopped in BusSleep.
       */
      FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
#  endif
      /* Reset of the Indication Flag */
      FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
      break;
# endif
# if ( FRNM_PDU_SCHEDULE_VARIANTS_347 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
    /* PRQA S 2003 2 */ /* MD_FrNm_2003 */
    case 3U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 4U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 7U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* SPEC-15873 */
      FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, (FrNm_TxMessageVoteType) (FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX) & FRNM_VOTE_BIT_INV_MASK)); /* SBSW_FRNM_CHANNEL_IDX */
      break;
# endif
# if (FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON ) && \
     ( ( FRNM_PDU_SCHEDULE_VARIANT_5 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) ) /* COV_FRNM_UNSUPPORTED_VARIANT XF tf xf xf */
    /* PRQA S 2003 2 */ /* MD_FrNm_2003 */
    case 5U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Fall-Through intended */
    case 6U:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      /* Note: Stop Message Timeout Timer ( Normal Operation -> Ready Sleep ) in variants (2,5,6).
       * Rational: In all other variants, Timer is only stopped in BusSleep.
       */
      FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
      break;
# endif
    /* PRQA S 2018 1 */ /* MD_MSR_14.1 */
    default:  /* COV_FRNM_UNSUPPORTED_VARIANT X */
      break;
  }

# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
  FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# endif
# if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  /* SPEC-15950 */
  Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX),
                              NM_STATE_NORMAL_OPERATION, NM_STATE_READY_SLEEP );
# endif
}
#endif

#if ( FRNM_TRANSMITSEPARATEVOTEOFCHANNELCONFIG == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
/**********************************************************************************************************************
  FrNm_TransmitVotePdu()
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
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransmitVotePdu( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType pduInfo;
# if ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
  PduIdType                 txPduIndirectionIndex;
# endif

  /* ----- Implementation ----------------------------------------------- */

  /* #10 If Vote is transmitted in separate PDU and
   *     ( state is not Ready Sleep and
   *       current cycle is a voting cycle and
   *       message transmission is allowed )
   *      Prepare Vote information for transmission
   *      Transmit Vote by calling FrIf_Transmit ( for each Vote Pdu in case of Dual Channel )
   */
  if(
# if ( FRNM_ISDEF_SEPARATEVOTEOFCHANNELCONFIG == STD_OFF ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
      ( FrNm_IsTransmitSeparateVoteOfChannelConfig(FRNM_CHANNEL_IDX) ) &&
# endif
      (
        /* Multiplexing in dynamic segment, transmission only in Repeat Message and Normal Operation and
         * only if FrNm_DisableCommunication has not been called
         */
        ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_READY_SLEEP )
# if ( FRNM_VOTINGCYCLEMASKOFCHANNELCONFIG == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
        && ( ( FrNm_GetCurrentFrCycle(FRNM_CHANNEL_IDX) & FrNm_GetVotingCycleMaskOfChannelConfig(FRNM_CHANNEL_IDX) ) == 0U )
# endif
# if ( FRNM_COM_CONTROL_ENABLED == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
        && ( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED )  /* COV_FRNM_COM_CONTROL_VARIANT */ /* ESCAN00063844 */
# endif
      )
    )
  {
    pduInfo.SduDataPtr = FrNm_GetAddrTxMessageVote(FRNM_CHANNEL_IDX); /* ESCAN00078357 */

# if ( FRNM_PDU_SCHEDULE_VARIANTS_256 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */ /* Variant 2 is excluded by "FrNm_IsTransmitSeperateVote()" */
    if ( (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 5U) ||
         (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 6U) )
    {
      pduInfo.SduLength = (PduLengthType)0U;
    }
    else
# endif
    {
      pduInfo.SduLength = (PduLengthType)1U;
    }

# if ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XX */
    for (txPduIndirectionIndex = FrNm_GetTxVotePduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
         txPduIndirectionIndex < FrNm_GetTxVotePduIdEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
         txPduIndirectionIndex++)
# endif
    {
      (void)FrIf_Transmit( FrNm_GetTxVotePduId(FRNM_TX_VOTE_PDU_INDIRECTION_INDEX), &pduInfo ); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER_NOWRITE */
    }
  }
}
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
  FrNm_TransmitDataPdu()
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
 *
 *
 */
FRNM_LOCAL_INLINE FUNC(void, FRNM_CODE) FrNm_TransmitDataPdu( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = FRNM_E_NO_ERROR;
  PduInfoType pduInfo;
# if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
  uint8_least pnClusterRequestByte;
# endif
# if ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON )
  PduIdType   txPduIndirectionIndex;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* Data transmission */
  /* #10 If DataPdu is transmitted in static slot (ScheduleVariant 1, 3, 5) OR
   *     ( state is not ReadySleep AND
   *     ( Current Cycle is Data Cycle AND
   *        message transmission is allowed ) ) )
   */
# if( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U )
  if( 
#  if ( FRNM_TRANSMITDATAINSTATICSLOTOFCHANNELCONFIG == STD_ON )
#   if( FRNM_ISDEF_TRANSMITDATAINSTATICSLOTOFCHANNELCONFIG == STD_OFF ) /* COV_FRNM_UNSUPPORTED_VARIANT TX */ /* not covered because of "FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U" above */
      ( FrNm_IsTransmitDataInStaticSlotOfChannelConfig(FRNM_CHANNEL_IDX) ) ||
#   endif
#  endif
      ( ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_READY_SLEEP )
#  if ( FRNM_DATACYCLEMASKOFCHANNELCONFIG == STD_ON ) || ( FRNM_COM_CONTROL_ENABLED == STD_ON )
        &&
        (
#   if ( FRNM_DATACYCLEMASKOFCHANNELCONFIG == STD_ON )
          ( ( FrNm_GetCurrentFrCycle(FRNM_CHANNEL_IDX) & FrNm_GetDataCycleMaskOfChannelConfig(FRNM_CHANNEL_IDX) ) == 0U )
#    if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
             &&
#    endif
#   endif
#   if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
          ( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED ) /* ESCAN00063844 */
#   endif
        )
#  endif
      )
    ) /* COV_FRNM_COM_CONTROL_VARIANT */
# endif /* ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U ) */
    {

# if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON )
      /* #20 Get User Data via PduR / Com */
      /* ESCAN00078357 */
      pduInfo.SduDataPtr = FrNm_GetAddrTxMessageData( FrNm_GetTxMessageData_UserDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX) );
      /* ESCAN00087841 */
      pduInfo.SduLength = FrNm_GetTxMessageData_UserDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX);

      (void)PduR_FrNmTriggerTransmit( FrNm_GetTxUserDataPduIdOfPbChannelConfig(FRNM_CHANNEL_IDX), &pduInfo ); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER */

      /* ESCAN00060592 */
#  if ( FRNM_DEV_ERROR_REPORT == STD_ON )
      if( (pduInfo.SduLength != FrNm_GetTxMessageData_UserDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX)) )
      {
        errorId = FRNM_E_PDUR_TRIGGERTX_ERROR;
      }
#  endif
# endif

# if ( FRNM_PN_FEATURE_ENABLED == STD_ON )
      /* #30 Apply Pn Filter mask to internal PN Eira requests and copy them for further handling */
#  if( FRNM_ISDEF_PNENABLEDOFPBCHANNELCONFIG == STD_OFF )
      if( FrNm_IsPnEnabledOfPbChannelConfig(FRNM_CHANNEL_IDX) ) /* COV_FRNM_PNFEATUREENABLED XF */
#  endif
      {

#  if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
#   if ( FRNM_ISDEF_PNEIRACALCENABLED == STD_OFF )
        if ( FrNm_IsPnEiraCalcEnabled() ) /* COV_FRNM_PNFEATUREENABLED */
#   endif
        {
          /* copy the requests and apply the PN filter mask: */
          SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          for( pnClusterRequestByte = 0U; pnClusterRequestByte < FrNm_GetSizeOfPnClusterReqNew(); pnClusterRequestByte++ )
          {
            FrNm_SetPnClusterReqNew(pnClusterRequestByte, (FrNm_PnClusterReqNewType) /* SBSW_FRNM_CSL_01 */
                                    (FrNm_GetPnClusterReqNew(pnClusterRequestByte) | (FrNm_PnClusterReqNewType)
                                    (FrNm_GetTxMessageData(FrNm_GetTxMessageData_PnFilterMaskStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX) + pnClusterRequestByte) &
                                      FrNm_GetPnFilterMask(pnClusterRequestByte))));
          }
          SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
#  endif /* ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON ) */
      }
# endif /* ( FRNM_PN_FEATURE_ENABLED == STD_ON ) */

      SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* #40 Prepare data for transmission */
      /* ESCAN00078357 */
      pduInfo.SduDataPtr = FrNm_GetAddrTxMessageData(FrNm_GetTxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX));

      pduInfo.SduLength = (PduLengthType)(FrNm_GetTxMessageDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX));

      /* #50 Transmit the NM Data message (twice in case of 'Dual Channel Pdu Enabled') */
# if ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON )
      for (txPduIndirectionIndex = FrNm_GetTxDataPduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
           txPduIndirectionIndex < FrNm_GetTxDataPduIdEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
           txPduIndirectionIndex++)
# endif
      {
        (void)FrIf_Transmit( FrNm_GetTxDataPduId(FRNM_TX_DATA_PDU_INDIRECTION_INDEX), &pduInfo );  /* SBSW_FRNM_FUNC_CALL_WITH_POINTER_NOWRITE */
      }

# if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
      /* ESCAN00082307 */
      /* #60 Set Flag in order to send additional Sync PDUs below (outside of Exclusive Area) */
#  if ( FRNM_ISDEF_SYNCPDUMASTERENABLEDOFCHANNELCONFIG == STD_OFF )
      if ( FrNm_IsSyncPduMasterEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
#  endif
      {
        /* Set decision flag to send a sync PDU */
        FrNm_SetSendSyncPdusFlag(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */
      }
# endif

      SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

# if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
  /* #70 Determine whether Sync PDUs shall be sent */
  /* ESCAN00082307 */
  if ( ( ( FrNm_GetCurrentFrCycle(FRNM_CHANNEL_IDX) % (FrNm_GetVotingCycleMaskOfChannelConfig(FRNM_CHANNEL_IDX) + 1) ) ==
           FrNm_GetSyncPduTxRequestCycleOffsetOfChannelConfig(FRNM_CHANNEL_IDX) ) &&
       ( FrNm_IsSendSyncPdusFlag(FRNM_CHANNEL_IDX) )
     )
  {
    pduInfo.SduDataPtr = (P2VAR(FrNm_TxSyncPduMessageDataType, AUTOMATIC, FRNM_VAR_NOINIT))(FrNm_GetAddrTxSyncPduMessageData(0U));
    pduInfo.SduLength = (PduLengthType)(FrNm_GetTxSyncPduMessageDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX));

    (void)FrIf_Transmit(FrNm_GetTxSyncPduId(FrNm_GetTxSyncPduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)), &pduInfo); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER_NOWRITE */
    (void)FrIf_Transmit(FrNm_GetTxSyncPduId(FrNm_GetTxSyncPduIdStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)+1U), &pduInfo); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER_NOWRITE */
  }
# endif

  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON ) && ( FRNM_COM_USER_DATA_SUPPORT == STD_ON )
  /* #80 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError( FRNM_MODULE_ID, FRNM_INSTANCE_ID, (FRNM_SID_MAINFUNCTION + FRNM_CHANNEL_IDX), errorId );
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  FRNM_DUMMY_STATEMENT_CONST(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
}
#endif

/**********************************************************************************************************************
  FrNm_LocalMainFunction()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * \trace SPEC-16002, SPEC-50134, SPEC-16192, SPEC-16158, SPEC-16005, SPEC-50105, SPEC-50114
 * \trace SPEC-15873, SPEC-2622864, SPEC-50104, SPEC-50064, SPEC-16177, SPEC-16206, SPEC-15917, SPEC-16100, SPEC-15963
 * \trace SPEC-16172, SPEC-16143, SPEC-15995, SPEC-16134, SPEC-16127
 */
#if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
FUNC( void, FRNM_CODE ) FrNm_LocalMainFunction( void )
#else
FUNC( void, FRNM_CODE ) FrNm_LocalMainFunction( CONST( NetworkHandleType, AUTOMATIC ) channel )
#endif
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                     errorId = FRNM_E_NO_ERROR;

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
  uint8 commRequested;
#endif
  uint16                    frCycleMacroTick;
  FrIf_StateType            frIfState;
  Std_ReturnType            frIfGetGlobalTimeRetVal;
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  Nm_StateType              lastState;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  else
#endif
# if ( FRNM_POSTBUILD_VARIANT_SUPPORT == STD_ON )
#  if ( FRNM_ISDEF_CHANNELCONFIGOFPCCONFIG == STD_OFF )
  if( FrNm_HasChannelConfig() ) /* COV_FRNM_INACTIVE_CHANNEL TX */
#  endif
# endif
  {
  /* #20 Check parameter 'channel' */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) && !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    if (FRNM_CHANNEL_IDX >= FrNm_GetSizeOfChannelConfig())
    {
      errorId = FRNM_E_INVALID_CHANNEL;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_CONTROL_ENABLED == STD_ON )
# if( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
    ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_7 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf xf xf */
      /* #200 Process Communication Control (if ComControlEnabled enabled) */
      FrNm_CommunicationControlHandling(FRNM_CHANNEL_PARA_ONLY);
# endif
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
      /* #210 Copy the global communication state. (no interrupt locks required) */
      commRequested = FrNm_IsNetworkRequested(FRNM_CHANNEL_IDX);
#endif

      /* #220 Check for (Re-)Start in Sleep */
      FrNm_StartupFromBusSleep(FRNM_CHANNEL_PARA_ONLY);

      /* #230 Further main function handling only necessary if not in state Bus Sleep */
      if( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_BUS_SLEEP )
      {
#if ( FRNM_MSGCONFIRMATIONFLAG == STD_ON ) && ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
        /* #2300 Process Tx Confirmation events */
        if( FrNm_IsMsgConfirmationFlag(FRNM_CHANNEL_IDX) )
        {
          FrNm_SetMsgConfirmationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
# if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
          if( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_ENABLED ) /* COV_FRNM_COM_CONTROL_VARIANT */
# endif
          {
            if( FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) > 0U ) /* COV_FRNM_MSGTIMEOUTTIME */
            {
              /* Note: Timeout Timer started on Message Confirmation in every Variant */
              FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, (FrNm_TimeoutTimerType)(FrNm_GetMsgTimeoutTimeOfPbChannelConfig(FRNM_CHANNEL_IDX) + 1U)); /* SBSW_FRNM_CHANNEL_IDX */
            }
          }
        }
#endif

        /* #2400 Process received message indications and Vote requests */
        FrNm_IndicationAndVoteHandling(FRNM_CHANNEL_PARA_ONLY);

        /* #2500 Determine FlexRay Global Time */
        frIfGetGlobalTimeRetVal = FrIf_GetGlobalTime( FrNm_GetChannelHandleOfChannelConfig(FRNM_CHANNEL_IDX), /* SBSW_FRNM_FUNC_CALL_WITH_POINTER */
                                                      FrNm_GetAddrCurrentFrCycle(FRNM_CHANNEL_IDX), &frCycleMacroTick );

        /* #2510 Synchronize to Repetition Cycle SPEC-16002, SPEC-16206 */
        if( (frIfGetGlobalTimeRetVal == E_OK)
#if ( FRNM_CYCLE_COUNTER_EMULATION == STD_ON )
            || (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_READY_SLEEP)
#endif
          )
        {
#if ( !FrNm_IsMainAcrossFrCycle() ) || ( FRNM_CYCLE_COUNTER_EMULATION == STD_ON )
# if ( FrNm_IsMainAcrossFrCycle() ) && ( FRNM_CYCLE_COUNTER_EMULATION == STD_ON )
          /* #2520 In case Cycle Counter Emulation is enabled and Global Time could not be retrieved,
           *         Increment the Cycle value by one. */
          if (frIfGetGlobalTimeRetVal == E_NOT_OK)
# endif
          {
            FrNm_IncCurrentFrCycle(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHANNEL_IDX */
          }
#endif
          /* #2600 If Repetition Cycle End is reached. */
          if( ( FrNm_GetCurrentFrCycle(FRNM_CHANNEL_IDX) % FrNm_GetRepetitionCycleOfChannelConfig(FRNM_CHANNEL_IDX) ) == 0U )
          {

#if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
            /* ESCAN00082307 */
            /* #26000 Reset decision flag to send a sync PDU */
            FrNm_SetSendSyncPdusFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
#endif

            /* # 26010 Do state handling */
            switch ( FrNm_GetNmState(FRNM_CHANNEL_IDX) )
            {
            case NM_STATE_SYNCHRONIZE:
              /* #26020 If new state is Synchronize
               *          If FrIf Controller state is Online. SPEC-16056
               *            Process transition to NetworkMode
               *          Otherwise
               *            Wait until synchronization is established by lower layer.
               */
              (void)FrIf_GetState( FrNm_GetChannelHandleOfChannelConfig(FRNM_CHANNEL_IDX), &frIfState );  /* SBSW_FRNM_FUNC_CALL_WITH_POINTER */
              /* SPEC-16056: Transition at next boundary and only if FrIf is online */
              if( frIfState == FRIF_STATE_ONLINE )
              {
                FrNm_TransitionToNetworkMode(FRNM_CHANNEL_PARA_ONLY);
              }
              else
              {
                break;
              }
              /* #26030 If new state is Repeat Message
               *          If Repeat Message Timer has elapsed
               *            Process transition to either Normal Operation or Ready Sleep.
               *          Otherwise
               *            Decrement Repeat Message Timer
               */
            case NM_STATE_REPEAT_MESSAGE: /* COV_FRNM_REPEATMSGTIME1 */
              if( FrNm_GetNetworkTimer(FRNM_CHANNEL_IDX) == 0U ) /* COV_FRNM_REPEATMSGTIME1 */
              {
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
                FrNm_TransitionToNormalOperationOrReadySleep(FRNM_CHANNEL_PARA_FIRST  commRequested);
#else
                FrNm_TransitionToNormalOperationOrReadySleep(FRNM_CHANNEL_PARA_FIRST  FALSE);
#endif
              }
              else
              {
                /* Decrementing of Repeat Message Timer */
                FrNm_DecNetworkTimer(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHANNEL_IDX */
              }
              break;
              /* #26040 If new state is Ready Sleep
               *          If Ready Sleep Counter has elapsed
               *            Process transition to BusSleepMode
               *          Otherwise
               *            Decrement Ready Sleep Counter. SPEC-15962
               */
            case NM_STATE_READY_SLEEP:
              if( FrNm_GetNetworkTimer(FRNM_CHANNEL_IDX) == 0U )
              {
                FrNm_TransitionToBusSleepMode(FRNM_CHANNEL_PARA_ONLY);
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
                /* PRQA S 3333 1 */ /* MD_FrNm_3333 */
                break;
#endif
              }
              /* Fall through intended */
              else
              {
                FrNm_DecNetworkTimer(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHANNEL_IDX */
              }
              /* #26050 If new state is Normal Operation or Ready Sleep
               */
            /* PRQA S 2003 1 */ /* MD_FrNm_2003 */
            default: /* COV_FRNM_REPEATMSGTIME1 */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
              /* Normal Operation + partly Ready Sleep handling */
# if ( FRNM_VOTING_NEXT_TO_LAST_REPETITION_CYCLE_DISABLE == STD_ON )
              /* #26060 Allow vote changes only if one of these conditions are true
                *         if vote inhibition is not enabled for this channel
                *         if the current state is not Ready Sleep
                *         if the Ready Sleep Counter does not hold the value for the next-to-last (1) or
                *         last repetition cycle (0) before the transition to Bus Sleep Mode
                */
#  if ( FRNM_VOTEINHIBITIONENABLEDOFCHANNELCONFIG == STD_ON )
              if (
#   if ( FRNM_ISDEF_VOTEINHIBITIONENABLEDOFCHANNELCONFIG == STD_OFF )
                   ( !FrNm_IsVoteInhibitionEnabledOfChannelConfig(FRNM_CHANNEL_IDX) ) ||
#   endif
                   ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_READY_SLEEP ) ||
                   ( FrNm_GetNetworkTimer(FRNM_CHANNEL_IDX) > 1U )
                 )
#  endif
# endif
              {
# if ( FRNM_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_ON ) /* ESCAN00089668 */
                /* #26070 Transition to Repeat Message state due to either
                 *  a) a Repeat Message Request
                 *  b) a Repeat Message Indication in last received NM message. SPEC-16160, SPEC-16217
                 *         Process Transition to RepeatMessage state
                 */
                /* PRQA S 0715 SUPPRESS0715 */ /* MD_MSR_1.1 */
                if(
#  if ( FRNM_ISDEF_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_OFF )
                  ( FrNm_IsRepeatMessageRequestBitActiveOfChannelConfig(FRNM_CHANNEL_IDX) ) &&
#  endif
                    ( ( FrNm_IsRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX) ) ||
                      ( (FrNm_GetRxMessageVote(FRNM_CHANNEL_IDX) & FRNM_REPEAT_MSG_BIT_MASK) == FRNM_REPEAT_MSG_BIT_MASK) ) )
                {
                  FrNm_TransitionToRepeatMessage(FRNM_CHANNEL_PARA_ONLY);
                }
                else
# endif
                {
                  /* #26080 Communication request handling, SPEC-15959
                   *        If communication request has changed
                   *          Save current communication request
                   *          If communication request is set
                   *              Process Transition to NormalOperation
                   *          Otherwise
                   *              Process Transition to ReadySleep
                   */
                  if( FrNm_IsLastNetworkRequested(FRNM_CHANNEL_IDX) != commRequested )
                  {
                    FrNm_SetLastNetworkRequested(FRNM_CHANNEL_IDX, commRequested); /* SBSW_FRNM_CHANNEL_IDX */

                    if( commRequested == TRUE ) /* COV_FRNM_READYSLEEPCNT */
                    {
                      FrNm_TransitionToNormalOperation(FRNM_CHANNEL_PARA_ONLY);
                    }
                    else
                    {
                      FrNm_TransitionToReadySleep(FRNM_CHANNEL_PARA_ONLY);
                    }
                  }
                }
                /* PRQA L:SUPPRESS0715 */
              }
#endif  /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) */
              break;
            }

            /* #26090  If Schedule Variant is 1
             *           Clear received Vote Bit in message buffer.
             */
#if ( (( FRNM_PDU_SCHEDULE_VARIANT_1 == STD_ON ) && ( FRNM_HW_VOTE_ENABLED == STD_OFF )) || \
      ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) )
# if ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_OFF ) && \
     ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
            if (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 1U)
# endif
            {
              /* Reset Vote Buffer (CBV) */
              FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
            }
#endif
          }

            /* #26100 If Synchronization Point is configured
             *          Call Nm_SynchronizationPoint
             */
#if ( FRNM_SYNCHRONIZATIONPOINTENABLEDOFCHANNELCONFIG == STD_ON )
# if ( FRNM_ISDEF_SYNCHRONIZATIONPOINTENABLEDOFCHANNELCONFIG == STD_OFF)
          if ( FrNm_IsSynchronizationPointEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
# endif
          {
            Nm_SynchronizationPoint( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
          }
#endif
        } /* else GetGlobalTime */
        else
        {
          /* #2700 If state is RepeatMessage or NormalOperation or (ReadySleep and Network is requested)
           *         Temporary store the current NM state
           *         Set NM state to Synchronize
           *         Clear Timeout Timer
           *         Clear Remote Sleep Timer
           *         Notify state change to Nm Interface
           */
          /* Change to Synchronize Mode / Bus Sleep Mode if State is Repeat Message */
          if( (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_REPEAT_MESSAGE)
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
            /* or Normal Operation */
            || (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION)
# if ( FRNM_CYCLE_COUNTER_EMULATION == STD_OFF )
            || ( (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_READY_SLEEP)
                 && ( FrNm_IsNetworkRequested(FRNM_CHANNEL_IDX) ) )
# endif
#endif
            ) /* COV_FRNM_REPEATMSGTIME2 */
          {
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
            lastState = FrNm_GetNmState(FRNM_CHANNEL_IDX);
#endif
            /* SPEC-16036, SPEC-16209: State Transition to Synchronize Mode */
            FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_SYNCHRONIZE); /* SBSW_FRNM_CHANNEL_IDX */

            /* ESCAN00078812 */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
# if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
            /* Note: Stop Message Timeout Timer ( Normal Operation -> Ready Sleep ) in all variants, when Network Mode is left. */
            FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# endif
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
            FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# endif
#endif
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
            /* SPEC-15950: Notify Nm Interface that state has changed */
            Nm_StateChangeNotification( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX), lastState, FrNm_GetNmState(FRNM_CHANNEL_IDX) );
#endif
          }
        }

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)

        /* SPEC-16141: Transmission of NM PDU(s) */
        /* Transmission only in Network Mode */
        if( ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_BUS_SLEEP ) &&
            ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_SYNCHRONIZE ) )
        {
# if ( FRNM_TRANSMITSEPARATEVOTEOFCHANNELCONFIG == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT XF */
        /* #2800 Process transmission of Vote PDU */
          FrNm_TransmitVotePdu(FRNM_CHANNEL_PARA_ONLY);
# endif

          /* #2900 Process transmission of Data PDU */
          FrNm_TransmitDataPdu(FRNM_CHANNEL_PARA_ONLY);
        }

#if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
        /* #3000 Handling of NM Timeout Timer */
        if( FrNm_GetTimeoutTimer(FRNM_CHANNEL_IDX) > 0U )
        {
          FrNm_DecTimeoutTimer(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHANNEL_IDX */
          if( FrNm_GetTimeoutTimer(FRNM_CHANNEL_IDX) == 0U )
          {
            /* SPEC-16143: Notify Timeout Exception to upper layer. */
            Nm_TxTimeoutException( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
          }
        }
#endif

# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
        /* #3100 Handling of Remote Sleep Indication Timer */
        if( FrNm_GetRemoteSleepTimer(FRNM_CHANNEL_IDX) > 0U )
        {
          FrNm_DecRemoteSleepTimer(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHANNEL_IDX */
          if( FrNm_GetRemoteSleepTimer(FRNM_CHANNEL_IDX) == 0U )
          {
            /* SPEC-16100: Notify Remote Sleep Indication */
            FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */
            Nm_RemoteSleepIndication( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
          }
        }
# endif

#endif /* if( FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */

      } /* FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_BUS_SLEEP */

#if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
      /* #240 Calculate Pn Era (if PnEraCalcEnabled enabled) */
      FrNm_PnEraCalculation( FRNM_CHANNEL_PARA_ONLY );
#endif
    } /* FrNm_HasChannelConfig() */
  } /* !FrNm_IsModuleInitialized() */
#if ( FRNM_POSTBUILD_VARIANT_SUPPORT == STD_ON )
# if ( FRNM_ISDEF_CHANNELCONFIGOFPCCONFIG == STD_OFF )
  else
  {
    /* Intentionally left empty */
  }
# endif
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, (FRNM_SID_MAINFUNCTION + FRNM_CHANNEL_IDX), errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* End of FrNm_LocalMainFunction() */ /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
  FrNm_LocalInit()
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
 */
FRNM_LOCAL FUNC( void, FRNM_CODE ) FrNm_LocalInit( FRNM_CHANNEL_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( FRNM_TXMESSAGEDATA == STD_ON ) || ( FRNM_RXMESSAGEDATA == STD_ON ) || ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
  uint8_least dataByte;
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all global and local variables */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
  /* SPEC-16115: The bus-communication is initially released. */
  FrNm_SetNetworkRequested(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetLastNetworkRequested(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetMsgConfirmationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
# if ( FRNM_MSGTIMEOUTTIMEOFPBCHANNELCONFIG == STD_ON )
  FrNm_SetTimeoutTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
# endif
#endif

  FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetNetworkRestartFlag(FRNM_CHANNEL_IDX, FRNM_NO_RESTART); /* SBSW_FRNM_CHANNEL_IDX */

#if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
  FrNm_SetRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
  /* Reset decision flag to send a sync PDU */ /* ESCAN00082307 */
  FrNm_SetSendSyncPdusFlag(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
  /* Reset Remote Sleep Timer and state. */
  FrNm_SetRemoteSleepTimer(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetRemoteSleepIndState(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_TXCONTROLSTATE == STD_ON ) /* ESCAN00089531 */
  /* Per default the transmission control status is enabled. */
  FrNm_SetTxControlState(FRNM_CHANNEL_IDX, MSG_TX_ENABLED); /* SBSW_FRNM_CHANNEL_IDX */
  FrNm_SetTxControlStateRequest(FRNM_CHANNEL_IDX, MSG_TX_ENABLED); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
  FrNm_SetCoordReadyToSleepState(FRNM_CHANNEL_IDX, FRNM_COORD_ASLEEP_OR_NOT_READY_TO_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */
#endif

  /* Default value of Emulated / Actual Fr Communication Cycle is 0 */
  FrNm_SetCurrentFrCycle(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */

  /* #20 Initialize RX, TX and Sync NM message data buffers */
  /* Initialize Tx (SPEC-16186, SPEC-15936)/ Rx Data Buffers / Votes (SPEC-15945) */
#if ( FRNM_TXMESSAGEDATA == STD_ON )
  for ( dataByte = FrNm_GetTxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte < FrNm_GetTxMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte++ )
  {
    FrNm_SetTxMessageData(dataByte, 0xFFU); /* SBSW_FRNM_CSL_03 */
  }
#endif

#if ( FRNM_RXMESSAGEDATA == STD_ON )
  for ( dataByte = FrNm_GetRxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte < FrNm_GetRxMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte++ )
  {
    FrNm_SetRxMessageData(dataByte, 0xFFU); /* SBSW_FRNM_CSL_03 */
  }
#endif

#if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
  for ( dataByte = FrNm_GetTxSyncPduMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte < FrNm_GetTxSyncPduMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
        dataByte++ )
  {
    FrNm_SetTxSyncPduMessageData(dataByte, 0x00U); /* SBSW_FRNM_CSL_03 */
  }
#endif

#if ( FRNM_TXMESSAGEDATA == STD_ON )
  /* #30 Set BYTE 0 to zero */
  /*   | Schedule Variant | CBV Used | BYTE0
   *   | 1                | TRUE     | VOTE+CBV
   *   | 1                | FALSE    | VOTE
   *   | 2,5,6            | TRUE     | CBV
   *   | 2,5,6            | FALSE    | USERDATA (But always initialized to zero)
   *   | 3,4,7            | TRUE     | CBV
   *   | 3,4,7            | FALSE    | USERDATA (But always initialized to zero)
   */
  FrNm_SetTxMessageData(FrNm_GetTxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), 0x00U); /* SBSW_FRNM_CSL_01 */

# if ( FRNM_PN_FEATURE_ENABLED == STD_ON )
#  if( FRNM_ISDEF_PNENABLEDOFPBCHANNELCONFIG == STD_OFF )
  if( FrNm_IsPnEnabledOfPbChannelConfig(FRNM_CHANNEL_IDX) ) /* COV_FRNM_PNFEATUREENABLED XF */
#  endif
  {
    FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), FRNM_PN_CRI_BIT_MASK); /* SBSW_FRNM_CSL_01 */
  }
# endif

  /* #40 Set Source Node Identifier Byte */
# if ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON )
  FrNm_SetTxMessageData( FrNm_GetTxMessageData_NIDIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), FrNm_GetNodeIdOfPbChannelConfig(FRNM_CHANNEL_IDX) ); /* SBSW_FRNM_CSL_01 */
# endif
#endif

  /* #50 Initialize Rx and Tx Vote buffers */
#if ( FRNM_TXMESSAGEVOTE == STD_ON )
  FrNm_SetTxMessageVote(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
#endif

#if ( FRNM_RXMESSAGEVOTE == STD_ON )
  FrNm_SetRxMessageVote(FRNM_CHANNEL_IDX, 0U); /* SBSW_FRNM_CHANNEL_IDX */
#endif

  /* #60 Set NmState to default state Bus Sleep without notification of upper layer */
  FrNm_SetNmState(FRNM_CHANNEL_IDX, NM_STATE_BUS_SLEEP); /* SBSW_FRNM_CHANNEL_IDX */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * FrNm_Init()
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
 */
FUNC( void, FRNM_CODE ) FrNm_Init( CONSTP2CONST( FrNm_ConfigType, AUTOMATIC, FRNM_INIT_DATA ) nmConfigPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FRNM_E_NO_ERROR;
#if ( FRNM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
  uint8 ecumErrorId = ECUM_NO_BSWERROR;
#endif
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  NetworkHandleType channel;
#endif

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_INIT )

#if ( FRNM_USE_INIT_POINTER == STD_ON )
  /* #10 Store selected configuration from 'nmConfigPtr' */
  FrNm_ConfigDataPtr = nmConfigPtr;
#else
  FRNM_DUMMY_STATEMENT_CONST(nmConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #20 Check that module is not initialized */
  if( FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_ALREADY_INITIALIZED;
  }
  else
#endif /* ( FRNM_DEV_ERROR_DETECT == STD_ON ) */
#if ( FRNM_USE_INIT_POINTER == STD_ON )
  /* #30 Check that configuration parameter 'nmConfigPtr' is not NULL */
  if ( nmConfigPtr == NULL_PTR )
  {
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    errorId = FRNM_E_INVALID_POINTER;
# endif
# if ( FRNM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
    ecumErrorId = ECUM_BSWERROR_NULLPTR;
# endif
  }
  else
#endif /* ( FRNM_USE_INIT_POINTER == STD_ON ) */
#if ( FRNM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
  /* #40 Check Generator Compatibility Version */
  if ( FrNm_GetGeneratorCompatibilityVersion() != (uint16)FRNM_EXPECTED_GENERATOR_COMPATIBILITY_VERSION )
  {
    ecumErrorId = ECUM_BSWERROR_COMPATIBILITYVERSION;
  }
  else
#endif
#if ( FRNM_FINALMAGICNUMBER == STD_ON )
  /* #50 Check Magic Number */
  if( FrNm_GetFinalMagicNumber() != FRNM_FINAL_MAGIC_NUMBER )
  {
    errorId = FRNM_E_INIT_FAILED;
    ecumErrorId = ECUM_BSWERROR_MAGICNUMBER;
  }
  else
#endif
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) && ( FRNM_PBCHANNELCONFIG == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* PRQA S 3325, 3201 4 */ /* MD_FrNm_3325_3201 */
  if( FrNm_GetSizeOfChannelConfig() != FrNm_GetSizeOfPbChannelConfig() ) /*lint !e506 */ /* COV_FRNM_INVALID_GENDATA */
  {
    errorId = FRNM_E_INVALID_GENDATA;
  }
  else
# endif
  {
    /* Initialize PN relevant variables */
#if defined ( FRNM_PN_CALC_FEATURE_ENABLED )
    FrNm_PnLocal_Init();
#endif
    /* Initialize each channel. */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    for( channel = 0U; channel < FrNm_GetSizeOfChannelConfig(); channel++ )
#endif
    {
      FrNm_LocalInit( FRNM_CHANNEL_PARA_ONLY );
    }
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    FrNm_SetModuleInitialized(TRUE);
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_INIT, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

#if ( FRNM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
  if ( ecumErrorId != ECUM_NO_BSWERROR )
  {
    (void)EcuM_BswErrorHook(FRNM_MODULE_ID, ecumErrorId);
  }
#endif
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_INIT )
}  /* PRQA S 2006,6010,6030,6080 */ /* MD_MSR_14.7,MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  FrNm_PassiveStartUp()
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
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_PassiveStartUp( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_PASSIVESTARTUP )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* #200 If NmState is BusSleep AND if network was not actively requested
     *      Set NetworkRestartFlag to "passive restart" for further handing in main function.
     */
    if( (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_BUS_SLEEP) &&
        (FrNm_GetNetworkRestartFlag(FRNM_CHANNEL_IDX) == FRNM_NO_RESTART) )
    {
      FrNm_SetNetworkRestartFlag(FRNM_CHANNEL_IDX, FRNM_PASSIVE_RESTART); /* SBSW_FRNM_CHANNEL_IDX */
      retVal =  E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_PASSIVESTARTUP, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_PASSIVESTARTUP )
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  FrNm_NetworkRequest()
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
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_NetworkRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_NETWORKREQUEST )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* #200 Set Network Requested to TRUE
     *      If state is BusSleep and no passive or active request has been issued before
     *        Set NetworkRestartFlag to ActiveRestart
     */
    FrNm_SetNetworkRequested(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */

    if( (FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_BUS_SLEEP) &&
        (FrNm_GetNetworkRestartFlag(FRNM_CHANNEL_IDX) == FRNM_NO_RESTART) )
    {
      FrNm_SetNetworkRestartFlag(FRNM_CHANNEL_IDX, FRNM_ACTIVE_RESTART); /* SBSW_FRNM_CHANNEL_IDX */
    }
    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_NETWORKREQUEST, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_NETWORKREQUEST )
  return retVal;
}

/**********************************************************************************************************************
 *  FrNm_NetworkRelease()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_NetworkRelease( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_NETWORKRELEASE )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* #200 Clear NetworkRequested flag */
    FrNm_SetNetworkRequested(FRNM_CHANNEL_IDX, FALSE); /* SBSW_FRNM_CHANNEL_IDX */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_NETWORKRELEASE, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_NETWORKRELEASE )
  return retVal;
}
#endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

/**********************************************************************************************************************
 *  FrNm_GetState()
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
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetState( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                 CONSTP2VAR( Nm_StateType, AUTOMATIC, FRNM_APPL_VAR ) nmStatePtr,
                                                 CONSTP2VAR( Nm_ModeType, AUTOMATIC, FRNM_APPL_VAR ) nmModePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETSTATE )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# endif
  /* Check parameter 'nmStatePtr' */
  else if( nmStatePtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  /* Check parameter 'nmModePtr' */
  else if( nmModePtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* ESCAN00060592 */
    /* #200 Write current state to nmStatePtr Pointer
     *      Write current mode to nmModePtr Pointer
     */
    *nmStatePtr = FrNm_GetNmState(FRNM_CHANNEL_IDX); /* SBSW_FRNM_GETSTATE */
    /* Set mode respective to the state */
    if(FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_BUS_SLEEP)        /* Bus Sleep */
    {
      *nmModePtr = NM_MODE_BUS_SLEEP; /* SBSW_FRNM_GETSTATE */
    }
    else if(FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_SYNCHRONIZE) /* Synchronize */
    {
      *nmModePtr = NM_MODE_SYNCHRONIZE; /* SBSW_FRNM_GETSTATE */
    }
    else                                                               /* Ready Sleep, Normal Operation, Repeat Message */
    {
      *nmModePtr = NM_MODE_NETWORK; /* SBSW_FRNM_GETSTATE */
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETSTATE, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETSTATE )
  return retVal;
}  /* PRQA S 2006,6010,6030,6080 */ /* MD_MSR_14.7,MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

#if ( FRNM_USER_DATA_ENABLED == STD_ON )
# if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_USER_DATA_SUPPORT == STD_OFF )
/**********************************************************************************************************************
 *  FrNm_SetUserData()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_SetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                    CONSTP2CONST( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmUserDataPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;
  uint8_least    txMessageDataByte;
  uint8_least    userDataByte;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_SETUSERDATA )

  /* ----- Development Error Checks ------------------------------------- */
#  if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#   if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#   else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#    if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#    endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#   endif
  /* Check parameter 'nmUserDataPtr' */
  else if( nmUserDataPtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
#  endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /*
      Note: Even though this function is not re-entered with the same channel handle,
            interrupt locks are required because the NM message might be transmitted from Interrupt Level.
    */

    SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #200 Write User Data into Tx Message Data buffer
     */
    /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
    for( txMessageDataByte = FrNm_GetTxMessageData_UserDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), userDataByte = 0U;
         txMessageDataByte < FrNm_GetTxMessageData_UserDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
         txMessageDataByte++, userDataByte++ )
    {
      FrNm_SetTxMessageData(txMessageDataByte, (FrNm_TxMessageDataType)nmUserDataPtr[userDataByte]); /* SBSW_FRNM_CSL_03 */
    }
    SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    FRNM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_SETUSERDATA, errorId);
  }
#  else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_SETUSERDATA )
  return retVal;
}  /* PRQA S 2006,6010,6030 */ /* MD_MSR_14.7,MD_MSR_STPTH,MD_MSR_STCYC */
# endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COM_USER_DATA_SUPPORT == STD_OFF ) */

/**********************************************************************************************************************
 *  FrNm_GetUserData()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetUserData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                    CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmUserDataPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;
  uint8_least    userDataByte;
  uint8_least    userDataPos;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETUSERDATA )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  /* Check parameter 'nmUserDataPtr' */
  else if ( nmUserDataPtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /*
       Note: Lock the interrupts while copying the user data.
             A NM message reception could be handled within the Rx Interrupt while copying the user data.
    */
    SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_6(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #200 Copy User Data from last received NM message */
    /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
    for( userDataPos = FrNm_GetRxMessageData_UserDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), userDataByte = 0U;
         userDataPos < FrNm_GetRxMessageData_UserDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
         userDataPos++, userDataByte++ )
    {
      nmUserDataPtr[userDataByte] = FrNm_GetRxMessageData(userDataPos); /* SBSW_FRNM_USER_DATA */
    }
    SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_6(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
# if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    FRNM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199, 3353 */ /* MD_MSR_14.2, MD_FrNm_3353 */ /*lint -e{438} */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETUSERDATA, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETUSERDATA )
  return retVal;
}
#endif  /* ( FRNM_USER_DATA_ENABLED == STD_ON ) */

#if ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON )
/**********************************************************************************************************************
 *  FrNm_GetNodeIdentifier()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                          CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmNodeIdPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETNODEIDENTIFIER )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  /* Check parameter 'nmNodeIdPtr' */
  else if ( nmNodeIdPtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* Note: Interrupt Locks not necessary due to one 8bit value is read. */
    /* #200 Copy Node Identifier from last received NM message to the passed pointer */
    *nmNodeIdPtr = FrNm_GetRxMessageData( FrNm_GetRxMessageData_NIDIdxOfPbChannelConfig(FRNM_CHANNEL_IDX) ); /* SBSW_FRNM_NODE_IDENTIFIER */
    retVal = E_OK;
# if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    FRNM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199, 3353 */ /* MD_MSR_14.2, MD_FrNm_3353 */ /*lint -e{438} */
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETNODEIDENTIFIER, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETNODEIDENTIFIER )
  return retVal;
}

/**********************************************************************************************************************
 *  FrNm_GetLocalNodeIdentifier()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetLocalNodeIdentifier( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                               CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR )
                                                               nmNodeIdPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETLOCALNODEIDENTIFIER )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  /* Check parameter 'nmNodeIdPtr' */
  else if ( nmNodeIdPtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* Copy local Node Identifier to the passed pointer */
    *nmNodeIdPtr = FrNm_GetNodeIdOfPbChannelConfig(FRNM_CHANNEL_IDX); /* SBSW_FRNM_NODE_IDENTIFIER */
    retVal = E_OK;
# if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    FRNM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199, 3353 */ /* MD_MSR_14.2, MD_FrNm_3353 */ /*lint -e{438} */
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETLOCALNODEIDENTIFIER, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETLOCALNODEIDENTIFIER )
  return retVal;
}
#endif /* ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON ) */

#if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  FrNm_RepeatMessageRequest()
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
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_RepeatMessageRequest( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_REPEATMESSAGEREQUEST )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
# if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
# endif

    /* #200 If current mode is Network Mode or RepeatMessageRequestBitActive is configured
     *       Set RepeatMsgRequestIndFlag for further handling in main function
     */
# if ( FRNM_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_ON ) /* ESCAN00089668 */
    if(
#  if ( FRNM_ISDEF_REPEATMESSAGEREQUESTBITACTIVEOFCHANNELCONFIG == STD_OFF )
        ( FrNm_IsRepeatMessageRequestBitActiveOfChannelConfig(FRNM_CHANNEL_IDX) ) &&
#  endif
        ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_BUS_SLEEP ) &&
        ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_SYNCHRONIZE ) )
    {
      FrNm_SetRepeatMsgRequestIndFlag(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */
      retVal = E_OK;
    }
# endif
    FRNM_DUMMY_STATEMENT(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_REPEATMESSAGEREQUEST, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_REPEATMESSAGEREQUEST )
  return retVal;
}
#endif /* ( FRNM_NODE_DETECTION_ENABLED == STD_ON ) */

#if ( ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) || \
      ( FRNM_USER_DATA_ENABLED == STD_ON ) )
/**********************************************************************************************************************
 *  FrNm_GetPduData()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_GetPduData( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                   CONSTP2VAR( uint8, AUTOMATIC, FRNM_APPL_VAR ) nmPduData )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;
  uint8_least    msgDataByte;
  uint8_least    msgDataPos;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETPDUDATA )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  /* Check parameter 'nmPduData' */
  else if ( nmPduData == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /*
       Note: Lock the interrupts while copying the user data.
             A NM message reception could be handled within the Rx Interrupt while copying the user data.
             (Only relevant if the copying takes more than one cycle. E.g. 16bit micros.)
    */
    SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_6(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /*
     * #200 Copy last received message data to passed pointer.
     */
    /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
    for( msgDataPos = FrNm_GetRxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), msgDataByte = 0U;
         msgDataPos < FrNm_GetRxMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
         msgDataPos++, msgDataByte++ )
    {
      nmPduData[msgDataByte] = FrNm_GetRxMessageData(msgDataPos); /* SBSW_FRNM_PDU_DATA */
    }
    SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_6(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    FRNM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199, 3353 */ /* MD_MSR_14.2, MD_FrNm_3353 */ /*lint -e{438} */
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETPDUDATA, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETPDUDATA )
  return retVal;
}
#endif /* ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ||
          ( FRNM_USER_DATA_ENABLED == STD_ON ) */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  FrNm_RequestBusSynchronization()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_RequestBusSynchronization( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_REQUESTBUSSYNC )
  /* No Init and channel checks are performed because this function does not implement any functionality */

  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_REQUESTBUSSYNC )

  return E_OK;
}
#endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  FrNm_CheckRemoteSleepIndication()
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
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_CheckRemoteSleepIndication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                                   CONSTP2VAR( boolean, AUTOMATIC, FRNM_APPL_VAR )
                                                                   nmRemoteSleepIndPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_CHECKREMOTESLEEPIND )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# endif
  /* Check parameter 'nmRemoteSleepIndPtr' */
  else if( nmRemoteSleepIndPtr == NULL_PTR )
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* #200 If current Mode is NetworkMode
     *       Copy current RemoteSleepIndState to passed pointer
     *      otherwise
     *       return E_NOT_OK
     */
    if( ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_BUS_SLEEP ) &&
        ( FrNm_GetNmState(FRNM_CHANNEL_IDX) != NM_STATE_SYNCHRONIZE ) )
    {
      *nmRemoteSleepIndPtr = (boolean)FrNm_IsRemoteSleepIndState(FRNM_CHANNEL_IDX); /* SBSW_FRNM_CHECKREMOTESLEEP */
      retVal = E_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_CHECKREMOTESLEEPIND, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_CHECKREMOTESLEEPIND )
  return retVal;
}
#endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON ) */

#if ( FRNM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  FrNm_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, FRNM_CODE ) FrNm_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, FRNM_APPL_VAR ) NmVerInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_GETVERSIONINFO )

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check parameter 'NmVerInfoPtr' */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  if (NmVerInfoPtr == NULL_PTR)
  {
    errorId = FRNM_E_INVALID_POINTER;
  }
  else
  /* #20 If parameter checks are passed */
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #200 Provide version information */
    NmVerInfoPtr->vendorID = (FRNM_VENDOR_ID); /* SBSW_FRNM_GETVERSIONINFO */
    NmVerInfoPtr->moduleID = (FRNM_MODULE_ID); /* SBSW_FRNM_GETVERSIONINFO */
    /* ESCAN00061349 */
    NmVerInfoPtr->sw_major_version = (FRNM_SW_MAJOR_VERSION); /* SBSW_FRNM_GETVERSIONINFO */
    NmVerInfoPtr->sw_minor_version = (FRNM_SW_MINOR_VERSION); /* SBSW_FRNM_GETVERSIONINFO */
    NmVerInfoPtr->sw_patch_version = (FRNM_SW_PATCH_VERSION); /* SBSW_FRNM_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FRNM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FRNM_E_NO_ERROR)
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_GETVERSIONINFO, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_GETVERSIONINFO )
}
#endif  /* ( FRNM_VERSION_INFO_API == STD_ON ) */

/**********************************************************************************************************************
 *  FrNm_InitMemory()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, FRNM_CODE ) FrNm_InitMemory( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #20 Set global state to not initialized */
  FrNm_SetModuleInitialized(FALSE);
#endif
#if (FRNM_USE_INIT_POINTER == STD_ON)
  /* #30 Set config pointer to NULL_PTR */
  FrNm_ConfigDataPtr = NULL_PTR;
#endif
}

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  FrNm_SetSleepReadyBit()
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
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_SetSleepReadyBit( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle,
                                                         CONST( boolean, AUTOMATIC ) nmSleepReadyBit )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_SETSLEEPREADYBIT )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF ) /* COV_FRNM_COORDINATOR_SUPPORT TX */
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #200 If nmSleepReadyBit is TRUE
     *       Set NM Coordinator Sleep Ready Bit in the TX NM message (Bit 3)
     *      otherwise
     *       Clear NM Coordinator Sleep Ready Bit in the TX NM message (Bit 3)
     */
    if ( nmSleepReadyBit == TRUE )
    {
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) | NM_COORD_BIT_SLEEP));
    }
    else
    {
      FrNm_SetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), (FrNm_TxMessageDataType) /* SBSW_FRNM_CSL_03 */
                            (FrNm_GetTxMessageData(FrNm_GetTxMessageData_CBVIdxOfPbChannelConfig(FRNM_CHANNEL_IDX)) & NM_COORD_BIT_INV_SLEEP));
    }
    retVal = E_OK;
    SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    FRNM_DUMMY_STATEMENT(FRNM_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_SETSLEEPREADYBIT, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_SETSLEEPREADYBIT )
  return retVal;

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON ) */

/**********************************************************************************************************************
 *  FrNm_RxIndication()
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
 *
 *
 *
 */
FUNC( void, FRNM_CODE ) FrNm_RxIndication( VAR( PduIdType, AUTOMATIC ) FrNmRxPduId,
                                           P2CONST(PduInfoType, AUTOMATIC, FRNM_APPL_VAR) PduInfoPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_RXINDICATION )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'RxPduId' */
  else if
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
        ( (FrNmRxPduId >> 1U) != FRNM_CHANNEL_IDX )
# else
#  if ( FRNM_PDUIDTOCHINDEX == STD_ON )
        ( FrNmRxPduId >= (NetworkHandleType)FrNm_GetSizeOfPduIdToChIndex() )
#  else
        ( (FrNmRxPduId >> 1U) >= (NetworkHandleType)FrNm_GetSizeOfChannelConfig() )
#  endif
# endif
  {
    errorId = FRNM_E_PDU_ID_INVALID;
  }
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
# if ( FRNM_PDUIDTOCHINDEX == STD_ON )
    channel = (NetworkHandleType)(FrNm_GetPduIdToChIndex(FrNmRxPduId) >> 1U);
# else
    channel = (NetworkHandleType)(FrNmRxPduId >> 1U);
# endif
#endif
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    /* Check parameter 'PduInfoPtr' */
    if( (PduInfoPtr) == NULL_PTR )
    {
      errorId = FRNM_E_INVALID_POINTER;
    }
    /* Check parameter 'SduDataPtr' */
    else if( (PduInfoPtr->SduDataPtr) == NULL_PTR )
    {
      errorId = FRNM_E_INVALID_POINTER;
    }
# if( FRNM_RXMESSAGEDATA == STD_ON )
    /* Check parameter 'SduLength' */
    else if( (PduInfoPtr->SduLength) != FrNm_GetRxMessageDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX) )
    {
      errorId = FRNM_E_RXINDICATION_DLC_ERROR;
    }
# endif
    else
#endif
    {
      /* #200 Copy received vote data
       *      Copy received message data
       *      Process received NM Coordinator Sleep Ready Bit
       *      Process received PNC information and apply filters
       *      Process CarWakeupIndication handling
       *      Set Message Indication Flag for further processing in main function
       *      If SyncPduMasterEnabled is configured
       *       Set Send Sync Pdus Flag for further processing in main function
       *      Notify message reception to NmIf
       */
# if ( FRNM_PDU_SCHEDULE_VARIANT_ONLY == 0U ) || \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY > 2u ) || \
     ( ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 7U ) && \
       ( ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) || \
         ( ( FRNM_PDU_SCHEDULE_VARIANT_3 == STD_ON ) || \
           ( FRNM_PDU_SCHEDULE_VARIANT_4 == STD_ON ) || \
           ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) ) ) ) ) && \
           ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY <= 1U ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf tx tf xf xf tf tf tf */
      /*lint -e522 */
      FrNm_CopyRxVoteData(FRNM_CHANNEL_PARA_FIRST  FrNmRxPduId, PduInfoPtr->SduDataPtr[0U]);
      /*lint +e522 */
#endif

#if ( FRNM_RXMESSAGEDATA == STD_ON )
      FrNm_CopyRxMessageData( FRNM_CHANNEL_PARA_FIRST  PduInfoPtr->SduDataPtr ); /* SBSW_FRNM_LOCALFUNCTION_CALL */
#endif

#if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
      FrNm_CoordReadyToSleepHandling( FRNM_CHANNEL_PARA_FIRST  PduInfoPtr->SduDataPtr[0U] );
#endif

#if ( FRNM_PN_FEATURE_ENABLED == STD_ON ) && defined ( FRNM_PN_CALC_FEATURE_ENABLED )
      FrNm_RxPnFilterHandling(FRNM_CHANNEL_PARA_FIRST  PduInfoPtr->SduDataPtr ); /* SBSW_FRNM_LOCALFUNCTION_CALL */
#endif

#if ( FRNM_CARWAKEUPRXENABLEDOFCHANNELCONFIG == STD_ON )
      FrNm_CarWakeupIndication(FRNM_CHANNEL_PARA_ONLY);
#endif

      /* Store Indication for handling in the task */
      FrNm_SetMsgIndicationFlag(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */

#if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
      /* ESCAN00082307 */
# if ( FRNM_ISDEF_SYNCPDUMASTERENABLEDOFCHANNELCONFIG == STD_OFF )
      if ( FrNm_IsSyncPduMasterEnabledOfChannelConfig(FRNM_CHANNEL_IDX) )
# endif
      {
        /* Set decision flag to send a sync PDU */
        FrNm_SetSendSyncPdusFlag(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */
      }
#endif

#if ( FRNM_PDU_RX_INDICATION_ENABLED == STD_ON )
      /* SPEC-16122: Notify NM Interface that a message has been received */
      Nm_PduRxIndication( FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) );
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_RXINDICATION, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FRNM_DUMMY_STATEMENT(FrNmRxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_RXINDICATION )
} /* PRQA S 6050,6080 */ /* MD_MSR_STCAL,MD_MSR_STMIF */

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  FrNm_TxConfirmation()
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
FUNC( void, FRNM_CODE ) FrNm_TxConfirmation( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_TXCONFIRMATION )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'FrNmTxPduId' */
  else if
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
      ( (FrNmTxPduId >> 1U) != FRNM_CHANNEL_IDX )
# else
#  if ( FRNM_PDUIDTOCHINDEX == STD_ON )
      ( FrNmTxPduId >= (NetworkHandleType)FrNm_GetSizeOfPduIdToChIndex() )
#  else
      ( (FrNmTxPduId >> 1U) >= (NetworkHandleType)FrNm_GetSizeOfChannelConfig() )
#  endif
# endif
  {
    errorId = FRNM_E_PDU_ID_INVALID;
  }
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
# if ( FRNM_PDUIDTOCHINDEX == STD_ON )
    channel = (NetworkHandleType)(FrNm_GetPduIdToChIndex(FrNmTxPduId) >> 1U);
# else
    channel = (NetworkHandleType)(FrNmTxPduId >> 1U);
# endif
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    if( channel >= FrNm_GetSizeOfChannelConfig() ) /* COV_FRNM_TXPDUID */
    {
      errorId = FRNM_E_INVALID_CHANNEL;
    }
    else
# endif
#endif
    {
      /* #200 Set Message Confirmation Flag for further processing in main function
       *      Notify successful transmission of User Data to PduR
       */
      FrNm_SetMsgConfirmationFlag(FRNM_CHANNEL_IDX, TRUE); /* SBSW_FRNM_CHANNEL_IDX */

#if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) && ( FRNM_PDUR_FRNM_TXCONFIRMATION == STD_ON )
      /* ESCAN00070927 */
      PduR_FrNmTxConfirmation(FrNm_GetTxUserDataPduIdOfPbChannelConfig(FRNM_CHANNEL_IDX));
#endif

    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_TXCONFIRMATION, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED ) && ( FRNM_DEV_ERROR_DETECT == STD_OFF ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  FRNM_DUMMY_STATEMENT(FrNmTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_TXCONFIRMATION )
}

/**********************************************************************************************************************
 *  FrNm_TriggerTransmit()
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
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 2 */ /* MD_FrNm_3673 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_TriggerTransmit( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId,
                                                        P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_VAR) PduInfoPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( FRNM_IMMEDIATE_TX_ENABLED == STD_ON )
  /* No functionality is required as all NM Tx PDUs are configured for immediate sending */
  Std_ReturnType retVal = E_OK;
  FRNM_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  uint8          errorId = FRNM_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least    txMsgByte;
  uint8_least    txMsgPosition;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_TRIGGERTRANSMIT )

  /* ----- Development Error Checks ------------------------------------- */
#  if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'FrNmTxPduId' */
  else if
#   if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
     ( (FrNmTxPduId >> 1U) != FRNM_CHANNEL_IDX )
#   else
#    if ( FRNM_PDUIDTOCHINDEX == STD_ON )
     ( FrNmTxPduId >= (NetworkHandleType)FrNm_GetSizeOfPduIdToChIndex() )
#    else
     ( (FrNmTxPduId >> 1U) >= (NetworkHandleType)FrNm_GetSizeOfChannelConfig() )
#    endif
#   endif
  {
    errorId = FRNM_E_PDU_ID_INVALID;
  }
  else
#  endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#  if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
#   if ( FRNM_PDUIDTOCHINDEX == STD_ON )
    channel = (NetworkHandleType)(FrNm_GetPduIdToChIndex(FrNmTxPduId) >> 1U);
#   else
    channel = (NetworkHandleType)(FrNmTxPduId >> 1U);
#   endif
#   if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    if( channel >= FrNm_GetSizeOfChannelConfig() ) /* COV_FRNM_TXPDUID */
    {
      errorId = FRNM_E_INVALID_CHANNEL;
    }
    else
#   endif
#  endif
#  if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
    /* Check parameter 'PduInfoPtr' */
    if( (PduInfoPtr) == NULL_PTR )
    {
      errorId = FRNM_E_INVALID_POINTER;
    }
    /* Check parameter 'SduDataPtr' */
    else if( (PduInfoPtr->SduDataPtr) == NULL_PTR )
    {
      errorId = FRNM_E_INVALID_POINTER;
    }
    else
#  endif
    {
      /* #200 If Tx Pdu Id is odd ( Schedule Variant > 2 )
       *       If Schedule Variant is ( 5 or 6 ) and message transmission is disallowed
       *        Set PduLength to zero and return E_NOT_OK
       *       otherwise (in all other Schedule Variants)
       *        Prepare vote data for transmission
       *      otherwise
       *       If ScheduleVariant is 2,4,6,7 and message transmission is disallowed
       *        Set PduLength to zero and return E_NOT_OK
       *       otherwise
       *        Fetch User Data from PduR
       *        Copy the available data to the provided data
       */
      /* SPEC-15884: Copy Tx data for the corresponding PDU */
      if (PduInfoPtr->SduLength >= FrNm_GetTxMessageDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX) )
      {
#  if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) || \
          ( FRNM_PDU_SCHEDULE_VARIANT_ONLY > 2U ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf xf */
        if( (FrNmTxPduId & 0x01U) == 0x01U )
        {
#   if ( FRNM_COM_CONTROL_ENABLED == STD_ON ) && \
       ( ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) || \
         ( FRNM_PDU_SCHEDULE_VARIANT_5 == STD_ON ) || \
         ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tx xf xf */
          if (
#    if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF ) /* COV_FRNM_UNSUPPORTED_VARIANT TX */ /* Cannot be fully covered, since ScheduleVariant 5 and 6 are also not covered. */
               (
                 ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 5U ) ||
                 ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 6U )
               ) &&
#    endif
               ( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_DISABLED ) /* ESCAN00063844 */
             ) /* COV_FRNM_COM_CONTROL_VARIANT XF xf xf xx */
          {
            /* Vote Pdu should be sent in the dynamic segment and shall not be sent,
             * because FrNm_DisableCommunication has been called
             */
            PduInfoPtr->SduLength = 0U; /* SBSW_FRNM_TRIGGERTRANSMIT */
          }
          else
#   endif
          {
            PduInfoPtr->SduDataPtr[0U] = FrNm_GetTxMessageVote(FRNM_CHANNEL_IDX); /* SBSW_FRNM_TRIGGERTRANSMIT */
            PduInfoPtr->SduLength = 1U; /* SBSW_FRNM_TRIGGERTRANSMIT */
            retVal = E_OK;
          }
        }
        else
#  endif
        {
#  if ( FRNM_COM_CONTROL_ENABLED == STD_ON ) && \
      ( ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U ) && \
        ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 3U ) && \
        ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 5U ) ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tf tx tx */
          if (
#   if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
               (
                 /* Pdu Schedule Variants 2,4,6,7: NM Data Pdu is sent in the dynamic segment */
                 ( (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) & 0x01U) == 0U ) ||
                 ( FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) == 7U )
               ) &&
#   endif
               ( FrNm_GetTxControlState(FRNM_CHANNEL_IDX) == MSG_TX_DISABLED ) /* ESCAN00063844 */
             ) /* COV_FRNM_UNSUPPORTED_VARIANT_AND_COM_CONTROL */
          {
            /* Data Pdu should be sent in the dynamic segment and shall not be sent,
             * because FrNm_DisableCommunication has been called
             */
            PduInfoPtr->SduLength = 0U; /* SBSW_FRNM_TRIGGERTRANSMIT */
          }
          else
#  endif
          {
#  if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON )
            /* Get User Data via PduR / Com */
            VAR( PduInfoType, AUTOMATIC ) PduInfo;

            /* ESCAN00078357 */
            PduInfo.SduDataPtr = FrNm_GetAddrTxMessageData(FrNm_GetTxMessageData_UserDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX));
            /* ESCAN00087841 */
            PduInfo.SduLength = FrNm_GetTxMessageData_UserDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX);

            (void)PduR_FrNmTriggerTransmit( FrNm_GetTxUserDataPduIdOfPbChannelConfig(FRNM_CHANNEL_IDX), &PduInfo ); /* SBSW_FRNM_FUNC_CALL_WITH_POINTER */
            /* ESCAN00060592 */
#   if ( FRNM_DEV_ERROR_REPORT == STD_ON )
            if( PduInfo.SduLength != FrNm_GetTxMessageData_UserDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX) )
            {
              errorId = FRNM_E_PDUR_TRIGGERTX_ERROR;
            }
#   endif
#  endif
            SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            /* PRQA S 3418 3 */ /* MD_FrNm_3418 */
            for( txMsgPosition = FrNm_GetTxMessageDataStartIdxOfPbChannelConfig(FRNM_CHANNEL_IDX), txMsgByte = 0U;
                 txMsgPosition < FrNm_GetTxMessageDataEndIdxOfPbChannelConfig(FRNM_CHANNEL_IDX);
                 txMsgPosition++, txMsgByte++ )
            {
              PduInfoPtr->SduDataPtr[txMsgByte] = FrNm_GetTxMessageData(txMsgPosition); /* SBSW_FRNM_TRIGGERTRANSMIT */
            }
            SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            PduInfoPtr->SduLength = FrNm_GetTxMessageDataLengthOfPbChannelConfig(FRNM_CHANNEL_IDX); /* SBSW_FRNM_TRIGGERTRANSMIT */
            retVal = E_OK;
          }
        }
      }
# if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
      FRNM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_TRIGGERTRANSMIT, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT( errorId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#endif /* ( FRNM_IMMEDIATE_TX_ENABLED == STD_OFF ) */

  FRNM_DUMMY_STATEMENT( FrNmTxPduId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_TRIGGERTRANSMIT )
  return retVal;
} /* PRQA S 6010,6080,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STMIF,MD_MSR_STCYC,MD_MSR_STCAL */
#endif /* ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) */

/**********************************************************************************************************************
 *  FrNm_StartupError()
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
FUNC( void, FRNM_CODE ) FrNm_StartupError( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = FRNM_E_NO_ERROR;
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
  Nm_StateType   lastState;
#endif

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_STARTUPERROR )

  /* ----- Development Error Checks ------------------------------------- */
#if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
# if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#  if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#  endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
# endif
  else
#endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
    /* #200 If communication is not requested and
     *      state is Synchronize (<CycleCounterEmulation> is ON)
     *       store current Nm state temporarily
     *       Re-Initialize FrNm ( Transition to BusSleep )
     *       Notify state change to NmIf
     *       Notify entering of Bus Sleep Mode to NmIf
     */
    if( ( !FrNm_IsNetworkRequested(FRNM_CHANNEL_IDX) )
# if ( FRNM_CYCLE_COUNTER_EMULATION == STD_ON )
          && ( FrNm_GetNmState(FRNM_CHANNEL_IDX) == NM_STATE_SYNCHRONIZE )
# endif
      )
#endif
    {
#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
      lastState = FrNm_GetNmState(FRNM_CHANNEL_IDX);
#endif
      SchM_Enter_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      FrNm_LocalInit( FRNM_CHANNEL_PARA_ONLY );
      SchM_Exit_FrNm_FRNM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON )
      Nm_StateChangeNotification( NetworkHandle, lastState, NM_STATE_BUS_SLEEP );
#endif
      Nm_BusSleepMode( NetworkHandle ); /* SPEC-15921 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_STARTUPERROR, errorId);
  }
#else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_STARTUPERROR )
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) || defined ( FRNM_PN_CALC_FEATURE_ENABLED )
/* ESCAN00078214 */
/**********************************************************************************************************************
 *  FrNm_Transmit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_Transmit( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId,
                                                 P2CONST( PduInfoType, AUTOMATIC, FRNM_APPL_VAR ) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* No Init and channel checks are performed because this function does not implement any functionality */

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_TRANSMIT )

  FRNM_DUMMY_STATEMENT(FrNmTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FRNM_DUMMY_STATEMENT(PduInfoPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_TRANSMIT )

# if ( FRNM_PASSIVE_MODE_ENABLED == STD_ON )
  return E_NOT_OK;
# else
  return E_OK;
# endif
}
#endif

#if ( FRNM_COM_CONTROL_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  FrNm_EnableCommunication()
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
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_EnableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_ENABLECOMMUNICATION )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
#  if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U ) && \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 3U ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tf tx */
#   if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#   endif

    /* #200 If Mode is Network Mode ( Repeat Message, Ready Sleep or Normal Operation state ) AND
     *      Schedule Variant is not 1 and not 3
     *       Allow message transmission
     */
    /* No use to call this function for Pdu Schedule Variant 1 or 3 because
     * the NM Vote and NM Data PDUs are sent in the static segment and sending them cannot be disabled,
     * thus sending them cannot be re-enabled
     */ /* ESCAN00063844 */
    if ( (FrNm_GetNmState(FRNM_CHANNEL_IDX) >= NM_STATE_READY_SLEEP) &&
         (FrNm_GetNmState(FRNM_CHANNEL_IDX) <= NM_STATE_REPEAT_MESSAGE) /* ESCAN00063840 */
#   if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
         /* ESCAN00063844 */
         && (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 1U)
         && (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 3U)
#   endif
       ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tf tf tx */
    {
      FrNm_SetTxControlStateRequest(FRNM_CHANNEL_IDX, MSG_TX_ENABLED); /* SBSW_FRNM_CHANNEL_IDX */
      retVal = E_OK;
    }
#  endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_ENABLECOMMUNICATION, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_ENABLECOMMUNICATION )
  return retVal;
}  /* PRQA S 2006,6010,6030 */ /* MD_MSR_14.7,MD_MSR_STPTH,MD_MSR_STCYC */

/**********************************************************************************************************************
 *  FrNm_DisableCommunication()
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
 */
FUNC( Std_ReturnType, FRNM_CODE ) FrNm_DisableCommunication( CONST( NetworkHandleType, AUTOMATIC ) NetworkHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8          errorId = FRNM_E_NO_ERROR;

  FrNm_Rtm_Start( RtmMeasurementPoint_FRNM_SID_DISABLECOMMUNICATION )

  /* ----- Development Error Checks ------------------------------------- */
# if ( FRNM_DEV_ERROR_DETECT == STD_ON ) /* COV_FRNM_DEV_ERROR_DETECT_ON */
  /* #10 Check that module is initialized. */
  if ( !FrNm_IsModuleInitialized() )
  {
    errorId = FRNM_E_UNINIT;
  }
  /* Check parameter 'NetworkHandle' */
#  if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
  else if( NetworkHandle != FrNm_GetChannelIdOfChannelConfig(FRNM_CHANNEL_IDX) )
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  else
  else if( ( NetworkHandle >= FrNm_GetSizeOfSysToNmChInd() )
#   if ( FRNM_ISDEF_SYSTONMCHIND == STD_OFF )
        || ( FrNm_GetSysToNmChInd(NetworkHandle) >= FrNm_GetSizeOfChannelConfig() )
#   endif
         ) /* COV_FRNM_SYSTONMCHANNELIND */
  {
    errorId = FRNM_E_INVALID_CHANNEL;
  }
#  endif
  else
# endif
  /* #20 If module is initialized and parameter checks are passed */
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) && \
    ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 1U ) && \
    ( FRNM_PDU_SCHEDULE_VARIANT_ONLY != 3U ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tf tx */
#if !defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED )
    NetworkHandleType channel;
    channel = FrNm_GetSysToNmChInd(NetworkHandle);
#endif

    /* #200 If Mode is Network Mode ( Repeat Message, Ready Sleep or Normal Operation state ) AND
     *      Schedule Variant is not 1 and not 3
     *       Disallow message transmission
     */
    /* No use to call this function for Pdu Schedule Variant 1 or 3 because
     * the NM Vote and NM Data PDUs are sent in the static segment and sending them cannot be disabled
     */ /* ESCAN00063844 */
    if ( (FrNm_GetNmState(FRNM_CHANNEL_IDX) >= NM_STATE_READY_SLEEP) &&
         (FrNm_GetNmState(FRNM_CHANNEL_IDX) <= NM_STATE_REPEAT_MESSAGE) /* ESCAN00063840 */
#  if ( FRNM_ISDEF_PDUSCHEDULEVARIANTOFCHANNELCONFIG == STD_OFF )
         /* ESCAN00063844 */
         && (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 1U)
         && (FrNm_GetPduScheduleVariantOfChannelConfig(FRNM_CHANNEL_IDX) != 3U)
#  endif
       ) /* COV_FRNM_UNSUPPORTED_VARIANT TF tf tf tf tx */
    {
      FrNm_SetTxControlStateRequest(FRNM_CHANNEL_IDX, MSG_TX_DISABLED); /* SBSW_FRNM_CHANNEL_IDX */
      retVal = E_OK;
    }
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if ( FRNM_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report errors if applicable */
  if ( errorId != FRNM_E_NO_ERROR )
  {
    (void)Det_ReportError(FRNM_MODULE_ID, FRNM_INSTANCE_ID, FRNM_SID_DISABLECOMMUNICATION, errorId);
  }
# else
  FRNM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif
  FRNM_DUMMY_STATEMENT_CONST(NetworkHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FrNm_Rtm_Stop( RtmMeasurementPoint_FRNM_SID_DISABLECOMMUNICATION )
  return retVal;
}
#endif  /* ( FRNM_COM_CONTROL_ENABLED == STD_ON ) */

#define FRNM_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*******************************************************************************
Verify the configuration of the FlexRay Network Management
*******************************************************************************/

/* Check Channel Configuration switch */
#if !( ( defined FRNM_NUMBER_OF_FRNM_CHANNELS ) && ( FRNM_NUMBER_OF_FRNM_CHANNELS > 0U ) )
# error: "FlexRay NM configuration issue: Number of FlexRay Channels has to be defined greater 0."
#endif
#if !( ( defined FRNM_NUMBER_OF_SYS_CHANNELS ) && ( FRNM_NUMBER_OF_SYS_CHANNELS > 0U ) )
# error "FlexRay NM configuration issue: The number of System NM channels has to be defined greater 0."
#endif

/* Check schedule variant */
#if !( ( defined FRNM_PDU_SCHEDULE_VARIANT_ONLY ) && \
     ( FRNM_PDU_SCHEDULE_VARIANT_ONLY >= 0U ) && ( FRNM_PDU_SCHEDULE_VARIANT_ONLY < 8U ) )
# error: "FlexRay NM configuration issue: FlexRay Schedule Variant has to be defined with a value between 0 and 7"
#endif

/* Check FlexRay Nm Configuration switches */
#if !defined ( FRNM_CONFIGURATION_VARIANT_PRECOMPILE )
# error: "FlexRay NM configuration issue: Config Variant Pre-Compile has to be defined"
#endif
#if !defined ( FRNM_CONFIGURATION_VARIANT_LINKTIME )
# error: "FlexRay NM configuration issue: Config Variant Link-Time has to be defined"
#endif
#if !defined ( FRNM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
# error: "FlexRay NM configuration issue: Config Variant Post-Build-Loadable has to be defined"
#endif

#if !( ( defined FRNM_CONFIGURATION_VARIANT ) && \
      ( ( FRNM_CONFIGURATION_VARIANT == FRNM_CONFIGURATION_VARIANT_PRECOMPILE ) || \
        ( FRNM_CONFIGURATION_VARIANT == FRNM_CONFIGURATION_VARIANT_LINKTIME ) || \
        ( FRNM_CONFIGURATION_VARIANT == FRNM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) ) )
# error: "FlexRay NM configuration issue: Config Variant has to be defined to Pre-Compile, Link-Time, or Post-Build-Loadable"
#endif
#if !( ( defined FRNM_VERSION_INFO_API ) && \
       ( ( FRNM_VERSION_INFO_API == STD_OFF ) || ( FRNM_VERSION_INFO_API == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Version Info Api has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_USER_DATA_ENABLED ) && \
       ( ( FRNM_USER_DATA_ENABLED == STD_OFF ) || ( FRNM_USER_DATA_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: User Data Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_NODE_DETECTION_ENABLED ) && \
       ( ( FRNM_NODE_DETECTION_ENABLED == STD_OFF ) || ( FRNM_NODE_DETECTION_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Node Detection Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_SOURCE_NODE_IDENTIFIER_ENABLED ) && \
       ( ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_OFF ) || ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Source Node Identifier Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_PASSIVE_MODE_ENABLED ) && \
       ( ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF ) || ( FRNM_PASSIVE_MODE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Passive Mode Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_PDU_RX_INDICATION_ENABLED ) && \
       ( ( FRNM_PDU_RX_INDICATION_ENABLED == STD_OFF ) || ( FRNM_PDU_RX_INDICATION_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Pdu Rx Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_STATE_CHANGE_INDICATION_ENABLED ) && \
       ( ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_OFF ) || ( FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: State Change Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_REMOTE_SLEEP_INDICATION_ENABLED ) && \
       ( ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_OFF ) || ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Remote Sleep Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_BUS_SYNCHRONIZATION_ENABLED ) && \
       ( ( FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_OFF ) || ( FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Bus Synchronization Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_DEV_ERROR_DETECT ) && \
       ( ( FRNM_DEV_ERROR_DETECT == STD_OFF ) || ( FRNM_DEV_ERROR_DETECT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Development Error Detection Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_CONTROL_BIT_VECTOR_ENABLED ) && \
       ( ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_OFF ) || ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Control Bit Vector Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_COORDINATOR_SYNC_SUPPORT ) && \
       ( ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_OFF ) || ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Coordinator Sync Support has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_CYCLE_COUNTER_EMULATION ) && \
       ( ( FRNM_CYCLE_COUNTER_EMULATION == STD_OFF ) || ( FRNM_CYCLE_COUNTER_EMULATION == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Cycle Counter Emulation has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_REPEAT_MESSAGE_BIT_ENABLED ) && \
       ( ( FRNM_REPEAT_MESSAGE_BIT_ENABLED == STD_OFF ) || ( FRNM_REPEAT_MESSAGE_BIT_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Repeat Message Bit Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_HW_VOTE_ENABLED ) && \
       ( ( FRNM_HW_VOTE_ENABLED == STD_OFF ) || ( FRNM_HW_VOTE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Hardware Vector NM Vote Support Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_DUAL_CHANNEL_PDU_ENABLED ) && \
       ( ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_OFF ) || ( FRNM_DUAL_CHANNEL_PDU_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Dual Channel PDU Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_IMMEDIATE_TX_ENABLED ) && \
       ( ( FRNM_IMMEDIATE_TX_ENABLED == STD_OFF ) || ( FRNM_IMMEDIATE_TX_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Immediate Transmission has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_COM_USER_DATA_SUPPORT ) && \
       ( ( FRNM_COM_USER_DATA_SUPPORT == STD_OFF ) || ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: User Data Support via Com has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_COM_CONTROL_ENABLED ) && \
       ( ( FRNM_COM_CONTROL_ENABLED == STD_OFF ) || ( FRNM_COM_CONTROL_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Communication Control Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_VOTING_NEXT_TO_LAST_REPETITION_CYCLE_DISABLE ) && \
       ( ( FRNM_VOTING_NEXT_TO_LAST_REPETITION_CYCLE_DISABLE == STD_OFF ) || \
         ( FRNM_VOTING_NEXT_TO_LAST_REPETITION_CYCLE_DISABLE == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Voting Next To Last Repetition Cycle Disable has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_RUNTIME_MEASUREMENT_SUPPORT ) && \
       ( ( FRNM_RUNTIME_MEASUREMENT_SUPPORT == STD_OFF ) || ( FRNM_RUNTIME_MEASUREMENT_SUPPORT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Runtime Measurement Support has to be defined STD_OFF or STD_ON"
#endif
/* ESCAN00070927 */
#if !( ( defined FRNM_PDUR_FRNM_TXCONFIRMATION ) && \
       ( ( FRNM_PDUR_FRNM_TXCONFIRMATION == STD_OFF ) || ( FRNM_PDUR_FRNM_TXCONFIRMATION == STD_ON ) ) )
# error: "FlexRay NM configuration issue: PduR FrNm TxConfirmation has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_CAR_WUP_FILTER_FEATURE_ENABLED ) && \
       ( ( FRNM_CAR_WUP_FILTER_FEATURE_ENABLED == STD_OFF ) || ( FRNM_CAR_WUP_FILTER_FEATURE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Car Wup Filter Feature Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_PN_FEATURE_ENABLED ) && \
       ( ( FRNM_PN_FEATURE_ENABLED == STD_OFF ) || ( FRNM_PN_FEATURE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: PN Feature Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_PN_ERA_CALC_FEATURE_ENABLED ) && \
       ( ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_OFF ) || ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: PN ERA Calc Feature Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_PN_EIRA_CALC_FEATURE_ENABLED ) && \
       ( ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_OFF ) || ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: PN EIRA Calc Feature Enabled has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_USE_DUMMY_STATEMENT ) && \
       ( ( FRNM_USE_DUMMY_STATEMENT == STD_OFF ) || ( FRNM_USE_DUMMY_STATEMENT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Use Dummy Statement has to be defined STD_OFF or STD_ON"
#endif
#if !( ( defined FRNM_POSTBUILD_VARIANT_SUPPORT ) && \
       ( ( FRNM_POSTBUILD_VARIANT_SUPPORT == STD_OFF ) || ( FRNM_POSTBUILD_VARIANT_SUPPORT == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Postbuild Variant Support has to be defined STD_OFF or STD_ON."
#endif
#if !( ( defined FRNM_USE_INIT_POINTER ) && \
       ( ( FRNM_USE_INIT_POINTER == STD_OFF ) || ( FRNM_USE_INIT_POINTER == STD_ON ) ) )
# error: "FlexRay NM configuration issue: Use Init Pointer has to be defined STD_OFF or STD_ON."
#endif
#if !( ( defined FRNM_ISDEF_PNEIRACALCENABLED ) && \
       ( ( FRNM_ISDEF_PNEIRACALCENABLED == STD_OFF ) || ( FRNM_ISDEF_PNEIRACALCENABLED == STD_ON ) ) )
# error: "FlexRay NM configuration issue: IsDef Pn Eira Calc Enabled has to be defined STD_OFF or STD_ON."
#endif
#if !( ( defined FRNM_ISDEF_PNINFOLENGTH ) && \
       ( ( FRNM_ISDEF_PNINFOLENGTH == STD_OFF ) || ( FRNM_ISDEF_PNINFOLENGTH == STD_ON ) ) )
# error: "FlexRay NM configuration issue: IsDef Pn Info Length has to be defined STD_OFF or STD_ON."
#endif

#if !( ( defined FRNM_SYNC_PDU_FEATURE_ENABLED ) && \
       ( ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_OFF ) || ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON ) ) )
/* ESCAN00082307 */
# error: "FlexRay NM configuration issue: Sync Pdu Feature Enabled has to be defined STD_OFF or STD_ON"
#endif

/* Check maximal number of Channels */
#if ( FRNM_NUMBER_OF_FRNM_CHANNELS > 128U )
# error: "FlexRay NM configuration issue: At most 128 NM channels are supported"
#endif

/* Statically Check configuration in case of a single channel config with indirection */
#if ( FRNM_SYSTONMCHIND == STD_ON )
# if ( FRNM_ISDEF_SYSTONMCHIND == STD_ON )
#  if ( FrNm_GetSysToNmChInd(0U) >= FrNm_GetSizeOfChannelConfig() )
#   error: "FlexRay NM configuration issue: Channel Indirection is incorrect"
#  endif
# endif
#endif

/* Check configuration of Single Channel optimization */
#if defined ( FRNM_OPTIMIZE_CHANNEL_ENABLED  ) && ( FRNM_NUMBER_OF_FRNM_CHANNELS > 1U )
# error "FlexRay NM configuration issue: Single Channel optimization is only possible in single channel configurations."
#endif

/* Check configuration of Passive Mode */
#if ( FRNM_PASSIVE_MODE_ENABLED == STD_ON )
# if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_COORDINATOR_SYNC_SUPPORT == FRNM_PASSIVE_MODE_ENABLED == STD_ON."
# endif
# if ( FRNM_SYNC_PDU_FEATURE_ENABLED == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_SYNC_PDU_FEATURE_ENABLED == FRNM_PASSIVE_MODE_ENABLED == STD_ON."
# endif
# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_NODE_DETECTION_ENABLED == FRNM_PASSIVE_MODE_ENABLED == STD_ON."
# endif
# if ( FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_REMOTE_SLEEP_INDICATION_ENABLED == FRNM_PASSIVE_MODE_ENABLED == STD_ON."
# endif
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Check configuration of Node Detection */
# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON ) && ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_OFF )
#  error: "FlexRay NM configuration issue: FRNM_NODE_DETECTION_ENABLED cannot be activated if FRNM_SOURCE_NODE_IDENTIFIER_ENABLED is STD_OFF."
# endif
#endif

/* Check configuration of Control Bit Vector */
#if ( FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_OFF )
# if ( FRNM_NODE_DETECTION_ENABLED == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_NODE_DETECTION_ENABLED requires FRNM_CONTROL_BIT_VECTOR_ENABLED to be STD_ON."
# endif
# if ( FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_COORDINATOR_SYNC_SUPPORT requires FRNM_CONTROL_BIT_VECTOR_ENABLED to be STD_ON."
# endif
# if ( FRNM_PN_FEATURE_ENABLED == STD_ON )
#  error: "FlexRay NM configuration issue: FRNM_PN_FEATURE_ENABLED requires FRNM_CONTROL_BIT_VECTOR_ENABLED to be STD_ON."
# endif
#endif

#if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Check configuration of Repeat Message Bit */
# if ( FRNM_REPEAT_MESSAGE_BIT_ENABLED != FRNM_NODE_DETECTION_ENABLED )
#  error: "FlexRay NM configuration issue: Repeat Message Bit Enabled has to be set to the same value as Node Detection Enabled."
# endif
#endif

/* Check PN Configuration */
#if ( FRNM_PN_FEATURE_ENABLED == STD_ON )
# if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON ) && ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF )
#  if !( ( FRNM_USER_DATA_ENABLED == STD_ON ) || ( FRNM_COM_USER_DATA_SUPPORT == STD_ON ) )
#   error "FlexRay NM configuration issue: FRNM_PN_EIRA_CALC_FEATURE_ENABLED activated requires FRNM_USER_DATA_ENABLED or FRNM_COM_USER_DATA_SUPPORT to be enabled."
#  endif
# endif
#else
# if ( FRNM_PN_ERA_CALC_FEATURE_ENABLED == STD_ON )
#  error "FlexRay NM configuration issue: FRNM_PN_ERA_CALC_FEATURE_ENABLED cannot be activated if FRNM_PN_FEATURE_ENABLED is STD_OFF."
# endif
# if ( FRNM_PN_EIRA_CALC_FEATURE_ENABLED == STD_ON )
#  error "FlexRay NM configuration issue: FRNM_PN_EIRA_CALC_FEATURE_ENABLED cannot be activated if FRNM_PN_FEATURE_ENABLED is STD_OFF."
# endif
#endif

/* Check that the configuration variant is not VARIANT-LINK-TIME */
#if ( FRNM_CONFIGURATION_VARIANT == FRNM_CONFIGURATION_VARIANT_LINKTIME)
# error "FlexRay NM configuration issue: FRNM_CONFIGURATION_VARIANT_LINKTIME is not supported!"
#endif

#if ( FRNM_PDU_SCHEDULE_VARIANT_ONLY        <= 1U )     || \
    ( FRNM_NODE_DETECTION_ENABLED           == STD_ON ) || \
    ( FRNM_CONTROL_BIT_VECTOR_ENABLED       == STD_ON ) || \
    ( FRNM_USER_DATA_ENABLED                == STD_ON ) || \
    ( FRNM_COORDINATOR_SYNC_SUPPORT         == STD_ON ) || \
    ( FRNM_COM_USER_DATA_SUPPORT            == STD_ON )
# if ( FRNM_PASSIVE_MODE_ENABLED            == STD_OFF )
#  if ( FRNM_TXMESSAGEDATA == STD_OFF )
#   error FlexRay NM configuration issue: FNM_TXMESSAGEDATA has to be STD_ON in case Passive Mode is STD_OFF and some features require transmission of message content.
#  endif
# endif
#endif
#if ( ( FRNM_CONTROL_BIT_VECTOR_ENABLED            == STD_ON ) || \
      ( FRNM_SOURCE_NODE_IDENTIFIER_ENABLED        == STD_ON ) || \
      ( FRNM_USER_DATA_ENABLED                     == STD_ON ) || \
      ( FRNM_CARWAKEUPRXENABLEDOFCHANNELCONFIG     == STD_ON ) )
# if ( FRNM_RXMESSAGEDATA == STD_OFF )
#   error FlexRay NM configuration issue: FNM_RXMESSAGEDATA has to be STD_ON in case some features required received message content.
# endif
#endif

#if ( FRNM_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG == STD_ON )
# if ( FRNM_ISDEF_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG == STD_ON )
#  if ( FRNM_PDU_SCHEDULE_VARIANT_2 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_5 == STD_ON ) || ( FRNM_PDU_SCHEDULE_VARIANT_6 == STD_ON ) /* COV_FRNM_UNSUPPORTED_VARIANT  Xf xf xf xf */
#   error FlexRay NM configuration issue: FRNM_HWVOTESUPPORTINSCHEDULEVARIANTOFCHANNELCONFIG has to be STD_OFF in case configuration has only Pdu Schedule Variant 2.
#  endif
# endif
#endif

#if ( FRNM_USE_ECUM_BSW_ERROR_HOOK != FRNM_FINALMAGICNUMBER )
# error FlexRay NM configuration issue: The value of FRNM_USE_ECUM_BSW_ERROR_HOOK has to be equal to FRNM_FINALMAGICNUMBER.
#endif

/* Check for SBSW_FRNM_VOTE_POSITION */
#if ( FRNM_VOTE_POSITION != 0U )
# error: Violation of SBSW_FRNM_VOTE_POSITION Safety Check
#endif

/* module specific MISRA deviations:
  MD_FrNm_0342:
      Reason:     Glue operator necessary for code optimizations.
      Risk:       K&R Compilers do not support the glue operator.
      Prevention: Do not use K&R Compilers.

  MD_FrNm_2003:
      Reason:     Fall through used for code optimizations.
      Risk:       Risk that a fall through is overseen by a code review and leads to unexpected results.
      Prevention: Fall through is marked with explicit comments to draw attention to it when code review is performed.

  MD_FrNm_3325_3201:
      Reason:     Run-time check of generated data that shall always return false.
      Risk:       There is no risk.
      Prevention: Covered by code review.

  MD_FrNm_3333:
      Reason:     A break statement before last statement of a switch case block is used for code optimizations
      Risk:       No risk as code after break statement is only in the else path and would not be executed anyway.
      Prevention: Covered by code review and deep state machine tests with code coverage analysis.

  MD_FrNm_3418:
      Reason:     Used to have a compact and common pattern.
      Risk:       Creation of two statements.
      Prevention: Covered by code review and deep state machine tests with code coverage analysis.

  MD_FrNm_3673:
      Reason:     Common sense for Vector components.
                  Note that declaring API parameter to const could lead to warnings or problems for some compilers.
      Risk:       Parameter is not defined as const and could be modified within this function.
      Prevention: The parameter is not modified. Covered by code review.

  MD_FrNm_3353
      Reason:     Variable is always set before it is used.
      Risk:       No Risk, since Dummy statement is required to avoid compiler warnings.
      Prevention: The parameter is not modified. Covered by code review.

  MD_FrNm_3355_3358
      Reason:     Statement checks the generated configuration. It ensures no Out-Of-Bounds access can occur.
      Risk:       No Risk.
      Prevention: Covered by code review.
*/

/* SBSW_JUSTIFICATION_BEGIN

\ID                SBSW_FRNM_CHANNEL_IDX
\DESCRIPTION       Access of FrNm_TxControlState, FrNm_TimeoutTimer, FrNm_RemoteSleepTimer, FrNm_RemoteSleepIndState, FrNm_MsgIndicationFlag,
                             FrNm_NetworkRestartFlag, FrNm_NmState, FrNm_RxMessageVote, FrNm_CoordReadyToSleepState, FrNm_NetworkTimer,
                             FrNm_LastNetworkRequested, FrNm_RepeatMsgRequestIndFlag, FrNm_NetworkRequested, FrNm_TxMessageVote,
                             FrNm_SendSyncPdusFlag, FrNm_MsgConfirmationFlag, FrNm_IncCurrentFrCycle, FrNm_TxControlStateRequest, FrNm_CurrentFrCycle
                   (via indirection over FrNm_ChannelConfig structure) with same index using ComStackLib.
                   The 'channel' variable which is used as index is always checked.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                SBSW_FRNM_PDURCANNMRXINDICATION
\DESCRIPTION       The pointer 'nmPnInfo' is passed to the PduR.
\COUNTERMEASURE \N The parameters are not used in address calculations for memory manipulating
                   operations. Such calculations include but are not limited to: Array indexing,
                   pointer offset calculation and related forms of pointer arithmetic. Furthermore
                   these parameters are not forwarded to other software modules which could use
                   them for such operations unless they implement appropriate countermeasures.
                   And the addresses in the pointer variables are not saved in global variables
                   so that they could be used outside the function call. In that case, they are no
                   longer valid.

\ID                SBSW_FRNM_CSL_01
\DESCRIPTION       Access of FrNm_PnClusterReq, FrNm_PnClusterReqNew, FrNm_TxMessageData, FrNm_RxMessageData, FrNm_NmState using ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

\ID                SBSW_FRNM_CSL_02
\DESCRIPTION       Access of FrNm_PnClusterReqExt, FrNm_PnClusterReqExtNew, FrNm_PnClusterReqNew, FrNm_PnClusterReq using ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                SBSW_FRNM_CSL_03
\DESCRIPTION       Access of FrNm_PnClusterResetTimer, FrNm_PnClusterResetTimerExt, FrNm_TxMessageData, FrNm_RxMessageData, FrNm_TxSyncPduMessageData using ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                SBSW_FRNM_GETSTATE
\DESCRIPTION       The function FrNm_GetState writes to the object referenced by parameters 'nmStatePtr', 'nmModePtr'.
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameters 'nmStatePtr', 'nmModePtr' is valid.

\ID                SBSW_FRNM_GETVERSIONINFO
\DESCRIPTION       The function FrNm_GetVersionInfo writes to the object referenced by parameter 'versioninfo'.
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter 'versioninfo' is valid.

\ID                SBSW_FRNM_TRIGGERTRANSMIT
\DESCRIPTION       The function FrNm_TriggerTransmit writes to the object referenced by parameter 'PduInfoPtr'.
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter 'PduInfoPtr' is valid.

\ID                SBSW_FRNM_NODE_IDENTIFIER
\DESCRIPTION       The function FrNm_GetLocalNodeIdentifier and FrNm_GetNodeIdentifier writes to the object referenced by parameters 'nmNodeIdPtr'.
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter 'nmNodeIdPtr' is valid.

\ID                SBSW_FRNM_CHECKREMOTESLEEP
\DESCRIPTION       The function FrNm_CheckRemoteSleepIndication writes to the object referenced by parameter 'nmRemoteSleepIndPtr'.
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter 'nmRemoteSleepIndPtr' is valid.

\ID                SBSW_FRNM_FUNC_CALL_WITH_POINTER
\DESCRIPTION       The pointer 'FrNm_CurrentFrCycle', 'frCycleMacroTick', 'frIfState', 'frNmVector' are passed to the FrIf.
                   The pointer 'PduInfo' is passed to the PduR.
                   The CSL determines the size of the passed buffer and the length value with an indirection.
\COUNTERMEASURE \N The parameters are not used in address calculations for memory manipulating
                   operations. Such calculations include but are not limited to: Array indexing,
                   pointer offset calculation and related forms of pointer arithmetic. Furthermore
                   these parameters are not forwarded to other software modules which could use
                   them for such operations unless they implement appropriate countermeasures.
                   And the addresses in the pointer variables are not saved in global variables
                   so that they could be used outside the function call. In that case, they are no
                   longer valid.

\ID                SBSW_FRNM_FUNC_CALL_WITH_POINTER_NOWRITE
\DESCRIPTION       The pointer 'pduInfo' is passed to the FrIf.
\COUNTERMEASURE \N The parameters are not used in address calculations for memory manipulating
                   operations. Such calculations include but are not limited to: Array indexing,
                   pointer offset calculation and related forms of pointer arithmetic. Furthermore
                   these parameters are not forwarded to other software modules which could use
                   them for such operations unless they implement appropriate countermeasures.
                   And the addresses in the pointer variables are not saved in global variables
                   so that they could be used outside the function call. In that case, they are no
                   longer valid.

\ID                SBSW_FRNM_USER_DATA
\DESCRIPTION       The function FrNm_GetUserData writes to the object referenced by parameter 'nmUserDataPtr'.
\COUNTERMEASURE \S The caller of function FrNm_GetUserData must ensure that the pointer passed to the parameter 'nmUserDataPtr'
                   references a valid memory location and that the size of the array referenced by parameter 'nmUserDataPtr'
                   is greater or equal to FrNm_GetRxMessageData_UserDataLengthOfPbChannelConfig(channel). [SMI-334]

\ID                SBSW_FRNM_PDU_DATA
\DESCRIPTION       The function FrNm_GetPduData writes to the object referenced by parameter 'nmPduData'
\COUNTERMEASURE \S The caller of function FrNm_GetPduData must ensure that the pointer passed to the parameter 'nmPduData'
                   references a valid memory location and that the size of the array referenced by parameter 'nmPduData'
                   is greater or equal to FrNm_GetRxMessageDataLengthOfPbChannelConfig(channel). [SMI-335]

\ID                SBSW_FRNM_LOCALFUNCTION_CALL
\DESCRIPTION       The pointer parameter is passed to the local functions FrNm_CopyRxVoteData, FrNm_CopyRxMessageData, FrNm_RxPnFilterHandling.
\COUNTERMEASURE \N Pointer parameter is directly passed through.

\ID                SBSW_FRNM_VOTE_POSITION
\DESCRIPTION       A preprocessor error directive ensures that define FRNM_VOTE_POSITION is always zero.
\COUNTERMEASURE \N The bounds of the write access to 'frNmVector' is checked during compile time.

SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

\ID COV_FRNM_COM_CONTROL_VARIANT
   \ACCEPT XF
   \ACCEPT XX
   \ACCEPT TX
   \REASON In Schedule Variant 1 message transmission cannot be suppressed, therefore a call of FrNm_DisableCommunication has no effect.

\ID COV_FRNM_SILENT_CHECK_PNCLUSTERREQEXT
   \REASON This condition is redundant and will either always evaluate to true or false. It is needed to prevent index out of bound access due to invalid generated data (silent BSW).

\ID COV_FRNM_UNSUPPORTED_VARIANT
   \REASON Schedule Variant 3-7 are not supported.

\ID COV_FRNM_UNSUPPORTED_VARIANT_AND_COM_CONTROL
   \ACCEPT XF
   \ACCEPT TF tf xf xf
   \ACCEPT XF tf xf xf
   \REASON In Schedule Variant 2 message transmission cannot be suppressed, therefore a call of FrNm_DisableCommunication has no effect
           and Schedule Variant 3-7 are not supported.

\ID COV_FRNM_TXPDUID
   \ALWAYS XF
   \REASON In case DEV_ERROR_DETECT is STD_ON a wrong TxPduId is already detected and condition always evaluate to XF.

\ID COV_FRNM_READYSLEEPCNT
   \ACCEPT XF
   \ACCEPT XX
   \REASON In configurations where ReadySleepCnt is zero, condition always evaluates to XF.

\ID COV_FRNM_REPEATMSGTIME1
   \ACCEPT TX
   \ACCEPT XX
   \REASON In configurations where RepeatMessageTime is zero, condition always evaluates to TX or XX.

\ID COV_FRNM_REPEATMSGTIME2
   \ACCEPT XF
   \REASON In configurations where RepeatMessageTime is zero, condition always evaluates to XF.

\ID COV_FRNM_MSGTIMEOUTTIME
   \ACCEPT XF
   \ACCEPT TX
   \REASON In configurations where FrNm_MsgTimeoutTime is greater zero on all channels the condition is always true.
           In Postbuild Loadable configurations where FrNm_MsgTimeoutTime zero on all channels the conditino is always false.

\ID COV_FRNM_SYSTONMCHANNELIND
   \ACCEPT TF tf xf
   \REASON In configurations where FrNm is active on every ComM channel, the System to Nm Channel Indirection array does not contain
           any invalid FrNm channel index. Therefore the second condition cannot evaluate to true.

\ID COV_FRNM_PNFEATUREENABLED
   \ACCEPT TX
   \REASON Configuration switch PnEiraCalcEnabled and the constant value of "FrNm_PnEnabledInVariant" is not channel dependent, but may vary in Postbuild-Selectable configurations.
           Therefore it evaluates to TX in pre-compile configurations.

\ID COV_FRNM_INACTIVE_CHANNEL
   \REASON Availability of FrNm channel may only vary in Postbuild-Selectable configurations, where FrNm is inactive in one variant.

\ID COV_FRNM_COORDINATOR_SUPPORT
   \REASON FRNM_COORDINATOR_SYNC_SUPPORT implies that there is more than one system channel. Therefore FRNM_ISDEF_SYSTONMCHIND is always STD_OFF.

\ID COV_FRNM_NO_ODD_RXPDUID
   \ACCEPT XF
   \REASON In Postbuild-Selectable configurations with Schedule Variant 2 in every variant, the value of FrNmRxPduId is always even. 
           The condition can therefore only evaluate to XF

\ID COV_FRNM_REPEATMSGREQUEST_DISABLED
   \ACCEPT X
   \REASON Cannot be covered in following configuration:
            Configuration Variant | Schedule Variant | RepeatMessageRequestBitActive
            A                     | 1                | enabled
            B                     | 2                | disabled

\ID COV_FRNM_INVALID_GENDATA
   \ALWAYS XF
   \REASON This statements checks if the static configuration is correctly generated so that all accesses to FrNm_PbChannelConfig are within its bounds.

\ID COV_FRNM_DEV_ERROR_DETECT_ON
   \ACCEPT TX
   \ACCEPT TX tx
   \ACCEPT TX tx tx
   \ACCEPT TF tx tf
   \ACCEPT XF tf xf
   \REASON In the non-QM configurations it is only possible to test FRNM_DEV_ERROR_DETECT with value STD_ON

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: FrNm.c
 *********************************************************************************************************************/
