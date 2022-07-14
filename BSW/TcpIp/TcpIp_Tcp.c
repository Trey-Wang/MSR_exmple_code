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
 *         \file  TcpIp_Tcp.c
 *        \brief  Implementation of Transmission Control Protocol (TCP)
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_TCP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_TCP == STD_ON)

#include "TcpIp_Tcp.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_Tcp_Cbk.h"
#include "TcpIp_Priv.h"
#include "TcpIp_Cbk.h"
#include "IpBase.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
#include "TcpIp_IpV4.h"
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
#include "TcpIp_IpV6.h"
#endif
#include "IpBase_Copy.h"
#include "IpBase_Sock.h"

#if (TCPIP_SUPPORT_TLS == STD_ON)
# include "Tls.h"
# include "Tls_Cbk.h"
#endif

#if ((defined TCPIP_SPECIAL_TCP_ACC_ENABLED) && (TCPIP_SPECIAL_TCP_ACC_ENABLED == STD_ON))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
/* This feature adds a user callback similar to the callback 'User_TcpAccepted'. This callback is called right after
   receiving a 'SYN' on a TCP listen socket and reports the listen socket ID and the ID of the socket that will accept
   the connection request. These sockets are the same ones that are reported in the offical call 'User_TcpAccepted'
   later on. The reported socket IDs may only be used for analysis purposes, the accepting socket may not be used in
   any way before it is fully connected (reported by call of User_TcpAccepted). */

#include TCPIP_SPECIAL_TCP_ACC_INCLUDEFILE  /* PRQA S 0832 */ /* MD_TCPIP_TCP_0832 */

/*
The following 3 defines are needed in a UserCfg.h file (will be inserted at the end of TcpIp_Cfg.h):

#define TCPIP_SPECIAL_TCP_ACC_ENABLED       STD_ON
#define TCPIP_SPECIAL_TCP_ACC_INCLUDEFILE   "User_Cbk.h"
#define TCPIP_SPECIAL_TCP_ACCEPTING_FCT(a,b,c)     User_TcpPreAccepted(a,b,c)
*/
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT CONFIGURABLE MACROS
 *********************************************************************************************************************/
/* these defines might be configurable in future  */
#define TCPIP_TCP_MIN_PATH_MTU                         48u  /* min size is 68 - IPv4-Header size */

/* [RFC6691 TCP Options and Maximum Segment Size (MSS) */
#define TCPIP_TCP_DEFAULT_TX_MSS_IPV4                 (TCPIP_IPV4_MTU_MIN_BYTE - TCPIP_IPV4_HDR_LEN_BYTE - TCPIP_TCP_HDR_LEN_BYTE) /* =  536 bytes |  (default IPv4 Maximum Transmission Unit) - 20 (IPv4 fixed header size) - 20 (TCP fixed header size) =  536. */
#define TCPIP_TCP_DEFAULT_TX_MSS_IPV6                 (TCPIP_IPV6_MTU_MIN_BYTE - TCPIP_IPV6_HDR_LEN_BYTE - TCPIP_TCP_HDR_LEN_BYTE) /* = 1220 bytes |  (default IPv6 Maximum Transmission Unit) - 40 (IPv6 fixed header size) - 20 (TCP fixed header size) = 1220. */

#define TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE   STD_ON

#define TCPIP_TCP_TX_RETRY_INT_RESET_VALUE_CYCLES   (3000 / TcpIp_VCfgGetMainFctPeriodMsec())  /* RFC 6298: Reset the initial RTO (retransmission timeout) to 3 seconds in case of a retransmission of a SYN segment */

#define TCPIP_TCP_CWND_IDLE_TIMEOUT_ENABLED            STD_ON  /* temporary switch to disable the cwnd reset on idle timeout */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define TCPIP_TCP_RST_LOG_SUPPORT   STD_OFF

#define TCPIP_TCP_EMPTY_PORT                   0x0000u
#define TCPIP_TCP_EMPTY_CHKSUM                 0x0000u

#define TCPIP_TCP_CODE_BIT_MASK_FSA            0x13u  /* FIN_SYN_ACK */
#define TCPIP_TCP_CODE_BIT_FIN                 0x01u
#define TCPIP_TCP_CODE_BIT_SYN                 0x02u
#define TCPIP_TCP_CODE_BIT_RST                 0x04u
#define TCPIP_TCP_CODE_BIT_PSH                 0x08u
#define TCPIP_TCP_CODE_BIT_ACK                 0x10u
#define TCPIP_TCP_CODE_BIT_URG                 0x20u
#define TCPIP_TCP_CODE_BIT_ECE                 0x40u
#define TCPIP_TCP_CODE_BIT_CWR                 0x80u

#define TCPIP_TCP_HDR_LEN_BYTE                   20u /* TCP header requires at least 20 bytes (without options) */
#define TCPIP_IPV4_HDR_LEN_BYTE                  20u /* IPv4 header requires at least 20 bytes (without options) */
#define TCPIP_IPV6_HDR_LEN_BYTE                  40u /* IPv6 fixed header requires 40 bytes */

#define TCPIP_IPV4_MTU_MIN_BYTE                 576u  /* Minmum supported MTU of IPv4 according to [RFC879 Page 25] */
#define TCPIP_IPV6_MTU_MIN_BYTE                1280u  /* Minmum supported MTU of IPv6 according to [RFC2460 5. Packet Size Issues] */

#define TCPIP_TCP_OPT_ID_END                   0
#define TCPIP_TCP_OPT_ID_NOP                   1
#define TCPIP_TCP_OPT_ID_MAX_SEG_SIZE          2
#define TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE         4
#define TCPIP_TCP_OPT_ID_SACK_PERMIT           4
#define TCPIP_TCP_OPT_LEN_SACK_PERMIT          2
#define TCPIP_TCP_OPT_ID_SACK                  5
#define TCPIP_TCP_OPT_LEN_SACK                 2
#define TCPIP_TCP_OPT_ID_TIMESTAMP             8
#define TCPIP_TCP_OPT_LEN_TIMESTAMP           10
#define TCPIP_TCP_OPT_ID_USER_TIMEOUT         28
#define TCPIP_TCP_OPT_LEN_USER_TIMEOUT         4

/* option flags used with TcpIp_Tcp_TxOneTimeOpts[] */
#define TCPIP_TCP_OPT_FLAG_USER_TIMEOUT        0x01u
#define TCPIP_TCP_OPT_FLAG_SACK_PERMIT         0x02u

/* defines used in variable TcpIp_Tcp_CongestionControlMode */
#define TCPIP_TCP_CONG_CTRL_MODE_SLOW_START             1
#define TCPIP_TCP_CONG_CTRL_MODE_CONGESTION_AVOIDANCE   2
#define TCPIP_TCP_CONG_CTRL_MODE_FAST_RETRANSMIT        3
#define TCPIP_TCP_CONG_CTRL_MODE_FAST_RECOVER           4

#define TCPIP_TCP_DO_NOT_QUEUE_ACKS                                    STD_ON
#define TCPIP_TCP_QUEUE_REQ_DATA_IN_MAIN_FUNCTION_ONLY                 STD_ON
#define TCPIP_TCP_FREE_ACKED_REQ_AND_RETR_ELEMENTS_IN_MAIN_FUNCTION    STD_ON

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define TCPIP_TCP_SEQ1_BIGGER_SEQ2(A, B)            ( ((A)-(B)-1) < 0x80000000u )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(A, B)   ( ((A)-(B)  ) < 0x80000000u )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_TCP_SEQ1_SMALLER_SEQ2(A, B)           ( TCPIP_TCP_SEQ1_BIGGER_SEQ2((B), (A)) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(A, B)  ( TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2((B), (A)) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Initial congestion window calculation according to [RFC5681 3.1. Slow Start and Congestion Avoidance] */
/* RFC 5681 defines an initial cwnd of 2*SMSS if (SMSS > 2190), but that will never be the case for our implementations of IPv4 and IPv6 */
#define TCPIP_TCP_DEFAULT_SEGMENT_LIMIT(SMSS)       ((1095 < (SMSS)) ? 3 : 4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  IpBase_PortType SrcPort;
  IpBase_PortType TgtPort;
  uint32          SeqNo;
  uint32          AckNo;
  uint8           DataOffsetAndRes; /*! |  Data Offset  |___|___|___|___| (Data Offset in 32bit words, ___ = Reserved) */ 
  uint8           CodeBits;         /*! |___|___|URG|ACK|PSH|RST|SYN|FIN| */
  uint16          Window;
  uint16          Checksum;
  uint16          UrgentPtr;
} TcpIp_Tcp_HdrType;

typedef struct
{
  uint8          Kind;
  uint8          OptLen;
} TcpIp_Tcp_HdrGenericOptType;  /* Generic option */

typedef struct
{
  uint8          Kind;
  uint8          OptLen;
  uint16         SegLen;
} TcpIp_Tcp_HdrMssOptType;  /* Maximum Segment Size option */

typedef struct
{
  uint8          Kind;
  uint8          OptLen;
  uint32         TSval;  /* TS Value */
  uint32         TSecr;  /* TS Echo Reply */
} TcpIp_Tcp_HdrTsOptType;  /* Timestamp option */  /* PRQA S 3205 */ /* MD_TCPIP_3205 */

#if (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON ))
typedef struct
{
  uint8  RstCallerId;
  uint32 SeqNo;
  uint32 AckNo;
  uint32 DstIpAddr;
  uint32 SrcIpAddr;
  uint16 DstPort;
  uint16 SrcPort;
  uint8  TcpIdx;
  uint8  TcpState;
} TcpIp_Tcp_RstReasonLogType;
#endif
  /* (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON )) */

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON ))
STATIC VAR(TcpIp_Tcp_RstReasonLogType, TCPIP_VAR_NOINIT) TcpIp_Tcp_RstReasonLog[100];
#endif
  /* (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON )) */

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(uint32, TCPIP_VAR_NOINIT) TcpIp_Tcp_GlobalTimestamp;  /* MainFunction counter */

#define TCPIP_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON ))
STATIC VAR(uint16, TCPIP_VAR_NOINIT) TcpIp_Tcp_RstReasonLogCnt;  /* counter */
#endif
  /* (( TCPIP_DEV_ERROR_DETECT == STD_ON ) && ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON )) */

#define TCPIP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(uint8, TCPIP_VAR_NOINIT) TcpIp_Tcp_RstTxQueueReadPos;
STATIC VAR(uint8, TCPIP_VAR_NOINIT) TcpIp_Tcp_RstTxQueueWritePos;
STATIC VAR(uint8, TCPIP_VAR_NOINIT) TcpIp_Tcp_RstTxQueueFillNum;

#define TCPIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VClosePartlyAcceptedSockets(
    TcpIp_TcpIdxType TcpIdx );

#if (TCPIP_TCP_CHECK_RX_CHECKSUM == STD_ON)
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VCheckRecChecksum(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 LenByte );
#endif

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmit(
    TcpIp_TcpIdxType TcpIdx );
#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
#else
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VExistsUntransmittedSegmentInRetryQueue(
    TcpIp_TcpIdxType TcpIdx );
#endif
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VFinalizeTxSegment(
    TcpIp_TcpIdxType TcpIdx,
    boolean SrcIsMainFct );

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmitSegment(
  TcpIp_TcpIdxType tcpIdx, 
  uint8            flags, 
  uint32           seqNo, 
  uint8            oneTimeOptions, 
  uint32           dataStartPos, 
  uint32           dataLenByte);

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VRestartSock(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VInitSock(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSendReset(
    TcpIp_TcpIdxType TcpIdx,
    TcpIp_IpVXAddrIdType                                       IpAddrId,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) RemSockPtr,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) LocSockPtr,
    uint32 SeqNo,
    uint32 AckNo,
    uint8 Flags );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDelRxQueue(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDelTxQueue(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSendFlags(
    TcpIp_TcpIdxType TcpIdx,
    uint8 Flags );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VTransmitResets( void );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VClearRetrQueue(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VHandleTcpOptions(
    TcpIp_TcpIdxType TcpIdx,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)  HdrOptPtr,
    uint8                                       HdrOptLen,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpSegSeqPtr,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpSegAckPtr );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VFreeFirstRetrQueueEle(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VFreeAckedReqAndRetrElements(
    TcpIp_TcpIdxType TcpIdx,
    uint32 SegAckNo );
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VQueueRemReqData(
    TcpIp_TcpIdxType TcpIdx );
#if (TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VInsElesInRetrQueue(
    TcpIp_TcpIdxType TcpIdx,
    TcpIp_TcpTxRetrQueueIdxType PrevRetrEleIdx,
    uint8 NumNewElements );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDropEndEleInRetrQueue(
    TcpIp_TcpIdxType TcpIdx,
    uint8 NumDropEle );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VUnqueueDataInReqArray(
    TcpIp_TcpIdxType TcpIdx,
    uint32 UnqueueLen );
#endif

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainStateHandling(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainRxDataForwarding(
    TcpIp_TcpIdxType TcpIdx,
    uint16 maxLenToForward);
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainIdleTimeoutHandling(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSplitFirstRetrQueueEle(
    TcpIp_TcpIdxType TcpIdx );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainTxDataHandling(
    TcpIp_TcpIdxType TcpIdx );

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VUpdateRttVars(
    TcpIp_TcpIdxType TcpIdx,
    uint32 ElementRtt );
#endif  /* (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON) */

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VIncAckSumN(
  TcpIp_TcpIdxType TcpIdx,
  uint32 IncSize );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VIncCwnd(
  TcpIp_TcpIdxType TcpIdx,
  uint32 IncSize );
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSetSsthreshEqu4(
  TcpIp_TcpIdxType TcpIdx );
#endif  /* (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON) */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveTrigger
 **********************************************************************************************************************/
/*! \brief      Resets all Keep-Alive counters for a TCP connection. 
 *              Should be triggered everytime when data has been transferred over the connection.
 *  \param[in]  TcpIdx           Index of the TCP connection.
 *  \context    task-level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveTrigger(TcpIp_TcpIdxType tcpIdx);

/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveReset
 **********************************************************************************************************************/
/*! \brief      Turns off the Keep-Alive mechanism for a TCP socket and resets all parameters to their default values.
 *  \param[in]  TcpIdx           Index of the TCP connection.
 *  \context    task-level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveReset(TcpIp_TcpIdxType tcpIdx);

/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveCopyParameters
 **********************************************************************************************************************/
/*! \brief      Copies TCP keep-alive socket parameters to another socket.
 *  \param[in]  tcpIdxSrc        Index of the socket from which the parameters shall be copied.
 *  \param[in]  tcpIdxDst        Index of the socket to which the parameters shall be copied.
 *  \context    ANY
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveCopyParameters(TcpIp_TcpIdxType tcpIdxSrc, TcpIp_TcpIdxType tcpIdxDst);

/***********************************************************************************************************************
 *  TcpIp_Tcp_VTransmitKeepAliveProbe
 **********************************************************************************************************************/
/*! \brief      Transmits an ACK-only segment with current sequence number decremented by one. (TCP keep alive segment)
 *  \param[in]  tcpIdx           Index of the TCP connection.
 *  \return     E_OK             Keep-alive segment transmitted.
 *  \return     E_OK             Keep-alive segment not transmitted.
 *  \context    task-level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmitKeepAliveProbe(TcpIp_TcpIdxType tcpIdx);
#endif

/***********************************************************************************************************************
 *  TcpIp_AddWithWrapAround16
 **********************************************************************************************************************/
/*! \brief      Increments a 16-bit variable up to (valueCount-1) with wrap-around.
 *  \details    Administers a ringbuffer where valueCount is the number of elements.
 *              Index values are zero based thus the value valueCount is the first invalid value of the sum 'value+incValue'.
 *  \param[in]  value            Value that shall be incremented.
 *  \param[in]  incValue         Value that shall be added.
 *  \param[in]  valueCount       Number of valid values. ([0...valueCount-1])
 *  \context    ANY
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(uint16, TCPIP_CODE) TcpIp_AddWithWrapAround16(
  uint16 value, 
  uint16 incValue, 
  uint16 valueCount);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  TcpIp_Tcp_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration of the Transport Control
 *              Protocol and may be used to initialize the data structures.
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \note       No configuration variant support implemented, so only TCPIP_CONFIG_VARIANT_1 (compile time) is available.
 *  \context    initialization
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Init( void )

{
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

# if ( TCPIP_TCP_RST_LOG_SUPPORT == STD_ON )
  TcpIp_Tcp_RstReasonLogCnt = 0;
# endif
  /* TCPIP_TCP_RST_LOG_SUPPORT == STD_ON */
#endif
  /* TCPIP_DEV_ERROR_DETECT == STD_ON */

  TcpIp_Tcp_GlobalTimestamp  = 0;
  TcpIp_Tcp_DynamicPortCount = 0;

  /* init TCP retry queue */
  TcpIp_Tcp_NumFreeRetrQueueEle   = TcpIp_VCfgGetMaxNumTcpSocks() * TcpIp_Tcp_VCfgGetTxRetrQueueSize();
  TcpIp_Tcp_FirstFreeRetrQueueEle = 0;
  TcpIp_Tcp_LastFreeRetrQueueEle  = TcpIp_Tcp_NumFreeRetrQueueEle -1;
  {
    /* link all elements in the empty list */
    uint16 EleIdx = 0;
    TcpIp_Tcp_TxRetrQueue[EleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext = EleIdx +1;
    for(EleIdx = 1; EleIdx < (TcpIp_Tcp_NumFreeRetrQueueEle -1); EleIdx++)
    {
      TcpIp_Tcp_TxRetrQueue[EleIdx].ElePrev = EleIdx -1;
      TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext = EleIdx +1;
    }
    TcpIp_Tcp_TxRetrQueue[EleIdx].ElePrev = EleIdx -1;
    TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
  }

  TcpIp_Tcp_RstTxQueueReadPos  = 0;
  TcpIp_Tcp_RstTxQueueWritePos = 0;
  TcpIp_Tcp_RstTxQueueFillNum  = 0;

#if (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON)
  TcpIp_Tcp_OooNumFreeElements = (TcpIp_Tcp_VCfgGetOooAvgNumSeg() * TcpIp_VCfgGetMaxNumTcpSocks());
  TcpIp_Tcp_OooFirstFreeEleIdx = 0;
  {
    /* link all free elments in one consecutive line */
    uint16 i;
    for(i=0; i<(TcpIp_Tcp_VCfgGetOooAvgNumSeg() * TcpIp_VCfgGetMaxNumTcpSocks()); i++)
    {
      TcpIp_Tcp_RxPreBufArray[i].NextEleIdx = (i+1);
    }
  }
#endif

  {
    TcpIp_TcpIdxType TcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
    for(TcpIdx = 0; TcpIdx < (TcpIp_VCfgGetMaxNumTcpSocks()); TcpIdx++)
    {
      /* init TCP retry queue */
      TcpIp_Tcp_FirstRetrQueueEle[TcpIdx] = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_LastRetrQueueEle[TcpIdx]  = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] = 0;

#if (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON)
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
      TcpIp_Tcp_OooLastChangedEleIdx[TcpIdx] = 0xFFFF;
#endif
      TcpIp_Tcp_OooFirstEleIdx[TcpIdx] = 0xFFFF;  /* invalid element index */
      TcpIp_Tcp_OooNumElements[TcpIdx] = 0;

#endif  /* TCPIP_TCP_OOO_RX_SUPPORT == STD_ON */

      TcpIp_Tcp_VRestartSock(TcpIdx);

      /* reset buffer to TCP socket index mapping */
      TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] = 0xFF; /* invalid buffer index */
      TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] = 0xFF; /* invalid buffer index */

      TcpIp_Tcp_RequestedRxBufferSize[TcpIdx] = 0;
      TcpIp_Tcp_RequestedTxBufferSize[TcpIdx] = 0;
    }
  }

  {
    /* reset buffer mapping */
    uint8 BufIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
    for(BufIdx = 0; BufIdx < TCPIP_TCP_RX_BUFFERS_NUM; BufIdx++)
    {
      TcpIp_Tcp_RxBuffer_IsUsedBySocket[BufIdx]  = TCPIP_INV_SOCKET_ID;  /* invalid TCP (socket) index */
    }
    for(BufIdx = 0; BufIdx < TCPIP_TCP_TX_BUFFERS_NUM; BufIdx++)
    {
      TcpIp_Tcp_TxBuffer_IsUsedBySocket[BufIdx]  = TCPIP_INV_SOCKET_ID;  /* invalid TCP (socket) index */
    }
  }

  {
    /* reset mapping of tx buffers to the TCP sockets */
    uint8 BufferIndex;
    for(BufferIndex = 0; BufferIndex < TCPIP_TCP_TX_BUFFERS_NUM; BufferIndex++)
    {
      TcpIp_Tcp_TxBuffer_IsUsedBySocket[BufferIndex] = TCPIP_INV_SOCKET_ID;
    }
  }

  return;
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_PrepareShutdown
 **********************************************************************************************************************/
/*! \brief      Prepare shutdown of TCP. Close "listen" socket, preconfig socket to "closed" instead of "listen".
 *  \param[in]  TcpIdx                tcp socket index
 *  \return     TRUE                  shutdown prepared
 *  \return     FALSE                 shutdownd request failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_PrepareShutdown( TcpIp_TcpIdxType TcpIdx )
{
  /* for socket:
     - if state is LISTEN, then set state to CLOSED */

  boolean RetValue = FALSE;

  if(TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx])
  {
    TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_CLOSED;
    TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
    RetValue = TRUE;
  }

  return RetValue;
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  LocalAddrId           local address identifier
 *  \param[in]  RxSockRemAddrPtr      pointer to the socket
 *  \param[in]  DataPtr               pointer to the received data
 *  \param[in]  LenByte               length of received data in bytes
 *  \param[in]  Prec                  precedence, part of TypeOfService from IP
 *  \param[in]  ChecksumCalculated    indicates if the TCP checksum is already checked by lower layer
 *  \note       The parameter 'ChecksumCalculated' is only used if the lower layer (hardware) is capable of checking
 *                the received TCP checksum.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_RxIndication(
    TcpIp_LocalAddrIdType LocalAddrId,
    P2VAR(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 LenByte
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
    )
{
  /* Segment variables */
  uint32 TcpIp_Tcp_SegSeq; /* SEG.SEQ - segment sequence number - first sequence number of a segment */
  uint32 TcpIp_Tcp_SegAck; /* SEG.ACK - segment acknowledgment number - acknowledgment from the
                                         receiving TCP (next sequence number expected by the receiving TCP) */
  uint16 TcpIp_Tcp_SegLen; /* SEG.LEN - segment length - the number of octets occupied by the data in the
                                         segment (_without_ counting SYN and FIN, different than specified in RFC) */
  uint16 TcpIp_Tcp_SegWnd; /* SEG.WND - segment window */

  TcpIp_SockIdxType   SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  TcpIp_TcpIdxType    TcpIdx;   /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  TcpIp_Tcp_HdrType  *HdrPtr;
  uint8               HdrAndOptLenByte;  /* TCP header length including TCP options */
  uint8              *TcpPayloadPtr;
  TcpIp_SockAddrInXType RxSockLocAddr;
  P2VAR(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_VAR) RxSockLocAddrPtr = (IpBase_SockAddrType*)&RxSockLocAddr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT, TCPIP_TCP_API_ID_RX_INDICATION, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_VALID_LOCALADDRID(LocalAddrId), TCPIP_TCP_API_ID_RX_INDICATION, TCPIP_E_INV_ARG )
  TcpIp_CheckDetErrorReturnVoid((RxSockRemAddrPtr != NULL_PTR), TCPIP_TCP_API_ID_RX_INDICATION, TCPIP_E_PARAM_POINTER )
  TcpIp_CheckDetErrorReturnVoid((DataPtr          != NULL_PTR), TCPIP_TCP_API_ID_RX_INDICATION, TCPIP_E_PARAM_POINTER )
  /* ------------------------------------------------- */

  HdrPtr           = (TcpIp_Tcp_HdrType *)(DataPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
  HdrAndOptLenByte = (uint8)(((HdrPtr->DataOffsetAndRes & 0xF0) >> 4) * 4);
  TcpPayloadPtr    = &(((uint8 *)(DataPtr))[HdrAndOptLenByte]);

  if ( (TCPIP_TCP_HDR_LEN_BYTE > LenByte) || (HdrAndOptLenByte > LenByte) || (TCPIP_TCP_HDR_LEN_BYTE > HdrAndOptLenByte) )
  {
    /* invalid length -> drop message immediately */
    TCPIP_DUMMY_ASM_NOP
    return;
  }

  TcpIp_Tcp_SegSeq = IPBASE_HTON32(HdrPtr->SeqNo);
  TcpIp_Tcp_SegAck = IPBASE_HTON32(HdrPtr->AckNo);
  TcpIp_Tcp_SegLen = LenByte - HdrAndOptLenByte;  /* TCP payload length */
  TcpIp_Tcp_SegWnd = IPBASE_HTON16(HdrPtr->Window);

  if (E_OK != TcpIp_GetIpAddr(LocalAddrId, (TcpIp_SockAddrType*)RxSockLocAddrPtr, NULL_PTR, NULL_PTR))  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    return;
  }

#if (TCPIP_TCP_CHECK_RX_CHECKSUM == STD_ON)

# if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
  if(ChecksumCalculated == TRUE)
  {
    /* The TCP checksum was already checked by the lower layer / hardware. If the checksum was wrong the packet will not reach the TCP at all, so no further check is necessary here. */
  }
  else
# endif
  {
    /* check TCP checksum */
    if(E_OK != TcpIp_Tcp_VCheckRecChecksum(RxSockRemAddrPtr, RxSockLocAddrPtr, DataPtr, LenByte))
    {
      return;  /* drop invalid message */
    }
  }
#endif /* TCPIP_TCP_CHECK_RX_CHECKSUM */

  /* set port values in the socket address structs (incoming function parameters) */
  ((IpBase_SockAddrInType *)RxSockLocAddrPtr)->sin_port = HdrPtr->TgtPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
  ((IpBase_SockAddrInType *)RxSockRemAddrPtr)->sin_port = HdrPtr->SrcPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */

  /* Ignore incoming messages with invalid port. Invalid network addresses are already filtered in the IP layer */
  if( TCPIP_PORT_ANY == ((IpBase_SockAddrInType *)RxSockLocAddrPtr)->sin_port )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
  {
    /* drop invalid message immediately */
    return;
  }

  /* Identify socket: */
  SockIdx = TcpIp_VRxTcpSockIdxIdent(RxSockRemAddrPtr, RxSockLocAddrPtr, LocalAddrId);

    if(TCPIP_INV_SOCK_IDX == SockIdx)
    {
      /* no corresponding socket found -> send reset and drop message */
      if(0 == (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits))
      {
        if(0 != (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits))
        {
          /* ACK */
          /* send reset <SEQ=SEG.ACK><CTL=RST> */
          TcpIp_Tcp_VSendReset(TCPIP_TCP_INV_IDX, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);
        }
        else
        {
          /* no ACK */
          /* send reset <ACK=SEG.SEQ+SEG.LEN><CTL=RST,ACK> */
          uint32 SeqNo = TcpIp_Tcp_SegSeq;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
          SeqNo += TcpIp_Tcp_SegLen;
          if(0 != (TCPIP_TCP_CODE_BIT_SYN & HdrPtr->CodeBits))
          {
            SeqNo++;
          }
          if(0 != (TCPIP_TCP_CODE_BIT_FIN & HdrPtr->CodeBits))
          {
            SeqNo++;
          }
          TcpIp_Tcp_VSendReset(TCPIP_TCP_INV_IDX, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, 0, SeqNo, TCPIP_TCP_CODE_BIT_ACK);
        }
      }
      return;  /* stop further interpretation of this message */
    }

    TcpIdx = TCPIP_SOCKIDX2TCPIDX(SockIdx);

    (void)IpBase_CopySockAddr((IpBase_SockAddrType *)(&(TcpIp_Tcp_RxIndArray[TcpIdx].RemSockAddr)), RxSockRemAddrPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    TcpIp_Tcp_RxIndArray[TcpIdx].LocAddrId = LocalAddrId;

    /* ------------------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------------------ */
    /* ------------------------------------------------------------------------------------------ */
    /* *** begin processing dependent on TCP (socket) state ***                                   */
    /* ------------------------------------------------------------------------------------------ */
    if(TCPIP_TCP_SOCK_STATE_CLOSED == TcpIp_Tcp_SockState[TcpIdx])
    {
      /* send reset <SEQ=SEG.ACK><CTL=RST> */
      TcpIp_Tcp_VSendReset(TcpIdx, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);
      return;
    }
    /* ------------------------------------------------------------------------------------------ */
    else if( (TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx]) )
    {

      /* [LISTEN] first: check for an RST */
      if(0 != (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits))
      {
        /* ignore message */
        return;
      }

      /* [LISTEN] second: check for an ACK */
      if(0 != (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits))
      {
        /* send reset <SEQ=SEG.ACK><CTL=RST> */
        TcpIp_Tcp_VSendReset(TcpIdx, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);
        return;
      }

      /* [LISTEN] third: check for a SYN */
      if(0 != (TCPIP_TCP_CODE_BIT_SYN & HdrPtr->CodeBits))
      {
        /* (security is an IP option) */
        /* check security/compartment for exact match: fail -> send reset <SEQ=SEG.ACK><CTL=RST> */
        TcpIp_TcpIdxType NewTcpIdx = TcpIp_Tcp_DuplicateSocket(TcpIdx);

        if (TCPIP_TCP_INV_IDX != NewTcpIdx)
        {
          /* general match -> forward to UL, ask for accept */
          TcpIp_Tcp_RcvNxt[NewTcpIdx] = TcpIp_Tcp_SegSeq+1;
          TcpIp_Tcp_SndWnd[NewTcpIdx] = TcpIp_Tcp_SegWnd;

          if (HdrAndOptLenByte > TCPIP_TCP_HDR_LEN_BYTE)
          {
            /* there is a TCP option included in the header */
            if (E_OK != TcpIp_Tcp_VHandleTcpOptions(NewTcpIdx, RxSockRemAddrPtr, RxSockLocAddrPtr, &DataPtr[TCPIP_TCP_HDR_LEN_BYTE], HdrAndOptLenByte - TCPIP_TCP_HDR_LEN_BYTE, &TcpIp_Tcp_SegSeq, &TcpIp_Tcp_SegAck))
            {
              /* stop processing of this message and drop it. */
               /* switch to state CLOSED, delete/clear TCB element, and return */
              TcpIp_Tcp_VRestartSock(NewTcpIdx);
              TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(NewTcpIdx));
              return;
            }
          }

          /* BSD:
             - Store for later use when polling this socket: TcpIp_Tcp_SegSeq, TcpIp_Tcp_SegWnd, TcpIp_Tcp_MaxSegLen
             - Only Backlog (number) connection requests can be stored.
             -> implementation limitation: (Backlog > 1) is not supported */

          TcpIp_Tcp_BackLogArray[TcpIdx].SynAvailable = TRUE;
          TcpIp_Tcp_BackLogArray[TcpIdx].MaxTxSegSize = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
          TcpIp_Tcp_BackLogArray[TcpIdx].RemPort      = HdrPtr->SrcPort;
          TcpIp_Tcp_BackLogArray[TcpIdx].RxSeqNo      = TcpIp_Tcp_SegSeq;
          TcpIp_Tcp_BackLogArray[TcpIdx].Window       = TcpIp_Tcp_SegWnd;

          TcpIp_Tcp_AcceptSocket(TcpIdx, NewTcpIdx);
        }
        else
        {
          /* reject connection request, send TCP.RST */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, (const IpBase_SockAddrType*)&TcpIp_Tcp_RxIndArray[TcpIdx].RemSockAddr, RxSockLocAddrPtr, TcpIp_Tcp_SegSeq, 0, 0);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        }
      }

      /* [LISTEN] fourth: other text or control */
      /* nothing to do here */

    }
    /* ------------------------------------------------------------------------------------------ */
    else if ( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[TcpIdx]) )
    {
      /* [SYNSENT] first: check the ACK bit */
      if(0 != (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits))
      {
        if( TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_Iss[TcpIdx]) ||
                      TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_SndNxt[TcpIdx]) )
        {
          /* ACK number is outside of the window. */
          if(0 == (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits))
          {
            /* send reset <SEQ=SEG.ACK><CTL=RST> */
            TcpIp_Tcp_VSendReset(TcpIdx, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);
          }
          else
          {
            /* RST bit is set, just ignore this message */
          }
          return;
        }
        else
        {
          /* Valid ACK received. */
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
          TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif
        }

        if(TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_SndUna[TcpIdx]))
        {
          /* duplicated message
             -> drop message silently */
          return;
        }
      }

      /* [SYNSENT] second: check the RST bit */
      if(0 != (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits))
      {
        if(0 != (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits))
        {
#if (TCPIP_SUPPORT_TLS == STD_ON)
          boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
          uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];
          /* set error: "Connection reset by peer" */
          TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10054);

          /* switch to state CLOSED, delete/clear TCB element, and return */
          TcpIp_Tcp_VRestartSock(TcpIdx);
          TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
          TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          /* drop message */
          return;
        }
        else
        {
          /* drop message silently */
          return;
        }
      }

      /* [SYNSENT] third: check the security and precedence */
      {
        /* check security/compartment for exact match: fail -> send reset <SEQ=SEG.ACK><CTL=RST> */
        /* security handling not supported   -> do nothing */
        /* precedance handling not supported -> do nothing */
      }

      /* [SYNSENT] fourth: check the SYN bit */
      if(0 != (TCPIP_TCP_CODE_BIT_SYN & HdrPtr->CodeBits))
      {
        /* no security compartment handling implemented */
        /* no precedence handling implemented */

        /* RFC: RCV.NXT is set to SEG.SEQ+1, IRS is set to SEG.SEQ. SND.UNA should be advanced
           to equal SEG.ACK (if there is an ACK), and any segments on the retransmission queue
           which are thereby acknowledged should be removed. */
        TcpIp_Tcp_RcvNxt[TcpIdx] = TcpIp_Tcp_SegSeq +1;
        if(0 != (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits))
        {
          TcpIp_Tcp_SndUna[TcpIdx] = TcpIp_Tcp_SegAck;
        }

        /* clear all segments in retransmission queue */
        TcpIp_Tcp_VDelTxQueue(TcpIdx);

        if (HdrAndOptLenByte > TCPIP_TCP_HDR_LEN_BYTE)
        {
          /* there is a TCP option included in the header */
          if (E_OK != TcpIp_Tcp_VHandleTcpOptions(TcpIdx, RxSockRemAddrPtr, RxSockLocAddrPtr, &DataPtr[TCPIP_TCP_HDR_LEN_BYTE], HdrAndOptLenByte - TCPIP_TCP_HDR_LEN_BYTE, &TcpIp_Tcp_SegSeq, &TcpIp_Tcp_SegAck))
          {
            /* stop processing of this message. Sending a RST was already triggered by TcpIp_Tcp_VHandleTcpOptions() */
            return;
          }
        }

        if(TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_Iss[TcpIdx]))
        {
          /* send ACK: <SEQ=SND.NXT><ACK=RCV.NXT><CTL=ACK> */
          TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);

          /* set send window according to received segment */
          TcpIp_Tcp_SndWnd[TcpIdx] = TcpIp_Tcp_SegWnd;
          TcpIp_Tcp_SndWl1[TcpIdx] = TcpIp_Tcp_SegSeq;
          TcpIp_Tcp_SndWl2[TcpIdx] = TcpIp_Tcp_SegAck;

          TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_ESTABLISHED;
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
          if(TRUE == TcpIp_Tcp_RtoResetPending[TcpIdx])
          {
            TcpIp_Tcp_RtoValid[TcpIdx]        = TCPIP_TCP_TX_RETRY_INT_RESET_VALUE_CYCLES;
            TcpIp_Tcp_Rto[TcpIdx]             = TCPIP_TCP_TX_RETRY_INT_RESET_VALUE_CYCLES;
            TcpIp_Tcp_RtoResetPending[TcpIdx] = FALSE;
          }
#endif

          /* TcpAccepted, accepted by peer */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(SockIdx, TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONNECTED, TcpIp_Tcp_UseTls[TcpIdx]);
#else
          TcpIp_VSockTcpStateChg(SockIdx, TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONNECTED, FALSE);
#endif
        }
        else
        {
          /* send SYN/ACK: <SEQ=ISS><ACK=RCV.NXT><CTL=SYN,ACK> */
          /* switch to state SYN_RCVD */
          TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_SYNRCVD;
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_SYNRCVD;
          TcpIp_Tcp_SndNxt[TcpIdx] = TcpIp_Tcp_Iss[TcpIdx];
          TcpIp_Tcp_VSendFlags(TcpIdx, (TCPIP_TCP_CODE_BIT_SYN | TCPIP_TCP_CODE_BIT_ACK));
        }

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
        TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif
      }

      /* [SYNSENT] fifth: if neither of the SYN or RST bits is set then drop the segment and return. */
      if(0 == ((TCPIP_TCP_CODE_BIT_SYN | TCPIP_TCP_CODE_BIT_RST) & HdrPtr->CodeBits))
      {
        /* drop message silently */
        return;
      }
    }
    /* ------------------------------------------------------------------------------------------ */
    else
    {
      /*
      States:
      TCPIP_TCP_SOCK_STATE_SYNRCVD
      TCPIP_TCP_SOCK_STATE_ESTABLISHED
      TCPIP_TCP_SOCK_STATE_CLOSEWAIT
      TCPIP_TCP_SOCK_STATE_LASTACK
      TCPIP_TCP_SOCK_STATE_FINWAIT1
      TCPIP_TCP_SOCK_STATE_FINWAIT2
      TCPIP_TCP_SOCK_STATE_CLOSING
      TCPIP_TCP_SOCK_STATE_TIMEWAIT
      */

      /* first: check sequence number */
      {
        boolean MsgIsAcceptable = TRUE;

        if(0 == TcpIp_Tcp_RcvWnd[TcpIdx])
        {
          if(0 == TcpIp_Tcp_SegLen)
          {
            if(TcpIp_Tcp_SegSeq != TcpIp_Tcp_RcvNxt[TcpIdx])
            {
              /* not accepted */
              MsgIsAcceptable = FALSE;
            }
          }
          else
          {
            /* not accepted */
            /* the remote host ignored my transmitted window size */
            MsgIsAcceptable = FALSE;
          }
        }
        else
        {
          if( TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_RcvNxt[TcpIdx], TcpIp_Tcp_SegSeq) ||
              TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(TcpIp_Tcp_SegSeq, (TcpIp_Tcp_RcvNxt[TcpIdx] + TcpIp_Tcp_RcvWnd[TcpIdx])) )
          {
            if(0 == TcpIp_Tcp_SegLen)
            {
              /* not accepted */
              MsgIsAcceptable = FALSE;
            }
            else
            {
              if( TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_RcvNxt[TcpIdx], (TcpIp_Tcp_SegSeq + (TcpIp_Tcp_SegLen -1))) ||
                  TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2((TcpIp_Tcp_SegSeq + (TcpIp_Tcp_SegLen -1)), (TcpIp_Tcp_RcvNxt[TcpIdx] + TcpIp_Tcp_RcvWnd[TcpIdx])) )
              {
                /* not accepted */
                MsgIsAcceptable = FALSE;
              }
            }
          }
        }

        if(FALSE == MsgIsAcceptable)
        {
          if( 0 == (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits) )
          {
            /* send ACK: <SEQ=SND.NXT><ACK=RCV.NXT><CTL=ACK> */
            TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);
          }
          /* drop message and return */
          return;
        }

      }

      /* From here on the segment is valid and accepted, and the text segment lies completely
         inside the reveiving window. Special treatment for out of order packet is done later. */

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
      TcpIp_Tcp_IdleTimeoutShort[TcpIdx] = TcpIp_Tcp_RtoValid[TcpIdx] +2;  /* reset the idle timer on each received segment */
#endif

      /* second: check the RST bit */
      if ( 0 != (TCPIP_TCP_CODE_BIT_RST & HdrPtr->CodeBits) )
      {
        /* RST flag was received */

        if( (TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[TcpIdx]) )
        {
          if(TRUE == TcpIp_Tcp_SockIsServer[TcpIdx])
          {
            /* opened passively */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* connection was reset by peer */

            /* delete/clear this TCB (user has to call Sock_Connect() again later): */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          else
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];
            /* opened actively */
            /* connection was refused -> signal to the user "connection refused" */
            TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10061);  /* "connection refused" */

            /* delete/clear this TCB (user has to call Sock_Connect() again later): */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CONN_REFUSED, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CONN_REFUSED, FALSE);
#endif
          }
          /* clear all segments from retransmission queue */
          TcpIp_Tcp_VDelTxQueue(TcpIdx);

          if(TRUE == TcpIp_Tcp_SockIsServer[TcpIdx])
          {
            /* opened passively */
            return;
          }
        }
        else if( (TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx]) ||
                 (TCPIP_TCP_SOCK_STATE_FINWAIT1    == TcpIp_Tcp_SockState[TcpIdx]) ||
                 (TCPIP_TCP_SOCK_STATE_FINWAIT2    == TcpIp_Tcp_SockState[TcpIdx]) ||
                 (TCPIP_TCP_SOCK_STATE_CLOSEWAIT   == TcpIp_Tcp_SockState[TcpIdx]) )
        {
          /* If the RST bit is set then, any outstanding RECEIVEs and SEND should receive "reset"
             responses. All segment queues should be flushed. Users should also receive an
             unsolicited general "connection reset" signal. Enter the CLOSED state, delete the TCB,
             and return. */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
          uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

          /* cancel all send and receive calls
             -> nothing to be done for AUTOSAR API */

          /* remove all elements from tx-queue */
          /* remove all elements from rx-queue */
          /* enter CLOSED state, delete/clear TCB */
          TcpIp_Tcp_VRestartSock(TcpIdx);
          TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

          /* signal a "connection reset" to the user */
          TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10054);  /* "Connection reset by peer" */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif

          /* return */
          return;
        }
        else if( (TCPIP_TCP_SOCK_STATE_CLOSING  == TcpIp_Tcp_SockState[TcpIdx]) ||
                 (TCPIP_TCP_SOCK_STATE_LASTACK  == TcpIp_Tcp_SockState[TcpIdx]) ||
                 (TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockState[TcpIdx]) )
        {
          /* enter CLOSED state, delete/clear TCB */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
          uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];
          TcpIp_Tcp_VRestartSock(TcpIdx);
          TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, UseTls);
#else
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, FALSE);
#endif

          return;
        }
        else
        {
          /* nothing to do (required by MISRA) */
        }
      }

      /* third: check security and precedence */
      /* security compartment and precedence is not supportet */

      /* fourth: check the SYN bit */
      if ( 0 != (TCPIP_TCP_CODE_BIT_SYN & HdrPtr->CodeBits) )
      {
        /* SYN flag was received */
        /*
        if( (TCPIP_TCP_SOCK_STATE_SYNRCVD     == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_FINWAIT1    == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_FINWAIT2    == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_CLOSEWAIT   == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_CLOSING     == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_LASTACK     == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_TIMEWAIT    == TcpIp_Tcp_SockState[TcpIdx]) )
        */
        {
#if (TCPIP_SUPPORT_TLS == STD_ON)
          boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
          uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];
          /* send reset <SEQ=SEG.ACK><CTL=RST> */
          TcpIp_Tcp_VSendReset(TcpIdx, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);

          /* cancel all send and receive calls
             -> nothing to be done for AUTOSAR API */

          /* remove all elements from tx-queue */
          /* remove all elements from rx-queue */
          /* enter CLOSED state, delete/clear TCB */
          TcpIp_Tcp_VRestartSock(TcpIdx);
          TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

          /* signal a "connection reset" to the user */
          TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10054);  /* "Connection reset by peer" */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          return;  /* return from RxIndication */
        }
      }

      /* fifth: check the ACK field */
      if ( 0 == (TCPIP_TCP_CODE_BIT_ACK & HdrPtr->CodeBits) )
      {
        /* no segment without ACK may arrive in this TCP state
           -> drop segment */
        return;
      }

      /* five.a: */
      if (HdrAndOptLenByte > TCPIP_TCP_HDR_LEN_BYTE)
      {
        /* there is a TCP option included in the header */
        if (E_OK != TcpIp_Tcp_VHandleTcpOptions(TcpIdx, RxSockRemAddrPtr, RxSockLocAddrPtr, &DataPtr[TCPIP_TCP_HDR_LEN_BYTE], HdrAndOptLenByte - TCPIP_TCP_HDR_LEN_BYTE, &TcpIp_Tcp_SegSeq, &TcpIp_Tcp_SegAck))
        {
          /* stop processing of this message. Sending a RST was already triggered by TcpIp_Tcp_VHandleTcpOptions() */
          return;
        }
      }

      if( (TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[TcpIdx]) )
      {
        if( TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_SegAck) &&
            TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_SndNxt[TcpIdx]) )
        {
          /* switch to state ESTABLISHED */
          TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_ESTABLISHED;
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
          /* decrease the cwnd here by 1 (for the SYN), the cwnd will be increased by this 1 in the next state some lines further down */
          TcpIp_Tcp_Cwnd[TcpIdx]--;
#endif

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
          TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
          if(TRUE == TcpIp_Tcp_RtoResetPending[TcpIdx])
          {
            TcpIp_Tcp_RtoValid[TcpIdx]        = TCPIP_TCP_TX_RETRY_INT_RESET_VALUE_CYCLES;
            TcpIp_Tcp_Rto[TcpIdx]             = TCPIP_TCP_TX_RETRY_INT_RESET_VALUE_CYCLES;
            TcpIp_Tcp_RtoResetPending[TcpIdx] = FALSE;
          }
#endif

          /* TcpConnected: */
          if(E_OK == TcpIp_VSockTcpStateChgAccepted(SockIdx, TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)]))  /* accepted by local host */
          {
            /* set send window according to received segment */
            TcpIp_Tcp_SndWnd[TcpIdx] = TcpIp_Tcp_SegWnd;
            TcpIp_Tcp_SndWl1[TcpIdx] = TcpIp_Tcp_SegSeq;
            TcpIp_Tcp_SndWl2[TcpIdx] = TcpIp_Tcp_SegAck;
          }
          else
          {
            /* connection refused by the user -> Reset the TCP connection */
            (void)TcpIp_Close(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TRUE);
            return;  /* leave the RxIndication without further processing of the received message */
          }
        }
        else
        {
          /* send reset <SEQ=SEG.ACK><CTL=RST> */
          TcpIp_Tcp_VSendReset(TcpIdx, TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId), RxSockRemAddrPtr, RxSockLocAddrPtr, TcpIp_Tcp_SegAck, 0, 0);
          return;
        }
      }

      if( (TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_FINWAIT1    == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_FINWAIT2    == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_CLOSEWAIT   == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_CLOSING     == TcpIp_Tcp_SockState[TcpIdx]) )
      {
        if( TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_SegAck) &&
            TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_SndNxt[TcpIdx]) )
        {

          if( TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_SegAck) )
          {
            /* new data is acknowlaged */
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
            TcpIp_Tcp_RetransmitTimeout[TcpIdx] = TcpIp_Tcp_RtoValid[TcpIdx];
            TcpIp_Tcp_RtoReloadValue[TcpIdx]    = TcpIp_Tcp_RtoValid[TcpIdx];
#else
            TcpIp_Tcp_RetransmitTimeout[TcpIdx] = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
            TcpIp_Tcp_RtoReloadValue[TcpIdx]    = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
#endif
          }
          else
          {
            /* no new data received, SegLen == 0 */
            if(0 == TcpIp_Tcp_SndWnd[TcpIdx])
            {
              /* Zero window probe ACK */
              uint32 ReloadValue;
              /* reset retransmission timer / cwnd idle timer */
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
              ReloadValue = TcpIp_Tcp_RtoValid[TcpIdx];
#else
              ReloadValue = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
#endif
              TcpIp_Tcp_IdleTimeoutShort[TcpIdx]   = ReloadValue +2;

              /* reset retransmit timing of zero window probe element in retry queue (if available) */
              TcpIp_Tcp_RetransmitTimeout[TcpIdx] = ReloadValue;
              TcpIp_Tcp_RtoReloadValue[TcpIdx]    = ReloadValue;
            }
          }

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
          if( TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_SegAck) )
          {
            /* new data is acknowlaged */
            uint32 Smss = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];  /* sender maximum segment size */
            uint32 SizeN = TcpIp_Tcp_SegAck - TcpIp_Tcp_SndUna[TcpIdx];

            TcpIp_Tcp_CwndSegNumLimit[TcpIdx] = 0;

            if(TCPIP_TCP_CONG_CTRL_MODE_SLOW_START == TcpIp_Tcp_CongestionControlMode[TcpIdx])
            {
              /* inc cwnd */
              CANOE_WRITE_STRING_2("TcpIp_Tcp_RxInd: TcpIdx %d: inc cwnd in slow start, SegAck is %d", TcpIdx, TcpIp_Tcp_SegAck);
              TcpIp_Tcp_VIncCwnd(TcpIdx, SizeN);
            }
            else if(TCPIP_TCP_CONG_CTRL_MODE_CONGESTION_AVOIDANCE == TcpIp_Tcp_CongestionControlMode[TcpIdx])
            {
              /* summerize incoming ACKs */
              TcpIp_Tcp_VIncAckSumN(TcpIdx, SizeN);
            }
            else   /* fast recover (fast retransmit is no 'stable' state) */
            {
              /* reset duplicate ACK counter */
              TcpIp_Tcp_DupAckCnt[TcpIdx] = 0;

              /* RFC 6582, 3.2 step 3 */
              if( TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_Recover[TcpIdx]) )
              {
                /* full ack */

                uint32 FlightSize;  /* FlightSize is the amount of outstanding data in the network */
                uint32 Max1;
                uint32 Min1;
                TcpIp_TcpTxRetrQueueIdxType EleIdx;

                /* check all retry queue elements for this socket */
                FlightSize = 0;
                EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
                while((EleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX) && (TcpIp_Tcp_TxRetrQueue[EleIdx].TransmitCounter != 0))
                {
                  FlightSize += TcpIp_Tcp_TxRetrQueue[EleIdx].SizeTotByte;
                  EleIdx      = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
                }
                /* cwnd = min (ssthresh, max(FlightSize, SMSS) + SMSS) */
                if(FlightSize > Smss)
                {
                  Max1 = FlightSize;
                }
                else
                {
                  Max1 = Smss;
                }
                if(TcpIp_Tcp_Ssthresh[TcpIdx] < (Max1 + Smss))
                {
                  Min1 = TcpIp_Tcp_Ssthresh[TcpIdx];
                }
                else
                {
                  Min1 = Max1 + Smss;
                }
                TcpIp_Tcp_Cwnd[TcpIdx] = Min1;
                CANOE_WRITE_STRING_2("TcpIp_Tcp_RxInd: TcpIdx %d: cwnd set to %d after full ack", TcpIdx, TcpIp_Tcp_Cwnd[TcpIdx]);

                /* exit fast recovery prcedure */
                TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_CONGESTION_AVOIDANCE;

              }
              else
              {
                /* partial ack */

                /* retransmit the first unacknowleged segment (fast retransmit) */
                /* -> set retransmit timeout of the first element to '0', then it will be sent in the next MainFunction */
                TcpIp_Tcp_FastRetransmitPending[TcpIdx] = TRUE;

                /* deflate cwnd */
                TcpIp_Tcp_Cwnd[TcpIdx] -= (TcpIp_Tcp_SegAck - TcpIp_Tcp_SndUna[TcpIdx]);
                if( (TcpIp_Tcp_SegAck - TcpIp_Tcp_SndUna[TcpIdx]) >= Smss )
                {
                  TcpIp_Tcp_Cwnd[TcpIdx] += Smss;
                }
                CANOE_WRITE_STRING_2("TcpIp_Tcp_RxInd: TcpIdx %d: cwnd set to %d after partial ack", TcpIdx, TcpIp_Tcp_Cwnd[TcpIdx]);

                /* do not exit fast recovery prcedure */
              }
            }
          }
          else
          {
            /* ACK with same value as in last message / no new data is ACKed */
            if(TcpIp_Tcp_SndNxt[TcpIdx] != TcpIp_Tcp_SndUna[TcpIdx])  /* data was sent, but no new data was ACKed */
            {
              /* duplicated ACK */

              /* count the ACKs */
              if(255 > TcpIp_Tcp_DupAckCnt[TcpIdx])
              {
                TcpIp_Tcp_DupAckCnt[TcpIdx]++;
              }

              switch(TcpIp_Tcp_DupAckCnt[TcpIdx])
              {
              case 1:
              case 2:
                {
                  /* send one new (previously unsent) packet with max SMSS (only a 'SHOULD' in the RFC) */
                  break;
                }
              case 3:
                {
                  /* this is the third received ACK */

                  if( TCPIP_TCP_SEQ1_BIGGER_SEQ2((TcpIp_Tcp_SegAck -1), TcpIp_Tcp_Recover[TcpIdx]) )
                  {
                    TcpIp_TcpTxRetrQueueIdxType EleIdx;
                    TcpIp_TcpTxRetrQueueIdxType LastEleIdx;

                    /* check TransmitCounter of all retry queue elements for this socket */
                    LastEleIdx = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
                    EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
                    while((EleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX) && (TcpIp_Tcp_TxRetrQueue[EleIdx].TransmitCounter != 0))
                    {
                      LastEleIdx = EleIdx;
                      EleIdx      = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
                    }
                    if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != LastEleIdx)
                    {
                      TcpIp_Tcp_Recover[TcpIdx] = (TcpIp_Tcp_TxRetrQueue[LastEleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte) -1;
                    }

                    /* enter fast retransmit (step 2 of Section 3.2 in RFC5681) */

                    /* set ssthresh */
                    TcpIp_Tcp_VSetSsthreshEqu4(TcpIdx);

                    /* retransmit the first element in the RetrQueue immediately */
                    TcpIp_Tcp_FastRetransmitPending[TcpIdx] = TRUE;

                    /* set cwnd */
                    TcpIp_Tcp_Cwnd[TcpIdx] = TcpIp_Tcp_Ssthresh[TcpIdx] + (3* TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]);

                    /* enter fast recover */
                    TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_FAST_RECOVER;
                  }
                  else
                  {
                    /* do not enter fast retransmit */
                  }

                  break;
                }
              default:
                {
                  /* 4 or more duplicated ACKs */
                  /* inc cwnd by SMSS */
                  TcpIp_Tcp_Cwnd[TcpIdx] += TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];

                  /* semding of additional available data (1*SMSS) is done in the MainFunction. No extra handling needed,
                  since the MainFunction tries to send as much data as the cwnd allows, so this update is done automatically. */
                }
              }
            }
            else
            {
              /* no data was sent, but new data was received */
              /* do nothing */
            }
          }

#endif  /* (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON) */

          TcpIp_Tcp_SndUna[TcpIdx] = TcpIp_Tcp_SegAck;

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
          TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

#if (TCPIP_TCP_FREE_ACKED_REQ_AND_RETR_ELEMENTS_IN_MAIN_FUNCTION == STD_ON)
          /* TcpIp_Tcp_VFreeAckedReqAndRetrElements() will be called in MainFunction. */
#else
          /* ------------------------------------------------------------------------------------ */
          /* (start) remove segments from tx (retransmission) queue */
          TcpIp_Tcp_VFreeAckedReqAndRetrElements(TcpIdx, TcpIp_Tcp_SegAck);
          /* (end) remove segments from tx (retransmission) queue */
          /* ------------------------------------------------------------------------------------ */
#endif

          /* adjust window */
          if( TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SndWl1[TcpIdx], TcpIp_Tcp_SegSeq) ||
              ( (TcpIp_Tcp_SndWl1[TcpIdx] == TcpIp_Tcp_SegSeq) &&
                TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2(TcpIp_Tcp_SndWl2[TcpIdx], TcpIp_Tcp_SegAck) ) )
          {
            /* set send window according to received segment */
            if (TcpIp_Tcp_SndWnd[TcpIdx] != TcpIp_Tcp_SegWnd)
            {
              if (0 == TcpIp_Tcp_SndWnd[TcpIdx])
              {
                /* This is a window update. Zero window is opened again
                 * Reset retransmission timeout of zero window probes.
                 */
                TcpIp_Tcp_RetransmitTimeout[TcpIdx] = 0;
              }

              TcpIp_Tcp_SndWnd[TcpIdx] = TcpIp_Tcp_SegWnd;
            }

            TcpIp_Tcp_SndWl1[TcpIdx] = TcpIp_Tcp_SegSeq;
            TcpIp_Tcp_SndWl2[TcpIdx] = TcpIp_Tcp_SegAck;
          }
        }
        else if(TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_SegAck))
        {
          /* ignore this ACK */
          if((0 == TcpIp_Tcp_SndWnd[TcpIdx]) && (TcpIp_Tcp_SndUna[TcpIdx] == (TcpIp_Tcp_SegAck +1)))
          {
            /* send window is closed, and one single byte is in the send-queue */
            /* this is an answer on a zero-window-probe */

            /* reset user timeout */
#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
            TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].UserTimeoutTs     = TcpIp_Tcp_GlobalTimestamp + TcpIp_Tcp_Uto[TcpIdx];
#else
            TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].UserTimeoutTs     = TcpIp_Tcp_GlobalTimestamp + TcpIp_Tcp_VCfgGetUserTimeoutDefCycles();
#endif
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
            TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif
            /* reset retransmission timeout */
            {
              uint32 NewTimeout;
              TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].TransmitCounter = 1;
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
              NewTimeout = TcpIp_Tcp_RtoValid[TcpIdx];
#else
              NewTimeout = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
#endif
              TcpIp_Tcp_RetransmitTimeout[TcpIdx] = NewTimeout;
              TcpIp_Tcp_RtoReloadValue[TcpIdx]    = NewTimeout;
            }
          }
        }
        else /* else if(TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SegAck, TcpIp_Tcp_SndNxt[TcpIdx])) */
        {
          /* send ACK */
          TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);

          /* drop segment and return */
          return;
        }

        if(TCPIP_TCP_SOCK_STATE_FINWAIT1 == TcpIp_Tcp_SockState[TcpIdx])
        {
          if (((TcpIp_Tcp_TxFlags[TcpIdx] & TCPIP_TCP_CODE_BIT_FIN) == 0) && (TcpIp_Tcp_SndNxt[TcpIdx] == TcpIp_Tcp_SegAck))
          {
            /* our FIN is now acknowledged -> switch to state FINWAIT2 */
            TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_FINWAIT2;
            TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
          }
        }

        if(TCPIP_TCP_SOCK_STATE_FINWAIT2 == TcpIp_Tcp_SockState[TcpIdx])
        {
          /* the retransmission queue is empty now */

          /* acknowlage the user's close request ("ok") */
        }
        else if(TCPIP_TCP_SOCK_STATE_CLOSEWAIT == TcpIp_Tcp_SockState[TcpIdx])
        {
          /* same as in state ESTABLISHED, nothing extra to do */
        }
        else if(TCPIP_TCP_SOCK_STATE_CLOSING == TcpIp_Tcp_SockState[TcpIdx])
        {
          if (((TcpIp_Tcp_TxFlags[TcpIdx] & TCPIP_TCP_CODE_BIT_FIN) == 0) && (TcpIp_Tcp_SndNxt[TcpIdx] == TcpIp_Tcp_SegAck))
          {
            /* our FIN is now acknowledged -> switch to state TIMEWAIT */
            TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_TIMEWAIT;
            TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_CLOSED;

            TcpIp_Tcp_MslTimeout[TcpIdx]    = TcpIp_Tcp_VCfgGet2MslTimeoutCycles();

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
            TcpIp_Tcp_VKeepAliveReset(TcpIdx);
#endif

            /* clear retransmission queue */
            TcpIp_Tcp_VClearRetrQueue(TcpIdx);
          }
          else
          {
            /* ignore the segment */
          }
        }
        else
        {
          /* nothing to do (required by MISRA) */
        }
      }
      else if(TCPIP_TCP_SOCK_STATE_LASTACK == TcpIp_Tcp_SockState[TcpIdx])
      {
        if (((TcpIp_Tcp_TxFlags[TcpIdx] & TCPIP_TCP_CODE_BIT_FIN) == 0) && (TcpIp_Tcp_SndNxt[TcpIdx] == TcpIp_Tcp_SegAck))
        {
          /* our FIN is now acknowledged */
#if (TCPIP_SUPPORT_TLS == STD_ON)
          boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
          uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

          /* check if we sent some data earlier and if now this data is acked -> TxConfirmation */

#if (TCPIP_TCP_FREE_ACKED_REQ_AND_RETR_ELEMENTS_IN_MAIN_FUNCTION == STD_ON)
          /* TcpIp_Tcp_VFreeAckedReqAndRetrElements() will be called in MainFunction. */
#else
          /* ------------------------------------------------------------------------------------ */
          /* (start) remove segments from tx (retransmissino) queue */
          TcpIp_Tcp_VFreeAckedReqAndRetrElements(TcpIdx, TcpIp_Tcp_SegAck);
          /* (end) remove segments from tx (retransmission) queue */
          /* ------------------------------------------------------------------------------------ */
#endif

          /* All data sent is acked -> cancel idle timeout */
          /* switch to state CLOSED, delete/clear TCB element, and return */
          TcpIp_Tcp_VRestartSock(TcpIdx);
          TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
#if (TCPIP_SUPPORT_TLS == STD_ON)
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, UseTls);
#else
          TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, FALSE);
#endif

          return;
        }
        else
        {
          if(0 == (TCPIP_TCP_CODE_BIT_URG & HdrPtr->CodeBits))
          {
            /* send ACK */
            TcpIp_Tcp_VSendFlags(TcpIdx, (TCPIP_TCP_CODE_BIT_ACK));
          }
          else
          {
            /* only the very last ACK is expected, do not respond to frames with not supported 'urgent' data */
          }
          return;
        }
      }
      else if(TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockState[TcpIdx])
      {
        if (((TcpIp_Tcp_TxFlags[TcpIdx] & TCPIP_TCP_CODE_BIT_FIN) == 0) && (TcpIp_Tcp_SndNxt[TcpIdx] == TcpIp_Tcp_SegAck))
        {
          /* our FIN is now acknowledged (again, this is a duplicate) */
          /* sending ACK is already triggered earlier */

          /* reset MSL Timeout */
          TcpIp_Tcp_MslTimeout[TcpIdx] = TcpIp_Tcp_VCfgGet2MslTimeoutCycles();
        }
        else
        {
          if(0 == (TCPIP_TCP_CODE_BIT_URG & HdrPtr->CodeBits))
          {
            /* send ACK */
            TcpIp_Tcp_VSendFlags(TcpIdx, (TCPIP_TCP_CODE_BIT_ACK));
          }
          else
          {
            /* only the very last ACK is expected, do not respond to frames with not supported 'urgent' data */
          }
          return;
        }
      }
      else
      {
        /* nothing to do (required by MISRA) */
      }

      /* sixth: check the URG bit */
      if( (0 != (TCPIP_TCP_CODE_BIT_URG & HdrPtr->CodeBits)) && (0 != HdrPtr->UrgentPtr) )
      {
        /* usage of urgend pointer/data is not supported by this implementation */

        /* drop segment */
        return;
      }

      /* seventh: process the segment text */
      if( (TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_FINWAIT1    == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_FINWAIT2    == TcpIp_Tcp_SockState[TcpIdx]) )
      {
        /* Once the TCP takes responsibility for the data it advances RCV.NXT over the data
           accepted, and adjusts RCV.WND as apporopriate to the current buffer availability. The
           total of RCV.NXT and RCV.WND should not be reduced. */

        if(TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SegSeq, TcpIp_Tcp_RcvNxt[TcpIdx]))
        {
          /* The segment starts before RcvNxt and ends after RcvNxt. Cut off first part of the
             segment, set new start to RcvNxt. */
          uint16 LenDiff = (uint16)(TcpIp_Tcp_RcvNxt[TcpIdx] - TcpIp_Tcp_SegSeq);

          TcpIp_Tcp_SegLen = TcpIp_Tcp_SegLen - LenDiff;
          TcpIp_Tcp_SegSeq = TcpIp_Tcp_RcvNxt[TcpIdx];
          TcpPayloadPtr    = &(((uint8 *)TcpPayloadPtr)[LenDiff]);
        }

        if(TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(TcpIp_Tcp_SegSeq, TcpIp_Tcp_RcvNxt[TcpIdx]))
        {
          if(TcpIp_Tcp_SegLen != 0)
          {
            if(TcpIp_Tcp_SegSeq == TcpIp_Tcp_RcvNxt[TcpIdx])
            {
              /* copy payload into rx buffer: ------- */
              if( TcpIp_Tcp_VCfgGetMaxRxMsgSizeByte() >= TcpIp_Tcp_SegLen )
              {
                if((TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferFillLevel[TcpIdx]) >= TcpIp_Tcp_SegLen)
                {
                  /* data fits into rx buffer */

                  if((TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_SegLen) < TcpIp_Tcp_RxBufferWritePos[TcpIdx])
                  {
                    /* copy data with taking care of buffer wrap-around (two blocks) */
                    TcpIp_DataType *LocBufPtr;
                    uint16 CopyLenByte;

                    /* copy bytes 0..((TCPIP_TCP_MAX_WINDOW_SIZE - TcpIp_Tcp_RxBufferWritePos[TcpIdx])-1) */
                    LocBufPtr = (TcpIp_DataType *) (TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, TcpIp_Tcp_RxBufferWritePos[TcpIdx]));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                    CopyLenByte = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferWritePos[TcpIdx];
                    IpBase_Copy( LocBufPtr, (const IpBase_CopyDataType *)TcpPayloadPtr, CopyLenByte );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

                    /* copy bytes (TCPIP_TCP_MAX_WINDOW_SIZE - TcpIp_Tcp_RxBufferWritePos[TcpIdx])..(TcpIp_Tcp_SegLen -1) */
                    LocBufPtr = (TcpIp_DataType *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, 0));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                    IpBase_Copy( LocBufPtr,  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                      (const IpBase_CopyDataType *)(&((uint8 *)(TcpPayloadPtr))[CopyLenByte]),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                      (TcpIp_Tcp_SegLen - CopyLenByte) );

                    TcpIp_Tcp_RxBufferWritePos[TcpIdx] = TcpIp_Tcp_SegLen - CopyLenByte;

                    TcpIp_Tcp_RxBufferFillLevel[TcpIdx] += TcpIp_Tcp_SegLen;
                    TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] += TcpIp_Tcp_SegLen;
                    IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].RemSock)), (IpBase_CopyDataType *)(RxSockRemAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                    IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].LocSock)), (IpBase_CopyDataType *)(RxSockLocAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                    TcpIp_Tcp_RcvWnd[TcpIdx] = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferFillLevel[TcpIdx];
                  }
                  else
                  {
                    TcpIp_DataType *LocBufPtr;
                    /* copy bytes 0..(TcpIp_Tcp_SegLen -1) */
                    LocBufPtr = (TcpIp_DataType *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, TcpIp_Tcp_RxBufferWritePos[TcpIdx]));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                    IpBase_Copy(LocBufPtr, (const IpBase_CopyDataType *)TcpPayloadPtr, TcpIp_Tcp_SegLen);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

                    TcpIp_Tcp_RxBufferWritePos[TcpIdx] += TcpIp_Tcp_SegLen;
                    if(TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] == TcpIp_Tcp_RxBufferWritePos[TcpIdx])
                    {
                      TcpIp_Tcp_RxBufferWritePos[TcpIdx] = 0;
                    }

                    TcpIp_Tcp_RxBufferFillLevel[TcpIdx] += TcpIp_Tcp_SegLen;
                    TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] += TcpIp_Tcp_SegLen;
                    IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].RemSock)), (IpBase_CopyDataType *)(RxSockRemAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                    IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].LocSock)), (IpBase_CopyDataType *)(RxSockLocAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                    TcpIp_Tcp_RcvWnd[TcpIdx] = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferFillLevel[TcpIdx];
                  }

                  /* Condition: This is the next consecutive message, out of order messages are not supported, yet. */
                  TcpIp_Tcp_RcvNxt[TcpIdx] = TcpIp_Tcp_SegSeq + TcpIp_Tcp_SegLen;

                  /* Send ACK: <SEQ=SND.NXT><ACK=RCV.NXT><CTL=ACK> */
                  TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);

                  /* now check if elements from the PreBufArray follow this segment directly */
#if (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON)
                  if(0 != TcpIp_Tcp_OooNumElements[TcpIdx])
                  {
                    /* only the first element in the ooo list migth match (all following have missing segments in between) */
                    if(TcpIp_Tcp_RcvNxt[TcpIdx] == TcpIp_Tcp_RxPreBufArray[TcpIp_Tcp_OooFirstEleIdx[TcpIdx]].SeqNo)
                    {
                      /* this element fits -> trigger RxIndication */

                      TcpIp_Tcp_SegLen = TcpIp_Tcp_RxPreBufArray[TcpIp_Tcp_OooFirstEleIdx[TcpIdx]].LenByte;

                      /* increase buffer position, taking care of the wraparound */
                      TCPIP_ASSERT(TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] < 0x0000FFFFu);
                      TcpIp_Tcp_RxBufferWritePos[TcpIdx] = TcpIp_AddWithWrapAround16(TcpIp_Tcp_RxBufferWritePos[TcpIdx], TcpIp_Tcp_SegLen, (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx]);

                      TcpIp_Tcp_RxBufferFillLevel[TcpIdx] += TcpIp_Tcp_SegLen;
                      TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] += TcpIp_Tcp_SegLen;
                      TcpIp_Tcp_RcvWnd[TcpIdx] = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferFillLevel[TcpIdx];
                      IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].RemSock)), (IpBase_CopyDataType *)(RxSockRemAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                      IpBase_Copy((IpBase_CopyDataType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].LocSock)), (IpBase_CopyDataType *)(RxSockLocAddrPtr), sizeof(TcpIp_SockAddrInXType));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */

                      /* Update RcvNxt (for ACK) -> no extra trigger, ACK is already triggered */
                      TcpIp_Tcp_RcvNxt[TcpIdx] += TcpIp_Tcp_SegLen;

                      /* free the element */
                      {
                        uint16 FreeEleIdx = TcpIp_Tcp_OooFirstEleIdx[TcpIdx];
                        if(1 != TcpIp_Tcp_OooNumElements[TcpIdx])
                        {
                          TcpIp_Tcp_OooFirstEleIdx[TcpIdx] = TcpIp_Tcp_RxPreBufArray[TcpIp_Tcp_OooFirstEleIdx[TcpIdx]].NextEleIdx;
                        }
                        else
                        {
                          TcpIp_Tcp_OooFirstEleIdx[TcpIdx] = 0xFFFF;  /* invalid */
                        }
                        TcpIp_Tcp_RxPreBufArray[FreeEleIdx].NextEleIdx = TcpIp_Tcp_OooFirstFreeEleIdx;
                        TcpIp_Tcp_OooFirstFreeEleIdx = FreeEleIdx;
                        TcpIp_Tcp_OooNumElements[TcpIdx]--;
                        TcpIp_Tcp_OooNumFreeElements++;
                      }
                    }
                  }
#endif  /* (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON) */
                }
                else
                {
                  /* data doesn't fit into rx buffer */
                  /* Discard message (maybe partly). Don't evaluate FIN flag! */
                  return;
                }
              }
              else
              {
                /* no data, or segmemt size larger than allowed */
              }
            }
            else
            {
              /* out of order message, previous message is missing */

#if (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON)
              /* queue this message if possible (enough memory in buffer, control element available) */

              /*
              check if there are already elements in the queue.
              - if true, check if new segment can be combined with existing one (left or right side).
              ....If it could be combined, check if two elements can be merged to one, if yes do it and remove unneeded element by moving following elements forward.
              - if false, add segment to list (this will be the first element in the list)
              */

              boolean SegCouldBeStored = FALSE;

              /* check if ooo list already contains elements */
              if(TcpIp_Tcp_OooNumElements[TcpIdx] != 0)
              {
                boolean CombinationIsPossible = FALSE;
                uint16  CurrentOooEleIdx;
                uint8   NumCheckedElements = 0;
                /* check if new segment can be combined with existing one (left or right side) */

                CurrentOooEleIdx = TcpIp_Tcp_OooFirstEleIdx[TcpIdx];
                do
                {
                  /* check for combination possibility */
                  if((TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].SeqNo + TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].LenByte) == TcpIp_Tcp_SegSeq)
                  {
                    /* match found, segment can be appended to existing element */
                    uint16 NextEleIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
                    TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].LenByte += TcpIp_Tcp_SegLen;

                    NextEleIdx = TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].NextEleIdx;
                    if( (TcpIp_Tcp_OooNumElements[TcpIdx] != (NumCheckedElements +1)) /* there is another ooo element in the queue */ &&
                      ((TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].SeqNo + TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].LenByte)
                      == TcpIp_Tcp_RxPreBufArray[NextEleIdx].SeqNo) )
                    {
                      /* current ooo element has 0 distance to the following ooo element */
                      /* combine the two elements an delete the second one */
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
                      TcpIp_Tcp_OooLastChangedEleIdx[TcpIdx] = CurrentOooEleIdx;
#endif
                      TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].LenByte   += TcpIp_Tcp_RxPreBufArray[NextEleIdx].LenByte;
                      TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].NextEleIdx = TcpIp_Tcp_RxPreBufArray[NextEleIdx].NextEleIdx;

                      /* prepend the now free element to the free elements list */
                      TcpIp_Tcp_OooNumElements[TcpIdx]--;
                      TcpIp_Tcp_RxPreBufArray[NextEleIdx].NextEleIdx = TcpIp_Tcp_OooFirstFreeEleIdx;
                      TcpIp_Tcp_OooFirstFreeEleIdx = NextEleIdx;
                      TcpIp_Tcp_OooNumFreeElements++;
                    }

                    CombinationIsPossible = TRUE;
                    SegCouldBeStored = TRUE;
                    break;  /* stop search for insert position */
                  }
                  else if((TcpIp_Tcp_SegSeq + TcpIp_Tcp_SegLen) == TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].SeqNo)
                  {
                    /* match found, segment can be prepended to existing element */
                    /* the segment won't get in contact with the previous one in the ooo list, because then it would have matched in an earlier check */
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
                    TcpIp_Tcp_OooLastChangedEleIdx[TcpIdx] = CurrentOooEleIdx;
#endif
                    TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].SeqNo    = TcpIp_Tcp_SegSeq;
                    TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].LenByte += TcpIp_Tcp_SegLen;

                    CombinationIsPossible = TRUE;
                    SegCouldBeStored = TRUE;
                    break;  /* stop search for insert position */  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_a */
                  }
                  else
                  {
                    /* received element can not be combined with current ooo element */
                  }

                  /* go to next element */
                  CurrentOooEleIdx = TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].NextEleIdx;
                  NumCheckedElements++;
                } while(TcpIp_Tcp_OooNumElements[TcpIdx] != NumCheckedElements);

                if(TRUE != CombinationIsPossible)
                {
                  /* the segment could not be combined with an existing one, so an extra element is needed */
                  if(0 != TcpIp_Tcp_OooNumFreeElements)
                  {
                    /* take a new element */
                    uint16 NewEleIdx = TcpIp_Tcp_OooFirstFreeEleIdx;
                    uint16 LastValidEleIdx = 0;  /* Init this variable 'LastValidEleIdx' to prevent MISRA checkers to issue the warning message 3353. The variable will always be set to a valid value before it is used */
                    TcpIp_Tcp_OooNumFreeElements--;
                    if(0!= TcpIp_Tcp_OooNumFreeElements)
                    {
                      TcpIp_Tcp_OooFirstFreeEleIdx = TcpIp_Tcp_RxPreBufArray[NewEleIdx].NextEleIdx;
                    }
                    TcpIp_Tcp_RxPreBufArray[NewEleIdx].SeqNo   = TcpIp_Tcp_SegSeq;
                    TcpIp_Tcp_RxPreBufArray[NewEleIdx].LenByte = TcpIp_Tcp_SegLen;

                    /* insert the new element into the existing list for the socket (in order!) */
                    CurrentOooEleIdx = TcpIp_Tcp_OooFirstEleIdx[TcpIdx];
                    NumCheckedElements = 0;
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
                    TcpIp_Tcp_OooLastChangedEleIdx[TcpIdx] = NewEleIdx;
#endif
                    while((TcpIp_Tcp_OooNumElements[TcpIdx] != NumCheckedElements) && (TCPIP_TCP_SEQ1_BIGGER_SEQ2(TcpIp_Tcp_SegSeq, TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].SeqNo)))
                    {
                      LastValidEleIdx = CurrentOooEleIdx;
                      CurrentOooEleIdx = TcpIp_Tcp_RxPreBufArray[CurrentOooEleIdx].NextEleIdx;
                      NumCheckedElements++;
                    }

                    if(TcpIp_Tcp_OooNumElements[TcpIdx] != NumCheckedElements)
                    {
                      /* insert new element */
                      if(0 == NumCheckedElements)
                      {
                        /* prepend the element to the list */
                        TcpIp_Tcp_RxPreBufArray[NewEleIdx      ].NextEleIdx = CurrentOooEleIdx;
                        TcpIp_Tcp_OooFirstEleIdx[TcpIdx] = NewEleIdx;
                      }
                      else
                      {
                        /* insert the element somewhere inside the list */
                        TcpIp_Tcp_RxPreBufArray[NewEleIdx      ].NextEleIdx = CurrentOooEleIdx;
                        TcpIp_Tcp_RxPreBufArray[LastValidEleIdx].NextEleIdx = NewEleIdx;  /* PRQA S 3353 */ /* MD_TCPIP_TCP_9.1_3353_a */ /* variable is always initialized ! */
                      }
                      TcpIp_Tcp_OooNumElements[TcpIdx]++;
                    }
                    else
                    {
                      /* append new element */
                      TcpIp_Tcp_RxPreBufArray[LastValidEleIdx].NextEleIdx = NewEleIdx;
                      TcpIp_Tcp_OooNumElements[TcpIdx]++;
                    }

                    SegCouldBeStored = TRUE;
                  }
                  else
                  {
                    /* no free element available, so just drop the received segment */
                    /* do nothing */
                  }
                }
              }
              else
              {
                /* the ooo list for this socket is empty, so insert a new one */
                uint16 NewEleIdx;
                if(0 != TcpIp_Tcp_OooNumFreeElements)
                {
                  NewEleIdx = TcpIp_Tcp_OooFirstFreeEleIdx;
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
                  TcpIp_Tcp_OooLastChangedEleIdx[TcpIdx] = NewEleIdx;
#endif
                  TcpIp_Tcp_OooFirstFreeEleIdx = TcpIp_Tcp_RxPreBufArray[NewEleIdx].NextEleIdx;
                  TcpIp_Tcp_OooFirstEleIdx[TcpIdx] = NewEleIdx;
                  TcpIp_Tcp_RxPreBufArray[NewEleIdx].SeqNo   = TcpIp_Tcp_SegSeq;
                  TcpIp_Tcp_RxPreBufArray[NewEleIdx].LenByte = TcpIp_Tcp_SegLen;
                  TcpIp_Tcp_OooNumElements[TcpIdx]++;
                  TcpIp_Tcp_OooNumFreeElements--;
                  SegCouldBeStored = TRUE;
                }
              }

              if(TRUE == SegCouldBeStored)
              {
                /* copy data to buffer */
                uint16 WritePos;

                {
                  /* increase buffer position, taking care of the wraparound */
                  WritePos = (uint16)(((uint32)TcpIp_Tcp_RxBufferWritePos[TcpIdx] + (TcpIp_Tcp_SegSeq - TcpIp_Tcp_RcvNxt[TcpIdx])) % (TcpIp_Tcp_AssignedRxBufferSize[TcpIdx]));
                }

                if(((uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_SegLen) < WritePos)
                {
                  /* copy data with taking care of buffer wraparound (two blocks) */
                  TcpIp_DataType *LocBufPtr;
                  uint16 CopyLenByte;

                  /* copy bytes 0..((TCPIP_TCP_MAX_WINDOW_SIZE - WritePos)-1) */
                  LocBufPtr = (TcpIp_DataType *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, WritePos));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                  CopyLenByte = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - WritePos;
                  IpBase_Copy( LocBufPtr, (const IpBase_CopyDataType *)TcpPayloadPtr, CopyLenByte );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

                  /* copy bytes (TCPIP_TCP_MAX_WINDOW_SIZE - WritePos)..(TcpIp_Tcp_SegLen -1) */
                  LocBufPtr = (TcpIp_DataType *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, 0));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                  IpBase_Copy( LocBufPtr,  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                    (const IpBase_CopyDataType *)(&((uint8 *)(TcpPayloadPtr))[CopyLenByte]),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
                    (TcpIp_Tcp_SegLen - CopyLenByte) );
                }
                else
                {
                  /* copy data (one block) */
                  TcpIp_DataType *LocBufPtr;
                  /* copy bytes 0..(TcpIp_Tcp_SegLen -1) */
                  LocBufPtr = (TcpIp_DataType *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, WritePos));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                  IpBase_Copy(LocBufPtr, (const IpBase_CopyDataType *)TcpPayloadPtr, TcpIp_Tcp_SegLen);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
                }

                /* trigger sending of ACK (eventually including SACK) */
                TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);
              }

#endif  /* (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON) */
            }
          }
        }
      }
      else if( (TCPIP_TCP_SOCK_STATE_CLOSEWAIT == TcpIp_Tcp_SockState[TcpIdx]) ||
               (TCPIP_TCP_SOCK_STATE_CLOSING   == TcpIp_Tcp_SockState[TcpIdx]) ||
               (TCPIP_TCP_SOCK_STATE_LASTACK   == TcpIp_Tcp_SockState[TcpIdx]) ||
               (TCPIP_TCP_SOCK_STATE_TIMEWAIT  == TcpIp_Tcp_SockState[TcpIdx]) )
      {
        /* ignore the segment text */
      }
      else
      {
        /* nothing to do (required by MISRA) */
      }

      /* eigth: check the FIN bit */
      if ( 0 != (TCPIP_TCP_CODE_BIT_FIN & HdrPtr->CodeBits) )
      {
        if( (TCPIP_TCP_SOCK_STATE_CLOSED == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_LISTEN   == TcpIp_Tcp_SockState[TcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_SYNSENT  == TcpIp_Tcp_SockState[TcpIdx]) )
        {
          /* do nothing */
        }
        else
        {
          /* cancel all send and receive calls
             -> nothing to be done for AUTOSAR API */

          /* RCV.NXT = FIN +1 */
          TcpIp_Tcp_RcvNxt[TcpIdx] = TcpIp_Tcp_SegSeq + TcpIp_Tcp_SegLen + 1;

          /* send ACK */
          TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);

          if( (TCPIP_TCP_SOCK_STATE_SYNRCVD     == TcpIp_Tcp_SockState[TcpIdx]) ||
              (TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx]) )
          {
            /* switch to state CLOSEWAIT */
            TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_CLOSEWAIT;
            TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_CLOSEWAIT;

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
            TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

            if(TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] == 0)
            {
              /* signal "FIN received" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_FIN_REC, TcpIp_Tcp_UseTls[TcpIdx]);
#else
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_FIN_REC, FALSE);
#endif
            }
            else
            {
              /* store event for later forwarding to the user */
              TcpIp_Tcp_EventIndicationPending[TcpIdx] = IPBASE_TCP_EVENT_FIN_RECEIVED;
            }
          }
          else if(TCPIP_TCP_SOCK_STATE_FINWAIT1 == TcpIp_Tcp_SockState[TcpIdx])
          {
            /* if our FIN has been ACKed (perhaps in this segment) */
            if(TcpIp_Tcp_SegAck == TcpIp_Tcp_SndNxt[TcpIdx])
            {
              /* switch to state TCPIP_TCP_SOCK_STATE_TIMEWAIT */
              TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_TIMEWAIT;
              TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_TIMEWAIT; /* only for sending ACK */

              /* start time-wait timer (MSL), and turn off the other timers */
              TcpIp_Tcp_MslTimeout[TcpIdx]    = TcpIp_Tcp_VCfgGet2MslTimeoutCycles();
              
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
              TcpIp_Tcp_VKeepAliveReset(TcpIdx);
#endif
            }
            else
            {
              /* switch to state CLOSING */
              TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_CLOSING;
              TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_CLOSING; /* only for sending ACK */
            }

            if(TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] == 0)
            {
              /* signal "connection closing" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, TcpIp_Tcp_UseTls[TcpIdx]);
#else
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, FALSE);
#endif
            }
            else
            {
              /* store event for later forwarding to the user */
              TcpIp_Tcp_EventIndicationPending[TcpIdx] = IPBASE_TCP_EVENT_FIN_RECEIVED;
            }
          }
          else if(TCPIP_TCP_SOCK_STATE_FINWAIT2 == TcpIp_Tcp_SockState[TcpIdx])
          {
            /* switch to state TCPIP_TCP_SOCK_STATE_TIMEWAIT */
            TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_TIMEWAIT;
            TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_TIMEWAIT; /* only for sending ACK */

            /* start time-wait timer (MSL), and turn off the other timers */
            TcpIp_Tcp_MslTimeout[TcpIdx]    = TcpIp_Tcp_VCfgGet2MslTimeoutCycles();
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
            TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

            if(TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] == 0)
            {
              /* signal "connection closing" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, TcpIp_Tcp_UseTls[TcpIdx]);
#else
              TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, FALSE);
#endif
            }
            else
            {
              /* store event for later forwarding to the user */
              TcpIp_Tcp_EventIndicationPending[TcpIdx] = IPBASE_TCP_EVENT_FIN_RECEIVED;
            }
          }
          else if( (TCPIP_TCP_SOCK_STATE_CLOSEWAIT == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_CLOSING == TcpIp_Tcp_SockState[TcpIdx]) ||
            (TCPIP_TCP_SOCK_STATE_LASTACK == TcpIp_Tcp_SockState[TcpIdx]) )
          {
            /* remain in current state */
          }
          else if(TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockState[TcpIdx])
          {
            /* remain in current state */

            /* restart time-wait timer (MSL) */
            TcpIp_Tcp_MslTimeout[TcpIdx] = TcpIp_Tcp_VCfgGet2MslTimeoutCycles();
          }
          else
          {
            /* nothing to do (required by MISRA) */
          }
        }
      }
    } /* end checking sock states */

}  /* end of RxIndication */  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

/***********************************************************************************************************************
 *  TcpIp_Tcp_Received
 **********************************************************************************************************************/
/*! \brief      mark part of buffer as read
 *  \param[in]  TcpIdx                tcp socket index
 *  \param[in]  DataLenByte           length of received data in bytes
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Received(
    TcpIp_TcpIdxType TcpIdx,
    uint32 DataLenByte )
{
  uint16 LocDataLen;
  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT, TCPIP_TCP_API_ID_RECEIVED, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_RECEIVED, TCPIP_E_INV_SOCK_ID )
  /* ------------------------------------------------- */

  if(TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] == 0)
  {
    /* This function should not be called for connections where no buffer is assigned */
    /* nothing to do, just return */
  }
  else
  {
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if((uint16)DataLenByte <= TcpIp_Tcp_RxBufferFillLevel[TcpIdx])
    {
      LocDataLen = (uint16)DataLenByte;
    }
    else
    {
      /* error, appl confirms more data than it has received */
      LocDataLen = TcpIp_Tcp_RxBufferFillLevel[TcpIdx];
    }
    TcpIp_Tcp_RxBufferFillLevel[TcpIdx] -= LocDataLen;
    TcpIp_Tcp_RxBufferReadPos[TcpIdx] = (TcpIp_Tcp_RxBufferReadPos[TcpIdx] + LocDataLen) % ((uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx]);

    TcpIp_Tcp_RcvWnd[TcpIdx] += LocDataLen;
    TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_ACK);

    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_CbkPathMtuChg
 **********************************************************************************************************************/
/*! \brief      The Path-MTU has changed for a certain destination address
 *  \param[in]  SockAddrPtr           net address that changed its state
 *  \param[in]  PathMtuSize           new size of the Path-MTU (transport layer part)
 *  \context    interrupt or task level
 **********************************************************************************************************************/
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_CbkPathMtuChg(
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr,
  uint16 PathMtuSize )
{
#if (TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE == STD_ON)

  TcpIp_TcpIdxType TcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* min size is 68 - IP-Header size */
  if(PathMtuSize < TCPIP_TCP_MIN_PATH_MTU)
  {
    /* if MTU is smaller than allowed, set to min value */
    PathMtuSize = TCPIP_TCP_MIN_PATH_MTU;
  }

  /* Go through all TCP connections an check if they use the given comunication partner address. If the address is used,
     the PathMtu value has to be adjusted, and queued send elements have to be resized if they are too big. */

  /* go through all TCP connections */
  for(TcpIdx = 0; TcpIdx < (TcpIp_VCfgGetMaxNumTcpSocks()); TcpIdx++)
  {
    uint16 NewMaxSegSize = (uint16)(PathMtuSize - TCPIP_TCP_HDR_LEN_BYTE - TcpIp_Tcp_TxOptLen[TcpIdx]);

    if( /* list of some checks: */
        /* check if socket is in an active state */
        (TCPIP_TCP_SOCK_STATE_LISTEN < TcpIp_Tcp_SockState[TcpIdx]) &&  /* PRQA S 3415 */ /* MD_TCPIP_12.4_3415 */
        (TCPIP_TCP_SOCK_STATE_CLOSED != TcpIp_Tcp_SockState[TcpIdx]) &&
        /* compare IP address */
        (TRUE == TcpIp_VSockIpAddrIsEqual(((const IpBase_SockAddrType *)&TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].RemSock), (SockAddrPtr))) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    {

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
      /* check if the new PathMtu is smaller than the currently used one */
      if( NewMaxSegSize < TcpIp_Tcp_TxMaxSegLenByte[TcpIdx])
      {
        /* New PathMtu is smaller than the old one */
        /* -> cwnd is reduced by the ratio of old and new PathMTU */
        /* take care of integer algorithmic !! */
        TcpIp_Tcp_Cwnd[TcpIdx] = (TcpIp_Tcp_Cwnd[TcpIdx] * NewMaxSegSize) / TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
      }
#endif

      /* socket is in a relevant state, so eventually the segment size has to be adapted */
      if( /* check if the new PathMtu is smaller than the currently used one */
        (NewMaxSegSize < TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]) &&
        /* check if there is any element in the RetrQueue for this connection */
        (TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
        )
      {
        /* now resize elements in TxRetrQueue if they are too big for the new Path-MTU. re-calculation of the TCP checksum is not necessary at all */

        /* for all elements in RetrQueue */
        /* if RetrQueue is full but elements have to be split, elements have to be removed from the (end of the) RetrQueue !! */

        /* check if TxLen of unqueued data is bigger than new PathMtu, eventually create new RetrQueue element and fill it. */

        TcpIp_TcpTxRetrQueueIdxType LocRetrQueueIdx;
        LocRetrQueueIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
        do
        {
          if(TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte > NewMaxSegSize)
          {
            /* resize necessary */
            uint8 NumAdditionalEle;  /* number of additional RetrQueue elements needed for splitting the current element */
            NumAdditionalEle = (uint8)(TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte / NewMaxSegSize);
            if(0 == (TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte % NewMaxSegSize))
            {
              /* the original element can exactly be devided in some new max size segments */
              NumAdditionalEle--;
            }
            /* now we know how many additional RetrQueue elements are necessary to split the current one */

            if(NumAdditionalEle <= TcpIp_Tcp_NumFreeRetrQueueEle)
            {
              /* enough free elements availabe */

              /* insert new element(s) in the socket's RetrQueue [F2] -------------------- */
              TcpIp_Tcp_VInsElesInRetrQueue(TcpIdx, LocRetrQueueIdx, NumAdditionalEle);

              {
                /* start code block A ----------------------------------------------------------------------------------- */
                TcpIp_TcpTxRetrQueueIdxType CurrentRetrQueueIdx = LocRetrQueueIdx;
                uint32 SizeTot = TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte;
                uint32 RemSize = SizeTot;
                /* reduce size of the current RetrQueue element -------------------- */
                TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte = NewMaxSegSize;
                RemSize -= NewMaxSegSize;
                TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter = 0;  /* reset transmit counter */
                /* fill the new elements with parts of the current RetrQueue element -------------------- */
                while(0 != RemSize)
                {
                  uint32 EleSize;
                  TcpIp_TcpTxRetrQueueIdxType LastEleIdx;
                  LastEleIdx = CurrentRetrQueueIdx;
                  CurrentRetrQueueIdx = TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].EleNext;
                  if(RemSize > NewMaxSegSize)
                  {
                    EleSize = NewMaxSegSize;
                  }
                  else
                  {
                    EleSize = RemSize;
                  }
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx   = TcpIp_Tcp_TxRetrQueue[LastEleIdx].DataBufStartIdx; /* copy idx, then inc it */
                  TcpIp_IncTxBufIdx(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx, TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte);
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte       = EleSize;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SeqNo             = TcpIp_Tcp_TxRetrQueue[LastEleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].UserTimeoutTs     = TcpIp_Tcp_TxRetrQueue[LastEleIdx].UserTimeoutTs;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TimeStamp         = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TimeStamp;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter   = 0;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].Flags             = (uint8)(TcpIp_Tcp_TxRetrQueue[LastEleIdx].Flags & (~(TCPIP_TCP_CODE_BIT_SYN))); /* without SYN flag */
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TcpIdx            = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TcpIdx;

                  RemSize -= EleSize;
                  LocRetrQueueIdx = CurrentRetrQueueIdx;
                }
                /* end code block A ------------------------------------------------------------------------------------- */
              }
            }
            else
            {
              /* check number of elements in the RetrQueue that follow the current element, and then check if freeing
              (some of) these elements would allow the current element to be split into several elements */
              TcpIp_TcpTxRetrQueueIdxType LocRetrQueueIdx_A = LocRetrQueueIdx;
              TcpIp_TcpTxRetrQueueIdxType NumFollowingRetrQueueElements = 0;
              while(TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx_A].EleNext != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX)
              {
                LocRetrQueueIdx_A = TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx_A].EleNext;
                NumFollowingRetrQueueElements++;
              }

              if(NumAdditionalEle <= (TcpIp_Tcp_NumFreeRetrQueueEle + NumFollowingRetrQueueElements))
              {
                /* enough free elements availabe if some elements at the end of the socket's RetrQueue are freed/dropped. */

                /* drop (NumAdditionalEle - TcpIp_Tcp_NumFreeRetrQueueEle) elements at the end of the socket's RetrQueue [F1] (+ [F3]) */
                {
                  uint8 NumDropEle = (uint8)(NumAdditionalEle - (uint8)TcpIp_Tcp_NumFreeRetrQueueEle);  /* TcpIp_Tcp_NumFreeRetrQueueEle is very small here, so it can be mapped to uint8 */
                  TcpIp_Tcp_VDropEndEleInRetrQueue(TcpIdx, NumDropEle);
                }
                /* insert new element(s) in the socket's RetrQueue [F2] */
                TcpIp_Tcp_VInsElesInRetrQueue(TcpIdx, LocRetrQueueIdx, NumAdditionalEle);

                {
                  /* start code block A ----------------------------------------------------------------------------------- */
                  TcpIp_TcpTxRetrQueueIdxType CurrentRetrQueueIdx = LocRetrQueueIdx;
                  uint32 SizeTot = TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte;
                  uint32 RemSize = SizeTot;
                  /* reduce size of the current RetrQueue element -------------------- */
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte = NewMaxSegSize;
                  RemSize -= NewMaxSegSize;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter = 0;  /* reset transmit counter */
                  /* fill the new elements with parts of the current RetrQueue element -------------------- */
                  while(0 != RemSize)
                  {
                    uint32 EleSize;
                    TcpIp_TcpTxRetrQueueIdxType LastEleIdx;
                    LastEleIdx = CurrentRetrQueueIdx;
                    CurrentRetrQueueIdx = TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].EleNext;
                    if(RemSize > NewMaxSegSize)
                    {
                      EleSize = NewMaxSegSize;
                    }
                    else
                    {
                      EleSize = RemSize;
                    }
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx   = TcpIp_Tcp_TxRetrQueue[LastEleIdx].DataBufStartIdx;
                    TcpIp_IncTxBufIdx(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx, TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte);
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte       = EleSize;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SeqNo             = TcpIp_Tcp_TxRetrQueue[LastEleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].UserTimeoutTs     = TcpIp_Tcp_TxRetrQueue[LastEleIdx].UserTimeoutTs;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TimeStamp         = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TimeStamp;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter   = 0;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].Flags             = (uint8)(TcpIp_Tcp_TxRetrQueue[LastEleIdx].Flags & (~(TCPIP_TCP_CODE_BIT_SYN))); /* without SYN flag */
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TcpIdx            = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TcpIdx;

                    RemSize -= EleSize;
                  }
                  /* end code block A ------------------------------------------------------------------------------------- */
                }

              }
              else
              {
                /* there are not enough RetrQueue elements available, even if all following elements in the socket's RetrQueue are dropped */
                uint8 NumDropEle = (uint8)NumFollowingRetrQueueElements;  /* TcpIp_Tcp_NumFreeRetrQueueEle is very small here, so it can be mapped to uint8 */
                uint8 NumAppendedEle;

                /* drop all elements at the end of the socket's RetrQueue [F1] (+ [F3]) */
                TcpIp_Tcp_VDropEndEleInRetrQueue(TcpIdx, NumDropEle);

                /* insert/append new element(s) in the socket's RetrQueue [F2] */
                LocRetrQueueIdx = TcpIp_Tcp_LastRetrQueueEle[TcpIdx];
                NumAppendedEle = (uint8)TcpIp_Tcp_NumFreeRetrQueueEle;
                TcpIp_Tcp_VInsElesInRetrQueue(TcpIdx, LocRetrQueueIdx, NumAppendedEle);

                {
                  /* start code block B ----------------------------------------------------------------------------------- */
                  TcpIp_TcpTxRetrQueueIdxType CurrentRetrQueueIdx = LocRetrQueueIdx;
                  uint32 SizeTot = TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].SizeTotByte;
                  uint32 RemSize = SizeTot;
                  uint8  RemEle = NumAppendedEle;  /* remaining elements that were inserted/appendet to the RetrQueue */
                  /* reduce size of the current RetrQueue element -------------------- */
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte = NewMaxSegSize;
                  RemSize -= NewMaxSegSize;
                  TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter = 0;  /* reset transmit counter */

                  /* fill the new elements with parts of the current RetrQueue element -------------------- */
                  while((0 != RemEle) && (0 != RemSize))
                  {
                    uint32 EleSize;
                    TcpIp_TcpTxRetrQueueIdxType LastEleIdx;
                    LastEleIdx = CurrentRetrQueueIdx;
                    CurrentRetrQueueIdx = TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].EleNext;
                    if(RemSize > NewMaxSegSize)
                    {
                      EleSize = NewMaxSegSize;
                    }
                    else
                    {
                      EleSize = RemSize;
                    }
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx   = TcpIp_Tcp_TxRetrQueue[LastEleIdx].DataBufStartIdx;
                    TcpIp_IncTxBufIdx(TCPIP_TCPIDX2SOCKIDX(TcpIdx), TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].DataBufStartIdx, TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte);
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SizeTotByte       = EleSize;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].SeqNo             = TcpIp_Tcp_TxRetrQueue[LastEleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].UserTimeoutTs     = TcpIp_Tcp_TxRetrQueue[LastEleIdx].UserTimeoutTs;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TimeStamp         = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TimeStamp;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TransmitCounter   = 0;
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].Flags             = (uint8)(TcpIp_Tcp_TxRetrQueue[LastEleIdx].Flags & (~(TCPIP_TCP_CODE_BIT_SYN))); /* without SYN flag */
                    TcpIp_Tcp_TxRetrQueue[CurrentRetrQueueIdx].TcpIdx            = TcpIp_Tcp_TxRetrQueue[LastEleIdx].TcpIdx;

                    RemSize -= EleSize;
                    RemEle--;
                  }

                  /* un-queue the remaining length (in the sockets ReqArray) [F3] */
                  TcpIp_Tcp_VUnqueueDataInReqArray(TcpIdx, RemSize);

                  /* end code block B ------------------------------------------------------------------------------------- */
                }
              }
            }

          }
          else
          {
            /* element isn't too big, so no re-forming is necessary -> do nothing, go to the next element */
          }
          LocRetrQueueIdx = TcpIp_Tcp_TxRetrQueue[LocRetrQueueIdx].EleNext;

        } while(LocRetrQueueIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX);

        TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = NewMaxSegSize;
      }
      else if( /* check if the new PathMtu is smaller than the currently used one */
        (NewMaxSegSize < TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]) &&
        /* check if there is any element in the RetrQueue for this connection */
        (TCPIP_TCP_TX_RETR_QUEUE_INV_IDX == TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
        )
      {
        TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = NewMaxSegSize;
      }
      else
      {
        /* now we have (NewMaxSegSize >= TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]) */
        if( NewMaxSegSize > TcpIp_Tcp_TxMaxSegLenByte[TcpIdx])
        {
          TcpIp_Tcp_SockMtuSize[TcpIdx] = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
        }
        else
        {
          TcpIp_Tcp_SockMtuSize[TcpIdx] = NewMaxSegSize;
        }
      }
    }
  }

#endif /** (TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE == STD_ON) */
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

/***********************************************************************************************************************
 *  TcpIp_Tcp_TxCheckAndPrepBuf
 **********************************************************************************************************************/
/*! \brief      - check connection state and check if a free Tcp_TxReqArray element is available
 *  \param[in]  TcpIdx                tcp socket index
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_TxCheckAndPrepBuf(
    TcpIp_TcpIdxType TcpIdx )
{
  Std_ReturnType RetValue;
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_INIT, TCPIP_TCP_API_ID_TX_CHECK_AND_PREP_BUF, TCPIP_E_NOTINIT, E_NOT_OK )
  TcpIp_CheckDetErrorReturnValue( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_TX_CHECK_AND_PREP_BUF, TCPIP_E_INV_SOCK_ID, E_NOT_OK )
  /* ------------------------------------------------- */

  SockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);

  if( (TCPIP_TCP_SOCK_STATE_ESTABLISHED != TcpIp_Tcp_SockState[TcpIdx]) &&
      (TCPIP_TCP_SOCK_STATE_CLOSEWAIT   != TcpIp_Tcp_SockState[TcpIdx]) )
  {
    return E_NOT_OK;
  }

  /* TCP TxConfirmationStreaming */
  {
    /* update TxReqArray element (the only element for this socket) */
    TcpIp_Tcp_TxReqArrayType *TxReqArrayElemPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];

    if(0 == TxReqArrayElemPtr->DataLenByte)
    {
      /* there is no tx data, yet */
      TxReqArrayElemPtr->DataBufStartIdx = TcpIp_TxBufWritePos[SockIdx];
    }
    else
    {
      /* there is already other data that has to be sent */
    }
    RetValue = E_OK;
  }

  return RetValue;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_InitSock
 **********************************************************************************************************************/
/*! \brief      Init a single socket on TCP level.
 *  \param[in]  SocketIdx             socket index
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_InitSock(
    TcpIp_SocketIdType SocketIdx )
{
  TcpIp_Tcp_VInitSock(TCPIP_SOCKIDX2TCPIDX(SocketIdx));
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_SendAsr
 **********************************************************************************************************************/
/*! \brief      send data that was provided earlier
 *  \param[in]  TcpIdx                tcp socket index
 *  \param[in]  TxLen                 length of the data that shall be transmitted
 *  \return     E_OK                  transmission requested
 *  \return     E_NOT_OK              transmission request failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_SendAsr(
    TcpIp_TcpIdxType TcpIdx,
    uint32 TxLen )
{
  /* Fill as many TxRetryQueue elements as necessary (or AS AVAILABLE). It may happen that not enough elements are available, and that must be handled in the MainFct. */

  TcpIp_Tcp_TxReqArrayType *TxReqArrayElemPtr;

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_INIT, TCPIP_TCP_API_ID_SEND_ASR, TCPIP_E_NOTINIT, E_NOT_OK )
  TcpIp_CheckDetErrorReturnValue( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_SEND_ASR, TCPIP_E_INV_SOCK_ID, E_NOT_OK )
  /* 'TxLen' will never be '0', that is assured by the calling function */
  TcpIp_CheckDetErrorReturnValue( (0 != TxLen), TCPIP_TCP_API_ID_SEND_ASR, TCPIP_E_INV_ARG, E_NOT_OK )
  /* ------------------------------------------------- */

  /* use pointer to TxReqArray: */
  {
    TxReqArrayElemPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];
    /* element is already partly filled (in TcpIp_Tcp_TxCheckAndPrepBuf()) */
    TxReqArrayElemPtr->DataLenByte     += TxLen;  /* might be smaller than the length originally used in ProvideTxBuf */
    /* TxReqArrayElemPtr->QueuedLen is not changed. If the retry-element already contained data then there already is a valid length, otherwise the length should already be '0' */
    TxReqArrayElemPtr->FullyPlanned    = FALSE;
  }

  TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx]  += TxLen;

#if (TCPIP_TCP_QUEUE_REQ_DATA_IN_MAIN_FUNCTION_ONLY == STD_ON)
  /* TcpIp_Tcp_VQueueRemReqData will be called only in MainFunction. */
#else
  /* queue data */
  (void)TcpIp_Tcp_VQueueRemReqData(TcpIdx);
#endif

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_GetSockIsConnected
 **********************************************************************************************************************/
/*! \brief       Get information if socket is connected and not yet closed.
 *  \param[in]   TcpIdx                 socket index
 *  \return      TRUE                   socket is connected 
 *               FALSE                  socket is not connected
 *  \context     task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_GetSockIsConnected(
    TcpIp_TcpIdxType TcpIdx )
{
  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_INIT, TCPIP_TCP_API_ID_GET_SOCK_IS_CONNECTED, TCPIP_E_NOTINIT, FALSE )
  TcpIp_CheckDetErrorReturnValue( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_GET_SOCK_IS_CONNECTED, TCPIP_E_INV_SOCK_ID, FALSE )
  /* ------------------------------------------------- */

  if( (TcpIp_Tcp_SockState[TcpIdx] >= TCPIP_TCP_SOCK_STATE_ESTABLISHED) &&
      (TcpIp_Tcp_SockState[TcpIdx] < TCPIP_TCP_SOCK_STATE_CLOSED) )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionRx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionRx( void )
{
  TcpIp_TcpIdxType tcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* #10 Iterate over all TCP sockets. */

  for (tcpIdx = 0; tcpIdx < (TcpIp_VCfgGetMaxNumTcpSocks()); tcpIdx++)
  {
    /* #20 Remove ACKed segments from the retry queue. */
    uint16 tmpRemIndLen = TcpIp_Tcp_RxBufferRemIndLen[tcpIdx];

#if (TCPIP_TCP_FREE_ACKED_REQ_AND_RETR_ELEMENTS_IN_MAIN_FUNCTION == STD_ON)
    TcpIp_Tcp_VFreeAckedReqAndRetrElements(tcpIdx, TcpIp_Tcp_SndUna[tcpIdx]);
#endif

    if (tmpRemIndLen > 0)
    {
      /* #30 Forward newly received data to upper layer. */
      TcpIp_Tcp_VMainRxDataForwarding(tcpIdx, tmpRemIndLen);
    }
  }
}  /* TcpIp_Tcp_MainFunctionRx() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionTx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionTx(void)
{
  TcpIp_TcpIdxType tcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* #10 Transmit pending resets. */

  if(0 != TcpIp_Tcp_RstTxQueueFillNum)
  {
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    TcpIp_Tcp_VTransmitResets();  /* transmit resets from reset-queue */
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  
  /* #20 Iterate over all TCP sockets. */

  for (tcpIdx = 0; tcpIdx < (TcpIp_VCfgGetMaxNumTcpSocks()); tcpIdx++)
  {
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
    /* #30 Increase CWND based on received ACKs during one round trip time. */
    if( TCPIP_TCP_CONG_CTRL_MODE_CONGESTION_AVOIDANCE == TcpIp_Tcp_CongestionControlMode[tcpIdx])
    {
      /* If ACKs have to be summarized during one RTT, the timeout is handled here. */
      if(0 != TcpIp_Tcp_AckSumTimeout[tcpIdx])
      {
        TcpIp_Tcp_AckSumTimeout[tcpIdx]--;
        if(0 == TcpIp_Tcp_AckSumTimeout[tcpIdx])
        {
          /* inc cwnd */
          CANOE_WRITE_STRING_2("TcpIp_Tcp_MainFunction: TcpIdx %d: inc cwnd in congestion avoidance, AckSum is %d", TcpIdx, TcpIp_Tcp_AckSumN[TcpIdx]);
          TcpIp_Tcp_VIncCwnd(tcpIdx, TcpIp_Tcp_AckSumN[tcpIdx]);
        }
      }
    }
#endif  /* (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON) */

    /* #40 Send pending TX data. */
    TcpIp_Tcp_VMainTxDataHandling(tcpIdx);
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* TcpIp_Tcp_MainFunctionTx() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionState
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionState(void)
{
  TcpIp_TcpIdxType tcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  TcpIp_Tcp_GlobalTimestamp++;

  /* #10 Iterate over all TCP sockets. */

  for (tcpIdx = 0; tcpIdx < (TcpIp_VCfgGetMaxNumTcpSocks()); tcpIdx++)
  {
    /* #20 Handle pending TCP socket state changes. */
    TcpIp_Tcp_VMainStateHandling(tcpIdx);

    /* #30 Notify upper layer about socket state changes. */
    if(TcpIp_Tcp_EventIndicationPending[tcpIdx] != 0)
    {
      if(TcpIp_Tcp_EventIndicationPending[tcpIdx] == IPBASE_TCP_EVENT_FIN_RECEIVED)
      {
        switch(TcpIp_Tcp_SockState[tcpIdx])
        {
        case TCPIP_TCP_SOCK_STATE_SYNRCVD:
        case TCPIP_TCP_SOCK_STATE_ESTABLISHED:
        case TCPIP_TCP_SOCK_STATE_CLOSEWAIT:
          {
            /* signal "FIN received" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(tcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(tcpIdx)], TCPIP_TCP_STATE_FIN_REC, TcpIp_Tcp_UseTls[tcpIdx]);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(tcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(tcpIdx)], TCPIP_TCP_STATE_FIN_REC, FALSE);
#endif
            break;
          }
        case TCPIP_TCP_SOCK_STATE_FINWAIT1:
        case TCPIP_TCP_SOCK_STATE_FINWAIT2:
        case TCPIP_TCP_SOCK_STATE_TIMEWAIT:
          {
            /* signal "connection closing" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(tcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(tcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, TcpIp_Tcp_UseTls[tcpIdx]);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(tcpIdx), TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(tcpIdx)], TCPIP_TCP_STATE_CONN_CLOSING, FALSE);
#endif
            break;
          }
        default:
          {
            /* this should not occur, do nothing */
          }
        }
        TcpIp_Tcp_EventIndicationPending[tcpIdx] = 0;  /* delete the pending event */
      }
    }

    /* #40 Handle TCP idle timeouts. */
    TcpIp_Tcp_VMainIdleTimeoutHandling(tcpIdx);
  }
} /* TcpIp_Tcp_MainFunctionState() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_DuplicateSocket
 **********************************************************************************************************************/
/*! \brief      duplicate parameters of listen socket and return a new (acceptable) socket
 *  \param[in]  ListenTcpIdx          listen socket index
 *  \return                           TcpIdx of the new socket or TCPIP_TCP_INV_IDX if socket could not be duplicated
 *  \context    task level
 **********************************************************************************************************************/
FUNC(TcpIp_TcpIdxType, TCPIP_CODE) TcpIp_Tcp_DuplicateSocket(
    TcpIp_TcpIdxType ListenTcpIdx)
{
  TcpIp_TcpIdxType  NewTcpIdx;
  TcpIp_SockIdxType NewSockIdx;
  TcpIp_SockIdxType ListenSockIdx;

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_INIT, TCPIP_TCP_API_ID_DUPL_AND_CONNECT, TCPIP_E_NOTINIT, E_NOT_OK )
  TcpIp_CheckDetErrorReturnValue( TCPIP_TCP_COND_VALID_TCPIDX(ListenTcpIdx), TCPIP_TCP_API_ID_DUPL_AND_CONNECT, TCPIP_E_INV_SOCK_ID, E_NOT_OK )
  /* ------------------------------------------------- */

  ListenSockIdx = TCPIP_TCPIDX2SOCKIDX(ListenTcpIdx);

  /* take a new TCP socket if available */
  {
    TcpIp_SockIdxType TempSockIdx;
    TcpIp_SockIdxType DerivedConnCnt = 0;  /* number of derived sockets */
    /* count TCP sockets that have the current listen socket as master */
    for(TempSockIdx = TcpIp_VCfgGetMaxNumUdpSocks(); TempSockIdx < TcpIp_VCfgGetMaxNumSocks(); TempSockIdx++)
    {
      if(TcpIp_SocketList[TempSockIdx].TcpMasterListenSockId == ListenSockIdx)
      {
        DerivedConnCnt++;
      }
    }

    if(DerivedConnCnt < TcpIp_SocketList[ListenSockIdx].TcpMaxNumListenSockets)
    {
      /* ok */
      TcpIp_DomainType Domain = ((IpBase_SockAddrType *)(&TcpIp_Tcp_RxIndArray[ListenTcpIdx].RemSockAddr))->sa_family;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

      if(E_OK == TcpIp_GetSocketForUser(Domain, TCPIP_IPPROTO_TCP, &NewSockIdx, TcpIp_SockUserIdx[ListenSockIdx]))
      {
        /* socket could be allocated */
        Std_ReturnType RetVal;

        NewTcpIdx = TCPIP_SOCKIDX2TCPIDX(NewSockIdx);

        if(0 != TcpIp_Tcp_RequestedRxBufferSize[ListenTcpIdx])
        {
          RetVal = TcpIp_Tcp_SetSockOpt(NewTcpIdx, TCPIP_PARAMID_TCP_RXWND_MAX, TcpIp_Tcp_RequestedRxBufferSize[ListenTcpIdx]);
          if(E_OK != RetVal)
          {
            /* no matching buffer available -> release the new socket */
            (void)TcpIp_Close(NewSockIdx, FALSE);
            return TCPIP_TCP_INV_IDX;  /* cancel this attempt to accept a connection */
          }
        }

        if(0 != TcpIp_Tcp_RequestedTxBufferSize[ListenTcpIdx])
        {
          RetVal = TcpIp_Tcp_SetSockOpt(NewTcpIdx, TCPIP_PARAMID_V_TCP_TXBUFSIZE, TcpIp_Tcp_RequestedTxBufferSize[ListenTcpIdx]);
          if(E_OK != RetVal)
          {
            /* no matching buffer available -> release the new socket */
            (void)TcpIp_Close(NewSockIdx, FALSE);
            return TCPIP_TCP_INV_IDX;  /* cancel this attempt to accept a connection */
          }
        }

        TcpIp_SocketList[NewSockIdx].TcpMasterListenSockId = ListenSockIdx;  /* set master socket index for this new socket */
      }
      else
      {
        /* no free sockets available */
        return TCPIP_TCP_INV_IDX;
      }
    }
    else
    {
      /* no more connection may be accepted on this listen socket */
      return TCPIP_TCP_INV_IDX;
    }
  }

  TcpIp_Tcp_RcvNxt[NewTcpIdx]       = TcpIp_Tcp_RcvNxt[ListenTcpIdx];
#if (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON)
  TcpIp_Tcp_NagleEnabled[NewTcpIdx] = TcpIp_Tcp_NagleEnabled[ListenTcpIdx];
#endif
  /* TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  TcpIp_Tcp_VKeepAliveCopyParameters(ListenTcpIdx, NewTcpIdx);
#endif

  TcpIp_Tcp_Iss[NewTcpIdx]    = TcpIp_Tcp_GlobalTimestamp * (250 * TcpIp_VCfgGetMainFctPeriodMsec());  /* inc iss every 4 microseconds */
  TcpIp_Tcp_SndUna[NewTcpIdx] = TcpIp_Tcp_Iss[NewTcpIdx];  /* ISS */
  TcpIp_Tcp_SndNxt[NewTcpIdx] = TcpIp_Tcp_Iss[NewTcpIdx];  /* ISS */
  TcpIp_Tcp_SndWl2[NewTcpIdx] = TcpIp_Tcp_Iss[NewTcpIdx];  /* ISS */

  /* set local family and port */
  ((IpBase_SockAddrInType *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin_family = ((IpBase_SockAddrInType *)&(TcpIp_Tcp_RxIndArray[ListenTcpIdx].RemSockAddr))->sin_family;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  ((IpBase_SockAddrInType *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin_port   = ((IpBase_SockAddrInType *)&(TcpIp_SocketList[ListenSockIdx].LocSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  if(IPBASE_AF_INET == ((IpBase_SockAddrInType *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    IpV4_AddrIdType IpV4AddrId      = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(TcpIp_Tcp_RxIndArray[ListenTcpIdx].LocAddrId);
    IpBase_AddrInType *IpV4AddrPtr  = &(((IpBase_SockAddrInType *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin_addr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    (void)IpV4_Ip_GetLocNetAddr(IpV4AddrId, IpV4AddrPtr);
  }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  if(IPBASE_AF_INET6 == ((IpBase_SockAddrIn6Type *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin6_family)
  {
    IpV6_AddrIdType IpV6AddrId      = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(TcpIp_Tcp_RxIndArray[ListenTcpIdx].LocAddrId);
    IpBase_AddrIn6Type *IpV6AddrPtr = &(((IpBase_SockAddrIn6Type *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin6_addr);
    (void)IpV6_GetLocalAddress(IpV6AddrId, IpV6AddrPtr, NULL_PTR);
  }
#endif

  (void) IpBase_CopySockAddr((IpBase_SockAddrType *)&(TcpIp_SocketList[NewSockIdx].RemSock), (IpBase_SockAddrType *)&(TcpIp_Tcp_RxIndArray[ListenTcpIdx].RemSockAddr));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

  TcpIp_Tcp_SockIsServer[NewTcpIdx]            = TcpIp_Tcp_SockIsServer[ListenTcpIdx];
  TcpIp_SocketList[NewSockIdx].TxIpAddrId      = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(TcpIp_Tcp_RxIndArray[ListenTcpIdx].LocAddrId);
  TcpIp_SocketList[NewSockIdx].BindLocalAddrId = TcpIp_SocketList[ListenSockIdx].BindLocalAddrId;

  if(IPBASE_AF_INET == ((IpBase_SockAddrInType *)&(TcpIp_SocketList[NewSockIdx].LocSock))->sin_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    TcpIp_Tcp_TxMaxSegLenByte[NewTcpIdx] = TCPIP_TCP_DEFAULT_TX_MSS_IPV4;
  }
  else
  {
    TcpIp_Tcp_TxMaxSegLenByte[NewTcpIdx] = TCPIP_TCP_DEFAULT_TX_MSS_IPV6;
  }

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
  /* set default cwnd */
  TcpIp_Tcp_CwndSegNumLimit[NewTcpIdx] = TCPIP_TCP_DEFAULT_SEGMENT_LIMIT(TcpIp_Tcp_TxMaxSegLenByte[NewTcpIdx]);
  TcpIp_Tcp_Cwnd[NewTcpIdx]            = TcpIp_Tcp_TxMaxSegLenByte[NewTcpIdx] * TcpIp_Tcp_CwndSegNumLimit[NewTcpIdx];
#endif

  /* Init SockMtuSize with received MaxSegLen */
  TcpIp_Tcp_SockMtuSize[NewTcpIdx] = TcpIp_Tcp_TxMaxSegLenByte[NewTcpIdx] +20;

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_SackOptActive[NewTcpIdx] = FALSE;
#endif

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_TimestampOptActive[NewTcpIdx]    = FALSE;
  TcpIp_Tcp_TimestampReceived[NewTcpIdx]     = 0;
#endif

  /* reset values for listen socket */
  TcpIp_Tcp_TxOneTimeOpts[NewTcpIdx]    = 0;
  TcpIp_Tcp_TxOneTimeOptsLen[NewTcpIdx] = 0;

  TcpIp_Tcp_SockState[NewTcpIdx]     = TcpIp_Tcp_SockState[ListenTcpIdx];
  TcpIp_Tcp_SockStateNext[NewTcpIdx] = TcpIp_Tcp_SockStateNext[ListenTcpIdx];

  return NewTcpIdx;
}  /* TcpIp_Tcp_DuplicateSocket() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  TcpIp_Tcp_AcceptSocket
 **********************************************************************************************************************/
/*! \brief      accept a connection on a socket duplicated by TcpIp_Tcp_DuplicateSocket()
 *  \param[in]  ListenTcpIdx         Index of the listen socket
 *  \param[in]  AcceptTcpIdx         Index of the duplicated/accept socket
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_AcceptSocket(
    TcpIp_TcpIdxType ListenTcpIdx,
    TcpIp_TcpIdxType AcceptTcpIdx)
{
  TcpIp_SockIdxType AcceptSockIdx = TCPIP_TCPIDX2SOCKIDX(AcceptTcpIdx);

  /* Trigger connecting of the socket ------------------ */
  /* send SYN and ACK */
  TcpIp_Tcp_VSendFlags(AcceptTcpIdx, (TCPIP_TCP_CODE_BIT_SYN | TCPIP_TCP_CODE_BIT_ACK) );
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  TcpIp_Tcp_VKeepAliveTrigger(AcceptTcpIdx);
#endif

  TcpIp_Tcp_SockState[AcceptTcpIdx]     = TCPIP_TCP_SOCK_STATE_SYNRCVD;
  TcpIp_Tcp_SockStateNext[AcceptTcpIdx] = TCPIP_TCP_SOCK_STATE_SYNRCVD;
  /* -> Main function will send message with (SYN | ACK) */

  TcpIp_Tcp_BackLogArray[ListenTcpIdx].SynAvailable = FALSE;  /* currently only one backlog element per socket is supported */
  TcpIp_Tcp_BackLogArray[AcceptTcpIdx].SynAvailable = FALSE;
  /* do not care about the other struct elements */

  TcpIp_VSockTcpStateChg(AcceptSockIdx, TcpIp_SockUserIdx[AcceptSockIdx], TCPIP_TCP_STATE_CONN_ESTAB, FALSE);  /* set socket to 'connected' */

#if ((defined TCPIP_SPECIAL_TCP_ACC_ENABLED) && (TCPIP_SPECIAL_TCP_ACC_ENABLED == STD_ON))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
  {
    TcpIp_SockIdxType ListenSockIdx = TCPIP_TCPIDX2SOCKIDX(ListenTcpIdx);
    (void)TCPIP_SPECIAL_TCP_ACCEPTING_FCT(ListenSockIdx, AcceptSockIdx, (TcpIp_SockAddrType *)&(TcpIp_SocketList[AcceptSockIdx].RemSock)); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  }
#endif
} /* TcpIp_Tcp_AcceptSocket() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_Open
 **********************************************************************************************************************/
/*! \brief      active or passive open of a connection
 *  \param[in]  SockIdx                socket index
 *  \param[in]  SockAddr               socket address
 *  \param[in]  ActiveOpen             active or passive open of a connection
 *  \context    task level
 *  \note       Format: OPEN (local port, foreign socket, active/passive [, timeout] [, precedence]
 *              [, security/compartment] [, options]) -> local connection name
 **********************************************************************************************************************/
FUNC(TcpIp_TcpIdxType, TCPIP_CODE) TcpIp_Tcp_Open(
    TcpIp_SockIdxType SockIdx,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) SockAddrPtr,
    boolean ActiveOpen
    /* , uint8 Precedence,
    uint8 Security */ )
{

  TcpIp_TcpIdxType TcpIdx = TCPIP_SOCKIDX2TCPIDX(SockIdx);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_VALID_TCPSOCKIDX(SockIdx), TCPIP_TCP_API_ID_OPEN, TCPIP_E_INV_SOCK_ID, TCPIP_TCP_INV_IDX )
  TcpIp_CheckDetErrorReturnValue( (SockAddrPtr != NULL_PTR), TCPIP_TCP_API_ID_OPEN, TCPIP_E_PARAM_POINTER, TCPIP_TCP_INV_IDX )
  /* ------------------------------------------------- */

  if( (TCPIP_TCP_SOCK_STATE_CLOSED != TcpIp_Tcp_SockState[TcpIdx]) &&
      (TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx]) )
  {
    /* RFC: "error: connection already exists" */
    /* Windows: WSAEISCONN - 10056 - "Socket is already connected" */
    return TCPIP_TCP_INV_IDX;
  }

  if((TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] == 0xFF) || (TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] == 0xFF))
  {
    /* one or both buffers assignments are missing, so it is not possible to open a connection */
    return TCPIP_TCP_INV_IDX;
  }

  {
    /* copy socket */
    (void) IpBase_CopySockAddr((IpBase_SockAddrType *)&(TcpIp_SocketList[SockIdx].RemSock), SockAddrPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    /* init variables for this socket */
    TcpIp_Tcp_RcvNxt[TcpIdx] = 0;

    if(TCPIP_TCP_ACTIVE_OPEN == ActiveOpen)
    {
      /* Connect (active open) */
      /*
        RFC:
        - select an initial send sequence number (ISS)
        - send a SYN segment of the form <SEQ=ISS><CTL=SYN>
        - set SND.UNA to ISS, SND.NXT to ISS+1, enter SYN-SENT state, and return.
      */
      TcpIp_Tcp_SockIsServer[TcpIdx] = FALSE;
      TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_SYN);

      TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_SYNSENT;
      TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_SYNSENT;

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
      TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
      TcpIp_Tcp_TxOneTimeOpts[TcpIdx]    |= TCPIP_TCP_OPT_FLAG_SACK_PERMIT;
      TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx] += (TCPIP_TCP_OPT_LEN_SACK_PERMIT +2);  /* option + 2x NOP */
#endif

      if(IPBASE_AF_INET == SockAddrPtr->sa_family) /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = TCPIP_TCP_DEFAULT_TX_MSS_IPV4;
      }
      else
      {
        TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = TCPIP_TCP_DEFAULT_TX_MSS_IPV6;
      }

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
      /* set default cwnd */
      TcpIp_Tcp_CwndSegNumLimit[TcpIdx] = TCPIP_TCP_DEFAULT_SEGMENT_LIMIT(TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]);
      TcpIp_Tcp_Cwnd[TcpIdx]            = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] * TcpIp_Tcp_CwndSegNumLimit[TcpIdx];
#endif
    }
    else
    {
      /* Listen (passive open) */
      TcpIp_Tcp_SockIsServer[TcpIdx]  = TRUE;
      TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_LISTEN;
      TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

      /* If buffers have been assinged to this listen socket before, release those buffers but remember the buffer sizes. */
      /* tx buffer */
      if(TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] != 0xFF)
      {
        /* copy buffer size */
        TcpIp_Tcp_RequestedTxBufferSize[TcpIdx] = TcpIp_Tcp_TxBufferSize[TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx]];

        /* now release the tx buffer */
        TcpIp_Tcp_TxBuffer_IsUsedBySocket[TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx]] = TCPIP_INV_SOCKET_ID;
        TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] = 0xFF;
      }
      /* rx buffer */
      if(TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] != 0)
      {
        /* copy buffer size */
        TcpIp_Tcp_RequestedRxBufferSize[TcpIdx] = TcpIp_Tcp_AssignedRxBufferSize[TcpIdx];
        TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] = 0;

        /* now release the rx buffer */
        TcpIp_Tcp_RxBuffer_IsUsedBySocket[TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx]] = TCPIP_INV_SOCKET_ID;
        TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] = 0xFF;
      }
    }
  }
  return TcpIdx;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 *  TcpIp_Tcp_Close
 **********************************************************************************************************************/
/*! \brief       has to close the TCP connection gracefully, and then signal success to SOCK
 *  \param[in]   TcpIdx                 socket index
 *  \context     task level
 *  \note        Format: CLOSE (local connection name)
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Close(
    TcpIp_TcpIdxType TcpIdx )
{
#if (TCPIP_SUPPORT_TLS == STD_ON)
  TcpIp_SocketIdType SocketId;
#endif
  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT, TCPIP_TCP_API_ID_CLOSE, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_CLOSE, TCPIP_E_INV_SOCK_ID )
  /* ------------------------------------------------- */

#if (TCPIP_SUPPORT_TLS == STD_ON)
  SocketId = TCPIP_TCPIDX2SOCKIDX(TcpIdx);
#endif

  switch(TcpIp_Tcp_SockState[TcpIdx])
  {
  case TCPIP_TCP_SOCK_STATE_CLOSED:
    {
      if(TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] != 0xFF)
      {
        /* release the buffer */
        TcpIp_Tcp_TxBuffer_IsUsedBySocket[TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx]] = TCPIP_INV_SOCKET_ID;
        TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] = 0xFF;
      }
      if(TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] != 0xFF)
      {
        /* release the buffer */
        TcpIp_Tcp_RxBuffer_IsUsedBySocket[TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx]] = TCPIP_INV_SOCKET_ID;
        TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] = 0xFF;
        TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] = 0;
      }

      break;
    }
  case TCPIP_TCP_SOCK_STATE_LISTEN:
    {
      /* RFC: Any outstanding RECEIVEs are returned with "error: closing"
      responses. Delete TCB, enter CLOSED state, and return. */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
      uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

      /* delete/clear TCB element */
      TcpIp_Tcp_VRestartSock(TcpIdx);
      TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

      /* only server sockets can be in state 'listen' */
      TcpIp_Tcp_VClosePartlyAcceptedSockets(TcpIdx);

#if (TCPIP_SUPPORT_TLS == STD_ON)
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, UseTls);
#else
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, FALSE);
#endif
      break;
    }
  case TCPIP_TCP_SOCK_STATE_SYNRCVD:
    {
      /* RFC: issue sending of FIN segment (eventually put this request in send queue) */

      /* send FIN */
      TcpIp_Tcp_VSendFlags(TcpIdx, (TCPIP_TCP_CODE_BIT_FIN));

      TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_FINWAIT1;
      TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_FINWAIT1;
      break;
    }
  case TCPIP_TCP_SOCK_STATE_SYNSENT:
    {
      /* RFC: Delete the TCB and return "error: closing" responses to any
      queued SENDs, or RECEIVEs. */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
      uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

      /* delete/clear TCB element */
      TcpIp_Tcp_VRestartSock(TcpIdx);
      TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, UseTls);
#else
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, FALSE);
#endif
      break;
    }
  case TCPIP_TCP_SOCK_STATE_ESTABLISHED:
    {
      /* RFC: Queue this until all preceding SENDs have been segmentized, then
      form a FIN segment and send it. In any case, enter FIN-WAIT-1 state. */

      /* send FIN */
      TcpIp_Tcp_VSendFlags(TcpIdx, (TCPIP_TCP_CODE_BIT_FIN | TCPIP_TCP_CODE_BIT_ACK));

      TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_FINWAIT1;
      TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_FINWAIT1;
      break;
    }
  case TCPIP_TCP_SOCK_STATE_FINWAIT1:
  case TCPIP_TCP_SOCK_STATE_FINWAIT2:
    {
      /* RFC: "error: connection closing" */
      /* An "ok" response would be acceptable, too, as long as a second FIN is not emitted. */
      TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10101);
      break;
    }
  case TCPIP_TCP_SOCK_STATE_CLOSEWAIT:
    {
      /* send FIN */
      TcpIp_Tcp_VSendFlags(TcpIdx, TCPIP_TCP_CODE_BIT_FIN);

      TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_LASTACK;
      TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_LASTACK;
      break;
    }
  default:
    {
      /* states: TCPIP_TCP_SOCK_STATE_CLOSING, TCPIP_TCP_SOCK_STATE_LASTACK, TCPIP_TCP_SOCK_STATE_TIMEWAIT */
      /* RFC: "error: connection closing" */
      TcpIp_SetLastSockError(TCPIP_SOCK_ERR_10101);

      /* ignore request silently, closing is already in progress or finished */
      break;
    }
  }

#if (TCPIP_SUPPORT_TLS == STD_ON)
  if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
  {
    Tls_Close(SocketId, FALSE);
  }
#endif

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_Tcp_Abort
 **********************************************************************************************************************/
/*! \brief       hard cancelation of connection
 *  \param[in]   TcpIdx                 socket index
 *  \context     task level
 *  \note        Format: ABORT (local connection name)
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Abort(
    TcpIp_TcpIdxType TcpIdx )
{
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT, TCPIP_TCP_API_ID_ABORT, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_ABORT, TCPIP_E_INV_SOCK_ID )
  /* ------------------------------------------------- */

  SockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);

  /* send RST, delete TCB, enter CLOSED state, and return */

  switch(TcpIp_Tcp_SockState[TcpIdx])
  {
  case TCPIP_TCP_SOCK_STATE_CLOSED:
    {
      /* do nothing */
      break;
    }
  case TCPIP_TCP_SOCK_STATE_LISTEN:
    {
      /* only server sockets can be in state 'listen'
         -> close not yet fully accepted connections */
      TcpIp_Tcp_VClosePartlyAcceptedSockets(TcpIdx);
      break;
    }
  default:
    {
      /* send RST */
      TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[SockIdx].TxIpAddrId,
        (IpBase_SockAddrType *)&(TcpIp_SocketList[SockIdx].RemSock),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        (IpBase_SockAddrType *)&(TcpIp_SocketList[SockIdx].LocSock),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      TcpIp_Tcp_SndNxt[TcpIdx], 0, 0);
      break;
    }
  }

  /* delete/clear TCB element */
  TcpIp_Tcp_VRestartSock(TcpIdx);
  TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
  if(TcpIp_Tcp_UseTls[TCPIP_SOCKHND2TCPIDX(SockIdx)] == TRUE)
  {
    Tls_Close(SockIdx, TRUE);
  }
#endif
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VClosePartlyAcceptedSockets
 **********************************************************************************************************************/
/*! \brief         Close partly established connections.
 *  \description   A server socket was closed. Now close all sockets that are about to establish a connection to this
 *                 server socket. These sockets are not yet known to the server socket user.
 *  \param[in]     TcpIdx                 socket index
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VClosePartlyAcceptedSockets(
    TcpIp_TcpIdxType TcpIdx )
{
  /* search for socket that have this socket as master socket and are in a state less than established */
  TcpIp_TcpIdxType LocTcpIdx;
  for(LocTcpIdx = 0; LocTcpIdx < TCPIP_MAX_NUM_TCP_SOCKETS; LocTcpIdx++)
  {
    if( (TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(LocTcpIdx)].TcpMasterListenSockId == TCPIP_TCPIDX2SOCKIDX(TcpIdx)) && (TcpIp_Tcp_SockState[LocTcpIdx] < TCPIP_TCP_SOCK_STATE_ESTABLISHED) )
    {
      /* abort the connection establishment */
      TcpIp_Tcp_Abort(LocTcpIdx);
    }
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_ResetSocket
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_ResetSocket(TcpIp_SocketIdType socketId)
{
  uint8 tcpIdx      = TCPIP_SOCKHND2TCPIDX(socketId); /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8 sockUserIdx = TcpIp_SockUserIdx[socketId];

  /* clear the reset queue */
  {
    TcpIp_Tcp_RstTxQueueReadPos  = 0;
    TcpIp_Tcp_RstTxQueueWritePos = 0;
    TcpIp_Tcp_RstTxQueueFillNum  = 0;
  }

  TCPIP_ASSERT(TCPIP_TCP_SOCK_STATE_CLOSED != TcpIp_Tcp_SockState[tcpIdx]);

#if (TCPIP_SUPPORT_TLS == STD_ON)
  /* reset the TLS connection */
  if(TcpIp_Tcp_UseTls[tcpIdx] == TRUE)
  {
    Tls_TcpIpEvent(socketId, IPBASE_TCP_EVENT_RESET);
  }
#endif

  /* delete/clear TCB element */
  TcpIp_Tcp_VRestartSock(tcpIdx);

  /* send reset cbk to upper layer */
#if (TCPIP_SUPPORT_TLS == STD_ON)
  TcpIp_VSockTcpStateChg(socketId, sockUserIdx, TCPIP_TCP_STATE_RST_REC, TcpIp_Tcp_UseTls[tcpIdx]);
#else
  TcpIp_VSockTcpStateChg(socketId, sockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VReleaseAsignedBuffers
 **********************************************************************************************************************/
/*! \brief       Release the rx and tx buffers assigned to the TCP socket
 *  \param[in]   TcpIdx                 TCP index
 *  \context     task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_VReleaseAsignedBuffers(
    TcpIp_TcpIdxType TcpIdx )
{
  uint8 RxBufIdx = TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx];
  uint8 TxBufIdx = TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx];

  if(RxBufIdx != 0xFF)  /* valid buffer index */
  {
    TcpIp_Tcp_RxBuffer_IsUsedBySocket[RxBufIdx]     = TCPIP_INV_SOCKET_ID;  /* invalid socket index */
    TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx]  = 0xFF;  /* invalid buffer index */
      TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] = 0;
  }
  if(TxBufIdx != 0xFF)  /* valid buffer index */
  {
    TcpIp_Tcp_TxBuffer_IsUsedBySocket[TxBufIdx]     = TCPIP_INV_SOCKET_ID;  /* invalid socket index */
    TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx]  = 0xFF;  /* invalid buffer index */
  }

  /* reset requested buffer sizes */
  TcpIp_Tcp_RequestedRxBufferSize[TcpIdx] = 0;
  TcpIp_Tcp_RequestedTxBufferSize[TcpIdx] = 0;
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_SetSockOpt
 **********************************************************************************************************************/
/*! \brief       set TCP socket option
 *  \param[in]   TcpIdx                 socket index
 *  \param[in]   OptName                option name
 *  \param[in]   OptVal                 option parameter
 *  \return      E_OK                   socket option change
 *  \return      E_NOT_OK               socket option change request failed
 *  \context     task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_SetSockOpt(
    TcpIp_TcpIdxType TcpIdx,
    uint8 OptName,
    uint32 OptVal )
{
  Std_ReturnType RetValue = E_NOT_OK;

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnValue( TCPIP_COND_INIT, TCPIP_TCP_API_ID_SET_SOCK_OPT, TCPIP_E_NOTINIT, E_NOT_OK )
  TcpIp_CheckDetErrorReturnValue( TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx), TCPIP_TCP_API_ID_SET_SOCK_OPT, TCPIP_E_INV_SOCK_ID, E_NOT_OK )
  /* ------------------------------------------------- */

  switch(OptName)
  {
  case TCPIP_PARAMID_TCP_RXWND_MAX:
    {
      uint32 RequestedBufferSize = OptVal;  /* OptVal represents the size of the needed buffer */
      uint8  BufIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

      if(TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] == 0)
      {
        if(TcpIp_Tcp_SockState[TcpIdx] == TCPIP_TCP_SOCK_STATE_CLOSED)
        {
          /* choose a matching rx buffer and assign it to the socket / TCP connection */
          for(BufIdx = 0; BufIdx < TCPIP_TCP_RX_BUFFERS_NUM; BufIdx++)
          {
            if((TcpIp_Tcp_RxBuffer_IsUsedBySocket[BufIdx] == TCPIP_INV_SOCKET_ID) && (RequestedBufferSize == TcpIp_Tcp_RxBufferSize[BufIdx]))
            {
              /* found a free and matching buffer */
              TcpIp_Tcp_RxBuffer_IsUsedBySocket[BufIdx]  = TCPIP_TCPIDX2SOCKIDX(TcpIdx);
              TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx] = BufIdx;

              TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] = RequestedBufferSize;
              TcpIp_Tcp_RcvWnd[TcpIdx]               = (uint16)RequestedBufferSize;

              RetValue = E_OK;
              break;  /* stop the for-loop */
            }
          }
        }
        else
        {
          /* buffer may only be assigned in state 'Closed' */
          /* -> nothing to do, return value already is negative */
        }
      }
      else
      {
        /* a buffer is already assigned, an updated assignment is not supported -> E_NOT_OK */
      }
      break;
    }

  case TCPIP_PARAMID_V_TCP_TXBUFSIZE:
    if (0xFF == TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx])
    {
      uint32 RequestedBufferSize = OptVal;  /* OptVal represents the size of the needed buffer */
      uint8  BufIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

      if( (TcpIp_Tcp_SockState[TcpIdx] == TCPIP_TCP_SOCK_STATE_CLOSED) || (TcpIp_Tcp_SockState[TcpIdx] == TCPIP_TCP_SOCK_STATE_ESTABLISHED) )
      {
        /* choose a matching tx buffer and assign it to the socket / TCP connection */
        for(BufIdx = 0; BufIdx < TCPIP_TCP_TX_BUFFERS_NUM; BufIdx++)
        {
          if((TcpIp_Tcp_TxBuffer_IsUsedBySocket[BufIdx] == TCPIP_INV_SOCKET_ID) && (RequestedBufferSize == TcpIp_Tcp_TxBufferSize[BufIdx]))
          {
            /* found a free and matching buffer */
            TcpIp_Tcp_TxBuffer_IsUsedBySocket[BufIdx]      = TCPIP_TCPIDX2SOCKIDX(TcpIdx);
            TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx] = BufIdx;
#if ((defined TCPIP_TCP_CONGESTION_CONTROL_SUPPORT) && (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
            TcpIp_Tcp_Ssthresh[TcpIdx]      = TcpIp_VCfgGetTxBufSizeByte(TcpIdx);  /* init to tx buffer size */
#endif
            RetValue = E_OK;
            break;  /* stop the for-loop */
          }
        }
      }
    }
    else
    {
      /* a buffer is already assigned, an updated assignment is not supported -> E_NOT_OK */
    }
    break;

#if (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON)
  case TCPIP_PARAMID_TCP_NAGLE:
    {
      if(TcpIp_Tcp_SockState[TcpIdx] == TCPIP_TCP_SOCK_STATE_CLOSED)
      {
        TcpIp_Tcp_NagleEnabled[TcpIdx] = (boolean)(OptVal);
        RetValue = E_OK;
      }
      break;
    }
#endif  /* (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON) */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  case TCPIP_PARAMID_TCP_KEEPALIVE:
    TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveEnabled = (boolean)(OptVal);
    RetValue = E_OK;
    break;

  case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
    if (OptVal <= TCPIP_MAX_SEC_IN_UINT32_CYCLE_VAR)
    {
      TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbeInterval = (OptVal * TCPIP_CYCLES_PER_SECOND);
      TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
      RetValue = E_OK;
    }
    break;

  case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
    if (OptVal <= 0xFFFFu)
    {
      TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbesMax = (uint16)(OptVal);
      TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
      RetValue = E_OK;
    }
    break;

  case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
    if (OptVal <= TCPIP_MAX_SEC_IN_UINT32_CYCLE_VAR)
    {
      TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveTimeout = (OptVal * TCPIP_CYCLES_PER_SECOND);
      TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
      RetValue = E_OK;
    }
    break;
#endif

#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
  case TCPIP_PARAMID_V_TCP_UTOUPDATE:
    {
      uint16 UtoValue = (uint16)(OptVal);
      TcpIp_Tcp_Uto[TcpIdx] = ((uint32)(UtoValue & 0x7FFF) * 1000) / TCPIP_MAIN_FCT_PERIOD_MSEC;  /* update timeout value */
      if(0 != (UtoValue & 0x8000))
      {
        /* user timeout value has granularity of minutes */
        TcpIp_Tcp_Uto[TcpIdx] *= 60;
      }
      TcpIp_Tcp_UtoAdv[TcpIdx]         = UtoValue;
      TcpIp_Tcp_UtoChangeable[TcpIdx]  = FALSE;  /* diable change by received option */
      /* TcpIp_Tcp_UtoToSend[TcpIdx]      = TRUE; */   /* set marker to send this option */ /* TBR */
      TcpIp_Tcp_TxOneTimeOpts[TcpIdx] |= TCPIP_TCP_OPT_FLAG_USER_TIMEOUT;
      RetValue = E_OK;
      break;
    }

  case TCPIP_PARAMID_V_TCP_UTOENABLED:
    {
      boolean Enabled = (boolean)(OptVal);
      TcpIp_Tcp_UtoEnabled[TcpIdx] = Enabled;  /* enable or disable option handling */
      if(FALSE == Enabled)
      {
        TcpIp_Tcp_UtoChangeable[TcpIdx] = TRUE;  /* set to default */
      }
      RetValue = E_OK;
      break;
    }

  case TCPIP_PARAMID_V_TCP_UTOCHANGEABLE:
    {
      TcpIp_Tcp_UtoChangeable[TcpIdx] = (boolean)(OptVal);  /* enable or disable timeout value update upon received tiemout option */
      RetValue = E_OK;
      break;
    }
#endif  /* (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON) */

  default:
    {
      /* Unknown TCP socket option. Do nothing, return E_NOT_OK */
    }
  }

  return RetValue;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if TCPIP_TCP_CHECK_RX_CHECKSUM == STD_ON
/***********************************************************************************************************************
 *  TcpIp_Tcp_VCheckRecChecksum
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VCheckRecChecksum(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)               DataPtr,
    uint16                                                   LenByte )
{
  uint16_least Checksum;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  Checksum = (uint16_least)TcpIp_VCalcPseudoHdrAndChecksum(RxSockRemAddrPtr, RxSockLocAddrPtr, DataPtr, LenByte, TCPIP_SOCK_PROT_TCP);

  /* If the calculated checksum on the sender side was 0 and transmitted as 0xFFFF, no special treatment on receiver
     side is necessary. */
  if(0 == Checksum)
  {
    /* checksum is valid -> OK */
    return E_OK;
  }
  else
  {
    /* checksum is invalid -> drop message silently */
    return E_NOT_OK;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* TCPIP_TCP_CHECK_RX_CHECKSUM */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VSendFlags
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSendFlags(
    TcpIp_TcpIdxType TcpIdx,
    uint8 Flags )
{
  TcpIp_Tcp_TxFlags[TcpIdx] |= Flags;
  if(0 != (Flags & TCPIP_TCP_CODE_BIT_RST))
  {
    /* if a reset is issued, RST shall be the only flag used */
    TcpIp_Tcp_TxFlags[TcpIdx] = TCPIP_TCP_CODE_BIT_RST;
  }
  else
  {
    if((Flags & TCPIP_TCP_CODE_BIT_SYN) != 0)
    {
      TcpIp_Tcp_TxNextSendSeqNo[TcpIdx] = TcpIp_Tcp_SndNxt[TcpIdx];
    }
  }
#if ((TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON) && (TcpIp_Tcp_VCfgGetNagleTimeoutCycles() > 0))
  if((0 != (TcpIp_Tcp_TxFlags[TcpIdx] & TCPIP_TCP_CODE_BIT_MASK_FSA) ) && (TRUE == TcpIp_Tcp_NagleEnabled[TcpIdx]))
  {
    TcpIp_Tcp_NagleTimeout[TcpIdx] = 0;
  }
#endif
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VSendReset
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSendReset(
    TcpIp_TcpIdxType TcpIdx,
    TcpIp_IpVXAddrIdType                                       IpAddrId,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) RemSockPtr,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) LocSockPtr,
    uint32 SeqNo,
    uint32 AckNo,
    uint8 Flags )
{
  if(TCPIP_TCP_INV_IDX != TcpIdx)
  {
    /* - delete all elements in the tx request array (for this socket) */
    TcpIp_Tcp_TxReqArrayWritePos[TcpIdx] = 0;
    TcpIp_Tcp_TxReqArrayReadPos[TcpIdx] = 0;
    TcpIp_Tcp_TxReqArrayFillNum[TcpIdx] = 0;

    if( (TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx]) )
    {
    }
    else
    {
      /* - reset length to send */
      TcpIp_Tcp_TxLenByteTx[TcpIdx] = 0;

      if( (TCPIP_TCP_SOCK_STATE_SYNRCVD != TcpIp_Tcp_SockState[TcpIdx]) )
      {
        TcpIp_Tcp_RcvNxt[TcpIdx]  = AckNo;
      }
      TcpIp_Tcp_TxFlags[TcpIdx] = 0;
    }
  }

  /* fill reset queue (if not already full) */
  if((TcpIp_Tcp_VCfgGetTxRstQueueSize()) != TcpIp_Tcp_RstTxQueueFillNum)
  {
    TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].SeqNo    = SeqNo;
    TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].AckNo    = AckNo;
    TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].Flags    = Flags;
    TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].IpAddrId = IpAddrId;
    (void) IpBase_CopySockAddr((IpBase_SockAddrType *)&(TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].LocSock), LocSockPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    (void) IpBase_CopySockAddr((IpBase_SockAddrType *)&(TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueWritePos].RemSock), RemSockPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    TcpIp_Tcp_IncTxRstQueueIdx(TcpIp_Tcp_RstTxQueueWritePos);
    TcpIp_Tcp_RstTxQueueFillNum++;
  }
  else
  {
    /* queue is too short, or there is a problem with sending the elements */
    /* do nothing but ignore this request */
  }
}  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
#else
/***********************************************************************************************************************
 *  TcpIp_Tcp_VExistsUntransmittedSegmentInRetryQueue
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VExistsUntransmittedSegmentInRetryQueue(TcpIp_TcpIdxType TcpIdx)
{
  boolean                     Result          = FALSE;
  TcpIp_TcpTxRetrQueueIdxType RetrQueueEleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];

  while (TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != RetrQueueEleIdx)
  {
    if (0 == TcpIp_Tcp_TxRetrQueue[RetrQueueEleIdx].TransmitCounter)
    {
      Result = TRUE;
      break;
    }

    RetrQueueEleIdx = TcpIp_Tcp_TxRetrQueue[RetrQueueEleIdx].EleNext;
  }

  return Result;
} /* TcpIp_Tcp_VExistsUntransmittedSegmentInRetryQueue() */
#endif

/***********************************************************************************************************************
 *  TcpIp_Tcp_VFinalizeTxSegment
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VFinalizeTxSegment(
    TcpIp_TcpIdxType TcpIdx,
    boolean SrcIsMainFct )
{
  /* Function checks if a TxRetyQueue element is available, and then uses / allocates it. */

  /* accumulated DataLenByte may not exceed max payload length (minus TCP header length) of lower layer !! */
  TcpIp_Tcp_TxRetrQueueType *TxRetrQueueElemPtr;
  uint8                      segmentFlags; /*! TCP flags that shall be set in this segment. */

  /* Defer sending a FIN flag if pending TX data exceeds size of this segment.
   * FIN flag must be set in last segment containing data.
   */
  if (TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] > TcpIp_Tcp_TxLenByteTx[TcpIdx])
  {
    segmentFlags = (uint8)(TcpIp_Tcp_TxFlags[TcpIdx] & ~TCPIP_TCP_CODE_BIT_FIN);
  }
  else
  {
    segmentFlags = TcpIp_Tcp_TxFlags[TcpIdx];
  }

  TCPIP_ASSERT_MSG(TcpIp_Tcp_TxLenByteTx[TcpIdx] <= TcpIp_Tcp_TxMaxSegLenByte[TcpIdx], "segmenting is currently not supported."); /* this should never happen, this case is filtered in earlier steps */

  if((0 == segmentFlags) && (0 == TcpIp_Tcp_TxLenByteTx[TcpIdx]))
  {
    /* neither data nor flags to send -> do nothing.
     * ACK-only segments will be sent later if required and are not managed in the retry queue.
     */
    return E_OK;
  }

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if(0 != (segmentFlags & TCPIP_TCP_CODE_BIT_RST))
  {
    /* if a reset is issued, RST shall be the only flag used */
    segmentFlags = TCPIP_TCP_CODE_BIT_RST;
    TcpIp_Tcp_TxLenByteTx[TcpIdx] = 0;
  }
  else if( (TCPIP_TCP_CODE_BIT_SYN      != segmentFlags) &&
           (TCPIP_TCP_SOCK_STATE_CLOSED != TcpIp_Tcp_SockState[TcpIdx]) )
  {
    /* ACK flag is required in every segment except for SYN. */
    segmentFlags |= TCPIP_TCP_CODE_BIT_ACK;
  }
  else
  {
    /* nothing to do */
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if(0 == TcpIp_Tcp_NumFreeRetrQueueEle)
  {
    /* (global) list of TxRetrQueue elements is full */
    return E_NOT_OK;
  }
  /* since we only need one RetrQueue element, we can proceed this request */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  TcpIp_Tcp_VKeepAliveTrigger(TcpIdx);
#endif

  /* ------------------------------------------------------------------------------------------- */
  /* ------------------------------------------------------------------------------------------- */

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
  if ((0 == TcpIp_Tcp_TxLenByteTx[TcpIdx]) && (TCPIP_TCP_CODE_BIT_ACK == segmentFlags))
  {
    /* Only ACK segment shall be sent. This will be done at end of TcpIp_Tcp_VTransmit(). */
  }
#else
  if(  (0 != TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx])            /* RetryQueue is not empty */  /* PRQA S 3415 */ /* MD_TCPIP_12.4_3415 */
    && (0 == TcpIp_Tcp_TxLenByteTx[TcpIdx])                   /* there is no data to send */
    && (segmentFlags == TCPIP_TCP_CODE_BIT_ACK)  /* only a single ACK flag is triggered to be sent */
    && (TRUE == TcpIp_Tcp_VExistsUntransmittedSegmentInRetryQueue(TcpIdx)) )
  {
    /* prevent the TCP from sending a duplicate ACK / adding a second ACK into the Tx queue */
    /* no additional element is needed, there already is an ACK like the new one at the start of the queue */
    TcpIp_Tcp_TxFlags[TcpIdx] = 0;  /* delete ACK flag */
  }
#endif
  else
  {
    /* Create new retry queue element and append it the queue. */
    TcpIp_TcpTxRetrQueueIdxType NewRetrQueueEleIdx;

    {

      /* take the frist element from the free-list */
      NewRetrQueueEleIdx = TcpIp_Tcp_FirstFreeRetrQueueEle;
      if(1 != TcpIp_Tcp_NumFreeRetrQueueEle)
      {
        TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].EleNext].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;  /* maybe setting this is not necessary */
        TcpIp_Tcp_FirstFreeRetrQueueEle = TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].EleNext;
      }
      else
      {
        TcpIp_Tcp_FirstFreeRetrQueueEle = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
        TcpIp_Tcp_LastFreeRetrQueueEle  = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      }
      TcpIp_Tcp_NumFreeRetrQueueEle--;
    }

    /* append the (free) element to the sockets RetrQueue */
    if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_LastRetrQueueEle[TcpIdx])
    {
      /* there already is a retry list for this socket */
      TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_LastRetrQueueEle[TcpIdx]].EleNext = NewRetrQueueEleIdx;
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].ElePrev = TcpIp_Tcp_LastRetrQueueEle[TcpIdx];
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].TcpIdx  = TcpIdx;
      TcpIp_Tcp_LastRetrQueueEle[TcpIdx] = NewRetrQueueEleIdx;
    }
    else
    {
      /* there is no retry list for this socket, yet */
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx].TcpIdx  = TcpIdx;
      TcpIp_Tcp_FirstRetrQueueEle[TcpIdx] = NewRetrQueueEleIdx;
      TcpIp_Tcp_LastRetrQueueEle[ TcpIdx] = NewRetrQueueEleIdx;
    }

    TxRetrQueueElemPtr = &TcpIp_Tcp_TxRetrQueue[NewRetrQueueEleIdx];

    /* map TxReqArray elements to TxRetrQueue element */
    TxRetrQueueElemPtr->SeqNo             = TcpIp_Tcp_TxNextSendSeqNo[TcpIdx];
    TxRetrQueueElemPtr->Flags             = segmentFlags;

#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
    TxRetrQueueElemPtr->UserTimeoutTs     = TcpIp_Tcp_GlobalTimestamp + TcpIp_Tcp_Uto[TcpIdx];
#else
    TxRetrQueueElemPtr->UserTimeoutTs     = TcpIp_Tcp_GlobalTimestamp + TcpIp_Tcp_VCfgGetUserTimeoutDefCycles();
#endif

#if ((TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON) || (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON))
    if((0 != TcpIp_Tcp_TxOneTimeOpts[TcpIdx]) && (TcpIp_Tcp_TxLenByteTx[TcpIdx] <= (uint32)(TcpIp_Tcp_SockMtuSize[TcpIdx] - TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx])))
    {
      TxRetrQueueElemPtr->OneTimeOptions    = TcpIp_Tcp_TxOneTimeOpts[TcpIdx];
      TxRetrQueueElemPtr->OneTimeOptionsLen = TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx];
      TcpIp_Tcp_TxOneTimeOpts[TcpIdx]       = 0;  /* clear global flags and len */
      TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx]    = 0;
    }
    else
    {
      TxRetrQueueElemPtr->OneTimeOptions    = 0;
      TxRetrQueueElemPtr->OneTimeOptionsLen = 0;
    }
#else
    TxRetrQueueElemPtr->OneTimeOptions    = 0;
    TxRetrQueueElemPtr->OneTimeOptionsLen = 0;
#endif

    TxRetrQueueElemPtr->TransmitCounter   = 0;
    TxRetrQueueElemPtr->TimeStamp         = TcpIp_Tcp_GlobalTimestamp;
    TxRetrQueueElemPtr->SizeTotByte       = TcpIp_Tcp_TxLenByteTx[TcpIdx];
    {
      TcpIp_Tcp_TxReqArrayType* ReqPtr;
      ReqPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];

      {
        TxRetrQueueElemPtr->DataBufStartIdx = (ReqPtr->DataBufStartIdx + ReqPtr->QueuedLen) % (TcpIp_VCfgGetTxBufSizeByte(TcpIdx));

        if(TRUE == SrcIsMainFct)
        {
          uint32 unqueuedReqDataSize = (ReqPtr->DataLenByte - ReqPtr->QueuedLen);

          TCPIP_ASSERT(TcpIp_Tcp_TxLenByteTx[TcpIdx]       <= unqueuedReqDataSize);
          TCPIP_ASSERT(TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] == unqueuedReqDataSize);

          /* Use the only element of the ReqArray and inc the queued len */
          if(TcpIp_Tcp_TxLenByteTx[TcpIdx] == unqueuedReqDataSize)
          {
            ReqPtr->FullyPlanned = TRUE;
          }

          ReqPtr->QueuedLen += TcpIp_Tcp_TxLenByteTx[TcpIdx];
          TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] -= TcpIp_Tcp_TxLenByteTx[TcpIdx];
        }
      }
    }

    TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx]++;

    TcpIp_Tcp_TxNextSendSeqNo[TcpIdx] += TcpIp_Tcp_TxLenByteTx[TcpIdx];

    if( 0 != (TCPIP_TCP_CODE_BIT_SYN & segmentFlags))
    {
      TcpIp_Tcp_TxNextSendSeqNo[TcpIdx]++;
    }

    if( 0 != (TCPIP_TCP_CODE_BIT_FIN & segmentFlags))
    {
      TcpIp_Tcp_TxNextSendSeqNo[TcpIdx]++;
    }

    /* Clear pending flags if they were set in this segment.
     * The ACK flag will be handled later in VTransmit(). 
     */
    TcpIp_Tcp_TxFlags[TcpIdx] &= (uint8)~(segmentFlags & ~TCPIP_TCP_CODE_BIT_ACK);

    TcpIp_Tcp_TxLenByteTx[TcpIdx] = 0;
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VTransmitSegment
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmitSegment(
  TcpIp_TcpIdxType tcpIdx, 
  uint8            flags, 
  uint32           seqNo, 
  uint8            oneTimeOptions, 
  uint32           dataStartPos, 
  uint32           dataLenByte)
{
  Std_ReturnType      retValue = E_NOT_OK;
  TcpIp_SockIdxType   sockIdx  = TCPIP_TCPIDX2SOCKIDX(tcpIdx);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8               headerLenByte;
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
  /* Init this variable 'sackNum' to prevent MISRA checkers to issue the warning message 3353. The variable will always be set to a valid value before it is used */
  uint8_least         sackNum = 0;  /* number of SACK elements to be sent */
#endif

  /* #50 Calculate TCP header length (based on required options). */
  headerLenByte = TCPIP_TCP_HDR_LEN_BYTE;
  if(0 != (TCPIP_TCP_CODE_BIT_SYN & flags))
  {
    /* send option for max segment size -> additional length */
    headerLenByte += TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE;
  }

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
  if( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[tcpIdx]) ||
                              (TRUE == TcpIp_Tcp_TimestampOptActive[tcpIdx]) )
  {
    /* send timestamp option -> additional length */
    headerLenByte += (TCPIP_TCP_OPT_LEN_TIMESTAMP +2);
  }
#endif

  if(0 != oneTimeOptions)
  {
    /* there are additional options to send -> additional length */
#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
    if(0 != (oneTimeOptions & TCPIP_TCP_OPT_FLAG_USER_TIMEOUT))
    {
      headerLenByte += TCPIP_TCP_OPT_LEN_USER_TIMEOUT;
    }
#endif
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
    /* only allowed in the SYN messages */
    if(0 != (oneTimeOptions & TCPIP_TCP_OPT_FLAG_SACK_PERMIT))
    {
      headerLenByte += (TCPIP_TCP_OPT_LEN_SACK_PERMIT +2);  /* including 2x NOP */
    }

#endif
  }

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
  if((0 != TcpIp_Tcp_OooNumElements[tcpIdx]) && (TcpIp_Tcp_SackOptActive[tcpIdx] == TRUE))
  {
    uint8       headerRemaining;
    uint8_least sackNumMax;
    uint8_least oooNum;
    headerLenByte += 4;  /* 2* NOP + kind + len */
    headerRemaining = (uint8)(60u - headerLenByte);
    sackNumMax = headerRemaining / 8;
    oooNum = TcpIp_Tcp_OooNumElements[tcpIdx];

    /* set currently number of used SACK elements */
    if(sackNumMax >= oooNum)
    {
      sackNum = oooNum;
    }
    else
    {
      sackNum = sackNumMax;
    }

    headerLenByte += (uint8)(sackNum *8);  /* SACK elements */
  }
#endif

  {
    uint16                           tgtLen = (uint16)dataLenByte + headerLenByte;
    TcpIp_IpTxRequestDescriptorType  ipTxReqestDescr;
    uint8                           *payloadPtr;

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
    CANOE_WRITE_STRING_3("TcpIp_Tcp_VTransmit: TcpIdx %d: TcpIp_Tcp_Cwnd is now %d, call IP ProvTxBuffer with length %d", TcpIdx, TcpIp_Tcp_Cwnd[TcpIdx], TgtLen);
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(IPBASE_AF_INET == ((IpBase_SockAddrType*)&(TcpIp_SocketList[sockIdx].RemSock))->sa_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
# endif
    {
      /* V4 */
      if ( BUFREQ_OK != IpV4_Ip_ProvideTxBuffer(&TcpIp_SocketList[sockIdx].TxIpAddrId, sockIdx, (IpBase_SockAddrInType *)&TcpIp_SocketList[sockIdx].RemSock, (uint8**)&payloadPtr, &tgtLen, &ipTxReqestDescr, FALSE) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* stop trying to get more buffers */
        return retValue;
      }

      if( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[tcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[tcpIdx]) )
      {
        /* store source address used in received or sent SYN segment. The source address must noch change until the TCP connection is closed. */
        (void)IpV4_Ip_GetLocNetAddr(TcpIp_SocketList[sockIdx].TxIpAddrId, &((IpBase_SockAddrInType *)&TcpIp_SocketList[sockIdx].LocSock)->sin_addr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        ((IpBase_SockAddrInType *)&TcpIp_SocketList[sockIdx].LocSock)->sin_family = IPBASE_AF_INET;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      }
    }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    {
      /* V6 */
      if( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[tcpIdx]) ||
          (TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[tcpIdx]) )
      {
        if ( BUFREQ_OK != IpV6_ProvideTxBuffer(&TcpIp_SocketList[sockIdx].TxIpAddrId, sockIdx,
          (IpV6_AddrType*)&(((IpBase_SockAddrIn6Type*)&(TcpIp_SocketList[sockIdx].RemSock))->sin6_addr),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
          (uint8**)&payloadPtr, &tgtLen, &ipTxReqestDescr, FALSE) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        {
          /* stop trying to get more buffers */
          return retValue;
        }
      }
      else
      {
        if ( BUFREQ_OK != IpV6_ProvideTxBuffer(&TcpIp_SocketList[sockIdx].TxIpAddrId, sockIdx,
          (IpV6_AddrType*)&(((IpBase_SockAddrIn6Type*)&(TcpIp_SocketList[sockIdx].RemSock))->sin6_addr),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
          (uint8**)&payloadPtr, &tgtLen, &ipTxReqestDescr, FALSE) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        {
          /* stop trying to get more buffers */
          return retValue;
        }
      }

      if( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[tcpIdx]) ||
        (TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[tcpIdx]) )
      {
        /* Determine the identifier of the ip address that has been selected by the IpV6.
         * If IpV6_ProvideTxBuffer has been called with an AutoSelect ip address identifier,
         * this address identifier will be replaced by an identifier for a specific source address.
         */
        ((IpBase_SockAddrIn6Type *)&TcpIp_SocketList[sockIdx].LocSock)->sin6_family = IPBASE_AF_INET6;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        (void)IpV6_GetLocalAddress(TcpIp_SocketList[sockIdx].TxIpAddrId, (IpBase_AddrIn6Type *)&(((IpBase_SockAddrIn6Type *)&TcpIp_SocketList[sockIdx].LocSock)->sin6_addr), NULL_PTR);
      }
    }
#endif

    {
      /* set header pointer */
      TcpIp_Tcp_HdrType *tcpHdr = (TcpIp_Tcp_HdrType *) payloadPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

      /* copy payload */
      if(0 != dataLenByte)
      {
        uint8  *tempPayloadPtr;

        tempPayloadPtr = &(((uint8 *)(tcpHdr))[headerLenByte]);  /* first byte following the TCP header */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

        if((dataStartPos + dataLenByte) > TcpIp_VCfgGetTxBufSizeByte(tcpIdx))
        {
          /* message is in end and start of ring buffer (splitted into two parts) */
          uint32 len1;
          uint32 len2;
          IpBase_CopyDataType *src1Ptr;
          IpBase_CopyDataType *src2Ptr;

          len1    = TcpIp_VCfgGetTxBufSizeByte(tcpIdx) - dataStartPos;
          len2    = dataLenByte - len1;
          src1Ptr = (IpBase_CopyDataType *)(TcpIp_GetTxBuffersElemPtr(sockIdx, dataStartPos));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
          src2Ptr = (IpBase_CopyDataType *)(TcpIp_GetTxBuffersElemPtr(sockIdx, 0));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

          /* copy the two data fragments */
          IpBase_Copy((IpBase_CopyDataType *)tempPayloadPtr, src1Ptr, len1);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
          IpBase_Copy((IpBase_CopyDataType *)(&tempPayloadPtr[len1]), src2Ptr, len2);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
        }
        else
        {
          /* message is in the ring buffer in one block (unsplitted) */
          IpBase_CopyDataType *srcPtr = (IpBase_CopyDataType *)(TcpIp_GetTxBuffersElemPtr(sockIdx, dataStartPos));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

          /* copy this data (fragment) */
          /* since the data is in one block, no Pbuf copy mechanism is necessary */
          IpBase_Copy((IpBase_CopyDataType *)tempPayloadPtr, srcPtr, dataLenByte);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
        }
      }  /* if(0 != DataLenByte) */

      /* fill header */
      tcpHdr->SrcPort          = ((IpBase_SockAddrInType *)&(TcpIp_SocketList[sockIdx].LocSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      tcpHdr->TgtPort          = ((IpBase_SockAddrInType *)&(TcpIp_SocketList[sockIdx].RemSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

      tcpHdr->SeqNo            = IPBASE_HTON32(seqNo);
      tcpHdr->AckNo            = IPBASE_HTON32(TcpIp_Tcp_RcvNxt[tcpIdx]);  /* acknowledge received bytes */

      tcpHdr->DataOffsetAndRes = 0x50;
      tcpHdr->CodeBits         = flags;
      if(0 != dataLenByte)
      {
        tcpHdr->CodeBits |= TCPIP_TCP_CODE_BIT_PSH;
      }
      tcpHdr->Window           = IPBASE_HTON16(TcpIp_Tcp_RcvWnd[tcpIdx]);
      tcpHdr->Checksum         = TCPIP_TCP_EMPTY_CHKSUM;
      tcpHdr->UrgentPtr        = 0;
      {
        uint8 *hdrOptPtr = (uint8*)(&(tcpHdr[1]));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        uint8  hdrOptLen = 0;

        if(0 != (TCPIP_TCP_CODE_BIT_SYN & flags))
        {
          /* append TCP option to the header */
          TcpIp_Tcp_HdrMssOptType *tempHdrOptPtr = (TcpIp_Tcp_HdrMssOptType *)hdrOptPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
          tempHdrOptPtr->Kind   = TCPIP_TCP_OPT_ID_MAX_SEG_SIZE;
          tempHdrOptPtr->OptLen = TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE;
          {
            /* max size depends on socket rx buffer size and ETH rx message size (minus header) */
            uint16 MaxLen = TcpIp_Tcp_VCfgGetMaxRxMsgSizeByte();

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
            if (IPBASE_AF_INET6 == ((TCPIP_P2C(IpBase_SockAddrType))&(TcpIp_SocketList[sockIdx].RemSock))->sa_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
            {
              /* Fixed IPv6 header requires 20 bytes more than already considered IPv4 header. So TCP MSS has to be reduced by 20 bytes. */
              MaxLen -= (TCPIP_IPV6_HDR_LEN_BYTE - TCPIP_IPV4_HDR_LEN_BYTE);
            }
#endif

            if(MaxLen > TcpIp_Tcp_AssignedRxBufferSize[tcpIdx])
            {
              /* MSS is limited by small TCP RX buffer */
              MaxLen = (uint16)TcpIp_Tcp_AssignedRxBufferSize[tcpIdx];
            }

            tempHdrOptPtr->SegLen = IPBASE_HTON16(MaxLen);  /* local max for this socket (for rx) */
          }

          hdrOptLen += TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE;
          hdrOptPtr  = &hdrOptPtr[hdrOptLen];  /* PRQA S 3198 */ /* MD_TCPIP_3198 */
        }

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
        if( (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[tcpIdx]) ||
          (TRUE == TcpIp_Tcp_TimestampOptActive[tcpIdx]) )
        {
          uint8 *tempHdrOptPtr;  /* TcpIp_Tcp_HdrTsOptType */
          uint8 tempOptLen;
          /* add two NOP elements */
          hdrOptPtr[0] = TCPIP_TCP_OPT_ID_NOP;
          hdrOptPtr[1] = TCPIP_TCP_OPT_ID_NOP;

          /* add timestamp header */
          tempHdrOptPtr = &hdrOptPtr[2];
          tempHdrOptPtr[0]                = TCPIP_TCP_OPT_ID_TIMESTAMP;  /* Kind */
          tempHdrOptPtr[1]                = TCPIP_TCP_OPT_LEN_TIMESTAMP;  /* OptLen */
          *((uint32*)(&tempHdrOptPtr[2])) = IPBASE_HTON32(TcpIp_Tcp_GlobalTimestamp);  /* TSval */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
          *((uint32*)(&tempHdrOptPtr[6])) = IPBASE_HTON32(TcpIp_Tcp_TimestampReceived[tcpIdx]);  /* TSecr */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

          tempOptLen = (TCPIP_TCP_OPT_LEN_TIMESTAMP +2);
          hdrOptLen += tempOptLen;
          hdrOptPtr  = &hdrOptPtr[tempOptLen];  /* PRQA S 3198 */ /* MD_TCPIP_3198 */
        }
#endif

#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
        if(0 != (oneTimeOptions & TCPIP_TCP_OPT_FLAG_USER_TIMEOUT))
        {
          uint8 *tempHdrOptPtr;
          uint8  tempOptLen;
          /* add timestamp header */
          tempHdrOptPtr = &hdrOptPtr[0];
          tempHdrOptPtr[0]                = TCPIP_TCP_OPT_ID_USER_TIMEOUT;  /* Kind */
          tempHdrOptPtr[1]                = TCPIP_TCP_OPT_LEN_USER_TIMEOUT;  /* OptLen */
          *((uint16*)(&tempHdrOptPtr[2])) = IPBASE_HTON16(TcpIp_Tcp_UtoAdv[tcpIdx]);  /* Value */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

          tempOptLen = (TCPIP_TCP_OPT_LEN_USER_TIMEOUT);
          hdrOptLen += tempOptLen;
          hdrOptPtr  = &hdrOptPtr[tempOptLen];  /* PRQA S 3198 */ /* MD_TCPIP_3198 */
        }
#endif

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
        if(0 != (oneTimeOptions & TCPIP_TCP_OPT_FLAG_SACK_PERMIT))
        {
          uint8 *tempHdrOptPtr;
          uint8  tempOptLen;
          /* add SACK permit option */
          tempHdrOptPtr = &hdrOptPtr[0];
          tempHdrOptPtr[0]                = TCPIP_TCP_OPT_ID_SACK_PERMIT;  /* Kind */
          tempHdrOptPtr[1]                = TCPIP_TCP_OPT_LEN_SACK_PERMIT;  /* OptLen */

          tempHdrOptPtr[2] = TCPIP_TCP_OPT_ID_NOP;
          tempHdrOptPtr[3] = TCPIP_TCP_OPT_ID_NOP;

          tempOptLen = (TCPIP_TCP_OPT_LEN_SACK_PERMIT +2);
          hdrOptLen += tempOptLen;
          hdrOptPtr  = &hdrOptPtr[tempOptLen];
        }

        if((0 != TcpIp_Tcp_OooNumElements[tcpIdx]) && (TcpIp_Tcp_SackOptActive[tcpIdx] == TRUE))
        {
          /* send list with SackNum (1..4) elements */
          uint8       *tempHdrOptPtr;
          uint8        tempOptLen;
          uint32      *seqPtr;
          uint16       oooEleIdx;
          uint8_least  sackNumAdded;

          /* add SACK option */
          tempHdrOptPtr = &hdrOptPtr[0];

          tempHdrOptPtr[0] = TCPIP_TCP_OPT_ID_NOP;
          tempHdrOptPtr[1] = TCPIP_TCP_OPT_ID_NOP;

          tempHdrOptPtr[2]                = TCPIP_TCP_OPT_ID_SACK;  /* Kind */
          tempHdrOptPtr[3]                = (uint8)(TCPIP_TCP_OPT_LEN_SACK + (sackNum *8));  /* OptLen */  /* PRQA S 3353 */ /* MD_TCPIP_TCP_9.1_3353_b */ /* variable is always initialized ! */

          /* add first SACK element */
          if(TcpIp_Tcp_OooLastChangedEleIdx[tcpIdx] != 0xFFFF)
          {
            oooEleIdx = TcpIp_Tcp_OooLastChangedEleIdx[tcpIdx];
          }
          else
          {
            oooEleIdx = TcpIp_Tcp_OooFirstEleIdx[tcpIdx];
          }
          seqPtr  = (uint32*)&tempHdrOptPtr[4];  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
          *seqPtr = IPBASE_HTON32(TcpIp_Tcp_RxPreBufArray[oooEleIdx].SeqNo);
          seqPtr  = &seqPtr[1];
          *seqPtr = IPBASE_HTON32(TcpIp_Tcp_RxPreBufArray[oooEleIdx].SeqNo + TcpIp_Tcp_RxPreBufArray[oooEleIdx].LenByte);
          seqPtr  = &seqPtr[1];
          sackNumAdded = 1;

          /* add further SACK elements */
          oooEleIdx = TcpIp_Tcp_OooFirstEleIdx[tcpIdx];
          while(sackNumAdded < sackNum)  /* PRQA S 3347 */ /* MD_TCPIP_TCP_9.1_3347_a */
          {
            if((TcpIp_Tcp_OooLastChangedEleIdx[tcpIdx] != 0xFFFF) && (oooEleIdx == TcpIp_Tcp_OooLastChangedEleIdx[tcpIdx]))
            {
              /* skip this element, it is already handled in the first SACK option element */
              oooEleIdx = TcpIp_Tcp_RxPreBufArray[oooEleIdx].NextEleIdx;
            }
            *seqPtr = IPBASE_HTON32(TcpIp_Tcp_RxPreBufArray[oooEleIdx].SeqNo);
            seqPtr  = &seqPtr[1];
            *seqPtr = IPBASE_HTON32(TcpIp_Tcp_RxPreBufArray[oooEleIdx].SeqNo + TcpIp_Tcp_RxPreBufArray[oooEleIdx].LenByte);
            seqPtr  = &seqPtr[1];

            oooEleIdx = TcpIp_Tcp_RxPreBufArray[oooEleIdx].NextEleIdx;
            sackNumAdded++;
          }

          tempOptLen = (uint8)(TCPIP_TCP_OPT_LEN_SACK + 2 +(sackNum *8));
          hdrOptLen += tempOptLen;
          /* HdrOptPtr  = &HdrOptPtr[TempOptLen]; */ /* This assignment can be skipped if this is the last possible header option */
        }
#endif

        if(0 != hdrOptLen)
        {
          /* there were some options added */
          TCPIP_ASSERT_MSG((hdrOptLen % 4) == 0, "Total option length must always be a multiple of 4.");
          tcpHdr->DataOffsetAndRes += (uint8)((hdrOptLen / 4) << 4);
        }
      }

      dataLenByte  += headerLenByte;

      /* checksum will be calculated by lower layer, value is already set to '0' */

      /* transmit */
      retValue = TcpIp_VIpTransmit(&ipTxReqestDescr, (uint16)dataLenByte, TCPIP_PROTOCOL_TCP, FALSE, 0, FALSE, (TCPIP_P2C(IpBase_SockAddrType))&TcpIp_SocketList[sockIdx].RemSock);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    }
  }

  return retValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010, 6030, 6060 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VTransmit
 **********************************************************************************************************************/
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmit(
    TcpIp_TcpIdxType TcpIdx )
{
  Std_ReturnType      RetValue = E_NOT_OK;
  uint32              DataLenByte;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  TcpIp_SockIdxType   SockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint16              RetrQueueEleIdx;
  boolean             SetNewRtoPending = FALSE;
#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
  uint16              RetrQueueEleCount = 0;
#endif

#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
  boolean             dataSegmentSent = FALSE;
#endif

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Check user timeout of oldest element in retry queue and close connection if timeout has exceeded. */

  if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX == TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
  {
    RetrQueueEleIdx = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
  }
  else
  {
    RetrQueueEleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
  }

  if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
  {
    if(TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].UserTimeoutTs == TcpIp_Tcp_GlobalTimestamp)
    {
      /* The first element in the RetryQueue has not been ACKed too long (UserTimeout), so cancel the connection */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
      uint8 SockUserIdx = TcpIp_SockUserIdx[SockIdx];

      /* send reset */
      TcpIp_Tcp_VSendReset( TcpIdx, TcpIp_SocketList[SockIdx].TxIpAddrId, (IpBase_SockAddrType*)&(TcpIp_SocketList[SockIdx].RemSock),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        (IpBase_SockAddrType*)&(TcpIp_SocketList[SockIdx].LocSock),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].SeqNo, 0, 0 );

      /* enter CLOSED state, delete/clear TCB */
      TcpIp_Tcp_VRestartSock(TcpIdx);
      TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
      /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
      TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif

      TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
      return E_NOT_OK;
    }
  }

  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Iterate over all elements in retry queue and send them if necessary. */
  while(RetrQueueEleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX)
  {
    TcpIp_Tcp_TxRetrQueueType *TxRetrQueueElemPtr;

    TxRetrQueueElemPtr = &TcpIp_Tcp_TxRetrQueue[RetrQueueEleIdx];
#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
    RetrQueueEleCount++;
#endif

#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
    /* Each segment in the queue must have payload data or be a SYN/FIN segment. */
    TCPIP_ASSERT((TxRetrQueueElemPtr->SizeTotByte > 0) || ((TxRetrQueueElemPtr->Flags & (TCPIP_TCP_CODE_BIT_SYN | TCPIP_TCP_CODE_BIT_FIN)) != 0));
#endif

/* ----- begin check if the segment shall be sent ------------------------------------------------- */
/* ----- begin check if the segment shall be sent ------------------------------------------------- */
/* ----- begin check if the segment shall be sent ------------------------------------------------- */
/* ----- begin check if the segment shall be sent ------------------------------------------------- */
/* ----- begin check if the segment shall be sent ------------------------------------------------- */

    if(TCPIP_TCP_SOCK_STATE_INVALID == TcpIp_Tcp_SockState[TcpIdx])
    {
      /* delete complete TxQueue */
      TcpIp_Tcp_VDelTxQueue(TcpIdx);

      break;  /* skip the rest of this while loop */
    }

    if( (0 == TxRetrQueueElemPtr->TransmitCounter) || (0 == TcpIp_Tcp_RetransmitTimeout[TcpIdx])  /* counter is 0 (new un-sent element), or global timeout expired, so send the element content */
#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
        || (TRUE == TcpIp_Tcp_FastRetransmitPending[TcpIdx])  /* fast retransmit pending */
#endif
      )
    {
      /* Segment was not transmitted or has to be retransmitted due to a retransmit timeout or a fast retransmit condition. */

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)

      /* #30 Adjust congestion control parameters. */

      if(0 != TxRetrQueueElemPtr->TransmitCounter)
      {
        /* Segment has to be retransmitted. */

        if(TRUE == TcpIp_Tcp_FastRetransmitPending[TcpIdx])
        {
          TcpIp_Tcp_FastRetransmitPending[TcpIdx] = FALSE;  /* reset value */
        }
        else
        {
          /* this is a re-transmit -> we probably have congestion */
          /* same handling for SYN and non-SYN messages */
          TcpIp_Tcp_Cwnd[TcpIdx] = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];   /* cwnd = LW = 1*SMSS */

          if(0 == (TCPIP_TCP_CODE_BIT_SYN & TxRetrQueueElemPtr->Flags))
          {
            if(1 == TxRetrQueueElemPtr->TransmitCounter)
            {
              TcpIp_Tcp_VSetSsthreshEqu4(TcpIdx);  /* ssthresh = max[FlightSize / 2, 2*SMSS] */
              CANOE_WRITE_STRING_1("TcpIp_Tcp_VTransmit: ssthresh was set to %d", TcpIp_Tcp_Ssthresh[TcpIdx]);
            }
          }
          else
          {
            /* SYN flag is set in this message */
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
            /* trigger reset of RTO for this connection on transition to state 'ESTABLISHED' (set to 3s) */
            TcpIp_Tcp_RtoResetPending[TcpIdx] = TRUE;
#endif

            TcpIp_Tcp_CwndSegNumLimit[TcpIdx] = 1;
          }

          /* RFC 6582, 3.2 4) */
          {
            TcpIp_TcpTxRetrQueueIdxType EleIdx;
            TcpIp_TcpTxRetrQueueIdxType LastEleIdx;

            /* check TransmitCounter of all retry queue elements for this socket */
            LastEleIdx = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
            EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
            while((EleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX) && (TcpIp_Tcp_TxRetrQueue[EleIdx].TransmitCounter != 0))
            {
              LastEleIdx = EleIdx;
              EleIdx      = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
            }
            if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != LastEleIdx)
            {
              TcpIp_Tcp_Recover[TcpIdx] = (TcpIp_Tcp_TxRetrQueue[LastEleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[LastEleIdx].SizeTotByte) -1;
            }
          }

          /* eventually exit fast recovery procedure */
          TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_SLOW_START;
        }
      }
      else
      {
        /* this is a new element that shall be sent */
      }

      if((TcpIp_Tcp_CwndSegNumLimit[TcpIdx] != 0) && (RetrQueueEleCount > TcpIp_Tcp_CwndSegNumLimit[TcpIdx]))
      {
        /* cwnd is set to an initial value, so only a limited number of segments may be sent */
        /* limit exceeded -> skip this element (and the following ones) */
      }
      else if( TCPIP_TCP_SEQ1_BIGGER_SEQ2((TxRetrQueueElemPtr->SeqNo + TxRetrQueueElemPtr->SizeTotByte),
          (TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].SeqNo + TcpIp_Tcp_Cwnd[TcpIdx] + (TcpIp_Tcp_DupAckCnt[TcpIdx] * TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]))) )
          /* (current element end sequence number) > (first element sequence number + cwnd + (x * SMSS)) */
      {
        /* Element does not fit into cwnd, so it can't be sent. Stop sending elements following the current one, since they won't fit into cwnd, neither. */
        /* skip this element */
        break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_b */
      }
      else
#endif
      {
        /* try to send the segment */
        DataLenByte = TxRetrQueueElemPtr->SizeTotByte;

        /* #40 Skip segment if it does not fit into the send window. */
        if(0 != DataLenByte)
        {
          /* check if tx window is big enough */
          if(0 != TcpIp_Tcp_SndWnd[TcpIdx])
          {
            if( (TCPIP_TCP_SEQ1_BIGGER_SEQ2((DataLenByte + TxRetrQueueElemPtr->SeqNo),
                (TcpIp_Tcp_SndWl2[TcpIdx] + TcpIp_Tcp_SndWnd[TcpIdx]))) )
            {
              /* tx window is too small for sending this message */
              break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_b */
            }
          }
          else
          {
            /* the send window has size '0' */
            if(RetrQueueEleIdx == TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
            {
              if(TxRetrQueueElemPtr->SizeTotByte == 1)
              {
                /* this is a zero window probe, just go ahead, so nothing here */
              }
              else if (0 == TcpIp_Tcp_RetransmitTimeout[TcpIdx])
              {
                /* the first element in the TxRetrQueue has to be split into 1 byte and the rest */
                TcpIp_Tcp_VSplitFirstRetrQueueEle(TcpIdx);
                DataLenByte = TxRetrQueueElemPtr->SizeTotByte;
              }
              else
              {
                break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_b */
              }
            }
            else
            {
              /* this is not a zero window probe element */
              break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_b */
            }
          }

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
          /* check if segment fits in cwnd */
          if(TCPIP_TCP_SEQ1_BIGGER_SEQ2((DataLenByte + TxRetrQueueElemPtr->SeqNo), (TcpIp_Tcp_SndWl2[TcpIdx] + TcpIp_Tcp_Cwnd[TcpIdx])))
          {
            /* cwnd does not allow to send this segment */
            break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_b */
          }
#endif
        }

/* ----- end check if the segment shall be sent ------------------------------------------------- */
/* ----- end check if the segment shall be sent ------------------------------------------------- */
/* ----- end check if the segment shall be sent ------------------------------------------------- */
/* ----- end check if the segment shall be sent ------------------------------------------------- */
/* ----- end check if the segment shall be sent ------------------------------------------------- */

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
        if(RetrQueueEleIdx == TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
        {
          TxRetrQueueElemPtr->SackFlag = 0;  /* delete SACK-flag, always retransmit the first element in the RetrQueue */
        }

        if(0 == TxRetrQueueElemPtr->SackFlag)
#endif
        {
          /* #60 Transmit segment. */
          if (TcpIp_Tcp_VTransmitSegment(TcpIdx, TxRetrQueueElemPtr->Flags, TxRetrQueueElemPtr->SeqNo, TxRetrQueueElemPtr->OneTimeOptions, TxRetrQueueElemPtr->DataBufStartIdx, TxRetrQueueElemPtr->SizeTotByte) == E_OK)
          {
            RetValue = E_OK;
#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
            dataSegmentSent = TRUE;
#endif

            if (TxRetrQueueElemPtr->TransmitCounter == 0)
            {
              /* "When the sender creates a segment and transmits it the sender advances SND.NXT."
               * [RFC793 3.7. Data Communication]
               */
              uint32 sndNxt = TxRetrQueueElemPtr->SeqNo + TxRetrQueueElemPtr->SizeTotByte;
              
              if ((TxRetrQueueElemPtr->Flags & TCPIP_TCP_CODE_BIT_SYN) != 0)
              {
                sndNxt++;
              }

              if ((TxRetrQueueElemPtr->Flags & TCPIP_TCP_CODE_BIT_FIN) != 0)
              {
                sndNxt++;
              }

              if (TCPIP_TCP_SEQ1_BIGGER_SEQ2(sndNxt, TcpIp_Tcp_SndNxt[TcpIdx]))
              {
                TcpIp_Tcp_SndNxt[TcpIdx] = sndNxt;
              }
              else
              {
                /* Avoid setting TcpIp_Tcp_SndNxt to smaller value if segment was recerated due to PathMTU change (TransmitCounter is reset to 0 in that case). */
              }
            }
          }
        }
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
        else
        {
          /* This element is sacked -> skip this retry once */
          TxRetrQueueElemPtr->SackFlag = 0; /* clear the SACK flag so that the retry will be issued the next time the retransmit-timeout occures */
          RetValue = E_OK;
        }
#endif

        if(E_OK == RetValue)
        {
          /* transmission was successful */

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
          TcpIp_Tcp_IdleTimeoutShort[TcpIdx] = TcpIp_Tcp_RtoValid[TcpIdx] +2;  /* reset the idle timer on each sent segment */
#endif

          TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

          if( (1 == TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx]) &&
            (0 == TxRetrQueueElemPtr->SizeTotByte) &&
            (TCPIP_TCP_CODE_BIT_ACK == TxRetrQueueElemPtr->Flags) )
          {
            /* Only one element in retry queue, and the element only contains an ACK without data. Remove this element from list */
            TcpIp_Tcp_VFreeFirstRetrQueueEle(TcpIdx);
          }
          else
          {
            /* reload retransmission timeout only for retransmissions of the first element in the RetrQueue */
            if(RetrQueueEleIdx == TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
            {
              /* trigger reset of RTO */
              SetNewRtoPending = TRUE;
            }
            TxRetrQueueElemPtr->TransmitCounter++;
          }

          TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
        else
        {
          /* This is an error condition, caused by the lower layer (IP, EthIf...). */
          /* Buffer is freed in the EthIf or Eth, but nothing was sent. So do not remove this segment
          from retransmission queue. */
          TCPIP_DUMMY_ASM_NOP
        }
      }
    }
    else
    {
      /* nothing to do, skip this element */
    }

    RetrQueueEleIdx = TxRetrQueueElemPtr->EleNext;
  }  /* while( all RetrQueue elements ) */

  if(SetNewRtoPending == TRUE)
  {
    uint32 NewTimeout;

    /* only for the first RetrQueue element */
    RetrQueueEleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
    if( (TCPIP_TCP_TX_RETR_QUEUE_INV_IDX == RetrQueueEleIdx) || (1 == TcpIp_Tcp_TxRetrQueue[RetrQueueEleIdx].TransmitCounter) )
    {
      /* first setting of the retransmit timer */
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
      NewTimeout = TcpIp_Tcp_RtoValid[TcpIdx];
#else
      NewTimeout = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
#endif
    }
    else
    {
      /* there has been at least one retransmission already */
      NewTimeout = TcpIp_Tcp_RtoReloadValue[TcpIdx] *2;
      if(NewTimeout > TcpIp_Tcp_VCfgGetTxRetryIntMaxCycles())
      {
        NewTimeout = TcpIp_Tcp_VCfgGetTxRetryIntMaxCycles();
      }
    }
    TcpIp_Tcp_RetransmitTimeout[TcpIdx] = NewTimeout;
    TcpIp_Tcp_RtoReloadValue[TcpIdx]    = NewTimeout;

    /* SetNewRtoPending = FALSE; */
  }

  if(0 != TcpIp_Tcp_RetransmitTimeout[TcpIdx])
  {
    TcpIp_Tcp_RetransmitTimeout[TcpIdx]--;
  }

#if (TCPIP_TCP_DO_NOT_QUEUE_ACKS == STD_ON)
  if (TCPIP_TCP_CODE_BIT_ACK == TcpIp_Tcp_TxFlags[TcpIdx])
  {
    if (dataSegmentSent == FALSE)
    {
      /* an ACK shall be sent */
      if (E_OK == TcpIp_Tcp_VTransmitSegment(TcpIdx, TCPIP_TCP_CODE_BIT_ACK, TcpIp_Tcp_SndNxt[TcpIdx], 0, 0, 0))
      {
        TcpIp_Tcp_TxFlags[TcpIdx] = 0;
      }
    }
    else
    {
      /* ACK was already sent with data segment. */
      TcpIp_Tcp_TxFlags[TcpIdx] = 0;
    }
  }
#endif

  return RetValue;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VTransmitResets
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VTransmitResets( void )
{
  /* calling this function will send all queued reset messages onto the bus !! */

  Std_ReturnType            RetValue;
  TcpIp_Tcp_HdrType        *HdrPtr;
  uint16                    TgtLen;
  TcpIp_SockIdxType         SockIdx = TcpIp_Tcp_VCfgGetSocketIdxRstTx();  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  TcpIp_Tcp_RstTxQueueType *RstQueuePtr;
  TcpIp_IpTxRequestDescriptorType IpTxReqestDescr;

  while(0 != TcpIp_Tcp_RstTxQueueFillNum)
  {
    RstQueuePtr = &(TcpIp_Tcp_RstTxQueue[TcpIp_Tcp_RstTxQueueReadPos]);

    if (IPBASE_AF_UNSPEC == ((IpBase_SockAddrType*)&(RstQueuePtr->RemSock))->sa_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    {
      /* remove elements that have destination addresses marked as unreachable by TcpIp_Tcp_VAddrResTimeout() */
      TcpIp_Tcp_RstTxQueueFillNum--;
      TcpIp_Tcp_IncTxRstQueueIdx(TcpIp_Tcp_RstTxQueueReadPos);
      continue;  /* PRQA S 0770 */ /* MD_TCPIP_1.1_0770 */
    }

    TgtLen = TCPIP_TCP_HDR_LEN_BYTE;
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(IPBASE_AF_INET == ((IpBase_SockAddrType*)&(RstQueuePtr->RemSock))->sa_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#endif
    {
      /* V4 */
      if ( BUFREQ_OK != IpV4_Ip_ProvideTxBuffer(&RstQueuePtr->IpAddrId, SockIdx, (IpBase_SockAddrInType *)&RstQueuePtr->RemSock, ((uint8 **)(&HdrPtr)), &TgtLen, &IpTxReqestDescr, FALSE) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
      {
        break;
      }
    }
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    else
#endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    {
      /* V6 */
      if ( BUFREQ_OK != IpV6_ProvideTxBuffer(&RstQueuePtr->IpAddrId, SockIdx,
                          (IpV6_AddrType*)&(((IpBase_SockAddrIn6Type*)&(RstQueuePtr->RemSock))->sin6_addr),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
                          ((uint8 **)(&HdrPtr)), &TgtLen, &IpTxReqestDescr, FALSE) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
      {
        break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_c */
      }
    }
#endif

    /* fill header */
    HdrPtr->SrcPort          = ((IpBase_SockAddrInType*)&(RstQueuePtr->LocSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    HdrPtr->TgtPort          = ((IpBase_SockAddrInType*)&(RstQueuePtr->RemSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    HdrPtr->SeqNo            = IPBASE_HTON32(RstQueuePtr->SeqNo);
    HdrPtr->AckNo            = IPBASE_HTON32(RstQueuePtr->AckNo);

    HdrPtr->DataOffsetAndRes = 0x50;
    if(0 == RstQueuePtr->Flags)
    {
      HdrPtr->CodeBits         = TCPIP_TCP_CODE_BIT_RST;
    }
    else
    {
      HdrPtr->CodeBits         = (uint8)(TCPIP_TCP_CODE_BIT_RST | RstQueuePtr->Flags);
    }
    HdrPtr->Window           = 0; /* byte order has to be considered if this value is not 0 */
    HdrPtr->Checksum         = TCPIP_TCP_EMPTY_CHKSUM;
    HdrPtr->UrgentPtr        = 0;

    /* checksum will be calculated by lower layer, value is already set to '0' */
    /* transmit */
    RetValue = TcpIp_VIpTransmit(&IpTxReqestDescr, TCPIP_TCP_HDR_LEN_BYTE, TCPIP_PROTOCOL_TCP, FALSE, 0, FALSE, (TCPIP_P2C(IpBase_SockAddrType))&RstQueuePtr->RemSock);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    if(E_OK == RetValue)
    {
      TcpIp_Tcp_RstTxQueueFillNum--;
      TcpIp_Tcp_IncTxRstQueueIdx(TcpIp_Tcp_RstTxQueueReadPos);
    }
    else
    {
      /* transmit failed
         Possible reasons are a missing address resolution or no available buffer */
      break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_c */
    }
  }

  return;
}

/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VRestartSock
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VRestartSock(
    TcpIp_TcpIdxType TcpIdx )
{
  TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_CLOSED;
  TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

#if (TCPIP_TCP_OOO_RX_SUPPORT == STD_ON)
  /* delete all elements from OOO-list */
  if(TcpIp_Tcp_OooNumElements[TcpIdx] != 0)
  {

    if(0 != TcpIp_Tcp_OooNumFreeElements)
    {
      uint16_least CurrentCount;
      uint16 EleIdx;

      /*  goto last element of the socket's ooo-list */
      EleIdx = TcpIp_Tcp_OooFirstEleIdx[TcpIdx];
      for(CurrentCount = 0; CurrentCount < (uint16_least)(TcpIp_Tcp_OooNumElements[TcpIdx] -1); CurrentCount++)
      {
        EleIdx = TcpIp_Tcp_RxPreBufArray[EleIdx].NextEleIdx;
      }
      /* last element in socket ooo list reached, now link it to the free-list */
      TcpIp_Tcp_RxPreBufArray[EleIdx].NextEleIdx = TcpIp_Tcp_OooFirstFreeEleIdx;
    }

    /* prepend ooo-list elements to the free list */
    TcpIp_Tcp_OooFirstFreeEleIdx  = TcpIp_Tcp_OooFirstEleIdx[TcpIdx];
    TcpIp_Tcp_OooNumFreeElements += TcpIp_Tcp_OooNumElements[TcpIdx];

    /* reset size of socket's ooo-list */
    TcpIp_Tcp_OooNumElements[TcpIdx] = 0;
  }
#endif

  /* release TCP assigned rx and tx buffer */
  TcpIp_Tcp_VReleaseAsignedBuffers(TcpIdx);

  TcpIp_Tcp_VInitSock(TcpIdx);
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VInitSock
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VInitSock(
    TcpIp_TcpIdxType TcpIdx )
{
  TcpIp_Tcp_Iss[TcpIdx]    = TcpIp_Tcp_GlobalTimestamp * (250 * TcpIp_VCfgGetMainFctPeriodMsec());  /* inc iss every 4 microseconds */
  TcpIp_Tcp_SndUna[TcpIdx] = TcpIp_Tcp_Iss[TcpIdx];  /* ISS */
  TcpIp_Tcp_SndNxt[TcpIdx] = TcpIp_Tcp_Iss[TcpIdx];  /* ISS */
  TcpIp_Tcp_RcvWnd[TcpIdx] = 0;
  TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] = 0;

  TcpIp_Tcp_TxLenByteTx[TcpIdx] = 0;
  TcpIp_Tcp_TxFlags[TcpIdx]     = 0;

  {
    /* init the only TxReqArray element that will will be used for this socket */
    TcpIp_Tcp_TxReqArrayType *TxReqArrayElemPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];
    TxReqArrayElemPtr->DataLenByte     = 0;
    TxReqArrayElemPtr->QueuedLen       = 0;
    TxReqArrayElemPtr->DataBufStartIdx = 0;
    TxReqArrayElemPtr->FullyPlanned    = TRUE;
  }

  TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] = 0;
  TcpIp_Tcp_RxBufferIndPos[TcpIdx] = 0;

  TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = 0; /* set MSS to zero. This value will be overwritten by the MSS option of a received SYN or the default MSS for IPv4/IpV6 */
  TcpIp_Tcp_SockMtuSize[TcpIdx]     = TcpIp_Tcp_VCfgGetMaxTxMsgSizeByte() + 20;
  TcpIp_Tcp_TxOptLen[TcpIdx]        = 0;

#if (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON)
  TcpIp_Tcp_NagleEnabled[TcpIdx] = TRUE;
# if (TcpIp_Tcp_VCfgGetNagleTimeoutCycles() > 0)
  TcpIp_Tcp_NagleTimeout[TcpIdx] = 0;
# endif
#endif  /* TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON */

#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_UtoEnabled[TcpIdx]     = FALSE;  /* RFC 5482 defines default = FALSE */
  TcpIp_Tcp_UtoChangeable[TcpIdx]  = TRUE;
  /* TcpIp_Tcp_UtoToSend[TcpIdx]      = FALSE; */ /* TBR */
#endif  /* TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON */

  TcpIp_Tcp_BackLogArray[TcpIdx].SynAvailable = FALSE;  /* currently only one backlog element per socket is supported */
  /* do not care about the other struct elements */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  TcpIp_Tcp_VKeepAliveReset(TcpIdx);
#endif

  /* delete tx and rx queue */
  TcpIp_Tcp_VDelTxQueue(TcpIdx);
  TcpIp_Tcp_VDelRxQueue(TcpIdx);

  TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] = 0;

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_TimestampOptActive[TcpIdx] = FALSE;
  TcpIp_Tcp_TimestampReceived[TcpIdx]  = 0;
#endif

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_Rto[TcpIdx]      = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();  /* set to RTOMin (instead of 3s like in RFC 2988 or 1s in RFC 6298) */
  TcpIp_Tcp_RtoValid[TcpIdx] = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();  /* set to RTOMin (instead of 3s like in RFC 2988 or 1s in RFC 6298) */
  TcpIp_Tcp_SRtt[TcpIdx]     = 0;
  TcpIp_Tcp_RttVar[TcpIdx]   = 0;
  TcpIp_Tcp_RtoState[TcpIdx] = 1;
  TcpIp_Tcp_RtoResetPending[TcpIdx] = FALSE;
#endif
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_RetransmitTimeout[TcpIdx] = TcpIp_Tcp_RtoValid[TcpIdx];
  TcpIp_Tcp_RtoReloadValue[TcpIdx]    = TcpIp_Tcp_RtoValid[TcpIdx];
#else
  TcpIp_Tcp_RetransmitTimeout[TcpIdx] = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
  TcpIp_Tcp_RtoReloadValue[TcpIdx]    = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
#endif

#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_Uto[TcpIdx] = TcpIp_Tcp_VCfgGetUserTimeoutDefCycles();
  TcpIp_Tcp_UtoAdv[TcpIdx]    = 0;
  TcpIp_Tcp_UtoRemote[TcpIdx] = 0;
#endif

  TcpIp_Tcp_TxOneTimeOpts[TcpIdx]    = 0;
  TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx] = 0;

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
  TcpIp_Tcp_SackOptActive[TcpIdx] = FALSE;
#endif

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
  /* init congestion control variables, according to RFC 5681 and RFC 6582 */
  /* congestion window (CWND) will be set when socket is opened, because it depends on IP version. */

  TcpIp_Tcp_AckSumN[TcpIdx]       = 0;
  TcpIp_Tcp_AckSumTimeout[TcpIdx] = 0;
  TcpIp_Tcp_DupAckCnt[TcpIdx]     = 0;
  TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_SLOW_START;
  TcpIp_Tcp_Recover[TcpIdx]       = TcpIp_Tcp_Iss[TcpIdx];
  TcpIp_Tcp_FastRetransmitPending[TcpIdx] = FALSE;
#endif

  TcpIp_Tcp_EventIndicationPending[TcpIdx] = 0;

#if (TCPIP_SUPPORT_TLS == STD_ON)
  TcpIp_Tcp_UseTls[TcpIdx] = FALSE;
#endif

}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VDelRxQueue
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDelRxQueue(
    TcpIp_TcpIdxType TcpIdx )
{
  TcpIp_Tcp_RxBufferFillLevel[TcpIdx] = 0;
  TcpIp_Tcp_RxBufferWritePos[TcpIdx] = 0;
  TcpIp_Tcp_RxBufferReadPos[TcpIdx] = 0;
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VDelTxQueue
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDelTxQueue(
    TcpIp_TcpIdxType TcpIdx )
{

  TcpIp_Tcp_TxReqArrayReadPos[TcpIdx]  = 0;
  TcpIp_Tcp_TxReqArrayWritePos[TcpIdx] = 0;
  TcpIp_Tcp_TxReqArrayFillNum[TcpIdx]  = 0;

  /* code marker: TcpIp_Tcp_TxRetrQueue fill or empty handling */

  if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
  {
    /* The TxRetrQueue is not empty. The complete list shall be deleted in one block, so the first element already has
    the previous element index marked as invalid as well as the next element index of the last list element is marked
    as invalid. */
    TcpIp_TcpTxRetrQueueIdxType EleIdx;
    TcpIp_TcpTxRetrQueueIdxType LocLastEleIdx;
    TcpIp_TcpTxRetrQueueIdxType EleNum;

    EleNum = 1;
    EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
    while(TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX)
    {
      EleIdx = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
      EleNum++;
    }
    LocLastEleIdx = EleIdx;

    if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_LastFreeRetrQueueEle)
    {
      /* free elements list is not emptry */
      /* link last element of the free elements list with the first element of the socket's RetrQueue */
      TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_LastFreeRetrQueueEle].EleNext = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
      TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]].ElePrev = TcpIp_Tcp_LastFreeRetrQueueEle;
    }
    else
    {
      /* Take the whole socket's list and use it as the ne free elements list. */
      TcpIp_Tcp_FirstFreeRetrQueueEle = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
    }

    /* mark the last element of the socket's RetrQueue as the global last element of the free elements list */
    TcpIp_Tcp_TxRetrQueue[LocLastEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_LastFreeRetrQueueEle = LocLastEleIdx;

    /* in-/decrease number of (global) free element counters */
    TcpIp_Tcp_NumFreeRetrQueueEle       += EleNum;
    TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] = 0;

    TcpIp_Tcp_FirstRetrQueueEle[TcpIdx] = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_LastRetrQueueEle[ TcpIdx] = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
  }
  else
  {
    /* no queue elements available -> do nothing */
  }

  TcpIp_VInitTxSocket(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VClearRetrQueue
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VClearRetrQueue(
    TcpIp_TcpIdxType TcpIdx )
{
  /* clear retransmission queue */
  TcpIp_Tcp_TxReqArrayReadPos[TcpIdx] = TcpIp_Tcp_TxReqArrayWritePos[TcpIdx];
  TcpIp_Tcp_TxReqArrayFillNum[TcpIdx] = 0;
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VHandleTcpOptions
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VHandleTcpOptions(
    TcpIp_TcpIdxType TcpIdx,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)  HdrOptPtr,
    uint8                                       HdrOptLen,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpSegSeqPtr,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpSegAckPtr )  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  /* there is a TCP option included in the header */
  uint8 ByteIdx = 0;
  const uint8 *TempOptPtr;
  boolean EndOptFound = FALSE;

  TCPIP_UNUSED_PARAMETER(TcpSegAckPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* go through header bytes to find options */
  while((HdrOptLen > ByteIdx) && (FALSE == EndOptFound))
  {
    TempOptPtr = &HdrOptPtr[ByteIdx];
    switch(*TempOptPtr)
    {
    case TCPIP_TCP_OPT_ID_MAX_SEG_SIZE:
      {
        /* option "Maximum Segment Size", option length is 4 byte */
        const TcpIp_Tcp_HdrMssOptType *OptPtr = (const TcpIp_Tcp_HdrMssOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
        if(TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE == OptPtr->OptLen)
        {
          if ( (TCPIP_TCP_SOCK_STATE_SYNRCVD > TcpIp_Tcp_SockState[TcpIdx]) )
          {
            uint16 MaxLen = IPBASE_HTON16(OptPtr->SegLen);
            /* Set largest possible segment size and reduce it if necessary. */
            TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = TcpIp_Tcp_VCfgGetMaxTxMsgSizeByte();

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
            if (IPBASE_AF_INET6 == ((TCPIP_P2C(IpBase_SockAddrType))&(TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].RemSock))->sa_family)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
            {
              /* Fixed IPv6 header requires 20 bytes more than already considered IPv4 header. So TCP MSS has to be reduced by 20 bytes. */
              TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] -= (TCPIP_IPV6_HDR_LEN_BYTE - TCPIP_IPV4_HDR_LEN_BYTE);
            }
#endif

            if (MaxLen < TcpIp_Tcp_TxMaxSegLenByte[TcpIdx])
            {
              /* Received MSS is smaller than largest possible segment size the ECU can send.
               * -> Use received MSS value.
               */
              TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] = MaxLen;
            }

            /* Init SockMtuSize with received MaxSegLen */
            TcpIp_Tcp_SockMtuSize[TcpIdx] = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] + TCPIP_TCP_HDR_LEN_BYTE;

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
            /* resize cwnd */
            TcpIp_Tcp_CwndSegNumLimit[TcpIdx] = TCPIP_TCP_DEFAULT_SEGMENT_LIMIT(MaxLen);
            TcpIp_Tcp_Cwnd[TcpIdx]            = MaxLen * TcpIp_Tcp_CwndSegNumLimit[TcpIdx];
#endif
          }
          else
          {
            /* this option may not be received or evaluated in this state -> ignore it */
          }

          ByteIdx += TCPIP_TCP_OPT_LEN_MAX_SEG_SIZE;
        }
        else
        {
          /* erroneous option */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])  /* added for fix of ESCAN00081155 */
          {
            /* -> send RST */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
        break;
      }
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
    case TCPIP_TCP_OPT_ID_SACK_PERMIT:
      {
        /* option "SACK Permitted", option length is 2 byte */
        const TcpIp_Tcp_HdrGenericOptType *OptPtr = (const TcpIp_Tcp_HdrGenericOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        if((TCPIP_TCP_OPT_LEN_SACK_PERMIT == OptPtr->OptLen) && (TCPIP_TCP_SOCK_STATE_SYNRCVD > TcpIp_Tcp_SockState[TcpIdx]))
        {
          /* received the permission to use the SACK option */
          TcpIp_Tcp_SackOptActive[TcpIdx] = TRUE;

#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
          /* trigger sending of 'SACK PERMIT' option for passive open */
          if(TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx])
          {
            TcpIp_Tcp_TxOneTimeOpts[TcpIdx]    |= TCPIP_TCP_OPT_FLAG_SACK_PERMIT;
            TcpIp_Tcp_TxOneTimeOptsLen[TcpIdx] += (TCPIP_TCP_OPT_LEN_SACK_PERMIT +2);  /* option + 2x NOP */
          }
#endif

          ByteIdx += OptPtr->OptLen;
        }
        else
        {
          /* erroneous option or received in the wrong TCP socket state */
          /* -> send RST */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
        break;
      }
    case TCPIP_TCP_OPT_ID_SACK:
      {
        /* option "SACK", option length is 2 + n*8 byte */
        const TcpIp_Tcp_HdrGenericOptType *OptPtr = (const TcpIp_Tcp_HdrGenericOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        uint8 RemOptLen = (uint8)(OptPtr->OptLen - TCPIP_TCP_OPT_LEN_SACK);
        if((0 != (RemOptLen >> 3)) && (TCPIP_TCP_SOCK_STATE_ESTABLISHED <= TcpIp_Tcp_SockState[TcpIdx]))
        {
          /* (OptLen - 2) is a multiple of 8, so the option has a valid length */
          uint8 SackEleNum = (uint8)(RemOptLen >> 3);  /* number of received sequence number pairs */
          const uint32 *SeqNrPtr = (const uint32 *)(&HdrOptPtr[2]);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

          CANOE_WRITE_STRING_1("TcpIp_Tcp_VHandleTcpOptions: Received SACK option with %d elements", SackEleNum);

          /* mark elements in TxRetrQueue as 'sacked' */
          if(0 != TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx])
          {
            while( 0 != SackEleNum)
            {
              uint32 LeftEdge  = IPBASE_HTON32(SeqNrPtr[0]);
              uint32 RightEdge = IPBASE_HTON32(SeqNrPtr[1]);
              uint8 ListIdx;
              TcpIp_TcpTxRetrQueueIdxType EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];

              for(ListIdx = 0; ListIdx < TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx]; ListIdx++)
              {
                if( TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(TcpIp_Tcp_TxRetrQueue[EleIdx].SeqNo, LeftEdge) &&
                    TCPIP_TCP_SEQ1_SMALLER_OR_EQUAL_SEQ2((TcpIp_Tcp_TxRetrQueue[EleIdx].SeqNo + TcpIp_Tcp_TxRetrQueue[EleIdx].SizeTotByte), RightEdge) )
                {
                  /* element is in the sequence number range of this SACK element -> mark as 'sacked' */
                  TcpIp_Tcp_TxRetrQueue[EleIdx].SackFlag = 1;
                }
                EleIdx = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
              }

              SackEleNum--;
              SeqNrPtr = &SeqNrPtr[2];
            }
          }

          ByteIdx += OptPtr->OptLen;
        }
        else
        {
          /* erroneous option */
          /* -> send RST */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
        break;
      }
#endif
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
    case TCPIP_TCP_OPT_ID_TIMESTAMP:
      {
        /* option "Timestamp", option length is 10 byte */
        const TcpIp_Tcp_HdrGenericOptType *OptPtr = (const TcpIp_Tcp_HdrGenericOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        if(TCPIP_TCP_OPT_LEN_TIMESTAMP == OptPtr->OptLen)
        {
          if( (TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx]) ||
              (TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[TcpIdx]) )
          {
            TcpIp_Tcp_TimestampOptActive[TcpIdx] = TRUE;
          }

          if(TRUE == TcpIp_Tcp_TimestampOptActive[TcpIdx])
          {
            /* read received timestamp value */
            {
              uint32 TsVal = IPBASE_HTON32(*((const uint32*)(&TempOptPtr[2])));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
              if(*TcpSegSeqPtr == TcpIp_Tcp_RcvNxt[TcpIdx])
              {
                /* only take over the timestamp from the most recent segment that advanced the window */
                TcpIp_Tcp_TimestampReceived[TcpIdx] = TsVal;
              }
            }

            /* read received timestamp echo reply */
            /* This is the timestamp that was earlier sent by this ECU -> evaluate RTT */
            if ( (TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx]) &&
                 (TCPIP_TCP_SEQ1_SMALLER_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], *TcpSegAckPtr)) )
            {
              uint32 TsEcr      = IPBASE_HTON32(*((const uint32*)(&TempOptPtr[6])));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
              uint32 ElementRtt = TcpIp_Tcp_GlobalTimestamp - TsEcr;
              TcpIp_TcpTxRetrQueueIdxType RetrQueueIdx;

              if(TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[TcpIdx])
              {
                TcpIp_Tcp_VUpdateRttVars(TcpIdx, ElementRtt);
              }

              /* Search for corresponing element in the TxRetrQueue. If element was already retransmitted, discard this timestamp echo reply. */
              for(RetrQueueIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]; RetrQueueIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX; RetrQueueIdx = TcpIp_Tcp_TxRetrQueue[RetrQueueIdx].EleNext)
              {
                if( (TsEcr == TcpIp_Tcp_TxRetrQueue[RetrQueueIdx].TimeStamp) )
                {
                  /* the element was not yet retransmitted, so the TsEcr may be used for the RRTM */
                  CANOE_WRITE_STRING_3("TcpIp_Tcp_VHandleTcpOptions: RetrQueue element found with matching Timestamp %d, acknowleged data size is %d, current segment RTT is %d)", TsEcr, (*TcpSegAckPtr - TcpIp_Tcp_SndUna[TcpIdx]) , ElementRtt);

                  /* use ElementRtt for connection RTT calculation */
                  TcpIp_Tcp_VUpdateRttVars(TcpIdx, ElementRtt);

                  break;  /* don't search for further elements, there is no benefit in finding more matching elements */
                }
              }
            }
          }

          ByteIdx += TCPIP_TCP_OPT_LEN_TIMESTAMP;
        }
        else
        {
          /* erroneous option */
          /* -> drop message and send RST */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
        break;
      }
#endif  /* (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON) */
#if (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON)
    case TCPIP_TCP_OPT_ID_USER_TIMEOUT:
      {
        /* option "User Timeout" */
        const TcpIp_Tcp_HdrGenericOptType *OptPtr = (const TcpIp_Tcp_HdrGenericOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        if(TCPIP_TCP_OPT_LEN_USER_TIMEOUT == OptPtr->OptLen)
        {
          if((TRUE == TcpIp_Tcp_UtoEnabled[TcpIdx]) && (TRUE == TcpIp_Tcp_UtoChangeable[TcpIdx]))
          {
            uint32 TimeoutRecCycles;
            uint16 OptVal = IPBASE_HTON16( *((const uint16*)(&TempOptPtr[2])) );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

            TcpIp_Tcp_UtoRemote[TcpIdx] = OptVal;

            if((OptVal & 0x8000) == 0)
            {
              /* granularity is seconds */
              TimeoutRecCycles = ((uint32)(OptVal & 0x7FFF) * 1000u) / TCPIP_MAIN_FCT_PERIOD_MSEC;
            }
            else
            {
              /* granularity is minutes */
              TimeoutRecCycles = ((uint32)(OptVal & 0x7FFF) * 60000u) / TCPIP_MAIN_FCT_PERIOD_MSEC;
            }

            if(0 != TimeoutRecCycles)
            {
              if(TimeoutRecCycles < TcpIp_Tcp_VCfgGetUserTimeoutMinCycles())
              {
                TcpIp_Tcp_Uto[TcpIdx] = TcpIp_Tcp_VCfgGetUserTimeoutMinCycles();
              }
              else if(TimeoutRecCycles > TcpIp_Tcp_VCfgGetUserTimeoutMaxCycles())
              {
                TcpIp_Tcp_Uto[TcpIdx] = TcpIp_Tcp_VCfgGetUserTimeoutMaxCycles();
              }
              else
              {
                TcpIp_Tcp_Uto[TcpIdx] = TimeoutRecCycles;
              }
            }
            else
            {
              /* reserved for future usage -> ignore this option */
            }
          }
          ByteIdx += TCPIP_TCP_OPT_LEN_USER_TIMEOUT;
        }
        else
        {
          /* erroneous option */
          /* -> drop message and send RST */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
        break;
      }
#endif  /* (TCPIP_TCP_USER_TIMEOUT_OPTION_SUPPORT == STD_ON) */
    case TCPIP_TCP_OPT_ID_NOP:
      {
        /* option "No-Operation"
        -> skip this byte */
        ByteIdx++;
        break;
      }
    case TCPIP_TCP_OPT_ID_END:
      {
        /* option "End of Option List"
        -> stop evaluation of options */
        EndOptFound = TRUE;
        break;
      }
    default:
      {
        /* unknown option */
        const TcpIp_Tcp_HdrGenericOptType *OptPtr = (const TcpIp_Tcp_HdrGenericOptType *)(TempOptPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        uint8 RemLen = (uint8)(HdrOptLen - ByteIdx);
        if((RemLen >= 2) && (RemLen >= OptPtr->OptLen) && (2 <= OptPtr->OptLen))
        {
          /* unknown options will be ignored */
          ByteIdx += OptPtr->OptLen;
        }
        else
        {
          /* error in options */
          /* send reset (could also just be ignored, but a reset seems to be better) */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[TCPIP_TCPIDX2SOCKIDX(TcpIdx)].TxIpAddrId, RxSockRemAddrPtr, RxSockLocAddrPtr, 0, (*TcpSegSeqPtr +1), 0);

          if(TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx])
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* enter CLOSED state, delete/clear TCB */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
            /* signal a "connection reset" to the user */
#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
          }
          return E_NOT_OK;
        }
      }
    }
  }

#if ((TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON) || (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON))
  {
    /* timestamp option and SACK option both have to be negotiated during the handshake within the same message */
    uint8 BlockedOptLen = 0;

    if ( (TCPIP_TCP_SOCK_STATE_SYNRCVD > TcpIp_Tcp_SockState[TcpIdx]) )
    {
#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
      if(TRUE == TcpIp_Tcp_TimestampOptActive[TcpIdx])
      {
        BlockedOptLen = (TCPIP_TCP_OPT_LEN_TIMESTAMP +2);  /* including 2* NOP */
      }
#endif
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
      if(TRUE == TcpIp_Tcp_SackOptActive[TcpIdx])
      {
        BlockedOptLen = 40;  /* maximum TCP option length */ /* overwrite len set for timestamp option */
      }
#endif
      TcpIp_Tcp_TxOptLen[TcpIdx]     += BlockedOptLen;
      if((TcpIp_Tcp_TxOptLen[TcpIdx] + TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]) > TcpIp_Tcp_VCfgGetMaxTxMsgSizeByte())
      {
        TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]  -= BlockedOptLen;
      }
    }
  }
#endif

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VAddrResTimeout
 **********************************************************************************************************************/
/*! \brief      Address resolution timout Callback
 *  \param[in]  DestAddrPtr           Pointer to destination address
 *  \context    interrupt or task level
 *  \note       The address resolution for the given address timed out on the given controller, after the address
 *              resolution request was accepted earlier.
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_VAddrResTimeout(
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestAddrPtr )
{
  TcpIp_TcpIdxType  TcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  CANOE_WRITE_STRING_1("TcpIp_Tcp_VAddrResTimeout: function called, AddrType = %d", DestAddrPtr->sa_family);

  for(TcpIdx = 0; TcpIdx < TcpIp_VCfgGetMaxNumTcpSocks(); TcpIdx++)
  {
    SockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);

    if( (TCPIP_TCP_SOCK_STATE_CLOSED  != TcpIp_Tcp_SockState[TcpIdx]) &&
        (TCPIP_TCP_SOCK_STATE_LISTEN  != TcpIp_Tcp_SockState[TcpIdx]) &&
        (TCPIP_TCP_SOCK_STATE_INVALID != TcpIp_Tcp_SockState[TcpIdx]) )
    {
      /* check the addr to find out the corresponding socket, then stop the retry mechanism on the socket and indicate an
         error to the user */
      if( TRUE == TcpIp_VSockIpAddrIsEqual((const IpBase_SockAddrType*)DestAddrPtr, (const IpBase_SockAddrType*)(&(TcpIp_SocketList[SockIdx].RemSock))) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* addr request timeout for this socket occured */
#if (TCPIP_SUPPORT_TLS == STD_ON)
        boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
        uint8 SockUserIdx = TcpIp_SockUserIdx[SockIdx];

        CANOE_WRITE_STRING_1("TcpIp_Tcp_VAddrResTimeout: Addr match on socket %d", SockIdx);

        /* issue a RESET to the user and close tcp socket */
        TcpIp_Tcp_VRestartSock(TcpIdx);
        TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));
#if (TCPIP_SUPPORT_TLS == STD_ON)
        TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
#else
        TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
#endif
      }
    }
  }

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (0 < TcpIp_Tcp_RstTxQueueFillNum)
  {
    uint8_least RstTxQueueReadPos = TcpIp_Tcp_RstTxQueueReadPos;
    uint8_least RstTxQueueIdx;

    for (RstTxQueueIdx = 0; RstTxQueueIdx < TcpIp_Tcp_RstTxQueueFillNum; RstTxQueueIdx++)
    {
      TCPIP_P2V(IpBase_SockAddrType) RemSockAddrPtr = (TCPIP_P2V(IpBase_SockAddrType))&(TcpIp_Tcp_RstTxQueue[RstTxQueueReadPos].RemSock);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

      if (TRUE == TcpIp_VSockIpAddrIsEqual((TCPIP_P2C(IpBase_SockAddrType))DestAddrPtr, RemSockAddrPtr))  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* mark entry in reset queue as invalid. This entry will be removed in TcpIp_Tcp_VTransmitResets() */
        RemSockAddrPtr->sa_family = IPBASE_AF_UNSPEC;
      }

      TcpIp_Tcp_IncTxRstQueueIdx(RstTxQueueReadPos);
    }
  }

  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VFreeFirstRetrQueueEle
 **********************************************************************************************************************/
/*! \brief        remove first element from socket's retransmit queue and add element to free-list
 *  \param[in]    TcpIdx               TCP index
 *  \context      interrupt or task level
 *  \pre          Interrupts MUST be locked during call.
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VFreeFirstRetrQueueEle(
  TcpIp_TcpIdxType TcpIdx )
{
  /* code marker: TcpIp_Tcp_TxRetrQueue fill or empty handling */

  TcpIp_TcpTxRetrQueueIdxType SockOldFirstEleIdx;  /* element that shall be removed / moved to free-list */
  TcpIp_TcpTxRetrQueueIdxType SockNewFirstEleIdx;
  TcpIp_TcpTxRetrQueueIdxType GlobNewLastFreeEleIdx;
  TcpIp_TcpTxRetrQueueIdxType GlobOldLastFreeEleIdx;

  SockOldFirstEleIdx     = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
  GlobOldLastFreeEleIdx  = TcpIp_Tcp_LastFreeRetrQueueEle;
  GlobNewLastFreeEleIdx  = SockOldFirstEleIdx;

  /* remove element from the socket's RetrQueue -------------------------------------- */
  if(TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] != 1)
  {
    /* there is at least one element left in the socket's RetrQueue */
    SockNewFirstEleIdx = TcpIp_Tcp_TxRetrQueue[SockOldFirstEleIdx].EleNext;

    TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]               = SockNewFirstEleIdx;
    TcpIp_Tcp_TxRetrQueue[SockNewFirstEleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx]--;
  }
  else
  {
    /* the socket's RetrQueue will be empty afterwards */

    TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]               = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_LastRetrQueueEle[TcpIdx]                = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] = 0;
  }

  /* append the element to the free-list --------------------------------------------- */
  if(TcpIp_Tcp_NumFreeRetrQueueEle != 0)
  {
    /* there are some (at least one) elements in the free-list */

    /* link last element of the free-list with the (old) first element of the socket's RetrQueue */
    TcpIp_Tcp_TxRetrQueue[GlobNewLastFreeEleIdx].ElePrev = GlobOldLastFreeEleIdx;
    TcpIp_Tcp_TxRetrQueue[GlobOldLastFreeEleIdx].EleNext = GlobNewLastFreeEleIdx;

    /* mark the element of the socket's RetrQueue that shall be removed as the global last element of the free elements list */
    TcpIp_Tcp_LastFreeRetrQueueEle                       = GlobNewLastFreeEleIdx;
    TcpIp_Tcp_TxRetrQueue[GlobNewLastFreeEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
  }
  else
  {
    /* the free-list is empty, so fill in the first element */

    /* set neighbor element indices to invalid */
    TcpIp_Tcp_TxRetrQueue[GlobNewLastFreeEleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    TcpIp_Tcp_TxRetrQueue[GlobNewLastFreeEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;

    /* mark the element as the global last and first element of the free elements list */
    TcpIp_Tcp_FirstFreeRetrQueueEle = GlobNewLastFreeEleIdx;
    TcpIp_Tcp_LastFreeRetrQueueEle  = GlobNewLastFreeEleIdx;
  }
  TcpIp_Tcp_NumFreeRetrQueueEle++;  /* inc free elements counter */
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VFreeAckedReqAndRetrElements
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
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VFreeAckedReqAndRetrElements(
  TcpIp_TcpIdxType TcpIdx,
  uint32 SegAckNo )
{
  boolean checkIfFirstSegInQueueIsAcked = TRUE;
  TcpIp_SockIdxType sockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);

  /* #10 Repeat removing first segment from TX Retry Queue if queue is not empty or a not fully ACKed segment has been found before. */
  while (checkIfFirstSegInQueueIsAcked == TRUE)
  {
    uint32 acknowledgedPayloadBytes = 0;

    /* #20 Enter critical section to protect retry queue access. */
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Check first segment in retry queue or abort operation if queue is empty. */
    if (TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] > 0)
    {
      TcpIp_Tcp_TxRetrQueueType *TxRetrQueueElemPtr = &TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]];
     
      /* #40 Calculate sequence number of the following segment. */
      uint32 nextSegSeqNo = TxRetrQueueElemPtr->SeqNo + TxRetrQueueElemPtr->SizeTotByte;

      if ((TxRetrQueueElemPtr->Flags & TCPIP_TCP_CODE_BIT_SYN) != 0)
      {
        /* SYN flag counts as one byte. */
        nextSegSeqNo++;
      }

      if ((TxRetrQueueElemPtr->Flags & TCPIP_TCP_CODE_BIT_FIN) != 0)
      {
        /* FIN flag counts as one byte. */
        nextSegSeqNo++;
      }

      /* #50 Check if segment is completely acknowledged. */
      if(TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(SegAckNo, nextSegSeqNo))
      {
        /* #60 Remove acknowledged length from TX request element for this socket and store acknowledged payload length for call to TxConfirmation. */
        acknowledgedPayloadBytes = TxRetrQueueElemPtr->SizeTotByte;

        if (acknowledgedPayloadBytes > 0)
        {
          TcpIp_Tcp_TxReqArrayType *TxReqArrayElemPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];
          
          /* Remove acknowledged length from TX request element for this socket. */
          TxReqArrayElemPtr->DataLenByte -= acknowledgedPayloadBytes;
          TxReqArrayElemPtr->QueuedLen   -= acknowledgedPayloadBytes;
          TcpIp_IncTxBufIdx(sockIdx, TxReqArrayElemPtr->DataBufStartIdx, acknowledgedPayloadBytes);
        }

        TcpIp_Tcp_VFreeFirstRetrQueueEle(TcpIdx);
      }
      else
      {
        /* #70 ELSE: Stop searching for further segments. If this segment is not ACKed others with higher sequence numbers will not be ACKed as well. */
        checkIfFirstSegInQueueIsAcked = FALSE;
      }
    }
    else
    {
      /* Tx Retry Queue is empty. All segments are acknowledged. Stop searching for further segments. */
      checkIfFirstSegInQueueIsAcked = FALSE;
    }

    /* #80 Leave critical section because upper layer is called next. */
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #90 Call TxConfirmation of upper layer if segment contained acknowledged payload bytes. */
    if (acknowledgedPayloadBytes > 0)
    {
      TcpIp_VSockTxConfirmation(sockIdx, acknowledgedPayloadBytes);
    }
  }

/* (end) remove segments from tx (retransmission) queue */
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VQueueRemReqData
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VQueueRemReqData(
  TcpIp_TcpIdxType TcpIdx )
{
  Std_ReturnType retVal;

  /* #10 Check if there are free elements in the retry queue. */

  if (TcpIp_Tcp_NumFreeRetrQueueEle > 0)
  {
    /* Add (part of) request to TxReqArray */
    uint32 RemaniningSize;  /* size of data not yet queued in the TxRetrQueue */

    TcpIp_Tcp_TxReqArrayType *TxReqPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];

    RemaniningSize = TxReqPtr->DataLenByte - TxReqPtr->QueuedLen;

    /* #20 Append new fully filled segments into the retry queue (if possible). */

    /* inc TxLen, or fill retry queue elements */
    while( (TcpIp_Tcp_NumFreeRetrQueueEle > 0) && (RemaniningSize >= TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]) )
    {
      /* add max size segment, and send */
      RemaniningSize                -= TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
      TcpIp_Tcp_TxLenByteTx[TcpIdx]  = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];

#if (TCPIP_TCP_QUEUE_REQ_DATA_IN_MAIN_FUNCTION_ONLY == STD_ON)
      (void)TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE);
#else
      if(E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, FALSE))
      {
        TxReqPtr->QueuedLen                 += TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
        TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] -= TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
        TcpIp_Tcp_TxLenByteTx[TcpIdx]        = 0;
      }
#endif
    }

    /* #30 Plan partially filled segment that will only be appended to the retry queue if nagle allows it. */

    if( (TcpIp_Tcp_NumFreeRetrQueueEle > 0) && (RemaniningSize > 0) )
    {
      if (0 == TcpIp_Tcp_TxLenByteTx[TcpIdx])
      {
        /* new RetrQueue element is needed */

#if (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON)
        /* set nagle algorithm timeout */
        if(TRUE == TcpIp_Tcp_NagleEnabled[TcpIdx])
        {
# if (TcpIp_Tcp_VCfgGetNagleTimeoutCycles() > 0)
          TcpIp_Tcp_NagleTimeout[TcpIdx] = TcpIp_Tcp_VCfgGetNagleTimeoutCycles();
# endif
        }
        else
#endif
          /* TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON */
        {
          /* send new element even though it is not full */
          TcpIp_Tcp_TxLenByteTx[TcpIdx]  = RemaniningSize;

#if (TCPIP_TCP_QUEUE_REQ_DATA_IN_MAIN_FUNCTION_ONLY == STD_ON)
          if (TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) == E_OK)
          {
            RemaniningSize = 0;
          }
#else
          if(E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, FALSE))
          {
            TxReqPtr->QueuedLen  += RemaniningSize;
            TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx] -= RemaniningSize;
            TcpIp_Tcp_TxLenByteTx[TcpIdx] = 0;
            RemaniningSize = 0;
          }
          else
          {
            /* nothing to do */
          }
#endif
        }
      }
      else
      {
        /* do nothing, this data will be part of a message that will be sent later */
      }
      /* add remaining size segment */
      TcpIp_Tcp_TxLenByteTx[TcpIdx] = RemaniningSize;

      TxReqPtr->FullyPlanned = TRUE;
    }

    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_VInsElesInRetrQueue
 **********************************************************************************************************************/
/*! \brief        insert some free elements into the TxRetrQueue
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    PrevRetrEleIdx       Index of the preceding element in the TxReqArray
 *  \param[in]    NumNewElements       Number of elements that shall be inserted
 *  \context      interrupt or task level
 *  \note         It must be assuerd that enough free elements are available for this operation. This is usually done
 *                by the calling function. There must be at least one previous element in the socket's RetrQueue.
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VInsElesInRetrQueue(
  TcpIp_TcpIdxType TcpIdx,
  TcpIp_TcpTxRetrQueueIdxType PrevRetrEleIdx,
  uint8 NumNewElements )
{
  /* [F2] */
  TcpIp_TcpTxRetrQueueIdxType LocRetrQueueEleIdx;
  TcpIp_TcpTxRetrQueueIdxType OldNextRetrQueueEleIdx;
  TcpIp_TcpTxRetrQueueIdxType LastAddedRetrQueueEleIdx;

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if(NumNewElements > TcpIp_Tcp_NumFreeRetrQueueEle)
  {
    /* error, this may not happen and should be prevented in the calling function */
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return;
  }
  if(0 == NumNewElements)
  {
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return;  /* error */
  }

  LocRetrQueueEleIdx     = TcpIp_Tcp_FirstFreeRetrQueueEle;
  OldNextRetrQueueEleIdx = TcpIp_Tcp_TxRetrQueue[PrevRetrEleIdx].EleNext;

  /* link new element with previous element */
  TcpIp_Tcp_TxRetrQueue[LocRetrQueueEleIdx].ElePrev = PrevRetrEleIdx;
  TcpIp_Tcp_TxRetrQueue[PrevRetrEleIdx    ].EleNext = LocRetrQueueEleIdx;

  /* append further new elements */
  {
    uint8 NumElementsAdded = 1;
    while(NumElementsAdded < NumNewElements)
    {
      /* get index of next element */
      LocRetrQueueEleIdx = TcpIp_Tcp_TxRetrQueue[LocRetrQueueEleIdx].EleNext;
      NumElementsAdded++;
    }
    LastAddedRetrQueueEleIdx = LocRetrQueueEleIdx;
    LocRetrQueueEleIdx = TcpIp_Tcp_TxRetrQueue[LocRetrQueueEleIdx].EleNext;

    if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX == LocRetrQueueEleIdx)
    {
      /* FreeList is empty now */
      TcpIp_Tcp_FirstFreeRetrQueueEle = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_LastFreeRetrQueueEle  = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_NumFreeRetrQueueEle   = 0;
    }
    else
    {
      TcpIp_Tcp_FirstFreeRetrQueueEle = LocRetrQueueEleIdx;
      TcpIp_Tcp_TxRetrQueue[LocRetrQueueEleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_NumFreeRetrQueueEle  -= NumNewElements;
    }
  }

  /* link last inserted element with old next-element */
  TcpIp_Tcp_TxRetrQueue[LastAddedRetrQueueEleIdx].EleNext = OldNextRetrQueueEleIdx;
  if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX == OldNextRetrQueueEleIdx)
  {
    /* elements were inserted at the end of the RetrQueue */
    TcpIp_Tcp_LastRetrQueueEle[TcpIdx] = LastAddedRetrQueueEleIdx;
  }
  else
  {
    /* elements were inserted somewhere inside the RetrQueue */
    TcpIp_Tcp_TxRetrQueue[OldNextRetrQueueEleIdx].ElePrev = LastAddedRetrQueueEleIdx;
  }

  /* adapt fill level variables */
  TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] += NumNewElements;

  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VDropEndEleInRetrQueue
 **********************************************************************************************************************/
/*! \brief        Drop elements at the end of the socket's TxRetrQueue
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    NumDropEle           Number of elements that shall be dropped
 *  \context      interrupt or task level
 *  \note         It must be assuerd that the socket's TxRetrQueue has enough elements for this operation. This is
 *                usually done by the calling function.
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VDropEndEleInRetrQueue(
  TcpIp_TcpIdxType TcpIdx,
  uint8 NumDropEle )
{
  /* [F1] */
  TcpIp_TcpTxRetrQueueIdxType OldLastRetrQueueEleIdx;
  TcpIp_TcpTxRetrQueueIdxType FirstRemRetrQueueEleIdx;
  TcpIp_TcpTxRetrQueueIdxType NewLastRetrQueueEleIdx;
  uint32 RemLen;

  if(NumDropEle == 0)
  {
    return;
  }

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if(NumDropEle > (TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] -1))
  {
    /* error, at least one element has to stay in the TxRetrQueue */
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return;
  }

  OldLastRetrQueueEleIdx  = TcpIp_Tcp_LastRetrQueueEle[TcpIdx];
  FirstRemRetrQueueEleIdx = TcpIp_Tcp_LastRetrQueueEle[TcpIdx];
  NewLastRetrQueueEleIdx  = TcpIp_Tcp_TxRetrQueue[OldLastRetrQueueEleIdx].ElePrev;
  RemLen = TcpIp_Tcp_TxRetrQueue[OldLastRetrQueueEleIdx].SizeTotByte;

  /* remove further new elements */
  {
    uint8 NumElementsRemoved = 1;

    while(NumElementsRemoved < NumDropEle)
    {
      /* get index of next removed element */
      FirstRemRetrQueueEleIdx = NewLastRetrQueueEleIdx;
      RemLen += TcpIp_Tcp_TxRetrQueue[NewLastRetrQueueEleIdx].SizeTotByte;
      NewLastRetrQueueEleIdx  = TcpIp_Tcp_TxRetrQueue[NewLastRetrQueueEleIdx].ElePrev;
      NumElementsRemoved++;
    }

    /* link removed element(s) with old last free element */
    if(0 == TcpIp_Tcp_NumFreeRetrQueueEle)
    {
      /* FreeList has been empty */
      TcpIp_Tcp_FirstFreeRetrQueueEle    = FirstRemRetrQueueEleIdx;
      TcpIp_Tcp_LastFreeRetrQueueEle     = OldLastRetrQueueEleIdx;
      TcpIp_Tcp_LastRetrQueueEle[TcpIdx] = NewLastRetrQueueEleIdx;
      TcpIp_Tcp_TxRetrQueue[NewLastRetrQueueEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      TcpIp_Tcp_TxRetrQueue[FirstRemRetrQueueEleIdx].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    }
    else
    {
      TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_LastFreeRetrQueueEle].EleNext = FirstRemRetrQueueEleIdx;
      TcpIp_Tcp_TxRetrQueue[FirstRemRetrQueueEleIdx].ElePrev = TcpIp_Tcp_LastFreeRetrQueueEle;
      TcpIp_Tcp_LastFreeRetrQueueEle     = OldLastRetrQueueEleIdx;
      TcpIp_Tcp_LastRetrQueueEle[TcpIdx] = NewLastRetrQueueEleIdx;
      TcpIp_Tcp_TxRetrQueue[NewLastRetrQueueEleIdx].EleNext = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
    }
  }

  /* adapt fill level variables */
  TcpIp_Tcp_NumFreeRetrQueueEle        += NumDropEle;
  TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] -= NumDropEle;

  /* call function to un-queue the removed len 'RemLen' -> [F3] */
  TcpIp_Tcp_VUnqueueDataInReqArray(TcpIdx, RemLen);

  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VUnqueueDataInReqArray
 **********************************************************************************************************************/
/*! \brief        Un-queue data from the socket's TxReqArray.
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    UnqueueLen           Data Length that shall be un-queued
 *  \context      interrupt or task level
 *  \note         It must be assuerd that the socket's TxReqArray contains at least the length of data that shall be
 *                un-queued.
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VUnqueueDataInReqArray(
    TcpIp_TcpIdxType TcpIdx,
    uint32 UnqueueLen )
{
  /* [F3] */
  /* go backwards through the socket's TxRequests
     remove Queued length from the element until UnqueueLen is reached
     inc Unqueued len for this socket */

  TcpIp_Tcp_TxReqArrayType *TxReqArrayElemPtr;
  uint32 RemUnqueueLen = UnqueueLen;

  {
    /* use pointer to TxReqArray: */
    TxReqArrayElemPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];

    RemUnqueueLen                  -= TxReqArrayElemPtr->QueuedLen;
    TxReqArrayElemPtr->QueuedLen    = 0;
    TxReqArrayElemPtr->FullyPlanned = FALSE;
  }
  if(0 != RemUnqueueLen)
  {
    /* reduce queued length for this request element */
    TxReqArrayElemPtr->QueuedLen -= RemUnqueueLen;
    TxReqArrayElemPtr->FullyPlanned = FALSE;
  }

  TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx]  += UnqueueLen;

  TcpIp_Tcp_TxNextSendSeqNo[TcpIdx]    -= UnqueueLen;
}

#endif  /* TCPIP_TCP_SUPPORT_RETRY_QUEUE_ELEMENT_RESIZE */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VMainStateHandling
 **********************************************************************************************************************/
/*! \brief      Main socket state handling
 *  \context    task level
 **********************************************************************************************************************/
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainStateHandling(
    TcpIp_TcpIdxType TcpIdx )
{
  if(TCPIP_TCP_SOCK_STATE_INVALID != TcpIp_Tcp_SockStateNext[TcpIdx])
  {
    /* state change is requested -> do not wait for more data, send immediately */
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_CLOSED --- */
    if ( TCPIP_TCP_SOCK_STATE_CLOSED == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockStateNext[TcpIdx] )
      {
        if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
        {
          TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_SYNSENT;
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
        }
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_LISTEN --- */
    if ( TCPIP_TCP_SOCK_STATE_LISTEN == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockStateNext[TcpIdx] )
      {
        if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
        {
          TcpIp_Tcp_SockState[TcpIdx]     = TCPIP_TCP_SOCK_STATE_SYNRCVD;
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
        }
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_SYNRCVD --- */
    if ( TCPIP_TCP_SOCK_STATE_SYNRCVD == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_SYNSENT --- */
    else if ( TCPIP_TCP_SOCK_STATE_SYNSENT == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_ESTABLISHED --- */
    else if ( TCPIP_TCP_SOCK_STATE_ESTABLISHED == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_CLOSEWAIT --- */
    else if ( TCPIP_TCP_SOCK_STATE_CLOSEWAIT == TcpIp_Tcp_SockState[TcpIdx] )
    {
      /* do nothing, wait for USER to decide to close the connection */
      TCPIP_DUMMY_ASM_NOP
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_LASTACK --- */
    else if ( TCPIP_TCP_SOCK_STATE_LASTACK == TcpIp_Tcp_SockState[TcpIdx] )
    {
      /* trigger sending if something to send is available */
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_FINWAIT1 --- */
    else if ( TCPIP_TCP_SOCK_STATE_FINWAIT1 == TcpIp_Tcp_SockState[TcpIdx] )
    {
      /* trigger sending if something to send is available */
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_CLOSING --- */
    else if ( TCPIP_TCP_SOCK_STATE_CLOSING == TcpIp_Tcp_SockState[TcpIdx] )
    {
      /* trigger sending if something to send is available */
      if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
      {
        TcpIp_Tcp_SockState[TcpIdx]     = TcpIp_Tcp_SockStateNext[TcpIdx];
        TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_INVALID;

        /* clear retransmission queue */
        TcpIp_Tcp_VClearRetrQueue(TcpIdx);
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_TIMEWAIT --- */
    else if ( TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockState[TcpIdx] )
    {
      if ( TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockStateNext[TcpIdx] )
      {
        /* trigger sending of final ACK */
        if ( E_OK == TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE) )
        {
          TcpIp_Tcp_SockStateNext[TcpIdx] = TCPIP_TCP_SOCK_STATE_CLOSED;

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
          TcpIp_Tcp_VKeepAliveReset(TcpIdx);
#endif

          /* clear retransmission queue */
          TcpIp_Tcp_VClearRetrQueue(TcpIdx);
        }
      }
      if ( TCPIP_TCP_SOCK_STATE_CLOSED == TcpIp_Tcp_SockStateNext[TcpIdx] )
      {
        if ((TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx] == 0) && (TcpIp_Tcp_TxFlags[TcpIdx] == 0))  /* wait for the last ACK to be sent / removed from tx queue */
        {
          /* decrease Timeout timer (MSL) */
          if (TcpIp_Tcp_MslTimeout[TcpIdx] > 0)
          {
              TcpIp_Tcp_MslTimeout[TcpIdx]--;
          } 
          else /* (TcpIp_Tcp_MslTimeout[TcpIdx] == 0), timeout expired */
          {
#if (TCPIP_SUPPORT_TLS == STD_ON)
            boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
#endif
            uint8 SockUserIdx = TcpIp_SockUserIdx[TCPIP_TCPIDX2SOCKIDX(TcpIdx)];

            /* switch to state CLOSED, delete/clear TCB element */
            TcpIp_Tcp_VRestartSock(TcpIdx);
            TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

#if (TCPIP_SUPPORT_TLS == STD_ON)
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, UseTls);
#else
            TcpIp_VSockTcpStateChg(TCPIP_TCPIDX2SOCKIDX(TcpIdx), SockUserIdx, TCPIP_TCP_STATE_CLOSED, FALSE);
#endif
          }
        }
      }
    }
    /* ---------------------------- */
    /* --- TCPIP_TCP_SOCK_STATE_X --- */
    else
    {
      /* do nothing */
      TCPIP_DUMMY_ASM_NOP
    }
    /* ---------------------------- */
  }
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VMainRxDataForwarding
 **********************************************************************************************************************/
/*! \brief      Main socket rx data forwarding
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainRxDataForwarding(
    TcpIp_TcpIdxType TcpIdx,
    uint16 maxLenToForward)
{
  TCPIP_ASSERT(maxLenToForward                  <= TcpIp_Tcp_RxBufferRemIndLen[TcpIdx]);
  TCPIP_ASSERT(TcpIp_Tcp_RxBufferIndPos[TcpIdx] <  TcpIp_Tcp_AssignedRxBufferSize[TcpIdx]);

  if (maxLenToForward > 0)
  {
    /* indicate data to upper layer */

    TcpIp_PbufType DataDesc[2];
    uint8 *LocBufPtr;

    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */  /* lock interrupts */

    if((TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferIndPos[TcpIdx]) >= maxLenToForward)
    {
      /* data is in linear buffer */
      /* indicate bytes 0..((TCPIP_TCP_MAX_WINDOW_SIZE - TcpIp_Tcp_RxBufferWritePos[TcpIdx])-1) */
      LocBufPtr = (uint8 *) (TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, TcpIp_Tcp_RxBufferIndPos[TcpIdx]));
      DataDesc[0].payload    = LocBufPtr;
      DataDesc[0].len        = maxLenToForward;
      DataDesc[0].totLen     = maxLenToForward;
    }
    else
    {
      uint16 firstChunkLen;
      /* data buffer includes wrap around */
      /* indicate data with taking care of buffer wrap-around (two blocks) */

      /* indicate bytes 0..((TCPIP_TCP_MAX_WINDOW_SIZE - TcpIp_Tcp_RxBufferIndPos[TcpIdx])-1) */
      LocBufPtr     = (uint8*)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, TcpIp_Tcp_RxBufferIndPos[TcpIdx]));
      firstChunkLen = (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx] - TcpIp_Tcp_RxBufferIndPos[TcpIdx];
      DataDesc[0].payload = LocBufPtr;
      DataDesc[0].len     = firstChunkLen;
      DataDesc[0].totLen  = maxLenToForward;

      /* indicate remaining bytes */
      LocBufPtr = (uint8 *)(TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, 0));
      DataDesc[1].payload = LocBufPtr;
      DataDesc[1].len     = maxLenToForward - firstChunkLen;
      DataDesc[1].totLen  = maxLenToForward;
    }

    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */  /* unlock interrupts */

#if (TCPIP_SUPPORT_TLS == STD_ON)
    if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
    {
      /* forward to TLS */
      TcpIp_SocketIdType SocketId = TCPIP_TCPIDX2SOCKIDX(TcpIdx);
      Tls_RxIndication((TcpIp_SocketIdType)SocketId, DataDesc[0].payload, DataDesc[0].len);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 0311 */ /* MD_TCPIP_11.5_0311_ASR */
      if(DataDesc[0].len != DataDesc[0].totLen)
      {
        Tls_RxIndication( (TcpIp_SocketIdType)SocketId, DataDesc[1].payload, DataDesc[1].len);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 0311 */ /* MD_TCPIP_11.5_0311_ASR */
      }
    }
    else
#endif
    {
      TcpIp_VSockRxIndicationAsr42x(TCPIP_TCPIDX2SOCKIDX(TcpIdx), (const IpBase_SockAddrType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].RemSock)), &DataDesc[0]);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    }

    /* lock interrupts */
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    {
      /* increase buffer position, taking care of the wraparound */
      TcpIp_Tcp_RxBufferIndPos[TcpIdx]     = TcpIp_AddWithWrapAround16(TcpIp_Tcp_RxBufferIndPos[TcpIdx], maxLenToForward, (uint16)TcpIp_Tcp_AssignedRxBufferSize[TcpIdx]);
      TcpIp_Tcp_RxBufferRemIndLen[TcpIdx] -= maxLenToForward;
    }
    /* unlock interrupts */
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VMainIdleTimeoutHandling
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
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainIdleTimeoutHandling(
    TcpIp_TcpIdxType TcpIdx )
{
#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
  /* #10 Check if TCP Keep-Alive is enabled for the socket. */
  if ((TcpIp_Tcp_SockState[TcpIdx] == TCPIP_TCP_SOCK_STATE_ESTABLISHED) && (TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveEnabled == TRUE))
  {
    /* Keep-alive is enabled for this connection. */
    uint32 keepAliveIdleTimeMax;

    /* #20 Check if timeout for first keep-alive segment or consecutive keep-alive segment has to be applied. */
    if (TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbes == 0)
    {
      /* No keep-alive probes sent so far. Wait for keep-alive timeout. */
      keepAliveIdleTimeMax = TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveTimeout;
    }
    else
    {
      /* Keep-Alive timeout already expired and at least one probe has been sent. Wait for next probe interval. */
      keepAliveIdleTimeMax = TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbeInterval;
    }

    /* #30 Increment idle time counter for the socket if timeout has not expired yet. */
    if (TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveIdleTime < keepAliveIdleTimeMax)
    {
      /* Timeout has not expired yet. */
      TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveIdleTime++;
    }
    /* #40 ELSE: Send keep-alive segment and reset idle time or close connection if configured number of keep-alive segments have been sent. */
    else
    {
      if (TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbes < TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbesMax)
      {
        if (TcpIp_Tcp_VTransmitKeepAliveProbe(TcpIdx) == E_OK)
        {
          /* Reset idle time and wait for next timeout. */ 
          TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveIdleTime = 0;
          TcpIp_Tcp_KeepAliveStatus[TcpIdx].KeepAliveProbes++;
        }
      }
      else
      {
        /* Maximum number of keep-alive probes have been sent and were not answered. Close connection. */

# if (TCPIP_SUPPORT_TLS == STD_ON)
        boolean UseTls = TcpIp_Tcp_UseTls[TcpIdx];
# endif
        TcpIp_SockIdxType SockIdx = TCPIP_TCPIDX2SOCKIDX(TcpIdx);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
        uint8 SockUserIdx = TcpIp_SockUserIdx[SockIdx];

        /* timeout expired -> close / reset connection */
        {
          /* sending a reset is an extension, there is no need for that according to the RFC */
          TcpIp_Tcp_VSendReset(TcpIdx, TcpIp_SocketList[SockIdx].TxIpAddrId, (IpBase_SockAddrType *)&(TcpIp_SocketList[SockIdx].RemSock), (IpBase_SockAddrType *)&(TcpIp_SocketList[SockIdx].LocSock), TcpIp_Tcp_SndNxt[TcpIdx], 0, 0);  /* seqNo, ackNo */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        }

        /* flush all queues, signal the user "error: connection aborted due to idle timeout" in
        general and for any outstanding calls, delete the TCB, enter the CLOSED state and
        return. */
        /* delete/clear TCB element */
        TcpIp_Tcp_VRestartSock(TcpIdx);
        TcpIp_VDelSockAddrMapping(TCPIP_TCPIDX2SOCKIDX(TcpIdx));

        /* signal error to user */
        /* there was no RST received but an internal RST was gernerated */
# if (TCPIP_SUPPORT_TLS == STD_ON)
        TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, UseTls);
# else
        TcpIp_VSockTcpStateChg(SockIdx, SockUserIdx, TCPIP_TCP_STATE_RST_REC, FALSE);
# endif
      }
    }
  }
#endif

  /* -------------------------------------------------------------------------------------------- */
  /* Handle TcpIp_Tcp_IdleTimeoutShort[] --------------------------------------------------------- */

  /* #50 Check if idle timeout for congestion window reset and zero window probe has been reached. */
  if(0 != TcpIp_Tcp_IdleTimeoutShort[TcpIdx])
  {
    TcpIp_Tcp_IdleTimeoutShort[TcpIdx]--;
    if(0 == TcpIp_Tcp_IdleTimeoutShort[TcpIdx])
    {
      /* timeout has expired */

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
# if (TCPIP_TCP_CWND_IDLE_TIMEOUT_ENABLED == STD_ON)
      {
        /* reset the congestion window because the connection has been idle for too long */
        /* cwnd = RW = min(IW,cwnd) */
        uint32 Iw = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx] * TCPIP_TCP_DEFAULT_SEGMENT_LIMIT(TcpIp_Tcp_TxMaxSegLenByte[TcpIdx]);

        if(Iw < TcpIp_Tcp_Cwnd[TcpIdx])
        {
          TcpIp_Tcp_Cwnd[TcpIdx] = Iw;
        }

        TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_SLOW_START;
      }
# endif
#endif

      /* handle Zero Window Probing or splitting of the first segment */
      if(TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_FirstRetrQueueEle[TcpIdx])
      {
        TcpIp_Tcp_VSplitFirstRetrQueueEle(TcpIdx);
      }
    }
  }

}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VSplitFirstRetrQueueEle
 **********************************************************************************************************************/
/*! \brief      Split the first RetrQueueEle to fit into the send window.
 *              If there are spare retry queue elements, the first element of the connections retry queue is split
                into two parts, otherwise the first element is reduced in its size and the remaining data is un-queued.
 *  \param[in]  TcpIdx       Index of the TCP connection
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSplitFirstRetrQueueEle(
    TcpIp_TcpIdxType TcpIdx )
{
  TcpIp_Tcp_TxRetrQueueType *FirstQueueElePtr = &TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]];
  uint32 SegLen  = FirstQueueElePtr->SizeTotByte;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint16 WinSize = TcpIp_Tcp_SndWnd[TcpIdx];
  /* Split the first segment. The length of the first segment is never '0' in this situation. */
  if((SegLen > WinSize) && (SegLen != 1))
  {
    TcpIp_TcpTxRetrQueueIdxType NewEleIdx;
    uint16 SizePartOne;

    if(0 == WinSize)
    {
      /* split 1 byte from the first segment */
      SizePartOne = 1;
    }
    else  /* (seglen > windowsize) */
    {
      /* The segment length of the first queue element is probably bigger than the current window size, otherwise it would be retransmitted using the normal retransmit mechanism. */
      /* split a window-sized segment from the first queue element */
      SizePartOne = WinSize;
    }

    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if(0 != TcpIp_Tcp_NumFreeRetrQueueEle)
    {
      /* use one free element */
      NewEleIdx = TcpIp_Tcp_FirstFreeRetrQueueEle;
      TcpIp_Tcp_NumFreeRetrQueueEle--;
      if(0 != TcpIp_Tcp_NumFreeRetrQueueEle)
      {
        /* there has been more than one element in the free-list */
        TcpIp_Tcp_FirstFreeRetrQueueEle = TcpIp_Tcp_TxRetrQueue[NewEleIdx].EleNext;
        TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstFreeRetrQueueEle].ElePrev = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;  /* probably not necessary assignment */
      }
      else
      {
        TcpIp_Tcp_FirstFreeRetrQueueEle = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
        TcpIp_Tcp_LastFreeRetrQueueEle  = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      }
    }
    else
    {
      /* one element of the retry queue has to be freed if possible */
      /* handling similar to PathMtu */
      if(1 != TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx])
      {
        /* there is more than one element in the connections retry queue -> drop the last element, and than split the first element in two parts */
        /* drop last retry queue element */
        TcpIp_Tcp_VDropEndEleInRetrQueue(TcpIdx, 1);
        /* use the one free element */
        NewEleIdx = TcpIp_Tcp_FirstFreeRetrQueueEle;
        TcpIp_Tcp_NumFreeRetrQueueEle = 0;   /* size was '1', so size -1 = 0 */
        TcpIp_Tcp_FirstFreeRetrQueueEle = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
        TcpIp_Tcp_LastFreeRetrQueueEle  = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;

        /* splitting the segment / queue element is done in the next step */
      }
      else
      {
        /* there is only one single element in the connections retry queue, and there is no spare one available
        -> reduce the sice of the (first and only) queue element */
        NewEleIdx = TCPIP_TCP_TX_RETR_QUEUE_INV_IDX;
      }
    }

    /* split the first element (content) into two parts */
    if(NewEleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX)
    {
      /* append the new element to the first element */
      TcpIp_TcpTxRetrQueueIdxType FirstEleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].DataBufStartIdx    = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].DataBufStartIdx + SizePartOne;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].SeqNo              = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].SeqNo           + SizePartOne;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].SizeTotByte        = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].SizeTotByte     - SizePartOne;
      TcpIp_Tcp_TxRetrQueue[FirstEleIdx].SizeTotByte      = SizePartOne;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].EleNext            = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].EleNext;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].ElePrev            = FirstEleIdx;

      if (TCPIP_TCP_TX_RETR_QUEUE_INV_IDX != TcpIp_Tcp_TxRetrQueue[FirstEleIdx].EleNext)
      {
        TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_TxRetrQueue[FirstEleIdx].EleNext].ElePrev = NewEleIdx;
      }
      else
      {
        TcpIp_Tcp_LastRetrQueueEle[TcpIdx] = NewEleIdx;
      }

      TcpIp_Tcp_TxRetrQueue[FirstEleIdx].EleNext          = NewEleIdx;
      if(0 != (TcpIp_Tcp_TxRetrQueue[FirstEleIdx].Flags & TCPIP_TCP_CODE_BIT_FIN))
      {
        TcpIp_Tcp_TxRetrQueue[NewEleIdx].Flags            = TCPIP_TCP_CODE_BIT_ACK | TCPIP_TCP_CODE_BIT_PSH | TCPIP_TCP_CODE_BIT_FIN;
        TcpIp_Tcp_TxRetrQueue[FirstEleIdx].Flags          = (uint8)(TcpIp_Tcp_TxRetrQueue[FirstEleIdx].Flags - (uint8)TCPIP_TCP_CODE_BIT_FIN);
      }
      else
      {
        TcpIp_Tcp_TxRetrQueue[NewEleIdx].Flags            = TCPIP_TCP_CODE_BIT_ACK | TCPIP_TCP_CODE_BIT_PSH;
      }
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].TcpIdx             = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].TcpIdx;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].TimeStamp          = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].TimeStamp;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].UserTimeoutTs      = TcpIp_Tcp_TxRetrQueue[FirstEleIdx].UserTimeoutTs;
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].TransmitCounter    = 0;  /* reset the transmit counter for both parts */
      TcpIp_Tcp_TxRetrQueue[FirstEleIdx].TransmitCounter  = 0;  /* reset the transmit counter for both parts */
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].OneTimeOptions     = 0;  /* one-time options will stay with the first segment */
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].OneTimeOptionsLen  = 0;
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
      TcpIp_Tcp_TxRetrQueue[NewEleIdx].SackFlag           = 0;  /* reset the SACK-flag for both parts */
      TcpIp_Tcp_TxRetrQueue[FirstEleIdx].SackFlag         = 0;  /* reset the SACK-flag for both parts */
#endif

      TcpIp_Tcp_TxRetrQueueFillNum[TcpIdx]++;
    }
    else
    {
      /* no new element available */
      /* reduce the size of the first (and only) element of the socket's retry queue, and un-queue the removed length from the socket's request queue */
      TcpIp_Tcp_TxRetrQueueType *ElePtr = &TcpIp_Tcp_TxRetrQueue[TcpIp_Tcp_FirstRetrQueueEle[TcpIdx]];
      uint32 OldSize = ElePtr->SizeTotByte;
      uint32 NewSize = SizePartOne;

      TcpIp_Tcp_VUnqueueDataInReqArray(TcpIdx, (OldSize - NewSize));

      ElePtr->SizeTotByte        = NewSize;
      if(0 != (ElePtr->Flags & TCPIP_TCP_CODE_BIT_FIN))
      {
        ElePtr->Flags           -= (uint8)TCPIP_TCP_CODE_BIT_FIN;
      }
      ElePtr->TransmitCounter    = 0;  /* reset the transmit counter for both parts */
#if (TCPIP_TCP_SACK_OPTION_SUPPORT == STD_ON)
      ElePtr->SackFlag           = 0;  /* reset the SACK-flag for both parts */
#endif
    }

    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* a) segment is smaller (or equal) than the window size, this should be handled by the normal retransmission mechanism
     * b) segment already has size '1', so no splitting possible. Segment will be sent in normal tx handling
     * -> do nothing
     */
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VMainTxDataHandling
 **********************************************************************************************************************/
/*! \brief      Main socket tx data handling
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VMainTxDataHandling(
    TcpIp_TcpIdxType TcpIdx )
{

  /* trigger sending function, eventually this called function will send data onto the bus */
  if( (TCPIP_TCP_SOCK_STATE_CLOSED != TcpIp_Tcp_SockState[TcpIdx]) &&
      (TCPIP_TCP_SOCK_STATE_LISTEN != TcpIp_Tcp_SockState[TcpIdx]) )
  {
    /* trigger queueing data into TxRetrQueue if available */
    while(0 != TcpIp_Tcp_TxTotNotQueuedLen[TcpIdx])
    {
      TcpIp_Tcp_TxReqArrayType* ReqPtr = &TcpIp_Tcp_TxReqArray[TcpIdx];

      if(ReqPtr->FullyPlanned == FALSE)
      {
        if(E_OK != TcpIp_Tcp_VQueueRemReqData(TcpIdx))
        {
          break;
        }
      }
      else
      {
        break;  /* PRQA S 0771 */ /* MD_TCPIP_TCP_14.6_0771_e */
      }
    }

#if (TCPIP_TCP_NAGLE_ALG_SUPPORT == STD_ON)
    if (TcpIp_Tcp_NagleEnabled[TcpIdx] == TRUE)
    {
# if (TcpIp_Tcp_VCfgGetNagleTimeoutCycles() > 0)
      /* - decrease Nagle timeout
      - if Nagle timeout expired -> send now */

      if (TcpIp_Tcp_NagleTimeout[TcpIdx] > 0)
      {
        TcpIp_Tcp_NagleTimeout[TcpIdx]--;
      }
# endif

      /* Build a new TCP segment (even if not completely filled) only if all data has been ACKed so far or the nagle timeout has expired. */
      if (TCPIP_TCP_SEQ1_BIGGER_OR_EQUAL_SEQ2(TcpIp_Tcp_SndUna[TcpIdx], TcpIp_Tcp_TxNextSendSeqNo[TcpIdx])
# if (TcpIp_Tcp_VCfgGetNagleTimeoutCycles() > 0)
        || (TcpIp_Tcp_NagleTimeout[TcpIdx] == 0)       
# endif
         )
      {
        (void) TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE);
      }
    }
    else
#endif
    {
      /* prepare packet for sending */
      (void) TcpIp_Tcp_VFinalizeTxSegment(TcpIdx, TRUE);
    }

    /* trigger sending the packet to the bus */
    (void) TcpIp_Tcp_VTransmit(TcpIdx);
  }

  if( ( (TCPIP_TCP_SOCK_STATE_TIMEWAIT == TcpIp_Tcp_SockState[TcpIdx]) &&
    (TCPIP_TCP_SOCK_STATE_TIMEWAIT != TcpIp_Tcp_SockStateNext[TcpIdx]) ) ||
    (TCPIP_TCP_SOCK_STATE_CLOSED == TcpIp_Tcp_SockState[TcpIdx]) )
  {
    /* clear retransmission queue */
    TcpIp_Tcp_VClearRetrQueue(TcpIdx);
  }
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

#if (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_VUpdateRttVars
 **********************************************************************************************************************/
/*! \brief        Update RTT (round-trip time) dependent variables.
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    ElementRtt           Round Trip Time of one element [cycles]
 *  \context      interrupt or task level
 *  \note         Use the elements RTT to update SRTT (smoothed round-trip time), RTTVAR (RTT variation) and
 *                RTO (retransmission timeout). Calculations according to RFC 2988. A counter increased in every call
 *                of the TcpMainFunction is used as the time base for all round-trip time dependent variables. So the
 *                clock granularity in milli-seconds is TcpIp_VCfgGetMainFctPeriodMsec().
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VUpdateRttVars(
  TcpIp_TcpIdxType TcpIdx,
  uint32 ElementRtt )
{
  CANOE_WRITE_STRING("TcpIp_Tcp_VUpdateRttVars: -------------------------------------------------");
  CANOE_WRITE_STRING_2("TcpIp_Tcp_VUpdateRttVars: TCP connection %d, new ElementRtt is %d)", TcpIdx, ElementRtt);
  CANOE_WRITE_STRING_4("TcpIp_Tcp_VUpdateRttVars old: TcpIp_Tcp_Rto is %d, TcpIp_Tcp_RtoValid is %d, TcpIp_Tcp_SRtt is %dms, TcpIp_Tcp_RttVar is %d)", TcpIp_Tcp_Rto[TcpIdx], TcpIp_Tcp_RtoValid[TcpIdx], TcpIp_Tcp_SRtt[TcpIdx], TcpIp_Tcp_RttVar[TcpIdx]);
  if(2 == TcpIp_Tcp_RtoState[TcpIdx])
  {
    /* normal operation */
    /*
      RTTVAR = (1 - beta) * RTTVAR + beta * |SRTT - R'|
      SRTT   = (1 - alpha) * SRTT + alpha * R'
      The above SHOULD be computed using alpha=1/8 and beta=1/4

      After the computation, a host MUST update
      RTO = SRTT + max (G, K*RTTVAR)

      Whenever RTO is computed, if it is less than 1 second then the RTO SHOULD be rounded up to 1 s.
    */
    uint32 DiffSrttR;
    if(TcpIp_Tcp_SRtt[TcpIdx] > ElementRtt)
    {
      DiffSrttR = TcpIp_Tcp_SRtt[TcpIdx] - ElementRtt;
    }
    else
    {
      DiffSrttR = ElementRtt - TcpIp_Tcp_SRtt[TcpIdx];
    }
    TcpIp_Tcp_RttVar[TcpIdx] = ((3 * TcpIp_Tcp_RttVar[TcpIdx]) + (DiffSrttR))  /4;
    TcpIp_Tcp_SRtt[TcpIdx]   = ((7 * TcpIp_Tcp_SRtt[TcpIdx])   + (ElementRtt)) /8;
    {
      uint32 x1;
      uint32 x2;
      /* RTO = SRTT + max (G, K*RTTVAR)    where K = 4 */
      x1 = 1;  /* clock granularity */
      x2 = TcpIp_Tcp_RttVar[TcpIdx] *4;  /* K * RTTVAR    where K = 4 */
      if(x1 > x2)
      {
        TcpIp_Tcp_Rto[TcpIdx] = TcpIp_Tcp_SRtt[TcpIdx] + x1;
      }
      else
      {
        TcpIp_Tcp_Rto[TcpIdx] = TcpIp_Tcp_SRtt[TcpIdx] + x2;
      }
    }
  }
  else
  {
    /* 1 == TcpIp_Tcp_RtoState[TcpIdx]) */
    /* This is the very first calculation ot the round-trip time depenedent variables for this connection */
    uint32 x1;
    uint32 x2;

    TcpIp_Tcp_SRtt[TcpIdx]   = ElementRtt;
    TcpIp_Tcp_RttVar[TcpIdx] = ElementRtt /2;  /* R/2 */
    {
      /* RTO = SRTT + max (G, K*RTTVAR)    where K = 4 */
      x1 = 1;  /* clock granularity */
      x2 = TcpIp_Tcp_RttVar[TcpIdx] *4;  /* K * RTTVAR    where K = 4 */
      if(x1 > x2)
      {
        TcpIp_Tcp_Rto[TcpIdx] = TcpIp_Tcp_SRtt[TcpIdx] + x1;
      }
      else
      {
        TcpIp_Tcp_Rto[TcpIdx] = TcpIp_Tcp_SRtt[TcpIdx] + x2;
      }
    }

    TcpIp_Tcp_RtoState[TcpIdx] = 2;  /* go to next state */
  }

  if(TcpIp_Tcp_Rto[TcpIdx] < TcpIp_Tcp_VCfgGetTxRetryIntMinCycles())
  {
    TcpIp_Tcp_RtoValid[TcpIdx] = TcpIp_Tcp_VCfgGetTxRetryIntMinCycles();
  }
  else
  {
    TcpIp_Tcp_RtoValid[TcpIdx] = TcpIp_Tcp_Rto[TcpIdx];
  }
  CANOE_WRITE_STRING_4("TcpIp_Tcp_VUpdateRttVars new: TcpIp_Tcp_Rto is %d, TcpIp_Tcp_RtoValid is %d, TcpIp_Tcp_SRtt is %dms, TcpIp_Tcp_RttVar is %d)", TcpIp_Tcp_Rto[TcpIdx], TcpIp_Tcp_RtoValid[TcpIdx], TcpIp_Tcp_SRtt[TcpIdx], TcpIp_Tcp_RttVar[TcpIdx]);

}
#endif  /* (TCPIP_TCP_TIMESTAMP_OPTION_SUPPORT == STD_ON) */

#if (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_VIncAckSumN
 **********************************************************************************************************************/
/*! \brief        Increase the sum of acknowlaged bytes.
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    IncSize              Size to add (acked bytes)
 *  \context      interrupt or task level
 *  \note         This function is only called while the socket is in state 'congestion avoidance'.
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VIncAckSumN(
  TcpIp_TcpIdxType TcpIdx,
  uint32 IncSize )
{
  TcpIp_Tcp_AckSumN[TcpIdx] += IncSize;

  if(0 == TcpIp_Tcp_AckSumTimeout[TcpIdx])
  {
    if(0 == TcpIp_Tcp_SRtt[TcpIdx])
    {
      TcpIp_Tcp_AckSumTimeout[TcpIdx] = 1;
    }
    else
    {
      TcpIp_Tcp_AckSumTimeout[TcpIdx] = TcpIp_Tcp_SRtt[TcpIdx];
    }
  }
  CANOE_WRITE_STRING_3("TcpIp_Tcp_VIncAckSumN: TcpIdx %d: current AckSumTimeout is %d, TcpIp_Tcp_SRtt is %d", TcpIdx, TcpIp_Tcp_AckSumTimeout[TcpIdx], TcpIp_Tcp_SRtt[TcpIdx]);
  CANOE_WRITE_STRING_4("TcpIp_Tcp_VIncAckSumN: TcpIdx %d: function called with IncSize %d, new AckSum is %d, current AckSumTimeout is %d", TcpIdx, IncSize, TcpIp_Tcp_AckSumN[TcpIdx], TcpIp_Tcp_AckSumTimeout[TcpIdx]);
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VIncCwnd
 **********************************************************************************************************************/
/*! \brief        Increase the congestion window.
 *  \param[in]    TcpIdx               TCP index
 *  \param[in]    IncSize              Size to add (acked bytes)
 *  \context      interrupt or task level
 *  \note
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VIncCwnd(
  TcpIp_TcpIdxType TcpIdx,
  uint32 IncSize )
{
  CANOE_WRITE_STRING_3("TcpIp_Tcp_VIncCwnd: TcpIdx %d: function called with IncSize %d, current cwnd is %d", TcpIdx, IncSize, TcpIp_Tcp_Cwnd[TcpIdx]);
  /* cwnd += min(N, SMSS) */
  if(IncSize > TcpIp_Tcp_TxMaxSegLenByte[TcpIdx])   /* SMSS = TcpIp_Tcp_SockMtuSize[TcpIdx] */
  {
    TcpIp_Tcp_Cwnd[TcpIdx] += TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];
  }
  else
  {
    TcpIp_Tcp_Cwnd[TcpIdx] += IncSize;
  }

  if(TcpIp_Tcp_Cwnd[TcpIdx] <= TcpIp_Tcp_Ssthresh[TcpIdx])
  {
    TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_SLOW_START;
  }
  else
  {
    TcpIp_Tcp_CongestionControlMode[TcpIdx] = TCPIP_TCP_CONG_CTRL_MODE_CONGESTION_AVOIDANCE;
  }
  CANOE_WRITE_STRING_3("TcpIp_Tcp_VIncCwnd: TcpIdx %d: TcpIp_Tcp_Cwnd is now %d, TcpIp_Tcp_CongestionControlMode is now %d", TcpIdx, TcpIp_Tcp_Cwnd[TcpIdx], TcpIp_Tcp_CongestionControlMode[TcpIdx]);

}

/***********************************************************************************************************************
 *  TcpIp_Tcp_VSetSsthreshEqu4
 **********************************************************************************************************************/
/*! \brief        Set Ssthresh according to equation (4) in RFC5681.
 *  \param[in]    TcpIdx               TCP index
 *  \context      interrupt or task level
 *  \note
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VSetSsthreshEqu4(
  TcpIp_TcpIdxType TcpIdx )
{
  /* ssthresh = max[FlightSize / 2, 2*SMSS] */
  uint32 FlightSize;  /* FlightSize is the amount of outstanding data in the network */
  uint32 Smss;  /* sender maximum segment size */
  TcpIp_TcpTxRetrQueueIdxType EleIdx;
  Smss = TcpIp_Tcp_TxMaxSegLenByte[TcpIdx];

  /* check all retry queue elements for this socket */
  FlightSize = 0;
  EleIdx = TcpIp_Tcp_FirstRetrQueueEle[TcpIdx];
  while((EleIdx != TCPIP_TCP_TX_RETR_QUEUE_INV_IDX) && (TcpIp_Tcp_TxRetrQueue[EleIdx].TransmitCounter != 0))
  {
    FlightSize += TcpIp_Tcp_TxRetrQueue[EleIdx].SizeTotByte;
    EleIdx      = TcpIp_Tcp_TxRetrQueue[EleIdx].EleNext;
  }

  if((FlightSize /2) > (Smss *2))
  {
    TcpIp_Tcp_Ssthresh[TcpIdx] = (FlightSize /2);
  }
  else
  {
    TcpIp_Tcp_Ssthresh[TcpIdx] = (Smss *2);
  }
  CANOE_WRITE_STRING_2("TcpIp_Tcp_VSetSsthreshEqu4: TcpIdx %d: TcpIp_Tcp_Ssthresh is now %d", TcpIdx, TcpIp_Tcp_Ssthresh[TcpIdx]);
}
#endif  /* (TCPIP_TCP_CONGESTION_CONTROL_SUPPORT == STD_ON) */

#if (TCPIP_TCP_KEEP_ALIVE_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveTrigger
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveTrigger(TcpIp_TcpIdxType tcpIdx)
{
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveIdleTime = 0;
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveProbes   = 0;
} /* TcpIp_Tcp_VKeepAliveTrigger() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveReset
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveReset(TcpIp_TcpIdxType tcpIdx)
{
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveEnabled       = FALSE;
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveTimeout       = TcpIp_Tcp_VCfgGetKeepAliveTimeoutDefCycles();
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveProbeInterval = TcpIp_Tcp_VCfgGetKeepAliveIntervalDefCycles();
  TcpIp_Tcp_KeepAliveStatus[tcpIdx].KeepAliveProbesMax     = TcpIp_Tcp_VCfgGetKeepAliveMaxProbesDef();

  TcpIp_Tcp_VKeepAliveTrigger(tcpIdx);
} /* TcpIp_Tcp_VKeepAliveReset() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VKeepAliveCopyParameters
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_VKeepAliveCopyParameters(TcpIp_TcpIdxType tcpIdxSrc, TcpIp_TcpIdxType tcpIdxDst)
{
  TcpIp_Tcp_KeepAliveStatus[tcpIdxDst].KeepAliveEnabled       = TcpIp_Tcp_KeepAliveStatus[tcpIdxSrc].KeepAliveEnabled;
  TcpIp_Tcp_KeepAliveStatus[tcpIdxDst].KeepAliveTimeout       = TcpIp_Tcp_KeepAliveStatus[tcpIdxSrc].KeepAliveTimeout;
  TcpIp_Tcp_KeepAliveStatus[tcpIdxDst].KeepAliveProbeInterval = TcpIp_Tcp_KeepAliveStatus[tcpIdxSrc].KeepAliveProbeInterval;
  TcpIp_Tcp_KeepAliveStatus[tcpIdxDst].KeepAliveProbesMax     = TcpIp_Tcp_KeepAliveStatus[tcpIdxSrc].KeepAliveProbesMax;
} /* TcpIp_Tcp_VKeepAliveReset() */

/***********************************************************************************************************************
 *  TcpIp_Tcp_VTransmitKeepAliveProbe
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_VTransmitKeepAliveProbe(TcpIp_TcpIdxType tcpIdx)
{
  return TcpIp_Tcp_VTransmitSegment(tcpIdx, TCPIP_TCP_CODE_BIT_ACK, TcpIp_Tcp_SndNxt[tcpIdx]-1, 0, 0, 0);
} /* TcpIp_Tcp_VTransmitKeepAliveProbe() */

#endif

/***********************************************************************************************************************
 *  TcpIp_Tcp_StoreChecksumInHeader
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_StoreChecksumInHeader(
    uint16           Checksum,
    TCPIP_P2V(uint8) TcpHdrPtr)
{
  TcpIp_Tcp_HdrType *HdrPtr = (TcpIp_Tcp_HdrType*)TcpHdrPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

  HdrPtr->Checksum = Checksum;

} /* TcpIp_Tcp_StoreChecksumInHeader() */

/***********************************************************************************************************************
 *  TcpIp_AddWithWrapAround16
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(uint16, TCPIP_CODE) TcpIp_AddWithWrapAround16(
  uint16 value, 
  uint16 incValue, 
  uint16 valueCount)
{
  uint16 remLen;

  TCPIP_ASSERT(value    < valueCount);
  TCPIP_ASSERT(incValue < valueCount);

  remLen = valueCount - value - 1;

  if (incValue <= remLen)
  {
    value += incValue;
  }
  else
  {
    value = (incValue - remLen - 1);
  }

  return value;
}

#if (TCPIP_SUPPORT_TLS == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsRxIndication
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsRxIndication(
    TcpIp_SocketIdType                                    SocketId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)              DataPtr,
    uint16                                                DataLen)
{
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    TcpIp_TcpIdxType TcpIdx = TCPIP_SOCKIDX2TCPIDX(SocketId);
    if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
    {
      uint8 UserId = TcpIp_SockUserIdx[SocketId];
      TcpIp_RxIndicationFunctions(UserId, SocketId, (TcpIp_SockAddrType *)(&(TcpIp_Tcp_RxSockInfo[TcpIdx].RemSock)), DataPtr, DataLen);
    }
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsTxConfirmation
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsTxConfirmation(
    TcpIp_SocketIdType                                    SocketId,
    uint16                                                DataLen)
{
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    TcpIp_TcpIdxType TcpIdx = TCPIP_SOCKIDX2TCPIDX(SocketId);
    if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
    {
      uint8 UserId = TcpIp_SockUserIdx[SocketId];
      TcpIp_TxConfirmationFunctions(UserId, SocketId, DataLen);
    }
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsConnected
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsConnected(
    TcpIp_SocketIdType SocketId )
{
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    TcpIp_TcpIdxType TcpIdx = TCPIP_SOCKIDX2TCPIDX(SocketId);
    if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
    {
      uint8 UserId = TcpIp_SockUserIdx[SocketId];
      TcpIp_TcpConnectedFunctions(UserId, SocketId);
    }
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsEvent
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsEvent(
    TcpIp_SocketIdType SocketId,
    TcpIp_EventType Event )
{
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    uint8 UserId = TcpIp_SockUserIdx[SocketId];
    TcpIp_TcpIpEventFunctions(UserId, SocketId, Event);
  }
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsValidationResult
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsValidationResult(
    TcpIp_SocketIdType  SocketId,
    TCPIP_P2V(uint8)    ValidationResultPtr,
    TCPIP_P2C(uint8)    CertChainPtr,
    uint32              CertChainLen )
{
  uint8 userId;

  TCPIP_ASSERT(TCPIP_COND_VALID_TCPSOCKIDX(SocketId));

  userId = TcpIp_SockUserIdx[SocketId];
  /* #10 Forward the valdidation result to the user. */
  TcpIp_TlsValidationResultFunctions(userId, SocketId, ValidationResultPtr, CertChainPtr, CertChainLen);
}

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsReceived
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsReceived(
    TcpIp_SocketIdType SocketId,
    uint32 DataLenByte )
{
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    TcpIp_TcpIdxType TcpIdx = TCPIP_SOCKIDX2TCPIDX(SocketId);
    if(TcpIp_Tcp_UseTls[TcpIdx] == TRUE)
    {
      TcpIp_Tcp_Received(TcpIdx, DataLenByte);
    }
  }
  
}

#endif

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* (TCPIP_SUPPORT_TCP == STD_ON) */

/* module specific MISRA deviations:

   module global deviations -> see file TcpIp.c

   TCPIP_TCP-specific deviations:
   MD_TCPIP_TCP_9.1_3347_a:
      Reason:     The variable SackNum is always initialized properly if TcpIp_Tcp_OooNumElements[TcpIdx] is not '0'
                  and SackOption is active for this connection. The path to init the variable has the same condition as
                  the current path (TcpIp_Tcp_OooNumElements and TcpIp_Tcp_SackOptActive).
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_9.1_3353_a:
      Reason:     LastValidEleIdx is always properly initialized if NumCheckedElements is not '0'
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_9.1_3353_b:
      Reason:     The variable SackNum is always initialized properly if TcpIp_Tcp_OooNumElements[TcpIdx] is not '0'
                  and SackOption is active for this connection. The path to init the variable has the same condition as
                  the current path (TcpIp_Tcp_OooNumElements and TcpIp_Tcp_SackOptActive).
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_14.6_0771_a:
      Reason:     The break statement is used multiple (two) times in a do-until struct to find a condition match. This
                  seems to be the most efficient way to do that.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_14.6_0771_b:
      Reason:     The break statement is used multiple times to stop the function TcpIp_Tcp_VTransmit from sending
                  queued elements. Once one element can't be sent, the following ones shouldn't be sent, neither.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_14.6_0771_c:
      Reason:     The break statement is used multiple times to stop the function TcpIp_Tcp_VTransmitResets from sending
                  queued reset elements. Once one element can't be sent, the following ones shouldn't be sent, neither.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_14.6_0771_d:
      Reason:     The break statement is used multiple (two) times times to stop the function
                  TcpIp_Tcp_VFreeAckedReqAndRetrElements from freeing further elements. Using a break is more efficient
                  than using an extra control variable, and the function is only very small and clear.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_14.6_0771_e:
      Reason:     The break statement is used multiple (two) times times to stop the function
                  TcpIp_Tcp_VMainTxDataHandling from trying to send more data. Using a break is more efficient than
                  using an extra control variable, and the function is only very small and clear.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_STPAR_6060:
      Reason:     The number of function parameters an not be reduced without extra copy actions to put several
                  parameters into one structure.
      Risk:       Some restricted compilers might not be able to compile the code.
      Prevention: Covered by code review.
   MD_TCPIP_TCP_0832
      Reason:     Include files shall not be define via a macro, but here this is done only for testing purposes.
      Risk:       Some compilers might not support defining an include file via macros.
      Prevention: Covered by code review.

*/

/**********************************************************************************************************************
 *  END OF FILE: Tcp.c
 *********************************************************************************************************************/
