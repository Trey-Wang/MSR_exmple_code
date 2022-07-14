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
 *         File:  Rtp_Types.h
 *       Module:  Tp_Rtp
 *    Generator:  Tp_Rtp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Types definitions of Realtime Transport Protocol (RTP)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(RTP_TYPES_H)
#define RTP_TYPES_H

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA S 0777, 0779 EOF */ /* MD_MSR_5.1 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "Rtp_Cfg.h"
#include "SoAd.h"
#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Rtp_StateType;

typedef enum
{
  RTP_STREAM_ENABLED                                                = 0U,
  RTP_STREAM_SHUTDOWN                                               = 1U,
  RTP_STREAM_DISABLED                                               = 2U
} Rtp_StreamStateType;

#define RTP_BYTES_PER_32WORD                                        4U
#define RTP_INVALID_RANDOM_NUMBER                                   0U

/* ----- RTP Fixed Header Fields --------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|X|   CC  |M|     PT      |        sequence number        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           timestamp                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |           synchronization source (SSRC) identifier            |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 */

typedef struct
{
  uint32        Ssrc;
  uint8         Version;
  uint8         NrCSRC;
  uint8         PayloadType;
  boolean       ExtensionHeader;
} Rtp_AddRtpHeaderType;

typedef struct
{
  uint32        RtpTimestamp;
  uint32        AsTimestamp;
  boolean       Padding;
  boolean       Marker;
} Rtp_TxRtpInfoType;

typedef struct
{
  uint32        RtpTimestamp;
  uint16        SequenceNumber;
  boolean       Padding;
  boolean       Marker;
} Rtp_RxRtpInfoType;

#define RTP_HEADER_SIZE                                             12U
#define RTP_CSRC_SIZE                                               4U

#define RTP_HEADER_OFFSET_FLAGS                                     0U
#define RTP_HEADER_OFFSET_SEQUENCENUMBER                            2U
#define RTP_HEADER_OFFSET_TIMESTAMP                                 4U
#define RTP_HEADER_OFFSET_SSRC                                      8U

#define RTP_HEADERFLAG_VERSION                                      0xC0U  /* 0b11000000-------- */
#define RTP_HEADERFLAG_PADDING                                      0x20U  /* 0b00100000-------- */
#define RTP_HEADERFLAG_EXTENSION                                    0x10U  /* 0b00010000-------- */
#define RTP_HEADERFLAG_CSRC_COUNT                                   0x0FU  /* 0b00001111-------- */
#define RTP_HEADERFLAG_MARKER                                       0x80U  /* 0b--------10000000 */
#define RTP_HEADERFLAG_PAYLOAD_TYPE                                 0x7FU  /* 0b--------01111111 */

#define RTP_GENERAL_HEADER_FLAGS                                    0x8000U  /* RTP Version = 2 */

#define RTP_VERSION                                                 2U

#define RTP_HEADERFLAG_BITOFFSET_VERSION                            6U
#define RTP_HEADERFLAG_BITOFFSET_PADDING                            5U
#define RTP_HEADERFLAG_BITOFFSET_EXTENSION                          4U
#define RTP_HEADERFLAG_BITOFFSET_CSRC_COUNT                         0U
#define RTP_HEADERFLAG_BITOFFSET_MARKER                             7U
#define RTP_HEADERFLAG_BITOFFSET_PAYLOAD_TYPE                       0U

/* ----- RTP Header Extension -----------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      defined by profile       |            length             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                       header extension                        |
 *  |                             ....                              |
 */

#define RTP_EXTENSIONHEADER_FIXED_SIZE                              4U
#define RTP_EXTENSIONHEADER_OFFSET_LENGH                            2U
#define RTP_EXTENSIONHEADER_BYTES_PER_LENGTH                        4U

/* --------------------------------------------------------------------------------------------------------------------
 * - RTCP packet formats
 * ------------------------------------------------------------------------------------------------------------------*/

/* ----- RTCP Fixed Header Fields --------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P| subtype |      PT       |            length             |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                           SSRC/CSRC                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_PACKETTYPE_SR                                      200U
#define RTP_RTCP_PACKETTYPE_RR                                      201U
#define RTP_RTCP_PACKETTYPE_SDES                                    202U
#define RTP_RTCP_PACKETTYPE_BYE                                     203U
#define RTP_RTCP_PACKETTYPE_APP                                     204U
#define RTP_RTCP_PACKETTYPE_AVB_RTCP                                208U

typedef struct
{
  uint16        Length;
  uint8         Version;
  uint8         Subtype;
  uint8         PayloadType;
  boolean       Padding;
} Rtp_RtcpHeaderType;

#define RTP_RTCP_FIXED_HEADER_SIZE                                  4U
#define RTP_RTCP_SSRC_SIZE                                          4U

#define RTP_RTCP_HEADERFLAG_VERSION                                 0xC0U  /* 0b11000000 */
#define RTP_RTCP_HEADERFLAG_PADDING                                 0x20U  /* 0b00100000 */
#define RTP_RTCP_HEADERFLAG_SUBTYPE                                 0x1FU  /* 0b00011111 */

#define RTP_RTCP_HEADERFLAG_BITOFFSET_VERSION                       6U
#define RTP_RTCP_HEADERFLAG_BITOFFSET_PADDING                       5U
#define RTP_RTCP_HEADERFLAG_BITOFFSET_SUBTYPE                       0U

#define RTP_RTCP_HEADER_PACKET_TYPE_OFFSET                          1U
#define RTP_RTCP_HEADER_LENGTH_OFFSET                               2U

/* ----- AVB RTCP: AVB Control RTCP Packet ----------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P| subtype |    PT=208     |           length=9            |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                           SSRC/CSRC                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         name (ASCII)                          |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      gmTimeBaseIndicator      |                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *  |                      gmIdentity - 80 bit                      |
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                      stream_id - 64 bit                       |
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         as_timestamp                          |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         rtp_timestamp                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_AVB_PACKET_SIZE                                    40U

#define RTP_RTCP_AVB_HEADER_FIXED_SIZE                              4U

#define RTP_RTCP_AVB_HEADER_FIXED_VSP                               0x80U /* Version = 2, Padding = 0, Subtype = 0 */
#define RTP_RTCP_AVB_HEADER_FIXED_VSP_OFFSET                        0U
#define RTP_RTCP_AVB_HEADER_FIXED_TYPE                              0xD0U /* Packet Type = 208 */
#define RTP_RTCP_AVB_HEADER_FIXED_TYPE_OFFSET                       1U
#define RTP_RTCP_AVB_HEADER_FIXED_LENGTH_HB                         0x00U /* Length = 9 */
#define RTP_RTCP_AVB_HEADER_FIXED_LENGTH_HB_OFFSET                  2U
#define RTP_RTCP_AVB_HEADER_FIXED_LENGTH_LB                         0x09U /* Length = 9 */
#define RTP_RTCP_AVB_HEADER_FIXED_LENGTH_LB_OFFSET                  3U

#define RTP_RTCP_AVB_SSRC_OFFSET                                    4U
#define RTP_RTCP_AVB_NAME_OFFSET                                    8U
#define RTP_RTCP_AVB_NAME_SIZE                                      4U
#define RTP_RTCP_AVB_GMTIMEBASEINDICATOR_OFFSET                     12U
#define RTP_RTCP_AVB_GMIDENTITY_OFFSET                              14U
#define RTP_RTCP_AVB_GMIDENTITY_SIZE                                10U
#define RTP_RTCP_AVB_STREAM_ID_OFFSET                               24U
#define RTP_RTCP_AVB_AS_TIMESTAMP_OFFSET                            32U
#define RTP_RTCP_AVB_RTP_TIMESTAMP_OFFSET                           36U

typedef uint8 Rtp_NameAsciiType[4];
typedef uint8 Rtp_GmIdentityType[10];

typedef struct
{
  uint64                StreamId;
  uint32                Ssrc;
} Rtp_AddAvbRtcpInfoType;

typedef struct
{
  uint32                NameAscii;
  uint32                AsTimestamp;
  uint32                RtpTimestamp;
  uint16                GmTimeBaseIndicator;
  Rtp_GmIdentityType    GmIdentity;
} Rtp_RxAvbRtcpInfoType;

/* ----- SR: Sender Report RTCP Packet --------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|   RC    |   PT=SR=200   |            length             | header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                        SSRC of sender                         |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |             NTP timestamp, most significant word              | sender
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  info
 *  |             NTP timestamp, least significant word             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         RTP timestamp                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     sender's packet count                     |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     sender's octet count                      |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                 SSRC_1 (SSRC of first source)                 | report
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
 *  |  fraction lost  |      cumulative number of packets lost      |   1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |           extended highest sequence number received           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                      interarrival jitter                      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         last SR (LSR)                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                  delay since last SR (DLSR)                   |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                SSRC_2 (SSRC of second source)                 | report
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
 *  :                              ...                              :   2
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                  profile-specific extensions                  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- RR: Receiver Report RTCP Packet --------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|   RC    |   PT=RR=201   |            length             | header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     SSRC of packet sender                     |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                 SSRC_1 (SSRC of first source)                 | report
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
 *  | fraction lost |       cumulative number of packets lost       |   1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |           extended highest sequence number received           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                      interarrival jitter                      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         last SR (LSR)                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                  delay since last SR (DLSR)                   |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                SSRC_2 (SSRC of second source)                 | report
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
 *  :                              ...                              :   2
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                  profile-specific extensions                  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_REPORT_HEADER_FIXED_SIZE                           4U
#define RTP_RTCP_REPORT_SSRC_SIZE                                   4U

#define RTP_RTCP_EMPTYRR_HEADER_FIXED_SIZE                          4U

#define RTP_RTCP_EMPTYRR_HEADER_FIXED_VPRC                          0x80U /* Version = 2, Padding = 0, RC = 0 */
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_VPRC_OFFSET                   0U
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_TYPE                          0xC9U /* Packet Type = 201 */
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_TYPE_OFFSET                   1U
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_HB                     0x00U /* Length = 1 */
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_HB_OFFSET              2U
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_LB                     0x01U /* Length = 1 */
#define RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_LB_OFFSET              3U

#define RTP_RTCP_EMPTYRR_SSRC_SIZE                                  4U
#define RTP_RTCP_EMPTYRR_PACKET_SIZE                                RTP_RTCP_EMPTYRR_HEADER_FIXED_SIZE + RTP_RTCP_EMPTYRR_SSRC_SIZE

/* ----- SDES: Source Description RTCP Packet -------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|   SC    |  PT=SDES=202  |            length             | header
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                          SSRC/CSRC_1                          | chunk
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   1
 *  |                          SDES items                           |
 *  |                              ...                              |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                          SSRC/CSRC_2                          | chunk
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   2
 *  |                          SDES items                           |
 *  |                              ...                              |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 */

#define RTP_RTCP_SDES_HEADER_FIXED_16BITWORD_OFFSET                 0U
#define RTP_RTCP_SDES_HEADER_FIXED_SIZE                             4U
#define RTP_RTCP_SDES_HEADER_PADDING_FLAG                           0x2000U
#define RTP_RTCP_SDES_HEADER_FIXED_16BITWORD                        0x81CAU
                                                                  /*    ++> Packet Type = 202                    */
                                                                  /*  ++> Version = 2, Padding = 0, SourceCount = 1  */

#define RTP_RTCP_SDES_LENGTH_OFFSET                                 2U
#define RTP_RTCP_SDES_FIRST_SSRC_OFFSET                             4U
#define RTP_RTCP_SDES_SSRC_SIZE                                     4U
#define RTP_RTCP_SDES_ITEM_TYPE_SIZE                                1U
#define RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE                     2U
#define RTP_RTCP_SDES_ITEM_FIRST_TYPE_OFFSET                        8U
#define RTP_RTCP_SDES_ITEM_FIRST_LENGTH_OFFSET                      9U
#define RTP_RTCP_SDES_ITEM_FIRST_VALUE_OFFSET                       10U
#define RTP_RTCP_SDES_CHUNK_SSRC_OFFSET                             0U
#define RTP_RTCP_SDES_ITEM_TYPE_OFFSET                              0U
#define RTP_RTCP_SDES_ITEM_LENGTH_OFFSET                            1U

/* ----- BYE: Goodbye RTCP Packet -------------------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P|   SC    |  PT=BYE=203   |            length             | header
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                           SSRC/CSRC                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                              ...                              |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |    length     |              reason for leaving               | (opt)
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_BYE_FIXED_HEADER_SIZE                              4U

#define RTP_RTCP_BYE_HEADER_FIXED_VPSC                              0x81U /* Version = 2, Padding = 0, SC = 1 */
#define RTP_RTCP_BYE_HEADER_FIXED_VPSC_OFFSET                       0U
#define RTP_RTCP_BYE_HEADER_FIXED_TYPE                              0xCBU /* Packet Type = 203 */
#define RTP_RTCP_BYE_HEADER_FIXED_TYPE_OFFSET                       1U
#define RTP_RTCP_BYE_HEADER_FIXED_LENGTH_HB                         0x00U /* Length = 1 */
#define RTP_RTCP_BYE_HEADER_FIXED_LENGTH_HB_OFFSET                  2U
#define RTP_RTCP_BYE_HEADER_FIXED_LENGTH_LB                         0x01U /* Length = 1 */
#define RTP_RTCP_BYE_HEADER_FIXED_LENGTH_LB_OFFSET                  3U

#define RTP_RTCP_BYE_SSRC_OFFSET                                    4U
#define RTP_RTCP_BYE_SSRC_SIZE                                      4U
#define RTP_RTCP_BYE_PACKET_SIZE                                    RTP_RTCP_BYE_FIXED_HEADER_SIZE + RTP_RTCP_BYE_SSRC_SIZE

/* ----- APP: Application-Defined RTCP Packet --------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |V=2|P| subtype |  PT=APP=204   |            length             |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                           SSRC/CSRC                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         name (ASCII)                          |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                  application-dependent data                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */


/* ----- END: End item which can be used for padding ----------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     END=0     |
 *  +-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_SDES_TYPE_END                                      0U

/* ----- CNAME: Canonical End-Point Identifier SDES Item ---------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    CNAME=1    |    length     |  user and domain name       ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define RTP_RTCP_SDES_TYPE_CNAME                                    1U

/* -----  NAME: User Name SDES Item -----------------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    NAME=2     |    length     |  common name of source      ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- EMAIL: Electronic Mail Address SDES Item ---------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    EMAIL=3    |    length     |  email address of source    ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- PHONE: Phone Number SDES Item --------------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    PHONE=4    |    length     |  phone number of source     ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- LOC: Geographic User Location SDES Item -----------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     LOC=5     |    length     | geographic location of site ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- TOOL: Application or Tool Name SDES Item ---------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    TOOL=6     |    length     | name/version of source appl ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- NOTE: Notice/Status SDES Item --------------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    NOTE=7     |    length     |  note about the source      ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/* ----- PRIV: Private Extensions SDES Item ---------------------------------------------------------------------------
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    PRIV=8     |    length     | prefix length |prefix string...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  ...             |  value string                               ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#endif /* RTP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Rtp_Types.h
 *********************************************************************************************************************/
