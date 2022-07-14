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
 *         File:  UdpNm_Priv.h
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

#ifndef UDPNM_PRIV_H /* PRQA S 0883 1 */ /* MD_MSR_19.15 */
#define UDPNM_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "UdpNm_Types.h"
#include "UdpNm_Cfg.h"

#if ( UDPNM_DEV_ERROR_DETECT == STD_ON )
# include "Det.h"
#endif /* UDPNM_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* only one instance is supported */
#define UDPNM_INSTANCE_ID          0u
#define UDPNM_INVALID_CHANNEL_IDX  (0xFFu)
#define UDPNM_COUNTER_STOP         (-1)

/**********************************************************************************************************************
 *  CRITICAL SECTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
#define UDPNM_BEGIN_CRITICAL_SECTION0() SchM_Enter_UdpNm_UDPNM_EXCLUSIVE_AREA_0()
#define UDPNM_END_CRITICAL_SECTION0()   SchM_Exit_UdpNm_UDPNM_EXCLUSIVE_AREA_0()

/* PRQA L:FCT_MACROS */

/**********************************************************************************************************************
 *  DEVELOPMENT ERROR TRACER (DET) MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
/* Development Error Tracer */
#if ( UDPNM_DEV_ERROR_DETECT == STD_ON )
# define UdpNm_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE          ) {if(!(CONDITION)) { (void)Det_ReportError( UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (API_ID), (ERROR_CODE)); return; }}
# define UdpNm_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) {if(!(CONDITION)) { (void)Det_ReportError( UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); }}
# define UdpNm_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE          ) {if(!(CONDITION)) { (void)Det_ReportError( UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (API_ID), (ERROR_CODE)); }}
# define UdpNm_CallDetReportError(                  API_ID, ERROR_CODE          )                     (void)Det_ReportError( UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (API_ID), (ERROR_CODE))
#else
# define UdpNm_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE          )
# define UdpNm_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
# define UdpNm_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE          )
# define UdpNm_CallDetReportError(                  API_ID, ERROR_CODE          )
#endif /* UDPNM_DEV_ERROR_DETECT */
/* PRQA L:FCT_MACROS */

/**********************************************************************************************************************
 *  CONFIGURATION ACCESS MACROS
 *********************************************************************************************************************/
#if ( defined(UDPNM_CONFIGURATION_VARIANT) && ((UDPNM_CONFIGURATION_VARIANT_PRECOMPILE == UDPNM_CONFIGURATION_VARIANT) \
      || (UDPNM_CONFIGURATION_VARIANT_LINKTIME == UDPNM_CONFIGURATION_VARIANT)) )
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */

/* NM PDU to Channel mapping */
#define UdpNm_VCfgGetNmTxPduId2ChannelId(NmTxPduId)     (UdpNm_NmTxPduId2ChannelId[(NmTxPduId)])
#define UdpNm_VCfgGetNmRxPduId2ChannelId(NmRxPduId)     (UdpNm_NmRxPduId2ChannelId[(NmRxPduId)])

/* channel configuration parameters */
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
#define UdpNm_VCfgGetMsgCycleOffsetTicks(Channel)       (UdpNm_ChannelConfigs[(Channel)].MsgCycleOffsetTicks)
#if ( UDPNM_IMMEDIATE_NM_TX_FEATURE_ENABLED == STD_ON )
#define UdpNm_VCfgGetImmediateNmCycleTicks(Channel)     (UdpNm_ChannelConfigs[(Channel)].ImmediateNmCycleTime)
#define UdpNm_VCfgGetImmediateNmTransmissions(Channel)  (UdpNm_ChannelConfigs[(Channel)].ImmediateNmTransmissions)
#endif
#define UdpNm_VCfgIsAllNmMessagesKeepAwake(Channel)     (UdpNm_ChannelConfigs[(Channel)].AllNmMessagesKeepAwake)
#define UdpNm_VCfgGetMsgCycleTicks(Channel)             (UdpNm_ChannelConfigs[(Channel)].MsgCycleTicks)
#define UdpNm_VCfgGetMsgTimeoutTicks(Channel)           (UdpNm_ChannelConfigs[(Channel)].MsgTimeoutTicks)
#endif
#define UdpNm_VCfgGetRemoteSleepIndTicks(Channel)       (UdpNm_ChannelConfigs[(Channel)].RemoteSleepIndTicks)
#define UdpNm_VCfgGetRepeatMessageTicks(Channel)        (UdpNm_ChannelConfigs[(Channel)].RepeatMessageTicks)
#define UdpNm_VCfgGetTimeoutTicks(Channel)              (UdpNm_ChannelConfigs[(Channel)].TimeoutTicks)
#define UdpNm_VCfgGetWaitBusSleepTicks(Channel)         (UdpNm_ChannelConfigs[(Channel)].WaitBusSleepTicks)
#if ( UDPNM_NODE_ID_ENABLED == STD_ON )
#define UdpNm_VCfgGetNodeId(Channel)                    (UdpNm_ChannelConfigs[(Channel)].NodeId)
#endif /* UDPNM_NODE_ID_ENABLED */
#define UdpNm_VCfgGetComMChannelHandle(Channel)         (UdpNm_ChannelConfigs[(Channel)].ComMChannelHandle)

/* PDU configuration */
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
#define UdpNm_VCfgGetSoAdTxPduId(Channel)               (UdpNm_PduConfigs[(Channel)].SoAdTxPduId)
#  if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
#define UdpNm_VCfgGetPduRUserDataTxPduId(Channel)       (UdpNm_PduConfigs[(Channel)].PduRUserDataTxPduId)
#    if ( UDPNM_PDUR_TX_CONF_ENABLED == STD_ON )
#define UdpNm_VCfgGetIsTxConfToBeCalled(Channel)        (UdpNm_PduConfigs[(Channel)].HasTxConfToBeCalled)
#    endif /* UDPNM_PDUR_TX_CONF_ENABLED */
#  endif /* UDPNM_COM_USER_DATA_SUPPORT */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
#define UdpNm_VCfgGetPduCbvPosition(Channel)            (UdpNm_PduConfigs[(Channel)].PduCbvPosition)
#define UdpNm_VCfgGetPduNidPosition(Channel)            (UdpNm_PduConfigs[(Channel)].PduNidPosition)
#define UdpNm_VCfgGetPduUserDataPosition(Channel)       (UdpNm_PduConfigs[(Channel)].PduUserDataPosition)
#define UdpNm_VCfgGetPduLength(Channel)                 (UdpNm_PduConfigs[(Channel)].PduLength)
#define UdpNm_VCfgGetUserDataLength(Channel)            (UdpNm_PduConfigs[(Channel)].UserDataLength)

/* Partial Networking */
#if ( UDPNM_PN_ENABLED == STD_ON )
/* Global PN configuration */
#define UDPNM_PN_CHANNEL_PN_ENABLED_MASK                0x01
#define UdpNm_VCfgPnGlobalGetPnFilterMask(ByteIdx)      (UdpNm_PnGlobal_FilterMask[(ByteIdx)])
#define UdpNm_VCfgPnGlobalGetRelevantPnCnt(ByteIdx)     (UdpNm_PnGlobal_RelevantPnInByte[(ByteIdx)])
#define UdpNm_VCfgPnGlobalGetPnPositions(ByteIdx)       (UdpNm_PnGlobal_PnPositions[(ByteIdx)])
#define UdpNm_VCfgPnChannelIsPnEnabled(Channel) \
  (boolean)((uint32)UdpNm_PnChannel_Config[(Channel)] & (uint32)UDPNM_PN_CHANNEL_PN_ENABLED_MASK)
#  if ( UDPNM_PN_EIRA_CALC_ENABLED == STD_ON )
/* EIRA PN configuration */
#define UdpNm_VCfgPnGlobalGetPnTimeoutCounters(ByteIdx) (UdpNm_PnGlobal_PnTimeoutCounters[(ByteIdx)])
#if ( 1 < UDPNM_CHANNEL_CNT )
# define UDPNM_IF_IS_EIRA_CALC_CHANNEL(ChannelIdx) if( (ChannelIdx) == UDPNM_PN_EIRA_CALC_CHANNEL )
#else
# define UDPNM_IF_IS_EIRA_CALC_CHANNEL(ChannelIdx)
#endif /* UDPNM_CHANNEL_CNT */
#  endif /* UDPNM_PN_EIRA_CALC_ENABLED */
#  if ( UDPNM_PN_ERA_CALC_ENABLED == STD_ON )
/* ERA PN configuration */
#define UDPNM_PN_CHANNEL_ERA_ENABLED_MASK               0x02
#define UDPNM_PN_CHANNEL_ERA_ENABLED_SHIFT              1U
#define UdpNm_VCfgPnChannelIsEraEnabled(Channel) \
  (boolean)(((uint8)UdpNm_PnChannel_Config[(Channel)] & (uint8)UDPNM_PN_CHANNEL_ERA_ENABLED_MASK) \
             >> UDPNM_PN_CHANNEL_ERA_ENABLED_SHIFT)
#define UdpNm_VCfgPnChannelGetPnStates(Channel, ByteIdx)          (UdpNm_PnChannel_PnStates[(Channel)][(ByteIdx)])
#define UdpNm_VCfgPnChannelGetPnTimeoutCounters(Channel, ByteIdx) \
  (UdpNm_PnChannel_PnTimeoutCounters[(Channel)][(ByteIdx)])
#define UdpNm_VCfgPnChannelGetEraPduId(Channel)                   (UdpNm_PnChannel_EraPduId[(Channel)])
#define UdpNm_VCfgPnChannelGetTimeoutTicks(Channel)               (UdpNm_PnChannel_PnTimeoutTicks[(Channel)])
#  endif /* UDPNM_PN_ERA_CALC_ENABLED */
#if (STD_ON == UDPNM_PN_HANDLE_MULTI_NET_REQ)
#define UDPNM_PN_CHANNEL_HANDLE_MULTI_NETW_REQ_MASK     0x04
#define UDPNM_PN_CHANNEL_HANDLE_MULTI_NETW_REQ_SHIFT    2U
#define UdpNm_VCfgPnChannelIsMultiNetwReqHandlingEnabled(Channel) \
  (boolean)(((uint8)UdpNm_PnChannel_Config[(Channel)] & (uint8)UDPNM_PN_CHANNEL_HANDLE_MULTI_NETW_REQ_MASK) \
             >> UDPNM_PN_CHANNEL_HANDLE_MULTI_NETW_REQ_SHIFT)
#endif /* UDPNM_PN_HANDLE_MULTI_NET_REQ */
#endif /* UDPNM_PN_ENABLED */

/* PRQA L:FCT_MACROS */
#endif /* UDPNM_CONFIGURATION_VARIANT */

/**********************************************************************************************************************
 *  CONTROL BIT VECTOR (CBV) MACROS
 *********************************************************************************************************************/
/* Structure of Control Bit Vector
  +-----++---------+---------+---------+---------+---------+---------+---------+---------+
  |     || Bit 7   | Bit 6   | Bit 5   | Bit 4   | Bit 3   | Bit 2   | Bit 1   | Bit 0   |
  |-----||---------|---------|---------|---------|---------|---------|---------|---------|
  |     ||         |         |         |         | NM      | Res     | Res     | Repeat  |
  | CBV || Res     | PNI     | Res     | Res     | Coord   | (NM     | (NM     | Message |
  |     ||         | Bit     |         |         | Sleep   |  Coord  |  Coord  | Request |
  |     ||         |         |         |         | Ready   |  ID)    |  ID)    |         |
  +-----++---------+---------+---------+---------+---------+---------+---------+---------+
*/

/* Control Bit Vector Helper Macros */
/* Repeat Message Request */
#define UDPNM_PDU_CBV_BIT_REPEAT_MSG_MASK     (0x01u)
/* NM Coordinator ID */
#define UDPNM_PDU_CBV_COORD_MASK              (0x06u)
#define UDPNM_PDU_CBV_COORD_SHIFT             (1u)
/* NM Coordinator Sleep Ready */
#define UDPNM_PDU_CBV_BIT_SLEEP_READY_MASK    (0x08u)
#define UDPNM_PDU_CBV_BIT_SLEEP_READY_SHIFT   (3u)
/* Partial Networking Information */
#define UDPNM_PDU_CBV_BIT_PNI_MASK            (0x40u)
#define UDPNM_PDU_CBV_BIT_PNI_SHIFT           (6u)

/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
/* Get the Control Bit Vector of the NM Tx PDU */
#define UdpNm_GetTxCbv(ChannelIdx)  UdpNm_TxPduBuffers[(ChannelIdx)][UdpNm_VCfgGetPduCbvPosition((ChannelIdx))]
/* Get the Control Bit Vector of the last received NM Rx PDU */
#define UdpNm_GetRxCbv(ChannelIdx)  UdpNm_RxPduBuffers[(ChannelIdx)][UdpNm_VCfgGetPduCbvPosition((ChannelIdx))]

/* Set the Coordinator ID in the Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_SetCoordIdBits(ChannelIdx, CoordIdBits)                                                                                      \
    {                                                                                                                                            \
      UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) & (~(uint32)UDPNM_PDU_CBV_COORD_MASK) );                    \
      UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) |                                                           \
                                             (((uint32)(CoordIdBits) << (uint32)UDPNM_PDU_CBV_COORD_SHIFT) & (uint32)UDPNM_PDU_CBV_COORD_MASK) ); \
    } /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Set Ready Sleep Bit in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_SetReadySleepBit(ChannelIdx) \
    UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) | (uint32)UDPNM_PDU_CBV_BIT_SLEEP_READY_MASK )

/* Clear the Ready Sleep Bit in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_ClearReadySleepBit(ChannelIdx) \
    UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32) UdpNm_GetTxCbv((ChannelIdx))) & (~(uint32)UDPNM_PDU_CBV_BIT_SLEEP_READY_MASK) )

/* Set Repeat Message Request in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_SetRptMsgReq(ChannelIdx) \
  UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) | (uint32)UDPNM_PDU_CBV_BIT_REPEAT_MSG_MASK )

/* Clear the Repeat Message Request in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_ClearRptMsgReq(ChannelIdx) \
    UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) & (~(uint32)UDPNM_PDU_CBV_BIT_REPEAT_MSG_MASK) )

/* Set Partial Network Information Bit in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_SetPNInfoBit(ChannelIdx) \
    UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) | (uint32)UDPNM_PDU_CBV_BIT_PNI_MASK )

/* Clear Partial Network Information Bit in Control Bit Vector of NM Tx PDU */
#define UdpNm_TxPdu_ClearPNIInfoBit(ChannelIdx) \
    UdpNm_GetTxCbv((ChannelIdx)) = (uint8)( ((uint32)UdpNm_GetTxCbv((ChannelIdx))) & (~(uint32)UDPNM_PDU_CBV_BIT_PNI_MASK) )

/* Get the Coordinator ID in Control Bit Vector of last received NM Rx PDU */
#define UdpNm_RecentRxPdu_GetCoordId(ChannelIdx) \
  (uint8)(((uint32)UdpNm_GetRxCbv((ChannelIdx)) & (uint32)UDPNM_PDU_CBV_COORD_MASK) \
    >> (uint32)UDPNM_PDU_CBV_COORD_SHIFT)

/* Check if Ready Sleep Bit is set in Control Bit Vector of last received NM Rx PDU */
#define UdpNm_RecentRxPdu_IsReadySleep(ChannelIdx) \
  (boolean)(((uint32)UdpNm_GetRxCbv((ChannelIdx)) & (uint32)UDPNM_PDU_CBV_BIT_SLEEP_READY_MASK) \
    >> (uint32)UDPNM_PDU_CBV_BIT_SLEEP_READY_SHIFT)

/* Check if Repeat Message Requested Bit is set in Control Bit Vector of last received NM Rx PDU */
#define UdpNm_RecentRxPdu_IsRptMsgRequested(ChannelIdx) \
  (boolean)((uint32)(UdpNm_GetRxCbv((ChannelIdx)) & (uint32)UDPNM_PDU_CBV_BIT_REPEAT_MSG_MASK))

/* Check if Partial Network Information Bit is set in Control Bit Vector of last received NM Rx PDU */
#define UdpNm_RecentRxPdu_IsPNInfoContained(ChannelIdx) \
  (boolean)(((uint32)UdpNm_GetRxCbv((ChannelIdx)) & (uint32)UDPNM_PDU_CBV_BIT_PNI_MASK) \
    >> (uint32)UDPNM_PDU_CBV_BIT_PNI_SHIFT)

/* PRQA L:FCT_MACROS */

#if ( defined(UDPNM_CONFIGURATION_VARIANT) && ((UDPNM_CONFIGURATION_VARIANT_LINKTIME < UDPNM_CONFIGURATION_VARIANT) \
       || (UDPNM_CONFIGURATION_VARIANT_PRECOMPILE > UDPNM_CONFIGURATION_VARIANT)) )
# error "UdpNm_Priv.h: invalid or missing UDPNM_CONFIGURATION_VARIANT, (value 1 or 2 expected in UdpNm_Cfg.h)"
#endif /* UDPNM_CONFIGURATION_VARIANT */

/**********************************************************************************************************************
 *  PRIVATE FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  UdpNm_VLocalMainFunction
 *********************************************************************************************************************/
/*! \brief      Local implementation of the UdpNm_MainFunction_X().
 *  \param[in]  UdpNmChannelHandle   Identification of the NM-channel
 *  \context    Task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_VLocalMainFunction(CONST(NetworkHandleType, AUTOMATIC) UdpNmChannelIdx);

#endif /* UDPNM_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: UdpNm_Priv.h
 *********************************************************************************************************************/
 
