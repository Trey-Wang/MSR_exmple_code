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
 *         File:  AvTp.h
 *      Project:  Audio Video Bridging Transport Protocol
 *       Module:  Tp_Ieee1722AvTp
 *    Generator:  Tp_Ieee1722AvTp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Header File of the Audio Video Bridging (AVB) Transport Protocol
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Dennis Mueller                visdmi        Vector Informatik GmbH
 *  Jeroen Laverman               vislje        Vector Informatik GmbH
 *  Michael Seidenspinner         vissem        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-10-14  visdmi  -             created
 *            2014-03-25  vislje  -             Added AVTP Audio format support
 *            2014-04-03  vislje  -             Added Multi-Controller support
 *                                              AvTp_Maap_TxDefend: set destination address to source address of con-
 *                                              flicting Maap_Probe.
 *                                              Add EthIf_UpdatePhysAddrFilter call to enable reception of Vlan frames
 *            2014-04-04  vislje  -             Change AvTp_SearchStreamIdx() and AvTp_SearchAvTpCtrlIdx() to
 *                                              LOCAL_INLINE functions and remove declaration from AvTp_priv.h
 *            2014-04-08  vislje                Fix DET-Issue in AvTp_Cbk_TrcvLinkStateChg() when MAAP = STD_OFF,
 *                                              Add AvTp_Client_MainFunction() and corresponding Filter,
 *                                              Add switch to select BINARY or LINEAR search,
 *                                              Modify AvTp_SearchStreamIdx() and AvTp_SearchAvTpCtrlIdx() to
 *                                              LOCAL INLINE,
 *                                              Modify AvTp_RxIndication() to switch by subtype,
 *                                              Add new subtype Compressed Video
 *                                              Add AvTp_SetMarker() to set Marker bit in compressed Video format
 *            2014-04-11  vislje  -             Fix invalid LenByte truncation (2Byte) in AvTp_Transmit() and Subtype
 *                                              equals AVTP Audio
 *            2014-04-14  vislje  -             Modify files to fit Msn.c and Msn.h template. Include 'SchM_AvTp.h' for
 *                                              critical sections
 *            2014-04-16  vislje  -             Add new subtype Clock Reference. Add AvTp_AVTPAudio_WriteSamples
 *            2014-04-28  vislje  -             Fix wrong return type from DET (E_NOT_OK) in AvTp_Maap_Check_Conflict()
 *                                              Add transmit functionality for Clock Reference and Compressed Video.
 *                                              Add DET condition macros. Change AvTp-Header fields to fit 1722-D8.
 *            2014-04-30  vislje  -             Gather type/#defines used as enums to improve readability
 *            2014-05-07  vislje  -             Moved local function prototypes from AvTp_priv.h to *.c-file.
 *                                              Add SchM_Enter/Exit define and fix missing semicolon
 *            2014-05-09  vislje  -             Add fragmentation protection to AvTp_ProvideTxBuffer(). Add MR-Bit to
 *                                              AvTp_RxPduInfoType. Add configuration access macros to improve read-
 *                                              ability. Add sanity check to AvTp_ProcessStreamPdu() to discover false
 *                                              streamdataLens
 *            2014-05-12  vislje  -             Replace AvTp_SearchAvTpCtrlIdx() by generated ROM-array AvTp_VCtrlMap[]
 *                                              Fix missing timestamp relay to upper layer in AvTp_ProcessStreamPdu()
 *                                              Fix length check in AvTp_ProcessStreamPdu()
 *            2014-05-13  vislje  -             Encapsulate references to EthStbM and GPtp to make it more generic
 *            2014-05-15  vislje  -             Add missing message checks for subtypes to AvTp_ProcessStreamPdu()
 *                                              Add AvTp_SetMr() to allow upper layer to set/reset Mr-bit
 *                                              Add Tv and Tu bit to AvTp_RxClockReferenceInfoType
 *                                              Move Subtypes to LOCAL_INLINE ProcessSUBTYPE()
 *            2014-05-16  vislje  -             Fix CANoe_Write-Api
 *                                              Add further CANoe write output
 *                                              Add AvTp_RxXPduInfoType to allocate memory for largest struct
 *            2014-05-19  vislje  -             Add AvTp_GetRxStreamConfig, AvTp_SetFs and AvTp_SetSy.
 *                                              Change struct and variable name from AvTp_StreamType to
 *                                              AvTp_StreamCfgType
 *                                              Replace AVTP_P2VAR by AVTP_P2CONSTCFG where applicable
 *            2014-05-20  vislje  -             Renaming to ensure consitent naming and abbreviations
 *            2014-05-26  vislje  -             Add StreamIdx to AvTp_SUBTYPE_Process() API
 *            2014-05-27  vislje  -             Move AvTpHdrLen from TxStreamInfoType to StreamCfgType
 *                                              Fix missing #ifdef encapsulation
 *                                              Add AvTp_GetTxStreamConfig
 *                                              Add Mr-Bit evaluation in AvTp_Transmit()
 *                                              Fix case condition in AvTp_SetEvent()
 *                                              Eliminate redundant if-statements
 *                                              Remove header version checks
 *            2014-05-28  vislje  -             Rename AvTp_AVTPAudio_WriteSample() to AvTp_Aaf_WriteSample()
 *                                              Add missing stream data length processing in CVF and CRF
 *            2014-06-04  vislje  -             Fix missing byte-order change at CRF-Timestampinterval
 *            2014-06-04  vislje  -             Delete redundant checks in static functions.
 *                                              Fix NULL_PTR access if Rx-Only- and Tx-Controller exist alongside
 *                                              Fix invalid CfgAccess-Macro (MTU)
 *                                              Fix AvTp_Maap_GenerateDynamicAddr() calculation
 *            2014-06-23  vislje  -             Add STATIC define
 *            2014-06-24  vislje  -             Add feature: Diagnostic Counters
 *            2014-07-04  vislje  -             Move BroadcastMacAddress to function TrcvLinkChgCbk
 *                                              Add MISRA findings / justifications
 *            2014-07-09  vislje  -             Changed call-by-value to call-by-reference in AvTp_SearchStreamIdx()
 *                                              Add missing Mr+Tu bit to AvTp_TxStreamMgmtType for Diagnostic Ctr
 *                                              Add DET in AvTp_ProcessStreamPdu() for unknown subtype
 *                                              Add QAC and PCLint findings
 *  01.00.01  2014-09-16  vislje  ESCAN00078402 AvTp tries to transmit frame although link-state is down
 *  01.00.02  2014-12-05  vissem  ESCAN00080004 DET macro contains too many arguments
 *            2015-01-08  vislje  ESCAN00080445 Compiler error: Syntax Error: missing ';' before '}' when DET is
 *                                              disabled
 *  01.01.00  2015-03-04  vislje  ESCAN00080450 Support EthTSynGlobalTimeDomains as source for time stamping
 *                                              information
 *            2015-04-10  vislje  ESCAN00082366 Support UserCallout, EthTSyn and GPtp as source for time stamping
 *                                              parallel
 *  01.01.01  2015-06-18  vislje  ESCAN00083518 Compiler error: C2078: too many initializers
 *            2015-07-17  vislje  ESCAN00084053 Compiler error: C2081: 'AvTp_GlobalTimestampType' : name in formal
 *                                              parameter list illegal
 *  01.02.00  2015-07-20  vislje  ESCAN00083860 Adopt Type of DataPtr in AvTp_RxIndication
 *  01.02.01  2015-08-07  vislje  ESCAN00084407 Compiler error: C4013: 'AvTp_RxIndication' and
 *                                              'AvTp_Cbk_TrcvLinkStateChg' undefined; assuming extern returning int
 *  01.03.00  2015-08-10  vissem  ESCAN00084356 Added support of the 'Vendor Specific' format
 *  02.00.00  2015-12-09  vissem  -             FEAT-1372: Support Java 8 and R14 CFG5 Breaking Changes
 *  03.00.00  2017-01-17  vissem  FEATC-270     FEAT-2068:  Release of AVTP
 *  04.00.00  2017-01-24  vissem  FEATC-1127    FEAT-2111:  Update to latest IEEE 1722a version (Tp_Ieee1722AvTp)
 *                                ESCAN00093679 Compiler error: Syntax error due to missing parenthesis
 *                                              (AvTp_GetVersionInfo)
 *                                ESCAN00085662 AvTp issues a DET error if IEEE1722-1 messages are received
 *  05.00.00  2017-03-10  vissem  ESCAN00094312 Updated to R18
 *********************************************************************************************************************/

#if !defined(AVTP_H)
#define AVTP_H

#if defined(C_COMMENT_VECTOR)
/*
    ToDo:
    - check for existing ToDo comments
*/
#endif

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "AvTp_Cbk.h"
#include "AvTp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   Tp_Ieee1722AvTp CQComponent : Implementation */

/* Version - BCD coded version number - main- and sub-version - release-version */
#define TP_IEEE1722AVTP_VERSION         (0x0500u) /* BCD coded version number */
#define TP_IEEE1722AVTP_RELEASE_VERSION (0x00u)   /* BCD coded release version number */

/* Vendor and module identification */
#define AVTP_VENDOR_ID                               30u
#define AVTP_MODULE_ID                               255u
#define AVTP_INSTANCE_ID                             115u

/* AUTOSAR Software Specification Version Information */
#define AVTP_AR_MAJOR_VERSION                        (0x04)
#define AVTP_AR_MINOR_VERSION                        (0x02)
#define AVTP_AR_PATCH_VERSION                        (0x01)

/* ----- Component version information ----- */
#define AVTP_SW_MAJOR_VERSION                         (TP_IEEE1722AVTP_VERSION >> 8u)
#define AVTP_SW_MINOR_VERSION                         (TP_IEEE1722AVTP_VERSION & 0x00FF)
#define AVTP_SW_PATCH_VERSION                         (TP_IEEE1722AVTP_RELEASE_VERSION)

/* ----- API service IDs ----- */
#define AVTP_API_ID_GET_VERSION_INFO                 (0x01u)

#define AVTP_API_ID_PROCESS_STREAM_PDU               (0x10u)
#define AVTP_API_ID_PROCESS_CONTROL_PDU              (0x11u)
#define AVTP_API_ID_CBK_TRCV_LINK_STATE_CHG          (0x12u)
#define AVTP_API_ID_PROVIDE_TX_BUFFER                (0x13u)
#define AVTP_API_ID_TRANSMIT                         (0x14u)
#define AVTP_API_ID_RX_INDICATION                    (0x15u)
#define AVTP_API_ID_GET_RX_STREAM_CONFIG             (0x16u)
#define AVTP_API_ID_GET_TX_STREAM_CONFIG             (0x17u)
#define AVTP_API_ID_SETEVENT                         (0x18u)
#define AVTP_API_ID_SETMARKER                        (0x19u)
#define AVTP_API_ID_SETMR                            (0x1Au)
#define AVTP_API_ID_SETSY                            (0x1Bu)
#define AVTP_API_ID_SETFS                            (0x1Cu)
#define AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_LISTENER (0x1Du)
#define AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_TALKER   (0x1Eu)
#define AVTP_API_ID_RESET_DIAGNOSTIC_COUNTER         (0x1Fu)

#define AVTP_MAAP_API_ID_RX_INDICATION               (0x20u)
#define AVTP_MAAP_API_ID_COMPARE_MAC                 (0x21u)
#define AVTP_MAAP_API_ID_CHECK_CONFLICT              (0x22u)
#define AVTP_MAAP_API_ID_CALCULATE_CONFLICT          (0x23u)
#define AVTP_MAAP_API_ID_TX_DEFEND                   (0x24u)

#define AVTP_API_ID_AAF_WRITESAMPLES                 (0x30u)

#define AVTP_API_ID_GET_CURRENT_TIME                 (0x40u)
#define AVTP_API_ID_SET_TU                           (0x41u)
#define AVTP_API_ID_SET_H264_TIMESTAMP               (0x42u)

/* ----- Error codes ----- */
/*!< used to check if no error occurred - use a value unequal to any error code */
#define AVTP_E_NO_ERROR                              (0x00u)
#define AVTP_E_NOT_INITIALIZED                       (0x01u)  /* AVTP has not been initialized */
#define AVTP_E_INV_POINTER                           (0x02u)  /* Invalid pointer in parameter list */
#define AVTP_E_INV_LENGTH                            (0x03u)  /* Invalid length in parameter list */
#define AVTP_E_INV_SUBTYPE                           (0x04u)  /* Invalid subtype in AvTp Pdu */
#define AVTP_E_NOT_TX_READY                          (0x05u)  /* AVTP is not ready for transmit */
#define AVTP_E_INV_STREAM_IDX                        (0x06u)  /* Invalid Stream Index */
#define AVTP_E_INV_VCTRL_IDX                         (0x07u)  /* Invalid Virtual Controller Index */
#define AVTP_E_INV_CTRL_IDX                          (0x08u)  /* Invalid AVTP Controller Index */
#define AVTP_E_MULTIPLE_CALLS                        (0x09u)  /* Function can only be can't be called multiple times */
#define AVTP_E_INV_FRAME_TYPE                        (0x0Au)  /* Received wrong frame type */
#define AVTP_E_INV_TIME_SOURCE                       (0x0Bu)  /* Unsupported Time source is requested */

#define AVTP_MAAP_E_NOT_INITIALIZED                  (0x20u)  /* AVTP has not been initialized */
#define AVTP_MAAP_E_INV_POINTER                      (0x21u)  /* Invalid pointer in parameter list */
#define AVTP_MAAP_E_INV_LENGTH                       (0x22u)  /* Invalid length in parameter list */

#define AVTP_AAF_E_INV_FORMAT                        (0x30u)  /* Not supported AVTP Audio format */

#define AVTP_CRF_E_INV_TYPE                          (0x40u)  /* Unexpected Clock Reference Type */

/* ----- Mapping of DET condition macros ----- */
/* PRQA S 3453 26 */ /* MD_MSR_19.7 */
#define AVTP_COND_NOT_UNINT(State)                  ( AVTP_STATE_UNINIT != (State) )
#define AVTP_COND_TX_READY(State)                   ( AVTP_STATE_TX_READY == (State) )
#define AVTP_COND_VALID_PTR(Pointer)                ( NULL_PTR != (Pointer) )
#define AVTP_COND_GREATER_ZERO(Value)               ( 0 < (Value) )
#define AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx)    ( AVTP_STREAM_CNT_TX > (StreamIdx) )
#define AVTP_COND_VALID_RX_STREAM_IDX(StreamIdx)    ( AVTP_STREAM_CNT_RX > (StreamIdx) )
#define AVTP_COND_NOT_BUSY(Flag)                    ( TRUE != (Flag) )
#define AVTP_COND_MIN_COM_HDR_LEN(LenByte)          ( AVTP_HDR_LEN_COMMON < (LenByte) )
#define AVTP_COND_MIN_STR_HDR_LEN(LenByte)          ( AVTP_HDR_LEN_STREAM < (LenByte) )
#define AVTP_COND_VALID_CTRL_IDX(AvTpCtrlIdx)       ( AVTP_CTRL_IDX_INVALID != (AvTpCtrlIdx) )
#define AVTP_CONT_VALID_VCTRL_IDX(VCtrlIdx)         ( AVTP_VCTRL_CNT > (VCtrlIdx) )
#define AVTP_CONT_VALID_FRAME_TYPE(FrameType)       ( AVTP_FRAME_TYPE == (FrameType) )

#define AVTP_COND_MAAP_MIN_LEN(LenByte)             ( AVTP_MAAP_MSG_LEN < (LenByte) )
#define AVTP_COND_SUBTYPE_IEC(Subtype)              ( AVTP_SUBTYPE_61883IIDC == (Subtype) )
#define AVTP_COND_SUBTYPE_CRF(Subtype)              ( AVTP_SUBTYPE_CRF == (Subtype) )
#define AVTP_COND_SUBTYPE_CVF(Subtype)              ( AVTP_SUBTYPE_CVF == (Subtype) )

#if (STD_ON == AVTP_AAF_SUPPORT)
#define AVTP_COND_SUBTYPE_AUDIO(Subtype)            ( AVTP_SUBTYPE_AAF == (Subtype) )
#define AVTP_COND_FORMAT_NOT_USERSPEC(Format)       ( AVTP_AAF_FMT_USERSPEC != (Format) )
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
#define AVTP_COND_TYPE_VIDEO_LINE(StreamIdx)        ( AVTP_CRF_TYPE_VIDEO_LINE == AvTp_CfgGetTxCrfType(StreamIdx) )
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */


#ifndef STATIC
#define STATIC static
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* ----- Mapping of error reporting macros ----- */
#if ( AVTP_DEV_ERROR_DETECT == STD_ON )
#  define AvTp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE ) \
          { if(!(CONDITION)) \
            { \
              (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
              return; \
            } \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define AvTp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) \
          { if(!(CONDITION)) \
            { \
              (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
              return (RET_VAL); \
            } \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define AvTp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) \
          { if(!(CONDITION)) \
            { \
              (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
            } \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define AvTp_CallDetReportError( API_ID, ERROR_CODE ) \
          { \
            (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define AvTp_CallDetReportErrorReturnValue( API_ID, ERROR_CODE, RET_VAL ) \
          { \
            (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
            return (RET_VAL); \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#  define AvTp_CallDetReportErrorReturnVoid( API_ID, ERROR_CODE ) \
          { \
            (void)Det_ReportError( AVTP_MODULE_ID, AVTP_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
            return; \
          }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#else
#  define AvTp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
#  define AvTp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
#  define AvTp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE )
#  define AvTp_CallDetReportError( API_ID, ERROR_CODE )
#  define AvTp_CallDetReportErrorReturnValue( API_ID, ERROR_CODE, RET_VAL )
#  define AvTp_CallDetReportErrorReturnVoid( API_ID, ERROR_CODE)
#endif  /* ( AVTP_DEV_ERROR_DETECT == STD_ON ) */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define AVTP_START_SEC_CODE
#include "MemMap.h"

 #if (STD_ON == AVTP_VERSION_INFO_API)
/**********************************************************************************************************************
 *  AvTp_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief        Return the BCD-coded version information of the AvTp module.
 *  \param[in]    VersionInfoPtr   Pointer to a memory location where the AvTp version information shall be stored.
 *  \return       none
 *  \context      This function can be called in any context.
 *  \aavailability This function is only available if AvTpVersionInfoApi is enabled.
 *********************************************************************************************************************/
extern FUNC(void, AVTP_CODE) AvTp_GetVersionInfo(
  AVTP_P2VAR(Std_VersionInfoType) VersionInfoPtr);
#endif  /* (STD_ON == AVTP_VERSION_INFO_API) */
/**********************************************************************************************************************
 *  AvTp_InitMemory
 *********************************************************************************************************************/
/*! \brief          Memory initialization of AvTp module.
 *  \param          none
 *  \return         none
 *  \context        This function can be called in any context.
 *  \note           This function has to be called before AvTp_Init.
 *********************************************************************************************************************/
extern FUNC(void, AVTP_CODE) AvTp_InitMemory(void);

/**********************************************************************************************************************
 *  AvTp_Init
 *********************************************************************************************************************/
/*! \brief          Initialization of AvTp module.
 *  \param          none
 *  \return         none
 *  \context        This function can be called in any context.
 *  \note           This function has to be called before using the module.
 *  \pre            Function AvTp_InitMemory() has to be called first.
 *********************************************************************************************************************/
extern FUNC(void, AVTP_CODE) AvTp_Init(void);

#if ( defined(AVTP_STREAM_CNT_TX) && (AVTP_STREAM_CNT_TX > 0) )
/**********************************************************************************************************************
 *    AvTp_ProvideTxBuffer
 *********************************************************************************************************************/
/*! \brief          Provides a transmit buffer to a upper layer module.
 *  \param[in]      StreamIdx   Pointer to the received data
 *  \param[in]      BufPtr      Pointer to the address where the buffer is placed
 *  \param[in,out]  LenBytePtr is an in/out parameter.\n
 *                  [in]  The requested buffer length.\n
 *                  [out] The actual buffer length reduced by Ethernet header and AvTp header length.
 *                      The actual buffer length is equal or bigger than the requested payload length
 *                      as far as the return value is BUFREQ_OK.
 *  \return         BUFREQ_OK        Buffer request accomplished successful.\n
 *                  BUFREQ_E_NOT_OK  Buffer request not successful. Buffer cannot be accessed.\n
 *                  BUFREQ_E_BUSY    Temporarily no buffer available. It's up the requester to retry for a certain
 *                                   time.\n
 *                  BUFREQ_E_OVFL    No Buffer of the required length can be provided.\n
 *  \context        This function can be called in task context.
 *********************************************************************************************************************/
extern FUNC(BufReq_ReturnType, AVTP_CODE) AvTp_ProvideTxBuffer(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AvTp_DataType) *BufPtr,
  AVTP_P2VAR(uint16) LenBytePtr);

/**********************************************************************************************************************
 *  AvTp_Transmit
 *********************************************************************************************************************/
/*! \brief      Transmits a AvTp stream pdu.
 *  \param[in]  StreamIdx      Index of TxStream
 *  \param[in]  LenByte        Byte count of the data to transmit
 *  \return     E_OK           Data transmission was successful\n
 *              E_NOT_OK       Data could not be transmitted\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_Transmit(
  AvTp_StreamIdxType StreamIdx,
  uint16 LenByte );

/**********************************************************************************************************************
 *  AvTp_SetMr
 *********************************************************************************************************************/
/*! \brief      Set Media-Clock-Restart-Bit (Mr) which will be transmitted with next sent Pdu of corresponding stream
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  Mr             Value of Mr-bit
 *  \return     E_OK           Setting Mr-bit was successful\n
 *              E_NOT_OK       Mr-bit could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetMr(
  AvTp_StreamIdxType StreamIdx,
  boolean Mr);

/**********************************************************************************************************************
 *  AvTp_SetEvent
 *********************************************************************************************************************/
/*! \brief      Set Event which will be transmitted with next sent Pdu
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  Event          Event
 *  \return     E_OK           Setting Event was successful\n
 *              E_NOT_OK       Event could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetEvent(
  AvTp_StreamIdxType StreamIdx,
  uint8 Event);


#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_SetSy
 *********************************************************************************************************************/
/*! \brief      Set Sy which will be transmitted with next sent Pdu
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  Sy             Sy field
 *  \return     E_OK           Setting Sy was successful\n
 *              E_NOT_OK       Sy could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetSy(
  AvTp_StreamIdxType StreamIdx,
  AvTp_DataType Sy);
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_Aaf_WriteSamples
 *********************************************************************************************************************/
/*! \brief      Write AVTP Audio samples to Buffer. If the configured number of samples per frame has been written
 *              the Frame is transmitted. This function can only used with subtype AVTP Audio and other formats than
 *              USERSPEC.
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  SamplePtr      Pointer to current sample frame (containing samples of all channel)
 *  \return     E_OK           Writing Sample was successful\n
 *              E_NOT_OK       Writing Sample failed\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_Aaf_WriteSamples(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2CONST(AvTp_DataType) SamplePtr);
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_SetMarker
 *********************************************************************************************************************/
/*! \brief      Set Marker Bit which will be transmitted with next sent Pdu
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  M              Marker Bit
 *  \return     E_OK           Setting Marker Bit was successful\n
 *              E_NOT_OK       Marker Bit could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetMarker(
  AvTp_StreamIdxType StreamIdx,
  boolean M);
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetFs
 *********************************************************************************************************************/
/*! \brief      Set Fs which will be transmitted with next sent Pdu
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  Fs             Fs bit
 *  \return     E_OK           Setting Fs was successful\n
 *              E_NOT_OK       Fs could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (STD_ON == AVTP_CRF_SUPPORT)
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetFs(
  AvTp_StreamIdxType StreamIdx,
  boolean Fs);
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetTu
 *********************************************************************************************************************/
/*! \brief      Set Tu for a Clock Reference Format PDU which will be transmitted with next sent Pdu
 *  \param[in]  StreamIdx      Index of Stream
 *  \param[in]  Tu             Timing uncertain
 *  \return     E_OK           Setting Tu was successful\n
 *              E_NOT_OK       Tu could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (STD_ON == AVTP_CRF_SUPPORT)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetTu(
  AvTp_StreamIdxType StreamIdx,
  boolean Tu);
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetH264Timestamp
 *********************************************************************************************************************/
/*! \brief      Set H264 Timestamp for a Compressed Video Format H264 PDU which will be transmitted with next sent
 *              Pdu\n
 *  \param[in]  StreamIdx         Index of Stream\n
 *  \param[in]  H264TimestampPtr  Pointer to the H264 Timestamp\n
 *  \return     E_OK              Setting the H264 Timestamp was successful\n
 *              E_NOT_OK          H264 Timestamp could not be set\n
 *  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (AVTP_CVF_H264_SUPPORT == STD_ON)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetH264Timestamp(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2CONST(uint32) H264TimestampPtr);
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (AVTP_CVF_H264_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  AvTp_GetTxStreamConfig
 *********************************************************************************************************************/
/*! \brief       Get a pointer to configured values of corresponding TxStream
 *  \param[in]   StreamIdx      Index of Stream
 *  \param[out]  StreamCfgPtr   Pointer to config of corresponding values
 *  \return      E_OK           Pointer is valid\n
 *               E_NOT_OK       Pointer is not valid\n
 *  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetTxStreamConfig(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2CONSTCFG(AvTp_StreamCfgType)) StreamCfgPtr);
#endif /* ( defined(AVTP_STREAM_CNT_TX) && (AVTP_STREAM_CNT_TX > 0) ) */

#if (0 < AVTP_STREAM_CNT_RX)
/**********************************************************************************************************************
 *  AvTp_GetRxStreamConfig
 *********************************************************************************************************************/
/*! \brief       Get a pointer to configured values of corresponding RxStream
 *  \param[in]   StreamIdx      Index of Stream
 *  \param[out]  StreamCfgPtr   Pointer to config of corresponding values
 *  \return      E_OK           Pointer is valid\n
 *               E_NOT_OK       Pointer is not valid\n
 *  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetRxStreamConfig(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2CONSTCFG(AvTp_StreamCfgType)) StreamCfgPtr);
#endif /* (0 < AVTP_STREAM_CNT_RX) ) */

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_ReadDiagnosticCounterListener
 *********************************************************************************************************************/
/*! \brief       Get a pointer to configured values of corresponding RxStream
 *  \param[in]   StreamIdx                      Index of Rx-Stream
 *  \param[out]  DiagnosticCounterListenerPtr   Pointer to set of Diagnostic Counters of a Listener stream
 *  \return      E_OK                           Pointer is valid\n
 *               E_NOT_OK                       Pointer is not valid\n
 *  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_ReadDiagnosticCounterListener(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2VAR(AvTp_DiagnosticCounterListenerType)) DiagnosticCounterListenerPtr);

/**********************************************************************************************************************
 *  AvTp_ReadDiagnosticCounterTalker
 *********************************************************************************************************************/
/*! \brief       Get a pointer to configured values of corresponding TxStream
 *  \param[in]   StreamIdx                    Index of Tx-Stream
 *  \param[out]  DiagnosticCounterTalkerPtr   Pointer to set of Diagnostic Counters of a Talker stream
 *  \return      E_OK                         Pointer is valid\n
 *               E_NOT_OK                     Pointer is not valid\n
 *  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_ReadDiagnosticCounterTalker(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2VAR(AvTp_DiagnosticCounterTalkerType)) DiagnosticCounterTalkerPtr);

/**********************************************************************************************************************
 *  AvTp_ResetDiagnosticCounter
 *********************************************************************************************************************/
/*! \brief       Get a pointer to configured values of corresponding Stream
 *  \param[in]   StreamIdx    Index of Stream
 *  \param[in]   IsListener   TRUE:  StreamIdx is a Listener(Rx)-StreamIdx
 *                            FALSE: StreamIdx is a Talker(Tx)-StreamIdx
 *  \return      E_OK         Reset has been successful \n
 *               E_NOT_OK     Reset has failed \n
 *  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, AVTP_CODE) AvTp_ResetDiagnosticCounter(
  AvTp_StreamIdxType StreamIdx,
  boolean IsListener);
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

#define AVTP_STOP_SEC_CODE
#include "MemMap.h"


#endif  /* AVTP_H */

/**********************************************************************************************************************
 *  END OF FILE: AvTp.h
 *********************************************************************************************************************/
