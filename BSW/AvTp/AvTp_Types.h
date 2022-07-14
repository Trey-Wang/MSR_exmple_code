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
 *         File:  AvTp_Types.h
 *       Module:  Tp_Ieee1722AvTp
 *    Generator:  Tp_Ieee1722AvTp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Types definitions of Audio Video Bridging (AVB) Transport Protocol AvTp
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (AVTP_TYPES_H)
#define AVTP_TYPES_H


/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "AvTp_Cfg.h"
#include "EthIf.h"

#if (AVTP_STBM_SUPPORT == STD_ON)
#include "StbM.h"
#endif /* (AVTP_STBM_SUPPORT == STD_ON) */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
#define AVTP_P2CONSTCFGROOT(Type)              P2CONST(Type, AUTOMATIC, AVTP_CONST)
#define AVTP_P2CONSTCFG(Type)                  P2CONST(Type, AUTOMATIC, AVTP_CONST)
#define AVTP_P2CONST(Type)                     P2CONST(Type, AUTOMATIC, AVTP_APPL_DATA)
#define AVTP_P2VAR(Type)                       P2VAR(Type, AUTOMATIC, AVTP_APPL_VAR)
#define AVTP_P2FUNC(RType, Type)               P2FUNC(RType, AVTP_CODE, Type)

/* AvTp special values */
#define AVTP_VERSION                           (0U)
#define AVTP_BUF_IDX_INVALID                   (255U)
#define AVTP_CTRL_IDX_INVALID                  (255U)
#define AVTP_STREAM_ID_LEN_BYTE                (8U)
#define AVTP_MAAP_DYN_ALLOC_END                (0x0000FDFFU)
#define AVTP_MSG_INVALID                       (FALSE)
#define AVTP_MSG_VALID                         (TRUE)

/* ----- IEEE1722a Conformance ----- */
#define AVTP_IEEE1722A_2014_D8_CONFORMANCE     (0u)
#define AVTP_IEEE1722A_2015_D16_CONFORMANCE    (1u)

/* Common Header Bit Positons */
#define AVTP_HDR_COM_H_BIT_POS                 (7U)  /* Header specific flag */
#define AVTP_HDR_COM_VERSION_BIT_POS           (4U)  /* Version info */

/* Control Header Bit Positons */
#define AVTP_HDR_CTRL_SUBTYPE_BIT_POS          (24U) /* Subtype */
#define AVTP_HDR_CTRL_VERSION_BIT_POS          (20U) /* Status info */
#define AVTP_HDR_CTRL_SSD_BIT_POS              (11U) /* Subtype Specific Data */
#define AVTP_HDR_CTRL_CTRL_DATA_LEN_BIT_POS    (0U)  /* Contrl Data length info */

/* Stream Header Bit Positons */
#define AVTP_HDR_STREAM_SV_BIT_POS             (7U)  /* StreamId valid flag */
#define AVTP_HDR_STREAM_MR_BIT_POS             (3U)  /* Media clock Restart flag */
#define AVTP_HDR_STREAM_SSD_BIT_POS            (1U)  /* Subtype Specific Data */
#define AVTP_HDR_STREAM_TV_BIT_POS             (0U)  /* Timestamp Valid flag */
#define AVTP_HDR_STREAM_SSD1_BIT_POS           (1U)  /* Subtype Specific Data 1 */
#define AVTP_HDR_STREAM_TU_BIT_POS             (0U)  /* Timestamp Uncertain flag */

/* AVTP Header length values */
#define AVTP_HDR_LEN_COMMON                    (12U) /* Common Header length */
#define AVTP_HDR_LEN_CTRL                      (12U) /* Control Header length */
#define AVTP_HDR_LEN_STREAM                    (24U) /* Stream Header length */
#define AVTP_HDR_LEN_H264_STREAM               (28U) /* Stream Header length with H264 Timestamp */
#if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
# define AVTP_HDR_LEN_CRF_AUDIO                (20U) /* Clock Reference Format AUDIO Header Length */
# define AVTP_HDR_LEN_CRF_VIDEO                (20U) /* Clock Reference Format VIDEO Header Length */
#else
# define AVTP_HDR_LEN_CRF_AUDIO                (28U) /* Clock Reference Format AUDIO Header Length */
# define AVTP_HDR_LEN_CRF_VIDEO                (32U) /* Clock Reference Format VIDEO Header Length */
#endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */

/* Control / Data values */ 
#define AVTP_HDR_COM_CD_DATA                   (0x0U)
#define AVTP_HDR_COM_CD_CONTROL                (0x1U)

#if (STD_ON == AVTP_MAAP_SUPPORT)
/* MAAP Message Types */
#define AVTP_MAAP_MSG_TYPE_PROBE               (0x01U)
#define AVTP_MAAP_MSG_TYPE_DEFEND              (0x02U)
#define AVTP_MAAP_MSG_TYPE_ANNOUNCE            (0x03U)
#define AVTP_MAAP_VERSION                      (1U)
#define AVTP_MAAP_DATA_LEN                     (16U) /* IEEE 1722: Specification: 16 */
#define AVTP_MAAP_MSG_LEN                      (28U)

/* MAAP values */
#define AVTP_MAAP_HDR_VERSION_BIT_POS          (11U)
#endif /* (STD_ON == AVTP_MAAP_SUPPORT */

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/* IEC 61883 Bit Positions */
#define AVTP_61883IIDC_TAG_BIT_POS             (6U) /* Tag */
#define AVTP_61883IIDC_TCODE_BIT_POS           (4U) /* Type Code */
#define AVTP_61883IIDC_SY_BIT_POS              (0U) /* Sy */

/* IEC 61883: Tag */
#define AVTP_61883IIDC_TAG_NO_CIP              (0U)
#define AVTP_61883IIDC_TAG_CIP                 (1U)

/* IEC 61883: Channel */
#define AVTP_61883IIDC_CHANNEL_AVB_SOURCE      (31U)

/* IEC 61883: Type Code */
#define AVTP_61883IIDC_TCODE_TALKER            (0xA)
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* AVTP Audio Bit Positions */
#define AVTP_AAF_NRS_BIT_POS                   (4U) /* Nominal Sample Rate */
#define AVTP_AAF_CHPERFRAME_H_BIT_POS          (0U) /* Channels Per Frame High Byte */
#define AVTP_AAF_CHPERFRAME_L_BIT_POS          (0U) /* Channels Per Frame Low Byte */
#define AVTP_AAF_SP_BIT_POS                    (4U) /* Timestamp Spares mode */
#define AVTP_AAF_EVT_BIT_POS                   (0)  /* Event */

/* AVTP Audio Format (see Table 9 IEEE 1722a-2015-d16) */
#define AVTP_AAF_FMT_USERSPEC                  (0x00U)
#define AVTP_AAF_FMT_FLOAT_32BIT               (0x01U)
#define AVTP_AAF_FMT_INT_32BIT                 (0x02U)
#define AVTP_AAF_FMT_INT_24BIT                 (0x03U)
#define AVTP_AAF_FMT_INT_16BIT                 (0x04U)
#define AVTP_AAF_FMT_AES3_32BIT                (0x05U) /* Not supported */

/* AVTP Audio Nominal Sample Rate (see table 11 IEEE 1722a-2015-d16) */
#define AVTP_AAF_NSR_USERSPEC                  (0x00U)
#define AVTP_AAF_NSR_8KHZ                      (0x01U)
#define AVTP_AAF_NSR_16KHZ                     (0x02U)
#define AVTP_AAF_NSR_32KHZ                     (0x03U)
#define AVTP_AAF_NSR_441KHZ                    (0x04U)
#define AVTP_AAF_NSR_48KHZ                     (0x05U)
#define AVTP_AAF_NSR_882KHZ                    (0x06U)
#define AVTP_AAF_NSR_96KHZ                     (0x07U)
#define AVTP_AAF_NSR_1764KHZ                   (0x08U)
#define AVTP_AAF_NSR_192KHZ                    (0x09U)
#define AVTP_AAF_NSR_24KHZ                     (0x0AU)

#define AVTP_AAF_NSR_16RPM                     (10U)
#define AVTP_AAF_NSR_33RRPM                    (11U)
#define AVTP_AAF_NSR_33RPM_REVERSE             (12U)
#define AVTP_AAF_NSR_45RPM                     (13U)
#define AVTP_AAF_NSR_78RPM                     (14U)
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/* Compressed Video Bit Positions */
#define AVTP_CVF_M_BIT_POS                     (4u)    /* Marker Bit */
#define AVTP_CVF_EVT_BIT_POS                   (0u)    /* Event */
#define AVTP_CVF_H264_PTV_BIT_POS              (5u)    /* PTV */

/* Format */
#define AVTP_CVF_FMT_RFC                       (0x02u)

/* Format Subtype */
#define AVTP_CVF_FMTSUBTYPE_MJPEG              (0x00u)
#define AVTP_CVF_FMTSUBTYPE_H264               (0x01u)
#define AVTP_CVF_FMTSUBTYPE_JPEG2000           (0x02u)
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/* CRF: Types (see table 11.1 IEEE 1722a-2014-d8) */
#define AVTP_CRF_TYPE_USER                     (0U)
#define AVTP_CRF_TYPE_AUDIO_SAMPLE             (1U)
#define AVTP_CRF_TYPE_VIDEO_FRAME              (2U)
#define AVTP_CRF_TYPE_VIDEO_LINE               (3U)
#define AVTP_CRF_TYPE_MACHINE_CYCLE            (4U)

/* CRF: Clock Frequency */
#define AVTP_CRF_NSR_USERSPEC                  (0U)
#define AVTP_CRF_NSR_8KHZ                      (1U)
#define AVTP_CRF_NSR_16KHZ                     (2U)
#define AVTP_CRF_NSR_32KHZ                     (3U)
#define AVTP_CRF_NSR_441KHZ                    (4U)
#define AVTP_CRF_NSR_882KHZ                    (5U)
#define AVTP_CRF_NSR_1764KHZ                   (6U)
#define AVTP_CRF_NSR_48KHZ                     (7U)
#define AVTP_CRF_NSR_96KHZ                     (8U)
#define AVTP_CRF_NSR_192KHZ                    (9U)

/* CRF: Clock Multiplier (see table 11.3 IEEE 1722a-2014-d8) */
#define AVTP_CRF_CM_NOMINAL_FREQUENCY          (0U)
#define AVTP_CRF_CM_NTSC_VIDEO_DROP_DOWN       (1U)
#define AVTP_CRF_CM_NTSC_VIDEO_PULL_UP         (2U)
#define AVTP_CRF_CM_PAL_VIDEO_DROP_DOWN        (3U)
#define AVTP_CRF_CM_PAL_VIDEO_PULL_UP          (4U)
#define AVTP_CRF_CM_ONE_EIGHTH                 (5U)
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/* PRQA S 3453 16 */ /* MD_MSR_19.7 */
/* Common Header Stream ID valid indicator */
#define AVTP_HDR_COM_H(param)                  ( (0x80   & (param)) >> AVTP_HDR_COM_H_BIT_POS )
/* Common Header Version */
#define AVTP_HDR_COM_VERSION(param)            ( (0x70   & (param)) >> AVTP_HDR_COM_VERSION_BIT_POS )

/* Control Header Subtype */
#define HDR_CTRL_CTRL_SUBTYPE(param)           ( (0xFF000000 & (param)) >> AVTP_HDR_CTRL_SUBTYPE_BIT_POS )
/* Control Header Version */
#define AVTP_HDR_CTRL_VERSION(param)           ( (0x00300000 & (param)) >> AVTP_HDR_CTRL_VERSION_BIT_POS )
/* Control Header Subtype Specific Data */
#define AVTP_HDR_CTRL_SSD(param)               ( (0x000FF800 & (param)) >> AVTP_HDR_CTRL_SSD_BIT_POS )
/* Control Header Control Data Length */
#define AVTP_HDR_CTRL_DATA_LEN(param)          ( (0x000003FF & (param)) >> AVTP_HDR_CTRL_CTRL_DATA_LEN_BIT_POS )

/* PRQA S 3453 12 */ /* MD_MSR_19.7 */
/* Stream Header Stream valid */
#define AVTP_HDR_STREAM_SV(param)              ( (0x80   & (param)) >> AVTP_HDR_STREAM_SV_BIT_POS)
/* Stream Header Media Restart */
#define AVTP_HDR_STREAM_MR(param)              ( (0x08   & (param)) >> AVTP_HDR_STREAM_MR_BIT_POS )
/* Stream Header Subtype Specific Data */
#define AVTP_HDR_STREAM_SSD(param)             ( (0x06   & (param)) >> AVTP_HDR_STREAM_SSD_BIT_POS )
/* Stream Header Timestamp valid */
#define AVTP_HDR_STREAM_TV(param)              ( (0x01   & (param)) >> AVTP_HDR_STREAM_TV_BIT_POS )
/* Stream Header Reserved  */
#define AVTP_HDR_STREAM_SSD1(param)            ( (0xFE   & (param)) >> AVTP_HDR_STREAM_SSD1_BIT_POS )
/* Stream Header Timestamp uncertain */
#define AVTP_HDR_STREAM_TU(param)              ( (0x01   & (param)) >> AVTP_HDR_STREAM_TU_BIT_POS )

#if (STD_ON == AVTP_MAAP_SUPPORT)
/* PRQA S 3453 7 */ /* MD_MSR_19.7 */
/* MAAP: Header Version */
#define AVTP_MAAP_HDR_VERSION(param)           ( (0xF800 & (param) ) >> AVTP_MAAP_HDR_VERSION_BIT_POS )
/* MAAP: Header Data length */
#define AVTP_MAAP_HDR_DATA_LEN(param)          (  0x07FF & (param) )
/* MAAP: Message Type */
#define AVTP_MAAP_MSG_TYPE(param)              (  0x0F   & (param) )
#endif /* (STD_ON == AVTP_MAAP_SUPPORT */

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
/* IEC: Tag */
#define AVTP_61883IIDC_TAG(param)              ( (0xC0  & (param) ) >> AVTP_61883IIDC_TAG_BIT_POS )
/* IEC: Channel */
#define AVTP_61883IIDC_CHANNEL(param)          (  0x3F  & (param) ) 
/* IEC: Type Code */
#define AVTP_61883IIDC_TCODE(param)            ( (0xF0  & (param) ) >> AVTP_61883IIDC_TCODE_BIT_POS )
/* IEC: Sy */
#define AVTP_61883IIDC_SY(param)               (  0x0F  & (param) )
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* PRQA S 3453 16 */ /* MD_MSR_19.7 */
/* AVTP Audio: Nominal Sample Rate */
#define AVTP_AAF_NRS(param)                    ( (0xF0   & (param)) >> AVTP_AAF_NRS_BIT_POS )
/* AVTP Audio: Channels Per Frame */
#define AVTP_AAF_CHPERFRAME(high, low)         ( ((0x03  & (uint16)(high)) << 8 ) | (uint16)(low) )
#define AVTP_AAF_CHPERFRAME_HIGH(param)        ( (0x0300 & (param)) >> 8 )
#define AVTP_AAF_CHPERFRAME_LOW(param)         (  0x00FF & (param) ) 
/* AVTP Audio: Spares Timestampe Mode */
#define AVTP_AAF_SP(param)                     ( (0x10  & (param)) >> AVTP_AAF_SP_BIT_POS )
/* AVTP Audio: Event */                       
#define AVTP_AAF_EVT(param)                    ( (0x0F  & (param)) >> AVTP_AAF_EVT_BIT_POS )

/* AVTP Audio: 24Bit Formt access methods */
#define AVTP_AAF_24BIT_1ST(param)                ( (param) & 0x000000FF)
#define AVTP_AAF_24BIT_2ND(param)              ( ( (param) & 0x0000FF00)  >> 8)
#define AVTP_AAF_24BIT_3RD(param)              ( ( (param) & 0x00FF0000)  >> 16)

/* AVTP Audio: SampleOffset */
#define AVTP_AAF_32BIT_2BYTE                   (4u)
#define AVTP_AAF_24BIT_2BYTE                   (3u)
#define AVTP_AAF_16BIT_2BYTE                   (2u)
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/* PRQA S 3453 6 */ /* MD_MSR_19.7 */
/* Compressed Video: M */
#define AVTP_CVF_M(param)                      ( (0x10   & (param)) >> AVTP_CVF_M_BIT_POS )
/* Compressed Video: Event */
#define AVTP_CVF_EVENT(param)                  ( (0x0F   & (param)) >> AVTP_CVF_EVT_BIT_POS )
/* Compressed Video H264: Ptv */
#define AVTP_CVF_H264_PTV(param)               ( (0x20u  & (param)) >> AVTP_CVF_H264_PTV_BIT_POS )
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
#  define AVTP_CRF_PULL_BIT_POS                  (29u)
#  define AVTP_CRF_FS_BIT_POS                    (1u)
#  define AVTP_CRF_TU_BIT_POS                    (0u)
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
#  define AVTP_CRF_FS(param)                     (  0x02u  &  (param) )
#  define AVTP_CRF_TU(param)                     (  0x01u  &  (param) )
#  define AVTP_CRF_BASE_FREQUENCY(param)         (  0x1FFFFFFFu  & (param) )
#  define AVTP_CRF_PULL(param)                   (  0xE0000000u  & (param) )
# else
#  define AVTP_CRF_FS_BIT_POS                    (0U)
/* Clock Reference Format (VideoLineSync): FS */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#  define AVTP_CRF_FS(param)                     (  0x01  &  (param) )
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8                             AvTp_DataType;
typedef uint8                             AvTp_StreamIdType[8];
typedef uint8                             AvTp_CtrlIdxType;
typedef uint8                             AvTp_PriorityType;
typedef uint32                            AvTp_TimestampType;
typedef uint8                             AvTp_StreamIdxType;
#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
typedef uint16                            AvTp_DiagnosticCounterIdxType;
typedef uint32                            AvTp_CounterType;
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

typedef struct
{
    uint32 nanoseconds;
    uint32 seconds;
    uint16 secondsHi;
} AvTp_GlobalTimestampType;

#if (STD_ON == AVTP_TIME_SUPPORT)
/* Used only for arithmetic */
typedef struct
{
  uint32 nanoseconds;
  uint64 seconds;
} AvTp_InternalTimestampType;

typedef sint64                            AvTp_TimediffType;
typedef uint64                            AvTp_UTimediffType;

typedef AvTp_TimediffType                 AvTp_PTOffsetType; /* Presentation Time Offset */

#define AVTP_TIMESTAMP_MAX_SECONDS        (0xFFFFFFFFFFFFLL)     /* 48 bit seconds */
#define AVTP_TIMESTAMP_MAX_NANOSECONDS    (1000000000UL)         /* 10^9 nanoseconds per second */
#define AVTP_TIMEDIFF_MAX_NANOSECONDS     (0x7FFFFFFFFFFFFFFFLL) /* 63 bit max timediff in nanoseconds */
#define AVTP_TIMEDIFF_MAX_SECONDS         (AVTP_TIMEDIFF_MAX_NANOSECONDS / AVTP_TIMESTAMP_MAX_NANOSECONDS)

#endif /* (STD_ON == AVTP_TIME_SUPPORT) */

/* AvTp Link State Types */
typedef EthTrcv_LinkStateType             AvTp_LinkStateType;
#define AVTP_LINK_STATE_ACTIVE            ETHTRCV_LINK_STATE_ACTIVE

/* AvTp State Types */
typedef uint8                             AvTp_StateType;
#define AVTP_STATE_UNINIT                 (0U)
#define AVTP_STATE_INIT                   (1U)
#define AVTP_STATE_LINK_ACTIVE            (2U)
#define AVTP_STATE_TX_READY               (3U)
#if (STD_ON == AVTP_MAAP_SUPPORT)
/* MAAP states of state machine */
#define AVTP_MAAP_STATE_UNINIT            (0U)
#define AVTP_MAAP_STATE_INIT              (1U)
#define AVTP_MAAP_STATE_INIT_LINK_AVTIVE  (2U)
#define AVTP_MAAP_STATE_INITIAL           (3U)
#define AVTP_MAAP_STATE_PROBE             (4U)
#define AVTP_MAAP_STATE_DEFEND            (5U)
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */


typedef uint8                             AvTp_SyncStateType;
#define AVTP_PORT_STATE_SYNC              (0x00U)
#define AVTP_PORT_STATE_UNSYNC            (0x01U)
#define AVTP_PORT_STATE_UNCERTAIN         (0x02U)
#define AVTP_PORT_STATE_NEVERSYNC         (0x03U)

#define AVTP_TIME_SOURCE_NONE             (0x00U)
#define AVTP_TIME_SOURCE_STBM             (0x01U)
#define AVTP_TIME_SOURCE_USER_CALLOUT     (0x02U)

#define AVTP_INV_TIME_BASE_ID             ((StbM_SynchronizedTimeBaseType)0xFFFFU) /* No Time Base Ref present */

/* AvTp Compare Types */                 
typedef sint8                             AvTp_CompareType;
#define AVTP_COMPARE_LESS                 (-1)
#define AVTP_COMPARE_EQUAL                (0)
#define AVTP_COMPARE_GREATER              (1)

/* AvTp Subtype values */
typedef uint8                             AvTp_SubtypeType;
#define AVTP_SUBTYPE_61883IIDC            (0x00U) /* IEC 61883/IIDC over AVTP */
#define AVTP_SUBTYPE_MMA                  (0x01U) /* MMA (MIDI) payload over AVTP  */
#define AVTP_SUBTYPE_CAMERA               (0x02U) /* BMW AVB Camera */
#define AVTP_SUBTYPE_AAF                  (0x02U) /* AVTP Audio Format */
#define AVTP_SUBTYPE_CVF                  (0x03U) /* Compressed Video Format */
#define AVTP_SUBTYPE_CRF                  (0x04U) /* Clock Reference Stream */
#define AVTP_SUBTYPE_MAAP                 (0xFEU) /* Multicast MAC Address Allocation Protocol */
#define AVTP_SUBTYPE_EXP                  (0xFFU) /* Experimental over AVTP */
#define AVTP_SUBTYPE_VSF                  (0x6FU) /* Vendor Specific Format */

/* State Change Function Pointer */
typedef AVTP_P2FUNC(void, AvTp_StateChangedFunctionFctPtrType) (AvTp_StateType);
/* User Callout Function Pointers */
typedef AVTP_P2FUNC(Std_ReturnType, AvTp_GetCurrentTimeFctPtrType) (AVTP_P2VAR(AvTp_GlobalTimestampType),
                                                                    AVTP_P2VAR(AvTp_SyncStateType) );

/* AvTp / AVDECC Common Header */
typedef struct
{
  uint8                                   Subtype;                        /* Subtype */
  uint8                                   HVersion;                       /* Header specific / version */
} AvTp_CommonHdrType;

/* AvTp Common Control Header */
typedef struct
{
  uint32                                  SubSvVerFsdCdl;                 /* Sub: Subtype, Sv: Stream_Id valid,
                                                                           * Ver: Version, Fsd: Format specific data,
                                                                           * Cdl: Control data length*/
  AvTp_StreamIdType                       StreamId;                       /* Stream ID */
} AvTp_ControlHdrType;

/* AvTp Common Stream Header */
typedef struct
{
  uint8                                   Subtype;                        /* Subtype */
  uint8                                   SvVerMrSsdTv;                   /* StreamValid,Version,MediaRestart,SSD,TV */
  uint8                                   SequenceNum;                    /* Sequence Number */
  uint8                                   SubtypeSpecificData1Tu;         /* SSD1 - Timestamp uncertain */
  AvTp_StreamIdType                       StreamId;                       /* Stream ID */
  AvTp_TimestampType                      Timestamp;                      /* Presentation Timestamp */
  uint32                                  SubtypeSpecificData2;           /* Subtype Specific Data 2 */
  uint16                                  StreamDataLen;                  /* Stream Data byte count */
  uint16                                  SubtypeSpecificData3;           /* Subtype Specific Data 3 */
} AvTp_StreamHdrType;

#if (STD_ON == AVTP_VSF_SUPPORT)
/* Vendor specific Header */
typedef struct
{
  uint8                                   Subtype;                        /* Subtype */
  uint8                                   SvVerMrRsvTv;                   /* StreamValid,Version,MediaRestart,Rsv,Tv */
  uint8                                   SequenceNum;                    /* Sequence Number */
  uint8                                   ReservedTu;                     /* Reserved, Timestamp uncertain */
  AvTp_StreamIdType                       StreamId;                       /* Stream ID */
  AvTp_TimestampType                      Timestamp;                      /* Presentation Timestamp */
  uint32                                  VendorEui1;                     /* Vendor EUI(-48) 1 (first 32 Bit) */
  uint16                                  StreamDataLen;                  /* Stream Data byte count */
  uint16                                  VendorEui2;                     /* Vendor EUI(-48) 2 (last 16 Bit) */
}AvTp_Vsf_HdrType;
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */

#if (STD_ON == AVTP_MAAP_SUPPORT)
/* MAAP Header */
typedef struct
{
  uint8                                   CdSubtype;                      /* Control/Data - Subtype */
  uint8                                   SvVerMsgType;                   /* Stream valid - Version - Message Type */
  uint16                                  MaapVerDataLen;                 /* Maap Version - Data Length */
  AvTp_StreamIdType                       StreamId;                       /* Stream ID */
  Eth_PhysAddrType                        ReqStartMac;                    /* Begin of requested MAC range */
  uint16                                  ReqCount;                       /* Count of requested MACs */
  Eth_PhysAddrType                        ConflictStartMac;               /* Start of conflicted MAC range */
  uint16                                  ConflictCount;                  /* Count of conflicted MACs */
} AvTp_Maap_MsgType;
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/* IEC 61883//IIDC format */
typedef struct
{
  uint32                                  GatewayInfo;                    /* Gateway Information */
  uint16                                  StreamDataLen;                  /* Stream byte count */
  uint8                                   TagChannel;                     /* Tag - Channel */
  uint8                                   TcodeSy;                        /* Tcode - SY */
} AvTp_61883IIDC_MsgType;
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* AVTP Audio format */
typedef struct
{
  uint8                                   Format;                         /* Type of samples contained */
  uint8                                   NsrChPerFrame_High;             /* Nominal Sample Rate, Channels per Frame */
  uint8                                   ChPerFrame_Low;                 /* Low Bytes of Channels per Frame */
  uint8                                   BitDepth;                       /* Number of actually used bits */
  uint16                                  StreamDataLen;                  /* Stream Data byte count */
  uint8                                   RSpEvt;                         /* Reserved - Spares Mode - Event */
  uint8                                   Reserved;
} AVTP_AAF_MsgType;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
typedef struct
{
  uint8                                   Format;                         /* Format */
  uint8                                   FormatSubtype;                  /* MJPEG / H264 / JPEG2000 */
  uint16                                  Reserved;                       /* Reserved */
  uint16                                  StreamDataLen;                  /* Stream Data byte count */
  uint8                                   PtvMEvt;                        /* PTV - M - Event */
  uint8                                   Reserved1;                      /* Reserved 1 */
#if ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON))
  uint32                                  H264Timestamp;                  /* h264_timestamp */
#endif /* ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON)) */
}AvTp_CVF_MsgType;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
typedef struct
{
  uint8                                   Subtype;
  uint8                                   SvVerMrRFsTu;
  uint8                                   SequenceNum;
  uint8                                   Type;
  AvTp_StreamIdType                       StreamId;
  uint32                                  PullBaseFreq;
  uint16                                  CrfDataLength;
  uint16                                  TimestampInterval;
} AvTp_Crf_HdrType;
# else
typedef struct
{
  uint16                                  StreamDataLen;                  /* Stream data length */
  uint16                                  Type;                           /* Timestamp type */
} AvTp_CRF_Pdu_MsgType;

typedef struct
{
  uint16                                  StreamDataLen;                  /* Stream data length */
  uint16                                  Type;                           /* Timestamp type */
  uint8                                   ClockFrequency;                 /* Clock Frequency */
  uint8                                   ClockMultiplier;                /* Clock Multiplier */
  uint16                                  TimestampInterval;              /* Timestamp Interval */
} AvTp_CRF_Audio_MsgType;


typedef struct
{
  uint16                                  StreamDataLen;                  /* Stream data length */
  uint16                                  Type;                           /* Timestamp type */
  uint32                                  ClockFrequency;                 /* Clock Frequency */
  uint8                                   ClockMultiplier;                /* Clock Multiplier */
  uint8                                   Reserved;                       /* Reserved */
  uint16                                  TimestampInterval;              /* Timestamp Interval */
} AvTp_CRF_VideoFrameSync_MsgType;

typedef struct
{
  uint16                                  StreamDataLen;                  /* Stream data length */
  uint16                                  Type;                           /* Timestamp type */
  uint32                                  ClockFrequency;                 /* Clock Frequency */
  uint8                                   ClockMultiplier;                /* Clock Multiplier */
  uint8                                   ResFs;                          /* Reserved - Frame sync bit */
  uint16                                  TimestampInterval;              /* Timestamp Interval */
} AvTp_CRF_VideoLineSync_MsgType;
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/********************************************************************
  RxPduInfoType
  -------------------------------------------------------------------
  Forwarded to upperlayer. Contains subtype-specific information
*********************************************************************/
/* Generic RxPduInfoType */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                       /* Media subtype */
} AvTp_RxPduInfoType; 

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/* RxPduInfoType -> IEC 61883_IIDC */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                       /* Media subtype */
  uint8                                   Sy;                            /* Sy */
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  uint8                                   SequenceNum;                   /* Sequence Num */
  boolean                                 MediaClockReset;               /* Media Clock reset */
  boolean                                 IsValidTimestamp;              /* Valid Timestamp indication */
  boolean                                 IsUncertainTimestamp;          /* Uncertain Timestamp indication */
  AvTp_TimestampType                      Timestamp;                     /* Timestamp value */
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */
} AvTp_61883IIDC_RxInfoType;
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* RxPduInfoType -> AVTP_Audio */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                       /* Media Subtype */
  uint8                                   Event;                         /* Event */
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  uint8                                   SequenceNum;                   /* Sequence Num */
  boolean                                 MediaClockReset;               /* Media Clock reset */
  boolean                                 IsValidTimestamp;              /* Valid Timestamp indication */
  boolean                                 IsUncertainTimestamp;          /* Uncertain Timestamp indication */
  AvTp_TimestampType                      Timestamp;                     /* Timestamp value */
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */
} AvTp_AAF_RxInfoType;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/* RxPduInfoType -> Compressed Video */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                       /* Media Subtype */
  uint8                                   M;                             /* Format_Subtype specific */
  uint8                                   Event;                         /* Event */
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  uint8                                   SequenceNum;                   /* Sequence Num */
  boolean                                 MediaClockReset;               /* Media Clock reset */
  boolean                                 IsValidTimestamp;              /* Valid Timestamp indication */
  boolean                                 IsUncertainTimestamp;          /* Uncertain Timestamp indication */
  AvTp_TimestampType                      Timestamp;                     /* Timestamp value */
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */
#if ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON))
  boolean                                 H264TimestampValid;           /* H264 Timestamp valid */
  uint32                                  H264Timestamp;                /* H264 Timestamp */
#endif /* ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON)) */
}AvTp_CVF_RxInfoType;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/* StreamType -> Clock Reference Format */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                       /* Clock Reference Subtype: 0x04 */
  boolean                                 Fs;                            /* Frame Sync */
#if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  boolean                                 Tu;                            /* Timing uncertain */
#endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  uint8                                   SequenceNum;                   /* Sequence Num */
  boolean                                 MediaClockReset;               /* Media Clock reset */
  boolean                                 IsValidTimestamp;              /* Valid Timestamp indication */
  boolean                                 IsUncertainTimestamp;          /* Uncertain Timestamp indication */
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */
} AvTp_CRF_RxInfoType;
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* Vendor Specific Subtype */
  uint8                                   Padding;                        /* 16Bit Aligned */
  uint16                                  VendorEui2;                     /* Vendor EUI 2 (last 16 Bit) */
  uint32                                  VendorEui1;                     /* Vendor EUI 1 (first 32 Bit) */
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  uint8                                   SequenceNum;                    /* Sequence Number */
  boolean                                 MediaClockReset;                /* Media Clock reset */
  boolean                                 IsValidTimestamp;               /* Valid Timestamp indication */
  boolean                                 IsUncertainTimestamp;           /* Uncertain Timestamp indication */
  AvTp_TimestampType                      Timestamp;                      /* Timestamp Value */
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */
}AvTp_VSF_RxInfoType;
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */

/*********************************************************************
  StreamType
  --------------------------------------------------------------------
  Contains configured, subtype specific data
**********************************************************************/
/* Generic StreamType */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* Media Subtype Indicator  */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
} AvTp_StreamCfgType;

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* IEC 61883 Subtype: 0x00 */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
  uint8                                   Tag;                            /* Tag */
  uint8                                   Channel;                        /* Channel */
}AvTp_61883IIDC_CfgType;
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* StreamType -> AVTP Audio (ROM) */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* AVTP Audio Subtype: 0x02 */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
  uint8                                   Format;                         /* Type of samples contained */
  uint8                                   Nsr;                            /* Nominal samples rate */
  uint16                                  ChanPerFram;                    /* Channels per Frame */
  uint8                                   BitDepth;                       /* Number of acutally used bits */
  boolean                                 SparesMode;                     /* Timestampe spares mode */
  uint16                                  SampleFramesPerPacket;          /* Number of Samples in one Frame */
  uint16                                  FrameSize_Byte;                 /* Frame size in byte */
} AvTp_AAF_CfgType;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/* StreamType -> Compressed Video Format */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* Compressed Video Subtype: 0x03 */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
  uint8                                   Format;                         /* Usually RFC Payload = 0x02 */
  uint8                                   FormatSubtype;                  /* MJPEG / H264 / JPEG2000 */
} AvTp_CVF_CfgType;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/* StreamType -> Clock Reference Format */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* Clock Reference Subtype: 0x04 */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
  uint8                                   Type;                           /* Type of CRF Stream */
  uint32                                  ClockFrequency;                 /* Clock Frequency (Audio only uint8) */
  uint8                                   ClockMultiplier;                /* Clock Multiplier */
  uint16                                  TimestampInterval;              /* Timestamp Interval */
} AvTp_CRF_CfgType;
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
/* StreamType -> Vendor Specific Format */
typedef struct
{
  AvTp_SubtypeType                        Subtype;                        /* Vendor Specific Format Subtype 0x6F  */
  uint8                                   AvTpHdrLen;                     /* Length of AvTp Header */
  uint32                                  VendorEui1;                     /* Vendor EUI-48 1 (first 32 Bit) */
  uint16                                  VendorEui2;                     /* Vendor EUI-48 2 (last 16 Bit) */
}AvTp_VSF_CfgType;
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */


/*********************************************************************
  Rx and Tx Info structs
  --------------------------------------------------------------------
  Contains configured, subtype specific data
**********************************************************************/
/* RxStreamInfo: Exists for each RxStream (ROM) to perform checks of received msgs */
typedef struct
{
  AvTp_CtrlIdxType                        AvTpCtrlIdx;                    /* AvTp Controller Index */
  AvTp_StreamIdType                       StreamId;                       /* StreamId */
  AVTP_P2CONSTCFG(AvTp_StreamCfgType)     StreamCfgPtr;                   /* Subtype of payload */
} AvTp_RxStreamInfoType;

/* TxStreamInfo: Exists for each TxStream (ROM) */
typedef struct
{
  AvTp_CtrlIdxType                        AvTpCtrlIdx;                    /* AvTp Controller Index */
  AvTp_PriorityType                       FramePriority;                  /* VLan Priority */
#if (STD_OFF == AVTP_MAAP_SUPPORT)
  Eth_PhysAddrType                        DefaultMulticastMacAddr;        /* Default Multicast Mac Address */
#endif /* (STD_OFF == AVTP_MAAP_SUPPORT) */
  AvTp_StateChangedFunctionFctPtrType     StateChgFunc;                   /* State Change Function Ptr */
  AvTp_StreamIdType                       StreamId;                       /* StreamId */
#if (STD_ON == AVTP_TIME_SUPPORT)
  StbM_SynchronizedTimeBaseType           TimeBaseId;                     /* Synchronized Time Base Id */
  uint8                                   TimeSource;
  AvTp_GetCurrentTimeFctPtrType           UserCalloutFct;
  AvTp_PTOffsetType                       PresentationTimeOffset;         /* Presentation Time Offset */
#endif /* (STD_ON == AVTP_TIME_SUPPORT) */
  AVTP_P2CONSTCFG(AvTp_StreamCfgType)     StreamCfgPtr;                   /* Subtype of payload */
} AvTp_TxStreamInfoType;

/* Used for Tx Buffer management (RAM) */
typedef struct
{
  uint8                                   BufIdx;                          /* Index of Tx Stream buffer */
  AVTP_P2VAR(AvTp_DataType)               BufPtr;                          /* Pointer to Tx Stream buffer */
  uint16                                  LenByte;                         /* Length of Tx Buffer */
  uint8                                   SequenceNum;                     /* Sequence Num of frame */
  boolean                                 Mr;                              /* Media Clock Restart */
#if (STD_ON == AVTP_MAAP_SUPPORT)
  Eth_PhysAddrType                        MulticastMacAddr;                /* Multicast Mac Address */
#endif /* (STD_OFF == AVTP_MAAP_SUPPORT) */
  uint8                                   Event;                           /* Stream Event */
#if (STD_ON == AVTP_61883IIDC_SUPPORT)
  uint8                                   Sy;                              /* Sy Field */
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */
#if (STD_ON == AVTP_AAF_SUPPORT)
  uint16                                  SampleCount;                     /* Number of Samples already written */
  boolean                                 Busy;                            /* Busy flag */
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */
#if (STD_ON == AVTP_CVF_SUPPORT)
  boolean                                 M;                               /* Marker bit */
# if ((AVTP_CVF_H264_SUPPORT == STD_ON) && (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE))
  boolean                                H264TimestampValid;               /* H264 Timestamp valid */
  uint32                                 H264Timestamp;                    /* H264 Timestamp */
# endif /* ((AVTP_CVF_H264_SUPPORT == STD_ON)&&(AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)) */
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */
#if (STD_ON == AVTP_CRF_SUPPORT)
  boolean                                 FS;                              /* Frame Sync bit */
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  boolean                                 Tu;                              /* Timing uncertain bit */
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */
#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  boolean                                 MediaClockResetLast;             /* Mr value of last transmitted frame */
  boolean                                 IsUncertainTimestampLast;        /* Tu value of last transmitted frame */
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
} AvTp_TxStreamMgmtType;

/* Used for Rx Stream Management (RAM) */
typedef struct
{
  uint8                                   SequenceNum;                   /* Sequence Num */
  boolean                                 MediaClockReset;               /* Media Clock reset */
  boolean                                 IsUncertainTimestamp;          /* Uncertain Timestamp indication */
} AvTp_RxStreamMgmtType;

/* Mapping from StreamId -> StreamIdx (ROM) */
typedef struct
{
  AvTp_StreamIdType                       StreamId;                       /* StreamId */
  uint8                                   StreamIdx;                      /* StreamIdx */
} AvTp_RxStreamIdMapType;


#if (STD_ON == AVTP_MAAP_SUPPORT)
/* MAAP Management struct (RAM) */
typedef struct
{
  AvTp_StateType                          Maap_State;                     /* MAAP-StateMachine state */
  uint8                                   Maap_ProbeCount;                /* Number of Probes to be transmitted */
  uint16                                  Maap_ProbeInterval;             /* Interval between 2 consecutive Probes */
  uint16                                  Maap_AnnounceInterval;          /* Interval between 2 consecutive Announce */
  uint16                                  Maap_ConflictCount;             /* Number of Conflicting addresses */
}AvTp_MaapMgmtType;
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

/* Controller Management Struct (ROM) */
typedef struct
{
  AvTp_CtrlIdxType                        VCtrlIdx;                 /* Virtual Controller Index */
  uint16                                  Mtu;                      /* MTU */
  uint8                                   TxStreamCnt;              /* Number of Tx Streams configured for AvTpCtrl */
  AVTP_P2CONST(AvTp_StreamIdxType)        TxStreamPtr;              /* Pointer to TxStreamIdx array */
#if (STD_ON == AVTP_MAAP_SUPPORT)
  AVTP_P2VAR(AvTp_MaapMgmtType)           Maap_MgmtPtr;             /* Maap Management */
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */
  uint8                                   RxStreamCnt;              /* Number of Rx Streams configured for AvTpCtrl */
  AVTP_P2CONST(AvTp_StreamIdxType)        RxStreamPtr;              /* Pointer to RxStreamIdx array */
}AvTp_ControllerType;

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
/* Diagnostic Counter struct (RAM) */
typedef struct
{
  AvTp_CounterType                        SeqNumMismatch;                 /* Incr on non-sequential sequence_num */
  AvTp_CounterType                        MediaReset;                     /* Incr on toggle of mr bit */
  AvTp_CounterType                        TimestampUncertain;             /* Incr on toggle of tu bit */
  AvTp_CounterType                        TimestampValid;                 /* Incr on tv-bit set */
  AvTp_CounterType                        TimestampNotValid;              /* Incr on tv bit clearded */
  AvTp_CounterType                        UnsupportedFormat;              /* Incr on unsupported media type */
  AvTp_CounterType                        FramesRx;                       /* Incr on sent/received frame */
  AvTp_CounterType                        LateTimestamp;                  /* Incr if TS is in the past */
  boolean                                 FirstFrame;                     /* Indicates reception of intial frame */
} AvTp_DiagnosticCounterListenerType;

typedef struct
{
  AvTp_CounterType                        MediaReset;                     /* Incr on toggle of mr bit */
  AvTp_CounterType                        TimestampUncertain;             /* Incr on toggle of tu bit */
  AvTp_CounterType                        TimestampValid;                 /* Incr on tv-bit set */
  AvTp_CounterType                        TimestampNotValid;              /* Incr on tv bit clearded */
  AvTp_CounterType                        FramesTx;                       /* Incr on sent/received frame */
} AvTp_DiagnosticCounterTalkerType;

#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

#endif  /* AVTP_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: AvTp_Types.h
 *********************************************************************************************************************/
