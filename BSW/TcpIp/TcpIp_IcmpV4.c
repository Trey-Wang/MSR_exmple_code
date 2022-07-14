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
 *         \file  TcpIp_IcmpV4.c
 *        \brief  Implementation of Internet Control Message Protocol for IPv4 (ICMPv4)
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_ICMPV4_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)

#include "TcpIp_IpV4_Cfg.h"
#if (IPV4_SUPPORT_ICMP  == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ComStack_Types.h"

#include "TcpIp_IcmpV4.h"
#include "TcpIp_IcmpV4_Cbk.h"
#include "TcpIp_IpV4_Priv.h"
#include "TcpIp_IpV4_Lcfg.h"

#include "TcpIp_IpV4.h"
#include "TcpIp_Priv.h"

#include "IpBase_Copy.h"
#include "IpBase.h"

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define IPV4_ICMP_HEADER_LEN                   8
#define IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN    64

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  uint8    Type;
  uint8    Code;
  uint16   Checksum;
} IpV4_Icmp_HdrStartType;

typedef struct
{
  uint8    Type;
  uint8    Code;
  uint16   Checksum;
  uint16   Identifier;
  uint16   SequNum;
} IpV4_Icmp_EchoMsgType; /* Echo or Echo Request */

typedef struct
{
  IpBase_AddrInType  Dest;
  uint16          Identifier;
  uint16          SequNum;
  IpV4_AddrIdType IpAddrId;
} IpV4_Icmp_PongRetryType; /* data to retry sending a PONG */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC VAR(IpV4_Icmp_PongRetryType, IPV4_VAR_NOINIT) IpV4_Icmp_EchoReplyPendingMsg;
STATIC VAR(boolean,                 IPV4_VAR_NOINIT) IpV4_Icmp_EchoReplyIsPending;

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC VAR(uint16, IPV4_VAR_NOINIT) IpV4_Icmp_SeqNum;
STATIC VAR(uint16, IPV4_VAR_NOINIT) IpV4_Icmp_EchoReplyDataLen;

#define IPV4_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV4_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/***********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingEchoReply
 **********************************************************************************************************************/
/*! \brief        Tries to send a pending echo reply, if there is one.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Icmp_VSendPendingEchoReply(void);

/***********************************************************************************************************************
 *  IpV4_Icmp_VRxEchoRequest
 **********************************************************************************************************************/
/*! \brief        Receives an ICMP Echo Request Message and sends an Echo Reply.
 *  \details      If the Echo reply cannot be sent directly because there is no ethernet buffer available or link-layer
 *                address resolution is still in progress, transmission of the Echo Reply will be scheduled for the MainFunction.
 *  \param[in]    AddrId            Local IP address identifier.
 *  \param[in]    RemNetAddr        Remote IP address value.
 *  \param[in]    DataPtr           Payload of the ICMP message starting with the Echo Request Identifier and Sequence Number values.
 *  \param[in]    DataLen           Length of the Payload.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Icmp_VRxEchoRequest(
  IpV4_AddrIdType   AddrId,
  IpBase_AddrInType RemNetAddr,
  IPV4_P2C(uint8)   DataPtr,
  uint16            DataLen);

/***********************************************************************************************************************
 *  IpV4_Icmp_VSendRawMessage
 **********************************************************************************************************************/
/*! \brief        Sends an ICMP message with the specified Type, Code and payload.
 *  \details      This function will automatically send a fragmented IP packet if the payload exceeds the MTU and
 *                fragmentation is enabled.
 *  \param[in]    AddrId            Source IP address identifier.
 *  \param[in]    DestIpAddr        Destination IP address value.
 *  \param[in]    Type              Type of the ICMP message.
 *  \param[in]    Code              Code of the ICMP message.
 *  \param[in]    VarHeaderBytesPtr Pointer to the 4 message dependent bytes that follow the Type, Code and Checksum
 *                                  fields in the ICMP header.
 *                                  Note: Exactly four bytes will be read from the pointer destination.
 *  \param[in]    PayloadPtr        Pointer to the payload of the ICMPv4 message.
 *  \param[in]    PayloadLen        Length of the payload in bytes.
 *  \return       BUFREQ_OK         Transmit triggered.
 *  \return       BUFREQ_E_BUSY     Transmit currently not possible. Retry later.
 *  \return       BUFREQ_E_NOT_OK   Transmit not possible due to unknown error.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Icmp_VSendRawMessage(
  IpV4_AddrIdType          AddrId,
  IpBase_AddrInType        DestIpAddr,
  uint8                    Type,
  uint8                    Code,
  IPV4_P2C(uint8)          VarHeaderBytesPtr,
  IPV4_P2C(uint8)          PayloadPtr,
  uint16                   PayloadLen);

/***********************************************************************************************************************
 *  IpV4_Icmp_SendEchoMessage
 **********************************************************************************************************************/
/*! \brief        Sends an ICMP Echo (Request/Reply) message.
 *  \details      -
 *  \param[in]    AddrId          Source IP address identifier.
 *  \param[in]    DestIpAddr      Destination IP address.
 *  \param[in]    IcmpType        Type of the ICMP message (ECHO_REQUEST, ECHO_REPLY).
 *  \param[in]    IcmpCode        Code of the ICMP message (usually 0).
 *  \param[in]    Identifier      Identifier of the Echo message
 *  \param[in]    SequenceNumber  Sequence Number of the ICMP message.
 *  \param[in]    DataPtr         Pointer to the payload of the Echo message.
 *  \param[in]    DataLenByte     Length of the payload in bytes.
 *  \return       BUFREQ_OK       Transmit triggered.
 *  \return       BUFREQ_E_BUSY   Transmit currently not possible. Retry later.
 *  \return       BUFREQ_E_NOT_OK Transmit not possible.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Icmp_SendEchoMessage(
  IpV4_AddrIdType          AddrId,
  IpBase_AddrInType        DestIpAddr,
  uint8                    IcmpType,
  uint8                    IcmpCode,
  uint16                   Identifier,
  uint16                   SequenceNumber,
  IPV4_P2C(uint8)          DataPtr,
  uint16                   DataLenByte);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  IpV4_Icmp_Init
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_Init(
  P2CONST(IpV4_Icmp_ConfigType, IPV4_CONST, IPV4_CONST) CfgPtr )
{
  IPV4_UNUSED_PARAMETER(CfgPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  IpV4_Icmp_EchoReplyIsPending = FALSE;
  IpV4_Icmp_SeqNum = 0;
} /* IpV4_Icmp_Init() */

/***********************************************************************************************************************
 *  IpV4_Icmp_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_MainFunction(void)
{
  /* ----- Implementation ----------------------------------------------- */ 
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);

  /* #10 Send pending ICMPv4 Echo Replies. */
  if(IpV4_Icmp_EchoReplyIsPending == TRUE)
  {
    IpV4_Icmp_VSendPendingEchoReply();
  }

} /* IpV4_Icmp_MainFunction() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Icmp_RxIndication
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_RxIndication(
  IpV4_AddrIdType AddrId,
  IpBase_AddrInType RemNetAddr,
  uint8             Ttl,
  IPV4_P2V(uint8)   DataPtr,
  uint16            DataLen,
    boolean           ChecksumCalculated)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IPV4_COND_VALID_IPADDRID(AddrId));
  TCPIP_ASSERT(IPV4_COND_VALID_PTR(DataPtr));

#if (IPV4_ICMP_SILENT_MODE == STD_OFF)
  if (DataLen >= IPV4_ICMP_HEADER_LEN)
  {
# if (IPV4_ICMP_CHECK_RX_CHECKSUM == STD_ON)
    /* #10 Check if the checksum is calculated. */
    if (ChecksumCalculated == FALSE) 
    {
      ChecksumCalculated = (boolean)(IpBase_TcpIpChecksumAdd(DataPtr, DataLen, 0, TRUE) == 0);
    }

    if (ChecksumCalculated == TRUE)
# else
    IPV4_UNUSED_PARAMETER(ChecksumCalculated);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif /* IPV4_ICMP_CHECK_RX_CHECKSUM */
    {
      IPV4_P2C(IpV4_Icmp_HdrStartType) icmpHdrStartPtr = (IPV4_P2V(IpV4_Icmp_HdrStartType))DataPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */
      uint16                           dataPos         = 4;
      boolean                          messageHandled  = FALSE;

      if (icmpHdrStartPtr->Type == IPV4_ICMP_TYPE_ECHO)
      {
        if (icmpHdrStartPtr->Code == 0)
        { /* #20 Send the Echo reply. */
          IpV4_Icmp_VRxEchoRequest(AddrId, RemNetAddr, &DataPtr[dataPos], DataLen - dataPos);
          messageHandled = TRUE;
        }
      }

      if ((messageHandled == FALSE) && (IpV4_Icmp_MsgHandler != NULL_PTR))
      {
        TcpIp_SockAddrInetType remoteSockAddr;
        remoteSockAddr.domain  = TCPIP_AF_INET;
        remoteSockAddr.port    = 0;
        remoteSockAddr.addr[0] = RemNetAddr; 
        /* #30 Discard the message. */
        /* ICMP type not supported -> silently discard message */
        IpV4_Icmp_MsgHandler(IPV4_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(AddrId), (IPV4_P2C(TcpIp_SockAddrType))&remoteSockAddr, Ttl, icmpHdrStartPtr->Type, icmpHdrStartPtr->Code, DataLen - dataPos, &DataPtr[dataPos]);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
      }
    }
  }
#endif
}  /* IpV4_Icmp_RxIndication() */ /* PRQA S 2006, 6010, 6030, 6060 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR */

/***********************************************************************************************************************
 *  IpV4_Icmp_SendEcho
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Icmp_SendEcho(
  IpV4_AddrIdType          AddrId,
  IpBase_AddrInType        Dest,
  IPV4_P2C(uint8)          DataPtr,
  uint16                   LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

 /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IPV4_COND_VALID_OR_ANY_IPADDRID(AddrId));
  TCPIP_ASSERT(IPV4_COND_VALID_PTR(DataPtr));

  /* #10 Increment the sequence number. */
  IpV4_Icmp_SeqNum++;
  /* #20 Check if echo message has been send successfully.*/
  if (IpV4_Icmp_SendEchoMessage(AddrId, Dest, IPV4_ICMP_TYPE_ECHO, 0, 0, IpV4_Icmp_SeqNum, DataPtr, LenByte) == BUFREQ_OK)
  {
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* IpV4_Icmp_SendEcho() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (IPV4_ICMP_ENABLE_DST_UNREACHABLE_MSG == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Icmp_SendDestUnreachable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_SendDestUnreachable(
  IpV4_AddrIdType           IpAddrId,
  IPV4_P2C(IpV4_Ip_HdrType) IpHeaderPtr,
  uint8                     IcmpCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 unusedHeaderBytes[4] = { 0, 0, 0, 0 };
  uint8  ipHdrLen     = IPV4_HDR_FIELD_GET_IHL_BYTE(IpHeaderPtr);
  uint16 ipPayloadLen = IPV4_HDR_FIELD_GET_TOTAL_LEN(IpHeaderPtr) - ipHdrLen;

  /* ----- Implementation ----------------------------------------------- */
  
  /* #10 Limit original IP packet payload to 64 bytes according to IETF RFC 793. */
  if (ipPayloadLen > IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN)
  {
    ipPayloadLen = IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN;
  }

  /* #20 Send ICMP Destination Unreachable message. */
  (void)IpV4_Icmp_VSendRawMessage(IpAddrId, IpHeaderPtr->SrcAddr, IPV4_ICMP_TYPE_DEST_UNRECHABLE, IcmpCode, &unusedHeaderBytes[0], (IPV4_P2C(uint8))IpHeaderPtr, ipHdrLen + ipPayloadLen);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
} /* IpV4_Icmp_SendDestUnreachable() */
#endif /* (IPV4_ICMP_ENABLE_DST_UNREACHABLE_MSG == STD_ON) */

/***********************************************************************************************************************
 *  IpV4_Icmp_SendEchoMessage
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Icmp_SendEchoMessage(
  IpV4_AddrIdType          AddrId,
  IpBase_AddrInType        DestIpAddr,
  uint8                    IcmpType,
  uint8                    IcmpCode,
  uint16                   Identifier,
  uint16                   SequenceNumber,
  IPV4_P2C(uint8)          DataPtr,
  uint16                   DataLenByte)
{
   /* ----- Local Variables ---------------------------------------------- */
  uint8 icmpHeaderBytes[4];

  /* ----- Implementation ----------------------------------------------- */
  IPBASE_PUT_UINT16(&icmpHeaderBytes[0], 0, Identifier);
  IPBASE_PUT_UINT16(&icmpHeaderBytes[0], 2, SequenceNumber);

  return IpV4_Icmp_VSendRawMessage(AddrId, DestIpAddr, IcmpType, IcmpCode, &icmpHeaderBytes[0], DataPtr, DataLenByte);
} /* IpV4_Icmp_SendEchoMessage() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/***********************************************************************************************************************
 *  IpV4_Icmp_VRxEchoRequest
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Icmp_VRxEchoRequest(
  IpV4_AddrIdType   AddrId,
  IpBase_AddrInType RemNetAddr,
  IPV4_P2C(uint8)   DataPtr,
  uint16            DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 echoIdentifier;
  uint16 echoSeqNum;
  uint16 dataPos = 0;
  uint16 echoDataLen;
  /* ----- Implementation ----------------------------------------------- */
  IPBASE_GET_UINT16(DataPtr, dataPos, echoIdentifier);
  dataPos += 2;
  IPBASE_GET_UINT16(DataPtr, dataPos, echoSeqNum);
  dataPos += 2;

  echoDataLen = DataLen - dataPos;

  /* #10 Try to send Echo reply directly. */
  if (IpV4_Icmp_SendEchoMessage(AddrId, RemNetAddr, IPV4_ICMP_TYPE_ECHO_REPLY, 0, echoIdentifier, echoSeqNum, &DataPtr[dataPos], echoDataLen) == BUFREQ_E_BUSY)
  {
    /* #20 Schedule pending reply for next MainFunction, if message cannot be sent directly due to pending link-layer address resolution or because no ethernet buffer is available. */
    IpV4_Icmp_EchoReplyPendingMsg.IpAddrId   = AddrId;
    IpV4_Icmp_EchoReplyPendingMsg.Dest       = RemNetAddr;
    IpV4_Icmp_EchoReplyPendingMsg.Identifier = echoIdentifier;
    IpV4_Icmp_EchoReplyPendingMsg.SequNum    = echoSeqNum;

    if (echoDataLen > IpV4_Icmp_VCfgGetEchoReplyPendLenMax())
    {
      echoDataLen = IpV4_Icmp_VCfgGetEchoReplyPendLenMax();
    }

    /* copy data that was received with incoming message */
    IpBase_Copy(&IpV4_Icmp_EchoReplyData[0], &DataPtr[dataPos], echoDataLen); /* PRQA S 0310 */ /* MD_IpV4_11.04 */

    IpV4_Icmp_EchoReplyDataLen   = echoDataLen;
    IpV4_Icmp_EchoReplyIsPending = TRUE;
  }
} /* IpV4_Icmp_VRxEchoRequest() */

/***********************************************************************************************************************
 *  IpV4_Icmp_VSendRawMessage
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
IPV4_LOCAL FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Icmp_VSendRawMessage(
  IpV4_AddrIdType          AddrId,
  IpBase_AddrInType        DestIpAddr,
  uint8                    Type,
  uint8                    Code,
  IPV4_P2C(uint8)          VarHeaderBytesPtr,
  IPV4_P2C(uint8)          PayloadPtr,
  uint16                   PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType               retVal;
  TcpIp_IpTxRequestDescriptorType ipTxReqestDescr;
  uint16                          ipPayloadLen = IPV4_ICMP_HEADER_LEN + PayloadLen;
  IPV4_P2V(uint8)                 ipBufPtr; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
  uint16                          ipBufLen = ipPayloadLen;
  IpBase_SockAddrInType           destSockAddr;

  /* ----- Implementation ----------------------------------------------- */
  destSockAddr.sin_family = IPBASE_AF_INET;
  destSockAddr.sin_addr   = DestIpAddr;
  destSockAddr.sin_port   = 0;
  /* #10 Obtain TX buffer from IP.*/
  retVal = IpV4_Ip_ProvideTxBuffer(&AddrId, (TcpIp_SockIdxType)(IpV4_Icmp_VCfgGetTxSockIdx()),
                                             &destSockAddr, &ipBufPtr, &ipBufLen, &ipTxReqestDescr, FALSE);

  if (retVal == BUFREQ_OK)
  {
    boolean copyTxData     = TRUE;
    uint16  dataRemain     = PayloadLen;
    uint16  dataPos        = 0;
    /* #20 Write ICMP message header. */
    /* IPv4 may provide multiple buffers if message exceeds link MTU,
     * but the first buffer will always be large enough for ICMP header.
     */
    
    /* Write ICMP type and code into message header. */
    ipBufPtr[0] = Type;
    ipBufPtr[1] = Code;

    /* Initialize checksum field. Value will be updated with calculated checksum later. */
    ipBufPtr[2] = 0;
    ipBufPtr[3] = 0;

    /* Write the four message dependent bytes in the ICMP header. */
    ipBufPtr[4] = VarHeaderBytesPtr[0];
    ipBufPtr[5] = VarHeaderBytesPtr[1];
    ipBufPtr[6] = VarHeaderBytesPtr[2];
    ipBufPtr[7] = VarHeaderBytesPtr[3];

    /* #30 Copy message payload into IP buffer.*/
    /* Copy message payload into IP buffer.
     * - Iterate over multiple IP buffer fragments if a fragmented IP packet needs to be sent. 
     */
    ipBufPtr  = &ipBufPtr[sizeof(IpV4_Icmp_EchoMsgType)];
    ipBufLen -= sizeof(IpV4_Icmp_EchoMsgType);
    /* #40 Iterate over multiple IP buffer fragments if a fragmented IP packet needs to be sent.*/
    while ((copyTxData == TRUE) && (dataRemain > 0))
    {
      if (dataRemain < ipBufLen)
      {
        ipBufLen = dataRemain;
      }

      /* Copy payload. */
      IpBase_Copy(ipBufPtr, &PayloadPtr[dataPos], ipBufLen); /* PRQA S 0310 */ /* MD_IpV4_11.04 */

      dataRemain -= ipBufLen;
      dataPos    += ipBufLen;

      if (dataRemain > 0)
      {
        /* Request next buffer fragment */
        if (IpV4_Ip_FragmentProvideTxBuffer(&ipTxReqestDescr, &ipBufPtr, &ipBufLen) != E_OK)
        {
          /* ERROR: This should not happen. If IpV4_Ip_ProvideTxBuffer() returns BUFREQ_OK there should be sufficient buffer space. */
          IpV4_ReportDetError(IPV4_IP_API_ID_V_INTERNAL_FUNCTION, IPV4_E_INV_PARAM);
          copyTxData = FALSE;
        }
      }
    }
    
    /* send message by IP: */
    /* #50 Trigger transmit of IP packet.*/
    if(IpV4_Ip_Transmit(&ipTxReqestDescr, ipPayloadLen, IPV4_IP_PROTOCOL_ICMP, FALSE, 0, FALSE) != E_OK)
    {
      /* transmit failed */
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  return retVal;
} /* IpV4_Icmp_VSendRawMessage() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/***********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingEchoReply
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Icmp_VSendPendingEchoReply(void)
{ 
  /* ----- Implementation ----------------------------------------------- */
  if (IpV4_Icmp_EchoReplyIsPending == TRUE)
  {
    if (BUFREQ_E_BUSY != IpV4_Icmp_SendEchoMessage(IpV4_Icmp_EchoReplyPendingMsg.IpAddrId, IpV4_Icmp_EchoReplyPendingMsg.Dest, IPV4_ICMP_TYPE_ECHO_REPLY, 0,
                                          IpV4_Icmp_EchoReplyPendingMsg.Identifier, IpV4_Icmp_EchoReplyPendingMsg.SequNum,
                                          IpV4_Icmp_EchoReplyData, IpV4_Icmp_EchoReplyDataLen))
    {
      /* Echo Reply sent or error occurred. No retry required. */
      IpV4_Icmp_EchoReplyIsPending = FALSE;
    }
    else
    {
      /* Retry later */
    }
  }
} /* IpV4_Icmp_VSendPendingEchoReply() */

/***********************************************************************************************************************
 *  IpV4_Icmp_Cbk_VAddrResTimeout
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_Cbk_VAddrResTimeout(
  uint8                                                   IpCtrlIdx,
  P2CONST(IpBase_SockAddrType, AUTOMATIC, IPV4_APPL_DATA) DestAddrPtr )
{
  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  if (IpV4_Icmp_EchoReplyPendingMsg.Dest == ((const IpBase_SockAddrInType*)DestAddrPtr)->sin_addr) /* PRQA S 0310 */ /* MD_IpV4_11.04 */
  {
    /* cancel ICMP echo reply */
    IpV4_Icmp_EchoReplyIsPending = FALSE;
  }
} /* IpV4_Icmp_Cbk_VAddrResTimeout() */

/***********************************************************************************************************************
 *  IpV4_Icmp_StoreChecksumInHeader
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_StoreChecksumInHeader(
  uint16          Checksum,
  IPV4_P2V(uint8) IcmpHdrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  IPV4_P2V(IpV4_Icmp_HdrStartType) msgPtr = (IPV4_P2V(IpV4_Icmp_HdrStartType))IcmpHdrPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */

  /* calculate checksum */
  msgPtr->Checksum = Checksum;

} /* IpV4_Icmp_StoreChecksumInHeader() */

/***********************************************************************************************************************
 *  IpV4_Icmp_VLocalIpAddrAssignmentChange()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Icmp_VLocalIpAddrAssignmentChange(
  IpV4_AddrIdType       IpAddrId,
  TcpIp_IpAddrStateType State)
{
  /* ----- Implementation ----------------------------------------------- */
  if (TCPIP_IPADDR_STATE_ASSIGNED != State)
  {
    if (IpV4_Icmp_EchoReplyPendingMsg.IpAddrId == IpAddrId)
    {
      /* cancel ICMP echo reply */
      IpV4_Icmp_EchoReplyIsPending = FALSE;
    }
  }
} /* IpV4_Icmp_VLocalIpAddrAssignmentChange() */

#define IPV4_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#endif  /* (IPV4_SUPPORT_ICMP == STD_ON) */
#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Icmp.c
 *********************************************************************************************************************/
