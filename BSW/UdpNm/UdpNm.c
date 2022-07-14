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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  UdpNm.c
 *      Project:  MICROSAR ETH
 *       Module:  UDP Network Management
 *    Generator:  Nm_AsrNmUdp.jar
 *
 *  Description:  Implementation
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

#define UDPNM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Platform_Types.h"

#include "UdpNm.h"
#include "UdpNm_Cbk.h"
#include "UdpNm_Cfg.h"
#include "UdpNm_Priv.h"
#include "SchM_UdpNm.h"
#if defined( UDPNM_E_NETWORK_TIMEOUT ) || defined( UDPNM_E_TCPIP_TRANSMIT_ERROR )
# include "Dem.h"
#endif
#include "Nm_Cbk.h"
#include "SoAd.h"
#include "IpBase.h"

/**********************************************************************************************************************
 *  COMPILE TIME CHECKS
 *********************************************************************************************************************/
/* Check consistency of source and header file */
#if ( (UDPNM_SW_MAJOR_VERSION != 3u) || (UDPNM_SW_MINOR_VERSION != 2u) || (UDPNM_SW_PATCH_VERSION != 2u) )
# error "UdpNm.c: Source and Header file are inconsistent!"
#endif

/* Check configuration variant for building the library */
#if ( defined(V_EXTENDED_BUILD_LIB_CHECK) \
        && (defined(UDPNM_CONFIGURATION_VARIANT) && (1u == UDPNM_CONFIGURATION_VARIANT)) )
# error "UdpNm.c: Switch configuration variant to link-time or post-build for library build!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif /* STATIC */

/* Turn off PC-lint warnings issued -> issues known and coverd by code review */
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL VARIABLES
 *********************************************************************************************************************/

#define UDPNM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/* variable holds the recent state of the network, which in some cases corresponds with the mode
 * - NM_STATE_BUS_SLEEP         (corresponds with NM_MODE_BUS_SLEEP)
 * - NM_STATE_PREPARE_BUS_SLEEP (corresponds with NM_MODE_PREPARE_BUS_SLEEP)
 * - NM_STATE_REPEATE_MESSAGE
 * - NM_STATE_NORMAL_OPERATION
 * - NM_STATE_READY_SLEEP
 */
STATIC VAR(Nm_StateType, UDPNM_VAR_ZERO_INIT)           UdpNm_State[UDPNM_CHANNEL_CNT] = { COMM_UNINIT };

#define UDPNM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define UDPNM_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/* variable holds the recent mode of the network
 * - NM_MODE_BUS_SLEEP
 * - NM_MODE_PREPARE_BUS_SLEEP
 * - NM_MODE_NETWORK_MODE
 */
STATIC VAR(Nm_ModeType, UDPNM_VAR_NOINIT)            UdpNm_Mode[UDPNM_CHANNEL_CNT];

/* variable holds the information if a network is RELEASED or REQUESTED */
STATIC VAR(UdpNm_NetworkStateType, UDPNM_VAR_NOINIT) UdpNm_NetworkState[UDPNM_CHANNEL_CNT];

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Tx PDU structure used during SoAd_IfTransmit to pass data location and length */
STATIC VAR(PduInfoType, UDPNM_VAR_NOINIT)            UdpNm_TxPduInfo[UDPNM_CHANNEL_CNT];
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_PN_ENABLED == STD_ON )
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
/* Variable used to transfer the EIRA data.
 * Pointer to buffer and the PDU length are initialized in UdpNm_Init() and never changed during
 * runtime.
 */
STATIC VAR(PduInfoType, UDPNM_VAR_NOINIT)            UdpNm_PnGlobal_EiraPdu;
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */

# if ( UDPNM_PN_ERA_CALC_ENABLED == STD_ON )
/* Variable used to transfer the ERA data.
 * Pointer to buffer is changed on context-switch (channel-context). PDU length is initialized
 * in UdpNm_Init() and never changed during runtime (equal over all channels).
 */
STATIC VAR(PduInfoType, UDPNM_VAR_NOINIT)            UdpNm_PnChannel_SharedEraPdu;
# endif /* UDPNM_PN_ERA_CALC_ENABLED */
#endif /* UDPNM_PN_ENABLED */

#define UDPNM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define UDPNM_START_SEC_VAR_NOINIT_32BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Counter triggering the cyclic NM message transmission in 'Repeat Message State' and
 * 'Normal Operation State'.
 */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_MsgCycleCnt[UDPNM_CHANNEL_CNT];

/* Counter monitoring the NM message transmission confirmation to detect a failed
 * NM message transmission.
 */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_MsgTimeoutCnt[UDPNM_CHANNEL_CNT];

# if( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
STATIC VAR(uint8, UDPNM_VAR_NOINIT) UdpNm_ImmediateNmTransmissionsCnt[UDPNM_CHANNEL_CNT];
# endif
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
/* Counter monitoring the NM message reception in 'Normal Operation State'.
 * Used to notify the Nm about the circumstance that all other nodes are in state
 * 'Ready Sleep State' and don't request the channel. (Nm_RemoteSleepIndication())
 */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_RemoteSleepIndCnt[UDPNM_CHANNEL_CNT];
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

/* Counter triggering the exit of 'Repeat Message State'. */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_RepeatMessageCnt[UDPNM_CHANNEL_CNT];

/* Counter triggering the transition from 'Ready Sleep State' to 'Prepare Bus-Sleep Mode'. */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_TimeoutCnt[UDPNM_CHANNEL_CNT];

/* Counter triggering the transition from 'Prepare Bus-Sleep Mode' to 'Bus-Sleep Mode'. */
STATIC VAR(sint32, UDPNM_VAR_NOINIT) UdpNm_WaitBusSleepCnt[UDPNM_CHANNEL_CNT];

#define UDPNM_STOP_SEC_VAR_NOINIT_32BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define UDPNM_START_SEC_VAR_NOINIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Control flag used for monitoring the NM message transmission confirmation
 * in combination with the UdpNm_MsgTimeoutCnt.
 */
STATIC VAR(boolean, UDPNM_VAR_NOINIT) UdpNm_MsgTxConfPending[UDPNM_CHANNEL_CNT];
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
/* Control flag used for the notification of the Nm about a remote sleep indication. */
STATIC VAR(boolean, UDPNM_VAR_NOINIT) UdpNm_RemoteSleepInd[UDPNM_CHANNEL_CNT];
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/* Control flag used to influence communication on specific channel. */
STATIC VAR(boolean, UDPNM_VAR_NOINIT) UdpNm_ComEnabled[UDPNM_CHANNEL_CNT];
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_PN_ENABLED == STD_ON )
/* Control flag indicating that a Rx indication for a channel was called since last
 * main function cycle.
 */
STATIC VAR(boolean, UDPNM_VAR_NOINIT) UdpNm_NmPduReceived[UDPNM_CHANNEL_CNT];

# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
/* State flags for the Partial Networks in EIRA context (global context).
 * PN state is mapped to bit with following meanings:
 *  0 -> 'PN Released' state
 *  1 -> 'PN Requested' state
 */
STATIC VAR(uint8, UDPNM_VAR_NOINIT) UdpNm_PnGlobal_PnStates[UDPNM_PN_INFO_LENGTH_BYTE];

/* Control flag indicating that a PDU transmit was triggered for a channel since last
 * main function cycle.
 */
STATIC VAR(boolean, UDPNM_VAR_NOINIT) UdpNm_NmPduTransmitted[UDPNM_CHANNEL_CNT];
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */
#endif /* UDPNM_PN_ENABLED */

#define UDPNM_STOP_SEC_VAR_NOINIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define UDPNM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  UdpNm_VTransmitPduCyclic
 *********************************************************************************************************************/
/*! \brief      Local function used in main function to transmit a NM PDU for corresponding Ethernet Network.
 *  \param[in]  UdpNmChannelIdx       Index of UdpNm local Channel Idx (doesn't correspond with global Channel Handle)
 *  \return     E_OK                  - Transmission request successfully executed
 *              E_NOT_OK              - Transmission request was rejected by SoAd
 *  \note       Function gets Tx PDU Data for addressed channel, modifies the PDU structure passed to SoAd and calls
 *              the internal UdpNm_VTransmitPdu.\n
 *              If UDPNM_COM_USER_DATA_SUPPORT is enabled the function collects the Tx PDU Data by calling the 
 *              Trigger Transmit API of PduR (PduR_UdpNmTriggerTransmit).
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_VTransmitPduCyclic(CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx);

/**********************************************************************************************************************
 *  UdpNm_VTransmitPdu
 *********************************************************************************************************************/
/*! \brief      Local function used to transmit a NM PDU for corresponding Ethernet Network.
 *  \param[in]  UdpNmChannelIdx       Index of UdpNm local Channel Idx (doesn't correspond with global Channel Handle)
 *  \param[in]  TxPduInfoPtr          Pointer to the PDU Information representing the PDU to be transmitted.
 *  \note       Function triggers the transmission of the PDU by calling SoAd_IfTransmit() and adapts internal states
 *              depending on the transmission of an NM PDU.
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_VTransmitPdu(
  CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_VAR) TxPduInfoPtr);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_VGetUdpNmChannelIdx
 *********************************************************************************************************************/
/*! \brief      Local function used to retrieve UdpNm local Channel Idx from global Channel Handle.
 *  \param[in]  NmChannelHandle       ComM Channel Handle
 *  \return     UdpNm local Channel index (UDPNM_INVALID_CHANNEL_IDX if no UdpNm Channel for passed ComM Channel Handle
 *              exists)
 *  \note       Iterates over all UdpNm Channels until the passed Channel Handle matches the Channel Handle of the
 *              corresponding UdpNm Channel.
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(UdpNm_ChannelIdxType, UDPNM_CODE) UdpNm_VGetUdpNmChannelIdx(
  CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle);

#if ( UDPNM_PN_FILTER_MASK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_PnFilterHandling
 *********************************************************************************************************************/
/*! \brief      Local function used to process Partial Networking filtering
 *  \param[in]  UdpNmChannelIdx       Index of UdpNm local Channel Idx
 *  \param[in]  PduInfoPtr            Pointer to UDP PDU related data containing DLC and pointer to UDP SDU buffer.
 *  \return     TRUE                  Received Message is relevant for this ECU
 *              FALSE                 Received Message did not pass the PN Filter
 *  \note       Called by UdpNm_SoAdIfRxIndication
 *  \context    Interrupt or task level
 *********************************************************************************************************************/
STATIC INLINE FUNC( boolean, UDPNM_CODE ) UdpNm_PnFilterHandling(
  CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx,
  P2CONST( PduInfoType, AUTOMATIC, UDPNM_APPL_VAR ) PduInfoPtr );
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  UdpNm_InitMemory
 *********************************************************************************************************************/
/*! \brief      Initialize global variables.
 *  \pre        This function has to be called before any other calls to the module.
 *  \context    Task level
 *  \note       AUTOSAR extension
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_InitMemory(void)
{
  UdpNm_ChannelIdxType ChannelIdx = UDPNM_CHANNEL_CNT;

  while ( 0u < ChannelIdx )
  {
    ChannelIdx--;
    UdpNm_State[ChannelIdx] = NM_STATE_UNINIT;
  }
}

/**********************************************************************************************************************
 *  UdpNm_Init
 *********************************************************************************************************************/
/*! \brief      Initialize the complete UdpNm module, i.e. all channels which are activated at configuration time are
 *              initialized. A UDP socket shall be set up with the TCP/IP stack.
 *  \param[in]  UdpNmConfigPtr       Pointer to a selected configuration structure
 *  \pre        This function has to be called before usage of the module.
 *  \pre        This function has to be called after initialization of the TCP/IP stack.
 *  \context    Initialization
 *********************************************************************************************************************/
/* PRQA S 3206 1 */ /* MD_UdpNm_3206 */
FUNC(void, UDPNM_CODE) UdpNm_Init(P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_CONST) UdpNmConfigPtr)
{
#if ( UDPNM_PN_ENABLED == STD_ON )
  uint8                 rxMsgBytePos;
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
  uint8                 PnGlByteIdx;
# endif /* UDPNM_PN_ENABLED */
#endif /* UDPNM_PN_EIRA_CALC_ENABLED */
  PduLengthType         UserDataLength; /* PRQA S 781 */ /* MD_UdpNm_781 */
  UdpNm_ChannelIdxType  ChannelIdx;

  /* Dummy statement to avoid compiler warnings */
  UDPNM_DUMMY_STATEMENT(UdpNmConfigPtr); /* PRQA S 3112 */ /* MD_UdpNm_14.2 */

  ChannelIdx = UDPNM_CHANNEL_CNT;

#if ( UDPNM_PN_ENABLED == STD_ON )
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
  PnGlByteIdx = UDPNM_PN_INFO_LENGTH_BYTE;

  while( 0u < PnGlByteIdx )
  {
    PnGlByteIdx--;
    {
      uint8 PnGlRelevantPnIdx = UdpNm_VCfgPnGlobalGetRelevantPnCnt(PnGlByteIdx);
      /* initialize all global PN bits to 0 -> 'PN Released' state (EIRA context) */
      UdpNm_PnGlobal_PnStates[PnGlByteIdx] = 0x00U;
      while( 0 < PnGlRelevantPnIdx)
      {
        PnGlRelevantPnIdx--;
        /* initialize all global PN Timeout counters for channel (stopped) (EIRA context) */
        UdpNm_VCfgPnGlobalGetPnTimeoutCounters(PnGlByteIdx)[PnGlRelevantPnIdx] = UDPNM_COUNTER_STOP;
      }
    }
  }
  UdpNm_PnGlobal_EiraPdu.SduDataPtr = &UdpNm_PnGlobal_PnStates[0];
  UdpNm_PnGlobal_EiraPdu.SduLength  = UDPNM_PN_INFO_LENGTH_BYTE;
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */

# if ( UDPNM_PN_ERA_CALC_ENABLED == STD_ON )
  UdpNm_PnChannel_SharedEraPdu.SduDataPtr = (SduDataPtrType)NULL_PTR;
  UdpNm_PnChannel_SharedEraPdu.SduLength  = UDPNM_PN_INFO_LENGTH_BYTE;
# endif
#endif /* UDPNM_PN_ENABLED */

  while ( 0u < ChannelIdx )
  {
    ChannelIdx--;
    UdpNm_Mode[ChannelIdx]                 = NM_MODE_BUS_SLEEP;
    UdpNm_State[ChannelIdx]                = NM_STATE_BUS_SLEEP;
    UdpNm_NetworkState[ChannelIdx]         = UDPNM_NETWORK_STATE_RELEASED;
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    UdpNm_TxPduInfo[ChannelIdx].SduLength  = UdpNm_VCfgGetPduLength(ChannelIdx);
    UdpNm_TxPduInfo[ChannelIdx].SduDataPtr = &(UdpNm_TxPduBuffers[ChannelIdx][0]);
    UdpNm_ComEnabled[ChannelIdx]           = TRUE;
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    UdpNm_RemoteSleepInd[ChannelIdx]       = FALSE;
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if ( UDPNM_PN_ENABLED == STD_ON )
    UdpNm_NmPduReceived[ChannelIdx]    = FALSE;
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
    UdpNm_NmPduTransmitted[ChannelIdx] = FALSE;
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */
#endif /* UDPNM_PN_ENABLED */

#if ( UDPNM_NODE_ID_ENABLED == STD_ON )
    if ( UDPNM_PDU_OFF != UdpNm_VCfgGetPduNidPosition(ChannelIdx) )
    {
# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
      UdpNm_TxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduNidPosition(ChannelIdx)] = UdpNm_VCfgGetNodeId(ChannelIdx);
# endif /* UDPNM_PASSIVE_MODE_ENABLED */
      UdpNm_RxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduNidPosition(ChannelIdx)] = 0x00u;
    }
#endif /* UDPNM_NODE_ID_ENABLED */
    if ( UDPNM_PDU_OFF != UdpNm_VCfgGetPduCbvPosition(ChannelIdx) )
    {
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
      UdpNm_TxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduCbvPosition(ChannelIdx)] = 0x00u;
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
      UdpNm_RxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduCbvPosition(ChannelIdx)] = 0x00u;
    }
    UserDataLength = UdpNm_VCfgGetUserDataLength(ChannelIdx);

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
    /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
    IpBase_Fill(
        (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_TxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduUserDataPosition(ChannelIdx)],
        (uint8)0xFFu,
        (uint32)UserDataLength);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
    /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
    /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
    IpBase_Fill(
        (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_RxPduBuffers[ChannelIdx][UdpNm_VCfgGetPduUserDataPosition(ChannelIdx)],
        (uint8)0xFFu,
        (uint32)UserDataLength);
#if ( UDPNM_PN_ENABLED == STD_ON )
    /* Initialize Rx Pn Buffer Array */
    for( rxMsgBytePos = 0; rxMsgBytePos < (UDPNM_PN_INFO_LENGTH_BYTE); rxMsgBytePos++ )
    {
      UdpNm_RxPnBuffers[ChannelIdx][rxMsgBytePos] = 0x00;
    }
#endif

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    UdpNm_MsgCycleCnt[ChannelIdx]       = UDPNM_COUNTER_STOP;
    UdpNm_MsgTimeoutCnt[ChannelIdx]     = UDPNM_COUNTER_STOP;
    UdpNm_MsgTxConfPending[ChannelIdx]  = FALSE;
#if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
    UdpNm_ImmediateNmTransmissionsCnt[ChannelIdx] = 0U;
#endif
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    UdpNm_RemoteSleepIndCnt[ChannelIdx] = UDPNM_COUNTER_STOP;
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
    UdpNm_RepeatMessageCnt[ChannelIdx]  = UDPNM_COUNTER_STOP;
    UdpNm_TimeoutCnt[ChannelIdx]        = UDPNM_COUNTER_STOP;
    UdpNm_WaitBusSleepCnt[ChannelIdx]   = UDPNM_COUNTER_STOP;

#if ( UDPNM_PN_ENABLED == STD_ON )
    /* Set PNI Bit in Tx PDUs used on channels where Partial Networking is active */
    if( TRUE == UdpNm_VCfgPnChannelIsPnEnabled(ChannelIdx) )
    {
      UdpNm_TxPdu_SetPNInfoBit(ChannelIdx);
    }
# if ( STD_ON == UDPNM_PN_ERA_CALC_ENABLED)
    if( TRUE == UdpNm_VCfgPnChannelIsEraEnabled(ChannelIdx) )
    {
      uint8 PnChByteIdx = UDPNM_PN_INFO_LENGTH_BYTE;
      while( 0 < PnChByteIdx )
      {
        PnChByteIdx--;
        {
          uint8 PnChRelevantPnIdx = UdpNm_VCfgPnGlobalGetRelevantPnCnt(PnChByteIdx);
          /* initialize all channel specific PN bits to 0 -> 'PN Released' state (ERA context) */
          UdpNm_VCfgPnChannelGetPnStates(ChannelIdx, PnChByteIdx) = 0x00U;
          while( 0 < PnChRelevantPnIdx)
          {
            PnChRelevantPnIdx--;
            /* initialize all channel specific PN Timeout counters for channel (stopped) (ERA context) */
            UdpNm_VCfgPnChannelGetPnTimeoutCounters(ChannelIdx, PnChByteIdx)[PnChRelevantPnIdx] = UDPNM_COUNTER_STOP;
          }
        }
      }
    }
# endif /* UDPNM_ERA_CALC_ENABLED */
#endif /* UDPNM_PN_ENABLED */
  }
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  UdpNm_PassiveStartUp
 *********************************************************************************************************************/
/*! \brief      Passive startup of the AUTOSAR UdpNm. It triggers the transition from Bus-Sleep Mode to the Network
 *              Mode in Repeat Message State.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Passive startup of network management has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_PassiveStartUp(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_PASSIVE_START_UP,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_PASSIVE_START_UP,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( ( NM_MODE_BUS_SLEEP != UdpNm_Mode[UdpNmChannelIdx] )
         && ( NM_MODE_PREPARE_BUS_SLEEP != UdpNm_Mode[UdpNmChannelIdx] ) )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* inform Nm about network mode */
    Nm_NetworkMode(NmChannelHandle);

#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
    /* inform Nm about the state change */
    Nm_StateChangeNotification(NmChannelHandle, UdpNm_State[UdpNmChannelIdx], NM_STATE_REPEAT_MESSAGE);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

    UdpNm_Mode[UdpNmChannelIdx]             = NM_MODE_NETWORK;
    UdpNm_State[UdpNmChannelIdx]            = NM_STATE_REPEAT_MESSAGE;
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
    UdpNm_MsgCycleCnt[UdpNmChannelIdx]      = UdpNm_VCfgGetMsgCycleOffsetTicks(UdpNmChannelIdx);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
    UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRepeatMessageTicks(UdpNmChannelIdx);
    UdpNm_TimeoutCnt[UdpNmChannelIdx]       = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
    RetVal                                  = E_OK;
  }

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  UdpNm_NetworkRequest
 *********************************************************************************************************************/
/*! \brief      Request the network, since ECU needs to communicate on the bus. Network state shall be changed to
 *              'requested'.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Requesting of network has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRequest(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx;
# if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
  Nm_StateType previousState;
# endif

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_NETWORK_REQUEST,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  UdpNm_CheckDetErrorReturnValue((NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_NETWORK_REQUEST,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
  previousState = UdpNm_State[UdpNmChannelIdx];
# endif

  if ( NM_MODE_NETWORK != UdpNm_Mode[UdpNmChannelIdx] )
  {
    /* inform Nm about network mode */
    Nm_NetworkMode(NmChannelHandle);

# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
#  if ( UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON )
    if ( NM_MODE_PREPARE_BUS_SLEEP == UdpNm_Mode[UdpNmChannelIdx] )
    {
      (void)UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
      UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgCycleTicks(UdpNmChannelIdx);
    }
    else
#  endif
#  if( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
    if ( UdpNm_VCfgGetImmediateNmTransmissions(UdpNmChannelIdx) > 0U )
    {
      UdpNm_MsgCycleCnt[UdpNmChannelIdx] = 0U;
      UdpNm_ImmediateNmTransmissionsCnt[UdpNmChannelIdx] = ( UdpNm_VCfgGetImmediateNmTransmissions(UdpNmChannelIdx) - 1U );
    }
    else
#  endif
    {
      UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgCycleOffsetTicks(UdpNmChannelIdx);
    }
# endif /* UDPNM_PASSIVE_MODE_ENABLED */

    /* go to repeat message state */
    UdpNm_Mode[UdpNmChannelIdx]             = NM_MODE_NETWORK;
    UdpNm_State[UdpNmChannelIdx]            = NM_STATE_REPEAT_MESSAGE;
    UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRepeatMessageTicks(UdpNmChannelIdx);
    UdpNm_TimeoutCnt[UdpNmChannelIdx]       = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
  }
  else if ( NM_STATE_READY_SLEEP == UdpNm_State[UdpNmChannelIdx] )
  {
    /* go to normal operation state */
    UdpNm_State[UdpNmChannelIdx]             = NM_STATE_NORMAL_OPERATION;
# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    UdpNm_MsgCycleCnt[UdpNmChannelIdx]       = UdpNm_VCfgGetMsgCycleOffsetTicks(UdpNmChannelIdx);
# endif /* UDPNM_PASSIVE_MODE_ENABLED */
# if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRemoteSleepIndTicks(UdpNmChannelIdx);
# endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
  }
  else
  {
    /* Nothing to do here; required by MISRA */
  }

# if ( UDPNM_PN_ENABLED == STD_ON )
#  if ( UDPNM_PN_HANDLE_MULTI_NET_REQ == STD_ON )
  if( TRUE == UdpNm_VCfgPnChannelIsMultiNetwReqHandlingEnabled(UdpNmChannelIdx) )
  {
    if( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] )
    {
      /* got to 'Repeat Message State' */
      UdpNm_Mode[UdpNmChannelIdx]             = NM_MODE_NETWORK;
      UdpNm_State[UdpNmChannelIdx]            = NM_STATE_REPEAT_MESSAGE;
      UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRepeatMessageTicks(UdpNmChannelIdx);
      UdpNm_TimeoutCnt[UdpNmChannelIdx]       = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
#   if( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
      if ( UdpNm_VCfgGetImmediateNmTransmissions(UdpNmChannelIdx) > 0U )
      {
        /* ESCAN00093017 */
        UdpNm_MsgCycleCnt[UdpNmChannelIdx] = 0U;
        UdpNm_ImmediateNmTransmissionsCnt[UdpNmChannelIdx] = ( UdpNm_VCfgGetImmediateNmTransmissions(UdpNmChannelIdx) - 1U );
      }
#   endif
    }
  }
#  endif /* UDPNM_PN_HANDLE_MULTI_NET_REQ */
# endif /* UDPNM_PN_ENABLED */

# if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
  /* inform Nm about the state change */
  if( UdpNm_State[UdpNmChannelIdx] != previousState )
  {
    Nm_StateChangeNotification(NmChannelHandle, previousState, UdpNm_State[UdpNmChannelIdx]);
  }
# endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

  /* design decision: accept network requests also in other states but the ones explicitly shown in SWS
      specification */
  UdpNm_NetworkState[UdpNmChannelIdx] = UDPNM_NETWORK_STATE_REQUESTED;
  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  UdpNm_NetworkRelease
 *********************************************************************************************************************/
/*! \brief      Release the network, since ECU doesn't have to communicate on the bus. Network state shall be changed
 *              to 'released'.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Releasing of network has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRelease(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_NETWORK_RELEASE,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_NETWORK_RELEASE,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( FALSE == UdpNm_ComEnabled[UdpNmChannelIdx] )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    if ( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
    {
# if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
      /* inform Nm about the state change */
      Nm_StateChangeNotification(NmChannelHandle, UdpNm_State[UdpNmChannelIdx], NM_STATE_READY_SLEEP);
# endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

      /* directly switch to ready sleep state from normal operation */
      UdpNm_State[UdpNmChannelIdx]             = NM_STATE_READY_SLEEP;
# if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
      UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UDPNM_COUNTER_STOP;
# endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
    }
    UdpNm_NetworkState[UdpNmChannelIdx] = UDPNM_NETWORK_STATE_RELEASED;
    RetVal                              = E_OK;
  }
  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_COM_CONTROL_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_DisableCommunication
 *********************************************************************************************************************/
/*! \brief      Disable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 *  \param[in]  NmChannelHandle   Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Disabling of NM PDU transmission ability has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_DisableCommunication(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_DISABLE_COMMUNICATION,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_DISABLE_COMMUNICATION,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  RetVal = E_NOT_OK;

  /*
   * If Passive Mode is enabled the API has to return E_NOT_OK.
   */
# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  if (( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] ) && ( TRUE == UdpNm_ComEnabled[UdpNmChannelIdx] ))
  {
    UdpNm_ComEnabled[UdpNmChannelIdx]        = FALSE;
    UdpNm_MsgCycleCnt[UdpNmChannelIdx]       = UDPNM_COUNTER_STOP;
#  if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UDPNM_COUNTER_STOP;
#  endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
    UdpNm_TimeoutCnt[UdpNmChannelIdx]        = UDPNM_COUNTER_STOP;
    RetVal                                   = E_OK;
  }
# endif /* UDPNM_PASSIVE_MODE_ENABLED */

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_COM_CONTROL_ENABLED */

#if ( defined(UDPNM_COM_CONTROL_ENABLED) && (STD_ON == UDPNM_COM_CONTROL_ENABLED) )
/**********************************************************************************************************************
 *  UdpNm_EnableCommunication
 *********************************************************************************************************************/
/*! \brief      Enable the NM PDU transmission ability due to a ISO14229 Communication Control (0x28) service.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Enabling of NM PDU transmission ability has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_EnableCommunication(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_ENABLE_COMMUNICATION,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_ENABLE_COMMUNICATION,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  RetVal = E_NOT_OK;

  /*
   * If Passive Mode is enabled the API has to return E_NOT_OK.
   */
# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  if (( FALSE == UdpNm_ComEnabled[UdpNmChannelIdx] ) && ( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] ))
  {
    UdpNm_ComEnabled[UdpNmChannelIdx]  = TRUE;
    UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgCycleOffsetTicks(UdpNmChannelIdx);
    UdpNm_TimeoutCnt[UdpNmChannelIdx]  = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
    RetVal                             = E_OK;
  }
# endif /* UDPNM_PASSIVE_MODE_ENABLED */

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_COM_CONTROL_ENABLED */

#if (( STD_ON == UDPNM_USER_DATA_ENABLED ) && ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED ) \
         &&  ( STD_OFF == UDPNM_COM_USER_DATA_SUPPORT ))
/**********************************************************************************************************************
 *  UdpNm_SetUserData
 *********************************************************************************************************************/
/*! \brief      Set user data for all NM messages transmitted on the bus after this function has returned without
 *              error.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmUserDataPtr        Pointer where the user data for the next transmitted NM message shall be copied
 *                                   from
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Setting of user data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetUserData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle, \
                                  P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) NmUserDataPtr)
{
  UdpNm_ChannelIdxType  UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_SET_USER_DATA,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_SET_USER_DATA,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmUserDataPtr),
    UDPNM_API_ID_SET_USER_DATA,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 311 6 */ /* MD_UdpNm_311_IpBase */
  /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
  /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
  IpBase_Copy(
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_TxPduBuffers[UdpNmChannelIdx][UdpNm_VCfgGetPduUserDataPosition(UdpNmChannelIdx)],
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&NmUserDataPtr[0],
      (uint32)UdpNm_VCfgGetUserDataLength(UdpNmChannelIdx));

  UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_USER_DATA_ENABLED, UDPNM_PASSIVE_MODE_ENABLED, UDPNM_COM_USER_DATA_SUPPORT */

#if ( UDPNM_USER_DATA_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_GetUserData
 *********************************************************************************************************************/
/*! \brief      Get user data from the most recently received NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmUserDataPtr        Pointer where user data out of the most recently received NM message shall be
 *                                   copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of user data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetUserData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle, \
                                  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmUserDataPtr)
{
  UdpNm_ChannelIdxType  UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_GET_USER_DATA,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_GET_USER_DATA,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmUserDataPtr),
    UDPNM_API_ID_GET_USER_DATA,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
  /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
  IpBase_Copy(
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&NmUserDataPtr[0],
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_RxPduBuffers[UdpNmChannelIdx][UdpNm_VCfgGetPduUserDataPosition(UdpNmChannelIdx)],
      (uint32)UdpNm_VCfgGetUserDataLength(UdpNmChannelIdx));

  UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_USER_DATA_ENABLED */

#if ( UDPNM_NODE_ID_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_GetNodeIdentifier
 *********************************************************************************************************************/
/*! \brief      Get node identifier from the most recently received NM PDU.
 *  \param[in]  NmChannelHandle   Identification of the NM-channel
 *  \param[out] NmNodeIdPtr       Pointer where the source node identifier from the most recently received NM PDU
 *                                shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of the source node identifier from the most recently received NM PDU has
 *                                failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle, \
                                  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmNodeIdPtr)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_GET_NODE_IDENTIFIER,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_GET_NODE_IDENTIFIER,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmNodeIdPtr),
    UDPNM_API_ID_GET_NODE_IDENTIFIER,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  *NmNodeIdPtr = UdpNm_RxPduBuffers[UdpNmChannelIdx][UdpNm_VCfgGetPduNidPosition(UdpNmChannelIdx)];

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_NODE_ID_ENABLED */

#if ( UDPNM_NODE_ID_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_GetLocalNodeIdentifier
 *********************************************************************************************************************/
/*! \brief      Get node identifier configured for the local node.
 *  \param[in]  NmChannelHandle   Identification of the NM-channel
 *  \param[out] NmNodeIdPtr       Pointer where node identifier of the local node shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of the node identifier of the local node has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetLocalNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmNodeIdPtr)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_GET_LOCAL_NODE_IDENTIFIER,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_GET_LOCAL_NODE_IDENTIFIER,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmNodeIdPtr),
    UDPNM_API_ID_GET_LOCAL_NODE_IDENTIFIER,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  *NmNodeIdPtr = UdpNm_VCfgGetNodeId(UdpNmChannelIdx);

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_NODE_ID_ENABLED */

#if ( UDPNM_NODE_DETECTION_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_RepeatMessageRequest
 *********************************************************************************************************************/
/*! \brief      Trigger transition to Repeat Message State and set Repeat Message Request Bit for all NM messages
 *              transmitted on the bus after this function has returned without error (dependent on the parameter
 *              'isRepeatMessageBitIndication').
 *  \param[in]  NmChannelHandle             Identification of the NM-channel
 *  \return     E_OK                        No error
 *  \return     E_NOT_OK                    Setting of Repeat Message Request Bit has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RepeatMessageRequest(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_REPEAT_MESSAGE_REQUEST,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_REPEAT_MESSAGE_REQUEST,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  RetVal = E_NOT_OK;

  if (( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
          || (NM_STATE_READY_SLEEP == UdpNm_State[UdpNmChannelIdx] ))
  {
    /* set 'Repeat Message Request Bit' in the CBV of the transmitted UDP NM message */
# if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    UdpNm_TxPdu_SetRptMsgReq(UdpNmChannelIdx);
# endif /* UDPNM_PASSIVE_MODE_ENABLED */
# if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
    /* inform Nm about the state change */
    Nm_StateChangeNotification(NmChannelHandle, UdpNm_State[UdpNmChannelIdx], NM_STATE_REPEAT_MESSAGE);
# endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

# if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    if ( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
    {
      UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UDPNM_COUNTER_STOP;
      if ( TRUE == UdpNm_RemoteSleepInd[UdpNmChannelIdx] )
      {
        Nm_RemoteSleepCancellation(NmChannelHandle);
      }
    }
# endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

    UdpNm_State[UdpNmChannelIdx]            = NM_STATE_REPEAT_MESSAGE;
    UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRepeatMessageTicks(UdpNmChannelIdx);
    RetVal                                  = E_OK;
  }

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_NODE_DETECTION_ENABLED */

#if (( STD_ON == UDPNM_NODE_ID_ENABLED ) || ( STD_ON == UDPNM_NODE_DETECTION_ENABLED) \
         ||  ( STD_ON == UDPNM_USER_DATA_ENABLED ))
/**********************************************************************************************************************
 *  UdpNm_GetPduData
 *********************************************************************************************************************/
/*! \brief      Get the whole PDU data out of the most recently received NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmPduDataPtr         Pointer where NM PDU shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of NM PDU data has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetPduData(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle, \
                                  CONSTP2VAR(uint8, AUTOMATIC, UDPNM_APPL_VAR) NmPduDataPtr)
{
  UdpNm_ChannelIdxType  UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_GET_PDU_DATA,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_GET_PDU_DATA,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmPduDataPtr),
    UDPNM_API_ID_GET_PDU_DATA,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* copy last received PDU data into nmPduDataPtr */
  UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
  /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
  IpBase_Copy(
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&NmPduDataPtr[0],
      (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_RxPduBuffers[UdpNmChannelIdx][0],
      (uint32)UdpNm_VCfgGetPduLength(UdpNmChannelIdx));

  UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_NODE_ID_ENABLED, UDPNM_NODE_DETECTION_ENABLED, UDPNM_USER_DATA_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_GetState
 *********************************************************************************************************************/
/*! \brief      Returns the state and the mode of the network management.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmStatePtr           Pointer where state of the network management shall be copied to
 *  \param[out] NmModePtr            Pointer where the mode of the network management shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Getting of NM state has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetState(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                  CONSTP2VAR(Nm_StateType, AUTOMATIC, UDPNM_APPL_VAR) NmStatePtr,
                                  CONSTP2VAR(Nm_ModeType, AUTOMATIC, UDPNM_APPL_VAR) NmModePtr)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_GET_STATE,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_GET_STATE,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmStatePtr),
    UDPNM_API_ID_GET_STATE,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmModePtr),
    UDPNM_API_ID_GET_STATE,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  *NmStatePtr = UdpNm_State[UdpNmChannelIdx];
  *NmModePtr  = UdpNm_Mode[UdpNmChannelIdx];

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if ( UDPNM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Returns the version information, vendor ID and AUTOSAR module ID of the component.
 *              The versions are BCD-coded.
 *  \param[in]  VersionInfoPtr        Pointer for version info
 *  \return     void
 *  \context    Initialization or task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, UDPNM_APPL_VAR) Versioninfo)
{
  UdpNm_CheckDetErrorReturnVoid(
    (NULL_PTR != Versioninfo),
    UDPNM_API_ID_GET_VERSION_INFO,
    UDPNM_E_NULL_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  Versioninfo->vendorID         = UDPNM_VENDOR_ID;
  Versioninfo->moduleID         = UDPNM_MODULE_ID;
  Versioninfo->sw_major_version = UDPNM_SW_MAJOR_VERSION;
  Versioninfo->sw_minor_version = UDPNM_SW_MINOR_VERSION;
  Versioninfo->sw_patch_version = UDPNM_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_VERSION_INFO_API */

#if (( UDPNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) && ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF ))
/**********************************************************************************************************************
 *  UdpNm_RequestBusSynchronization
 *********************************************************************************************************************/
/*! \brief      Request bus synchronization.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Requesting of bus synchronization has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_REQUEST_BUS_SYNCHRONIZATION,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_REQUEST_BUS_SYNCHRONIZATION,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  RetVal = E_NOT_OK;

  if (( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] ) && ( TRUE == UdpNm_ComEnabled[UdpNmChannelIdx] ))
  {
    (void)UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
    RetVal = E_OK;
  }

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_BUS_SYNCHRONIZATION_ENABLED, UDPNM_PASSIVE_MODE_ENABLED */

#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_CheckRemoteSleepIndication
 *********************************************************************************************************************/
/*! \brief      Check if remote sleep indication takes place or not.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[out] NmRemoteSleepIndPtr  Pointer where check result of remote sleep indication shall be copied to
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Checking of remote sleep indication bits has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_CheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                  CONSTP2VAR(boolean, AUTOMATIC, UDPNM_APPL_VAR) NmRemoteSleepIndPtr)
{
  Std_ReturnType       RetVal;
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_CHECK_REMOTE_SLEEP_INDICATION,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_CHECK_REMOTE_SLEEP_INDICATION,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NULL_PTR != NmRemoteSleepIndPtr),
    UDPNM_API_ID_CHECK_REMOTE_SLEEP_INDICATION,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  RetVal = E_NOT_OK;

  if (( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
          || ( NM_STATE_READY_SLEEP == UdpNm_State[UdpNmChannelIdx] ))
  {
    *NmRemoteSleepIndPtr = UdpNm_RemoteSleepInd[UdpNmChannelIdx];
    RetVal               = E_OK;
  }

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_SetCoordBits
 *********************************************************************************************************************/
/*! \brief      Sets the NM coordinator ID in the control bit vector of each NM message.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmCoordBits          2 bit value to set the NM coordinator ID in the control bit vector of each NM
 *                                   message (coding as depicted in Figure "Control Bit Vector")
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Setting the coordinator ID bits has failed
 *  \context    Task level
 *********************************************************************************************************************/
/* PRQA S 3206 2 */ /* MD_UdpNm_3206 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetCoordBits(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                    CONST(uint8, AUTOMATIC) NmCoordBits)
{
#if( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  UdpNm_ChannelIdxType UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_SET_COORD_BITS,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_SET_COORD_BITS,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_TxPdu_SetCoordIdBits(UdpNmChannelIdx, NmCoordBits); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#else
# if ( UDPNM_USE_DUMMY_STATEMENT == STD_ON )
  /* Dummy statement to avoid compiler warnings */
  UDPNM_DUMMY_STATEMENT_CONST(NmChannelHandle); /* PRQA S 3112 */ /* MD_UdpNm_14.2 */
  UDPNM_DUMMY_STATEMENT_CONST(NmCoordBits); /* PRQA S 3112 */ /* MD_UdpNm_14.2 */
# endif /* UDPNM_USE_DUMMY_STATEMENT */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if ( UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_SetSleepReadyBit
 *********************************************************************************************************************/
/*! \brief      Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 *  \param[in]  NmChannelHandle      Identification of the NM-channel
 *  \param[in]  NmSleepReadyBit      Value written to ReadySleep Bit in CBV
 *  \return     E_OK              No error
 *  \return     E_NOT_OK          Writing of remote sleep indication bit has failed
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetSleepReadyBit(CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle,
                                                        CONST(boolean, AUTOMATIC) NmSleepReadyBit)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  UdpNmChannelIdx = UdpNm_VGetUdpNmChannelIdx(NmChannelHandle);

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_SET_SLEEP_READY_BIT,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_SET_SLEEP_READY_BIT,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if ( TRUE == NmSleepReadyBit )
  {
    UdpNm_TxPdu_SetReadySleepBit(UdpNmChannelIdx);
  }
  else
  {
    UdpNm_TxPdu_ClearReadySleepBit(UdpNmChannelIdx);
  }

  if ( ( UdpNm_State[UdpNmChannelIdx] == NM_STATE_NORMAL_OPERATION ) ||
       ( UdpNm_State[UdpNmChannelIdx] == NM_STATE_REPEAT_MESSAGE ) 
     )
  {
    (void)UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
  }

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_COORDINATOR_SYNC_SUPPORT */

#if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_Transmit
 *********************************************************************************************************************/
/*! \brief      UdpNm_Transmit is implemented as an empty function and shall always return E_OK. The function
 *              UdpNm_Transmit is only available if the configuration switch UdpNmComUserDataSupport is enabled.
 *  \param[in]  UdpNmSrcPduId        This parameter contains a unique identifier referencing to the PDU Routing Table
 *                                   and thereby specifying the socket to be used for transmission of the data
 *  \param[in]  UdpNmSrcPduInfoPtr   A pointer to a structure with socket related data: data length and pointer to a
 *                                   data buffer
 *  \return     E_OK                 The request has been accepted
 *  \return     E_NOT_OK             The request has not been accepted, e.g. due to a still ongoing transmission in the
 *                                   corresponding socket or the to be transmitted message is too long
 *  \context    Task level
 *********************************************************************************************************************/
/* PRQA S 3206 2 */ /* MD_UdpNm_3206 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_Transmit(VAR(PduIdType, AUTOMATIC) UdpNmTxPduId,
                                                P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) UdpNmSrcPduInfoPtr)
{
# if (STD_ON == UDPNM_UL_IMMEDIATE_TX_SUPPORT )
  UdpNm_ChannelIdxType UdpNmChannelIdx;

  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_NM_TX_PDU_CNT > UdpNmTxPduId),
    UDPNM_API_ID_TRANSMIT,
    UDPNM_E_INVALID_PDUID,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNmChannelIdx = UdpNm_VCfgGetNmTxPduId2ChannelId(UdpNmTxPduId);

  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_TRANSMIT,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (UdpNmSrcPduInfoPtr != NULL_PTR),
    UDPNM_API_ID_TRANSMIT,
    UDPNM_E_NULL_POINTER,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* User Data passed for transmission must fit into allocated space for User Data in NM PDU */
  UdpNm_CheckDetErrorReturnValue(
    (UdpNmSrcPduInfoPtr->SduLength <= UdpNm_VCfgGetUserDataLength(UdpNmChannelIdx)),
    UDPNM_API_ID_TRANSMIT,
    UDPNM_E_INVALID_USER_DATA_LEN,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* Transmit NM PDU */
  return UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
# else

  /* Dummy statements to avoid compiler warnings */
  UDPNM_DUMMY_STATEMENT(UdpNmTxPduId); /* PRQA S 3112 */ /* MD_UdpNm_14.2 */
  UDPNM_DUMMY_STATEMENT(UdpNmSrcPduInfoPtr); /* PRQA S 3112 */ /* MD_UdpNm_14.2 */

  return E_OK;
# endif /* UDPNM_UL_IMMEDIATE_TX_SUPPORT */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_COM_USER_DATA_SUPPORT */

/**********************************************************************************************************************
 *  UdpNm_VLocalMainFunction
 *********************************************************************************************************************/
/*! \brief      Local implementation of the UdpNm_MainFunction_X().
 *  \param[in]  UdpNmChannelHandle   Identification of the NM-channel
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_VLocalMainFunction(CONST(NetworkHandleType, AUTOMATIC) UdpNmChannelIdx)
{
#if ( UDPNM_PN_ENABLED == STD_ON )
  uint8 pnByte;
  uint8 rxPnBufferNew[UDPNM_PN_INFO_LENGTH_BYTE];
  boolean IsNmPduReceived;
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
  boolean IsNmPduTransmitted;
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */
#endif /* UDPNM_PN_ENABLED */

#if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
  uint8 successfulTransmission;
#endif

  /* DET checks */
  UdpNm_CheckDetErrorReturnVoid(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_MAIN_FUNCTION,
    UDPNM_E_NO_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnVoid(
    (UDPNM_CHANNEL_CNT > UdpNmChannelIdx),
    UDPNM_API_ID_MAIN_FUNCTION,
    UDPNM_E_INVALID_CHANNEL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if ( UDPNM_PN_ENABLED == STD_ON )
  UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* Save Rx-Indication in local variable and reset the global Rx-Indication variable.
   * This is done to recognize additional receptions occurring in interrupt-context during
   * the processing of the main function, which therefore must be processed in next main function cycle.
   */
  IsNmPduReceived                         = UdpNm_NmPduReceived[UdpNmChannelIdx];
  UdpNm_NmPduReceived[UdpNmChannelIdx]    = FALSE;
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
  IsNmPduTransmitted                      = UdpNm_NmPduTransmitted[UdpNmChannelIdx];
  UdpNm_NmPduTransmitted[UdpNmChannelIdx] = FALSE;
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */

  for( pnByte = 0; pnByte < UDPNM_PN_INFO_LENGTH_BYTE; pnByte++ )
  {
    /* Copy received Pn Information to local buffer for calculation (data consistency) */
    rxPnBufferNew[pnByte] = UdpNm_RxPnBuffers[UdpNmChannelIdx][pnByte];
    /* Clear for next reception */
    UdpNm_RxPnBuffers[UdpNmChannelIdx][pnByte] = 0x00;
  }

  UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif /* UDPNM_PN_ENABLED */

  /*
   * Timer handling
   */
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  /* Message Cycle Timer */
  if ( 0 < UdpNm_MsgCycleCnt[UdpNmChannelIdx] )
  {
    UdpNm_MsgCycleCnt[UdpNmChannelIdx]--;
  }
  /* Message Timeout Timer (only decrement if a NM PDU transmission is pending) */
  if ( (0 < UdpNm_MsgTimeoutCnt[UdpNmChannelIdx]) && (TRUE == UdpNm_MsgTxConfPending[UdpNmChannelIdx]) )
  {
    UdpNm_MsgTimeoutCnt[UdpNmChannelIdx]--;
  }
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
  if ( 0 < UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] )
  {
    UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx]--;
  }
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
  if ( 0 < UdpNm_RepeatMessageCnt[UdpNmChannelIdx] )
  {
    UdpNm_RepeatMessageCnt[UdpNmChannelIdx]--;
  }
  if ( 0 < UdpNm_TimeoutCnt[UdpNmChannelIdx] )
  {
    UdpNm_TimeoutCnt[UdpNmChannelIdx]--;
  }
  if ( 0 < UdpNm_WaitBusSleepCnt[UdpNmChannelIdx] )
  {
    UdpNm_WaitBusSleepCnt[UdpNmChannelIdx]--;
  }
  /*
   * Repeat Message State
   */
  if ( NM_STATE_REPEAT_MESSAGE == UdpNm_State[UdpNmChannelIdx] )
  {
    /* NM Timeout isn't allowed to expire in STATE_REPEAT_MESSAGE */
    if ( 0 == UdpNm_TimeoutCnt[UdpNmChannelIdx] )
    {
#if defined( UDPNM_E_NETWORK_TIMEOUT )
      Dem_ReportErrorStatus(UDPNM_E_NETWORK_TIMEOUT, DEM_EVENT_STATUS_FAILED);
#endif /* UDPNM_E_NETWORK_TIMEOUT */

      /* Timeout time expired -> restart timer */
      UdpNm_TimeoutCnt[UdpNmChannelIdx] = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
    }

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    /* Trigger transmission of a NM PDU every time the Message Cycle Time is expired */
    if ( 0 == UdpNm_MsgCycleCnt[UdpNmChannelIdx] )
    {
      /* Transmit NM PDU */
# if( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
      successfulTransmission = UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
# else
      (void)UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
# endif
      
# if( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
      if( UdpNm_ImmediateNmTransmissionsCnt[UdpNmChannelIdx] > 0U )
      {
        /* Restart Message Counter with Immediate Nm Transmission Cycle time */
        UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetImmediateNmCycleTicks(UdpNmChannelIdx);
        /* Do not decrement Immediate Cycle Counter in case transmit request was not accepted by SoAd */
        if( successfulTransmission == E_OK )
        {
          UdpNm_ImmediateNmTransmissionsCnt[UdpNmChannelIdx]--;
        }
      }
      else
# endif
      {
        /* Restart Message Counter with normal Message Cycle time */
        UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgCycleTicks(UdpNmChannelIdx);
      }
    }
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
    if ( 0 == UdpNm_RepeatMessageCnt[UdpNmChannelIdx] )
    {
      UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UDPNM_COUNTER_STOP;
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
# if ( UDPNM_NODE_DETECTION_ENABLED == STD_ON )
      /* reset 'Repeat Message Request Bit' in the CBV of the transmitted UDP NM message */
      UdpNm_TxPdu_ClearRptMsgReq(UdpNmChannelIdx);
# endif /* UDPNM_NODE_DETECTION_ENABLED */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
      if ( UDPNM_NETWORK_STATE_REQUESTED == UdpNm_NetworkState[UdpNmChannelIdx] )
      {
#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
        /* inform Nm about the state change */
        Nm_StateChangeNotification(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx),
          UdpNm_State[UdpNmChannelIdx], NM_STATE_NORMAL_OPERATION);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

        UdpNm_State[UdpNmChannelIdx] = NM_STATE_NORMAL_OPERATION;
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
        UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRemoteSleepIndTicks(UdpNmChannelIdx);
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
      }
      else
      {
#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
        /* inform Nm about the state change */
        Nm_StateChangeNotification(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx), \
          UdpNm_State[UdpNmChannelIdx], NM_STATE_READY_SLEEP);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */
        UdpNm_State[UdpNmChannelIdx] = NM_STATE_READY_SLEEP;
#if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
        /* Clear Immediate NM transmission count in case REPEAT_MESSAGE is left to READY_SLEEP state, due to a Network Release */
        UdpNm_ImmediateNmTransmissionsCnt[UdpNmChannelIdx] = 0U;
#endif
      }
    }
  }

  /*
   * Normal Operation State
   */
  else if ( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
  {
    if ( 0 == UdpNm_TimeoutCnt[UdpNmChannelIdx] )
    {
#if defined( UDPNM_E_NETWORK_TIMEOUT )
      Dem_ReportErrorStatus(UDPNM_E_NETWORK_TIMEOUT, DEM_EVENT_STATUS_FAILED);
#endif /* UDPNM_E_NETWORK_TIMEOUT */

      /* Timeout time expired -> restart timer */
      UdpNm_TimeoutCnt[UdpNmChannelIdx] = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
      /* inform Nm about the timeout */
       Nm_TxTimeoutException(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
    }
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
    if ( 0 == UdpNm_MsgCycleCnt[UdpNmChannelIdx] )
    {
      UdpNm_MsgCycleCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgCycleTicks(UdpNmChannelIdx);
      (void)UdpNm_VTransmitPduCyclic(UdpNmChannelIdx);
    }
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
    if ( 0 == UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] )
    {
      UdpNm_RemoteSleepInd[UdpNmChannelIdx] = TRUE;
      /* inform Nm about remote sleep indication */
      Nm_RemoteSleepIndication(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
    }
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
  }

  /*
   * Ready Sleep State
   */
  else if ( NM_STATE_READY_SLEEP == UdpNm_State[UdpNmChannelIdx] )
  {
    if ( 0 == UdpNm_TimeoutCnt[UdpNmChannelIdx] )
    {
      /* inform Nm about bus sleep mode */
      Nm_PrepareBusSleepMode(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));

#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
      /* inform Nm about the state change */
      Nm_StateChangeNotification(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx), UdpNm_State[UdpNmChannelIdx], \
        NM_STATE_PREPARE_BUS_SLEEP);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

      UdpNm_Mode[UdpNmChannelIdx]            = NM_MODE_PREPARE_BUS_SLEEP;
      UdpNm_State[UdpNmChannelIdx]           = NM_STATE_PREPARE_BUS_SLEEP;
      UdpNm_TimeoutCnt[UdpNmChannelIdx]      = UDPNM_COUNTER_STOP;
      UdpNm_WaitBusSleepCnt[UdpNmChannelIdx] = UdpNm_VCfgGetWaitBusSleepTicks(UdpNmChannelIdx);
    }
  }

  /*
   * Prepare Bus Sleep State
   */
  else if ( NM_STATE_PREPARE_BUS_SLEEP == UdpNm_State[UdpNmChannelIdx] )
  {
    if ( 0 == UdpNm_WaitBusSleepCnt[UdpNmChannelIdx] )
    {
      /* inform Nm about bus sleep mode */
      Nm_BusSleepMode(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));

#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
      /* inform Nm about the state change */
      Nm_StateChangeNotification(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx), UdpNm_State[UdpNmChannelIdx], \
        NM_STATE_BUS_SLEEP);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

      UdpNm_Mode[UdpNmChannelIdx]            = NM_MODE_BUS_SLEEP;
      UdpNm_State[UdpNmChannelIdx]           = NM_STATE_BUS_SLEEP;
      UdpNm_WaitBusSleepCnt[UdpNmChannelIdx] = UDPNM_COUNTER_STOP;
    }
  }

  /* Invalid State */
  else
  {
    /* Nothing to do here, required by MISRA */
  }

#if ( UDPNM_PN_ENABLED == STD_ON )
  /*
   * Partial Networking
   */
  if( TRUE == UdpNm_VCfgPnChannelIsPnEnabled(UdpNmChannelIdx) )
  {
    boolean  IsEiraRxIndToBeCalled;
    boolean  IsEraRxIndToBeCalled;
    uint8    ByteIdx;
    boolean  IsPnNmPduReceived;

    IsEiraRxIndToBeCalled = (boolean)FALSE;
    IsEraRxIndToBeCalled  = (boolean)FALSE;
    ByteIdx               = (uint8)UDPNM_PN_INFO_LENGTH_BYTE;
    IsPnNmPduReceived     = (boolean)(IsNmPduReceived && (UdpNm_RecentRxPdu_IsPNInfoContained(UdpNmChannelIdx)));

    while( 0 < ByteIdx )
    {
      ByteIdx--;
      {
        uint8 RelevantPnIdx;
        uint8 RxPnInfo;
        uint8 TxPnInfo;

        RelevantPnIdx = UdpNm_VCfgPnGlobalGetRelevantPnCnt(ByteIdx);
        RxPnInfo      = rxPnBufferNew[ByteIdx];
        TxPnInfo      = UdpNm_TxPduBuffers[UdpNmChannelIdx][UDPNM_PN_INFO_OFFSET + ByteIdx];

# if ( UDPNM_PN_FILTER_MASK_ENABLED == STD_ON )
        RxPnInfo &= UdpNm_VCfgPnGlobalGetPnFilterMask(ByteIdx);
        TxPnInfo &= UdpNm_VCfgPnGlobalGetPnFilterMask(ByteIdx);
# endif /* UDPNM_PN_FILTER_MASK_ENABLED */
        /* only process the relevant PNs for this node (PN Bits set in filter mask) */
        while( 0 < RelevantPnIdx)
        {
          RelevantPnIdx--;
          {
            uint8 BitShiftVal;
            uint8 PnBitMask;
            uint8 PnBitStateRx;
            uint8 PnBitStateTx;

            BitShiftVal  = UdpNm_VCfgPnGlobalGetPnPositions(ByteIdx)[RelevantPnIdx];
            PnBitMask    = (uint8)((uint32)1U << (uint32)BitShiftVal);
            PnBitStateRx = (uint8)((uint32)RxPnInfo & (uint32)PnBitMask);
            PnBitStateTx = (uint8)((uint32)TxPnInfo & (uint32)PnBitMask);
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
            /* 
             * EIRA Handling
             */
            if( TRUE == IsPnNmPduReceived )
            {
              /* EIRA PN State RX */
              /* check if PN is requested in received NM PDU */
              if( 0U !=  PnBitStateRx )
              { /* PN is requested */
                /* check if PN has changed its state from 'PN Released' to 'PN Requested' */
                if( 0U !=  (PnBitStateRx ^ ( UdpNm_PnGlobal_PnStates[ByteIdx] & PnBitMask )) )
                { /* EIRA PN state has changed */
                  /* trigger call of PduR Rx Indication to indicate that a PN has changed its state */
                  IsEiraRxIndToBeCalled = TRUE;
                }
                /* Reset PN Timeout counter */
                UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx] = UDPNM_PN_EIRA_PN_TIMEOUT_TICKS;
                /* Set PN Bit in State/EIRA Bitfield */
                UdpNm_PnGlobal_PnStates[ByteIdx] |= PnBitMask;
              }
            }
            if( TRUE == IsNmPduTransmitted )
            {
              /* EIRA PN State TX */
              if( 0U != PnBitStateTx )
              { /* PN is requested */
                /* check if PN has changed its state from 'PN Released' to 'PN Requested' */
                if( 0U !=  (PnBitStateTx ^ ( UdpNm_PnGlobal_PnStates[ByteIdx] & PnBitMask )) )
                { /* EIRA PN state has changed */
                  /* trigger call of PduR Rx Indication to indicate that a PN has changed its state */
                  IsEiraRxIndToBeCalled = TRUE;
                }
                /* Reset PN Timeout counter */
                UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx] = UDPNM_PN_EIRA_PN_TIMEOUT_TICKS;
                /* Set PN Bit in State/EIRA Bitfield */
                UdpNm_PnGlobal_PnStates[ByteIdx] |= PnBitMask;
              }
            }

            /* Because EIRA PN Timeout must be independent from the main function cycle time it is only calculated
             * in one main function. The main function is selected by the smallest tick time to achieve the best
             * resolution.
             */
            UDPNM_IF_IS_EIRA_CALC_CHANNEL(UdpNmChannelIdx)
            {
              /* EIRA PN Timeout */
              if( 0 < UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx] )
              {
                UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx]--;
              }
              else if( 0 == UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx] )
              { /* Timeout exceeded -> PN state change to 'PN Released' */
                /* Reset PN Bit in State/EIRA Bitfield */
                UdpNm_PnGlobal_PnStates[ByteIdx] &= (uint8)(~(uint32)PnBitMask);
                /* trigger call of PduR Rx Indication to indicate that a PN has changed its state */
                IsEiraRxIndToBeCalled = TRUE;
                /* Stop counter */
                UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx)[RelevantPnIdx] = UDPNM_COUNTER_STOP;
              }
              else
              {
                /* nothing to do, only for MISRA compliance */
              }
            }
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */
# if ( UDPNM_PN_ERA_CALC_ENABLED == STD_ON )
            /* 
             * ERA Handling
             */
            if( TRUE == UdpNm_VCfgPnChannelIsEraEnabled(UdpNmChannelIdx))
            {
              if( TRUE == IsPnNmPduReceived )
              {
                /* ERA PN State */
                /* check if PN is requested in received NM PDU */
                if( 0U !=  PnBitStateRx )
                { /* PN is requested */
                  /* check if PN has changed its state from 'PN Released' to 'PN Requested' */
                  if( 0U != (PnBitStateRx ^ ( UdpNm_VCfgPnChannelGetPnStates(UdpNmChannelIdx, ByteIdx) & PnBitMask)) )
                  { /* PRQA S 0715 */ /* MISRA 1.1: Splitting into functions is less efficient */
                    /* ERA PN state has changed */
                    /* trigger call of PduR Rx Indication to indicate that a PN has changed its state */
                    IsEraRxIndToBeCalled = TRUE;
                  }
                  /* Reset PN Timeout counter */
                  UdpNm_VCfgPnChannelGetPnTimeoutCounters(UdpNmChannelIdx, ByteIdx)[RelevantPnIdx]
                    =  UdpNm_VCfgPnChannelGetTimeoutTicks(UdpNmChannelIdx);
                  /* Set PN Bit in State/ERA Bitfield */
                  UdpNm_VCfgPnChannelGetPnStates(UdpNmChannelIdx, ByteIdx) |= PnBitMask;
                }
              }

              /* ERA PN Timeout */
              if( 0 < UdpNm_VCfgPnChannelGetPnTimeoutCounters(UdpNmChannelIdx, ByteIdx)[RelevantPnIdx] )
              {
                UdpNm_VCfgPnChannelGetPnTimeoutCounters(UdpNmChannelIdx, ByteIdx)[RelevantPnIdx]--;
              }
              else if( 0 == UdpNm_VCfgPnChannelGetPnTimeoutCounters(UdpNmChannelIdx, ByteIdx)[RelevantPnIdx] )
              { /* Timeout exceeded -> PN state change to 'PN Released' */
                /* Reset PN Bit in State/ERA Bitfield */
                UdpNm_VCfgPnChannelGetPnStates(UdpNmChannelIdx, ByteIdx) &= (uint8)(~(uint32)PnBitMask);
                /* trigger call of PduR Rx Indication to indicate that a PN has changed its state */
                IsEraRxIndToBeCalled = TRUE;
                /* Stop counter */
                UdpNm_VCfgPnChannelGetPnTimeoutCounters(UdpNmChannelIdx, ByteIdx)[RelevantPnIdx] = UDPNM_COUNTER_STOP;
              }
              else
              {
                /* nothing to do, only for MISRA compliance */
              }
            }
# endif /* UDPNM_PN_ERA_CALC_ENABLED */
          }
        }
      }
    }
# if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
    if( TRUE == IsEiraRxIndToBeCalled )
    {
      /* call PduR Rx Indication to indicate a change in an EIRA PN state */
      PduR_UdpNmRxIndication(UDPNM_PN_EIRA_RX_N_SDU_ID, &UdpNm_PnGlobal_EiraPdu);
    }
# endif /* UDPNM_PN_EIRA_CALC_ENABLED */
# if ( UDPNM_PN_ERA_CALC_ENABLED == STD_ON )
    if( TRUE == IsEraRxIndToBeCalled )
    {
      /* set pointer of PduInfoType to ERA data of channel */
      UdpNm_PnChannel_SharedEraPdu.SduDataPtr = &UdpNm_VCfgPnChannelGetPnStates(UdpNmChannelIdx, 0U);
      /* call PduR Rx Indication to indicate a change in an ERA PN state */
      PduR_UdpNmRxIndication(UdpNm_VCfgPnChannelGetEraPduId(UdpNmChannelIdx), &UdpNm_PnChannel_SharedEraPdu);
    }
# endif /* UDPNM_PN_ERA_CALC_ENABLED */
  }
#endif /* UDPNM_PN_ENABLED */

} /* PRQA S 2006, 6010, 6030, 6050, 6080 */ /*  MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  UdpNm_SoAdIfTxConfirmation / UdpNm_SoAdTxConfirmation
 *********************************************************************************************************************/
/*! \brief      This service confirms a previous successfully processed transmit request.
 *  \param[in]  UdpNmTxPduId         Identification of the network through PDU-ID
 *  \context    Interrupt or task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfTxConfirmation(PduIdType UdpNmTxPduId)
{
  UdpNm_ChannelIdxType   UdpNmChannelIdx;
    
  /* DET checks */
  UdpNm_CheckDetErrorReturnVoid(
    (UDPNM_NM_TX_PDU_CNT > UdpNmTxPduId),
    UDPNM_API_ID_SOADIF_TX_CONFIRMATION,
    UDPNM_E_INVALID_PDUID); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNmChannelIdx = UdpNm_VCfgGetNmTxPduId2ChannelId(UdpNmTxPduId);

  UdpNm_CheckDetErrorReturnVoid(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_SOADIF_TX_CONFIRMATION,
    UDPNM_E_INVALID_CHANNEL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnVoid(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_SOADIF_TX_CONFIRMATION,
    UDPNM_E_NO_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
#  if ( UDPNM_PDUR_TX_CONF_ENABLED == STD_ON )
  /*
   * Inform the PduR about transmission of I-PDU requested by the Trigger-Transmit-API.
   * According to ESCAN00070928 "R8 AR4-89: Support Optional Confirmation" this call must be optional.
   */
  if ( TRUE == UdpNm_VCfgGetIsTxConfToBeCalled(UdpNmChannelIdx) )
  {
    PduR_UdpNmTxConfirmation(UdpNm_VCfgGetPduRUserDataTxPduId(UdpNmChannelIdx));
  }
#  endif /* UDPNM_PDUR_TX_CONF_ENABLED */
# endif /* UDPNM_COM_USER_DATA_SUPPORT */

  if ( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] )
  {
    UdpNm_MsgTxConfPending[UdpNmChannelIdx] = FALSE;
    UdpNm_MsgTimeoutCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgTimeoutTicks(UdpNmChannelIdx);

    /* reload repeat Nm timeout timer */
    UdpNm_TimeoutCnt[UdpNmChannelIdx]    = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_SoAdIfRxIndication
 *********************************************************************************************************************/
/*! \brief      This service indicates a successful reception of a received NM message to the UdpNm after passing all
 *              filters and validation checks.
 *  \param[in]  udpNmRxPduId         Identification of the network through PDU-ID
 *  \param[in]  udpSduPtr            Pointer to the received SDU
 *  \context    Interrupt or task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication(PduIdType RxPduId,
                                                P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr)
{
  UdpNm_ChannelIdxType   UdpNmChannelIdx;

  /* DET checks */
  UdpNm_CheckDetErrorReturnVoid(
    (UDPNM_NM_RX_PDU_CNT > RxPduId),
    UDPNM_API_ID_SOADIF_RX_INDICATION,
    UDPNM_E_INVALID_PDUID); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNmChannelIdx = UdpNm_VCfgGetNmRxPduId2ChannelId(RxPduId);

  UdpNm_CheckDetErrorReturnVoid(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_SOADIF_RX_INDICATION,
    UDPNM_E_INVALID_CHANNEL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnVoid(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_SOADIF_RX_INDICATION,
    UDPNM_E_NO_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnVoid(
    (NULL_PTR != PduInfoPtr),
    UDPNM_API_ID_SOADIF_RX_INDICATION,
    UDPNM_E_NULL_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if ( UDPNM_PN_FILTER_MASK_ENABLED == STD_ON )
  if( ( UdpNm_PnFilterHandling( UdpNmChannelIdx, PduInfoPtr ) ) ||
      ( UdpNm_VCfgIsAllNmMessagesKeepAwake(UdpNmChannelIdx) == TRUE ) )
#endif
  {
    if ( NM_MODE_NETWORK == UdpNm_Mode[UdpNmChannelIdx] )
    {
      /* reload UdpNm Timeout */
      UdpNm_TimeoutCnt[UdpNmChannelIdx] = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);

#if ( UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
      if ( NM_STATE_NORMAL_OPERATION == UdpNm_State[UdpNmChannelIdx] )
      {
        UdpNm_RemoteSleepIndCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRemoteSleepIndTicks(UdpNmChannelIdx);
      }
      if (( NM_STATE_REPEAT_MESSAGE != UdpNm_State[UdpNmChannelIdx] )
              && ( TRUE == UdpNm_RemoteSleepInd[UdpNmChannelIdx] ))
      {
        UdpNm_RemoteSleepInd[UdpNmChannelIdx] = FALSE;

        /* inform Nm about remote sleep cancellation */
        Nm_RemoteSleepCancellation(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
      }
#endif /* UDPNM_REMOTE_SLEEP_IND_ENABLED */
    }
    else if ( NM_MODE_PREPARE_BUS_SLEEP == UdpNm_Mode[UdpNmChannelIdx] )
    {
      /* inform Nm about network mode */
      Nm_NetworkMode(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));

#if ( UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON )
      /* inform Nm about the state change */
      Nm_StateChangeNotification(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx), \
        UdpNm_State[UdpNmChannelIdx], NM_STATE_REPEAT_MESSAGE);
#endif /* UDPNM_STATE_CHANGE_IND_ENABLED */

      UdpNm_Mode[UdpNmChannelIdx]             = NM_MODE_NETWORK;
      UdpNm_State[UdpNmChannelIdx]            = NM_STATE_REPEAT_MESSAGE;
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
      UdpNm_MsgCycleCnt[UdpNmChannelIdx]      = UdpNm_VCfgGetMsgCycleOffsetTicks(UdpNmChannelIdx);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
      UdpNm_RepeatMessageCnt[UdpNmChannelIdx] = UdpNm_VCfgGetRepeatMessageTicks(UdpNmChannelIdx);
      UdpNm_TimeoutCnt[UdpNmChannelIdx]       = UdpNm_VCfgGetTimeoutTicks(UdpNmChannelIdx);
    }
    else if ( NM_MODE_BUS_SLEEP == UdpNm_Mode[UdpNmChannelIdx] )
    {
      /* inform Nm about network start */
      Nm_NetworkStartIndication(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
    }
    else
    {
      /* nothing to do, only for MISRA compliance */
    }

    /* copy received PDU */
    /* NM PDU Length is configured globally -> no check of length of PduInfoPtr */
    UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* PRQA S 3305 5 */ /* MD_UdpNm_3305_IpBase */
    /* PRQA S 310 4 */ /* MD_UdpNm_310_IpBase */
    IpBase_Copy(
        (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&UdpNm_RxPduBuffers[UdpNmChannelIdx][0],
        (IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))&PduInfoPtr->SduDataPtr[0],
        (uint32)UdpNm_VCfgGetPduLength(UdpNmChannelIdx));

#if ( UDPNM_PN_ENABLED == STD_ON )
    /* Indicate that a NM PDU was received and processing must be done
     * in next main function cycle.
     */
    UdpNm_NmPduReceived[UdpNmChannelIdx] = TRUE;
#endif /* UDPNM_PN_ENABLED */

    UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if (( STD_ON == UDPNM_NODE_DETECTION_ENABLED ) && ( STD_ON == UDPNM_REPEAT_MSG_IND_ENABLED ))
    if ( TRUE == UdpNm_RecentRxPdu_IsRptMsgRequested(UdpNmChannelIdx) )
    {
      if ( E_NOT_OK == UdpNm_RepeatMessageRequest(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx)) )
      {
        UdpNm_CallDetReportError(
          UDPNM_API_ID_SOADIF_RX_INDICATION,
          UDPNM_E_INVALID_CHANNEL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }

      /* Inform Nm about 'Repeat Message Indication Bit' */
      Nm_RepeatMessageIndication(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
    }
#endif /* UDPNM_NODE_DETECTION_ENABLED, UDPNM_REPEAT_MSG_IND_ENABLED */

#if ( UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON )
    if ( TRUE == UdpNm_RecentRxPdu_IsReadySleep(UdpNmChannelIdx) )
    {
      Nm_CoordReadyToSleepIndication(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
    }
#endif /* UDPNM_COORDINATOR_SYNC_SUPPORT */

#if ( UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON )
    /* inform Nm about received PDU */
    Nm_PduRxIndication(UdpNm_VCfgGetComMChannelHandle(UdpNmChannelIdx));
#endif /* UDPNM_PDU_RX_INDICATION_ENABLED */
  }
} /* PRQA S 2006, 6050 */ /* MD_MSR_14.7, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
/**********************************************************************************************************************
 *  UdpNm_VTransmitPduCyclic
 *********************************************************************************************************************/
/*! \brief      Local function used in main function to transmit a NM PDU for corresponding Ethernet Network.
 *  \param[in]  UdpNmChannelIdx       Index of UdpNm local Channel Idx (doesn't correspond with global Channel Handle)
 *  \return     E_OK                  - Transmission request successfully executed
 *              E_NOT_OK              - Transmission request was rejected by SoAd
 *  \note       Function gets Tx PDU Data for addressed channel, modifies the PDU structure passed to SoAd and calls
 *              the internal UdpNm_VTransmitPdu.\n
 *              If UDPNM_COM_USER_DATA_SUPPORT is enabled the function collects the Tx PDU Data by calling the 
 *              Trigger Transmit API of PduR (PduR_UdpNmTriggerTransmit).
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_VTransmitPduCyclic(CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx)
{
  /* DET checks */
  UdpNm_CheckDetErrorReturnValue(
    (NM_STATE_UNINIT != UdpNm_State[UdpNmChannelIdx]),
    UDPNM_API_ID_V_TRANSMIT_PDU,
    UDPNM_E_NO_INIT,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  UdpNm_CheckDetErrorReturnValue(
    (UDPNM_INVALID_CHANNEL_IDX != UdpNmChannelIdx),
    UDPNM_API_ID_V_TRANSMIT_PDU,
    UDPNM_E_INVALID_CHANNEL,
    E_NOT_OK); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* transmit PDU transforming return code */
  if ( TRUE == UdpNm_ComEnabled[UdpNmChannelIdx] )
  {
# if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
    /* modify TxPduInfo to point to User Data location within the NM PDU */
    UdpNm_TxPduInfo[UdpNmChannelIdx].SduDataPtr =
      &(UdpNm_TxPduBuffers[UdpNmChannelIdx][UdpNm_VCfgGetPduUserDataPosition(UdpNmChannelIdx)]);

    UdpNm_TxPduInfo[UdpNmChannelIdx].SduLength = UdpNm_VCfgGetUserDataLength(UdpNmChannelIdx);

    /* trigger copy of User Data into Tx PDU */
    if( E_OK != PduR_UdpNmTriggerTransmit(UdpNm_VCfgGetPduRUserDataTxPduId(UdpNmChannelIdx),
      &UdpNm_TxPduInfo[UdpNmChannelIdx]) )
    {
      UdpNm_CallDetReportError(
        UDPNM_API_ID_V_TRANSMIT_PDU,
        UDPNM_E_PDUR_TRIGGERTX_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    /* check user data length */
    UdpNm_CheckDetErrorReturnValue(
      (UdpNm_VCfgGetUserDataLength(UdpNmChannelIdx) == UdpNm_TxPduInfo[UdpNmChannelIdx].SduLength),
      UDPNM_API_ID_V_TRANSMIT_PDU,
      UDPNM_E_PDUR_TRIGGERTX_ERROR, E_NOT_OK ); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* re modify TxPduInfo to point to whole NM PDU */
    UdpNm_TxPduInfo[UdpNmChannelIdx].SduDataPtr = &(UdpNm_TxPduBuffers[UdpNmChannelIdx][0]);
    UdpNm_TxPduInfo[UdpNmChannelIdx].SduLength = UdpNm_VCfgGetPduLength(UdpNmChannelIdx);
# endif /* UDPNM_COM_USER_DATA_SUPPORT */
    return(UdpNm_VTransmitPdu(UdpNmChannelIdx, &UdpNm_TxPduInfo[UdpNmChannelIdx]));
  }
  else
  {
    return(E_NOT_OK);
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  UdpNm_VTransmitPdu
 *********************************************************************************************************************/
/*! \brief      Local function used to transmit a NM PDU for corresponding Ethernet Network.
 *  \param[in]  UdpNmChannelIdx       Index of UdpNm local Channel Idx (doesn't correspond with global Channel Handle)
 *  \param[in]  TxPduInfoPtr          Pointer to the PDU Information representing the PDU to be transmitted.
 *  \note       Function triggers the transmission of the PDU by calling SoAd_IfTransmit() and adapts internal states
 *              depending on the transmission of an NM PDU.
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_VTransmitPdu(
  CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_VAR) TxPduInfoPtr)
{
  Std_ReturnType RetVal;

  /* transmit PDU and reset Message Timeout */
  RetVal =  SoAd_IfTransmit(UdpNm_VCfgGetSoAdTxPduId(UdpNmChannelIdx), TxPduInfoPtr);

  if ( E_OK == RetVal )
  {
    UdpNm_MsgTxConfPending[UdpNmChannelIdx] = TRUE;
    UdpNm_MsgTimeoutCnt[UdpNmChannelIdx] = UdpNm_VCfgGetMsgTimeoutTicks(UdpNmChannelIdx);
# if ( UDPNM_PN_ENABLED == STD_ON )
#  if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
    UdpNm_NmPduTransmitted[UdpNmChannelIdx] = TRUE;
#  endif /* UDPNM_PN_EIRA_CALC_ENABLED */
# endif /* UDPNM_PN_ENABLED */
  }
  else
  {
# if defined( UDPNM_E_TCPIP_TRANSMIT_ERROR )
    Dem_ReportErrorStatus(UDPNM_E_TCPIP_TRANSMIT_ERROR, DEM_EVENT_STATUS_FAILED); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# endif /* UDPNM_E_TCPIP_TRANSMIT_ERROR */
  }
  return RetVal;
}

#endif /* UDPNM_PASSIVE_MODE_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_VGetUdpNmChannelIdx
 *********************************************************************************************************************/
/*! \brief      Local function used to retrieve UdpNm local Channel Idx from global Channel Handle.
 *  \param[in]  NmChannelHandle       ComM Channel Handle
 *  \return     UdpNm local Channel index (UDPNM_INVALID_CHANNEL_IDX if no UdpNm Channel for passed ComM Channel Handle
 *              exists)
 *  \note       Iterates over all UdpNm Channels until the passed Channel Handle matches the Channel Handle of the
 *              corresponding UdpNm Channel.
 *  \context    Task level
 *********************************************************************************************************************/
STATIC INLINE FUNC(UdpNm_ChannelIdxType, UDPNM_CODE) UdpNm_VGetUdpNmChannelIdx(
  CONST(NetworkHandleType, AUTOMATIC) NmChannelHandle)
{
  UdpNm_ChannelIdxType UdpNmChannelIdx = UDPNM_INVALID_CHANNEL_IDX;
  UdpNm_ChannelIdxType ChannelIdx      = UDPNM_CHANNEL_CNT;

  while( 0 < ChannelIdx)
  {
    --ChannelIdx;
    if( NmChannelHandle == UdpNm_VCfgGetComMChannelHandle(ChannelIdx) )
    {
      UdpNmChannelIdx = ChannelIdx;
      break;
    }
  }

  return UdpNmChannelIdx;
}

#if ( UDPNM_PN_FILTER_MASK_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  UdpNm_PnFilterHandling
 *********************************************************************************************************************/
STATIC INLINE FUNC( boolean, UDPNM_CODE ) UdpNm_PnFilterHandling(
  CONST(UdpNm_ChannelIdxType, AUTOMATIC) UdpNmChannelIdx,
  P2CONST( PduInfoType, AUTOMATIC, UDPNM_APPL_VAR ) PduInfoPtr )
{
  boolean      msgRelevant = FALSE;
  uint8_least  rxMsgBytePos;
  uint8        reqCluster;

  if( UdpNm_VCfgPnChannelIsPnEnabled(UdpNmChannelIdx) == TRUE )
  {
    if( (PduInfoPtr->SduDataPtr[UdpNm_VCfgGetPduCbvPosition(UdpNmChannelIdx)] & (uint32)UDPNM_PDU_CBV_BIT_PNI_MASK) == (uint32)UDPNM_PDU_CBV_BIT_PNI_MASK )
    {
      UDPNM_BEGIN_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      for( rxMsgBytePos = 0U; rxMsgBytePos < UDPNM_PN_INFO_LENGTH_BYTE; rxMsgBytePos++ )
      {
        if( UdpNm_VCfgPnGlobalGetPnFilterMask(rxMsgBytePos) != 0x00U )
        {
          reqCluster = (uint8)(PduInfoPtr->SduDataPtr[(UDPNM_PN_INFO_OFFSET + rxMsgBytePos)] & UdpNm_VCfgPnGlobalGetPnFilterMask(rxMsgBytePos));
          if( reqCluster != 0U ) /* PRQA S 0715 */ /* MD_MSR_1.1 */
          { /* PRQA S 0715 */ /* MD_MSR_1.1 */

            UdpNm_RxPnBuffers[UdpNmChannelIdx][rxMsgBytePos] |= reqCluster;

            msgRelevant = TRUE;
          }
        }
      }
      UDPNM_END_CRITICAL_SECTION0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    msgRelevant = TRUE;
  }

  return msgRelevant;
}
#endif

#define UDPNM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/*******************************************************************************
  Verify the configuration of the UDP Network Management
*******************************************************************************/
#if ( UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON )
# if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
#  error "UDP NM configuration issue: UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED cannot be activated if UDPNM_IMMEDIATE_RESTART_ENABLED is STD_ON."
# endif
#endif

#if( UDPNM_PN_HANDLE_MULTI_NET_REQ == STD_ON )
# if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_OFF )
#  error "UDP NM configuration issue: UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED must be STD_ON if UDPNM_PN_HANDLE_MULTI_NET_REQ is STD_ON."
# endif
#endif

/**********************************************************************************************************************
 * Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations
  MD_UdpNm_14.2:
    Reason:     Dummy statement to avoid compiler warnings.
    Risk:       There is no risk as such statements have no effect on the code.
    Prevention: Covered by code review.
  MD_UdpNm_781:
    Reason:     Identifier in local scope is selected to have the same naming as the member of the structure for
                fast and clear identification of the data used in the local scope.
    Risk:       No risk
    Prevention: Covered by code review.
  MD_UdpNm_310_IpBase:
    Reason:     The IpBase API uses a different base type as the local element.
    Risk:       The IpBase API might cause access problems on some platforms if elements are not aligned properly.
    Prevention: Covered by component tests and review of IpBase.
  MD_UdpNm_3305_IpBase:
    Reason:     IpBase API  uses IpBase_CopyType as type of pointer to the data and therefore the cast
                is needed to pass the data.
    Risk:       Risk of an unaligned memory exception.
    Prevention: Covered by component tests and review of IpBase.
  MD_UdpNm_311_IpBase:
    Reason:     IpBase API  uses a pointer pointing to variable data although the buffer is the source for the copy.
                To use IpBase API the pointer pointing to const data must be casted to the IpBase pointer type.
    Risk:       Risk of trying to write non writable memory space within IpBase API.
    Prevention: Covered by component tests and review of IpBase.
  MD_UdpNm_3206:
    Reason:     In some configuration constelations the parameters passed to the API aren't needed.
    Risk:       There is no risk.
    Prevention: Covered by code review.
  MD_UdpNm_3451:
    Reason:     Schedulable entities are declared by the RTE and also by the BSW modules.
    Risk:       No functional risk.
    Prevention: Not required.
 */

/**********************************************************************************************************************
 *  END OF FILE: UdpNm.c
 *********************************************************************************************************************/
 
