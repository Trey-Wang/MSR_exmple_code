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
 *         File:  UdpNm_Types.h
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

#ifndef UDPNM_TYPES_H /* PRQA S 0883 1 */ /* MD_MSR_19.15 */
#define UDPNM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "UdpNm_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 UdpNm_NetworkStateType;
typedef uint8 UdpNm_PduPositionType;
typedef uint8 UdpNm_ConfigType;
typedef uint8 UdpNm_ChannelIdxType;

#if ( UDPNM_PN_ENABLED == STD_ON )
typedef boolean UdpNm_PnStateType;
/*
 * The UdpNm_Pn_ConfigType is a Bitfield containing the configuration flags of the Partial Networking Feature
 * for a specific channel.
 *
 *  ===========================================================================================
 *  Bit  | Name                    | Description
 *  -------------------------------------------------------------------------------------------
 *  0    | PN Enabled              | Indicates if partial networking for the specific channel
 *       |                         | is enabled
 *  1    | Era Enabled             | Indicates if Era calculation for the specific channel is
 *       |                         | enabled
 *  2    | Multi Netw Req Handling | Indicates if the Multiple Network Request Handling is
 *       |                         | enabled
 *  3-7  | Reserved                | Reserved for future purposes
 *  ===========================================================================================
 */
typedef uint8 UdpNm_Pn_ConfigType;

#endif /* UDPNM_PN_ENABLED */

typedef struct  
{
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  PduIdType SoAdTxPduId;
#  if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
  PduIdType PduRUserDataTxPduId;
#    if ( UDPNM_PDUR_TX_CONF_ENABLED == STD_ON )
  boolean HasTxConfToBeCalled;
#    endif /* UDPNM_PDUR_TX_CONF_ENABLED */
#  endif /* UDPNM_COM_USER_DATA_SUPPORT */
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
  UdpNm_PduPositionType PduCbvPosition;
  UdpNm_PduPositionType PduNidPosition;
  PduLengthType PduLength;
  PduLengthType UserDataLength;
  uint8 PduUserDataPosition;
} UdpNm_PduConfigType;

typedef struct
{
#if ( UDPNM_PASSIVE_MODE_ENABLED == STD_OFF )
  sint32 MsgCycleOffsetTicks;
  sint32 MsgCycleTicks;
  sint32 MsgTimeoutTicks;
#endif /* UDPNM_PASSIVE_MODE_ENABLED */
  sint32 RemoteSleepIndTicks;
  sint32 RepeatMessageTicks;
  sint32 TimeoutTicks;
  sint32 WaitBusSleepTicks;
  uint16 ImmediateNmCycleTime;
  uint8  ImmediateNmTransmissions;
#if ( UDPNM_NODE_ID_ENABLED == STD_ON )
  uint8 NodeId;
#endif /* UDPNM_NODE_ID_ENABLED */
  uint8  AllNmMessagesKeepAwake;
  uint8 ComMChannelHandle;
} UdpNm_ChannelConfigType;

#endif /* UDPNM_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: UdpNm_Types.h
 *********************************************************************************************************************/
 
