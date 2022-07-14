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
 *         File:  AvTp_Priv.h
 *       Module:  Tp_Ieee1722AvTp
 *    Generator:  Tp_Ieee1722AvTp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Private header of the Audio Video Bridging (AVB) Transport Protocol
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (AVTP_PRIV_H)
#define AVTP_PRIV_H




/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "AvTp.h"
#if (AVTP_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif  /* AVTP_DEV_ERROR_DETECT */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define AVTP_MAAP_PROBE_INTERVAL                 (AVTP_MAAP_PROBE_INTERVAL_BASE / AVTP_MAIN_FUNCTION_CYCLE)
#define AVTP_MAAP_ANNOUNCE_INTERVAL              (AVTP_MAAP_ANNOUNCE_INTERVAL_BASE / AVTP_MAIN_FUNCTION_CYCLE)

#if defined ( AVTP_CANOE_DEBUG )
  #define AVTP_VENABLE_CANOE_WRITE_STRING                      STD_ON
#else                                                          
  #define AVTP_VENABLE_CANOE_WRITE_STRING                      STD_OFF
#endif /* AVTP_CANOE_DEBUG */

/* Timestamp */
#define AVTP_NANOSECONDS_PER_SECOND   (1000000000UL)

/* Time Base Status Masks */
#define AVTP_TIME_BASE_STATUS_TIMEOUT_MASK               (0x01U)
#define AVTP_TIME_BASE_STATUS_SYNC_TO_GW_MASK            (0x04U)
#define AVTP_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK      (0x08U)
#define AVTP_TIME_BASE_STATUS_TIMELEAP_FUTURE_MASK       (0x10U)
#define AVTP_TIME_BASE_STATUS_TIMELEAP_PAST_MASK         (0x20U)
#define AVTP_TIME_BASE_STATUS_TIMELEAP_MASK              (0x02U)


/* ----- Diagnostic Counter output in CANoe ----- */
/* Listener */
#define AVTP_DIAG_WRITE_LISTENER_SEQNUM_MISMATCH               STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_MEDIA_RESET                   STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_UNCERTAIN           STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_VALID               STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_NOT_VALID           STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_UNSUPPORTED_FORMAT            STD_OFF
#define AVTP_DIAG_WRITE_LISTENER_FRAMES_RX                     STD_OFF
/* Talker */
#define AVTP_DIAG_WRITE_TALKER_FRAMES_TX                       STD_OFF
#define AVTP_DIAG_WRITE_TALKER_MEDIA_RESET                     STD_OFF
#define AVTP_DIAG_WRITE_TALKER_TIMESTAMP_UNCERTAIN             STD_OFF
#define AVTP_DIAG_WRITE_TALKER_TIMESTAMP_VALID                 STD_OFF
#define AVTP_DIAG_WRITE_TALKER_TIMESTAMP_NOT_VALID             STD_OFF


/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
#define AvTp_Htons(HostShort16)                             IPBASE_HTON16((uint16)(HostShort16))
#define AvTp_Htonl(HostLong32)                              IPBASE_HTON32((uint32)(HostLong32))
#define AvTp_Ntohs(NetworkShort16)                          IPBASE_HTON16((uint16)(NetworkShort16))
#define AvTp_Ntohl(NetworkLong32)                           IPBASE_HTON32((uint32)(NetworkLong32))

/* ----- Configuration Access Macros ----- */
/* PRQA S 3453 FCT_MACRO */ /* MD_MSR_19.7 */
#define AvTp_GetMaapBaseAddress(param)                      AvTp_TxStreamMgmt[AvTp_Ctrl[(param)].TxStreamPtr[0]]. \
                                                              MulticastMacAddr

#define AvTp_CfgGetAvTpCtrlIdx(StreamIdx)                   AvTp_TxStreamInfo[(StreamIdx)].AvTpCtrlIdx
#define AvTp_CfgGetVCtrlIdx(StreamIdx)                      AvTp_Ctrl[AvTp_TxStreamInfo[(StreamIdx)].AvTpCtrlIdx]. \
                                                              VCtrlIdx
#define AvTp_CfgGetMTU(StreamIdx)                           AvTp_Ctrl[AvTp_TxStreamInfo[(StreamIdx)].AvTpCtrlIdx].Mtu
#define AvTp_CfgGetRxStreamCfgPtr(StreamIdx)                AvTp_RxStreamInfo[(StreamIdx)].StreamCfgPtr
#define AvTp_CfgGetTxStreamCfgPtr(StreamIdx)                AvTp_TxStreamInfo[(StreamIdx)].StreamCfgPtr
#define AvTp_CfgGetTxSubtype(StreamIdx)                     AvTp_CfgGetTxStreamCfgPtr((StreamIdx))->Subtype

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/* IEC Format access macros */
#define AvTp_CfgGetTxIecTag(StreamIdx)                      ((AVTP_P2CONSTCFG(AvTp_61883IIDC_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Tag
#define AvTp_CfgGetTxIecChannel(StreamIdx)                  ((AVTP_P2CONSTCFG(AvTp_61883IIDC_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Channel

#define AvTp_CfgGetRxIecTag(StreamIdx)                      ((AVTP_P2CONSTCFG(AvTp_61883IIDC_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->Tag
#define AvTp_CfgGetRxIecChannel(StreamIdx)                  ((AVTP_P2CONSTCFG(AvTp_61883IIDC_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->Channel
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/* AVTP Audio Format access macros */
#define AvTp_CfgGetTxAvtpFormat(StreamIdx)                  ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Format
#define AvTp_CfgGetTxAvtpNsr(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Nsr
#define AvTp_CfgGetTxAvtpChanPerFram(StreamIdx)             ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->ChanPerFram
#define AvTp_CfgGetTxAvtpBitDepth(StreamIdx)                ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->BitDepth
#define AvTp_CfgGetTxAvtpSparseMode(StreamIdx)              ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->SparesMode

#define AvTp_CfgGetRxAvtpFormat(StreamIdx)                  ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->Format
#define AvTp_CfgGetRxAvtpNsr(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->Nsr
#define AvTp_CfgGetRxAvtpChanPerFram(StreamIdx)             ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->ChanPerFram
#define AvTp_CfgGetRxAvtpBitDepth(StreamIdx)                ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->BitDepth
#define AvTp_CfgGetRxAvtpSparseMode(StreamIdx)              ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->SparesMode
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/* Compressed Video Format access macros */
#define AvTp_CfgGetTxCmpVideoFormat(StreamIdx)              ((AVTP_P2CONSTCFG(AvTp_CVF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Format
#define AvTp_CfgGetTxCmpVideoFormatSubtype(StreamIdx)       ((AVTP_P2CONSTCFG(AvTp_CVF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              FormatSubtype

#define AvTp_CfgGetRxCmpVideoFormat(StreamIdx)              ((AVTP_P2CONSTCFG(AvTp_CVF_CfgType)) \
                                                              AvTp_RxStreamInfo[StreamIdx].StreamCfgPtr)->Format
#define AvTp_CfgGetRxCmpVideoFormatSubtype(StreamIdx)       ((AVTP_P2CONSTCFG(AvTp_CVF_CfgType)) \
                                                              AvTp_RxStreamInfo[StreamIdx].StreamCfgPtr)->\
                                                              FormatSubtype
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/* Clock Reference Format access macros */
#define AvTp_CfgGetTxCrfType(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->Type
#define AvTp_CfgGetTxCrfClockFrequency(StreamIdx)           ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              ClockFrequency
#define AvTp_CfgGetTxCrfClockMultiplier(StreamIdx)          ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              ClockMultiplier
#define AvTp_CfgGetTxCrfClockTimestampInterval(StreamIdx)   ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              TimestampInterval

#define AvTp_CfgGetRxCrfType(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->Type
#define AvTp_CfgGetRxCrfClockFrequency(StreamIdx)           ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->\
                                                              ClockFrequency
#define AvTp_CfgGetRxCrfClockMultiplier(StreamIdx)          ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->\
                                                              ClockMultiplier
#define AvTp_CfgGetRxCrfClockTimestampInterval(StreamIdx)   ((AVTP_P2CONSTCFG(AvTp_CRF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->\
                                                              TimestampInterval
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
/* EUI Macros */
#define AvTp_CfgGetTxVsfEui1(StreamIdx)((AVTP_P2CONSTCFG(AvTp_VSF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              VendorEui1
#define AvTp_CfgGetTxVsfEui2(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_VSF_CfgType)) \
                                                              AvTp_CfgGetTxStreamCfgPtr((StreamIdx)))->\
                                                              VendorEui2

#define AvTp_CfgGetRxVsfEui1(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_VSF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->\
                                                              VendorEui1
#define AvTp_CfgGetRxVsfEui2(StreamIdx)                     ((AVTP_P2CONSTCFG(AvTp_VSF_CfgType)) \
                                                              AvTp_CfgGetRxStreamCfgPtr((StreamIdx)))->\
                                                              VendorEui2
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */
/* PRQA L: FCT_MACRO */

#define AVTP_ENTER_CRITICAL_SECTION_0()                     SchM_Enter_AvTp_AVTP_EXCLUSIVE_AREA_0()
#define AVTP_LEAVE_CRITICAL_SECTION_0()                     SchM_Exit_AvTp_AVTP_EXCLUSIVE_AREA_0()

/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
#define AVTP_SET_BIT(Field, Pos)                            ((Field) |= (uint8)((uint8)0x01u << (uint8)(Pos)))
#define AVTP_RESET_BIT(Field, Pos)                          ((Field) = \
  (uint8)((uint32)(Field) & (uint32)(~(uint32)((uint32)0x01u << (uint32)(Pos)))))

/**********************************************************************************************************************
*  CANOE DEBUG SUPPORT 
**********************************************************************************************************************/
#if (AVTP_VENABLE_CANOE_WRITE_STRING == STD_ON)
#include "stdio.h"


#define CANOE_DBG_MODULE_NAME "AvTp"

#define CANOE_WRITE_STRING(Txt) \
  CANoeAPI_WriteString("[" CANOE_DBG_MODULE_NAME "] " Txt);

/* PRQA S 1030,5013 3 */ /* MD_AvTp_CANoeEmu */
#define CANOE_WRITE_STRINGF(Txt, ...) \
  _snprintf(VCanoeWriteStrBuffer, sizeof(VCanoeWriteStrBuffer), "[" CANOE_DBG_MODULE_NAME "] " Txt, __VA_ARGS__); \
  CANoeAPI_WriteString(VCanoeWriteStrBuffer);

  #define CANOE_WRITE_STRING1(txt, p1)                     CANOE_WRITE_STRINGF(txt, p1)
  #define CANOE_WRITE_STRING2(txt, p1, p2)                 CANOE_WRITE_STRINGF(txt, p1, p2)
  #define CANOE_WRITE_STRING3(txt, p1, p2, p3)             CANOE_WRITE_STRINGF(txt, p1, p2, p3)
  #define CANOE_WRITE_STRING4(txt, p1, p2, p3, p4)         CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4)
  #define CANOE_WRITE_STRING5(txt, p1, p2, p3, p4, p5)     CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4, p5)
  #define CANOE_WRITE_STRING6(txt, p1, p2, p3, p4, p5, p6) CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4, p5, p6)

  STATIC char VCanoeWriteStrBuffer[256]; /* PRQA S 5013 */ /* MD_AvTp_CANoeEmu */

#else
  #define CANOE_WRITE_STRING(txt)
  #define CANOE_WRITE_STRING1(txt, p1)
  #define CANOE_WRITE_STRING2(txt, p1, p2)
  #define CANOE_WRITE_STRING3(txt, p1, p2, p3)
  #define CANOE_WRITE_STRING4(txt, p1, p2, p3, p4)
  #define CANOE_WRITE_STRING5(txt, p1, p2, p3, p4, p5)  
  #define CANOE_WRITE_STRING6(txt, p1, p2, p3, p4, p5, p6)
#endif /* (AVTP_VENABLE_CANOE_WRITE_STRING == STD_ON)) */


#endif  /* AVTP_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: AvTp_Priv.h
 *********************************************************************************************************************/
